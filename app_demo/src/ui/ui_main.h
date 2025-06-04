#ifndef UI_MAIN_H
#define UI_MAIN_H

#include "typedefine.h"
#ifdef __cplusplus
extern "C" {
#endif

extern lv_obj_t * src_Panel;
extern lv_obj_t * Main_Panel;
extern lv_obj_t * statusBar_Panel;
typedef enum _Switch_t
{
	MUTEX_OFF = 0,
	MUTEX_NO = 1
}Switch_t;


typedef struct _message_body_t
{
    int msg_id;  
    uint32     wparam;  
    uint32     lparam;  
    void*   pparam;  
} message_body_t, *pmessage_body_t;


typedef struct _queue_message_body_t
{
	long msgType;
	message_body_t msg;
} queue_message_body_t;

//Business event registration
void event_trans_register(uint32 msg_id);
//UI Event Registration
void event_ui_register(uint32 msg_id);

//Thread lock operation interface
void event_thread_mutex(Switch_t status);
bool event_thread_mutex_status();

//Status bar interface
void updateStatusBarIcon(lv_timer_t * timer);
void suspendStatusBarIcon(void);
void resumeStatusBarIcon(void);

//Creating UI interface
void lvgl_MainMenu(void);

void ui_create_enter_amount();

void ui_create_select_pay_type();

void ui_create_read_card();

void ui_read_ic_card();

void set_fail_msg(const char * format, ...);

void ui_create_payresult_fail();

void ui_create_payresult_success();

void ui_create_pay_process();

void ui_create_enter_pin();

void ui_create_qrcode();

void ui_create_signature();

void ui_create_print();

void ui_create_printing();

void ui_create_waters();

void ui_create_details();

void ui_create_filter();

void ui_create_setting();

void ui_create_setting_network();

void ui_create_setting_backlight();

void ui_create_setting_systemtime();

void ui_create_setting_year();

void ui_create_setting_date();

void ui_create_setting_time();

void ui_create_about();

void ui_create_ad();

void ui_create_scan_code();

void ui_create_remove_card();
void ui_create_nfc_retap();
void ui_create_see_phone();
void ui_create_ota_check();
void  ui_create_disp_common_msg();
void ui_create_wifi_connect();
void ui_create_wifi_select();
void ui_create_wifi_list();
void ui_create_wifi_search();
void ui_create_multiapp_sel();
void ui_create_enter_online_pin();
void ui_create_enter_offline_pin();
void ui_online_process();
void ui_create_apn_set();
void ui_create_apn_username();
void ui_create_apn_userpwd();
#ifdef __cplusplus
}
#endif

#endif