#ifndef UI_BUSINESS_H
#define UI_BUSINESS_H

#include "typedefine.h"
#ifdef __cplusplus
extern "C" {
#endif

#define INPUT_NOT_READCARD   (0)     // No card read
#define INPUT_STRIPE        (1<<0)  // Card swipe
#define INPUT_INSERTIC      (1<<1)  // Card insertion
#define INPUT_RFCARD        (1<<2)  // Card waving
#define INPUT_QRCODE        (1<<3)  // Scanned
#define INPUT_SCANCODE      (1<<4)  // Scanned

typedef struct tagCardDataInfo
{
	ST_MSR_DATA ptTracker1;
	ST_MSR_DATA ptTracker2;
	ST_MSR_DATA ptTracker3;
	PR_INT8 CardSN[3+1];
	PR_INT8 Pan[19+1+1];		//card number
	PR_INT8 ExpDate[4+1];
    PR_INT8 ServiceCode[3+1];
    PR_INT8 CardCompany[3+1];	// Credit Card Company

}CardDataInfo;

typedef enum
{
// / Ordinary transactions
        TT_SALE=0,
		TT_SALE_EC_QPBOC,	  // Entered from shortcut menu 1, card reading process only supports electronic cash and QPOC processes
		TT_SALE_CREDIT_DEBIT, // Entered from shortcut menu 2, card reading process only supports marked debit and credit
        TT_QUERY,
        TT_REFUND,            // Refund
        TT_SALE_VOID,           // Void
        TT_AUTH,                // Authorization
        TT_AUTH_CANCEL,			// Authorization cancellation
        TT_AUTH_SETTLEMENT,		// Authorization completion (notification)
        TT_AUTH_COMPLETE,		// Authorization completion (request)
        TT_AUTH_COMPLETE_VOID,	// Authorization completion void
        TT_MAX,
        TT_INVALID = TT_MAX,
}TransactionType_T;

typedef struct tagTransactionData
{
	int nTransType;   						/* Transaction type */
    int nStatus;   						    /* Transaction status */
    char sOrderNo[32];                      /* Transaction order number: transaction time + serial number */
	int nPosEntryMode;						/* Card swipe type */
	char sTrace[6+1];					    /* POS serial number */
	char sBatch[6+1];					    //batch num
    
	char sCardNo[19+1];						/* Primary account number */
	char sAmount[12 + 1];			        /* Amount */
	char sTransTime[14 + 1];				/* Transaction time */
	char sExpDate[4 + 1];					/* Card expiration date */
    char sCardSN[3+1];
    char sServiceCode[3+1];
    char sCardCompany[3+1];             	// Credit Card Company
	char sPin[16];
    char sPinKsn[16];					    // Password
    char sTracker2[137 + 1];
	int nTracker2Len; 
    char signatureFlag;                 //check signature flag  1:check 0:no check
    

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