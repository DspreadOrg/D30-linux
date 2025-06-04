#ifndef	_EMV_DEVICE_IMPL_H
#define	_EMV_DEVICE_IMPL_H

#ifdef __cplusplus
extern "C"
{
#endif

// EMV parameter tag definition
#define EMVTAG_APP_PAN      0x5A
#define EMVTAG_APP_EXPDATA  0x5F24
#define EMVTAG_APP_PAN_SN   0x5F34
#define EMVTAG_TRACK2     0x57
#define EMVTAG_AC       0x9F26
#define EMVTAG_CID        0x9F27
#define EMVTAG_IAD        0x9F10    //Issuer Application Data
#define EMVTAG_RND_NUM      0x9F37    //Random Number
#define EMVTAG_ATC        0x9F36
#define EMVTAG_TVR        0x95
#define EMVTAG_TXN_DATE     0x9A
#define EMVTAG_TXN_TIME			0x9F21
#define EMVTAG_TXN_TYPE     0x9C
#define EMVTAG_AMOUNT     0x9F02
#define EMVTAG_CURRENCY     0x5F2A
#define EMVTAG_AIP        0x82
#define EMVTAG_COUNTRY_CODE   0x9F1A
#define EMVTAG_OTHER_AMOUNT   0x9F03
#define EMVTAG_TERM_CAP     0x9F33
#define EMVTAG_CVM        0x9F34
#define EMVTAG_TERM_TYPE    0x9F35
#define EMVTAG_IFD        0x9F1E
#define EMVTAG_DF       0x84
#define EMVTAG_APP_VER      0x9F09
#define EMVTAG_TXN_SN     0x9F41
#define EMVTAG_CARD_ID      0x9F63
#define EMVTAG_AID        0x4F
#define EMVTAG_SCRIPT_RESULT  0xDF31
#define EMVTAG_ARC        0x8A
#define EMVTAG_ISS_COUNTRY_CODE 0x5F28
#define EMVTAG_EC_AUTH_CODE   0x9F74
#define EMVTAG_EC_BALANCE   0x9F79
#define EMVTAG_TSI        0x9B
#define EMVTAG_APP_LABEL    0x50
#define EMVTAG_APP_NAME     0x9F12
#define EMVTAG_TRACK3         0x58
#define EMVTAG_AFL              0x94
#define EMVTAG_RFID_OFFLINE_BALANCE 0x9F5D
#define EMVTAG_PAN_SFI_RECORDNO 0xBC
#define EMVTAG_TRACK2_RECORDNO 0xBE

#define EMV_OFFLINEPIN_INPUT    1   /*offline pin*/
#define EMV_OFFLINE_ONLY_INPUT  2   /*latest offline pin*/
#define EMV_ONLINEPIN_INPUT   3   /*online pin*/

/*
 * Terminal configuration parameters
 */
typedef  struct  _EmvTermConfig_t
{
	unsigned char _type;            /* 9F35(Terminal), n2, 1 */
	unsigned char _cap[3];          /* 9F33(Terminal), b,  3 */
	unsigned char _add_cap[5];      /* 9F40(Terminal), b,  5 */
	unsigned char _ics[8];				/*	ICS	*/
	unsigned char _ifd_serial_num[8];       /* 9F1E(Terminal), an8, 8 bytes */
	unsigned char _term_country_code[2];    /* 9F1A(Terminal), n3, 2 bytes */
	unsigned char _terminal_id[8];          /* 9F1C(Terminal), an8, 8 bytes */
	unsigned char _trans_curr_code[2];      /* 5F2A(Terminal), n3, 2 bytes */
	unsigned char _trans_curr_exp;          /* 5F36(Terminal), n1, 1 bytes */
	unsigned char _merchant_name[20];		/* 9F4E商户名称*/
}EmvTermConfig_t;


/*
 * The output AID candidate list structure
 */
typedef struct _AidCandidate_t
{
	unsigned char _aid[16];		/*	AID	Application Identifier*/
	unsigned char _aid_len;		/*	length of AID*/
	unsigned char _lable[20];           /* Application label
								50(ICC), ans, 1-16 bytes, we prepare 20 for some PBOC2 errors*/
	unsigned char _lable_len;		/*	length of lable*/
	unsigned char _preferred_name[20];  /* Application Priority Name9F12(ICC), ans, 1-16 bytes */
	unsigned char _preferred_name_len;	/*	length of preferred name*/
	unsigned char _priority;            /* Application priority identifier 87(ICC), b, 1 bytes */
	unsigned char  _enable;              /* indicate whether the candidate is enabled 
									0	remove
									1	available*/
	unsigned char _resv[3];		/*	reserve bytes	*/
	int   _file_offset;         		/* the offset of this AID	in the parameters file */
}AidCandidate_t;

typedef  struct  _EmvOnlineData_t
{
	char iccResponse[3];            /* 39 Field */
	unsigned char ackdata[1024];       /* 55 Field */
	int ackdatalen;      				/* 55 Field length */
}EmvOnlineData_t;



#define MAX_EC_RECORD_COUNT 10
// Trading Date 9A 3
// Trading time 9F21 3
// Authorization amount 9F02 6
// Other amounts 9F03 6
// Terminal Country Code 9F1A 2
// Trading currency code 5F2A 2
// Merchant Name 9F4E 20
// Transaction Type 9C 1
// Application Transaction Counter (ATC) 9F36 2
typedef struct _EcTxnLog_t
{
	unsigned  char Date[3];
	unsigned  char Time[3];
	unsigned  char AuthAmout[6];
	unsigned  char OtherAmount[6];
	unsigned  char TerminalCountryCode[2];
	unsigned  char CurrencyCode[2];
	unsigned  char MerchantName[20];
	unsigned  char TxnType;
	unsigned  char ATC[2];
}EcTxnLog;

typedef struct  
{
  EcTxnLog Logs[MAX_EC_RECORD_COUNT];
  int Count;
}EcTxnLogs;

typedef enum
{
	EMV_DISP_READING_CARD= 0,
	EMV_DISP_SEE_PHONE ,
	EMV_DISP_NFC_RETAP,
	EMV_DISP_REMOVE_CARD,

}EmvKernelDisp;

typedef struct _EmvCallBack_t
{	
// Multi-application selection Return value >=0 Return the selected AID serial number -1 failed
	int (*EMV_AidSelect)(AidCandidate_t *pList, int listNum);
// Card number confirmation, return value 0 confirmation -1 failed
	int (*EMV_ConfirmCardInfo)(char *pan,int len);
// Request pin cType: 1 offline password 2 last offline password 3 online password Return value - 1 input failed - 2 BYPASS not entered - 3 aborted transaction and Timeout > 0 input password length
	int (*EMV_InputPasswd)(int cType, char *pszPin);
// Identity confirmation type: value 0-5, namely: ID card/official certificate/passport/entry certificate/temporary ID card/other pcon: ID number; return value 1 ID card holder identity confirmation 0 ID confirmation failed
	int (*EMV_CertConfirm)(unsigned char type, unsigned char *pcon, unsigned char len);
// Online result: Online result-1 Online failed >=0 Online successful (with message response)
	int (*EMV_OnlineProcess)(EmvOnlineData_t* pOnlineData);
// Information required by the kernel
	int (*EMV_ProcessDisp)(EmvKernelDisp cType);
}EmvCallBack_t;


typedef	enum
{
	SIMPLE = 0,  
	PBOC,    
	QPBOC, 
}EmvTransFlow;

typedef enum
{
	CONTACT_ICC = 0,
	CONTACTLESS_ICC
}IccType;

// Kernel transaction type
typedef enum
{
  EMV_L2_QUERY = 0,
  EMV_L2_SALE,
  EMV_L2_AUTH,
  EMV_L2_EC_CASH_LOAD,
  EMV_L2_EC_DESIGNATED_ACCOUNT_LOAD,
  EMV_L2_EC_UNDESIGNATED_ACCOUNT_LOAD,
  EMV_L2_EC_CASH_LOAD_VOID,
  EMV_L2_EC_OFFLINE_BALANCE,
  EMV_L2_SALE_LOG,
  EMV_L2_EC_LOAD_ONE_BY_ONE_LOG,
  EMV_L2_EC_LOAD_ALL_IN_ONE_LOG,
  EMV_L2_SALE_VOID,
  EMV_L2_SALE_REFUND,
}EMVTransType;

typedef struct _EmvTransParams_t
{
	IccType icc_type;					/*card type*/
	EMVTransType trans_type;			/*EMV trans type*/
	unsigned char trans_amount[12+1];	/*trans amt*/
	unsigned char trans_time[7+1];		/*trans time*/
	EmvTransFlow trans_flow;			/*trnas flow*/
	unsigned char ec_support;			/*Unused*/
	unsigned char sm_support;			/*Unused*/
	unsigned char force_online_enable;	/*Unused */
	unsigned char tsc[4+1];				/*IC Transaction serial number in 4-byte bcd format */
}EmvTransParams_t;

// emv transaction return value
typedef enum
{
	APP_RC_START = -1,
	APP_RC_COMPLETED = 0,
	APP_RC_TERMINAL,
	APP_RC_CANCEL,
	APP_RC_EMV_DENAIL,
	APP_RC_EMV_GAC2_DENAIL,
	APP_RC_NFC_NOT_ALLOW,
	APP_RC_EMV_APP_BLOCK,
	APP_RC_EMV_APP_SEE_PHONE,
	APP_RC_EMV_TRANS_TRY_ANOTHER_INTERFACE,
	APP_RC_EMV_TRANS_GPO_NOT_SUPPORT,
	APP_RC_FALL_BACK,
	APP_RC_EMV_CARD_BLOCK,
	APP_RC_CARD_NOT_SUPPORT,

	APP_RC_NFC_RETAP_TIMEOUT,
	APP_RC_NFC_RETAP_CANCEL,
	APP_RC_NFC_TERMINAL,
	APP_RC_NFC_DOUBLETAP_DENAIL,
	APP_RC_NFC_MULTI_CARD,
	APP_RC_NFC_TRY_AGAIN,
	APP_RC_TRANS_REVERSEL,
	APP_RC_NUMS,
}EMV_L2_Return;

int Emv_KernelInit(const EmvTermConfig_t emvTermCfg, EmvCallBack_t t_callbackfun);

EMV_L2_Return Emv_Process(EmvTransParams_t emvTransParams);

unsigned char* Emv_GetCoreData(unsigned int tagname, int *pvallen);

int Emv_SetCoreData(unsigned int tagname, unsigned char *pvalue, int valuelen);

// Get a series of TLV data in tagname[], and the returned data format is TLV
int Emv_FetchData(unsigned int* tagname, int count, unsigned char* obuf, int olen);

int Emv_ClrCAPKFile();

int	Emv_ClrAIDFile();

int Emv_PARAM_InputCAPKData(unsigned char *ptlvstrin, int tlvlenin);

int Emv_PARAM_InputAIDData(unsigned char *ptlvstrin, int tlvlenin);

int	Emv_GetAidTotalNum();

int	Emv_GetCapkTotalNum();

unsigned long EMV_L2_GetLastError();

#ifdef __cplusplus
}
#endif
#endif

