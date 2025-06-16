#include "appinc.h"
#include "memtrack.h"
#include <stdio.h>
#include <string.h>
#include <pthread.h>

// Memory allocation record structure
typedef struct MemRecord {
    void* address;          // Allocated memory address
    size_t size;            // Allocated memory size
    char file[256];         // Assign the file name that occurred
    int line;               // Assign the line number that occurred
    char function[128];     // Assign the function name that occurred
    struct MemRecord* next; // Next node in the linked list
} MemRecord;

// global variable
static MemRecord* record_list = NULL;
static pthread_mutex_t record_mutex = PTHREAD_MUTEX_INITIALIZER;
static int initialized = 0;

// Add memory allocation record
static void add_record(void* address, size_t size, const char* file, int line, const char* func) {
    if (address == NULL) return;
    
    pthread_mutex_lock(&record_mutex);
    
    MemRecord* new_record = (MemRecord*)malloc(sizeof(MemRecord));
    if (new_record == NULL) {
        pthread_mutex_unlock(&record_mutex);
        return;
    }
    
    new_record->address = address;
    new_record->size = size;
    strncpy(new_record->file, file, sizeof(new_record->file) - 1);
    new_record->line = line;
    strncpy(new_record->function, func, sizeof(new_record->function) - 1);
    new_record->next = record_list;
    
    record_list = new_record;
    
    OsLog(LOG_DEBUG,"ALLOC: %p, size=%zu, at %s:%d in %s\n", 
           address, size, file, line, func);
    
    pthread_mutex_unlock(&record_mutex);
}

// Remove memory allocation records
static int remove_record(void* address, const char* file, int line, const char* func) {
    if (address == NULL) return 1;
    
    pthread_mutex_lock(&record_mutex);
    
    MemRecord* current = record_list;
    MemRecord* previous = NULL;
    
    while (current != NULL) {
        if (current->address == address) {
            OsLog(LOG_DEBUG,"FREE: %p, allocated at %s:%d in %s, freed at %s:%d in %s\n", 
                   address, current->file, current->line, current->function,
                   file, line, func);
            
            if (previous == NULL) {
                record_list = current->next;
            } else {
                previous->next = current->next;
            }
            
            free(current);
            pthread_mutex_unlock(&record_mutex);
            return 1;
        }
        
        previous = current;
        current = current->next;
    }
    
    OsLog(LOG_DEBUG,"WARNING: double free or invalid free of %p at %s:%d in %s\n", 
           address, file, line, func);
    
    pthread_mutex_unlock(&record_mutex);
    return 0;
}

// Report all unreleased memory
static void report_leaks(void) {
    pthread_mutex_lock(&record_mutex);
    
    MemRecord* current = record_list;
    size_t total_leaked = 0;
    int leak_count = 0;
    
    OsLog(LOG_DEBUG,"\n===== Memory Leak Report =====\n");
    
    while (current != NULL) {
        OsLog(LOG_DEBUG,"LEAK: %p, size=%zu, allocated at %s:%d in %s\n", 
               current->address, current->size, 
               current->file, current->line, current->function);
        total_leaked += current->size;
        leak_count++;
        
        current = current->next;
    }
    
    OsLog(LOG_DEBUG,"Total leaks: %d blocks, %zu bytes\n", leak_count, total_leaked);
    OsLog(LOG_DEBUG,"==============================\n");
    
    pthread_mutex_unlock(&record_mutex);
}

// Initialize memory tracing
void memtrack_init(void) {
    if (!initialized) {
        record_list = NULL;
        pthread_mutex_init(&record_mutex, NULL);
        initialized = 1;
        OsLog(LOG_DEBUG,"Memory tracking initialized\n");
    }
}

// Clean up memory tracing and report leaks
void memtrack_cleanup(void) {
    if (initialized) {
        report_leaks();
        
        // Clean up linked list
        pthread_mutex_lock(&record_mutex);
        MemRecord* current = record_list;
        while (current != NULL) {
            MemRecord* temp = current;
            current = current->next;
            free(temp);
        }
        record_list = NULL;
        pthread_mutex_unlock(&record_mutex);
        
        pthread_mutex_destroy(&record_mutex);
        initialized = 0;
        OsLog(LOG_DEBUG,"Memory tracking cleaned up\n");
    }
}

// Packaged memory allocation function
void* memtrack_malloc(size_t size, const char* file, int line, const char* func) {
    void* ptr = malloc(size);
    add_record(ptr, size, file, line, func);
    return ptr;
}

void* memtrack_calloc(size_t nmemb, size_t size, const char* file, int line, const char* func) {
    void* ptr = calloc(nmemb, size);
    add_record(ptr, nmemb * size, file, line, func);
    return ptr;
}

void* memtrack_realloc(void* ptr, size_t size, const char* file, int line, const char* func) {
    if (ptr == NULL) {
        return memtrack_malloc(size, file, line, func);
    }
    
    // Try to find the original allocation record first
    pthread_mutex_lock(&record_mutex);
    MemRecord* current = record_list;
    size_t old_size = 0;
    
    while (current != NULL) {
        if (current->address == ptr) {
            old_size = current->size;
            break;
        }
        current = current->next;
    }
    pthread_mutex_unlock(&record_mutex);
    
    void* new_ptr = realloc(ptr, size);
    
    if (new_ptr != ptr) {
        // Memory location has been changed, old records need to be removed and new records need to be added
        remove_record(ptr, file, line, func);
        add_record(new_ptr, size, file, line, func);
    } else if (current != NULL) {
        // The memory location has not been changed, update size
        pthread_mutex_lock(&record_mutex);
        current->size = size;
        pthread_mutex_unlock(&record_mutex);
        OsLog(LOG_DEBUG,"REALLOC: %p, old_size=%zu, new_size=%zu, at %s:%d in %s\n", 
               new_ptr, old_size, size, file, line, func);
    }
    
    return new_ptr;
}

// Packaged memory release function
void memtrack_free(void* ptr, const char* file, int line, const char* func) {
    if (remove_record(ptr, file, line, func)) {
        free(ptr);
    }
}