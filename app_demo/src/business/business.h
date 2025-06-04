#ifndef UI_BUSINESS_H
#define UI_BUSINESS_H

#include "typedefine.h"
#ifdef __cplusplus
extern "C" {
#endif

#define INPUT_NOT_READCARD   (0)     //未读卡
#define INPUT_STRIPE        (1<<0)  //刷卡
#define INPUT_INSERTIC      (1<<1)  //插卡
#define INPUT_RFCARD        (1<<2)  //挥卡
#define INPUT_QRCODE        (1<<3)  //被扫
#define INPUT_SCANCODE      (1<<4)  //被扫

typedef struct tagCardDataInfo
{
	ST_MSR_DATA ptTracker1;
	ST_MSR_DATA ptTracker2;
	ST_MSR_DATA ptTracker3;
	PR_INT8 CardSN[3+1];
	PR_INT8 Pan[19+1+1];		//主账号
	PR_INT8 ExpDate[4+1];
    PR_INT8 ServiceCode[3+1];
    PR_INT8 CardCompany[3+1];	//信用卡公司

}CardDataInfo;

typedef enum
{
// / Ordinary transactions
        TT_SALE=0,
		TT_SALE_EC_QPBOC,	  // 快捷键菜单1进入，读卡流程只支持电子现金与QPOC流程
		TT_SALE_CREDIT_DEBIT, // 快捷键菜单2进入，读卡流程只支持标志借贷记
        TT_QUERY,
        TT_REFUND,            // 退货
        TT_SALE_VOID,           // 撤销
        TT_AUTH,                // 预授权
        TT_AUTH_CANCEL,			//预授权撤销
        TT_AUTH_SETTLEMENT,		//预授权完成（通知）
        TT_AUTH_COMPLETE,		//预授权完成（请求）
        TT_AUTH_COMPLETE_VOID,	//预授权完成撤销
        TT_MAX,
        TT_INVALID = TT_MAX,
}TransactionType_T;

typedef struct tagTransactionData
{
	int nTransType;   						/*交易类型,*/
    int nStatus;   						    /*交易状态*/
    char sOrderNo[32];                      /*交易订单号  交易时间+流水*/
	int nPosEntryMode;						/*刷卡类型*/
	char sTrace[6+1];					    /*POS流水号*/
	char sBatch[6+1];					    //批次号
    
	char sCardNo[19+1];						/*主帐号*/
	char sAmount[12 + 1];			        /*金额*/
	char sTransTime[14 + 1];				/*交易时间*/
	char sExpDate[4 + 1];					/*卡有效期*/
    char sCardSN[3+1];
    char sServiceCode[3+1];
    char sCardCompany[3+1];             	//信用卡公司
	char sPin[16];
    char sPinKsn[16];					    //密码
    char sTracker2[137 + 1];
	int nTracker2Len; 
    char signatureFlag;                 //是否电子签名表示 1电子签名，0未签名
    int emv_emter_online_pin_result;
    int emv_enter_offline_pin_result;
    int emv_multi_app_select_result;
    int icc_type;
}TransactionData;

// Transaction data settings
void clear_transaction_data();
TransactionData* get_transaction_data();
// 
void sale_process();

void read_cards_process();

void stop_readcards();

void start_print();

void start_print_record();
#ifdef __cplusplus
}
#endif

#endif