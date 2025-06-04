#include "appinc.h"

static void touch_key_event_cb(lv_event_t * e)
{
    int keyCode;

    lv_event_code_t code = lv_event_get_code(e);
    char* index = (char*)lv_event_get_user_data(e);
    if( code == LV_EVENT_KEY){
        keyCode = lv_event_get_key(e);
        switch(keyCode){
            case KB_KEY_ENTER:
                event_ui_register(UI_PROCESSING);
                break;
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
            case 1:
                event_ui_register(UI_PROCESSING);
                break;
        }
    }
}

void ui_create_qrcode() {
    char szDisplayAmount[16] = {0};
    char szDisplayQrUrl[256] = {0};
    char *paymentAppList[4] = {"Google.png","Apple.png","MasterCard.png","Visa.png"};
    int nAmount = atoi(get_transaction_data()->sAmount);
    sprintf(szDisplayAmount,"total $%ld.%02ld",nAmount/100,nAmount%100);

    lv_timer_enable(false);
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


    #if 1
    sprintf(szDisplayQrUrl,"this is a demo from Cloud Code, and the terminal will display \"Transaction Success\" after press enter.\n amount: $%ld.%02ld",nAmount/100,nAmount%100);
    lv_obj_t * ui_qrcode_pad = lv_qrcode_create(Main_Panel, 320, lv_color_hex(0x000000), lv_color_hex(0xFFFFFF));
    lv_obj_set_style_border_width(ui_qrcode_pad, 0, 0);
    lv_qrcode_update(ui_qrcode_pad, szDisplayQrUrl, strlen(szDisplayQrUrl));
    lv_obj_align(ui_qrcode_pad, LV_ALIGN_TOP_MID, 0, 200);
    #else
    lv_obj_t * rfid_btn = lv_img_create(Main_Panel);
    lv_obj_align(rfid_btn, LV_ALIGN_TOP_MID, 0, 43);
    ui_lv_img_set_src(rfid_btn, (char*)"yunma.png");
    #endif


    lv_obj_t * tip_lable = lv_label_create(Main_Panel);
    lv_label_set_text(tip_lable, "Open payment App");
    lv_obj_align(tip_lable, LV_ALIGN_TOP_MID, 0, 580);
    lv_obj_set_style_text_font(tip_lable, &ali_middle_24, 0);

    lv_obj_t *payment_pad = lv_obj_create(Main_Panel);
    lv_obj_set_size(payment_pad, 320, 24);
    lv_obj_align(payment_pad, LV_ALIGN_TOP_MID, 0, 610);
    lv_obj_set_style_border_width(payment_pad, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_all(payment_pad, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_flex_flow(payment_pad, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(payment_pad,LV_FLEX_ALIGN_SPACE_EVENLY, 0, 0);

    for (size_t i = 0; i < sizeof(paymentAppList)/sizeof(char*); i++)
    {
        lv_obj_t * payment_img = lv_img_create(payment_pad);
        ui_lv_img_set_src(payment_img, paymentAppList[i]);
    }

    lv_obj_t *btn = lv_btn_create(Main_Panel);
    lv_obj_set_size(btn, 400, 80);
    lv_obj_align(btn, LV_ALIGN_TOP_MID, 0, 650);
    lv_obj_add_event_cb(btn, touch_key_event_cb, LV_EVENT_CLICKED, "1");
    lv_obj_set_style_bg_color(btn, lv_color_hex(0xFFFFFF), 0); 

    // Set red border
    lv_obj_set_style_border_color(btn, lv_color_hex(0xFF0039), 0);  // Red border
    lv_obj_set_style_border_width(btn, 2, 0);  // Border width 1 pixel
    lv_obj_set_style_radius(btn, 0, 0);  // Remove rounded corners

    // Create tags and set red font
    lv_obj_t *label = lv_label_create(btn);
    lv_label_set_text(label, "Check Payment Status");
    lv_obj_set_style_text_color(label, lv_color_hex(0xFF0039), 0);  // Red font
    lv_obj_set_style_text_font(label, &ali_middle_24, 0);
    lv_obj_center(label);
    lv_timer_enable(true);
}

