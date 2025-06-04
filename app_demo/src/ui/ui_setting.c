#include "appinc.h"

#define ITEM_PAD_HIGHT  80

typedef struct
{
    char* str_index;
    char* str_content;
} st_menu_list;

static st_menu_list menu_list [] = {{"1","Network"},{"2","Back Light"},{"3","System Time"},{"4","OTA"}};
static lv_obj_t* tone_switch = NULL;

static void touch_key_event_cb(lv_event_t * e)
{
    int keyCode;

    lv_event_code_t code = lv_event_get_code(e);
    char* index = (char*)lv_event_get_user_data(e);
    if( code == LV_EVENT_KEY){
        keyCode = lv_event_get_key(e);
        //OsLog(LOG_DEBUG," keyCode = %04x",keyCode);
        switch(keyCode){
            case KB_KEY_4:
                break;
            case KB_KEY_CANCEL://cancel
                event_ui_register(UI_IDLE);
                break;
        }  
    }else if (code == LV_EVENT_CLICKED){   
        keyCode = atoi(index);
        switch(keyCode){
            case 9:
                event_ui_register(UI_IDLE);
                break;
            case 1:
                event_ui_register(UI_SETTING_NETWORK);
                break;
            case 2:
                event_ui_register(UI_SETTING_BACKLIGHT);
                break;
            case 3:
                event_ui_register(UI_SETTING_SYSTIME);
                break;
            case 4:
                event_ui_register(UI_OTA_CHECK);
                break;    
        }
    }
}

void ui_create_setting() {

    ui_lvgl_page_construct("Setting",touch_key_event_cb);
    
    // 1 .add pad
    lv_obj_t *menu_pad = lv_obj_create(Main_Panel);
    lv_obj_set_size(menu_pad,480,780);
    lv_obj_set_pos(menu_pad,0,70);
    lv_obj_set_style_radius(menu_pad, 0, 0);
    lv_obj_set_style_pad_all(menu_pad,0,0);
    lv_obj_set_style_border_width(menu_pad,0,0);
   // lv_obj_clear_flag(menu_pad, LV_OBJ_FLAG_SCROLLABLE); // Disable scrolling
    for (size_t i = 0; i < sizeof(menu_list)/sizeof(st_menu_list); i++)
    {
        lv_obj_t *item_pad = lv_obj_create(menu_pad);
        lv_obj_set_size(item_pad,480,ITEM_PAD_HIGHT);
        lv_obj_set_pos(item_pad,0,i*ITEM_PAD_HIGHT);
        lv_obj_set_style_radius(item_pad, 0, 0);
        lv_obj_set_style_pad_all(item_pad,0,0);
        lv_obj_set_style_border_width(item_pad,0,0);
        lv_obj_add_flag(item_pad,LV_OBJ_FLAG_CLICKABLE);
        lv_obj_add_event_cb(item_pad,touch_key_event_cb,LV_EVENT_ALL,menu_list[i].str_index);

        lv_obj_t* index_label = lv_label_create(item_pad);
        lv_label_set_text(index_label, menu_list[i].str_index);
        lv_obj_set_size(index_label,30,30);
        lv_obj_set_pos(index_label,14,40);
        lv_obj_set_style_radius(index_label,30,0);
        // lv_obj_set_style_border_color(index_label,);
        lv_obj_set_style_border_width(index_label,1,0);
        lv_obj_set_style_pad_all(index_label,0,0);
        lv_obj_set_style_text_font(index_label, &ali_middle_24, 0);
        lv_obj_set_style_text_align(index_label, LV_TEXT_ALIGN_CENTER, 0);

        lv_obj_t* content_label = lv_label_create(item_pad);
        lv_label_set_text(content_label,  menu_list[i].str_content);
        lv_obj_set_height(content_label,30);
        lv_obj_set_pos(content_label,60,40);
        lv_obj_set_style_text_font(content_label, &ali_middle_24, 0);
        lv_obj_set_style_text_align(content_label, LV_TEXT_ALIGN_LEFT, 0);
        

        lv_obj_t *ui_item_separator = lv_obj_create(item_pad);
        lv_obj_set_size(ui_item_separator, 480, 2);
        lv_obj_align(ui_item_separator, LV_ALIGN_BOTTOM_MID, 0, -2);
        lv_obj_set_style_bg_color(ui_item_separator, lv_color_hex(0xDDDDDD), 0);
        lv_obj_set_style_border_width(ui_item_separator, 0, 0);
        lv_obj_set_style_pad_all(ui_item_separator, 0, 0);         
    }
    
    lv_timer_enable(true);
    
}