/*-----------------------------------------------------------------------------
|   Includes
+----------------------------------------------------------------------------*/
#include "appinc.h"

int screen;

void event_trans_handle(uint32 event_trans_id)
{
    switch(event_trans_id)
    {
        case EVENT_PAYMENT:
			sale_init();
            break;
		case EVENT_READCARD:
			read_cards_process();
            break;
		case EVENT_PRINT:
			start_print();
            break;
		case EVENT_PRINT_RECORD:
			start_print_record();
            break;
		case EVENT_RECORD_TOTAL:
			TranRecord_ReadTradeTotal();
			break;
		case EVENT_WIFI_SEARCH:
			wifi_search();
			break;
		case EVENT_WIFI_CONNECT:
			wifi_connect();
			break;
		case EVENT_APN_SET:
			apn_cfg();
			break;	
		case EVENT_OTA_CHECK:
			ota_process();
			break;			
        default:
			OsLog(LOG_DEBUG,"EVENT TRANS NOT IMPLEMENTED YET [%d]", event_trans_id );
            break;
    }
}


void event_ui_handle(uint32 event_ui_id){
	OsLog(LOG_DEBUG,"event_ui_handle id [%d]", event_ui_id );
    switch(event_ui_id){
		case UI_IDLE:
			lvgl_MainMenu();
			break;
		case UI_ENTER_AMOUNT:
			ui_create_enter_amount();
			break;
		case UI_SELECT_PAYTYPE:
			ui_create_select_pay_type();
			break;
		case UI_READCARD:
			ui_create_read_card();
			break;
		case UI_READ_ICCARD:
			ui_read_ic_card();
			break;
		case UI_STOP_READCARD:
			stop_readcards();
            break;	
		case UI_ENTER_OFFLINE_PIN:
			ui_create_enter_offline_pin();
			break;	
		case UI_ENTER_ONLINE_PIN:
			ui_create_enter_online_pin();
			break;	
		case UI_MULTI_APP_SELECT:
			ui_create_multiapp_sel();
			break;	
		case UI_PROCESSING:
			ui_create_pay_process();
			break;	
		case UI_QRCODE:
			ui_create_qrcode();
			break;
		case UI_SCAN_CODE:
			ui_create_scan_code();
			break;
		case UI_SIGNATURE:
			ui_create_signature();
			break;	
		case UI_RESULT_SUCCESS:
			ui_create_payresult_success();
			break;	
		case UI_RESULT_FAIL:
			ui_create_payresult_fail();
			break;	
		case UI_PRINTER:
			ui_create_print();
			break;
		case UI_PRINTERING:
			ui_create_printing();
			break;
		case UI_WATERS:
			ui_create_waters();
			break;
		case UI_WATERS_DETAILS:
			ui_create_details();
			break;
		case UI_WATERS_FILTER:
			ui_create_filter();
			break;
		case UI_SETTING:
			ui_create_setting();
			break;
		case UI_SETTING_NETWORK:
			ui_create_setting_network();
			break;
		case UI_SETTING_BACKLIGHT:
			ui_create_setting_backlight();
			break;
		case UI_SETTING_SYSTIME:
			ui_create_setting_systemtime();
			break;
		case UI_ABOUT:
			ui_create_about();
			break;
		case UI_AD:
			ui_create_ad();
			break;
		case UI_WIFI_SEARCH:
			ui_create_wifi_search();
			break;	
		case UI_DISP_WIFI_LIST:
			ui_create_wifi_list();
			break;
		case UI_WIFI_SELECT:
			ui_create_wifi_select();
			break;	
		case UI_WIFI_CONNECT:
			ui_create_wifi_connect();
			break;
		case UI_APN_SET:
			ui_create_apn_set();
			break;			
		case UI_APN_USERNAME:
			ui_create_apn_username();
			break;
    	case UI_APN_UISERPWD:
			ui_create_apn_userpwd();
			break;
		case UI_DISP_COMMON_MSG:
			ui_create_disp_common_msg();
			break;	
		case UI_OTA_CHECK:
			ui_create_ota_check();
			break;	
		case UI_SEE_PHONE:
			ui_create_see_phone();
			break;
    	case UI_NFC_RETAP:
			ui_create_nfc_retap();
			break;
		case UI_REMOVE_CARD:
			ui_create_remove_card();
			break;	
		default:
			OsLog(LOG_DEBUG,"EVENT UI NOT IMPLEMENTED YET [%d]", event_ui_id );
			break;
	}
}

