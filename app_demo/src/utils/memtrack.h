// memtrack.h
#ifndef MEMTRACK_H
#define MEMTRACK_H

#include <stdlib.h>

// Initialize memory tracing
void memtrack_init(void);

// Clean up memory tracing and report leaks
void memtrack_cleanup(void);

// Packaged memory allocation function
void* memtrack_malloc(size_t size, const char* file, int line, const char* func);
void* memtrack_calloc(size_t nmemb, size_t size, const char* file, int line, const char* func);
void* memtrack_realloc(void* ptr, size_t size, const char* file, int line, const char* func);

// Packaged memory release function
void memtrack_free(void* ptr, const char* file, int line, const char* func);

// Macro definition, simplified use
#define trackmalloc(size) memtrack_malloc(size, __FILE__, __LINE__, __func__)
#define trackcalloc(nmemb, size) memtrack_calloc(nmemb, size, __FILE__, __LINE__, __func__)
#define trackrealloc(ptr, size) memtrack_realloc(ptr, size, __FILE__, __LINE__, __func__)
#define trackfree(ptr) memtrack_free(ptr, __FILE__, __LINE__, __func__)

#endif // MEMTRACK_H