#ifndef _VT_DEBUG_H_
#define _VT_DEBUG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <assert.h>
#include "types.h"

#ifdef _DEBUG
#undef NDEBUG			/* for assert() to work */
#endif

VT_DLL_PROC int g_debug_level;

#define VT_SetDebugLevel(l)  (g_debug_level = l)

#define VT_DEBUG_OFF             0
#define VT_DEBUG_SHOW_ERROR      1
#define VT_DEBUG_SHOW_PROGRESS   2
#define VT_DEBUG_SHOW_ALL        3

/* for debuging VTIDM library */
#define VT_TRACE_ERROR(msgproc)     if (0 < g_debug_level) msgproc
#define VT_TRACE_PROGRESS(msgproc)  if (1 < g_debug_level) msgproc
#define VT_TRACE_ALL(msgproc)       if (2 < g_debug_level) msgproc

/* for debuging device plugins */
#define VT_DEBUG_ERROR(msgproc)     if (0 < g_ddcb->debugLevel) msgproc
#define VT_DEBUG_PROGRESS(msgproc)  if (1 < g_ddcb->debugLevel) msgproc
#define VT_DEBUG_ALL(msgproc)       if (2 < g_ddcb->debugLevel) msgproc

#define VT_MDEBUG_ERROR(msgproc)    if (0 < gm_ddcb->debugLevel) msgproc
#define VT_MDEBUG_PROGRESS(msgproc) if (1 < gm_ddcb->debugLevel) msgproc
#define VT_MDEBUG_ALL(msgproc)      if (2 < gm_ddcb->debugLevel) msgproc
#define VT_MDEBUG_MSG(msgproc)      if (3 < gm_ddcb->debugLevel) msgproc

VT_DLL_PROC void VT_Verbose(char const *fmt, ...); 
/*  #define VT_Verbose printf */

#ifdef _DEBUG
#define VT_DEBUG printf
#else
#define VT_DEBUG
#endif

enum { PB_DEFAULT, PB_ASCII, PB_INT, PB_UINT, PB_HEX };

VT_DLL_PROC void  VT_PrintBuffer(char * buf, int sz, char * str);
VT_DLL_PROC void  VT_PrintBufferEx(int type, char * buf, int sz, char * str);

#ifdef __cplusplus
}
#endif

#endif /* VT_DEBUG_H */


