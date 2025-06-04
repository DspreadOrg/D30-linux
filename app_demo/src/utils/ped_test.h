#ifndef _PED_TEST_H_
#define _PED_TEST_H_

#include "appinc.h"

#ifdef __cplusplus
extern "C"
{
#endif

extern int Ped_Dukpt_Init();
extern void dukpt_test();
extern void pinpadDupktTest1();
extern void pinpadMKSKTest();
PR_INT32 ped_write_ipek(PR_UINT8 SrcKeyType,PR_UINT8 SrcKeyIdx,PR_UINT8 DstKeyType,PR_UINT8 DstKeyIdx,
					PR_UINT8 *pKey,PR_INT32 keyLen,PR_UINT8 *pKcv,PR_INT32 kcvLen,PR_UINT8 *pKsn);
#ifdef __cplusplus
}
#endif
#endif 
