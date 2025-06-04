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

#ifndef _APP_SERVER_H
#define _APP_SERVER_H

/*-----------------------------------------------------------------------------
|   Includes
+----------------------------------------------------------------------------*/
#include "app_http.h"

#define URL_SERVER_PORT             (443)
#define ONLINE_REQUEST_TIMEOUT      (100000)
#define ONLINE_PACKET_TIMEOUT       (3000)
#define ONLINE_SEND_DATA_LEN        (512)
#define CFG_NET_CHECK_TICK          (50)
#define TLV_PARAM_DATA_MAX          (512)
#define FIRMWARE_REQUEST_LENGTH     (3200)
#define SERVER_REQUEST_BODY_LENGTH  (512)
#define SERVER_REQUEST_LENGTH       (1024)
#define SERVER_RECV_LENGTH          (4096)

typedef struct {
    char* data;
    size_t length;
} Buffer;

/*-----------------------------------------------------------------------------
|   Enumerations
+----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
|   prototypes
+----------------------------------------------------------------------------*/
Rc_t RequestPackHttpFrame(char * strMsg, char * pURL, char * pMethod,char * pFrame,char * pRange, u32 FrameLengthMax );
s32 RequestHttpTransmit(char * pSend,u32 SendLen, char * pURL,u32 port, char * pReceived, u32 FrameLengthMax);
u32 GetHttpHeadFramLen(const char *strMsg);
Rc_t UpdateKeyDataToTLV(char * strMsg, char * strKey, u32 Tag, pu8 pTradingFile );
Rc_t UpdateKeyDataToTLV_AndCompare(char * strMsg, char * strKey, u32 Tag, pu8 pTradingFile, char * strCompareValue );
Rc_t KeyDataCompare(char * strMsg, char * strKey,const char * strCompareValue);
Rc_t HttpLineStatusCheck(pu8 pTradingFile, const char * pHttpBody);
#endif

