#include "appinc.h"

static char err_msg[512] = {0};
static lv_obj_t * time_lable;
static lv_timer_t * result_timer = NULL;
static int timer_count = 0;

void saveRec(){
    get_transaction_data()->nStatus = 0x01;
    TranRecord_WriteAndUpdateTradeRec(get_transaction_data());
    TransactionData tempTec;
    memset(&tempTec,0x0,sizeof(TransactionData));
    //Write transaction records
    TranRecord_ReadTradeByOrder(get_transaction_data()->sOrderNo,&tempTec);
    DSP_Info("sOrderNo %s, sCardNo %s,sAmount %s, sTracker2 %s",tempTec.sOrderNo,tempTec.sCardNo,tempTec.sAmount,tempTec.sTracker2);
}

static void pay_failed_cb(lv_timer_t *timer) {
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

static void pay_success_cb(lv_timer_t *timer) {
    char szTime[8] = {0};
   
    timer_count++;
    sprintf(szTime,"(%ds)",3-timer_count);
    lv_label_set_text(time_lable, szTime);
    // If the execution frequency reaches 5 times, delete the timer
    if (timer_count >= 3) {
        saveRec();
        // if(get_transaction_data()->nTransType == TT_SALE && get_transaction_data()->nPosEntryMode != INPUT_QRCODE){
        if(get_transaction_data()->nTransType == TT_SALE ){
            event_ui_register(UI_PRINTER);
        }else{
            event_ui_register(UI_IDLE);
        }
        if (result_timer != NULL )
        {
            lv_timer_del(result_timer);
            result_timer = NULL;
        }
    }
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
                if(get_transaction_data()->nTransType == TT_SALE && get_transaction_data()->nStatus == APP_RC_COMPLETED){
                    OsLog(LOG_DEBUG,"get_transaction_data()->nStatus [%d]", get_transaction_data()->nStatus );
                    saveRec();
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
                
                if(get_transaction_data()->nTransType == TT_SALE && get_transaction_data()->nStatus == APP_RC_COMPLETED){
                    OsLog(LOG_DEBUG,"get_transaction_data()->nStatus [%d]", get_transaction_data()->nStatus );
                    saveRec();
                    event_ui_register(UI_SIGNATURE);
                }else{
                    event_ui_register(UI_IDLE);
                }
                break;
        }
    }
}

void set_fail_msg(const char * format, ...)
{
    va_list args;

    memset(err_msg,0x00,sizeof(err_msg));
	va_start(args, format);
    vsnprintf(err_msg, sizeof(err_msg), format, args);
	va_end(args);
	OsLog(LOG_DEBUG,"%s",err_msg);
    return;
}

void ui_create_payresult_fail() {
    lv_timer_enable(false);
    lv_obj_clean(Main_Panel);
    lv_group_remove_all_objs(group_keypad_indev);
    lv_obj_clear_flag(Main_Panel, LV_OBJ_FLAG_SCROLLABLE); // Disable scrolling
    
    resumeStatusBarIcon();
    timer_count = 0;
    
    lv_obj_t * result_img = lv_img_create(Main_Panel);
    lv_obj_align(result_img, LV_ALIGN_TOP_MID, 0, 200);
    ui_lv_img_set_src(result_img, (char*)"icon_fail.png");
    lv_obj_add_event_cb(result_img, touch_key_event_cb, LV_EVENT_ALL,"");
    lv_group_add_obj(group_keypad_indev,result_img);

    lv_obj_t * msg_label = lv_label_create(Main_Panel);
    lv_label_set_text(msg_label, err_msg);
    lv_obj_align(msg_label, LV_ALIGN_TOP_MID, 0, 340);
    lv_obj_set_style_text_color(msg_label, lv_color_hex(0x1B1B1B), 0);
    lv_obj_set_style_text_font(msg_label, &ali_middle_36, 0);

    time_lable = lv_label_create(Main_Panel);
    lv_obj_align(time_lable, LV_ALIGN_TOP_MID, 0, 390);
    lv_label_set_text(time_lable, "(3s)");
    lv_obj_set_style_text_color(time_lable, lv_palette_main(LV_PALETTE_GREY), 0);
    lv_obj_set_style_text_font(time_lable, &ali_middle_24, 0);
    
    result_timer = lv_timer_create(pay_failed_cb, 1000,NULL);
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
    lv_obj_clear_flag(btn, LV_OBJ_FLAG_SCROLLABLE); // Remove button scrollbar
    lv_obj_add_event_cb(btn, touch_key_event_cb, LV_EVENT_ALL,"9");

    lv_timer_enable(true);
}

void ui_create_payresult_success() {
    char szDisplayAmount[16] = {0};
    timer_count = 0;
    int nAmount = atoi(get_transaction_data()->sAmount);
    sprintf(szDisplayAmount,"$%ld.%02ld",nAmount/100,nAmount%100);

    lv_timer_enable(false);
    lv_obj_clean(Main_Panel);
    lv_group_remove_all_objs(group_keypad_indev);
    lv_obj_clear_flag(Main_Panel, LV_OBJ_FLAG_SCROLLABLE); // Disable scrolling
    
    resumeStatusBarIcon();
    lv_obj_t * result_img = lv_img_create(Main_Panel);
    lv_obj_align(result_img, LV_ALIGN_TOP_MID, 0, 200);
    ui_lv_img_set_src(result_img, (char*)"icon_success.png");
    lv_obj_add_event_cb(result_img, touch_key_event_cb, LV_EVENT_ALL,"");
    lv_group_add_obj(group_keypad_indev,result_img);

    lv_obj_t * tip_lable = lv_label_create(Main_Panel);
    lv_label_set_text(tip_lable, "Payment complete");
    lv_obj_align(tip_lable, LV_ALIGN_TOP_MID, 0, 340);
    lv_obj_set_style_text_color(tip_lable, lv_color_hex(0x1B1B1B), 0);
    lv_obj_set_style_text_font(tip_lable, &ali_middle_24, 0);


    lv_obj_t * amount_lable = lv_label_create(Main_Panel);
    lv_label_set_text(amount_lable, szDisplayAmount);
    lv_obj_align(amount_lable, LV_ALIGN_TOP_MID, 0, 380);
    lv_obj_set_style_text_color(amount_lable, lv_color_hex(0x1B1B1B), 0);
    lv_obj_set_style_text_font(amount_lable, &lv_font_montserrat_40, 0);

    time_lable = lv_label_create(Main_Panel);
    lv_obj_align(time_lable, LV_ALIGN_TOP_MID, 0, 440);
    lv_label_set_text(time_lable, "(3s)");
    lv_obj_set_style_text_color(time_lable, lv_palette_main(LV_PALETTE_GREY), 0);
    lv_obj_set_style_text_font(time_lable, &ali_middle_24, 0);

    lv_obj_t *btn = lv_btn_create(Main_Panel);
    lv_obj_set_size(btn, 400, 80);
    lv_obj_align(btn, LV_ALIGN_TOP_MID, 0, 650);
    lv_obj_add_event_cb(btn, touch_key_event_cb, LV_EVENT_CLICKED, "1");

    // Set the background color to gray
    lv_obj_set_style_bg_color(btn, lv_color_hex(0xFF0039), 0);  // gray background
    lv_obj_set_style_bg_opa(btn, LV_OPA_COVER, 0);  // Ensure opacity
    lv_obj_set_style_radius(btn, 0, 0);  // Remove rounded corners

    // Red font
    lv_obj_t *label = lv_label_create(btn);
    lv_label_set_text(label, "Done");
    lv_obj_set_style_text_color(label, lv_color_hex(0xFFFFFF), 0);  // Red font
    lv_obj_set_style_text_font(label, &ali_middle_24, 0);
    lv_obj_center(label);
    
     
    result_timer = lv_timer_create(pay_success_cb, 1000,NULL);
	lv_timer_set_repeat_count(result_timer, 3);
    lv_timer_enable(true);
}

void ui_create_pay_process() {
    lv_timer_enable(false);
    lv_obj_clean(Main_Panel);
    lv_group_remove_all_objs(group_keypad_indev);
    lv_obj_clear_flag(Main_Panel, LV_OBJ_FLAG_SCROLLABLE); // Disable scrolling
    
    timer_count = 0;
    //Create a Spinner object
    lv_obj_t * result_img = lv_img_create(Main_Panel);
    lv_obj_align(result_img, LV_ALIGN_TOP_MID, 0, 200);
    ui_lv_img_set_src(result_img, (char*)"wait.png");

    lv_obj_t * tip_lable = lv_label_create(Main_Panel);
    lv_label_set_text(tip_lable, "Processing...");
    lv_obj_align(tip_lable, LV_ALIGN_TOP_MID, 0, 350);
    lv_obj_set_style_text_color(tip_lable, lv_color_hex(0x1B1B1B ), 0);
    lv_obj_set_style_text_font(tip_lable, &ali_middle_24, 0);
    
    lv_timer_enable(true);
}


static void print_process_cb(lv_timer_t *timer){
    event_trans_register(EVENT_PRINT);
    lv_timer_del(timer);
}


static void touch_key_event_print_cb(lv_event_t * e)
{
    int keyCode;

    lv_event_code_t code = lv_event_get_code(e);
    char* index = (char*)lv_event_get_user_data(e);
    if( code == LV_EVENT_KEY){
        keyCode = lv_event_get_key(e);
        switch(keyCode){
            case KB_KEY_ENTER:
                event_ui_register(UI_PRINTERING);
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
                event_ui_register(UI_PRINTERING);
                break;
        }
    }
}

void ui_create_print() {

    lv_timer_enable(false);
    lv_obj_clean(Main_Panel);
    lv_group_remove_all_objs(group_keypad_indev);
    lv_obj_clear_flag(Main_Panel, LV_OBJ_FLAG_SCROLLABLE); // Disable scrolling
    
    lv_obj_t * result_img = lv_img_create(Main_Panel);
    lv_obj_align(result_img, LV_ALIGN_TOP_MID, 0, 200);
    ui_lv_img_set_src(result_img, (char*)"icon_print2.png");

    lv_obj_t * amount_lable = lv_label_create(Main_Panel);
    lv_label_set_text(amount_lable, "Print cardholder copy?");
    lv_obj_align(amount_lable, LV_ALIGN_TOP_MID, 0, 350);
    lv_obj_set_style_text_color(amount_lable, lv_color_hex(0x1B1B1B), 0);
    lv_obj_set_style_text_font(amount_lable, &ali_middle_24, 0);


    lv_obj_t *print_cancle_button = lv_btn_create(Main_Panel);
    lv_obj_set_size(print_cancle_button, 200, 80);
    lv_obj_align(print_cancle_button, LV_ALIGN_BOTTOM_LEFT, 12, 8);
    lv_obj_set_style_pad_all(print_cancle_button,0,0);
    lv_obj_set_style_bg_color(print_cancle_button, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(print_cancle_button,lv_color_hex(0xBCBCBC), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(print_cancle_button,1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_event_cb(print_cancle_button, touch_key_event_print_cb, LV_EVENT_ALL, "0");

    lv_obj_t * print_cancl_label =lv_label_create(print_cancle_button);
    lv_label_set_text(print_cancl_label, "No");
    lv_obj_align(print_cancl_label, LV_ALIGN_CENTER, 0, -4);
    lv_obj_set_style_text_color(print_cancl_label, lv_color_hex(0x000000), 0);
    lv_obj_set_style_text_font(print_cancl_label, &ali_middle_24, 0);

    lv_obj_t *print_comfirm_button = lv_btn_create(Main_Panel);
    lv_obj_set_size(print_comfirm_button, 200, 80);
    lv_obj_align(print_comfirm_button, LV_ALIGN_BOTTOM_RIGHT, -12, 8);
    lv_obj_set_style_pad_all(print_comfirm_button,0,0);
    lv_obj_set_style_bg_color(print_comfirm_button, lv_color_hex(0xFF0039), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(print_comfirm_button,0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_event_cb(print_comfirm_button, touch_key_event_print_cb, LV_EVENT_ALL, "28");

    lv_obj_t * print_comfirm_label =lv_label_create(print_comfirm_button);
    lv_label_set_text(print_comfirm_label, "YES");
    lv_obj_align(print_comfirm_label, LV_ALIGN_CENTER, 0, -4);
    lv_obj_set_style_text_font(print_comfirm_label, &ali_middle_24, 0);
    
    lv_group_add_obj(group_keypad_indev,print_comfirm_label);
    lv_obj_add_event_cb(print_comfirm_label, touch_key_event_print_cb, LV_EVENT_ALL, "28");
    lv_timer_enable(true);
}

void ui_create_printing() {

    lv_timer_enable(false);
    lv_obj_clean(Main_Panel);
    lv_group_remove_all_objs(group_keypad_indev);
    lv_obj_clear_flag(Main_Panel, LV_OBJ_FLAG_SCROLLABLE); // Disable scrolling
    
    lv_obj_t * result_img = lv_img_create(Main_Panel);
    lv_obj_align(result_img, LV_ALIGN_TOP_MID, 0, 200);
    ui_lv_img_set_src(result_img, (char*)"icon_printing2.png");

    lv_obj_t * amount_lable = lv_label_create(Main_Panel);
    lv_label_set_text(amount_lable, "Please Wait...");
    lv_obj_align(amount_lable, LV_ALIGN_TOP_MID, 0, 350);
    lv_obj_set_style_text_color(amount_lable, lv_color_hex(0x1B1B1B), 0);
    lv_obj_set_style_text_font(amount_lable, &ali_middle_24, 0);

    lv_timer_t * timer= lv_timer_create(print_process_cb, 1000,NULL);
	lv_timer_set_repeat_count(timer, 1);
    lv_timer_enable(true);
}