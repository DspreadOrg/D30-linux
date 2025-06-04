#ifndef __DISP_H__
#define __DISP_H__

#include <linux/input.h>

#ifdef __cplusplus
extern "C"
{
#endif

#define BOOL	unsigned char
#define INT64   long
#define INT32	int
#define INT16 	short
#define INT8	char

#define UINT64	unsigned  long
#define UINT8	unsigned  char

#define FAILD   	(-1)
#define PARAM_ERROR		(-2)
#define UNKNOW_ERROR	(-255) // defined as


#define Bool		BOOL
#define Int64		INT64
#define Int32		INT32
#define	Int16		INT16
#define Int8		INT8

#define Uint64		UINT64
#define	Uint32		unsigned  int
#define Uint16		UINT16
#define Uint8		UINT8

#define True		TRUE
#define False		FALSE
#define Faild       FAILD

#define SHOW_STR_MAX_LINE 	6

#define DEFAULT_LEFT_MARGIN			10 //  Default number of left edge pixels left blank
#define DEFAULT_HEIGHT_LINE			26	// Default row height
#define DEFAULT_LINE_YPOINT(LINE)	(18 + DEFAULT_HEIGHT_LINE*(LINE)) //Default Y-axis coordinates for each row, calculation method, status bar 22, each character 32 high, with 1 vertical spacing,
#define DEFAULT_PORT_WIDTH			300  // Default display area width
#define DEFAULT_PORT_HEIGHT			(DEFAULT_HEIGHT_LINE)   // Default input box area height

typedef enum KB_KEYMAP_e
{
	EM_KEY_INVALID = -1,
	EM_KEY_1,
	EM_KEY_2,
	EM_KEY_3,
	EM_KEY_4,
	EM_KEY_5,
	EM_KEY_6,
	EM_KEY_7,
	EM_KEY_8,
	EM_KEY_9,
	EM_KEY_0,
	EM_KEY_F1,
	EM_KEY_F2,
	EM_KEY_F3,
	EM_KEY_F4,
	EM_KEY_CLEAR,
	EM_KEY_ENTER,
	EM_KEY_UP,
	EM_KEY_DOWN,
	EM_KEY_POWER,
	EM_KEY_CANCEL,
	EM_KEY_MAX
}KB_KEYMAP_T;

typedef struct
{
    unsigned char Second;
    unsigned char Minute;
    unsigned char Hour;
    unsigned char DayOfMonth;
    unsigned char Month;
    unsigned int Year;
    unsigned char CheckValue[4];
}SetLocalTimeType;

typedef enum DPORT_DTYPE_e
{
	EM_DTYPE_NORMAL,			//Positive display
	EM_DTYPE_REVERT,			//reverse display
	EM_DTYPE_WITH_UNDERLINE,	//Highlight with underline
	EM_DTYPE_WITH_TOPLINE,		//With a marked line, it is clearly visible
	EM_DTYPE_WITH_RECT			//add frame
}DPORT_DTYPE_T;

// Align Definition
typedef enum _ALIGN_DEF_T
{
	EM_ALIGN_LEFT 	=0,
	EM_ALIGN_RIGHT  =1,
	EM_ALIGN_CENTER = 2
}ALIGN_DEF_T;

typedef struct POINT_t
{
	Uint32 X;
	Uint32 Y;

}POINT_T;

typedef enum DPORT_STYLE_e
{
	DPORT_DEFAULT_STYLE 				= (1 << 0),	//	Default Style
	DPORT_DISABLE_NUMBERKEY 		= (1 << 1),	// Prohibit selection using numeric keys
	DPORT_AUTO_INDEX_SEPARATELY 	= (1 << 2),	// Automatically add index numbers and index each page independently
	DPORT_AUTO_INDEX_CONTINUITY	= (1 << 3),	// Automatically add index numbers, all items are indexed consecutively
	DPORT_ENABLE_BYPASS 				= (1 << 4),	// Enable BYPASS function
	DPORT_LIST_ALWAYS_RETURN_NUMBERKEY = (1 << 5),	//
	DPORT_ENABLE_LANGEN = (1 << 6),	// Display in English
	DPORT_PWD_INPUT = (1 << 7),	// Password frame
}DPORT_STYLE_T;


typedef enum DPORT_LINETYPE_e
{
	EM_LINETYPE_SOLID	 	= 0,	//full line
	EM_LINETYPE_DOTTED 	= 1,	//Dashed line
}DPORT_LINETYPE_T;

typedef enum DPORT_COLOR_e
{
	EM_COLOR_BLACK,
	EM_COLOR_WHITE,
	EM_COLOR_GREY,
	EM_COLOR_RED,
	EM_COLOR_MAX,
}DPORT_COLOR_T;

typedef enum DPORT_IME_TYPE
{
	DPORT_IME_PY = (1<<0),
	DPORT_IME_EN = (1<<1),
	DPORT_IME_NUM =(1<<2),
	DPORT_IME_SYMBOL = (1<<3)
}DPORT_IME_T;

// Prompt box interface type
typedef enum DPORT_TIPPAGE_KEY_TYPE_e
{
	EM_STYLE_OK = (1 <<0),	//confirm key
	EM_STYLE_CANCEL= (1 <<1)//cancel key
}DPORT_TIPPAGE_KEY_TYPE_T;

// /Return Value Definition
typedef enum DPORT_RET
{
	RET_DPORT_TIMEOUT = -3,
	RET_DRPOT_CANCEL =  -2,	
	RET_DPORT_FAILD  = -1,
	RET_DPORT_NORMAL =  0,
	RET_DPORT_OK     =  RET_DPORT_NORMAL,
	RET_DPORT_BYPASS = RET_DPORT_NORMAL,

}DPORT_RET_T;

typedef struct tagInputPinPara
{
	Int8 szPan[20];
	Int32 nPanLen;
	Int32 nKeyIndex;
	Int32 nPinMode;
	Int8  szHint[64];
	Int32  nMinPinLen;
	Int32  nMaxPinLen;
}InputPinPara;

typedef enum DPORT_FONT_SIZE
{
	DPORT_DEFAULT 	= 0,
	DPORT_FONT_32     	= 1,
	DPORT_FONT_24		= 2,
	DPORT_FONT_22		= 3,
	DPORT_FONT_16		= 4,
	DPORT_FONT_48     	= 5,
	DPORT_FONT_MAX
}DPORT_FONT_SIZE_T;


typedef enum DPORT_LANGEUGE
{
	DPORT_CHINESE	= 0,
	DPORT_ENGLISH  	= 1
}DPORT_LANGEUGE_T;
 

typedef enum DPORT_STYLE
{
	DPORT_LEFT	= 0,
	DPORT_RIGHT,
	DPORT_CENTER
}DPORT_TEXTSTYLE_T;

typedef struct tagListShowInfo{
	char pszText[32+1];
	Uint8 uFlag;
}ListShowInfo;

/*
 * @author:
 * @Date: 06-10-2022
 * @Record: create it;
 * 
 * @Function Name: Disp_nSetMsgBoxLanguage
 * @Function function: Set messageBox display language
 * 
 * @Input parameters: language setting language (DPORT_CHINESE or DPORT_ENGLISH)
 * 
 * @Output Parameters: None
 * 
 * @Return value: None
 * 
 * @Remark:
 * 
 * @Call Example:
 */
void Disp_nSetMsgBoxLanguage(Uint8 language);

/*
 * @author:
 * @Date: 10-10-2022
 * @Record: create it;
 * 
 * @Function Name: Disp_bInit
 * @Function Function: Disp module initialization
 * 
 * @Input parameters: None
 * 
 * @Output Parameters: None
 * 
 * @Return value: None
 * 
 * @Remark:
 * Must be called once during system initialization
 * @Call Example:
 */
void Disp_vInit(void);
// barState 0 does not display status bar 1 shows status bar
void Disp_vInitEx(int barState);

/*
 * @author:
 * @Date: 10-10-2022
 * @Record: create it;
 * 
 * @Function Name: Disp_vClearLine
 * @Function function: Clear the display content of the specified line
 * 
 * @Input parameter:nAtLine Specify the line number
 * 
 * @Output Parameters: None
 * 
 * @Return value: None
 * 
 * @Remark:
 * 
 * @Call Example:
 */
void Disp_vClearLine(Uint8 nAtLine);


/*
 * @author:
 * @Date: 10-10-2022
 * @Record: create it;
 * 
 * @Function Name: Disp_vShowStrAt
 * @Function function: Display the content of a string in the specified row and column
 * @Input parameters:
 * nAtLine Specifies the line number of the line
 * nAtColumn Specifies the column location (not implemented at present, all are filled in 0 by default)
 * The data displayed by fmt is input using variable parameters
 * 
 * @Output Parameters: None
 * 
 * @Return value:
 * none
 * @Remark:
 * Line number range: For 933, the large font subscript is: 0-5, a total of 6 lines, and the small font subscript is: 0-6, a total of 7 lines
 * @Call Example:
 * DPort_vShowStrAt(1, 0, "%s", "Show here");
 */
void Disp_vShowStrAt(Uint8 nAtLine, Uint8 nAtColumn, Int8* fmt, ...);

/*
 * @author:
 * @Date: 10-10-2022
 * @Record: create it;
 * 
 * @Function Name: Disp_vShowStrAtEx
 * @Function function: Display the content of a string in the specified row and column, and can automatically wrap the line
 * @Input parameters:
 * nAtLine Specifies the line number of the line
 * nPattern alignment See ALIGN_DEF_T definition
 * The data displayed by fmt is input using variable parameters
 * 
 * @Output Parameters: None
 * 
 * @Return value:
 * none
 * @Remark:
 * Line number range: For 933, the large font subscript is: 0-5, a total of 6 lines, and the small font subscript is: 0-6, a total of 7 lines
 * @Call Example:
 */
void Disp_vShowStrAtEx(Uint8 nAtLine, Uint8 nPattern, Int8* fmt, ...);

void Disp_vShowPersianStr(Uint8 nAtLine, Uint8 nPattern, Int8* fmt, ...);

/*
 * @author:
 * @Date: 10-10-2022
 * @Record: create it;
 * 
 * @Function Name: Disp_vShowStr
 * @Function function: Display string content in specified row and column
 * @Input parameters:
 * nAtLine Specifies the position of the row
 * nType display type See DPORT_DTYPE_T definition
 * nPattern alignment See ALIGN_DEF_T definition
 * The data displayed by fmt is input using variable parameters
 * 
 * @Output Parameters: None
 * 
 * @Return value: None
 * 
 * @Remark:
 * Line number range: For 933, the large font subscript is: 0-5, a total of 6 lines, and the small font subscript is: 0-6, a total of 7 lines
 * @Call Example:
 */
void Disp_vShowStr(Uint8 nAtLine, Uint8 nType, Uint8 nPattern, Int8* fmt, ...);

/*
 * @author:
 * @Date: 10-10-2022
 * @Record: create it;
 * 
 * @Function Name: Disp_vShowImage
 * @Function function: Display pictures at specified pixel locations
 * @Input parameters:
 * pcBmpData picture data buffering
 * LenofBufferSize pBmpData points to buffer length
 * tStartPoint specifies the starting X and Y points of the screen display. The starting position can be obtained using Disp_vGetUserDisplayRange
 * uBmpWidth picture width
 * uBmpHeight Picture High
 * 
 * @Output Parameters: None
 * 
 * @Return value:
 * none
 * 
 * @Remark:
 * 
 * @Call Example:
 */
void Disp_vShowImage(Uint8* pcBmpData, Uint32 LenofBufferSize, POINT_T tStartPoint, Uint32 uBmpWidth, Uint32 uBmpHeight);

/*
 * @author:
 * @Date: 10-10-2022
 * @Record: create it;
 * 
 * @Function Name: Disp_bDrawLineAt
 * @Function function: Draw a straight line at the specified point (nBeginX, nBeginY) to (nEndX, nEndY)
 * @Input parameters:
 * tBegin Specifies the starting point X and Y coordinates of the straight line
 * tEnd Specifies the end point X and Y coordinates of the line
 * nType Specifies the line drawing type, solid line and dotted line (only straight lines are supported)
 * nColor Specifies the color to be drawn
 * 
 * @Output Parameters: None
 * 
 * @Return value:
 * none
 * 
 * @Remark:
 * Reference POINT_RANGE_T to define the scope
 * 933 resolution is 320X240
 * The start and end positions can be obtained through Disp_vGetUserDisplayRange
 * @Call Example:
 */
void Disp_vDrawLineAt(POINT_T tBegin, POINT_T tEnd, DPORT_LINETYPE_T nType, DPORT_COLOR_T nColor);

/*
 * @author:
 * @Date: 10-10-2022
 * @Record: create it;
 * 
 * @Function Name: Disp_vClearPort
 * @Function function: Clear all content displayed by the application
 * @Input parameters: None
 * 
 * @Output Parameters: None
 * 
 * @Return value:
 * none
 * 
 * @Remark:
 * 
 * @Call Example:
 */
void Disp_vClearPort(void);

/*
 * @author:
 * @Date: 10-25-2022
 * @Record: create it;
 * 
 * @Function Name: Disp_nUserInputView
 * @Function Function: User input box control
 * @Input parameters:
 * X Y --The starting position of the control display
 * nWidth nHeight--Control displays width and height
 * nLimtMin -----Input minimum length limit
 * nLimtMax -----Input maximum length limit
 * nInputMethod --- Input method type
 * nOutBufferLen--- Output cache length
 * nTimeOut----Timeout time (unit seconds)
 * nStyle -----Number key response and automatic indexing function (reserve, fill in 0)
 * 
 * @Output parameters: pOutBuffer --- Output cache
 * 
 * @Return value:
 * Success --Control input length
 * Failed -- RET_DPORT_FAILD
 * Timeout --RET_DPORT_TIMEOUT
 * User Cancel-RET_DRPOT_CANCEL
 * 
 * @Remark:
 * The maximum characters displayed in a single line vary according to the font size
 * @Call Example:
 */
Int32 Disp_nUserInputView(int X, int Y, int nWidth, int nHeight, 
							Uint32 nLimtMin,
							Uint32 nLimtMax,
							Uint32 nInputMethod,
							Uint32 nOutBufferLen,
							Uint8 *pOutBuffer,
							Int32 nTimeOut,
							Uint32 nStyle);

/*
 * @author:
 * @Date: 10-25-2022
 * @Record: create it;
 * 
 * @Function Name: Disp_nUserInputViewEx
 * @Function function: User input box control (can set the order of input method type settings)
 * @Input parameters:
 * X Y --The starting position of the control display
 * nWidth nHeight--Control displays width and height
 * nLimtMin -----Input minimum length limit
 * nLimtMax -----Input maximum length limit
 * pInputMethod --- Input method type
 * nLenOfInputMethod --number of input methods
 * nOutBufferLen--- Output cache length
 * nTimeOut----Timeout time (unit seconds)
 * nStyle -----Number key response and automatic indexing function (reserve, fill in 0)
 * 
 * @Output parameters: pOutBuffer --- Output cache
 * 
 * @Return value:
 * Success --Control input length
 * Failed -- RET_DPORT_FAILD
 * Timeout --RET_DPORT_TIMEOUT
 * User Cancel-RET_DRPOT_CANCEL
 * 
 * @Remark:
 * A single line displays up to 15 Chinese characters and 30 characters;
 * You can use nShowLineHeight to define the number of lines occupied by the control and expand the number of characters entered by the control.
 * For example: the maximum support for displaying 45 Chinese characters input can be adjusted to
 * Disp_nUserInputView(1,3,1,45,DPORT_IME_PY,sizeof(auOutBuffer),auOutBuffer);
 * @Call Example:
 * PR_INT8 auOutBuffer[10+1]={0};
 * Uint32 InputMethod[] = {DPORT_IME_PY, DPORT_IME_EN};
 * 
 * Disp_nUserInputView(1,2,1,10,InputMethod,sizeof(aucOutBuffer),aucOutBuffer);
 */
Int32 Disp_nUserInputViewEx(int X, int Y, int nWidth, int nHeight, 
							Uint32 nLimtMin,
							Uint32 nLimtMax,
							Uint32* pInputMethod,
							Uint32 nLenOfInputMethod,
							Uint32 nOutBufferLen,
							Uint8 *pOutBuffer,
							Int32 nTimeOut,
							Uint32 nStyle);

/*
 * @author:
 * @Date: 10-25-2022
 * @Record: create it;
 * 
 * @Function Name: Disp_nUserInputViewEx
 * @Function function: User input box control (can set the order of input method type settings)
 * @Input parameters:
 * X Y --The starting position of the control display
 * nWidth nHeight--Control displays width and height
 * nLimtMin -----Input minimum length limit
 * nLimtMax -----Input maximum length limit
 * pInputMethod --- Input method type
 * nLenOfInputMethod --number of input methods
 * nOutBufferLen--- Output cache length
 * nTimeOut----Timeout time (unit seconds)
 * nStyle -----Number key response and automatic indexing function (reserve, fill in 0)
 * 
 * @Output parameters: pOutBuffer --- Output cache
 * 
 * @Return value:
 * Success --Control input length
 * Failed -- RET_DPORT_FAILD
 * Timeout --RET_DPORT_TIMEOUT
 * User Cancel-RET_DRPOT_CANCEL
 * 
 * @Remark:
 * A single line displays up to 15 Chinese characters and 30 characters;
 * You can use nShowLineHeight to define the number of lines occupied by the control and expand the number of characters entered by the control.
 * For example: the maximum support for displaying 45 Chinese characters input can be adjusted to
 * Disp_nUserInputView(1,3,1,45,DPORT_IME_PY,sizeof(auOutBuffer),auOutBuffer);
 * @Call Example:
 * PR_INT8 auOutBuffer[10+1]={0};
 * Uint32 InputMethod[] = {DPORT_IME_PY, DPORT_IME_EN};
 * 
 * Disp_nUserInputView(1,2,1,10,InputMethod,sizeof(aucOutBuffer),aucOutBuffer);
 */
Int32 Disp_nUserInputViewExEx(int X, int Y, int nWidth, int nHeight, 
							Uint32 nLimtMin,
							Uint32 nLimtMax,
							Uint32* pInputMethod,
							Uint32 nLenOfInputMethod,
							Uint32 nOutBufferLen,
							Uint8 *pOutBuffer,
							Int32 nTimeOut,
							Uint32 nStyle, 
							Uint8 *pszInfo);

/*
 * @author:
 * @Date: 10-27-2022
 * @Record: create it;
 * 
 * @Function Name: Disp_nMessageBox
 * @Function Function: Prompt Box Control
 * @Input parameters:
 * szTitle --Title information
 * theStype --Interface style reference DPORT_TIPPAGE_KEY_TYPE_T
 * nTimeoutSec --Timeout time (unit seconds)
 * nStyle -----Number key response and automatic indexing function (reserve, fill in 0)
 * args --format string
 * 
 * @Output Parameters: None
 * 
 * @Return value:
 * RET_DPORT_OK --- Confirm key returns value
 * RET_DRPOT_CANCEL --- Cancel key returns value
 * RET_DPORT_TIMEOUT --Timeout
 * @Remark:
 * Maximum support: 100 Chinese characters, 200 characters display
 * @Call Example:
 * Interface type: Currently there is only single confirmation key and confirmation cancel key
 * Disp_nMessageBox("Title Information",EM_STYLE_OK, 5,"%s","Transaction successful");
 * Disp_nMessageBox("Title Information",EM_STYLE_OK|EM_STYLE_CANCEL, 5, "%s","Transaction successful");
 */
Int32 Disp_nMessageBox(Int8 *szTitle, Int32 theStype, Int32 nTimeoutSec, Uint32 nStyle, Int8 *args,...);

/*
 * @author:
 * @Date: 10-27-2022
 * @Record: create it;
 * 
 * @Function Name: Disp_nMessageBoxEx
 * @Function function: Prompt box control (content can be turned on pages)
 * @Input parameters:
 * X Y -- Prompt box coordinates
 * nWidth nHeight -- Prompt box size
 * szTitle --Title information
 * theStype --Interface style reference DPORT_TIPPAGE_KEY_TYPE_T
 * nTimeoutSec --Timeout time (unit seconds)
 * nStyle -----Number key response and automatic indexing function (reserve, fill in 0)
 * args --format string
 * 
 * @Output Parameters: None
 * 
 * @Return value: key value
 * RET_DPORT_OK --Confirm key returns value
 * RET_DRPOT_CANCEL --Cancel key returns value
 * RET_DPORT_TIMEOUT --Timeout
 * 
 * @Remark:
 * Maximum support: 100 Chinese characters, 200 characters display
 * @Call Example:
 * Interface type: Currently there is only single confirmation key and confirmation cancel key
 * Disp_nMessageBoxEx(5, 27, 310, 200, NULL,MB_OK | MB_CANCEL, 0, 0, "%s", "Content Test");
 * Disp_nMessageBoxEx(5, 27, 310, 200, "Title Test", MB_OK, 0, 0,"%s", "Content Test");
 */
int Disp_nMessageBoxEx(int X, int Y, int nWidth, int nHeight, char* szTitle, int theStype, int nTimeoutSec, Uint32 nStyle, char *args,...);

/*
 * @author:
 * @Date: 10-27-2022
 * @Record: create it;
 * 
 * @Function Name: Disp_nShowEditBox
 * @Function function: Display string control (content can be turned on pages)
 * @Input parameters:
 * X Y -- Prompt box coordinates
 * nWidth nHeight -- Prompt box size
 * nTimeoutSec --Timeout time (unit seconds)
 * nStyle -----Number key response and automatic indexing function (reserve, fill in 0)
 * fmt --format string
 * 
 * @Output Parameters: None
 * 
 * @Return value: key value
 * RET_DPORT_OK --- Confirm key returns value
 * RET_DPORT_TIMEOUT --- Timeout
 * RET_DRPOT_CANCEL --User Cancel
 * RET_DPORT_FAILD --parameter error
 * @Remark:
 * Maximum support: 100 Chinese characters, 200 characters display
 * @Call Example:
 * 
 * Disp_nShowEditBox(10, 30, 300, 60, 10, 0,"%s", "Transaction successful");
 */
int Disp_nShowEditBox(int X, int Y, int nWidth, int nHeight, int nTimeoutSec, unsigned int nStyle, char* fmt, ...);

/*
 * @author:
 * @Date: 10-27-2022
 * @Record: create it;
 * 
 * @Function Name: Disp_nShowEditBoxEx
 * @Function function: Display string control (content can be turned on pages)
 * @Input parameters:
 * X Y -- Prompt box coordinates
 * nWidth nHeight -- Prompt box size
 * nTimeoutSec --Timeout time (unit seconds)
 * nStyle --numeric key response and automatic indexing function (reserve, fill in 0)
 * string --Show string
 * 
 * @Output Parameters: None
 * 
 * @Return value: key value
 * RET_DPORT_OK --- Confirm key returns value
 * RET_DPORT_TIMEOUT --- Timeout
 * RET_DRPOT_CANCEL --User Cancel
 * 
 * @Remark:
 * 
 * @Call Example:
 * 
 * Disp_nShowEditBox(10, 30, 300, 60, 10, 0, "Transaction successful");
 */
int Disp_nShowEditBoxEx(int X, int Y, int nWidth, int nHeight, int nTimeoutSec, unsigned int nStyle, char* string);

/*
 * @author:
 * @Date: 10-27-2022
 * @Record: create it;
 * 
 * @Function Name: Disp_nShowLogoView
 * @Function function: Display standby screen
 * @Input parameters:
 * szLogoPath -- The image path of the standby screen, the format needs to be 565 format
 * szScreenTip -- Standby screen prompt information content
 * 
 * @Output Parameters: None
 * 
 * @Return value: None
 * 
 * @Remark:
 * 
 * @Call Example:
 * 
 * Disp_vShowLogoView("UnionPay.bmp", "China UnionPay");
 */
void Disp_vShowLogoView(Int8 *szLogoPath, Int8* pszScreenTip);

/*
 * @author:
 * @Date: 10-27-2022
 * @Record: create it;
 * 
 * @Function Name: Disp_nShowAmountInputView
 * @Function function: Display amount input interface
 * @Input parameters:
 * nX nY --Amount box coordinates
 * nWidth nHeight --Amount box size
 * pszCurrencyFlag -- Currency symbol string
 * nMinLen --Minimum length limit
 * nMaxLen --Maximum length limit
 * nLenOfBuffer --buffer length pointed to by pszAmount
 * nTimeoutSec --Timeout time (unit seconds)
 * nStyle -----Number key response and automatic indexing function (reserve, fill in 0)
 * @Output parameters:
 * pszAmount --Enter the amount buffer
 * @Return value:
 * Success -- Amount length
 * Failed -- RET_DPORT_FAILD
 * User Cancel -- RET_DRPOT_CANCEL
 * @Remark:
 * 
 * @Call Example:
 */
int 	Disp_nShowAmountInputView(int nX, int nY, int nWidth, int nHeight,
								  char* pszCurrencyFlag, int nMinLen, int nMaxLen, 
								  int nLenOfBuffer,char* pszAmount, int nTimeoutSec,
								  unsigned int nStyle);

/*
 * @author:
 * @Date: 10-27-2022
 * @Record: create it;
 * 
 * @Function Name: Disp_nShowAmountInputViewEx
 * @Function function: Display amount input interface
 * @Input parameters:
 * nX nY --Amount box coordinates
 * nWidth nHeight --Amount box size
 * pszCurrencyFlag -- Currency symbol string
 * nMinLen --Minimum length limit
 * nMaxLen --Maximum length limit
 * nLenOfBuffer --buffer length pointed to by pszAmount
 * nTimeoutSec --Timeout time (unit seconds)
 * pszInfo --A prompt when input is less than the length limit
 * nStyle -----Number key response and automatic indexing function (reserve, fill in 0)
 * @Output parameters:
 * pszAmount --Enter the amount buffer
 * @Return value:
 * Success -- Amount length
 * Failed -- RET_DPORT_FAILD
 * User Cancel -- RET_DRPOT_CANCEL
 * @Remark:
 * 
 * @Call Example:
 */
int Disp_nShowAmountInputViewEx(int nX, int nY, int nWidth, int nHeight,
								  char* pszCurrencyFlag, int nMinLen, int nMaxLen, 
								  int nLenOfBuffer,char* pszAmount, int nTimeoutSec,
								  unsigned int nStyle, char *pszInfo);


/*
 * @author:
 * @Date: 10-27-2022
 * @Record: create it;
 * 
 * @Function Name: Disp_nShowPasswdInputView
 * @Function function: Display user login password input box
 * @Input parameters:
 * nX nY --Password box coordinates
 * nWidth nHeight --password box size
 * nLenOfBuffer --buffer length pointed to by pszPasswd
 * nTimeoutSec --Timeout time
 * nStyle -----Number key response and automatic indexing function (reserve, fill in 0)
 * @Output parameters:
 * pszPasswd -- Enter password buffer
 * 
 * @Return value:
 * Success -- Returns the entered password length
 * Failed -- RET_DPORT_FAILD (including timeout)
 * User Cancel -- RET_DRPOT_CANCEL
 * @Remark:
 * The maximum length of the string entered by the user is determined based on the width of the display box and the width of each character.
 * @Call Example:
 */
int 	Disp_nShowPasswdInputView(int nX, int nY, int nWidth, int nHeight, int nMinLen, 
							int nMaxLen,int nLenOfBuffer,char* pszPasswd, 
							int nTimeoutSec, unsigned int nStyle);


/*
 * @author:
 * @Date: 10-27-2022
 * @Record: create it;
 * 
 * @Function Name: Disp_nShowPINPADInputView
 * @Function function: Built-in password keyboard input box
 * @Input parameters:
 * X --Input box start coordinates
 * Y --Input box start coordinates
 * nWidth --Input box width
 * nHeight --Input box height
 * pszInputPin --Input PIN attribute
 * nLenOfPinBuffer --buffer length pointed to by pszPin
 * ntimeoutsec: Timeout time, seconds in units, maximum of 60 seconds
 * 
 * @Output parameters: pszPin The PIN code returned
 * 
 * @Return value:
 * Success - Fixed return the character length entered by the user
 * Failed - RET_DPORT_FAILD
 * User Cancel -RET_DRPOT_CANCEL
 * Timeout -RET_DPORT_TIMEOUT
 * @Remark:
 * The maximum length of the string entered by the user is determined based on the width of the display box and the width of each character.
 * @Call Example:
 */
int 	Disp_nShowPINPADInputView(int nX,int nY,int nWidth,int nHeight,
		  	  	  	  	  	  	  InputPinPara *pszInputPin,
		  	  	  	  	  	  	  int nLenOfPinBuffer,int ntimeoutsec,char* pszPin);

/*
 * @author:
 * @Date: 10-27-2022
 * @Record: create it;
 * 
 * @Function Name: Disp_nShowIPInputLine
 * @Function Function: Display IP input box
 * @Input parameters:
 * X --Input box start coordinates
 * Y --Input box start coordinates
 * nWidth --Input box width
 * nHeight --Input box height
 * nMinLen --Input minimum length limit
 * nMaxLen --Input maximum length limit
 * nLenOfBuffer --The size of memory pointed to by pszIP
 * nTimeoutSec --Timeout time
 * nStyle -----Number key response and automatic indexing function (reserve, fill in 0)
 * 
 * @Output parameters: pszIP --Return IP string
 * 
 * @Return value:
 * Success - Fixed return the character length entered by the user
 * Failed - RET_DPORT_FAILD
 * User Cancel - RET_DRPOT_CANCEL
 * Timeout -RET_DPORT_TIMEOUT
 * @Remark:
 * 
 * @Call Example:
 */

int 	Disp_nShowIPInputLine(int X, int Y, int nWidth, int nHeight, int nMinLen, int nMaxLen, 
								int nLenOfBuffer,char *pszIP, int nTimeoutSec, unsigned int nStyle);

/*
 * @author:
 * @Date: 10-27-2022
 * @Record: create it;
 * 
 * @Function Name: Disp_nGetUserDisplayRange
 * @Function function: Return to the screen to display the area range (from X, Y axis to parameters)
 * @Input parameters:
 * 
 * @Output parameters:
 * pnStartX Start X coordinate
 * pnEndX Start Y coordinate
 * pnStartY Terminate X coordinate
 * pnEndY terminates Y coordinate
 * 
 * @Return value:
 * none
 * @Remark:
 * 
 * @Call Example:
 * 
 * int nStartX, nEndX, nStartY, nEndY;
 * Disp_vGetUserDisplayRange(&nStartX, nEndX, &nStartY, &nEndY);
 */
void Disp_vGetUserDisplayRange(int *pnStartX, int *pnEndX, int *pnStartY, int *pnEndY);

/*
 * @author:
 * @Date: 10-25-2022
 * @Record: create it;
 * 
 * @Function Name: Disp_nShowListView
 * @Function Function: List display window, used to display single list window
 * 
 * @Input parameters:
 * iX iY --List box coordinates
 * uWidth uHeight --list size
 * pszTitle --list title
 * pszListItem -- Show list
 * nItemCount -- Show the number of items
 * nItemsPerPage --The number of items displayed per page
 * nSelectItem --Initially selected item
 * nTimeoutSec --Timeout time: unit seconds
 * nStyle -----Number key response and automatic indexing function
 * @Output parameters:
 * pnIndexOfSelect --The selected item index (counted from 0)
 * 
 * @Return value:
 * Success:RET_DPORT_NORMAL
 * Timeout:RET_DPORT_TIMEOUT
 * Cancel:RET_DRPOT_CANCEL
 * 
 * @Note: Only supported in English and digital inputs
 * 
 * @Call Example:
 */
int Disp_nShowListView(int iX, int iY, unsigned int uWidth, unsigned int uHeight, char* pszTitle, char** pszListItem, int nItemCount, int nItemsPerPage,
									 int nSelectItem, int nTimeoutSec, int* pnIndexOfSelect, unsigned int nStyle);
/*
 * @author:
 * @Date: 10-25-2022
 * @Record: create it;
 * 
 * @Function Name: Disp_nShowListViewEx
 * @Function Function: List display window, used to display single list window
 * 
 * @Input parameters:
 * pszTitle --list title
 * pszListItem -- Show list
 * nItemCount -- Show the number of items
 * nItemsPerPage --The number of items displayed per page
 * nSelectItem --Initially selected item
 * nTimeoutSec --Timeout time: unit seconds
 * nStyle -----Number key response and automatic indexing function
 * @Output parameters:
 * pnIndexOfSelect --The selected item index (counted from 0)
 * 
 * @Return value:
 * Success:RET_DPORT_NORMAL
 * Timeout:RET_DPORT_TIMEOUT
 * Cancel:RET_DRPOT_CANCEL
 * 
 * @Note: Only supported in English and digital inputs
 * 
 * @Call Example:
 */
int Disp_nShowListViewEx(char* pszTitle, char** pszListItem, int nItemCount, int nItemsPerPage,
									 int nSelectItem, int nTimeoutSec, int* pnIndexOfSelect, 
									 unsigned int nStyle);

/*
 * @author:
 * @Date: 10-25-2022
 * @Record: create it;
 * 
 * @Function Name: Disp_nShowTableView
 * @Function Function: Double-list display UI. Used for double-column display
 * 
 * @Input parameters:
 * iX iY --List box coordinates
 * uWidth uHeight --list size
 * pszTitle --list title
 * pszListItem -- Show list
 * nItemCount -- Show the number of items
 * nItemsPerPage --The number of items displayed per page
 * nSelectItem --Initially selected item
 * nTimeoutSec --Timeout time: unit seconds
 * nStyle -----Number key response and automatic indexing function
 * @Output parameters:
 * pnIndexOfSelect --The selected item index (counted from 0)
 * 
 * @Return value:
 * Success:RET_DPORT_NORMAL
 * Timeout:RET_DPORT_TIMEOUT
 * Cancel:RET_DRPOT_CANCEL
 * Failed:RET_DPORT_FAILD
 * 
 * @Note: Only supported in English and digital inputs
 * 
 * @Call Example:
 */
int Disp_nShowTableView(int iX, int iY, unsigned int uWidth, unsigned int uHeight,
							char* pszTitle, char** pszListItem, int nItemCount, int nItemsPerPage,
							int nSelectItem,int nTimeoutSec, int* pnIndexOfSelect, unsigned int nStyle);

/*
 * @author:
 * @Date: 10-25-2022
 * @Record: create it;
 * 
 * @Function Name: Disp_nShowTableViewEx
 * @Function Function: Double-list display UI. Used for double-column display
 * 
 * @Input parameters:
 * pszTitle --list title
 * pszListItem -- Show list
 * nItemCount -- Show the number of items
 * nItemsPerPage --The number of items displayed per page
 * nSelectItem --Initially selected item
 * nTimeoutSec --Timeout time: unit seconds
 * nStyle -----Number key response and automatic indexing function
 * @Output parameters:
 * pnIndexOfSelect --The selected item index (counted from 0)
 * 
 * @Return value:
 * 
 * @Note: Only supported in English and digital inputs
 * 
 * @Call Example:
 */

// List control
int Disp_nShowTableViewEx(char* pszTitle, char** pszListItem, int nItemCount,
					int nItemsPerPage, int nSelectItem,int nTimeoutSec, 
					int* pnIndexOfSelect, unsigned int nStyle);

/*
 * @author:
 * @Date: 10-25-2022
 * @Record: create it;
 * 
 * @Function Name: Disp_vCleanArea
 * @Function function: Clear the specified area
 * 
 * @Input parameters:
 * X Y ---Clear the coordinates of the area
 * nWidth nHeight --- Clear area size
 * @Output parameters:
 * none
 * 
 * @Return value:
 * 
 * @Remark:
 * 
 * @Call Example:
 */
void Disp_vCleanArea(int X, int Y, int nWidth, int nHeight);

/*
 * @author:
 * @Date: 10-25-2022
 * @Record: create it;
 * 
 * @Function Name: Disp_nShowDateInputView
 * @Function function: Display date input box
 * 
 * @Input parameters:
 * nTimeOutSec ---Timeout time
 * @Output parameters:
 * pDate ---Returned time value
 * 
 * @Return value:
 * Success:RET_DPORT_NORMAL
 * Failed:RET_DPORT_FAILD
 * Cancel:RET_DRPOT_CANCEL
 * Timeout:RET_DPORT_TIMEOUT
 * 
 * @Remark:
 * 
 * @Call Example:
 */
int Disp_nShowDateInputView(SetLocalTimeType* pDate,  int nTimeOutSec);

/*
 * @author:
 * @Date: 10-25-2022
 * @Record: create it;
 * 
 * @Function Name: Disp_vSetSysFont
 * @Function function: Set system font
 * 
 * @Input parameters:
 * FontType --- Font Type
 * @Output Parameters: None
 * 
 * @Return value: None
 * 
 * @Remark:
 * 
 * @Call Example:
 */
void Disp_vSetSysFont(DPORT_FONT_SIZE_T FontType);

/*
 * @author:
 * @Date: 2022-08-14
 * @Record: create it;
 * 
 * @Function Name: Disp_vDrawTextA
 * @Function function: Display string when setting coordinates
 * 
 * @Input parameters:
 * x x coordinate
 * y y coordinates
 * w display width
 * h Display length
 * pstar display string
 * len string length
 * nStyle font display method
 * @Output Parameters: None
 * 
 * @Return value: None
 * 
 * @Remark:
 * 
 * @Call Example:
 */
void Disp_vDrawTextA(int x, int y, int w, int h, const char * pStr, int len, DPORT_TEXTSTYLE_T nStyle);

int Disp_ReleasKey();

int Disp_GetKey(int timeoutMs);

int Disp_GetTouchPadAxis(int *piX, int *piY, int timeoutMs);
/*
 * struct input_event {
 * struct timeval time;
 * __u16 type;
 * __u16 code;
 * __s32 value;
 * };
 */
/*
 * @brief Get touch screen events and return to the standard linux input structure
 * 
 * @param event input_event structure,
 * @param iTimeOut timeout event
 * @return INT Return result
 */
int Disp_GetTouchPadLinux(struct input_event *event, int timeoutMs);
/*
 * @brief Get key event and return the standard linux input structure
 * 
 * @param event input_event structure,
 * @param iTimeOut timeout event
 * @return INT Return result
 */
int Disp_GetKeyLinux(struct input_event *event, int timeoutMs);

int Disp_nShowListWithSlideButtonEx(int iX, int iY, unsigned int uWidth, unsigned int uHeight, char* pszTitle, ListShowInfo *pListShowInfo,
 										int nItemCount, int nItemsPerPage,int nTimeoutSec);
int Disp_nShowProgressBar(int iX, int iY, unsigned int uWidth, unsigned int uHeight, int *pLevl,int nTimeoutSec);

void Disp_vSetStatusBarStatus(int nState);

void Disp_vDrawRectWithColor(int iX,int iY,unsigned int uWidth,unsigned int uHeight,unsigned int wColor);

typedef  void (*pUICallback)(int runstate, void *par);

void Disp_vRegisterPauseCallBack(pUICallback callback);

void Disp_vRegisterResumeCallBack(pUICallback callback);
// pBuf Monochrome bmp cache 10K
int Disp_nElecSign(unsigned char* pBuf,int* pOutLen,char*pCode,int timeout,int x,int y,int width,int height,int isShowBtn);

int Disp_nElecSignEx(unsigned char* pBuf,int* pOutLen,char*pCode,float compress,int timeout,int x,int y,int width,int height,int isShowBtn);
#ifdef __cplusplus
}
#endif
#endif


