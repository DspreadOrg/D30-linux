#include "appinc.h"


static lv_obj_t * pin_lable;
static char szPin[16];


// 检查是否为闰年
bool is_leap_year(int year) {
    return (year % 400 == 0) || (year % 100 != 0 && year % 4 == 0);
}

// 检查月份天数是否有效
bool is_valid_day(int year, int month, int day) {
    if (day < 1) return false;
    
    int days_in_month[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    
    // 处理闰年二月
    if (month == 2 && is_leap_year(year)) {
        return day <= 29;
    }
    
    return month >= 1 && month <= 12 && day <= days_in_month[month-1];
}

// 检查时间格式是否为yyyyMMddHHmmss且年份≥1970
int set_yyyyMMddHHmmss(const char* time_str) {
    // 检查长度是否为14
    if (strlen(time_str) != 14) {
        return -1;
    }
    
    // 检查是否全是数字
    for (int i = 0; i < 14; i++) {
        if (time_str[i] < '0' || time_str[i] > '9') {
            return -1;
        }
    }
    
    // 提取各时间部分
    char year_str[5] = {0};
    char month_str[3] = {0};
    char day_str[3] = {0};
    char hour_str[3] = {0};
    char minute_str[3] = {0};
    char second_str[3] = {0};
    ST_TIME setTime;
    
    strncpy(year_str, time_str, 4);
    strncpy(month_str, time_str+4, 2);
    strncpy(day_str, time_str+6, 2);
    strncpy(hour_str, time_str+8, 2);
    strncpy(minute_str, time_str+10, 2);
    strncpy(second_str, time_str+12, 2);
    
    int year = atoi(year_str);
    int month = atoi(month_str);
    int day = atoi(day_str);
    int hour = atoi(hour_str);
    int minute = atoi(minute_str);
    int second = atoi(second_str);
    
    // 检查年份≥1970
    if (year < 1970) {
        return -1;
    }
    
    // 检查日期有效性
    if (!is_valid_day(year, month, day)) {
        return -1;
    }
    
    // 检查时间有效性
    if (hour < 0 || hour > 23 || minute < 0 || minute > 59 || second < 0 || second > 59) {
        return -1;
    }
    
	setTime.Year = year;
	setTime.Month = month;
	setTime.Day = day;
	setTime.Hour = hour;
	setTime.Minute = minute;
	setTime.Second = second;

    if(OsSetTime(&setTime) != 0){
        return -2;
    }
    return 0;
}

static void return_update(void* pra)
{
    event_ui_register(UI_IDLE);
    return;
};

static void touch_key_event_cb(lv_event_t * e)
{
    int keyCode;

    lv_event_code_t code = lv_event_get_code(e);
    char* index = (char*)lv_event_get_user_data(e);
    if( code == LV_EVENT_KEY){
        keyCode = lv_event_get_key(e);
        switch(keyCode){
            case KB_KEY_ENTER:
                if(strlen(szPin) == 14){
                    if(set_yyyyMMddHHmmss(szPin) == 0){
                        ui_modal_dialog_by_type(NULL,MODAL_TYPE_SUCCESS,"Set Success",3,return_update);
                       
                    }else{
                        ui_modal_dialog_by_type(NULL,MODAL_TYPE_ERROR,"Set Failed",3,return_update);
                    }
                }
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
                if(strlen(szPin) == 14){
                    if(set_yyyyMMddHHmmss(szPin) == 0){
                        ui_modal_dialog_by_type(NULL,MODAL_TYPE_SUCCESS,"Set Success",3,return_update);
                       
                    }else{
                        ui_modal_dialog_by_type(NULL,MODAL_TYPE_ERROR,"Set Failed",3,return_update);
                    }
                }
                break;
        }
    }
}
// Number key callback function
static void num_key_event_cb(lv_event_t *e) {
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
        event_ui_register(UI_IDLE);
        return;
    }else {
        if(strlen(szPin) < 14){
            strcat(szPin, num);
        }
    }
    lv_label_set_text(pin_lable, szPin);
}

void ui_create_setting_systemtime() {
   
    memset(szPin,0x0,sizeof(szPin));
    lv_timer_enable(false);
    lv_obj_clean(Main_Panel);
    lv_group_remove_all_objs(group_keypad_indev);

    lv_group_remove_all_objs(group_keypad_indev);
    lv_obj_clear_flag(Main_Panel, LV_OBJ_FLAG_SCROLLABLE); // Disable scrolling

    ui_lvgl_page_construct("Set System Time",touch_key_event_cb);

    lv_obj_t * tip_lable = lv_label_create(Main_Panel);
    lv_label_set_text(tip_lable, "Enter yyyyMMddHHmmss");
    lv_obj_align(tip_lable, LV_ALIGN_TOP_MID, 0, 120);
    lv_obj_set_style_text_color(tip_lable, lv_color_hex(0x1B1B1B), 0);
    lv_obj_set_style_text_font(tip_lable, &ali_middle_24, 0);


    pin_lable = lv_label_create(Main_Panel);
    lv_obj_align(pin_lable, LV_ALIGN_TOP_MID, 0, 180);
    lv_label_set_text(pin_lable, " ");
    lv_obj_set_style_text_color(pin_lable, lv_color_hex(0x1B1B1B), 0);
    lv_obj_set_style_text_font(pin_lable, &ali_middle_36, 0);

    // 5. Add a horizontal line (achieved through an object with a height of 1px)
    lv_obj_t * line = lv_obj_create(Main_Panel);
    lv_obj_set_size(line, 300, 2); // Width 200px, height 1px
    lv_obj_align(line, LV_ALIGN_TOP_MID, 0, 240);
    lv_obj_set_style_bg_color(line, lv_palette_main(LV_PALETTE_GREY), 0);
    lv_obj_clear_flag(line, LV_OBJ_FLAG_SCROLLABLE);

    ui_lvgl_create_numpinpad(Main_Panel,num_key_event_cb,touch_key_event_cb);
    lv_timer_enable(true);
}