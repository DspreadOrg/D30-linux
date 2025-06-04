#include "appinc.h"
#include "ui_utils.h"

static ST_WifiApInfo * gAps = NULL;
static int wifi_ap_num = 0;
static int sel_idx = 0;
static unsigned char wifi_pwd[32];


static void touch_key_event_cb(lv_event_t * e)
{
    int keyCode;

    lv_event_code_t code = lv_event_get_code(e);
    char* index = (char*)lv_event_get_user_data(e);
     OsLog(LOG_DEBUG," code = %d",code);
    if( code == LV_EVENT_KEY){
        keyCode = lv_event_get_key(e);
        OsLog(LOG_DEBUG," keyCode = %04x",keyCode);
        switch(keyCode){
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
                break;
        }
    }
}

static void wifi_Select_cb(lv_event_t * event)
{	    
    lv_event_code_t code = lv_event_get_code(event);
    lv_indev_t * indev = lv_indev_get_act();
    lv_indev_type_t indev_type = lv_indev_get_type(indev);
	
    lv_obj_t * obj = lv_event_get_target(event);
    lv_obj_t * base=lv_event_get_user_data(event);
    const char * text=lv_list_get_btn_text(base, obj);
    unsigned char wifi_name[80]={0};
   if(indev_type==LV_INDEV_TYPE_POINTER && code == LV_EVENT_CLICKED)
    {
        memset(wifi_name,0,sizeof(wifi_name));
        strcat((char*)wifi_name,text);
        // T_U8_VIEW uvSel={wifi_name,strlen((char*)wifi_name)};
        sel_idx = lv_obj_get_index(obj);
        OsLog(LOG_DEBUG,"ap name [%d,%s]",sel_idx,wifi_name);
        event_ui_register(UI_WIFI_SELECT);
    }	
}


void ui_create_wifi_list()
{
    unsigned char index[8];
    int i;

    lv_timer_enable(false);
    lv_obj_clean(Main_Panel);

    ui_lvgl_page_construct("Setting",touch_key_event_cb);
    
  

    lv_obj_t *menu_pad = lv_obj_create(Main_Panel);
    lv_obj_set_size(menu_pad,480,750);
    lv_obj_set_pos(menu_pad,0,70);
    lv_obj_set_style_radius(menu_pad, 0, 0);
    lv_obj_set_style_pad_all(menu_pad,0,0);
    lv_obj_set_style_border_width(menu_pad,0,0);

    lv_obj_t * main_list = lv_list_create(menu_pad);
    lv_obj_set_size(main_list,480,780);
    //lv_obj_align(main_list,LV_ALIGN_TOP_RIGHT, 20,20);
    lv_group_remove_all_objs(group_keypad_indev);
    lv_obj_add_style(main_list, &ali_middle_14, LV_STATE_DEFAULT);

    lv_obj_t * btn_list;
    unsigned char wifi_disp[32]={0};
    lv_group_remove_all_objs(group_keypad_indev);
    for(i=0;i<wifi_ap_num;i++)
    {
        memset(wifi_disp,0x00,32);
        memcpy(wifi_disp,gAps[i].Essid,strlen(gAps[i].Essid));
        // Memset_(serNum,0,sizeof(serNum));
        // Sprintf_(serNum,"%d. ",i);
        OsLog(LOG_DEBUG,"--i=%d--%s",i,wifi_disp);
        btn_list = lv_list_add_btn(main_list, NULL,wifi_disp);
        lv_obj_add_style(btn_list, &ali_middle_24, LV_STATE_DEFAULT);
        lv_obj_add_event_cb(btn_list, wifi_Select_cb, LV_EVENT_ALL, main_list);
        lv_group_add_obj(group_keypad_indev,btn_list);
    }


    lv_timer_enable(true);
}

void ui_create_wifi_search()
{
    lv_timer_enable(false);
    lv_obj_clean(Main_Panel);
    lv_group_remove_all_objs(group_keypad_indev);
    lv_obj_clear_flag(Main_Panel, LV_OBJ_FLAG_SCROLLABLE);
    
    lv_obj_t * result_img = lv_img_create(Main_Panel);
    lv_obj_align(result_img, LV_ALIGN_TOP_MID, 0, 200);
    ui_lv_img_set_src(result_img, (char*)"wait.png");

    lv_obj_t * tip_lable = lv_label_create(Main_Panel);
    lv_label_set_text(tip_lable, "Wifi searching...");
    lv_obj_align(tip_lable, LV_ALIGN_TOP_MID, 0, 350);
    lv_obj_set_style_text_color(tip_lable, lv_color_hex(0x1B1B1B ), 0);
    lv_obj_set_style_text_font(tip_lable, &ali_middle_24, 0);
    
    event_trans_register(EVENT_WIFI_SEARCH);
    lv_timer_enable(true);
}

static void WifiConnect_cb(lv_event_t * event)
{
    lv_event_code_t code = lv_event_get_code(event);
	lv_indev_t * indev = lv_indev_get_act();
	lv_indev_type_t indev_type = lv_indev_get_type(indev);
    lv_obj_t * ta = lv_event_get_target(event);
    const char * txt = lv_textarea_get_text(ta);
    if(code == LV_EVENT_CANCEL){
        event_ui_register(UI_IDLE);

    }else if(code == LV_EVENT_READY){
        if(strlen(txt)>0){
            memset(wifi_pwd,0,sizeof(wifi_pwd));
            strcat((char*)wifi_pwd,txt);
            event_ui_register(UI_WIFI_CONNECT);
        }
    }

}

void ui_create_wifi_select()
{
    lv_timer_enable(false);
    lv_obj_clean(Main_Panel);
    lv_group_remove_all_objs(group_keypad_indev);
    lv_obj_clear_flag(Main_Panel, LV_OBJ_FLAG_SCROLLABLE); // Disable scrolling

    ui_lvgl_page_construct("Wifi Connect",touch_key_event_cb);
  

    lv_obj_t *menu_pad = lv_obj_create(Main_Panel);
    lv_obj_set_size(menu_pad,480,750);
    lv_obj_set_pos(menu_pad,0,70);
    lv_obj_set_style_radius(menu_pad, 0, 0);
    lv_obj_set_style_pad_all(menu_pad,0,0);
    lv_obj_set_style_border_width(menu_pad,0,0);

    lv_obj_t * tip_lable = lv_label_create(Main_Panel);
    lv_label_set_text(tip_lable, "input ap password");
    lv_obj_align(tip_lable, LV_ALIGN_TOP_MID, 0, 80);
    lv_obj_set_style_text_color(tip_lable, lv_color_hex(0x1B1B1B), 0);
    lv_obj_set_style_text_font(tip_lable, &ali_middle_24, 0);


        lv_obj_t *input = lv_textarea_create(menu_pad);
    lv_obj_set_size(input,LV_PCT(100), 50);
    lv_obj_add_event_cb(input, WifiConnect_cb, LV_EVENT_ALL, NULL);
;
    lv_textarea_set_max_length(input, 30);
	lv_textarea_set_align(input, LV_TEXT_ALIGN_CENTER);
    lv_textarea_set_one_line(input, true);   
	lv_obj_align_to(input,menu_pad,LV_ALIGN_TOP_MID, 0, 15);
    lv_obj_set_style_text_font(input, &ali_middle_24, 0);

    lv_obj_align(input, LV_ALIGN_TOP_MID, 0, 130);

    lv_obj_t * line = lv_obj_create(menu_pad);
    lv_obj_set_size(line, 300, 2); //
    lv_obj_align(line, LV_ALIGN_TOP_MID, 0, 180);
    lv_obj_set_style_bg_color(line, lv_palette_main(LV_PALETTE_GREY), 0);
    lv_obj_clear_flag(line, LV_OBJ_FLAG_SCROLLABLE);       
    

    lv_obj_t *keypad=lv_keyboard_create(menu_pad);
    static lv_style_t style_keypad;
    lv_style_init(&style_keypad);
    lv_style_set_text_font(&style_keypad, &lv_font_montserrat_24);
    lv_style_set_text_align(&style_keypad, LV_TEXT_ALIGN_CENTER);
	lv_style_set_bg_color(&style_keypad, lv_palette_lighten(LV_PALETTE_GREY, 5));
	lv_style_set_border_color(&style_keypad, lv_palette_lighten(LV_PALETTE_GREY, 5));
    lv_obj_add_style(keypad, &style_keypad, LV_STATE_DEFAULT);  

    lv_obj_set_size(keypad,LV_PCT(100), 200);
    lv_obj_align(keypad, LV_ALIGN_TOP_MID, 0, 240);
    lv_keyboard_set_textarea(keypad,input);	

    lv_timer_enable(true);
}

void ui_create_wifi_connect()
{
    lv_timer_enable(false);
    lv_obj_clean(Main_Panel);
    lv_group_remove_all_objs(group_keypad_indev);
    lv_obj_clear_flag(Main_Panel, LV_OBJ_FLAG_SCROLLABLE);
    
    lv_obj_t * result_img = lv_img_create(Main_Panel);
    lv_obj_align(result_img, LV_ALIGN_TOP_MID, 0, 200);
    ui_lv_img_set_src(result_img, (char*)"wait.png");

    lv_obj_t * tip_lable = lv_label_create(Main_Panel);
    lv_label_set_text(tip_lable, "Wifi connecting...");
    lv_obj_align(tip_lable, LV_ALIGN_TOP_MID, 0, 350);
    lv_obj_set_style_text_color(tip_lable, lv_color_hex(0x1B1B1B ), 0);
    lv_obj_set_style_text_font(tip_lable, &ali_middle_24, 0);
    
    event_trans_register(EVENT_WIFI_CONNECT);
    lv_timer_enable(true);
}

void wifi_search()
{
    PR_INT32 res = 1;
    PR_INT32 i, num,ret;

    PR_INT32 rssi,nIndexOfSelect;
	if(gAps != NULL){
        free(gAps);
        gAps = NULL;
    }
	
	num = OsWifiScan (&gAps);
    if(num <= 0){
        OsLog(LOG_DEBUG,"Dspread: OsWifiScan ret = %d",num);
        set_fail_msg("Search fail");
        event_ui_register(UI_RESULT_FAIL);
    }
    else
    {
        wifi_ap_num = num;
        event_ui_register(UI_DISP_WIFI_LIST);
    }
}

void wifi_connect()
{
	ST_WifiApSet Ap;
	PR_INT32 rssi,ret,nIndexOfSelect,lvl;

	OsLog(LOG_ERROR,"Dspread: connect to  %d【%s】 pwd 【%s】 \n",sel_idx,gAps[sel_idx].Essid,wifi_pwd);
	memset(&Ap,0x0,sizeof(ST_WifiApSet));

	memcpy(Ap.Essid,gAps[sel_idx].Essid,strlen(gAps[sel_idx].Essid));
	memcpy(Ap.KeyUnion.PskKey.Key,wifi_pwd,strlen(wifi_pwd));
    Ap.KeyUnion.PskKey.KeyLen = strlen(wifi_pwd);
	Ap.AuthMode = gAps[sel_idx].AuthMode;
	Ap.SecMode = gAps[sel_idx].SecMode;
	memcpy(Ap.Bssid,gAps[sel_idx].Bssid,strlen(gAps[sel_idx].Bssid));
	ret = OsWifiConnect(&Ap,10*1000);
    OsLog(LOG_DEBUG,"OsWifiConnect ret = %d",ret);
    if(ret != 0){
        set_common_msg(1,"Connect fail");
        event_ui_register(UI_DISP_COMMON_MSG);
    }
	else
    {
        OsWlSetAutoConnectStatus(1);
        set_common_msg(0,"Connect Success");
        event_ui_register(UI_DISP_COMMON_MSG);
    }
}