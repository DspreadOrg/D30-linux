#ifndef _TASK_HANDLE_H
#define _TASK_HANDLE_H

#include "appinc.h"

typedef enum
{
    EVENT_PAYMENT = 1,
    EVENT_READCARD,
    EVENT_PRINT,
    EVENT_PRINT_RECORD,
    EVENT_RECORD_TOTAL,
    EVENT_SETTING,
    EVENT_WIFI_SEARCH,
    EVENT_WIFI_CONNECT,
    EVENT_OTA_CHECK,
    EVENT_APN_SET,
}EVENT_TRANS;

typedef enum
{
    UI_IDLE = 1,
    UI_ENTER_AMOUNT,
    UI_SELECT_PAYTYPE,
    UI_READCARD,
    UI_READ_ICCARD,
    UI_STOP_READCARD,
    UI_ENTER_ONLINE_PIN,
    UI_ENTER_OFFLINE_PIN,
    UI_MULTI_APP_SELECT,
    UI_SEE_PHONE,   //10
    UI_NFC_RETAP,
    UI_REMOVE_CARD,
    UI_SIGNATURE,
    UI_PROCESSING,//14
    UI_QRCODE,
    UI_SCAN_CODE,
    UI_RESULT_FAIL, //17
    UI_RESULT_SUCCESS,
    UI_RESULT_TERMINAL,
    UI_RESULT_DENAIL,
    UI_RESULT_APP_BLOCK,
    UI_RESULT_FALL_BACK,
    UI_RESULT_NFC_TRY_AGAIN,
    UI_RESULT_NFC_TERMINAL, //20
    UI_RESULT_CARD_NOT_SUPPORT,
    UI_PRINTER,  //26
    UI_PRINTERING,
    UI_WATERS,
    UI_WATERS_DETAILS,
    UI_WATERS_FILTER,
    UI_SETTING,
    UI_SETTING_NETWORK,
    UI_SETTING_BACKLIGHT,
    UI_SETTING_SYSTIME,//30
    UI_SETTING_YEAR,
    UI_SETTING_DATE,
    UI_SETTING_TIME,
    UI_ABOUT,
    UI_AD,
    UI_WIFI_SEARCH,
    UI_DISP_WIFI_LIST,
    UI_WIFI_SELECT,
    UI_WIFI_CONNECT,
    UI_APN_SET,
    UI_APN_USERNAME,
    UI_APN_UISERPWD,
    UI_DISP_COMMON_MSG,
    UI_OTA_CHECK,
}EVENT_UI;

typedef enum KB_KEYVALUE
{
    KB_KEY_CANCEL = 0x01,
    KB_KEY_CLEAR = 0x0E,
    KB_KEY_ENTER = 0x1C,
    KB_KEY_0 = 0x30,
	KB_KEY_1,
	KB_KEY_2,
	KB_KEY_3,
	KB_KEY_4,
	KB_KEY_5,
	KB_KEY_6,
	KB_KEY_7,
	KB_KEY_8,
	KB_KEY_9,
	KB_KEY_POWER = 0x74,
    KB_KEY_DOWN = 0x197,
    KB_KEY_UP = 0x19C,
	KB_KEY_MAX
}KEY_VALUE;


#endif