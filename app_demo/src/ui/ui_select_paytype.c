#include "appinc.h"


static void touch_key_event_cb(lv_event_t * e)
{
    int keyCode;
    
    lv_event_code_t code = lv_event_get_code(e);
    char* index = (char*)lv_event_get_user_data(e);
    if( code == LV_EVENT_KEY){
        keyCode = lv_event_get_key(e);
        OsLog(LOG_DEBUG," keyCode = %04x",keyCode);
        switch(keyCode){
            case KB_KEY_CANCEL://cancel
                event_ui_register(UI_IDLE);
                break;
        }  
    }else if (code == LV_EVENT_CLICKED){   
        keyCode = atoi(index);
        switch(keyCode){
            case 9: //cancel
                event_ui_register(UI_IDLE);
                break;
            case 1: //card
                event_ui_register(UI_READCARD);
                break;
            case 2: //Scan
                get_transaction_data()->nPosEntryMode = INPUT_SCANCODE;
                event_ui_register(UI_SCAN_CODE);
                break;
            case 3: //qr cord
                get_transaction_data()->nPosEntryMode = INPUT_QRCODE;
                event_ui_register(UI_QRCODE);
                break;
        }
    }
}

void ui_create_select_pay_type() {
    char szDisplayAmount[16] = {0};
    int nAmount = atoi(get_transaction_data()->sAmount);

    sprintf(szDisplayAmount,"total $%ld.%02ld",nAmount/100,nAmount%100);
    lv_timer_enable(false);
    // suspendStatusBarIcon();
    lv_obj_clean(Main_Panel);
    lv_group_remove_all_objs(group_keypad_indev);
    lv_obj_clear_flag(Main_Panel, LV_OBJ_FLAG_SCROLLABLE); // Disable scrolling

    ui_lvgl_page_construct("Sale",touch_key_event_cb);

    //1prompt
    lv_obj_t * amount_lable = lv_label_create(Main_Panel);
    lv_label_set_text(amount_lable, szDisplayAmount);
    lv_obj_align(amount_lable, LV_ALIGN_TOP_MID, 0, 100);
    lv_obj_set_style_text_color(amount_lable, lv_color_hex(0x191E3B), 0);
    lv_obj_set_style_text_font(amount_lable, &ali_middle_36, 0);

    lv_obj_t * tip_lable = lv_label_create(Main_Panel);
    lv_label_set_text(tip_lable, "Select payment method");
    lv_obj_align(tip_lable, LV_ALIGN_TOP_LEFT, 10, 190);
    lv_obj_set_style_text_color(tip_lable, lv_color_hex(0xFF0039), 0);
    lv_obj_set_style_text_font(tip_lable, &ali_middle_24, 0);

    lv_obj_t *card_btn = ui_lvgl_menubtn_creat(Main_Panel,460,180,"Card","icon_card2.png");
    lv_obj_set_pos(card_btn, 10, 230);
    lv_obj_add_event_cb(card_btn, touch_key_event_cb, LV_EVENT_ALL,"1");
    lv_obj_set_style_radius(card_btn, 0, 0);  // Remove rounded corners


    lv_obj_t *scan_btn = ui_lvgl_menubtn_creat(Main_Panel,225,200,"Scan code","icon_Scan code2.png");
    lv_obj_set_pos(scan_btn, 10, 420);
    lv_obj_add_event_cb(scan_btn, touch_key_event_cb, LV_EVENT_ALL,"2");
    lv_obj_set_style_radius(scan_btn, 0, 0);  // Remove rounded corners


    lv_obj_t *qrcode_btn = ui_lvgl_menubtn_creat(Main_Panel,225,200,"Generate","icon_generate2.png");
    lv_obj_set_pos(qrcode_btn, 245, 420); // Set the position X and Y of the button object
    lv_obj_add_event_cb(qrcode_btn, touch_key_event_cb, LV_EVENT_ALL,"3");
    lv_obj_set_style_radius(qrcode_btn, 0, 0);  // Remove rounded corners

    lv_timer_enable(true);
}