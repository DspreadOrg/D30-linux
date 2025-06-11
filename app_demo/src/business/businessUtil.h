#ifndef UI_BUSINESS_UTILS_H
#define UI_BUSINESS_UTILS_H

#include "typedefine.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define PR_ARRAY_SIZE(x)	(sizeof(x)/sizeof(x[0]))
#define BUFCLR(Buf)      memset(Buf, 0, sizeof(Buf)) 

#define PARAM_ISFIRST_RUNNINT    (char*)"is_first_running" 
#define PARAM_TRADE_NUM          (char*)"trace_num" 
#define PARAM_BATCH_NUM          (char*)"batch_num" 
#define PARAM_EMV_TSC          (char*)"emv_tsc" 
#define PARAM_EMV_READCARD_FLAG    (char*)"emv_readcard_flag"    // 1: Register a callback to EMV, and EMV will callback the card reading interface; 0: External card detection, and the card type will be passed to EMV after the detection is completed.

// Write a record
PR_INT32 TranRecord_WriteAndUpdateTradeRec(TransactionData *pTransactionRec);
// Read records through index, start with index 0, and the index value is 0~the number of records in table -1
PR_INT32 TranRecord_ReadTradeByIndex(PR_INT32 nIndex,TransactionData *pTransactionRec);
// Read records through orderNo order number
PR_INT32 TranRecord_ReadTradeByOrder(char* pOrderNo,TransactionData *pTransactionRec);
// Get the number of records in the transaction table
PR_INT32 TranRecord_GetTradeCount();
// Clear transaction history
PR_INT32 TranRecord_ClearTraderec();

PR_INT32 MANAGE_GetSysParam(PR_INT8 *thePosParamKey, void* theBuffer, PR_UINT32 theBufferLength);

PR_INT32 MANAGE_SetSysParam(PR_INT8 *thePosParamKey, void* theData, PR_UINT32 theDataLength);

PR_INT32 MANAGE_GetIntSysParam(PR_INT8 *thePosParamIndex,void *intValue);

PR_INT32 MANAGE_SetIntSysParam(PR_INT8 *thePosParamIndex, PR_INT32 theIntValue);

PR_INT32 Business_GetTradeNum(PR_INT8 * szTrace,PR_INT32 nInBufLen);

PR_INT32 Business_GetBatchNum(PR_INT8 * szTrace,PR_INT32 nInBufLen);

PR_INT32 Business_GetTradeTsc(PR_INT8 * pTsc,PR_INT32 nInBufLen);

void Business_getAppPath(PR_INT8* pPath,PR_INT32 nBufLen);

PR_INT32 Business_getSysTimeStr(PR_INT8* pBuf,PR_INT32 nBufLen);

PR_INT32 Business_getSysTime(PR_INT8* pBuf,PR_INT32 nBufLen);

PR_INT32 Is_Valid_Network();

PR_INT32 PrintOrder(int nCount,TransactionData *pTxnRec);

void PlayKeyTone();

#ifdef __cplusplus
}
#endif
#endif