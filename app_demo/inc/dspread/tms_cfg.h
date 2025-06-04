#ifndef _TMS_CFG_H
#define _TMS_CFG_H

/*
 * -----------------------------------------------------------------------------
 * |   Includes
 * +----------------------------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * |   Macros
 * +----------------------------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * |   Typedefs
 * +----------------------------------------------------------------------------
 */

enum TMS_ERR_RCODE {
    TMS_ERR_OK        =  0,
    TMS_ERR_PARAM     = -1,
    TMS_ERR_CONNECT   = -2,
    TMS_ERR_NO_AVAIALABLE = -3,
    TMS_ERR_SN_NOT_REGISTER = -4,
};


typedef enum
{
    TMS_DISP_START_CHECK = 0,
    TMS_DISP_START_DOWNLOAD,
    TMS_DISP_DOWNLOADING_PROGRESS,
    TMS_DISP_UPGRADING,
}TMS_DISP_STEP;

typedef struct _LarkTmsCallBack_t
{	
    s32 (*ssl_connect)(char * pHost,ssize_t port,int timeout);
    void (*ssl_disconnect)();
    s32 (*ssl_send)(char *  pData, ssize_t DataLength, int Timeout );
    s32 (*ssl_recv)(char * recvBuf, ssize_t recvlen, int timeout);
}LarkTmsCallBack_t, *pLarkTmsCallBack_t;

extern void set_user_appver(char * ver);
extern int tms_client_check(const char *szUrl);
extern int tms_client(const char *szUrl, bool isForce);
extern void Tms_Disp_Callback_Register(void (*Tms_disp)(unsigned long, unsigned long, unsigned long, void*));

extern void LarkTms_Disp_Callback_Register(void (*Tms_disp)(unsigned int, void*));
extern int larktms_client_check(const char *szUrl,char *version);
extern int larktms_client(const char *szUrl,char *version);

extern int larktms_ssl_Init(pLarkTmsCallBack_t LarkTmsCb);
#endif
