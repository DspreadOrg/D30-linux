#ifndef EMV_IMPL_H
#define EMV_IMPL_H

#include "appinc.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define PED_PIN_IPEK_INDEX 0x01
#define PED_EMV_IPEK_INDEX 0x02
#define PED_TRK_IPEK_INDEX 0x03

PR_INT32 EmvL2_Init();

PR_INT32 EmvL2_Proc(EmvTransParams_t emvTransParams);

void EmvL2_SetDefaultAidAndCapk();

PR_INT32 Emv_GetPanByTag5A(char* pan);

PR_INT32 Emv_GetCardInfo(TransactionData *pEmvTransData);

#ifdef __cplusplus
}
#endif

#endif