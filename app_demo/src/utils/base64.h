#ifndef _BASE64_H_
#define _BASE64_H_

u32 base64_encode( const unsigned char * bindata, char * base64, u32 binlength );
u32 base64_decode( const char * base64, unsigned char * bindata );
#endif
