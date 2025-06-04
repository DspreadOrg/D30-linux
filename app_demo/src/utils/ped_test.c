#include "ped_test.h"
int Ped_Dukpt_Init()
{

	//return 0;
	PR_INT32 nRet = -1;

    // unsigned char ipek[] = {"\xD5\x24\x39\x14\x3B\xD6\x58\x46\xDD\xFA\xDB\xE3\x71\x9D\xD8\x72"};
    //  unsigned char ksn[] = "\x12\x02\x40\x60\x30\x00\x01\xE0\x00\x01";

     //F6F6ECB6FDC207C22334395D7C58C2C2

    unsigned char ipek[] = {"\xF6\xF6\xEC\xB6\xFD\xC2\x07\xC2\x23\x34\x39\x5D\x7C\x58\xC2\xC2"};
    unsigned char ksn[] = "\x12\x02\x40\x60\x30\x00\x01\xE0\x00\x01";

    //A22E57A0444BB6BA453E994B2D0F4EF2
    // unsigned char ipek[] = {"\xA2\x2E\x57\xA0\x44\x4B\xB6\xBA\x45\x3E\x99\x4B\x2D\x0F\x4E\xF2"};
    // unsigned char ksn[] = "\x12\x02\x40\x11\x70\x00\x10\xE0\x00\x01";

	// unsigned char ipek[] = {"\xF8\x9B\x22\xE6\x9E\x01\xBB\xE4\x56\x1D\x5F\xA8\x29\x35\x2D\x8B"};
    // unsigned char ksn[] = "\x12\x02\x40\x11\x70\x00\x09\xE0\x00\x01";
    
    //8FD8A3EDCB82AF17F19F8313880A4CE3
    // unsigned char ipek[] = {"\x8F\xD8\xA3\xED\xCB\x82\xAF\x17\xF1\x9F\x83\x13\x88\x0A\x4C\xE3"};
    // unsigned char ksn[] = "\x12\x02\x40\x11\x70\x00\x08\xE0\x00\x01";
    // //7BFDBD0FCDBD79FC91E3CC67F41EF307
    // unsigned char ipek[] = {"\x7B\xFD\xBD\x0F\xCD\xBD\x79\xFC\x91\xE3\xCC\x67\xF4\x1E\xF3\x07"};
    // unsigned char ksn[] = "\x12\x02\x40\x11\x70\x00\x07\xE0\x00\x01";
    // //3461241901938AEE8C302F243FC3E894
    // unsigned char ipek[] = {"\x34\x61\x24\x19\x01\x93\x8A\xEE\x8C\x30\x2F\x24\x3F\xC3\xE8\x94"};
    // unsigned char ksn[] = "\x12\x02\x40\x11\x70\x00\x06\xE0\x00\x01";

    //13F222E68C500F9270DC3F79214B83F3
    // unsigned char ipek[] = {"\x13\xF2\x22\xE6\x8C\x50\x0F\x92\x70\xDC\x3F\x79\x21\x4B\x83\xF3"};
    // unsigned char ksn[] = "\x12\x02\x40\x11\x70\x00\x05\xE0\x00\x01";

    //DA85E848D45A6264A1531015AEDC6E04
	// unsigned char ipek[] = {"\xDA\x85\xE8\x48\xD4\x5A\x62\x64\xA1\x53\x10\x15\xAE\xDC\x6E\x04"};
    // unsigned char ksn[] = "\x12\x02\x40\x11\x70\x00\x04\xE0\x00\x01";

    //D3A7D08B487DCBF836569CDD56A854A3
    // unsigned char ipek[] = {"\xD3\xA7\xD0\x8B\x48\x7D\xCB\xF8\x36\x56\x9C\xDD\x56\xA8\x54\xA3"};
    // unsigned char ksn[] = "\x12\x02\x40\x11\x70\x00\x03\xE0\x00\x01";

    // 2D551FF7F96BFE1C1BE9930C5FAB1823
    // unsigned char ipek[] = {"\x2D\x55\x1F\xF7\xF9\x6B\xFE\x1C\x1B\xE9\x93\x0C\x5F\xAB\x18\x23"};
    // unsigned char ksn[] = "\x12\x02\x40\x11\x70\x00\x02\xE0\x00\x01";

    // 3A12F87DDD98C2E275DF989DC45399BF
    // unsigned char ipek[] = {"\x3A\x12\xF8\x7D\xDD\x98\xC2\xE2\x75\xDF\x98\x9D\xC4\x53\x99\xBF"};
    // unsigned char ksn[] = "\x12\x02\x40\x11\x70\x00\x01\xE0\x00\x01";

	nRet = ped_write_ipek(PED_TLK,0,PED_TIK,PED_PIN_IPEK_INDEX,ipek,16,NULL,0,ksn);
    OsLog(LOG_DEBUG,"==========PED_PIN_IPEK_INDEX nRet = %d",nRet);
    nRet = ped_write_ipek(PED_TLK,0,PED_TIK,PED_TRK_IPEK_INDEX,ipek,16,NULL,0,ksn);
    OsLog(LOG_DEBUG,"==========PED_TRK_IPEK_INDEX nRet = %d",nRet);
    nRet = ped_write_ipek(PED_TLK,0,PED_TIK,PED_EMV_IPEK_INDEX,ipek,16,NULL,0,ksn);
    OsLog(LOG_DEBUG,"==========PED_EMV_IPEK_INDEX nRet = %d",nRet);
	
	OsLog(LOG_DEBUG,"==========nRet = %d",nRet);
	unsigned char data[32] = {0};
	OsPedGetKsnDukpt(PED_PIN_IPEK_INDEX,ksn);
	nBcd2Asc(ksn,20,data,0);
	OsLog(LOG_DEBUG,"==========ksn = %s",data);
	
	return nRet;
}


PR_INT32 ped_write_ipek(PR_UINT8 SrcKeyType,PR_UINT8 SrcKeyIdx,PR_UINT8 DstKeyType,PR_UINT8 DstKeyIdx,
					PR_UINT8 *pKey,PR_INT32 keyLen,PR_UINT8 *pKcv,PR_INT32 kcvLen,PR_UINT8 *pKsn)
{
    PR_UINT8 tmep[2048] = {0};
    PR_INT32 offset = 0;
	PR_INT32 nRet = -1;

	nRet = OsPedOpen();
    if(nRet != RET_OK){
        OsLog(LOG_DEBUG,"==========OsPedOpen nRet = %d",nRet);
        return PR_FAILD;
    }
    memset(tmep,0x0,sizeof(tmep));
    tmep[offset] = 0x03;
    offset += 1;
    //SrcKeyType
    tmep[offset] = SrcKeyType;
    offset += 1;
    //SrcKeyIdx
    tmep[offset] = SrcKeyIdx;
    offset += 1;
    //DstKeyIdx：
    tmep[offset] = DstKeyIdx;
    offset += 1;
    //7 bytes 保留域，随机数
    offset += 7;
    //DstKeyType
    tmep[offset] = DstKeyType;
    offset += 1;
    //DstKeyLen
    tmep[offset] = (uchar)keyLen;
    offset += 1;
    //DstKeyValue
    memcpy(tmep+offset,pKey,keyLen);
    offset += 24;
    //KcvMode
    if(kcvLen <= 0){
        tmep[offset] = 0x0;
    }else{
        tmep[offset] = 0x1;
    }
    offset += 1;
    //KcvData
    offset += 128;
    //KCV
    if(kcvLen > 0){
        memcpy(tmep+offset,pKcv,kcvLen);
    }
    offset += 8;
    //填充随机数 
    if(pKsn != NULL){
        memcpy(tmep+offset,pKsn,10);
    }
    offset += 10;
    nRet = OsPedWriteTIK(tmep);
	if(nRet != RET_OK){
        OsLog(LOG_DEBUG,"==========OsPedWriteTIK nRet = %d",nRet);
        nRet =  PR_FAILD;
    }else{
		nRet =  PR_NORMAL;
	}
	OsPedClose();
	return nRet;						
}

void dukpt_test()
{
	PR_INT32 nRet = -1;
	unsigned char outData[512] = {0};
	unsigned char ksn[32] = {0};
	unsigned char temp[64] = {0};
	nRet = OsPedOpen();
    if(nRet != RET_OK){
        return;
    }
	unsigned char data[] = {"\x7A""\xF9""\x0F""\x71""\xFE""\xC0""\x8D""\x2E""\x5F""\x34""\x23""\xD3""\xBC""\x6C""\x94""\x34"};
    nRet = OsPedIncreaseKsnDukpt(PED_PIN_IPEK_INDEX);
	if(nRet == RET_OK)
	{
		nRet = OsPedDesDukpt(PED_TRK_IPEK_INDEX,0x01,NULL,16,data,outData,ksn,0x03);
		if(nRet == RET_OK)
		{
			OsLog(LOG_DEBUG,"==========OsPedDesDukpt ok nRet= %d",nRet);
			nBcd2Asc(outData,32,temp,1);
			OsLog(LOG_DEBUG,"%s",temp);
			memset(temp,0,sizeof(temp));
			nBcd2Asc(ksn,20,temp,1);

			OsLog(LOG_DEBUG,"%s",temp);
		}
		else{
			OsLog(LOG_DEBUG,"==========OsPedDesDukpt error nRet= %d",nRet);
		}
	}
	else
	{
		OsLog(LOG_DEBUG,"==========ksn increase error nRet= %d",nRet);
	}
	OsPedClose();
	return ;
}

int packWriteKeyBuf(uchar SrcKeyType,uchar SrcKeyIdx,uchar DstKeyType,uchar DstKeyIdx,uchar *pKey,int keyLen,uchar *pKcv,int kcvLen,uchar *pKsn,uchar *pOut){
    uchar tmep[2048] = {0};
    int offset = 0;

    memset(tmep,0x0,sizeof(tmep));
    tmep[offset] = 0x03;
    offset += 1;
    //SrcKeyType
    tmep[offset] = SrcKeyType;
    offset += 1;
    //SrcKeyIdx
    tmep[offset] = SrcKeyIdx;
    offset += 1;
    //DstKeyIdx：
    tmep[offset] = DstKeyIdx;
    offset += 1;
    //7 bytes 保留域，随机数
    offset += 7;
    //DstKeyType
    tmep[offset] = DstKeyType;
    offset += 1;
    //DstKeyLen
    tmep[offset] = (uchar)keyLen;
    offset += 1;
    //DstKeyValue
    memcpy(tmep+offset,pKey,keyLen);
    offset += 24;
    //KcvMode
    if(kcvLen <= 0){
        tmep[offset] = 0x0;
    }else{
        tmep[offset] = 0x1;
    }
    offset += 1;
    //KcvData
    offset += 128;
    //KCV
    if(kcvLen > 0){
        memcpy(tmep+offset,pKcv,kcvLen);
    }
    offset += kcvLen;
    //填充随机数 
    if(pKsn != NULL){
        memcpy(tmep+offset,pKsn,10);
    }
    offset += 10;
    memcpy(pOut,tmep,offset);
    dumpBytes("key load ",tmep,offset);
    return offset;
}

void pinpadDupktTest1(){
    int ret;
    uchar tmep[2048] = {0};
    int offset = 0;
    unsigned char tlk[] = {"\x11""\x11""\x11""\x11""\x11""\x11""\x11""\x11""\x22""\x22""\x22""\x22""\x22""\x22""\x22""\x22"};

    unsigned char tik[] = {"\xA8""\xD0""\xB1""\xCF""\x0C""\xE5""\x98""\x70""\x08""\x8C""\xAE""\xD6""\x53""\xBC""\xAA""\xA3"};
    unsigned char tikCkv[] = {"\xC5""\x96""\x1E""\x62""\xBF""\xCE""\x71""\x10"};
    // unsigned char initKsn[] = {"\x00""\x00""\x00""\x00""\x00""\x00""\x00""\x00""\x00""\x01"};

    unsigned char ipek[] = {"\x7A""\xF9""\x0F""\x71""\xFE""\xC0""\x8D""\x2E""\x5F""\x34""\x23""\xD3""\xBC""\x6C""\x94""\x44"};
    unsigned char initKsn[] = "\x02\x02\x31\x11\x00\x00\x04\xE0\x00\x01";
    unsigned char data[] = {"\x7A""\xF9""\x0F""\x71""\xFE""\xC0""\x8D""\x2E""\x5F""\x34""\x23""\xD3""\xBC""\x6C""\x94""\x34"};

    //7AF90F71FEC08D2E5F3423D3BC6C9444
    
    //02023111000004E00001

    unsigned char mac[16] = {0};
    unsigned char enc[32] = {0};
    unsigned char dnc[32] = {0};
    unsigned char PinBlock[32] = {0};
    unsigned char kcvData[8+1] = {0};
    unsigned char kcv[8+1] = {0};
    unsigned char ksn[10+1] = {0};
    unsigned char uzRand[1024] = {0};

    ret = OsPedOpen();
    if(ret != RET_OK){
        OsLog(LOG_DEBUG,"OsPedOpen失败 \n");
        return;
    }
    ret = OsPedGetVer(tmep);
    if(ret != RET_OK){
        OsLog(LOG_DEBUG,"OsPedGetVer失败  [%d]\n",ret);
        goto EXIT;
    }
    OsLog(LOG_DEBUG,"密码键盘版本 %s \n",tmep);

    ret = OsPedEraseKeys();
    if(ret != RET_OK){
        OsLog(LOG_DEBUG,"OsPedEraseKeys失败 \n");
        goto EXIT;
    }
  
    memset(tmep,0x0,sizeof(tmep));
    offset = packWriteKeyBuf(PED_TLK,0,PED_TIK,2,ipek,16,NULL,0,initKsn,tmep);
    ret = OsPedWriteTIK(tmep);
    if(ret != RET_OK){
        OsLog(LOG_DEBUG,"PED_TIK 写入失败 \n");
        goto EXIT;
    }
    OsLog(LOG_DEBUG,"PED_TIK 2 写入成功 \n");  

    memset(tmep,0x0,sizeof(tmep));
    offset = packWriteKeyBuf(PED_TLK,0,PED_TIK,1,ipek,16,NULL,0,initKsn,tmep);
    ret = OsPedWriteTIK(tmep);
    if(ret != RET_OK){
        OsLog(LOG_DEBUG,"PED_TIK 写入失败 \n");
        goto EXIT;
    }
    OsLog(LOG_DEBUG,"PED_TIK 1 写入成功 \n");  

    memset(tmep,0x0,sizeof(tmep));
    offset = packWriteKeyBuf(PED_TLK,0,PED_TIK,3,ipek,16,NULL,0,initKsn,tmep);
    ret = OsPedWriteTIK(tmep);
    if(ret != RET_OK){
        OsLog(LOG_DEBUG,"PED_TIK 写入失败 \n");
        goto EXIT;
    }
    OsLog(LOG_DEBUG,"PED_TIK 3 写入成功 \n");  

    ret = OsPedIncreaseKsnDukpt(2);
    if(ret != RET_OK){
        OsLog(LOG_DEBUG,"OsPedIncreaseKsnDukpt 失败 \n");
        goto EXIT;
    }  
    OsLog(LOG_DEBUG,"OsPedIncreaseKsnDukpt 成功 \n");

    ret = OsPedGetKsnDukpt(2,ksn);
    if(ret != RET_OK){
        OsLog(LOG_DEBUG,"OsPedGetKsnDukpt 失败 \n");
        goto EXIT;
    } 
    dumpBytes("加1后的ksn",ksn,10);

    memset(ksn,0x0,sizeof(ksn));
    ret = OsPedDesDukpt(2,0x01,NULL, 16, data, enc, ksn,0x03);
    if(ret != RET_OK){
        OsLog(LOG_DEBUG,"OsPedDesDukpt 加密失败 \n");
        goto EXIT;
    }
    dumpBytes("当前ksn",ksn,10);
    dumpBytes("加密数据",enc,16);

    memset(ksn,0x0,sizeof(ksn));
    ret = OsPedDesDukpt(2,0x01,NULL, 16, enc, dnc, ksn,0x02);
    if(ret != RET_OK){
        OsLog(LOG_DEBUG,"OsPedDesDukpt 解密失败 \n");
        goto EXIT;
    }
    dumpBytes("当前ksn",ksn,10);
    dumpBytes("解密数据",dnc,16);

    // //mac计算
    // memset(ksn,0x0,sizeof(ksn));
    // ret = OsPedGetMacDukpt(1, tik, 16, mac, ksn,0x23);
    // if(ret != RET_OK){
    //     printf("OsPedGetMac 失败 \n");
    //     goto EXIT;
    // }
    // dumpBytes("当前ksn",ksn,10);
    // dumpBytes("Mac结果",mac,8);

    // memset(ksn,0x0,sizeof(ksn));
    // ret = OsPedGetPinDukpt(1,(unsigned char *)"6228480068726674572", (char *)"0,4,6,12", 0x20, 30*1000, ksn,PinBlock);
    // if(ret != RET_OK){
    //     printf("OsPedGetPinBlock 失败 \n");
    //     goto EXIT;
    // }
    // dumpBytes("加密密码",PinBlock,8);
    // dumpBytes("当前ksn",ksn,10);
    // printf("OsPedGetPinBlock 结束 \n");    
EXIT:
    OsLog(LOG_DEBUG,"关闭密码键盘 \n");
    OsPedClose();
}

void show_pin(int len)
{
	OsLog(LOG_DEBUG,"---------show_pin-------");	
}

void pinpadMKSKTest(){
    int ret;
    uchar tmep[2048] = {0};
    int offset = 0;
    unsigned char tlk[] = {"\x11""\x11""\x11""\x11""\x11""\x11""\x11""\x11""\x22""\x22""\x22""\x22""\x22""\x22""\x22""\x22"};

    unsigned char mk_plaintext[] = {"\x22""\x22""\x22""\x22""\x22""\x22""\x22""\x22""\x33""\x33""\x33""\x33""\x33""\x33""\x33""\x33"};
    unsigned char mk[] = {"\xA8""\xD0""\xB1""\xCF""\x0C""\xE5""\x98""\x70""\x08""\x8C""\xAE""\xD6""\x53""\xBC""\xAA""\xA3"};
    unsigned char mkCkv[] = {"\xC5""\x96""\x1E""\x62""\xBF""\xCE""\x71""\x10"};

    unsigned char tpk[] = {"\x7A""\xF9""\x0F""\x71""\xFE""\xC0""\x8D""\x2E""\x5F""\x34""\x23""\xD3""\xBC""\x6C""\x94""\x34"};
    unsigned char tpkCkv[] = {"\xE1""\x8D""\xE2""\x5E""\xCB""\xCF""\x15""\x91"};
    
    unsigned char tdk[] = {"\x5F""\x34""\x23""\xD3""\xBC""\x6C""\x94""\x34""\x48""\x49""\x16""\x0C""\x0F""\x59""\xAB""\x79"};
    unsigned char tdkCkv[] = {"\x6B""\x94""\x18""\x19""\x1F""\xDA""\x83""\xC6"};

    // unsigned char tak[] = {"\x5D""\xEB""\x51""\x54""\xAA""\xCA""\xBB""\xAF""\x5D""\xEB""\x51""\x54""\xAA""\xCA""\xBB""\xAF"};
    unsigned char tak[] = {"\x5D""\xEB""\x51""\x54""\xAA""\xCA""\xBB""\xAF"};
    unsigned char takCkv[] = {"\x34""\x5E""\xF2""\x50""\x64""\x17""\x13""\x97"};

    unsigned char mac[16] = {0};
    unsigned char enc[32] = {0};
    unsigned char dnc[32] = {0};
    unsigned char PinBlock[32] = {0};
    unsigned char kcvData[8+1] = {0};
    unsigned char kcv[8+1] = {0};

    ret = OsPedOpen();
    if(ret != RET_OK){
        printf("OsPedOpen失败 \n");
        return;
    }
    ret = OsPedGetVer(tmep);
    if(ret != RET_OK){
        printf("OsPedGetVer失败  [%d]\n",ret);
        goto EXIT;
    }
    printf("密码键盘版本 %s \n",tmep);

    ret = OsPedEraseKeys();
    if(ret != RET_OK){
        printf("OsPedEraseKeys失败 \n");
        goto EXIT;
    }
    //写入TLK
    memset(tmep,0x0,sizeof(tmep));
    offset = packWriteKeyBuf(PED_TLK,0,PED_TLK,1,tlk,16,NULL,0,NULL,tmep);
    ret = OsPedWriteKey(tmep);
    if(ret != RET_OK){
        printf("PED_TLK 写入失败 \n");
        goto EXIT;
    }
    printf("PED_TLK 写入成功 \n");


    memset(tmep,0x0,sizeof(tmep));
    //写入密文主密钥（TLK加密）
    // offset = packWriteKeyBuf(PED_TLK,1,PED_TMK,1,mk,16,mkCkv,8,NULL,tmep);
    //写入明文主密钥
    offset = packWriteKeyBuf(PED_TLK,0,PED_TMK,1,mk_plaintext,16,NULL,0,NULL,tmep);
    ret = OsPedWriteKey(tmep);
    if(ret != RET_OK){
        printf("PED_TMK 写入失败 \n");
        goto EXIT;
    }
    printf("PED_TMK 写入成功 \n");    

    memset(tmep,0x0,sizeof(tmep));
    offset = packWriteKeyBuf(PED_TMK,1,PED_TPK,1,tpk,16,tpkCkv,8,NULL,tmep);
    ret = OsPedWriteKey(tmep);
    if(ret != RET_OK){
        printf("PED_TPK 写入失败 \n");
        goto EXIT;
    }
    printf("PED_TPK 写入成功 \n"); 

    memset(tmep,0x0,sizeof(tmep));
    offset = packWriteKeyBuf(PED_TMK,1,PED_TDK,1,tdk,16,tdkCkv,8,NULL,tmep);
    ret = OsPedWriteKey(tmep);
    if(ret != RET_OK){
        printf("PED_TDK 写入失败 \n");
        goto EXIT;
    }
    printf("PED_TDK 写入成功 \n"); 

    memset(tmep,0x0,sizeof(tmep));
    offset = packWriteKeyBuf(PED_TMK,1,PED_TAK,1,tak,8,takCkv,8,NULL,tmep);
    ret = OsPedWriteKey(tmep);
    if(ret != RET_OK){
        printf("PED_TAK 写入失败 \n");
        goto EXIT;
    }
    printf("PED_TAK 写入成功 \n");

    //mac计算
    ret = OsPedGetMac(1, tak, 16, mac, 0x02);
    if(ret != RET_OK){
        printf("OsPedGetMac 失败 \n");
        goto EXIT;
    }
    dumpBytes("Mac结果",mac,8);
    printf("Mac计算成功 \n");

    ret = OsPedDes(1,NULL, tak, 16, enc, 1);
    if(ret != RET_OK){
        printf("OsPedDes 加密失败 \n");
        goto EXIT;
    }
    dumpBytes("加密数据",enc,16);
    printf("加密数据成功 \n");

    ret = OsPedDes(1,NULL, enc, 16, dnc, 0);
    if(ret != RET_OK){
        printf("OsPedDes 解密失败 \n");
        goto EXIT;
    }
    dumpBytes("解密数据",dnc,16);
    printf("解密数据成功 \n");

    ret = OsPedGetKcv(PED_TDK,1,0,8,kcvData,kcv);
    if(ret != RET_OK){
        printf("OsPedGetKcv 失败 \n");
        goto EXIT;
    }
    #if 0
    dumpBytes("获取密码的3字节KCV",kcv,3);
    Disp_vInit();
    Disp_vClearPort();
    Disp_vShowStr(0, EM_DTYPE_REVERT, EM_ALIGN_CENTER,(char*)"磁条卡账户充值");
	Disp_vShowStr(1,EM_DTYPE_NORMAL,EM_ALIGN_CENTER,(char*)"0.01");
    Disp_vShowStr(2,EM_DTYPE_NORMAL,EM_ALIGN_LEFT,(char*)"请输入金额");
    Disp_vShowStr(4,EM_DTYPE_NORMAL,EM_ALIGN_LEFT,(char*)"输错请按清除键");
    ret = OsPedGetPinBlockEx (1,(unsigned char *)"6228480068726674572", (char *)"0,4,6,12", 0, 30*1000, PinBlock,show_pin);
    Disp_vClearPort();
	Disp_vShowStr(2,EM_DTYPE_NORMAL,EM_ALIGN_CENTER,(char*)"输密完成");
    if(ret != RET_OK){
        printf("OsPedGetPinBlock 失败 \n");
        goto EXIT;
    }
    printf("OsPedGetPinBlock 结束 \n");
    dumpBytes("加密密码",PinBlock,8);
    #endif
EXIT:
    printf("关闭密码键盘 \n");
    OsPedClose();
}