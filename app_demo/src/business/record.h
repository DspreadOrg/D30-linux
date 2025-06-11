#ifndef UI_RECORD_H
#define UI_RECORD_H

#include "typedefine.h"
#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    char sOrderNo[32+1];                       // Database record index, used to find detailed transaction files.
    char sYear[8+1];                       	// YY/MM/DD
	char sAmt[16+1];						// Currency + number
	char szTransDesc[32+1];					// PAN-sOrderNo-HH:MM AM/PM
    char szTransName[16+1];                 // Transaction name
	char szPaymentOrg;						// 1. Visa, 2. MasterCard, 3. UnionPay, 4. JCB, 5. American Express, 6. Diners Club
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