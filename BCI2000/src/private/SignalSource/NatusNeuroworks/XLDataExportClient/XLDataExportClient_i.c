

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 8.01.0622 */
/* at Tue Jan 19 04:14:07 2038
 */
/* Compiler settings for XLDataExportClient.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 8.01.0622 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */



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
        EXTERN_C __declspec(selectany) const type name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

#endif // !_MIDL_USE_GUIDDEF_

MIDL_DEFINE_GUID(IID, IID_IXLDataExportReceiver,0x18B4D726,0xC3C0,0x415A,0xA8,0xF9,0xFC,0x6E,0xB6,0x80,0xC3,0x7D);


MIDL_DEFINE_GUID(IID, LIBID_XLDATAEXPORTCLIENTLib,0xB7B249CB,0x1CDC,0x49D1,0x83,0xB6,0x37,0xF7,0xEE,0xF5,0x77,0x41);


MIDL_DEFINE_GUID(IID, DIID__IXLDataExportReceiverEvents,0xD9B66E4B,0x899F,0x4400,0x84,0x6A,0xC3,0xAD,0x36,0x76,0x4E,0x7D);


MIDL_DEFINE_GUID(CLSID, CLSID_XLDataExportReceiver,0x301D67B5,0xE7CA,0x46AD,0xAA,0xEB,0x48,0xFD,0xE5,0x38,0xCB,0xF4);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



