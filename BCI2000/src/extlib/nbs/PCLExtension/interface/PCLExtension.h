

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


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

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __PCLExtension_h__
#define __PCLExtension_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IPCLArgument_FWD_DEFINED__
#define __IPCLArgument_FWD_DEFINED__
typedef interface IPCLArgument IPCLArgument;
#endif 	/* __IPCLArgument_FWD_DEFINED__ */


#ifndef __IPCLWavefileLock_FWD_DEFINED__
#define __IPCLWavefileLock_FWD_DEFINED__
typedef interface IPCLWavefileLock IPCLWavefileLock;
#endif 	/* __IPCLWavefileLock_FWD_DEFINED__ */


#ifndef __IPCLWavefile_FWD_DEFINED__
#define __IPCLWavefile_FWD_DEFINED__
typedef interface IPCLWavefile IPCLWavefile;
#endif 	/* __IPCLWavefile_FWD_DEFINED__ */


#ifndef __IPCLGraphicSurfaceLock_FWD_DEFINED__
#define __IPCLGraphicSurfaceLock_FWD_DEFINED__
typedef interface IPCLGraphicSurfaceLock IPCLGraphicSurfaceLock;
#endif 	/* __IPCLGraphicSurfaceLock_FWD_DEFINED__ */


#ifndef __IPCLGraphicSurface_FWD_DEFINED__
#define __IPCLGraphicSurface_FWD_DEFINED__
typedef interface IPCLGraphicSurface IPCLGraphicSurface;
#endif 	/* __IPCLGraphicSurface_FWD_DEFINED__ */


#ifndef __IPCLArray_FWD_DEFINED__
#define __IPCLArray_FWD_DEFINED__
typedef interface IPCLArray IPCLArray;
#endif 	/* __IPCLArray_FWD_DEFINED__ */


#ifndef __IPCLStringArray_FWD_DEFINED__
#define __IPCLStringArray_FWD_DEFINED__
typedef interface IPCLStringArray IPCLStringArray;
#endif 	/* __IPCLStringArray_FWD_DEFINED__ */


#ifndef __IPCLIntArray_FWD_DEFINED__
#define __IPCLIntArray_FWD_DEFINED__
typedef interface IPCLIntArray IPCLIntArray;
#endif 	/* __IPCLIntArray_FWD_DEFINED__ */


#ifndef __IPCLDoubleArray_FWD_DEFINED__
#define __IPCLDoubleArray_FWD_DEFINED__
typedef interface IPCLDoubleArray IPCLDoubleArray;
#endif 	/* __IPCLDoubleArray_FWD_DEFINED__ */


#ifndef __IPCLBoolArray_FWD_DEFINED__
#define __IPCLBoolArray_FWD_DEFINED__
typedef interface IPCLBoolArray IPCLBoolArray;
#endif 	/* __IPCLBoolArray_FWD_DEFINED__ */


#ifndef __IPCLString_FWD_DEFINED__
#define __IPCLString_FWD_DEFINED__
typedef interface IPCLString IPCLString;
#endif 	/* __IPCLString_FWD_DEFINED__ */


#ifndef __IPCLInt_FWD_DEFINED__
#define __IPCLInt_FWD_DEFINED__
typedef interface IPCLInt IPCLInt;
#endif 	/* __IPCLInt_FWD_DEFINED__ */


#ifndef __IPCLDouble_FWD_DEFINED__
#define __IPCLDouble_FWD_DEFINED__
typedef interface IPCLDouble IPCLDouble;
#endif 	/* __IPCLDouble_FWD_DEFINED__ */


#ifndef __IPCLBool_FWD_DEFINED__
#define __IPCLBool_FWD_DEFINED__
typedef interface IPCLBool IPCLBool;
#endif 	/* __IPCLBool_FWD_DEFINED__ */


#ifndef __IPCLArguments_FWD_DEFINED__
#define __IPCLArguments_FWD_DEFINED__
typedef interface IPCLArguments IPCLArguments;
#endif 	/* __IPCLArguments_FWD_DEFINED__ */


#ifndef __IPCLObject_FWD_DEFINED__
#define __IPCLObject_FWD_DEFINED__
typedef interface IPCLObject IPCLObject;
#endif 	/* __IPCLObject_FWD_DEFINED__ */


#ifndef __IPCLExtensionObject_FWD_DEFINED__
#define __IPCLExtensionObject_FWD_DEFINED__
typedef interface IPCLExtensionObject IPCLExtensionObject;
#endif 	/* __IPCLExtensionObject_FWD_DEFINED__ */


#ifndef __IPCLMethod_FWD_DEFINED__
#define __IPCLMethod_FWD_DEFINED__
typedef interface IPCLMethod IPCLMethod;
#endif 	/* __IPCLMethod_FWD_DEFINED__ */


#ifndef __IPCLMethodList_FWD_DEFINED__
#define __IPCLMethodList_FWD_DEFINED__
typedef interface IPCLMethodList IPCLMethodList;
#endif 	/* __IPCLMethodList_FWD_DEFINED__ */


#ifndef __IPCLType_FWD_DEFINED__
#define __IPCLType_FWD_DEFINED__
typedef interface IPCLType IPCLType;
#endif 	/* __IPCLType_FWD_DEFINED__ */


#ifndef __IPCLLibrary_FWD_DEFINED__
#define __IPCLLibrary_FWD_DEFINED__
typedef interface IPCLLibrary IPCLLibrary;
#endif 	/* __IPCLLibrary_FWD_DEFINED__ */


#ifndef __IPCLGraphicSurfaceLock_FWD_DEFINED__
#define __IPCLGraphicSurfaceLock_FWD_DEFINED__
typedef interface IPCLGraphicSurfaceLock IPCLGraphicSurfaceLock;
#endif 	/* __IPCLGraphicSurfaceLock_FWD_DEFINED__ */


#ifndef __IPCLWavefileLock_FWD_DEFINED__
#define __IPCLWavefileLock_FWD_DEFINED__
typedef interface IPCLWavefileLock IPCLWavefileLock;
#endif 	/* __IPCLWavefileLock_FWD_DEFINED__ */


#ifndef __IPCLArgument_FWD_DEFINED__
#define __IPCLArgument_FWD_DEFINED__
typedef interface IPCLArgument IPCLArgument;
#endif 	/* __IPCLArgument_FWD_DEFINED__ */


#ifndef __IPCLBool_FWD_DEFINED__
#define __IPCLBool_FWD_DEFINED__
typedef interface IPCLBool IPCLBool;
#endif 	/* __IPCLBool_FWD_DEFINED__ */


#ifndef __IPCLDouble_FWD_DEFINED__
#define __IPCLDouble_FWD_DEFINED__
typedef interface IPCLDouble IPCLDouble;
#endif 	/* __IPCLDouble_FWD_DEFINED__ */


#ifndef __IPCLInt_FWD_DEFINED__
#define __IPCLInt_FWD_DEFINED__
typedef interface IPCLInt IPCLInt;
#endif 	/* __IPCLInt_FWD_DEFINED__ */


#ifndef __IPCLString_FWD_DEFINED__
#define __IPCLString_FWD_DEFINED__
typedef interface IPCLString IPCLString;
#endif 	/* __IPCLString_FWD_DEFINED__ */


#ifndef __IPCLExtensionObject_FWD_DEFINED__
#define __IPCLExtensionObject_FWD_DEFINED__
typedef interface IPCLExtensionObject IPCLExtensionObject;
#endif 	/* __IPCLExtensionObject_FWD_DEFINED__ */


#ifndef __IPCLGraphicSurface_FWD_DEFINED__
#define __IPCLGraphicSurface_FWD_DEFINED__
typedef interface IPCLGraphicSurface IPCLGraphicSurface;
#endif 	/* __IPCLGraphicSurface_FWD_DEFINED__ */


#ifndef __IPCLWavefile_FWD_DEFINED__
#define __IPCLWavefile_FWD_DEFINED__
typedef interface IPCLWavefile IPCLWavefile;
#endif 	/* __IPCLWavefile_FWD_DEFINED__ */


#ifndef __IPCLArray_FWD_DEFINED__
#define __IPCLArray_FWD_DEFINED__
typedef interface IPCLArray IPCLArray;
#endif 	/* __IPCLArray_FWD_DEFINED__ */


#ifndef __IPCLBoolArray_FWD_DEFINED__
#define __IPCLBoolArray_FWD_DEFINED__
typedef interface IPCLBoolArray IPCLBoolArray;
#endif 	/* __IPCLBoolArray_FWD_DEFINED__ */


#ifndef __IPCLDoubleArray_FWD_DEFINED__
#define __IPCLDoubleArray_FWD_DEFINED__
typedef interface IPCLDoubleArray IPCLDoubleArray;
#endif 	/* __IPCLDoubleArray_FWD_DEFINED__ */


#ifndef __IPCLIntArray_FWD_DEFINED__
#define __IPCLIntArray_FWD_DEFINED__
typedef interface IPCLIntArray IPCLIntArray;
#endif 	/* __IPCLIntArray_FWD_DEFINED__ */


#ifndef __IPCLStringArray_FWD_DEFINED__
#define __IPCLStringArray_FWD_DEFINED__
typedef interface IPCLStringArray IPCLStringArray;
#endif 	/* __IPCLStringArray_FWD_DEFINED__ */


#ifndef __IPCLLibrary_FWD_DEFINED__
#define __IPCLLibrary_FWD_DEFINED__
typedef interface IPCLLibrary IPCLLibrary;
#endif 	/* __IPCLLibrary_FWD_DEFINED__ */


#ifndef __IPCLMethod_FWD_DEFINED__
#define __IPCLMethod_FWD_DEFINED__
typedef interface IPCLMethod IPCLMethod;
#endif 	/* __IPCLMethod_FWD_DEFINED__ */


#ifndef __IPCLMethodList_FWD_DEFINED__
#define __IPCLMethodList_FWD_DEFINED__
typedef interface IPCLMethodList IPCLMethodList;
#endif 	/* __IPCLMethodList_FWD_DEFINED__ */


#ifndef __IPCLObject_FWD_DEFINED__
#define __IPCLObject_FWD_DEFINED__
typedef interface IPCLObject IPCLObject;
#endif 	/* __IPCLObject_FWD_DEFINED__ */


#ifndef __IPCLType_FWD_DEFINED__
#define __IPCLType_FWD_DEFINED__
typedef interface IPCLType IPCLType;
#endif 	/* __IPCLType_FWD_DEFINED__ */


#ifndef __IPCLArguments_FWD_DEFINED__
#define __IPCLArguments_FWD_DEFINED__
typedef interface IPCLArguments IPCLArguments;
#endif 	/* __IPCLArguments_FWD_DEFINED__ */


/* header files for imported files */
#include "unknwn.h"
#include "oaidl.h"

#ifdef __cplusplus
extern "C"{
#endif 

void * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void * ); 

#ifndef __IPCLArgument_INTERFACE_DEFINED__
#define __IPCLArgument_INTERFACE_DEFINED__

/* interface IPCLArgument */
/* [object][oleautomation][dual][uuid] */ 


EXTERN_C const IID IID_IPCLArgument;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("D43BB8BB-2BA3-492e-B3AB-150071B879A9")
    IPCLArgument : public IDispatch
    {
    public:
        virtual /* [id] */ HRESULT __stdcall getType( 
            /* [out] */ BSTR *type) = 0;
        
        virtual /* [id] */ HRESULT __stdcall getRank( 
            /* [retval][out] */ long *rank) = 0;
        
        virtual /* [id] */ HRESULT __stdcall getIsReference( 
            /* [retval][out] */ long *isRef) = 0;
        
        virtual /* [id] */ HRESULT __stdcall getLastError( 
            /* [out] */ BSTR *message) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IPCLArgumentVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IPCLArgument * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IPCLArgument * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IPCLArgument * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IPCLArgument * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IPCLArgument * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IPCLArgument * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IPCLArgument * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [id] */ HRESULT ( __stdcall *getType )( 
            IPCLArgument * This,
            /* [out] */ BSTR *type);
        
        /* [id] */ HRESULT ( __stdcall *getRank )( 
            IPCLArgument * This,
            /* [retval][out] */ long *rank);
        
        /* [id] */ HRESULT ( __stdcall *getIsReference )( 
            IPCLArgument * This,
            /* [retval][out] */ long *isRef);
        
        /* [id] */ HRESULT ( __stdcall *getLastError )( 
            IPCLArgument * This,
            /* [out] */ BSTR *message);
        
        END_INTERFACE
    } IPCLArgumentVtbl;

    interface IPCLArgument
    {
        CONST_VTBL struct IPCLArgumentVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPCLArgument_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IPCLArgument_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IPCLArgument_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IPCLArgument_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IPCLArgument_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IPCLArgument_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IPCLArgument_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IPCLArgument_getType(This,type)	\
    (This)->lpVtbl -> getType(This,type)

#define IPCLArgument_getRank(This,rank)	\
    (This)->lpVtbl -> getRank(This,rank)

#define IPCLArgument_getIsReference(This,isRef)	\
    (This)->lpVtbl -> getIsReference(This,isRef)

#define IPCLArgument_getLastError(This,message)	\
    (This)->lpVtbl -> getLastError(This,message)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [id] */ HRESULT __stdcall IPCLArgument_getType_Proxy( 
    IPCLArgument * This,
    /* [out] */ BSTR *type);


void __RPC_STUB IPCLArgument_getType_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT __stdcall IPCLArgument_getRank_Proxy( 
    IPCLArgument * This,
    /* [retval][out] */ long *rank);


void __RPC_STUB IPCLArgument_getRank_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT __stdcall IPCLArgument_getIsReference_Proxy( 
    IPCLArgument * This,
    /* [retval][out] */ long *isRef);


void __RPC_STUB IPCLArgument_getIsReference_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT __stdcall IPCLArgument_getLastError_Proxy( 
    IPCLArgument * This,
    /* [out] */ BSTR *message);


void __RPC_STUB IPCLArgument_getLastError_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IPCLArgument_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_PCLExtension_0116 */
/* [local] */ 

typedef struct tagPCLAudioParameters
    {
    unsigned long frameCount;
    unsigned long channelCount;
    unsigned long dataSizeBytes;
    unsigned long framesPerSecond;
    } 	PCLAudioParameters;



extern RPC_IF_HANDLE __MIDL_itf_PCLExtension_0116_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_PCLExtension_0116_v0_0_s_ifspec;

#ifndef __IPCLWavefileLock_INTERFACE_DEFINED__
#define __IPCLWavefileLock_INTERFACE_DEFINED__

/* interface IPCLWavefileLock */
/* [object][oleautomation][dual][uuid] */ 


EXTERN_C const IID IID_IPCLWavefileLock;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("E0934AAF-8832-44cf-A490-06F9172FC7BB")
    IPCLWavefileLock : public IDispatch
    {
    public:
        virtual /* [id] */ HRESULT __stdcall getDataPtr( 
            /* [retval][out] */ byte **dataPtr) = 0;
        
        virtual /* [id] */ HRESULT __stdcall getParameters( 
            /* [out] */ PCLAudioParameters *parameters) = 0;
        
        virtual /* [id] */ HRESULT __stdcall getLastError( 
            /* [out] */ BSTR *message) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IPCLWavefileLockVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IPCLWavefileLock * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IPCLWavefileLock * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IPCLWavefileLock * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IPCLWavefileLock * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IPCLWavefileLock * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IPCLWavefileLock * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IPCLWavefileLock * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [id] */ HRESULT ( __stdcall *getDataPtr )( 
            IPCLWavefileLock * This,
            /* [retval][out] */ byte **dataPtr);
        
        /* [id] */ HRESULT ( __stdcall *getParameters )( 
            IPCLWavefileLock * This,
            /* [out] */ PCLAudioParameters *parameters);
        
        /* [id] */ HRESULT ( __stdcall *getLastError )( 
            IPCLWavefileLock * This,
            /* [out] */ BSTR *message);
        
        END_INTERFACE
    } IPCLWavefileLockVtbl;

    interface IPCLWavefileLock
    {
        CONST_VTBL struct IPCLWavefileLockVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPCLWavefileLock_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IPCLWavefileLock_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IPCLWavefileLock_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IPCLWavefileLock_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IPCLWavefileLock_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IPCLWavefileLock_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IPCLWavefileLock_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IPCLWavefileLock_getDataPtr(This,dataPtr)	\
    (This)->lpVtbl -> getDataPtr(This,dataPtr)

#define IPCLWavefileLock_getParameters(This,parameters)	\
    (This)->lpVtbl -> getParameters(This,parameters)

#define IPCLWavefileLock_getLastError(This,message)	\
    (This)->lpVtbl -> getLastError(This,message)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [id] */ HRESULT __stdcall IPCLWavefileLock_getDataPtr_Proxy( 
    IPCLWavefileLock * This,
    /* [retval][out] */ byte **dataPtr);


void __RPC_STUB IPCLWavefileLock_getDataPtr_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT __stdcall IPCLWavefileLock_getParameters_Proxy( 
    IPCLWavefileLock * This,
    /* [out] */ PCLAudioParameters *parameters);


void __RPC_STUB IPCLWavefileLock_getParameters_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT __stdcall IPCLWavefileLock_getLastError_Proxy( 
    IPCLWavefileLock * This,
    /* [out] */ BSTR *message);


void __RPC_STUB IPCLWavefileLock_getLastError_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IPCLWavefileLock_INTERFACE_DEFINED__ */


#ifndef __IPCLWavefile_INTERFACE_DEFINED__
#define __IPCLWavefile_INTERFACE_DEFINED__

/* interface IPCLWavefile */
/* [object][oleautomation][dual][uuid] */ 


EXTERN_C const IID IID_IPCLWavefile;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("A60D18A4-8C3F-44f0-97FA-B399E56FFDC2")
    IPCLWavefile : public IPCLArgument
    {
    public:
        virtual /* [id] */ HRESULT __stdcall getIsNull( 
            /* [out] */ long *null) = 0;
        
        virtual /* [id] */ HRESULT __stdcall getParameters( 
            /* [out] */ PCLAudioParameters *parameters) = 0;
        
        virtual /* [id] */ HRESULT __stdcall resize( 
            /* [in] */ long frameCount,
            /* [in] */ long channelCount) = 0;
        
        virtual /* [id] */ HRESULT __stdcall lock( 
            /* [retval][out] */ IPCLWavefileLock **lockObject) = 0;
        
        virtual /* [id] */ HRESULT __stdcall replace( 
            /* [in] */ PCLAudioParameters parameters,
            /* [in] */ byte *data) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IPCLWavefileVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IPCLWavefile * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IPCLWavefile * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IPCLWavefile * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IPCLWavefile * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IPCLWavefile * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IPCLWavefile * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IPCLWavefile * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [id] */ HRESULT ( __stdcall *getType )( 
            IPCLWavefile * This,
            /* [out] */ BSTR *type);
        
        /* [id] */ HRESULT ( __stdcall *getRank )( 
            IPCLWavefile * This,
            /* [retval][out] */ long *rank);
        
        /* [id] */ HRESULT ( __stdcall *getIsReference )( 
            IPCLWavefile * This,
            /* [retval][out] */ long *isRef);
        
        /* [id] */ HRESULT ( __stdcall *getLastError )( 
            IPCLWavefile * This,
            /* [out] */ BSTR *message);
        
        /* [id] */ HRESULT ( __stdcall *getIsNull )( 
            IPCLWavefile * This,
            /* [out] */ long *null);
        
        /* [id] */ HRESULT ( __stdcall *getParameters )( 
            IPCLWavefile * This,
            /* [out] */ PCLAudioParameters *parameters);
        
        /* [id] */ HRESULT ( __stdcall *resize )( 
            IPCLWavefile * This,
            /* [in] */ long frameCount,
            /* [in] */ long channelCount);
        
        /* [id] */ HRESULT ( __stdcall *lock )( 
            IPCLWavefile * This,
            /* [retval][out] */ IPCLWavefileLock **lockObject);
        
        /* [id] */ HRESULT ( __stdcall *replace )( 
            IPCLWavefile * This,
            /* [in] */ PCLAudioParameters parameters,
            /* [in] */ byte *data);
        
        END_INTERFACE
    } IPCLWavefileVtbl;

    interface IPCLWavefile
    {
        CONST_VTBL struct IPCLWavefileVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPCLWavefile_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IPCLWavefile_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IPCLWavefile_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IPCLWavefile_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IPCLWavefile_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IPCLWavefile_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IPCLWavefile_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IPCLWavefile_getType(This,type)	\
    (This)->lpVtbl -> getType(This,type)

#define IPCLWavefile_getRank(This,rank)	\
    (This)->lpVtbl -> getRank(This,rank)

#define IPCLWavefile_getIsReference(This,isRef)	\
    (This)->lpVtbl -> getIsReference(This,isRef)

#define IPCLWavefile_getLastError(This,message)	\
    (This)->lpVtbl -> getLastError(This,message)


#define IPCLWavefile_getIsNull(This,null)	\
    (This)->lpVtbl -> getIsNull(This,null)

#define IPCLWavefile_getParameters(This,parameters)	\
    (This)->lpVtbl -> getParameters(This,parameters)

#define IPCLWavefile_resize(This,frameCount,channelCount)	\
    (This)->lpVtbl -> resize(This,frameCount,channelCount)

#define IPCLWavefile_lock(This,lockObject)	\
    (This)->lpVtbl -> lock(This,lockObject)

#define IPCLWavefile_replace(This,parameters,data)	\
    (This)->lpVtbl -> replace(This,parameters,data)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [id] */ HRESULT __stdcall IPCLWavefile_getIsNull_Proxy( 
    IPCLWavefile * This,
    /* [out] */ long *null);


void __RPC_STUB IPCLWavefile_getIsNull_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT __stdcall IPCLWavefile_getParameters_Proxy( 
    IPCLWavefile * This,
    /* [out] */ PCLAudioParameters *parameters);


void __RPC_STUB IPCLWavefile_getParameters_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT __stdcall IPCLWavefile_resize_Proxy( 
    IPCLWavefile * This,
    /* [in] */ long frameCount,
    /* [in] */ long channelCount);


void __RPC_STUB IPCLWavefile_resize_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT __stdcall IPCLWavefile_lock_Proxy( 
    IPCLWavefile * This,
    /* [retval][out] */ IPCLWavefileLock **lockObject);


void __RPC_STUB IPCLWavefile_lock_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT __stdcall IPCLWavefile_replace_Proxy( 
    IPCLWavefile * This,
    /* [in] */ PCLAudioParameters parameters,
    /* [in] */ byte *data);


void __RPC_STUB IPCLWavefile_replace_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IPCLWavefile_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_PCLExtension_0118 */
/* [local] */ 

typedef struct tagPCLGraphicParameters
    {
    unsigned long widthPixels;
    unsigned long heightPixels;
    unsigned long pitchBytes;
    unsigned long bitsPerPixel;
    unsigned long redMask;
    unsigned long greenMask;
    unsigned long blueMask;
    unsigned long alphaMask;
    } 	PCLGraphicParameters;



extern RPC_IF_HANDLE __MIDL_itf_PCLExtension_0118_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_PCLExtension_0118_v0_0_s_ifspec;

#ifndef __IPCLGraphicSurfaceLock_INTERFACE_DEFINED__
#define __IPCLGraphicSurfaceLock_INTERFACE_DEFINED__

/* interface IPCLGraphicSurfaceLock */
/* [object][oleautomation][dual][uuid] */ 


EXTERN_C const IID IID_IPCLGraphicSurfaceLock;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("44E35511-BC78-40a5-B713-0E9077933E8D")
    IPCLGraphicSurfaceLock : public IDispatch
    {
    public:
        virtual /* [id] */ HRESULT __stdcall getDataPtr( 
            /* [retval][out] */ byte **dataPtr) = 0;
        
        virtual /* [id] */ HRESULT __stdcall getParameters( 
            /* [out] */ PCLGraphicParameters *parameters) = 0;
        
        virtual /* [id] */ HRESULT __stdcall getLastError( 
            /* [out] */ BSTR *message) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IPCLGraphicSurfaceLockVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IPCLGraphicSurfaceLock * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IPCLGraphicSurfaceLock * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IPCLGraphicSurfaceLock * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IPCLGraphicSurfaceLock * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IPCLGraphicSurfaceLock * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IPCLGraphicSurfaceLock * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IPCLGraphicSurfaceLock * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [id] */ HRESULT ( __stdcall *getDataPtr )( 
            IPCLGraphicSurfaceLock * This,
            /* [retval][out] */ byte **dataPtr);
        
        /* [id] */ HRESULT ( __stdcall *getParameters )( 
            IPCLGraphicSurfaceLock * This,
            /* [out] */ PCLGraphicParameters *parameters);
        
        /* [id] */ HRESULT ( __stdcall *getLastError )( 
            IPCLGraphicSurfaceLock * This,
            /* [out] */ BSTR *message);
        
        END_INTERFACE
    } IPCLGraphicSurfaceLockVtbl;

    interface IPCLGraphicSurfaceLock
    {
        CONST_VTBL struct IPCLGraphicSurfaceLockVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPCLGraphicSurfaceLock_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IPCLGraphicSurfaceLock_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IPCLGraphicSurfaceLock_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IPCLGraphicSurfaceLock_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IPCLGraphicSurfaceLock_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IPCLGraphicSurfaceLock_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IPCLGraphicSurfaceLock_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IPCLGraphicSurfaceLock_getDataPtr(This,dataPtr)	\
    (This)->lpVtbl -> getDataPtr(This,dataPtr)

#define IPCLGraphicSurfaceLock_getParameters(This,parameters)	\
    (This)->lpVtbl -> getParameters(This,parameters)

#define IPCLGraphicSurfaceLock_getLastError(This,message)	\
    (This)->lpVtbl -> getLastError(This,message)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [id] */ HRESULT __stdcall IPCLGraphicSurfaceLock_getDataPtr_Proxy( 
    IPCLGraphicSurfaceLock * This,
    /* [retval][out] */ byte **dataPtr);


void __RPC_STUB IPCLGraphicSurfaceLock_getDataPtr_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT __stdcall IPCLGraphicSurfaceLock_getParameters_Proxy( 
    IPCLGraphicSurfaceLock * This,
    /* [out] */ PCLGraphicParameters *parameters);


void __RPC_STUB IPCLGraphicSurfaceLock_getParameters_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT __stdcall IPCLGraphicSurfaceLock_getLastError_Proxy( 
    IPCLGraphicSurfaceLock * This,
    /* [out] */ BSTR *message);


void __RPC_STUB IPCLGraphicSurfaceLock_getLastError_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IPCLGraphicSurfaceLock_INTERFACE_DEFINED__ */


#ifndef __IPCLGraphicSurface_INTERFACE_DEFINED__
#define __IPCLGraphicSurface_INTERFACE_DEFINED__

/* interface IPCLGraphicSurface */
/* [object][oleautomation][dual][uuid] */ 


EXTERN_C const IID IID_IPCLGraphicSurface;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("A96615CA-28B1-4958-9329-7F1A453610B7")
    IPCLGraphicSurface : public IPCLArgument
    {
    public:
        virtual /* [id] */ HRESULT __stdcall getIsNull( 
            /* [out] */ long *null) = 0;
        
        virtual /* [id] */ HRESULT __stdcall getSize( 
            /* [out] */ long *widthPixels,
            /* [out] */ long *heightPixels) = 0;
        
        virtual /* [id] */ HRESULT __stdcall resize( 
            /* [in] */ long widthPixels,
            /* [in] */ long heightPixels) = 0;
        
        virtual /* [id] */ HRESULT __stdcall lock( 
            /* [retval][out] */ IPCLGraphicSurfaceLock **lockObject) = 0;
        
        virtual /* [id] */ HRESULT __stdcall replace( 
            /* [in] */ PCLGraphicParameters parameters,
            /* [in] */ byte *data) = 0;
        
        virtual /* [id] */ HRESULT __stdcall convertCustomUnits( 
            /* [in] */ double customWidth,
            /* [in] */ double customHeight,
            /* [out] */ double *pixelWidth,
            /* [out] */ double *pixelHeight) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IPCLGraphicSurfaceVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IPCLGraphicSurface * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IPCLGraphicSurface * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IPCLGraphicSurface * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IPCLGraphicSurface * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IPCLGraphicSurface * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IPCLGraphicSurface * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IPCLGraphicSurface * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [id] */ HRESULT ( __stdcall *getType )( 
            IPCLGraphicSurface * This,
            /* [out] */ BSTR *type);
        
        /* [id] */ HRESULT ( __stdcall *getRank )( 
            IPCLGraphicSurface * This,
            /* [retval][out] */ long *rank);
        
        /* [id] */ HRESULT ( __stdcall *getIsReference )( 
            IPCLGraphicSurface * This,
            /* [retval][out] */ long *isRef);
        
        /* [id] */ HRESULT ( __stdcall *getLastError )( 
            IPCLGraphicSurface * This,
            /* [out] */ BSTR *message);
        
        /* [id] */ HRESULT ( __stdcall *getIsNull )( 
            IPCLGraphicSurface * This,
            /* [out] */ long *null);
        
        /* [id] */ HRESULT ( __stdcall *getSize )( 
            IPCLGraphicSurface * This,
            /* [out] */ long *widthPixels,
            /* [out] */ long *heightPixels);
        
        /* [id] */ HRESULT ( __stdcall *resize )( 
            IPCLGraphicSurface * This,
            /* [in] */ long widthPixels,
            /* [in] */ long heightPixels);
        
        /* [id] */ HRESULT ( __stdcall *lock )( 
            IPCLGraphicSurface * This,
            /* [retval][out] */ IPCLGraphicSurfaceLock **lockObject);
        
        /* [id] */ HRESULT ( __stdcall *replace )( 
            IPCLGraphicSurface * This,
            /* [in] */ PCLGraphicParameters parameters,
            /* [in] */ byte *data);
        
        /* [id] */ HRESULT ( __stdcall *convertCustomUnits )( 
            IPCLGraphicSurface * This,
            /* [in] */ double customWidth,
            /* [in] */ double customHeight,
            /* [out] */ double *pixelWidth,
            /* [out] */ double *pixelHeight);
        
        END_INTERFACE
    } IPCLGraphicSurfaceVtbl;

    interface IPCLGraphicSurface
    {
        CONST_VTBL struct IPCLGraphicSurfaceVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPCLGraphicSurface_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IPCLGraphicSurface_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IPCLGraphicSurface_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IPCLGraphicSurface_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IPCLGraphicSurface_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IPCLGraphicSurface_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IPCLGraphicSurface_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IPCLGraphicSurface_getType(This,type)	\
    (This)->lpVtbl -> getType(This,type)

#define IPCLGraphicSurface_getRank(This,rank)	\
    (This)->lpVtbl -> getRank(This,rank)

#define IPCLGraphicSurface_getIsReference(This,isRef)	\
    (This)->lpVtbl -> getIsReference(This,isRef)

#define IPCLGraphicSurface_getLastError(This,message)	\
    (This)->lpVtbl -> getLastError(This,message)


#define IPCLGraphicSurface_getIsNull(This,null)	\
    (This)->lpVtbl -> getIsNull(This,null)

#define IPCLGraphicSurface_getSize(This,widthPixels,heightPixels)	\
    (This)->lpVtbl -> getSize(This,widthPixels,heightPixels)

#define IPCLGraphicSurface_resize(This,widthPixels,heightPixels)	\
    (This)->lpVtbl -> resize(This,widthPixels,heightPixels)

#define IPCLGraphicSurface_lock(This,lockObject)	\
    (This)->lpVtbl -> lock(This,lockObject)

#define IPCLGraphicSurface_replace(This,parameters,data)	\
    (This)->lpVtbl -> replace(This,parameters,data)

#define IPCLGraphicSurface_convertCustomUnits(This,customWidth,customHeight,pixelWidth,pixelHeight)	\
    (This)->lpVtbl -> convertCustomUnits(This,customWidth,customHeight,pixelWidth,pixelHeight)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [id] */ HRESULT __stdcall IPCLGraphicSurface_getIsNull_Proxy( 
    IPCLGraphicSurface * This,
    /* [out] */ long *null);


void __RPC_STUB IPCLGraphicSurface_getIsNull_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT __stdcall IPCLGraphicSurface_getSize_Proxy( 
    IPCLGraphicSurface * This,
    /* [out] */ long *widthPixels,
    /* [out] */ long *heightPixels);


void __RPC_STUB IPCLGraphicSurface_getSize_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT __stdcall IPCLGraphicSurface_resize_Proxy( 
    IPCLGraphicSurface * This,
    /* [in] */ long widthPixels,
    /* [in] */ long heightPixels);


void __RPC_STUB IPCLGraphicSurface_resize_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT __stdcall IPCLGraphicSurface_lock_Proxy( 
    IPCLGraphicSurface * This,
    /* [retval][out] */ IPCLGraphicSurfaceLock **lockObject);


void __RPC_STUB IPCLGraphicSurface_lock_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT __stdcall IPCLGraphicSurface_replace_Proxy( 
    IPCLGraphicSurface * This,
    /* [in] */ PCLGraphicParameters parameters,
    /* [in] */ byte *data);


void __RPC_STUB IPCLGraphicSurface_replace_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT __stdcall IPCLGraphicSurface_convertCustomUnits_Proxy( 
    IPCLGraphicSurface * This,
    /* [in] */ double customWidth,
    /* [in] */ double customHeight,
    /* [out] */ double *pixelWidth,
    /* [out] */ double *pixelHeight);


void __RPC_STUB IPCLGraphicSurface_convertCustomUnits_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IPCLGraphicSurface_INTERFACE_DEFINED__ */


#ifndef __IPCLArray_INTERFACE_DEFINED__
#define __IPCLArray_INTERFACE_DEFINED__

/* interface IPCLArray */
/* [object][oleautomation][dual][uuid] */ 


EXTERN_C const IID IID_IPCLArray;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("DD64D836-F3DF-4097-A12A-45EF441B2FD0")
    IPCLArray : public IPCLArgument
    {
    public:
        virtual /* [id] */ HRESULT __stdcall getCount( 
            /* [retval][out] */ long *count) = 0;
        
        virtual /* [id] */ HRESULT __stdcall resize( 
            /* [in] */ long count) = 0;
        
        virtual /* [id] */ HRESULT __stdcall getSubArray( 
            /* [in] */ long index,
            /* [retval][out] */ IPCLArray **subArray) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IPCLArrayVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IPCLArray * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IPCLArray * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IPCLArray * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IPCLArray * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IPCLArray * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IPCLArray * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IPCLArray * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [id] */ HRESULT ( __stdcall *getType )( 
            IPCLArray * This,
            /* [out] */ BSTR *type);
        
        /* [id] */ HRESULT ( __stdcall *getRank )( 
            IPCLArray * This,
            /* [retval][out] */ long *rank);
        
        /* [id] */ HRESULT ( __stdcall *getIsReference )( 
            IPCLArray * This,
            /* [retval][out] */ long *isRef);
        
        /* [id] */ HRESULT ( __stdcall *getLastError )( 
            IPCLArray * This,
            /* [out] */ BSTR *message);
        
        /* [id] */ HRESULT ( __stdcall *getCount )( 
            IPCLArray * This,
            /* [retval][out] */ long *count);
        
        /* [id] */ HRESULT ( __stdcall *resize )( 
            IPCLArray * This,
            /* [in] */ long count);
        
        /* [id] */ HRESULT ( __stdcall *getSubArray )( 
            IPCLArray * This,
            /* [in] */ long index,
            /* [retval][out] */ IPCLArray **subArray);
        
        END_INTERFACE
    } IPCLArrayVtbl;

    interface IPCLArray
    {
        CONST_VTBL struct IPCLArrayVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPCLArray_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IPCLArray_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IPCLArray_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IPCLArray_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IPCLArray_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IPCLArray_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IPCLArray_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IPCLArray_getType(This,type)	\
    (This)->lpVtbl -> getType(This,type)

#define IPCLArray_getRank(This,rank)	\
    (This)->lpVtbl -> getRank(This,rank)

#define IPCLArray_getIsReference(This,isRef)	\
    (This)->lpVtbl -> getIsReference(This,isRef)

#define IPCLArray_getLastError(This,message)	\
    (This)->lpVtbl -> getLastError(This,message)


#define IPCLArray_getCount(This,count)	\
    (This)->lpVtbl -> getCount(This,count)

#define IPCLArray_resize(This,count)	\
    (This)->lpVtbl -> resize(This,count)

#define IPCLArray_getSubArray(This,index,subArray)	\
    (This)->lpVtbl -> getSubArray(This,index,subArray)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [id] */ HRESULT __stdcall IPCLArray_getCount_Proxy( 
    IPCLArray * This,
    /* [retval][out] */ long *count);


void __RPC_STUB IPCLArray_getCount_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT __stdcall IPCLArray_resize_Proxy( 
    IPCLArray * This,
    /* [in] */ long count);


void __RPC_STUB IPCLArray_resize_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT __stdcall IPCLArray_getSubArray_Proxy( 
    IPCLArray * This,
    /* [in] */ long index,
    /* [retval][out] */ IPCLArray **subArray);


void __RPC_STUB IPCLArray_getSubArray_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IPCLArray_INTERFACE_DEFINED__ */


#ifndef __IPCLStringArray_INTERFACE_DEFINED__
#define __IPCLStringArray_INTERFACE_DEFINED__

/* interface IPCLStringArray */
/* [object][oleautomation][dual][uuid] */ 


EXTERN_C const IID IID_IPCLStringArray;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("CEB1D686-B7EC-44bc-8FDA-870C50884088")
    IPCLStringArray : public IPCLArray
    {
    public:
        virtual /* [id] */ HRESULT __stdcall setValue( 
            /* [in] */ long index,
            /* [in] */ BSTR value) = 0;
        
        virtual /* [id] */ HRESULT __stdcall getValue( 
            /* [in] */ long index,
            /* [retval][out] */ BSTR *value) = 0;
        
        virtual /* [id] */ HRESULT __stdcall add( 
            /* [in] */ BSTR value) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IPCLStringArrayVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IPCLStringArray * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IPCLStringArray * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IPCLStringArray * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IPCLStringArray * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IPCLStringArray * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IPCLStringArray * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IPCLStringArray * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [id] */ HRESULT ( __stdcall *getType )( 
            IPCLStringArray * This,
            /* [out] */ BSTR *type);
        
        /* [id] */ HRESULT ( __stdcall *getRank )( 
            IPCLStringArray * This,
            /* [retval][out] */ long *rank);
        
        /* [id] */ HRESULT ( __stdcall *getIsReference )( 
            IPCLStringArray * This,
            /* [retval][out] */ long *isRef);
        
        /* [id] */ HRESULT ( __stdcall *getLastError )( 
            IPCLStringArray * This,
            /* [out] */ BSTR *message);
        
        /* [id] */ HRESULT ( __stdcall *getCount )( 
            IPCLStringArray * This,
            /* [retval][out] */ long *count);
        
        /* [id] */ HRESULT ( __stdcall *resize )( 
            IPCLStringArray * This,
            /* [in] */ long count);
        
        /* [id] */ HRESULT ( __stdcall *getSubArray )( 
            IPCLStringArray * This,
            /* [in] */ long index,
            /* [retval][out] */ IPCLArray **subArray);
        
        /* [id] */ HRESULT ( __stdcall *setValue )( 
            IPCLStringArray * This,
            /* [in] */ long index,
            /* [in] */ BSTR value);
        
        /* [id] */ HRESULT ( __stdcall *getValue )( 
            IPCLStringArray * This,
            /* [in] */ long index,
            /* [retval][out] */ BSTR *value);
        
        /* [id] */ HRESULT ( __stdcall *add )( 
            IPCLStringArray * This,
            /* [in] */ BSTR value);
        
        END_INTERFACE
    } IPCLStringArrayVtbl;

    interface IPCLStringArray
    {
        CONST_VTBL struct IPCLStringArrayVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPCLStringArray_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IPCLStringArray_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IPCLStringArray_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IPCLStringArray_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IPCLStringArray_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IPCLStringArray_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IPCLStringArray_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IPCLStringArray_getType(This,type)	\
    (This)->lpVtbl -> getType(This,type)

#define IPCLStringArray_getRank(This,rank)	\
    (This)->lpVtbl -> getRank(This,rank)

#define IPCLStringArray_getIsReference(This,isRef)	\
    (This)->lpVtbl -> getIsReference(This,isRef)

#define IPCLStringArray_getLastError(This,message)	\
    (This)->lpVtbl -> getLastError(This,message)


#define IPCLStringArray_getCount(This,count)	\
    (This)->lpVtbl -> getCount(This,count)

#define IPCLStringArray_resize(This,count)	\
    (This)->lpVtbl -> resize(This,count)

#define IPCLStringArray_getSubArray(This,index,subArray)	\
    (This)->lpVtbl -> getSubArray(This,index,subArray)


#define IPCLStringArray_setValue(This,index,value)	\
    (This)->lpVtbl -> setValue(This,index,value)

#define IPCLStringArray_getValue(This,index,value)	\
    (This)->lpVtbl -> getValue(This,index,value)

#define IPCLStringArray_add(This,value)	\
    (This)->lpVtbl -> add(This,value)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [id] */ HRESULT __stdcall IPCLStringArray_setValue_Proxy( 
    IPCLStringArray * This,
    /* [in] */ long index,
    /* [in] */ BSTR value);


void __RPC_STUB IPCLStringArray_setValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT __stdcall IPCLStringArray_getValue_Proxy( 
    IPCLStringArray * This,
    /* [in] */ long index,
    /* [retval][out] */ BSTR *value);


void __RPC_STUB IPCLStringArray_getValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT __stdcall IPCLStringArray_add_Proxy( 
    IPCLStringArray * This,
    /* [in] */ BSTR value);


void __RPC_STUB IPCLStringArray_add_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IPCLStringArray_INTERFACE_DEFINED__ */


#ifndef __IPCLIntArray_INTERFACE_DEFINED__
#define __IPCLIntArray_INTERFACE_DEFINED__

/* interface IPCLIntArray */
/* [object][oleautomation][dual][uuid] */ 


EXTERN_C const IID IID_IPCLIntArray;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("6602A29A-38D8-4c3e-A980-F04A0DD64895")
    IPCLIntArray : public IPCLArray
    {
    public:
        virtual /* [id] */ HRESULT __stdcall getDataPtr( 
            /* [retval][out] */ long **dataPtr) = 0;
        
        virtual /* [id] */ HRESULT __stdcall setValue( 
            /* [in] */ long index,
            /* [in] */ long value) = 0;
        
        virtual /* [id] */ HRESULT __stdcall getValue( 
            /* [in] */ long index,
            /* [retval][out] */ long *value) = 0;
        
        virtual /* [id] */ HRESULT __stdcall add( 
            /* [in] */ long value) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IPCLIntArrayVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IPCLIntArray * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IPCLIntArray * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IPCLIntArray * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IPCLIntArray * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IPCLIntArray * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IPCLIntArray * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IPCLIntArray * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [id] */ HRESULT ( __stdcall *getType )( 
            IPCLIntArray * This,
            /* [out] */ BSTR *type);
        
        /* [id] */ HRESULT ( __stdcall *getRank )( 
            IPCLIntArray * This,
            /* [retval][out] */ long *rank);
        
        /* [id] */ HRESULT ( __stdcall *getIsReference )( 
            IPCLIntArray * This,
            /* [retval][out] */ long *isRef);
        
        /* [id] */ HRESULT ( __stdcall *getLastError )( 
            IPCLIntArray * This,
            /* [out] */ BSTR *message);
        
        /* [id] */ HRESULT ( __stdcall *getCount )( 
            IPCLIntArray * This,
            /* [retval][out] */ long *count);
        
        /* [id] */ HRESULT ( __stdcall *resize )( 
            IPCLIntArray * This,
            /* [in] */ long count);
        
        /* [id] */ HRESULT ( __stdcall *getSubArray )( 
            IPCLIntArray * This,
            /* [in] */ long index,
            /* [retval][out] */ IPCLArray **subArray);
        
        /* [id] */ HRESULT ( __stdcall *getDataPtr )( 
            IPCLIntArray * This,
            /* [retval][out] */ long **dataPtr);
        
        /* [id] */ HRESULT ( __stdcall *setValue )( 
            IPCLIntArray * This,
            /* [in] */ long index,
            /* [in] */ long value);
        
        /* [id] */ HRESULT ( __stdcall *getValue )( 
            IPCLIntArray * This,
            /* [in] */ long index,
            /* [retval][out] */ long *value);
        
        /* [id] */ HRESULT ( __stdcall *add )( 
            IPCLIntArray * This,
            /* [in] */ long value);
        
        END_INTERFACE
    } IPCLIntArrayVtbl;

    interface IPCLIntArray
    {
        CONST_VTBL struct IPCLIntArrayVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPCLIntArray_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IPCLIntArray_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IPCLIntArray_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IPCLIntArray_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IPCLIntArray_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IPCLIntArray_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IPCLIntArray_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IPCLIntArray_getType(This,type)	\
    (This)->lpVtbl -> getType(This,type)

#define IPCLIntArray_getRank(This,rank)	\
    (This)->lpVtbl -> getRank(This,rank)

#define IPCLIntArray_getIsReference(This,isRef)	\
    (This)->lpVtbl -> getIsReference(This,isRef)

#define IPCLIntArray_getLastError(This,message)	\
    (This)->lpVtbl -> getLastError(This,message)


#define IPCLIntArray_getCount(This,count)	\
    (This)->lpVtbl -> getCount(This,count)

#define IPCLIntArray_resize(This,count)	\
    (This)->lpVtbl -> resize(This,count)

#define IPCLIntArray_getSubArray(This,index,subArray)	\
    (This)->lpVtbl -> getSubArray(This,index,subArray)


#define IPCLIntArray_getDataPtr(This,dataPtr)	\
    (This)->lpVtbl -> getDataPtr(This,dataPtr)

#define IPCLIntArray_setValue(This,index,value)	\
    (This)->lpVtbl -> setValue(This,index,value)

#define IPCLIntArray_getValue(This,index,value)	\
    (This)->lpVtbl -> getValue(This,index,value)

#define IPCLIntArray_add(This,value)	\
    (This)->lpVtbl -> add(This,value)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [id] */ HRESULT __stdcall IPCLIntArray_getDataPtr_Proxy( 
    IPCLIntArray * This,
    /* [retval][out] */ long **dataPtr);


void __RPC_STUB IPCLIntArray_getDataPtr_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT __stdcall IPCLIntArray_setValue_Proxy( 
    IPCLIntArray * This,
    /* [in] */ long index,
    /* [in] */ long value);


void __RPC_STUB IPCLIntArray_setValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT __stdcall IPCLIntArray_getValue_Proxy( 
    IPCLIntArray * This,
    /* [in] */ long index,
    /* [retval][out] */ long *value);


void __RPC_STUB IPCLIntArray_getValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT __stdcall IPCLIntArray_add_Proxy( 
    IPCLIntArray * This,
    /* [in] */ long value);


void __RPC_STUB IPCLIntArray_add_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IPCLIntArray_INTERFACE_DEFINED__ */


#ifndef __IPCLDoubleArray_INTERFACE_DEFINED__
#define __IPCLDoubleArray_INTERFACE_DEFINED__

/* interface IPCLDoubleArray */
/* [object][oleautomation][dual][uuid] */ 


EXTERN_C const IID IID_IPCLDoubleArray;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("CA5BDA29-B722-4518-AF47-26C9F8628752")
    IPCLDoubleArray : public IPCLArray
    {
    public:
        virtual /* [id] */ HRESULT __stdcall getDataPtr( 
            /* [retval][out] */ double **dataPtr) = 0;
        
        virtual /* [id] */ HRESULT __stdcall setValue( 
            /* [in] */ long index,
            /* [in] */ double value) = 0;
        
        virtual /* [id] */ HRESULT __stdcall getValue( 
            /* [in] */ long index,
            /* [retval][out] */ double *value) = 0;
        
        virtual /* [id] */ HRESULT __stdcall add( 
            /* [in] */ double value) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IPCLDoubleArrayVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IPCLDoubleArray * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IPCLDoubleArray * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IPCLDoubleArray * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IPCLDoubleArray * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IPCLDoubleArray * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IPCLDoubleArray * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IPCLDoubleArray * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [id] */ HRESULT ( __stdcall *getType )( 
            IPCLDoubleArray * This,
            /* [out] */ BSTR *type);
        
        /* [id] */ HRESULT ( __stdcall *getRank )( 
            IPCLDoubleArray * This,
            /* [retval][out] */ long *rank);
        
        /* [id] */ HRESULT ( __stdcall *getIsReference )( 
            IPCLDoubleArray * This,
            /* [retval][out] */ long *isRef);
        
        /* [id] */ HRESULT ( __stdcall *getLastError )( 
            IPCLDoubleArray * This,
            /* [out] */ BSTR *message);
        
        /* [id] */ HRESULT ( __stdcall *getCount )( 
            IPCLDoubleArray * This,
            /* [retval][out] */ long *count);
        
        /* [id] */ HRESULT ( __stdcall *resize )( 
            IPCLDoubleArray * This,
            /* [in] */ long count);
        
        /* [id] */ HRESULT ( __stdcall *getSubArray )( 
            IPCLDoubleArray * This,
            /* [in] */ long index,
            /* [retval][out] */ IPCLArray **subArray);
        
        /* [id] */ HRESULT ( __stdcall *getDataPtr )( 
            IPCLDoubleArray * This,
            /* [retval][out] */ double **dataPtr);
        
        /* [id] */ HRESULT ( __stdcall *setValue )( 
            IPCLDoubleArray * This,
            /* [in] */ long index,
            /* [in] */ double value);
        
        /* [id] */ HRESULT ( __stdcall *getValue )( 
            IPCLDoubleArray * This,
            /* [in] */ long index,
            /* [retval][out] */ double *value);
        
        /* [id] */ HRESULT ( __stdcall *add )( 
            IPCLDoubleArray * This,
            /* [in] */ double value);
        
        END_INTERFACE
    } IPCLDoubleArrayVtbl;

    interface IPCLDoubleArray
    {
        CONST_VTBL struct IPCLDoubleArrayVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPCLDoubleArray_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IPCLDoubleArray_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IPCLDoubleArray_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IPCLDoubleArray_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IPCLDoubleArray_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IPCLDoubleArray_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IPCLDoubleArray_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IPCLDoubleArray_getType(This,type)	\
    (This)->lpVtbl -> getType(This,type)

#define IPCLDoubleArray_getRank(This,rank)	\
    (This)->lpVtbl -> getRank(This,rank)

#define IPCLDoubleArray_getIsReference(This,isRef)	\
    (This)->lpVtbl -> getIsReference(This,isRef)

#define IPCLDoubleArray_getLastError(This,message)	\
    (This)->lpVtbl -> getLastError(This,message)


#define IPCLDoubleArray_getCount(This,count)	\
    (This)->lpVtbl -> getCount(This,count)

#define IPCLDoubleArray_resize(This,count)	\
    (This)->lpVtbl -> resize(This,count)

#define IPCLDoubleArray_getSubArray(This,index,subArray)	\
    (This)->lpVtbl -> getSubArray(This,index,subArray)


#define IPCLDoubleArray_getDataPtr(This,dataPtr)	\
    (This)->lpVtbl -> getDataPtr(This,dataPtr)

#define IPCLDoubleArray_setValue(This,index,value)	\
    (This)->lpVtbl -> setValue(This,index,value)

#define IPCLDoubleArray_getValue(This,index,value)	\
    (This)->lpVtbl -> getValue(This,index,value)

#define IPCLDoubleArray_add(This,value)	\
    (This)->lpVtbl -> add(This,value)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [id] */ HRESULT __stdcall IPCLDoubleArray_getDataPtr_Proxy( 
    IPCLDoubleArray * This,
    /* [retval][out] */ double **dataPtr);


void __RPC_STUB IPCLDoubleArray_getDataPtr_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT __stdcall IPCLDoubleArray_setValue_Proxy( 
    IPCLDoubleArray * This,
    /* [in] */ long index,
    /* [in] */ double value);


void __RPC_STUB IPCLDoubleArray_setValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT __stdcall IPCLDoubleArray_getValue_Proxy( 
    IPCLDoubleArray * This,
    /* [in] */ long index,
    /* [retval][out] */ double *value);


void __RPC_STUB IPCLDoubleArray_getValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT __stdcall IPCLDoubleArray_add_Proxy( 
    IPCLDoubleArray * This,
    /* [in] */ double value);


void __RPC_STUB IPCLDoubleArray_add_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IPCLDoubleArray_INTERFACE_DEFINED__ */


#ifndef __IPCLBoolArray_INTERFACE_DEFINED__
#define __IPCLBoolArray_INTERFACE_DEFINED__

/* interface IPCLBoolArray */
/* [object][oleautomation][dual][uuid] */ 


EXTERN_C const IID IID_IPCLBoolArray;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("FF2B4A6F-7F73-41b4-A696-F28C1F3147ED")
    IPCLBoolArray : public IPCLArray
    {
    public:
        virtual /* [id] */ HRESULT __stdcall getDataPtr( 
            /* [retval][out] */ byte **dataPtr) = 0;
        
        virtual /* [id] */ HRESULT __stdcall setValue( 
            /* [in] */ long index,
            /* [in] */ byte value) = 0;
        
        virtual /* [id] */ HRESULT __stdcall getValue( 
            /* [in] */ long index,
            /* [retval][out] */ byte *value) = 0;
        
        virtual /* [id] */ HRESULT __stdcall add( 
            /* [in] */ byte value) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IPCLBoolArrayVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IPCLBoolArray * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IPCLBoolArray * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IPCLBoolArray * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IPCLBoolArray * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IPCLBoolArray * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IPCLBoolArray * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IPCLBoolArray * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [id] */ HRESULT ( __stdcall *getType )( 
            IPCLBoolArray * This,
            /* [out] */ BSTR *type);
        
        /* [id] */ HRESULT ( __stdcall *getRank )( 
            IPCLBoolArray * This,
            /* [retval][out] */ long *rank);
        
        /* [id] */ HRESULT ( __stdcall *getIsReference )( 
            IPCLBoolArray * This,
            /* [retval][out] */ long *isRef);
        
        /* [id] */ HRESULT ( __stdcall *getLastError )( 
            IPCLBoolArray * This,
            /* [out] */ BSTR *message);
        
        /* [id] */ HRESULT ( __stdcall *getCount )( 
            IPCLBoolArray * This,
            /* [retval][out] */ long *count);
        
        /* [id] */ HRESULT ( __stdcall *resize )( 
            IPCLBoolArray * This,
            /* [in] */ long count);
        
        /* [id] */ HRESULT ( __stdcall *getSubArray )( 
            IPCLBoolArray * This,
            /* [in] */ long index,
            /* [retval][out] */ IPCLArray **subArray);
        
        /* [id] */ HRESULT ( __stdcall *getDataPtr )( 
            IPCLBoolArray * This,
            /* [retval][out] */ byte **dataPtr);
        
        /* [id] */ HRESULT ( __stdcall *setValue )( 
            IPCLBoolArray * This,
            /* [in] */ long index,
            /* [in] */ byte value);
        
        /* [id] */ HRESULT ( __stdcall *getValue )( 
            IPCLBoolArray * This,
            /* [in] */ long index,
            /* [retval][out] */ byte *value);
        
        /* [id] */ HRESULT ( __stdcall *add )( 
            IPCLBoolArray * This,
            /* [in] */ byte value);
        
        END_INTERFACE
    } IPCLBoolArrayVtbl;

    interface IPCLBoolArray
    {
        CONST_VTBL struct IPCLBoolArrayVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPCLBoolArray_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IPCLBoolArray_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IPCLBoolArray_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IPCLBoolArray_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IPCLBoolArray_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IPCLBoolArray_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IPCLBoolArray_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IPCLBoolArray_getType(This,type)	\
    (This)->lpVtbl -> getType(This,type)

#define IPCLBoolArray_getRank(This,rank)	\
    (This)->lpVtbl -> getRank(This,rank)

#define IPCLBoolArray_getIsReference(This,isRef)	\
    (This)->lpVtbl -> getIsReference(This,isRef)

#define IPCLBoolArray_getLastError(This,message)	\
    (This)->lpVtbl -> getLastError(This,message)


#define IPCLBoolArray_getCount(This,count)	\
    (This)->lpVtbl -> getCount(This,count)

#define IPCLBoolArray_resize(This,count)	\
    (This)->lpVtbl -> resize(This,count)

#define IPCLBoolArray_getSubArray(This,index,subArray)	\
    (This)->lpVtbl -> getSubArray(This,index,subArray)


#define IPCLBoolArray_getDataPtr(This,dataPtr)	\
    (This)->lpVtbl -> getDataPtr(This,dataPtr)

#define IPCLBoolArray_setValue(This,index,value)	\
    (This)->lpVtbl -> setValue(This,index,value)

#define IPCLBoolArray_getValue(This,index,value)	\
    (This)->lpVtbl -> getValue(This,index,value)

#define IPCLBoolArray_add(This,value)	\
    (This)->lpVtbl -> add(This,value)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [id] */ HRESULT __stdcall IPCLBoolArray_getDataPtr_Proxy( 
    IPCLBoolArray * This,
    /* [retval][out] */ byte **dataPtr);


void __RPC_STUB IPCLBoolArray_getDataPtr_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT __stdcall IPCLBoolArray_setValue_Proxy( 
    IPCLBoolArray * This,
    /* [in] */ long index,
    /* [in] */ byte value);


void __RPC_STUB IPCLBoolArray_setValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT __stdcall IPCLBoolArray_getValue_Proxy( 
    IPCLBoolArray * This,
    /* [in] */ long index,
    /* [retval][out] */ byte *value);


void __RPC_STUB IPCLBoolArray_getValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT __stdcall IPCLBoolArray_add_Proxy( 
    IPCLBoolArray * This,
    /* [in] */ byte value);


void __RPC_STUB IPCLBoolArray_add_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IPCLBoolArray_INTERFACE_DEFINED__ */


#ifndef __IPCLString_INTERFACE_DEFINED__
#define __IPCLString_INTERFACE_DEFINED__

/* interface IPCLString */
/* [object][oleautomation][dual][uuid] */ 


EXTERN_C const IID IID_IPCLString;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("023D3759-685A-4c63-85B3-9A50D545E565")
    IPCLString : public IPCLArgument
    {
    public:
        virtual /* [id] */ HRESULT __stdcall getValue( 
            /* [retval][out] */ BSTR *val) = 0;
        
        virtual /* [id] */ HRESULT __stdcall setValue( 
            /* [in] */ BSTR val) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IPCLStringVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IPCLString * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IPCLString * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IPCLString * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IPCLString * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IPCLString * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IPCLString * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IPCLString * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [id] */ HRESULT ( __stdcall *getType )( 
            IPCLString * This,
            /* [out] */ BSTR *type);
        
        /* [id] */ HRESULT ( __stdcall *getRank )( 
            IPCLString * This,
            /* [retval][out] */ long *rank);
        
        /* [id] */ HRESULT ( __stdcall *getIsReference )( 
            IPCLString * This,
            /* [retval][out] */ long *isRef);
        
        /* [id] */ HRESULT ( __stdcall *getLastError )( 
            IPCLString * This,
            /* [out] */ BSTR *message);
        
        /* [id] */ HRESULT ( __stdcall *getValue )( 
            IPCLString * This,
            /* [retval][out] */ BSTR *val);
        
        /* [id] */ HRESULT ( __stdcall *setValue )( 
            IPCLString * This,
            /* [in] */ BSTR val);
        
        END_INTERFACE
    } IPCLStringVtbl;

    interface IPCLString
    {
        CONST_VTBL struct IPCLStringVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPCLString_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IPCLString_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IPCLString_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IPCLString_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IPCLString_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IPCLString_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IPCLString_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IPCLString_getType(This,type)	\
    (This)->lpVtbl -> getType(This,type)

#define IPCLString_getRank(This,rank)	\
    (This)->lpVtbl -> getRank(This,rank)

#define IPCLString_getIsReference(This,isRef)	\
    (This)->lpVtbl -> getIsReference(This,isRef)

#define IPCLString_getLastError(This,message)	\
    (This)->lpVtbl -> getLastError(This,message)


#define IPCLString_getValue(This,val)	\
    (This)->lpVtbl -> getValue(This,val)

#define IPCLString_setValue(This,val)	\
    (This)->lpVtbl -> setValue(This,val)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [id] */ HRESULT __stdcall IPCLString_getValue_Proxy( 
    IPCLString * This,
    /* [retval][out] */ BSTR *val);


void __RPC_STUB IPCLString_getValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT __stdcall IPCLString_setValue_Proxy( 
    IPCLString * This,
    /* [in] */ BSTR val);


void __RPC_STUB IPCLString_setValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IPCLString_INTERFACE_DEFINED__ */


#ifndef __IPCLInt_INTERFACE_DEFINED__
#define __IPCLInt_INTERFACE_DEFINED__

/* interface IPCLInt */
/* [object][oleautomation][dual][uuid] */ 


EXTERN_C const IID IID_IPCLInt;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("CF00C2BD-1595-4c6d-B2C9-8EB88B79A078")
    IPCLInt : public IPCLArgument
    {
    public:
        virtual /* [id] */ HRESULT __stdcall getValue( 
            /* [retval][out] */ long *val) = 0;
        
        virtual /* [id] */ HRESULT __stdcall setValue( 
            /* [in] */ long val) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IPCLIntVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IPCLInt * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IPCLInt * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IPCLInt * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IPCLInt * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IPCLInt * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IPCLInt * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IPCLInt * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [id] */ HRESULT ( __stdcall *getType )( 
            IPCLInt * This,
            /* [out] */ BSTR *type);
        
        /* [id] */ HRESULT ( __stdcall *getRank )( 
            IPCLInt * This,
            /* [retval][out] */ long *rank);
        
        /* [id] */ HRESULT ( __stdcall *getIsReference )( 
            IPCLInt * This,
            /* [retval][out] */ long *isRef);
        
        /* [id] */ HRESULT ( __stdcall *getLastError )( 
            IPCLInt * This,
            /* [out] */ BSTR *message);
        
        /* [id] */ HRESULT ( __stdcall *getValue )( 
            IPCLInt * This,
            /* [retval][out] */ long *val);
        
        /* [id] */ HRESULT ( __stdcall *setValue )( 
            IPCLInt * This,
            /* [in] */ long val);
        
        END_INTERFACE
    } IPCLIntVtbl;

    interface IPCLInt
    {
        CONST_VTBL struct IPCLIntVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPCLInt_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IPCLInt_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IPCLInt_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IPCLInt_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IPCLInt_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IPCLInt_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IPCLInt_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IPCLInt_getType(This,type)	\
    (This)->lpVtbl -> getType(This,type)

#define IPCLInt_getRank(This,rank)	\
    (This)->lpVtbl -> getRank(This,rank)

#define IPCLInt_getIsReference(This,isRef)	\
    (This)->lpVtbl -> getIsReference(This,isRef)

#define IPCLInt_getLastError(This,message)	\
    (This)->lpVtbl -> getLastError(This,message)


#define IPCLInt_getValue(This,val)	\
    (This)->lpVtbl -> getValue(This,val)

#define IPCLInt_setValue(This,val)	\
    (This)->lpVtbl -> setValue(This,val)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [id] */ HRESULT __stdcall IPCLInt_getValue_Proxy( 
    IPCLInt * This,
    /* [retval][out] */ long *val);


void __RPC_STUB IPCLInt_getValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT __stdcall IPCLInt_setValue_Proxy( 
    IPCLInt * This,
    /* [in] */ long val);


void __RPC_STUB IPCLInt_setValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IPCLInt_INTERFACE_DEFINED__ */


#ifndef __IPCLDouble_INTERFACE_DEFINED__
#define __IPCLDouble_INTERFACE_DEFINED__

/* interface IPCLDouble */
/* [object][oleautomation][dual][uuid] */ 


EXTERN_C const IID IID_IPCLDouble;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("C41A6728-6307-4846-883C-0C8BF4AACA17")
    IPCLDouble : public IPCLArgument
    {
    public:
        virtual /* [id] */ HRESULT __stdcall getValue( 
            /* [retval][out] */ double *val) = 0;
        
        virtual /* [id] */ HRESULT __stdcall setValue( 
            /* [in] */ double val) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IPCLDoubleVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IPCLDouble * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IPCLDouble * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IPCLDouble * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IPCLDouble * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IPCLDouble * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IPCLDouble * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IPCLDouble * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [id] */ HRESULT ( __stdcall *getType )( 
            IPCLDouble * This,
            /* [out] */ BSTR *type);
        
        /* [id] */ HRESULT ( __stdcall *getRank )( 
            IPCLDouble * This,
            /* [retval][out] */ long *rank);
        
        /* [id] */ HRESULT ( __stdcall *getIsReference )( 
            IPCLDouble * This,
            /* [retval][out] */ long *isRef);
        
        /* [id] */ HRESULT ( __stdcall *getLastError )( 
            IPCLDouble * This,
            /* [out] */ BSTR *message);
        
        /* [id] */ HRESULT ( __stdcall *getValue )( 
            IPCLDouble * This,
            /* [retval][out] */ double *val);
        
        /* [id] */ HRESULT ( __stdcall *setValue )( 
            IPCLDouble * This,
            /* [in] */ double val);
        
        END_INTERFACE
    } IPCLDoubleVtbl;

    interface IPCLDouble
    {
        CONST_VTBL struct IPCLDoubleVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPCLDouble_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IPCLDouble_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IPCLDouble_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IPCLDouble_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IPCLDouble_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IPCLDouble_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IPCLDouble_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IPCLDouble_getType(This,type)	\
    (This)->lpVtbl -> getType(This,type)

#define IPCLDouble_getRank(This,rank)	\
    (This)->lpVtbl -> getRank(This,rank)

#define IPCLDouble_getIsReference(This,isRef)	\
    (This)->lpVtbl -> getIsReference(This,isRef)

#define IPCLDouble_getLastError(This,message)	\
    (This)->lpVtbl -> getLastError(This,message)


#define IPCLDouble_getValue(This,val)	\
    (This)->lpVtbl -> getValue(This,val)

#define IPCLDouble_setValue(This,val)	\
    (This)->lpVtbl -> setValue(This,val)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [id] */ HRESULT __stdcall IPCLDouble_getValue_Proxy( 
    IPCLDouble * This,
    /* [retval][out] */ double *val);


void __RPC_STUB IPCLDouble_getValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT __stdcall IPCLDouble_setValue_Proxy( 
    IPCLDouble * This,
    /* [in] */ double val);


void __RPC_STUB IPCLDouble_setValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IPCLDouble_INTERFACE_DEFINED__ */


#ifndef __IPCLBool_INTERFACE_DEFINED__
#define __IPCLBool_INTERFACE_DEFINED__

/* interface IPCLBool */
/* [object][oleautomation][dual][uuid] */ 


EXTERN_C const IID IID_IPCLBool;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("525A6207-B78E-47b1-AB8E-4DED017B299D")
    IPCLBool : public IPCLArgument
    {
    public:
        virtual /* [id] */ HRESULT __stdcall getValue( 
            /* [retval][out] */ long *val) = 0;
        
        virtual /* [id] */ HRESULT __stdcall setValue( 
            /* [in] */ long val) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IPCLBoolVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IPCLBool * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IPCLBool * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IPCLBool * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IPCLBool * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IPCLBool * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IPCLBool * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IPCLBool * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [id] */ HRESULT ( __stdcall *getType )( 
            IPCLBool * This,
            /* [out] */ BSTR *type);
        
        /* [id] */ HRESULT ( __stdcall *getRank )( 
            IPCLBool * This,
            /* [retval][out] */ long *rank);
        
        /* [id] */ HRESULT ( __stdcall *getIsReference )( 
            IPCLBool * This,
            /* [retval][out] */ long *isRef);
        
        /* [id] */ HRESULT ( __stdcall *getLastError )( 
            IPCLBool * This,
            /* [out] */ BSTR *message);
        
        /* [id] */ HRESULT ( __stdcall *getValue )( 
            IPCLBool * This,
            /* [retval][out] */ long *val);
        
        /* [id] */ HRESULT ( __stdcall *setValue )( 
            IPCLBool * This,
            /* [in] */ long val);
        
        END_INTERFACE
    } IPCLBoolVtbl;

    interface IPCLBool
    {
        CONST_VTBL struct IPCLBoolVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPCLBool_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IPCLBool_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IPCLBool_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IPCLBool_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IPCLBool_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IPCLBool_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IPCLBool_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IPCLBool_getType(This,type)	\
    (This)->lpVtbl -> getType(This,type)

#define IPCLBool_getRank(This,rank)	\
    (This)->lpVtbl -> getRank(This,rank)

#define IPCLBool_getIsReference(This,isRef)	\
    (This)->lpVtbl -> getIsReference(This,isRef)

#define IPCLBool_getLastError(This,message)	\
    (This)->lpVtbl -> getLastError(This,message)


#define IPCLBool_getValue(This,val)	\
    (This)->lpVtbl -> getValue(This,val)

#define IPCLBool_setValue(This,val)	\
    (This)->lpVtbl -> setValue(This,val)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [id] */ HRESULT __stdcall IPCLBool_getValue_Proxy( 
    IPCLBool * This,
    /* [retval][out] */ long *val);


void __RPC_STUB IPCLBool_getValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT __stdcall IPCLBool_setValue_Proxy( 
    IPCLBool * This,
    /* [in] */ long val);


void __RPC_STUB IPCLBool_setValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IPCLBool_INTERFACE_DEFINED__ */


#ifndef __IPCLArguments_INTERFACE_DEFINED__
#define __IPCLArguments_INTERFACE_DEFINED__

/* interface IPCLArguments */
/* [object][oleautomation][dual][uuid] */ 


EXTERN_C const IID IID_IPCLArguments;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("B8D5BA56-01D5-4237-92A1-73C972C8470B")
    IPCLArguments : public IDispatch
    {
    public:
        virtual /* [id] */ HRESULT __stdcall getCount( 
            /* [retval][out] */ long *count) = 0;
        
        virtual /* [id] */ HRESULT __stdcall getArgument( 
            /* [in] */ long index,
            /* [retval][out] */ IPCLArgument **argument) = 0;
        
        virtual /* [id] */ HRESULT __stdcall getReturnValue( 
            /* [retval][out] */ IPCLArgument **value) = 0;
        
        virtual /* [id] */ HRESULT __stdcall getLastError( 
            /* [out] */ BSTR *message) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IPCLArgumentsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IPCLArguments * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IPCLArguments * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IPCLArguments * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IPCLArguments * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IPCLArguments * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IPCLArguments * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IPCLArguments * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [id] */ HRESULT ( __stdcall *getCount )( 
            IPCLArguments * This,
            /* [retval][out] */ long *count);
        
        /* [id] */ HRESULT ( __stdcall *getArgument )( 
            IPCLArguments * This,
            /* [in] */ long index,
            /* [retval][out] */ IPCLArgument **argument);
        
        /* [id] */ HRESULT ( __stdcall *getReturnValue )( 
            IPCLArguments * This,
            /* [retval][out] */ IPCLArgument **value);
        
        /* [id] */ HRESULT ( __stdcall *getLastError )( 
            IPCLArguments * This,
            /* [out] */ BSTR *message);
        
        END_INTERFACE
    } IPCLArgumentsVtbl;

    interface IPCLArguments
    {
        CONST_VTBL struct IPCLArgumentsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPCLArguments_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IPCLArguments_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IPCLArguments_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IPCLArguments_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IPCLArguments_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IPCLArguments_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IPCLArguments_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IPCLArguments_getCount(This,count)	\
    (This)->lpVtbl -> getCount(This,count)

#define IPCLArguments_getArgument(This,index,argument)	\
    (This)->lpVtbl -> getArgument(This,index,argument)

#define IPCLArguments_getReturnValue(This,value)	\
    (This)->lpVtbl -> getReturnValue(This,value)

#define IPCLArguments_getLastError(This,message)	\
    (This)->lpVtbl -> getLastError(This,message)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [id] */ HRESULT __stdcall IPCLArguments_getCount_Proxy( 
    IPCLArguments * This,
    /* [retval][out] */ long *count);


void __RPC_STUB IPCLArguments_getCount_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT __stdcall IPCLArguments_getArgument_Proxy( 
    IPCLArguments * This,
    /* [in] */ long index,
    /* [retval][out] */ IPCLArgument **argument);


void __RPC_STUB IPCLArguments_getArgument_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT __stdcall IPCLArguments_getReturnValue_Proxy( 
    IPCLArguments * This,
    /* [retval][out] */ IPCLArgument **value);


void __RPC_STUB IPCLArguments_getReturnValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT __stdcall IPCLArguments_getLastError_Proxy( 
    IPCLArguments * This,
    /* [out] */ BSTR *message);


void __RPC_STUB IPCLArguments_getLastError_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IPCLArguments_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_PCLExtension_0130 */
/* [local] */ 




extern RPC_IF_HANDLE __MIDL_itf_PCLExtension_0130_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_PCLExtension_0130_v0_0_s_ifspec;

#ifndef __IPCLObject_INTERFACE_DEFINED__
#define __IPCLObject_INTERFACE_DEFINED__

/* interface IPCLObject */
/* [object][oleautomation][dual][uuid] */ 


EXTERN_C const IID IID_IPCLObject;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("83CE0178-FAE2-4734-8A90-31389543E4AE")
    IPCLObject : public IDispatch
    {
    public:
        virtual /* [id] */ HRESULT __stdcall callMethod( 
            /* [in] */ long methodIndex,
            /* [in] */ IPCLArguments *arguments) = 0;
        
        virtual /* [id] */ HRESULT __stdcall getType( 
            /* [out] */ IPCLType **type) = 0;
        
        virtual /* [id] */ HRESULT __stdcall getLastError( 
            /* [out] */ BSTR *message) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IPCLObjectVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IPCLObject * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IPCLObject * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IPCLObject * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IPCLObject * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IPCLObject * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IPCLObject * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IPCLObject * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [id] */ HRESULT ( __stdcall *callMethod )( 
            IPCLObject * This,
            /* [in] */ long methodIndex,
            /* [in] */ IPCLArguments *arguments);
        
        /* [id] */ HRESULT ( __stdcall *getType )( 
            IPCLObject * This,
            /* [out] */ IPCLType **type);
        
        /* [id] */ HRESULT ( __stdcall *getLastError )( 
            IPCLObject * This,
            /* [out] */ BSTR *message);
        
        END_INTERFACE
    } IPCLObjectVtbl;

    interface IPCLObject
    {
        CONST_VTBL struct IPCLObjectVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPCLObject_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IPCLObject_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IPCLObject_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IPCLObject_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IPCLObject_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IPCLObject_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IPCLObject_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IPCLObject_callMethod(This,methodIndex,arguments)	\
    (This)->lpVtbl -> callMethod(This,methodIndex,arguments)

#define IPCLObject_getType(This,type)	\
    (This)->lpVtbl -> getType(This,type)

#define IPCLObject_getLastError(This,message)	\
    (This)->lpVtbl -> getLastError(This,message)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [id] */ HRESULT __stdcall IPCLObject_callMethod_Proxy( 
    IPCLObject * This,
    /* [in] */ long methodIndex,
    /* [in] */ IPCLArguments *arguments);


void __RPC_STUB IPCLObject_callMethod_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT __stdcall IPCLObject_getType_Proxy( 
    IPCLObject * This,
    /* [out] */ IPCLType **type);


void __RPC_STUB IPCLObject_getType_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT __stdcall IPCLObject_getLastError_Proxy( 
    IPCLObject * This,
    /* [out] */ BSTR *message);


void __RPC_STUB IPCLObject_getLastError_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IPCLObject_INTERFACE_DEFINED__ */


#ifndef __IPCLExtensionObject_INTERFACE_DEFINED__
#define __IPCLExtensionObject_INTERFACE_DEFINED__

/* interface IPCLExtensionObject */
/* [object][oleautomation][dual][uuid] */ 


EXTERN_C const IID IID_IPCLExtensionObject;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("0DECE6D3-85AE-4d3c-953A-15A55504C9E6")
    IPCLExtensionObject : public IPCLArgument
    {
    public:
        virtual /* [id] */ HRESULT __stdcall getValue( 
            /* [retval][out] */ IPCLObject **val) = 0;
        
        virtual /* [id] */ HRESULT __stdcall setValue( 
            /* [in] */ IPCLObject *val) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IPCLExtensionObjectVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IPCLExtensionObject * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IPCLExtensionObject * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IPCLExtensionObject * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IPCLExtensionObject * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IPCLExtensionObject * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IPCLExtensionObject * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IPCLExtensionObject * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [id] */ HRESULT ( __stdcall *getType )( 
            IPCLExtensionObject * This,
            /* [out] */ BSTR *type);
        
        /* [id] */ HRESULT ( __stdcall *getRank )( 
            IPCLExtensionObject * This,
            /* [retval][out] */ long *rank);
        
        /* [id] */ HRESULT ( __stdcall *getIsReference )( 
            IPCLExtensionObject * This,
            /* [retval][out] */ long *isRef);
        
        /* [id] */ HRESULT ( __stdcall *getLastError )( 
            IPCLExtensionObject * This,
            /* [out] */ BSTR *message);
        
        /* [id] */ HRESULT ( __stdcall *getValue )( 
            IPCLExtensionObject * This,
            /* [retval][out] */ IPCLObject **val);
        
        /* [id] */ HRESULT ( __stdcall *setValue )( 
            IPCLExtensionObject * This,
            /* [in] */ IPCLObject *val);
        
        END_INTERFACE
    } IPCLExtensionObjectVtbl;

    interface IPCLExtensionObject
    {
        CONST_VTBL struct IPCLExtensionObjectVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPCLExtensionObject_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IPCLExtensionObject_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IPCLExtensionObject_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IPCLExtensionObject_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IPCLExtensionObject_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IPCLExtensionObject_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IPCLExtensionObject_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IPCLExtensionObject_getType(This,type)	\
    (This)->lpVtbl -> getType(This,type)

#define IPCLExtensionObject_getRank(This,rank)	\
    (This)->lpVtbl -> getRank(This,rank)

#define IPCLExtensionObject_getIsReference(This,isRef)	\
    (This)->lpVtbl -> getIsReference(This,isRef)

#define IPCLExtensionObject_getLastError(This,message)	\
    (This)->lpVtbl -> getLastError(This,message)


#define IPCLExtensionObject_getValue(This,val)	\
    (This)->lpVtbl -> getValue(This,val)

#define IPCLExtensionObject_setValue(This,val)	\
    (This)->lpVtbl -> setValue(This,val)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [id] */ HRESULT __stdcall IPCLExtensionObject_getValue_Proxy( 
    IPCLExtensionObject * This,
    /* [retval][out] */ IPCLObject **val);


void __RPC_STUB IPCLExtensionObject_getValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT __stdcall IPCLExtensionObject_setValue_Proxy( 
    IPCLExtensionObject * This,
    /* [in] */ IPCLObject *val);


void __RPC_STUB IPCLExtensionObject_setValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IPCLExtensionObject_INTERFACE_DEFINED__ */


#ifndef __IPCLMethod_INTERFACE_DEFINED__
#define __IPCLMethod_INTERFACE_DEFINED__

/* interface IPCLMethod */
/* [object][oleautomation][dual][uuid] */ 


EXTERN_C const IID IID_IPCLMethod;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("B74B2585-4C45-4747-A297-9ED841F4731D")
    IPCLMethod : public IDispatch
    {
    public:
        virtual /* [id] */ HRESULT __stdcall setName( 
            /* [in] */ BSTR name) = 0;
        
        virtual /* [id] */ HRESULT __stdcall addArgument( 
            /* [in] */ BSTR name,
            /* [in] */ BSTR type,
            /* [in] */ long rank,
            /* [in] */ long isReference,
            /* [in] */ BSTR description) = 0;
        
        virtual /* [id] */ HRESULT __stdcall setReturnValue( 
            /* [in] */ BSTR type,
            /* [in] */ long rank,
            /* [in] */ BSTR description) = 0;
        
        virtual /* [id] */ HRESULT __stdcall setDescription( 
            /* [in] */ BSTR desc) = 0;
        
        virtual /* [id] */ HRESULT __stdcall getLastError( 
            /* [out] */ BSTR *message) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IPCLMethodVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IPCLMethod * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IPCLMethod * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IPCLMethod * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IPCLMethod * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IPCLMethod * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IPCLMethod * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IPCLMethod * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [id] */ HRESULT ( __stdcall *setName )( 
            IPCLMethod * This,
            /* [in] */ BSTR name);
        
        /* [id] */ HRESULT ( __stdcall *addArgument )( 
            IPCLMethod * This,
            /* [in] */ BSTR name,
            /* [in] */ BSTR type,
            /* [in] */ long rank,
            /* [in] */ long isReference,
            /* [in] */ BSTR description);
        
        /* [id] */ HRESULT ( __stdcall *setReturnValue )( 
            IPCLMethod * This,
            /* [in] */ BSTR type,
            /* [in] */ long rank,
            /* [in] */ BSTR description);
        
        /* [id] */ HRESULT ( __stdcall *setDescription )( 
            IPCLMethod * This,
            /* [in] */ BSTR desc);
        
        /* [id] */ HRESULT ( __stdcall *getLastError )( 
            IPCLMethod * This,
            /* [out] */ BSTR *message);
        
        END_INTERFACE
    } IPCLMethodVtbl;

    interface IPCLMethod
    {
        CONST_VTBL struct IPCLMethodVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPCLMethod_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IPCLMethod_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IPCLMethod_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IPCLMethod_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IPCLMethod_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IPCLMethod_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IPCLMethod_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IPCLMethod_setName(This,name)	\
    (This)->lpVtbl -> setName(This,name)

#define IPCLMethod_addArgument(This,name,type,rank,isReference,description)	\
    (This)->lpVtbl -> addArgument(This,name,type,rank,isReference,description)

#define IPCLMethod_setReturnValue(This,type,rank,description)	\
    (This)->lpVtbl -> setReturnValue(This,type,rank,description)

#define IPCLMethod_setDescription(This,desc)	\
    (This)->lpVtbl -> setDescription(This,desc)

#define IPCLMethod_getLastError(This,message)	\
    (This)->lpVtbl -> getLastError(This,message)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [id] */ HRESULT __stdcall IPCLMethod_setName_Proxy( 
    IPCLMethod * This,
    /* [in] */ BSTR name);


void __RPC_STUB IPCLMethod_setName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT __stdcall IPCLMethod_addArgument_Proxy( 
    IPCLMethod * This,
    /* [in] */ BSTR name,
    /* [in] */ BSTR type,
    /* [in] */ long rank,
    /* [in] */ long isReference,
    /* [in] */ BSTR description);


void __RPC_STUB IPCLMethod_addArgument_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT __stdcall IPCLMethod_setReturnValue_Proxy( 
    IPCLMethod * This,
    /* [in] */ BSTR type,
    /* [in] */ long rank,
    /* [in] */ BSTR description);


void __RPC_STUB IPCLMethod_setReturnValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT __stdcall IPCLMethod_setDescription_Proxy( 
    IPCLMethod * This,
    /* [in] */ BSTR desc);


void __RPC_STUB IPCLMethod_setDescription_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT __stdcall IPCLMethod_getLastError_Proxy( 
    IPCLMethod * This,
    /* [out] */ BSTR *message);


void __RPC_STUB IPCLMethod_getLastError_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IPCLMethod_INTERFACE_DEFINED__ */


#ifndef __IPCLMethodList_INTERFACE_DEFINED__
#define __IPCLMethodList_INTERFACE_DEFINED__

/* interface IPCLMethodList */
/* [object][oleautomation][dual][uuid] */ 


EXTERN_C const IID IID_IPCLMethodList;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("721705E4-275F-4aed-BFA9-6957E40BDE4F")
    IPCLMethodList : public IDispatch
    {
    public:
        virtual /* [id] */ HRESULT __stdcall addMethod( 
            /* [out] */ IPCLMethod **method) = 0;
        
        virtual /* [id] */ HRESULT __stdcall addNew( 
            /* [out] */ IPCLMethod **method) = 0;
        
        virtual /* [id] */ HRESULT __stdcall addPredefinedVariable( 
            /* [in] */ BSTR type,
            /* [in] */ BSTR name,
            /* [in] */ BSTR value) = 0;
        
        virtual /* [id] */ HRESULT __stdcall getLastError( 
            /* [out] */ BSTR *message) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IPCLMethodListVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IPCLMethodList * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IPCLMethodList * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IPCLMethodList * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IPCLMethodList * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IPCLMethodList * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IPCLMethodList * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IPCLMethodList * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [id] */ HRESULT ( __stdcall *addMethod )( 
            IPCLMethodList * This,
            /* [out] */ IPCLMethod **method);
        
        /* [id] */ HRESULT ( __stdcall *addNew )( 
            IPCLMethodList * This,
            /* [out] */ IPCLMethod **method);
        
        /* [id] */ HRESULT ( __stdcall *addPredefinedVariable )( 
            IPCLMethodList * This,
            /* [in] */ BSTR type,
            /* [in] */ BSTR name,
            /* [in] */ BSTR value);
        
        /* [id] */ HRESULT ( __stdcall *getLastError )( 
            IPCLMethodList * This,
            /* [out] */ BSTR *message);
        
        END_INTERFACE
    } IPCLMethodListVtbl;

    interface IPCLMethodList
    {
        CONST_VTBL struct IPCLMethodListVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPCLMethodList_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IPCLMethodList_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IPCLMethodList_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IPCLMethodList_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IPCLMethodList_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IPCLMethodList_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IPCLMethodList_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IPCLMethodList_addMethod(This,method)	\
    (This)->lpVtbl -> addMethod(This,method)

#define IPCLMethodList_addNew(This,method)	\
    (This)->lpVtbl -> addNew(This,method)

#define IPCLMethodList_addPredefinedVariable(This,type,name,value)	\
    (This)->lpVtbl -> addPredefinedVariable(This,type,name,value)

#define IPCLMethodList_getLastError(This,message)	\
    (This)->lpVtbl -> getLastError(This,message)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [id] */ HRESULT __stdcall IPCLMethodList_addMethod_Proxy( 
    IPCLMethodList * This,
    /* [out] */ IPCLMethod **method);


void __RPC_STUB IPCLMethodList_addMethod_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT __stdcall IPCLMethodList_addNew_Proxy( 
    IPCLMethodList * This,
    /* [out] */ IPCLMethod **method);


void __RPC_STUB IPCLMethodList_addNew_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT __stdcall IPCLMethodList_addPredefinedVariable_Proxy( 
    IPCLMethodList * This,
    /* [in] */ BSTR type,
    /* [in] */ BSTR name,
    /* [in] */ BSTR value);


void __RPC_STUB IPCLMethodList_addPredefinedVariable_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT __stdcall IPCLMethodList_getLastError_Proxy( 
    IPCLMethodList * This,
    /* [out] */ BSTR *message);


void __RPC_STUB IPCLMethodList_getLastError_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IPCLMethodList_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_PCLExtension_0134 */
/* [local] */ 




extern RPC_IF_HANDLE __MIDL_itf_PCLExtension_0134_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_PCLExtension_0134_v0_0_s_ifspec;

#ifndef __IPCLType_INTERFACE_DEFINED__
#define __IPCLType_INTERFACE_DEFINED__

/* interface IPCLType */
/* [object][oleautomation][dual][uuid] */ 


EXTERN_C const IID IID_IPCLType;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("12349DAF-FD20-4ba6-B500-0FC8E0E49A0B")
    IPCLType : public IDispatch
    {
    public:
        virtual /* [id] */ HRESULT __stdcall getName( 
            /* [out] */ BSTR *name) = 0;
        
        virtual /* [id] */ HRESULT __stdcall getDescription( 
            /* [out] */ BSTR *desc) = 0;
        
        virtual /* [id] */ HRESULT __stdcall getParentName( 
            /* [out] */ BSTR *parent) = 0;
        
        virtual /* [id] */ HRESULT __stdcall getMethods( 
            /* [in] */ IPCLMethodList *list) = 0;
        
        virtual /* [id] */ HRESULT __stdcall createObject( 
            /* [in] */ long newIndex,
            /* [in] */ IPCLArguments *arguments,
            /* [out] */ IPCLObject **object) = 0;
        
        virtual /* [id] */ HRESULT __stdcall getLibrary( 
            /* [out] */ IPCLLibrary **lib) = 0;
        
        virtual /* [id] */ HRESULT __stdcall getLastError( 
            /* [out] */ BSTR *message) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IPCLTypeVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IPCLType * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IPCLType * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IPCLType * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IPCLType * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IPCLType * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IPCLType * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IPCLType * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [id] */ HRESULT ( __stdcall *getName )( 
            IPCLType * This,
            /* [out] */ BSTR *name);
        
        /* [id] */ HRESULT ( __stdcall *getDescription )( 
            IPCLType * This,
            /* [out] */ BSTR *desc);
        
        /* [id] */ HRESULT ( __stdcall *getParentName )( 
            IPCLType * This,
            /* [out] */ BSTR *parent);
        
        /* [id] */ HRESULT ( __stdcall *getMethods )( 
            IPCLType * This,
            /* [in] */ IPCLMethodList *list);
        
        /* [id] */ HRESULT ( __stdcall *createObject )( 
            IPCLType * This,
            /* [in] */ long newIndex,
            /* [in] */ IPCLArguments *arguments,
            /* [out] */ IPCLObject **object);
        
        /* [id] */ HRESULT ( __stdcall *getLibrary )( 
            IPCLType * This,
            /* [out] */ IPCLLibrary **lib);
        
        /* [id] */ HRESULT ( __stdcall *getLastError )( 
            IPCLType * This,
            /* [out] */ BSTR *message);
        
        END_INTERFACE
    } IPCLTypeVtbl;

    interface IPCLType
    {
        CONST_VTBL struct IPCLTypeVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPCLType_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IPCLType_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IPCLType_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IPCLType_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IPCLType_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IPCLType_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IPCLType_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IPCLType_getName(This,name)	\
    (This)->lpVtbl -> getName(This,name)

#define IPCLType_getDescription(This,desc)	\
    (This)->lpVtbl -> getDescription(This,desc)

#define IPCLType_getParentName(This,parent)	\
    (This)->lpVtbl -> getParentName(This,parent)

#define IPCLType_getMethods(This,list)	\
    (This)->lpVtbl -> getMethods(This,list)

#define IPCLType_createObject(This,newIndex,arguments,object)	\
    (This)->lpVtbl -> createObject(This,newIndex,arguments,object)

#define IPCLType_getLibrary(This,lib)	\
    (This)->lpVtbl -> getLibrary(This,lib)

#define IPCLType_getLastError(This,message)	\
    (This)->lpVtbl -> getLastError(This,message)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [id] */ HRESULT __stdcall IPCLType_getName_Proxy( 
    IPCLType * This,
    /* [out] */ BSTR *name);


void __RPC_STUB IPCLType_getName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT __stdcall IPCLType_getDescription_Proxy( 
    IPCLType * This,
    /* [out] */ BSTR *desc);


void __RPC_STUB IPCLType_getDescription_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT __stdcall IPCLType_getParentName_Proxy( 
    IPCLType * This,
    /* [out] */ BSTR *parent);


void __RPC_STUB IPCLType_getParentName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT __stdcall IPCLType_getMethods_Proxy( 
    IPCLType * This,
    /* [in] */ IPCLMethodList *list);


void __RPC_STUB IPCLType_getMethods_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT __stdcall IPCLType_createObject_Proxy( 
    IPCLType * This,
    /* [in] */ long newIndex,
    /* [in] */ IPCLArguments *arguments,
    /* [out] */ IPCLObject **object);


void __RPC_STUB IPCLType_createObject_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT __stdcall IPCLType_getLibrary_Proxy( 
    IPCLType * This,
    /* [out] */ IPCLLibrary **lib);


void __RPC_STUB IPCLType_getLibrary_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT __stdcall IPCLType_getLastError_Proxy( 
    IPCLType * This,
    /* [out] */ BSTR *message);


void __RPC_STUB IPCLType_getLastError_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IPCLType_INTERFACE_DEFINED__ */


#ifndef __IPCLLibrary_INTERFACE_DEFINED__
#define __IPCLLibrary_INTERFACE_DEFINED__

/* interface IPCLLibrary */
/* [object][oleautomation][dual][uuid] */ 


EXTERN_C const IID IID_IPCLLibrary;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("5483302B-931F-4506-A9D0-30F5C22348CB")
    IPCLLibrary : public IDispatch
    {
    public:
        virtual /* [id] */ HRESULT __stdcall getName( 
            /* [out] */ BSTR *name) = 0;
        
        virtual /* [id] */ HRESULT __stdcall getAuthor( 
            /* [out] */ BSTR *author) = 0;
        
        virtual /* [id] */ HRESULT __stdcall getVersion( 
            /* [out] */ BSTR *ver) = 0;
        
        virtual /* [id] */ HRESULT __stdcall getDescription( 
            /* [out] */ BSTR *desc) = 0;
        
        virtual /* [id] */ HRESULT __stdcall getPresentationMinimumVersion( 
            /* [out] */ double *ver) = 0;
        
        virtual /* [id] */ HRESULT __stdcall getNamespaceName( 
            /* [out] */ BSTR *ns) = 0;
        
        virtual /* [id] */ HRESULT __stdcall getTypeCount( 
            /* [out] */ long *count) = 0;
        
        virtual /* [id] */ HRESULT __stdcall getType( 
            /* [in] */ long index,
            /* [out] */ IPCLType **type) = 0;
        
        virtual /* [id] */ HRESULT __stdcall getLastError( 
            /* [out] */ BSTR *message) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IPCLLibraryVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IPCLLibrary * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IPCLLibrary * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IPCLLibrary * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IPCLLibrary * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IPCLLibrary * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IPCLLibrary * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IPCLLibrary * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [id] */ HRESULT ( __stdcall *getName )( 
            IPCLLibrary * This,
            /* [out] */ BSTR *name);
        
        /* [id] */ HRESULT ( __stdcall *getAuthor )( 
            IPCLLibrary * This,
            /* [out] */ BSTR *author);
        
        /* [id] */ HRESULT ( __stdcall *getVersion )( 
            IPCLLibrary * This,
            /* [out] */ BSTR *ver);
        
        /* [id] */ HRESULT ( __stdcall *getDescription )( 
            IPCLLibrary * This,
            /* [out] */ BSTR *desc);
        
        /* [id] */ HRESULT ( __stdcall *getPresentationMinimumVersion )( 
            IPCLLibrary * This,
            /* [out] */ double *ver);
        
        /* [id] */ HRESULT ( __stdcall *getNamespaceName )( 
            IPCLLibrary * This,
            /* [out] */ BSTR *ns);
        
        /* [id] */ HRESULT ( __stdcall *getTypeCount )( 
            IPCLLibrary * This,
            /* [out] */ long *count);
        
        /* [id] */ HRESULT ( __stdcall *getType )( 
            IPCLLibrary * This,
            /* [in] */ long index,
            /* [out] */ IPCLType **type);
        
        /* [id] */ HRESULT ( __stdcall *getLastError )( 
            IPCLLibrary * This,
            /* [out] */ BSTR *message);
        
        END_INTERFACE
    } IPCLLibraryVtbl;

    interface IPCLLibrary
    {
        CONST_VTBL struct IPCLLibraryVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPCLLibrary_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IPCLLibrary_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IPCLLibrary_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IPCLLibrary_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IPCLLibrary_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IPCLLibrary_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IPCLLibrary_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IPCLLibrary_getName(This,name)	\
    (This)->lpVtbl -> getName(This,name)

#define IPCLLibrary_getAuthor(This,author)	\
    (This)->lpVtbl -> getAuthor(This,author)

#define IPCLLibrary_getVersion(This,ver)	\
    (This)->lpVtbl -> getVersion(This,ver)

#define IPCLLibrary_getDescription(This,desc)	\
    (This)->lpVtbl -> getDescription(This,desc)

#define IPCLLibrary_getPresentationMinimumVersion(This,ver)	\
    (This)->lpVtbl -> getPresentationMinimumVersion(This,ver)

#define IPCLLibrary_getNamespaceName(This,ns)	\
    (This)->lpVtbl -> getNamespaceName(This,ns)

#define IPCLLibrary_getTypeCount(This,count)	\
    (This)->lpVtbl -> getTypeCount(This,count)

#define IPCLLibrary_getType(This,index,type)	\
    (This)->lpVtbl -> getType(This,index,type)

#define IPCLLibrary_getLastError(This,message)	\
    (This)->lpVtbl -> getLastError(This,message)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [id] */ HRESULT __stdcall IPCLLibrary_getName_Proxy( 
    IPCLLibrary * This,
    /* [out] */ BSTR *name);


void __RPC_STUB IPCLLibrary_getName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT __stdcall IPCLLibrary_getAuthor_Proxy( 
    IPCLLibrary * This,
    /* [out] */ BSTR *author);


void __RPC_STUB IPCLLibrary_getAuthor_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT __stdcall IPCLLibrary_getVersion_Proxy( 
    IPCLLibrary * This,
    /* [out] */ BSTR *ver);


void __RPC_STUB IPCLLibrary_getVersion_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT __stdcall IPCLLibrary_getDescription_Proxy( 
    IPCLLibrary * This,
    /* [out] */ BSTR *desc);


void __RPC_STUB IPCLLibrary_getDescription_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT __stdcall IPCLLibrary_getPresentationMinimumVersion_Proxy( 
    IPCLLibrary * This,
    /* [out] */ double *ver);


void __RPC_STUB IPCLLibrary_getPresentationMinimumVersion_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT __stdcall IPCLLibrary_getNamespaceName_Proxy( 
    IPCLLibrary * This,
    /* [out] */ BSTR *ns);


void __RPC_STUB IPCLLibrary_getNamespaceName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT __stdcall IPCLLibrary_getTypeCount_Proxy( 
    IPCLLibrary * This,
    /* [out] */ long *count);


void __RPC_STUB IPCLLibrary_getTypeCount_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT __stdcall IPCLLibrary_getType_Proxy( 
    IPCLLibrary * This,
    /* [in] */ long index,
    /* [out] */ IPCLType **type);


void __RPC_STUB IPCLLibrary_getType_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT __stdcall IPCLLibrary_getLastError_Proxy( 
    IPCLLibrary * This,
    /* [out] */ BSTR *message);


void __RPC_STUB IPCLLibrary_getLastError_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IPCLLibrary_INTERFACE_DEFINED__ */



#ifndef __PCLExtension_LIBRARY_DEFINED__
#define __PCLExtension_LIBRARY_DEFINED__

/* library PCLExtension */
/* [uuid] */ 























EXTERN_C const IID LIBID_PCLExtension;
#endif /* __PCLExtension_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long *, unsigned long            , BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserMarshal(  unsigned long *, unsigned char *, BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserUnmarshal(unsigned long *, unsigned char *, BSTR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long *, BSTR * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


