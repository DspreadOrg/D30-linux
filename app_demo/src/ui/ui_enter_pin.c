#include "appinc.h"

static lv_obj_t * pin_lable;
static char szPin[16];

static void touch_key_event_cb(lv_event_t * e)
{
    int keyCode;

    lv_event_code_t code = lv_event_get_code(e);
    char* index = (char*)lv_event_get_user_data(e);
    if( code == LV_EVENT_KEY){
        keyCode = lv_event_get_key(e);
        switch(keyCode){
            case KB_KEY_ENTER:
                if(strlen(szPin) >= 4){
                    memcpy(get_transaction_data()->sPin,szPin,strlen(szPin));
                    get_transaction_data()->emv_enter_offline_pin_result = 0;
                }
                break;
            case KB_KEY_CANCEL://cancel
                get_transaction_data()->emv_enter_offline_pin_result = -1;
                break;
        }  
    }else if (code == LV_EVENT_CLICKED){   
        keyCode = atoi(index);
        switch(keyCode){
            case 9:
                get_transaction_data()->emv_enter_offline_pin_result = -1;
                break;
            case 1:
                if(strlen(szPin) >= 4){
                    memcpy(get_transaction_data()->sPin,szPin,strlen(szPin));
                    get_transaction_data()->emv_enter_offline_pin_result = 0;
                }
                break;
        }
    }
}
// Number key callback function
static void num_key_event_cb(lv_event_t *e) {
    char szDisplayStart[16] = {0};
    char start[] = "****************";
    int nAmount;
    lv_event_code_t code = lv_event_get_code(e);
    const char *num = (char*)lv_event_get_user_data(e);

    OsLog(LOG_DEBUG,"num = %s",num);
    // // Processing digital inputs
    if(strcmp(num, "Backspace") == 0){
        size_t len = strlen(szPin);
        if(len >= 1) {
            szPin[len-1] = '\0';
        } 
    }else if(strcmp(num, "Cancel") == 0){
        get_transaction_data()->emv_enter_offline_pin_result = -1;
        return;
    }else {
        if(strlen(szPin) < 12){
            strcat(szPin, num);
        }
    }
    memcpy(szDisplayStart,start,strlen(szPin));
    lv_label_set_text(pin_lable, szDisplayStart);
}

static char pin[16] = {0};
// Number key callback function
static void show_pin_star(int pinLen) {
    char szDisplayStart[16] = {0};
    char start[] = "****************";
    
    if(pinLen <= 0){
        lv_label_set_text(pin_lable, " ");
    }else{
        memcpy(szDisplayStart,start,pinLen);
        lv_label_set_text(pin_lable, szDisplayStart);
    }
}

void* enterpin_thread_function(void* arg) {
    char Ksn[10] = {0};
    char pin[16] = {0};

    memset(pin,0x0,sizeof(pin));
    int ret = OsPedGetPinBlockDukptBySoftKeyboard(1,get_transaction_data()->sCardNo, 4, 12, 120, Ksn, pin,( void*)show_pin_star);
    OsLog(LOG_DEBUG,"OsPedGetPinBlockDukptBySoftKeyboard [%d]",ret);
    if(ret == 0){
        memcpy(get_transaction_data()->sPin,pin,8);
        event_ui_register(UI_SIGNATURE);
    }else{
        if(ret != -1){
            //Cancel not displaying error messages
            set_fail_msg("enter pin failed");
            event_ui_register(UI_RESULT_FAIL);
        }else{
            event_ui_register(UI_IDLE);
        }
    }
    return NULL;
}

void enter_pin_process(){
    pthread_t thread_id;
    pthread_create(&thread_id, NULL, enterpin_thread_function, NULL);
}

void ui_create_enter_offline_pin() {
   
    char szDisplayAmount[16] = {0};
    int nAmount = atoi(get_transaction_data()->sAmount);
    sprintf(szDisplayAmount,"total $%ld.%02ld",nAmount/100,nAmount%100);
    memset(szPin,0x0,sizeof(szPin));

    lv_timer_enable(false);
    lv_obj_clean(Main_Panel);
    lv_group_remove_all_objs(group_keypad_indev);

    lv_group_remove_all_objs(group_keypad_indev);
    lv_obj_clear_flag(Main_Panel, LV_OBJ_FLAG_SCROLLABLE); 

    ui_lvgl_page_construct("Sale",touch_key_event_cb);

    lv_obj_t * amount_lable = lv_label_create(Main_Panel);
    lv_label_set_text(amount_lable, szDisplayAmount);
    lv_obj_align(amount_lable, LV_ALIGN_TOP_MID, 0, 80);
    lv_obj_set_style_text_color(amount_lable, lv_color_hex(0x191E3B), 0);
    lv_obj_set_style_text_font(amount_lable, &ali_middle_36, 0);

    lv_obj_t * tip_lable = lv_label_create(Main_Panel);
    lv_label_set_text(tip_lable, "Enter youe PIN code");
    lv_obj_align(tip_lable, LV_ALIGN_TOP_MID, 0, 130);
    lv_obj_set_style_text_color(tip_lable, lv_color_hex(0x1B1B1B), 0);
    lv_obj_set_style_text_font(tip_lable, &ali_middle_24, 0);


    pin_lable = lv_label_create(Main_Panel);
    lv_obj_align(pin_lable, LV_ALIGN_TOP_MID, 0, 180);
    lv_label_set_text(pin_lable, " ");
    lv_obj_set_style_text_color(pin_lable, lv_color_hex(0x1B1B1B), 0);
    lv_obj_set_style_text_font(pin_lable, &lv_font_montserrat_48, 0);

    lv_obj_t * line = lv_obj_create(Main_Panel);
    lv_obj_set_size(line, 300, 2); 
    lv_obj_align(line, LV_ALIGN_TOP_MID, 0, 240);
    lv_obj_set_style_bg_color(line, lv_palette_main(LV_PALETTE_GREY), 0);
    lv_obj_clear_flag(line, LV_OBJ_FLAG_SCROLLABLE);

    ui_lvgl_create_numpinpad(Main_Panel,num_key_event_cb,touch_key_event_cb);
 
    lv_timer_enable(true);
}
void disp_mask_pin(int pinLen)
{
    char szDisplayStart[16] = {0};
    char start[] = "****************";
    
    if(pinLen <= 0){
        lv_label_set_text(pin_lable, " ");
    }else{
        memcpy(szDisplayStart,start,pinLen);
        lv_label_set_text(pin_lable, szDisplayStart);
    }
}

void ui_create_enter_online_pin()
{
    char szDisplayAmount[16] = {0};
    int nAmount = atoi(get_transaction_data()->sAmount);
    sprintf(szDisplayAmount,"total $%ld.%02ld",nAmount/100,nAmount%100);
    memset(szPin,0x0,sizeof(szPin));

    lv_timer_enable(false);
    lv_obj_clean(Main_Panel);
    lv_group_remove_all_objs(group_keypad_indev);

    lv_group_remove_all_objs(group_keypad_indev);
    lv_obj_clear_flag(Main_Panel, LV_OBJ_FLAG_SCROLLABLE); 

    ui_lvgl_page_construct("Sale",touch_key_event_cb);

    lv_obj_t * amount_lable = lv_label_create(Main_Panel);
    lv_label_set_text(amount_lable, szDisplayAmount);
    lv_obj_align(amount_lable, LV_ALIGN_TOP_MID, 0, 80);
    lv_obj_set_style_text_color(amount_lable, lv_color_hex(0x191E3B), 0);
    lv_obj_set_style_text_font(amount_lable, &ali_middle_36, 0);

    lv_obj_t * tip_lable = lv_label_create(Main_Panel);
    lv_label_set_text(tip_lable, "Enter youe PIN code");
    lv_obj_align(tip_lable, LV_ALIGN_TOP_MID, 0, 130);
    lv_obj_set_style_text_color(tip_lable, lv_color_hex(0x1B1B1B), 0);
    lv_obj_set_style_text_font(tip_lable, &ali_middle_24, 0);


    pin_lable = lv_label_create(Main_Panel);
    lv_obj_align(pin_lable, LV_ALIGN_TOP_MID, 0, 180);
    lv_label_set_text(pin_lable, " ");
    lv_obj_set_style_text_color(pin_lable, lv_color_hex(0x1B1B1B), 0);
    lv_obj_set_style_text_font(pin_lable, &lv_font_montserrat_48, 0);

    lv_obj_t * line = lv_obj_create(Main_Panel);
    lv_obj_set_size(line, 300, 2); 
    lv_obj_align(line, LV_ALIGN_TOP_MID, 0, 240);
    lv_obj_set_style_bg_color(line, lv_palette_main(LV_PALETTE_GREY), 0);
    lv_obj_clear_flag(line, LV_OBJ_FLAG_SCROLLABLE);

    lv_timer_enable(true);
}