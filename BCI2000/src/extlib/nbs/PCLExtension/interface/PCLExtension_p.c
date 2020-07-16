

/* this ALWAYS GENERATED file contains the proxy stub code */


 /* File created by MIDL compiler version 6.00.0361 */
/* at Tue Apr 20 13:50:57 2010
 */
/* Compiler settings for PCLExtension.idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#if !defined(_M_IA64) && !defined(_M_AMD64)


#pragma warning( disable: 4049 )  /* more than 64k source lines */
#if _MSC_VER >= 1200
#pragma warning(push)
#endif
#pragma warning( disable: 4100 ) /* unreferenced arguments in x86 call */
#pragma warning( disable: 4211 )  /* redefine extent to static */
#pragma warning( disable: 4232 )  /* dllimport identity*/
#define USE_STUBLESS_PROXY


/* verify that the <rpcproxy.h> version is high enough to compile this file*/
#ifndef __REDQ_RPCPROXY_H_VERSION__
#define __REQUIRED_RPCPROXY_H_VERSION__ 475
#endif


#include "rpcproxy.h"
#ifndef __RPCPROXY_H_VERSION__
#error this stub requires an updated version of <rpcproxy.h>
#endif // __RPCPROXY_H_VERSION__


#include "PCLExtension.h"

#define TYPE_FORMAT_STRING_SIZE   363                               
#define PROC_FORMAT_STRING_SIZE   2371                              
#define TRANSMIT_AS_TABLE_SIZE    0            
#define WIRE_MARSHAL_TABLE_SIZE   1            

typedef struct _MIDL_TYPE_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ TYPE_FORMAT_STRING_SIZE ];
    } MIDL_TYPE_FORMAT_STRING;

typedef struct _MIDL_PROC_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ PROC_FORMAT_STRING_SIZE ];
    } MIDL_PROC_FORMAT_STRING;


static RPC_SYNTAX_IDENTIFIER  _RpcTransferSyntax = 
{{0x8A885D04,0x1CEB,0x11C9,{0x9F,0xE8,0x08,0x00,0x2B,0x10,0x48,0x60}},{2,0}};


extern const MIDL_TYPE_FORMAT_STRING __MIDL_TypeFormatString;
extern const MIDL_PROC_FORMAT_STRING __MIDL_ProcFormatString;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IPCLArgument_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO IPCLArgument_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IPCLWavefileLock_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO IPCLWavefileLock_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IPCLWavefile_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO IPCLWavefile_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IPCLGraphicSurfaceLock_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO IPCLGraphicSurfaceLock_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IPCLGraphicSurface_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO IPCLGraphicSurface_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IPCLArray_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO IPCLArray_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IPCLStringArray_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO IPCLStringArray_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IPCLIntArray_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO IPCLIntArray_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IPCLDoubleArray_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO IPCLDoubleArray_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IPCLBoolArray_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO IPCLBoolArray_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IPCLString_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO IPCLString_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IPCLInt_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO IPCLInt_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IPCLDouble_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO IPCLDouble_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IPCLBool_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO IPCLBool_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IPCLArguments_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO IPCLArguments_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IPCLObject_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO IPCLObject_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IPCLExtensionObject_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO IPCLExtensionObject_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IPCLMethod_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO IPCLMethod_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IPCLMethodList_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO IPCLMethodList_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IPCLType_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO IPCLType_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IPCLLibrary_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO IPCLLibrary_ProxyInfo;


extern const USER_MARSHAL_ROUTINE_QUADRUPLE UserMarshalRoutines[ WIRE_MARSHAL_TABLE_SIZE ];

#if !defined(__RPC_WIN32__)
#error  Invalid build platform for this stub.
#endif

#if !(TARGET_IS_NT50_OR_LATER)
#error You need a Windows 2000 or later to run this stub because it uses these features:
#error   /robust command line switch.
#error However, your C/C++ compilation flags indicate you intend to run this app on earlier systems.
#error This app will die there with the RPC_X_WRONG_STUB_VERSION error.
#endif


static const MIDL_PROC_FORMAT_STRING __MIDL_ProcFormatString =
    {
        0,
        {

	/* Procedure getName */


	/* Procedure getName */


	/* Procedure getType */

			0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/*  2 */	NdrFcLong( 0x0 ),	/* 0 */
/*  6 */	NdrFcShort( 0x7 ),	/* 7 */
/*  8 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 10 */	NdrFcShort( 0x0 ),	/* 0 */
/* 12 */	NdrFcShort( 0x8 ),	/* 8 */
/* 14 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 16 */	0x8,		/* 8 */
			0x3,		/* Ext Flags:  new corr desc, clt corr check, */
/* 18 */	NdrFcShort( 0x1 ),	/* 1 */
/* 20 */	NdrFcShort( 0x0 ),	/* 0 */
/* 22 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter name */


	/* Parameter name */


	/* Parameter type */

/* 24 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
/* 26 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 28 */	NdrFcShort( 0x20 ),	/* Type Offset=32 */

	/* Return value */


	/* Return value */


	/* Return value */

/* 30 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 32 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 34 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure getRank */

/* 36 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 38 */	NdrFcLong( 0x0 ),	/* 0 */
/* 42 */	NdrFcShort( 0x8 ),	/* 8 */
/* 44 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 46 */	NdrFcShort( 0x0 ),	/* 0 */
/* 48 */	NdrFcShort( 0x24 ),	/* 36 */
/* 50 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 52 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 54 */	NdrFcShort( 0x0 ),	/* 0 */
/* 56 */	NdrFcShort( 0x0 ),	/* 0 */
/* 58 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter rank */

/* 60 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 62 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 64 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 66 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 68 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 70 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure getIsReference */

/* 72 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 74 */	NdrFcLong( 0x0 ),	/* 0 */
/* 78 */	NdrFcShort( 0x9 ),	/* 9 */
/* 80 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 82 */	NdrFcShort( 0x0 ),	/* 0 */
/* 84 */	NdrFcShort( 0x24 ),	/* 36 */
/* 86 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 88 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 90 */	NdrFcShort( 0x0 ),	/* 0 */
/* 92 */	NdrFcShort( 0x0 ),	/* 0 */
/* 94 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter isRef */

/* 96 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 98 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 100 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 102 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 104 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 106 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure getDescription */


	/* Procedure getLastError */


	/* Procedure getLastError */


	/* Procedure getLastError */

/* 108 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 110 */	NdrFcLong( 0x0 ),	/* 0 */
/* 114 */	NdrFcShort( 0xa ),	/* 10 */
/* 116 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 118 */	NdrFcShort( 0x0 ),	/* 0 */
/* 120 */	NdrFcShort( 0x8 ),	/* 8 */
/* 122 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 124 */	0x8,		/* 8 */
			0x3,		/* Ext Flags:  new corr desc, clt corr check, */
/* 126 */	NdrFcShort( 0x1 ),	/* 1 */
/* 128 */	NdrFcShort( 0x0 ),	/* 0 */
/* 130 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter desc */


	/* Parameter message */


	/* Parameter message */


	/* Parameter message */

/* 132 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
/* 134 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 136 */	NdrFcShort( 0x20 ),	/* Type Offset=32 */

	/* Return value */


	/* Return value */


	/* Return value */


	/* Return value */

/* 138 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 140 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 142 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure getDataPtr */


	/* Procedure getDataPtr */

/* 144 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 146 */	NdrFcLong( 0x0 ),	/* 0 */
/* 150 */	NdrFcShort( 0x7 ),	/* 7 */
/* 152 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 154 */	NdrFcShort( 0x0 ),	/* 0 */
/* 156 */	NdrFcShort( 0x35 ),	/* 53 */
/* 158 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 160 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 162 */	NdrFcShort( 0x0 ),	/* 0 */
/* 164 */	NdrFcShort( 0x0 ),	/* 0 */
/* 166 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter dataPtr */


	/* Parameter dataPtr */

/* 168 */	NdrFcShort( 0x2012 ),	/* Flags:  must free, out, srv alloc size=8 */
/* 170 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 172 */	NdrFcShort( 0x2e ),	/* Type Offset=46 */

	/* Return value */


	/* Return value */

/* 174 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 176 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 178 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure getParameters */

/* 180 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 182 */	NdrFcLong( 0x0 ),	/* 0 */
/* 186 */	NdrFcShort( 0x8 ),	/* 8 */
/* 188 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 190 */	NdrFcShort( 0x0 ),	/* 0 */
/* 192 */	NdrFcShort( 0x3c ),	/* 60 */
/* 194 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 196 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 198 */	NdrFcShort( 0x0 ),	/* 0 */
/* 200 */	NdrFcShort( 0x0 ),	/* 0 */
/* 202 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter parameters */

/* 204 */	NdrFcShort( 0x4112 ),	/* Flags:  must free, out, simple ref, srv alloc size=16 */
/* 206 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 208 */	NdrFcShort( 0x3a ),	/* Type Offset=58 */

	/* Return value */

/* 210 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 212 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 214 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure getVersion */


	/* Procedure getParentName */


	/* Procedure getLastError */


	/* Procedure getLastError */


	/* Procedure getLastError */

/* 216 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 218 */	NdrFcLong( 0x0 ),	/* 0 */
/* 222 */	NdrFcShort( 0x9 ),	/* 9 */
/* 224 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 226 */	NdrFcShort( 0x0 ),	/* 0 */
/* 228 */	NdrFcShort( 0x8 ),	/* 8 */
/* 230 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 232 */	0x8,		/* 8 */
			0x3,		/* Ext Flags:  new corr desc, clt corr check, */
/* 234 */	NdrFcShort( 0x1 ),	/* 1 */
/* 236 */	NdrFcShort( 0x0 ),	/* 0 */
/* 238 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter ver */


	/* Parameter parent */


	/* Parameter message */


	/* Parameter message */


	/* Parameter message */

/* 240 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
/* 242 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 244 */	NdrFcShort( 0x20 ),	/* Type Offset=32 */

	/* Return value */


	/* Return value */


	/* Return value */


	/* Return value */


	/* Return value */

/* 246 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 248 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 250 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure getValue */


	/* Procedure getValue */


	/* Procedure getCount */


	/* Procedure getIsNull */


	/* Procedure getIsNull */

/* 252 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 254 */	NdrFcLong( 0x0 ),	/* 0 */
/* 258 */	NdrFcShort( 0xb ),	/* 11 */
/* 260 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 262 */	NdrFcShort( 0x0 ),	/* 0 */
/* 264 */	NdrFcShort( 0x24 ),	/* 36 */
/* 266 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 268 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 270 */	NdrFcShort( 0x0 ),	/* 0 */
/* 272 */	NdrFcShort( 0x0 ),	/* 0 */
/* 274 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter val */


	/* Parameter val */


	/* Parameter count */


	/* Parameter null */


	/* Parameter null */

/* 276 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 278 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 280 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */


	/* Return value */


	/* Return value */


	/* Return value */


	/* Return value */

/* 282 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 284 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 286 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure getParameters */

/* 288 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 290 */	NdrFcLong( 0x0 ),	/* 0 */
/* 294 */	NdrFcShort( 0xc ),	/* 12 */
/* 296 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 298 */	NdrFcShort( 0x0 ),	/* 0 */
/* 300 */	NdrFcShort( 0x3c ),	/* 60 */
/* 302 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 304 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 306 */	NdrFcShort( 0x0 ),	/* 0 */
/* 308 */	NdrFcShort( 0x0 ),	/* 0 */
/* 310 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter parameters */

/* 312 */	NdrFcShort( 0x4112 ),	/* Flags:  must free, out, simple ref, srv alloc size=16 */
/* 314 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 316 */	NdrFcShort( 0x3a ),	/* Type Offset=58 */

	/* Return value */

/* 318 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 320 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 322 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure resize */


	/* Procedure resize */

/* 324 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 326 */	NdrFcLong( 0x0 ),	/* 0 */
/* 330 */	NdrFcShort( 0xd ),	/* 13 */
/* 332 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 334 */	NdrFcShort( 0x10 ),	/* 16 */
/* 336 */	NdrFcShort( 0x8 ),	/* 8 */
/* 338 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x3,		/* 3 */
/* 340 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 342 */	NdrFcShort( 0x0 ),	/* 0 */
/* 344 */	NdrFcShort( 0x0 ),	/* 0 */
/* 346 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter widthPixels */


	/* Parameter frameCount */

/* 348 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 350 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 352 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter heightPixels */


	/* Parameter channelCount */

/* 354 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 356 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 358 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */


	/* Return value */

/* 360 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 362 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 364 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure lock */

/* 366 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 368 */	NdrFcLong( 0x0 ),	/* 0 */
/* 372 */	NdrFcShort( 0xe ),	/* 14 */
/* 374 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 376 */	NdrFcShort( 0x0 ),	/* 0 */
/* 378 */	NdrFcShort( 0x8 ),	/* 8 */
/* 380 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 382 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 384 */	NdrFcShort( 0x0 ),	/* 0 */
/* 386 */	NdrFcShort( 0x0 ),	/* 0 */
/* 388 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter lockObject */

/* 390 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 392 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 394 */	NdrFcShort( 0x44 ),	/* Type Offset=68 */

	/* Return value */

/* 396 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 398 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 400 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure replace */

/* 402 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 404 */	NdrFcLong( 0x0 ),	/* 0 */
/* 408 */	NdrFcShort( 0xf ),	/* 15 */
/* 410 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 412 */	NdrFcShort( 0x39 ),	/* 57 */
/* 414 */	NdrFcShort( 0x8 ),	/* 8 */
/* 416 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x3,		/* 3 */
/* 418 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 420 */	NdrFcShort( 0x0 ),	/* 0 */
/* 422 */	NdrFcShort( 0x0 ),	/* 0 */
/* 424 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter parameters */

/* 426 */	NdrFcShort( 0x8a ),	/* Flags:  must free, in, by val, */
/* 428 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 430 */	NdrFcShort( 0x3a ),	/* Type Offset=58 */

	/* Parameter data */

/* 432 */	NdrFcShort( 0x148 ),	/* Flags:  in, base type, simple ref, */
/* 434 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 436 */	0x1,		/* FC_BYTE */
			0x0,		/* 0 */

	/* Return value */

/* 438 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 440 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 442 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure getParameters */

/* 444 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 446 */	NdrFcLong( 0x0 ),	/* 0 */
/* 450 */	NdrFcShort( 0x8 ),	/* 8 */
/* 452 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 454 */	NdrFcShort( 0x0 ),	/* 0 */
/* 456 */	NdrFcShort( 0x4c ),	/* 76 */
/* 458 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 460 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 462 */	NdrFcShort( 0x0 ),	/* 0 */
/* 464 */	NdrFcShort( 0x0 ),	/* 0 */
/* 466 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter parameters */

/* 468 */	NdrFcShort( 0x8112 ),	/* Flags:  must free, out, simple ref, srv alloc size=32 */
/* 470 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 472 */	NdrFcShort( 0x62 ),	/* Type Offset=98 */

	/* Return value */

/* 474 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 476 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 478 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure getSize */

/* 480 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 482 */	NdrFcLong( 0x0 ),	/* 0 */
/* 486 */	NdrFcShort( 0xc ),	/* 12 */
/* 488 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 490 */	NdrFcShort( 0x0 ),	/* 0 */
/* 492 */	NdrFcShort( 0x40 ),	/* 64 */
/* 494 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x3,		/* 3 */
/* 496 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 498 */	NdrFcShort( 0x0 ),	/* 0 */
/* 500 */	NdrFcShort( 0x0 ),	/* 0 */
/* 502 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter widthPixels */

/* 504 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 506 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 508 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter heightPixels */

/* 510 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 512 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 514 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 516 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 518 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 520 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure lock */

/* 522 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 524 */	NdrFcLong( 0x0 ),	/* 0 */
/* 528 */	NdrFcShort( 0xe ),	/* 14 */
/* 530 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 532 */	NdrFcShort( 0x0 ),	/* 0 */
/* 534 */	NdrFcShort( 0x8 ),	/* 8 */
/* 536 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 538 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 540 */	NdrFcShort( 0x0 ),	/* 0 */
/* 542 */	NdrFcShort( 0x0 ),	/* 0 */
/* 544 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter lockObject */

/* 546 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 548 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 550 */	NdrFcShort( 0x70 ),	/* Type Offset=112 */

	/* Return value */

/* 552 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 554 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 556 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure replace */

/* 558 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 560 */	NdrFcLong( 0x0 ),	/* 0 */
/* 564 */	NdrFcShort( 0xf ),	/* 15 */
/* 566 */	NdrFcShort( 0x2c ),	/* x86 Stack size/offset = 44 */
/* 568 */	NdrFcShort( 0x49 ),	/* 73 */
/* 570 */	NdrFcShort( 0x8 ),	/* 8 */
/* 572 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x3,		/* 3 */
/* 574 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 576 */	NdrFcShort( 0x0 ),	/* 0 */
/* 578 */	NdrFcShort( 0x0 ),	/* 0 */
/* 580 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter parameters */

/* 582 */	NdrFcShort( 0x8a ),	/* Flags:  must free, in, by val, */
/* 584 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 586 */	NdrFcShort( 0x62 ),	/* Type Offset=98 */

	/* Parameter data */

/* 588 */	NdrFcShort( 0x148 ),	/* Flags:  in, base type, simple ref, */
/* 590 */	NdrFcShort( 0x24 ),	/* x86 Stack size/offset = 36 */
/* 592 */	0x1,		/* FC_BYTE */
			0x0,		/* 0 */

	/* Return value */

/* 594 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 596 */	NdrFcShort( 0x28 ),	/* x86 Stack size/offset = 40 */
/* 598 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure convertCustomUnits */

/* 600 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 602 */	NdrFcLong( 0x0 ),	/* 0 */
/* 606 */	NdrFcShort( 0x10 ),	/* 16 */
/* 608 */	NdrFcShort( 0x20 ),	/* x86 Stack size/offset = 32 */
/* 610 */	NdrFcShort( 0x20 ),	/* 32 */
/* 612 */	NdrFcShort( 0x50 ),	/* 80 */
/* 614 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x5,		/* 5 */
/* 616 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 618 */	NdrFcShort( 0x0 ),	/* 0 */
/* 620 */	NdrFcShort( 0x0 ),	/* 0 */
/* 622 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter customWidth */

/* 624 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 626 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 628 */	0xc,		/* FC_DOUBLE */
			0x0,		/* 0 */

	/* Parameter customHeight */

/* 630 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 632 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 634 */	0xc,		/* FC_DOUBLE */
			0x0,		/* 0 */

	/* Parameter pixelWidth */

/* 636 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 638 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 640 */	0xc,		/* FC_DOUBLE */
			0x0,		/* 0 */

	/* Parameter pixelHeight */

/* 642 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 644 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 646 */	0xc,		/* FC_DOUBLE */
			0x0,		/* 0 */

	/* Return value */

/* 648 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 650 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 652 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure setValue */


	/* Procedure setValue */


	/* Procedure resize */

/* 654 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 656 */	NdrFcLong( 0x0 ),	/* 0 */
/* 660 */	NdrFcShort( 0xc ),	/* 12 */
/* 662 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 664 */	NdrFcShort( 0x8 ),	/* 8 */
/* 666 */	NdrFcShort( 0x8 ),	/* 8 */
/* 668 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 670 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 672 */	NdrFcShort( 0x0 ),	/* 0 */
/* 674 */	NdrFcShort( 0x0 ),	/* 0 */
/* 676 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter val */


	/* Parameter val */


	/* Parameter count */

/* 678 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 680 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 682 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */


	/* Return value */


	/* Return value */

/* 684 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 686 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 688 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure getSubArray */

/* 690 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 692 */	NdrFcLong( 0x0 ),	/* 0 */
/* 696 */	NdrFcShort( 0xd ),	/* 13 */
/* 698 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 700 */	NdrFcShort( 0x8 ),	/* 8 */
/* 702 */	NdrFcShort( 0x8 ),	/* 8 */
/* 704 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x3,		/* 3 */
/* 706 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 708 */	NdrFcShort( 0x0 ),	/* 0 */
/* 710 */	NdrFcShort( 0x0 ),	/* 0 */
/* 712 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter index */

/* 714 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 716 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 718 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter subArray */

/* 720 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 722 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 724 */	NdrFcShort( 0x8a ),	/* Type Offset=138 */

	/* Return value */

/* 726 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 728 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 730 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure setValue */

/* 732 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 734 */	NdrFcLong( 0x0 ),	/* 0 */
/* 738 */	NdrFcShort( 0xe ),	/* 14 */
/* 740 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 742 */	NdrFcShort( 0x8 ),	/* 8 */
/* 744 */	NdrFcShort( 0x8 ),	/* 8 */
/* 746 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 748 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 750 */	NdrFcShort( 0x0 ),	/* 0 */
/* 752 */	NdrFcShort( 0x1 ),	/* 1 */
/* 754 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter index */

/* 756 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 758 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 760 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter value */

/* 762 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 764 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 766 */	NdrFcShort( 0xa4 ),	/* Type Offset=164 */

	/* Return value */

/* 768 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 770 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 772 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure getValue */

/* 774 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 776 */	NdrFcLong( 0x0 ),	/* 0 */
/* 780 */	NdrFcShort( 0xf ),	/* 15 */
/* 782 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 784 */	NdrFcShort( 0x8 ),	/* 8 */
/* 786 */	NdrFcShort( 0x8 ),	/* 8 */
/* 788 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x3,		/* 3 */
/* 790 */	0x8,		/* 8 */
			0x3,		/* Ext Flags:  new corr desc, clt corr check, */
/* 792 */	NdrFcShort( 0x1 ),	/* 1 */
/* 794 */	NdrFcShort( 0x0 ),	/* 0 */
/* 796 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter index */

/* 798 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 800 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 802 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter value */

/* 804 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
/* 806 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 808 */	NdrFcShort( 0x20 ),	/* Type Offset=32 */

	/* Return value */

/* 810 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 812 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 814 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure add */

/* 816 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 818 */	NdrFcLong( 0x0 ),	/* 0 */
/* 822 */	NdrFcShort( 0x10 ),	/* 16 */
/* 824 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 826 */	NdrFcShort( 0x0 ),	/* 0 */
/* 828 */	NdrFcShort( 0x8 ),	/* 8 */
/* 830 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 832 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 834 */	NdrFcShort( 0x0 ),	/* 0 */
/* 836 */	NdrFcShort( 0x1 ),	/* 1 */
/* 838 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter value */

/* 840 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 842 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 844 */	NdrFcShort( 0xa4 ),	/* Type Offset=164 */

	/* Return value */

/* 846 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 848 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 850 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure getDataPtr */

/* 852 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 854 */	NdrFcLong( 0x0 ),	/* 0 */
/* 858 */	NdrFcShort( 0xe ),	/* 14 */
/* 860 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 862 */	NdrFcShort( 0x0 ),	/* 0 */
/* 864 */	NdrFcShort( 0x38 ),	/* 56 */
/* 866 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 868 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 870 */	NdrFcShort( 0x0 ),	/* 0 */
/* 872 */	NdrFcShort( 0x0 ),	/* 0 */
/* 874 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter dataPtr */

/* 876 */	NdrFcShort( 0x2012 ),	/* Flags:  must free, out, srv alloc size=8 */
/* 878 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 880 */	NdrFcShort( 0xae ),	/* Type Offset=174 */

	/* Return value */

/* 882 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 884 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 886 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure setValue */

/* 888 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 890 */	NdrFcLong( 0x0 ),	/* 0 */
/* 894 */	NdrFcShort( 0xf ),	/* 15 */
/* 896 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 898 */	NdrFcShort( 0x10 ),	/* 16 */
/* 900 */	NdrFcShort( 0x8 ),	/* 8 */
/* 902 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x3,		/* 3 */
/* 904 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 906 */	NdrFcShort( 0x0 ),	/* 0 */
/* 908 */	NdrFcShort( 0x0 ),	/* 0 */
/* 910 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter index */

/* 912 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 914 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 916 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter value */

/* 918 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 920 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 922 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 924 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 926 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 928 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure getValue */

/* 930 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 932 */	NdrFcLong( 0x0 ),	/* 0 */
/* 936 */	NdrFcShort( 0x10 ),	/* 16 */
/* 938 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 940 */	NdrFcShort( 0x8 ),	/* 8 */
/* 942 */	NdrFcShort( 0x24 ),	/* 36 */
/* 944 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x3,		/* 3 */
/* 946 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 948 */	NdrFcShort( 0x0 ),	/* 0 */
/* 950 */	NdrFcShort( 0x0 ),	/* 0 */
/* 952 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter index */

/* 954 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 956 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 958 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter value */

/* 960 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 962 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 964 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 966 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 968 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 970 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure add */

/* 972 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 974 */	NdrFcLong( 0x0 ),	/* 0 */
/* 978 */	NdrFcShort( 0x11 ),	/* 17 */
/* 980 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 982 */	NdrFcShort( 0x8 ),	/* 8 */
/* 984 */	NdrFcShort( 0x8 ),	/* 8 */
/* 986 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 988 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 990 */	NdrFcShort( 0x0 ),	/* 0 */
/* 992 */	NdrFcShort( 0x0 ),	/* 0 */
/* 994 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter value */

/* 996 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 998 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1000 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 1002 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1004 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1006 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure getDataPtr */

/* 1008 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1010 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1014 */	NdrFcShort( 0xe ),	/* 14 */
/* 1016 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1018 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1020 */	NdrFcShort( 0x40 ),	/* 64 */
/* 1022 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 1024 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1026 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1028 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1030 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter dataPtr */

/* 1032 */	NdrFcShort( 0x2012 ),	/* Flags:  must free, out, srv alloc size=8 */
/* 1034 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1036 */	NdrFcShort( 0xb6 ),	/* Type Offset=182 */

	/* Return value */

/* 1038 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1040 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1042 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure setValue */

/* 1044 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1046 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1050 */	NdrFcShort( 0xf ),	/* 15 */
/* 1052 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 1054 */	NdrFcShort( 0x18 ),	/* 24 */
/* 1056 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1058 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x3,		/* 3 */
/* 1060 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1062 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1064 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1066 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter index */

/* 1068 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1070 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1072 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter value */

/* 1074 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1076 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1078 */	0xc,		/* FC_DOUBLE */
			0x0,		/* 0 */

	/* Return value */

/* 1080 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1082 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 1084 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure getValue */

/* 1086 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1088 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1092 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1094 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 1096 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1098 */	NdrFcShort( 0x2c ),	/* 44 */
/* 1100 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x3,		/* 3 */
/* 1102 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1104 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1106 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1108 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter index */

/* 1110 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1112 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1114 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter value */

/* 1116 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 1118 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1120 */	0xc,		/* FC_DOUBLE */
			0x0,		/* 0 */

	/* Return value */

/* 1122 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1124 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1126 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure add */

/* 1128 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1130 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1134 */	NdrFcShort( 0x11 ),	/* 17 */
/* 1136 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 1138 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1140 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1142 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 1144 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1146 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1148 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1150 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter value */

/* 1152 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1154 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1156 */	0xc,		/* FC_DOUBLE */
			0x0,		/* 0 */

	/* Return value */

/* 1158 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1160 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1162 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure getDataPtr */

/* 1164 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1166 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1170 */	NdrFcShort( 0xe ),	/* 14 */
/* 1172 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1174 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1176 */	NdrFcShort( 0x35 ),	/* 53 */
/* 1178 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 1180 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1182 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1184 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1186 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter dataPtr */

/* 1188 */	NdrFcShort( 0x2012 ),	/* Flags:  must free, out, srv alloc size=8 */
/* 1190 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1192 */	NdrFcShort( 0x2e ),	/* Type Offset=46 */

	/* Return value */

/* 1194 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1196 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1198 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure setValue */

/* 1200 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1202 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1206 */	NdrFcShort( 0xf ),	/* 15 */
/* 1208 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 1210 */	NdrFcShort( 0xd ),	/* 13 */
/* 1212 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1214 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x3,		/* 3 */
/* 1216 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1218 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1220 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1222 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter index */

/* 1224 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1226 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1228 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter value */

/* 1230 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1232 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1234 */	0x1,		/* FC_BYTE */
			0x0,		/* 0 */

	/* Return value */

/* 1236 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1238 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1240 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure getValue */

/* 1242 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1244 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1248 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1250 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 1252 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1254 */	NdrFcShort( 0x21 ),	/* 33 */
/* 1256 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x3,		/* 3 */
/* 1258 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1260 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1262 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1264 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter index */

/* 1266 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1268 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1270 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter value */

/* 1272 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 1274 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1276 */	0x1,		/* FC_BYTE */
			0x0,		/* 0 */

	/* Return value */

/* 1278 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1280 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1282 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure add */

/* 1284 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1286 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1290 */	NdrFcShort( 0x11 ),	/* 17 */
/* 1292 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1294 */	NdrFcShort( 0x5 ),	/* 5 */
/* 1296 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1298 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 1300 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1302 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1304 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1306 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter value */

/* 1308 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1310 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1312 */	0x1,		/* FC_BYTE */
			0x0,		/* 0 */

	/* Return value */

/* 1314 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1316 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1318 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure getLastError */


	/* Procedure getValue */

/* 1320 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1322 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1326 */	NdrFcShort( 0xb ),	/* 11 */
/* 1328 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1330 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1332 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1334 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 1336 */	0x8,		/* 8 */
			0x3,		/* Ext Flags:  new corr desc, clt corr check, */
/* 1338 */	NdrFcShort( 0x1 ),	/* 1 */
/* 1340 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1342 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter message */


	/* Parameter val */

/* 1344 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
/* 1346 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1348 */	NdrFcShort( 0x20 ),	/* Type Offset=32 */

	/* Return value */


	/* Return value */

/* 1350 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1352 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1354 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure setValue */

/* 1356 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1358 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1362 */	NdrFcShort( 0xc ),	/* 12 */
/* 1364 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1366 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1368 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1370 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 1372 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 1374 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1376 */	NdrFcShort( 0x1 ),	/* 1 */
/* 1378 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter val */

/* 1380 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 1382 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1384 */	NdrFcShort( 0xa4 ),	/* Type Offset=164 */

	/* Return value */

/* 1386 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1388 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1390 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure getPresentationMinimumVersion */


	/* Procedure getValue */

/* 1392 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1394 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1398 */	NdrFcShort( 0xb ),	/* 11 */
/* 1400 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1402 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1404 */	NdrFcShort( 0x2c ),	/* 44 */
/* 1406 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 1408 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1410 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1412 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1414 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter ver */


	/* Parameter val */

/* 1416 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 1418 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1420 */	0xc,		/* FC_DOUBLE */
			0x0,		/* 0 */

	/* Return value */


	/* Return value */

/* 1422 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1424 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1426 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure setValue */

/* 1428 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1430 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1434 */	NdrFcShort( 0xc ),	/* 12 */
/* 1436 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 1438 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1440 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1442 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 1444 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1446 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1448 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1450 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter val */

/* 1452 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1454 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1456 */	0xc,		/* FC_DOUBLE */
			0x0,		/* 0 */

	/* Return value */

/* 1458 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1460 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1462 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure getCount */

/* 1464 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1466 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1470 */	NdrFcShort( 0x7 ),	/* 7 */
/* 1472 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1474 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1476 */	NdrFcShort( 0x24 ),	/* 36 */
/* 1478 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 1480 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1482 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1484 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1486 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter count */

/* 1488 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 1490 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1492 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 1494 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1496 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1498 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure getArgument */

/* 1500 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1502 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1506 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1508 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 1510 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1512 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1514 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x3,		/* 3 */
/* 1516 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1518 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1520 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1522 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter index */

/* 1524 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1526 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1528 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter argument */

/* 1530 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 1532 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1534 */	NdrFcShort( 0xc2 ),	/* Type Offset=194 */

	/* Return value */

/* 1536 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1538 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1540 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure getReturnValue */

/* 1542 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1544 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1548 */	NdrFcShort( 0x9 ),	/* 9 */
/* 1550 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1552 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1554 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1556 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 1558 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1560 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1562 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1564 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter value */

/* 1566 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 1568 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1570 */	NdrFcShort( 0xc2 ),	/* Type Offset=194 */

	/* Return value */

/* 1572 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1574 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1576 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure callMethod */

/* 1578 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1580 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1584 */	NdrFcShort( 0x7 ),	/* 7 */
/* 1586 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 1588 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1590 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1592 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 1594 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1596 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1598 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1600 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter methodIndex */

/* 1602 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1604 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1606 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter arguments */

/* 1608 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 1610 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1612 */	NdrFcShort( 0xd8 ),	/* Type Offset=216 */

	/* Return value */

/* 1614 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1616 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1618 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure getType */

/* 1620 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1622 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1626 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1628 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1630 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1632 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1634 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 1636 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1638 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1640 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1642 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter type */

/* 1644 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 1646 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1648 */	NdrFcShort( 0xea ),	/* Type Offset=234 */

	/* Return value */

/* 1650 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1652 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1654 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure getValue */

/* 1656 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1658 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1662 */	NdrFcShort( 0xb ),	/* 11 */
/* 1664 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1666 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1668 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1670 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 1672 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1674 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1676 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1678 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter val */

/* 1680 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 1682 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1684 */	NdrFcShort( 0x100 ),	/* Type Offset=256 */

	/* Return value */

/* 1686 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1688 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1690 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure setValue */

/* 1692 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1694 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1698 */	NdrFcShort( 0xc ),	/* 12 */
/* 1700 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1702 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1704 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1706 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 1708 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1710 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1712 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1714 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter val */

/* 1716 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 1718 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1720 */	NdrFcShort( 0x104 ),	/* Type Offset=260 */

	/* Return value */

/* 1722 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1724 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1726 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure setName */

/* 1728 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1730 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1734 */	NdrFcShort( 0x7 ),	/* 7 */
/* 1736 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1738 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1740 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1742 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 1744 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 1746 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1748 */	NdrFcShort( 0x1 ),	/* 1 */
/* 1750 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter name */

/* 1752 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 1754 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1756 */	NdrFcShort( 0xa4 ),	/* Type Offset=164 */

	/* Return value */

/* 1758 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1760 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1762 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure addArgument */

/* 1764 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1766 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1770 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1772 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 1774 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1776 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1778 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x6,		/* 6 */
/* 1780 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 1782 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1784 */	NdrFcShort( 0x3 ),	/* 3 */
/* 1786 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter name */

/* 1788 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 1790 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1792 */	NdrFcShort( 0xa4 ),	/* Type Offset=164 */

	/* Parameter type */

/* 1794 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 1796 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1798 */	NdrFcShort( 0xa4 ),	/* Type Offset=164 */

	/* Parameter rank */

/* 1800 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1802 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1804 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter isReference */

/* 1806 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1808 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 1810 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter description */

/* 1812 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 1814 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 1816 */	NdrFcShort( 0xa4 ),	/* Type Offset=164 */

	/* Return value */

/* 1818 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1820 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 1822 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure setReturnValue */

/* 1824 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1826 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1830 */	NdrFcShort( 0x9 ),	/* 9 */
/* 1832 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 1834 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1836 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1838 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x4,		/* 4 */
/* 1840 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 1842 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1844 */	NdrFcShort( 0x2 ),	/* 2 */
/* 1846 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter type */

/* 1848 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 1850 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1852 */	NdrFcShort( 0xa4 ),	/* Type Offset=164 */

	/* Parameter rank */

/* 1854 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1856 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1858 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter description */

/* 1860 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 1862 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1864 */	NdrFcShort( 0xa4 ),	/* Type Offset=164 */

	/* Return value */

/* 1866 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1868 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 1870 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure setDescription */

/* 1872 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1874 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1878 */	NdrFcShort( 0xa ),	/* 10 */
/* 1880 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1882 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1884 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1886 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 1888 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 1890 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1892 */	NdrFcShort( 0x1 ),	/* 1 */
/* 1894 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter desc */

/* 1896 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 1898 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1900 */	NdrFcShort( 0xa4 ),	/* Type Offset=164 */

	/* Return value */

/* 1902 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1904 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1906 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure addMethod */

/* 1908 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1910 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1914 */	NdrFcShort( 0x7 ),	/* 7 */
/* 1916 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1918 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1920 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1922 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 1924 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1926 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1928 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1930 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter method */

/* 1932 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 1934 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1936 */	NdrFcShort( 0x116 ),	/* Type Offset=278 */

	/* Return value */

/* 1938 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1940 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1942 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure addNew */

/* 1944 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1946 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1950 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1952 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1954 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1956 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1958 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 1960 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1962 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1964 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1966 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter method */

/* 1968 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 1970 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1972 */	NdrFcShort( 0x116 ),	/* Type Offset=278 */

	/* Return value */

/* 1974 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1976 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1978 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure addPredefinedVariable */

/* 1980 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1982 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1986 */	NdrFcShort( 0x9 ),	/* 9 */
/* 1988 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 1990 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1992 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1994 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x4,		/* 4 */
/* 1996 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 1998 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2000 */	NdrFcShort( 0x3 ),	/* 3 */
/* 2002 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter type */

/* 2004 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 2006 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2008 */	NdrFcShort( 0xa4 ),	/* Type Offset=164 */

	/* Parameter name */

/* 2010 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 2012 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2014 */	NdrFcShort( 0xa4 ),	/* Type Offset=164 */

	/* Parameter value */

/* 2016 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 2018 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2020 */	NdrFcShort( 0xa4 ),	/* Type Offset=164 */

	/* Return value */

/* 2022 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2024 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 2026 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure getAuthor */


	/* Procedure getDescription */

/* 2028 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2030 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2034 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2036 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2038 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2040 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2042 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 2044 */	0x8,		/* 8 */
			0x3,		/* Ext Flags:  new corr desc, clt corr check, */
/* 2046 */	NdrFcShort( 0x1 ),	/* 1 */
/* 2048 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2050 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter author */


	/* Parameter desc */

/* 2052 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
/* 2054 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2056 */	NdrFcShort( 0x20 ),	/* Type Offset=32 */

	/* Return value */


	/* Return value */

/* 2058 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2060 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2062 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure getMethods */

/* 2064 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2066 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2070 */	NdrFcShort( 0xa ),	/* 10 */
/* 2072 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2074 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2076 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2078 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 2080 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2082 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2084 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2086 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter list */

/* 2088 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 2090 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2092 */	NdrFcShort( 0x12c ),	/* Type Offset=300 */

	/* Return value */

/* 2094 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2096 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2098 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure createObject */

/* 2100 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2102 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2106 */	NdrFcShort( 0xb ),	/* 11 */
/* 2108 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 2110 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2112 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2114 */	0x47,		/* Oi2 Flags:  srv must size, clt must size, has return, has ext, */
			0x4,		/* 4 */
/* 2116 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2118 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2120 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2122 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter newIndex */

/* 2124 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2126 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2128 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter arguments */

/* 2130 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 2132 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2134 */	NdrFcShort( 0xd8 ),	/* Type Offset=216 */

	/* Parameter object */

/* 2136 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 2138 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2140 */	NdrFcShort( 0x100 ),	/* Type Offset=256 */

	/* Return value */

/* 2142 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2144 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 2146 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure getLibrary */

/* 2148 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2150 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2154 */	NdrFcShort( 0xc ),	/* 12 */
/* 2156 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2158 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2160 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2162 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 2164 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2166 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2168 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2170 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter lib */

/* 2172 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 2174 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2176 */	NdrFcShort( 0x13e ),	/* Type Offset=318 */

	/* Return value */

/* 2178 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2180 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2182 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure getLastError */

/* 2184 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2186 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2190 */	NdrFcShort( 0xd ),	/* 13 */
/* 2192 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2194 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2196 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2198 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 2200 */	0x8,		/* 8 */
			0x3,		/* Ext Flags:  new corr desc, clt corr check, */
/* 2202 */	NdrFcShort( 0x1 ),	/* 1 */
/* 2204 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2206 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter message */

/* 2208 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
/* 2210 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2212 */	NdrFcShort( 0x20 ),	/* Type Offset=32 */

	/* Return value */

/* 2214 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2216 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2218 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure getNamespaceName */

/* 2220 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2222 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2226 */	NdrFcShort( 0xc ),	/* 12 */
/* 2228 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2230 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2232 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2234 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 2236 */	0x8,		/* 8 */
			0x3,		/* Ext Flags:  new corr desc, clt corr check, */
/* 2238 */	NdrFcShort( 0x1 ),	/* 1 */
/* 2240 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2242 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter ns */

/* 2244 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
/* 2246 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2248 */	NdrFcShort( 0x20 ),	/* Type Offset=32 */

	/* Return value */

/* 2250 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2252 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2254 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure getTypeCount */

/* 2256 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2258 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2262 */	NdrFcShort( 0xd ),	/* 13 */
/* 2264 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2266 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2268 */	NdrFcShort( 0x24 ),	/* 36 */
/* 2270 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 2272 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2274 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2276 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2278 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter count */

/* 2280 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 2282 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2284 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 2286 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2288 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2290 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure getType */

/* 2292 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2294 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2298 */	NdrFcShort( 0xe ),	/* 14 */
/* 2300 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 2302 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2304 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2306 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x3,		/* 3 */
/* 2308 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2310 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2312 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2314 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter index */

/* 2316 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2318 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2320 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter type */

/* 2322 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 2324 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2326 */	NdrFcShort( 0x154 ),	/* Type Offset=340 */

	/* Return value */

/* 2328 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2330 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2332 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure getLastError */

/* 2334 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2336 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2340 */	NdrFcShort( 0xf ),	/* 15 */
/* 2342 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2344 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2346 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2348 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 2350 */	0x8,		/* 8 */
			0x3,		/* Ext Flags:  new corr desc, clt corr check, */
/* 2352 */	NdrFcShort( 0x1 ),	/* 1 */
/* 2354 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2356 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter message */

/* 2358 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
/* 2360 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2362 */	NdrFcShort( 0x20 ),	/* Type Offset=32 */

	/* Return value */

/* 2364 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2366 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2368 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

			0x0
        }
    };

static const MIDL_TYPE_FORMAT_STRING __MIDL_TypeFormatString =
    {
        0,
        {
			NdrFcShort( 0x0 ),	/* 0 */
/*  2 */	
			0x11, 0x4,	/* FC_RP [alloced_on_stack] */
/*  4 */	NdrFcShort( 0x1c ),	/* Offset= 28 (32) */
/*  6 */	
			0x13, 0x0,	/* FC_OP */
/*  8 */	NdrFcShort( 0xe ),	/* Offset= 14 (22) */
/* 10 */	
			0x1b,		/* FC_CARRAY */
			0x1,		/* 1 */
/* 12 */	NdrFcShort( 0x2 ),	/* 2 */
/* 14 */	0x9,		/* Corr desc: FC_ULONG */
			0x0,		/*  */
/* 16 */	NdrFcShort( 0xfffc ),	/* -4 */
/* 18 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 20 */	0x6,		/* FC_SHORT */
			0x5b,		/* FC_END */
/* 22 */	
			0x17,		/* FC_CSTRUCT */
			0x3,		/* 3 */
/* 24 */	NdrFcShort( 0x8 ),	/* 8 */
/* 26 */	NdrFcShort( 0xfff0 ),	/* Offset= -16 (10) */
/* 28 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 30 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 32 */	0xb4,		/* FC_USER_MARSHAL */
			0x83,		/* 131 */
/* 34 */	NdrFcShort( 0x0 ),	/* 0 */
/* 36 */	NdrFcShort( 0x4 ),	/* 4 */
/* 38 */	NdrFcShort( 0x0 ),	/* 0 */
/* 40 */	NdrFcShort( 0xffde ),	/* Offset= -34 (6) */
/* 42 */	
			0x11, 0xc,	/* FC_RP [alloced_on_stack] [simple_pointer] */
/* 44 */	0x8,		/* FC_LONG */
			0x5c,		/* FC_PAD */
/* 46 */	
			0x11, 0x14,	/* FC_RP [alloced_on_stack] [pointer_deref] */
/* 48 */	NdrFcShort( 0x2 ),	/* Offset= 2 (50) */
/* 50 */	
			0x13, 0x8,	/* FC_OP [simple_pointer] */
/* 52 */	0x1,		/* FC_BYTE */
			0x5c,		/* FC_PAD */
/* 54 */	
			0x11, 0x4,	/* FC_RP [alloced_on_stack] */
/* 56 */	NdrFcShort( 0x2 ),	/* Offset= 2 (58) */
/* 58 */	
			0x15,		/* FC_STRUCT */
			0x3,		/* 3 */
/* 60 */	NdrFcShort( 0x10 ),	/* 16 */
/* 62 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 64 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 66 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 68 */	
			0x11, 0x10,	/* FC_RP [pointer_deref] */
/* 70 */	NdrFcShort( 0x2 ),	/* Offset= 2 (72) */
/* 72 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 74 */	NdrFcLong( 0xe0934aaf ),	/* -527218001 */
/* 78 */	NdrFcShort( 0x8832 ),	/* -30670 */
/* 80 */	NdrFcShort( 0x44cf ),	/* 17615 */
/* 82 */	0xa4,		/* 164 */
			0x90,		/* 144 */
/* 84 */	0x6,		/* 6 */
			0xf9,		/* 249 */
/* 86 */	0x17,		/* 23 */
			0x2f,		/* 47 */
/* 88 */	0xc7,		/* 199 */
			0xbb,		/* 187 */
/* 90 */	
			0x11, 0x8,	/* FC_RP [simple_pointer] */
/* 92 */	0x1,		/* FC_BYTE */
			0x5c,		/* FC_PAD */
/* 94 */	
			0x11, 0x4,	/* FC_RP [alloced_on_stack] */
/* 96 */	NdrFcShort( 0x2 ),	/* Offset= 2 (98) */
/* 98 */	
			0x15,		/* FC_STRUCT */
			0x3,		/* 3 */
/* 100 */	NdrFcShort( 0x20 ),	/* 32 */
/* 102 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 104 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 106 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 108 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 110 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 112 */	
			0x11, 0x10,	/* FC_RP [pointer_deref] */
/* 114 */	NdrFcShort( 0x2 ),	/* Offset= 2 (116) */
/* 116 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 118 */	NdrFcLong( 0x44e35511 ),	/* 1155749137 */
/* 122 */	NdrFcShort( 0xbc78 ),	/* -17288 */
/* 124 */	NdrFcShort( 0x40a5 ),	/* 16549 */
/* 126 */	0xb7,		/* 183 */
			0x13,		/* 19 */
/* 128 */	0xe,		/* 14 */
			0x90,		/* 144 */
/* 130 */	0x77,		/* 119 */
			0x93,		/* 147 */
/* 132 */	0x3e,		/* 62 */
			0x8d,		/* 141 */
/* 134 */	
			0x11, 0xc,	/* FC_RP [alloced_on_stack] [simple_pointer] */
/* 136 */	0xc,		/* FC_DOUBLE */
			0x5c,		/* FC_PAD */
/* 138 */	
			0x11, 0x10,	/* FC_RP [pointer_deref] */
/* 140 */	NdrFcShort( 0x2 ),	/* Offset= 2 (142) */
/* 142 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 144 */	NdrFcLong( 0xdd64d836 ),	/* -580593610 */
/* 148 */	NdrFcShort( 0xf3df ),	/* -3105 */
/* 150 */	NdrFcShort( 0x4097 ),	/* 16535 */
/* 152 */	0xa1,		/* 161 */
			0x2a,		/* 42 */
/* 154 */	0x45,		/* 69 */
			0xef,		/* 239 */
/* 156 */	0x44,		/* 68 */
			0x1b,		/* 27 */
/* 158 */	0x2f,		/* 47 */
			0xd0,		/* 208 */
/* 160 */	
			0x12, 0x0,	/* FC_UP */
/* 162 */	NdrFcShort( 0xff74 ),	/* Offset= -140 (22) */
/* 164 */	0xb4,		/* FC_USER_MARSHAL */
			0x83,		/* 131 */
/* 166 */	NdrFcShort( 0x0 ),	/* 0 */
/* 168 */	NdrFcShort( 0x4 ),	/* 4 */
/* 170 */	NdrFcShort( 0x0 ),	/* 0 */
/* 172 */	NdrFcShort( 0xfff4 ),	/* Offset= -12 (160) */
/* 174 */	
			0x11, 0x14,	/* FC_RP [alloced_on_stack] [pointer_deref] */
/* 176 */	NdrFcShort( 0x2 ),	/* Offset= 2 (178) */
/* 178 */	
			0x13, 0x8,	/* FC_OP [simple_pointer] */
/* 180 */	0x8,		/* FC_LONG */
			0x5c,		/* FC_PAD */
/* 182 */	
			0x11, 0x14,	/* FC_RP [alloced_on_stack] [pointer_deref] */
/* 184 */	NdrFcShort( 0x2 ),	/* Offset= 2 (186) */
/* 186 */	
			0x13, 0x8,	/* FC_OP [simple_pointer] */
/* 188 */	0xc,		/* FC_DOUBLE */
			0x5c,		/* FC_PAD */
/* 190 */	
			0x11, 0xc,	/* FC_RP [alloced_on_stack] [simple_pointer] */
/* 192 */	0x1,		/* FC_BYTE */
			0x5c,		/* FC_PAD */
/* 194 */	
			0x11, 0x10,	/* FC_RP [pointer_deref] */
/* 196 */	NdrFcShort( 0x2 ),	/* Offset= 2 (198) */
/* 198 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 200 */	NdrFcLong( 0xd43bb8bb ),	/* -734283589 */
/* 204 */	NdrFcShort( 0x2ba3 ),	/* 11171 */
/* 206 */	NdrFcShort( 0x492e ),	/* 18734 */
/* 208 */	0xb3,		/* 179 */
			0xab,		/* 171 */
/* 210 */	0x15,		/* 21 */
			0x0,		/* 0 */
/* 212 */	0x71,		/* 113 */
			0xb8,		/* 184 */
/* 214 */	0x79,		/* 121 */
			0xa9,		/* 169 */
/* 216 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 218 */	NdrFcLong( 0xb8d5ba56 ),	/* -1193952682 */
/* 222 */	NdrFcShort( 0x1d5 ),	/* 469 */
/* 224 */	NdrFcShort( 0x4237 ),	/* 16951 */
/* 226 */	0x92,		/* 146 */
			0xa1,		/* 161 */
/* 228 */	0x73,		/* 115 */
			0xc9,		/* 201 */
/* 230 */	0x72,		/* 114 */
			0xc8,		/* 200 */
/* 232 */	0x47,		/* 71 */
			0xb,		/* 11 */
/* 234 */	
			0x11, 0x10,	/* FC_RP [pointer_deref] */
/* 236 */	NdrFcShort( 0x2 ),	/* Offset= 2 (238) */
/* 238 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 240 */	NdrFcLong( 0x12349daf ),	/* 305438127 */
/* 244 */	NdrFcShort( 0xfd20 ),	/* -736 */
/* 246 */	NdrFcShort( 0x4ba6 ),	/* 19366 */
/* 248 */	0xb5,		/* 181 */
			0x0,		/* 0 */
/* 250 */	0xf,		/* 15 */
			0xc8,		/* 200 */
/* 252 */	0xe0,		/* 224 */
			0xe4,		/* 228 */
/* 254 */	0x9a,		/* 154 */
			0xb,		/* 11 */
/* 256 */	
			0x11, 0x10,	/* FC_RP [pointer_deref] */
/* 258 */	NdrFcShort( 0x2 ),	/* Offset= 2 (260) */
/* 260 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 262 */	NdrFcLong( 0x83ce0178 ),	/* -2083651208 */
/* 266 */	NdrFcShort( 0xfae2 ),	/* -1310 */
/* 268 */	NdrFcShort( 0x4734 ),	/* 18228 */
/* 270 */	0x8a,		/* 138 */
			0x90,		/* 144 */
/* 272 */	0x31,		/* 49 */
			0x38,		/* 56 */
/* 274 */	0x95,		/* 149 */
			0x43,		/* 67 */
/* 276 */	0xe4,		/* 228 */
			0xae,		/* 174 */
/* 278 */	
			0x11, 0x10,	/* FC_RP [pointer_deref] */
/* 280 */	NdrFcShort( 0x2 ),	/* Offset= 2 (282) */
/* 282 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 284 */	NdrFcLong( 0xb74b2585 ),	/* -1219811963 */
/* 288 */	NdrFcShort( 0x4c45 ),	/* 19525 */
/* 290 */	NdrFcShort( 0x4747 ),	/* 18247 */
/* 292 */	0xa2,		/* 162 */
			0x97,		/* 151 */
/* 294 */	0x9e,		/* 158 */
			0xd8,		/* 216 */
/* 296 */	0x41,		/* 65 */
			0xf4,		/* 244 */
/* 298 */	0x73,		/* 115 */
			0x1d,		/* 29 */
/* 300 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 302 */	NdrFcLong( 0x721705e4 ),	/* 1914111460 */
/* 306 */	NdrFcShort( 0x275f ),	/* 10079 */
/* 308 */	NdrFcShort( 0x4aed ),	/* 19181 */
/* 310 */	0xbf,		/* 191 */
			0xa9,		/* 169 */
/* 312 */	0x69,		/* 105 */
			0x57,		/* 87 */
/* 314 */	0xe4,		/* 228 */
			0xb,		/* 11 */
/* 316 */	0xde,		/* 222 */
			0x4f,		/* 79 */
/* 318 */	
			0x11, 0x10,	/* FC_RP [pointer_deref] */
/* 320 */	NdrFcShort( 0x2 ),	/* Offset= 2 (322) */
/* 322 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 324 */	NdrFcLong( 0x5483302b ),	/* 1417883691 */
/* 328 */	NdrFcShort( 0x931f ),	/* -27873 */
/* 330 */	NdrFcShort( 0x4506 ),	/* 17670 */
/* 332 */	0xa9,		/* 169 */
			0xd0,		/* 208 */
/* 334 */	0x30,		/* 48 */
			0xf5,		/* 245 */
/* 336 */	0xc2,		/* 194 */
			0x23,		/* 35 */
/* 338 */	0x48,		/* 72 */
			0xcb,		/* 203 */
/* 340 */	
			0x11, 0x10,	/* FC_RP [pointer_deref] */
/* 342 */	NdrFcShort( 0x2 ),	/* Offset= 2 (344) */
/* 344 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 346 */	NdrFcLong( 0x12349daf ),	/* 305438127 */
/* 350 */	NdrFcShort( 0xfd20 ),	/* -736 */
/* 352 */	NdrFcShort( 0x4ba6 ),	/* 19366 */
/* 354 */	0xb5,		/* 181 */
			0x0,		/* 0 */
/* 356 */	0xf,		/* 15 */
			0xc8,		/* 200 */
/* 358 */	0xe0,		/* 224 */
			0xe4,		/* 228 */
/* 360 */	0x9a,		/* 154 */
			0xb,		/* 11 */

			0x0
        }
    };

static const USER_MARSHAL_ROUTINE_QUADRUPLE UserMarshalRoutines[ WIRE_MARSHAL_TABLE_SIZE ] = 
        {
            
            {
            BSTR_UserSize
            ,BSTR_UserMarshal
            ,BSTR_UserUnmarshal
            ,BSTR_UserFree
            }

        };



/* Object interface: IUnknown, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}} */


/* Object interface: IDispatch, ver. 0.0,
   GUID={0x00020400,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}} */


/* Object interface: IPCLArgument, ver. 0.0,
   GUID={0xD43BB8BB,0x2BA3,0x492e,{0xB3,0xAB,0x15,0x00,0x71,0xB8,0x79,0xA9}} */

#pragma code_seg(".orpc")
static const unsigned short IPCLArgument_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    0,
    36,
    72,
    108
    };

static const MIDL_STUBLESS_PROXY_INFO IPCLArgument_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &IPCLArgument_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO IPCLArgument_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &IPCLArgument_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(11) _IPCLArgumentProxyVtbl = 
{
    &IPCLArgument_ProxyInfo,
    &IID_IPCLArgument,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *) (INT_PTR) -1 /* IPCLArgument::getType */ ,
    (void *) (INT_PTR) -1 /* IPCLArgument::getRank */ ,
    (void *) (INT_PTR) -1 /* IPCLArgument::getIsReference */ ,
    (void *) (INT_PTR) -1 /* IPCLArgument::getLastError */
};


static const PRPC_STUB_FUNCTION IPCLArgument_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _IPCLArgumentStubVtbl =
{
    &IID_IPCLArgument,
    &IPCLArgument_ServerInfo,
    11,
    &IPCLArgument_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Standard interface: __MIDL_itf_PCLExtension_0116, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */


/* Object interface: IPCLWavefileLock, ver. 0.0,
   GUID={0xE0934AAF,0x8832,0x44cf,{0xA4,0x90,0x06,0xF9,0x17,0x2F,0xC7,0xBB}} */

#pragma code_seg(".orpc")
static const unsigned short IPCLWavefileLock_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    144,
    180,
    216
    };

static const MIDL_STUBLESS_PROXY_INFO IPCLWavefileLock_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &IPCLWavefileLock_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO IPCLWavefileLock_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &IPCLWavefileLock_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(10) _IPCLWavefileLockProxyVtbl = 
{
    &IPCLWavefileLock_ProxyInfo,
    &IID_IPCLWavefileLock,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *) (INT_PTR) -1 /* IPCLWavefileLock::getDataPtr */ ,
    (void *) (INT_PTR) -1 /* IPCLWavefileLock::getParameters */ ,
    (void *) (INT_PTR) -1 /* IPCLWavefileLock::getLastError */
};


static const PRPC_STUB_FUNCTION IPCLWavefileLock_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _IPCLWavefileLockStubVtbl =
{
    &IID_IPCLWavefileLock,
    &IPCLWavefileLock_ServerInfo,
    10,
    &IPCLWavefileLock_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Object interface: IPCLWavefile, ver. 0.0,
   GUID={0xA60D18A4,0x8C3F,0x44f0,{0x97,0xFA,0xB3,0x99,0xE5,0x6F,0xFD,0xC2}} */

#pragma code_seg(".orpc")
static const unsigned short IPCLWavefile_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    0,
    36,
    72,
    108,
    252,
    288,
    324,
    366,
    402
    };

static const MIDL_STUBLESS_PROXY_INFO IPCLWavefile_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &IPCLWavefile_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO IPCLWavefile_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &IPCLWavefile_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(16) _IPCLWavefileProxyVtbl = 
{
    &IPCLWavefile_ProxyInfo,
    &IID_IPCLWavefile,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *) (INT_PTR) -1 /* IPCLArgument::getType */ ,
    (void *) (INT_PTR) -1 /* IPCLArgument::getRank */ ,
    (void *) (INT_PTR) -1 /* IPCLArgument::getIsReference */ ,
    (void *) (INT_PTR) -1 /* IPCLArgument::getLastError */ ,
    (void *) (INT_PTR) -1 /* IPCLWavefile::getIsNull */ ,
    (void *) (INT_PTR) -1 /* IPCLWavefile::getParameters */ ,
    (void *) (INT_PTR) -1 /* IPCLWavefile::resize */ ,
    (void *) (INT_PTR) -1 /* IPCLWavefile::lock */ ,
    (void *) (INT_PTR) -1 /* IPCLWavefile::replace */
};


static const PRPC_STUB_FUNCTION IPCLWavefile_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _IPCLWavefileStubVtbl =
{
    &IID_IPCLWavefile,
    &IPCLWavefile_ServerInfo,
    16,
    &IPCLWavefile_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Standard interface: __MIDL_itf_PCLExtension_0118, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */


/* Object interface: IPCLGraphicSurfaceLock, ver. 0.0,
   GUID={0x44E35511,0xBC78,0x40a5,{0xB7,0x13,0x0E,0x90,0x77,0x93,0x3E,0x8D}} */

#pragma code_seg(".orpc")
static const unsigned short IPCLGraphicSurfaceLock_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    144,
    444,
    216
    };

static const MIDL_STUBLESS_PROXY_INFO IPCLGraphicSurfaceLock_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &IPCLGraphicSurfaceLock_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO IPCLGraphicSurfaceLock_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &IPCLGraphicSurfaceLock_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(10) _IPCLGraphicSurfaceLockProxyVtbl = 
{
    &IPCLGraphicSurfaceLock_ProxyInfo,
    &IID_IPCLGraphicSurfaceLock,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *) (INT_PTR) -1 /* IPCLGraphicSurfaceLock::getDataPtr */ ,
    (void *) (INT_PTR) -1 /* IPCLGraphicSurfaceLock::getParameters */ ,
    (void *) (INT_PTR) -1 /* IPCLGraphicSurfaceLock::getLastError */
};


static const PRPC_STUB_FUNCTION IPCLGraphicSurfaceLock_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _IPCLGraphicSurfaceLockStubVtbl =
{
    &IID_IPCLGraphicSurfaceLock,
    &IPCLGraphicSurfaceLock_ServerInfo,
    10,
    &IPCLGraphicSurfaceLock_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Object interface: IPCLGraphicSurface, ver. 0.0,
   GUID={0xA96615CA,0x28B1,0x4958,{0x93,0x29,0x7F,0x1A,0x45,0x36,0x10,0xB7}} */

#pragma code_seg(".orpc")
static const unsigned short IPCLGraphicSurface_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    0,
    36,
    72,
    108,
    252,
    480,
    324,
    522,
    558,
    600
    };

static const MIDL_STUBLESS_PROXY_INFO IPCLGraphicSurface_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &IPCLGraphicSurface_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO IPCLGraphicSurface_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &IPCLGraphicSurface_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(17) _IPCLGraphicSurfaceProxyVtbl = 
{
    &IPCLGraphicSurface_ProxyInfo,
    &IID_IPCLGraphicSurface,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *) (INT_PTR) -1 /* IPCLArgument::getType */ ,
    (void *) (INT_PTR) -1 /* IPCLArgument::getRank */ ,
    (void *) (INT_PTR) -1 /* IPCLArgument::getIsReference */ ,
    (void *) (INT_PTR) -1 /* IPCLArgument::getLastError */ ,
    (void *) (INT_PTR) -1 /* IPCLGraphicSurface::getIsNull */ ,
    (void *) (INT_PTR) -1 /* IPCLGraphicSurface::getSize */ ,
    (void *) (INT_PTR) -1 /* IPCLGraphicSurface::resize */ ,
    (void *) (INT_PTR) -1 /* IPCLGraphicSurface::lock */ ,
    (void *) (INT_PTR) -1 /* IPCLGraphicSurface::replace */ ,
    (void *) (INT_PTR) -1 /* IPCLGraphicSurface::convertCustomUnits */
};


static const PRPC_STUB_FUNCTION IPCLGraphicSurface_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _IPCLGraphicSurfaceStubVtbl =
{
    &IID_IPCLGraphicSurface,
    &IPCLGraphicSurface_ServerInfo,
    17,
    &IPCLGraphicSurface_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Object interface: IPCLArray, ver. 0.0,
   GUID={0xDD64D836,0xF3DF,0x4097,{0xA1,0x2A,0x45,0xEF,0x44,0x1B,0x2F,0xD0}} */

#pragma code_seg(".orpc")
static const unsigned short IPCLArray_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    0,
    36,
    72,
    108,
    252,
    654,
    690
    };

static const MIDL_STUBLESS_PROXY_INFO IPCLArray_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &IPCLArray_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO IPCLArray_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &IPCLArray_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(14) _IPCLArrayProxyVtbl = 
{
    &IPCLArray_ProxyInfo,
    &IID_IPCLArray,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *) (INT_PTR) -1 /* IPCLArgument::getType */ ,
    (void *) (INT_PTR) -1 /* IPCLArgument::getRank */ ,
    (void *) (INT_PTR) -1 /* IPCLArgument::getIsReference */ ,
    (void *) (INT_PTR) -1 /* IPCLArgument::getLastError */ ,
    (void *) (INT_PTR) -1 /* IPCLArray::getCount */ ,
    (void *) (INT_PTR) -1 /* IPCLArray::resize */ ,
    (void *) (INT_PTR) -1 /* IPCLArray::getSubArray */
};


static const PRPC_STUB_FUNCTION IPCLArray_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _IPCLArrayStubVtbl =
{
    &IID_IPCLArray,
    &IPCLArray_ServerInfo,
    14,
    &IPCLArray_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Object interface: IPCLStringArray, ver. 0.0,
   GUID={0xCEB1D686,0xB7EC,0x44bc,{0x8F,0xDA,0x87,0x0C,0x50,0x88,0x40,0x88}} */

#pragma code_seg(".orpc")
static const unsigned short IPCLStringArray_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    0,
    36,
    72,
    108,
    252,
    654,
    690,
    732,
    774,
    816
    };

static const MIDL_STUBLESS_PROXY_INFO IPCLStringArray_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &IPCLStringArray_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO IPCLStringArray_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &IPCLStringArray_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(17) _IPCLStringArrayProxyVtbl = 
{
    &IPCLStringArray_ProxyInfo,
    &IID_IPCLStringArray,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *) (INT_PTR) -1 /* IPCLArgument::getType */ ,
    (void *) (INT_PTR) -1 /* IPCLArgument::getRank */ ,
    (void *) (INT_PTR) -1 /* IPCLArgument::getIsReference */ ,
    (void *) (INT_PTR) -1 /* IPCLArgument::getLastError */ ,
    (void *) (INT_PTR) -1 /* IPCLArray::getCount */ ,
    (void *) (INT_PTR) -1 /* IPCLArray::resize */ ,
    (void *) (INT_PTR) -1 /* IPCLArray::getSubArray */ ,
    (void *) (INT_PTR) -1 /* IPCLStringArray::setValue */ ,
    (void *) (INT_PTR) -1 /* IPCLStringArray::getValue */ ,
    (void *) (INT_PTR) -1 /* IPCLStringArray::add */
};


static const PRPC_STUB_FUNCTION IPCLStringArray_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _IPCLStringArrayStubVtbl =
{
    &IID_IPCLStringArray,
    &IPCLStringArray_ServerInfo,
    17,
    &IPCLStringArray_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Object interface: IPCLIntArray, ver. 0.0,
   GUID={0x6602A29A,0x38D8,0x4c3e,{0xA9,0x80,0xF0,0x4A,0x0D,0xD6,0x48,0x95}} */

#pragma code_seg(".orpc")
static const unsigned short IPCLIntArray_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    0,
    36,
    72,
    108,
    252,
    654,
    690,
    852,
    888,
    930,
    972
    };

static const MIDL_STUBLESS_PROXY_INFO IPCLIntArray_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &IPCLIntArray_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO IPCLIntArray_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &IPCLIntArray_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(18) _IPCLIntArrayProxyVtbl = 
{
    &IPCLIntArray_ProxyInfo,
    &IID_IPCLIntArray,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *) (INT_PTR) -1 /* IPCLArgument::getType */ ,
    (void *) (INT_PTR) -1 /* IPCLArgument::getRank */ ,
    (void *) (INT_PTR) -1 /* IPCLArgument::getIsReference */ ,
    (void *) (INT_PTR) -1 /* IPCLArgument::getLastError */ ,
    (void *) (INT_PTR) -1 /* IPCLArray::getCount */ ,
    (void *) (INT_PTR) -1 /* IPCLArray::resize */ ,
    (void *) (INT_PTR) -1 /* IPCLArray::getSubArray */ ,
    (void *) (INT_PTR) -1 /* IPCLIntArray::getDataPtr */ ,
    (void *) (INT_PTR) -1 /* IPCLIntArray::setValue */ ,
    (void *) (INT_PTR) -1 /* IPCLIntArray::getValue */ ,
    (void *) (INT_PTR) -1 /* IPCLIntArray::add */
};


static const PRPC_STUB_FUNCTION IPCLIntArray_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _IPCLIntArrayStubVtbl =
{
    &IID_IPCLIntArray,
    &IPCLIntArray_ServerInfo,
    18,
    &IPCLIntArray_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Object interface: IPCLDoubleArray, ver. 0.0,
   GUID={0xCA5BDA29,0xB722,0x4518,{0xAF,0x47,0x26,0xC9,0xF8,0x62,0x87,0x52}} */

#pragma code_seg(".orpc")
static const unsigned short IPCLDoubleArray_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    0,
    36,
    72,
    108,
    252,
    654,
    690,
    1008,
    1044,
    1086,
    1128
    };

static const MIDL_STUBLESS_PROXY_INFO IPCLDoubleArray_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &IPCLDoubleArray_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO IPCLDoubleArray_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &IPCLDoubleArray_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(18) _IPCLDoubleArrayProxyVtbl = 
{
    &IPCLDoubleArray_ProxyInfo,
    &IID_IPCLDoubleArray,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *) (INT_PTR) -1 /* IPCLArgument::getType */ ,
    (void *) (INT_PTR) -1 /* IPCLArgument::getRank */ ,
    (void *) (INT_PTR) -1 /* IPCLArgument::getIsReference */ ,
    (void *) (INT_PTR) -1 /* IPCLArgument::getLastError */ ,
    (void *) (INT_PTR) -1 /* IPCLArray::getCount */ ,
    (void *) (INT_PTR) -1 /* IPCLArray::resize */ ,
    (void *) (INT_PTR) -1 /* IPCLArray::getSubArray */ ,
    (void *) (INT_PTR) -1 /* IPCLDoubleArray::getDataPtr */ ,
    (void *) (INT_PTR) -1 /* IPCLDoubleArray::setValue */ ,
    (void *) (INT_PTR) -1 /* IPCLDoubleArray::getValue */ ,
    (void *) (INT_PTR) -1 /* IPCLDoubleArray::add */
};


static const PRPC_STUB_FUNCTION IPCLDoubleArray_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _IPCLDoubleArrayStubVtbl =
{
    &IID_IPCLDoubleArray,
    &IPCLDoubleArray_ServerInfo,
    18,
    &IPCLDoubleArray_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Object interface: IPCLBoolArray, ver. 0.0,
   GUID={0xFF2B4A6F,0x7F73,0x41b4,{0xA6,0x96,0xF2,0x8C,0x1F,0x31,0x47,0xED}} */

#pragma code_seg(".orpc")
static const unsigned short IPCLBoolArray_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    0,
    36,
    72,
    108,
    252,
    654,
    690,
    1164,
    1200,
    1242,
    1284
    };

static const MIDL_STUBLESS_PROXY_INFO IPCLBoolArray_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &IPCLBoolArray_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO IPCLBoolArray_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &IPCLBoolArray_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(18) _IPCLBoolArrayProxyVtbl = 
{
    &IPCLBoolArray_ProxyInfo,
    &IID_IPCLBoolArray,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *) (INT_PTR) -1 /* IPCLArgument::getType */ ,
    (void *) (INT_PTR) -1 /* IPCLArgument::getRank */ ,
    (void *) (INT_PTR) -1 /* IPCLArgument::getIsReference */ ,
    (void *) (INT_PTR) -1 /* IPCLArgument::getLastError */ ,
    (void *) (INT_PTR) -1 /* IPCLArray::getCount */ ,
    (void *) (INT_PTR) -1 /* IPCLArray::resize */ ,
    (void *) (INT_PTR) -1 /* IPCLArray::getSubArray */ ,
    (void *) (INT_PTR) -1 /* IPCLBoolArray::getDataPtr */ ,
    (void *) (INT_PTR) -1 /* IPCLBoolArray::setValue */ ,
    (void *) (INT_PTR) -1 /* IPCLBoolArray::getValue */ ,
    (void *) (INT_PTR) -1 /* IPCLBoolArray::add */
};


static const PRPC_STUB_FUNCTION IPCLBoolArray_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _IPCLBoolArrayStubVtbl =
{
    &IID_IPCLBoolArray,
    &IPCLBoolArray_ServerInfo,
    18,
    &IPCLBoolArray_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Object interface: IPCLString, ver. 0.0,
   GUID={0x023D3759,0x685A,0x4c63,{0x85,0xB3,0x9A,0x50,0xD5,0x45,0xE5,0x65}} */

#pragma code_seg(".orpc")
static const unsigned short IPCLString_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    0,
    36,
    72,
    108,
    1320,
    1356
    };

static const MIDL_STUBLESS_PROXY_INFO IPCLString_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &IPCLString_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO IPCLString_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &IPCLString_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(13) _IPCLStringProxyVtbl = 
{
    &IPCLString_ProxyInfo,
    &IID_IPCLString,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *) (INT_PTR) -1 /* IPCLArgument::getType */ ,
    (void *) (INT_PTR) -1 /* IPCLArgument::getRank */ ,
    (void *) (INT_PTR) -1 /* IPCLArgument::getIsReference */ ,
    (void *) (INT_PTR) -1 /* IPCLArgument::getLastError */ ,
    (void *) (INT_PTR) -1 /* IPCLString::getValue */ ,
    (void *) (INT_PTR) -1 /* IPCLString::setValue */
};


static const PRPC_STUB_FUNCTION IPCLString_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _IPCLStringStubVtbl =
{
    &IID_IPCLString,
    &IPCLString_ServerInfo,
    13,
    &IPCLString_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Object interface: IPCLInt, ver. 0.0,
   GUID={0xCF00C2BD,0x1595,0x4c6d,{0xB2,0xC9,0x8E,0xB8,0x8B,0x79,0xA0,0x78}} */

#pragma code_seg(".orpc")
static const unsigned short IPCLInt_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    0,
    36,
    72,
    108,
    252,
    654
    };

static const MIDL_STUBLESS_PROXY_INFO IPCLInt_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &IPCLInt_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO IPCLInt_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &IPCLInt_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(13) _IPCLIntProxyVtbl = 
{
    &IPCLInt_ProxyInfo,
    &IID_IPCLInt,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *) (INT_PTR) -1 /* IPCLArgument::getType */ ,
    (void *) (INT_PTR) -1 /* IPCLArgument::getRank */ ,
    (void *) (INT_PTR) -1 /* IPCLArgument::getIsReference */ ,
    (void *) (INT_PTR) -1 /* IPCLArgument::getLastError */ ,
    (void *) (INT_PTR) -1 /* IPCLInt::getValue */ ,
    (void *) (INT_PTR) -1 /* IPCLInt::setValue */
};


static const PRPC_STUB_FUNCTION IPCLInt_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _IPCLIntStubVtbl =
{
    &IID_IPCLInt,
    &IPCLInt_ServerInfo,
    13,
    &IPCLInt_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Object interface: IPCLDouble, ver. 0.0,
   GUID={0xC41A6728,0x6307,0x4846,{0x88,0x3C,0x0C,0x8B,0xF4,0xAA,0xCA,0x17}} */

#pragma code_seg(".orpc")
static const unsigned short IPCLDouble_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    0,
    36,
    72,
    108,
    1392,
    1428
    };

static const MIDL_STUBLESS_PROXY_INFO IPCLDouble_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &IPCLDouble_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO IPCLDouble_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &IPCLDouble_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(13) _IPCLDoubleProxyVtbl = 
{
    &IPCLDouble_ProxyInfo,
    &IID_IPCLDouble,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *) (INT_PTR) -1 /* IPCLArgument::getType */ ,
    (void *) (INT_PTR) -1 /* IPCLArgument::getRank */ ,
    (void *) (INT_PTR) -1 /* IPCLArgument::getIsReference */ ,
    (void *) (INT_PTR) -1 /* IPCLArgument::getLastError */ ,
    (void *) (INT_PTR) -1 /* IPCLDouble::getValue */ ,
    (void *) (INT_PTR) -1 /* IPCLDouble::setValue */
};


static const PRPC_STUB_FUNCTION IPCLDouble_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _IPCLDoubleStubVtbl =
{
    &IID_IPCLDouble,
    &IPCLDouble_ServerInfo,
    13,
    &IPCLDouble_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Object interface: IPCLBool, ver. 0.0,
   GUID={0x525A6207,0xB78E,0x47b1,{0xAB,0x8E,0x4D,0xED,0x01,0x7B,0x29,0x9D}} */

#pragma code_seg(".orpc")
static const unsigned short IPCLBool_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    0,
    36,
    72,
    108,
    252,
    654
    };

static const MIDL_STUBLESS_PROXY_INFO IPCLBool_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &IPCLBool_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO IPCLBool_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &IPCLBool_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(13) _IPCLBoolProxyVtbl = 
{
    &IPCLBool_ProxyInfo,
    &IID_IPCLBool,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *) (INT_PTR) -1 /* IPCLArgument::getType */ ,
    (void *) (INT_PTR) -1 /* IPCLArgument::getRank */ ,
    (void *) (INT_PTR) -1 /* IPCLArgument::getIsReference */ ,
    (void *) (INT_PTR) -1 /* IPCLArgument::getLastError */ ,
    (void *) (INT_PTR) -1 /* IPCLBool::getValue */ ,
    (void *) (INT_PTR) -1 /* IPCLBool::setValue */
};


static const PRPC_STUB_FUNCTION IPCLBool_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _IPCLBoolStubVtbl =
{
    &IID_IPCLBool,
    &IPCLBool_ServerInfo,
    13,
    &IPCLBool_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Object interface: IPCLArguments, ver. 0.0,
   GUID={0xB8D5BA56,0x01D5,0x4237,{0x92,0xA1,0x73,0xC9,0x72,0xC8,0x47,0x0B}} */

#pragma code_seg(".orpc")
static const unsigned short IPCLArguments_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    1464,
    1500,
    1542,
    108
    };

static const MIDL_STUBLESS_PROXY_INFO IPCLArguments_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &IPCLArguments_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO IPCLArguments_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &IPCLArguments_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(11) _IPCLArgumentsProxyVtbl = 
{
    &IPCLArguments_ProxyInfo,
    &IID_IPCLArguments,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *) (INT_PTR) -1 /* IPCLArguments::getCount */ ,
    (void *) (INT_PTR) -1 /* IPCLArguments::getArgument */ ,
    (void *) (INT_PTR) -1 /* IPCLArguments::getReturnValue */ ,
    (void *) (INT_PTR) -1 /* IPCLArguments::getLastError */
};


static const PRPC_STUB_FUNCTION IPCLArguments_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _IPCLArgumentsStubVtbl =
{
    &IID_IPCLArguments,
    &IPCLArguments_ServerInfo,
    11,
    &IPCLArguments_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Standard interface: __MIDL_itf_PCLExtension_0130, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */


/* Object interface: IPCLObject, ver. 0.0,
   GUID={0x83CE0178,0xFAE2,0x4734,{0x8A,0x90,0x31,0x38,0x95,0x43,0xE4,0xAE}} */

#pragma code_seg(".orpc")
static const unsigned short IPCLObject_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    1578,
    1620,
    216
    };

static const MIDL_STUBLESS_PROXY_INFO IPCLObject_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &IPCLObject_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO IPCLObject_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &IPCLObject_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(10) _IPCLObjectProxyVtbl = 
{
    &IPCLObject_ProxyInfo,
    &IID_IPCLObject,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *) (INT_PTR) -1 /* IPCLObject::callMethod */ ,
    (void *) (INT_PTR) -1 /* IPCLObject::getType */ ,
    (void *) (INT_PTR) -1 /* IPCLObject::getLastError */
};


static const PRPC_STUB_FUNCTION IPCLObject_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _IPCLObjectStubVtbl =
{
    &IID_IPCLObject,
    &IPCLObject_ServerInfo,
    10,
    &IPCLObject_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Object interface: IPCLExtensionObject, ver. 0.0,
   GUID={0x0DECE6D3,0x85AE,0x4d3c,{0x95,0x3A,0x15,0xA5,0x55,0x04,0xC9,0xE6}} */

#pragma code_seg(".orpc")
static const unsigned short IPCLExtensionObject_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    0,
    36,
    72,
    108,
    1656,
    1692
    };

static const MIDL_STUBLESS_PROXY_INFO IPCLExtensionObject_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &IPCLExtensionObject_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO IPCLExtensionObject_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &IPCLExtensionObject_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(13) _IPCLExtensionObjectProxyVtbl = 
{
    &IPCLExtensionObject_ProxyInfo,
    &IID_IPCLExtensionObject,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *) (INT_PTR) -1 /* IPCLArgument::getType */ ,
    (void *) (INT_PTR) -1 /* IPCLArgument::getRank */ ,
    (void *) (INT_PTR) -1 /* IPCLArgument::getIsReference */ ,
    (void *) (INT_PTR) -1 /* IPCLArgument::getLastError */ ,
    (void *) (INT_PTR) -1 /* IPCLExtensionObject::getValue */ ,
    (void *) (INT_PTR) -1 /* IPCLExtensionObject::setValue */
};


static const PRPC_STUB_FUNCTION IPCLExtensionObject_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _IPCLExtensionObjectStubVtbl =
{
    &IID_IPCLExtensionObject,
    &IPCLExtensionObject_ServerInfo,
    13,
    &IPCLExtensionObject_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Object interface: IPCLMethod, ver. 0.0,
   GUID={0xB74B2585,0x4C45,0x4747,{0xA2,0x97,0x9E,0xD8,0x41,0xF4,0x73,0x1D}} */

#pragma code_seg(".orpc")
static const unsigned short IPCLMethod_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    1728,
    1764,
    1824,
    1872,
    1320
    };

static const MIDL_STUBLESS_PROXY_INFO IPCLMethod_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &IPCLMethod_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO IPCLMethod_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &IPCLMethod_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(12) _IPCLMethodProxyVtbl = 
{
    &IPCLMethod_ProxyInfo,
    &IID_IPCLMethod,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *) (INT_PTR) -1 /* IPCLMethod::setName */ ,
    (void *) (INT_PTR) -1 /* IPCLMethod::addArgument */ ,
    (void *) (INT_PTR) -1 /* IPCLMethod::setReturnValue */ ,
    (void *) (INT_PTR) -1 /* IPCLMethod::setDescription */ ,
    (void *) (INT_PTR) -1 /* IPCLMethod::getLastError */
};


static const PRPC_STUB_FUNCTION IPCLMethod_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _IPCLMethodStubVtbl =
{
    &IID_IPCLMethod,
    &IPCLMethod_ServerInfo,
    12,
    &IPCLMethod_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Object interface: IPCLMethodList, ver. 0.0,
   GUID={0x721705E4,0x275F,0x4aed,{0xBF,0xA9,0x69,0x57,0xE4,0x0B,0xDE,0x4F}} */

#pragma code_seg(".orpc")
static const unsigned short IPCLMethodList_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    1908,
    1944,
    1980,
    108
    };

static const MIDL_STUBLESS_PROXY_INFO IPCLMethodList_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &IPCLMethodList_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO IPCLMethodList_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &IPCLMethodList_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(11) _IPCLMethodListProxyVtbl = 
{
    &IPCLMethodList_ProxyInfo,
    &IID_IPCLMethodList,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *) (INT_PTR) -1 /* IPCLMethodList::addMethod */ ,
    (void *) (INT_PTR) -1 /* IPCLMethodList::addNew */ ,
    (void *) (INT_PTR) -1 /* IPCLMethodList::addPredefinedVariable */ ,
    (void *) (INT_PTR) -1 /* IPCLMethodList::getLastError */
};


static const PRPC_STUB_FUNCTION IPCLMethodList_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _IPCLMethodListStubVtbl =
{
    &IID_IPCLMethodList,
    &IPCLMethodList_ServerInfo,
    11,
    &IPCLMethodList_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Standard interface: __MIDL_itf_PCLExtension_0134, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */


/* Object interface: IPCLType, ver. 0.0,
   GUID={0x12349DAF,0xFD20,0x4ba6,{0xB5,0x00,0x0F,0xC8,0xE0,0xE4,0x9A,0x0B}} */

#pragma code_seg(".orpc")
static const unsigned short IPCLType_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    0,
    2028,
    216,
    2064,
    2100,
    2148,
    2184
    };

static const MIDL_STUBLESS_PROXY_INFO IPCLType_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &IPCLType_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO IPCLType_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &IPCLType_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(14) _IPCLTypeProxyVtbl = 
{
    &IPCLType_ProxyInfo,
    &IID_IPCLType,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *) (INT_PTR) -1 /* IPCLType::getName */ ,
    (void *) (INT_PTR) -1 /* IPCLType::getDescription */ ,
    (void *) (INT_PTR) -1 /* IPCLType::getParentName */ ,
    (void *) (INT_PTR) -1 /* IPCLType::getMethods */ ,
    (void *) (INT_PTR) -1 /* IPCLType::createObject */ ,
    (void *) (INT_PTR) -1 /* IPCLType::getLibrary */ ,
    (void *) (INT_PTR) -1 /* IPCLType::getLastError */
};


static const PRPC_STUB_FUNCTION IPCLType_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _IPCLTypeStubVtbl =
{
    &IID_IPCLType,
    &IPCLType_ServerInfo,
    14,
    &IPCLType_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Object interface: IPCLLibrary, ver. 0.0,
   GUID={0x5483302B,0x931F,0x4506,{0xA9,0xD0,0x30,0xF5,0xC2,0x23,0x48,0xCB}} */

#pragma code_seg(".orpc")
static const unsigned short IPCLLibrary_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    0,
    2028,
    216,
    108,
    1392,
    2220,
    2256,
    2292,
    2334
    };

static const MIDL_STUBLESS_PROXY_INFO IPCLLibrary_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &IPCLLibrary_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO IPCLLibrary_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &IPCLLibrary_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(16) _IPCLLibraryProxyVtbl = 
{
    &IPCLLibrary_ProxyInfo,
    &IID_IPCLLibrary,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *) (INT_PTR) -1 /* IPCLLibrary::getName */ ,
    (void *) (INT_PTR) -1 /* IPCLLibrary::getAuthor */ ,
    (void *) (INT_PTR) -1 /* IPCLLibrary::getVersion */ ,
    (void *) (INT_PTR) -1 /* IPCLLibrary::getDescription */ ,
    (void *) (INT_PTR) -1 /* IPCLLibrary::getPresentationMinimumVersion */ ,
    (void *) (INT_PTR) -1 /* IPCLLibrary::getNamespaceName */ ,
    (void *) (INT_PTR) -1 /* IPCLLibrary::getTypeCount */ ,
    (void *) (INT_PTR) -1 /* IPCLLibrary::getType */ ,
    (void *) (INT_PTR) -1 /* IPCLLibrary::getLastError */
};


static const PRPC_STUB_FUNCTION IPCLLibrary_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _IPCLLibraryStubVtbl =
{
    &IID_IPCLLibrary,
    &IPCLLibrary_ServerInfo,
    16,
    &IPCLLibrary_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};

static const MIDL_STUB_DESC Object_StubDesc = 
    {
    0,
    NdrOleAllocate,
    NdrOleFree,
    0,
    0,
    0,
    0,
    0,
    __MIDL_TypeFormatString.Format,
    1, /* -error bounds_check flag */
    0x50002, /* Ndr library version */
    0,
    0x6000169, /* MIDL Version 6.0.361 */
    0,
    UserMarshalRoutines,
    0,  /* notify & notify_flag routine table */
    0x1, /* MIDL flag */
    0, /* cs routines */
    0,   /* proxy/server info */
    0   /* Reserved5 */
    };

const CInterfaceProxyVtbl * _PCLExtension_ProxyVtblList[] = 
{
    ( CInterfaceProxyVtbl *) &_IPCLBoolProxyVtbl,
    ( CInterfaceProxyVtbl *) &_IPCLGraphicSurfaceLockProxyVtbl,
    ( CInterfaceProxyVtbl *) &_IPCLDoubleProxyVtbl,
    ( CInterfaceProxyVtbl *) &_IPCLDoubleArrayProxyVtbl,
    ( CInterfaceProxyVtbl *) &_IPCLLibraryProxyVtbl,
    ( CInterfaceProxyVtbl *) &_IPCLArrayProxyVtbl,
    ( CInterfaceProxyVtbl *) &_IPCLArgumentsProxyVtbl,
    ( CInterfaceProxyVtbl *) &_IPCLStringProxyVtbl,
    ( CInterfaceProxyVtbl *) &_IPCLBoolArrayProxyVtbl,
    ( CInterfaceProxyVtbl *) &_IPCLObjectProxyVtbl,
    ( CInterfaceProxyVtbl *) &_IPCLMethodProxyVtbl,
    ( CInterfaceProxyVtbl *) &_IPCLStringArrayProxyVtbl,
    ( CInterfaceProxyVtbl *) &_IPCLIntArrayProxyVtbl,
    ( CInterfaceProxyVtbl *) &_IPCLWavefileProxyVtbl,
    ( CInterfaceProxyVtbl *) &_IPCLWavefileLockProxyVtbl,
    ( CInterfaceProxyVtbl *) &_IPCLTypeProxyVtbl,
    ( CInterfaceProxyVtbl *) &_IPCLArgumentProxyVtbl,
    ( CInterfaceProxyVtbl *) &_IPCLIntProxyVtbl,
    ( CInterfaceProxyVtbl *) &_IPCLGraphicSurfaceProxyVtbl,
    ( CInterfaceProxyVtbl *) &_IPCLExtensionObjectProxyVtbl,
    ( CInterfaceProxyVtbl *) &_IPCLMethodListProxyVtbl,
    0
};

const CInterfaceStubVtbl * _PCLExtension_StubVtblList[] = 
{
    ( CInterfaceStubVtbl *) &_IPCLBoolStubVtbl,
    ( CInterfaceStubVtbl *) &_IPCLGraphicSurfaceLockStubVtbl,
    ( CInterfaceStubVtbl *) &_IPCLDoubleStubVtbl,
    ( CInterfaceStubVtbl *) &_IPCLDoubleArrayStubVtbl,
    ( CInterfaceStubVtbl *) &_IPCLLibraryStubVtbl,
    ( CInterfaceStubVtbl *) &_IPCLArrayStubVtbl,
    ( CInterfaceStubVtbl *) &_IPCLArgumentsStubVtbl,
    ( CInterfaceStubVtbl *) &_IPCLStringStubVtbl,
    ( CInterfaceStubVtbl *) &_IPCLBoolArrayStubVtbl,
    ( CInterfaceStubVtbl *) &_IPCLObjectStubVtbl,
    ( CInterfaceStubVtbl *) &_IPCLMethodStubVtbl,
    ( CInterfaceStubVtbl *) &_IPCLStringArrayStubVtbl,
    ( CInterfaceStubVtbl *) &_IPCLIntArrayStubVtbl,
    ( CInterfaceStubVtbl *) &_IPCLWavefileStubVtbl,
    ( CInterfaceStubVtbl *) &_IPCLWavefileLockStubVtbl,
    ( CInterfaceStubVtbl *) &_IPCLTypeStubVtbl,
    ( CInterfaceStubVtbl *) &_IPCLArgumentStubVtbl,
    ( CInterfaceStubVtbl *) &_IPCLIntStubVtbl,
    ( CInterfaceStubVtbl *) &_IPCLGraphicSurfaceStubVtbl,
    ( CInterfaceStubVtbl *) &_IPCLExtensionObjectStubVtbl,
    ( CInterfaceStubVtbl *) &_IPCLMethodListStubVtbl,
    0
};

PCInterfaceName const _PCLExtension_InterfaceNamesList[] = 
{
    "IPCLBool",
    "IPCLGraphicSurfaceLock",
    "IPCLDouble",
    "IPCLDoubleArray",
    "IPCLLibrary",
    "IPCLArray",
    "IPCLArguments",
    "IPCLString",
    "IPCLBoolArray",
    "IPCLObject",
    "IPCLMethod",
    "IPCLStringArray",
    "IPCLIntArray",
    "IPCLWavefile",
    "IPCLWavefileLock",
    "IPCLType",
    "IPCLArgument",
    "IPCLInt",
    "IPCLGraphicSurface",
    "IPCLExtensionObject",
    "IPCLMethodList",
    0
};

const IID *  _PCLExtension_BaseIIDList[] = 
{
    &IID_IDispatch,
    &IID_IDispatch,
    &IID_IDispatch,
    &IID_IDispatch,
    &IID_IDispatch,
    &IID_IDispatch,
    &IID_IDispatch,
    &IID_IDispatch,
    &IID_IDispatch,
    &IID_IDispatch,
    &IID_IDispatch,
    &IID_IDispatch,
    &IID_IDispatch,
    &IID_IDispatch,
    &IID_IDispatch,
    &IID_IDispatch,
    &IID_IDispatch,
    &IID_IDispatch,
    &IID_IDispatch,
    &IID_IDispatch,
    &IID_IDispatch,
    0
};


#define _PCLExtension_CHECK_IID(n)	IID_GENERIC_CHECK_IID( _PCLExtension, pIID, n)

int __stdcall _PCLExtension_IID_Lookup( const IID * pIID, int * pIndex )
{
    IID_BS_LOOKUP_SETUP

    IID_BS_LOOKUP_INITIAL_TEST( _PCLExtension, 21, 16 )
    IID_BS_LOOKUP_NEXT_TEST( _PCLExtension, 8 )
    IID_BS_LOOKUP_NEXT_TEST( _PCLExtension, 4 )
    IID_BS_LOOKUP_NEXT_TEST( _PCLExtension, 2 )
    IID_BS_LOOKUP_NEXT_TEST( _PCLExtension, 1 )
    IID_BS_LOOKUP_RETURN_RESULT( _PCLExtension, 21, *pIndex )
    
}

const ExtendedProxyFileInfo PCLExtension_ProxyFileInfo = 
{
    (PCInterfaceProxyVtblList *) & _PCLExtension_ProxyVtblList,
    (PCInterfaceStubVtblList *) & _PCLExtension_StubVtblList,
    (const PCInterfaceName * ) & _PCLExtension_InterfaceNamesList,
    (const IID ** ) & _PCLExtension_BaseIIDList,
    & _PCLExtension_IID_Lookup, 
    21,
    2,
    0, /* table of [async_uuid] interfaces */
    0, /* Filler1 */
    0, /* Filler2 */
    0  /* Filler3 */
};
#if _MSC_VER >= 1200
#pragma warning(pop)
#endif


#endif /* !defined(_M_IA64) && !defined(_M_AMD64)*/



/* this ALWAYS GENERATED file contains the proxy stub code */


 /* File created by MIDL compiler version 6.00.0361 */
/* at Tue Apr 20 13:50:57 2010
 */
/* Compiler settings for PCLExtension.idl:
    Oicf, W1, Zp8, env=Win64 (32b run,appending)
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#if defined(_M_IA64) || defined(_M_AMD64)


#pragma warning( disable: 4049 )  /* more than 64k source lines */
#if _MSC_VER >= 1200
#pragma warning(push)
#endif

#pragma warning( disable: 4211 )  /* redefine extent to static */
#pragma warning( disable: 4232 )  /* dllimport identity*/
#define USE_STUBLESS_PROXY


/* verify that the <rpcproxy.h> version is high enough to compile this file*/
#ifndef __REDQ_RPCPROXY_H_VERSION__
#define __REQUIRED_RPCPROXY_H_VERSION__ 475
#endif


#include "rpcproxy.h"
#ifndef __RPCPROXY_H_VERSION__
#error this stub requires an updated version of <rpcproxy.h>
#endif // __RPCPROXY_H_VERSION__


#include "PCLExtension.h"

#define TYPE_FORMAT_STRING_SIZE   363                               
#define PROC_FORMAT_STRING_SIZE   2493                              
#define TRANSMIT_AS_TABLE_SIZE    0            
#define WIRE_MARSHAL_TABLE_SIZE   1            

typedef struct _MIDL_TYPE_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ TYPE_FORMAT_STRING_SIZE ];
    } MIDL_TYPE_FORMAT_STRING;

typedef struct _MIDL_PROC_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ PROC_FORMAT_STRING_SIZE ];
    } MIDL_PROC_FORMAT_STRING;


static RPC_SYNTAX_IDENTIFIER  _RpcTransferSyntax = 
{{0x8A885D04,0x1CEB,0x11C9,{0x9F,0xE8,0x08,0x00,0x2B,0x10,0x48,0x60}},{2,0}};


extern const MIDL_TYPE_FORMAT_STRING __MIDL_TypeFormatString;
extern const MIDL_PROC_FORMAT_STRING __MIDL_ProcFormatString;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IPCLArgument_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO IPCLArgument_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IPCLWavefileLock_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO IPCLWavefileLock_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IPCLWavefile_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO IPCLWavefile_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IPCLGraphicSurfaceLock_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO IPCLGraphicSurfaceLock_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IPCLGraphicSurface_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO IPCLGraphicSurface_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IPCLArray_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO IPCLArray_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IPCLStringArray_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO IPCLStringArray_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IPCLIntArray_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO IPCLIntArray_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IPCLDoubleArray_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO IPCLDoubleArray_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IPCLBoolArray_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO IPCLBoolArray_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IPCLString_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO IPCLString_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IPCLInt_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO IPCLInt_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IPCLDouble_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO IPCLDouble_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IPCLBool_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO IPCLBool_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IPCLArguments_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO IPCLArguments_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IPCLObject_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO IPCLObject_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IPCLExtensionObject_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO IPCLExtensionObject_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IPCLMethod_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO IPCLMethod_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IPCLMethodList_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO IPCLMethodList_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IPCLType_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO IPCLType_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IPCLLibrary_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO IPCLLibrary_ProxyInfo;


extern const USER_MARSHAL_ROUTINE_QUADRUPLE UserMarshalRoutines[ WIRE_MARSHAL_TABLE_SIZE ];

#if !defined(__RPC_WIN64__)
#error  Invalid build platform for this stub.
#endif

static const MIDL_PROC_FORMAT_STRING __MIDL_ProcFormatString =
    {
        0,
        {

	/* Procedure getName */


	/* Procedure getName */


	/* Procedure getType */

			0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/*  2 */	NdrFcLong( 0x0 ),	/* 0 */
/*  6 */	NdrFcShort( 0x7 ),	/* 7 */
/*  8 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 10 */	NdrFcShort( 0x0 ),	/* 0 */
/* 12 */	NdrFcShort( 0x8 ),	/* 8 */
/* 14 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 16 */	0xa,		/* 10 */
			0x3,		/* Ext Flags:  new corr desc, clt corr check, */
/* 18 */	NdrFcShort( 0x1 ),	/* 1 */
/* 20 */	NdrFcShort( 0x0 ),	/* 0 */
/* 22 */	NdrFcShort( 0x0 ),	/* 0 */
/* 24 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter name */


	/* Parameter name */


	/* Parameter type */

/* 26 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
/* 28 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 30 */	NdrFcShort( 0x20 ),	/* Type Offset=32 */

	/* Return value */


	/* Return value */


	/* Return value */

/* 32 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 34 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 36 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure getRank */

/* 38 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 40 */	NdrFcLong( 0x0 ),	/* 0 */
/* 44 */	NdrFcShort( 0x8 ),	/* 8 */
/* 46 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 48 */	NdrFcShort( 0x0 ),	/* 0 */
/* 50 */	NdrFcShort( 0x24 ),	/* 36 */
/* 52 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 54 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 56 */	NdrFcShort( 0x0 ),	/* 0 */
/* 58 */	NdrFcShort( 0x0 ),	/* 0 */
/* 60 */	NdrFcShort( 0x0 ),	/* 0 */
/* 62 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter rank */

/* 64 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 66 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 68 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 70 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 72 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 74 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure getIsReference */

/* 76 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 78 */	NdrFcLong( 0x0 ),	/* 0 */
/* 82 */	NdrFcShort( 0x9 ),	/* 9 */
/* 84 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 86 */	NdrFcShort( 0x0 ),	/* 0 */
/* 88 */	NdrFcShort( 0x24 ),	/* 36 */
/* 90 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 92 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 94 */	NdrFcShort( 0x0 ),	/* 0 */
/* 96 */	NdrFcShort( 0x0 ),	/* 0 */
/* 98 */	NdrFcShort( 0x0 ),	/* 0 */
/* 100 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter isRef */

/* 102 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 104 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 106 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 108 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 110 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 112 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure getDescription */


	/* Procedure getLastError */


	/* Procedure getLastError */


	/* Procedure getLastError */

/* 114 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 116 */	NdrFcLong( 0x0 ),	/* 0 */
/* 120 */	NdrFcShort( 0xa ),	/* 10 */
/* 122 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 124 */	NdrFcShort( 0x0 ),	/* 0 */
/* 126 */	NdrFcShort( 0x8 ),	/* 8 */
/* 128 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 130 */	0xa,		/* 10 */
			0x3,		/* Ext Flags:  new corr desc, clt corr check, */
/* 132 */	NdrFcShort( 0x1 ),	/* 1 */
/* 134 */	NdrFcShort( 0x0 ),	/* 0 */
/* 136 */	NdrFcShort( 0x0 ),	/* 0 */
/* 138 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter desc */


	/* Parameter message */


	/* Parameter message */


	/* Parameter message */

/* 140 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
/* 142 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 144 */	NdrFcShort( 0x20 ),	/* Type Offset=32 */

	/* Return value */


	/* Return value */


	/* Return value */


	/* Return value */

/* 146 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 148 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 150 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure getDataPtr */


	/* Procedure getDataPtr */

/* 152 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 154 */	NdrFcLong( 0x0 ),	/* 0 */
/* 158 */	NdrFcShort( 0x7 ),	/* 7 */
/* 160 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 162 */	NdrFcShort( 0x0 ),	/* 0 */
/* 164 */	NdrFcShort( 0x35 ),	/* 53 */
/* 166 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 168 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 170 */	NdrFcShort( 0x0 ),	/* 0 */
/* 172 */	NdrFcShort( 0x0 ),	/* 0 */
/* 174 */	NdrFcShort( 0x0 ),	/* 0 */
/* 176 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter dataPtr */


	/* Parameter dataPtr */

/* 178 */	NdrFcShort( 0x2012 ),	/* Flags:  must free, out, srv alloc size=8 */
/* 180 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 182 */	NdrFcShort( 0x2e ),	/* Type Offset=46 */

	/* Return value */


	/* Return value */

/* 184 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 186 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 188 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure getParameters */

/* 190 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 192 */	NdrFcLong( 0x0 ),	/* 0 */
/* 196 */	NdrFcShort( 0x8 ),	/* 8 */
/* 198 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 200 */	NdrFcShort( 0x0 ),	/* 0 */
/* 202 */	NdrFcShort( 0x3c ),	/* 60 */
/* 204 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 206 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 208 */	NdrFcShort( 0x0 ),	/* 0 */
/* 210 */	NdrFcShort( 0x0 ),	/* 0 */
/* 212 */	NdrFcShort( 0x0 ),	/* 0 */
/* 214 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter parameters */

/* 216 */	NdrFcShort( 0x4112 ),	/* Flags:  must free, out, simple ref, srv alloc size=16 */
/* 218 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 220 */	NdrFcShort( 0x3a ),	/* Type Offset=58 */

	/* Return value */

/* 222 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 224 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 226 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure getVersion */


	/* Procedure getParentName */


	/* Procedure getLastError */


	/* Procedure getLastError */


	/* Procedure getLastError */

/* 228 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 230 */	NdrFcLong( 0x0 ),	/* 0 */
/* 234 */	NdrFcShort( 0x9 ),	/* 9 */
/* 236 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 238 */	NdrFcShort( 0x0 ),	/* 0 */
/* 240 */	NdrFcShort( 0x8 ),	/* 8 */
/* 242 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 244 */	0xa,		/* 10 */
			0x3,		/* Ext Flags:  new corr desc, clt corr check, */
/* 246 */	NdrFcShort( 0x1 ),	/* 1 */
/* 248 */	NdrFcShort( 0x0 ),	/* 0 */
/* 250 */	NdrFcShort( 0x0 ),	/* 0 */
/* 252 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter ver */


	/* Parameter parent */


	/* Parameter message */


	/* Parameter message */


	/* Parameter message */

/* 254 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
/* 256 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 258 */	NdrFcShort( 0x20 ),	/* Type Offset=32 */

	/* Return value */


	/* Return value */


	/* Return value */


	/* Return value */


	/* Return value */

/* 260 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 262 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 264 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure getValue */


	/* Procedure getValue */


	/* Procedure getCount */


	/* Procedure getIsNull */


	/* Procedure getIsNull */

/* 266 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 268 */	NdrFcLong( 0x0 ),	/* 0 */
/* 272 */	NdrFcShort( 0xb ),	/* 11 */
/* 274 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 276 */	NdrFcShort( 0x0 ),	/* 0 */
/* 278 */	NdrFcShort( 0x24 ),	/* 36 */
/* 280 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 282 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 284 */	NdrFcShort( 0x0 ),	/* 0 */
/* 286 */	NdrFcShort( 0x0 ),	/* 0 */
/* 288 */	NdrFcShort( 0x0 ),	/* 0 */
/* 290 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter val */


	/* Parameter val */


	/* Parameter count */


	/* Parameter null */


	/* Parameter null */

/* 292 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 294 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 296 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */


	/* Return value */


	/* Return value */


	/* Return value */


	/* Return value */

/* 298 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 300 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 302 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure getParameters */

/* 304 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 306 */	NdrFcLong( 0x0 ),	/* 0 */
/* 310 */	NdrFcShort( 0xc ),	/* 12 */
/* 312 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 314 */	NdrFcShort( 0x0 ),	/* 0 */
/* 316 */	NdrFcShort( 0x3c ),	/* 60 */
/* 318 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 320 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 322 */	NdrFcShort( 0x0 ),	/* 0 */
/* 324 */	NdrFcShort( 0x0 ),	/* 0 */
/* 326 */	NdrFcShort( 0x0 ),	/* 0 */
/* 328 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter parameters */

/* 330 */	NdrFcShort( 0x4112 ),	/* Flags:  must free, out, simple ref, srv alloc size=16 */
/* 332 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 334 */	NdrFcShort( 0x3a ),	/* Type Offset=58 */

	/* Return value */

/* 336 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 338 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 340 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure resize */


	/* Procedure resize */

/* 342 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 344 */	NdrFcLong( 0x0 ),	/* 0 */
/* 348 */	NdrFcShort( 0xd ),	/* 13 */
/* 350 */	NdrFcShort( 0x20 ),	/* ia64 Stack size/offset = 32 */
/* 352 */	NdrFcShort( 0x10 ),	/* 16 */
/* 354 */	NdrFcShort( 0x8 ),	/* 8 */
/* 356 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x3,		/* 3 */
/* 358 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 360 */	NdrFcShort( 0x0 ),	/* 0 */
/* 362 */	NdrFcShort( 0x0 ),	/* 0 */
/* 364 */	NdrFcShort( 0x0 ),	/* 0 */
/* 366 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter widthPixels */


	/* Parameter frameCount */

/* 368 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 370 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 372 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter heightPixels */


	/* Parameter channelCount */

/* 374 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 376 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 378 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */


	/* Return value */

/* 380 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 382 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 384 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure lock */

/* 386 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 388 */	NdrFcLong( 0x0 ),	/* 0 */
/* 392 */	NdrFcShort( 0xe ),	/* 14 */
/* 394 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 396 */	NdrFcShort( 0x0 ),	/* 0 */
/* 398 */	NdrFcShort( 0x8 ),	/* 8 */
/* 400 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 402 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 404 */	NdrFcShort( 0x0 ),	/* 0 */
/* 406 */	NdrFcShort( 0x0 ),	/* 0 */
/* 408 */	NdrFcShort( 0x0 ),	/* 0 */
/* 410 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter lockObject */

/* 412 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 414 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 416 */	NdrFcShort( 0x44 ),	/* Type Offset=68 */

	/* Return value */

/* 418 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 420 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 422 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure replace */

/* 424 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 426 */	NdrFcLong( 0x0 ),	/* 0 */
/* 430 */	NdrFcShort( 0xf ),	/* 15 */
/* 432 */	NdrFcShort( 0x28 ),	/* ia64 Stack size/offset = 40 */
/* 434 */	NdrFcShort( 0x39 ),	/* 57 */
/* 436 */	NdrFcShort( 0x8 ),	/* 8 */
/* 438 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x3,		/* 3 */
/* 440 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 442 */	NdrFcShort( 0x0 ),	/* 0 */
/* 444 */	NdrFcShort( 0x0 ),	/* 0 */
/* 446 */	NdrFcShort( 0x0 ),	/* 0 */
/* 448 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter parameters */

/* 450 */	NdrFcShort( 0x8a ),	/* Flags:  must free, in, by val, */
/* 452 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 454 */	NdrFcShort( 0x3a ),	/* Type Offset=58 */

	/* Parameter data */

/* 456 */	NdrFcShort( 0x148 ),	/* Flags:  in, base type, simple ref, */
/* 458 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 460 */	0x1,		/* FC_BYTE */
			0x0,		/* 0 */

	/* Return value */

/* 462 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 464 */	NdrFcShort( 0x20 ),	/* ia64 Stack size/offset = 32 */
/* 466 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure getParameters */

/* 468 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 470 */	NdrFcLong( 0x0 ),	/* 0 */
/* 474 */	NdrFcShort( 0x8 ),	/* 8 */
/* 476 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 478 */	NdrFcShort( 0x0 ),	/* 0 */
/* 480 */	NdrFcShort( 0x4c ),	/* 76 */
/* 482 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 484 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 486 */	NdrFcShort( 0x0 ),	/* 0 */
/* 488 */	NdrFcShort( 0x0 ),	/* 0 */
/* 490 */	NdrFcShort( 0x0 ),	/* 0 */
/* 492 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter parameters */

/* 494 */	NdrFcShort( 0x8112 ),	/* Flags:  must free, out, simple ref, srv alloc size=32 */
/* 496 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 498 */	NdrFcShort( 0x62 ),	/* Type Offset=98 */

	/* Return value */

/* 500 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 502 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 504 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure getSize */

/* 506 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 508 */	NdrFcLong( 0x0 ),	/* 0 */
/* 512 */	NdrFcShort( 0xc ),	/* 12 */
/* 514 */	NdrFcShort( 0x20 ),	/* ia64 Stack size/offset = 32 */
/* 516 */	NdrFcShort( 0x0 ),	/* 0 */
/* 518 */	NdrFcShort( 0x40 ),	/* 64 */
/* 520 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x3,		/* 3 */
/* 522 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 524 */	NdrFcShort( 0x0 ),	/* 0 */
/* 526 */	NdrFcShort( 0x0 ),	/* 0 */
/* 528 */	NdrFcShort( 0x0 ),	/* 0 */
/* 530 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter widthPixels */

/* 532 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 534 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 536 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter heightPixels */

/* 538 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 540 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 542 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 544 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 546 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 548 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure lock */

/* 550 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 552 */	NdrFcLong( 0x0 ),	/* 0 */
/* 556 */	NdrFcShort( 0xe ),	/* 14 */
/* 558 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 560 */	NdrFcShort( 0x0 ),	/* 0 */
/* 562 */	NdrFcShort( 0x8 ),	/* 8 */
/* 564 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 566 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 568 */	NdrFcShort( 0x0 ),	/* 0 */
/* 570 */	NdrFcShort( 0x0 ),	/* 0 */
/* 572 */	NdrFcShort( 0x0 ),	/* 0 */
/* 574 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter lockObject */

/* 576 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 578 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 580 */	NdrFcShort( 0x70 ),	/* Type Offset=112 */

	/* Return value */

/* 582 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 584 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 586 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure replace */

/* 588 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 590 */	NdrFcLong( 0x0 ),	/* 0 */
/* 594 */	NdrFcShort( 0xf ),	/* 15 */
/* 596 */	NdrFcShort( 0x38 ),	/* ia64 Stack size/offset = 56 */
/* 598 */	NdrFcShort( 0x49 ),	/* 73 */
/* 600 */	NdrFcShort( 0x8 ),	/* 8 */
/* 602 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x3,		/* 3 */
/* 604 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 606 */	NdrFcShort( 0x0 ),	/* 0 */
/* 608 */	NdrFcShort( 0x0 ),	/* 0 */
/* 610 */	NdrFcShort( 0x0 ),	/* 0 */
/* 612 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter parameters */

/* 614 */	NdrFcShort( 0x8a ),	/* Flags:  must free, in, by val, */
/* 616 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 618 */	NdrFcShort( 0x62 ),	/* Type Offset=98 */

	/* Parameter data */

/* 620 */	NdrFcShort( 0x148 ),	/* Flags:  in, base type, simple ref, */
/* 622 */	NdrFcShort( 0x28 ),	/* ia64 Stack size/offset = 40 */
/* 624 */	0x1,		/* FC_BYTE */
			0x0,		/* 0 */

	/* Return value */

/* 626 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 628 */	NdrFcShort( 0x30 ),	/* ia64 Stack size/offset = 48 */
/* 630 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure convertCustomUnits */

/* 632 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 634 */	NdrFcLong( 0x0 ),	/* 0 */
/* 638 */	NdrFcShort( 0x10 ),	/* 16 */
/* 640 */	NdrFcShort( 0x30 ),	/* ia64 Stack size/offset = 48 */
/* 642 */	NdrFcShort( 0x20 ),	/* 32 */
/* 644 */	NdrFcShort( 0x50 ),	/* 80 */
/* 646 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x5,		/* 5 */
/* 648 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 650 */	NdrFcShort( 0x0 ),	/* 0 */
/* 652 */	NdrFcShort( 0x0 ),	/* 0 */
/* 654 */	NdrFcShort( 0x0 ),	/* 0 */
/* 656 */	NdrFcShort( 0x28 ),	/* 40 */

	/* Parameter customWidth */

/* 658 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 660 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 662 */	0xc,		/* FC_DOUBLE */
			0x0,		/* 0 */

	/* Parameter customHeight */

/* 664 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 666 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 668 */	0xc,		/* FC_DOUBLE */
			0x0,		/* 0 */

	/* Parameter pixelWidth */

/* 670 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 672 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 674 */	0xc,		/* FC_DOUBLE */
			0x0,		/* 0 */

	/* Parameter pixelHeight */

/* 676 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 678 */	NdrFcShort( 0x20 ),	/* ia64 Stack size/offset = 32 */
/* 680 */	0xc,		/* FC_DOUBLE */
			0x0,		/* 0 */

	/* Return value */

/* 682 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 684 */	NdrFcShort( 0x28 ),	/* ia64 Stack size/offset = 40 */
/* 686 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure setValue */


	/* Procedure setValue */


	/* Procedure resize */

/* 688 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 690 */	NdrFcLong( 0x0 ),	/* 0 */
/* 694 */	NdrFcShort( 0xc ),	/* 12 */
/* 696 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 698 */	NdrFcShort( 0x8 ),	/* 8 */
/* 700 */	NdrFcShort( 0x8 ),	/* 8 */
/* 702 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 704 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 706 */	NdrFcShort( 0x0 ),	/* 0 */
/* 708 */	NdrFcShort( 0x0 ),	/* 0 */
/* 710 */	NdrFcShort( 0x0 ),	/* 0 */
/* 712 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter val */


	/* Parameter val */


	/* Parameter count */

/* 714 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 716 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 718 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */


	/* Return value */


	/* Return value */

/* 720 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 722 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 724 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure getSubArray */

/* 726 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 728 */	NdrFcLong( 0x0 ),	/* 0 */
/* 732 */	NdrFcShort( 0xd ),	/* 13 */
/* 734 */	NdrFcShort( 0x20 ),	/* ia64 Stack size/offset = 32 */
/* 736 */	NdrFcShort( 0x8 ),	/* 8 */
/* 738 */	NdrFcShort( 0x8 ),	/* 8 */
/* 740 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x3,		/* 3 */
/* 742 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 744 */	NdrFcShort( 0x0 ),	/* 0 */
/* 746 */	NdrFcShort( 0x0 ),	/* 0 */
/* 748 */	NdrFcShort( 0x0 ),	/* 0 */
/* 750 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter index */

/* 752 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 754 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 756 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter subArray */

/* 758 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 760 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 762 */	NdrFcShort( 0x8a ),	/* Type Offset=138 */

	/* Return value */

/* 764 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 766 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 768 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure setValue */

/* 770 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 772 */	NdrFcLong( 0x0 ),	/* 0 */
/* 776 */	NdrFcShort( 0xe ),	/* 14 */
/* 778 */	NdrFcShort( 0x20 ),	/* ia64 Stack size/offset = 32 */
/* 780 */	NdrFcShort( 0x8 ),	/* 8 */
/* 782 */	NdrFcShort( 0x8 ),	/* 8 */
/* 784 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 786 */	0xa,		/* 10 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 788 */	NdrFcShort( 0x0 ),	/* 0 */
/* 790 */	NdrFcShort( 0x1 ),	/* 1 */
/* 792 */	NdrFcShort( 0x0 ),	/* 0 */
/* 794 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter index */

/* 796 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 798 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 800 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter value */

/* 802 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 804 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 806 */	NdrFcShort( 0xa4 ),	/* Type Offset=164 */

	/* Return value */

/* 808 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 810 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 812 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure getValue */

/* 814 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 816 */	NdrFcLong( 0x0 ),	/* 0 */
/* 820 */	NdrFcShort( 0xf ),	/* 15 */
/* 822 */	NdrFcShort( 0x20 ),	/* ia64 Stack size/offset = 32 */
/* 824 */	NdrFcShort( 0x8 ),	/* 8 */
/* 826 */	NdrFcShort( 0x8 ),	/* 8 */
/* 828 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x3,		/* 3 */
/* 830 */	0xa,		/* 10 */
			0x3,		/* Ext Flags:  new corr desc, clt corr check, */
/* 832 */	NdrFcShort( 0x1 ),	/* 1 */
/* 834 */	NdrFcShort( 0x0 ),	/* 0 */
/* 836 */	NdrFcShort( 0x0 ),	/* 0 */
/* 838 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter index */

/* 840 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 842 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 844 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter value */

/* 846 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
/* 848 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 850 */	NdrFcShort( 0x20 ),	/* Type Offset=32 */

	/* Return value */

/* 852 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 854 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 856 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure add */

/* 858 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 860 */	NdrFcLong( 0x0 ),	/* 0 */
/* 864 */	NdrFcShort( 0x10 ),	/* 16 */
/* 866 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 868 */	NdrFcShort( 0x0 ),	/* 0 */
/* 870 */	NdrFcShort( 0x8 ),	/* 8 */
/* 872 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 874 */	0xa,		/* 10 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 876 */	NdrFcShort( 0x0 ),	/* 0 */
/* 878 */	NdrFcShort( 0x1 ),	/* 1 */
/* 880 */	NdrFcShort( 0x0 ),	/* 0 */
/* 882 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter value */

/* 884 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 886 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 888 */	NdrFcShort( 0xa4 ),	/* Type Offset=164 */

	/* Return value */

/* 890 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 892 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 894 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure getDataPtr */

/* 896 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 898 */	NdrFcLong( 0x0 ),	/* 0 */
/* 902 */	NdrFcShort( 0xe ),	/* 14 */
/* 904 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 906 */	NdrFcShort( 0x0 ),	/* 0 */
/* 908 */	NdrFcShort( 0x38 ),	/* 56 */
/* 910 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 912 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 914 */	NdrFcShort( 0x0 ),	/* 0 */
/* 916 */	NdrFcShort( 0x0 ),	/* 0 */
/* 918 */	NdrFcShort( 0x0 ),	/* 0 */
/* 920 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter dataPtr */

/* 922 */	NdrFcShort( 0x2012 ),	/* Flags:  must free, out, srv alloc size=8 */
/* 924 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 926 */	NdrFcShort( 0xae ),	/* Type Offset=174 */

	/* Return value */

/* 928 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 930 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 932 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure setValue */

/* 934 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 936 */	NdrFcLong( 0x0 ),	/* 0 */
/* 940 */	NdrFcShort( 0xf ),	/* 15 */
/* 942 */	NdrFcShort( 0x20 ),	/* ia64 Stack size/offset = 32 */
/* 944 */	NdrFcShort( 0x10 ),	/* 16 */
/* 946 */	NdrFcShort( 0x8 ),	/* 8 */
/* 948 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x3,		/* 3 */
/* 950 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 952 */	NdrFcShort( 0x0 ),	/* 0 */
/* 954 */	NdrFcShort( 0x0 ),	/* 0 */
/* 956 */	NdrFcShort( 0x0 ),	/* 0 */
/* 958 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter index */

/* 960 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 962 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 964 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter value */

/* 966 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 968 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 970 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 972 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 974 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 976 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure getValue */

/* 978 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 980 */	NdrFcLong( 0x0 ),	/* 0 */
/* 984 */	NdrFcShort( 0x10 ),	/* 16 */
/* 986 */	NdrFcShort( 0x20 ),	/* ia64 Stack size/offset = 32 */
/* 988 */	NdrFcShort( 0x8 ),	/* 8 */
/* 990 */	NdrFcShort( 0x24 ),	/* 36 */
/* 992 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x3,		/* 3 */
/* 994 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 996 */	NdrFcShort( 0x0 ),	/* 0 */
/* 998 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1000 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1002 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter index */

/* 1004 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1006 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 1008 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter value */

/* 1010 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 1012 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 1014 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 1016 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1018 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 1020 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure add */

/* 1022 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1024 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1028 */	NdrFcShort( 0x11 ),	/* 17 */
/* 1030 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 1032 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1034 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1036 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 1038 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1040 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1042 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1044 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1046 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter value */

/* 1048 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1050 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 1052 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 1054 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1056 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 1058 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure getDataPtr */

/* 1060 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1062 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1066 */	NdrFcShort( 0xe ),	/* 14 */
/* 1068 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 1070 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1072 */	NdrFcShort( 0x40 ),	/* 64 */
/* 1074 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 1076 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1078 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1080 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1082 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1084 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter dataPtr */

/* 1086 */	NdrFcShort( 0x2012 ),	/* Flags:  must free, out, srv alloc size=8 */
/* 1088 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 1090 */	NdrFcShort( 0xb6 ),	/* Type Offset=182 */

	/* Return value */

/* 1092 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1094 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 1096 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure setValue */

/* 1098 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1100 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1104 */	NdrFcShort( 0xf ),	/* 15 */
/* 1106 */	NdrFcShort( 0x20 ),	/* ia64 Stack size/offset = 32 */
/* 1108 */	NdrFcShort( 0x18 ),	/* 24 */
/* 1110 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1112 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x3,		/* 3 */
/* 1114 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1116 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1118 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1120 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1122 */	NdrFcShort( 0x20 ),	/* 32 */

	/* Parameter index */

/* 1124 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1126 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 1128 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter value */

/* 1130 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1132 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 1134 */	0xc,		/* FC_DOUBLE */
			0x0,		/* 0 */

	/* Return value */

/* 1136 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1138 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 1140 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure getValue */

/* 1142 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1144 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1148 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1150 */	NdrFcShort( 0x20 ),	/* ia64 Stack size/offset = 32 */
/* 1152 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1154 */	NdrFcShort( 0x2c ),	/* 44 */
/* 1156 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x3,		/* 3 */
/* 1158 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1160 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1162 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1164 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1166 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter index */

/* 1168 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1170 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 1172 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter value */

/* 1174 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 1176 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 1178 */	0xc,		/* FC_DOUBLE */
			0x0,		/* 0 */

	/* Return value */

/* 1180 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1182 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 1184 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure add */

/* 1186 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1188 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1192 */	NdrFcShort( 0x11 ),	/* 17 */
/* 1194 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 1196 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1198 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1200 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 1202 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1204 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1206 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1208 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1210 */	NdrFcShort( 0x8 ),	/* 8 */

	/* Parameter value */

/* 1212 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1214 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 1216 */	0xc,		/* FC_DOUBLE */
			0x0,		/* 0 */

	/* Return value */

/* 1218 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1220 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 1222 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure getDataPtr */

/* 1224 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1226 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1230 */	NdrFcShort( 0xe ),	/* 14 */
/* 1232 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 1234 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1236 */	NdrFcShort( 0x35 ),	/* 53 */
/* 1238 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 1240 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1242 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1244 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1246 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1248 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter dataPtr */

/* 1250 */	NdrFcShort( 0x2012 ),	/* Flags:  must free, out, srv alloc size=8 */
/* 1252 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 1254 */	NdrFcShort( 0x2e ),	/* Type Offset=46 */

	/* Return value */

/* 1256 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1258 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 1260 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure setValue */

/* 1262 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1264 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1268 */	NdrFcShort( 0xf ),	/* 15 */
/* 1270 */	NdrFcShort( 0x20 ),	/* ia64 Stack size/offset = 32 */
/* 1272 */	NdrFcShort( 0xd ),	/* 13 */
/* 1274 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1276 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x3,		/* 3 */
/* 1278 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1280 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1282 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1284 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1286 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter index */

/* 1288 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1290 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 1292 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter value */

/* 1294 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1296 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 1298 */	0x1,		/* FC_BYTE */
			0x0,		/* 0 */

	/* Return value */

/* 1300 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1302 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 1304 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure getValue */

/* 1306 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1308 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1312 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1314 */	NdrFcShort( 0x20 ),	/* ia64 Stack size/offset = 32 */
/* 1316 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1318 */	NdrFcShort( 0x21 ),	/* 33 */
/* 1320 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x3,		/* 3 */
/* 1322 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1324 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1326 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1328 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1330 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter index */

/* 1332 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1334 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 1336 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter value */

/* 1338 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 1340 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 1342 */	0x1,		/* FC_BYTE */
			0x0,		/* 0 */

	/* Return value */

/* 1344 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1346 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 1348 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure add */

/* 1350 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1352 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1356 */	NdrFcShort( 0x11 ),	/* 17 */
/* 1358 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 1360 */	NdrFcShort( 0x5 ),	/* 5 */
/* 1362 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1364 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 1366 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1368 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1370 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1372 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1374 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter value */

/* 1376 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1378 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 1380 */	0x1,		/* FC_BYTE */
			0x0,		/* 0 */

	/* Return value */

/* 1382 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1384 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 1386 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure getLastError */


	/* Procedure getValue */

/* 1388 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1390 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1394 */	NdrFcShort( 0xb ),	/* 11 */
/* 1396 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 1398 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1400 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1402 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 1404 */	0xa,		/* 10 */
			0x3,		/* Ext Flags:  new corr desc, clt corr check, */
/* 1406 */	NdrFcShort( 0x1 ),	/* 1 */
/* 1408 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1410 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1412 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter message */


	/* Parameter val */

/* 1414 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
/* 1416 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 1418 */	NdrFcShort( 0x20 ),	/* Type Offset=32 */

	/* Return value */


	/* Return value */

/* 1420 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1422 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 1424 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure setValue */

/* 1426 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1428 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1432 */	NdrFcShort( 0xc ),	/* 12 */
/* 1434 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 1436 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1438 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1440 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 1442 */	0xa,		/* 10 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 1444 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1446 */	NdrFcShort( 0x1 ),	/* 1 */
/* 1448 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1450 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter val */

/* 1452 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 1454 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 1456 */	NdrFcShort( 0xa4 ),	/* Type Offset=164 */

	/* Return value */

/* 1458 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1460 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 1462 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure getPresentationMinimumVersion */


	/* Procedure getValue */

/* 1464 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1466 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1470 */	NdrFcShort( 0xb ),	/* 11 */
/* 1472 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 1474 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1476 */	NdrFcShort( 0x2c ),	/* 44 */
/* 1478 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 1480 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1482 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1484 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1486 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1488 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter ver */


	/* Parameter val */

/* 1490 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 1492 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 1494 */	0xc,		/* FC_DOUBLE */
			0x0,		/* 0 */

	/* Return value */


	/* Return value */

/* 1496 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1498 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 1500 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure setValue */

/* 1502 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1504 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1508 */	NdrFcShort( 0xc ),	/* 12 */
/* 1510 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 1512 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1514 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1516 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 1518 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1520 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1522 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1524 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1526 */	NdrFcShort( 0x8 ),	/* 8 */

	/* Parameter val */

/* 1528 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1530 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 1532 */	0xc,		/* FC_DOUBLE */
			0x0,		/* 0 */

	/* Return value */

/* 1534 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1536 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 1538 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure getCount */

/* 1540 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1542 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1546 */	NdrFcShort( 0x7 ),	/* 7 */
/* 1548 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 1550 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1552 */	NdrFcShort( 0x24 ),	/* 36 */
/* 1554 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 1556 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1558 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1560 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1562 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1564 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter count */

/* 1566 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 1568 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 1570 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 1572 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1574 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 1576 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure getArgument */

/* 1578 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1580 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1584 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1586 */	NdrFcShort( 0x20 ),	/* ia64 Stack size/offset = 32 */
/* 1588 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1590 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1592 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x3,		/* 3 */
/* 1594 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1596 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1598 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1600 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1602 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter index */

/* 1604 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1606 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 1608 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter argument */

/* 1610 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 1612 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 1614 */	NdrFcShort( 0xc2 ),	/* Type Offset=194 */

	/* Return value */

/* 1616 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1618 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 1620 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure getReturnValue */

/* 1622 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1624 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1628 */	NdrFcShort( 0x9 ),	/* 9 */
/* 1630 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 1632 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1634 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1636 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 1638 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1640 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1642 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1644 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1646 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter value */

/* 1648 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 1650 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 1652 */	NdrFcShort( 0xc2 ),	/* Type Offset=194 */

	/* Return value */

/* 1654 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1656 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 1658 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure callMethod */

/* 1660 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1662 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1666 */	NdrFcShort( 0x7 ),	/* 7 */
/* 1668 */	NdrFcShort( 0x20 ),	/* ia64 Stack size/offset = 32 */
/* 1670 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1672 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1674 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 1676 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1678 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1680 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1682 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1684 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter methodIndex */

/* 1686 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1688 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 1690 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter arguments */

/* 1692 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 1694 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 1696 */	NdrFcShort( 0xd8 ),	/* Type Offset=216 */

	/* Return value */

/* 1698 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1700 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 1702 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure getType */

/* 1704 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1706 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1710 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1712 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 1714 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1716 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1718 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 1720 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1722 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1724 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1726 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1728 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter type */

/* 1730 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 1732 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 1734 */	NdrFcShort( 0xea ),	/* Type Offset=234 */

	/* Return value */

/* 1736 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1738 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 1740 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure getValue */

/* 1742 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1744 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1748 */	NdrFcShort( 0xb ),	/* 11 */
/* 1750 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 1752 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1754 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1756 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 1758 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1760 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1762 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1764 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1766 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter val */

/* 1768 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 1770 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 1772 */	NdrFcShort( 0x100 ),	/* Type Offset=256 */

	/* Return value */

/* 1774 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1776 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 1778 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure setValue */

/* 1780 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1782 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1786 */	NdrFcShort( 0xc ),	/* 12 */
/* 1788 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 1790 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1792 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1794 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 1796 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1798 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1800 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1802 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1804 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter val */

/* 1806 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 1808 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 1810 */	NdrFcShort( 0x104 ),	/* Type Offset=260 */

	/* Return value */

/* 1812 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1814 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 1816 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure setName */

/* 1818 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1820 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1824 */	NdrFcShort( 0x7 ),	/* 7 */
/* 1826 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 1828 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1830 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1832 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 1834 */	0xa,		/* 10 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 1836 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1838 */	NdrFcShort( 0x1 ),	/* 1 */
/* 1840 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1842 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter name */

/* 1844 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 1846 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 1848 */	NdrFcShort( 0xa4 ),	/* Type Offset=164 */

	/* Return value */

/* 1850 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1852 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 1854 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure addArgument */

/* 1856 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1858 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1862 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1864 */	NdrFcShort( 0x38 ),	/* ia64 Stack size/offset = 56 */
/* 1866 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1868 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1870 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x6,		/* 6 */
/* 1872 */	0xa,		/* 10 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 1874 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1876 */	NdrFcShort( 0x3 ),	/* 3 */
/* 1878 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1880 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter name */

/* 1882 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 1884 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 1886 */	NdrFcShort( 0xa4 ),	/* Type Offset=164 */

	/* Parameter type */

/* 1888 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 1890 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 1892 */	NdrFcShort( 0xa4 ),	/* Type Offset=164 */

	/* Parameter rank */

/* 1894 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1896 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 1898 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter isReference */

/* 1900 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1902 */	NdrFcShort( 0x20 ),	/* ia64 Stack size/offset = 32 */
/* 1904 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter description */

/* 1906 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 1908 */	NdrFcShort( 0x28 ),	/* ia64 Stack size/offset = 40 */
/* 1910 */	NdrFcShort( 0xa4 ),	/* Type Offset=164 */

	/* Return value */

/* 1912 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1914 */	NdrFcShort( 0x30 ),	/* ia64 Stack size/offset = 48 */
/* 1916 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure setReturnValue */

/* 1918 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1920 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1924 */	NdrFcShort( 0x9 ),	/* 9 */
/* 1926 */	NdrFcShort( 0x28 ),	/* ia64 Stack size/offset = 40 */
/* 1928 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1930 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1932 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x4,		/* 4 */
/* 1934 */	0xa,		/* 10 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 1936 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1938 */	NdrFcShort( 0x2 ),	/* 2 */
/* 1940 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1942 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter type */

/* 1944 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 1946 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 1948 */	NdrFcShort( 0xa4 ),	/* Type Offset=164 */

	/* Parameter rank */

/* 1950 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1952 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 1954 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter description */

/* 1956 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 1958 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 1960 */	NdrFcShort( 0xa4 ),	/* Type Offset=164 */

	/* Return value */

/* 1962 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1964 */	NdrFcShort( 0x20 ),	/* ia64 Stack size/offset = 32 */
/* 1966 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure setDescription */

/* 1968 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1970 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1974 */	NdrFcShort( 0xa ),	/* 10 */
/* 1976 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 1978 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1980 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1982 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 1984 */	0xa,		/* 10 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 1986 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1988 */	NdrFcShort( 0x1 ),	/* 1 */
/* 1990 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1992 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter desc */

/* 1994 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 1996 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 1998 */	NdrFcShort( 0xa4 ),	/* Type Offset=164 */

	/* Return value */

/* 2000 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2002 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 2004 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure addMethod */

/* 2006 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2008 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2012 */	NdrFcShort( 0x7 ),	/* 7 */
/* 2014 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 2016 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2018 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2020 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 2022 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2024 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2026 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2028 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2030 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter method */

/* 2032 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 2034 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 2036 */	NdrFcShort( 0x116 ),	/* Type Offset=278 */

	/* Return value */

/* 2038 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2040 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 2042 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure addNew */

/* 2044 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2046 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2050 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2052 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 2054 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2056 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2058 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 2060 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2062 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2064 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2066 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2068 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter method */

/* 2070 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 2072 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 2074 */	NdrFcShort( 0x116 ),	/* Type Offset=278 */

	/* Return value */

/* 2076 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2078 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 2080 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure addPredefinedVariable */

/* 2082 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2084 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2088 */	NdrFcShort( 0x9 ),	/* 9 */
/* 2090 */	NdrFcShort( 0x28 ),	/* ia64 Stack size/offset = 40 */
/* 2092 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2094 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2096 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x4,		/* 4 */
/* 2098 */	0xa,		/* 10 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 2100 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2102 */	NdrFcShort( 0x3 ),	/* 3 */
/* 2104 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2106 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter type */

/* 2108 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 2110 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 2112 */	NdrFcShort( 0xa4 ),	/* Type Offset=164 */

	/* Parameter name */

/* 2114 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 2116 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 2118 */	NdrFcShort( 0xa4 ),	/* Type Offset=164 */

	/* Parameter value */

/* 2120 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 2122 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 2124 */	NdrFcShort( 0xa4 ),	/* Type Offset=164 */

	/* Return value */

/* 2126 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2128 */	NdrFcShort( 0x20 ),	/* ia64 Stack size/offset = 32 */
/* 2130 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure getAuthor */


	/* Procedure getDescription */

/* 2132 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2134 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2138 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2140 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 2142 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2144 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2146 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 2148 */	0xa,		/* 10 */
			0x3,		/* Ext Flags:  new corr desc, clt corr check, */
/* 2150 */	NdrFcShort( 0x1 ),	/* 1 */
/* 2152 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2154 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2156 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter author */


	/* Parameter desc */

/* 2158 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
/* 2160 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 2162 */	NdrFcShort( 0x20 ),	/* Type Offset=32 */

	/* Return value */


	/* Return value */

/* 2164 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2166 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 2168 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure getMethods */

/* 2170 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2172 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2176 */	NdrFcShort( 0xa ),	/* 10 */
/* 2178 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 2180 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2182 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2184 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 2186 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2188 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2190 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2192 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2194 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter list */

/* 2196 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 2198 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 2200 */	NdrFcShort( 0x12c ),	/* Type Offset=300 */

	/* Return value */

/* 2202 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2204 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 2206 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure createObject */

/* 2208 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2210 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2214 */	NdrFcShort( 0xb ),	/* 11 */
/* 2216 */	NdrFcShort( 0x28 ),	/* ia64 Stack size/offset = 40 */
/* 2218 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2220 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2222 */	0x47,		/* Oi2 Flags:  srv must size, clt must size, has return, has ext, */
			0x4,		/* 4 */
/* 2224 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2226 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2228 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2230 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2232 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter newIndex */

/* 2234 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2236 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 2238 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter arguments */

/* 2240 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 2242 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 2244 */	NdrFcShort( 0xd8 ),	/* Type Offset=216 */

	/* Parameter object */

/* 2246 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 2248 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 2250 */	NdrFcShort( 0x100 ),	/* Type Offset=256 */

	/* Return value */

/* 2252 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2254 */	NdrFcShort( 0x20 ),	/* ia64 Stack size/offset = 32 */
/* 2256 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure getLibrary */

/* 2258 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2260 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2264 */	NdrFcShort( 0xc ),	/* 12 */
/* 2266 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 2268 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2270 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2272 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 2274 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2276 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2278 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2280 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2282 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter lib */

/* 2284 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 2286 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 2288 */	NdrFcShort( 0x13e ),	/* Type Offset=318 */

	/* Return value */

/* 2290 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2292 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 2294 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure getLastError */

/* 2296 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2298 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2302 */	NdrFcShort( 0xd ),	/* 13 */
/* 2304 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 2306 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2308 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2310 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 2312 */	0xa,		/* 10 */
			0x3,		/* Ext Flags:  new corr desc, clt corr check, */
/* 2314 */	NdrFcShort( 0x1 ),	/* 1 */
/* 2316 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2318 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2320 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter message */

/* 2322 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
/* 2324 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 2326 */	NdrFcShort( 0x20 ),	/* Type Offset=32 */

	/* Return value */

/* 2328 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2330 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 2332 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure getNamespaceName */

/* 2334 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2336 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2340 */	NdrFcShort( 0xc ),	/* 12 */
/* 2342 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 2344 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2346 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2348 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 2350 */	0xa,		/* 10 */
			0x3,		/* Ext Flags:  new corr desc, clt corr check, */
/* 2352 */	NdrFcShort( 0x1 ),	/* 1 */
/* 2354 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2356 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2358 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter ns */

/* 2360 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
/* 2362 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 2364 */	NdrFcShort( 0x20 ),	/* Type Offset=32 */

	/* Return value */

/* 2366 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2368 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 2370 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure getTypeCount */

/* 2372 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2374 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2378 */	NdrFcShort( 0xd ),	/* 13 */
/* 2380 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 2382 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2384 */	NdrFcShort( 0x24 ),	/* 36 */
/* 2386 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 2388 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2390 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2392 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2394 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2396 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter count */

/* 2398 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 2400 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 2402 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 2404 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2406 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 2408 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure getType */

/* 2410 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2412 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2416 */	NdrFcShort( 0xe ),	/* 14 */
/* 2418 */	NdrFcShort( 0x20 ),	/* ia64 Stack size/offset = 32 */
/* 2420 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2422 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2424 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x3,		/* 3 */
/* 2426 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2428 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2430 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2432 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2434 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter index */

/* 2436 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2438 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 2440 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter type */

/* 2442 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 2444 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 2446 */	NdrFcShort( 0x154 ),	/* Type Offset=340 */

	/* Return value */

/* 2448 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2450 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 2452 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure getLastError */

/* 2454 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2456 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2460 */	NdrFcShort( 0xf ),	/* 15 */
/* 2462 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 2464 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2466 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2468 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 2470 */	0xa,		/* 10 */
			0x3,		/* Ext Flags:  new corr desc, clt corr check, */
/* 2472 */	NdrFcShort( 0x1 ),	/* 1 */
/* 2474 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2476 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2478 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter message */

/* 2480 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
/* 2482 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 2484 */	NdrFcShort( 0x20 ),	/* Type Offset=32 */

	/* Return value */

/* 2486 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2488 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 2490 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

			0x0
        }
    };

static const MIDL_TYPE_FORMAT_STRING __MIDL_TypeFormatString =
    {
        0,
        {
			NdrFcShort( 0x0 ),	/* 0 */
/*  2 */	
			0x11, 0x4,	/* FC_RP [alloced_on_stack] */
/*  4 */	NdrFcShort( 0x1c ),	/* Offset= 28 (32) */
/*  6 */	
			0x13, 0x0,	/* FC_OP */
/*  8 */	NdrFcShort( 0xe ),	/* Offset= 14 (22) */
/* 10 */	
			0x1b,		/* FC_CARRAY */
			0x1,		/* 1 */
/* 12 */	NdrFcShort( 0x2 ),	/* 2 */
/* 14 */	0x9,		/* Corr desc: FC_ULONG */
			0x0,		/*  */
/* 16 */	NdrFcShort( 0xfffc ),	/* -4 */
/* 18 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 20 */	0x6,		/* FC_SHORT */
			0x5b,		/* FC_END */
/* 22 */	
			0x17,		/* FC_CSTRUCT */
			0x3,		/* 3 */
/* 24 */	NdrFcShort( 0x8 ),	/* 8 */
/* 26 */	NdrFcShort( 0xfff0 ),	/* Offset= -16 (10) */
/* 28 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 30 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 32 */	0xb4,		/* FC_USER_MARSHAL */
			0x83,		/* 131 */
/* 34 */	NdrFcShort( 0x0 ),	/* 0 */
/* 36 */	NdrFcShort( 0x8 ),	/* 8 */
/* 38 */	NdrFcShort( 0x0 ),	/* 0 */
/* 40 */	NdrFcShort( 0xffde ),	/* Offset= -34 (6) */
/* 42 */	
			0x11, 0xc,	/* FC_RP [alloced_on_stack] [simple_pointer] */
/* 44 */	0x8,		/* FC_LONG */
			0x5c,		/* FC_PAD */
/* 46 */	
			0x11, 0x14,	/* FC_RP [alloced_on_stack] [pointer_deref] */
/* 48 */	NdrFcShort( 0x2 ),	/* Offset= 2 (50) */
/* 50 */	
			0x13, 0x8,	/* FC_OP [simple_pointer] */
/* 52 */	0x1,		/* FC_BYTE */
			0x5c,		/* FC_PAD */
/* 54 */	
			0x11, 0x4,	/* FC_RP [alloced_on_stack] */
/* 56 */	NdrFcShort( 0x2 ),	/* Offset= 2 (58) */
/* 58 */	
			0x15,		/* FC_STRUCT */
			0x3,		/* 3 */
/* 60 */	NdrFcShort( 0x10 ),	/* 16 */
/* 62 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 64 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 66 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 68 */	
			0x11, 0x10,	/* FC_RP [pointer_deref] */
/* 70 */	NdrFcShort( 0x2 ),	/* Offset= 2 (72) */
/* 72 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 74 */	NdrFcLong( 0xe0934aaf ),	/* -527218001 */
/* 78 */	NdrFcShort( 0x8832 ),	/* -30670 */
/* 80 */	NdrFcShort( 0x44cf ),	/* 17615 */
/* 82 */	0xa4,		/* 164 */
			0x90,		/* 144 */
/* 84 */	0x6,		/* 6 */
			0xf9,		/* 249 */
/* 86 */	0x17,		/* 23 */
			0x2f,		/* 47 */
/* 88 */	0xc7,		/* 199 */
			0xbb,		/* 187 */
/* 90 */	
			0x11, 0x8,	/* FC_RP [simple_pointer] */
/* 92 */	0x1,		/* FC_BYTE */
			0x5c,		/* FC_PAD */
/* 94 */	
			0x11, 0x4,	/* FC_RP [alloced_on_stack] */
/* 96 */	NdrFcShort( 0x2 ),	/* Offset= 2 (98) */
/* 98 */	
			0x15,		/* FC_STRUCT */
			0x3,		/* 3 */
/* 100 */	NdrFcShort( 0x20 ),	/* 32 */
/* 102 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 104 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 106 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 108 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 110 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 112 */	
			0x11, 0x10,	/* FC_RP [pointer_deref] */
/* 114 */	NdrFcShort( 0x2 ),	/* Offset= 2 (116) */
/* 116 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 118 */	NdrFcLong( 0x44e35511 ),	/* 1155749137 */
/* 122 */	NdrFcShort( 0xbc78 ),	/* -17288 */
/* 124 */	NdrFcShort( 0x40a5 ),	/* 16549 */
/* 126 */	0xb7,		/* 183 */
			0x13,		/* 19 */
/* 128 */	0xe,		/* 14 */
			0x90,		/* 144 */
/* 130 */	0x77,		/* 119 */
			0x93,		/* 147 */
/* 132 */	0x3e,		/* 62 */
			0x8d,		/* 141 */
/* 134 */	
			0x11, 0xc,	/* FC_RP [alloced_on_stack] [simple_pointer] */
/* 136 */	0xc,		/* FC_DOUBLE */
			0x5c,		/* FC_PAD */
/* 138 */	
			0x11, 0x10,	/* FC_RP [pointer_deref] */
/* 140 */	NdrFcShort( 0x2 ),	/* Offset= 2 (142) */
/* 142 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 144 */	NdrFcLong( 0xdd64d836 ),	/* -580593610 */
/* 148 */	NdrFcShort( 0xf3df ),	/* -3105 */
/* 150 */	NdrFcShort( 0x4097 ),	/* 16535 */
/* 152 */	0xa1,		/* 161 */
			0x2a,		/* 42 */
/* 154 */	0x45,		/* 69 */
			0xef,		/* 239 */
/* 156 */	0x44,		/* 68 */
			0x1b,		/* 27 */
/* 158 */	0x2f,		/* 47 */
			0xd0,		/* 208 */
/* 160 */	
			0x12, 0x0,	/* FC_UP */
/* 162 */	NdrFcShort( 0xff74 ),	/* Offset= -140 (22) */
/* 164 */	0xb4,		/* FC_USER_MARSHAL */
			0x83,		/* 131 */
/* 166 */	NdrFcShort( 0x0 ),	/* 0 */
/* 168 */	NdrFcShort( 0x8 ),	/* 8 */
/* 170 */	NdrFcShort( 0x0 ),	/* 0 */
/* 172 */	NdrFcShort( 0xfff4 ),	/* Offset= -12 (160) */
/* 174 */	
			0x11, 0x14,	/* FC_RP [alloced_on_stack] [pointer_deref] */
/* 176 */	NdrFcShort( 0x2 ),	/* Offset= 2 (178) */
/* 178 */	
			0x13, 0x8,	/* FC_OP [simple_pointer] */
/* 180 */	0x8,		/* FC_LONG */
			0x5c,		/* FC_PAD */
/* 182 */	
			0x11, 0x14,	/* FC_RP [alloced_on_stack] [pointer_deref] */
/* 184 */	NdrFcShort( 0x2 ),	/* Offset= 2 (186) */
/* 186 */	
			0x13, 0x8,	/* FC_OP [simple_pointer] */
/* 188 */	0xc,		/* FC_DOUBLE */
			0x5c,		/* FC_PAD */
/* 190 */	
			0x11, 0xc,	/* FC_RP [alloced_on_stack] [simple_pointer] */
/* 192 */	0x1,		/* FC_BYTE */
			0x5c,		/* FC_PAD */
/* 194 */	
			0x11, 0x10,	/* FC_RP [pointer_deref] */
/* 196 */	NdrFcShort( 0x2 ),	/* Offset= 2 (198) */
/* 198 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 200 */	NdrFcLong( 0xd43bb8bb ),	/* -734283589 */
/* 204 */	NdrFcShort( 0x2ba3 ),	/* 11171 */
/* 206 */	NdrFcShort( 0x492e ),	/* 18734 */
/* 208 */	0xb3,		/* 179 */
			0xab,		/* 171 */
/* 210 */	0x15,		/* 21 */
			0x0,		/* 0 */
/* 212 */	0x71,		/* 113 */
			0xb8,		/* 184 */
/* 214 */	0x79,		/* 121 */
			0xa9,		/* 169 */
/* 216 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 218 */	NdrFcLong( 0xb8d5ba56 ),	/* -1193952682 */
/* 222 */	NdrFcShort( 0x1d5 ),	/* 469 */
/* 224 */	NdrFcShort( 0x4237 ),	/* 16951 */
/* 226 */	0x92,		/* 146 */
			0xa1,		/* 161 */
/* 228 */	0x73,		/* 115 */
			0xc9,		/* 201 */
/* 230 */	0x72,		/* 114 */
			0xc8,		/* 200 */
/* 232 */	0x47,		/* 71 */
			0xb,		/* 11 */
/* 234 */	
			0x11, 0x10,	/* FC_RP [pointer_deref] */
/* 236 */	NdrFcShort( 0x2 ),	/* Offset= 2 (238) */
/* 238 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 240 */	NdrFcLong( 0x12349daf ),	/* 305438127 */
/* 244 */	NdrFcShort( 0xfd20 ),	/* -736 */
/* 246 */	NdrFcShort( 0x4ba6 ),	/* 19366 */
/* 248 */	0xb5,		/* 181 */
			0x0,		/* 0 */
/* 250 */	0xf,		/* 15 */
			0xc8,		/* 200 */
/* 252 */	0xe0,		/* 224 */
			0xe4,		/* 228 */
/* 254 */	0x9a,		/* 154 */
			0xb,		/* 11 */
/* 256 */	
			0x11, 0x10,	/* FC_RP [pointer_deref] */
/* 258 */	NdrFcShort( 0x2 ),	/* Offset= 2 (260) */
/* 260 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 262 */	NdrFcLong( 0x83ce0178 ),	/* -2083651208 */
/* 266 */	NdrFcShort( 0xfae2 ),	/* -1310 */
/* 268 */	NdrFcShort( 0x4734 ),	/* 18228 */
/* 270 */	0x8a,		/* 138 */
			0x90,		/* 144 */
/* 272 */	0x31,		/* 49 */
			0x38,		/* 56 */
/* 274 */	0x95,		/* 149 */
			0x43,		/* 67 */
/* 276 */	0xe4,		/* 228 */
			0xae,		/* 174 */
/* 278 */	
			0x11, 0x10,	/* FC_RP [pointer_deref] */
/* 280 */	NdrFcShort( 0x2 ),	/* Offset= 2 (282) */
/* 282 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 284 */	NdrFcLong( 0xb74b2585 ),	/* -1219811963 */
/* 288 */	NdrFcShort( 0x4c45 ),	/* 19525 */
/* 290 */	NdrFcShort( 0x4747 ),	/* 18247 */
/* 292 */	0xa2,		/* 162 */
			0x97,		/* 151 */
/* 294 */	0x9e,		/* 158 */
			0xd8,		/* 216 */
/* 296 */	0x41,		/* 65 */
			0xf4,		/* 244 */
/* 298 */	0x73,		/* 115 */
			0x1d,		/* 29 */
/* 300 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 302 */	NdrFcLong( 0x721705e4 ),	/* 1914111460 */
/* 306 */	NdrFcShort( 0x275f ),	/* 10079 */
/* 308 */	NdrFcShort( 0x4aed ),	/* 19181 */
/* 310 */	0xbf,		/* 191 */
			0xa9,		/* 169 */
/* 312 */	0x69,		/* 105 */
			0x57,		/* 87 */
/* 314 */	0xe4,		/* 228 */
			0xb,		/* 11 */
/* 316 */	0xde,		/* 222 */
			0x4f,		/* 79 */
/* 318 */	
			0x11, 0x10,	/* FC_RP [pointer_deref] */
/* 320 */	NdrFcShort( 0x2 ),	/* Offset= 2 (322) */
/* 322 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 324 */	NdrFcLong( 0x5483302b ),	/* 1417883691 */
/* 328 */	NdrFcShort( 0x931f ),	/* -27873 */
/* 330 */	NdrFcShort( 0x4506 ),	/* 17670 */
/* 332 */	0xa9,		/* 169 */
			0xd0,		/* 208 */
/* 334 */	0x30,		/* 48 */
			0xf5,		/* 245 */
/* 336 */	0xc2,		/* 194 */
			0x23,		/* 35 */
/* 338 */	0x48,		/* 72 */
			0xcb,		/* 203 */
/* 340 */	
			0x11, 0x10,	/* FC_RP [pointer_deref] */
/* 342 */	NdrFcShort( 0x2 ),	/* Offset= 2 (344) */
/* 344 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 346 */	NdrFcLong( 0x12349daf ),	/* 305438127 */
/* 350 */	NdrFcShort( 0xfd20 ),	/* -736 */
/* 352 */	NdrFcShort( 0x4ba6 ),	/* 19366 */
/* 354 */	0xb5,		/* 181 */
			0x0,		/* 0 */
/* 356 */	0xf,		/* 15 */
			0xc8,		/* 200 */
/* 358 */	0xe0,		/* 224 */
			0xe4,		/* 228 */
/* 360 */	0x9a,		/* 154 */
			0xb,		/* 11 */

			0x0
        }
    };

static const USER_MARSHAL_ROUTINE_QUADRUPLE UserMarshalRoutines[ WIRE_MARSHAL_TABLE_SIZE ] = 
        {
            
            {
            BSTR_UserSize
            ,BSTR_UserMarshal
            ,BSTR_UserUnmarshal
            ,BSTR_UserFree
            }

        };



/* Object interface: IUnknown, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}} */


/* Object interface: IDispatch, ver. 0.0,
   GUID={0x00020400,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}} */


/* Object interface: IPCLArgument, ver. 0.0,
   GUID={0xD43BB8BB,0x2BA3,0x492e,{0xB3,0xAB,0x15,0x00,0x71,0xB8,0x79,0xA9}} */

#pragma code_seg(".orpc")
static const unsigned short IPCLArgument_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    0,
    38,
    76,
    114
    };

static const MIDL_STUBLESS_PROXY_INFO IPCLArgument_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &IPCLArgument_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO IPCLArgument_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &IPCLArgument_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(11) _IPCLArgumentProxyVtbl = 
{
    &IPCLArgument_ProxyInfo,
    &IID_IPCLArgument,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *) (INT_PTR) -1 /* IPCLArgument::getType */ ,
    (void *) (INT_PTR) -1 /* IPCLArgument::getRank */ ,
    (void *) (INT_PTR) -1 /* IPCLArgument::getIsReference */ ,
    (void *) (INT_PTR) -1 /* IPCLArgument::getLastError */
};


static const PRPC_STUB_FUNCTION IPCLArgument_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _IPCLArgumentStubVtbl =
{
    &IID_IPCLArgument,
    &IPCLArgument_ServerInfo,
    11,
    &IPCLArgument_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Standard interface: __MIDL_itf_PCLExtension_0116, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */


/* Object interface: IPCLWavefileLock, ver. 0.0,
   GUID={0xE0934AAF,0x8832,0x44cf,{0xA4,0x90,0x06,0xF9,0x17,0x2F,0xC7,0xBB}} */

#pragma code_seg(".orpc")
static const unsigned short IPCLWavefileLock_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    152,
    190,
    228
    };

static const MIDL_STUBLESS_PROXY_INFO IPCLWavefileLock_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &IPCLWavefileLock_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO IPCLWavefileLock_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &IPCLWavefileLock_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(10) _IPCLWavefileLockProxyVtbl = 
{
    &IPCLWavefileLock_ProxyInfo,
    &IID_IPCLWavefileLock,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *) (INT_PTR) -1 /* IPCLWavefileLock::getDataPtr */ ,
    (void *) (INT_PTR) -1 /* IPCLWavefileLock::getParameters */ ,
    (void *) (INT_PTR) -1 /* IPCLWavefileLock::getLastError */
};


static const PRPC_STUB_FUNCTION IPCLWavefileLock_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _IPCLWavefileLockStubVtbl =
{
    &IID_IPCLWavefileLock,
    &IPCLWavefileLock_ServerInfo,
    10,
    &IPCLWavefileLock_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Object interface: IPCLWavefile, ver. 0.0,
   GUID={0xA60D18A4,0x8C3F,0x44f0,{0x97,0xFA,0xB3,0x99,0xE5,0x6F,0xFD,0xC2}} */

#pragma code_seg(".orpc")
static const unsigned short IPCLWavefile_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    0,
    38,
    76,
    114,
    266,
    304,
    342,
    386,
    424
    };

static const MIDL_STUBLESS_PROXY_INFO IPCLWavefile_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &IPCLWavefile_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO IPCLWavefile_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &IPCLWavefile_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(16) _IPCLWavefileProxyVtbl = 
{
    &IPCLWavefile_ProxyInfo,
    &IID_IPCLWavefile,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *) (INT_PTR) -1 /* IPCLArgument::getType */ ,
    (void *) (INT_PTR) -1 /* IPCLArgument::getRank */ ,
    (void *) (INT_PTR) -1 /* IPCLArgument::getIsReference */ ,
    (void *) (INT_PTR) -1 /* IPCLArgument::getLastError */ ,
    (void *) (INT_PTR) -1 /* IPCLWavefile::getIsNull */ ,
    (void *) (INT_PTR) -1 /* IPCLWavefile::getParameters */ ,
    (void *) (INT_PTR) -1 /* IPCLWavefile::resize */ ,
    (void *) (INT_PTR) -1 /* IPCLWavefile::lock */ ,
    (void *) (INT_PTR) -1 /* IPCLWavefile::replace */
};


static const PRPC_STUB_FUNCTION IPCLWavefile_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _IPCLWavefileStubVtbl =
{
    &IID_IPCLWavefile,
    &IPCLWavefile_ServerInfo,
    16,
    &IPCLWavefile_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Standard interface: __MIDL_itf_PCLExtension_0118, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */


/* Object interface: IPCLGraphicSurfaceLock, ver. 0.0,
   GUID={0x44E35511,0xBC78,0x40a5,{0xB7,0x13,0x0E,0x90,0x77,0x93,0x3E,0x8D}} */

#pragma code_seg(".orpc")
static const unsigned short IPCLGraphicSurfaceLock_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    152,
    468,
    228
    };

static const MIDL_STUBLESS_PROXY_INFO IPCLGraphicSurfaceLock_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &IPCLGraphicSurfaceLock_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO IPCLGraphicSurfaceLock_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &IPCLGraphicSurfaceLock_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(10) _IPCLGraphicSurfaceLockProxyVtbl = 
{
    &IPCLGraphicSurfaceLock_ProxyInfo,
    &IID_IPCLGraphicSurfaceLock,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *) (INT_PTR) -1 /* IPCLGraphicSurfaceLock::getDataPtr */ ,
    (void *) (INT_PTR) -1 /* IPCLGraphicSurfaceLock::getParameters */ ,
    (void *) (INT_PTR) -1 /* IPCLGraphicSurfaceLock::getLastError */
};


static const PRPC_STUB_FUNCTION IPCLGraphicSurfaceLock_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _IPCLGraphicSurfaceLockStubVtbl =
{
    &IID_IPCLGraphicSurfaceLock,
    &IPCLGraphicSurfaceLock_ServerInfo,
    10,
    &IPCLGraphicSurfaceLock_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Object interface: IPCLGraphicSurface, ver. 0.0,
   GUID={0xA96615CA,0x28B1,0x4958,{0x93,0x29,0x7F,0x1A,0x45,0x36,0x10,0xB7}} */

#pragma code_seg(".orpc")
static const unsigned short IPCLGraphicSurface_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    0,
    38,
    76,
    114,
    266,
    506,
    342,
    550,
    588,
    632
    };

static const MIDL_STUBLESS_PROXY_INFO IPCLGraphicSurface_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &IPCLGraphicSurface_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO IPCLGraphicSurface_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &IPCLGraphicSurface_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(17) _IPCLGraphicSurfaceProxyVtbl = 
{
    &IPCLGraphicSurface_ProxyInfo,
    &IID_IPCLGraphicSurface,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *) (INT_PTR) -1 /* IPCLArgument::getType */ ,
    (void *) (INT_PTR) -1 /* IPCLArgument::getRank */ ,
    (void *) (INT_PTR) -1 /* IPCLArgument::getIsReference */ ,
    (void *) (INT_PTR) -1 /* IPCLArgument::getLastError */ ,
    (void *) (INT_PTR) -1 /* IPCLGraphicSurface::getIsNull */ ,
    (void *) (INT_PTR) -1 /* IPCLGraphicSurface::getSize */ ,
    (void *) (INT_PTR) -1 /* IPCLGraphicSurface::resize */ ,
    (void *) (INT_PTR) -1 /* IPCLGraphicSurface::lock */ ,
    (void *) (INT_PTR) -1 /* IPCLGraphicSurface::replace */ ,
    (void *) (INT_PTR) -1 /* IPCLGraphicSurface::convertCustomUnits */
};


static const PRPC_STUB_FUNCTION IPCLGraphicSurface_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _IPCLGraphicSurfaceStubVtbl =
{
    &IID_IPCLGraphicSurface,
    &IPCLGraphicSurface_ServerInfo,
    17,
    &IPCLGraphicSurface_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Object interface: IPCLArray, ver. 0.0,
   GUID={0xDD64D836,0xF3DF,0x4097,{0xA1,0x2A,0x45,0xEF,0x44,0x1B,0x2F,0xD0}} */

#pragma code_seg(".orpc")
static const unsigned short IPCLArray_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    0,
    38,
    76,
    114,
    266,
    688,
    726
    };

static const MIDL_STUBLESS_PROXY_INFO IPCLArray_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &IPCLArray_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO IPCLArray_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &IPCLArray_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(14) _IPCLArrayProxyVtbl = 
{
    &IPCLArray_ProxyInfo,
    &IID_IPCLArray,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *) (INT_PTR) -1 /* IPCLArgument::getType */ ,
    (void *) (INT_PTR) -1 /* IPCLArgument::getRank */ ,
    (void *) (INT_PTR) -1 /* IPCLArgument::getIsReference */ ,
    (void *) (INT_PTR) -1 /* IPCLArgument::getLastError */ ,
    (void *) (INT_PTR) -1 /* IPCLArray::getCount */ ,
    (void *) (INT_PTR) -1 /* IPCLArray::resize */ ,
    (void *) (INT_PTR) -1 /* IPCLArray::getSubArray */
};


static const PRPC_STUB_FUNCTION IPCLArray_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _IPCLArrayStubVtbl =
{
    &IID_IPCLArray,
    &IPCLArray_ServerInfo,
    14,
    &IPCLArray_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Object interface: IPCLStringArray, ver. 0.0,
   GUID={0xCEB1D686,0xB7EC,0x44bc,{0x8F,0xDA,0x87,0x0C,0x50,0x88,0x40,0x88}} */

#pragma code_seg(".orpc")
static const unsigned short IPCLStringArray_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    0,
    38,
    76,
    114,
    266,
    688,
    726,
    770,
    814,
    858
    };

static const MIDL_STUBLESS_PROXY_INFO IPCLStringArray_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &IPCLStringArray_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO IPCLStringArray_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &IPCLStringArray_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(17) _IPCLStringArrayProxyVtbl = 
{
    &IPCLStringArray_ProxyInfo,
    &IID_IPCLStringArray,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *) (INT_PTR) -1 /* IPCLArgument::getType */ ,
    (void *) (INT_PTR) -1 /* IPCLArgument::getRank */ ,
    (void *) (INT_PTR) -1 /* IPCLArgument::getIsReference */ ,
    (void *) (INT_PTR) -1 /* IPCLArgument::getLastError */ ,
    (void *) (INT_PTR) -1 /* IPCLArray::getCount */ ,
    (void *) (INT_PTR) -1 /* IPCLArray::resize */ ,
    (void *) (INT_PTR) -1 /* IPCLArray::getSubArray */ ,
    (void *) (INT_PTR) -1 /* IPCLStringArray::setValue */ ,
    (void *) (INT_PTR) -1 /* IPCLStringArray::getValue */ ,
    (void *) (INT_PTR) -1 /* IPCLStringArray::add */
};


static const PRPC_STUB_FUNCTION IPCLStringArray_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _IPCLStringArrayStubVtbl =
{
    &IID_IPCLStringArray,
    &IPCLStringArray_ServerInfo,
    17,
    &IPCLStringArray_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Object interface: IPCLIntArray, ver. 0.0,
   GUID={0x6602A29A,0x38D8,0x4c3e,{0xA9,0x80,0xF0,0x4A,0x0D,0xD6,0x48,0x95}} */

#pragma code_seg(".orpc")
static const unsigned short IPCLIntArray_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    0,
    38,
    76,
    114,
    266,
    688,
    726,
    896,
    934,
    978,
    1022
    };

static const MIDL_STUBLESS_PROXY_INFO IPCLIntArray_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &IPCLIntArray_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO IPCLIntArray_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &IPCLIntArray_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(18) _IPCLIntArrayProxyVtbl = 
{
    &IPCLIntArray_ProxyInfo,
    &IID_IPCLIntArray,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *) (INT_PTR) -1 /* IPCLArgument::getType */ ,
    (void *) (INT_PTR) -1 /* IPCLArgument::getRank */ ,
    (void *) (INT_PTR) -1 /* IPCLArgument::getIsReference */ ,
    (void *) (INT_PTR) -1 /* IPCLArgument::getLastError */ ,
    (void *) (INT_PTR) -1 /* IPCLArray::getCount */ ,
    (void *) (INT_PTR) -1 /* IPCLArray::resize */ ,
    (void *) (INT_PTR) -1 /* IPCLArray::getSubArray */ ,
    (void *) (INT_PTR) -1 /* IPCLIntArray::getDataPtr */ ,
    (void *) (INT_PTR) -1 /* IPCLIntArray::setValue */ ,
    (void *) (INT_PTR) -1 /* IPCLIntArray::getValue */ ,
    (void *) (INT_PTR) -1 /* IPCLIntArray::add */
};


static const PRPC_STUB_FUNCTION IPCLIntArray_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _IPCLIntArrayStubVtbl =
{
    &IID_IPCLIntArray,
    &IPCLIntArray_ServerInfo,
    18,
    &IPCLIntArray_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Object interface: IPCLDoubleArray, ver. 0.0,
   GUID={0xCA5BDA29,0xB722,0x4518,{0xAF,0x47,0x26,0xC9,0xF8,0x62,0x87,0x52}} */

#pragma code_seg(".orpc")
static const unsigned short IPCLDoubleArray_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    0,
    38,
    76,
    114,
    266,
    688,
    726,
    1060,
    1098,
    1142,
    1186
    };

static const MIDL_STUBLESS_PROXY_INFO IPCLDoubleArray_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &IPCLDoubleArray_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO IPCLDoubleArray_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &IPCLDoubleArray_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(18) _IPCLDoubleArrayProxyVtbl = 
{
    &IPCLDoubleArray_ProxyInfo,
    &IID_IPCLDoubleArray,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *) (INT_PTR) -1 /* IPCLArgument::getType */ ,
    (void *) (INT_PTR) -1 /* IPCLArgument::getRank */ ,
    (void *) (INT_PTR) -1 /* IPCLArgument::getIsReference */ ,
    (void *) (INT_PTR) -1 /* IPCLArgument::getLastError */ ,
    (void *) (INT_PTR) -1 /* IPCLArray::getCount */ ,
    (void *) (INT_PTR) -1 /* IPCLArray::resize */ ,
    (void *) (INT_PTR) -1 /* IPCLArray::getSubArray */ ,
    (void *) (INT_PTR) -1 /* IPCLDoubleArray::getDataPtr */ ,
    (void *) (INT_PTR) -1 /* IPCLDoubleArray::setValue */ ,
    (void *) (INT_PTR) -1 /* IPCLDoubleArray::getValue */ ,
    (void *) (INT_PTR) -1 /* IPCLDoubleArray::add */
};


static const PRPC_STUB_FUNCTION IPCLDoubleArray_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _IPCLDoubleArrayStubVtbl =
{
    &IID_IPCLDoubleArray,
    &IPCLDoubleArray_ServerInfo,
    18,
    &IPCLDoubleArray_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Object interface: IPCLBoolArray, ver. 0.0,
   GUID={0xFF2B4A6F,0x7F73,0x41b4,{0xA6,0x96,0xF2,0x8C,0x1F,0x31,0x47,0xED}} */

#pragma code_seg(".orpc")
static const unsigned short IPCLBoolArray_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    0,
    38,
    76,
    114,
    266,
    688,
    726,
    1224,
    1262,
    1306,
    1350
    };

static const MIDL_STUBLESS_PROXY_INFO IPCLBoolArray_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &IPCLBoolArray_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO IPCLBoolArray_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &IPCLBoolArray_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(18) _IPCLBoolArrayProxyVtbl = 
{
    &IPCLBoolArray_ProxyInfo,
    &IID_IPCLBoolArray,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *) (INT_PTR) -1 /* IPCLArgument::getType */ ,
    (void *) (INT_PTR) -1 /* IPCLArgument::getRank */ ,
    (void *) (INT_PTR) -1 /* IPCLArgument::getIsReference */ ,
    (void *) (INT_PTR) -1 /* IPCLArgument::getLastError */ ,
    (void *) (INT_PTR) -1 /* IPCLArray::getCount */ ,
    (void *) (INT_PTR) -1 /* IPCLArray::resize */ ,
    (void *) (INT_PTR) -1 /* IPCLArray::getSubArray */ ,
    (void *) (INT_PTR) -1 /* IPCLBoolArray::getDataPtr */ ,
    (void *) (INT_PTR) -1 /* IPCLBoolArray::setValue */ ,
    (void *) (INT_PTR) -1 /* IPCLBoolArray::getValue */ ,
    (void *) (INT_PTR) -1 /* IPCLBoolArray::add */
};


static const PRPC_STUB_FUNCTION IPCLBoolArray_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _IPCLBoolArrayStubVtbl =
{
    &IID_IPCLBoolArray,
    &IPCLBoolArray_ServerInfo,
    18,
    &IPCLBoolArray_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Object interface: IPCLString, ver. 0.0,
   GUID={0x023D3759,0x685A,0x4c63,{0x85,0xB3,0x9A,0x50,0xD5,0x45,0xE5,0x65}} */

#pragma code_seg(".orpc")
static const unsigned short IPCLString_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    0,
    38,
    76,
    114,
    1388,
    1426
    };

static const MIDL_STUBLESS_PROXY_INFO IPCLString_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &IPCLString_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO IPCLString_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &IPCLString_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(13) _IPCLStringProxyVtbl = 
{
    &IPCLString_ProxyInfo,
    &IID_IPCLString,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *) (INT_PTR) -1 /* IPCLArgument::getType */ ,
    (void *) (INT_PTR) -1 /* IPCLArgument::getRank */ ,
    (void *) (INT_PTR) -1 /* IPCLArgument::getIsReference */ ,
    (void *) (INT_PTR) -1 /* IPCLArgument::getLastError */ ,
    (void *) (INT_PTR) -1 /* IPCLString::getValue */ ,
    (void *) (INT_PTR) -1 /* IPCLString::setValue */
};


static const PRPC_STUB_FUNCTION IPCLString_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _IPCLStringStubVtbl =
{
    &IID_IPCLString,
    &IPCLString_ServerInfo,
    13,
    &IPCLString_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Object interface: IPCLInt, ver. 0.0,
   GUID={0xCF00C2BD,0x1595,0x4c6d,{0xB2,0xC9,0x8E,0xB8,0x8B,0x79,0xA0,0x78}} */

#pragma code_seg(".orpc")
static const unsigned short IPCLInt_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    0,
    38,
    76,
    114,
    266,
    688
    };

static const MIDL_STUBLESS_PROXY_INFO IPCLInt_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &IPCLInt_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO IPCLInt_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &IPCLInt_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(13) _IPCLIntProxyVtbl = 
{
    &IPCLInt_ProxyInfo,
    &IID_IPCLInt,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *) (INT_PTR) -1 /* IPCLArgument::getType */ ,
    (void *) (INT_PTR) -1 /* IPCLArgument::getRank */ ,
    (void *) (INT_PTR) -1 /* IPCLArgument::getIsReference */ ,
    (void *) (INT_PTR) -1 /* IPCLArgument::getLastError */ ,
    (void *) (INT_PTR) -1 /* IPCLInt::getValue */ ,
    (void *) (INT_PTR) -1 /* IPCLInt::setValue */
};


static const PRPC_STUB_FUNCTION IPCLInt_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _IPCLIntStubVtbl =
{
    &IID_IPCLInt,
    &IPCLInt_ServerInfo,
    13,
    &IPCLInt_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Object interface: IPCLDouble, ver. 0.0,
   GUID={0xC41A6728,0x6307,0x4846,{0x88,0x3C,0x0C,0x8B,0xF4,0xAA,0xCA,0x17}} */

#pragma code_seg(".orpc")
static const unsigned short IPCLDouble_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    0,
    38,
    76,
    114,
    1464,
    1502
    };

static const MIDL_STUBLESS_PROXY_INFO IPCLDouble_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &IPCLDouble_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO IPCLDouble_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &IPCLDouble_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(13) _IPCLDoubleProxyVtbl = 
{
    &IPCLDouble_ProxyInfo,
    &IID_IPCLDouble,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *) (INT_PTR) -1 /* IPCLArgument::getType */ ,
    (void *) (INT_PTR) -1 /* IPCLArgument::getRank */ ,
    (void *) (INT_PTR) -1 /* IPCLArgument::getIsReference */ ,
    (void *) (INT_PTR) -1 /* IPCLArgument::getLastError */ ,
    (void *) (INT_PTR) -1 /* IPCLDouble::getValue */ ,
    (void *) (INT_PTR) -1 /* IPCLDouble::setValue */
};


static const PRPC_STUB_FUNCTION IPCLDouble_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _IPCLDoubleStubVtbl =
{
    &IID_IPCLDouble,
    &IPCLDouble_ServerInfo,
    13,
    &IPCLDouble_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Object interface: IPCLBool, ver. 0.0,
   GUID={0x525A6207,0xB78E,0x47b1,{0xAB,0x8E,0x4D,0xED,0x01,0x7B,0x29,0x9D}} */

#pragma code_seg(".orpc")
static const unsigned short IPCLBool_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    0,
    38,
    76,
    114,
    266,
    688
    };

static const MIDL_STUBLESS_PROXY_INFO IPCLBool_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &IPCLBool_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO IPCLBool_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &IPCLBool_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(13) _IPCLBoolProxyVtbl = 
{
    &IPCLBool_ProxyInfo,
    &IID_IPCLBool,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *) (INT_PTR) -1 /* IPCLArgument::getType */ ,
    (void *) (INT_PTR) -1 /* IPCLArgument::getRank */ ,
    (void *) (INT_PTR) -1 /* IPCLArgument::getIsReference */ ,
    (void *) (INT_PTR) -1 /* IPCLArgument::getLastError */ ,
    (void *) (INT_PTR) -1 /* IPCLBool::getValue */ ,
    (void *) (INT_PTR) -1 /* IPCLBool::setValue */
};


static const PRPC_STUB_FUNCTION IPCLBool_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _IPCLBoolStubVtbl =
{
    &IID_IPCLBool,
    &IPCLBool_ServerInfo,
    13,
    &IPCLBool_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Object interface: IPCLArguments, ver. 0.0,
   GUID={0xB8D5BA56,0x01D5,0x4237,{0x92,0xA1,0x73,0xC9,0x72,0xC8,0x47,0x0B}} */

#pragma code_seg(".orpc")
static const unsigned short IPCLArguments_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    1540,
    1578,
    1622,
    114
    };

static const MIDL_STUBLESS_PROXY_INFO IPCLArguments_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &IPCLArguments_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO IPCLArguments_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &IPCLArguments_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(11) _IPCLArgumentsProxyVtbl = 
{
    &IPCLArguments_ProxyInfo,
    &IID_IPCLArguments,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *) (INT_PTR) -1 /* IPCLArguments::getCount */ ,
    (void *) (INT_PTR) -1 /* IPCLArguments::getArgument */ ,
    (void *) (INT_PTR) -1 /* IPCLArguments::getReturnValue */ ,
    (void *) (INT_PTR) -1 /* IPCLArguments::getLastError */
};


static const PRPC_STUB_FUNCTION IPCLArguments_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _IPCLArgumentsStubVtbl =
{
    &IID_IPCLArguments,
    &IPCLArguments_ServerInfo,
    11,
    &IPCLArguments_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Standard interface: __MIDL_itf_PCLExtension_0130, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */


/* Object interface: IPCLObject, ver. 0.0,
   GUID={0x83CE0178,0xFAE2,0x4734,{0x8A,0x90,0x31,0x38,0x95,0x43,0xE4,0xAE}} */

#pragma code_seg(".orpc")
static const unsigned short IPCLObject_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    1660,
    1704,
    228
    };

static const MIDL_STUBLESS_PROXY_INFO IPCLObject_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &IPCLObject_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO IPCLObject_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &IPCLObject_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(10) _IPCLObjectProxyVtbl = 
{
    &IPCLObject_ProxyInfo,
    &IID_IPCLObject,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *) (INT_PTR) -1 /* IPCLObject::callMethod */ ,
    (void *) (INT_PTR) -1 /* IPCLObject::getType */ ,
    (void *) (INT_PTR) -1 /* IPCLObject::getLastError */
};


static const PRPC_STUB_FUNCTION IPCLObject_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _IPCLObjectStubVtbl =
{
    &IID_IPCLObject,
    &IPCLObject_ServerInfo,
    10,
    &IPCLObject_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Object interface: IPCLExtensionObject, ver. 0.0,
   GUID={0x0DECE6D3,0x85AE,0x4d3c,{0x95,0x3A,0x15,0xA5,0x55,0x04,0xC9,0xE6}} */

#pragma code_seg(".orpc")
static const unsigned short IPCLExtensionObject_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    0,
    38,
    76,
    114,
    1742,
    1780
    };

static const MIDL_STUBLESS_PROXY_INFO IPCLExtensionObject_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &IPCLExtensionObject_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO IPCLExtensionObject_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &IPCLExtensionObject_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(13) _IPCLExtensionObjectProxyVtbl = 
{
    &IPCLExtensionObject_ProxyInfo,
    &IID_IPCLExtensionObject,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *) (INT_PTR) -1 /* IPCLArgument::getType */ ,
    (void *) (INT_PTR) -1 /* IPCLArgument::getRank */ ,
    (void *) (INT_PTR) -1 /* IPCLArgument::getIsReference */ ,
    (void *) (INT_PTR) -1 /* IPCLArgument::getLastError */ ,
    (void *) (INT_PTR) -1 /* IPCLExtensionObject::getValue */ ,
    (void *) (INT_PTR) -1 /* IPCLExtensionObject::setValue */
};


static const PRPC_STUB_FUNCTION IPCLExtensionObject_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _IPCLExtensionObjectStubVtbl =
{
    &IID_IPCLExtensionObject,
    &IPCLExtensionObject_ServerInfo,
    13,
    &IPCLExtensionObject_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Object interface: IPCLMethod, ver. 0.0,
   GUID={0xB74B2585,0x4C45,0x4747,{0xA2,0x97,0x9E,0xD8,0x41,0xF4,0x73,0x1D}} */

#pragma code_seg(".orpc")
static const unsigned short IPCLMethod_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    1818,
    1856,
    1918,
    1968,
    1388
    };

static const MIDL_STUBLESS_PROXY_INFO IPCLMethod_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &IPCLMethod_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO IPCLMethod_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &IPCLMethod_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(12) _IPCLMethodProxyVtbl = 
{
    &IPCLMethod_ProxyInfo,
    &IID_IPCLMethod,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *) (INT_PTR) -1 /* IPCLMethod::setName */ ,
    (void *) (INT_PTR) -1 /* IPCLMethod::addArgument */ ,
    (void *) (INT_PTR) -1 /* IPCLMethod::setReturnValue */ ,
    (void *) (INT_PTR) -1 /* IPCLMethod::setDescription */ ,
    (void *) (INT_PTR) -1 /* IPCLMethod::getLastError */
};


static const PRPC_STUB_FUNCTION IPCLMethod_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _IPCLMethodStubVtbl =
{
    &IID_IPCLMethod,
    &IPCLMethod_ServerInfo,
    12,
    &IPCLMethod_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Object interface: IPCLMethodList, ver. 0.0,
   GUID={0x721705E4,0x275F,0x4aed,{0xBF,0xA9,0x69,0x57,0xE4,0x0B,0xDE,0x4F}} */

#pragma code_seg(".orpc")
static const unsigned short IPCLMethodList_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    2006,
    2044,
    2082,
    114
    };

static const MIDL_STUBLESS_PROXY_INFO IPCLMethodList_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &IPCLMethodList_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO IPCLMethodList_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &IPCLMethodList_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(11) _IPCLMethodListProxyVtbl = 
{
    &IPCLMethodList_ProxyInfo,
    &IID_IPCLMethodList,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *) (INT_PTR) -1 /* IPCLMethodList::addMethod */ ,
    (void *) (INT_PTR) -1 /* IPCLMethodList::addNew */ ,
    (void *) (INT_PTR) -1 /* IPCLMethodList::addPredefinedVariable */ ,
    (void *) (INT_PTR) -1 /* IPCLMethodList::getLastError */
};


static const PRPC_STUB_FUNCTION IPCLMethodList_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _IPCLMethodListStubVtbl =
{
    &IID_IPCLMethodList,
    &IPCLMethodList_ServerInfo,
    11,
    &IPCLMethodList_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Standard interface: __MIDL_itf_PCLExtension_0134, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */


/* Object interface: IPCLType, ver. 0.0,
   GUID={0x12349DAF,0xFD20,0x4ba6,{0xB5,0x00,0x0F,0xC8,0xE0,0xE4,0x9A,0x0B}} */

#pragma code_seg(".orpc")
static const unsigned short IPCLType_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    0,
    2132,
    228,
    2170,
    2208,
    2258,
    2296
    };

static const MIDL_STUBLESS_PROXY_INFO IPCLType_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &IPCLType_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO IPCLType_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &IPCLType_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(14) _IPCLTypeProxyVtbl = 
{
    &IPCLType_ProxyInfo,
    &IID_IPCLType,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *) (INT_PTR) -1 /* IPCLType::getName */ ,
    (void *) (INT_PTR) -1 /* IPCLType::getDescription */ ,
    (void *) (INT_PTR) -1 /* IPCLType::getParentName */ ,
    (void *) (INT_PTR) -1 /* IPCLType::getMethods */ ,
    (void *) (INT_PTR) -1 /* IPCLType::createObject */ ,
    (void *) (INT_PTR) -1 /* IPCLType::getLibrary */ ,
    (void *) (INT_PTR) -1 /* IPCLType::getLastError */
};


static const PRPC_STUB_FUNCTION IPCLType_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _IPCLTypeStubVtbl =
{
    &IID_IPCLType,
    &IPCLType_ServerInfo,
    14,
    &IPCLType_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Object interface: IPCLLibrary, ver. 0.0,
   GUID={0x5483302B,0x931F,0x4506,{0xA9,0xD0,0x30,0xF5,0xC2,0x23,0x48,0xCB}} */

#pragma code_seg(".orpc")
static const unsigned short IPCLLibrary_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    0,
    2132,
    228,
    114,
    1464,
    2334,
    2372,
    2410,
    2454
    };

static const MIDL_STUBLESS_PROXY_INFO IPCLLibrary_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &IPCLLibrary_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO IPCLLibrary_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &IPCLLibrary_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(16) _IPCLLibraryProxyVtbl = 
{
    &IPCLLibrary_ProxyInfo,
    &IID_IPCLLibrary,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *) (INT_PTR) -1 /* IPCLLibrary::getName */ ,
    (void *) (INT_PTR) -1 /* IPCLLibrary::getAuthor */ ,
    (void *) (INT_PTR) -1 /* IPCLLibrary::getVersion */ ,
    (void *) (INT_PTR) -1 /* IPCLLibrary::getDescription */ ,
    (void *) (INT_PTR) -1 /* IPCLLibrary::getPresentationMinimumVersion */ ,
    (void *) (INT_PTR) -1 /* IPCLLibrary::getNamespaceName */ ,
    (void *) (INT_PTR) -1 /* IPCLLibrary::getTypeCount */ ,
    (void *) (INT_PTR) -1 /* IPCLLibrary::getType */ ,
    (void *) (INT_PTR) -1 /* IPCLLibrary::getLastError */
};


static const PRPC_STUB_FUNCTION IPCLLibrary_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _IPCLLibraryStubVtbl =
{
    &IID_IPCLLibrary,
    &IPCLLibrary_ServerInfo,
    16,
    &IPCLLibrary_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};

static const MIDL_STUB_DESC Object_StubDesc = 
    {
    0,
    NdrOleAllocate,
    NdrOleFree,
    0,
    0,
    0,
    0,
    0,
    __MIDL_TypeFormatString.Format,
    1, /* -error bounds_check flag */
    0x50002, /* Ndr library version */
    0,
    0x6000169, /* MIDL Version 6.0.361 */
    0,
    UserMarshalRoutines,
    0,  /* notify & notify_flag routine table */
    0x1, /* MIDL flag */
    0, /* cs routines */
    0,   /* proxy/server info */
    0   /* Reserved5 */
    };

const CInterfaceProxyVtbl * _PCLExtension_ProxyVtblList[] = 
{
    ( CInterfaceProxyVtbl *) &_IPCLBoolProxyVtbl,
    ( CInterfaceProxyVtbl *) &_IPCLGraphicSurfaceLockProxyVtbl,
    ( CInterfaceProxyVtbl *) &_IPCLDoubleProxyVtbl,
    ( CInterfaceProxyVtbl *) &_IPCLDoubleArrayProxyVtbl,
    ( CInterfaceProxyVtbl *) &_IPCLLibraryProxyVtbl,
    ( CInterfaceProxyVtbl *) &_IPCLArrayProxyVtbl,
    ( CInterfaceProxyVtbl *) &_IPCLArgumentsProxyVtbl,
    ( CInterfaceProxyVtbl *) &_IPCLStringProxyVtbl,
    ( CInterfaceProxyVtbl *) &_IPCLBoolArrayProxyVtbl,
    ( CInterfaceProxyVtbl *) &_IPCLObjectProxyVtbl,
    ( CInterfaceProxyVtbl *) &_IPCLMethodProxyVtbl,
    ( CInterfaceProxyVtbl *) &_IPCLStringArrayProxyVtbl,
    ( CInterfaceProxyVtbl *) &_IPCLIntArrayProxyVtbl,
    ( CInterfaceProxyVtbl *) &_IPCLWavefileProxyVtbl,
    ( CInterfaceProxyVtbl *) &_IPCLWavefileLockProxyVtbl,
    ( CInterfaceProxyVtbl *) &_IPCLTypeProxyVtbl,
    ( CInterfaceProxyVtbl *) &_IPCLArgumentProxyVtbl,
    ( CInterfaceProxyVtbl *) &_IPCLIntProxyVtbl,
    ( CInterfaceProxyVtbl *) &_IPCLGraphicSurfaceProxyVtbl,
    ( CInterfaceProxyVtbl *) &_IPCLExtensionObjectProxyVtbl,
    ( CInterfaceProxyVtbl *) &_IPCLMethodListProxyVtbl,
    0
};

const CInterfaceStubVtbl * _PCLExtension_StubVtblList[] = 
{
    ( CInterfaceStubVtbl *) &_IPCLBoolStubVtbl,
    ( CInterfaceStubVtbl *) &_IPCLGraphicSurfaceLockStubVtbl,
    ( CInterfaceStubVtbl *) &_IPCLDoubleStubVtbl,
    ( CInterfaceStubVtbl *) &_IPCLDoubleArrayStubVtbl,
    ( CInterfaceStubVtbl *) &_IPCLLibraryStubVtbl,
    ( CInterfaceStubVtbl *) &_IPCLArrayStubVtbl,
    ( CInterfaceStubVtbl *) &_IPCLArgumentsStubVtbl,
    ( CInterfaceStubVtbl *) &_IPCLStringStubVtbl,
    ( CInterfaceStubVtbl *) &_IPCLBoolArrayStubVtbl,
    ( CInterfaceStubVtbl *) &_IPCLObjectStubVtbl,
    ( CInterfaceStubVtbl *) &_IPCLMethodStubVtbl,
    ( CInterfaceStubVtbl *) &_IPCLStringArrayStubVtbl,
    ( CInterfaceStubVtbl *) &_IPCLIntArrayStubVtbl,
    ( CInterfaceStubVtbl *) &_IPCLWavefileStubVtbl,
    ( CInterfaceStubVtbl *) &_IPCLWavefileLockStubVtbl,
    ( CInterfaceStubVtbl *) &_IPCLTypeStubVtbl,
    ( CInterfaceStubVtbl *) &_IPCLArgumentStubVtbl,
    ( CInterfaceStubVtbl *) &_IPCLIntStubVtbl,
    ( CInterfaceStubVtbl *) &_IPCLGraphicSurfaceStubVtbl,
    ( CInterfaceStubVtbl *) &_IPCLExtensionObjectStubVtbl,
    ( CInterfaceStubVtbl *) &_IPCLMethodListStubVtbl,
    0
};

PCInterfaceName const _PCLExtension_InterfaceNamesList[] = 
{
    "IPCLBool",
    "IPCLGraphicSurfaceLock",
    "IPCLDouble",
    "IPCLDoubleArray",
    "IPCLLibrary",
    "IPCLArray",
    "IPCLArguments",
    "IPCLString",
    "IPCLBoolArray",
    "IPCLObject",
    "IPCLMethod",
    "IPCLStringArray",
    "IPCLIntArray",
    "IPCLWavefile",
    "IPCLWavefileLock",
    "IPCLType",
    "IPCLArgument",
    "IPCLInt",
    "IPCLGraphicSurface",
    "IPCLExtensionObject",
    "IPCLMethodList",
    0
};

const IID *  _PCLExtension_BaseIIDList[] = 
{
    &IID_IDispatch,
    &IID_IDispatch,
    &IID_IDispatch,
    &IID_IDispatch,
    &IID_IDispatch,
    &IID_IDispatch,
    &IID_IDispatch,
    &IID_IDispatch,
    &IID_IDispatch,
    &IID_IDispatch,
    &IID_IDispatch,
    &IID_IDispatch,
    &IID_IDispatch,
    &IID_IDispatch,
    &IID_IDispatch,
    &IID_IDispatch,
    &IID_IDispatch,
    &IID_IDispatch,
    &IID_IDispatch,
    &IID_IDispatch,
    &IID_IDispatch,
    0
};


#define _PCLExtension_CHECK_IID(n)	IID_GENERIC_CHECK_IID( _PCLExtension, pIID, n)

int __stdcall _PCLExtension_IID_Lookup( const IID * pIID, int * pIndex )
{
    IID_BS_LOOKUP_SETUP

    IID_BS_LOOKUP_INITIAL_TEST( _PCLExtension, 21, 16 )
    IID_BS_LOOKUP_NEXT_TEST( _PCLExtension, 8 )
    IID_BS_LOOKUP_NEXT_TEST( _PCLExtension, 4 )
    IID_BS_LOOKUP_NEXT_TEST( _PCLExtension, 2 )
    IID_BS_LOOKUP_NEXT_TEST( _PCLExtension, 1 )
    IID_BS_LOOKUP_RETURN_RESULT( _PCLExtension, 21, *pIndex )
    
}

const ExtendedProxyFileInfo PCLExtension_ProxyFileInfo = 
{
    (PCInterfaceProxyVtblList *) & _PCLExtension_ProxyVtblList,
    (PCInterfaceStubVtblList *) & _PCLExtension_StubVtblList,
    (const PCInterfaceName * ) & _PCLExtension_InterfaceNamesList,
    (const IID ** ) & _PCLExtension_BaseIIDList,
    & _PCLExtension_IID_Lookup, 
    21,
    2,
    0, /* table of [async_uuid] interfaces */
    0, /* Filler1 */
    0, /* Filler2 */
    0  /* Filler3 */
};
#if _MSC_VER >= 1200
#pragma warning(pop)
#endif


#endif /* defined(_M_IA64) || defined(_M_AMD64)*/

