/*
********************************************************************************
*
*   File Name:
*       app_server.h
*   Author:
*       SW R&D Department
*   Version:
*       V1.0
*   Description:
*
*
********************************************************************************
*/

#ifndef _APP_REQIUEST_H
#define _APP_REQIUEST_H

#include "appinc.h"

/********GLOBLE TAG******/
#define URL_TEST    "https://www.baidu.com/port/test"

#define SIZE_ARRAY(x)  						(sizeof(x)/sizeof(*x))

typedef struct{
	int Rc;
	int error_code;
	char title_msg[64+1];
	char description[64+1];
	char authorization_code[32+1];		
}ONLINE_STATUS;


Rc_t online_common(pu8 pTradingFile);

#endif