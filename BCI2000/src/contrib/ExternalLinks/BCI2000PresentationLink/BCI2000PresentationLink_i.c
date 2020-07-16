

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


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


#ifdef __cplusplus
extern "C"{
#endif 


#include <rpc.h>
#include <rpcndr.h>

#ifdef _MIDL_USE_GUIDDEF_

#ifndef INITGUID
#define INITGUID
#include <guiddef.h>
#undef INITGUID
#else
#include <guiddef.h>
#endif

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        DEFINE_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8)

#else // !_MIDL_USE_GUIDDEF_

#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        const type name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

#endif !_MIDL_USE_GUIDDEF_

MIDL_DEFINE_GUID(IID, LIBID_BCI2000PresentationLinkLib,0x92EB0322,0x0798,0x48D0,0xBB,0x72,0xD2,0xAD,0xDE,0x4B,0x77,0x81);


MIDL_DEFINE_GUID(CLSID, CLSID_BCI2000PCLLibrary,0xE03BF661,0x4CDE,0x4252,0x86,0xD8,0x93,0x32,0x60,0x25,0x5E,0x8C);


MIDL_DEFINE_GUID(CLSID, CLSID_BCI2000ControllerType,0x69E9A267,0xDFDC,0x4ACE,0xB6,0x90,0x63,0xD9,0x52,0xE7,0x4E,0xC1);


MIDL_DEFINE_GUID(CLSID, CLSID_BCI2000Controller,0xE0DC5246,0x7815,0x4C37,0x9D,0x85,0x4D,0xBE,0xC8,0x2F,0x4D,0x0B);


MIDL_DEFINE_GUID(CLSID, CLSID_BCI2000DataPortExtension,0x20F54217,0x4B33,0x4956,0x99,0x61,0x30,0x52,0x44,0x7F,0x2F,0x79);


MIDL_DEFINE_GUID(CLSID, CLSID_BCI2000DataPort,0xE2E0A5C1,0xE632,0x402B,0xAB,0x24,0x3C,0xE3,0xA5,0xF2,0x38,0xD3);


MIDL_DEFINE_GUID(CLSID, CLSID_BCI2000ActiveDataPort,0x32950798,0xDDBD,0x46B4,0xAB,0xA3,0x34,0x38,0xFD,0x94,0xB4,0x2D);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



