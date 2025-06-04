#include "appinc.h"

static lv_obj_t *menu_pad = NULL;

static void touch_key_event_cb(lv_event_t * e)
{
    int keyCode;
    lv_event_code_t code = lv_event_get_code(e);
    char* index = (char*)lv_event_get_user_data(e);
    // OsLog(LOG_DEBUG," code = %d",code);
    if( code == LV_EVENT_KEY){
        keyCode = lv_event_get_key(e);
        OsLog(LOG_DEBUG," keyCode = %04x",keyCode);
        switch(keyCode){
            case KB_KEY_CANCEL://cancel
                event_ui_register(UI_IDLE);
                break;
            case KB_KEY_UP:
                // lv_obj_scroll_to_y(content_pad,0,LV_ANIM_ON);
                lv_obj_scroll_by_bounded(menu_pad,0,50,LV_ANIM_ON);
                break;
            case KB_KEY_DOWN:
                // lv_obj_scroll_to_y(content_pad,50,LV_ANIM_ON);
                lv_obj_scroll_by_bounded(menu_pad,0,-50,LV_ANIM_ON);
                break;
        }  
    }else if (code == LV_EVENT_CLICKED){   
        keyCode = atoi(index);
        switch(keyCode){
            case 9:
                event_ui_register(UI_IDLE);
                break;
        }
    }
}

void ui_create_about() {

    ui_lvgl_page_construct("About",touch_key_event_cb);
    
    // 1 .add pad
    menu_pad = lv_obj_create(Main_Panel);
    lv_obj_set_size(menu_pad,480,830);
    lv_obj_set_pos(menu_pad,0,70);
    lv_obj_set_style_radius(menu_pad, 0, 0);
    lv_obj_set_style_pad_all(menu_pad,0,0);
    lv_obj_set_style_border_width(menu_pad,0,0);
    #if 1
    lv_obj_t *type_label = lv_label_create(menu_pad);
    lv_label_set_text(type_label, "D30");
    lv_obj_set_size(type_label,284,30);
    lv_obj_set_pos(type_label,18,20);
    lv_obj_set_style_pad_all(type_label,0,0);
    lv_obj_set_style_text_font(type_label, &ali_middle_42, 0);
    lv_obj_set_style_text_align(type_label, LV_TEXT_ALIGN_LEFT, 0);
    
    lv_obj_t *param_label = lv_label_create(menu_pad);
    lv_label_set_text(param_label, "Product Parameters");
    lv_obj_set_size(param_label,284,30);
    lv_obj_set_pos(param_label,18,60);
    lv_obj_set_style_pad_all(param_label,0,0);
    lv_obj_set_style_text_font(param_label, &ali_bold_20, 0);
    lv_obj_set_style_text_align(param_label, LV_TEXT_ALIGN_LEFT, 0);
    
    lv_obj_t *ui_item_separator = lv_obj_create(menu_pad);
    lv_obj_set_size(ui_item_separator, 284, 2);
    lv_obj_set_pos(ui_item_separator, 18, 90);
    lv_obj_set_style_bg_color(ui_item_separator, lv_color_hex(0xDDDDDD), 0);
    lv_obj_set_style_border_width(ui_item_separator, 0, 0);
    lv_obj_set_style_pad_all(ui_item_separator, 0, 0); 

    // Get Span container
    lv_obj_t *content_label = lv_spangroup_create(menu_pad);
    lv_obj_set_width(content_label,460);
    lv_obj_set_pos(content_label,18,130);
    lv_spangroup_set_mode(content_label, LV_SPAN_MODE_BREAK);
    lv_span_t * span = lv_spangroup_new_span(content_label);
    if (span) {
        lv_span_set_text(span, "Processor");
        lv_style_set_text_font(&span->style, &ali_middle_24);               // Set Span Text Style
        lv_style_set_text_color(&span->style, lv_color_hex(0x1B1B1B));     // Set Span text color   
    }
    lv_span_t * span2 = lv_spangroup_new_span(content_label);
    if (span2) {
        lv_span_set_text(span2, "\nLinux:ARM Cortex-A35");
        lv_style_set_text_font(&span2->style, &ali_middle_24);               // Set Span Text Style
        lv_style_set_text_color(&span2->style, lv_color_hex(0x008CFF));     // Set Span text color
    }

    
    lv_span_t * span11 = lv_spangroup_new_span(content_label);
    if (span11) {
        lv_span_set_text(span11, "\n\nOS");
        lv_style_set_text_font(&span11->style, &ali_middle_24);               // Set Span Text Style
        lv_style_set_text_color(&span11->style, lv_color_hex(0x1B1B1B));     // Set Span text color   
    }
    lv_span_t * span12 = lv_spangroup_new_span(content_label);
    if (span12) {
        lv_span_set_text(span12, "\nLinux");
        lv_style_set_text_font(&span12->style, &ali_middle_24);               // Set Span Text Style
        lv_style_set_text_color(&span12->style, lv_color_hex(0x008CFF));     // Set Span text color
    }

    lv_span_t * span21 = lv_spangroup_new_span(content_label);
    if (span21) {
        lv_span_set_text(span21, "\n\nMemory");
        lv_style_set_text_font(&span21->style, &ali_middle_24);               // Set Span Text Style
        lv_style_set_text_color(&span21->style, lv_color_hex(0x1B1B1B));     // Set Span text color   
    }
    lv_span_t * span22 = lv_spangroup_new_span(content_label);
    if (span22) {
        lv_span_set_text(span22, "\nLinux:128MB RAM+128MB ROM");
        lv_style_set_text_font(&span22->style, &ali_middle_24);               // Set Span Text Style
        lv_style_set_text_color(&span22->style, lv_color_hex(0x008CFF));     // Set Span text color
    }
    
    lv_span_t * span31 = lv_spangroup_new_span(content_label);
    if (span31) {
        lv_span_set_text(span31, "\n\nApp Version");
        lv_style_set_text_font(&span31->style, &ali_middle_24);               // Set Span Text Style
        lv_style_set_text_color(&span31->style, lv_color_hex(0x1B1B1B));     // Set Span text color   
    }
    lv_span_t * span32 = lv_spangroup_new_span(content_label);
    if (span32) {
        lv_span_set_text(span32, "\n"APP_VERSION);
        lv_style_set_text_font(&span32->style, &ali_middle_24);               // Set Span Text Style
        lv_style_set_text_color(&span32->style, lv_color_hex(0x008CFF));     // Set Span text color
    }
    
    
    #else
    // lv_obj_t *about_img = lv_img_create(menu_pad);
    // ui_lv_img_set_src(about_img, "about.png");
    // lv_obj_align(about_img, LV_ALIGN_TOP_MID, 0, 0);  
    #endif  
    lv_timer_enable(true);

    
}