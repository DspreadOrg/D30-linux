#ifndef UI_RECORD_H
#define UI_RECORD_H

#include "typedefine.h"
#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    char sOrderNo[32+1];                       // db记录索引 用于对应查找详细流水文件
    char sYear[8+1];                       	// YY/MM/DD
	char sAmt[16+1];						// currenry + number
	char szTransDesc[32+1];					// PAN-sOrderNo-HH:MM AM/PM
    char szTransName[16+1];                 // 交易名称
	char szPaymentOrg;						//1.visa,2.master,3.union,4.jcb,5.amx,6.diners
}ST_WATER_BRIEF;

extern ST_WATER_BRIEF stWaterList[100];
extern TransactionData stTransactionRecord;

void TranRecord_set_time_limit(int second);
void TranRecord_set_type_limit(int type);
extern void TranRecord_ReadTradeTotal(void);

#ifdef __cplusplus
}
#endif

#endif