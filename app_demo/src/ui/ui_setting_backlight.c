#include "appinc.h"

#define ITEM_PAD_HIGHT  45

static lv_obj_t *backlight_bar[10] = {0};
static lv_obj_t *up_img = NULL;
static lv_obj_t *down_img = NULL;
static int value_backlight = 2;    //default 20%

static void backlight_up(void)
{
    ui_lv_img_set_src(up_img, (char*)"+blue.png");
    ui_lv_img_set_src(down_img, (char*)"-grey.png");
    value_backlight++; 
    if (value_backlight>10)
    {
        value_backlight = 10;
    }
    OsScrBrightness(value_backlight);
    if (value_backlight>sizeof(backlight_bar)/sizeof(lv_obj_t*))
    {
        value_backlight = sizeof(backlight_bar)/sizeof(lv_obj_t*);
    }
    for (size_t i = 0; i < sizeof(backlight_bar)/sizeof(lv_obj_t*); i++)
    {
        if (i<value_backlight)
        {
            lv_obj_set_style_bg_color(backlight_bar[i],lv_color_hex(0x008CFF),0);
        }
        else
        {
            lv_obj_set_style_bg_color(backlight_bar[i],lv_color_hex(0xC4C1C1),0);
        }
    }
}

static void backlight_down(void)
{
    ui_lv_img_set_src(down_img, (char*)"-blue.png");
    ui_lv_img_set_src(up_img, (char*)"+grey.png");
    value_backlight--;
    if (value_backlight<1)
    {
        value_backlight = 1;
    }
    OsScrBrightness(value_backlight);
    for (size_t i = 0; i < sizeof(backlight_bar)/sizeof(lv_obj_t*); i++)
    {
        if (i<value_backlight)
        {
            lv_obj_set_style_bg_color(backlight_bar[i],lv_color_hex(0x008CFF),0);
        }
        else
        {
            lv_obj_set_style_bg_color(backlight_bar[i],lv_color_hex(0xC4C1C1),0);
        }
    }
}

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
                event_ui_register(UI_SETTING);
                break;
            case KB_KEY_DOWN:
                backlight_down();
                break;
            case KB_KEY_UP:
                backlight_up();
                break;
        }  
    }else if (code == LV_EVENT_CLICKED){   
        keyCode = atoi(index);
        switch(keyCode){
            case 9:
                event_ui_register(UI_SETTING);
                break;
            case 407:
                backlight_down();
                break;
            case 412:
                backlight_up();
                break;
        }
    }
}

void ui_create_setting_backlight() {

    ui_lvgl_page_construct("Backlight",touch_key_event_cb);
    memset(backlight_bar,0x00,sizeof(backlight_bar));
    
    // 1 .add pad
    lv_obj_t *menu_pad = lv_obj_create(Main_Panel);
    lv_obj_set_size(menu_pad,480,240);
    lv_obj_set_pos(menu_pad,0,80);
    lv_obj_set_style_radius(menu_pad, 0, 0);
    lv_obj_set_style_pad_all(menu_pad,0,0);
    lv_obj_set_style_border_width(menu_pad,0,0);
    lv_obj_clear_flag(menu_pad, LV_OBJ_FLAG_SCROLLABLE); // Disable scrolling
    //todo get backlight value
    for (size_t i = 0; i < sizeof(backlight_bar)/sizeof(lv_obj_t*); i++)
    {
        lv_obj_t *item_pad = lv_obj_create(menu_pad);
        lv_obj_set_size(item_pad,72,12);
        lv_obj_align(item_pad,LV_ALIGN_BOTTOM_MID,0, -38-i*16);
        lv_obj_set_style_radius(item_pad, 0, 0);
        lv_obj_set_style_pad_all(item_pad,0,0);
        lv_obj_set_style_border_width(item_pad,0,0);
        backlight_bar[i] = item_pad;
        if (i<value_backlight)
        {
            lv_obj_set_style_bg_color(item_pad,lv_color_hex(0x008CFF),0);
        }
        else
        {
            lv_obj_set_style_bg_color(item_pad,lv_color_hex(0xC4C1C1),0);
        }
    }
    
    up_img = lv_img_create(menu_pad);
    ui_lv_img_set_src(up_img, (char*)"+grey.png");
    lv_obj_add_flag(up_img,LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(up_img,touch_key_event_cb,LV_EVENT_ALL,"412");
    lv_obj_align(up_img,LV_ALIGN_TOP_RIGHT,-20,58);
    
    down_img = lv_img_create(menu_pad);
    ui_lv_img_set_src(down_img, (char*)"-grey.png");
    lv_obj_add_flag(down_img,LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(down_img,touch_key_event_cb,LV_EVENT_ALL,"407");
    lv_obj_align(down_img,LV_ALIGN_TOP_LEFT,20,58);
    lv_timer_enable(true);
    
}