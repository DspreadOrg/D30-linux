#include "ota.h"

#define TMS_FW_HEART_CUSTOM_URL               "https://www.dspreadser.net:9011/terminal"  
static lv_obj_t * tip_lable = NULL;
static lv_obj_t *percentage_lable = NULL;
static lv_obj_t * Upgrade_bar = NULL;
// static unsigned char tip_msg[64];
// static unsigned char percentage_msg[8];

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

    larktms_ssl_Init(&larktmsCbk);
    LarkTms_Disp_Callback_Register(TmsDispCallback);
}

int ota_process()
{
    int result = -1,key;
    unsigned char dispMsg[64]= {0};

    larktms_init();
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