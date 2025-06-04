#ifndef TYPE_DEFINE_H
#define TYPE_DEFINE_H

//app data type define
#define int64    long long
#define uint64	 unsigned  long long
#define uint32	 unsigned  int
#define uchar	 unsigned  char

#define PR_BOOL		long
#define PR_INT64    long long
#define PR_INT32	int
#define PR_INT16 	short
#define PR_INT8		char

#define PR_UINT64	unsigned  long long
#define PR_UINT32	unsigned  int
#define PR_UINT16	unsigned  short
#define PR_UINT8	unsigned  char

#define PR_TRUE		(1==1)
#define PR_FALSE	(1==0)

#define PR_Bool		PR_BOOL
#define PR_Int64	PR_INT64
#define PR_Int32	PR_INT32
#define	PR_Int16	PR_INT16
#define PR_Int8		PR_INT8

#define PR_Uint64	PR_UINT64
#define	PR_Uint32	PR_UINT32
#define PR_Uint16	PR_UINT16
#define PR_Uint8	PR_UINT8

#define PR_True		PR_TRUE
#define PR_False	PR_FALSE

#define PR_bool		PR_BOOL
#define PR_int64	PR_INT64
#define PR_int32	PR_INT32
#define	PR_int16	PR_INT16
#define PR_int8		PR_INT8

#define PR_uint64	PR_UINT64
#define	PR_uint32	PR_UINT32
#define PR_uint16	PR_UINT16
#define PR_uint8	PR_UINT8

#define PR_true		PR_TRUE
#define PR_false	PR_FALSE

#define PR_NULL  NULL
#define PR_NORMAL   0 // success
#define PR_FAILD   -1
#define PR_TIMEOUT -2
#define PR_CANCLE  -3
#define PR_BYPASS  -4
#define PR_NOT_REC -5  //Not Rec
#define PR_ICC_ERR -6




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


#endif
