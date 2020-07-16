/************************************************************************
 * TITLE:       NIDAQEx.h
 *              Header for supporting code module for NI-DAQ Examples
 *
 * DESCR:       This header file is to be used with any NI-DAQ example
 *              program that uses the supporting code module, NIDAQEx.c(pp).
 *
 * Copyright (c) 1997, National Instruments Corp.  All rights reserved.
 ************************************************************************/

#ifndef  _NIDAQEX_H_
#define  _NIDAQEX_H_


/*
 * Includes
 */

#ifdef _CVI_

   /* for LabWindows/CVI */
   #include <ansi_c.h>
   #include <utility.h>   /* for Delay() and KeyHit()              */
   #include <userint.h>   /* for YGraphPopup() and MessagePopup()  */

   /* NI-DAQ related includes ------------------------------------ */

   #include <dataacq.h>   /* for all NI-DAQ funcs and consts       */

   /* ------------------------------------------------------------ */

   /* for declarations below - windows.h #defines these for winApps */
   #ifndef WINAPI
      /* assign to CVIFUNC, #defined in "cvidef.h" */
      #define WINAPI CVIFUNC
   #endif
   // for use in examples, these are defined as nothing
   #ifndef EXPORT
      #define EXPORT
   #endif
   #ifndef EXPORT32
      #define EXPORT32
   #endif


#else

   /* For other Windows C/C++ IDEs */
   #include <windows.h>
   #include <stdio.h>
   #include <math.h>
   #ifdef WIN32
      #include <conio.h> // for _kbhit
      #include <mmsystem.h> // for timeGetTime
   #endif
   #ifdef WIN16
      #include <io.h> // for _wyield
   #endif

   /* NI-DAQ related includes ------------------------------------ */

   #include "nidaq.h"      /* for NI-DAQ function prototypes       */
   #include "nidaqcns.h"   /* for NI-DAQ constants                 */
   #include "nidaqerr.h"   /* for NI-DAQ error codes               */

   /* -------------------------------------------------------------*/

   // EXPORT HEADERS for DLL exporting
   #ifdef _NIDAQEX_DLL_
      #ifdef WIN32
         #define EXPORT
         #define EXPORT32 __declspec(dllexport)
      #else
         #define EXPORT   __export
         #define EXPORT32
      #endif // #ifdef WIN32
   #else
      // for use in examples, these are defined as nothing
      #define EXPORT
      #define EXPORT32
   #endif  // #ifndef _NIDAQEX_DLL_
#endif



/*
 * Typedefs, defines, constants, etc.
 */
#ifndef _NIDAQ_Header_

typedef char           i8;
typedef unsigned char  u8;
typedef short          i16;
typedef unsigned short u16;
typedef long           i32;
typedef unsigned long  u32;
typedef float          f32;
typedef double         f64;

#endif

typedef i16 nidaqExRetType;



/* for 'lType' */
#define WFM_DATA_U8             0
#define WFM_DATA_I16            2
#define WFM_DATA_F64            4
#define WFM_DATA_U32            7

/* internal constants
   - if using CVI, change as necessary for different waveforms...
   - other IDE users, changes here will not affect anything...
 */
#define WFM_PERIODS            10
#define WFM_MIN_PTS_IN_PERIOD   2
#define WFM_U8_MODULO         256
#define WFM_I16_AMPL         2047
#define WFM_F64_AMPL         4.99
#define WFM_2PI              6.2831853071796


/* error return codes for NIDAQPlotWaveform and NIDAQMakeBuffer */
#ifndef NoError
   #define NoError 0
#endif
/* these error codes are consistent with CVI error codes */
#define NIDAQEX_INVALID_BUFFER         -12
#define NIDAQEX_INVALID_NUMPTS         -14
#define NIDAQEX_INVALID_TYPE           -53


/* C++ wrapper */
#ifdef __cplusplus

   #define CPPHEADER    extern "C" {
   #define CPPTRAILER   }

#else

   #define CPPHEADER
   #define CPPTRAILER

#endif


/*
 * Function prototypes
 */

CPPHEADER

EXPORT32 nidaqExRetType EXPORT WINAPI NIDAQPlotWaveform(void* pvBuffer, u32 lNumPts, u32 lType);
EXPORT32 nidaqExRetType EXPORT WINAPI NIDAQMakeBuffer(void* pvBuffer, u32 lNumPts, u32 lType);
EXPORT32 nidaqExRetType EXPORT WINAPI NIDAQDelay(f64 dSec);
EXPORT32 nidaqExRetType EXPORT WINAPI NIDAQErrorHandler(i16 iStatus, char *strFuncName, i16 iIgnoreWarning);
EXPORT32 nidaqExRetType EXPORT WINAPI NIDAQYield(i16 iYieldMode);
EXPORT32 nidaqExRetType EXPORT WINAPI NIDAQMean(void* pvBuffer, u32 lNumPts, u32 lType, f64* dMean);
EXPORT32 nidaqExRetType EXPORT WINAPI NIDAQWaitForKey(f64 dTimeLimit);

CPPTRAILER


#endif  /* _NIDAQEX_H_ */
