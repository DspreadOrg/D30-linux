#ifndef UI_UTILS_H
#define UI_UTILS_H

#include "appinc.h"

#ifdef __cplusplus
extern "C" {
#endif

extern lv_style_t menubtn_style_default;

void remove_last_byte(char *str);

void get_feature_code(const char* date, const char* factory, char* out_code);

void ui_lvgl_style_init();

void ui_lv_img_set_src(lv_obj_t * obj, char* fileName);

void ui_lvgl_page_construct(char *str_title, lv_event_cb_t event_cb);

void ui_lvgl_page_construct_v2(lv_obj_t * obj,char *str_title, lv_event_cb_t event_cb);

void ui_lvgl_create_numpinpad(lv_obj_t *parent,lv_event_cb_t num_key_event_cb,lv_event_cb_t touch_key_event_cb);

lv_obj_t * ui_lvgl_menubtn_creat(lv_obj_t *parent,int w, int h,char* text,char* filename);

lv_style_t* ui_lvgl_style_gradient(lv_color_t start_color, lv_color_t grad_color);

void set_common_msg(int icon_type,const char * format, ...);

#ifdef __cplusplus
}
#endif

#endif