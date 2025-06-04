/*
 * File Name:
 * lib_fun.h
 * Author:
 * jiaxin
 * Version:
 * V1.0
 * Description:
 */

#ifndef _LIB_FUN_H
#define _LIB_FUN_H

#ifdef __cplusplus
extern "C"
{
#endif//__cplusplus

/*
 * -----------------------------------------------------------------------------
 * |   Includes
 * +----------------------------------------------------------------------------
 */
#include "app_emv.h"
#include "appinc.h"

#if 0
#define CFG_LIB_DEBUG
#endif

#if 0
#define CFG_CARD_SIMULATION
#endif
/*
 * -----------------------------------------------------------------------------
 * |   Typedefs
 * +----------------------------------------------------------------------------
 */

typedef struct _applications
{
    unsigned int  aidLen;
    unsigned char  aid[32];
    unsigned int  appLabelLen;
    unsigned char  appLabel[32];
}s_AppInfo_t, *ps_AppInfo_t;


/*
 * EMVApplication Strcut
 */
typedef struct emv_application{
    unsigned int appnum;
    ps_AppInfo_t pAppinfo;
}s_EMV_AppList_t, *ps_EMV_AppList_t;

typedef enum
{
	EMV_PIN_TYPE_START = 0,

	EMV_PIN_TYPE_ONLINE,
	EMV_PIN_TYPE_OFFLINE,
	EMV_PIN_TYPE_LAST_OFFLINE,
	EMV_PIN_TYPE_MANAGE,
	EMV_PIN_TYPE_REINPUT,
	EMV_PIN_TYPE_INPUT_NEWPIN,
	EMV_PIN_TYPE_REINPUT_NEWPIN,
	EMV_PIN_TYPE_INPUT_OFFLINE_NEWPIN,
	EMV_PIN_TYPE_REINPUT_OFFLINE_NEWPIN,

	EMV_PIN_TYPE_NUMS,
}Emv_Pin_Type_t;


typedef enum{
    PIN_CONFIRM = 0,
    PIN_CANCEL,
    PIN_TIMEOUT,
    PIN_BYPASS,
}e_InPutPin_t;

/*
 * EMV_Transaction Callback
 */
typedef struct EMV_CallbackFunc
{
/*
 * data
 */
    void (*dispmsg_cb)(pmessage_body_t pmsg); 
    void (*emv_selectapp_cb)(unsigned char* appList);
    unsigned int (*getcardinfo_cb)(pu8 pTlvInBuf, pu8 pTlvOutBuf);
    void (*maskPinDisp_cb)(unsigned char *maskPin);
// u32 (emv_onlineprocess_cb)(ps_EMV_onlineReqProcess_t online_req, ps_EMV_OnlineResult_t online_rsp);
    void (*onlineprocess_cb)(pu8 pTlvOnlineReqData);
    void (*traderesult_cb)(App_Rc_Code_t reCode);
    int  (*pinEnter_cb)(Emv_Pin_Type_t pinType,unsigned char *pan,unsigned char *ksn,unsigned char *pinBlock);
}s_Trans_CallbackFunc_t, *ps_Trans_CallbackFunc_t;


extern u32 get_tlv_value(pu8 pool,u32 T,pu8 V );
extern void set_tlv_view(u8 *pTlvBuf, u32 dwTag, T_U8_VIEW uvVal);
extern T_U8_VIEW get_tlv_view(u8 *pTlvBuf, u32 dwTag);
extern int _asc_2_bcd(unsigned char * ascstr, int asclen, unsigned char * bcdstr, int align);
extern u32 asc_to_bcd(pu8 src,u32 len,pu8 dest );

extern u32 WaitEvents(u32 events,u32 timeout,pvoid pbuffer );
extern void emv_trans_cb_init(ps_Trans_CallbackFunc_t emv_trans_cb);

#ifdef __cplusplus
}
#endif//__cplusplus

#endif  /* #ifndef _TRANS_CBK_H */