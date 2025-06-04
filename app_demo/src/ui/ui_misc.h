#ifndef UI_MISC_H
#define UI_MISC_H

#include "appinc.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
    MODAL_RESULT_NULL,
    MODAL_RESULT_CANCEL,
    MODAL_RESULT_CONFIRM,
    MODAL_RESULT_TIMEOUT,
    MODAL_RESULT_QUEUE,
    MODAL_RESULT_LEFT,
    MODAL_RESULT_RIGHT,
}em_modal_result;

typedef enum{
    MODAL_TYPE_DEFAULT,
    MODAL_TYPE_SUCCESS,
    MODAL_TYPE_ERROR,
    MODAL_TYPE_WAITING,
    MODAL_TYPE_WARNING,
    MODAL_TYPE_SELECT,
}em_modal_type;

typedef void (*modal_cb)(void*);
typedef struct {
	unsigned int w;
	unsigned int h;
	char *buf;
}sign_buf_t;

extern void ui_modal_close(void);
extern bool ui_madal_is_run(void);
extern em_modal_result ui_modal_result(void);
extern em_modal_result ui_modal_dialog_for_handle(char* title, char* content, int time_out, modal_cb callback);
extern em_modal_result ui_modal_dialog_by_type(char* title, em_modal_type type,char* content, int time_out, modal_cb callback);
extern em_modal_result ui_modal_dialog_for_comm_hidden(char* title, char* content, int time_out, modal_cb callback);
extern em_modal_result ui_modal_dialog_nomal(char* title, char* content, int time_out, char* str_cancel, char* str_confirm, modal_cb callback);
extern em_modal_result ui_modal_dialog_error(char* title, char* content, int time_out, char* str_cancel, char* str_confirm, modal_cb callback);
extern em_modal_result ui_modal_dialog_success(char* title, char* content, int time_out, char* str_cancel, char* str_confirm, modal_cb callback);
extern em_modal_result ui_modal_dialog_waiting(char* title, char* content, int time_out, char* str_cancel, char* str_confirm, modal_cb callback);
extern em_modal_result ui_modal_dialog_warning(char* title, char* content, int time_out, char* str_cancel, char* str_confirm, modal_cb callback);

extern int lv_canvas_save_as_mono_bmp(sign_buf_t *bitmap, const char *filename);
//Bmp Go up and down white borders+compress to specified width
extern int process_bmp(const char* input_file, const char* output_file, int target_width);

#ifdef __cplusplus
}
#endif

#endif