#ifndef _CMACRO_H__
#define _CMACRO_H__ 

#include <string.h>

#ifdef __cplusplus
extern "C"
{
#endif

/*-----------------------------------------------------------------------------
 *   define   types
 *-----------------------------------------------------------------------------*/
#ifndef uint32_t
#define uint32_t  unsigned int  
#endif

#ifndef uint16_t
#define uint16_t  unsigned short 
#endif

#ifndef uint8_t
#define uint8_t   unsigned char  
#endif

#ifndef uchar
#define uchar   unsigned char  
#endif
/*-----------------------------------------------------------------------------
 *  define 
 *-----------------------------------------------------------------------------*/
#ifndef YES
#define YES       0x01 
#endif

#ifndef NO
#define NO        0x00 
#endif

#ifndef OK
#define OK        0x01 
#endif

#ifndef ERROR
#define ERROR     0x01 
#endif

#ifndef ON
#define ON        0x01 
#endif

#ifndef OFF
#define OFF       0x00 
#endif

#ifndef NORMAL
#define NORMAL    0x00 
#endif

#ifndef EXCEPTION
#define EXCEPTION  0x01 
#endif

#ifndef true
#define true       0x00 
#endif

#ifndef false
#define false      0x01 
#endif

#ifndef yes
#define yes        0x01
#endif

#ifndef no
#define no         0x00 
#endif

#ifndef success
#define success    0x00 
#endif

#ifndef fail
#define fail       0x01 
#endif

/**************************************************
       define
***************************************************/
//Get the offset of member MEMBER in structure TYPE
#ifndef FPOS
#define FPOS(TYPE, MEMBER) ((unsigned int)&((TYPE *)0)->MEMBER)
#endif

//Get the size of the MEMBER member of the TYPE structure
#ifndef FSIZE
#define FSIZE(TYPE, MEMBER) (sizeof(((TYPE*)0)->MEMBER))
#endif

#ifndef LEN
#define LEN(x,Type)   (sizeof(x)/sizeof(Type))
#endif

#ifndef DIM
#define DIM(x)    (sizeof(x)/sizeof(x[0]))   
#endif

#ifndef CLRBUF
#define CLRBUF(Buf)     memset(Buf, 0, sizeof(Buf))  
#endif

#ifndef BUFCLR
#define BUFCLR(Buf)      memset(Buf, 0, sizeof(Buf)) 
#endif

#ifndef ZERO_STRUCT
#define ZERO_STRUCT(x)     memset((char *) &(x), 0, sizeof(x))
#endif

#ifndef CLRSTRU
#define CLRSTRU(x)      memset((char *) &(x), 0, sizeof(x)) 
#endif

#ifndef FILLBUF
#define FILLBUF(Buf,x)  memset(Buf, x, sizeof(Buf)) 
#endif

#ifndef MAX
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#endif

#ifndef MIN
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#endif


/**************************************************
       Transform macro definition
***************************************************/

#ifndef BCD2BIN
#define BCD2BIN(x)         (((x) & 0x0f) + ((x)>>4)*10)
#endif

#ifndef BIN2BCD
#define BIN2BCD(x)          ((((x)/10)<<4) + (x)%10) 
#endif

#ifndef LHALFB
#define LHALFB(x)     (unsigned char)(x&0x0F)  //Get the low half byte
#endif

#ifndef HHALFB
#define HHALFB(x)     (unsigned char)(x>>4)    //Get the high half byte
#endif

#ifndef LBYTE
#define LBYTE(x)      (unsigned char)(x)       //Get 0-7 digits
#endif

#ifndef HBYTE
#define HBYTE(x)      (unsigned char)(x>>8)    //Get 8-15 digits
#endif

#ifndef H2BYTE
#define H2BYTE(x)     (unsigned char)(x>>16)   //Get 16-23 digits
#endif

#ifndef H3BYTE
#define H3BYTE(x)     (unsigned char)(x>>24)   //Get 24-31 digits
#endif

#ifndef LHALFW
#define LHALFW(x)     (unsigned short)(x)      //Get low half letters
#endif

#ifndef HHALFW
#define HHALFW(x)     (unsigned short)(x>>16)  //Obtain high half width characters
#endif
//----------------位操作相关------------------
#ifndef IFBIT
	#define IFBIT(Data,bit)      (Data&(1<<bit))  //
#endif

#ifndef GETBIT
	#define GETBIT(Data,bit)     ((Data&(1<<bit))>>bit)  //
#endif

// Move the old bit to a new bit, such as 00100000 MOVEBIT (0x20,5,3)=00001000
#ifndef MOVEBIT
	#define MOVEBIT(Data,oldbit,newbit)     (((Data&(1<<oldbit))>>oldbit)<<newbit)
#endif
// Specify the position 'old' to move the Offset bit to the right, for example: 00100000 MOVERBIT (0x20,5,2)=00001000
#ifndef MOVERBIT
	#define MOVERBIT(Data,oldbit,OffsetBit)    ((Data&(1<<oldbit))>>OffsetBit)
#endif
// Move the old position to the left offset bit, such as 00100000 MOVELBIT (0x20,5,2)=1000000
#ifndef MOVELBIT
	#define MOVELBIT(Data,oldbit,OffsetBit)    ((Data&(1<<oldbit))<<OffsetBit)
#endif

#ifndef SETBIT
	#define SETBIT(Data,bit)     (((Data |(1<<bit)))
#endif

#ifndef CLRBIT
	#define CLRBIT(Data,bit)   ((Data &(~(1<<bit))))
#endif
//----------------Byte conversion operation related----------------
//Half byte conversion b3<->b0 b2<->b1
#ifndef CONVERTHBYTE
	#define CONVERTHBYTE(x)  (MOVERBIT(x,3,(3-0))|MOVERBIT(x,2,(2-1))|MOVELBIT(x,0,(3-0))|MOVELBIT(x,1,(2-1)))
#endif
//Byte conversion b7<->b0 b6<->b1 b5<->b2 b4<->b3
#ifndef CONVERTBYTE
	#define CONVERTBYTE(x)  ((x>>7)|MOVERBIT(x,6,(6-1))|MOVERBIT(x,5,(5-2))|MOVERBIT(x,4,(4-3))|MOVELBIT(x,1,(6-1))|MOVELBIT(x,2,(5-2))|MOVELBIT(x,3,(4-3))|(x<<7))
#endif
//----------------Byte merge operation related----------------
//Merge half bytes into 8 bits
#ifndef COMBINE8
	#define COMBINE8(High,Low)         ((unsigned char)(High<<4) | (unsigned char)(Low & 0x0F))
#endif
//Merge bytes into 16 bits
#ifndef COMBINE16
	#define COMBINE16(High,Low)        ((unsigned short)(High<<8) | (unsigned short)(Low & 0xFF))
#endif
//Merge 4 bytes into 32 bits
#ifndef COMBINE32
	#define COMBINE32(Hhigh,Hlow,Lhigh,Llow)  ( (unsigned long)(Hhigh<<24) | (unsigned long)(((unsigned char)Hlow)<<16) | (unsigned long)(((unsigned char)Lhigh)<<8) | (unsigned long)((unsigned char)Llow))
#endif



/**************************************************
       Determine the definition of return value macros
***************************************************/
#define  assert_ok(x)             {if(x){return EXCEPTION;}}   //Non-zero returns EXCEPTION
#define  assert_null(x)           {if(!x)  return 1;} 
#define  ASSERT(p)

#define  ASSERT_NORMAL(x)              assert_ok(x)
#define  ASSERT_TRANSBOUNDARY(x,y)    {if(x>y){DISPERR(x);return 1;}} /* Cross border detection */
#define  STOP_LOOP()                  {while(1){sleep(60);}}

#define EM_SHOW_MASK       (uchar)1
#define EM_SHOW_NOMASK     (uchar)0


#ifndef FALSE
#define FALSE (1 == 0)
#endif

#ifndef TRUE
#define TRUE  (1 == 1)
#endif


#ifdef __cplusplus
}
#endif
#endif



