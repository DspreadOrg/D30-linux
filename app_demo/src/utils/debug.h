#pragma once

#include "osal.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define CFG_DBG

#ifdef CFG_DBG
static inline void getInfo( const char* module, int line, const char* func ){
    OsLog(LOG_DEBUG, "\rDspread: %s | %s | %d | ", module, func,line);
}

#define DSP_Debug()  do{ getInfo(__FILE__, __LINE__, __FUNCTION__);}while(0)
#define DSP_Info(fmt, ...)  do{ getInfo(__FILE__, __LINE__, __FUNCTION__); OsLog(LOG_DEBUG,fmt,##__VA_ARGS__);}while(0)

#else
	#define DSP_Debug() ((void)0)
	#define DSP_Info(fmt, ...)  ((void)0)
#endif


#ifdef __cplusplus
}
#endif
