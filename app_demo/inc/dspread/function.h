#ifndef _FUNCTION_H__
#define _FUNCTION_H__
#ifdef __cplusplus
extern "C"
{
#endif
#include "appinc.h"

int msb_byte4_to_uint32 (const unsigned char byte[4]);
int lsb_byte4_to_uint32 (const unsigned char byte[4]);
unsigned short msb_byte2_to_uint16 (const unsigned char byte[2]);
unsigned short lsb_byte2_to_uint16 (const unsigned char byte[2]);
void msb_uint32_to_byte4 (unsigned int byte4,unsigned char byte[4]);
void lsb_uint32_to_byte4 (unsigned int byte4,unsigned char byte[4]);
void msb_uint16_to_byte2 (unsigned short byte2,unsigned char byte[2]);
void lsb_uint16_to_byte2 (unsigned short byte2,unsigned char byte[2]);
int c_asc2hex (unsigned char *in, unsigned char *out);
int c_hex2asc (unsigned char in,unsigned char *out);
int c_str2hex (unsigned int inlen,const void *inbuf, void *outbuf);
int c_hex2str (unsigned int inlen, const void *inbuf,void *outbuf);
int c_compress_str (unsigned int inlen,const void *inbuf,void *outbuf);
int c_decompress_str(unsigned int inlen, const void *inbuf,void *outbuf);
unsigned char c_makelrc(unsigned char start, unsigned int inlen, const void *inbuf);
int c_checklrc(uint8_t start, uint32_t inlen, const void *in, uint8_t check);
unsigned int c_make4lrc(unsigned int start, unsigned int inlen, const void *inbuf);
int c_check4lrc(unsigned int start, unsigned int inlen, const void *inbuf, unsigned int check);
int c_buf8xor(unsigned int inlen, const void *inbuf, void *outbuf);
int c_bufxor(unsigned int len, const void *inbuf1, const void *inbuf2, void *outbuf);
int c_make_crc16_ccitt (unsigned int inlen, unsigned char *inbuf,unsigned char *crc16);
void c_bcd2asc(unsigned char* ascii_buf, unsigned char* bcd_buf, int conv_len, unsigned char type);
void c_asc2bcd (unsigned char* bcd_buf, unsigned char* ascii_buf, int conv_len, unsigned char type);
char *c_lower2upper(char *str);
char *c_upper2lower(char *str);
int c_get_visible_char (int len, char *in, char *out);
int PubStrcmp(const char *s1,const char* s2);
int PubStrcpy(char *s1,const char* s2);
void c_asc2dcb(char *data, int len);
int c_string_to_ip (unsigned char *string, unsigned char *ip);
int  PubAscToHex(unsigned char *upcHexBuf, unsigned char *upcAscBuf,  int nConvLen, unsigned char ucType);
int  PubHexToAsc(unsigned char *upcAscBuf, unsigned char *upcHexBuf,int nConvLen, unsigned char ucType);
unsigned short General_memcpy(unsigned char *string1, unsigned char *string2, unsigned short len);

#ifdef __cplusplus
}
#endif
#endif
