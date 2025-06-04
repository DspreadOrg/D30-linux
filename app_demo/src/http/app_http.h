/*
********************************************************************************
*
*   File Name:
*       app_Http.h
*   Author:
*       SW R&D Department
*   Version:
*       V1.0
*   Description:
*
*
********************************************************************************
*/

#ifndef _APP_HTTP_H
#define _APP_HTTP_H

/*-----------------------------------------------------------------------------
|   Includes
+----------------------------------------------------------------------------*/
#include "appinc.h"
/*-----------------------------------------------------------------------------
|   Macros
+----------------------------------------------------------------------------*/
#define HTTP_REQUEST_FRAME_LENGTH_MAX        (512)
#define HTTP_RESPONSE_FRAME_LENGTH_MAX       (1024)
#define HTTP_FIRMWARE_FRAME_LENGTH_MAX       (4096)

/*-----------------------------------------------------------------------------
|   Enumerations
+----------------------------------------------------------------------------*/



/*-----------------------------------------------------------------------------
|   Typedefs
+----------------------------------------------------------------------------*/

typedef enum{
	TCP,
	HTTP,
	HTTPS,
	UNKNOWN_PROTO
} T_WAN_PROTOCOL;

typedef struct {
		T_WAN_PROTOCOL eProtocol;
		T_U8_VIEW uvSite;
		u16 wPort;
		T_U8_VIEW uvPath;
} T_URL_INFO;

typedef struct
{
    char * pMethod;
    char * pUrl;
    char * pVersion;
    char * pStatus;
    char * pStatusDescription;
}HttpLine_t,*pHttpLine_t;

typedef struct
{
    char * pAccept;
    char * pAcceptCharset;
    char * pAcceptEncoding;
    char * pAcceptLanguage;
    char * pAuthorization;
    char * pContentType;
    char * pContentEncoding;
    char * pContentLength;
    char * pHost;
    char * pIfModifiedSince;
    char * pReferer;
    char * pUserAgent;
    char * pCookie;
    char * pPragma;
    char * pFrom;
    char * pConnection;
    char * pRange;
    char * pUA_Pixels;
    char * pUA_Color;
    char * pUA_OS;
    char * pUA_CPU;
    char * pDate;
    char * pServer;
    char * pVary;
    char * pKeepAlive;
    char * PDisposition;
    char * pTotalLength;
    char * pHash;
}HttpHead_t,*pHttpHead_t;

typedef struct
{
    char * pRequestBody;
    HttpLine_t Line;
    HttpHead_t Head;
    char * pRequestMessage;
}HttpPack_t,*pHttpPack_t;

typedef struct
{
	const char * pHost;
	const char * pMessage;
	unsigned int Port;
}SslRequest_t,*pSslRequest_t;

typedef struct
{
	const char * pHost;
	const char * pMessage;
	unsigned int Port;
}SocRequest_t,*pSocRequest_t;

typedef struct
{
    char * pResponseBody;
	HttpLine_t Line;
	HttpHead_t Head;
	char * pResponseParam;
}HttpUnpack_t,*pHttpUnpack_t;

/*-----------------------------------------------------------------------------
|   prototypes
+----------------------------------------------------------------------------*/
extern int HttpUrlGetHost(char * strMsg, char * pValue, unsigned int ValueLenMax);
extern int HttpUrlGetScheme(char * strMsg, char * pValue, unsigned int ValueLenMax);
extern int HttpUrlGetPath(char * strMsg, char * pValue, unsigned int ValueLenMax);
extern int HttpFramePack(pHttpPack_t pHttpRequest);
extern int HttpFrameUnpack(pHttpUnpack_t pHttpUnpack);
extern int HttpGetStrKey(char * strMsg, char * strKey, char * pValue, unsigned int ValueLenMax);
#endif

