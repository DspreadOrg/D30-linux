#include "appinc.h"

#define ITEM_PAD_HIGHT  80

typedef struct
{
    char* str_index;
    char* str_content;
} st_menu_list;

static st_menu_list menu_list [] = {{"1","Auto Connect(Wifi)"},{"2","Auto Connect(4G)"},{"3","APN"},{"4","Airplane Mode"},{"5","Wifi connect"}};
static lv_obj_t* airplane_switch = NULL;
static lv_obj_t* wifi_auto_conn_switch = NULL;
static lv_obj_t* wl_auto_conn_switch = NULL;

static void return_setting(void* pra)
{
    event_ui_register(UI_SETTING_NETWORK);
    return;
};

static void touch_key_event_cb(lv_event_t * e)
{
    int keyCode;

    lv_event_code_t code = lv_event_get_code(e);
    char* index = (char*)lv_event_get_user_data(e);
     //OsLog(LOG_DEBUG," code = %d",code);
    if( code == LV_EVENT_KEY){
        keyCode = lv_event_get_key(e);
        OsLog(LOG_DEBUG," keyCode = %04x",keyCode);
        switch(keyCode){
            case KB_KEY_1:
                break;
            case KB_KEY_2:
                break;
            case KB_KEY_3:
                break;
            case KB_KEY_4:
                break;
            case KB_KEY_CANCEL://cancel
                event_ui_register(UI_SETTING);
                break;
        }  
    }else if (code == LV_EVENT_CLICKED){   
        keyCode = atoi(index);
        switch(keyCode){
            case 9:
                event_ui_register(UI_SETTING);
                break;
            case 3:
                event_ui_register(UI_APN_SET);
                break;
            case 5:
                event_ui_register(UI_WIFI_SEARCH);
                break;
        }
    }
}


static void wifi_switch_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * sw = lv_event_get_target(e);
    
    if(code == LV_EVENT_VALUE_CHANGED) {
        bool state = lv_obj_get_state(sw) & LV_STATE_CHECKED;
        if(state){
            OsLog(LOG_DEBUG,"open wifi auto connect");
            OsWifiSetAutoConnectStatus(1);
        }
        else
        {
            OsLog(LOG_DEBUG,"close wifi auto connect");
            OsWifiSetAutoConnectStatus(0);
            OsWifiDisconnect();
        }
    }
}

static void wl_switch_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * sw = lv_event_get_target(e);
    
    if(code == LV_EVENT_VALUE_CHANGED) {
        bool state = lv_obj_get_state(sw) & LV_STATE_CHECKED;
        if(state){
            OsLog(LOG_DEBUG,"open 4g auto connect");
            OsWlSetAutoConnectStatus(1);
        }
        else
        {
            OsLog(LOG_DEBUG,"close 4g auto connect");
            OsWifiSetAutoConnectStatus(0);
        }
    }
}

static void airplane_switch_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * sw = lv_event_get_target(e);
    
    if(code == LV_EVENT_VALUE_CHANGED) {
        bool state = lv_obj_get_state(sw) & LV_STATE_CHECKED;
        if(state){
            OsLog(LOG_DEBUG,"open airplane");
            OsSetAirplaneMode(1);
        }
        else
        {
            OsLog(LOG_DEBUG,"close airplane");
            OsSetAirplaneMode(0);
        }
    }
}

void ui_create_setting_network() {

    ui_lvgl_page_construct("Network",touch_key_event_cb);
    int mode = 0,wifi_auto = 0,wl_auto = 0;
    OsGetAirplaneMode(&mode);
    OsWifiGetAutoConnectStatus(&wifi_auto);
    OsWlGetAutoConnectStatus(&wl_auto);
    // 1 .add pad
    lv_obj_t *menu_pad = lv_obj_create(Main_Panel);
    lv_obj_set_size(menu_pad,480,780);
    lv_obj_set_pos(menu_pad,0,70);
    lv_obj_set_style_radius(menu_pad, 0, 0);
    lv_obj_set_style_pad_all(menu_pad,0,0);
    lv_obj_set_style_border_width(menu_pad,0,0);
    lv_obj_clear_flag(menu_pad, LV_OBJ_FLAG_SCROLLABLE); // 
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
        if (i == 0) //add switch
        {
            wifi_auto_conn_switch = lv_switch_create(item_pad);
            lv_obj_align(wifi_auto_conn_switch,LV_ALIGN_RIGHT_MID,-20,5);
            lv_obj_set_size(wifi_auto_conn_switch,100,30);
            //todo 
            lv_obj_add_event_cb(wifi_auto_conn_switch, wifi_switch_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
            // lv_obj_add_state(airplane_switch,LV_STATE_CHECKED);
            if(wifi_auto == 1){
                lv_obj_add_state(wifi_auto_conn_switch,LV_STATE_CHECKED);
            }else{
                lv_obj_clear_state(wifi_auto_conn_switch,LV_STATE_CHECKED);
            }
        }
        else if(i == 1)
        {
            wl_auto_conn_switch = lv_switch_create(item_pad);
            lv_obj_align(wl_auto_conn_switch,LV_ALIGN_RIGHT_MID,-20,5);
            lv_obj_set_size(wl_auto_conn_switch,100,30);
            //todo 
            lv_obj_add_event_cb(wl_auto_conn_switch, wl_switch_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
            if(wl_auto == 1){
                lv_obj_add_state(wl_auto_conn_switch,LV_STATE_CHECKED);
            }else{
                lv_obj_clear_state(wl_auto_conn_switch,LV_STATE_CHECKED);
            }
        }
        else if (i == 3) //add switch
        {
            airplane_switch = lv_switch_create(item_pad);
            lv_obj_align(airplane_switch,LV_ALIGN_RIGHT_MID,-20,5);
            lv_obj_set_size(airplane_switch,100,30);
            //todo 
            lv_obj_add_event_cb(airplane_switch, airplane_switch_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
            // lv_obj_add_state(airplane_switch,LV_STATE_CHECKED);

            if(mode == 1){
                lv_obj_add_state(airplane_switch,LV_STATE_CHECKED);
            }else{
                lv_obj_clear_state(airplane_switch,LV_STATE_CHECKED);
            }
        }

        lv_obj_t *ui_item_separator = lv_obj_create(item_pad);
        lv_obj_set_size(ui_item_separator, 480, 2);
        lv_obj_align(ui_item_separator, LV_ALIGN_BOTTOM_MID, 0, -2);
        lv_obj_set_style_bg_color(ui_item_separator, lv_color_hex(0xDDDDDD), 0);
        lv_obj_set_style_border_width(ui_item_separator, 0, 0);
        lv_obj_set_style_pad_all(ui_item_separator, 0, 0);         
    }
    
    lv_timer_enable(true);
    
}
#define APN_OPER                        "46000"
#define APN                        "China Mobile Internet"
#define OPERTOR_NAME                            "cmnet"
char apn[32] = {0};
char user_name[64] = {0};
char user_pwd[32]={0};

static void ApnSet_cb(lv_event_t * event)
{
    lv_event_code_t code = lv_event_get_code(event);
	lv_indev_t * indev = lv_indev_get_act();
	lv_indev_type_t indev_type = lv_indev_get_type(indev);
    lv_obj_t * ta = lv_event_get_target(event);
    const char * txt = lv_textarea_get_text(ta);
    if(code == LV_EVENT_CANCEL){
        event_ui_register(UI_SETTING_NETWORK);

    }else if(code == LV_EVENT_READY){
        if(strlen(txt)>0){
            memset(apn,0,sizeof(apn));
            memset(user_name,0,sizeof(user_name));
            memset(user_pwd,0,sizeof(user_pwd));
            strcat((char*)apn,txt);
            event_ui_register(UI_APN_USERNAME);
        }
    }

}

void ui_create_apn_set()
{
    lv_timer_enable(false);
    lv_obj_clean(Main_Panel);
    lv_group_remove_all_objs(group_keypad_indev);
    lv_obj_clear_flag(Main_Panel, LV_OBJ_FLAG_SCROLLABLE); // Disable scrolling

    ui_lvgl_page_construct("Apn Set",touch_key_event_cb);
  

    lv_obj_t *menu_pad = lv_obj_create(Main_Panel);
    lv_obj_set_size(menu_pad,480,750);
    lv_obj_set_pos(menu_pad,0,70);
    lv_obj_set_style_radius(menu_pad, 0, 0);
    lv_obj_set_style_pad_all(menu_pad,0,0);
    lv_obj_set_style_border_width(menu_pad,0,0);

    lv_obj_t * tip_lable = lv_label_create(Main_Panel);
    lv_label_set_text(tip_lable, "Input Apn Name");  //If there is no username, press the confirm button
    lv_obj_align(tip_lable, LV_ALIGN_TOP_MID, 0, 80);
    lv_obj_set_style_text_color(tip_lable, lv_color_hex(0x1B1B1B), 0);
    lv_obj_set_style_text_font(tip_lable, &ali_middle_24, 0);

    lv_obj_t *input = lv_textarea_create(menu_pad);
    lv_obj_set_size(input,LV_PCT(100), 50);
    lv_obj_add_event_cb(input, ApnSet_cb, LV_EVENT_ALL, NULL);
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

static void UserName_cb(lv_event_t * event)
{
    lv_event_code_t code = lv_event_get_code(event);
	lv_indev_t * indev = lv_indev_get_act();
	lv_indev_type_t indev_type = lv_indev_get_type(indev);
    lv_obj_t * ta = lv_event_get_target(event);
    const char * txt = lv_textarea_get_text(ta);
    if(code == LV_EVENT_CANCEL){
        event_ui_register(UI_SETTING_NETWORK);

    }else if(code == LV_EVENT_READY){
        if(strlen(txt)>0){
            memset(user_name,0,sizeof(user_name));
            strcat((char*)user_name,txt);
            event_ui_register(UI_APN_UISERPWD);
        }
        else
        {
            event_trans_register(EVENT_APN_SET);
            ui_modal_dialog_warning("APN","setting...",3,NULL,"OK",return_setting);
        }
    }

}

void ui_create_apn_username()
{
    lv_timer_enable(false);
    lv_obj_clean(Main_Panel);
    lv_group_remove_all_objs(group_keypad_indev);
    lv_obj_clear_flag(Main_Panel, LV_OBJ_FLAG_SCROLLABLE); // Disable scrolling

    ui_lvgl_page_construct("Apn Set",touch_key_event_cb);
  

    lv_obj_t *menu_pad = lv_obj_create(Main_Panel);
    lv_obj_set_size(menu_pad,480,750);
    lv_obj_set_pos(menu_pad,0,70);
    lv_obj_set_style_radius(menu_pad, 0, 0);
    lv_obj_set_style_pad_all(menu_pad,0,0);
    lv_obj_set_style_border_width(menu_pad,0,0);

    lv_obj_t * tip_lable = lv_label_create(Main_Panel);
    lv_label_set_text(tip_lable, "Input User Name");
    lv_obj_align(tip_lable, LV_ALIGN_TOP_MID, 0, 80);
    lv_obj_set_style_text_color(tip_lable, lv_color_hex(0x1B1B1B), 0);
    lv_obj_set_style_text_font(tip_lable, &ali_middle_24, 0);

    lv_obj_t * tip_lable2 = lv_label_create(Main_Panel);
    lv_label_set_text(tip_lable2, "If there is no username, press the confirm button");  //
    lv_obj_align(tip_lable2, LV_ALIGN_TOP_MID, 0, 140);
    lv_obj_set_style_text_color(tip_lable2, lv_color_hex(0xFF0000), 0);
    lv_obj_set_style_text_font(tip_lable2, &ali_middle_18, 0);


    lv_obj_t *input = lv_textarea_create(menu_pad);
    lv_obj_set_size(input,LV_PCT(100), 50);
    lv_obj_add_event_cb(input, UserName_cb, LV_EVENT_ALL, NULL);
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

static void UserPwd_cb(lv_event_t * event)
{
    lv_event_code_t code = lv_event_get_code(event);
	lv_indev_t * indev = lv_indev_get_act();
	lv_indev_type_t indev_type = lv_indev_get_type(indev);
    lv_obj_t * ta = lv_event_get_target(event);
    const char * txt = lv_textarea_get_text(ta);
    if(code == LV_EVENT_CANCEL){
        event_ui_register(UI_SETTING_NETWORK);

    }else if(code == LV_EVENT_READY){
        if(strlen(txt)>0){
            memset(user_pwd,0,sizeof(user_pwd));
            strcat((char*)user_pwd,txt);
            event_trans_register(EVENT_APN_SET);
            ui_modal_dialog_warning("APN","setting...",3,NULL,"OK",return_setting);
        }
    }

}
void ui_create_apn_userpwd()
{
    lv_timer_enable(false);
    lv_obj_clean(Main_Panel);
    lv_group_remove_all_objs(group_keypad_indev);
    lv_obj_clear_flag(Main_Panel, LV_OBJ_FLAG_SCROLLABLE); //

    ui_lvgl_page_construct("Apn Set",touch_key_event_cb);
  

    lv_obj_t *menu_pad = lv_obj_create(Main_Panel);
    lv_obj_set_size(menu_pad,480,750);
    lv_obj_set_pos(menu_pad,0,70);
    lv_obj_set_style_radius(menu_pad, 0, 0);
    lv_obj_set_style_pad_all(menu_pad,0,0);
    lv_obj_set_style_border_width(menu_pad,0,0);

    lv_obj_t * tip_lable = lv_label_create(Main_Panel);
    lv_label_set_text(tip_lable, "Input password");
    lv_obj_align(tip_lable, LV_ALIGN_TOP_MID, 0, 80);
    lv_obj_set_style_text_color(tip_lable, lv_color_hex(0x1B1B1B), 0);
    lv_obj_set_style_text_font(tip_lable, &ali_middle_24, 0);


    lv_obj_t *input = lv_textarea_create(menu_pad);
    lv_obj_set_size(input,LV_PCT(100), 50);
    lv_obj_add_event_cb(input, UserPwd_cb, LV_EVENT_ALL, NULL);
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

void apn_cfg()
{
    int ret = 0;
    // char oper[8] = {0};
    // char networkType[16] = {0};
    // int count = 10;

    // ret = OsWlGetQNetworkInfo(networkType,oper);
    // if(ret != RET_OK)
    // {
    //     return;
    // }

    // OsLog(LOG_DEBUG,"networkType:%s",networkType);
    // OsLog(LOG_DEBUG,"oper:%s",oper);

    // if(strcmp(oper,APN_OPER) == 0){
    //     OsSetApnParams(APN,NULL,NULL);
    //     return;
    // }

     if(strlen(user_name) >0 && strlen(user_pwd) > 0)
        ret = OsSetApnParams(apn,user_name,user_pwd);
     else   
        ret = OsSetApnParams(apn,NULL,NULL);
     OsLog(LOG_DEBUG," set apn ret = %d",ret );
}