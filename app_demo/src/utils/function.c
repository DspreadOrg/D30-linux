#include "function.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NORMAL    0x00 
#define EXCEPTION  0x01 

/*
 * Function: msb_byte4_to_uint32
 * Description:  4 bytes to int (Big-Endian)
 * Example:     "\x30\x31\x32\x33" -> 0x30313233
 * Author: none @ 10/16/2018 9:01:33 AM
 * return:
 */
int msb_byte4_to_uint32 (const unsigned char byte[4])
{
    unsigned int tmp = 0;

	tmp = byte[0];
	tmp <<= 8;
	tmp += byte[1];
	tmp <<= 8;
	tmp += byte[2];
	tmp <<= 8;
	tmp += byte[3];

	return tmp;
}		/* -----  end of function msb_byte4_to_uint32  ----- */


/*
 * Function: lsb_byte4_to_uint32
 * Description:  4 bytes to int (Little-Endian)
 * Example:     "\x30\x31\x32\x33" -> 0x33323130
 * Author: none @ 10/16/2018 9:04:44 AM
 * return:
 */
int lsb_byte4_to_uint32 (const unsigned char byte[4])
{
    unsigned int tmp = 0;

	tmp = byte[3];
	tmp <<= 8;
	tmp += byte[2];
	tmp <<= 8;
	tmp += byte[1];
	tmp <<= 8;
	tmp += byte[0];

	return tmp;
}		/* -----  end of function lsb_byte4_to_uint32  ----- */


/*
 * Function: msb_byte2_to_uint16
 * Description:  2 bytes to unsigned short(Big-Endian)
 * Example:     "\x30\x31" -> 0x3031
 * Author: none @ 10/16/2018 9:13:55 AM
 * return:
 */
unsigned short msb_byte2_to_uint16 (const unsigned char byte[2])
{
	return ((byte[0]<<8)+byte[1]);
}		/* -----  end of function msb_byte2_to_uint16  ----- */


/*
 * Function: lsb_byte2_to_uint16
 * Description:  2 bytes to unsigned short(Little-Endian)
 * Example:     "\x30\x31" -> 0x3130
 * Author: none @ 10/16/2018 9:07:25 AM
 * return:
 */
unsigned short lsb_byte2_to_uint16 (const unsigned char byte[2])
{
	return ((byte[1]<<8)+byte[0]);
}		/* -----  end of function lsb_byte2_to_uint16  ----- */


/*
 * Function: msb_uint32_to_byte4
 * Description: unsigned int to byte4 (Big-Endian)
 * Example:     0x30313233 -> "\x30\x31\x32\x33"
 * Author: none @ 10/16/2018 9:18:07 AM
 * return:
 */
void msb_uint32_to_byte4 (unsigned int byte4,unsigned char byte[4])
{
    byte[3] = (unsigned char)byte4;
    byte4 >>= 8;
    byte[2] = (unsigned char)byte4;
    byte4 >>= 8;
    byte[1] = (unsigned char)byte4;
    byte4 >>= 8;
    byte[0] = (unsigned char)byte4;
}		/* -----  end of function msb_uint32_to_byte4  ----- */


/*
 * Function: lsb_uint32_to_byte4
 * Description: unsigned int to byte4 (Little-Endian)
 * Example:     0x30313233 -> "\x33\x32\x31\x30"
 * Author: none @ 10/16/2018 9:20:35 AM
 * return:
 */
void lsb_uint32_to_byte4 (unsigned int byte4,unsigned char byte[4])
{
    byte[0] = (unsigned char)byte4;
    byte4 >>= 8;
    byte[1] = (unsigned char)byte4;
    byte4 >>= 8;
    byte[2] = (unsigned char)byte4;
    byte4 >>= 8;
    byte[3] = (unsigned char)byte4;
}		/* -----  end of function lsb_uint32_to_byte4  ----- */


/*
 * Function: msb_uint16_to_byte2
 * Description:  unsigned short to byte2(Big-Endian)
 * Example:      0x3031 -> "\x30\x31"
 * Author: none @ 10/16/2018 9:22:02 AM
 * return:
 */
void msb_uint16_to_byte2 (unsigned short byte2,unsigned char byte[2])
{
    byte[1] = (unsigned char)byte2;
    byte2 >>= 8;
    byte[0] = (unsigned char)byte2;
}		/* -----  end of function msb_uint16_to_byte2  ----- */


/*
 * Function: lsb_uint16_to_byte2
 * Description:  unsigned short to byte2(Little-Endian)
 * Example:      0x3031 -> "\x31\x30"
 * Author: none @ 10/16/2018 9:24:02 AM
 * return:
 */
void lsb_uint16_to_byte2 (unsigned short byte2,unsigned char byte[2])
{
    byte[0] = (unsigned char)byte2;
    byte2 >>= 8;
    byte[1] = (unsigned char)byte2;
}		/* -----  end of function lsb_uint16_to_byte2  ----- */



/*
 * Function: c_asc2hex
 * Description: 2 byte ASC characters to 1 byte hexadecimal
 * Example: '3''4' -> 0x34
 * Author: none @ 10/16/2018 9:27:13 AM
 * return:
 */
int c_asc2hex (unsigned char *in, unsigned char *out)
{
	unsigned char tmp;
    if ( in == NULL || out == NULL ) {
        return 1;
    }
	if (in[0] >= '0' && in[0] <= '9')
	{
		tmp = (unsigned char)((in[0] - '0') << 4);
	}
	else if (in[0] >= 'a' && in[0] <= 'f')
	{
		tmp = (unsigned char)((in[0] - 'a' + 10) << 4);
	}
	else if (in[0] >= 'A' && in[0] <= 'F')
	{
		tmp = (unsigned char)((in[0] - 'A' + 10) << 4);
	}
	else
	{
        return 1;
	}
	if (in[1] >= '0' && in[1] <= '9')
	{
		tmp |= (unsigned char)(in[1] - '0');
	}
	else if (in[1] >= 'a' && in[1] <= 'f')
	{
		tmp |= ((in[1] - 'a') + 10);
	}
	else if (in[1] >= 'A' && in[1] <= 'F')
	{
		tmp |= ((in[1] - 'A') + 10);
	}
	else
	{
        return 1;
	}
    *out = tmp;
	return 0;
}		/* -----  end of function c_asc2hex  ----- */


/*
 * Function: c_hex2asc
 * Description: 1 byte hexadecimal to 2 byte ASC characters
 * Example: 0x34 -> '3''4'
 * Author: none @ 10/16/2018 9:31:23 AM
 * return:
 */
int c_hex2asc (unsigned char in,unsigned char *out)
{
	unsigned char tmp;
    if ( out == NULL ) {
        return 1;
    }
	tmp = (in>>4);
	if (tmp >= 10)
	{
		tmp += ('A'-10);
	}
	else
	{
		tmp += '0';
	}
	out[0] = tmp;
	tmp = (in&0x0F);
	if (tmp >= 10)
	{
		tmp += ('A'-10);
	}
	else
	{
		tmp += '0';
	}
	out[1] = tmp;
    return 0;
}		/* -----  end of function c_hex2asc  ----- */

/*
 * Function: c_str2hex
 * Description: string to hexadecimal array
 * Example: "123456" -> 0x12 0x34 0x56
 * Author: none @ 10/16/2018 9:35:28 AM
 * return:
 */
int c_str2hex (unsigned int inlen,const void *inbuf, void *outbuf)
{
	unsigned int i;
    unsigned char *in=(unsigned char *)inbuf;
    unsigned char *out=(unsigned char *)outbuf;

    if ( inlen%2 || inbuf == NULL || outbuf == NULL ) {
        return 1;
    }
	for (i=0; i<inlen/2; i++)
	{
        if (c_asc2hex(&in[2*i],&out[i])) {
            return 1;
        }
    }
    return 0;
}		/* -----  end of function c_str2hex  ----- */

/*
 * Function: c_hex2str
 * Description: Hexadecimal string
 * Example: 0x31 0x32 0x33 -> "123"
 * Author: none @ 10/16/2018 9:44:52 AM
 * return:
 */
int c_hex2str (unsigned int inlen, const void *inbuf,void *outbuf)
{
	unsigned int i;
    if ( inlen == 0 || inbuf == NULL || outbuf == NULL ) {
        return 1;
    }
    unsigned char *in=(unsigned char *)inbuf;
    unsigned char *out=(unsigned char *)outbuf;
	for (i=0; i<inlen; i++)
	{
        c_hex2asc(in[i],&out[2*i]);
    }
	return 0;
}		/* -----  end of function c_hex2str  ----- */



/*
 * function: c_asc2bcd
 * description:
 * author:
 * return:
 */
void c_asc2bcd (uchar* bcd_buf, uchar* ascii_buf, int conv_len, uchar type)
{
	int     cnt;
	char    ch, ch1;

	if (conv_len & 0x01 && type)
		ch1 = 0;
	else
		ch1 = 0x55;

	for (cnt = 0; cnt < conv_len; ascii_buf++, cnt++)
	{
		if (*ascii_buf >= 'a')
			ch = *ascii_buf - 'a' + 10;
		else if (*ascii_buf >= 'A')
			ch = *ascii_buf - 'A' + 10;
		else if (*ascii_buf >= '0')
			ch = *ascii_buf - '0';
		else
			ch = 0;
		if (ch1 == 0x55)
			ch1 = ch;
		else
		{
			*bcd_buf++ = (ch1 << 4) | ch;
			ch1 = 0x55;
		}
	}

	if (ch1 != 0x55)
		*bcd_buf = ch1 << 4;

	return;
}		/* -----  end of function c_asc2bcd  ----- */

/*
 * function: c_bcd2asc
 * Description: Implement bcd code to asc
 * author:
 * return:
 */

void c_bcd2asc(uchar* ascii_buf, uchar* bcd_buf, int conv_len, uchar type)
{
	int cnt;

	if (conv_len & 0x01 && type)
	{
		cnt = 1;
		conv_len++;
	}
	else
		cnt = 0;
	for (; cnt < conv_len; cnt++, ascii_buf++)
	{
		*ascii_buf = ((cnt & 0x01) ? (*bcd_buf++ &0x0f) : (*bcd_buf >> 4));
		*ascii_buf += ((*ascii_buf > 9) ? ('A' - 10) : '0');
	}

	return;
}		/* -----  end of function c_bcd2asc  ----- */

/*
 * Function: c_compress_str
 * Description: Compress 2byte->1bytes
 * Example: 0x31 0x3A -> 0x1A
 * Author: none @ 10/16/2018 9:48:59 AM
 * return: 0-succ 1-fail (less than 0x30 or greater than 0x3F)
 */
int c_compress_str (unsigned int inlen,const void *inbuf,void *outbuf)
{
	unsigned int i;
    unsigned char high,low;
    unsigned char *in=(unsigned char *)inbuf;
    unsigned char *out=(unsigned char *)outbuf;
    if ( inlen%2 || inbuf == NULL || outbuf == NULL ) {
        return 1;
    }
    for ( i=0 ; i<inlen ; i++  ) {
        high = in[2*i];
        low = in[2*i+1];
        if ( (high >= 0x30 && high <= 0x3F)
             && (low >= 0x30 && low <= 0x3F)  ) {
              out[i] = (unsigned char)(high<<4) | (unsigned char)(low & 0x0F);
        } else {
            return 1;
        }
    }
    return 0;
}		/* -----  end of function c_compress_str  ----- */


/*
 * Function: c_decompress_str
 * Description:  decompression 1byte->2bytes
 * Example: 0x1A ->  0x31 0x3A
 * Author: none @ 10/16/2018 9:52:32 AM
 * return:
 */
int c_decompress_str(unsigned int inlen, const void *inbuf,void *outbuf)
{
	unsigned int i;
    if ( inlen == 0 || inbuf == NULL || outbuf == NULL ) {
        return 1;
    }
    unsigned char *in=(unsigned char *)inbuf;
    unsigned char *out=(unsigned char *)outbuf;
    for ( i=0 ; i<inlen ; i++  ) {
        out[2*i] = (in[i]>>4)+'0';
        out[2*i+1] = (in[i]&0x0F)+'0';
    }
    return 0;
}		/* -----  end of function c_decompress_str  ----- */


/*
 * Function:c_lower2upper
 * Description: convert capitalization
 * Example: "123aBc" -> "123ABC"
 * Author: none @ 10/16/2018 3:38:12 PM
 * return:
 */
char *c_lower2upper(char *str)
{
    char *orign=str;
    for (; *str!='\0'; str++)
    {
        if ( (*str >= 'a') && (*str <= 'z') ) {
            *str = *str + ('A' - 'a');
        } 
    }
    return orign;
}		/* -----  end of function c_lower2upper   ----- */


/*
 * Function:c_upper2lower
 * Description: Turn lowercase
 * Example: "123aBc" -> "123abc"
 * Author: none @ 10/16/2018 3:39:49 PM
 * return:
 */
char *c_upper2lower(char *str)
{
    char *orign=str;
    for (; *str!='\0'; str++)
    {
        if ( (*str >= 'A') && (*str <= 'Z') ) {
            *str = *str + ('a' - 'A');
        } 
    }
    return orign;
}		/* -----  end of function  c_upper2lower  ----- */


/*
 * Function: c_makelrc
 * Description:  calc lrc
 * Author: none @ 10/16/2018 10:11:07 AM
 * In: start：Initial value
 * return: 1byte lrc
 */
unsigned char c_makelrc(unsigned char start, unsigned int inlen, const void *inbuf)
{
	unsigned int i;
    unsigned char lrc=start;
    unsigned char *in=(unsigned char *)inbuf;
	for (i=0; i<inlen; i++)
	{
		lrc ^= in[i];
	}
	return lrc;
}

/*
 * Function: c_checklrc
 * Description: check lrc
 * Author: none @ 10/16/2018 10:09:27 AM
 * In: Initial value: check: The value that needs to be checked
 * return: 0-succ 1-fail
 */
int c_checklrc(uint8_t start, uint32_t inlen, const void *in, uint8_t check)
{
    if ( c_makelrc(start,inlen,in) == check ) {
        return 0;
    } else {
        return 1;
    }
}

/*
 * Function: c_make4lrc
 * Description:  calc 4lrc
 * Author: none @ 10/16/2018 10:14:11 AM
 * In: start:Initial value
 * return: unsigned int lrc
 */
unsigned int c_make4lrc(unsigned int start, unsigned int inlen, const void *inbuf)
{
    unsigned int *in = (unsigned int *)inbuf;
    unsigned int i,lrc=start;
	for (i=0; i<inlen/4; i++)
	{
		lrc ^= *in++;
	}
	return lrc;
}		/* -----  end of function c_make4lrc  ----- */


/*
 * Function: c_check4lrc
 * Description: check 4byte lrc
 * Author: none @ 10/16/2018 10:17:08 AM
 * In: start: initial value check: value that needs to be compared
 * return: 0-succ 1-fail
 */
int c_check4lrc(unsigned int start, unsigned int inlen, const void *inbuf, unsigned int check)
{
    if ( c_make4lrc(start,inlen,inbuf) == check ) {
        return 0;
    } else {
        return 1;
    }
}		/* -----  end of function c_check4lrc  ----- */


/*
 * Function: c_buf8xor
 * Description: Inlen length array every 8 bytes XOR, and the length is less than 8 times complement 0 XOR
 * Author: none @ 10/16/2018 10:19:43 AM
 * return:
 */
int c_buf8xor(unsigned int inlen, const void *inbuf, void *outbuf)
{
	unsigned int i=0;
	unsigned int j,m,n;
	unsigned char ucIBuf[8];
    unsigned char *in=(unsigned char *)inbuf;
	memset(ucIBuf,0,sizeof(ucIBuf));
	m = inlen/8;
	n = inlen%8;
	while(m--)
	{
		for(j=0;j<8;j++)
		{
			ucIBuf[j] ^= in[i+j];
		}
		i += 8;
	}
	if(n)
	{
		for(j=0;j<n;j++)
		{
			ucIBuf[j] ^= in[i+j];
		}
	}
	memcpy((unsigned char *)outbuf,ucIBuf,8);
    return 0;
}		/* -----  end of function c_buf8xor  ----- */

/*
 * Function: c_bufxor
 * Description: 2 inlen length arrays perform exclusive OR
 * Author: none @ 10/16/2018 10:21:11 AM
 * return:
 */
int c_bufxor(unsigned int len, const void *inbuf1, const void *inbuf2, void *outbuf)
{
	unsigned int i;
    unsigned char *str1=(unsigned char *)inbuf1;
    unsigned char *str2=(unsigned char *)inbuf2;
    unsigned char *out=(unsigned char *)outbuf;
	for(i=0;i<len;i++)
	{
		out[i] = str1[i]^str2[i];
	}
    return 0;
}		/* -----  end of function c_bufxor  ----- */


/*
 * Function: c_make_crc16_ccitt xmodem
 * Description:  crc16
 * Author: none @ 10/16/2018 10:39:00 AM
 * return: 0-succ  1-fail
 */
int c_make_crc16_ccitt (unsigned int inlen, unsigned char *inbuf,unsigned char *crc16)
{
	unsigned int crc= 0;
	unsigned char temp;
    unsigned int table[16]={ /* CRC ??????????*/
        0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50a5, 0x60c6, 0x70e7,
        0x8108, 0x9129, 0xa14a, 0xb16b, 0xc18c, 0xd1ad, 0xe1ce, 0xf1ef
    };
    if ( inbuf == NULL || crc16 == NULL || inlen == 0 ) {
        return 1;
    }
	while (inlen -- != 0)
	{
		temp = ((unsigned char)( crc>>8))>>4;
		crc <<= 4;
		crc ^= table[temp^(*inbuf/16)];
		temp = ((unsigned char)( crc>>8))>>4;
		crc <<= 4;
		crc ^= table[temp^(*inbuf&0x0f)];
		inbuf++;
	}
	crc16[0] = crc/256;
	crc16[1] = crc%256;
    return 0;
}		/* -----  end of function c_make_crc16_ccitt  ----- */

/*
 * function: c_get_visible_char
 * Description: Extract visible characters in front of a field
 * author:
 * return:
 */
int c_get_visible_char (int len, char *in, char *out)
{
    int i = 0;
    for (i=0; i<len; i++)
    {
        if (!( (in[i] >= '0' && in[i] <= '9') 
               ||(in[i] >= 'a' && in[i] <= 'z')
               ||(in[i] >= 'A' && in[i] <= 'Z')
             ) ) {
            break;
        }
        out[i] = in[i];
    }
    return i; 
}		/* -----  end of function c_get_visible_char  ----- */


void c_asc2dcb(char *data, int len)
{
	int i;
	char tmp;
	for(i=0; i<len; i++)
	{
		tmp = ((data[i]&0xf0)>>4)*10;
		tmp += (data[i]&0x0f);
		data[i] = tmp;
	}
}
/*
 * Function description:
 * String comparison
 * Input parameter: Comparative string
 * Return value: 0: Same
 * Others: Different
 */
int PubStrcmp(const char *s1,const char* s2)
{
	while(*s1   &&   *s1==*s2)
	{
		s1++;
		s2++;
	}
	return *s1 - *s2;
}
/*
 * Function description:
 * String copy
 * Enter parameters:
 * Output parameters:
 * Return value: 0: Success
 */
int PubStrcpy(char *s1,const char* s2)
{
	while(*s2)
	{
		*s1++ = *s2++;
	}
	*s1 = 0x00;
	return 0;
}


/*
 * function: c_string_to_ip
 * Description: String IP to 4 char types
 * author:
 * return:
 */
int c_string_to_ip (uchar *string, uchar *ip)
{
    int len = 0;
    char buf[4][8];
    int  j = 0;
    int  i = 0;
    int  k = 0;
    if ( string == NULL || ip == NULL || len > 16 ) {
        return -1;
    }
    len = strlen((char *)string);
    memset(buf,0,4*8);
    for ( i=0 ; i<len ; i++ ) {
        if ( string[i] == '.' ) {
            j++;
            k = 0;
            continue;
        }
        if ( j > 3 ) {
            return -1;
        }
        buf[j][k++] = string[i];
    }
    ip[0] = atoi(buf[0])&0xFF;
    ip[1] = atoi(buf[1])&0xFF;
    ip[2] = atoi(buf[2])&0xFF;
    ip[3] = atoi(buf[3])&0xFF;
    return 0;
}		/* -----  end of function c_string_to_ip   ----- */


/*
 * Function function: convert ASCII string into hexadecimal data, and the converted hexadecimal data length is ASCII
 * Half of the string, ASCII length is odd, after conversion, right-complement zero.
 * Entry parameters: upcAscBuf - ASCII string to be converted
 * nConvLen -- The converted string length
 * ucType - the converted type
 * Export parameters: upcHexBuf - converted hexadecimal data
 * Return Value: NORMAL - Conversion was successful
 * EXCEPTION - Failed, pcHexBuf or pcAscBuf is NULL
 * ERRSIZE - Illegal conversion range
 * ERRCHAR - Illegal characters exist
 */
int  PubAscToHex(uchar *upcHexBuf, uchar *upcAscBuf,
                 int nConvLen, uchar ucType)
{
    int nCnt;
    char cChar, cChar1;

    if (upcHexBuf == NULL || upcAscBuf == NULL)
    {
        ;
        return EXCEPTION;
    }

    if (nConvLen < 0)
    {
        ;
        return EXCEPTION;
    }
    else if (nConvLen == 0)
    {
        upcHexBuf[0] = 0;
        return NORMAL;
    }

    if (nConvLen & 0x01 && ucType)
        cChar1 = 0;
    else
        cChar1 = 0x55;

    for (nCnt = 0; nCnt < nConvLen; upcAscBuf++, nCnt++)
    {
        if (*upcAscBuf >= 'a')
            cChar = *upcAscBuf - 'a' + 10;
        else if (*upcAscBuf >= 'A')
            cChar = *upcAscBuf - 'A' + 10;
        else if (*upcAscBuf >= '0')
            cChar = *upcAscBuf - '0';
        else
            cChar = 0;

        if (cChar1 == 0x55)
            cChar1 = cChar;
        else
        {
            *upcHexBuf++ = (cChar1 << 4) | cChar;
            cChar1 = 0x55;
        }
    }

    if (cChar1 != 0x55)
        *upcHexBuf = cChar1 << 4;

    return NORMAL;
}

/*
 * Function function: Convert hexadecimal data into ASCII string.
 * Entry parameters: upcHexBuf - hexadecimal data to be converted
 * nConvLen -- The converted string length
 * ucType - the string type to convert
 * Export parameters: upcAscBuf - Converted ASCII string
 * Return Value: NORMAL - Conversion was successful
 * EXCEPTION - Failed, pcHexBuf or pcAscBuf is empty
 * ERRSIZE - Error conversion length
 */
int  PubHexToAsc(uchar *upcAscBuf, uchar *upcHexBuf,
                 int nConvLen, uchar ucType)
{
    int nCnt;

    if (upcAscBuf == NULL || upcHexBuf == NULL)
    {
        ;
        return EXCEPTION;
    }

    if (nConvLen < 0)
    {
        ;
        return EXCEPTION;
    }
    else if (nConvLen == 0)
    {
        upcAscBuf[0] = 0;
        return NORMAL;
    }

    if (nConvLen & 0x01 && ucType)
    {
        nCnt = 1;
        nConvLen ++;
    }
    else
        nCnt = 0;
    for (; nCnt < nConvLen; nCnt ++, upcAscBuf ++)
    {
        *upcAscBuf = ((nCnt & 0x01) ? (*upcHexBuf++ & 0x0f) : (*upcHexBuf >> 4));
        *upcAscBuf += ((*upcAscBuf > 9) ? ('A' - 10) : '0');
    }

    return NORMAL;
}


/*
 * Function name: General_memcpy
 * Function description: Same as memcpy usage
 * Entry parameters:
 * Entry parameter description:
 * Export parameters:
 * Export parameter description:
 * Return value: cpy_len: length of copy
 * Return value description:
 */
unsigned short General_memcpy(unsigned char *string1, unsigned char *string2, unsigned short len)
{
	unsigned short cpy_len;

	if(len == 0)
	{
		len = strlen((char *)string2);
	}

	if(!len)
	{
		return 0;
	}
	cpy_len = len;

	while(len--)
	{
		*string1++ = *string2++;
	}
	return cpy_len;
}