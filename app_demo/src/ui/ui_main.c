#include "debug.h"
#include "appinc.h"


lv_obj_t * src_Panel = NULL;
lv_obj_t * Main_Panel = NULL;
lv_obj_t * statusBar_Panel = NULL;

static lv_obj_t * Icon_wifi = NULL;
static lv_obj_t * Icon_gprs = NULL;
static lv_obj_t * Icon_time = NULL;
static lv_obj_t * Icon_ppp_status = NULL;
static lv_obj_t * Icon_battery = NULL;

static lv_timer_t * statusbar_timer = NULL;

static int dev_gprs_signal_lvl = 0;
static int dev_sim_status = 0;
static int dev_ppp_status = 0;
static int dev_wifi_signal_lvl = 0;

static bool event_thread_flag = false;
static int Lvgl_Queue_Id = 0;

static lv_obj_t * menu_list[4] = {0};
static int mainindex = -1;

//Current main interface dwell time
static lv_timer_t * idel_timer = NULL;
static int idleTimeStart = 0;
static int idleIndex = 0;
static char* stridle[4] = {"idle1.png","idle2.png","idle3.png","idle4.png"};

//These two functions are only called in ui_main
extern void event_ui_handle(uint32 event_ui_id);
extern void event_trans_handle(uint32 event_trans_id);

static void update_menu_state(void)
{
    idleTimeStart = 0;
    for (size_t i = 0; i < sizeof(menu_list)/sizeof(lv_obj_t*); i++)
    {
        if (i == mainindex)
        {
            lv_obj_add_state(menu_list[i],LV_STATE_PRESSED);
        }
        else
        {
            lv_obj_clear_state(menu_list[i],LV_STATE_PRESSED);
        }
    }
    return;
}

static void key_down(void)
{
    if (mainindex == -1)
    {
        mainindex = 0;
    }
    else
    {
        mainindex++;
        if (mainindex > (sizeof(menu_list)/sizeof(lv_obj_t*)-1))
        {
            mainindex = (sizeof(menu_list)/sizeof(lv_obj_t*)-1);
        }
    }
    update_menu_state();
    return;
}

static void key_up(void)
{
    if (mainindex == -1)
    {
        mainindex = sizeof(menu_list)/sizeof(lv_obj_t*)-1;
    }
    else
    {
        mainindex--;
        if (mainindex < 0)
        {
            mainindex = 0;
        }
    }
    update_menu_state();
    return;
}

static void key_enter(void)
{
    if (mainindex != -1)
    {
        switch(mainindex){
            case 0:
                event_trans_register(EVENT_PAYMENT);
                break;
            case 1:
                TranRecord_set_time_limit(0);
                event_ui_register(UI_WATERS);
                break;
            case 2:
                event_ui_register(UI_SETTING);
                break;
            case 3:
                event_ui_register(UI_ABOUT);
                break;
        }
    }
    return;
}

static void touch_key_event_cb(lv_event_t * e)
{
    int keyCode;
    lv_event_code_t code = lv_event_get_code(e);
    char* index = (char*)lv_event_get_user_data(e);
   
    if (code == LV_EVENT_CLICKED)
    {   
        OsLog(LOG_DEBUG," ======code = %s",index);
        keyCode = atoi(index);
        switch(keyCode){
            case 1:
                sale_init();
                break;
            case 2:
                TranRecord_set_time_limit(0);
                event_ui_register(UI_WATERS);
                break;
            case 3:
                event_ui_register(UI_SETTING);
                break;
            case 4:
                event_ui_register(UI_ABOUT);
                break;
        }
    }else if( code == LV_EVENT_KEY){
        keyCode = lv_event_get_key(e);
        OsLog(LOG_DEBUG," ======keyCode = %d",keyCode);  
        switch(keyCode){
            case KB_KEY_1:
                TranRecord_set_time_limit(0);
                event_trans_register(EVENT_PAYMENT);
                break;
            case KB_KEY_2:
                event_ui_register(UI_WATERS);
                break;
            case KB_KEY_3:
                event_ui_register(UI_SETTING);
                break;
            case KB_KEY_4:
                event_ui_register(UI_ABOUT);
                break;
            case KB_KEY_CANCEL:
                break;
        }
    }
    else if (code == LV_EVENT_DELETE)
    {
        if (idel_timer != NULL)
        {
            lv_timer_pause(idel_timer);
            lv_timer_del(idel_timer);
            idel_timer = NULL;
        }
        
    }
}

static void lv_event_idle_timer(struct _lv_timer_t *t)
{   
    idleTimeStart++;
    if (idleTimeStart >= 20)
    {
        event_ui_register(UI_AD);
    }
    return;
}

#ifndef MAIN_UI_NEW

void lvgl_MainMenu(void)
{
    idleTimeStart = 0;
    lv_obj_clean(Main_Panel);
    lv_group_remove_all_objs(group_keypad_indev);

    OsCloseCamera();
    resumeStatusBarIcon();
    lv_obj_t * tip_lable = lv_label_create(Main_Panel);
    lv_label_set_text(tip_lable, "DSPREAD");
    lv_obj_align(tip_lable, LV_ALIGN_TOP_MID, 0, 80);
    lv_obj_set_style_text_font(tip_lable, &ali_middle_42, 0);
    lv_obj_set_style_text_color(tip_lable, lv_color_hex(0x191E3B), 0);
    lv_obj_add_event_cb(tip_lable, touch_key_event_cb, LV_EVENT_ALL,"");
    lv_group_add_obj(group_keypad_indev,tip_lable);
    
    // Create top New Sale card (red)
    lv_obj_t *new_sale_card = lv_obj_create(Main_Panel);
    lv_obj_set_size(new_sale_card, 440, 240);  // The width is slightly smaller than the screen width
    lv_obj_align(new_sale_card, LV_ALIGN_TOP_MID, 0, 200);
    lv_obj_set_style_bg_color(new_sale_card, lv_color_hex(0xff3333), 0);  // Red background
    lv_obj_set_style_radius(new_sale_card, 0, 0);  // Remove rounded corners
    lv_obj_add_event_cb(new_sale_card, touch_key_event_cb, LV_EVENT_ALL,"1");

    //Add New Sale Card Content
    //Image (using LVGL built-in symbols as an example here)
    lv_obj_t *new_sale_img = lv_img_create(new_sale_card);
    ui_lv_img_set_src(new_sale_img,"icon_NEW SALE2.png");
    lv_obj_align(new_sale_img, LV_ALIGN_TOP_MID, 0, 20);

    // text
    lv_obj_t *new_sale_label = lv_label_create(new_sale_card);
    lv_label_set_text(new_sale_label, "New Sale");
    lv_obj_set_style_text_font(new_sale_label, &ali_middle_36, 0);
    lv_obj_set_style_text_color(new_sale_label, lv_color_white(), 0);  // White font
    lv_obj_align_to(new_sale_label, new_sale_img, LV_ALIGN_OUT_BOTTOM_MID, 0, 20);

    // Create Left Trade Card (purple) - Align with the top of the Right Setting Card
    lv_obj_t *trade_card = lv_obj_create(Main_Panel);
    lv_obj_set_size(trade_card, 210, 300);
    lv_obj_align(trade_card, LV_ALIGN_TOP_LEFT, 20, 460);  // The Y coordinate is the same as the Setting card at 240+200+20
    lv_obj_set_style_bg_color(trade_card, lv_color_hex(0x9933ff), 0);  // Purple background
    lv_obj_set_style_radius(trade_card, 0, 0);  //Remove rounded corners
    lv_obj_add_event_cb(trade_card, touch_key_event_cb, LV_EVENT_ALL, "2");

    // Add Trade Card Content
    lv_obj_t *trade_img = lv_img_create(trade_card);
    ui_lv_img_set_src(trade_img,"icon_Transactions.png");
    lv_obj_align(trade_img, LV_ALIGN_TOP_MID, 0, 80);

    lv_obj_t *trade_label = lv_label_create(trade_card);
    lv_label_set_text(trade_label, "Transactions");
    lv_obj_set_style_text_font(trade_label, &ali_middle_24, 0);
    lv_obj_set_style_text_color(trade_label, lv_color_white(), 0);  // White font
    lv_obj_align_to(trade_label, trade_img, LV_ALIGN_OUT_BOTTOM_MID, 0, 20);

    // Create a Setting card on the right side (light blue)
    lv_obj_t *setting_card = lv_obj_create(Main_Panel);
    lv_obj_set_size(setting_card, 210, 140);
    lv_obj_align(setting_card, LV_ALIGN_TOP_RIGHT, -20, 460);  // Align the Y coordinate 460 with the Trade card
    lv_obj_set_style_bg_color(setting_card, lv_color_hex(0x66ccff), 0);  // Light blue background
    lv_obj_set_style_radius(setting_card, 0, 0);  // Remove rounded corners
    lv_obj_add_event_cb(setting_card, touch_key_event_cb, LV_EVENT_ALL, "3");

    // Add Setting Card Content
    lv_obj_t *setting_img = lv_img_create(setting_card);
    ui_lv_img_set_src(setting_img,"icon_setting.png");
    lv_obj_align(setting_img, LV_ALIGN_TOP_MID, 0, 15);

    lv_obj_t *setting_label = lv_label_create(setting_card);
    lv_label_set_text(setting_label, "Setting");
    lv_obj_set_style_text_font(setting_label, &lv_font_montserrat_20, 0);
    lv_obj_set_style_text_color(setting_label, lv_color_white(), 0);  // White font
    lv_obj_align_to(setting_label, setting_img, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);

    // Create the About card on the right (dark blue)
    lv_obj_t *about_card = lv_obj_create(Main_Panel);
    lv_obj_set_size(about_card, 210, 140);
    lv_obj_align_to(about_card, setting_card, LV_ALIGN_OUT_BOTTOM_MID, 0, 20);
    lv_obj_set_style_bg_color(about_card, lv_color_hex(0x0066cc), 0);  // Light blue background
    lv_obj_set_style_radius(about_card, 0, 0);  // Remove rounded corners
    lv_obj_add_event_cb(about_card, touch_key_event_cb, LV_EVENT_ALL, "4");

    // Add About Card Content
    lv_obj_t *about_img = lv_img_create(about_card);
    ui_lv_img_set_src(about_img,"icon_about.png");
    lv_obj_align(about_img, LV_ALIGN_TOP_MID, 0, 15);

    lv_obj_t *about_label = lv_label_create(about_card);
    lv_label_set_text(about_label, "About");
    lv_obj_set_style_text_font(about_label, &lv_font_montserrat_20, 0);
    lv_obj_set_style_text_color(about_label, lv_color_white(), 0);  // White font
    lv_obj_align_to(about_label, about_img, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
}
#endif
static void lv_event_ad_timer(struct _lv_timer_t *t)
{   
    lv_obj_t *temp_img = t->user_data;
    idleIndex++;
    idleIndex = idleIndex%(sizeof(stridle)/sizeof(char*));
    ui_lv_img_set_src(temp_img, (char*)stridle[idleIndex]);
    return;
}


static void touch_key_ad_event_cb(lv_event_t * e)
{
    int keyCode;
    lv_event_code_t code = lv_event_get_code(e);
    char* index = (char*)lv_event_get_user_data(e);
   
    if (code == LV_EVENT_CLICKED)
    {   
        event_ui_register(UI_IDLE);       
    }else if( code == LV_EVENT_KEY){
        event_ui_register(UI_IDLE);    
    }
    else if (code == LV_EVENT_DELETE)
    {
        lv_timer_t* temp = e->user_data;
        lv_timer_del(temp);
    }
}

void ui_create_ad() {

    suspendStatusBarIcon();
    lv_timer_enable(false);
    lv_obj_clean(Main_Panel);
    lv_group_remove_all_objs(group_keypad_indev);
    lv_obj_clear_flag(Main_Panel, LV_OBJ_FLAG_SCROLLABLE); // Disable scrolling
    //pad
    lv_obj_t *idle_pad = lv_obj_create(Main_Panel);
    lv_obj_set_size(idle_pad, 320, 240);
    lv_obj_align(idle_pad, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_set_style_border_width(idle_pad, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_all(idle_pad, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_t *idle_img = lv_img_create(idle_pad);
    idleIndex = 0;
    ui_lv_img_set_src(idle_img, (char*)stridle[idleIndex]);
    lv_obj_align(idle_img, LV_ALIGN_CENTER, 0, 0);
    lv_timer_t* payment_img_timer = lv_timer_create(lv_event_ad_timer, 3000, idle_img);   // Responsible for updating card swiping prompts
    lv_obj_add_event_cb(idle_img, touch_key_ad_event_cb, LV_EVENT_ALL, payment_img_timer);   // Responsible for turning off the timer
    lv_obj_add_flag(idle_img,LV_OBJ_FLAG_CLICKABLE);
    lv_group_add_obj(group_keypad_indev,idle_img);

    lv_timer_enable(true);
}

void event_thread_mutex(Switch_t status)
{
    if(status == MUTEX_NO)
    {
        event_thread_flag = true;
    }
    else if(status == MUTEX_OFF)
    {
        event_thread_flag = false;
    }
}

bool event_thread_mutex_status()
{
    return event_thread_flag;
}

static int Event_Queue_Id = 0;
void event_trans_register(uint32 msg_id)
{
	OsSleep(50);
    DSP_Info("event_trans_register msg_id = %d ",msg_id);
	if(event_thread_mutex_status())
	{   
		return;
	}
	event_thread_mutex(MUTEX_NO);
	message_body_t msg;
    msg.msg_id = msg_id;
	if (msgsnd(Event_Queue_Id, &msg, sizeof(message_body_t), 0) == -1) {
		DSP_Info("EventRegister event_id = %d msgsnd failed",msg_id);
        event_thread_mutex(MUTEX_OFF);
    }
}

void *event_trans_thread(void *args)
{
	message_body_t msg;
	int ret = 0;

	key_t key;
	char *file = "/tmp/event_msg";
	FILE *fp = fopen(file, "w");
	if (fp != NULL)
	{
		fclose(fp);
	}

    // Create a unique key
    key = ftok("/tmp/event_msg", 'm');
    if (key == -1) {
		return NULL;
    }

	    // Create a message queue
    Event_Queue_Id = msgget(key, IPC_CREAT | 0666);
    if (Event_Queue_Id == -1) {
        DSP_Info("Event_Queue_Id create failed");
		return NULL;
    }
    while(1)
    {
		memset(&msg,0,sizeof(message_body_t));
		ret = -1;
		ret = msgrcv(Event_Queue_Id, &msg, sizeof(message_body_t), 0, 0);
		if (ret != -1)
		{
			event_trans_handle(msg.msg_id);
			event_thread_mutex(MUTEX_OFF);	
		}
        OsSleep(10);
    }

}

void *statusbar_thread(void *args)
{
    int sim_status = 0;
    OsWlLock();
    OsWifiOpen();

	while(1){
        dev_wifi_signal_lvl = OsGetWifiSignalStrength();
        OsWlGetSimStatus(&dev_sim_status);
        // DSP_Info("dev_sim_status = %d",dev_sim_status);

        // DSP_Info("dev_ppp_status = %d",dev_ppp_status);
        
        if(dev_sim_status){
            dev_gprs_signal_lvl = OsWlGetSignal();
            if(RET_OK == OsWlCheck()){
                dev_ppp_status = 1;
            }else{
                dev_ppp_status = 0;
            }
        }else{
            dev_sim_status = 0;
            dev_ppp_status = 0;
        }
        OsSleep(500);
    }
}

void backgroundthread_init()
{
	pthread_t event_tid;
	pthread_t signal_tid;
	
    if(pthread_create(&signal_tid,NULL,statusbar_thread,NULL) != 0){
        DSP_Info("can't create thread\n");
    }
        
	if(pthread_create(&event_tid,NULL,event_trans_thread,NULL) != 0){
        DSP_Info("can't create thread\n");
    }     
}

int lvgl_queue_init()
{
	int ret = 0;

	key_t key;
	char *file = "/tmp/lvgl_msg";
	FILE *fp = fopen(file, "w");
	if (fp != NULL)
	{
        fclose(fp);
	}

    // Create a unique key
    key = ftok("/tmp/lvgl_msg", 'm');
    if (key == -1) {
        DSP_Debug();
		goto EXIT;
    }

	    // 创建消息队列
    Lvgl_Queue_Id = msgget(key, IPC_CREAT | 0666);
    if (Lvgl_Queue_Id == -1) {
		goto EXIT;
    }
    ret = 0;
 EXIT:
    return ret;      
}

void event_ui_register(uint32 msg_id)
{
    DSP_Debug();
    OsLog(LOG_DEBUG, "msg_id:  %d ", msg_id);
    queue_message_body_t queMsg;
    queMsg.msgType = 1;
    queMsg.msg.msg_id = msg_id;
	if (msgsnd(Lvgl_Queue_Id, &queMsg, sizeof(queue_message_body_t)-sizeof(long), 0) == -1) {
		DSP_Debug();
    }
    OsSleep(5);
}

void updateStatusBarIcon(lv_timer_t * timer)
{   
    ST_TIME sysTime;
    char szBuf[16] = {0};
    int bat_val;

    OsGetTime(&sysTime);
    sprintf(szBuf,"%d:%02d",sysTime.Hour,sysTime.Minute);
    lv_label_set_text(Icon_time, (char*)szBuf);

    bat_val = OsCheckBattery();
    // DSP_Info("bat_val = %d",bat_val);
    if(bat_val == BATTERY_LEVEL_COMPLETE){
        ui_lv_img_set_src(Icon_battery,(char*)"battery_charging_finish.png");
    }else if(bat_val == BATTERY_LEVEL_CHARGE){
        ui_lv_img_set_src(Icon_battery, (char*)"battery_charging.png");
    }else if(bat_val == BATTERY_LEVEL_4){
        ui_lv_img_set_src(Icon_battery, (char*)"battery_4.png");
    }else if(bat_val == BATTERY_LEVEL_3){
        ui_lv_img_set_src(Icon_battery, (char*)"battery_3.png");
    }else if(bat_val == BATTERY_LEVEL_2){
        ui_lv_img_set_src(Icon_battery, (char*)"battery_2.png");
    }else if(bat_val == BATTERY_LEVEL_1){
        ui_lv_img_set_src(Icon_battery, (char*)"battery_1.png");
    }else{
        ui_lv_img_set_src(Icon_battery, (char*)"battery_0.png");
    }

    if(dev_sim_status){
        //There is a SIM card available
        lv_obj_clear_flag(Icon_gprs, LV_OBJ_FLAG_HIDDEN);
        if(dev_gprs_signal_lvl == 1){
            ui_lv_img_set_src(Icon_gprs, (char*)"signal_1.png");
        }else if(dev_gprs_signal_lvl == 2){
            ui_lv_img_set_src(Icon_gprs,  (char*)"signal_2.png");
        }else if(dev_gprs_signal_lvl == 3){
            ui_lv_img_set_src(Icon_gprs,  (char*)"signal_3.png");
        }else if(dev_gprs_signal_lvl == 4){
            ui_lv_img_set_src(Icon_gprs,  (char*)"signal_4.png");
        }else if(dev_gprs_signal_lvl == 5){
            ui_lv_img_set_src(Icon_gprs,  (char*)"signal_5.png");
        }else{
            ui_lv_img_set_src(Icon_gprs,  (char*)"signal_0.png");
        }
        if(dev_ppp_status){
            lv_obj_clear_flag(Icon_ppp_status, LV_OBJ_FLAG_HIDDEN); //Clear hidden flag=Show image
            ui_lv_img_set_src(Icon_ppp_status,  (char*)"icon_4g.png");
        }else{
            lv_obj_add_flag(Icon_ppp_status, LV_OBJ_FLAG_HIDDEN);//Set hidden flag=hide image
        }
        
    }else{
        //No SIM card
        lv_obj_clear_flag(Icon_gprs, LV_OBJ_FLAG_HIDDEN);//Set hidden flag=hide image
        ui_lv_img_set_src(Icon_gprs,  (char*)"sim_no.png");
        lv_obj_add_flag(Icon_ppp_status, LV_OBJ_FLAG_HIDDEN);//Set hidden flag=hide image
    }

    lv_obj_clear_flag(Icon_wifi, LV_OBJ_FLAG_HIDDEN);
    if(dev_wifi_signal_lvl == 1 || dev_wifi_signal_lvl == 2){
        ui_lv_img_set_src(Icon_wifi, (char*)"wifi_1.png");
    }else if(dev_wifi_signal_lvl == 3){
        ui_lv_img_set_src(Icon_wifi,  (char*)"wifi_2.png");
    }else if(dev_wifi_signal_lvl == 4){
        ui_lv_img_set_src(Icon_wifi,  (char*)"wifi_3.png");
    }else{
        lv_obj_add_flag(Icon_wifi, LV_OBJ_FLAG_HIDDEN); //Clear hidden flag=Show image
    }
}

void showStatusBar(){
    // lv_obj_clean(lv_scr_act());
    src_Panel = lv_scr_act();
    if (NULL == Main_Panel)
    {
        Main_Panel=lv_obj_create(lv_scr_act());
    }
    lv_obj_set_style_bg_opa(src_Panel, LV_OPA_COVER, LV_PART_MAIN);//Set screen transparency
    lv_obj_set_style_bg_opa(Main_Panel, LV_OPA_COVER, LV_PART_MAIN);//Set screen transparency
    
    lv_obj_set_style_pad_all(Main_Panel, 0, 0); // Clear all margins
    lv_obj_set_style_radius(Main_Panel, 0, 0); // Set the fillet radius to 0
    // lv_obj_set_size(Main_Panel,320,240);
    lv_obj_set_pos(Main_Panel,0,24);
    lv_obj_set_size(Main_Panel,LV_PCT(100),830);
    lv_obj_set_scrollbar_mode(Main_Panel, LV_SCROLLBAR_MODE_OFF);

    statusBar_Panel=lv_obj_create(lv_scr_act());
    lv_obj_set_style_pad_all(statusBar_Panel, 0, 0); // Clear all margins
    lv_obj_set_style_radius(statusBar_Panel, 0, 0); // Set the fillet radius to 0
    lv_obj_set_pos(statusBar_Panel,0,0);
    lv_obj_set_size(statusBar_Panel,LV_PCT(100),24);
    lv_obj_set_scrollbar_mode(statusBar_Panel, LV_SCROLLBAR_MODE_OFF);

    Icon_gprs = lv_img_create(statusBar_Panel);
    lv_obj_align(Icon_gprs, LV_ALIGN_TOP_LEFT, 5, 4);

    Icon_ppp_status = lv_img_create(statusBar_Panel);
    lv_obj_align(Icon_ppp_status, LV_ALIGN_TOP_LEFT, 26, 10);

    lv_style_t input_style;
    lv_style_init(&input_style);
    lv_style_set_text_font(&input_style, &lv_font_montserrat_14);
    lv_style_set_text_align(&input_style, LV_TEXT_ALIGN_CENTER);
    Icon_time = lv_label_create(statusBar_Panel);
    lv_obj_align(Icon_time, LV_ALIGN_TOP_MID, 0, 6);
    lv_label_set_text(Icon_time, (char*)"");
    lv_style_set_text_font(&input_style, &lv_font_montserrat_14);
    
    Icon_battery = lv_img_create(statusBar_Panel);
    lv_obj_align(Icon_battery, LV_ALIGN_TOP_RIGHT, -10, 5);

    Icon_wifi = lv_img_create(statusBar_Panel);
    lv_obj_align(Icon_wifi, LV_ALIGN_TOP_RIGHT, -38, 6);

    statusbar_timer = lv_timer_create(updateStatusBarIcon,500,NULL); 
    lv_timer_set_repeat_count(statusbar_timer, -1);
}


void suspendStatusBarIcon(void)
{
    lv_timer_pause(statusbar_timer);
    lv_obj_set_pos(Main_Panel,0,0);
    lv_obj_set_size(Main_Panel,480,854);
    lv_obj_add_flag(statusBar_Panel,LV_OBJ_FLAG_HIDDEN);
    return;
}

void resumeStatusBarIcon(void)
{
    if (statusbar_timer->paused)
    {   
        lv_obj_clear_flag(statusBar_Panel,LV_OBJ_FLAG_HIDDEN);
        lv_obj_set_pos(Main_Panel,0,24);
        lv_obj_set_size(Main_Panel,LV_PCT(100),830);
        updateStatusBarIcon(NULL);
        lv_timer_resume(statusbar_timer);
    }
}

void ui_appinit()
{
    lv_timer_enable(false);
    lv_obj_clean(Main_Panel);
    lv_group_remove_all_objs(group_keypad_indev);
    lv_obj_clear_flag(Main_Panel, LV_OBJ_FLAG_SCROLLABLE);
    
    lv_obj_t * result_img = lv_img_create(Main_Panel);
    lv_obj_align(result_img, LV_ALIGN_TOP_MID, 0, 200);
    ui_lv_img_set_src(result_img, (char*)"wait.png");

    lv_obj_t * tip_lable = lv_label_create(Main_Panel);
    lv_label_set_text(tip_lable, "App init...");
    lv_obj_align(tip_lable, LV_ALIGN_TOP_MID, 0, 350);
    lv_obj_set_style_text_color(tip_lable, lv_color_hex(0x1B1B1B ), 0);
    lv_obj_set_style_text_font(tip_lable, &ali_middle_24, 0);
   
    lv_timer_enable(true);
}

void app_init()
{
    ui_appinit();
    lv_timer_handler();
    //You can perform some time-consuming operations during startup here
    #if 1    //for test
    Ped_Dukpt_Init();
    #endif 
}

void lvgl_main()
{
    int ret = 0;
    queue_message_body_t queMsg;

    lv_init();
	lv_port_disp_init();
	lv_port_indev_init();
    lvgl_queue_init();
    ui_lvgl_style_init();
    backgroundthread_init();
    showStatusBar();
    app_init();
    lvgl_MainMenu();
    while(1)
    {
        memset(&queMsg,0,sizeof(queue_message_body_t));
        ret = -1;
        ret = msgrcv(Lvgl_Queue_Id, &queMsg, sizeof(queue_message_body_t)-sizeof(long), 1, IPC_NOWAIT);
        if (ret != -1){
            event_ui_handle(queMsg.msg.msg_id);
        }
        lv_timer_handler();
        OsSleep(5);
    }
    
}