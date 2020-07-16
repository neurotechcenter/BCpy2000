

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0500 */
/* at Tue Mar 27 18:48:45 2012
 */
/* Compiler settings for .\BCI2000PresentationLink.idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext, robust
    error checks: stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__


#ifndef __BCI2000PresentationLink_i_h__
#define __BCI2000PresentationLink_i_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __BCI2000PCLLibrary_FWD_DEFINED__
#define __BCI2000PCLLibrary_FWD_DEFINED__

#ifdef __cplusplus
typedef class BCI2000PCLLibrary BCI2000PCLLibrary;
#else
typedef struct BCI2000PCLLibrary BCI2000PCLLibrary;
#endif /* __cplusplus */

#endif 	/* __BCI2000PCLLibrary_FWD_DEFINED__ */


#ifndef __BCI2000ControllerType_FWD_DEFINED__
#define __BCI2000ControllerType_FWD_DEFINED__

#ifdef __cplusplus
typedef class BCI2000ControllerType BCI2000ControllerType;
#else
typedef struct BCI2000ControllerType BCI2000ControllerType;
#endif /* __cplusplus */

#endif 	/* __BCI2000ControllerType_FWD_DEFINED__ */


#ifndef __BCI2000Controller_FWD_DEFINED__
#define __BCI2000Controller_FWD_DEFINED__

#ifdef __cplusplus
typedef class BCI2000Controller BCI2000Controller;
#else
typedef struct BCI2000Controller BCI2000Controller;
#endif /* __cplusplus */

#endif 	/* __BCI2000Controller_FWD_DEFINED__ */


#ifndef __BCI2000DataPortExtension_FWD_DEFINED__
#define __BCI2000DataPortExtension_FWD_DEFINED__

#ifdef __cplusplus
typedef class BCI2000DataPortExtension BCI2000DataPortExtension;
#else
typedef struct BCI2000DataPortExtension BCI2000DataPortExtension;
#endif /* __cplusplus */

#endif 	/* __BCI2000DataPortExtension_FWD_DEFINED__ */


#ifndef __BCI2000DataPort_FWD_DEFINED__
#define __BCI2000DataPort_FWD_DEFINED__

#ifdef __cplusplus
typedef class BCI2000DataPort BCI2000DataPort;
#else
typedef struct BCI2000DataPort BCI2000DataPort;
#endif /* __cplusplus */

#endif 	/* __BCI2000DataPort_FWD_DEFINED__ */


#ifndef __BCI2000ActiveDataPort_FWD_DEFINED__
#define __BCI2000ActiveDataPort_FWD_DEFINED__

#ifdef __cplusplus
typedef class BCI2000ActiveDataPort BCI2000ActiveDataPort;
#else
typedef struct BCI2000ActiveDataPort BCI2000ActiveDataPort;
#endif /* __cplusplus */

#endif 	/* __BCI2000ActiveDataPort_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"
#include "PCLExtension.h"
#include "DataPortExtension.h"

#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __BCI2000PresentationLinkLib_LIBRARY_DEFINED__
#define __BCI2000PresentationLinkLib_LIBRARY_DEFINED__

/* library BCI2000PresentationLinkLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_BCI2000PresentationLinkLib;

EXTERN_C const CLSID CLSID_BCI2000PCLLibrary;

#ifdef __cplusplus

class DECLSPEC_UUID("E03BF661-4CDE-4252-86D8-933260255E8C")
BCI2000PCLLibrary;
#endif

EXTERN_C const CLSID CLSID_BCI2000ControllerType;

#ifdef __cplusplus

class DECLSPEC_UUID("69E9A267-DFDC-4ACE-B690-63D952E74EC1")
BCI2000ControllerType;
#endif

EXTERN_C const CLSID CLSID_BCI2000Controller;

#ifdef __cplusplus

class DECLSPEC_UUID("E0DC5246-7815-4C37-9D85-4DBEC82F4D0B")
BCI2000Controller;
#endif

EXTERN_C const CLSID CLSID_BCI2000DataPortExtension;

#ifdef __cplusplus

class DECLSPEC_UUID("20F54217-4B33-4956-9961-3052447F2F79")
BCI2000DataPortExtension;
#endif

EXTERN_C const CLSID CLSID_BCI2000DataPort;

#ifdef __cplusplus

class DECLSPEC_UUID("E2E0A5C1-E632-402B-AB24-3CE3A5F238D3")
BCI2000DataPort;
#endif

EXTERN_C const CLSID CLSID_BCI2000ActiveDataPort;

#ifdef __cplusplus

class DECLSPEC_UUID("32950798-DDBD-46B4-ABA3-3438FD94B42D")
BCI2000ActiveDataPort;
#endif
#endif /* __BCI2000PresentationLinkLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


