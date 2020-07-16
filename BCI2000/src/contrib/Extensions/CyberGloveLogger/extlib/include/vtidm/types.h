/*============================================================================
<<<<<<< types.h
  $Id: types.h,v 1.4 2002/10/11 22:33:49 dgomez Exp $
=======
  $Id: types.h,v 1.4 2002/10/11 22:33:49 dgomez Exp $
>>>>>>> 1.3


  ============================================================================*/
#ifndef _VT_TYPES_H_
#define _VT_TYPES_H_

#ifdef __cplusplus
extern "C" {
#endif

/*==================================================
  basic types
 *==================================================*/
typedef unsigned char       vt_uchar;   /* 8 bits */
typedef unsigned int        vt_uint;  
typedef unsigned long       vt_ulong; 
typedef unsigned short      vt_ushort;
typedef char                vt_bool;
typedef int                 vt_error;

#ifndef vt_handle
typedef void * vt_handle;
#endif

/* irix6.5 defined MIN in sys/param.h */
/*#pragma set woff ??? */
#ifndef _IRIX
  #define MIN(a, b) (((a) > (b)) ? (b):(a))
#endif
/*#pragma reset woff ??? */

#ifndef HIGH_BYTE
#define HIGH_BYTE(s) ((s)>>8)
#endif

#ifndef LOW_BYTE
#define LOW_BYTE(s) ((s) & 0xFF)
#endif

#ifndef MAKE_SHORT
#define MAKE_SHORT(hi, low) ( (hi << 8) | low)
#endif

#ifndef VT_DLL_PROC
#if defined (_WIN32) 
#  if !defined(_TEST_MODULE_)
#    ifdef VT_BUILD_DLL
#      define VT_DLL_PROC __declspec( dllexport ) 
#    else
#      define VT_DLL_PROC __declspec( dllimport ) 
#    endif
#  elif defined (_TEST_MODULE_)
#    if defined (_DLL_MODULE_) 
#      define VT_DLL_PROC
#    else
#      define VT_DLL_PROC __declspec( dllimport ) 
#    endif
#  endif
#else  /* UNIX */
#  ifdef VT_BUILD_DLL
#    ifdef _TEST_MODULE_
#      define VT_DLL_PROC extern 
#    else
#      define VT_DLL_PROC 
#    endif
#  else
#    define VT_DLL_PROC extern
#  endif
#endif
#endif

/* Constants */
#ifndef TRUE			/* in case the macro already exist */
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

/* path seperator */
#ifdef _WIN32
#define DSEP "\\"
#else  /* unix */
#define DSEP "/"
#endif

#define VT_UNIT_CM      0
#define VT_UNIT_INCH    1
#define VT_UNIT_M       2
#define VT_UNIT_RAD     3
#define VT_UNIT_DEG     4

#if !defined (BUFSZ)
#define BUFSZ 16384 //8196 
#endif

#ifdef _WIN32
#include <windows.h>
#endif

#ifdef __cplusplus
}
#endif

#endif







