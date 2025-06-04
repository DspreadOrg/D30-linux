
#include "appinc.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <sys/time.h>

#define BASE_TYPE_ASSERT 	PR_bStdAssert

#define Asc2Dec(theAscValue) (theAscValue-'0')
#define Dec2Asc(theDecValue) (theDecValue+'0')

// / definition for CPU Structure

#define PR_BigtoLittle16(A)  (( ((PR_UINT16)(A) & 0xff00) >> 8) | \
                                  	  (( (PR_UINT16)(A) & 0x00ff) << 8))

#define PR_BigtoLittle32(A)     ((( (PR_UINT32)(A) & 0xff000000) >> 24) | \
                                       (( (PR_UINT32)(A) & 0x00ff0000) >> 8)   | \
                                       (( (PR_UINT32)(A) & 0x0000ff00) << 8)   | \
                                       (( (PR_UINT32)(A) & 0x000000ff) << 24))

#define PR_HIGH_NIBBLE(theByte)		((theByte>>4)&0x0F)
#define PR_LOW_NIBBLE(theByte)		(theByte&0x0F)

#define PR_MAKE_BYTE(theHighNibble, theLowNibble) 	((PR_UINT8)(((theHighNibble) << 4)| (theLowNibble)))

#define PR_VALID_RANGE(theValue, theMin, theMax)	((theValue>=theMin) &&(theValue<=theMax))

#define PR_MIN(A, B) (((A) < (B)) ? (A):(B))
#define PR_MAX(A, B) (((A) > (B)) ? (A):(B))

// Simple operation definition
#define MAKEBYTE(l, h)			((unsigned char)(((unsigned char)(l) & 0x0F) | ((((unsigned char)(h)) << 4) & 0xF0)))
#define MAKEWORD(l, h)			((unsigned short)((((unsigned char)(l))&0x00FF) | ((((unsigned short)((unsigned char)(h))) << 8)&0xFF00)))
#define MAKELONG(l, h)			((long)(((unsigned short)(l)) | ((unsigned long)((unsigned short)(h))) << 16))
#define LOWORD(l)				((unsigned short)((l) & 0xFFFF))
#define HIWORD(l)				((unsigned short)(((unsigned long)(l) >> 16) & 0xFFFF))
#define LOBYTE(w)				((unsigned char)((w) & 0xFF))
#define HIBYTE(w)				((unsigned char)(((unsigned short)(w) >> 8) & 0xFF))
#define HIHALFBYTE(b)			((unsigned char)(((b) >> 4) & 0x0F))
#define LOHALFBYTE(b)			((unsigned char)((b) & 0x0F))
#define max(a, b)				(((a) > (b)) ? (a) : (b))
#define min(a, b)				(((a) < (b)) ? (a) : (b))
#define	MAKEPARAM(l, h)			(MAKELONG((l), (h)))
#define	LOPARAM(l)				LOWORD(l)
#define	HIPARAM(l)				HIWORD(l)

// Alignment conversion
#define	ALIGN32BIT(n)			(((n) + 3) & (~3))
#define	ALIGN16BIT(n)			(((n) + 1) & (~1))
#define	ALIGNATOM(n)			ALIGN32BIT(n)

// Define the highest bit
#define	MARKHIGHBIT(nType)		((nType)(~(((nType)-1) >> 1)))

// Define the maximum value of the type
#define	MAXUINT					((UINT)-1)
#define	MAXINT					((INT)0x7FFFFFFF)
#define	MAXULONG				((ULONG)-1)
#define	MAXLONG					((LONG)0x7FFFFFFF)
#define	MAXWORD					((WORD)-1)
#define	MAXSHORT				((short)0x00007FFF)
#define	MAXBYTE					((BYTE)-1)
#define	MAXCHAR					((char)0x0000007F)

#define PR_STR_BUF_LEN(theStringLength) (theStringLength+1)

#define PR_VALID_RANGE(theValue, theMin, theMax)	\
    ((theValue>=theMin) &&(theValue<=theMax))


PR_BOOL _bStdAssertExecIsTrue(PR_BOOL 			bExpressionRet,
								  PR_INT8*		  	szExpression,
								  PR_UINT32 		nLine,
								  PR_INT8*			szFile)
{
	if(bExpressionRet == PR_TRUE) return PR_TRUE;

	OsLog(LOG_ERROR,"%s:[ln:%d,F:%s,FN:%s] EXP Return False ! ", nLine,szExpression,szFile);

	return PR_FALSE;
}


#define PR_bStdAssert(EXPRESSION)  _bStdAssertExecIsTrue((EXPRESSION),(#EXPRESSION),__LINE__, __FILE__)

PR_INT32 HexChar2Bin(PR_UINT8 theHexChar)
{
	if ((theHexChar >= '0')&&(theHexChar <= '9'))
		return theHexChar - '0';
	if ((theHexChar >= 'A')&&(theHexChar <= 'F'))
		return theHexChar - 'A' + 10;
	if ((theHexChar >= 'a')&&(theHexChar <= 'f'))
		return theHexChar - 'a' + 10;

	return -1;
}


PR_UINT8 Dec2Bcd(PR_UINT8 theDecValue)
{
	return ((theDecValue/10)<<4 | (theDecValue%10));
}

PR_UINT8 Bcd2Dec(PR_UINT8 theBcdValue)
{
	return ((theBcdValue>>4)&(0x0F))*10 + (theBcdValue&(0x0F));
}

PR_Bool PR_Hex2Str(PR_UINT8* theHexData, PR_UINT32 theHexDataLength,
                PR_INT8* theAscBuffer, PR_UINT32 theAscBufferLength)
{	
    PR_UINT32 dataIndex;

    BASE_TYPE_ASSERT(theAscBufferLength >= PR_STR_BUF_LEN(theHexDataLength*2));

    if (theAscBufferLength < PR_STR_BUF_LEN(theHexDataLength*2))
    {
        return PR_FALSE;
    }

    for (dataIndex = 0; dataIndex< theHexDataLength; dataIndex++)
    {
        sprintf((theAscBuffer+2*dataIndex), "%02X", theHexData[dataIndex]);
    }

    return PR_TRUE;
}


/*
 * ASC solution to HEX
 * Input: ASC code to be solved
 * ASC code length to be solved
 * Return: HEX after solution
 */
PR_UINT16 Asc2Hex(PR_INT8 *pszAscData, PR_UINT16 wAscDataLen)
{
	PR_UINT16 wRet = 0;
	PR_INT8 *pszPR_NULL = pszAscData + wAscDataLen;
	
	for(; pszAscData < pszPR_NULL; pszAscData ++)
	{
		if(*pszAscData < '0' || *pszAscData > '9')
			return 0;
		wRet *= 10;
		wRet += ((*pszAscData) - '0');
	}
	
	return wRet;
}

PR_UINT32 AscToLongHex(PR_INT8 *pszAscData, PR_UINT16 nAscDataLen)
{
	PR_UINT32 wRet = 0;
	PR_INT8 *pszPR_NULL = pszAscData + nAscDataLen;
	
	for(; pszAscData < pszPR_NULL; pszAscData ++)
	{
		if(*pszAscData < '0' || *pszAscData > '9')
			return 0;
		wRet *= 10;
		wRet += ((*pszAscData) - '0');
	}
	
	return wRet;
}

/*
 * Procedure: Seprate16to8
 * Function: Split 16-bit data to 8-bit
 * Input: pwSrc: Source pointer
 * wSrcSize: Source data size
 * pbDest: target pointer
 * wDestSize: Target buffer space size
 * Output: The size of the target space for the specific operation
 * Desc:
 */
PR_UINT16 Word2Byte(PR_UINT16 *pwSrc, PR_UINT16 wSrcSize, PR_UINT8*pbDest, PR_UINT16 wDestSize)
{
	PR_UINT16 *pwSrcTemp;
	
// Check if the parameters are valid
	if(pbDest == PR_NULL)
		return 0;
	if((wSrcSize << 1) < wDestSize)
		wDestSize = (wSrcSize << 1);
	
	if(wDestSize & 0x0001)
	{
// If the target size is odd, do an odd-digit decompression separately
		*(pbDest + wDestSize - 1) = LOBYTE(*(pwSrc + (wDestSize >> 1)));
	}
	
// Loop, copy from the tail, because you want to consider executing in the same space
	for(pwSrcTemp = (pwSrc + (wDestSize >> 1) - 1), pbDest += wDestSize - 1 - (wDestSize & 0x0001);
		pwSrcTemp >= pwSrc; pwSrcTemp--)
	{
		*pbDest-- = HIBYTE(*pwSrcTemp);
		*pbDest-- = LOBYTE(*pwSrcTemp);
	}
	
	return wDestSize;
}

/*
 * Procedure: Combine8to16
 * Function: Compress 8-bit data to 16-bit
 * Input: pbSrc: Source pointer
 * wSrcSize: Source data size
 * pwDest: Target pointer
 * wDestSize: Target buffer space size
 * Output: The size of the target space for the specific operation
 * Desc:
 */
PR_UINT16 Byte2Word(PR_UINT8* pbSrc, PR_UINT16 wSrcSize, PR_UINT16* pwDest, PR_UINT16 wDestSize)
{
	PR_UINT16 *pwTemp;
	
// Check if the parameters are valid
	if(pwDest == PR_NULL)
		return 0;
	if(((wSrcSize + 1) >> 1) < wDestSize)
		wDestSize = ((wSrcSize + 1) >> 1);
	
// Compression from front to back
	for(pwTemp = pwDest + wDestSize;
		pwDest < pwTemp; pwDest ++, pbSrc += 2)
	{
		*pwDest = MAKEWORD(*pbSrc, *(pbSrc + 1));
	}
	
// If the source is an odd number, clear the last byte
	if((wSrcSize & 0x0001) && (((wSrcSize + 1) >> 1) <= wDestSize))
		*(pwTemp - 1) &= 0x00FF;

	return wDestSize;
}


// /=====================================

void PaddingCopy(PR_UINT8* 			pcBuffer,
				 PR_UINT32 			nOutLength,
				 PR_UINT8* 			pcData,
				 PR_UINT32 			nDataLength,
				 PR_PADDING_STYLE 	nStyle,
				 PR_UINT8   		cPaddingChar)
{
	PR_UINT8* paddingStart;
	PR_UINT8* dataStart;
	PR_UINT32 paddingLength;

	BASE_TYPE_ASSERT(nOutLength >= nDataLength);

	paddingLength = nOutLength - nDataLength;

	if (paddingLength > 0)
	{
		paddingStart = (EM_PADDING_LEFT==nStyle) ? pcBuffer : (pcBuffer+nDataLength);
		dataStart = (EM_PADDING_LEFT==nStyle) ? (pcBuffer+paddingLength) : pcBuffer;

		memcpy(dataStart, pcData, nDataLength);
		memset(paddingStart, cPaddingChar, paddingLength);
	}
	else
	{
		memcpy(pcBuffer, pcData, nDataLength);
	}
}


PR_INT32 PR_nUtilAddAsc(PR_INT8* theNumberOne, PR_INT8* theNumberTwo, PR_INT8* theResultSum)
{
// / FIXME Note test this function
	PR_INT32 numberOneLength;
	PR_INT32 numberTwoLength;
	PR_INT32 minLength;
	PR_INT32 maxLength;
	PR_INT8* minNumber;
	PR_INT8* maxNumber;
	PR_INT32 loop;
	PR_UINT8 carry;

	if(!BASE_TYPE_ASSERT( (theNumberOne!=theResultSum) && (theNumberTwo!=theResultSum)))
	{
		return PR_FAILD;
	}

	numberOneLength = strlen(theNumberOne);
	numberTwoLength = strlen(theNumberTwo);
	minLength 		= PR_MIN(numberOneLength, numberTwoLength);
	maxLength 		= PR_MAX(numberOneLength, numberTwoLength);

// minLength  =  MIN(numberOneLength, numberTwoLength);
// maxLength = MAX(numberOneLength, numberTwoLength);

	if(numberOneLength==minLength)
	{
		minNumber = theNumberOne;
		maxNumber = theNumberTwo;
	}
	else
	{
		minNumber = theNumberTwo;
		maxNumber = theNumberOne;
	}

// Make the same length, with a shorter front fill '0' (ASCII),
// And fill in the shorter values ​​into the results.
	if(maxLength!=minLength)
	{

		memset(theResultSum, '0', maxLength);
		strcpy(theResultSum+(maxLength-minLength), minNumber);
	}
	else
	{
		strcpy(theResultSum, minNumber);
	}

	carry = 0;

	for(loop = maxLength-1; loop >=0; loop--)
	{

		PR_UINT8 singleSum  =  Asc2Dec(maxNumber[loop]) + Asc2Dec(theResultSum[loop]) + carry;

		theResultSum[loop] = Dec2Asc(singleSum% 10);
		carry = singleSum / 10;
	}

	if(0!=carry)
	{
		for(loop = maxLength; loop >=0; loop--)
		{
			theResultSum[loop+1] = theResultSum[loop];
		}
		theResultSum[0] = Dec2Asc(carry);
	}

	return strlen(theResultSum);
}

PR_INT32 PR_nUtilSubAsc(PR_INT8* theNumberOne, PR_INT8* theNumberTwo, PR_INT8* theResult)
{
// / FIXME Note to test this function
	PR_INT32 numberOneLength;
	PR_INT32 numberTwoLength;
	PR_INT32 loop;
	PR_INT32 carry;

	if(!BASE_TYPE_ASSERT( (theNumberOne!=theResult) && (theNumberTwo!=theResult)))
		return PR_FAILD;

	numberOneLength = strlen(theNumberOne);
	numberTwoLength =  strlen(theNumberTwo);

	PaddingCopy((PR_UINT8*)theResult, numberOneLength, (PR_UINT8*)theNumberTwo, numberTwoLength, EM_PADDING_LEFT, '0');
	theResult[numberOneLength] = 0;

	carry = 0;
	for (loop=numberOneLength-1; loop>=0; loop--)
	{
		PR_INT32 singleNumOne;
		if ((Asc2Dec(theNumberOne[loop]) + carry  - Asc2Dec(theResult[loop])) < 0)
		{
// Need to borrow a place
			singleNumOne = 10 + (Asc2Dec(theNumberOne[loop]) + carry);
			carry = -1;
		}
		else
		{
			singleNumOne = (Asc2Dec(theNumberOne[loop]) + carry);
			carry = 0;
		}
		theResult[loop] = (PR_INT8)Dec2Asc(singleNumOne - Asc2Dec(theResult[loop]));
	}

// Remove the extra 0 in front
	if (theResult[0] == '0')
	{
		for (loop = 0; loop < (PR_INT32)strlen(theResult)-1; loop++)
		{
			theResult[loop] = theResult[loop+1];
		}
		theResult[loop] = 0;
	}
	return strlen(theResult);
}


void BinaryXOR(PR_UINT8* theData1, PR_UINT8* theData2, PR_UINT32 theDataLength, PR_UINT8* theResult)
{
	PR_UINT32 i;

	for (i=0; i<theDataLength; i++)
	{
		theResult[i] = theData1[i] ^ theData2[i];
	}
}



/*
 * PR_INT32 Asc2Bcd(PR_UINT8* bcd,  PR_INT8* asc, PR_INT32 len, PR_INT32 fmt)
 * {
 * PR_INT32 i, odd;
 * PR_INT8 c;
 * 
 * odd = len&0x01;
 * 
 * if ( odd && !fmt)
 * bcd++ = (*asc++) & '\x0F';
 * 
 * len >>= 1;
 * 
 * for (i=0; i<len; i++) {
 * c = (((*asc++)  & 0x0F)<< 4);
 * c |= (*asc++) & '\x0F';
 * 
 * bcd++ = c;
 * }
 * 
 * if ( odd && fmt)
 * bcd = (((*asc++)  & 0x0F)<< 4);
 * 
 * return (i+odd);
 * }
 */
/*
 * Function: Convert BCD code to ASCII code.If there is a value of 10->15, it will be converted
 * Change to capital letters: A->F.
 * Parameters: #bcd BCD code characters entered
 * #asclen The length of the ASCII string output
 * #asc output ASCII string
 * #align 1 BCD left fill 0x00, right aligned
 * 0 BCD right fill 0x00, left aligned
 * Return value: 0 Success
 * -1 Failed
 */
PR_INT32 nBcd2Asc(PR_INT8* bcd, PR_INT32 asclen, PR_INT8* asc, PR_INT32 align)
{
	PR_INT32 cnt;
	PR_INT8* pbcd = bcd;
	PR_INT8* pasc = asc;

	if ((asclen & 0x01) && align)
	{  //Determine if it is an odd number and align it towards that direction
		cnt = 1;
		asclen ++;
	}
	else
	{
		cnt = 0;
	}

	for (; cnt < asclen; cnt++, pasc++) {
		*pasc = ((cnt & 0x01) ? (*pbcd ++ & 0x0f) : (*pbcd >> 4));
		*pasc += ((*pasc > 9) ? ('A' - 10) : '0');
	}
	
	*pasc = 0;
// /return 0;
	return asclen;
}



PR_INT32 nAsc2Bcd(PR_INT8* ascstr, PR_INT32 asclen, PR_UINT8* bcdstr, PR_INT32 align)
{
	PR_INT32 i = 0;
	PR_INT32 ch;

	for (i = 0; i < (asclen + 1) / 2; i++)
		*(bcdstr + i) = 0x00;

	if ((asclen & 0x01) && align)
		align = 1;
	else
		align = 0;

	for (i = align; i < asclen + align; i ++) {
		ch = *(ascstr + i - align);
		ch = HexChar2Bin(ch);
		if(ch < 0)
			return -1;
		*(bcdstr + (i / 2)) |= (ch << (((i + 1) % 2) * 4));
	}

// return (asclen + align);
	return (asclen + 1)/2;
}



void uint_2_c4(PR_UINT32 num, PR_UINT8 * c4)
{
	*c4 = (num >> 24);
	*(c4 + 1) = ((num >> 16) & 0x00FF);
	*(c4 + 2) = ((num >> 8) & 0x00FF);
	*(c4 + 3) = (num & 0x00FF);

}


PR_INT32 PR_nUtilBcd2Asc(PR_UINT32 nLenOfBcd,
						 PR_INT8*  pcBCDStreamIn,
						 PR_INT32  nAlign,
						 PR_UINT32 nLenOfBuffer,
						 PR_INT8*  pcAscStreamOut)
{
	return nBcd2Asc(pcBCDStreamIn, nLenOfBcd * 2, pcAscStreamOut, nAlign);
}

PR_INT32 PR_nUtilAsc2Bcd(PR_UINT32  nLenOfAsc,
						  PR_INT8*  pcAscStreamIn,
						  PR_INT32  nAlign,
						  PR_UINT32 nLenOfBuffer,
						  PR_INT8*  pcBcdStreamOut)
{
    PR_INT32   cnt;
    PR_INT8    ch, ch1;

	if ((nLenOfAsc & 0x01) && nAlign)
		ch1 = 0;
	else
		ch1 = 0x55;

    for (cnt = 0; cnt < nLenOfAsc; pcAscStreamIn++, cnt++)
    {
        if (*pcAscStreamIn >= 'a')
            ch = *pcAscStreamIn - 'a' + 10;
        else if (*pcAscStreamIn >= 'A')
            ch = *pcAscStreamIn - 'A' + 10;
        else if (*pcAscStreamIn >= '0')
            ch = *pcAscStreamIn - '0';
        else
            ch = 0;
        if (ch1 == 0x55)
            ch1 = ch;
        else
        {
            *pcBcdStreamOut++ = (ch1 << 4) | ch;
            ch1 = 0x55;
        }
    }

    if (ch1 != 0x55)
        *pcBcdStreamOut = ch1 << 4;

    return (cnt + 1)/2;
}


void PR_vUtilBinaryXOR(PR_UINT32 nDataLength,
								   PR_UINT8* pcData1,
								   PR_UINT8* pcData2,
								   PR_UINT8* pcResult)
{
	BinaryXOR(pcData1, pcData2, nDataLength, pcResult);
}

PR_INT32 PR_nUtilHex2Asc(PR_UINT32 nHexDataLength,PR_UINT8* pcHexData, PR_UINT32 nLenOfBuffer,PR_INT8* pcAscData)
{
	PR_UINT32 nIndex = 0;

	if(!BASE_TYPE_ASSERT(pcHexData != PR_NULL)
	  || !BASE_TYPE_ASSERT(pcAscData != PR_NULL)
	  || !BASE_TYPE_ASSERT(2*nHexDataLength<=nLenOfBuffer))
	{
		return PR_FAILD;
	}

	for(nIndex = 0; nIndex< nHexDataLength; nIndex++)
	{
		sprintf((pcAscData+2*nIndex), "%02x", pcHexData[nIndex]);
	}

	return (2*nHexDataLength);
}

PR_INT8* PR_pcFindFirstNonSpecifiedChar(PR_UINT32 nLenOfStr, PR_INT8* pcString, PR_INT8 cChr, PR_UINT32* nRemainDataLength)
{
	PR_UINT32 i;

	for (i=0; i<nLenOfStr; i++)
	{
		if (cChr!=pcString[i])
		{
			break;
		}
	}

	if (0!=nRemainDataLength)
	{
		*nRemainDataLength = (nLenOfStr-i);
	}

	return (nLenOfStr!=i) ? pcString+i : PR_NULL;
}

PR_INT32 PR_nUtilBuildeString(PR_UINT32 nLenOfStringBuffer, PR_INT8* pcString, PR_UINT32 nLenOfAscStream, PR_UINT8* pcAscStream)
{
	if(PR_FALSE==BASE_TYPE_ASSERT(nLenOfAscStream+1 <= nLenOfStringBuffer))
		return PR_FAILD;

	if (nLenOfAscStream > 0)
	{
		memcpy(pcString, pcAscStream, nLenOfAscStream);
	}

	pcString[nLenOfAscStream] = 0;
	return nLenOfAscStream;
}

PR_INT32 PR_nUtilMemcpyEx(PR_UINT8* 			pcData,
						  PR_UINT32 			nDataLength,
						  PR_PADDING_STYLE 		nStyle,
						  PR_UINT8   			cPaddingChar,
						  PR_UINT8* 			pcBuffer,
						  PR_UINT32 			nOutLength)
{
	PaddingCopy(pcBuffer, nOutLength,pcData,  nDataLength, nStyle, cPaddingChar);
	return 0;
}

PR_BOOL PR_bUtilIsDigitalStr(PR_INT8* pszStrInput)
{
	PR_INT32 nIndex = 0;
	PR_INT32 nLen = strlen((const char*)pszStrInput);

	for(nIndex = 0; nIndex < nLen; nIndex ++)
	{
		if(!isdigit(pszStrInput[nIndex])) return PR_FALSE;
	}
	return PR_TRUE;
}


static char * _TrimLef(const char * pszText, int iLen, char cChar)
{
	int idx = 0;

	while(idx < iLen && pszText[idx] == cChar)
	{
		idx ++;
	}

	return (char *)&pszText[idx];
}

PR_INT32 PR_nUtilNumberToAmt(PR_INT8* pszNum, PR_INT32 nLenOfAmtBuffer, PR_INT8 * pszAmt)
{
	PR_INT8  szBuffer[12+2]={0};
	PR_INT32 nLen = 0;

	if(!PR_bUtilIsDigitalStr(pszNum)) return PR_FAILD;

	memset(szBuffer, 0, sizeof(szBuffer));
	strcpy(szBuffer, _TrimLef(pszNum, strlen(pszNum), '0'));

	nLen = strlen(szBuffer);
	if(nLen <= 0)
	{
		strncpy(pszAmt, "0.00", nLenOfAmtBuffer);
	}
	else if(nLen == 1){
		snprintf(pszAmt, nLenOfAmtBuffer,"0.0%s", szBuffer);
	}
	else if(nLen == 2){
		snprintf(pszAmt, nLenOfAmtBuffer,"0.%s", szBuffer);
	}
	else
	{
// Add characters. and '\0' length
		if(nLen+2 > nLenOfAmtBuffer) return PR_FAILD;

		memcpy(pszAmt, szBuffer, nLen - 2);
		pszAmt[nLen-2] = 0;
		strcat(pszAmt, ".");
		strcat(pszAmt, szBuffer+nLen-2);

	}

	return strlen(pszAmt)+1;
}


PR_INT64 PR_nUtilAscToLongHex(PR_INT8 *pszAscData, PR_INT32 nAscDataLen)
{
	PR_INT64 nRet = 0;
	char *pszPR_NULL = pszAscData + nAscDataLen;

	for(; pszAscData < pszPR_NULL; pszAscData ++)
	{
		if(*pszAscData < '0' || *pszAscData > '9')
			return 0;
		nRet *= 10;
		nRet += ((*pszAscData) - '0');
	}

	return nRet;
}

PR_INT32 PR_nUtilCompareAmount(PR_INT8* psTheAmount1, PR_INT8* psTheAmount2)
{
	const PR_INT8* amount1WithoutLeftZeros;
	const PR_INT8* amount2WithoutLeftZeros;
	const PR_INT8* zero = "0";

	BASE_TYPE_ASSERT(psTheAmount1 != PR_NULL);
	BASE_TYPE_ASSERT(psTheAmount2 != PR_NULL);

// Filter out the extra 0 on the left
	amount1WithoutLeftZeros = PR_pcFindFirstNonSpecifiedChar(strlen(psTheAmount1),psTheAmount1, '0', PR_NULL);
	amount2WithoutLeftZeros = PR_pcFindFirstNonSpecifiedChar(strlen(psTheAmount2),psTheAmount2, '0', PR_NULL);

	amount1WithoutLeftZeros = (PR_NULL==amount1WithoutLeftZeros) ? zero : amount1WithoutLeftZeros;
	amount2WithoutLeftZeros = (PR_NULL==amount2WithoutLeftZeros) ? zero : amount2WithoutLeftZeros;

	if (strlen(amount1WithoutLeftZeros)==strlen(amount2WithoutLeftZeros))
	{
		return strcmp(amount1WithoutLeftZeros, amount2WithoutLeftZeros);
	}
	else
	{
		return strlen(amount1WithoutLeftZeros)-strlen(amount2WithoutLeftZeros);
	}

}


#define MD5_MAX_BUFFER 		64  

#define F(x, y, z) ((z) ^ ((x) & ((y) ^ (z))))
#define G(x, y, z) ((y) ^ ((z) & ((x) ^ (y))))
#define H(x, y, z) ((x)^(y)^(z))  
#define I(x, y, z) ((y)^((x)|(~z)))  

#define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32 - (n))))

#define FF(a, b, c, d, x, s, ac){	/*L*/\
	(a) += F((b), (c), (d)) + (x) + (PR_UINT32)(ac);	/*L*/\
	(a) = ROTATE_LEFT(a, s);	/*L*/\
	(a) += (b);		/*L*/\
	}  
#define GG(a, b, c, d, x, s, ac){	/*L*/\
	(a) += G((b), (c), (d)) + (x) + (PR_UINT32)(ac);	/*L*/\
	(a) = ROTATE_LEFT((a), (s));	/*L*/\
	(a) += (b);		/*L*/\
	}
#define HH(a, b, c, d, x, s, ac){	/*L*/\
	(a) += H((b), (c), (d)) + (x) + (PR_UINT32)(ac);	/*L*/\
	(a) = ROTATE_LEFT((a), (s));	/*L*/\
	(a) += (b);		/*L*/\
	}  
#define II(a, b, c, d, x, s, ac){	/*L*/\
	(a) += I((b), (c), (d)) + (x) + (PR_UINT32)(ac);	/*L*/\
	(a) = ROTATE_LEFT((a), (s));	/*L*/\
	(a) += (b);		/*L*/\
	}  

// MD5 context.
typedef struct {  
	PR_UINT32 State[4];				// state (ABCD) 
	PR_UINT32 Count[2]; 				//bytes of number
	PR_UINT8 Buffer[MD5_MAX_BUFFER];	// input buffer 
} MD5_CTX;  

void TkMD5Init(MD5_CTX* Context);  
void TkMD5Update(MD5_CTX* Context, PR_UINT8* Input, PR_UINT32 InputLen);  
void TkMD5Final(PR_UINT8* Digest, MD5_CTX* Context);  

const PR_UINT32 T1[64] = {
	0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee, 0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501, 
	0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be, 0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821, 
	0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa, 0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8, 
	0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed, 0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a, 
	0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c, 0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70, 
	0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05, 0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665, 
	0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039, 0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1, 
	0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1, 0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
	};
const PR_UINT32 InitMd5[4] = {0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476};    

void TkMD5Init(MD5_CTX* Context)  
{  
	Context->Count[0] = 0;
	Context->Count[1] = 0;
	memcpy(Context->State, InitMd5, sizeof(InitMd5));
} 

static void MD5Transform(PR_UINT32* State, PR_UINT8* Block)
{	
	PR_UINT32 i, j;
	PR_UINT32 a = State[0], b = State[1], c = State[2], d = State[3], x[16];

	memcpy(x, Block, 64);

	for(i = 0; i < 16; i += 4)
	{
		FF(a, b, c, d, x[i], 7, T1[i]);
		FF(d, a, b, c, x[i + 1], 12, T1[i + 1]);
		FF(c, d, a, b, x[i + 2], 17, T1[i + 2]);
		FF(b, c, d, a, x[i + 3], 22, T1[i + 3]);
	}
	for(j = 1; i < 32; i += 4)
	{
		GG(a, b, c, d, x[j & 0x0f], 5, T1[i]);
		j += 5;
		GG(d, a, b, c, x[j & 0x0f], 9, T1[i + 1]);
		j += 5;
		GG(c, d, a, b, x[j & 0x0f], 14, T1[i + 2]);
		j += 5;
		GG(b, c, d, a, x[j & 0x0f], 20, T1[i + 3]);
		j += 5;
	}
	for(j = 5; i < 48; i += 4)
	{
		HH(a, b, c, d, x[j & 0x0f], 4, T1[i]);
		j += 3;
		HH(d, a, b, c, x[j & 0x0f], 11, T1[i + 1]);
		j += 3;
		HH(c, d, a, b, x[j & 0x0f], 16, T1[i + 2]);
		j += 3;
		HH(b, c, d, a, x[j & 0x0f], 23, T1[i + 3]);
		j += 3;
	}
	for(j = 0; i < 64; i += 4)
	{
		II(a, b, c, d, x[j & 0x0f], 6, T1[i]);
		j += 7;
		II(d, a, b, c, x[j & 0x0f], 10, T1[i + 1]);
		j += 7;
		II(c, d, a, b, x[j & 0x0f], 15, T1[i + 2]);
		j += 7;
		II(b, c, d, a, x[j & 0x0f], 21, T1[i + 3]);
		j += 7;
	}
	State[0] += a;
	State[1] += b;
	State[2] += c;
	State[3] += d;
	
// Zeroize sensitive information.
	memset(x, 0, 64); 
} 

void TkMD5Update(MD5_CTX* Context, PR_UINT8* Input, PR_UINT32 InputLen)  
{  
	PR_UINT32 i, Index, PartLen;

 	Index = (Context->Count[1]) & 0x3f;
	if(Context->Count[1] > 0xffffffff - InputLen)
	{
		Context->Count[1] -= ((~InputLen) + 1);
		Context->Count[0]++;
	}
	else
	{
		Context->Count[1] += InputLen;
	}
	PartLen = 64 - Index;

// Transform as many times as possible.
	if(InputLen >= PartLen) 
	{
		memcpy(&Context->Buffer[Index], Input, PartLen);
		MD5Transform(Context->State, Context->Buffer);
		for(i = PartLen; i + 63 < InputLen; i += 64)
		{
			MD5Transform(Context->State, &Input[i]);
		}
		Index = 0;
	}
	else
	{
		i = 0;
	}

// Buffer remaining input
	memcpy(&Context->Buffer[Index], &Input[i], InputLen - i);  
}  

void TkMD5Final(PR_UINT8* Digest, MD5_CTX* Context)   
{  
	PR_UINT32 Bits[2];
	PR_UINT32 Index, PadLen;
	PR_UINT8 Padding[64];
	PR_UINT8 BitLen[8];

	Bits[0] = ((Context->Count[0]) << 3) | (Context->Count[1] >> 29);
	Bits[1] = (Context->Count[1]) << 3;
	Index = (Context->Count[1]) & 0x3f;

// Append padding
	PadLen = (Index < 56) ? (56 - Index) : (120 - Index);
	Padding[0] = 0x80;
	memset(Padding + 1, 0, PadLen - 1);

	TkMD5Update(Context,(PR_UINT8*)Padding, PadLen);
	
// Append length
	BitLen[7] = (PR_UINT8)(Bits[0] >> 24);
	BitLen[6] = (PR_UINT8)((Bits[0] >> 16) & 0xff);
	BitLen[5] = (PR_UINT8)((Bits[0] >> 8) & 0xff);
	BitLen[4] = (PR_UINT8)(Bits[0] & 0xff);
	BitLen[3] = (PR_UINT8)(Bits[1] >> 24);
	BitLen[2] = (PR_UINT8)((Bits[1] >> 16) & 0xff);
	BitLen[1] = (PR_UINT8)((Bits[1] >> 8) & 0xff);
	BitLen[0] = (PR_UINT8)(Bits[1] & 0xff);
	
	TkMD5Update(Context, (PR_UINT8*)BitLen, 8);

	memcpy(Digest, Context->State, 16);
		
// Zeroize sensitive information.
	memset(Context, 0, sizeof(MD5_CTX));  
}  

// Digests a byte buf and ret the result.
PR_INT32 PR_nGetMD5(void* Buf, PR_UINT32 Len, void* Digest)
{  
	MD5_CTX Context;
	PR_UINT8* In;
	PR_UINT8* Out;

	if (Buf == PR_NULL || Digest == PR_NULL)
	{
		return PR_FAILD;
	}
	
	In = (PR_UINT8*)Buf;
	Out = (PR_UINT8*)Digest;
	
	TkMD5Init(&Context);
	
	TkMD5Update(&Context, In, Len);  
	
	TkMD5Final(Out, &Context);

	return 16;
}

PR_UINT16 PR_nGetCRC16(PR_INT32 InitValue, const PR_INT8* pszStrInput,	PR_INT32 nLenOfBuffer)
{
	static PR_UINT32 crcl6_table[16] = /* CRC-16s */
	{
		0x0000, 0xCC01, 0xD801, 0x1400,
			0xF001, 0x3C00, 0x2800, 0xE401,
			0xA001, 0X6C00, 0x7800, 0xB401,
			0x5000, 0x9C01, 0x8801, 0x4400
	};
	PR_UINT16 total; /* the CRC-16 value we compute */
	PR_INT32 r1;
	
	total = InitValue;
/*
 * process each byte
 */
	while ( nLenOfBuffer-- > 0 )
	{
/*
 * do the lower four bits
 */
		r1 = crcl6_table[ total & 0xF ];
		total = ( total >> 4 ) & 0x0FFF;
		total = total ^ r1 ^ crcl6_table[ *pszStrInput & 0xF ];
/*
 * do the upper four bits
 */
		r1 = crcl6_table[ total & 0xF ];
		total = ( total >> 4 ) & 0x0FFF;
		total = total ^ r1 ^ crcl6_table[ ( *pszStrInput >> 4 ) & 0xF ];
/*
 * advance to next byte
 */
		pszStrInput++;
	}
	return total;
}

PR_Bool  RngGenByteVector(void* Ret, PR_UINT32 Len)
{
	PR_UINT8* p = (PR_UINT8*)Ret;
	int i = 0;
        struct timeval tpstart;

    if (Ret == NULL)
    {
        return PR_FALSE;
	}
	
	for(i=0; i<Len; i++)
	{

        gettimeofday(&tpstart,NULL);
        srand(tpstart.tv_usec);
        *p = rand()%0xff;
	    p++;
	}

	return PR_TRUE;
}

u32 str2num(char *strAmt, u32 dwLen)
{
    u32 dwAmt = 0, dwFct = 1;
    int i;
    for(i = dwLen - 1; i >= 0; i--)
    {
        if(strAmt[i] >= '0' && strAmt[i] <= '9')
        {
            dwAmt += (strAmt[i] - '0')*dwFct;
            dwFct *= 10;
        }
    }
    return dwAmt;
}

char *uv2str(char *strRet, T_U8_VIEW *uv)
{
	if(strRet && uv)
	{
		memcpy(strRet, uv->head, uv->len);
		strRet[uv->len] = 0;
    }
	return strRet;
}

int uvcmpstr(T_U8_VIEW *uv, char *str)
{

	u32 i;
	if(uv->len != strlen(str)) return -1;
	for(i = 0; i < uv->len; i++)
		if(uv->head[i] != str[i]) return -1;
	return 0;
}

u32 su2num(T_U8_VIEW *uv)
{
    u32 dwAmt = 0, dwFct = 1;
    int i;
    for(i = uv->len - 1; i >= 0; i--)
    {
        if(uv->head[i] >= '0' && uv->head[i] <= '9')
        {
            dwAmt += (uv->head[i] - '0')*dwFct;
            dwFct *= 10;
        }
    }
    return dwAmt;
}

char *str_in_uv(T_U8_VIEW uvSrc, char *sub)
{
	char *bp, *sp;
	u8 *pTail = uvSrc.head + uvSrc.len;
    if(!UV_OK(uvSrc) || !sub) return (char *)uvSrc.head;

    while(uvSrc.head < pTail)
    {
        bp = (char *)uvSrc.head;
        sp = sub;
        do
        {
            if(!*sp) return (char *)uvSrc.head;
        }while(*sp++ == *bp++);
        uvSrc.head++;
    }
    return NULL;
}

u32 uv2asc(u8 *pAsc, T_U8_VIEW uvBin)
{
	u32 i;
	u8 *pDest = pAsc;
	for(i = 0; i < uvBin.len; i++)
		pDest += sprintf((char *)pDest, "%02X", uvBin.head[i]);
	return (pDest - pAsc);
}

u32 c2_2_uint(pu8  c2)
{
	return ((u32)(*(c2)) << 8) + (*(c2 + 1));
}

u32 c4_2_uint(pu8  c4)
{
	return (((u32)(*c4) << 24) + ((u32)(*(c4 + 1)) << 16) +
	        ((u32)(*(c4 + 2)) << 8) + (*(c4 + 3)));
}
