#include "appinc.h"
static lv_timer_t *result_timer;
static void touch_key_event_cb(lv_event_t * e)
{
    int keyCode;

    lv_event_code_t code = lv_event_get_code(e);
    char* index = (char*)lv_event_get_user_data(e);
    if( code == LV_EVENT_KEY){
        keyCode = lv_event_get_key(e);
        switch(keyCode){
            case KB_KEY_CANCEL://cancel
                OsCloseCamera();
                if (result_timer != NULL )
                {
                    lv_timer_del(result_timer);
                    result_timer = NULL;
                }
                lv_obj_set_style_bg_opa(src_Panel, LV_OPA_COVER, LV_PART_MAIN);//Set screen transparency
                lv_obj_set_style_bg_opa(Main_Panel, LV_OPA_COVER, LV_PART_MAIN);//Set screen transparency
                event_ui_register(UI_IDLE);
                break;
        }  
    }else if (code == LV_EVENT_CLICKED){   
        keyCode = atoi(index);
        switch(keyCode){
            case 9: //cancel
                OsCloseCamera();
                if (result_timer != NULL )
                {
                    lv_timer_del(result_timer);
                    result_timer = NULL;
                }
                lv_obj_set_style_bg_opa(src_Panel, LV_OPA_COVER, LV_PART_MAIN);//Set screen transparency
                lv_obj_set_style_bg_opa(Main_Panel, LV_OPA_COVER, LV_PART_MAIN);//Set screen transparency
                event_ui_register(UI_IDLE);
                break;
        }
    }
}
static int timer_count = 0;
static char qrCode[256] = {0};
static void scan_qrcode_process_cb(lv_timer_t *timer){
    memset(qrCode,0x0,sizeof(qrCode));
    timer_count++;
    if(OsReadScanResult(qrCode,sizeof(qrCode)-1) == 0){
        PlayKeyTone();
        OsLog(LOG_DEBUG,"qrCode = %s",qrCode);
        if (result_timer != NULL )
        {
            lv_timer_del(result_timer);
            result_timer = NULL;
        }
        OsCloseCamera();
        lv_obj_set_style_bg_opa(src_Panel, LV_OPA_COVER, LV_PART_MAIN);//Set screen transparency
        lv_obj_set_style_bg_opa(Main_Panel, LV_OPA_COVER, LV_PART_MAIN);//Set screen transparency
        event_ui_register(UI_PROCESSING);
    }else{
        if (timer_count >= 600) {
            if (result_timer != NULL )
            {
                lv_timer_del(result_timer);
                result_timer = NULL;
            }
            OsCloseCamera();
            lv_obj_set_style_bg_opa(src_Panel, LV_OPA_COVER, LV_PART_MAIN);//Set screen transparency
            lv_obj_set_style_bg_opa(Main_Panel, LV_OPA_COVER, LV_PART_MAIN);//Set screen transparency
            set_fail_msg("Scan Code Timeout");
            event_ui_register(UI_RESULT_FAIL);
        }
    }
    
}

// Set universal style
void set_border_style(lv_obj_t* obj) {
    lv_obj_set_style_radius(obj, 0, 0); 
    lv_obj_set_style_bg_color(obj, lv_color_white(), 0);
    lv_obj_set_style_bg_opa(obj, LV_OPA_COVER, 0);
    lv_obj_set_style_border_width(obj, 0, 0);
    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
}

// thread function
// void* thread_function(void* arg) {
//     OsOpenCamera();
//     return NULL;
// }

void ui_create_scan_code() {
    char szDisplayAmount[16] = {0};
    int nAmount = atoi(get_transaction_data()->sAmount);
    sprintf(szDisplayAmount,"total $%ld.%02ld",nAmount/100,nAmount%100);
    timer_count = 0;

    // pthread_t thread_id;
    // pthread_create(&thread_id, NULL, thread_function, NULL);

    lv_timer_enable(false);
    lv_obj_clean(Main_Panel);
    lv_group_remove_all_objs(group_keypad_indev);
    lv_obj_clear_flag(Main_Panel, LV_OBJ_FLAG_SCROLLABLE); // Disable scrolling
    
    // Get screen size
    lv_coord_t screen_w = 480;
    lv_coord_t screen_h = 854;
    lv_coord_t center_x = (screen_w - 400) / 2;
    lv_coord_t center_y = (screen_h - 400) / 2;

    // Create 4 white rectangles as borders
    lv_obj_t* top = lv_obj_create(src_Panel);
    lv_obj_t* bottom = lv_obj_create(src_Panel);
    lv_obj_t* left = lv_obj_create(src_Panel);
    lv_obj_t* right = lv_obj_create(src_Panel);

    ui_lvgl_page_construct("Sale",touch_key_event_cb);
    resumeStatusBarIcon();
    lv_obj_move_foreground(Main_Panel);  // Raise Main-Panel to the top layer
    lv_obj_move_foreground(statusBar_Panel);  // Raise Main-Panel to the top layer

    //3prompt
    lv_obj_t * amount_lable = lv_label_create(top);
    lv_label_set_text(amount_lable, szDisplayAmount);
    lv_obj_align(amount_lable, LV_ALIGN_TOP_MID, 0, 100);
    lv_obj_set_style_text_color(amount_lable, lv_color_hex(0x191E3B), 0);
    lv_obj_set_style_text_font(amount_lable, &ali_middle_36, 0);

    //3prompt
    lv_obj_t * tip_lable = lv_label_create(bottom);
    lv_label_set_text(tip_lable, "Please QR code inside of\n         square to scan");
    lv_obj_align(tip_lable, LV_ALIGN_TOP_MID, 0, 20);
    lv_obj_set_style_text_color(tip_lable, lv_color_hex(0x191E3B), 0);
    lv_obj_set_style_text_font(tip_lable, &ali_middle_24, 0);

    set_border_style(top);
    set_border_style(bottom);
    set_border_style(left);
    set_border_style(right);

    // Positioning border
    lv_obj_set_size(top, screen_w, center_y);
    lv_obj_set_pos(top, 0, 0);

    lv_obj_set_size(bottom, screen_w, screen_h - center_y - 400);
    lv_obj_set_pos(bottom, 0, center_y + 400);

    lv_obj_set_size(left, center_x, 400);
    lv_obj_set_pos(left, 0, center_y);

    lv_obj_set_size(right, screen_w - center_x - 400, 400);
    lv_obj_set_pos(right, center_x + 400, center_y);
    // OsSleep(300);
    lv_obj_set_style_bg_opa(src_Panel, LV_OPA_TRANSP, LV_PART_MAIN);//Set screen transparency
    lv_obj_set_style_bg_opa(Main_Panel, LV_OPA_TRANSP, LV_PART_MAIN);//Set screen transparency
    
    result_timer = lv_timer_create(scan_qrcode_process_cb, 100,NULL);
	lv_timer_set_repeat_count(result_timer, 600);
    lv_timer_enable(true);
}

