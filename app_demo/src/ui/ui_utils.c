#include "appinc.h"

//menu
lv_style_t menubtn_style_default; // menu style
lv_style_t menubtn_style_pressed; //
unsigned char com_msg[128];
static int g_icon_type = 0;
static lv_obj_t * time_lable;
static lv_timer_t * result_timer = NULL;
static int timer_count = 0;

void ui_lvgl_style_init(){
    // init style
    lv_style_init(&menubtn_style_default);
    lv_style_set_bg_color(&menubtn_style_default, lv_color_white()); // 
    lv_style_set_border_color(&menubtn_style_default, lv_color_hex(0xBCBCBC)); // 
    lv_style_set_border_width(&menubtn_style_default, 1); // 
    lv_style_set_radius(&menubtn_style_default, 8); //

    // Initialize by pressing style # 008CFF
    lv_style_init(&menubtn_style_pressed);
    lv_style_set_bg_color(&menubtn_style_pressed, lv_color_hex(0xE7F5FF)); // 
    lv_style_set_border_color(&menubtn_style_pressed, lv_color_hex(0x1596FF)); // 

}

void ui_lv_img_set_src(lv_obj_t * obj, char* fileName){
    char filePath[128] = {0};
    char appPath[128] = {0};
    struct stat st;

    memset(filePath,0x0,sizeof(filePath));
    getcwd(appPath,sizeof(appPath));
    strcat(filePath,"A:");
    strcat(filePath,appPath);
    strcat(filePath,"/res/");
    strcat(filePath,fileName);
    // OsLog(LOG_DEBUG,"filePath = %s",filePath);
    lv_img_set_src(obj, filePath);
}

lv_obj_t * ui_lvgl_menubtn_creat(lv_obj_t *parent,int w, int h,char* text,char* filename)
{
    lv_obj_t *menu_btn = lv_obj_create(parent);
    lv_obj_set_size(menu_btn, w, h);
    
    lv_obj_t * icon = lv_img_create(menu_btn);
    ui_lv_img_set_src(icon, filename); // Use checkmark icon

    lv_obj_t * label = lv_label_create(menu_btn);
    lv_label_set_text(label, text);
    lv_obj_set_style_text_color(label, lv_color_hex(0x1B1B1B), 0);
    lv_obj_set_style_text_font(label, &ali_middle_24, 0);

    lv_obj_set_flex_flow(menu_btn, LV_FLEX_FLOW_COLUMN); // Vertical arrangement
    lv_obj_set_flex_align(menu_btn, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER); // All centered

    lv_obj_add_flag(menu_btn,LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_style(menu_btn, &menubtn_style_default, 0);
    lv_obj_add_style(menu_btn, &menubtn_style_pressed, LV_STATE_PRESSED);
    return menu_btn;
}

void ui_lvgl_page_construct(char *str_title, lv_event_cb_t event_cb)
{
    OsLog(LOG_DEBUG,"ui_create_waters" );
    lv_timer_enable(false);
    lv_obj_clean(Main_Panel);
    lv_group_remove_all_objs(group_keypad_indev);
    lv_obj_clear_flag(Main_Panel, LV_OBJ_FLAG_SCROLLABLE); // Disable scrolling


     // 1. Add the '<' key
     lv_obj_t * back_pad = lv_obj_create(Main_Panel);
     lv_obj_set_size(back_pad,200,60);
     lv_obj_add_flag(back_pad,LV_OBJ_FLAG_CLICKABLE);
     lv_obj_set_style_pad_all(back_pad,0,0);
     lv_obj_set_style_border_width(back_pad,0,0);
     lv_obj_add_event_cb(back_pad, event_cb, LV_EVENT_ALL,"9");
     lv_obj_align(back_pad,LV_ALIGN_TOP_LEFT, 0, 0); //Creating a PAD is easy to click on<
     lv_obj_t * back_btn = lv_img_create(back_pad);
     lv_obj_align(back_btn, LV_ALIGN_TOP_LEFT, 5, 4);
     ui_lv_img_set_src(back_btn, (char*)"icon_back.png");
     lv_obj_add_flag(back_btn, LV_OBJ_FLAG_CLICKABLE);
     lv_obj_add_event_cb(back_btn, event_cb, LV_EVENT_ALL,"9");
     lv_group_add_obj(group_keypad_indev,back_btn);
 
     // 2. Add title 'Sale'
     lv_obj_t * title = lv_label_create(Main_Panel);
     lv_label_set_text(title, str_title);
     lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 12);
     lv_obj_set_style_text_font(title, &ali_middle_36, 0);
}

void ui_lvgl_page_construct_v2(lv_obj_t * obj,char *str_title, lv_event_cb_t event_cb)
{
    OsLog(LOG_DEBUG,"ui_create_waters" );
    lv_timer_enable(false);
    lv_obj_clean(obj);
    lv_group_remove_all_objs(group_keypad_indev);
    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE); // Disable scrolling


     // 1. Add the '<' key
     lv_obj_t * back_pad = lv_obj_create(obj);
     lv_obj_set_size(back_pad,200,60);
     lv_obj_add_flag(back_pad,LV_OBJ_FLAG_CLICKABLE);
     lv_obj_set_style_pad_all(back_pad,0,0);
     lv_obj_set_style_border_width(back_pad,0,0);
     lv_obj_add_event_cb(back_pad, event_cb, LV_EVENT_ALL,"9");
     lv_obj_align(back_pad,LV_ALIGN_TOP_LEFT, 0, 0); //Creating a PAD is easy to click on<
     lv_obj_t * back_btn = lv_img_create(back_pad);
     lv_obj_align(back_btn, LV_ALIGN_TOP_LEFT, 5, 4);
     ui_lv_img_set_src(back_btn, (char*)"icon_back.png");
     lv_obj_add_flag(back_btn, LV_OBJ_FLAG_CLICKABLE);
     lv_obj_add_event_cb(back_btn, event_cb, LV_EVENT_ALL,"9");
     lv_group_add_obj(group_keypad_indev,back_btn);
 
     // 2. Add title 'Sale'
     lv_obj_t * title = lv_label_create(obj);
     lv_label_set_text(title, str_title);
     lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 12);
     lv_obj_set_style_text_font(title, &ali_middle_36, 0);
}


void ui_lvgl_create_numpinpad(lv_obj_t *parent,lv_event_cb_t num_key_event_cb,lv_event_cb_t touch_key_event_cb)
{
   // Create a numeric keypad panel
   lv_obj_t *numpad = lv_obj_create(parent);
   lv_obj_set_size(numpad, 480, 560);
   lv_obj_align(numpad, LV_ALIGN_BOTTOM_MID, 0, 10);
   lv_obj_set_style_bg_color(numpad, lv_color_hex(0xF3EFDD), 0);
   lv_obj_set_style_pad_all(numpad, 10, 0);
   lv_obj_set_flex_flow(numpad, LV_FLEX_FLOW_ROW_WRAP);
   lv_obj_set_flex_align(numpad, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
   lv_obj_set_style_border_width(numpad, 0, 0);
   lv_obj_set_style_radius(numpad, 20, 0);
   
   // Number keys 1-9
   const char *nums[] = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "Cancel", "0", "Backspace"};
   for(int i = 0; i < 12; i++) {
       lv_obj_t *btn = lv_btn_create(numpad);
       lv_obj_set_size(btn, 140, 100);
       lv_obj_add_event_cb(btn, num_key_event_cb, LV_EVENT_CLICKED, (void*)nums[i]);
       
       lv_obj_t *label = lv_label_create(btn);
       lv_label_set_text(label, nums[i]);
       lv_obj_set_style_text_color(label, lv_color_hex(0x191E3B), 0);
       lv_obj_center(label);
       if(i == 9){
           lv_obj_set_style_text_font(label, &ali_middle_24, 0); 
           lv_obj_set_style_bg_color(btn, lv_color_hex(0xFF5900), 0); 
       }else if(i == 11){
           lv_obj_set_style_text_font(label, &ali_middle_24, 0); 
           lv_obj_set_style_bg_color(btn, lv_color_hex(0xFFD500), 0);
       }else{
           lv_obj_set_style_text_font(label, &ali_middle_36, 0);  //28
           lv_obj_set_style_bg_color(btn, lv_color_white(), 0);
       }
       
   }
   
   lv_obj_t *btn = lv_btn_create(numpad);
   lv_obj_set_size(btn, 450, 80);
   lv_obj_add_event_cb(btn, touch_key_event_cb, LV_EVENT_CLICKED, "1");
   lv_obj_set_style_bg_color(btn, lv_color_hex(0xFF0039), 0);
   lv_obj_set_style_radius(btn, 40, 0);
   
   lv_obj_t *label = lv_label_create(btn);
   lv_label_set_text(label, "Confirm");
   lv_obj_set_style_text_font(label, &ali_middle_24, 0);
   lv_obj_center(label);
}


void remove_last_byte(char *str) {
    // Get the length of the string
    size_t len = strlen(str);
    
    // If the string is not empty, delete the last byte
    if (len > 0) {
        str[len - 1] = '\0';
    }
}

void get_feature_code(const char* date, const char* factory, char* out_code)
{
    char szBlock[16+1] = {0};
    char szTemp[8+1] = {0};
    int i;

    OsLog(LOG_DEBUG,"get_feature_code date= %s\n",date);
    OsLog(LOG_DEBUG,"get_feature_code factory= %s\n",factory);
    memcpy(szBlock, date, 12);

    for(i = 9; i < 16; i++)
    {
        szBlock[i] |= factory[i-9];
    }
    for(i = 0; i < 8; i++)
    {
        szTemp[i] = szBlock[i]^szBlock[i+8];
    }
    for(i = 0; i < 4; i++)
    {
        szTemp[i] = szTemp[i]^szTemp[i+4];
    }
    OsLog(LOG_DEBUG,"out= %02x%02x%02x%02x\n",szTemp[0],szTemp[1],szTemp[2],szTemp[3]);
    sprintf(out_code,"%02x%02x%02x%02x",szTemp[0],szTemp[1],szTemp[2],szTemp[3]);

    return ;
}

lv_style_t* ui_lvgl_style_gradient(lv_color_t start_color, lv_color_t grad_color)
{
    static lv_style_t grad_style;
    lv_style_init(&grad_style);
    lv_style_set_radius(&grad_style, 0); // Set rounded corners
    lv_style_set_bg_opa(&grad_style, LV_OPA_COVER); // Set opacity
    lv_style_set_bg_color(&grad_style, start_color); // Set starting color
    lv_style_set_bg_grad_color(&grad_style, grad_color); // Set end color
    lv_style_set_bg_grad_dir(&grad_style, LV_GRAD_DIR_VER); // Set gradient direction
    lv_style_set_bg_main_stop(&grad_style, 0); // Set Start Position
    lv_style_set_bg_grad_stop(&grad_style, 280); // Set end position
    return &grad_style;
}

void set_common_msg(int icon_type,const char * format, ...)
{
    va_list args;

    memset(com_msg,0x00,sizeof(com_msg));
	va_start(args, format);
    vsnprintf(com_msg, sizeof(com_msg), format, args);
	va_end(args);

    g_icon_type = icon_type;
	OsLog(LOG_DEBUG,"%s",com_msg);
    return;
}

static void touch_key_event_cb(lv_event_t * e)
{
    int keyCode;

    lv_event_code_t code = lv_event_get_code(e);
    char* index = (char*)lv_event_get_user_data(e);
    if( code == LV_EVENT_KEY){
        keyCode = lv_event_get_key(e);
        switch(keyCode){
            case KB_KEY_ENTER:
                if (result_timer != NULL )
                {
                    lv_timer_del(result_timer);
                    result_timer = NULL;
                }
                saveRec();
                if(get_transaction_data()->nTransType == TT_SALE ){
                    event_ui_register(UI_PRINTER);
                }else{
                    event_ui_register(UI_IDLE);
                }
                break;
            case KB_KEY_CANCEL://cancel
                if (result_timer != NULL )
                {
                    lv_timer_del(result_timer);
                    result_timer = NULL;
                }
                event_ui_register(UI_IDLE);
                break;
        }  
    }else if (code == LV_EVENT_CLICKED){   
        keyCode = atoi(index);
        switch(keyCode){
            case 9: //cancel
                if (result_timer != NULL )
                {
                    lv_timer_del(result_timer);
                    result_timer = NULL;
                }
                event_ui_register(UI_IDLE);
                break;
            case 1: //confirm
                if (result_timer != NULL )
                {
                    lv_timer_del(result_timer);
                    result_timer = NULL;
                }
                saveRec();
                if(get_transaction_data()->nTransType == TT_SALE ){
                    event_ui_register(UI_PRINTER);
                }else{
                    event_ui_register(UI_IDLE);
                }
                break;
        }
    }
}

static void disp_msg_cb(lv_timer_t *timer) {
    char szTime[8] = {0};
   
    timer_count++;
    sprintf(szTime,"(%ds)",10-timer_count);
    lv_label_set_text(time_lable, szTime);
    // If the execution frequency reaches 5 times, delete the timer
    if (timer_count >= 10) {
        if (result_timer != NULL )
        {
            lv_timer_del(result_timer);
            result_timer = NULL;
        }
        event_ui_register(UI_IDLE);
    }
}

void ui_create_disp_common_msg() {
    lv_timer_enable(false);
    lv_obj_clean(Main_Panel);
    lv_group_remove_all_objs(group_keypad_indev);
    lv_obj_clear_flag(Main_Panel, LV_OBJ_FLAG_SCROLLABLE); // Disable scrolling
    
    resumeStatusBarIcon();
    timer_count = 0;
    
    lv_obj_t * result_img = lv_img_create(Main_Panel);
    lv_obj_align(result_img, LV_ALIGN_TOP_MID, 0, 200);
    if(g_icon_type == 0)
        ui_lv_img_set_src(result_img, (char*)"icon_success.png");
    else if(g_icon_type ==1)
        ui_lv_img_set_src(result_img, (char*)"icon_fail.png");
    else
        ui_lv_img_set_src(result_img, (char*)"icon_fail.png");    
    lv_obj_add_event_cb(result_img, touch_key_event_cb, LV_EVENT_ALL,"");
    lv_group_add_obj(group_keypad_indev,result_img);

    lv_obj_t * msg_label = lv_label_create(Main_Panel);
    lv_label_set_text(msg_label, com_msg);
    lv_obj_align(msg_label, LV_ALIGN_TOP_MID, 0, 340);
    lv_obj_set_style_text_color(msg_label, lv_color_hex(0x1B1B1B), 0);
    lv_obj_set_style_text_font(msg_label, &ali_middle_36, 0);

    time_lable = lv_label_create(Main_Panel);
    lv_obj_align(time_lable, LV_ALIGN_TOP_MID, 0, 390);
    lv_label_set_text(time_lable, "(3s)");
    lv_obj_set_style_text_color(time_lable, lv_palette_main(LV_PALETTE_GREY), 0);
    lv_obj_set_style_text_font(time_lable, &ali_middle_24, 0);
    
    result_timer = lv_timer_create(disp_msg_cb, 1000,NULL);
	lv_timer_set_repeat_count(result_timer, 10);
    lv_timer_enable(true);

    lv_obj_t * btn = lv_btn_create(Main_Panel);
    lv_obj_add_style(btn, &menubtn_style_default, 0);
    lv_obj_set_size(btn, 400, 80);
    lv_obj_align(btn, LV_ALIGN_TOP_MID, 0, 650);
    lv_obj_t * btn_label = lv_label_create(btn);
    lv_label_set_text(btn_label, "Back");
    lv_obj_set_style_text_font(btn_label, &ali_middle_24, 0);
    lv_obj_set_style_align(btn_label, LV_ALIGN_CENTER, 0); // horizontally centered
    lv_obj_set_style_text_color(btn_label, lv_color_hex(0x1B1B1B), 0);
    lv_obj_clear_flag(btn, LV_OBJ_FLAG_SCROLLABLE); // 
    lv_obj_add_event_cb(btn, touch_key_event_cb, LV_EVENT_ALL,"9");

    lv_timer_enable(true);
}