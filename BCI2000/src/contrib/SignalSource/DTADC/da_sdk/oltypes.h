#if !defined(__OLTYPES__)
#define __OLTYPES__

/*-----------------------------------------------------------------------

 Copyright  (C)  1992.   Data  Translation,  Inc.,  100  Locke   Drive,
 Marlboro Massachusetts 01752-1192.

 All rights reserved.  This software is furnished to purchaser under  a
 license  for  use  on a single computer system and can be copied (with
 the inclusion of DTI's copyright notice) only for use in such  system,
 except  as  may  be otherwise provided in writing by Data Translation,
 Inc., 100 Locke Drive, Marlboro, Massachusetts 01752-1192.

 The information in this document is subject to change  without  notice
 and  should not be construed as a commitment by Data Translation, Inc.
 Data Translation, Inc.  assumes no responsibility for any errors  that
 may appear in this document.

 Data Translation cannot assume any responsibility for the use  of  any
 portion  of  this  software  on  any  equipment  not  supplied by Data
 Translation, Inc.

FILE NAME: oltypes.h

ABSTRACT:

   This unit contains declarations for the DT Open Layers.


10/13/92
---------------------------------------------------------------------------*/
// If NT, define out this schtuff
#ifdef _WIN32
#define __export
#define __huge
#define _huge
#define huge

#define DllExport __declspec(dllexport)
#define DllImport __declspec(dllimport)

#include <windef.h>

#else // else MUST be Win 3.1

#undef WINAPI
#define WINAPI    __far __pascal __loadds
#undef CALLBACK
#define CALLBACK  __far __pascal __loadds

#endif // if _WIN32

typedef signed char     CHR;
typedef CHR FAR*        LPCHR;
typedef CHR*            PCHR;
typedef unsigned char   UCHR;
typedef UCHR FAR*       LPUCHR;
typedef UCHR*           PUCHR;
typedef short           SHRT;
typedef SHRT FAR*       LPSHRT;
typedef SHRT*           PSHRT;
typedef unsigned short  USHRT;
typedef USHRT FAR*      LPUSHRT;
typedef USHRT*          PUSHRT;
typedef long            LNG;
typedef LNG FAR*        LPLNG;
typedef LNG*            PLNG;
typedef unsigned long   ULNG;
typedef ULNG FAR*       LPULNG;
typedef ULNG*           PULNG;
typedef float           FLT;
typedef FLT FAR*        LPFLT;
typedef FLT*            PFLT;
typedef double          DBL;
typedef DBL FAR*        LPDBL;
typedef DBL*            PDBL;

#if !defined(UINT)
typedef unsigned int    UINT;
#endif

typedef UINT FAR*       LPUINT;
typedef UINT*           PUINT;
typedef BOOL FAR*       LPBOOL;
typedef BOOL*           PBOOL;


typedef unsigned long    ECODE;      // Error/Warning code definition
typedef ECODE FAR*   LPECODE;   // Far pointer to Error/Warning code definition
typedef ECODE*       PECODE;   // Far pointer to Error/Warning code definition

typedef ECODE  OLSTATUS;

typedef enum
{
    DTUSB_BUS,
    DTPCI_BUS,
    DTPCMCIA_BUS,
    DTISA_BUS
} DTBUS_TYPE;

typedef struct
{
   ULONG Info[32];
}  DRIVER_SPECIFIC_INFO, *PDRIVER_SPECIFIC_INFO;

#endif
