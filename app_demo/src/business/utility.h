/*
 * Utility.h
 * 
 * Created on: Sep 11, 2014
 * Author: yanbin
 * Note:
 */

#ifndef UTILITY_H_
#define UTILITY_H_

#include "typedefine.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef enum _PADDING_STYE
{
	EM_PADDING_LEFT,
	EM_PADDING_RIGHT,
}PR_PADDING_STYLE;

/*
 * @Record: create it;
 * 
 * @Function Name: PR_nUtilBcd2Asc
 * @Function function: Convert BCD byte stream to ASC byte stream and return the converted length
 * @Input parameters:
 * nLenOfBcd BCD byte stream length
 * pcBCDStreamIn BCD Byte Stream Data Pointer
 * nAlign converted ASC code alignment, PR_ALIGN_RIGHT - right alignment, PR_ALIGN_LEFT - left alignment
 * nLenOfBuffer pcAscStreamOut The length of the BUFFER pointed to by the pointer
 * @Output parameters: pcAscStreamOut Output ASC data byte stream
 * @Return value:
 * Success: Returns the converted length (>0)
 * Failed: PR_FAILD
 * 
 * @Remark:
 * @Call Example:
 */
 PR_INT32 PR_nUtilBcd2Asc(PR_UINT32 nLenOfBcd,
						 PR_INT8*  pcBCDStreamIn,
						 PR_INT32  nAlign,
						 PR_UINT32 nLenOfBuffer,
						 PR_INT8*  pcAscStreamOut);

/*
 * @Record: create it;
 * 
 * @Function Name: PR_nUtilAsc2Bcd
 * @Function function: Convert ASC byte stream to BCD byte stream and return the converted length
 * @Input parameters: nLenOfAsc【Input】 ASC byte stream length
 * pcAscStreamIn【Input】 ASC Byte Stream Data Pointer
 * nAlign【Input】Converted ASC code alignment, EM_ALIGN_RIGHT - Right aligned, EM_ALIGN_LEFT - Left aligned
 * nLenOfBuffer【Input】 pcBcdStreamOut The length of the pointer pointing to BUFFER
 * 
 * 
 * 
 * @Output parameters: pcBcdStreamOut【Output】 Output BCD data byte stream
 * @Return value: Success: Returns the converted length (>0)
 * Failed: PR_FAILD
 * 
 * @Remark:
 * @Call Example:
 */

PR_INT32 PR_nUtilAsc2Bcd(PR_UINT32  nLenOfAsc,
						  PR_INT8*  pcAscStreamIn,
						  PR_INT32  nAlign,
						  PR_UINT32 nLenOfBuffer,
						  PR_INT8*  pcBcdStreamOut);

/*
 * @Record: create it;
 * 
 * @Function Name: PR_vUtilBinaryXOR
 * @Function function: Exclusively OR input Data1 and Data2, return the result to result
 * @Input parameters: nDataLength【Input】 The length of XOR data is required
 * pcData1【Input】Byte stream pointer that requires data 1 of XOR
 * pcData2【Input】Byte stream pointer for data 2 that requires XOR
 * 
 * 
 * 
 * @Output parameters: pcResult【Output】 The byte stream after outputting XOR
 * @Return value: None
 * 
 * @Remark:
 * @Call Example:
 */

 void PR_vUtilBinaryXOR(PR_UINT32 nDataLength,
					   PR_UINT8* pcData1,
					   PR_UINT8* pcData2,
					   PR_UINT8* pcResult);

/*
 * @Record: create it;
 * 
 * @Function Name: PR_nUtilHex2Asc
 * @Function function: Convert the HEX byte stream to the corresponding ASC byte stream and return the converted length
 * @Input parameters: nHexDataLength【Input】 The length of the input HEX byte stream
 * pcHexDataIn [Input] The input HEX byte stream data pointer
 * nLenOfBuffer【Input】The length of buffer pointed to by pcAscDataOut
 * 
 * @Output parameters: pcAscDataOut【Out】Output ASC byte stream pointer
 * @Return value:
 * Success: converted length
 * Failed: PR_FAILD
 * 
 * @Remark:
 * @Call Example:
 */

PR_INT32 PR_nUtilHex2Asc(PR_UINT32 nHexDataLength,
						 PR_UINT8* pcHexDataIn,
						 PR_UINT32 nLenOfBuffer,
						 PR_INT8*  pcAscDataOut);

/*
 * @Record: create it;
 * 
 * @Function Name: PR_pcFindFirstNonSpecifiedChar
 * @Function function: Returns the character position in the input character that is different from the character specified in the parameter
 * @Input parameters: nLenOfStr【Input】 The length of the input character
 * pcString【Input】Input string
 * cChr【Input】Specified Character
 * 
 * @Output parameters: pnRemainDataLength【Output】Reserved character length
 * @Return value:
 * Success: Returns the character position of the specified character in the parameter
 * Failed: PR_NULL
 * @Remark:
 * @Call Example:
 */

PR_INT8* PR_pcFindFirstNonSpecifiedChar(PR_UINT32 nLenOfStr,
										PR_INT8*  pcString,
										PR_INT8   cChr,
										PR_UINT32* pnRemainDataLength);

/*
 * @Function Name: PR_nUtilBuildeString
 * @function function: copy the character stream data into a string (character data stream with '\0' as the ending character)
 * @Input parameters: nLenOfStringBuffer【Input】 Character byte stream data length
 * pcString【Input】 Data pointer for character byte stream
 * nLenOfAscStream【Input】
 * The buffer length pointed to by pcAscStream
 * 
 * @Output parameters: pcAscStream【Output】 Converted string data
 * @Return value: Success - Returns the length of the string (>0), with the end character in the length
 * Failed - Return PR_FAILD
 * 
 * @Remark:
 * @Call Example:
 */
PR_INT32 PR_nUtilBuildeString(PR_UINT32 nLenOfStringBuffer,
							  PR_INT8*  pcString,
							  PR_UINT32 nLenOfAscStream,
							  PR_UINT8* pcAscStream);
/*
 * @Function Name: PR_nUtilMemcpyEx
 * @Function function: Memory copy operation. If the length of the target buffer needs to be copied exceeds the length of the source data, the remaining space will be automatically filled with the characters specified by PaddingChar.
 * @Input parameters: pcData【Input】 Source data
 * nDataLength【Input】 Data length of source data
 * cPaddingChar【Input】Fill characters
 * nOutLength【Input】 The buffer length pointed to by pcBuffer
 * 
 * @Output parameters: pcBuffer【Output】Converted string data
 * @Return value: 0
 * 
 * @Remark:
 * @Call Example:
 */
PR_INT32 PR_nUtilMemcpyEx(PR_UINT8* 			pcData,
						  PR_UINT32 			nDataLength,
						  PR_PADDING_STYLE 		nStyle,
						  PR_UINT8   			cPaddingChar,
						  PR_UINT8* 			pcBuffer,
						  PR_UINT32 			nOutLength);


/*
 * @Function Name: PR_bUtilIsDigitalStr
 * @Function function: Determine whether the input character is a numeric string
 * @Input parameters: pszStrInput【Input】 Source data
 * 
 * @Output Parameters: None
 * @Return value: Numeric character - Return PR_TRUE
 * Other strings - Return PR_FALSE
 * 
 * @Remark:
 * @Call Example:
 */

PR_BOOL PR_bUtilIsDigitalStr(PR_INT8* pszStrInput);


/*
 * @Function Name: PR_nUtilNumberToAmt
 * @Function function: Convert the entered numeric string to an amount format
 * @Input parameters: pszNum【Input】numerical characters
 * nLenOfAmtBuffer【Input】 The size of the output amount string buffer
 * 
 * @Output parameters: pszAmt【Output】 Converted amount string
 * @Return value: Success - Returns the length of the string (>0), with the end character in the length
 * Failed - Return PR_FAILD
 * 
 * @Remark:
 * @Call Example:
 */

PR_INT32 PR_nUtilNumberToAmt(PR_INT8* pszNum, PR_INT32 nLenOfAmtBuffer, PR_INT8 * pszAmt);

/*
 * @Function Name: PR_nUtilAscToLongHex
 * @Function function: Convert the input ASC characters to long long type and return
 * @Input parameters: pszAscData【Input】 The input ASC characters
 * nAscDataLen【Input】The length of the input ASC byte stream
 * 
 * @Return value: Success - Converted long long data
 * Failed - Return PR_FAILD
 * 
 * @Remark:
 * @Call Example:
 */

PR_INT64 PR_nUtilAscToLongHex(PR_INT8 *pszAscData, PR_INT32 nAscDataLen);

/*
 * @Record: create it;
 * 
 * @Function Name: PR_nUtilAddAsc
 * @Function function: add the input ASC characters
 * @Input parameter: theNumberOne【Input】Add number
 * theNumberTwo【Input】Add number
 * 
 * @Output parameters:
 * theResultSum 【Output】 Returns the result of the operation
 * @Return value: Success - Returns the length of the string (>0), with the end character in the length
 * Failed - Return PR_FAILD
 * 
 * @Remark:
 * @Call Example:
 */
// / Asc string
PR_INT32 PR_nUtilAddAsc(PR_INT8* theNumberOne, PR_INT8* theNumberTwo, PR_INT8* theResultSum);


/*
 * @Function Name: PR_nUtilAddAsc
 * @Function function: Subtract the input ASC characters
 * @Input parameters: theNumberOne【Input】 Subtraction
 * theNumberTwo【Input】 is subtracted
 * 
 * @Output parameters:
 * theResult 【Output】 Returns the result of the operation
 * @Return value: Success - Returns the length of the string (>0), with the end character in the length
 * Failed - Return PR_FAILD
 * 
 * @Remark:
 * @Call Example:
 */
// / Sub
PR_INT32 PR_nUtilSubAsc(PR_INT8* theNumberOne, PR_INT8* theNumberTwo, PR_INT8* theResult);

/*
 * @Function Name: PR_nUtilCompareAmount
 * @Function function: Comparison of the input amount
 * @Input parameters: szAmount1【Input】Amount1
 * szAmout2【Input】Amount 2
 * 
 * @Output parameters:
 * 
 * @Return value:
 * 
 * psTheAmount1 > psTheAmount2: Return greater than 0
 * psTheAmount1 = psTheAmount2: Return 0
 * psTheAmount1 < psTheAmount2: Return less than 0
 * 
 * @Remark:
 * @Call Example:
 */
PR_INT32 PR_nUtilCompareAmount(PR_INT8* psTheAmount1, PR_INT8* psTheAmount2);


/*
 * @Function Name: PR_vGetMD5
 * @Function Function: Calculate MD5 Value
 * 
 * @Input parameters:
 * Buf -- The data pointer to calculate
 * Len --The length of data to be calculated
 * 
 * @Output parameters: Digest--md5 calculation result
 * @Return value: Success: Return the length of the MD5 value (fixed to 16)
 * Failed: PR_FAILD
 * @Remark:
 * @Call Example:
 */
PR_INT32 PR_nGetMD5(void* Buf, PR_UINT32 Len, void* Digest);

/*
 * @Function Name: PR_nGetCRC16
 * @Function Function: Calculate CRC16 value
 * 
 * @Input parameters:
 * InitValue --The initial value of CRC16
 * pszStrInput --------------------------------------------------------------------------------------------------------------------------
 * nLenOfBuffer --The length of the CRC16 value data needs to be calculated
 * 
 * @Output Parameters: None
 * @Return value:
 * Return the calculation result
 * @Remark:
 * @Call Example:
 */
PR_UINT16 PR_nGetCRC16(PR_INT32 InitValue, const PR_INT8* pszStrInput, PR_INT32 nLenOfBuffer);

/*
 * @Function Name: RngGenByteVector
 * @Function Function: Generate random numbers
 * 
 * @Input parameters:
 * Len The length of the random data stream
 * 
 * @Output parameters: Ret Random Data Stream
 * @Return value: Success:PR_TRUE
 * Failed: PR_FAILD
 * @Remark:
 * @Call Example:
 */
PR_Bool  RngGenByteVector(void* Ret, PR_UINT32 Len);

PR_INT32 nBcd2Asc(PR_INT8* bcd, PR_INT32 asclen, PR_INT8* asc, PR_INT32 align);

PR_INT32 nAsc2Bcd(PR_INT8* ascstr, PR_INT32 asclen, PR_UINT8* bcdstr, PR_INT32 align);

#ifdef __cplusplus
}
#endif
#endif /* UTILITY_H_ */


