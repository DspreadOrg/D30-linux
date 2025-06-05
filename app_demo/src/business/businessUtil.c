#include "appinc.h"

// Parameter record table
#define DB_PARAM "ParamTbl"
#define PARAM_MAX_LEN 128

// Transaction record table
#define DB_TXNRECODE  "RecordTbl"

typedef struct _ParamRecord
{
	PR_INT8 ID[32];
    PR_INT32 nParamLen;
	PR_INT8 Paramdata[PARAM_MAX_LEN];               
}ParamRecord;

#define PARAM_REC_LENGTH sizeof(ParamRecord) 

PR_INT32 TranRecord_WriteAndUpdateTradeRec(TransactionData *pTransactionRec)
{
	PR_INT32 nRet = PR_FAILD;
    TransactionData temRec;
    HANDLE ptTable = PR_NULL;

    if(pTransactionRec == NULL || strlen(pTransactionRec->sOrderNo) <= 0){
        DSP_Info("TxnRecord is emopty!");
        return PR_FAILD;
    }
	do
	{
		
        memset(&temRec,0x0,sizeof(temRec));
		ptTable = DB_hOpen(DB_TXNRECODE,sizeof(TransactionData));

		if(ptTable == PR_NULL)
		{
			DSP_Info("Open TxnRecord Table fail!");
			nRet = PR_FAILD;
			break;
		}
        if(0 >= DB_nReadRecByKey(ptTable, pTransactionRec->sOrderNo, sizeof(TransactionData), (PR_INT8 *)&temRec)){
// Add a record
            PR_UINT32 nWritenLen = DB_nAppendRecByKey(ptTable, pTransactionRec->sOrderNo,(PR_INT8 *)pTransactionRec, sizeof(TransactionData));
            if(nWritenLen <= 0)
            {
                DSP_Info("sOrderNo:%s Errcode:%d",pTransactionRec->sOrderNo,DB_nGetLastError(ptTable));
                nRet = PR_FAILD;
                break;
            }else{
                nRet = PR_NORMAL;
                break;
            }
        }
// Transaction record storage replacement
        if(0 >= DB_nReplaceRecByKey(ptTable, pTransactionRec->sOrderNo, sizeof(TransactionData),(PR_INT8 *)pTransactionRec))
        {
            DSP_Info("sOrderNo:%s Errcode:%d",pTransactionRec->sOrderNo,DB_nGetLastError(ptTable));
            nRet = PR_FAILD;
            break;
        }
		nRet = PR_NORMAL;
	}while(0);

    if(ptTable != NULL){
        DB_vClose(ptTable);
    }
	return nRet;
}

// nIndex starts at 0
PR_INT32 TranRecord_ReadTradeByIndex(PR_INT32 nIndex,TransactionData *pTransactionRec)
{
	HANDLE ptTable = PR_NULL;

    if(pTransactionRec == NULL){
        DSP_Info("Params is error!");
        return PR_FAILD;
    }
	memset(pTransactionRec, 0, sizeof(TransactionData));
	ptTable = DB_hOpen(DB_TXNRECODE,sizeof(TransactionData));
	if(ptTable == PR_NULL)
	{
		DSP_Info("Open TxnRecord Table fail!");
		return PR_FAILD;
	}

	if(nIndex > DB_nGetRecordSUM(ptTable))
	{
		DSP_Info("the index: %d > the record sum : %d", nIndex, DB_nGetRecordSUM(ptTable));
		DB_vClose(ptTable);
		return PR_FAILD;
	}
	if(DB_nReadRec(ptTable, nIndex, sizeof(TransactionData),(PR_INT8 *)pTransactionRec) <= 0)
	{
		DB_vClose(ptTable);
		DSP_Info("Read TxtRec Failed：%d", nIndex);
		return PR_FAILD;
	}
	DB_vClose(ptTable);
	return PR_NORMAL;
}

PR_INT32 TranRecord_ReadTradeByOrder(char* pOrderNo,TransactionData *pTransactionRec)
{
	HANDLE ptTable = PR_NULL;

    if(pOrderNo == NULL || strlen(pOrderNo) <= 0 || pTransactionRec == NULL){
        DSP_Info("Params is error!");
        return PR_FAILD;
    }
	memset(pTransactionRec, 0, sizeof(TransactionData));
	ptTable = DB_hOpen(DB_TXNRECODE,sizeof(TransactionData));
	if(ptTable == PR_NULL)
	{
		DSP_Info("Open TxnRecord Table fail!");
		return PR_FAILD;
	}

	if(DB_nReadRecByKey(ptTable, pOrderNo, sizeof(TransactionData),(PR_INT8 *)pTransactionRec) <= 0)
	{
		DB_vClose(ptTable);
		DSP_Info("Read TxtRec Failed：%s", pOrderNo);
		return PR_FAILD;
	}
	DB_vClose(ptTable);
	return PR_NORMAL;
}

PR_INT32 TranRecord_GetTradeCount()
{
	PR_INT32 nRet = PR_FAILD;
	HANDLE ptTable = PR_NULL;

	ptTable = DB_hOpen(DB_TXNRECODE,sizeof(TransactionData));
	if(ptTable == PR_NULL)
	{
		DSP_Info("Open TxnRecord Table fail!");
		return PR_FAILD;
	}
	nRet = DB_nGetRecordSUM(ptTable);
	DB_vClose(ptTable);
	return nRet;
}

PR_INT32 TranRecord_ClearTraderec()
{
	PR_INT32 nRet = PR_FAILD;
	HANDLE ptTable = PR_NULL;

	ptTable = DB_hOpen(DB_TXNRECODE,sizeof(TransactionData));
	if(ptTable == PR_NULL)
	{
		DSP_Info("Open TxnRecord Table fail!");
		return PR_FAILD;
	}
	nRet = (DB_bClear(ptTable) == PR_TRUE)?PR_NORMAL:PR_FAILD;
	DB_vClose(ptTable);
	return nRet;
}


PR_INT32 MANAGE_GetSysParam(PR_INT8 *thePosParamKey, void* theBuffer, PR_UINT32 theBufferLength)
{
	HANDLE ptTable = PR_NULL;
	int errcode;

	ptTable = DB_hOpen(DB_PARAM,PARAM_REC_LENGTH);
	if(ptTable == PR_NULL)
	{
		OsLog(LOG_ERROR,"DB_PARAM Open Failed");
		return PR_FAILD;
	}
	ParamRecord record;
    memset(&record,0x0,sizeof(ParamRecord));
	if(sizeof(ParamRecord) != DB_nReadRecByKey(ptTable, thePosParamKey, sizeof(ParamRecord), (PR_INT8 *)&record))
	{
		errcode = DB_nGetLastError(ptTable);
		OsLog(LOG_ERROR,"Param ID:%s Errcode:%d",thePosParamKey,errcode);
		DB_vClose(ptTable);
		if(errcode == RET_INTERFACE_NOT_REC){
			return PR_NOT_REC;
		} 
		return PR_FAILD;
	}
	if(theBufferLength < record.nParamLen)
	{
		OsLog(LOG_ERROR,"Param ID:%s The Buffer Length Small ",thePosParamKey);
		DB_vClose(ptTable);
		return PR_FAILD;
	}
	memcpy(theBuffer,record.Paramdata,record.nParamLen);
	DB_vClose(ptTable);

	return PR_NORMAL;
}

PR_INT32 MANAGE_SetSysParam(PR_INT8 *thePosParamKey, void* theData, PR_UINT32 theDataLength)
{
	HANDLE ptTable = PR_NULL;
	int errcode;

    ptTable = DB_hOpen(DB_PARAM,PARAM_REC_LENGTH);
	if(ptTable == PR_NULL)
	{
		OsLog(LOG_ERROR,"DB_PARAM Open Failed");
		return PR_FAILD;
	}

	ParamRecord record;
    memset(&record,0x0,sizeof(ParamRecord));
	if(0 >= DB_nReadRecByKey(ptTable, thePosParamKey, sizeof(ParamRecord), (PR_INT8 *)&record))
	{
		errcode = DB_nGetLastError(ptTable);
		if(errcode != RET_INTERFACE_NOT_REC){
			OsLog(LOG_ERROR,"Param ID:%s Errcode:%d",thePosParamKey,DB_nGetLastError(ptTable));
			DB_vClose(ptTable);
			return PR_FAILD;
		}
// Append
		memset(&record,0x0,sizeof(ParamRecord));
		memcpy(record.ID,thePosParamKey, strlen(thePosParamKey));
		memcpy(record.Paramdata,theData, theDataLength);
		record.nParamLen = theDataLength;

		PR_UINT32 nWritenLen = DB_nAppendRecByKey(ptTable, thePosParamKey,(PR_INT8 *)&record, sizeof(ParamRecord));
		if(nWritenLen <= 0)
		{
			OsLog(LOG_ERROR,"Param ID:%s Errcode:%d",thePosParamKey,DB_nGetLastError(ptTable));
			DB_vClose(ptTable);
			return PR_FAILD;
		}
		DB_vClose(ptTable);
		return PR_NORMAL;
	}
// Replace
	if(theDataLength > PARAM_MAX_LEN)
	{
		DB_vClose(ptTable);
		OsLog(LOG_ERROR,"Param ID:%s The DataLength > Max Length",thePosParamKey);
		return PR_FAILD;
	}
	memset(record.Paramdata,0x0,sizeof(record.Paramdata));
	memcpy(record.Paramdata,theData, theDataLength);
	record.nParamLen = theDataLength;
	if(0 >= DB_nReplaceRecByKey(ptTable, thePosParamKey, sizeof(record),(PR_INT8 *)&record))
	{
		OsLog(LOG_ERROR,"Param ID:%s Errcode:%d",thePosParamKey,DB_nGetLastError(ptTable));
		DB_vClose(ptTable);
		return PR_FAILD;
	}
	DB_vClose(ptTable);

	return PR_NORMAL;
}

PR_INT32 MANAGE_GetIntSysParam(PR_INT8 *thePosParamIndex,void *intValue)
{
	PR_INT32 nRet = PR_FAILD ;

	nRet = MANAGE_GetSysParam(thePosParamIndex, intValue, sizeof(PR_INT32));
	return nRet;
}
PR_INT32 MANAGE_SetIntSysParam(PR_INT8 *thePosParamIndex, PR_INT32 theIntValue)
{
	PR_INT32 nRet = PR_FAILD ;

	nRet = MANAGE_SetSysParam(thePosParamIndex, &theIntValue, sizeof(PR_INT32));
	return nRet;
}

PR_INT32 Business_GetTradeNum(PR_INT8 * szTrace,PR_INT32 nInBufLen)
{
	PR_INT32 nWaterNo;
    PR_INT32 nWaterNoNew;
    PR_INT32 ret = PR_FAILD;
	PR_UINT8 szBuffer[8];
    
    if(nInBufLen < 6){
         return PR_FAILD;
    }
    BUFCLR(szBuffer);
    if (PR_NORMAL != MANAGE_GetSysParam(PARAM_TRADE_NUM, szBuffer,sizeof(szBuffer)))
    {
// No flow number exists
        nWaterNo = 1;
    }else{
		nWaterNo = atoi((PR_INT8 *)szBuffer);
		if (nWaterNo > 999999 || nWaterNo == 0)
		{
			nWaterNo = 1;
		}
	}
    
    nWaterNoNew = nWaterNo+1;
    if (nWaterNoNew > 999999)
    {
        nWaterNoNew = 1;
    }
    BUFCLR(szBuffer);
    sprintf((char *)szBuffer, "%06d",nWaterNoNew);
    if(PR_NORMAL != MANAGE_SetSysParam(PARAM_TRADE_NUM, szBuffer,strlen((char*)szBuffer))){
        DSP_Info("nWaterNoNew %d write err!",nWaterNoNew);
        return PR_FAILD;
    }
	DSP_Info("cur  nWaterNo %d",nWaterNo);
	sprintf(szTrace, "%06d", nWaterNo);
	return PR_NORMAL;
}


PR_INT32 Business_GetBatchNum(PR_INT8 * szTrace,PR_INT32 nInBufLen)
{
	PR_INT32 nWaterNo;
    PR_INT32 ret = PR_FAILD;
	PR_UINT8 szBuffer[8] = {0};
    
    if(nInBufLen < 6){
         return PR_FAILD;
    }
    BUFCLR(szBuffer);
    if (PR_NORMAL != MANAGE_GetSysParam(PARAM_BATCH_NUM, szBuffer,sizeof(szBuffer)))
    {
        nWaterNo = 1;
		BUFCLR(szBuffer);
		sprintf((char *)szBuffer, "%06d",nWaterNo);
		if(PR_NORMAL != MANAGE_SetSysParam(PARAM_BATCH_NUM, szBuffer,strlen((char*)szBuffer))){
			DSP_Info("nWaterNo %d write err!",nWaterNo);
			return PR_FAILD;
		}
    }else{
		nWaterNo = atoi((PR_INT8 *)szBuffer);
		if (nWaterNo > 999999 || nWaterNo == 0)
		{
			nWaterNo = 1;
			BUFCLR(szBuffer);
			sprintf((char *)szBuffer, "%06d",nWaterNo);
			if(PR_NORMAL != MANAGE_SetSysParam(PARAM_BATCH_NUM, szBuffer,strlen((char*)szBuffer))){
				DSP_Info("nWaterNo %d write err!",nWaterNo);
				return PR_FAILD;
			}
		}
	}
	DSP_Info("cur batch %d",nWaterNo);
	sprintf(szTrace, "%06d", nWaterNo);
	return PR_NORMAL;
}

PR_INT32 Business_GetTradeTsc(PR_INT8 * pTsc,PR_INT32 nInBufLen)
{
	PR_INT32 nWaterNo;
    PR_INT32 nWaterNoNew;
    PR_INT32 ret = PR_FAILD;
	PR_UINT8 szBuffer[8];
    
    if(nInBufLen < 4){
         return PR_FAILD;
    }
    BUFCLR(szBuffer);
    if (PR_NORMAL != MANAGE_GetSysParam(PARAM_EMV_TSC, szBuffer,sizeof(szBuffer)))
    {
// tsc does not exist
        nWaterNo = 1;
    }else{
		nWaterNo = atoi((PR_INT8 *)szBuffer);
		if (nWaterNo > 99999999 || nWaterNo == 0)
		{
			nWaterNo = 1;
		}
	}
    
    nWaterNoNew = nWaterNo+1;
    if (nWaterNoNew > 99999999)
    {
        nWaterNoNew = 1;
    }
    BUFCLR(szBuffer);
    sprintf((char *)szBuffer, "%08d",nWaterNoNew);
    if(PR_NORMAL != MANAGE_SetSysParam(PARAM_EMV_TSC, szBuffer,strlen((char*)szBuffer))){
        DSP_Info("nWaterNoNew %d write err!",nWaterNoNew);
        return PR_FAILD;
    }
	DSP_Info("cur TSC%d",nWaterNo);
	BUFCLR(szBuffer);
	sprintf((char*)szBuffer, "%08d", nWaterNo);
	PR_nUtilAsc2Bcd(8,(char*)szBuffer,EM_ALIGN_RIGHT,4,(PR_INT8*)pTsc);
	return PR_NORMAL;
}

// Get the app installation directory
void Business_getAppPath(PR_INT8* pPath,PR_INT32 nBufLen){
	getcwd(pPath, nBufLen);
}

PR_INT32 Is_Valid_Network(){
	PR_INT8 ssid[256] = {0};
    PR_INT8 bssid[20] = {0};
    PR_INT32 rssi = 0;
	PR_INT32 ret;

	OsWlLock();
    if(OsWlCheck() == RET_OK){
		return PR_NORMAL;
	}
    OsWifiOpen();
	ret = OsWifiCheck(ssid,bssid,&rssi);
    if(ret == 0 && strlen(ssid) > 0){
        return PR_NORMAL;
    }

	return PR_FAILD;
}


PR_INT32 Business_getSysTimeStr(PR_INT8* pBuf,PR_INT32 nBufLen){
	ST_TIME Time;
	PR_INT8 tembuf[16] = {0};

	if(pBuf == NULL || nBufLen < 14){
		return PR_FAILD;
	}
	memset(&Time,0x0,sizeof(ST_TIME));
	OsGetTime(&Time);
	sprintf(tembuf,"%04d%02d%02d%02d%02d%02d",Time.Year,Time.Month,Time.Day,Time.Hour,Time.Minute,Time.Second);
	memcpy(pBuf,tembuf,14);
	return PR_NORMAL;
}

PR_INT32 Business_getSysTime(PR_INT8* pBuf,PR_INT32 nBufLen){
	ST_TIME Time;
	PR_INT8 tembuf[16] = {0};

	if(pBuf == NULL || nBufLen < 14){
		return PR_FAILD;
	}
	memset(&Time,0x0,sizeof(ST_TIME));
	OsGetTime(&Time);
	sprintf(tembuf,"%04d%02d%02d%02d%02d%02d",Time.Year,Time.Month,Time.Day,Time.Hour,Time.Minute,Time.Second);
    PR_nUtilAsc2Bcd(14,tembuf,0,nBufLen,pBuf);
	return PR_NORMAL;
}


PR_INT32 PR_nMaskCardNo(const PR_INT8* szInput, const PR_INT8* pszCardNOOutput)
{
	char szCardNo[22+1] = {0};
	int nLenOfStr = strlen(szInput);

	if(nLenOfStr > 22) return PR_FAILD;

	strncpy(szCardNo, szInput, sizeof(szCardNo));

	if(strlen(szCardNo) >= 10) memset(szCardNo+6, '*', strlen(szCardNo) - 10);

	strncpy((PR_INT8*)pszCardNOOutput, (const PR_INT8*)szCardNo,  nLenOfStr);

	return nLenOfStr +1;
}



PR_INT32 PrintOrder(int nCount,TransactionData *pTxnRec)
{
    PR_INT32 ret;
    PR_INT8 temp[256];
    PR_INT8 displayAmount[16] = {0};
    PR_INT8 FilePath[128] = {0};
	PR_INT8 AppPath[128] = {0};
   
    ret = OsPrnOpen(PRN_REAL,NULL);
    if(ret != RET_OK){
        return ret;
    }
    OsPrnSetGray(1);
    OsPrnSetSpace(0,4);
    ret = OsPrnCheck();
    if( ret != RET_OK){
        goto exit;
    }
    for(int i = 0;i< nCount;i++){
        OsPrnReset();
        Business_getAppPath(AppPath,sizeof(AppPath));
// sprintf(FilePath,"%s/res/CLOUDCODE.bmp",AppPath);
// sprintf(FilePath,"%s/res/yl.bmp",AppPath);
// OsLog(LOG_WARN,"bmp path： %s",FilePath);
// OsPrnPutImage((unsigned char *)FilePath);

		OsPrnSetPrintParams(24,1,2,ALIGN_TYPE_CENTER);
		OsPrnPrintf((char *)"POS Purchase Order");
        OsPrnSetPrintParams(24,1,1,ALIGN_TYPE_RIGHT);
        OsPrnPrintf((char *)"MERCHANT COPY");
        OsPrnSetPrintParams(16,1,1,ALIGN_TYPE_LEFT);
        OsPrnPrintf((char *)"================================================");
        OsPrnPrintf((char *)" ");
        OsPrnPrintf((char *)"MERCHANT NAME");
        OsPrnSetPrintParams(24,1,1,ALIGN_TYPE_LEFT);
        OsPrnPrintf((char *)"DSPREAD");
        OsPrnSetPrintParams(16,1,1,ALIGN_TYPE_LEFT);
        OsPrnPrintf((char *)"MERCHANT NO",0,0);
        OsPrnSetPrintParams(24,1,1,ALIGN_TYPE_LEFT);
        OsPrnPrintf((char *)"886043482133343");
        OsPrnSetPrintParams(16,1,1,ALIGN_TYPE_LEFT);
        OsPrnPrintf((char *)"TERMINAL NO                OPERATOR NO");
        OsPrnSetPrintParams(24,1,1,ALIGN_TYPE_LEFT);
        OsPrnPrintf((char *)"157345768                01");
		if (get_transaction_data()->nPosEntryMode != INPUT_QRCODE)
		{
			OsPrnSetPrintParams(16,1,1,ALIGN_TYPE_LEFT);
			OsPrnPrintf((char *)"CARD NUMBER");
			OsPrnSetPrintParams(24,1,1,ALIGN_TYPE_LEFT);
			memset(temp,0x0,sizeof(temp));
			PR_nMaskCardNo(pTxnRec->sCardNo,temp);
// strcat(temp,"       CUP");		//org unknow
			OsPrnPrintf(temp);
		}
// OsPrnSetPrintParams(16,1,1,ALIGN_TYPE_LEFT);
// OsPrnPrintf((char *)"ISS NO                     ACQ NO");
// OsPrnSetPrintParams(24,1,1,ALIGN_TYPE_LEFT);
// OsPrnPrintf((char *)"ICBC          19992900");
        OsPrnSetPrintParams(16,1,1,ALIGN_TYPE_LEFT);
        OsPrnPrintf((char *)"TRANS TYPE");
        OsPrnSetPrintParams(24,2,1,ALIGN_TYPE_LEFT);
        OsPrnPrintf((char *)"SALE");
        OsPrnSetPrintParams(16,1,1,ALIGN_TYPE_LEFT);
        OsPrnPrintf((char *)"EXP DATE");
        OsPrnSetPrintParams(24,1,1,ALIGN_TYPE_LEFT);
        OsPrnPrintf((char *)"2025/05 ");
        OsPrnSetPrintParams(16,1,1,ALIGN_TYPE_LEFT);
        OsPrnPrintf((char *)"BATCH NO                     VUCHER NO");
        OsPrnSetPrintParams(24,1,1,ALIGN_TYPE_LEFT);
        memset(temp,0x0,sizeof(temp));
        sprintf(temp,"%s              %s",pTxnRec->sTrace,pTxnRec->sBatch);
        OsPrnPrintf(temp);
// OsPrnSetPrintParams(16,1,1,ALIGN_TYPE_LEFT);
// OsPrnPrintf((char *)"AUTH NO ");
// OsPrnSetPrintParams(24,1,1,ALIGN_TYPE_RIGHT);
// OsPrnPrintf((char *)"867543234321");
        OsPrnSetPrintParams(16,1,1,ALIGN_TYPE_LEFT);
        OsPrnPrintf((char *)"DATE/TIME");
        OsPrnSetPrintParams(24,1,1,ALIGN_TYPE_RIGHT);
        OsPrnPrintf((char *)pTxnRec->sTransTime);
        OsPrnSetPrintParams(16,1,1,ALIGN_TYPE_LEFT);
        OsPrnPrintf((char *)"AMOUNT");
        OsPrnSetPrintParams(24,2,1,ALIGN_TYPE_LEFT);
        memset(temp,0x0,sizeof(temp));
        PR_nUtilNumberToAmt(pTxnRec->sAmount,sizeof(displayAmount),displayAmount);
        sprintf(temp,"$%s",displayAmount);
        OsPrnPrintf((char *)temp);
        OsPrnSetPrintParams(16,1,1,ALIGN_TYPE_LEFT);
        OsPrnPrintf((char *)"REFERENCE ");
		if (get_transaction_data()->nPosEntryMode != INPUT_QRCODE)
		{
			OsPrnPrintf((char *)"================================================");
			
			sprintf(FilePath,"%s/res/%s.bmp",AppPath,pTxnRec->sTrace);
			OsLog(LOG_WARN,"bmp path: %s",FilePath);
// if(pTxnRec->signatureFlag){
// OSPRN PUTELECSIGNATABYPG ();
			if (access(FilePath, F_OK) == 0){
				OsPrnPutImage((unsigned char *)FilePath);
				//
			}else{
				OsPrnPrintf((char *)"CARDHOLDER SIGNATURE ");
				OsPrnPrintf((char *)" ");// 5line
				OsPrnPrintf((char *)" ");// 5line
				OsPrnPrintf((char *)" ");// 5line
				OsPrnPrintf((char *)" ");// 5line
				OsPrnPrintf((char *)" ");// 5line
			}
		}
        
        OsPrnPrintf((char *)"================================================");
        OsPrnPrintf((char *)"I ACKNOWLEDGE SATISFACTORY RECEIPT OF FELATIUE GOODS/SERVICES");
        OsPrnPrintf((char *)" ");// 5line
        OsPrnPrintf((char *)" ");// 5line
        OsPrnPrintf((char *)" ");// 5line
        OsPrnPrintf((char *)" ");// 5line
        OsPrnPrintf((char *)" ");// 5line
        if((ret = OsPrnStart()) != RET_OK){
            DSP_Info(ret == ERR_PRN_BUSY?(char*)"Printer Busy":
            ret == ERR_PRN_PAPEROUT?(char*)"Printer Paper Out":
            ret == ERR_PRN_OVERHEAT?(char*)"Printer Overheat":
            ret == ERR_PRN_OVERVOLTAGE?(char*)"Printer Overvoltage ":
            ret == ERR_BATTERY_VOLTAGE_TOO_LOW?(char*)"Printer Voltage Too Low":(char*)"Printer Exception");
            goto exit;
        }
    }
    OsPrnFeed(48);
exit:
    OsPrnClose();
	remove(FilePath);
    return ret;
}

void PlayKeyTone(){
	PR_INT8 filePath[128] = {0};

	Business_getAppPath(filePath,sizeof(filePath));
	strcat(filePath,"/res/b.wav");
	OsPlayAudio(filePath);
}
   