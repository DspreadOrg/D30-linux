#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <Stdarg.h>

#define TRUE 1
#define FALSE 0

#define RET_OK 0  //Success
#define ERR_INVALID_HANDLE -1000 //invalid handle
#define ERR_TIME_OUT -1001 //timeout
#define ERR_APP_NOT_EXIST -1002 //The application does not exist
#define ERR_INVALID_PARAM -1003 //Illegal participation
#define ERR_DEV_NOT_EXIST -1004 //The device does not exist
#define ERR_DEV_BUSY -1005 //The device is occupied
#define ERR_DEV_NOT_OPEN -1006 //device is not open
#define ERR_ACCESS_DENY -1007 //Unauthorized access
#define ERR_FONT_NOT_EXIST -1008 //No specified font
#define ERR_FILE_FORMAT -1009 //File format error
#define ERR_USER_CANCEL -1010 //User cancellation
#define ERR_NO_PORT -1011 //No communication port available
#define ERR_NOT_CONNECT -1012 //not connected
#define ERR_MEM_FAULT -1013 //memory error
#define ERR_SYS_BAD -1014 //There is an issue with the system configuration
#define ERR_SYS_NOT_SUPPORT -1015 //The system does not provide this function
#define ERR_STR_LEN -1016 //string is too long
#define ERR_TOO_MANY_HANDLE -1017 //Too many handles
#define ERR_APP_MODE -1018 //Mode error
#define ERR_FILE_NOT_EXIST -1019 //file does not exist
#define ERR_TS_DISABLE -1020 //Touch screen not turned on
#define ERR_FONT_CODE -1021 //Character encoding error
#define ERR_VERSION_TOO_LOW -1022 //Version too low
#define ERR_BATTERY_ABSENT -1023 //The battery does not exist
#define ERR_BATTERY_VOLTAGE_TOO_LOW -1024 //Battery voltage too low

/*
 * 4 system functions
 */
// 4 system functions
#define ERR_FILE_NOT_FOUND -2201 //File not found
#define ERR_VERIFY_SIGN_FAIL -2204 //Signature verification failed
#define ERR_NO_SPACE -2205 //Insufficient system space
#define ERR_NEED_ADMIN -2207 //Need higher permissions
#define ERR_PUK_NOT_EXIST -2208 PUK //does not exist
#define ERR_OS_VERSION_TOO_LOW -2209 //The system version is too low

#define FILE_TYPE_APP 1 //Application package
#define FILE_TYPE_APP_PARAM 2 //Application Data File
#define FILE_TYPE_SYS_LIB 3 //System dynamic library files
#define FILE_TYPE_PUB_KEY 4 //User public key file
#define FILE_TYPE_AUP 5 //Application upgrade package

typedef enum{
    LOG_DEBUG, /*Display debugging information*/
    LOG_INFO, /*Display prompt information*/
    LOG_WARN, /*Display warning messages*/
    LOG_ERROR, /*Display error message*/
} LOG_T;

// ST_TIME time structure
typedef struct{
    int Year; /*年 1970– 2037*/
    int Month; /*月 1 –12*/
    int Day; /*日 1 –31*/
    int Hour; /*时 0 – 23*/
    int Minute; /*分 0 –59*/
    int Second; /*秒 0 –59*/
    int DayOfWeek; /*周 1–周 7（仅读取时间有效）*/
} ST_TIME;

// ST_TIMER timer structure
typedef struct{
    long long Start;
    long long Stop;
    long long TimeLeft;
} ST_TIMER;

// ST_APP_INFO Application Information Structure
typedef struct{
    char Id[64];
    char Name[64];
    char Bin[64];
    char Artwork[64];
    char Desc[64];
    char Vender[32];
    char Version[32] ;
} ST_APP_INFO;

// ST_OPT_INFO Optional system component information structure
typedef struct {
    char Name[64];
    char Version[32];
} ST_OPT_INFO;

long long get_sys_tick();

// Function Set the current date and time of the system
int OsSetTime(const ST_TIME *Time);
// Get the terminal system date and time.
void OsGetTime(ST_TIME *Time);
// Non-connected module test interface
int OsYmRfidTest();
// Get security attack status.0 means normal non-0 attack status
int OsGetAntiTamperState(int* AntiTamperState);
// Get device mode mode 0 user version 1 debug version
int OsGetDevMode(int *mode);
// Obtain the CID ciphertext of the customer application
int OsGetEncCID(char *EncrptyCid);
// Get the CID ciphertext of the OS firmware
int OsGetSysFwEncCID(char *EncrptyCid);
// Get PCBA board number
int OsGetYMPcbaNo(char *pcbaNo);
// Set PCBA board number
int OsSetYMPcbaNo(char *pcbaNo);
// Creates a timer for the specified time.
int OsTimerSet(ST_TIMER *Timer, unsigned long Ms);
// Detect the remaining value of the specified timer.
unsigned long OsTimerCheck(ST_TIMER *Timer);
// Sets the current process/thread pause.
void OsSleep(unsigned int Ms);
// Set the LOG information label.
void OsLogSetTag(const char *Tag);
// Save LOG
int OsLog(LOG_T Prio, const char *fmt, ...);
// Gets the accumulated time from power-on to current operation of the system, excluding sleep time, in milliseconds.
unsigned long OsGetTickCount(void);
// Get information about installed application software.
int OsGetAppInfo(ST_APP_INFO AppInfo[],int InfoCnt);
// Get information about installed optional system components.
int OsGetOptInfo(ST_OPT_INFO OptInfo[],int InfoCnt);
// Get application information through appid
int OsGetAppInfoByAppid(char* appid,ST_APP_INFO *pInfo);
/*
 * Function Sets the frequency and duration of the buzzer whistle.
 * parameter
 * ToneType 【Input】 Beep type, range 【1, 7】
 * DurationMs 【Input】Duration, Range【10,10000】Units: milliseconds.If DurationMs<10, the duration is set to 10; if DurationMs>10000, the duration is set to 10000.
 * Return None
 */
void OsBeep(int ToneType, int DurationMs);

void OsPlayAudio(char* filePath);
/*
 * Function Sets the switch and duty cycle of the key tone.
 * parameter
 * OnOff【Input】 0 Turn off the key tone, 1 Turn on the key tone.
 * DutyCycle【Input】key duty cycle, range【1,99】.If DutyCycle <1, or DutyCycle >99, keep the original duty cycle unchanged.
 * When DutyCycle=50, the volume is maximum.
 */
void OsSetKeyTone(int OnOff,int DutyCycle);

/*
 * Function Set the low battery LED flash frequency.
 * parameter
 * Freq【Input】 1~20HZ.
 * Return 0 Success Other failures
 */
int OsSetLPLEDFreq(int Freq);
/*
 * Function Get the low battery LED flash frequency.
 * parameter
 * Freq【Output】 1~20HZ.
 * Return 0 Success Other failures
 */
int OsGetLPLEDFreq(int *Freq);

typedef void (*RUNAPP_CB)(char *appid, char *str, void *data);
/*
 * Function Switch to the specified sub-app.
 * parameter
 * AppId【Input】 The sub-app ID that needs to be switched.
 * Argv【Input】 The parameter list for the child application to run, can be NULL.(Not used yet)
 * Data【Input】 Custom data, passed to parameters CbOut and CbErr callbacks.(Not used yet)
 * CbOut【Input】 Standard output information callback function.(Not used yet)
 * CbErr【Input】 Standard error message callback function.(Not used yet)
 * return
 * RET_OK Switch successfully
 * ERR_APP_NOT_EXIST sub-app does not exist
 * ERR_ACCESS_DENY Access device was denied
 * ERR_APP_MODE mode error
 * ERR_INVALID_PARAM Illegal Parameters
 * ERR_NEED_ADMIN requires system main application permissions
 */
int OsRunApp(char *AppId, char **Argv, void *Data, RUNAPP_CB CbOut,RUNAPP_CB CbErr);
/*
 * Function Switch to the specified sub-app.
 * parameter
 * AppId【Input】 The sub-app ID that needs to be switched.
 * Argc: [Input] Number of application entry parameters: maximum 12 parameters;
 * Argv: [Input] The maximum number of parameters is 12, the length is less than 31 bytes
 * return
 * RET_OK Switch successfully
 * ERR_APP_NOT_EXIST sub-app does not exist
 * ERR_ACCESS_DENY Access device was denied
 * ERR_APP_MODE mode error
 * ERR_INVALID_PARAM Illegal Parameters
 * ERR_NEED_ADMIN requires system main application permissions
 */
int OsRunAppEx(char *AppId, int Argc,char Argv[][32]);
/*
 * Function Install or update application software, application data, OPT packages, user public keys, and peripheral firmware (FWP packages).
 * parameter
 * AppId【Input】Application ID reserved.
 * FilePath【Input】 The apk file path to be installed cannot be NULL.
 * FileType
 * FILE_TYPE_APP application software installation package, i.e. AIP package
 * FILE_TYPE_APP_PARAM Application Data File
 * Optional system components such as FILE_TYPE_SYS_LIB dynamic library, namely the OPT package
 * FILE_TYPE_PUB_KEY User public key file
 * FILE_TYPE_AUP Application Upgrade Package
 * FILE_TYPE_FWP Device Firmware Package
 * return
 * RET_OK Success
 * ERR_PUK_NOT_EXIST The specified user public key does not exist
 * ERR_FILE_NOT_FOUND file does not exist
 * ERR_FILE_FORMAT File format error
 * ERR_INVALID_PARAM Illegal Parameters
 * ERR_VERIFY_SIGN_FAIL Signature Error
 * ERR_APP_MODE mode error
 */
int OsInstallFile(const char *AppId,const char *FilePath,int FileType);

/*
 * Function Uninstall the specified application or OPT package.
 * parameter
 * AppId【Input】Application ID.
 * FileType
 * FILE_TYPE_APP application package (including all files that the application has installed)
 * FILE_TYPE_SYS_LIB OPT package (including all files in the OPT package)
 * return
 * RET_OK Success
 * The ERR_APP_NOT_EXIST application or OPT package does not exist
 * The ERR_FONT_NOT_EXIST font library does not exist
 */
int OsUninstallFile(const char *AppId, int FileType);

/*
 * Features Get the current system firmware version.
 * parameter
 * Version【Output】 Firmware version buffer,
 * Size 【Input】 Version Buffer length.Recommended length is 64 bytes.
 * return
 * RET_OK successfully obtained
 * ERR_INVALID_PARAM Illegal Parameters
 */
int OsFirmwareGetVersion(char *Version,int Size);

/*
 * Function Upgrade system firmware.
 * Parameter FwPath Firmware path, YMG
 * FwType 【Input】 Firmware Type
 * FW_TYPE_SE_APP SE Application
 * FW_TYPE_SE_FW_CFG SE firmware configuration
 * FW_TYPE_SE_FW SE firmware
 * return
 * RET_OK Update successfully
 * The RR_FILE_NOT_FOUND file does not exist
 * ERR_VERIFY_SIGN_FAIL Signature Error
 * ERR_APP_MODE mode error
 * ERR_SYS_NOT_SUPPORT OS incompatible
 */

#define FW_TYPE_SE_APP     1 //SE App se_fota_application.YMG
#define FW_TYPE_SE_FW_CFG  2 //SE FW  se_fota_dtb_pro.YMG
#define FW_TYPE_SE_FW      3 //SE FW  se_fota_firmware.YMG

int OsFirmwareUpgrade(const char *FwPath, int FwType);

/*
 * Function Get operating system and module firmware version information.
 * Parameter VerType Version Category:
 * 0x01: System version
 * 0x02: Middleware version number
 * 0x03: Secure CPU App version number
 * 0x04: Secure CPU firmware version
 * Version【Output】 The size of the version information buffer must be no less than 31 bytes.
 * Return None
 * Usage 1. If Version[0] is equal to 0x00, it means that the corresponding module does not exist;
 * 2. The length of each version of information is less than or equal to 31 bytes and ends with the "\0" character.
 */
void OsGetSysVer(int VerType, char *Version);

void OsGetSysVersion(int VerType, char *Version);

// Get the wireless module version number cache 32 bytes
int OsGetWirelessModuleVer(char *ver);

int OsGetTermSn(char* Sn);

// Set SN maximum not exceeding 31 bytes
int OsSetTermSn(char* Sn);

int OsLed(unsigned int red,unsigned int green,unsigned int yellow,unsigned int blue);

int OsGetSysParam(char *key, char *value);

int OsSetSysParam(char *key, char *value);
/*
 * Function File upgrade function callback pointer.
 * Parameters Result Upgrade result >=0 Upgrade successful Other failures
 * Does Reboot need to be restarted? 1 requires 0 does not need it?
 * Return 1 sdk processing restart 0 Application processing restart.
 */
typedef  int (*pUpgradeCallback)(int Result, char Reboot);

int OsUpgradeFile(char *pZipPath,pUpgradeCallback callback);

int OsDownLoadAndUpgradeFile();

// Get Startup Mode pMode 1 Factory Mode
int OsGetSysBootMode(int *pMode);
// Get the hardware version hw_version[0] ='4', type A30 '7', type A50 Other unknown
void OsGetHwVersion(char *hw_version);
int OsGetFactoryTestStep();
int OsSetFactoryTestStep(int flag);

// Get the current DNS that is in effect
int OsGetCurDNSAddr(char* dnsaddr,int dnsaddrLen);
// Set up DNS for wireless or WIFI, type type 0 wifi, 1 wireless
int OsGetDNSAddr(int type,char* dnsaddr,int dnsaddrLen);
// Get DNS for setting wireless or WIFI, type type 0 wifi, 1 wireless, set 0.0.0.0 Clear settings
int OsSetDNSAddr(int type,char* dnsaddr);
// Set the preferred network type, type value 0: Auto GSM/LTE, 1: GSM only, 3: LTE only
int OsSetPreferredNetworkType(int type);
// Get the preferred network type value 0:Auto GSM/LTE, 1:GSM only, 3:LTE only
int OsGetPreferredNetworkType(int *type);

// Set how long it takes for the power to shut down after deep sleep (unit 0.5h) Time value (0<Time<=48)
int OsSetSleepShutdownTime(int Time);
// Get how long it takes for deep sleep to shut down without operation (unit 0.5h)
int OsGetSleepShutdownTime(int *Time);
// TP reset
int OsResetTP();

int OsGetMisFlag(int *flag);
/*
 * 5. Encrypt and decrypt
 */
// 5. Encrypt and decrypt
#define ERR_DATA_TOO_BIG -2400  //RSA The encrypted data is greater than the modulus
#define ERR_GEN_RANDOM -2401  //Failed to generate random number
#define ERR_GEN_FAIL -2402  //Failed to generate RSA key pair

#define SHA_TYPE_1    1
#define SHA_TYPE_224  2
#define SHA_TYPE_256  3
#define SHA_TYPE_384  4
#define SHA_TYPE_512  5

/*
 * Generate true random numbers.
 * parameter
 * Random【Output】 Pointer to store random numbers
 * RandomLen 【Lost
 * Enter] The length of the random number to be read (less than or equal to 4096 bytes)
 */
void OsGetRandom(unsigned char *Random,int RandomLen);
/*
 * Calculate the hash value of the SHA (Secure Hash Algorithm) family.
 * parameter
 * Mode algorithm type SHA_TYPE_1 SHA_TYPE_224 SHA_TYPE_256 SHA_TYPE_384 SHA_TYPE_512
 * Data【Input】Input data buffer
 * DataLen 【Input】Input data length
 * ShaOut【Out】 The output SHA value, the storage space is recommended to be greater than 64 bytes.
 */
void OsSHA(int Mode, const void *Data, int DataLen, unsigned char*ShaOut);

/*
 * DES/TDES encryption and decryption operation.
 * parameter
 * Input【Input】 8 bytes of input data
 * Output【Output】 8 bytes of output data
 * DesKey【Input】 DES/TDES key
 * KeyLen 【Input】 8, 16 or 24 (bytes)
 * Mode 1-encryption 0-decryption
 */
void OsDES(const unsigned char *Input, unsigned char *Output, const unsigned char *DesKey, int KeyLen, int Mode);

/*
 * AES encryption and decryption operation.
 * parameter
 * Input【Input】 16 bytes of input data
 * Output【Output】 16 bytes of output data
 * AesKey【Input】 Key
 * KeyLen 【Input】 16, 24 or 32 (bytes)
 * Mode 1-encryption 0-decryption
 */
void OsAES(const unsigned char *Input, unsigned char *Output, const unsigned char *AesKey,int KeyLen, int Mode);

/*
 * Perform RSA encryption and decryption operations.
 * parameter
 * Modulus 【Input】 The modular pointer (i.e. n=p*q) that stores RSA operations is stored in the order in which the high is in front and the low is in the back.
 * ModulusLen 【Input】module length (in bytes)
 * Exp【Input】Storing the index pointer for RSA operations.Store in the order of high position in front and low position in back.
 * ExpLen 【Input】 Exponent length (in bytes)
 * DataIn [Input] Input data buffer pointer, the length is the same as the module length
 * DataOut【Out】 Output data buffer pointer, the length is the same as the module length
 * Mode 1-encryption 0-decryption
 * return
 * RET_OK Success
 * ERR_INVALID_PARAM Illegal Parameters
 * ERR_DATA_TOO_BIG ExpLen greater than ModulusLen
 */
int OsRSA(const unsigned char *Modulus, int ModulusLen,const unsigned char *Exp, int ExpLen, const unsigned char *DataIn, unsigned char *DataOut,int Mode);

// RSA RSA_NO_PADDING public key decryption, input data length is equal to NLen
int OsEmvRSAPublicDecrypt(unsigned char *N, int NLen,unsigned char *E, int ELen, unsigned char *DataIn,unsigned char *DataOut);
/*
 * Generates RSA public-private key pairs with specified index and modulo length.
 * Parameter Modulus 【Output】 The module of the key (stored in the order of the high position in front and the low position in the back)
 * PriExp [Output] Private key index (stored in the order of high and low and low)
 * ModulusLen 【Input】The length of the module (unit: bytes, can be: 64, 128, 256, 512)
 * PubExp [Input] Public Key Index.Only: "\x00\x00\x00\x03" or "\x00\x01\x00\x01"
 * return
 * RET_OK Success
 * ERR_INVALID_PARAM Illegal Parameters
 * ERR_GEN_RANDOM Data failed randomly
 * ERR_GEN_FAIL Generation failed
 */
int OsRSAKeyGen(unsigned char *Modulus, unsigned char *PriExp, int ModulusLen, const unsigned char * PubExp);

/*
 * 7LCD
 */

/*
 * Function Set screen brightness.
 * Parameter Brightness Brightness value, value range is [0~10], 0: Turn off the backlight.10: The brightest.
 * The default value is 8, and no other values ​​are active.
 */
void OsScrBrightness(int Brightness);

/*
 * Function Get the LCD physical screen size.
 * parameter
 * Width【Output】Width, unit in pixels.
 * Height【Output】Height, unit in pixels.
 * Return None
 */
void OsScrGetSize(int *Width, int *Height);

/*
 * 6 PED
 */
#define ERR_PED_NO_KEY -3801 //The key does not exist 
#define ERR_PED_KEY_IDX_ERR -3802 //Key index error 
#define ERR_PED_DERIVE_ERR -3803  //When writing the key, the key hierarchy is incorrect, and the source key hierarchy is lower than the destination key hierarchy
#define ERR_PED_CHECK_KEY_FAIL -3804 //Key verification failed 
#define ERR_PED_NO_PIN_INPUT -3805 //No PIN input
#define ERR_PED_PIN_INPUT_CANCEL -3806 //Cancel PIN input
#define ERR_PED_WAIT_INTERVAL -3807 //The interval between function calls is less than the set minimum time interval (calculated)
#define ERR_PED_KCV_MODE_ERR -3808 //KCV mode error
#define ERR_PED_KEY_TAG_ERR -3809 //Key label error, unable to use the key
#define ERR_PED_KEY_TYPE_ERR -3810 //The key type is not supported or the key type specified for the index does not match 
#define ERR_PED_PIN_LEN_ERR -3811 //The length of the input PIN does not match the expected length 
#define ERR_PED_DSTKEY_IDX_ERR -3812 //Target key index error 
#define ERR_PED_SRCKEY_IDX_ERR -3813 //Source key index error 
#define ERR_PED_KEY_LEN_ERR -3814 //Key length error 
#define ERR_PED_INPUT_PIN_TIMEOUT -3815 //PIN input timeout
#define ERR_PED_NO_ICC -3816 //IC card does not exist
#define ERR_PED_ICC_INIT_ERR -3817 //IC card initialization error
#define ERR_PED_GROUP_IDX_ERR -3818 //DUKPT group index error
#define ERR_PED_TAMPERED -3819 //PED block 
#define ERR_PED_NO_MORE_BUF -3820 //There are no free buffers available 
#define ERR_PED_NORMAL_ERR -3821 //PED General Error
#define ERR_PED_NEED_ADMIN -3822 //The current operation requires advanced permissions 
#define ERR_PED_DUKPT_KSN_OVERFLOW -3823 //Current DUKPT overflow
#define ERR_PED_KCV_CHECK_FAIL -3824 //KCV verification failed
#define ERR_PED_SRCKEY_TYPE_ERR -3825 //When writing the key, the type of the source key ID does not match the type of the source key
#define ERR_PED_UNSPT_CMD -3826 //Command not supported 
#define ERR_PED_ADMIN_ERR -3827 //Failed to obtain sensitive system services 
#define ERR_PED_DOWNLOAD_INACTIVE -3828 //PED download function not activated
#define ERR_PED_KCV_ODD_CHECK_FAIL -3829 //KCV odd verification failed
#define ERR_PED_PED_DATA_RW_FAIL -3830 //Failed to read PED data 
#define ERR_PED_ICC_CMD_ERR -3831 //IC card operation error (offline plaintext, ciphertext PIN verification)
#define ERR_PED_DUKPT_NEED_INC_KSN -3832  //DUKPT KSN needs to add 1 first
#define ERR_PED_DUKPT_NO_KCV -3833 //DUKPT key does not have KCV
#define ERR_PED_NO_FREE_FLASH -3834 //PED storage space is insufficient
#define ERR_PED_INPUT_CLEAR -3835 //The user presses the CLEAR key to exit PIN input
#define ERR_PED_INPUT_BYPASS_BYFUNCTION -3836 //The user presses the FN/ATM4 key to cancel the PIN input
#define ERR_PED_NO_PIN_MODE -3837 //PIN input mode not set
#define ERR_PED_DATA_MAC_ERR -3838 //Data MAC verification error
#define ERR_PED_DATA_CRC_ERR -3839 //Data CRC check error
#define ERR_PED_KEY_VALUE_INVALID -3840 //The work key value does not meet the requirements or already exists

#define PED_TLK 0x01 //Loading Key 
#define PED_TMK 0x02 //Master Key 
#define PED_TPK 0x03 //PIN Key 
#define PED_TAK 0x04 //MAC Key 
#define PED_TDK 0x05 //Data Key 
#define PED_TIK 0x10 //DUKPT Initial Key 
#define PED_TRK 0x07 //MSR Key 
#define PED_TAESK 0x20 //AES Key 
#define PED_SM2_PVT_KEY 0x30 //SM2 Private Key 
#define PED_SM2_PUB_KEY 0x31 //SM2 Public Key 
#define PED_SM4_TMK 0x32 //SM4 Master Key 
#define PED_SM4_TPK 0x33 //SM4 PIN Key 
#define PED_SM4_TAK 0x34 //SM4 MAC Key 
#define PED_SM4_TDK 0x35 //SM4 Data Key 
#define PED_AES_TLK 0x27 //AES Loading Key 
#define PED_AES_TMK 0x28 //AES Master Key 
#define PED_AES_TPK 0x23 //AES PIN Key 
#define PED_AES_TAK 0x24 //AES MAC Key 
#define PED_AES_TDK 0x20 //AES Data Key 
#define PED_AES_TIK 0x51 //AES Initial Key 


#define  PED_ASTERISK_ALIGN_LEFT 0    //left alignment 
#define  PED_ASTERISK_ALIGN_CENTER 2  //Center 
#define  PED_ASTERISK_ALIGN_RIGHT 1    //right alignment 

typedef struct { 
    int ModulusLen; /* Module length (position) */ 
    unsigned char Modulus[512]; /* Module, if it is less than 512 bytes, add 0x00 to the left */ 
    int ExponentLen; /* Index length (bits) */ 
    unsigned char Exponent [512]; /* Index, if it is less than 512 bytes, add 0x00 to the left */ 
    unsigned char KeyInfo[128]; /* RSA key information */ 
 } ST_RSA_KEY; 

typedef struct { 
    int ModulusLen; /* PIN encryption public key module length (bits) */ 
    unsigned char Modulus[256]; /* PIN encryption public key modulus */ 
    unsigned char Exponent [4]; /* PIN encryption public key index */ 
    int IccRandomLen; /* The length of the random number obtained from the IC card */ 
    unsigned char IccRandom[8]; /* Random number obtained from IC card */ 
} ST_RSA_PINKEY; 

/*
 * 6.4 Basic PED
 */
/*
 * Function Turn on Prolin's PED service.
 * Parameters None
 * return
 * RET_OK Success
 * ERR_DEV_BUSY The device is occupied
 */
int OsPedOpen (void); 

/*
 * Feature Get the current version of PED.
 * Parameters PedVer【Output】 PED version information buffer, 6 bytes.
 * return
 * RET_OK Success
 * ERR_DEV_NOT_OPEN PED device is not turned on
 * ERR_INVALID_PARAM Illegal Parameters
 */
int OsPedGetVer (unsigned char * PedVer); 

/*
 * Function Erase all key information saved by the PED service.
 * Parameters None
 * return
 * RET_OK Success
 * ERR_DEV_NOT_OPEN The device is not turned on
 * Others Please refer to the PED function return value list
 */
int OsPedEraseKeys (void); 

/*
 * Function Disconnect from PED service.
 * Parameters None
 * Return None
 * Usage: This function must be called before the program exits and closes the device.
 */
void OsPedClose (void); 


/*
 * 6.5 PIN input
 */

/*
 * Function Listen and obtain the number of PIN keys entered by the user in the current state and the historical key sequence between the current and the last listening.
 * 
 * parameter
 * 
 * KeyCacheCnt【Output】 The current number of historical key values ​​obtained.
 * KeyCache [Output] Not yet implemented
 * return
 * >= 0 Get the PIN input event successfully, the return value size indicates the number of "*" in the PIN input interface at this time.
 * ERR_DEV_NOT_OPEN PED device is not turned on
 * ERR_INVALID_PARAM Illegal Parameters
 * Others Please refer to the PED function return value list
 */
int OsPedPinKeyNotify(int *KeyCacheCnt, unsigned char *KeyCache);

/*
 * Function Set the line and alignment method of the pin input box to display the third line by default.
 * Parameters None
 * Return RET_OK Success
 */
int OsPedSetAsteriskLayout(int line,int align);

/*
 * 6.6 MK/SK
 */
/*
 * Function Write a key, including write and divergence of TLK, TMK, TWK, SM4_TMK and SM4_TWK, and you can choose to use KCV to verify the correctness of the key.
 * Parameter KeyBlock【Input】 See specifications for details
 * return
 * 0 Success
 * ERR_DEV_NOT_OPEN PED device is not turned on
 * ERR_INVALID_PARAM Illegal Parameters
 * ERR_SYS_BAD System Error
 * ERR_PED_KEY_IDX_ERR Key index error
 * ERR_PED_KEY_TYPE_ERR Key type error
 * ERR_PED_TAMPERED PED is locked
 * ERR_PED_NO_MORE_BUF Insufficient system memory space
 * ERR_PED_NORMAL_ERR PED General Error (KeyBlock Format Error)
 * ERR_PED_DERIVE_ERR Key divergence error
 * ERR_PED_KCV_MODE_ERR PED KCV verification mode error
 * ERR_PED_KCV_CHECK_FAIL PED KCV verification failed
 * Others Please refer to the PED function return value list
 */
int OsPedWriteKey (const unsigned char * KeyBlock);

/*
 * Function: Within the specified time limit, scan the PIN of the specified length of ExpPinLenIn on the keyboard, and output the PIN block generated by the Mode specified algorithm encryption.
 * parameter
 * KeyIdx The index number of TPK, with a value range of [1-100].
 * DataIn【Input】
 * TimeoutMs Enter the timeout unit of PIN: the maximum value of milliseconds is 3000000: means there is no timeout time, and PED does not perform timeout control.
 * PinBlock【Output】8 or 16 bytes, pointing to the generated PIN block.When Mode is 0x10, the PinBlock length is 16 bytes.
 * return
 * RET_OK Success
 * ERR_DEV_NOT_OPEN PED device is not turned on
 * ERR_INVALID_PARAM Illegal Parameters
 * Others Please refer to the PED function return value list
 */
int OsPedGetPinBlock (int KeyIdx,const unsigned char *DataIn, const char *ExpPinLen, int Mode, unsigned long TimeoutMs, unsigned char *PinBlock);
 
typedef void (*PIN_STAR_DISPLAY)(int len);//Password keyboard displays * function pointer, len current password length
/*
 * Function: Within the specified time limit, scan the PIN of the specified length of ExpPinLenIn on the keyboard, and output the PIN block generated by the Mode specified algorithm encryption.
 * parameter
 * KeyIdx The index number of TPK, with a value range of [1-100].
 * DataIn【Input】
 * TimeoutMs Enter the timeout unit of PIN: the maximum value of milliseconds is 3000000: means there is no timeout time, and PED does not perform timeout control.
 * PinBlock【Output】8 or 16 bytes, pointing to the generated PIN block.When Mode is 0x10, the PinBlock length is 16 bytes.
 * pin_star_display input * display function pointer
 * return
 * RET_OK Success
 * ERR_DEV_NOT_OPEN PED device is not turned on
 * ERR_INVALID_PARAM Illegal Parameters
 * Others Please refer to the PED function return value list
 */
int OsPedGetPinBlockEx (int KeyIdx,const unsigned char *DataIn, const char *ExpPinLen, int Mode, unsigned long TimeoutMs, unsigned char *PinBlock,PIN_STAR_DISPLAY pin_star_display);

int OsPedGetPinBlockBySoftKeyboard(int KeyIdx,const unsigned char *DataIn, int minPinLen,int maxPinLen,unsigned long TimeoutMs, unsigned char *PinBlock,PIN_STAR_DISPLAY pin_star_display);
/*
 * Function Use the MAC key specified by KeyIdx to perform Mode-specified operations on DataIn.
 * parameter
 * KeyIdx TAK index number, with a value range of [1~100].
 * DataIn [Input] The data that requires MAC operation, the length is less than or equal to 2032 bytes.
 * DataInLen MAC operation data length, when the length cannot be divisible by 8 bytes, it will automatically supplement "\x00".
 * Mac【Output】8 bytes, MAC output.
 * Mode divides DataIn into 8 bytes, namely BLOCK1, BLOCK2, BLOCK3, etc.
 * return
 * RET_OK Success
 * ERR_DEV_NOT_OPEN PED device is not turned on
 * ERR_INVALID_PARAM Illegal Parameters
 * Others Please refer to the PED function return value list
 */
int OsPedGetMac(int KeyIdx, const unsigned char *DataIn, int DataInLen, unsigned char *Mac, int Mode);

/*
 * Function
 * Use TDK to perform DES/TDES decryption or encryption operation on data of DataInLen length, and output plain text or ciphertext; a specified TDK can only be used for encryption or decryption operations.
 * parameter
 * KeyIdx TDK index number, value range is [1~100].
 * InitVector [Input] The initial vector of the encryption and decryption operation, with a length of 8 bytes.When Mode=0x02/0x03/0x04/0x05, the initial vector is required. When it is NULL, the default is "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"; when Mode=0x00/0x01, the initial vector is not required, it can be NULL.
 * DataIn [Input] points to the data to be calculated.
 * The data length that DataInLen needs to be calculated, in units of bytes, should be less than or equal to 1024; when Mode is 0x00~0x05, it must be a multiple of 8; when Mode is 0x06~0x07, it may not be a multiple of 8.
 * DataOut【Out】 Points to the calculated data.
 * Mode 0x00: ECB decryption 0x01: ECB encryption 0x02: CBC decryption 0x03: CBC encryption 0x04: OFB decryption 0x05: OFB encryption 0x06: CFB8 decryption 0x07: CFB8 encryption
 * return
 * RET_OK Success
 * ERR_DEV_NOT_OPEN PED device is not turned on
 * ERR_INVALID_PARAM Illegal Parameters
 * Others Please refer to the PED function return value list
 */
int OsPedDes(int KeyIdx,unsigned char * InitVector, const unsigned char *DataIn, int DataInLen, unsigned char *DataOut, int Mode);

/*
 * Function
 * Get the KCV value of the key for both parties to verify the key: when the key type is not TIK, a piece of data is encrypted with the specified key and algorithm, and the first 3 bytes of the data ciphertext is returned; when the key type is TIK, the 8-byte KCV injected (calculated) with the TIK is returned.
 * parameter
 * KeyType Key Type
 * KeyIdx key index number
 * KcvModeKCV verification mode
 * KcvDataLen participates in calculating the data length of KCV, which is no more than 128 bytes and is divisible by 8; it can be 0 when the key type is TIK; when the KCV verification mode is 0x03 or 0x04, the data length must be divisible by 16.
 * KcvData【Input】The data participating in the calculation of KCV, and can be NULL when the key type is TIK.
 * Kcv【Output】KCV value, when the key types are TIK and AESTIK, the KCV is 8 bytes; when the key type is AES, the KCV is 5 bytes, and the rest is 3 bytes.
 * return
 * RET_OK Success
 * ERR_DEV_NOT_OPEN PED device is not turned on
 * ERR_INVALID_PARAM Illegal Parameters
 * Others Please refer to the PED function return value list
 */
int OsPedGetKcv(int KeyType, int KeyIdx, int KcvMode, int KcvDataLen, unsigned char *KcvData, unsigned char *Kcv);


/*
 * 6.7 Dukpt
 */
/*
 * Function Write a TIK key and optionally use KCV to verify the correctness of the key.
 * Parameter KeyBlock【Input】
 * return
 * ERR_OK Success
 * ERR_DEV_NOT_OPEN The device is not turned on
 * ERR_INVALID_PARAM Illegal Parameters
 * Others Please refer to the PED function return value list
 */
int OsPedWriteTIK (const unsigned char *KeyBlock);

/*
 * Function: Scan the PIN entered on the keyboard within the specified time limit and output the generated PIN block using the DUKPT's PIN key.
 * parameter
 * GroupIdx DUKPT Group index number, with value range [1~100].
 * DataIn【Input】
 * ExpPinLen【Input】
 * Mode Select the format of the PIN block:
 * TimeoutMs Enter the timeout time of the PIN, in milliseconds.The maximum value is 3000000: indicates that there is no timeout time and PED does not perform timeout control.
 * Ksn【Output】 10 bytes, pointing to the current KSN.
 * PinBlock【Output】 8 bytes, pointing to the generated PIN block.
 * return
 * RET_OK Success
 * ERR_DEV_NOT_OPEN The device is not turned on
 * ERR_INVALID_PARAM Illegal Parameters
 * Others Please refer to the PED function return value list
 */
int OsPedGetPinDukpt (int GroupIdx, const unsigned char *DataIn, const char *ExpPinLen, int Mode, unsigned long TimeoutMs, unsigned char *Ksn, unsigned char *PinBlock);
/*
 * Function: Scan the PIN entered on the keyboard within the specified time limit and output the generated PIN block using the DUKPT's PIN key.
 * parameter
 * GroupIdx DUKPT Group index number, with value range [1~100].
 * DataIn【Input】
 * ExpPinLen【Input】
 * Mode Select the format of the PIN block:
 * TimeoutMs Enter the timeout time of the PIN, in milliseconds.The maximum value is 3000000: indicates that there is no timeout time and PED does not perform timeout control.
 * Ksn【Output】 10 bytes, pointing to the current KSN.
 * PinBlock【Output】 8 bytes, pointing to the generated PIN block.
 * pin_star_display input * display function pointer
 * return
 * RET_OK Success
 * ERR_DEV_NOT_OPEN The device is not turned on
 * ERR_INVALID_PARAM Illegal Parameters
 * Others Please refer to the PED function return value list
 */
int OsPedGetPinDukptEx (int GroupIdx, const unsigned char *DataIn, const char *ExpPinLen, int Mode, unsigned long TimeoutMs, unsigned char *Ksn, unsigned char *PinBlock,PIN_STAR_DISPLAY pin_star_display);

int OsPedGetPinBlockDukptBySoftKeyboard(int KeyIdx,const unsigned char *DataIn, int minPinLen,int maxPinLen,unsigned long TimeoutMs, unsigned char *Ksn,unsigned char *PinBlock,PIN_STAR_DISPLAY pin_star_display);
/*
 * Function Use the DUKPT key to calculate the MAC value.
 * parameter
 * GroupIdx DUKPT Group index number, with value range [1~100].
 * DataIn [Input] points to the data content that needs to be calculated by the MAC.
 * The length of DataInLen data is less than or equal to 2032 bytes. When it is not divided by 8 bytes, it will automatically supplement "\x00".
 * Mac【Output】 Points to the resulting MAC.
 * Ksn【Output】 Points to the current KSN.
 * Mode
 * return
 * RET_OK Success
 * ERR_DEV_NOT_OPEN The device is not turned on
 * ERR_INVALID_PARAM Illegal Parameters
 * Others Please refer to the PED function return value list
 */
int OsPedGetMacDukpt(int GroupIdx, const unsigned char *DataIn, int DataInLen, unsigned char *Mac, unsigned char *Ksn, int Mode);

/*
 * Function Use the DUKPT key to encrypt and decrypt the input data.
 * parameter
 * GroupIdx DUKPT Group index number, with value range [1~100].
 * KeyVarType【Input】
 * InitVector [Input] Encrypts and decrypts the initial vector of the operation, 8 bytes can be NULL.
 * DataInLen The data length that needs to be encrypted and decrypted is less than or equal to 8192
 * DataIn [Input] Points to data that needs to be calculated
 * DataOut【Out】 Points to the data after the operation
 * Ksn【Output】 Points to the current KSN, 10 bytes
 * Mode Encryption and Decryption Mode
 * return
 * RET_OK Success
 * ERR_DEV_NOT_OPEN The device is not turned on
 * ERR_INVALID_PARAM Illegal Parameters
 * Others Please refer to the PED function return value list
 */
int OsPedDesDukpt(int GroupIdx, int KeyVarType, unsigned char *InitVector, int DataInLen, unsigned char *DataIn, unsigned char *DataOut, unsigned char *Ksn, int Mode);

/*
 * Function Get the current KSN value.
 * parameter
 * GroupIdx DUKPT Group index number, with value range [1~100].
 * Ksn【Output】 Points to the current KSN, 10 bytes
 * return
 * RET_OK Success
 * ERR_DEV_NOT_OPEN The device is not turned on
 * ERR_INVALID_PARAM Illegal Parameters
 * Others Please refer to the PED function return value list
 */
int OsPedGetKsnDukpt(int GroupIdx, unsigned char *Ksn);
/*
 * Function Increase the KSN value of the specified DUKPT group.
 * Parameter GroupIdx DUKPT Group Index number, value range is [1~100].
 * Return RET_OK Success
 * ERR_DEV_NOT_OPEN The device is not turned on
 * ERR_INVALID_PARAM Illegal Parameters
 * Others Please refer to the PED function return value list
 */
int OsPedIncreaseKsnDukpt (int GroupIdx);


void OsCloseSoftKeyboard ();

/*
 * 11 printer
 */

#define ALIGN_TYPE_LEFT 0    //left alignment
#define ALIGN_TYPE_CENTER 1   //center
#define ALIGN_TYPE_RIGHT 2   //right alignment


#define ERR_PRN_BUSY -3701 //printer busy
#define ERR_PRN_PAPEROUT -3702 //printer is out of paper
#define ERR_PRN_WRONG_PACKAGE -3703 //Data packet format error
#define ERR_PRN_OVERHEAT -3704 //Printer overheating
#define ERR_PRN_OUTOFMEMORY -3705 //Printing data is too large and exceeds the buffer length
#define ERR_PRN_OVERVOLTAGE -3706 //Voltage too high

enum{
    PRN_REAL = 0,   //Physical printing equipment;
    PRN_BMP,        //Virtual printing device, the printing results are saved locally in BMP format.
};

/*
 * Features Turn on the printer device, including both physical and virtual printers.
 * parameter
 * printertype【Input】
 * Printer device type PRN_REAL: Physical printing device PRN_BMP: Virtual printing device, print results are saved locally in BMP format.
 * targetname【Input】When the printing device is a physical printer, this parameter must be NULL;
 * When the printing device is a virtual printer, it points to the path output to the local BMP file.If the specified file already exists, overwrite it.
 * return
 * RET_OK Success
 * ERR_DEV_NOT_EXIST The device does not exist
 * ERR_INVALID_PARAM Illegal Parameters
 * ERR_DEV_BUSY The device is occupied
 * ERR_BATTERY_ABSENT The battery does not exist
 * ERR_BATTERY_VOLTA
 * GE_TOO_LOW The battery voltage is too low
 */
int OsPrnOpen(unsigned int printertype, const char*targetname );

/*
 * Function Reset the printing parameters and clear the print cache.
 * Parameters None
 * Return None
 */
void OsPrnReset(void);
/*
 * Function Turn off the printer.
 * Parameters None
 * Return None
 * Usage: This function needs to be called when the program exits and closes the device.
 */
void OsPrnClose(void);


/*
 * Function Set printing grayscale.
 * Parameter Level
 * Level = 0, reserved.
 * Level = 1, default blackness, that is, ordinary print order.
 * Level = 2, reserved.
 * Level = 3, double-layer thermal printing.
 * Level =4, double-layer thermal printing, with higher blackness than 3.
 * Illegal value does not change the current setting.
 * Return None
 * Usage This interface is only valid for physical printers.
 */
void OsPrnSetGray(int Level);

/*
 * Function Set the print spacing.
 * parameter
 * CharSpace word spacing, unit is dot (invalid for forced non-equido width fonts, such as various Arabic fonts and Thai fonts)
 * LineSpace Line spacing, unit in number of points
 * Return None
 * usage
 * 1. After the printer is set effectively, it will remain valid until the next time OsPrnReset( );
 * 2. The default word spacing of the printer is 0;
 * 3. The default line spacing thermal sensitivity of the printer is 0 and the needle is 2;
 * 4. The maximum line spacing value of the printer is 255;
 * 5. The maximum word spacing value of the printer is 255;
 * 6. The parameter value is illegal and does not change the current setting.
 */
void OsPrnSetSpace(int CharSpace, int LineSpace);

/*
 * Function Set the font inverting attribute, and print normally by default.
 * Parameter Attr
 * 0: Normal
 * 1: Reverse
 * Return None
 * Usage: Valid for both physical and virtual printers. Invalid for printing graphics inverse display.
 */
void OsPrnSetReversal(int Attr);

/*
 * Function Set left and right margins.
 * parameter
 * Left【Input】Left margin, the effective range is [0, 100], and the default value is 0.
 * Right【Input】 Right margin, the valid range is [0, 100], and the default value is 0.
 * return
 * RET_OK Success
 * ERR_INVALID_PARAM Illegal Parameters
 * Usage If the physical printer is set to print vertically, the left margin corresponds to the upper margin of the corresponding page and the right margin corresponds to the lower margin of the corresponding page
 */
int OsPrnSetIndent (unsigned int Left, unsigned int Right);
/*
 * Function Query the current status of the printer and is only applicable to physical printers.
 * Parameters None
 * return
 * RET_OK The printer is working normally
 * ERR_PRN_BUSY The printer is busy
 * ERR_PRN_PAPEROUT Printer out of paper
 * ERR_PRN_OVERHEAT The printer is overheated
 */
int OsPrnCheck(void);


/*
 * Function Set printing parameters.Customize
 * parameter
 * FontSize Font Size 12 16 24
 * DoubleWidth double width 1~4
 * DoubleHeight 1~4
 * AlignType Left Center Right
 * Return None
 */
void  OsPrnSetPrintParams(int FontSize,int DoubleWidth, int DoubleHeight,int AlignType);
/*
 * Function Set printing parameters.Customize
 * parameter
 * FontSize Font Size 12 16 24
 * DoubleWidth double width 1~4
 * DoubleHeight 1~4
 * AlignType Left Center Right
 * IsPersian 1 Persian
 * Return None
 */
void OsPrnSetPrintParamsEx(int FontSize,int DoubleWidth, int DoubleHeight,int AlignType,int IsPersian);
/*
 * Function In the printing buffer, perform several dot lines of paper removal.
 * Parameters Pixel
 * Number of dots to execute the paper:
 * >0: Feed
 * <0: Refuse paper
 * =0: No action
 * Return None
 * Usage: Effective for both physical and virtual printers.
 */
void OsPrnFeed(int Pixel);

/*
 * Function Format the output string into the print buffer.
 * Parameter Str【Input】String pointer to print
 * Return None
 * usage
 * 1. Support variable parameters;
 * 2. Support the control characters of ‘\n’ [line break] and ‘\f’ [page break] in the printing string;
 * 3. If the print packet is too long, the program will overflow;
 * 4. If the printed string exceeds the printing range, the line will be automatically wrapped and continued to print;
 * 5. The maximum buffer is 2048 bytes;
 * 6. Save str into the print buffer, and after calling OsPrnStart( ), start printing the data in the buffer in the order of writing to the buffer.
 */
void OsPrnPrintf(const char *Str, ...);

/*
 * Function: Directly print the layouted data.
 * parameter
 * data【Input】The string pointer to print.
 * len【Input】The length of the string pointer to be printed.
 * return
 * RET_OK The printer is working normally
 * ERR_INVALID_PARAM Illegal Parameters
 * ERR_DEV_NOT_OPEN Device failed to open
 * ERR_PRN_PAPEROUT Printer out of paper
 * ERR_PRN_OVERHEAT The printer is overheated
 * ERR_PRN_OVERVOLTAGE The printer voltage is too high
 * ERR_PRN_BUSY The printer device is busy
 * usage
 * 1. After calling this function, the printer will print and will not return until the printing is finished;
 * 2. After printing, the function returns to the print status, so there is no need to query the print status again;
 */
int OsPrnRawData (const char *data, int len);

/*
 * Function Output graphics into the print buffer.
 * Parameter Logo【Input】 The path of the logo file to be printed.
 * Return None
 * Usage The steps for generating graph data are as follows:
 * 1. Draw LOGO: You can use the "Start/Programs/Attachments/Drawing" tool in Windows to draw graphics, and the storage requirements for the graphics are "monochrome, BMP format";
 * 2. Use the "Graphics Conversion Tool" provided by our company to convert the BMP graphics file into a header file such as Logo.h (multiple BMP files can be selected, and the converted header file will contain the same number of arrays as BMP, and the definition of the array name is related to the BMP file name).
 */
void OsPrnPutImage(const unsigned char *Logo);
/*
 * Function Output electronic signature monochrome bmp data to the cache area.
 * Parameters SignData Electronic Signature Monochrome BMP Data
 * len data length
 * Return None
 * Usage: cache only prints the last electronic signature set (only cache one, data is less than 10K
 */
void OsPrnPutElecSignData(unsigned char *SignData,int len);

/*
 * Function: Take electronic signature data into the printing buffer.
 * Parameters None
 * Return None
 * Usage: cache only prints the last electronic signature set (only cache one, data is less than 10K
 */
void OsPrnPutElecSignDataByJpg();
/*
 * Function Start the printer and print out the data in the buffer.
 * Parameters None
 * return
 * RET_OK The printer is working normally
 * ERR_PRN_BUSY The printer is busy
 * ERR_PRN_PAPEROUT Printer out of paper
 * ERR_PRN_WRONG_PACKAGE Packet format error
 * ERR_PRN_OVERHEAT The printer is overheated
 * ERR_PRN_OUTOFMEMORY Print data is too large
 * usage
 * 1. After calling this function, the printer will print and will not return until the printing is finished;
 * 2. After printing, the function returns to the print status, so there is no need to query the print status again;
 * 3. If the printer successfully completes the printing task and calls the function again, re-print the document.
 */
int OsPrnStart(void);

/*
 * Function Clear the print buffer
 * Parameters None
 * Return None
 */
void OsPrnClrBuf(void);

/*
 * Function Set whether the printer is pre-papered
 * Parameter cmd 【Input】 1: Print without pre-paint
 * 2: Printing pre-painted paper
 * return
 * RET_OK Success
 * ERR_INVALID_PARAM Illegal Parameters
 * Usage 1. The function only affects the printer's pre-paper function, and the default is pre-paper.2. Only thermal printers are supported.
 */
int OsPrnSetParam(unsigned int cmd);


/*
 * 13 code
 */

/*
 * Function Character encoding conversion.
 * parameter
 * FromCharset 【Input】Original character encoding
 * ToCharset【Input】 Target character encoding
 * InBuf 【Input】 The original encoded string has the ending character with ‘\0’; unicode encoding needs to end with ‘\0\0’.
 * OutBuf 【Output】Converted target encoding string
 * LenOut 【Input】The size of the OutBuf array is at least 1.5 times the size of the InBuf array.
 * return
 * >=0 Conversion is successful, return the converted string length
 * ERR_INVALID_PARAM Illegal Parameters
 */
int OsCodeConvert (const char *FromCharset, const char *ToCharset,const char *InBuf, char *OutBuf, unsigned int LenOut);

/*
 * 14 Magnetic Stripe Reader
 */
// 14 Magnetic Stripe Reader
#define ERR_MSR_FAILED -2701 //operation failed
#define ERR_MSR_HEADERR -2702 //No starting symbol found
#define ERR_MSR_ENDERR -2703 //No terminator found
#define ERR_MSR_LRCERR -2704 //LEC inspection error
#define ERR_MSR_PARERR -2705 //Data verification error in a certain position of the magnetic track
#define ERR_MSR_NOT_SWIPED -2706 //No card swipe
#define ERR_MSR_NO_DATA -2707 //Magnetic stripe card has no data
#define ERR_MSR_END_ZEROERR -2708 //Magnetic stripe card data format error
#define ERR_MSR_PED_DECRYPTERR -2709 //PED decryption failed
#define ERR_MSR_NO_TRACK_ERR -2710 //No corresponding track was detected on the magnetic card

// ST_MSR_DATA track information structure
typedef struct
{
    unsigned char TeackData[256]; /*Decoded bitstream*/
    int DataLen; /*Track data length*/
    int Status; /*When the track data status is equal to 0, it indicates successful data reading;Failed to read other value identifiers*/
}ST_MSR_DATA;


/*
 * Turn on the magnetic stripe reader device.
 * Parameters None
 * return
 * RET_OK Success
 * ERR_DEV_NOT_EXIST The device does not exist
 * ERR_DEV_BUSY The device is occupied
 */
int OsMsrOpen(void);
/*
 * Turn off the magnetic stripe reader device.
 * Parameters None
 * Return None
 * Usage: This function needs to be called when the program exits and closes the device.
 */
void OsMsrClose(void);
/*
 * Soft reset the magnetic stripe card reader and clear the read magnetic stripe card data.
 * Parameters None
 * Return None
 */
void OsMsrReset(void);
/*
 * Check whether to swipe the card.
 * Parameters None
 * return
 * TRUE
 * FALSE Not brushed
 * ERR_DEV_NOT_OPEN The device is not turned on
 */
int OsMsrSwiped(void);
/*
 * Read the magnetic stripe card data.
 * Parameter Track1【Output】 Output 1 Track data
 * Track2【Output】 Output 2 Track data
 * Track3【Output】 Output 3 track data
 * return
 * RET_OK successfully read
 * ERR_MSR_NOT_SWIPED No card swipe
 * ERR_INVALID_PARAM Illegal Parameters
 * ERR_DEV_NOT_OPEN The device is not turned on
 */
int OsMsrRead(ST_MSR_DATA *Track1, ST_MSR_DATA *Track2, ST_MSR_DATA *Track3);
/*
 * Read data from a common single-sided magnetic stripe card or a double-sided magnetic stripe card encoded by JIS.
 * parameter
 * Track1【Output】 Output 1 Track data
 * Track2【Output】 Output 2 Track data
 * Track3【Output】 Output 3 track data
 * Track4【Output】 Output 4 track data
 * return
 * RET_OK successfully read
 * ERR_MSR_NOT_SWIPED No card swipe
 * ERR_INVALID_PARAM Illegal Parameters
 * ERR_DEV_NOT_OPEN The device is not turned on
 */
int OsMsrReadJIS(ST_MSR_DATA *Track1, ST_MSR_DATA *Track2, ST_MSR_DATA *Track3, ST_MSR_DATA *Track4);



/*
 * 15IC card reader
 */

#define ERR_SCI_HW_NOCARD -2800 //No cards
#define ERR_SCI_HW_STEP -2801//Not initialized during data exchange, not powered on during hot reset.
#define ERR_SCI_HW_PARITY -2802 //Parity check failed
#define ERR_SCI_HW_TIMEOUT -2803 //timeout
#define ERR_SCI_TCK -2804 //TCK error
#define ERR_SCI_ATR_TS -2810 //ATR TS error
#define ERR_SCI_ATR_TA1 -2811 //ATR TA1 error
#define ERR_SCI_ATR_TD1 -2812 //ATR TD1 error
#define ERR_SCI_ATR_TA2 -2813 //ATR TA2 error
#define ERR_SCI_ATR_TB1 -2814 //ATR TB1 error
#define ERR_SCI_ATR_TB2 -2815 //ATR TB2 error
#define ERR_SCI_ATR_TC2 -2816 //ATR TC2 error
#define ERR_SCI_ATR_TD2 -2817 //ATR TD2 error
#define ERR_SCI_ATR_TA3 -2818 //ATR TA3 error
#define ERR_SCI_ATR_TB3 -2819 //ATR TB3 error
#define ERR_SCI_ATR_TC3 -2820 //ATR TC3 error
#define ERR_SCI_T_ORDER -2821 //The protocol is not T0 or T1
#define ERR_SCI_PPS_PPSS -2830 //PPSS error on PPS
#define ERR_SCI_PPS_PPS0 -2831 //PPS0 error on PPS
#define ERR_SCI_PPS_PCK -2832 //ATR PCK error on PPS
#define ERR_SCI_T0_PARAM -2840 //The data transmitted on track 0 is too long.
#define ERR_SCI_T0_REPEAT -2841 //Character repetition on track 0 exceeds the maximum limit
#define ERR_SCI_T0_PROB -2842 //T0 program byte error occurred
#define ERR_SCI_T1_PARAM -2850 //The data transmitted in T1 is too long
#define ERR_SCI_T1_BWT -2851 //T1 BWT Too large
#define ERR_SCI_T1_CWT -2852 //T1 CWT Too large
#define ERR_SCI_T1_BREP -2853 //T1 Too many repeated blocks
#define ERR_SCI_T1_LRC -2854 //T1LRC error
#define ERR_SCI_T1_NAD -2855 //T1NAD error
#define ERR_SCI_T1_LEN -2856 //T1 LEN error
#define ERR_SCI_T1_PCB -2857 //T1 PCB error
#define ERR_SCI_T1_SRC -2858 //T1SRC error
#define ERR_SCI_T1_SRL -2859 //T1SRL error
#define ERR_SCI_T1_SRA -2860 //T1SRA error
#define ERR_SCI_PARAM -2880 //illegal parameter

typedef struct
{
    unsigned char Cmd[4]; /*CLA, INS, P1, P2*/
    int LC; /*Effective data length of DataIn sent to IC card */
    unsigned char DataIn[512]; /*Data sent to IC card*/
    int LE; /*Expected length of returned data*/
}ST_APDU_REQ;

typedef struct
{
    int LenOut; /* The actual length of data returned from the IC card*/
    unsigned char DataOut[512]; /*Data pointer returned from IC card*/
    unsigned char SWA; /*IC card status word 1*/
    unsigned char SWB; /*IC card status word 2*/
}ST_APDU_RSP;

enum{
    ICC_USER_SLOT = 0,   //user card
    ICC_SAM1_SLOT, //SAM Kaka Seat1
    ICC_SAM2_SLOT, //SAM Kaka Seat 2
    ICC_SAM3_SLOT, //SAM Kaka Seat 3
    ICC_SAM4_SLOT, //SAM Kaka Seat 4
};

/*
 * Function Turn on the IC card reading device.
 * Parameter Slot
 * IC card channel number:
 * ICC_USER_SLOT User Card
 * ICC_SAM1_SLOT SAM card holder 1
 * ICC_SAM2_SLOT SAM card holder 2
 * ICC_SAM3_SLOT SAM card holder 3
 * ICC_SAM4_SLOT SAM card holder 4
 * return
 * RET_OK Success
 * ERR_DEV_NOT_EXIST The device does not exist
 * ERR_DEV_BUSY The device is occupied
 */
int OsIccOpen(int Slot);

/*
 * Function Check whether the designated card holder has a card inserted.
 * Parameter Slot
 * IC card channel number:
 * ICC_USER_SLOT User Card
 * ICC_SAM1_SLOT SAM card holder 1
 * ICC_SAM2_SLOT SAM card holder 2
 * ICC_SAM3_SLOT SAM card holder 3
 * ICC_SAM4_SLOT SAM card holder 4
 * return
 * RET_OK Insert with card
 * Others Please refer to the function return value list
 */
int OsIccDetect(int Slot);

/*
 * Function Initialize the IC card device.
 * parameter
 * Slot
 * IC card channel number:
 * ICC_USER_SLOT User Card
 * ICC_SAM1_SLOT SAM card holder 1
 * ICC_SAM2_SLOT SAM card holder 2
 * ICC_SAM3_SLOT SAM card holder 3
 * ICC_SAM4_SLOT SAM card holder 4
 * Option(bits 0~1) card voltage selection: 00 - 5V, 01 - 1.8V, 10 - 3V
 * (bit 2) 0;
 * (bits 3~4) 0
 * (bit 5) indicates the supported specification type: 0 - EMV specification 1 - ISO7816 specification
 * (bits 6 ~ 31) Reserved: Option default setting is 0 (ie: 5V, non-PPS, standard rate and EMVx compliant)
 * Atr【Output】
 * 1. Answer To Reset, the card returns up to 34 bytes of response data.
 * 2. Its content is: reset response content length (1 byte) + reset response content
 * return
 * RET_OK Success
 * Others Please refer to the function return value list
 */
int OsIccInit(int Slot,unsigned long Option,unsigned char *Atr);

/*
 * Function Interact commands with the IC card.
 * parameter
 * Slot
 * IC card channel number:
 * ICC_USER_SLOT User Card
 * ICC_SAM1_SLOT SAM card holder 1
 * ICC_SAM2_SLOT SAM card holder 2
 * ICC_SAM3_SLOT SAM card holder 3
 * ICC_SAM4_SLOT SAM card holder 4
 * CtrlFlag
 * 1. Bit0 means whether it is automatically sent under T=0 protocol
 * GET RESPONSE directive:
 * 1-Yes
 * 0-No
 * 2. Bit1~Bit31 Reserved
 * ApduReq【Input】Send command data structure to IC card
 * ApduRsp【Output】 The data structure that receives responses from the IC card
 * return
 * RET_OK Success
 * Others Please refer to the function return value list
 */
int OsIccExchange(int Slot,int CtrlFlag,const ST_APDU_REQ *ApduReq,ST_APDU_RSP *ApduRsp);

/*
 * Function Transparent transmission/receiving function.
 * Parameter Slot
 * IC card channel number:
 * ICC_USER_SLOT User Card
 * ICC_SAM1_SLOT SAM card holder 1
 * ICC_SAM2_SLOT SAM card holder 2
 * ICC_SAM3_SLOT SAM card holder 3
 * ICC_SAM4_SLOT SAM card holder 4
 * 
 * CtrlFlag Reserved
 * pucTxBuff【Input】 Data buffer to be transferred
 * iTxLen 【Input】The length of data to be transferred, in bytes
 * pucRxBuff【Output】Buffer for receiving card response data
 * piRxLen【Output】 The number of card data bytes received
 * return
 * 0 Success
 * Other Failed (see function return value list)
 */
int OsIccTransfer(int Slot,int CtrlFlag, const unsigned char *pucTxBuff,int iTxLen,unsigned char *pucRxBuff,int *piRxLen);
/*
 * Function Turn off the IC card device.
 * Parameter Slot
 * IC card channel number:
 * ICC_USER_SLOT User Card
 * ICC_SAM1_SLOT SAM card holder 1
 * ICC_SAM2_SLOT SAM card holder 2
 * ICC_SAM3_SLOT SAM card holder 3
 * ICC_SAM4_SLOT SAM card holder 4
 * return
 * RET_OK Success
 * Others Please refer to the function return value list
 */
int OsIccClose(int Slot);


/*
 * 16 RF card reader
 */
#define PCD_ERR_PAR_FLAG -2901 //Verification error
#define PCD_ERR_CRC_FLAG -2902 //CRC error
#define PCD_ERR_WTO_FLAG -2903 //Timeout or no card
#define PCD_ERR_COLL_FLAG -2904 //Multiple card conflicts
#define PCD_ERR_ECD_FLAG -2905 //Frame format error
#define PCD_ERR_EMD_FLAG -2906 //interference
#define PCD_ERR_COM_FLAG -2907 //Chip malfunction, unable to communicate correctly
#define PCD_ERR_AUT_FLAG -2908 //M1 Authentication error
#define PCD_ERR_TRANSMIT_FLAG -2909 //transmission error
#define PCD_ERR_PROTOCOL_FLAG -2910 //protocol error
#define PCD_ERR_PARAMFILE_FLAG -2911 //The configuration file does not exist
#define PCD_ERR_USER_CANCEL -2912 //User cancels transaction
#define PCD_ERR_CARRIER_OBTAIN_FLAG -2913 //Carrier not obtained
#define PCD_ERR_CONFIG_FLAG -2914 //Failed to configure register
#define PCD_ERR_RXLEN_EXCEED_FLAG -2915 //The length of the data returned by the card exceeds the set receiving length
#define PCD_ERR_NOT_ALLOWED_FLAG -2951 //Parameter error or value not allowed
#define PCD_CHIP_ABNORMAL -2952 //The chip does not exist or is abnormal
#define PCD_CHIP_NOT_OPENED -2953 //Module not opened
#define PCD_CHIP_CARDEXIST -2954 //The card has not been moved away
#define PCD_ERR_NOT_IDLE_FLAG -2955 //The card is not in idle state
#define PCD_ERR_NOT_POLLING_FLAG -2956 //The card has not been POLLING processed
#define PCD_ERR_NOT_WAKEUP_FLAG -2957 //The card did not wake up
#define PCD_ERR_NOT_ACTIVE_FLAG -2958 //Card not activated
#define PCD_ERR_NOT_SUPPORT -2959 //The chip does not support

typedef struct pcd_user_t{
    unsigned char wait_retry_limit_w; /* S(WTX)Response sending times allowed to be written*/
    unsigned int wait_retry_limit_val; /* S(WTX)Maximum number of retries for response*/
    unsigned char check_cancel_key_w; /*Response cancel key write allowed*/
    unsigned char check_cancel_key_val; /* 0 Indicates no response to the cancel button，1 Indicate response to cancel button*/
    int (*check_cancel_key_function)(void);/**/
    unsigned char os_picc_transfer_set_w; /**/
    unsigned char os_picc_transfer_set_val; /**/
    unsigned char anti_interference_flag; /**/
    unsigned char protocol_layer_fwt_set_w; /**/
    unsigned int protocol_layer_fwt_set_val; /**/
    unsigned char os_picc_transfer_rxlen_set_w; /* */
    unsigned int os_picc_transfer_rxlen_set_val; /**/
    unsigned char os_picc_transfer_direct_transmit_set_w; /**/
    unsigned char configure_technology_type_w; /* */
    unsigned char configure_technology_type_val; /**/
    unsigned char reserved[34]; /**/
}PCD_USER_ST;

/*
 * Function The PCD module is powered on and processed to make the module enter the ready state.
 * Parameters None
 * return
 * 0 The device is successfully turned on
 * ERR_BATTERY_VOLTAG
 * E_TOO_LOW The battery voltage is too low
 * ERR_BATTERY_ABSENT The battery does not exist
 * Others Failed to open the device (see function return value list)
 */
int OsPiccOpen(void);

/*
 * Function PCD module power-down processing.
 * Parameters None
 * return
 * 0 Success
 * Other Failed (see function return value list)
 */
int OsPiccClose(void);

/*
 * Function Carrier reset.
 * Parameters None
 * return
 * 0 Carrier reset successfully
 * Other Carrier reset failed (see function return value list)
 */
int OsPiccResetCarrier(void);


/*
 * Function Card search, temporarily only includes two types of wheel searches: "A" and "B" cards.
 * parameter
 * pcPiccType【Output】
 * Find card type:
 * "A" - A Card
 * "B" - B card
 * pucATQx 【Output】
 * Card response data when looking for cards:
 * A card is 2 bytes
 * The B card is 12 bytes
 * return
 * 0 Successfully searched for the card
 * Others: Card search failed (see function return value list)
 */
int OsPiccPoll(char*pcPiccType, unsigned char*pucATQx);


/*
 * Function Transparent transmission/receiving function.
 * Parameter pucTxBuff【Input】 Data buffer to be transferred
 * iTxLen 【Input】The length of data to be transferred, in bytes
 * pucRxBuff【Output】Buffer for receiving card response data
 * piRxLen【Output】 The number of card data bytes received
 * return
 * 0 Success
 * Other Failed (see function return value list)
 */
int OsPiccTransfer(const unsigned char*pucTxBuff,int iTxLen, unsigned char*pucRxBuff, int *piRxLen);

/*
 * Function: Follow EMV mode to move the card.
 * Parameters None
 * return
 * 0 Success
 * Others Failed
 */
int OsPiccRemove (void);

/*
 * Function On the specified channel, sends APDU format data to the card and receives a response.
 * parameter
 * cid 【Input】 Used to specify the card logical channel number.
 * The value range is 0~14, and the values ​​are all 0.
 * ApduReq 【Input】Send to PICC card Command Data Structure
 * ST_APDU_REQ
 * ApduRsp 【Output】Data structure returned from PICC card
 * ST_APDU_RSP
 * return
 * 0 Success
 * Other Failed (see function return value list)
 */
int OsPiccIsoCommand(int cid, ST_APDU_REQ*ApduReq,ST_APDU_RSP*ApduRsp);

/*
 * Function Turn off the carrier.
 * Parameters None
 * return
 * 0 The carrier is successfully turned off
 * Other Failed (see function return value list)
 * Usage: Carrier-off operation is performed on the contactless IC card reader, and the status of the card in the RF field will become a power-off state.
 */
int OsPiccOffCarrier(void);

/*
 * Features Find the Felica card.
 * parameter
 * pucResp 【Output】
 * Card response data when looking for a card: Data format: 1 byte LEN + n byte response; LEN = n + 1
 * piRespLen【Output】 The number of bytes of the received card response data
 * return
 * 0 Successfully searched for the card
 * Others: Card search failed (see function return value list)
 */
int OsPiccFelicaPoll(unsigned char*pucResp,int *piRespLen);

/*
 * Function Transparent transmission/receiving function.
 * Parameter pucTxBuff【Input】 Data buffer to be transferred Data format: 1 byte LEN + n byte command; LEN = n + 1
 * iTxLen 【Input】The length of data to be transferred, in bytes
 * pucRxBuff【Output】 Buffer for receiving card response data Data format: 1 byte LEN + n byte response; LEN = n + 1
 * piRxLen【Output】 The number of card data bytes received
 * return
 * 0 Success
 * Other Failed (see function return value list)
 */
int OsPiccFelicaTransfer(const unsigned char*pucTxBuff,int iTxLen, unsigned char*pucRxBuff, int *piRxLen);

/*
 * 17 Communication Port
 */
/*
 * Function Switch usb to serial port.
 * Parameters None
 * return
 * 0 Success
 * Others Failed
 */
int OsStartUsbSerial();

/*
 * Function Get the serial port number.
 * Parameter SerialPort【Output】, cannot be NULL
 * return
 * 0 Success
 * Others Failed
 */
int OsQuerySerialPort(char* SerialPort);
/*
 * Function Detect whether the port exists
 * parameter
 * return
 * 0 Success
 * Others Failed
 */
int OsCheckSerialPortExist(char* SerialPort,int TimeOut_ms);
/*
 * Function Switch the serial port to usb.
 * Parameters None
 * return
 * 0 Success
 * Others Failed
 */
int OsStopUsbSerial();

int OsStartCommModuleUsbSerial();
int OsStopCommModuleUsbSerial();
int OsStartUsbSerialWithUsbcharge();

/*
 * 21GPRS/CDMA wireless module
 */
#define ERR_NET_IF -3307  //The network interface link is unavailable (the link has not been established or there are no corresponding devices)
#define PPP_LOGINING 1  //PPP Logging in
#define PPP_LOGOUTING 2 //PPP Logging out
#define WL_CSD_READY 3 //CSD Dialing service ready
#define WL_GPRS_CSD_READY 4 //GPRS and CSD Dialing service ready
#define ERR_WL_POWER_ONING -3501 //Wireless module powered on
#define ERR_WL_POWER_OFF -3502 //The wireless module is not powered on
#define ERR_WL_NOT_INIT -3503 //Module not initialized
#define ERR_WL_NEEDPIN -3504 //SIM card requires PIN
#define ERR_WL_RSPERR -3505 //Module response error
#define ERR_WL_NORSP -3506 //Module not responding
#define ERR_WL_NEEDPUK -3507 //SIM card requires PUK
#define ERR_WL_WRONG_PIN -3508 //SIM card PIN error
#define ERR_WL_NOSIM -3509 //No SIM card inserted
#define ERR_WL_NOREG -3510 //Cannot register on the network
#define ERR_WL_AUTO_RST -3511 //Module automatic reset
#define ERR_WL_BUF -3512 //Wireless module memory error
#define ERR_WL_GET_SIGNAL -3513 //Signal acquisition, please wait for 3 seconds
#define ERR_WL_NOTYPE -3514 //Module cannot be recognized
#define ERR_WL_PPP_ONLINE -3515 //Module PPP online, unable to sleep
#define ERR_WL_ERR_BUSY -3516 //Module busy
#define ERR_WL_SLEEP_ONING -3517 //The wireless module is entering sleep mode
#define ERR_WL_SLEEP_FAIL -3518 //Wireless module failed to enter sleep mode
#define ERR_WL_SIM_FAILURE -3519 //Wireless module failed to operate SIM card

/*
 * Function Turn on the wireless module.Establish a connection to the Prolin wireless service and obtain permissions to use wireless devices/modules.
 * Parameters None
 * return
 * RET_OK Open the wireless module successfully
 * ERR_DEV_BUSY module/device has been occupied by other applications
 * ERR_DEV_NOT_EXIST module/device does not exist
 * ERR_BATTERY_VOLTA
 * GE_TOO_LOW The battery voltage is too low
 * ERR_BATTERY_ABSENT Terminal battery does not exist
 */
int OsWlLock(void);

/*
 * Prototype void OsWlUnLock(void);
 * Function Free the right to use the wireless device and disconnect from the Prolin wireless service.
 * Parameters None
 * Return None
 */
void OsWlUnLock(void);

/*
 * Function Initialize the wireless device.
 * Parameter SimPin【Input】
 * The password string pointer of the SIM card, with a length less than 50 bytes.Can be NULL, indicating that no password is required.
 * return
 * RET_OK Success
 * ERR_DEV_NOT_OPEN The device/module is not turned on
 * ERR_DEV_NOT_EXIST The wireless device/module does not exist
 * ERR_NO_PORT The terminal physical serial port is insufficient
 * ERR_WL_NEEDPIN SIM card requires PIN
 * ERR_WL_RSPERR module/device response error
 * The ERR_WL_NORSP module does not respond
 * ERR_WL_NEEDPUK SIM card requires PUK
 * ERR_WL_WRONG_PIN PIN Error
 * ERR_WL_NOSIM No SIM card
 * ERR_WL_NOTYPE module type cannot be recognized
 * ERR_WL_NOREG Unable to register with GPRS network
 */
int OsWlInit(const char *SimPin);

/*
 * Function Get wireless signal strength.
 * Parameters None
 * return
 * 0~5 Signal strength level: 0 means no signal; 5
 * Indicates the strongest signal.
 * ERR_DEV_NOT_EXIST The wireless module does not exist
 * ERR_WL_RSPERR module response error
 * ERR_WL_POWER_ONING The wireless module is in power-off state
 */
int OsWlGetSignal(void);

/*
 * Function Query the wireless link status.
 * Parameters None
 * return
 * PPP_LOGOUTING Disconnecting
 * PPP_LOGINING Link is being created
 * RET_OK Link creation was successful
 * ERR_DEV_NOT_EXIST The wireless module does not exist
 * ERR_WL_POWER_ONING The wireless module is powered on
 * ERR_WL_POWER_OFF Wireless module powered off
 * The ERR_WL_NOT_INIT module is not initialized
 * ERR_NET_PASSWD Password Error
 * ERR_NET_LOGOUT The application is actively disconnected
 * The ERR_NET_IF link is unavailable, indicating that the link is not established or has been disconnected
 */
int OsWlCheck(void);

/*
 * Function Log in to the wireless network and establish a wireless link.
 * parameter
 * APN【Input】
 * GPRS is the wireless access point name (AccessPointName).
 * CDMA is a dialing number.The length is not more than 50 characters; when NULL, the application dials itself first, and the protocol stack directly performs PPP login.
 * Name【Input】The user name cannot exceed 50 bytes in length; NULL is not allowed; when there is no user name, it is represented by the empty string "".
 * Password【Input】 Password, the length cannot exceed 50 bytes; it is not allowed to be NULL; when there is no password, it is represented by the empty string "".
 * The algorithm used during Auth authentication
 * TimeOutMs Timeout time, unit is milliseconds; the value range is 0~3600000.When it is less than 0, it is automatically set to 0; when it is greater than 3600000, it is automatically set to 3600000.
 * KeepAlive link check interval, unit is milliseconds; the value range is 0~3600000; when 0, the KeepAlive function is not enabled; when 0~10000, it is automatically set to 10000; when 10000~3600000, the link check will be performed according to the set value.
 * ReserParam retains parameters for extension use.
 * return
 * PPP_LOGINING is being processed
 * PPP_LOGOUTING The wireless module is logging out
 * RET_OK Link establishment successfully
 * ERR_DEV_NOT_EXIST No wireless module
 * ERR_DEV_NOT_OPEN No execution OsWlLock succeeded
 * ERR_INVALID_PARAM Illegal Parameters
 * ERR_WL_POWER_ONING The wireless module is powered on
 * ERR_WL_POWER_OFF The wireless module is not powered on
 * ERR_WL_NOT_INIT No initialization successful
 * ERR_NET_PASSWD Error password
 * ERR_NET_SERVER_BUSY The server is busy and communication failed
 * ERR_NET_AUTH Unable to connect to the RADIUS server
 */
int OsWlLogin(const char *APN, const char *Name, const char *Password, long Auth, int TimeOutMs, int KeepAlive, int ReserParam);

/*
 * Function Exit the wireless network and disconnect the wireless link.
 * Parameters None
 * return
 * PPP_LOGOUTING Link is disconnected
 * ERR_DEV_NOT_OPEN module/device is not turned on
 */
int OsWlLogout(void);

// TODO Added Interface
// Get the sim card status
int OsWlGetSimStatus(int* simStatus);
// Get the operator
int OsWlGetSimOperator(char* simOperator);
// Get IMEI
int OsWlGetImei(char* Imei);
// Get Imsi
int OsWlGetImsi(char* Imsi);
// Get Iccid
int OsWlGetIccid(char* Iccid);
// Obtain signal strength
int OsWlGetSignalStrength(void);
// Get module REG
int OsWlGetModuleREG(int *state, char* lac, char* cid);
// Get IP
int OsWlGetNetworkIp(char* ipaddr);

int OsWlGetQNetworkInfo(char* networkType, char *oper);
// //获取自动连接状态
// int OsGetAutoConnectStatus(int *status);
// //设置自动连接状态
// int OsSetAutoConnectStatus(int onoff);
// Get the wireless automatic connection status
int OsWlGetAutoConnectStatus(int *status);
// Set the automatic connection status
int OsWlSetAutoConnectStatus(int onoff);
// Set the flight mode
int OsSetAirplaneMode(int onoff);
// Get the flight mode status
int OsGetAirplaneMode(int *onoff);
// Setting up an APN
int OsSetApnParams(char* apn, char* user,char* password);
// Get APN
int OsGetApnParams(char* apn, char* user,char* password);
// Get the current network type 0: NOSERVICE networkless, 1:GSM 2G, 2:LTE 4G
int OsWLGetNetworkType(int* type);
// Open Send AT
int OsCustATCmdOpen();
// Send AT
int OsSendCustATCmd(char* atCmd, char* resp);
// Close send AT
void OsCustATCmdClose();
// Power off the module
int OsWLModulePowerDown(void);
// Power on the module
int OsWLModulePowerOn(void);
/*
 * 22 WiFi
 */
// Function return value list
#define ERR_MODE_NOT_SUPPORT -3350 //Mode setting error
#define ERR_WIFI_POWER_OFF -3351 //Module not powered on
#define ERR_NOT_FOUND_AP -3352 //AP not found
#define ERR_AUTH_SEC_MODE -3353 //Authentication mode or encryption mode error
#define ERR_WIFI_BAD_SIGNAL -3354 //Poor WiFi signal
#define ERR_NET_PASSWD -3355 //Password error
#define RET_CONNECTING 1 //connecting
#define ERR_EAP_ID -3359 //Certificate chain error or certificate verification failure

// Encryption type list
#define PARE_CIPHERS_NONE 0x00000000 //no encryption
#define PARE_CIPHERS_WEP64 0x00000001 //WEP 40bit secret key
#define PARE_CIPHERS_WEP128 0x00000002 //WEP 104bit secret key
#define PARE_CIPHERS_WEPX 0x00000004 //WEP encryption, unknown key bit
#define PARE_CIPHERS_CCMP 0x00000010 //AES encryption method
#define PARE_CIPHERS_TKIP 0x00000020 //TKIP encryption method

// Authentication mode:
enum WIFI_AUTH_MODE{
    AUTH_NONE_OPEN=1,
    AUTH_NONE_WEP,
    AUTH_NONE_WEP_SHARED, /*This mode is set to 'AUTH_NONE_WEP' during scanning  */
    AUTH_IEEE8021X,
    AUTH_WPA_PSK,
    AUTH_WPA_EAP,
    AUTH_WPA_WPA2_PSK,
    AUTH_WPA_WPA2_EAP,
    AUTH_WPA2_PSK,
    AUTH_WPA2_EAP
};

// WEP64, WEP128 extension
typedef struct _WepSecKey{
    char Key[4][40]; /* WEP key data */
    int KeyLen; /* WEP key length */
    int Idx; /* WEP key index [0,3] */
} WEP_SEC_KEY;

// WPA/WPA2-PSK extension
typedef struct _WpaPskKey{
    char Key[64]; /* PSK Key Data*/
    int KeyLen; /* PSK Key Length */
} WPA_PSK_KEY;

// EAP Extensions
typedef struct _WpaEapKey{
    int EapType; /* EAP type */
    char Pwd[132]; /* password */
    char Id[132]; /* characteristic */
    char CaCert[132]; /* Path and file name of CA certificate */
    char CliCert[132];/* The path and file name of the client certificate */
    char PriKey[132]; /* The private key file from the file path to the client */
    char PriKeyPwd[132]; /* Password private key file*/
} WPA_EAP_KEY;

// Scan the hot spot information
typedef struct _WifiApInfo
{
    char Essid[33];/* AP name */
    char Bssid[20];/* MAC address*/
    int Channel; /* channel */
    int Mode; /* Connection method, 0:Station; 1:IBSS */
    int Rssi; /* Signal value, range [-99, 0]*/
    int AuthMode; /* authentication mode */
    int SecMode; /* encryption mode, NONE,WEP,TKIP,CCMP */
}ST_WifiApInfo;

// Connection hotspot settings:
typedef struct _WifiApSet{
    char Essid[33]; /* AP name, maximum supported 32 bytes, ending with '\ 0' */
    char Bssid[20];/*MAC address, ending with '\ 0'; If there is no AP with the same ESSID, Bscid can be '\ 0'*/
    int Channel; /* Channel, only valid in IBSS mode, 0: default setting*/
    int Mode; /* Connection method, 0: Station;  1:IBSS */
    int AuthMode; /* authentication mode*/
    int SecMode; /* encryption mode, NONE,WEP,TKIP,CCMP */
    union KEY_UNION{ /* Key setting */
        WEP_SEC_KEY WepKey; /* WEP mode*/
        WPA_PSK_KEY PskKey; /* wpa,wpa2-psk mode*/
        WPA_EAP_KEY EapKey; /* wpa,wpa2-eap mode*/
    } KeyUnion;
}ST_WifiApSet;

// WPS mode
typedef enum 
{
    WPS_MODE_PBC = 1,    /* Button based connection, also known as PBC connection*/
    WPS_MODE_PIN_CLIENT, /*Connect using the PIN code generated by the terminal*/
    WPS_MODE_PIN_AP      /*Connect using the PIN code on the AP end*/
}WPS_MODE;

/*
 * Function Connect to WiFi service and obtain access.
 * Parameters None
 * return
 * RET_OK Success
 * ERR_DEV_NOT_EXIST WiFi module driver loads abnormally or the module is incorrect
 * ERR_DEV_BUSY WiFi has been used
 * ERR_BATTERY_VOLTAG
 * E_TOO_LOW The battery voltage is too low
 * ERR_BATTERY_ABSENT The battery does not exist
 */
int OsWifiOpen(void);

/*
 * Function Release the use of the WiFi module. After calling this function, WiFi communication will not be affected.
 * Parameters None
 * Return None
 */
void OsWifiClose(void);


/*
 * Function Scan the existing network.
 * Parameters Aps【Output】 Scan the hot spot information structure ST_WifiApInfo, store the scanned network information
 * return
 * >=0 Number of APs found
 * ERR_MEM_FAULT Memory Error
 * ERR_INVALID_PARAM parameter error
 * ERR_WIFI_POWER_OFF WiFi module is not powered on
 * ERR_DEV_NOT_OPEN No WiFi device usage rights were obtained
 * Usage: Aps needs to be released after running out
 */
int OsWifiScan (ST_WifiApInfo **Aps);

/*
 * Function Connect to the specified wireless network.
 * parameter
 * Ap【Input】 Connect to the hotspot setting structure ST_WifiApSet to store the specified wireless network attributes.
 * TimeOutMs【Input】 Timeout time, [Unit: milliseconds]; the value range is 0~3600000.
 * return
 * RET_OK Connection is successful
 * RET_CONNECTING Connecting
 * ERR_NOT_CONNECT Connection failed
 * ERR_WIFI_BAD_SIGNAL WiFi signal difference
 * ERR_NOT_FOUND_AP AP not found
 * ERR_NET_PASSWD Password Error
 * ERR_AUTH_SEC_MODE Authentication mode or encryption mode error
 * ERR_WIFI_POWER_OFF WiFi module is not powered on
 * ERR_DEV_NOT_OPEN No WiFi device usage rights were obtained
 * ERR_INVALID_PARAM parameter error
 */
int OsWifiConnect(const ST_WifiApSet *Ap,int TimeOutMs);

/*
 * Function Disconnect from the current network.
 * Parameters None
 * return
 * RET_OK Disconnected
 * ERR_DEV_NOT_OPEN No WiFi device usage rights were obtained
 */
int OsWifiDisconnect(void);

/*
 * Function Get the current network status of the terminal.
 * parameter
 * Essid【Output】 The ESSID of the currently connected network cannot be NULL, and the length is 33 bytes.
 * Bssid [Output] The BSSID of the currently connected network can be NULL and has a length of 20 bytes.
 * Rssi【Output】 Signal strength, cannot be NULL.The value range is [-99, 0], and 0 means the signal is the strongest.
 * return
 * RET_OK Connection is successful
 * RET_CONNECTING Connecting
 * ERR_NOT_CONNECT Not connected to the network
 * ERR_WIFI_BAD_SIGNAL WiFi signal difference
 * ERR_NOT_FOUND_AP AP not found
 * ERR_NET_PASSWD Password Error
 * ERR_AUTH_SEC_MODE Authentication mode or encryption mode error
 * ERR_INVALID_PARAM parameter error
 */
int OsWifiCheck(char *Essid,char *Bssid,int *Rssi);

/*
 * Function Send commands to the WPA_Supplicant background service and get the return result.
 * parameter
 * Argv【Input】 The command supported by WPA_Supplicant cannot be NULL.
 * Argc【Input】 Argv The number of commands or parameters stored in a two-dimensional array.
 * Result【Output】 WPA_Supplicant returns the result, cannot be NULL, the length is greater than 2048 bytes.
 * Len【Input】Result array length
 * return
 * RET_OK Send successfully
 * ERR_INVALID_PARAM parameter error
 * ERR_WIFI_POWER_OFF WiFi module is not powered on
 * ERR_DEV_NOT_OPEN No WiFi device usage rights were obtained
 */
int OsWifiCmd (const char *Argv[],int Argc,char *Result,int Len);

// Detect wifi status
int OsGetWifiStatus(char *Essid,char *Bssid,char* Ip,int *Rssi);
// Detect wifi signal mild 0 not connected 1~4 grids
int OsGetWifiSignalStrength();

int OsWifiGetAutoConnectStatus(int *status);

int OsWifiSetAutoConnectStatus(int onoff);

int OsWifiReconnect();

int OsWifiModuleWakeUp();

int OsWifiModuleSleep();
/*
 * 29 Power Management
 */
// 29 Power Management
#define BATTERY_LEVEL_0 0
#define BATTERY_LEVEL_1 1
#define BATTERY_LEVEL_2 2
#define BATTERY_LEVEL_3 3
#define BATTERY_LEVEL_4 4
#define BATTERY_LEVEL_CHARGE 5
#define BATTERY_LEVEL_COMPLETE 6
#define BATTERY_LEVEL_ABSENT 7

// Events broadcast by PM_MSG_T pm
typedef enum{
    PM_MSG_NO_EVEN, /*no information*/
    PM_MSG_ENTER_SLEEP, /*The machine enters sleep mode*/
    PM_MSG_EXIT_SLEEP,  /*Machine exits sleep mode*/
    PM_MSG_ENTER_SCREENSAVER,   /*The machine enters the screensaver*/
    PM_MSG_EXIT_SCREENSAVER,    /*Machine exits screensaver*/
    PM_MSG_ENTER_POWEROFF,  /*The machine is starting to shut down*/
    PM_MSG_OWER_ABORMAL,    /*Machine power supply abnormality*/
    PM_MSG_VBATTERY_DAMAGE, /*Machine battery damage*/
}PM_MSG_T;

// PM_REQ_T Action requested by the client
typedef enum{
    PM_FORBID_SLEEP,    /*Prevent the machine from entering sleep mode*/
    PM_ALLOW_SLEEP,    /*Allow the machine to enter sleep mode*/
    PM_FORBID_SCREENASVER,    /*Prevent the machine from entering the screensaver*/
    PM_ALLOW_SCREENASVER,    /*Allow the machine to enter the screensaver*/
    PM_FORBID_POWEROFF,    /*Prevent the machine from shutting down*/
    PM_ALLOW_POWEROFF,    /*Allow the machine to shut down*/
}PM_REQ_T;

// POWER_TYPE Power supply type
typedef enum{
    POWER_ADAPTER = 1,    /*Adapter power supply*/
    POWER_USB,    /*USB peripheral power supply*/
    POWER_BATTERY,    /*On Battery*/
    POWER_WPC,    /*Unlimited base power supply*/
}POWER_TYPE;

typedef enum { 
    WAKEUP_SRC_NONE = 0, /* No sleep wake-up has been performed, no wake-up source */ 
    WAKEUP_SRC_AP,/* AP wake-up */ 
    WAKEUP_SRC_RTC,/* RTC wake-up */ 
    WAKEUP_SRC_KB,/* Press the button to wake up */ 
} WAKEUP_SOURCE; 

// Detect battery power
int OsCheckBattery(void);
// Detect power supply type
int OsCheckPowerSupply(void);
// Restart the machine
int OsReboot(void);
// Close the terminal
int OsPowerOff(void);
// Added interface
// Get the battery power
int OsGetBatCap(void);
// Get the battery voltage
int OsGetBatVol(void);
// Get the charging status
int OsGetBatChgState(void);

// Set first-level sleep time
int OsSysSleepTime(int Time);

int OsGetSysSleepTime(int *Time);

// Set the secondary sleep time
int OsSetSysDeepSleepTime(int Time);

int OsGetSysDeepSleepTime(int *Time);
// 1 Allowed to sleep 0 Not allowed to sleep
int OsSetSysSleepStatus(int status);

int OsGetSysSleepStatus(int *status);

// 0 Invalid 1Ap wakeup 2RTC wakeup 3 key wakeup
int OsWakeupSource(void);

/*
 * Custom database interface
 */
typedef void*		HANDLE; // Universal pointer definition
typedef int (*IsMatch)(const void *, int, const void *, int);

typedef enum DBINTERFACE_RET
{
    RET_INTERFACE_NOT_REC = -4,
	RET_INTERFACE_LENGTHERR = -3,
	RET_INTERFACE_PARAMERR = -2,
	RET_INTERFACE_FAILD    = -1,
	RET_INTERFACE_NORMAL   =  0,
}DB_INTERFACE_T;

// typedef PRecordCompareFunc IsMatch;
typedef struct _SearchingHelper
{
	void* 	pKeywords;
	int 	nLenOfKeyWords;
	int	nOffset;
	IsMatch fFilter;
}SearchingHelper_T;

typedef enum _tagSortType
{
	SORT_BY_NOTHNG = 0,	// NOT BE SORTABLE
	SORT_BY_USER_DEFINE,
}SortType;


/*
 * @author:
 * @Date:
 * @Record: create it;
 * 
 * @Function Name: DB_bInit
 * @Function Function: Database Initialization
 * @Input parameters:
 * szAppID Pass in the current APP ID
 * 
 * @Output Parameters: None
 * @Return value:
 * Success: 0
 * Failed: F
 * @Remark:
 * Before using the database interface, the database must be initialized first
 * @Call Example:
 */
int DB_bInit(char *szAppID);
/*
 * @author:
 * @Date:
 * @Record: create it;
 * 
 * @Function Name: DB_vUnInit
 * @Function Function: Database Logout
 * @Remark:
 * Called when exiting the application
 * @Call Example:
 */
void DB_vUnInit(void);
/*
 * @author:
 * @Date:
 * @Record: create it;
 * 
 * @Function Name: DB_bInitBySysService
 * @Function Function: Database Initialization
 * @Input parameters:
 * szDbName Pass in database name
 * szDbName Pass in database name
 * 
 * @Output Parameters: None
 * @Return value:
 * Success: 0
 * Failed: Others
 * @Remark:
 * Before using the database interface, the database must be initialized first
 * @Call Example:
 */
int DB_bInitBySysService(char* szDbPath,char *szDbName);

/*
 * @author:
 * @Date:
 * @Record: create it;
 * 
 * @Function Name: DB_hOpen
 * @Function function: Open the specified record table
 * @Input parameters:
 * szTableName table name
 * nMaxLenOfRec Maximum length of single record
 * @Output Parameters: None
 * @Return value:
 * Success: Return to the table handle;
 * Failed: NULL;
 * @Remark:
 * 
 * @Call Example:
 */

HANDLE DB_hOpen(const char* pszName, int nMaxLenOfRec);

/*
 * @author:
 * @Date:
 * @Record: create it;
 * 
 * @Function Name: DB_vClose
 * @Function function: Close the specified table
 * @Input parameters:
 * handle corresponding table handle
 * 
 * @Output Parameters: None
 * @Return value: None
 * 
 * @Remark:
 * 
 * @Call Example:
 */
void  DB_vClose(HANDLE hTable);

/*
 * @author:
 * @Date:
 * @Record: create it;
 * 
 * @Function Name: DB_bClear
 * @Function function: Clear table (no deletion table)
 * @Input parameters:
 * hTable corresponding table handle
 * @Output Parameters: None
 * @Return value:
 * Success: 0
 * Failed: Others
 * @Remark:
 * 
 * @Call Example:
 */
int DB_bClear(HANDLE hTable);

/*
 * @author:
 * @Date:
 * @Record: create it;
 * 
 * @Function Name: DB_bRemove
 * @Function function: Delete table
 * @Input parameters:
 * pszName table name
 * nAccess (reserved, not used) table access permissions (EM_ACCESS_PRIVATE – APP private, unaccessible by other applications, EM_ACCESS_SHARED — shared access, all other applications can access)
 * @Output Parameters: None
 * @Return value:
 * Success: 0
 * Failed: Others
 * @Remark:
 * 
 * @Call Example:
 */
int DB_bRemove(const char* pszName, int nAccess);

/*
 * @author:
 * @Date:
 * @Record: create it;
 * 
 * @Function Name: DB_nReadRec
 * @Function function: Read records through index
 * @Input parameters:
 * hTable table handle
 * nIndexOfRec Record index value (calculated from 0)
 * nLenOfBuffer length of buffer pointed to by pRecordBuf
 * @Output parameters:
 * pRecordBuf Returns the record content
 * @Return value:
 * Success: Returns the actual length value of the record
 * Failed: RET_INTERFACE_FAILD
 * @Remark:
 * 
 * @Call Example:
 */
int DB_nReadRec(HANDLE hTable, int nIndexOfRec, int nLenOfBuffer, char *pRecordBuf);

/*
 * @author:
 * @Date:
 * @Record: create it;
 * 
 * @Function Name: DB_nReadRecByKey
 * @Function function: Read records through keywords (fast speed is recommended)
 * @Input parameters:
 * hTable table handle
 * pKey query keywords must be unique
 * nLenOfBuffer length of buffer pointed to by pRecordBuf
 * @Output parameters:
 * pRecordBuf Returns the record content
 * @Return value:
 * Success: Returns the actual length value of the record
 * Failed: RET_INTERFACE_FAILD
 * @Remark:
 * 
 * @Call Example:
 */
int DB_nReadRecByKey(HANDLE hTable, char* pKey, int nLenOfBuffer, char *pRecordBuf);

/*
 * @author:
 * @Date:
 * @Record: create it;
 * 
 * @Function Name: DB_nAppendRecByKey
 * @Function function: Add records (recommended with index keywords)
 * @Input parameters:
 * hTable table handle
 * pKey query keywords must be unique
 * pRecordBuf Add record data pointer
 * nRecLen Add record length
 * @Output Parameters: None
 * 
 * @Return value:
 * Success: Returns the actual length value of the record
 * Failed: RET_INTERFACE_FAILD
 * @Call Example:
 */
int DB_nAppendRecByKey(HANDLE hTable, char* pKey, char* pRecordBuf, int nRecLen);

/*
 * @author:
 * @Date:
 * @Record: create it;
 * 
 * @Function Name: DB_nReplaceRec
 * @Function function: Update record
 * @Input parameters:
 * hTable table handle
 * nIndexOfRec table record index (calculated from 0)
 * pRecord data that needs to be updated
 * nRecordLen Record Length
 * @Output Parameters: None
 * 
 * @Return value:
 * Success: Returns the actual length value of the record
 * Failed: RET_INTERFACE_FAILD
 * @Remark:
 * 
 * @Call Example:
 */
int DB_nReplaceRec(HANDLE hTable, int nIndexOfRec, char* pRecord, int nRecordLen);


/*
 * @author:
 * @Date:
 * @Record: create it;
 * 
 * @Function Name: DB_nReplaceRecByKey
 * @Function function: Update record
 * @Input parameters:
 * hTable table handle
 * pKey query keywords must be unique
 * pRecord data that needs to be updated
 * nRecordLen Record Length
 * @Output Parameters: None
 * 
 * @Return value:
 * Success: Returns the actual length value of the record
 * Failed: RET_INTERFACE_FAILD
 * @Remark:
 * 
 * @Call Example:
 */
int DB_nReplaceRecByKey(HANDLE hTable, char* pKey,int nRecordLen,char* pRecord);

/*
 * @author:
 * @Date:
 * @Record: create it;
 * 
 * @Function Name: DB_bDeleteRec
 * @Function function: Delete record
 * @Input parameters:
 * hTable table handle
 * nIndex table record index (calculated from 0)
 * @Output Parameters: None
 * 
 * @Return value:
 * Success: 0
 * Failed: Others
 * @Remark:
 * 
 * @Call Example:
 */
int DB_bDeleteRec(HANDLE hTable, int nIndex);

/*
 * @author:
 * @Date: 06-03-2015
 * @Record: create it;
 * 
 * @Function Name: DB_bDeleteRecByKey
 * @Function function: Delete record
 * @Input parameters:
 * hTable table handle
 * pKey query keywords must be unique
 * @Output Parameters: None
 * 
 * @Return value:
 * Success: 0
 * Failed: Others
 * @Remark:
 * 
 * @Call Example:
 */
int DB_bDeleteRecByKey(HANDLE hTable, char* pKey);

/*
 * @author:
 * @Date:
 * @Record: create it;
 * 
 * @Function Name: DB_bIsExsit
 * @Function function: determines whether the table name exists
 * @Input parameters:
 * pszTableName table name
 * nAccess access mode (reserved)
 * 
 * @Output Parameters: None
 * 
 * @Return value:
 * Existence: 0 Existence
 * None: Others
 * @Remark:
 * 
 * @Call Example:
 */
int DB_bIsExsit(const char* pszTableName, int nAccess);

/*
 * @author:
 * @Date:
 * @Record: create it;
 * 
 * @Function Name: DB_nGetRecordSUM
 * @Function function: Get the number of valid records
 * @Input parameters:
 * hTable table handle
 * 
 * @Output Parameters: None
 * 
 * @Return value:
 * Success: Total records; (>= 0)
 * Failed: RET_INTERFACE_FAILD
 * @Remark:
 * 
 * @Call Example:
 */
int DB_nGetRecordSUM(HANDLE hTable);

/*
 * @author:
 * @Date:
 * @Record: create it;
 * 
 * @Function Name: DB_nGetLastError
 * @Function function: Get current error message
 * @Input parameters:
 * handle table handle
 * 
 * @Output Parameters: None
 * @Return value:
 * Error code
 * 
 * @Remark:
 * 
 * @Call Example:
 */
int DB_nGetLastError(HANDLE hTable);


/*
 * @author:
 * @Date:
 * @Record: create it;
 * 
 * @Function Name: DB_nGetFirstRecord
 * @Function function: Get the first record
 * @Input parameters:
 * hTable table handle
 * nIndexOfRec Index Pointer (not used)
 * nRecLen pRecordBuf points to buffer length
 * 
 * @Output parameter: pRecordBuf - The obtained data
 * 
 * @Return value:
 * Success: The actual length read
 * Failed: RET_INTERFACE_FAILD
 * @Remark:
 * 
 * @Call Example:
 */
int DB_nGetFirstRecord(HANDLE hTable, int *nIndexOfRec, void* pRecordBuf, int nRecLen);


/*
 * Camera
 */
int OsOpenCamera(int type);


void OsCloseCamera();

int OsReadScanResult(char* pContent,int inLen);

/*
 * Lvgl
 */
int OsLvglDispFlush(int wX, int wY, int wWidth, int wHeight, unsigned char* pucBuffer);

#ifdef __cplusplus
}
#endif



