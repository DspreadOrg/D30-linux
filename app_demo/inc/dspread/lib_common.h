/*
 * File Name:
 * kernel_api.h
 * Author:
 * SW R&D Department
 * Version:
 * V1.0
 * Description:
 */

#ifndef _LIB_COMMON_H
#define _LIB_COMMON_H
#include <stdbool.h>
/*
 * -----------------------------------------------------------------------------
 * |   Includes
 * +----------------------------------------------------------------------------
 */


/*
 * -----------------------------------------------------------------------------
 * |   Macros
 * +----------------------------------------------------------------------------
 */
#ifndef NULL
#define NULL        ((void *)0)
#endif

/*
 * Big-Endian Pointer to Long
 */
#define BE_PtrToLongLong(ptr)           \
	( \
		(((unsigned long long)(*((u8 *)(ptr) + 0))) << 56) | \
		(((unsigned long long)(*((u8 *)(ptr) + 1))) << 48) | \
		(((unsigned long long)(*((u8 *)(ptr) + 2))) << 40) | \
		(((unsigned long long)(*((u8 *)(ptr) + 3))) << 32)| \
		(((unsigned long long)(*((u8 *)(ptr) + 4))) << 24) | \
		(((unsigned long long)(*((u8 *)(ptr) + 5))) << 16) | \
		(((unsigned long long)(*((u8 *)(ptr) + 6))) << 8) | \
		(((unsigned long long)(*((u8 *)(ptr) + 7))) << 0) \
	)

/*
 * Big-Endian Pointer to Long
 */
#define BE_PtrToLong(ptr)           \
	( \
		(((u32)(*((u8 *)(ptr) + 0))) << 24) | \
		(((u32)(*((u8 *)(ptr) + 1))) << 16) | \
		(((u32)(*((u8 *)(ptr) + 2))) << 8) | \
		(((u32)(*((u8 *)(ptr) + 3))) << 0) \
	)

/*
 * Big-Endian Pointer to Short
 */
#define BE_PtrToShort(ptr)          \
	( \
		(((u16)(*((u8 *)(ptr) + 0))) << 8) | \
		(((u16)(*((u8 *)(ptr) + 1))) << 0) \
	)

/*
 * Little-Endian Pointer to Long
 */
#define LE_PtrToLong(ptr)           \
	( \
		(((u32)(*((u8 *)(ptr) + 0))) << 0) | \
		(((u32)(*((u8 *)(ptr) + 1))) << 8) | \
		(((u32)(*((u8 *)(ptr) + 2))) << 16) | \
		(((u32)(*((u8 *)(ptr) + 3))) << 24) \
	)

/*
 * Little-Endian Pointer to Short
 */
#define LE_PtrToShort(ptr)          \
	( \
		(((u16)(*((u8 *)(ptr) + 0))) << 0) | \
		(((u16)(*((u8 *)(ptr) + 1))) << 8) \
	)

/*
 * Long Split
 */
#define LONG_HH(val)        ((((u32)(val)) >> 24) & 0xff)
#define LONG_HL(val)        ((((u32)(val)) >> 16) & 0xff)
#define LONG_LH(val)        ((((u32)(val)) >> 8) & 0xff)
#define LONG_LL(val)        (((u32)(val)) & 0xff)
#define LONG_H(val)         ((((u32)(val)) >> 16) & 0xffff)
#define LONG_L(val)         (((u32)(val)) & 0xffff)

/*
 * Short Split
 */
#define SHORT_H(val)        ((((u16)(val)) >> 8) & 0xff)
#define SHORT_L(val)        (((u16)(val)) & 0xff)

#define BIT(idx)            ((u32)0x01 << (idx))
#define BIT_GET(var, idx)   ((((var) & BIT(idx)) == 0) ? 0 : 1)
#define BIT_SET(var, idx)   ((var) |= BIT(idx))
#define BIT_CLR(var, idx)   ((var) &= ~(BIT(idx)))

/*
 * -----------------------------------------------------------------------------
 * |   Enumerations
 * +----------------------------------------------------------------------------
 */

typedef enum _sha_type_t
{
    SHA_1,    // 120 bits
    SHA_128,  //md5  128bits
	SHA_224,
    SHA_256,
    SHA_384,
    SHA_512,
}Sha_Type_t;

typedef enum _des_type_t
{
    DES_ECB_ENCRYPT,
    DES_ECB_DECRYPT,
    DES_CBC_ENCRYPT,
    DES_CBC_DECRYPT,
}Des_Type_t;

typedef enum
{
    AES_ENCRYPT,
    AES_DECRYPT,

    AES_ECB_ENCRYPT,
    AES_ECB_DECRYPT,
    AES_CBC_ENCRYPT,
    AES_CBC_DECRYPT,

}AES_Type_t;

/*
 * -----------------------------------------------------------------------------
 * |   Typedefs
 * +----------------------------------------------------------------------------
 */
typedef signed long long	s64,   *ps64;
typedef signed long 		s32,   *ps32;
typedef signed short		s16,   *ps16;
typedef signed char 		s8,    *ps8;
typedef signed int			sint,  *psint;

typedef unsigned long long	u64,   *pu64;
typedef unsigned long		u32,   *pu32;
typedef unsigned short		u16,   *pu16;
typedef unsigned char		u8,    *pu8;
typedef unsigned int		uint,  *puint;

typedef void			   *pvoid;
typedef const void		   *pcvoid;

typedef const s64		   *pcs64;
typedef const s32		   *pcs32;
typedef const s16		   *pcs16;
typedef const s8		   *pcs8;
typedef const sint		   *pcsint;
typedef const u64		   *pcu64;
typedef const u32		   *pcu32;
typedef const u16		   *pcu16;
typedef const u8		   *pcu8;
typedef const uint		   *pcuint;
typedef const char 		   *pc8;



typedef u32 (*pCallback_Process_EntryPoint_Fun)(pu8 param,pu32 len);
typedef u32 (*pCallback_Process_EMV)(pu8 param,pu32 len);


// Translate baseKernel.h in rupay,pure kernel, global.h in amex
// The general functions in the following are declared here
typedef struct ClessSignal_Message
{
	unsigned char	ucSignalType;                /* DET, RA, STOP, CLEAN, TIMEOUT, ACT */
	unsigned char   *MessageData;
	unsigned int	uiMessageDatalen;
}ClessSignal_Message;

typedef struct EMV_Queue
{
	ClessSignal_Message *pElement;
	unsigned char front;
	unsigned char rear;
	unsigned char ucMaxElement;
}EMV_Queue;



/*
 * -----------------------------------------------------------------------------
 * |   Variables
 * +----------------------------------------------------------------------------
 */



/*
 * -----------------------------------------------------------------------------
 * |   Constants
 * +----------------------------------------------------------------------------
 */



/*
 * -----------------------------------------------------------------------------
 * |   prototypes
 * +----------------------------------------------------------------------------
 */
#ifdef __cplusplus
extern "C"
{
#endif

extern int Printf_(const char *fmt, ...);
extern int Sprintf_(char *str, const char *fmt, ...);

extern int Memcmp_(const void *str1, const void *str2, int n);
extern void * Memcpy_(void *dest, const void *src, int n);
extern void * Memset_(void *str, int c, int n);
extern int Strlen_(const char *str);

extern u32 tlv_new_(pu8 pTlvData,u32 T,u32 L,pu8 pV);
extern u32 tlv_get_t_(pu8 pTlvData);
extern u32 tlv_get_t_len_(pu8 pTlvData);
extern u32 tlv_get_l_(pu8 pTlvData);
extern u32 tlv_get_l_len_(pu8 pTlvData);
extern pu8 tlv_get_v_(pu8 pTlvData);
extern u32 tlv_get_node_l_(pu8 pTlvData);
extern u32 tlv_add_child_(pu8 pTlvData,u32 T,u32 L,pu8 pV);
extern pu8 tlv_find_(pu8 pTlvData,u32 T);
extern u32 tlv_delete_(pu8 pTlvData,u32 T);
extern u32 tlv_modify_(pu8 pTlvData,u32 T,u32 L,pu8 pV);
extern u32 tlv_replace_(pu8 pTlvData, u32 T, u32 L, pu8 pV);
extern u32 tlv_move_(pu8 pTlvData, pu8 pTlvDstData, u32 T);
extern int tlv_is_composite_(pu8 pTlvData);
extern pu8 tlv_get_first_child_(pu8 pTlvData);
extern pu8 tlv_get_next_child_(pu8 pTlvData);
extern u32 tlv_batch_move_(pu8 pTlvData, pu8 pTlvDstData);

#ifdef __cplusplus
}
#endif
/*
 * End of File
 */

#undef GLOBAL

#endif  /* #ifndef _KERNEL_API_H */

