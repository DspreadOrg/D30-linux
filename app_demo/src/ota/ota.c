#include "ota.h"

#define TMS_FW_HEART_CUSTOM_URL               "https://www.dspreadser.net:9011/terminal"  
static lv_obj_t * tip_lable = NULL;
static lv_obj_t *percentage_lable = NULL;
static lv_obj_t * Upgrade_bar = NULL;
// static unsigned char tip_msg[64];
// static unsigned char percentage_msg[8];

static void touch_key_event_update_cb(lv_event_t * e)
{
    int keyCode;

    lv_event_code_t code = lv_event_get_code(e);
    char* index = (char*)lv_event_get_user_data(e);
    if( code == LV_EVENT_KEY){
        keyCode = lv_event_get_key(e);
        switch(keyCode){
            case KB_KEY_ENTER:
                event_ui_register(UI_OTA_CHECK);
                break;
            case KB_KEY_CANCEL://cancel
                event_ui_register(UI_IDLE);
                break;
        }  
    }else if (code == LV_EVENT_CLICKED){   
        keyCode = atoi(index);
        switch(keyCode){
            case 0: //cancel
                event_ui_register(UI_IDLE);
                break;
            case 28:
                event_ui_register(UI_OTA_CHECK);
                break;
        }
    }
}
void ui_create_update() {

    lv_timer_enable(false);
    lv_obj_clean(Main_Panel);
    lv_group_remove_all_objs(group_keypad_indev);
    lv_obj_clear_flag(Main_Panel, LV_OBJ_FLAG_SCROLLABLE); // Disable scrolling
    
    lv_obj_t * result_img = lv_img_create(Main_Panel);
    lv_obj_align(result_img, LV_ALIGN_TOP_MID, 0, 200);
    ui_lv_img_set_src(result_img, (char*)"update.png");

    lv_obj_t * amount_lable = lv_label_create(Main_Panel);
    lv_label_set_text(amount_lable, "There is an update task. Update?");
    lv_obj_align(amount_lable, LV_ALIGN_TOP_MID, 0, 450);
    lv_obj_set_style_text_color(amount_lable, lv_color_hex(0x1B1B1B), 0);
    lv_obj_set_style_text_font(amount_lable, &ali_middle_24, 0);


    lv_obj_t *update_cancle_button = lv_btn_create(Main_Panel);
    lv_obj_set_size(update_cancle_button, 200, 80);
    lv_obj_align(update_cancle_button, LV_ALIGN_BOTTOM_LEFT, 12, -10);
    lv_obj_set_style_pad_all(update_cancle_button,0,0);
    lv_obj_set_style_bg_color(update_cancle_button, lv_color_hex(0xFFA500), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(update_cancle_button,lv_color_hex(0xFFA500), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(update_cancle_button,1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_event_cb(update_cancle_button, touch_key_event_update_cb, LV_EVENT_ALL, "0");

    lv_obj_t * update_cancl_label =lv_label_create(update_cancle_button);
    lv_label_set_text(update_cancl_label, "No");
    lv_obj_align(update_cancl_label, LV_ALIGN_CENTER, 0, -4);
    // lv_obj_set_style_text_color(update_cancl_label, lv_color_hex(0x000000), 0);
    lv_obj_set_style_text_font(update_cancl_label, &ali_middle_24, 0);

    lv_obj_t *update_comfirm_button = lv_btn_create(Main_Panel);
    lv_obj_set_size(update_comfirm_button, 200, 80);
    lv_obj_align(update_comfirm_button, LV_ALIGN_BOTTOM_RIGHT, -12, -10);
    lv_obj_set_style_pad_all(update_comfirm_button,0,0);
    lv_obj_set_style_bg_color(update_comfirm_button, lv_color_hex(0x228B22), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(update_comfirm_button,0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_event_cb(update_comfirm_button, touch_key_event_update_cb, LV_EVENT_ALL, "28");

    lv_obj_t * update_comfirm_label =lv_label_create(update_comfirm_button);
    lv_label_set_text(update_comfirm_label, "YES");
    lv_obj_align(update_comfirm_label, LV_ALIGN_CENTER, 0, -4);
    lv_obj_set_style_text_font(update_comfirm_label, &ali_middle_24, 0);
    
    lv_group_add_obj(group_keypad_indev,update_comfirm_label);
    lv_obj_add_event_cb(update_comfirm_label, touch_key_event_update_cb, LV_EVENT_ALL, "28");
    lv_timer_enable(true);
}
void TmsDispCallback(u32 id, char *pMsg)
{
    u32 i ;
    char dispBuff[8] = {0};
    int curValue = 0;
    OsLog(LOG_DEBUG,"Dspread: %s:id=%d",__FUNCTION__,id);
    switch(id)
    {
        case TMS_DISP_START_CHECK:
            lv_label_set_text(tip_lable, "Ota checking...");
            break;
        case TMS_DISP_DOWNLOADING_PROGRESS:

            curValue = atoi(pMsg);

            sprintf(dispBuff,"%d%%",curValue);
            lv_label_set_text(percentage_lable, dispBuff);
            UpdateOTAProcess(curValue);
            break;
        case TMS_DISP_START_DOWNLOAD:
            lv_label_set_text(tip_lable, "Download...");
            break;
        case TMS_DISP_UPGRADING:
            lv_label_set_text(tip_lable, "Upgrading...");
            break;
        case TMS_DISP_HAVE_UPDATE_TASE:
            if(0 == get_app_status())  // Receive TMS task notifications when the APP is idle
                event_ui_register(UI_OTA_HAVE_UDPATE_TASK);
            break;
        default:
            break;
    }

}

void larktms_init()
{
    static LarkTmsCallBack_t larktmsCbk;
    larktmsCbk.ssl_connect = ssl_server_connect;
    larktmsCbk.ssl_disconnect= ssl_server_disconnect;
    larktmsCbk.ssl_send = ssl_send_msg;
    larktmsCbk.ssl_recv = ssl_recv_msg;

    larktms_service_start(&larktmsCbk,TmsDispCallback,TMS_FW_HEART_CUSTOM_URL,APP_VERSION);
}

int ota_process()
{
    int result = -1,key;
    unsigned char dispMsg[64]= {0};

    result = larktms_client_check(TMS_FW_HEART_CUSTOM_URL,APP_VERSION);

    switch(result)
    {
        case TMS_ERR_OK:
            strcpy(dispMsg,"Update now?");
            break;
        case TMS_ERR_CONNECT:
            strcpy(dispMsg,"network error");
            break;
        case TMS_ERR_NO_AVAIALABLE:
            strcpy(dispMsg,"No updated tasks");
            break;
        case TMS_ERR_SN_NOT_REGISTER:
            strcpy(dispMsg,"Device not registered");
            break;
        default:
            strcpy(dispMsg,"Check failed");
            break;    
    }

    if(result == TMS_ERR_OK)
    {
        result = larktms_client(TMS_FW_HEART_CUSTOM_URL,APP_VERSION);
        if(result  != TMS_ERR_OK)
        {
            set_common_msg(1,"Upgrade Fail");
            event_ui_register(UI_DISP_COMMON_MSG);
        }
    }
    else
    {
        set_common_msg(1,dispMsg);
        event_ui_register(UI_DISP_COMMON_MSG);
    }

    return 0;
}


void UpdateOTAProcess(int process)
{
    lv_bar_set_range(Upgrade_bar,0,100);
    lv_bar_set_value(Upgrade_bar, process, LV_ANIM_OFF);
}
static void event_cb(lv_event_t * e)
{
    lv_obj_draw_part_dsc_t * dsc = lv_event_get_param(e);
    if(dsc->part != LV_PART_INDICATOR) return;

    lv_obj_t * obj= lv_event_get_target(e);

    lv_draw_label_dsc_t label_dsc;
    lv_draw_label_dsc_init(&label_dsc);
    label_dsc.font = LV_FONT_DEFAULT;

    lv_point_t txt_size;
    //lv_txt_get_size(&txt_size, buf, label_dsc.font, label_dsc.letter_space, label_dsc.line_space, LV_COORD_MAX, label_dsc.flag);

    lv_area_t txt_area;
    /*If the indicator is long enough put the text inside on the right*/
    if(lv_area_get_width(dsc->draw_area) > txt_size.x + 20) {
        txt_area.x2 = dsc->draw_area->x2 - 5;
        txt_area.x1 = txt_area.x2 - txt_size.x + 1;
        label_dsc.color = lv_color_white();
    }
    /*If the indicator is still short put the text out of it on the right*/
    else {
        txt_area.x1 = dsc->draw_area->x2 + 5;
        txt_area.x2 = txt_area.x1 + txt_size.x - 1;
        label_dsc.color = lv_color_black();
    }

    txt_area.y1 = dsc->draw_area->y1 + (lv_area_get_height(dsc->draw_area) - txt_size.y) / 2;
    txt_area.y2 = txt_area.y1 + txt_size.y - 1;
}
void ui_create_ota_check()
{
    lv_timer_enable(false);
    lv_obj_clean(Main_Panel);
    lv_group_remove_all_objs(group_keypad_indev);
    lv_obj_clear_flag(Main_Panel, LV_OBJ_FLAG_SCROLLABLE);
    
    lv_obj_t * result_img = lv_img_create(Main_Panel);
    lv_obj_align(result_img, LV_ALIGN_TOP_MID, 0, 200);
    ui_lv_img_set_src(result_img, (char*)"wait.png");

    
    tip_lable = lv_label_create(Main_Panel);
    lv_label_set_text(tip_lable, "Ota checking...");
    lv_obj_align(tip_lable, LV_ALIGN_TOP_MID, 0, 350);
    lv_obj_set_style_text_color(tip_lable, lv_color_hex(0x1B1B1B ), 0);
    lv_obj_set_style_text_font(tip_lable, &ali_middle_24, 0);
   
    percentage_lable = lv_label_create(Main_Panel);
    lv_label_set_text(percentage_lable, " ");
    lv_obj_align(percentage_lable, LV_ALIGN_TOP_MID, 0, 380);
    lv_obj_set_style_text_color(percentage_lable, lv_color_hex(0x1B1B1B ), 0);
    lv_obj_set_style_text_font(percentage_lable, &ali_middle_24, 0);

    static lv_style_t style_indic;

    lv_style_init(&style_indic);
    lv_style_set_bg_opa(&style_indic, LV_OPA_COVER);
    lv_style_set_bg_color(&style_indic, lv_palette_main(LV_PALETTE_GREEN));
    lv_style_set_bg_grad_color(&style_indic, lv_palette_main(LV_PALETTE_BLUE));

    Upgrade_bar = lv_bar_create(Main_Panel);
    lv_obj_add_event_cb(Upgrade_bar, event_cb, LV_EVENT_DRAW_PART_END, NULL);
    lv_obj_set_size(Upgrade_bar, LV_PCT(80), 20);
    lv_obj_align(Upgrade_bar,LV_ALIGN_TOP_MID, 0,420);
    lv_obj_add_style(Upgrade_bar, &style_indic, LV_PART_INDICATOR);
    
    event_trans_register(EVENT_OTA_CHECK);
    lv_timer_enable(true);
}