
#pragma warning( disable: 4049 )  /* more than 64k source lines */

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 6.00.0347 */
/* at Wed Feb 11 12:17:23 2004
 */
/* Compiler settings for C:\development\Versions\NervusSDK33\FileAccessSDK33\FileAccess3.idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 440
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

#ifndef __FileAccess3_h__
#define __FileAccess3_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IAsciiWrite_FWD_DEFINED__
#define __IAsciiWrite_FWD_DEFINED__
typedef interface IAsciiWrite IAsciiWrite;
#endif 	/* __IAsciiWrite_FWD_DEFINED__ */


#ifndef __IFileRead3_FWD_DEFINED__
#define __IFileRead3_FWD_DEFINED__
typedef interface IFileRead3 IFileRead3;
#endif 	/* __IFileRead3_FWD_DEFINED__ */


#ifndef __IFileWrite3_FWD_DEFINED__
#define __IFileWrite3_FWD_DEFINED__
typedef interface IFileWrite3 IFileWrite3;
#endif 	/* __IFileWrite3_FWD_DEFINED__ */


#ifndef __IEegStorage_FWD_DEFINED__
#define __IEegStorage_FWD_DEFINED__
typedef interface IEegStorage IEegStorage;
#endif 	/* __IEegStorage_FWD_DEFINED__ */


#ifndef __IEegSegments_FWD_DEFINED__
#define __IEegSegments_FWD_DEFINED__
typedef interface IEegSegments IEegSegments;
#endif 	/* __IEegSegments_FWD_DEFINED__ */


#ifndef __IInitEegSegments_FWD_DEFINED__
#define __IInitEegSegments_FWD_DEFINED__
typedef interface IInitEegSegments IInitEegSegments;
#endif 	/* __IInitEegSegments_FWD_DEFINED__ */


#ifndef __IEegData_FWD_DEFINED__
#define __IEegData_FWD_DEFINED__
typedef interface IEegData IEegData;
#endif 	/* __IEegData_FWD_DEFINED__ */


#ifndef __IInitEegData_FWD_DEFINED__
#define __IInitEegData_FWD_DEFINED__
typedef interface IInitEegData IInitEegData;
#endif 	/* __IInitEegData_FWD_DEFINED__ */


#ifndef __IUserEvent_FWD_DEFINED__
#define __IUserEvent_FWD_DEFINED__
typedef interface IUserEvent IUserEvent;
#endif 	/* __IUserEvent_FWD_DEFINED__ */


#ifndef __IEventWrapper_FWD_DEFINED__
#define __IEventWrapper_FWD_DEFINED__
typedef interface IEventWrapper IEventWrapper;
#endif 	/* __IEventWrapper_FWD_DEFINED__ */


#ifndef __IInitEventWrapper_FWD_DEFINED__
#define __IInitEventWrapper_FWD_DEFINED__
typedef interface IInitEventWrapper IInitEventWrapper;
#endif 	/* __IInitEventWrapper_FWD_DEFINED__ */


#ifndef __IUserEvent2_FWD_DEFINED__
#define __IUserEvent2_FWD_DEFINED__
typedef interface IUserEvent2 IUserEvent2;
#endif 	/* __IUserEvent2_FWD_DEFINED__ */


#ifndef __IUserEvents_FWD_DEFINED__
#define __IUserEvents_FWD_DEFINED__
typedef interface IUserEvents IUserEvents;
#endif 	/* __IUserEvents_FWD_DEFINED__ */


#ifndef __IEventUsers_FWD_DEFINED__
#define __IEventUsers_FWD_DEFINED__
typedef interface IEventUsers IEventUsers;
#endif 	/* __IEventUsers_FWD_DEFINED__ */


#ifndef __ITrendWrite_FWD_DEFINED__
#define __ITrendWrite_FWD_DEFINED__
typedef interface ITrendWrite ITrendWrite;
#endif 	/* __ITrendWrite_FWD_DEFINED__ */


#ifndef __ITrendRead_FWD_DEFINED__
#define __ITrendRead_FWD_DEFINED__
typedef interface ITrendRead ITrendRead;
#endif 	/* __ITrendRead_FWD_DEFINED__ */


#ifndef __IImpedanceEvent_FWD_DEFINED__
#define __IImpedanceEvent_FWD_DEFINED__
typedef interface IImpedanceEvent IImpedanceEvent;
#endif 	/* __IImpedanceEvent_FWD_DEFINED__ */


#ifndef __IEventWrapperProxy_FWD_DEFINED__
#define __IEventWrapperProxy_FWD_DEFINED__
typedef interface IEventWrapperProxy IEventWrapperProxy;
#endif 	/* __IEventWrapperProxy_FWD_DEFINED__ */


#ifndef __ISegments_FWD_DEFINED__
#define __ISegments_FWD_DEFINED__
typedef interface ISegments ISegments;
#endif 	/* __ISegments_FWD_DEFINED__ */


#ifndef __IStorageMan_FWD_DEFINED__
#define __IStorageMan_FWD_DEFINED__
typedef interface IStorageMan IStorageMan;
#endif 	/* __IStorageMan_FWD_DEFINED__ */


#ifndef __IStorageManEx_FWD_DEFINED__
#define __IStorageManEx_FWD_DEFINED__
typedef interface IStorageManEx IStorageManEx;
#endif 	/* __IStorageManEx_FWD_DEFINED__ */


#ifndef __IStorageManProxy_FWD_DEFINED__
#define __IStorageManProxy_FWD_DEFINED__
typedef interface IStorageManProxy IStorageManProxy;
#endif 	/* __IStorageManProxy_FWD_DEFINED__ */


#ifndef __IEventWrapperCopy_FWD_DEFINED__
#define __IEventWrapperCopy_FWD_DEFINED__
typedef interface IEventWrapperCopy IEventWrapperCopy;
#endif 	/* __IEventWrapperCopy_FWD_DEFINED__ */


#ifndef __ISleepData_FWD_DEFINED__
#define __ISleepData_FWD_DEFINED__
typedef interface ISleepData ISleepData;
#endif 	/* __ISleepData_FWD_DEFINED__ */


#ifndef __FileWrite3_FWD_DEFINED__
#define __FileWrite3_FWD_DEFINED__

#ifdef __cplusplus
typedef class FileWrite3 FileWrite3;
#else
typedef struct FileWrite3 FileWrite3;
#endif /* __cplusplus */

#endif 	/* __FileWrite3_FWD_DEFINED__ */


#ifndef __FileRead3_FWD_DEFINED__
#define __FileRead3_FWD_DEFINED__

#ifdef __cplusplus
typedef class FileRead3 FileRead3;
#else
typedef struct FileRead3 FileRead3;
#endif /* __cplusplus */

#endif 	/* __FileRead3_FWD_DEFINED__ */


#ifndef __EventWrapper_FWD_DEFINED__
#define __EventWrapper_FWD_DEFINED__

#ifdef __cplusplus
typedef class EventWrapper EventWrapper;
#else
typedef struct EventWrapper EventWrapper;
#endif /* __cplusplus */

#endif 	/* __EventWrapper_FWD_DEFINED__ */


#ifndef __UserEvent_FWD_DEFINED__
#define __UserEvent_FWD_DEFINED__

#ifdef __cplusplus
typedef class UserEvent UserEvent;
#else
typedef struct UserEvent UserEvent;
#endif /* __cplusplus */

#endif 	/* __UserEvent_FWD_DEFINED__ */


#ifndef __TrendWrite_FWD_DEFINED__
#define __TrendWrite_FWD_DEFINED__

#ifdef __cplusplus
typedef class TrendWrite TrendWrite;
#else
typedef struct TrendWrite TrendWrite;
#endif /* __cplusplus */

#endif 	/* __TrendWrite_FWD_DEFINED__ */


#ifndef __TrendRead_FWD_DEFINED__
#define __TrendRead_FWD_DEFINED__

#ifdef __cplusplus
typedef class TrendRead TrendRead;
#else
typedef struct TrendRead TrendRead;
#endif /* __cplusplus */

#endif 	/* __TrendRead_FWD_DEFINED__ */


#ifndef __ImpedanceEvent_FWD_DEFINED__
#define __ImpedanceEvent_FWD_DEFINED__

#ifdef __cplusplus
typedef class ImpedanceEvent ImpedanceEvent;
#else
typedef struct ImpedanceEvent ImpedanceEvent;
#endif /* __cplusplus */

#endif 	/* __ImpedanceEvent_FWD_DEFINED__ */


#ifndef __EegData_FWD_DEFINED__
#define __EegData_FWD_DEFINED__

#ifdef __cplusplus
typedef class EegData EegData;
#else
typedef struct EegData EegData;
#endif /* __cplusplus */

#endif 	/* __EegData_FWD_DEFINED__ */


#ifndef __EegSegments_FWD_DEFINED__
#define __EegSegments_FWD_DEFINED__

#ifdef __cplusplus
typedef class EegSegments EegSegments;
#else
typedef struct EegSegments EegSegments;
#endif /* __cplusplus */

#endif 	/* __EegSegments_FWD_DEFINED__ */


#ifndef __EventWrapperProxy_FWD_DEFINED__
#define __EventWrapperProxy_FWD_DEFINED__

#ifdef __cplusplus
typedef class EventWrapperProxy EventWrapperProxy;
#else
typedef struct EventWrapperProxy EventWrapperProxy;
#endif /* __cplusplus */

#endif 	/* __EventWrapperProxy_FWD_DEFINED__ */


#ifndef __SleepData_FWD_DEFINED__
#define __SleepData_FWD_DEFINED__

#ifdef __cplusplus
typedef class SleepData SleepData;
#else
typedef struct SleepData SleepData;
#endif /* __cplusplus */

#endif 	/* __SleepData_FWD_DEFINED__ */


#ifndef __AsciiWrite_FWD_DEFINED__
#define __AsciiWrite_FWD_DEFINED__

#ifdef __cplusplus
typedef class AsciiWrite AsciiWrite;
#else
typedef struct AsciiWrite AsciiWrite;
#endif /* __cplusplus */

#endif 	/* __AsciiWrite_FWD_DEFINED__ */


#ifndef __Segments_FWD_DEFINED__
#define __Segments_FWD_DEFINED__

#ifdef __cplusplus
typedef class Segments Segments;
#else
typedef struct Segments Segments;
#endif /* __cplusplus */

#endif 	/* __Segments_FWD_DEFINED__ */


#ifndef __StorageMan_FWD_DEFINED__
#define __StorageMan_FWD_DEFINED__

#ifdef __cplusplus
typedef class StorageMan StorageMan;
#else
typedef struct StorageMan StorageMan;
#endif /* __cplusplus */

#endif 	/* __StorageMan_FWD_DEFINED__ */


#ifndef __StorageManProxy_FWD_DEFINED__
#define __StorageManProxy_FWD_DEFINED__

#ifdef __cplusplus
typedef class StorageManProxy StorageManProxy;
#else
typedef struct StorageManProxy StorageManProxy;
#endif /* __cplusplus */

#endif 	/* __StorageManProxy_FWD_DEFINED__ */


#ifndef __EventWrapperCopy_FWD_DEFINED__
#define __EventWrapperCopy_FWD_DEFINED__

#ifdef __cplusplus
typedef class EventWrapperCopy EventWrapperCopy;
#else
typedef struct EventWrapperCopy EventWrapperCopy;
#endif /* __cplusplus */

#endif 	/* __EventWrapperCopy_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"
#include "AddInIF.h"
#include "AtlantisEventStructs.h"

#ifdef __cplusplus
extern "C"{
#endif 

void * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void * ); 

/* interface __MIDL_itf_FileAccess3_0000 */
/* [local] */ 










extern RPC_IF_HANDLE __MIDL_itf_FileAccess3_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_FileAccess3_0000_v0_0_s_ifspec;

#ifndef __IAsciiWrite_INTERFACE_DEFINED__
#define __IAsciiWrite_INTERFACE_DEFINED__

/* interface IAsciiWrite */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IAsciiWrite;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("B0355642-4646-4039-92E0-69E797470B71")
    IAsciiWrite : public IUnknown
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE SetFileName( 
            /* [in] */ BSTR bstrFileName) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IAsciiWriteVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IAsciiWrite * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IAsciiWrite * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IAsciiWrite * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetFileName )( 
            IAsciiWrite * This,
            /* [in] */ BSTR bstrFileName);
        
        END_INTERFACE
    } IAsciiWriteVtbl;

    interface IAsciiWrite
    {
        CONST_VTBL struct IAsciiWriteVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IAsciiWrite_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IAsciiWrite_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IAsciiWrite_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IAsciiWrite_SetFileName(This,bstrFileName)	\
    (This)->lpVtbl -> SetFileName(This,bstrFileName)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring] */ HRESULT STDMETHODCALLTYPE IAsciiWrite_SetFileName_Proxy( 
    IAsciiWrite * This,
    /* [in] */ BSTR bstrFileName);


void __RPC_STUB IAsciiWrite_SetFileName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IAsciiWrite_INTERFACE_DEFINED__ */


#ifndef __IFileRead3_INTERFACE_DEFINED__
#define __IFileRead3_INTERFACE_DEFINED__

/* interface IFileRead3 */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IFileRead3;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("F5AEC09C-C032-49d7-8EDA-522A64E22885")
    IFileRead3 : public IUnknown
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Read( 
            /* [in] */ double dDate,
            /* [in] */ double dFraction,
            /* [in] */ double dDuration,
            /* [in] */ BOOL bForceSendInfo,
            /* [in] */ BOOL bSendFilterInit) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE SelectSensors( 
            /* [in] */ IUnknown *pISensorGroup) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE SetEegStorage( 
            /* [in] */ IEegStorage *pIEegStorage) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE ReleaseEegStorage( void) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Initialize( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IFileRead3Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IFileRead3 * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IFileRead3 * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IFileRead3 * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *Read )( 
            IFileRead3 * This,
            /* [in] */ double dDate,
            /* [in] */ double dFraction,
            /* [in] */ double dDuration,
            /* [in] */ BOOL bForceSendInfo,
            /* [in] */ BOOL bSendFilterInit);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *SelectSensors )( 
            IFileRead3 * This,
            /* [in] */ IUnknown *pISensorGroup);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetEegStorage )( 
            IFileRead3 * This,
            /* [in] */ IEegStorage *pIEegStorage);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *ReleaseEegStorage )( 
            IFileRead3 * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *Initialize )( 
            IFileRead3 * This);
        
        END_INTERFACE
    } IFileRead3Vtbl;

    interface IFileRead3
    {
        CONST_VTBL struct IFileRead3Vtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IFileRead3_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IFileRead3_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IFileRead3_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IFileRead3_Read(This,dDate,dFraction,dDuration,bForceSendInfo,bSendFilterInit)	\
    (This)->lpVtbl -> Read(This,dDate,dFraction,dDuration,bForceSendInfo,bSendFilterInit)

#define IFileRead3_SelectSensors(This,pISensorGroup)	\
    (This)->lpVtbl -> SelectSensors(This,pISensorGroup)

#define IFileRead3_SetEegStorage(This,pIEegStorage)	\
    (This)->lpVtbl -> SetEegStorage(This,pIEegStorage)

#define IFileRead3_ReleaseEegStorage(This)	\
    (This)->lpVtbl -> ReleaseEegStorage(This)

#define IFileRead3_Initialize(This)	\
    (This)->lpVtbl -> Initialize(This)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring] */ HRESULT STDMETHODCALLTYPE IFileRead3_Read_Proxy( 
    IFileRead3 * This,
    /* [in] */ double dDate,
    /* [in] */ double dFraction,
    /* [in] */ double dDuration,
    /* [in] */ BOOL bForceSendInfo,
    /* [in] */ BOOL bSendFilterInit);


void __RPC_STUB IFileRead3_Read_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IFileRead3_SelectSensors_Proxy( 
    IFileRead3 * This,
    /* [in] */ IUnknown *pISensorGroup);


void __RPC_STUB IFileRead3_SelectSensors_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IFileRead3_SetEegStorage_Proxy( 
    IFileRead3 * This,
    /* [in] */ IEegStorage *pIEegStorage);


void __RPC_STUB IFileRead3_SetEegStorage_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IFileRead3_ReleaseEegStorage_Proxy( 
    IFileRead3 * This);


void __RPC_STUB IFileRead3_ReleaseEegStorage_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IFileRead3_Initialize_Proxy( 
    IFileRead3 * This);


void __RPC_STUB IFileRead3_Initialize_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IFileRead3_INTERFACE_DEFINED__ */


#ifndef __IFileWrite3_INTERFACE_DEFINED__
#define __IFileWrite3_INTERFACE_DEFINED__

/* interface IFileWrite3 */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IFileWrite3;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("87550C7F-43DB-465d-BF77-9BE0EF74DC08")
    IFileWrite3 : public IUnknown
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE SetEegStorage( 
            /* [in] */ IEegStorage *pIEegStorage) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE EnableFlush( 
            /* [in] */ VARIANT_BOOL bEnable) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE SetWriteImmediately( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IFileWrite3Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IFileWrite3 * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IFileWrite3 * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IFileWrite3 * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetEegStorage )( 
            IFileWrite3 * This,
            /* [in] */ IEegStorage *pIEegStorage);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *EnableFlush )( 
            IFileWrite3 * This,
            /* [in] */ VARIANT_BOOL bEnable);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetWriteImmediately )( 
            IFileWrite3 * This);
        
        END_INTERFACE
    } IFileWrite3Vtbl;

    interface IFileWrite3
    {
        CONST_VTBL struct IFileWrite3Vtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IFileWrite3_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IFileWrite3_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IFileWrite3_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IFileWrite3_SetEegStorage(This,pIEegStorage)	\
    (This)->lpVtbl -> SetEegStorage(This,pIEegStorage)

#define IFileWrite3_EnableFlush(This,bEnable)	\
    (This)->lpVtbl -> EnableFlush(This,bEnable)

#define IFileWrite3_SetWriteImmediately(This)	\
    (This)->lpVtbl -> SetWriteImmediately(This)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring] */ HRESULT STDMETHODCALLTYPE IFileWrite3_SetEegStorage_Proxy( 
    IFileWrite3 * This,
    /* [in] */ IEegStorage *pIEegStorage);


void __RPC_STUB IFileWrite3_SetEegStorage_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IFileWrite3_EnableFlush_Proxy( 
    IFileWrite3 * This,
    /* [in] */ VARIANT_BOOL bEnable);


void __RPC_STUB IFileWrite3_EnableFlush_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IFileWrite3_SetWriteImmediately_Proxy( 
    IFileWrite3 * This);


void __RPC_STUB IFileWrite3_SetWriteImmediately_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IFileWrite3_INTERFACE_DEFINED__ */


#ifndef __IEegStorage_INTERFACE_DEFINED__
#define __IEegStorage_INTERFACE_DEFINED__

/* interface IEegStorage */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IEegStorage;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("DDE93137-F71D-4e69-85D0-6D46974E1478")
    IEegStorage : public IDispatch
    {
    public:
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_bReadOnly( 
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_bOpen( 
            /* [retval][out] */ VARIANT_BOOL *pbOpen) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetEegSegments( 
            /* [retval][out] */ IEegSegments **ppIEegSegments) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetVideoSegments( 
            /* [retval][out] */ IEegSegments **ppIEegSegments) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetEegData( 
            /* [retval][out] */ IEegData **ppIEegData) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetEvents( 
            /* [retval][out] */ IUserEvents **ppIUserEvents) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetEventWrapper( 
            /* [retval][out] */ IEventWrapper **ppIEventWrapper) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetSleepData( 
            /* [out] */ ISleepData **ppISleepData) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IEegStorageVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IEegStorage * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IEegStorage * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IEegStorage * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IEegStorage * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IEegStorage * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IEegStorage * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IEegStorage * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_bReadOnly )( 
            IEegStorage * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_bOpen )( 
            IEegStorage * This,
            /* [retval][out] */ VARIANT_BOOL *pbOpen);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetEegSegments )( 
            IEegStorage * This,
            /* [retval][out] */ IEegSegments **ppIEegSegments);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetVideoSegments )( 
            IEegStorage * This,
            /* [retval][out] */ IEegSegments **ppIEegSegments);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetEegData )( 
            IEegStorage * This,
            /* [retval][out] */ IEegData **ppIEegData);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetEvents )( 
            IEegStorage * This,
            /* [retval][out] */ IUserEvents **ppIUserEvents);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetEventWrapper )( 
            IEegStorage * This,
            /* [retval][out] */ IEventWrapper **ppIEventWrapper);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetSleepData )( 
            IEegStorage * This,
            /* [out] */ ISleepData **ppISleepData);
        
        END_INTERFACE
    } IEegStorageVtbl;

    interface IEegStorage
    {
        CONST_VTBL struct IEegStorageVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IEegStorage_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IEegStorage_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IEegStorage_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IEegStorage_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IEegStorage_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IEegStorage_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IEegStorage_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IEegStorage_get_bReadOnly(This,pVal)	\
    (This)->lpVtbl -> get_bReadOnly(This,pVal)

#define IEegStorage_get_bOpen(This,pbOpen)	\
    (This)->lpVtbl -> get_bOpen(This,pbOpen)

#define IEegStorage_GetEegSegments(This,ppIEegSegments)	\
    (This)->lpVtbl -> GetEegSegments(This,ppIEegSegments)

#define IEegStorage_GetVideoSegments(This,ppIEegSegments)	\
    (This)->lpVtbl -> GetVideoSegments(This,ppIEegSegments)

#define IEegStorage_GetEegData(This,ppIEegData)	\
    (This)->lpVtbl -> GetEegData(This,ppIEegData)

#define IEegStorage_GetEvents(This,ppIUserEvents)	\
    (This)->lpVtbl -> GetEvents(This,ppIUserEvents)

#define IEegStorage_GetEventWrapper(This,ppIEventWrapper)	\
    (This)->lpVtbl -> GetEventWrapper(This,ppIEventWrapper)

#define IEegStorage_GetSleepData(This,ppISleepData)	\
    (This)->lpVtbl -> GetSleepData(This,ppISleepData)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE IEegStorage_get_bReadOnly_Proxy( 
    IEegStorage * This,
    /* [retval][out] */ VARIANT_BOOL *pVal);


void __RPC_STUB IEegStorage_get_bReadOnly_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE IEegStorage_get_bOpen_Proxy( 
    IEegStorage * This,
    /* [retval][out] */ VARIANT_BOOL *pbOpen);


void __RPC_STUB IEegStorage_get_bOpen_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IEegStorage_GetEegSegments_Proxy( 
    IEegStorage * This,
    /* [retval][out] */ IEegSegments **ppIEegSegments);


void __RPC_STUB IEegStorage_GetEegSegments_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IEegStorage_GetVideoSegments_Proxy( 
    IEegStorage * This,
    /* [retval][out] */ IEegSegments **ppIEegSegments);


void __RPC_STUB IEegStorage_GetVideoSegments_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IEegStorage_GetEegData_Proxy( 
    IEegStorage * This,
    /* [retval][out] */ IEegData **ppIEegData);


void __RPC_STUB IEegStorage_GetEegData_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IEegStorage_GetEvents_Proxy( 
    IEegStorage * This,
    /* [retval][out] */ IUserEvents **ppIUserEvents);


void __RPC_STUB IEegStorage_GetEvents_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IEegStorage_GetEventWrapper_Proxy( 
    IEegStorage * This,
    /* [retval][out] */ IEventWrapper **ppIEventWrapper);


void __RPC_STUB IEegStorage_GetEventWrapper_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IEegStorage_GetSleepData_Proxy( 
    IEegStorage * This,
    /* [out] */ ISleepData **ppISleepData);


void __RPC_STUB IEegStorage_GetSleepData_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IEegStorage_INTERFACE_DEFINED__ */


#ifndef __IEegSegments_INTERFACE_DEFINED__
#define __IEegSegments_INTERFACE_DEFINED__

/* interface IEegSegments */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IEegSegments;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("A82442C0-8C14-46ac-9ED5-27C2D2469AB8")
    IEegSegments : public IDispatch
    {
    public:
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_lcSegments( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE QuerySegmentStartTime( 
            /* [in] */ double dDate,
            /* [in] */ double dFraction,
            /* [out] */ double *pdDate,
            /* [out] */ double *pdFraction,
            /* [retval][out] */ long *pliSegment) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE QuerySegmentEndTime( 
            /* [in] */ double dDate,
            /* [in] */ double dFraction,
            /* [out] */ double *pdDate,
            /* [out] */ double *pdFraction,
            /* [retval][out] */ long *pliSegment) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE QuerySegmentStartTime2( 
            /* [in] */ long liSegment,
            /* [out] */ double *pdDate,
            /* [out] */ double *pdFraction) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE QuerySegmentEndTime2( 
            /* [in] */ long liSegment,
            /* [out] */ double *pdDate,
            /* [out] */ double *pdFraction) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE QuerySegmentDuration( 
            /* [in] */ long liSegment,
            /* [retval][out] */ double *pdDuration) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE QueryLastSegment( 
            /* [in] */ double dDate,
            /* [in] */ double dFraction,
            /* [out] */ long *pliSegment,
            /* [retval][out] */ VARIANT_BOOL *pbInSegment) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE QueryStartTime( 
            /* [out] */ double *pdDate,
            /* [out] */ double *pdFraction) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE QueryEndTime( 
            /* [out] */ double *pdDate,
            /* [out] */ double *pdFraction) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE QueryTotalRecSecs( 
            /* [retval][out] */ double *pVal) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE NrvDate2RecSec( 
            /* [in] */ double dDate,
            /* [in] */ double dFraction,
            /* [retval][out] */ double *pdRecSec) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE RecSec2NrvDate( 
            /* [in] */ double dRecSec,
            /* [out] */ double *pdDate,
            /* [out] */ double *pdFraction) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IEegSegmentsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IEegSegments * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IEegSegments * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IEegSegments * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IEegSegments * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IEegSegments * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IEegSegments * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IEegSegments * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_lcSegments )( 
            IEegSegments * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *QuerySegmentStartTime )( 
            IEegSegments * This,
            /* [in] */ double dDate,
            /* [in] */ double dFraction,
            /* [out] */ double *pdDate,
            /* [out] */ double *pdFraction,
            /* [retval][out] */ long *pliSegment);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *QuerySegmentEndTime )( 
            IEegSegments * This,
            /* [in] */ double dDate,
            /* [in] */ double dFraction,
            /* [out] */ double *pdDate,
            /* [out] */ double *pdFraction,
            /* [retval][out] */ long *pliSegment);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *QuerySegmentStartTime2 )( 
            IEegSegments * This,
            /* [in] */ long liSegment,
            /* [out] */ double *pdDate,
            /* [out] */ double *pdFraction);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *QuerySegmentEndTime2 )( 
            IEegSegments * This,
            /* [in] */ long liSegment,
            /* [out] */ double *pdDate,
            /* [out] */ double *pdFraction);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *QuerySegmentDuration )( 
            IEegSegments * This,
            /* [in] */ long liSegment,
            /* [retval][out] */ double *pdDuration);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *QueryLastSegment )( 
            IEegSegments * This,
            /* [in] */ double dDate,
            /* [in] */ double dFraction,
            /* [out] */ long *pliSegment,
            /* [retval][out] */ VARIANT_BOOL *pbInSegment);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *QueryStartTime )( 
            IEegSegments * This,
            /* [out] */ double *pdDate,
            /* [out] */ double *pdFraction);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *QueryEndTime )( 
            IEegSegments * This,
            /* [out] */ double *pdDate,
            /* [out] */ double *pdFraction);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *QueryTotalRecSecs )( 
            IEegSegments * This,
            /* [retval][out] */ double *pVal);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *NrvDate2RecSec )( 
            IEegSegments * This,
            /* [in] */ double dDate,
            /* [in] */ double dFraction,
            /* [retval][out] */ double *pdRecSec);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *RecSec2NrvDate )( 
            IEegSegments * This,
            /* [in] */ double dRecSec,
            /* [out] */ double *pdDate,
            /* [out] */ double *pdFraction);
        
        END_INTERFACE
    } IEegSegmentsVtbl;

    interface IEegSegments
    {
        CONST_VTBL struct IEegSegmentsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IEegSegments_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IEegSegments_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IEegSegments_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IEegSegments_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IEegSegments_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IEegSegments_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IEegSegments_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IEegSegments_get_lcSegments(This,pVal)	\
    (This)->lpVtbl -> get_lcSegments(This,pVal)

#define IEegSegments_QuerySegmentStartTime(This,dDate,dFraction,pdDate,pdFraction,pliSegment)	\
    (This)->lpVtbl -> QuerySegmentStartTime(This,dDate,dFraction,pdDate,pdFraction,pliSegment)

#define IEegSegments_QuerySegmentEndTime(This,dDate,dFraction,pdDate,pdFraction,pliSegment)	\
    (This)->lpVtbl -> QuerySegmentEndTime(This,dDate,dFraction,pdDate,pdFraction,pliSegment)

#define IEegSegments_QuerySegmentStartTime2(This,liSegment,pdDate,pdFraction)	\
    (This)->lpVtbl -> QuerySegmentStartTime2(This,liSegment,pdDate,pdFraction)

#define IEegSegments_QuerySegmentEndTime2(This,liSegment,pdDate,pdFraction)	\
    (This)->lpVtbl -> QuerySegmentEndTime2(This,liSegment,pdDate,pdFraction)

#define IEegSegments_QuerySegmentDuration(This,liSegment,pdDuration)	\
    (This)->lpVtbl -> QuerySegmentDuration(This,liSegment,pdDuration)

#define IEegSegments_QueryLastSegment(This,dDate,dFraction,pliSegment,pbInSegment)	\
    (This)->lpVtbl -> QueryLastSegment(This,dDate,dFraction,pliSegment,pbInSegment)

#define IEegSegments_QueryStartTime(This,pdDate,pdFraction)	\
    (This)->lpVtbl -> QueryStartTime(This,pdDate,pdFraction)

#define IEegSegments_QueryEndTime(This,pdDate,pdFraction)	\
    (This)->lpVtbl -> QueryEndTime(This,pdDate,pdFraction)

#define IEegSegments_QueryTotalRecSecs(This,pVal)	\
    (This)->lpVtbl -> QueryTotalRecSecs(This,pVal)

#define IEegSegments_NrvDate2RecSec(This,dDate,dFraction,pdRecSec)	\
    (This)->lpVtbl -> NrvDate2RecSec(This,dDate,dFraction,pdRecSec)

#define IEegSegments_RecSec2NrvDate(This,dRecSec,pdDate,pdFraction)	\
    (This)->lpVtbl -> RecSec2NrvDate(This,dRecSec,pdDate,pdFraction)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE IEegSegments_get_lcSegments_Proxy( 
    IEegSegments * This,
    /* [retval][out] */ long *pVal);


void __RPC_STUB IEegSegments_get_lcSegments_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IEegSegments_QuerySegmentStartTime_Proxy( 
    IEegSegments * This,
    /* [in] */ double dDate,
    /* [in] */ double dFraction,
    /* [out] */ double *pdDate,
    /* [out] */ double *pdFraction,
    /* [retval][out] */ long *pliSegment);


void __RPC_STUB IEegSegments_QuerySegmentStartTime_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IEegSegments_QuerySegmentEndTime_Proxy( 
    IEegSegments * This,
    /* [in] */ double dDate,
    /* [in] */ double dFraction,
    /* [out] */ double *pdDate,
    /* [out] */ double *pdFraction,
    /* [retval][out] */ long *pliSegment);


void __RPC_STUB IEegSegments_QuerySegmentEndTime_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IEegSegments_QuerySegmentStartTime2_Proxy( 
    IEegSegments * This,
    /* [in] */ long liSegment,
    /* [out] */ double *pdDate,
    /* [out] */ double *pdFraction);


void __RPC_STUB IEegSegments_QuerySegmentStartTime2_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IEegSegments_QuerySegmentEndTime2_Proxy( 
    IEegSegments * This,
    /* [in] */ long liSegment,
    /* [out] */ double *pdDate,
    /* [out] */ double *pdFraction);


void __RPC_STUB IEegSegments_QuerySegmentEndTime2_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IEegSegments_QuerySegmentDuration_Proxy( 
    IEegSegments * This,
    /* [in] */ long liSegment,
    /* [retval][out] */ double *pdDuration);


void __RPC_STUB IEegSegments_QuerySegmentDuration_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IEegSegments_QueryLastSegment_Proxy( 
    IEegSegments * This,
    /* [in] */ double dDate,
    /* [in] */ double dFraction,
    /* [out] */ long *pliSegment,
    /* [retval][out] */ VARIANT_BOOL *pbInSegment);


void __RPC_STUB IEegSegments_QueryLastSegment_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IEegSegments_QueryStartTime_Proxy( 
    IEegSegments * This,
    /* [out] */ double *pdDate,
    /* [out] */ double *pdFraction);


void __RPC_STUB IEegSegments_QueryStartTime_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IEegSegments_QueryEndTime_Proxy( 
    IEegSegments * This,
    /* [out] */ double *pdDate,
    /* [out] */ double *pdFraction);


void __RPC_STUB IEegSegments_QueryEndTime_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IEegSegments_QueryTotalRecSecs_Proxy( 
    IEegSegments * This,
    /* [retval][out] */ double *pVal);


void __RPC_STUB IEegSegments_QueryTotalRecSecs_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IEegSegments_NrvDate2RecSec_Proxy( 
    IEegSegments * This,
    /* [in] */ double dDate,
    /* [in] */ double dFraction,
    /* [retval][out] */ double *pdRecSec);


void __RPC_STUB IEegSegments_NrvDate2RecSec_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IEegSegments_RecSec2NrvDate_Proxy( 
    IEegSegments * This,
    /* [in] */ double dRecSec,
    /* [out] */ double *pdDate,
    /* [out] */ double *pdFraction);


void __RPC_STUB IEegSegments_RecSec2NrvDate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IEegSegments_INTERFACE_DEFINED__ */


#ifndef __IInitEegSegments_INTERFACE_DEFINED__
#define __IInitEegSegments_INTERFACE_DEFINED__

/* interface IInitEegSegments */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IInitEegSegments;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1874E9C2-2380-4af3-9D37-6726E9748B89")
    IInitEegSegments : public IEegSegments
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Initialize( 
            /* [in] */ INrvSegments3 *pINrvSegments3) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Uninitialize( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IInitEegSegmentsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IInitEegSegments * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IInitEegSegments * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IInitEegSegments * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IInitEegSegments * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IInitEegSegments * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IInitEegSegments * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IInitEegSegments * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_lcSegments )( 
            IInitEegSegments * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *QuerySegmentStartTime )( 
            IInitEegSegments * This,
            /* [in] */ double dDate,
            /* [in] */ double dFraction,
            /* [out] */ double *pdDate,
            /* [out] */ double *pdFraction,
            /* [retval][out] */ long *pliSegment);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *QuerySegmentEndTime )( 
            IInitEegSegments * This,
            /* [in] */ double dDate,
            /* [in] */ double dFraction,
            /* [out] */ double *pdDate,
            /* [out] */ double *pdFraction,
            /* [retval][out] */ long *pliSegment);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *QuerySegmentStartTime2 )( 
            IInitEegSegments * This,
            /* [in] */ long liSegment,
            /* [out] */ double *pdDate,
            /* [out] */ double *pdFraction);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *QuerySegmentEndTime2 )( 
            IInitEegSegments * This,
            /* [in] */ long liSegment,
            /* [out] */ double *pdDate,
            /* [out] */ double *pdFraction);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *QuerySegmentDuration )( 
            IInitEegSegments * This,
            /* [in] */ long liSegment,
            /* [retval][out] */ double *pdDuration);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *QueryLastSegment )( 
            IInitEegSegments * This,
            /* [in] */ double dDate,
            /* [in] */ double dFraction,
            /* [out] */ long *pliSegment,
            /* [retval][out] */ VARIANT_BOOL *pbInSegment);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *QueryStartTime )( 
            IInitEegSegments * This,
            /* [out] */ double *pdDate,
            /* [out] */ double *pdFraction);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *QueryEndTime )( 
            IInitEegSegments * This,
            /* [out] */ double *pdDate,
            /* [out] */ double *pdFraction);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *QueryTotalRecSecs )( 
            IInitEegSegments * This,
            /* [retval][out] */ double *pVal);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *NrvDate2RecSec )( 
            IInitEegSegments * This,
            /* [in] */ double dDate,
            /* [in] */ double dFraction,
            /* [retval][out] */ double *pdRecSec);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *RecSec2NrvDate )( 
            IInitEegSegments * This,
            /* [in] */ double dRecSec,
            /* [out] */ double *pdDate,
            /* [out] */ double *pdFraction);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *Initialize )( 
            IInitEegSegments * This,
            /* [in] */ INrvSegments3 *pINrvSegments3);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *Uninitialize )( 
            IInitEegSegments * This);
        
        END_INTERFACE
    } IInitEegSegmentsVtbl;

    interface IInitEegSegments
    {
        CONST_VTBL struct IInitEegSegmentsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IInitEegSegments_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IInitEegSegments_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IInitEegSegments_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IInitEegSegments_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IInitEegSegments_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IInitEegSegments_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IInitEegSegments_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IInitEegSegments_get_lcSegments(This,pVal)	\
    (This)->lpVtbl -> get_lcSegments(This,pVal)

#define IInitEegSegments_QuerySegmentStartTime(This,dDate,dFraction,pdDate,pdFraction,pliSegment)	\
    (This)->lpVtbl -> QuerySegmentStartTime(This,dDate,dFraction,pdDate,pdFraction,pliSegment)

#define IInitEegSegments_QuerySegmentEndTime(This,dDate,dFraction,pdDate,pdFraction,pliSegment)	\
    (This)->lpVtbl -> QuerySegmentEndTime(This,dDate,dFraction,pdDate,pdFraction,pliSegment)

#define IInitEegSegments_QuerySegmentStartTime2(This,liSegment,pdDate,pdFraction)	\
    (This)->lpVtbl -> QuerySegmentStartTime2(This,liSegment,pdDate,pdFraction)

#define IInitEegSegments_QuerySegmentEndTime2(This,liSegment,pdDate,pdFraction)	\
    (This)->lpVtbl -> QuerySegmentEndTime2(This,liSegment,pdDate,pdFraction)

#define IInitEegSegments_QuerySegmentDuration(This,liSegment,pdDuration)	\
    (This)->lpVtbl -> QuerySegmentDuration(This,liSegment,pdDuration)

#define IInitEegSegments_QueryLastSegment(This,dDate,dFraction,pliSegment,pbInSegment)	\
    (This)->lpVtbl -> QueryLastSegment(This,dDate,dFraction,pliSegment,pbInSegment)

#define IInitEegSegments_QueryStartTime(This,pdDate,pdFraction)	\
    (This)->lpVtbl -> QueryStartTime(This,pdDate,pdFraction)

#define IInitEegSegments_QueryEndTime(This,pdDate,pdFraction)	\
    (This)->lpVtbl -> QueryEndTime(This,pdDate,pdFraction)

#define IInitEegSegments_QueryTotalRecSecs(This,pVal)	\
    (This)->lpVtbl -> QueryTotalRecSecs(This,pVal)

#define IInitEegSegments_NrvDate2RecSec(This,dDate,dFraction,pdRecSec)	\
    (This)->lpVtbl -> NrvDate2RecSec(This,dDate,dFraction,pdRecSec)

#define IInitEegSegments_RecSec2NrvDate(This,dRecSec,pdDate,pdFraction)	\
    (This)->lpVtbl -> RecSec2NrvDate(This,dRecSec,pdDate,pdFraction)


#define IInitEegSegments_Initialize(This,pINrvSegments3)	\
    (This)->lpVtbl -> Initialize(This,pINrvSegments3)

#define IInitEegSegments_Uninitialize(This)	\
    (This)->lpVtbl -> Uninitialize(This)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring] */ HRESULT STDMETHODCALLTYPE IInitEegSegments_Initialize_Proxy( 
    IInitEegSegments * This,
    /* [in] */ INrvSegments3 *pINrvSegments3);


void __RPC_STUB IInitEegSegments_Initialize_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IInitEegSegments_Uninitialize_Proxy( 
    IInitEegSegments * This);


void __RPC_STUB IInitEegSegments_Uninitialize_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IInitEegSegments_INTERFACE_DEFINED__ */


#ifndef __IEegData_INTERFACE_DEFINED__
#define __IEegData_INTERFACE_DEFINED__

/* interface IEegData */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IEegData;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("35838D50-D0E7-41a3-9AA0-E3215F4D7A2B")
    IEegData : public IUnknown
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE SetMaxChannelCount( 
            /* [in] */ long lcMaxChannels) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetMaxChannelCount( 
            /* [retval][out] */ long *plcMaxChannels) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE StartSampling( 
            /* [in] */ NRVDATE nrvDate,
            /* [in] */ long lPacketSize,
            /* [size_is][in] */ BYTE *pPacket) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE StopSampling( void) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE WriteStaticInfoPacket( 
            /* [in] */ long lPacketSize,
            /* [size_is][in] */ BYTE *pPacket) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE CanWriteStaticInfoPacket( 
            /* [in] */ GUID guid,
            /* [retval][out] */ BOOL *pbVal) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE WriteInfoPacket( 
            /* [in] */ long lPacketSize,
            /* [size_is][in] */ BYTE *pPacket) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE WriteData( 
            /* [in] */ long liChannel,
            /* [size_is][in] */ short *psData,
            /* [in] */ long lcSamples) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Flush( void) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_bSampling( 
            /* [retval][out] */ BOOL *pVal) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetDataPacket( 
            /* [in] */ NRVDATE nrvDate,
            /* [in] */ double dDuration,
            /* [in] */ long lPacketSize,
            /* [size_is][out] */ BYTE *pPacket) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetChannelCount( 
            /* [in] */ NRVDATE nrvDate,
            /* [retval][out] */ long *plcChannels) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetSampleCount( 
            /* [in] */ long liChannel,
            /* [in] */ NRVDATE nrvDate,
            /* [in] */ double dDuration,
            /* [retval][out] */ long *plSamples) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetSampleCount2( 
            /* [in] */ NRVDATE nrvDate,
            /* [in] */ double dDuration,
            /* [in] */ long lcChannels,
            /* [size_is][out] */ long *plSamples) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetData( 
            /* [in] */ long liChannel,
            /* [in] */ NRVDATE nrvDate,
            /* [in] */ double dDuration,
            /* [in] */ long lcSamples,
            /* [size_is][out] */ short *psData) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_bSupportsGetData( 
            /* [retval][out] */ BOOL *pVal) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_lcInfoChange( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE QueryLastInfoChange( 
            /* [in] */ GUID guidInfoChange,
            /* [in] */ NRVDATE nrvDate,
            /* [out] */ long *plPacketSize,
            /* [retval][out] */ long *pliChange) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE QueryNextInfoChange( 
            /* [in] */ NRVDATE nrvDate,
            /* [out] */ NRVDATE *pnrvDate,
            /* [out] */ GUID *pguidInfoPacket,
            /* [out] */ long *plPacketSize,
            /* [retval][out] */ long *pliChange) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE QueryNextInfoChange2( 
            /* [in] */ GUID guidInfoPacket,
            /* [in] */ long liChange,
            /* [out] */ long *plPacketSize,
            /* [retval][out] */ long *pliChange) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE QueryInfoChange( 
            /* [in] */ long liInfoChange,
            /* [out] */ NRVDATE *pnrvDate,
            /* [out] */ long *plPacketSize,
            /* [retval][out] */ GUID *pguidInfoPacket) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetInfoPacket( 
            /* [in] */ GUID guidInfoPacket,
            /* [in] */ NRVDATE nrvDate,
            /* [in] */ long lPacketSize,
            /* [size_is][out] */ BYTE *pPacket) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetInfoPacket2( 
            /* [in] */ long liChange,
            /* [in] */ long lPacketSize,
            /* [size_is][out] */ BYTE *pPacket) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE QueryStaticInfoPacketSize( 
            /* [in] */ GUID guid,
            /* [retval][out] */ long *plSize) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetStaticInfoPacket( 
            /* [in] */ GUID guid,
            /* [in] */ long lPacketSize,
            /* [size_is][out] */ BYTE *pPacket) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetTestID( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE ReadExtraData( 
            /* [in] */ GUID guidType,
            /* [in] */ BSTR bstrName,
            /* [in] */ LONGLONG llPos,
            /* [in] */ ULONG lcSamples,
            /* [size_is][out] */ BYTE *pSamples,
            /* [retval][out] */ ULONG *plSamplesRead) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE WriteExtraData( 
            /* [in] */ GUID guidType,
            /* [in] */ BSTR bstrName,
            /* [in] */ LONGLONG llPos,
            /* [in] */ ULONG lcSamples,
            /* [size_is][in] */ BYTE *pSamples,
            /* [retval][out] */ ULONG *plSamplesWritten) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE CreateExtraData( 
            /* [in] */ GUID guidType,
            /* [in] */ BSTR bstrName) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetExtraDataSize( 
            /* [in] */ GUID guidType,
            /* [in] */ BSTR bstrName,
            /* [retval][out] */ ULONGLONG *pllSize) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IEegDataVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IEegData * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IEegData * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IEegData * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetMaxChannelCount )( 
            IEegData * This,
            /* [in] */ long lcMaxChannels);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetMaxChannelCount )( 
            IEegData * This,
            /* [retval][out] */ long *plcMaxChannels);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *StartSampling )( 
            IEegData * This,
            /* [in] */ NRVDATE nrvDate,
            /* [in] */ long lPacketSize,
            /* [size_is][in] */ BYTE *pPacket);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *StopSampling )( 
            IEegData * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *WriteStaticInfoPacket )( 
            IEegData * This,
            /* [in] */ long lPacketSize,
            /* [size_is][in] */ BYTE *pPacket);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *CanWriteStaticInfoPacket )( 
            IEegData * This,
            /* [in] */ GUID guid,
            /* [retval][out] */ BOOL *pbVal);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *WriteInfoPacket )( 
            IEegData * This,
            /* [in] */ long lPacketSize,
            /* [size_is][in] */ BYTE *pPacket);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *WriteData )( 
            IEegData * This,
            /* [in] */ long liChannel,
            /* [size_is][in] */ short *psData,
            /* [in] */ long lcSamples);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *Flush )( 
            IEegData * This);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_bSampling )( 
            IEegData * This,
            /* [retval][out] */ BOOL *pVal);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetDataPacket )( 
            IEegData * This,
            /* [in] */ NRVDATE nrvDate,
            /* [in] */ double dDuration,
            /* [in] */ long lPacketSize,
            /* [size_is][out] */ BYTE *pPacket);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetChannelCount )( 
            IEegData * This,
            /* [in] */ NRVDATE nrvDate,
            /* [retval][out] */ long *plcChannels);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetSampleCount )( 
            IEegData * This,
            /* [in] */ long liChannel,
            /* [in] */ NRVDATE nrvDate,
            /* [in] */ double dDuration,
            /* [retval][out] */ long *plSamples);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetSampleCount2 )( 
            IEegData * This,
            /* [in] */ NRVDATE nrvDate,
            /* [in] */ double dDuration,
            /* [in] */ long lcChannels,
            /* [size_is][out] */ long *plSamples);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetData )( 
            IEegData * This,
            /* [in] */ long liChannel,
            /* [in] */ NRVDATE nrvDate,
            /* [in] */ double dDuration,
            /* [in] */ long lcSamples,
            /* [size_is][out] */ short *psData);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_bSupportsGetData )( 
            IEegData * This,
            /* [retval][out] */ BOOL *pVal);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_lcInfoChange )( 
            IEegData * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *QueryLastInfoChange )( 
            IEegData * This,
            /* [in] */ GUID guidInfoChange,
            /* [in] */ NRVDATE nrvDate,
            /* [out] */ long *plPacketSize,
            /* [retval][out] */ long *pliChange);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *QueryNextInfoChange )( 
            IEegData * This,
            /* [in] */ NRVDATE nrvDate,
            /* [out] */ NRVDATE *pnrvDate,
            /* [out] */ GUID *pguidInfoPacket,
            /* [out] */ long *plPacketSize,
            /* [retval][out] */ long *pliChange);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *QueryNextInfoChange2 )( 
            IEegData * This,
            /* [in] */ GUID guidInfoPacket,
            /* [in] */ long liChange,
            /* [out] */ long *plPacketSize,
            /* [retval][out] */ long *pliChange);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *QueryInfoChange )( 
            IEegData * This,
            /* [in] */ long liInfoChange,
            /* [out] */ NRVDATE *pnrvDate,
            /* [out] */ long *plPacketSize,
            /* [retval][out] */ GUID *pguidInfoPacket);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetInfoPacket )( 
            IEegData * This,
            /* [in] */ GUID guidInfoPacket,
            /* [in] */ NRVDATE nrvDate,
            /* [in] */ long lPacketSize,
            /* [size_is][out] */ BYTE *pPacket);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetInfoPacket2 )( 
            IEegData * This,
            /* [in] */ long liChange,
            /* [in] */ long lPacketSize,
            /* [size_is][out] */ BYTE *pPacket);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *QueryStaticInfoPacketSize )( 
            IEegData * This,
            /* [in] */ GUID guid,
            /* [retval][out] */ long *plSize);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetStaticInfoPacket )( 
            IEegData * This,
            /* [in] */ GUID guid,
            /* [in] */ long lPacketSize,
            /* [size_is][out] */ BYTE *pPacket);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetTestID )( 
            IEegData * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *ReadExtraData )( 
            IEegData * This,
            /* [in] */ GUID guidType,
            /* [in] */ BSTR bstrName,
            /* [in] */ LONGLONG llPos,
            /* [in] */ ULONG lcSamples,
            /* [size_is][out] */ BYTE *pSamples,
            /* [retval][out] */ ULONG *plSamplesRead);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *WriteExtraData )( 
            IEegData * This,
            /* [in] */ GUID guidType,
            /* [in] */ BSTR bstrName,
            /* [in] */ LONGLONG llPos,
            /* [in] */ ULONG lcSamples,
            /* [size_is][in] */ BYTE *pSamples,
            /* [retval][out] */ ULONG *plSamplesWritten);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *CreateExtraData )( 
            IEegData * This,
            /* [in] */ GUID guidType,
            /* [in] */ BSTR bstrName);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetExtraDataSize )( 
            IEegData * This,
            /* [in] */ GUID guidType,
            /* [in] */ BSTR bstrName,
            /* [retval][out] */ ULONGLONG *pllSize);
        
        END_INTERFACE
    } IEegDataVtbl;

    interface IEegData
    {
        CONST_VTBL struct IEegDataVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IEegData_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IEegData_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IEegData_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IEegData_SetMaxChannelCount(This,lcMaxChannels)	\
    (This)->lpVtbl -> SetMaxChannelCount(This,lcMaxChannels)

#define IEegData_GetMaxChannelCount(This,plcMaxChannels)	\
    (This)->lpVtbl -> GetMaxChannelCount(This,plcMaxChannels)

#define IEegData_StartSampling(This,nrvDate,lPacketSize,pPacket)	\
    (This)->lpVtbl -> StartSampling(This,nrvDate,lPacketSize,pPacket)

#define IEegData_StopSampling(This)	\
    (This)->lpVtbl -> StopSampling(This)

#define IEegData_WriteStaticInfoPacket(This,lPacketSize,pPacket)	\
    (This)->lpVtbl -> WriteStaticInfoPacket(This,lPacketSize,pPacket)

#define IEegData_CanWriteStaticInfoPacket(This,guid,pbVal)	\
    (This)->lpVtbl -> CanWriteStaticInfoPacket(This,guid,pbVal)

#define IEegData_WriteInfoPacket(This,lPacketSize,pPacket)	\
    (This)->lpVtbl -> WriteInfoPacket(This,lPacketSize,pPacket)

#define IEegData_WriteData(This,liChannel,psData,lcSamples)	\
    (This)->lpVtbl -> WriteData(This,liChannel,psData,lcSamples)

#define IEegData_Flush(This)	\
    (This)->lpVtbl -> Flush(This)

#define IEegData_get_bSampling(This,pVal)	\
    (This)->lpVtbl -> get_bSampling(This,pVal)

#define IEegData_GetDataPacket(This,nrvDate,dDuration,lPacketSize,pPacket)	\
    (This)->lpVtbl -> GetDataPacket(This,nrvDate,dDuration,lPacketSize,pPacket)

#define IEegData_GetChannelCount(This,nrvDate,plcChannels)	\
    (This)->lpVtbl -> GetChannelCount(This,nrvDate,plcChannels)

#define IEegData_GetSampleCount(This,liChannel,nrvDate,dDuration,plSamples)	\
    (This)->lpVtbl -> GetSampleCount(This,liChannel,nrvDate,dDuration,plSamples)

#define IEegData_GetSampleCount2(This,nrvDate,dDuration,lcChannels,plSamples)	\
    (This)->lpVtbl -> GetSampleCount2(This,nrvDate,dDuration,lcChannels,plSamples)

#define IEegData_GetData(This,liChannel,nrvDate,dDuration,lcSamples,psData)	\
    (This)->lpVtbl -> GetData(This,liChannel,nrvDate,dDuration,lcSamples,psData)

#define IEegData_get_bSupportsGetData(This,pVal)	\
    (This)->lpVtbl -> get_bSupportsGetData(This,pVal)

#define IEegData_get_lcInfoChange(This,pVal)	\
    (This)->lpVtbl -> get_lcInfoChange(This,pVal)

#define IEegData_QueryLastInfoChange(This,guidInfoChange,nrvDate,plPacketSize,pliChange)	\
    (This)->lpVtbl -> QueryLastInfoChange(This,guidInfoChange,nrvDate,plPacketSize,pliChange)

#define IEegData_QueryNextInfoChange(This,nrvDate,pnrvDate,pguidInfoPacket,plPacketSize,pliChange)	\
    (This)->lpVtbl -> QueryNextInfoChange(This,nrvDate,pnrvDate,pguidInfoPacket,plPacketSize,pliChange)

#define IEegData_QueryNextInfoChange2(This,guidInfoPacket,liChange,plPacketSize,pliChange)	\
    (This)->lpVtbl -> QueryNextInfoChange2(This,guidInfoPacket,liChange,plPacketSize,pliChange)

#define IEegData_QueryInfoChange(This,liInfoChange,pnrvDate,plPacketSize,pguidInfoPacket)	\
    (This)->lpVtbl -> QueryInfoChange(This,liInfoChange,pnrvDate,plPacketSize,pguidInfoPacket)

#define IEegData_GetInfoPacket(This,guidInfoPacket,nrvDate,lPacketSize,pPacket)	\
    (This)->lpVtbl -> GetInfoPacket(This,guidInfoPacket,nrvDate,lPacketSize,pPacket)

#define IEegData_GetInfoPacket2(This,liChange,lPacketSize,pPacket)	\
    (This)->lpVtbl -> GetInfoPacket2(This,liChange,lPacketSize,pPacket)

#define IEegData_QueryStaticInfoPacketSize(This,guid,plSize)	\
    (This)->lpVtbl -> QueryStaticInfoPacketSize(This,guid,plSize)

#define IEegData_GetStaticInfoPacket(This,guid,lPacketSize,pPacket)	\
    (This)->lpVtbl -> GetStaticInfoPacket(This,guid,lPacketSize,pPacket)

#define IEegData_GetTestID(This,pVal)	\
    (This)->lpVtbl -> GetTestID(This,pVal)

#define IEegData_ReadExtraData(This,guidType,bstrName,llPos,lcSamples,pSamples,plSamplesRead)	\
    (This)->lpVtbl -> ReadExtraData(This,guidType,bstrName,llPos,lcSamples,pSamples,plSamplesRead)

#define IEegData_WriteExtraData(This,guidType,bstrName,llPos,lcSamples,pSamples,plSamplesWritten)	\
    (This)->lpVtbl -> WriteExtraData(This,guidType,bstrName,llPos,lcSamples,pSamples,plSamplesWritten)

#define IEegData_CreateExtraData(This,guidType,bstrName)	\
    (This)->lpVtbl -> CreateExtraData(This,guidType,bstrName)

#define IEegData_GetExtraDataSize(This,guidType,bstrName,pllSize)	\
    (This)->lpVtbl -> GetExtraDataSize(This,guidType,bstrName,pllSize)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring] */ HRESULT STDMETHODCALLTYPE IEegData_SetMaxChannelCount_Proxy( 
    IEegData * This,
    /* [in] */ long lcMaxChannels);


void __RPC_STUB IEegData_SetMaxChannelCount_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IEegData_GetMaxChannelCount_Proxy( 
    IEegData * This,
    /* [retval][out] */ long *plcMaxChannels);


void __RPC_STUB IEegData_GetMaxChannelCount_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IEegData_StartSampling_Proxy( 
    IEegData * This,
    /* [in] */ NRVDATE nrvDate,
    /* [in] */ long lPacketSize,
    /* [size_is][in] */ BYTE *pPacket);


void __RPC_STUB IEegData_StartSampling_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IEegData_StopSampling_Proxy( 
    IEegData * This);


void __RPC_STUB IEegData_StopSampling_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IEegData_WriteStaticInfoPacket_Proxy( 
    IEegData * This,
    /* [in] */ long lPacketSize,
    /* [size_is][in] */ BYTE *pPacket);


void __RPC_STUB IEegData_WriteStaticInfoPacket_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IEegData_CanWriteStaticInfoPacket_Proxy( 
    IEegData * This,
    /* [in] */ GUID guid,
    /* [retval][out] */ BOOL *pbVal);


void __RPC_STUB IEegData_CanWriteStaticInfoPacket_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IEegData_WriteInfoPacket_Proxy( 
    IEegData * This,
    /* [in] */ long lPacketSize,
    /* [size_is][in] */ BYTE *pPacket);


void __RPC_STUB IEegData_WriteInfoPacket_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IEegData_WriteData_Proxy( 
    IEegData * This,
    /* [in] */ long liChannel,
    /* [size_is][in] */ short *psData,
    /* [in] */ long lcSamples);


void __RPC_STUB IEegData_WriteData_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IEegData_Flush_Proxy( 
    IEegData * This);


void __RPC_STUB IEegData_Flush_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE IEegData_get_bSampling_Proxy( 
    IEegData * This,
    /* [retval][out] */ BOOL *pVal);


void __RPC_STUB IEegData_get_bSampling_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IEegData_GetDataPacket_Proxy( 
    IEegData * This,
    /* [in] */ NRVDATE nrvDate,
    /* [in] */ double dDuration,
    /* [in] */ long lPacketSize,
    /* [size_is][out] */ BYTE *pPacket);


void __RPC_STUB IEegData_GetDataPacket_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IEegData_GetChannelCount_Proxy( 
    IEegData * This,
    /* [in] */ NRVDATE nrvDate,
    /* [retval][out] */ long *plcChannels);


void __RPC_STUB IEegData_GetChannelCount_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IEegData_GetSampleCount_Proxy( 
    IEegData * This,
    /* [in] */ long liChannel,
    /* [in] */ NRVDATE nrvDate,
    /* [in] */ double dDuration,
    /* [retval][out] */ long *plSamples);


void __RPC_STUB IEegData_GetSampleCount_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IEegData_GetSampleCount2_Proxy( 
    IEegData * This,
    /* [in] */ NRVDATE nrvDate,
    /* [in] */ double dDuration,
    /* [in] */ long lcChannels,
    /* [size_is][out] */ long *plSamples);


void __RPC_STUB IEegData_GetSampleCount2_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IEegData_GetData_Proxy( 
    IEegData * This,
    /* [in] */ long liChannel,
    /* [in] */ NRVDATE nrvDate,
    /* [in] */ double dDuration,
    /* [in] */ long lcSamples,
    /* [size_is][out] */ short *psData);


void __RPC_STUB IEegData_GetData_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE IEegData_get_bSupportsGetData_Proxy( 
    IEegData * This,
    /* [retval][out] */ BOOL *pVal);


void __RPC_STUB IEegData_get_bSupportsGetData_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE IEegData_get_lcInfoChange_Proxy( 
    IEegData * This,
    /* [retval][out] */ long *pVal);


void __RPC_STUB IEegData_get_lcInfoChange_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IEegData_QueryLastInfoChange_Proxy( 
    IEegData * This,
    /* [in] */ GUID guidInfoChange,
    /* [in] */ NRVDATE nrvDate,
    /* [out] */ long *plPacketSize,
    /* [retval][out] */ long *pliChange);


void __RPC_STUB IEegData_QueryLastInfoChange_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IEegData_QueryNextInfoChange_Proxy( 
    IEegData * This,
    /* [in] */ NRVDATE nrvDate,
    /* [out] */ NRVDATE *pnrvDate,
    /* [out] */ GUID *pguidInfoPacket,
    /* [out] */ long *plPacketSize,
    /* [retval][out] */ long *pliChange);


void __RPC_STUB IEegData_QueryNextInfoChange_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IEegData_QueryNextInfoChange2_Proxy( 
    IEegData * This,
    /* [in] */ GUID guidInfoPacket,
    /* [in] */ long liChange,
    /* [out] */ long *plPacketSize,
    /* [retval][out] */ long *pliChange);


void __RPC_STUB IEegData_QueryNextInfoChange2_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IEegData_QueryInfoChange_Proxy( 
    IEegData * This,
    /* [in] */ long liInfoChange,
    /* [out] */ NRVDATE *pnrvDate,
    /* [out] */ long *plPacketSize,
    /* [retval][out] */ GUID *pguidInfoPacket);


void __RPC_STUB IEegData_QueryInfoChange_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IEegData_GetInfoPacket_Proxy( 
    IEegData * This,
    /* [in] */ GUID guidInfoPacket,
    /* [in] */ NRVDATE nrvDate,
    /* [in] */ long lPacketSize,
    /* [size_is][out] */ BYTE *pPacket);


void __RPC_STUB IEegData_GetInfoPacket_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IEegData_GetInfoPacket2_Proxy( 
    IEegData * This,
    /* [in] */ long liChange,
    /* [in] */ long lPacketSize,
    /* [size_is][out] */ BYTE *pPacket);


void __RPC_STUB IEegData_GetInfoPacket2_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IEegData_QueryStaticInfoPacketSize_Proxy( 
    IEegData * This,
    /* [in] */ GUID guid,
    /* [retval][out] */ long *plSize);


void __RPC_STUB IEegData_QueryStaticInfoPacketSize_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IEegData_GetStaticInfoPacket_Proxy( 
    IEegData * This,
    /* [in] */ GUID guid,
    /* [in] */ long lPacketSize,
    /* [size_is][out] */ BYTE *pPacket);


void __RPC_STUB IEegData_GetStaticInfoPacket_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IEegData_GetTestID_Proxy( 
    IEegData * This,
    /* [retval][out] */ BSTR *pVal);


void __RPC_STUB IEegData_GetTestID_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IEegData_ReadExtraData_Proxy( 
    IEegData * This,
    /* [in] */ GUID guidType,
    /* [in] */ BSTR bstrName,
    /* [in] */ LONGLONG llPos,
    /* [in] */ ULONG lcSamples,
    /* [size_is][out] */ BYTE *pSamples,
    /* [retval][out] */ ULONG *plSamplesRead);


void __RPC_STUB IEegData_ReadExtraData_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IEegData_WriteExtraData_Proxy( 
    IEegData * This,
    /* [in] */ GUID guidType,
    /* [in] */ BSTR bstrName,
    /* [in] */ LONGLONG llPos,
    /* [in] */ ULONG lcSamples,
    /* [size_is][in] */ BYTE *pSamples,
    /* [retval][out] */ ULONG *plSamplesWritten);


void __RPC_STUB IEegData_WriteExtraData_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IEegData_CreateExtraData_Proxy( 
    IEegData * This,
    /* [in] */ GUID guidType,
    /* [in] */ BSTR bstrName);


void __RPC_STUB IEegData_CreateExtraData_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IEegData_GetExtraDataSize_Proxy( 
    IEegData * This,
    /* [in] */ GUID guidType,
    /* [in] */ BSTR bstrName,
    /* [retval][out] */ ULONGLONG *pllSize);


void __RPC_STUB IEegData_GetExtraDataSize_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IEegData_INTERFACE_DEFINED__ */


#ifndef __IInitEegData_INTERFACE_DEFINED__
#define __IInitEegData_INTERFACE_DEFINED__

/* interface IInitEegData */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IInitEegData;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("98F4BD79-CE69-43fb-B561-A328C83C68BF")
    IInitEegData : public IUnknown
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Initialize( 
            /* [in] */ INrvData3 *pINrvData3) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Uninitialize( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IInitEegDataVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IInitEegData * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IInitEegData * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IInitEegData * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *Initialize )( 
            IInitEegData * This,
            /* [in] */ INrvData3 *pINrvData3);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *Uninitialize )( 
            IInitEegData * This);
        
        END_INTERFACE
    } IInitEegDataVtbl;

    interface IInitEegData
    {
        CONST_VTBL struct IInitEegDataVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IInitEegData_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IInitEegData_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IInitEegData_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IInitEegData_Initialize(This,pINrvData3)	\
    (This)->lpVtbl -> Initialize(This,pINrvData3)

#define IInitEegData_Uninitialize(This)	\
    (This)->lpVtbl -> Uninitialize(This)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring] */ HRESULT STDMETHODCALLTYPE IInitEegData_Initialize_Proxy( 
    IInitEegData * This,
    /* [in] */ INrvData3 *pINrvData3);


void __RPC_STUB IInitEegData_Initialize_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IInitEegData_Uninitialize_Proxy( 
    IInitEegData * This);


void __RPC_STUB IInitEegData_Uninitialize_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IInitEegData_INTERFACE_DEFINED__ */


#ifndef __IUserEvent_INTERFACE_DEFINED__
#define __IUserEvent_INTERFACE_DEFINED__

/* interface IUserEvent */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IUserEvent;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("BFD5C4C5-2835-486a-A301-1CE28D6146C1")
    IUserEvent : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Create( 
            /* [in] */ BSTR bstrEventType,
            /* [in] */ double dDate,
            /* [in] */ double dFraction) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetEventType( 
            /* [retval][out] */ BSTR *pbstrGUID) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetLabel( 
            /* [in] */ BSTR bstrLabel) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetLabel( 
            /* [retval][out] */ BSTR *pbstrLabel) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetText( 
            /* [in] */ BSTR bstrText) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetText( 
            /* [retval][out] */ BSTR *pbstrText) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetTimeStamp( 
            /* [in] */ double dDate,
            /* [in] */ double dFraction) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetTimeStamp( 
            /* [out] */ double *pdDate,
            /* [out] */ double *pdFraction) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetDuration( 
            /* [in] */ double dDuration) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetDuration( 
            /* [retval][out] */ double *pdDuration) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Clone( 
            /* [in] */ IUserEvent *pIUserEvent) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetUniqueGUID( 
            /* [retval][out] */ BSTR *bstrGUID) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetUser( 
            /* [retval][out] */ BSTR *bstrUser) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetCreationDate( 
            /* [retval][out] */ double *pdCreationDate) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetSystemDetected( 
            /* [retval][out] */ VARIANT_BOOL *pbSystem) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IUserEventVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IUserEvent * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IUserEvent * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IUserEvent * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IUserEvent * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IUserEvent * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IUserEvent * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IUserEvent * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Create )( 
            IUserEvent * This,
            /* [in] */ BSTR bstrEventType,
            /* [in] */ double dDate,
            /* [in] */ double dFraction);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetEventType )( 
            IUserEvent * This,
            /* [retval][out] */ BSTR *pbstrGUID);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetLabel )( 
            IUserEvent * This,
            /* [in] */ BSTR bstrLabel);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetLabel )( 
            IUserEvent * This,
            /* [retval][out] */ BSTR *pbstrLabel);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetText )( 
            IUserEvent * This,
            /* [in] */ BSTR bstrText);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetText )( 
            IUserEvent * This,
            /* [retval][out] */ BSTR *pbstrText);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetTimeStamp )( 
            IUserEvent * This,
            /* [in] */ double dDate,
            /* [in] */ double dFraction);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetTimeStamp )( 
            IUserEvent * This,
            /* [out] */ double *pdDate,
            /* [out] */ double *pdFraction);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetDuration )( 
            IUserEvent * This,
            /* [in] */ double dDuration);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetDuration )( 
            IUserEvent * This,
            /* [retval][out] */ double *pdDuration);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Clone )( 
            IUserEvent * This,
            /* [in] */ IUserEvent *pIUserEvent);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetUniqueGUID )( 
            IUserEvent * This,
            /* [retval][out] */ BSTR *bstrGUID);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetUser )( 
            IUserEvent * This,
            /* [retval][out] */ BSTR *bstrUser);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetCreationDate )( 
            IUserEvent * This,
            /* [retval][out] */ double *pdCreationDate);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetSystemDetected )( 
            IUserEvent * This,
            /* [retval][out] */ VARIANT_BOOL *pbSystem);
        
        END_INTERFACE
    } IUserEventVtbl;

    interface IUserEvent
    {
        CONST_VTBL struct IUserEventVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IUserEvent_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IUserEvent_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IUserEvent_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IUserEvent_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IUserEvent_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IUserEvent_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IUserEvent_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IUserEvent_Create(This,bstrEventType,dDate,dFraction)	\
    (This)->lpVtbl -> Create(This,bstrEventType,dDate,dFraction)

#define IUserEvent_GetEventType(This,pbstrGUID)	\
    (This)->lpVtbl -> GetEventType(This,pbstrGUID)

#define IUserEvent_SetLabel(This,bstrLabel)	\
    (This)->lpVtbl -> SetLabel(This,bstrLabel)

#define IUserEvent_GetLabel(This,pbstrLabel)	\
    (This)->lpVtbl -> GetLabel(This,pbstrLabel)

#define IUserEvent_SetText(This,bstrText)	\
    (This)->lpVtbl -> SetText(This,bstrText)

#define IUserEvent_GetText(This,pbstrText)	\
    (This)->lpVtbl -> GetText(This,pbstrText)

#define IUserEvent_SetTimeStamp(This,dDate,dFraction)	\
    (This)->lpVtbl -> SetTimeStamp(This,dDate,dFraction)

#define IUserEvent_GetTimeStamp(This,pdDate,pdFraction)	\
    (This)->lpVtbl -> GetTimeStamp(This,pdDate,pdFraction)

#define IUserEvent_SetDuration(This,dDuration)	\
    (This)->lpVtbl -> SetDuration(This,dDuration)

#define IUserEvent_GetDuration(This,pdDuration)	\
    (This)->lpVtbl -> GetDuration(This,pdDuration)

#define IUserEvent_Clone(This,pIUserEvent)	\
    (This)->lpVtbl -> Clone(This,pIUserEvent)

#define IUserEvent_GetUniqueGUID(This,bstrGUID)	\
    (This)->lpVtbl -> GetUniqueGUID(This,bstrGUID)

#define IUserEvent_GetUser(This,bstrUser)	\
    (This)->lpVtbl -> GetUser(This,bstrUser)

#define IUserEvent_GetCreationDate(This,pdCreationDate)	\
    (This)->lpVtbl -> GetCreationDate(This,pdCreationDate)

#define IUserEvent_GetSystemDetected(This,pbSystem)	\
    (This)->lpVtbl -> GetSystemDetected(This,pbSystem)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IUserEvent_Create_Proxy( 
    IUserEvent * This,
    /* [in] */ BSTR bstrEventType,
    /* [in] */ double dDate,
    /* [in] */ double dFraction);


void __RPC_STUB IUserEvent_Create_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IUserEvent_GetEventType_Proxy( 
    IUserEvent * This,
    /* [retval][out] */ BSTR *pbstrGUID);


void __RPC_STUB IUserEvent_GetEventType_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IUserEvent_SetLabel_Proxy( 
    IUserEvent * This,
    /* [in] */ BSTR bstrLabel);


void __RPC_STUB IUserEvent_SetLabel_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IUserEvent_GetLabel_Proxy( 
    IUserEvent * This,
    /* [retval][out] */ BSTR *pbstrLabel);


void __RPC_STUB IUserEvent_GetLabel_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IUserEvent_SetText_Proxy( 
    IUserEvent * This,
    /* [in] */ BSTR bstrText);


void __RPC_STUB IUserEvent_SetText_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IUserEvent_GetText_Proxy( 
    IUserEvent * This,
    /* [retval][out] */ BSTR *pbstrText);


void __RPC_STUB IUserEvent_GetText_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IUserEvent_SetTimeStamp_Proxy( 
    IUserEvent * This,
    /* [in] */ double dDate,
    /* [in] */ double dFraction);


void __RPC_STUB IUserEvent_SetTimeStamp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IUserEvent_GetTimeStamp_Proxy( 
    IUserEvent * This,
    /* [out] */ double *pdDate,
    /* [out] */ double *pdFraction);


void __RPC_STUB IUserEvent_GetTimeStamp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IUserEvent_SetDuration_Proxy( 
    IUserEvent * This,
    /* [in] */ double dDuration);


void __RPC_STUB IUserEvent_SetDuration_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IUserEvent_GetDuration_Proxy( 
    IUserEvent * This,
    /* [retval][out] */ double *pdDuration);


void __RPC_STUB IUserEvent_GetDuration_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IUserEvent_Clone_Proxy( 
    IUserEvent * This,
    /* [in] */ IUserEvent *pIUserEvent);


void __RPC_STUB IUserEvent_Clone_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IUserEvent_GetUniqueGUID_Proxy( 
    IUserEvent * This,
    /* [retval][out] */ BSTR *bstrGUID);


void __RPC_STUB IUserEvent_GetUniqueGUID_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IUserEvent_GetUser_Proxy( 
    IUserEvent * This,
    /* [retval][out] */ BSTR *bstrUser);


void __RPC_STUB IUserEvent_GetUser_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IUserEvent_GetCreationDate_Proxy( 
    IUserEvent * This,
    /* [retval][out] */ double *pdCreationDate);


void __RPC_STUB IUserEvent_GetCreationDate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IUserEvent_GetSystemDetected_Proxy( 
    IUserEvent * This,
    /* [retval][out] */ VARIANT_BOOL *pbSystem);


void __RPC_STUB IUserEvent_GetSystemDetected_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IUserEvent_INTERFACE_DEFINED__ */


#ifndef __IEventWrapper_INTERFACE_DEFINED__
#define __IEventWrapper_INTERFACE_DEFINED__

/* interface IEventWrapper */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IEventWrapper;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1E2F536C-A286-4d6b-A76E-E064ADE612FA")
    IEventWrapper : public IUnknown
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Clone( 
            /* [in] */ IEventWrapper *pIEventWrapper) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE DeleteEvent( 
            /* [in] */ BYTE *pPacket) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE ModifyEvent( 
            /* [in] */ BYTE *pPacket) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE InsertEvent( 
            /* [in] */ BYTE *pPacket) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE SetUserEventTypes( 
            /* [in] */ BYTE *pPacket) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE SetEvent( 
            /* [in] */ BYTE *pPacket) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetUserEventTypes( 
            /* [out] */ BYTE **pPacket) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetEvent( 
            /* [in] */ long lPos,
            /* [out] */ BYTE **ppPacket) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetEventFromGUID( 
            /* [in] */ GUID guid,
            /* [out] */ BYTE **ppPacket) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE QueryCount( 
            /* [retval][out] */ long *plcEvents) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE CopyEvent( 
            /* [in] */ BYTE *pPacket) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_bModified( 
            /* [retval][out] */ BOOL *pVal) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE AddEventNotify( 
            /* [in] */ IEventNotify *pIEventNotify) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE RemoveEventNotify( 
            /* [in] */ IEventNotify *pIEventNotify) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Save( void) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE PruneEvents( 
            /* [in] */ INrvSegments3 *pINrvSegments3) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE SetFilter( 
            /* [in] */ IEventFilter *pIEventFilter) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE CanModify( 
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_bFiltered( 
            /* [retval][out] */ BOOL *pVal) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE SetReadOnly( 
            /* [in] */ BOOL bReadOnly) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetEvent2( 
            /* [in] */ long lPos,
            /* [out] */ long *plSize,
            /* [size_is][size_is][out] */ BYTE **ppPacket) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE InsertEvent2( 
            /* [in] */ long lSize,
            /* [size_is][in] */ BYTE *pPacket) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE SetUserEventTypes2( 
            /* [in] */ long lSize,
            /* [size_is][in] */ BYTE *pPacket) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetUserEventTypes2( 
            /* [out] */ long *plSize,
            /* [size_is][size_is][out] */ BYTE **pPacket) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IEventWrapperVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IEventWrapper * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IEventWrapper * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IEventWrapper * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *Clone )( 
            IEventWrapper * This,
            /* [in] */ IEventWrapper *pIEventWrapper);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *DeleteEvent )( 
            IEventWrapper * This,
            /* [in] */ BYTE *pPacket);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *ModifyEvent )( 
            IEventWrapper * This,
            /* [in] */ BYTE *pPacket);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *InsertEvent )( 
            IEventWrapper * This,
            /* [in] */ BYTE *pPacket);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetUserEventTypes )( 
            IEventWrapper * This,
            /* [in] */ BYTE *pPacket);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetEvent )( 
            IEventWrapper * This,
            /* [in] */ BYTE *pPacket);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetUserEventTypes )( 
            IEventWrapper * This,
            /* [out] */ BYTE **pPacket);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetEvent )( 
            IEventWrapper * This,
            /* [in] */ long lPos,
            /* [out] */ BYTE **ppPacket);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetEventFromGUID )( 
            IEventWrapper * This,
            /* [in] */ GUID guid,
            /* [out] */ BYTE **ppPacket);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *QueryCount )( 
            IEventWrapper * This,
            /* [retval][out] */ long *plcEvents);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *CopyEvent )( 
            IEventWrapper * This,
            /* [in] */ BYTE *pPacket);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_bModified )( 
            IEventWrapper * This,
            /* [retval][out] */ BOOL *pVal);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *AddEventNotify )( 
            IEventWrapper * This,
            /* [in] */ IEventNotify *pIEventNotify);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *RemoveEventNotify )( 
            IEventWrapper * This,
            /* [in] */ IEventNotify *pIEventNotify);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *Save )( 
            IEventWrapper * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *PruneEvents )( 
            IEventWrapper * This,
            /* [in] */ INrvSegments3 *pINrvSegments3);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetFilter )( 
            IEventWrapper * This,
            /* [in] */ IEventFilter *pIEventFilter);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *CanModify )( 
            IEventWrapper * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_bFiltered )( 
            IEventWrapper * This,
            /* [retval][out] */ BOOL *pVal);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetReadOnly )( 
            IEventWrapper * This,
            /* [in] */ BOOL bReadOnly);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetEvent2 )( 
            IEventWrapper * This,
            /* [in] */ long lPos,
            /* [out] */ long *plSize,
            /* [size_is][size_is][out] */ BYTE **ppPacket);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *InsertEvent2 )( 
            IEventWrapper * This,
            /* [in] */ long lSize,
            /* [size_is][in] */ BYTE *pPacket);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetUserEventTypes2 )( 
            IEventWrapper * This,
            /* [in] */ long lSize,
            /* [size_is][in] */ BYTE *pPacket);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetUserEventTypes2 )( 
            IEventWrapper * This,
            /* [out] */ long *plSize,
            /* [size_is][size_is][out] */ BYTE **pPacket);
        
        END_INTERFACE
    } IEventWrapperVtbl;

    interface IEventWrapper
    {
        CONST_VTBL struct IEventWrapperVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IEventWrapper_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IEventWrapper_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IEventWrapper_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IEventWrapper_Clone(This,pIEventWrapper)	\
    (This)->lpVtbl -> Clone(This,pIEventWrapper)

#define IEventWrapper_DeleteEvent(This,pPacket)	\
    (This)->lpVtbl -> DeleteEvent(This,pPacket)

#define IEventWrapper_ModifyEvent(This,pPacket)	\
    (This)->lpVtbl -> ModifyEvent(This,pPacket)

#define IEventWrapper_InsertEvent(This,pPacket)	\
    (This)->lpVtbl -> InsertEvent(This,pPacket)

#define IEventWrapper_SetUserEventTypes(This,pPacket)	\
    (This)->lpVtbl -> SetUserEventTypes(This,pPacket)

#define IEventWrapper_SetEvent(This,pPacket)	\
    (This)->lpVtbl -> SetEvent(This,pPacket)

#define IEventWrapper_GetUserEventTypes(This,pPacket)	\
    (This)->lpVtbl -> GetUserEventTypes(This,pPacket)

#define IEventWrapper_GetEvent(This,lPos,ppPacket)	\
    (This)->lpVtbl -> GetEvent(This,lPos,ppPacket)

#define IEventWrapper_GetEventFromGUID(This,guid,ppPacket)	\
    (This)->lpVtbl -> GetEventFromGUID(This,guid,ppPacket)

#define IEventWrapper_QueryCount(This,plcEvents)	\
    (This)->lpVtbl -> QueryCount(This,plcEvents)

#define IEventWrapper_CopyEvent(This,pPacket)	\
    (This)->lpVtbl -> CopyEvent(This,pPacket)

#define IEventWrapper_get_bModified(This,pVal)	\
    (This)->lpVtbl -> get_bModified(This,pVal)

#define IEventWrapper_AddEventNotify(This,pIEventNotify)	\
    (This)->lpVtbl -> AddEventNotify(This,pIEventNotify)

#define IEventWrapper_RemoveEventNotify(This,pIEventNotify)	\
    (This)->lpVtbl -> RemoveEventNotify(This,pIEventNotify)

#define IEventWrapper_Save(This)	\
    (This)->lpVtbl -> Save(This)

#define IEventWrapper_PruneEvents(This,pINrvSegments3)	\
    (This)->lpVtbl -> PruneEvents(This,pINrvSegments3)

#define IEventWrapper_SetFilter(This,pIEventFilter)	\
    (This)->lpVtbl -> SetFilter(This,pIEventFilter)

#define IEventWrapper_CanModify(This,pVal)	\
    (This)->lpVtbl -> CanModify(This,pVal)

#define IEventWrapper_get_bFiltered(This,pVal)	\
    (This)->lpVtbl -> get_bFiltered(This,pVal)

#define IEventWrapper_SetReadOnly(This,bReadOnly)	\
    (This)->lpVtbl -> SetReadOnly(This,bReadOnly)

#define IEventWrapper_GetEvent2(This,lPos,plSize,ppPacket)	\
    (This)->lpVtbl -> GetEvent2(This,lPos,plSize,ppPacket)

#define IEventWrapper_InsertEvent2(This,lSize,pPacket)	\
    (This)->lpVtbl -> InsertEvent2(This,lSize,pPacket)

#define IEventWrapper_SetUserEventTypes2(This,lSize,pPacket)	\
    (This)->lpVtbl -> SetUserEventTypes2(This,lSize,pPacket)

#define IEventWrapper_GetUserEventTypes2(This,plSize,pPacket)	\
    (This)->lpVtbl -> GetUserEventTypes2(This,plSize,pPacket)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring] */ HRESULT STDMETHODCALLTYPE IEventWrapper_Clone_Proxy( 
    IEventWrapper * This,
    /* [in] */ IEventWrapper *pIEventWrapper);


void __RPC_STUB IEventWrapper_Clone_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IEventWrapper_DeleteEvent_Proxy( 
    IEventWrapper * This,
    /* [in] */ BYTE *pPacket);


void __RPC_STUB IEventWrapper_DeleteEvent_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IEventWrapper_ModifyEvent_Proxy( 
    IEventWrapper * This,
    /* [in] */ BYTE *pPacket);


void __RPC_STUB IEventWrapper_ModifyEvent_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IEventWrapper_InsertEvent_Proxy( 
    IEventWrapper * This,
    /* [in] */ BYTE *pPacket);


void __RPC_STUB IEventWrapper_InsertEvent_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IEventWrapper_SetUserEventTypes_Proxy( 
    IEventWrapper * This,
    /* [in] */ BYTE *pPacket);


void __RPC_STUB IEventWrapper_SetUserEventTypes_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IEventWrapper_SetEvent_Proxy( 
    IEventWrapper * This,
    /* [in] */ BYTE *pPacket);


void __RPC_STUB IEventWrapper_SetEvent_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IEventWrapper_GetUserEventTypes_Proxy( 
    IEventWrapper * This,
    /* [out] */ BYTE **pPacket);


void __RPC_STUB IEventWrapper_GetUserEventTypes_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IEventWrapper_GetEvent_Proxy( 
    IEventWrapper * This,
    /* [in] */ long lPos,
    /* [out] */ BYTE **ppPacket);


void __RPC_STUB IEventWrapper_GetEvent_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IEventWrapper_GetEventFromGUID_Proxy( 
    IEventWrapper * This,
    /* [in] */ GUID guid,
    /* [out] */ BYTE **ppPacket);


void __RPC_STUB IEventWrapper_GetEventFromGUID_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IEventWrapper_QueryCount_Proxy( 
    IEventWrapper * This,
    /* [retval][out] */ long *plcEvents);


void __RPC_STUB IEventWrapper_QueryCount_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IEventWrapper_CopyEvent_Proxy( 
    IEventWrapper * This,
    /* [in] */ BYTE *pPacket);


void __RPC_STUB IEventWrapper_CopyEvent_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE IEventWrapper_get_bModified_Proxy( 
    IEventWrapper * This,
    /* [retval][out] */ BOOL *pVal);


void __RPC_STUB IEventWrapper_get_bModified_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IEventWrapper_AddEventNotify_Proxy( 
    IEventWrapper * This,
    /* [in] */ IEventNotify *pIEventNotify);


void __RPC_STUB IEventWrapper_AddEventNotify_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IEventWrapper_RemoveEventNotify_Proxy( 
    IEventWrapper * This,
    /* [in] */ IEventNotify *pIEventNotify);


void __RPC_STUB IEventWrapper_RemoveEventNotify_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IEventWrapper_Save_Proxy( 
    IEventWrapper * This);


void __RPC_STUB IEventWrapper_Save_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IEventWrapper_PruneEvents_Proxy( 
    IEventWrapper * This,
    /* [in] */ INrvSegments3 *pINrvSegments3);


void __RPC_STUB IEventWrapper_PruneEvents_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IEventWrapper_SetFilter_Proxy( 
    IEventWrapper * This,
    /* [in] */ IEventFilter *pIEventFilter);


void __RPC_STUB IEventWrapper_SetFilter_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IEventWrapper_CanModify_Proxy( 
    IEventWrapper * This,
    /* [retval][out] */ VARIANT_BOOL *pVal);


void __RPC_STUB IEventWrapper_CanModify_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE IEventWrapper_get_bFiltered_Proxy( 
    IEventWrapper * This,
    /* [retval][out] */ BOOL *pVal);


void __RPC_STUB IEventWrapper_get_bFiltered_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IEventWrapper_SetReadOnly_Proxy( 
    IEventWrapper * This,
    /* [in] */ BOOL bReadOnly);


void __RPC_STUB IEventWrapper_SetReadOnly_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IEventWrapper_GetEvent2_Proxy( 
    IEventWrapper * This,
    /* [in] */ long lPos,
    /* [out] */ long *plSize,
    /* [size_is][size_is][out] */ BYTE **ppPacket);


void __RPC_STUB IEventWrapper_GetEvent2_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IEventWrapper_InsertEvent2_Proxy( 
    IEventWrapper * This,
    /* [in] */ long lSize,
    /* [size_is][in] */ BYTE *pPacket);


void __RPC_STUB IEventWrapper_InsertEvent2_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IEventWrapper_SetUserEventTypes2_Proxy( 
    IEventWrapper * This,
    /* [in] */ long lSize,
    /* [size_is][in] */ BYTE *pPacket);


void __RPC_STUB IEventWrapper_SetUserEventTypes2_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IEventWrapper_GetUserEventTypes2_Proxy( 
    IEventWrapper * This,
    /* [out] */ long *plSize,
    /* [size_is][size_is][out] */ BYTE **pPacket);


void __RPC_STUB IEventWrapper_GetUserEventTypes2_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IEventWrapper_INTERFACE_DEFINED__ */


#ifndef __IInitEventWrapper_INTERFACE_DEFINED__
#define __IInitEventWrapper_INTERFACE_DEFINED__

/* interface IInitEventWrapper */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IInitEventWrapper;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("26228DE0-54CE-4e79-BF01-052BBDF21277")
    IInitEventWrapper : public IEventWrapper
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Initialize( 
            /* [in] */ INrvData3 *pINrvData3,
            /* [in] */ long lWriteMode) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Uninitialize( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IInitEventWrapperVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IInitEventWrapper * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IInitEventWrapper * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IInitEventWrapper * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *Clone )( 
            IInitEventWrapper * This,
            /* [in] */ IEventWrapper *pIEventWrapper);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *DeleteEvent )( 
            IInitEventWrapper * This,
            /* [in] */ BYTE *pPacket);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *ModifyEvent )( 
            IInitEventWrapper * This,
            /* [in] */ BYTE *pPacket);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *InsertEvent )( 
            IInitEventWrapper * This,
            /* [in] */ BYTE *pPacket);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetUserEventTypes )( 
            IInitEventWrapper * This,
            /* [in] */ BYTE *pPacket);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetEvent )( 
            IInitEventWrapper * This,
            /* [in] */ BYTE *pPacket);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetUserEventTypes )( 
            IInitEventWrapper * This,
            /* [out] */ BYTE **pPacket);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetEvent )( 
            IInitEventWrapper * This,
            /* [in] */ long lPos,
            /* [out] */ BYTE **ppPacket);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetEventFromGUID )( 
            IInitEventWrapper * This,
            /* [in] */ GUID guid,
            /* [out] */ BYTE **ppPacket);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *QueryCount )( 
            IInitEventWrapper * This,
            /* [retval][out] */ long *plcEvents);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *CopyEvent )( 
            IInitEventWrapper * This,
            /* [in] */ BYTE *pPacket);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_bModified )( 
            IInitEventWrapper * This,
            /* [retval][out] */ BOOL *pVal);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *AddEventNotify )( 
            IInitEventWrapper * This,
            /* [in] */ IEventNotify *pIEventNotify);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *RemoveEventNotify )( 
            IInitEventWrapper * This,
            /* [in] */ IEventNotify *pIEventNotify);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *Save )( 
            IInitEventWrapper * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *PruneEvents )( 
            IInitEventWrapper * This,
            /* [in] */ INrvSegments3 *pINrvSegments3);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetFilter )( 
            IInitEventWrapper * This,
            /* [in] */ IEventFilter *pIEventFilter);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *CanModify )( 
            IInitEventWrapper * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_bFiltered )( 
            IInitEventWrapper * This,
            /* [retval][out] */ BOOL *pVal);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetReadOnly )( 
            IInitEventWrapper * This,
            /* [in] */ BOOL bReadOnly);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetEvent2 )( 
            IInitEventWrapper * This,
            /* [in] */ long lPos,
            /* [out] */ long *plSize,
            /* [size_is][size_is][out] */ BYTE **ppPacket);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *InsertEvent2 )( 
            IInitEventWrapper * This,
            /* [in] */ long lSize,
            /* [size_is][in] */ BYTE *pPacket);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetUserEventTypes2 )( 
            IInitEventWrapper * This,
            /* [in] */ long lSize,
            /* [size_is][in] */ BYTE *pPacket);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetUserEventTypes2 )( 
            IInitEventWrapper * This,
            /* [out] */ long *plSize,
            /* [size_is][size_is][out] */ BYTE **pPacket);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *Initialize )( 
            IInitEventWrapper * This,
            /* [in] */ INrvData3 *pINrvData3,
            /* [in] */ long lWriteMode);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *Uninitialize )( 
            IInitEventWrapper * This);
        
        END_INTERFACE
    } IInitEventWrapperVtbl;

    interface IInitEventWrapper
    {
        CONST_VTBL struct IInitEventWrapperVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IInitEventWrapper_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IInitEventWrapper_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IInitEventWrapper_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IInitEventWrapper_Clone(This,pIEventWrapper)	\
    (This)->lpVtbl -> Clone(This,pIEventWrapper)

#define IInitEventWrapper_DeleteEvent(This,pPacket)	\
    (This)->lpVtbl -> DeleteEvent(This,pPacket)

#define IInitEventWrapper_ModifyEvent(This,pPacket)	\
    (This)->lpVtbl -> ModifyEvent(This,pPacket)

#define IInitEventWrapper_InsertEvent(This,pPacket)	\
    (This)->lpVtbl -> InsertEvent(This,pPacket)

#define IInitEventWrapper_SetUserEventTypes(This,pPacket)	\
    (This)->lpVtbl -> SetUserEventTypes(This,pPacket)

#define IInitEventWrapper_SetEvent(This,pPacket)	\
    (This)->lpVtbl -> SetEvent(This,pPacket)

#define IInitEventWrapper_GetUserEventTypes(This,pPacket)	\
    (This)->lpVtbl -> GetUserEventTypes(This,pPacket)

#define IInitEventWrapper_GetEvent(This,lPos,ppPacket)	\
    (This)->lpVtbl -> GetEvent(This,lPos,ppPacket)

#define IInitEventWrapper_GetEventFromGUID(This,guid,ppPacket)	\
    (This)->lpVtbl -> GetEventFromGUID(This,guid,ppPacket)

#define IInitEventWrapper_QueryCount(This,plcEvents)	\
    (This)->lpVtbl -> QueryCount(This,plcEvents)

#define IInitEventWrapper_CopyEvent(This,pPacket)	\
    (This)->lpVtbl -> CopyEvent(This,pPacket)

#define IInitEventWrapper_get_bModified(This,pVal)	\
    (This)->lpVtbl -> get_bModified(This,pVal)

#define IInitEventWrapper_AddEventNotify(This,pIEventNotify)	\
    (This)->lpVtbl -> AddEventNotify(This,pIEventNotify)

#define IInitEventWrapper_RemoveEventNotify(This,pIEventNotify)	\
    (This)->lpVtbl -> RemoveEventNotify(This,pIEventNotify)

#define IInitEventWrapper_Save(This)	\
    (This)->lpVtbl -> Save(This)

#define IInitEventWrapper_PruneEvents(This,pINrvSegments3)	\
    (This)->lpVtbl -> PruneEvents(This,pINrvSegments3)

#define IInitEventWrapper_SetFilter(This,pIEventFilter)	\
    (This)->lpVtbl -> SetFilter(This,pIEventFilter)

#define IInitEventWrapper_CanModify(This,pVal)	\
    (This)->lpVtbl -> CanModify(This,pVal)

#define IInitEventWrapper_get_bFiltered(This,pVal)	\
    (This)->lpVtbl -> get_bFiltered(This,pVal)

#define IInitEventWrapper_SetReadOnly(This,bReadOnly)	\
    (This)->lpVtbl -> SetReadOnly(This,bReadOnly)

#define IInitEventWrapper_GetEvent2(This,lPos,plSize,ppPacket)	\
    (This)->lpVtbl -> GetEvent2(This,lPos,plSize,ppPacket)

#define IInitEventWrapper_InsertEvent2(This,lSize,pPacket)	\
    (This)->lpVtbl -> InsertEvent2(This,lSize,pPacket)

#define IInitEventWrapper_SetUserEventTypes2(This,lSize,pPacket)	\
    (This)->lpVtbl -> SetUserEventTypes2(This,lSize,pPacket)

#define IInitEventWrapper_GetUserEventTypes2(This,plSize,pPacket)	\
    (This)->lpVtbl -> GetUserEventTypes2(This,plSize,pPacket)


#define IInitEventWrapper_Initialize(This,pINrvData3,lWriteMode)	\
    (This)->lpVtbl -> Initialize(This,pINrvData3,lWriteMode)

#define IInitEventWrapper_Uninitialize(This)	\
    (This)->lpVtbl -> Uninitialize(This)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring] */ HRESULT STDMETHODCALLTYPE IInitEventWrapper_Initialize_Proxy( 
    IInitEventWrapper * This,
    /* [in] */ INrvData3 *pINrvData3,
    /* [in] */ long lWriteMode);


void __RPC_STUB IInitEventWrapper_Initialize_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IInitEventWrapper_Uninitialize_Proxy( 
    IInitEventWrapper * This);


void __RPC_STUB IInitEventWrapper_Uninitialize_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IInitEventWrapper_INTERFACE_DEFINED__ */


#ifndef __IUserEvent2_INTERFACE_DEFINED__
#define __IUserEvent2_INTERFACE_DEFINED__

/* interface IUserEvent2 */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IUserEvent2;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("B5D85332-1313-4fe5-8BA9-1959DB24D12A")
    IUserEvent2 : public IUnknown
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetEventMarker( 
            /* [out] */ EVENTMARKER *pEventMarker) = 0;
        
        virtual /* [helpstring][local] */ HRESULT STDMETHODCALLTYPE SetPacket( 
            /* [in] */ BYTE *pPacket) = 0;
        
        virtual /* [helpstring][local] */ HRESULT STDMETHODCALLTYPE GetPacket( 
            /* [out] */ BYTE **ppPacket) = 0;
        
        virtual /* [helpstring][local] */ HRESULT STDMETHODCALLTYPE SetPacketPermanent( 
            /* [in] */ BYTE *pPacket) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE SetPacket2( 
            /* [in] */ long lSize,
            /* [size_is][in] */ BYTE *pPacket) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetPacket2( 
            /* [out] */ long *plSize,
            /* [size_is][size_is][out] */ BYTE **ppPacket) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE SetPacketPermanent2( 
            /* [in] */ long lSize,
            /* [size_is][in] */ BYTE *pPacket) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IUserEvent2Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IUserEvent2 * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IUserEvent2 * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IUserEvent2 * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetEventMarker )( 
            IUserEvent2 * This,
            /* [out] */ EVENTMARKER *pEventMarker);
        
        /* [helpstring][local] */ HRESULT ( STDMETHODCALLTYPE *SetPacket )( 
            IUserEvent2 * This,
            /* [in] */ BYTE *pPacket);
        
        /* [helpstring][local] */ HRESULT ( STDMETHODCALLTYPE *GetPacket )( 
            IUserEvent2 * This,
            /* [out] */ BYTE **ppPacket);
        
        /* [helpstring][local] */ HRESULT ( STDMETHODCALLTYPE *SetPacketPermanent )( 
            IUserEvent2 * This,
            /* [in] */ BYTE *pPacket);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetPacket2 )( 
            IUserEvent2 * This,
            /* [in] */ long lSize,
            /* [size_is][in] */ BYTE *pPacket);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetPacket2 )( 
            IUserEvent2 * This,
            /* [out] */ long *plSize,
            /* [size_is][size_is][out] */ BYTE **ppPacket);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetPacketPermanent2 )( 
            IUserEvent2 * This,
            /* [in] */ long lSize,
            /* [size_is][in] */ BYTE *pPacket);
        
        END_INTERFACE
    } IUserEvent2Vtbl;

    interface IUserEvent2
    {
        CONST_VTBL struct IUserEvent2Vtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IUserEvent2_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IUserEvent2_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IUserEvent2_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IUserEvent2_GetEventMarker(This,pEventMarker)	\
    (This)->lpVtbl -> GetEventMarker(This,pEventMarker)

#define IUserEvent2_SetPacket(This,pPacket)	\
    (This)->lpVtbl -> SetPacket(This,pPacket)

#define IUserEvent2_GetPacket(This,ppPacket)	\
    (This)->lpVtbl -> GetPacket(This,ppPacket)

#define IUserEvent2_SetPacketPermanent(This,pPacket)	\
    (This)->lpVtbl -> SetPacketPermanent(This,pPacket)

#define IUserEvent2_SetPacket2(This,lSize,pPacket)	\
    (This)->lpVtbl -> SetPacket2(This,lSize,pPacket)

#define IUserEvent2_GetPacket2(This,plSize,ppPacket)	\
    (This)->lpVtbl -> GetPacket2(This,plSize,ppPacket)

#define IUserEvent2_SetPacketPermanent2(This,lSize,pPacket)	\
    (This)->lpVtbl -> SetPacketPermanent2(This,lSize,pPacket)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring] */ HRESULT STDMETHODCALLTYPE IUserEvent2_GetEventMarker_Proxy( 
    IUserEvent2 * This,
    /* [out] */ EVENTMARKER *pEventMarker);


void __RPC_STUB IUserEvent2_GetEventMarker_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][local] */ HRESULT STDMETHODCALLTYPE IUserEvent2_SetPacket_Proxy( 
    IUserEvent2 * This,
    /* [in] */ BYTE *pPacket);


void __RPC_STUB IUserEvent2_SetPacket_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][local] */ HRESULT STDMETHODCALLTYPE IUserEvent2_GetPacket_Proxy( 
    IUserEvent2 * This,
    /* [out] */ BYTE **ppPacket);


void __RPC_STUB IUserEvent2_GetPacket_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][local] */ HRESULT STDMETHODCALLTYPE IUserEvent2_SetPacketPermanent_Proxy( 
    IUserEvent2 * This,
    /* [in] */ BYTE *pPacket);


void __RPC_STUB IUserEvent2_SetPacketPermanent_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IUserEvent2_SetPacket2_Proxy( 
    IUserEvent2 * This,
    /* [in] */ long lSize,
    /* [size_is][in] */ BYTE *pPacket);


void __RPC_STUB IUserEvent2_SetPacket2_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IUserEvent2_GetPacket2_Proxy( 
    IUserEvent2 * This,
    /* [out] */ long *plSize,
    /* [size_is][size_is][out] */ BYTE **ppPacket);


void __RPC_STUB IUserEvent2_GetPacket2_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IUserEvent2_SetPacketPermanent2_Proxy( 
    IUserEvent2 * This,
    /* [in] */ long lSize,
    /* [size_is][in] */ BYTE *pPacket);


void __RPC_STUB IUserEvent2_SetPacketPermanent2_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IUserEvent2_INTERFACE_DEFINED__ */


#ifndef __IUserEvents_INTERFACE_DEFINED__
#define __IUserEvents_INTERFACE_DEFINED__

/* interface IUserEvents */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IUserEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("0272DEF0-57BD-4841-BD55-3EF6F96CA073")
    IUserEvents : public IDispatch
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetCount( 
            /* [retval][out] */ long *plcEvents) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetAt( 
            /* [in] */ long liEvent,
            /* [retval][out] */ IUserEvent **ppIEvent) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetFromGUID( 
            /* [in] */ BSTR bstrGUID,
            /* [retval][out] */ IUserEvent **ppIEvent) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Delete( 
            /* [in] */ long liEvent) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE DeleteFromGUID( 
            /* [in] */ BSTR bstrGUID) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Modify( 
            /* [in] */ IUserEvent *pIEvent) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Insert( 
            /* [in] */ IUserEvent *pIEvent) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Seek( 
            /* [in] */ double dDate,
            /* [in] */ double dFraction,
            /* [retval][out] */ long *pliEvent) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE AddEventNotify( 
            /* [in] */ IEventNotify *pIEventNotify) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE RemoveEventNotify( 
            /* [in] */ IEventNotify *pIEventNotify) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetEventsFromEventType( 
            /* [in] */ BSTR bstrEventTypeGUID,
            /* [in] */ IUserEvents *pIUserEvents) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Clone( 
            /* [in] */ IUserEvents *pIUserEvents) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Clear( void) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE IsEvent( 
            /* [in] */ BSTR bstrUniqueGuid) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE CanModify( 
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetEventTypes( 
            /* [retval][out] */ IDispatch **ppIEventCache) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE SetEventTypes( 
            /* [in] */ IDispatch *pIEventCache) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_bModified( 
            /* [retval][out] */ BOOL *pVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IUserEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IUserEvents * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IUserEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IUserEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IUserEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IUserEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IUserEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IUserEvents * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetCount )( 
            IUserEvents * This,
            /* [retval][out] */ long *plcEvents);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetAt )( 
            IUserEvents * This,
            /* [in] */ long liEvent,
            /* [retval][out] */ IUserEvent **ppIEvent);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetFromGUID )( 
            IUserEvents * This,
            /* [in] */ BSTR bstrGUID,
            /* [retval][out] */ IUserEvent **ppIEvent);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *Delete )( 
            IUserEvents * This,
            /* [in] */ long liEvent);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *DeleteFromGUID )( 
            IUserEvents * This,
            /* [in] */ BSTR bstrGUID);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *Modify )( 
            IUserEvents * This,
            /* [in] */ IUserEvent *pIEvent);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *Insert )( 
            IUserEvents * This,
            /* [in] */ IUserEvent *pIEvent);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *Seek )( 
            IUserEvents * This,
            /* [in] */ double dDate,
            /* [in] */ double dFraction,
            /* [retval][out] */ long *pliEvent);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *AddEventNotify )( 
            IUserEvents * This,
            /* [in] */ IEventNotify *pIEventNotify);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *RemoveEventNotify )( 
            IUserEvents * This,
            /* [in] */ IEventNotify *pIEventNotify);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetEventsFromEventType )( 
            IUserEvents * This,
            /* [in] */ BSTR bstrEventTypeGUID,
            /* [in] */ IUserEvents *pIUserEvents);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *Clone )( 
            IUserEvents * This,
            /* [in] */ IUserEvents *pIUserEvents);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *Clear )( 
            IUserEvents * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *IsEvent )( 
            IUserEvents * This,
            /* [in] */ BSTR bstrUniqueGuid);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *CanModify )( 
            IUserEvents * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetEventTypes )( 
            IUserEvents * This,
            /* [retval][out] */ IDispatch **ppIEventCache);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetEventTypes )( 
            IUserEvents * This,
            /* [in] */ IDispatch *pIEventCache);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_bModified )( 
            IUserEvents * This,
            /* [retval][out] */ BOOL *pVal);
        
        END_INTERFACE
    } IUserEventsVtbl;

    interface IUserEvents
    {
        CONST_VTBL struct IUserEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IUserEvents_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IUserEvents_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IUserEvents_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IUserEvents_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IUserEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IUserEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IUserEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IUserEvents_GetCount(This,plcEvents)	\
    (This)->lpVtbl -> GetCount(This,plcEvents)

#define IUserEvents_GetAt(This,liEvent,ppIEvent)	\
    (This)->lpVtbl -> GetAt(This,liEvent,ppIEvent)

#define IUserEvents_GetFromGUID(This,bstrGUID,ppIEvent)	\
    (This)->lpVtbl -> GetFromGUID(This,bstrGUID,ppIEvent)

#define IUserEvents_Delete(This,liEvent)	\
    (This)->lpVtbl -> Delete(This,liEvent)

#define IUserEvents_DeleteFromGUID(This,bstrGUID)	\
    (This)->lpVtbl -> DeleteFromGUID(This,bstrGUID)

#define IUserEvents_Modify(This,pIEvent)	\
    (This)->lpVtbl -> Modify(This,pIEvent)

#define IUserEvents_Insert(This,pIEvent)	\
    (This)->lpVtbl -> Insert(This,pIEvent)

#define IUserEvents_Seek(This,dDate,dFraction,pliEvent)	\
    (This)->lpVtbl -> Seek(This,dDate,dFraction,pliEvent)

#define IUserEvents_AddEventNotify(This,pIEventNotify)	\
    (This)->lpVtbl -> AddEventNotify(This,pIEventNotify)

#define IUserEvents_RemoveEventNotify(This,pIEventNotify)	\
    (This)->lpVtbl -> RemoveEventNotify(This,pIEventNotify)

#define IUserEvents_GetEventsFromEventType(This,bstrEventTypeGUID,pIUserEvents)	\
    (This)->lpVtbl -> GetEventsFromEventType(This,bstrEventTypeGUID,pIUserEvents)

#define IUserEvents_Clone(This,pIUserEvents)	\
    (This)->lpVtbl -> Clone(This,pIUserEvents)

#define IUserEvents_Clear(This)	\
    (This)->lpVtbl -> Clear(This)

#define IUserEvents_IsEvent(This,bstrUniqueGuid)	\
    (This)->lpVtbl -> IsEvent(This,bstrUniqueGuid)

#define IUserEvents_CanModify(This,pVal)	\
    (This)->lpVtbl -> CanModify(This,pVal)

#define IUserEvents_GetEventTypes(This,ppIEventCache)	\
    (This)->lpVtbl -> GetEventTypes(This,ppIEventCache)

#define IUserEvents_SetEventTypes(This,pIEventCache)	\
    (This)->lpVtbl -> SetEventTypes(This,pIEventCache)

#define IUserEvents_get_bModified(This,pVal)	\
    (This)->lpVtbl -> get_bModified(This,pVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring] */ HRESULT STDMETHODCALLTYPE IUserEvents_GetCount_Proxy( 
    IUserEvents * This,
    /* [retval][out] */ long *plcEvents);


void __RPC_STUB IUserEvents_GetCount_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IUserEvents_GetAt_Proxy( 
    IUserEvents * This,
    /* [in] */ long liEvent,
    /* [retval][out] */ IUserEvent **ppIEvent);


void __RPC_STUB IUserEvents_GetAt_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IUserEvents_GetFromGUID_Proxy( 
    IUserEvents * This,
    /* [in] */ BSTR bstrGUID,
    /* [retval][out] */ IUserEvent **ppIEvent);


void __RPC_STUB IUserEvents_GetFromGUID_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IUserEvents_Delete_Proxy( 
    IUserEvents * This,
    /* [in] */ long liEvent);


void __RPC_STUB IUserEvents_Delete_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IUserEvents_DeleteFromGUID_Proxy( 
    IUserEvents * This,
    /* [in] */ BSTR bstrGUID);


void __RPC_STUB IUserEvents_DeleteFromGUID_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IUserEvents_Modify_Proxy( 
    IUserEvents * This,
    /* [in] */ IUserEvent *pIEvent);


void __RPC_STUB IUserEvents_Modify_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IUserEvents_Insert_Proxy( 
    IUserEvents * This,
    /* [in] */ IUserEvent *pIEvent);


void __RPC_STUB IUserEvents_Insert_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IUserEvents_Seek_Proxy( 
    IUserEvents * This,
    /* [in] */ double dDate,
    /* [in] */ double dFraction,
    /* [retval][out] */ long *pliEvent);


void __RPC_STUB IUserEvents_Seek_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IUserEvents_AddEventNotify_Proxy( 
    IUserEvents * This,
    /* [in] */ IEventNotify *pIEventNotify);


void __RPC_STUB IUserEvents_AddEventNotify_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IUserEvents_RemoveEventNotify_Proxy( 
    IUserEvents * This,
    /* [in] */ IEventNotify *pIEventNotify);


void __RPC_STUB IUserEvents_RemoveEventNotify_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IUserEvents_GetEventsFromEventType_Proxy( 
    IUserEvents * This,
    /* [in] */ BSTR bstrEventTypeGUID,
    /* [in] */ IUserEvents *pIUserEvents);


void __RPC_STUB IUserEvents_GetEventsFromEventType_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IUserEvents_Clone_Proxy( 
    IUserEvents * This,
    /* [in] */ IUserEvents *pIUserEvents);


void __RPC_STUB IUserEvents_Clone_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IUserEvents_Clear_Proxy( 
    IUserEvents * This);


void __RPC_STUB IUserEvents_Clear_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IUserEvents_IsEvent_Proxy( 
    IUserEvents * This,
    /* [in] */ BSTR bstrUniqueGuid);


void __RPC_STUB IUserEvents_IsEvent_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IUserEvents_CanModify_Proxy( 
    IUserEvents * This,
    /* [retval][out] */ VARIANT_BOOL *pVal);


void __RPC_STUB IUserEvents_CanModify_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IUserEvents_GetEventTypes_Proxy( 
    IUserEvents * This,
    /* [retval][out] */ IDispatch **ppIEventCache);


void __RPC_STUB IUserEvents_GetEventTypes_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IUserEvents_SetEventTypes_Proxy( 
    IUserEvents * This,
    /* [in] */ IDispatch *pIEventCache);


void __RPC_STUB IUserEvents_SetEventTypes_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE IUserEvents_get_bModified_Proxy( 
    IUserEvents * This,
    /* [retval][out] */ BOOL *pVal);


void __RPC_STUB IUserEvents_get_bModified_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IUserEvents_INTERFACE_DEFINED__ */


#ifndef __IEventUsers_INTERFACE_DEFINED__
#define __IEventUsers_INTERFACE_DEFINED__

/* interface IEventUsers */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IEventUsers;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("337E0E77-E7EF-4c3b-9747-62532CDBCDE0")
    IEventUsers : public IUnknown
    {
    public:
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_lcUsers( 
            /* [retval][out] */ long *plcUsers) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_strUser( 
            /* [in] */ long liIndex,
            /* [retval][out] */ BSTR *pbstrUser) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IEventUsersVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IEventUsers * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IEventUsers * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IEventUsers * This);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_lcUsers )( 
            IEventUsers * This,
            /* [retval][out] */ long *plcUsers);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_strUser )( 
            IEventUsers * This,
            /* [in] */ long liIndex,
            /* [retval][out] */ BSTR *pbstrUser);
        
        END_INTERFACE
    } IEventUsersVtbl;

    interface IEventUsers
    {
        CONST_VTBL struct IEventUsersVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IEventUsers_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IEventUsers_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IEventUsers_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IEventUsers_get_lcUsers(This,plcUsers)	\
    (This)->lpVtbl -> get_lcUsers(This,plcUsers)

#define IEventUsers_get_strUser(This,liIndex,pbstrUser)	\
    (This)->lpVtbl -> get_strUser(This,liIndex,pbstrUser)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE IEventUsers_get_lcUsers_Proxy( 
    IEventUsers * This,
    /* [retval][out] */ long *plcUsers);


void __RPC_STUB IEventUsers_get_lcUsers_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE IEventUsers_get_strUser_Proxy( 
    IEventUsers * This,
    /* [in] */ long liIndex,
    /* [retval][out] */ BSTR *pbstrUser);


void __RPC_STUB IEventUsers_get_strUser_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IEventUsers_INTERFACE_DEFINED__ */


#ifndef __ITrendWrite_INTERFACE_DEFINED__
#define __ITrendWrite_INTERFACE_DEFINED__

/* interface ITrendWrite */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_ITrendWrite;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("8AB24A6B-623B-46af-8191-07B99ADDCAE3")
    ITrendWrite : public IUnknown
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE SetEegStorage( 
            IEegStorage *pIEegStorage) = 0;
        
        virtual /* [helpstring][propput] */ HRESULT STDMETHODCALLTYPE put_bWriteImmediately( 
            /* [in] */ VARIANT_BOOL bWriteImmediatly) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_bWriteImmediately( 
            /* [retval][out] */ VARIANT_BOOL *pbWriteImmediatly) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ITrendWriteVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ITrendWrite * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ITrendWrite * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ITrendWrite * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetEegStorage )( 
            ITrendWrite * This,
            IEegStorage *pIEegStorage);
        
        /* [helpstring][propput] */ HRESULT ( STDMETHODCALLTYPE *put_bWriteImmediately )( 
            ITrendWrite * This,
            /* [in] */ VARIANT_BOOL bWriteImmediatly);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_bWriteImmediately )( 
            ITrendWrite * This,
            /* [retval][out] */ VARIANT_BOOL *pbWriteImmediatly);
        
        END_INTERFACE
    } ITrendWriteVtbl;

    interface ITrendWrite
    {
        CONST_VTBL struct ITrendWriteVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ITrendWrite_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ITrendWrite_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ITrendWrite_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ITrendWrite_SetEegStorage(This,pIEegStorage)	\
    (This)->lpVtbl -> SetEegStorage(This,pIEegStorage)

#define ITrendWrite_put_bWriteImmediately(This,bWriteImmediatly)	\
    (This)->lpVtbl -> put_bWriteImmediately(This,bWriteImmediatly)

#define ITrendWrite_get_bWriteImmediately(This,pbWriteImmediatly)	\
    (This)->lpVtbl -> get_bWriteImmediately(This,pbWriteImmediatly)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring] */ HRESULT STDMETHODCALLTYPE ITrendWrite_SetEegStorage_Proxy( 
    ITrendWrite * This,
    IEegStorage *pIEegStorage);


void __RPC_STUB ITrendWrite_SetEegStorage_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput] */ HRESULT STDMETHODCALLTYPE ITrendWrite_put_bWriteImmediately_Proxy( 
    ITrendWrite * This,
    /* [in] */ VARIANT_BOOL bWriteImmediatly);


void __RPC_STUB ITrendWrite_put_bWriteImmediately_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE ITrendWrite_get_bWriteImmediately_Proxy( 
    ITrendWrite * This,
    /* [retval][out] */ VARIANT_BOOL *pbWriteImmediatly);


void __RPC_STUB ITrendWrite_get_bWriteImmediately_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ITrendWrite_INTERFACE_DEFINED__ */


#ifndef __ITrendRead_INTERFACE_DEFINED__
#define __ITrendRead_INTERFACE_DEFINED__

/* interface ITrendRead */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_ITrendRead;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("D88A5F33-023B-44ea-8AB1-BF223EF52414")
    ITrendRead : public IUnknown
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE ReadTrendData( void) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE SetEegStorage( 
            IEegStorage *pIEegStorage) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ITrendReadVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ITrendRead * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ITrendRead * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ITrendRead * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *ReadTrendData )( 
            ITrendRead * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetEegStorage )( 
            ITrendRead * This,
            IEegStorage *pIEegStorage);
        
        END_INTERFACE
    } ITrendReadVtbl;

    interface ITrendRead
    {
        CONST_VTBL struct ITrendReadVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ITrendRead_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ITrendRead_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ITrendRead_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ITrendRead_ReadTrendData(This)	\
    (This)->lpVtbl -> ReadTrendData(This)

#define ITrendRead_SetEegStorage(This,pIEegStorage)	\
    (This)->lpVtbl -> SetEegStorage(This,pIEegStorage)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring] */ HRESULT STDMETHODCALLTYPE ITrendRead_ReadTrendData_Proxy( 
    ITrendRead * This);


void __RPC_STUB ITrendRead_ReadTrendData_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ITrendRead_SetEegStorage_Proxy( 
    ITrendRead * This,
    IEegStorage *pIEegStorage);


void __RPC_STUB ITrendRead_SetEegStorage_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ITrendRead_INTERFACE_DEFINED__ */


#ifndef __IImpedanceEvent_INTERFACE_DEFINED__
#define __IImpedanceEvent_INTERFACE_DEFINED__

/* interface IImpedanceEvent */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IImpedanceEvent;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("67156C30-82C8-44df-8F14-946FD105665E")
    IImpedanceEvent : public IUserEvent
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Initialize( 
            /* [in] */ BSTR bstrDeviceID,
            /* [in] */ LONG lcInputs) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetInputCount( 
            /* [retval][out] */ LONG *plcInputs) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE CountActiveInputs( 
            /* [retval][out] */ LONG *plcActiveInputs) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetDeviceID( 
            /* [retval][out] */ BSTR *pbstrDeviceID) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetThreshold( 
            /* [retval][out] */ LONG *plcThreshold) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE SetThreshold( 
            /* [in] */ LONG lThreshold) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_strSensor( 
            /* [in] */ long liIndex,
            /* [retval][out] */ BSTR *pbstrSensor) = 0;
        
        virtual /* [helpstring][propput] */ HRESULT STDMETHODCALLTYPE put_strSensor( 
            /* [in] */ long liIndex,
            /* [in] */ BSTR bstrSensor) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_lInputID( 
            /* [in] */ long liIndex,
            /* [retval][out] */ LONG *plInputID) = 0;
        
        virtual /* [helpstring][propput] */ HRESULT STDMETHODCALLTYPE put_lInputID( 
            /* [in] */ long liIndex,
            /* [in] */ LONG lInputID) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_bCheckActive( 
            /* [in] */ long liIndex,
            /* [retval][out] */ VARIANT_BOOL *pbCheckActive) = 0;
        
        virtual /* [helpstring][propput] */ HRESULT STDMETHODCALLTYPE put_bCheckActive( 
            /* [in] */ long liIndex,
            /* [in] */ VARIANT_BOOL bCheckActive) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_dImpedance( 
            /* [in] */ long liIndex,
            /* [retval][out] */ double *pdImpedance) = 0;
        
        virtual /* [helpstring][propput] */ HRESULT STDMETHODCALLTYPE put_dImpedance( 
            /* [in] */ long liIndex,
            /* [in] */ double pdImpedance) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_lXPos( 
            /* [in] */ long liIndex,
            /* [retval][out] */ LONG *plXpos) = 0;
        
        virtual /* [helpstring][propput] */ HRESULT STDMETHODCALLTYPE put_lXPos( 
            /* [in] */ long liIndex,
            /* [in] */ LONG plXpos) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_lYPos( 
            /* [in] */ long liIndex,
            /* [retval][out] */ LONG *plYpos) = 0;
        
        virtual /* [helpstring][propput] */ HRESULT STDMETHODCALLTYPE put_lYPos( 
            /* [in] */ long liIndex,
            /* [in] */ LONG plYpos) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IImpedanceEventVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IImpedanceEvent * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IImpedanceEvent * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IImpedanceEvent * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IImpedanceEvent * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IImpedanceEvent * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IImpedanceEvent * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IImpedanceEvent * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Create )( 
            IImpedanceEvent * This,
            /* [in] */ BSTR bstrEventType,
            /* [in] */ double dDate,
            /* [in] */ double dFraction);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetEventType )( 
            IImpedanceEvent * This,
            /* [retval][out] */ BSTR *pbstrGUID);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetLabel )( 
            IImpedanceEvent * This,
            /* [in] */ BSTR bstrLabel);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetLabel )( 
            IImpedanceEvent * This,
            /* [retval][out] */ BSTR *pbstrLabel);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetText )( 
            IImpedanceEvent * This,
            /* [in] */ BSTR bstrText);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetText )( 
            IImpedanceEvent * This,
            /* [retval][out] */ BSTR *pbstrText);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetTimeStamp )( 
            IImpedanceEvent * This,
            /* [in] */ double dDate,
            /* [in] */ double dFraction);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetTimeStamp )( 
            IImpedanceEvent * This,
            /* [out] */ double *pdDate,
            /* [out] */ double *pdFraction);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetDuration )( 
            IImpedanceEvent * This,
            /* [in] */ double dDuration);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetDuration )( 
            IImpedanceEvent * This,
            /* [retval][out] */ double *pdDuration);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Clone )( 
            IImpedanceEvent * This,
            /* [in] */ IUserEvent *pIUserEvent);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetUniqueGUID )( 
            IImpedanceEvent * This,
            /* [retval][out] */ BSTR *bstrGUID);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetUser )( 
            IImpedanceEvent * This,
            /* [retval][out] */ BSTR *bstrUser);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetCreationDate )( 
            IImpedanceEvent * This,
            /* [retval][out] */ double *pdCreationDate);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetSystemDetected )( 
            IImpedanceEvent * This,
            /* [retval][out] */ VARIANT_BOOL *pbSystem);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *Initialize )( 
            IImpedanceEvent * This,
            /* [in] */ BSTR bstrDeviceID,
            /* [in] */ LONG lcInputs);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetInputCount )( 
            IImpedanceEvent * This,
            /* [retval][out] */ LONG *plcInputs);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *CountActiveInputs )( 
            IImpedanceEvent * This,
            /* [retval][out] */ LONG *plcActiveInputs);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetDeviceID )( 
            IImpedanceEvent * This,
            /* [retval][out] */ BSTR *pbstrDeviceID);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetThreshold )( 
            IImpedanceEvent * This,
            /* [retval][out] */ LONG *plcThreshold);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetThreshold )( 
            IImpedanceEvent * This,
            /* [in] */ LONG lThreshold);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_strSensor )( 
            IImpedanceEvent * This,
            /* [in] */ long liIndex,
            /* [retval][out] */ BSTR *pbstrSensor);
        
        /* [helpstring][propput] */ HRESULT ( STDMETHODCALLTYPE *put_strSensor )( 
            IImpedanceEvent * This,
            /* [in] */ long liIndex,
            /* [in] */ BSTR bstrSensor);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_lInputID )( 
            IImpedanceEvent * This,
            /* [in] */ long liIndex,
            /* [retval][out] */ LONG *plInputID);
        
        /* [helpstring][propput] */ HRESULT ( STDMETHODCALLTYPE *put_lInputID )( 
            IImpedanceEvent * This,
            /* [in] */ long liIndex,
            /* [in] */ LONG lInputID);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_bCheckActive )( 
            IImpedanceEvent * This,
            /* [in] */ long liIndex,
            /* [retval][out] */ VARIANT_BOOL *pbCheckActive);
        
        /* [helpstring][propput] */ HRESULT ( STDMETHODCALLTYPE *put_bCheckActive )( 
            IImpedanceEvent * This,
            /* [in] */ long liIndex,
            /* [in] */ VARIANT_BOOL bCheckActive);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_dImpedance )( 
            IImpedanceEvent * This,
            /* [in] */ long liIndex,
            /* [retval][out] */ double *pdImpedance);
        
        /* [helpstring][propput] */ HRESULT ( STDMETHODCALLTYPE *put_dImpedance )( 
            IImpedanceEvent * This,
            /* [in] */ long liIndex,
            /* [in] */ double pdImpedance);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_lXPos )( 
            IImpedanceEvent * This,
            /* [in] */ long liIndex,
            /* [retval][out] */ LONG *plXpos);
        
        /* [helpstring][propput] */ HRESULT ( STDMETHODCALLTYPE *put_lXPos )( 
            IImpedanceEvent * This,
            /* [in] */ long liIndex,
            /* [in] */ LONG plXpos);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_lYPos )( 
            IImpedanceEvent * This,
            /* [in] */ long liIndex,
            /* [retval][out] */ LONG *plYpos);
        
        /* [helpstring][propput] */ HRESULT ( STDMETHODCALLTYPE *put_lYPos )( 
            IImpedanceEvent * This,
            /* [in] */ long liIndex,
            /* [in] */ LONG plYpos);
        
        END_INTERFACE
    } IImpedanceEventVtbl;

    interface IImpedanceEvent
    {
        CONST_VTBL struct IImpedanceEventVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IImpedanceEvent_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IImpedanceEvent_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IImpedanceEvent_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IImpedanceEvent_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IImpedanceEvent_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IImpedanceEvent_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IImpedanceEvent_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IImpedanceEvent_Create(This,bstrEventType,dDate,dFraction)	\
    (This)->lpVtbl -> Create(This,bstrEventType,dDate,dFraction)

#define IImpedanceEvent_GetEventType(This,pbstrGUID)	\
    (This)->lpVtbl -> GetEventType(This,pbstrGUID)

#define IImpedanceEvent_SetLabel(This,bstrLabel)	\
    (This)->lpVtbl -> SetLabel(This,bstrLabel)

#define IImpedanceEvent_GetLabel(This,pbstrLabel)	\
    (This)->lpVtbl -> GetLabel(This,pbstrLabel)

#define IImpedanceEvent_SetText(This,bstrText)	\
    (This)->lpVtbl -> SetText(This,bstrText)

#define IImpedanceEvent_GetText(This,pbstrText)	\
    (This)->lpVtbl -> GetText(This,pbstrText)

#define IImpedanceEvent_SetTimeStamp(This,dDate,dFraction)	\
    (This)->lpVtbl -> SetTimeStamp(This,dDate,dFraction)

#define IImpedanceEvent_GetTimeStamp(This,pdDate,pdFraction)	\
    (This)->lpVtbl -> GetTimeStamp(This,pdDate,pdFraction)

#define IImpedanceEvent_SetDuration(This,dDuration)	\
    (This)->lpVtbl -> SetDuration(This,dDuration)

#define IImpedanceEvent_GetDuration(This,pdDuration)	\
    (This)->lpVtbl -> GetDuration(This,pdDuration)

#define IImpedanceEvent_Clone(This,pIUserEvent)	\
    (This)->lpVtbl -> Clone(This,pIUserEvent)

#define IImpedanceEvent_GetUniqueGUID(This,bstrGUID)	\
    (This)->lpVtbl -> GetUniqueGUID(This,bstrGUID)

#define IImpedanceEvent_GetUser(This,bstrUser)	\
    (This)->lpVtbl -> GetUser(This,bstrUser)

#define IImpedanceEvent_GetCreationDate(This,pdCreationDate)	\
    (This)->lpVtbl -> GetCreationDate(This,pdCreationDate)

#define IImpedanceEvent_GetSystemDetected(This,pbSystem)	\
    (This)->lpVtbl -> GetSystemDetected(This,pbSystem)


#define IImpedanceEvent_Initialize(This,bstrDeviceID,lcInputs)	\
    (This)->lpVtbl -> Initialize(This,bstrDeviceID,lcInputs)

#define IImpedanceEvent_GetInputCount(This,plcInputs)	\
    (This)->lpVtbl -> GetInputCount(This,plcInputs)

#define IImpedanceEvent_CountActiveInputs(This,plcActiveInputs)	\
    (This)->lpVtbl -> CountActiveInputs(This,plcActiveInputs)

#define IImpedanceEvent_GetDeviceID(This,pbstrDeviceID)	\
    (This)->lpVtbl -> GetDeviceID(This,pbstrDeviceID)

#define IImpedanceEvent_GetThreshold(This,plcThreshold)	\
    (This)->lpVtbl -> GetThreshold(This,plcThreshold)

#define IImpedanceEvent_SetThreshold(This,lThreshold)	\
    (This)->lpVtbl -> SetThreshold(This,lThreshold)

#define IImpedanceEvent_get_strSensor(This,liIndex,pbstrSensor)	\
    (This)->lpVtbl -> get_strSensor(This,liIndex,pbstrSensor)

#define IImpedanceEvent_put_strSensor(This,liIndex,bstrSensor)	\
    (This)->lpVtbl -> put_strSensor(This,liIndex,bstrSensor)

#define IImpedanceEvent_get_lInputID(This,liIndex,plInputID)	\
    (This)->lpVtbl -> get_lInputID(This,liIndex,plInputID)

#define IImpedanceEvent_put_lInputID(This,liIndex,lInputID)	\
    (This)->lpVtbl -> put_lInputID(This,liIndex,lInputID)

#define IImpedanceEvent_get_bCheckActive(This,liIndex,pbCheckActive)	\
    (This)->lpVtbl -> get_bCheckActive(This,liIndex,pbCheckActive)

#define IImpedanceEvent_put_bCheckActive(This,liIndex,bCheckActive)	\
    (This)->lpVtbl -> put_bCheckActive(This,liIndex,bCheckActive)

#define IImpedanceEvent_get_dImpedance(This,liIndex,pdImpedance)	\
    (This)->lpVtbl -> get_dImpedance(This,liIndex,pdImpedance)

#define IImpedanceEvent_put_dImpedance(This,liIndex,pdImpedance)	\
    (This)->lpVtbl -> put_dImpedance(This,liIndex,pdImpedance)

#define IImpedanceEvent_get_lXPos(This,liIndex,plXpos)	\
    (This)->lpVtbl -> get_lXPos(This,liIndex,plXpos)

#define IImpedanceEvent_put_lXPos(This,liIndex,plXpos)	\
    (This)->lpVtbl -> put_lXPos(This,liIndex,plXpos)

#define IImpedanceEvent_get_lYPos(This,liIndex,plYpos)	\
    (This)->lpVtbl -> get_lYPos(This,liIndex,plYpos)

#define IImpedanceEvent_put_lYPos(This,liIndex,plYpos)	\
    (This)->lpVtbl -> put_lYPos(This,liIndex,plYpos)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring] */ HRESULT STDMETHODCALLTYPE IImpedanceEvent_Initialize_Proxy( 
    IImpedanceEvent * This,
    /* [in] */ BSTR bstrDeviceID,
    /* [in] */ LONG lcInputs);


void __RPC_STUB IImpedanceEvent_Initialize_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IImpedanceEvent_GetInputCount_Proxy( 
    IImpedanceEvent * This,
    /* [retval][out] */ LONG *plcInputs);


void __RPC_STUB IImpedanceEvent_GetInputCount_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IImpedanceEvent_CountActiveInputs_Proxy( 
    IImpedanceEvent * This,
    /* [retval][out] */ LONG *plcActiveInputs);


void __RPC_STUB IImpedanceEvent_CountActiveInputs_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IImpedanceEvent_GetDeviceID_Proxy( 
    IImpedanceEvent * This,
    /* [retval][out] */ BSTR *pbstrDeviceID);


void __RPC_STUB IImpedanceEvent_GetDeviceID_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IImpedanceEvent_GetThreshold_Proxy( 
    IImpedanceEvent * This,
    /* [retval][out] */ LONG *plcThreshold);


void __RPC_STUB IImpedanceEvent_GetThreshold_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IImpedanceEvent_SetThreshold_Proxy( 
    IImpedanceEvent * This,
    /* [in] */ LONG lThreshold);


void __RPC_STUB IImpedanceEvent_SetThreshold_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE IImpedanceEvent_get_strSensor_Proxy( 
    IImpedanceEvent * This,
    /* [in] */ long liIndex,
    /* [retval][out] */ BSTR *pbstrSensor);


void __RPC_STUB IImpedanceEvent_get_strSensor_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput] */ HRESULT STDMETHODCALLTYPE IImpedanceEvent_put_strSensor_Proxy( 
    IImpedanceEvent * This,
    /* [in] */ long liIndex,
    /* [in] */ BSTR bstrSensor);


void __RPC_STUB IImpedanceEvent_put_strSensor_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE IImpedanceEvent_get_lInputID_Proxy( 
    IImpedanceEvent * This,
    /* [in] */ long liIndex,
    /* [retval][out] */ LONG *plInputID);


void __RPC_STUB IImpedanceEvent_get_lInputID_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput] */ HRESULT STDMETHODCALLTYPE IImpedanceEvent_put_lInputID_Proxy( 
    IImpedanceEvent * This,
    /* [in] */ long liIndex,
    /* [in] */ LONG lInputID);


void __RPC_STUB IImpedanceEvent_put_lInputID_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE IImpedanceEvent_get_bCheckActive_Proxy( 
    IImpedanceEvent * This,
    /* [in] */ long liIndex,
    /* [retval][out] */ VARIANT_BOOL *pbCheckActive);


void __RPC_STUB IImpedanceEvent_get_bCheckActive_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput] */ HRESULT STDMETHODCALLTYPE IImpedanceEvent_put_bCheckActive_Proxy( 
    IImpedanceEvent * This,
    /* [in] */ long liIndex,
    /* [in] */ VARIANT_BOOL bCheckActive);


void __RPC_STUB IImpedanceEvent_put_bCheckActive_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE IImpedanceEvent_get_dImpedance_Proxy( 
    IImpedanceEvent * This,
    /* [in] */ long liIndex,
    /* [retval][out] */ double *pdImpedance);


void __RPC_STUB IImpedanceEvent_get_dImpedance_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput] */ HRESULT STDMETHODCALLTYPE IImpedanceEvent_put_dImpedance_Proxy( 
    IImpedanceEvent * This,
    /* [in] */ long liIndex,
    /* [in] */ double pdImpedance);


void __RPC_STUB IImpedanceEvent_put_dImpedance_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE IImpedanceEvent_get_lXPos_Proxy( 
    IImpedanceEvent * This,
    /* [in] */ long liIndex,
    /* [retval][out] */ LONG *plXpos);


void __RPC_STUB IImpedanceEvent_get_lXPos_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput] */ HRESULT STDMETHODCALLTYPE IImpedanceEvent_put_lXPos_Proxy( 
    IImpedanceEvent * This,
    /* [in] */ long liIndex,
    /* [in] */ LONG plXpos);


void __RPC_STUB IImpedanceEvent_put_lXPos_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE IImpedanceEvent_get_lYPos_Proxy( 
    IImpedanceEvent * This,
    /* [in] */ long liIndex,
    /* [retval][out] */ LONG *plYpos);


void __RPC_STUB IImpedanceEvent_get_lYPos_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput] */ HRESULT STDMETHODCALLTYPE IImpedanceEvent_put_lYPos_Proxy( 
    IImpedanceEvent * This,
    /* [in] */ long liIndex,
    /* [in] */ LONG plYpos);


void __RPC_STUB IImpedanceEvent_put_lYPos_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IImpedanceEvent_INTERFACE_DEFINED__ */


#ifndef __IEventWrapperProxy_INTERFACE_DEFINED__
#define __IEventWrapperProxy_INTERFACE_DEFINED__

/* interface IEventWrapperProxy */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IEventWrapperProxy;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("0B96E985-0462-404f-BC3F-CED34A6D2636")
    IEventWrapperProxy : public IEventWrapper
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Initialize( 
            /* [in] */ IEventWrapper *pIEventWrapper) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Uninitialize( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IEventWrapperProxyVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IEventWrapperProxy * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IEventWrapperProxy * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IEventWrapperProxy * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *Clone )( 
            IEventWrapperProxy * This,
            /* [in] */ IEventWrapper *pIEventWrapper);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *DeleteEvent )( 
            IEventWrapperProxy * This,
            /* [in] */ BYTE *pPacket);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *ModifyEvent )( 
            IEventWrapperProxy * This,
            /* [in] */ BYTE *pPacket);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *InsertEvent )( 
            IEventWrapperProxy * This,
            /* [in] */ BYTE *pPacket);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetUserEventTypes )( 
            IEventWrapperProxy * This,
            /* [in] */ BYTE *pPacket);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetEvent )( 
            IEventWrapperProxy * This,
            /* [in] */ BYTE *pPacket);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetUserEventTypes )( 
            IEventWrapperProxy * This,
            /* [out] */ BYTE **pPacket);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetEvent )( 
            IEventWrapperProxy * This,
            /* [in] */ long lPos,
            /* [out] */ BYTE **ppPacket);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetEventFromGUID )( 
            IEventWrapperProxy * This,
            /* [in] */ GUID guid,
            /* [out] */ BYTE **ppPacket);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *QueryCount )( 
            IEventWrapperProxy * This,
            /* [retval][out] */ long *plcEvents);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *CopyEvent )( 
            IEventWrapperProxy * This,
            /* [in] */ BYTE *pPacket);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_bModified )( 
            IEventWrapperProxy * This,
            /* [retval][out] */ BOOL *pVal);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *AddEventNotify )( 
            IEventWrapperProxy * This,
            /* [in] */ IEventNotify *pIEventNotify);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *RemoveEventNotify )( 
            IEventWrapperProxy * This,
            /* [in] */ IEventNotify *pIEventNotify);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *Save )( 
            IEventWrapperProxy * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *PruneEvents )( 
            IEventWrapperProxy * This,
            /* [in] */ INrvSegments3 *pINrvSegments3);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetFilter )( 
            IEventWrapperProxy * This,
            /* [in] */ IEventFilter *pIEventFilter);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *CanModify )( 
            IEventWrapperProxy * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_bFiltered )( 
            IEventWrapperProxy * This,
            /* [retval][out] */ BOOL *pVal);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetReadOnly )( 
            IEventWrapperProxy * This,
            /* [in] */ BOOL bReadOnly);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetEvent2 )( 
            IEventWrapperProxy * This,
            /* [in] */ long lPos,
            /* [out] */ long *plSize,
            /* [size_is][size_is][out] */ BYTE **ppPacket);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *InsertEvent2 )( 
            IEventWrapperProxy * This,
            /* [in] */ long lSize,
            /* [size_is][in] */ BYTE *pPacket);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetUserEventTypes2 )( 
            IEventWrapperProxy * This,
            /* [in] */ long lSize,
            /* [size_is][in] */ BYTE *pPacket);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetUserEventTypes2 )( 
            IEventWrapperProxy * This,
            /* [out] */ long *plSize,
            /* [size_is][size_is][out] */ BYTE **pPacket);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *Initialize )( 
            IEventWrapperProxy * This,
            /* [in] */ IEventWrapper *pIEventWrapper);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *Uninitialize )( 
            IEventWrapperProxy * This);
        
        END_INTERFACE
    } IEventWrapperProxyVtbl;

    interface IEventWrapperProxy
    {
        CONST_VTBL struct IEventWrapperProxyVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IEventWrapperProxy_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IEventWrapperProxy_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IEventWrapperProxy_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IEventWrapperProxy_Clone(This,pIEventWrapper)	\
    (This)->lpVtbl -> Clone(This,pIEventWrapper)

#define IEventWrapperProxy_DeleteEvent(This,pPacket)	\
    (This)->lpVtbl -> DeleteEvent(This,pPacket)

#define IEventWrapperProxy_ModifyEvent(This,pPacket)	\
    (This)->lpVtbl -> ModifyEvent(This,pPacket)

#define IEventWrapperProxy_InsertEvent(This,pPacket)	\
    (This)->lpVtbl -> InsertEvent(This,pPacket)

#define IEventWrapperProxy_SetUserEventTypes(This,pPacket)	\
    (This)->lpVtbl -> SetUserEventTypes(This,pPacket)

#define IEventWrapperProxy_SetEvent(This,pPacket)	\
    (This)->lpVtbl -> SetEvent(This,pPacket)

#define IEventWrapperProxy_GetUserEventTypes(This,pPacket)	\
    (This)->lpVtbl -> GetUserEventTypes(This,pPacket)

#define IEventWrapperProxy_GetEvent(This,lPos,ppPacket)	\
    (This)->lpVtbl -> GetEvent(This,lPos,ppPacket)

#define IEventWrapperProxy_GetEventFromGUID(This,guid,ppPacket)	\
    (This)->lpVtbl -> GetEventFromGUID(This,guid,ppPacket)

#define IEventWrapperProxy_QueryCount(This,plcEvents)	\
    (This)->lpVtbl -> QueryCount(This,plcEvents)

#define IEventWrapperProxy_CopyEvent(This,pPacket)	\
    (This)->lpVtbl -> CopyEvent(This,pPacket)

#define IEventWrapperProxy_get_bModified(This,pVal)	\
    (This)->lpVtbl -> get_bModified(This,pVal)

#define IEventWrapperProxy_AddEventNotify(This,pIEventNotify)	\
    (This)->lpVtbl -> AddEventNotify(This,pIEventNotify)

#define IEventWrapperProxy_RemoveEventNotify(This,pIEventNotify)	\
    (This)->lpVtbl -> RemoveEventNotify(This,pIEventNotify)

#define IEventWrapperProxy_Save(This)	\
    (This)->lpVtbl -> Save(This)

#define IEventWrapperProxy_PruneEvents(This,pINrvSegments3)	\
    (This)->lpVtbl -> PruneEvents(This,pINrvSegments3)

#define IEventWrapperProxy_SetFilter(This,pIEventFilter)	\
    (This)->lpVtbl -> SetFilter(This,pIEventFilter)

#define IEventWrapperProxy_CanModify(This,pVal)	\
    (This)->lpVtbl -> CanModify(This,pVal)

#define IEventWrapperProxy_get_bFiltered(This,pVal)	\
    (This)->lpVtbl -> get_bFiltered(This,pVal)

#define IEventWrapperProxy_SetReadOnly(This,bReadOnly)	\
    (This)->lpVtbl -> SetReadOnly(This,bReadOnly)

#define IEventWrapperProxy_GetEvent2(This,lPos,plSize,ppPacket)	\
    (This)->lpVtbl -> GetEvent2(This,lPos,plSize,ppPacket)

#define IEventWrapperProxy_InsertEvent2(This,lSize,pPacket)	\
    (This)->lpVtbl -> InsertEvent2(This,lSize,pPacket)

#define IEventWrapperProxy_SetUserEventTypes2(This,lSize,pPacket)	\
    (This)->lpVtbl -> SetUserEventTypes2(This,lSize,pPacket)

#define IEventWrapperProxy_GetUserEventTypes2(This,plSize,pPacket)	\
    (This)->lpVtbl -> GetUserEventTypes2(This,plSize,pPacket)


#define IEventWrapperProxy_Initialize(This,pIEventWrapper)	\
    (This)->lpVtbl -> Initialize(This,pIEventWrapper)

#define IEventWrapperProxy_Uninitialize(This)	\
    (This)->lpVtbl -> Uninitialize(This)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring] */ HRESULT STDMETHODCALLTYPE IEventWrapperProxy_Initialize_Proxy( 
    IEventWrapperProxy * This,
    /* [in] */ IEventWrapper *pIEventWrapper);


void __RPC_STUB IEventWrapperProxy_Initialize_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IEventWrapperProxy_Uninitialize_Proxy( 
    IEventWrapperProxy * This);


void __RPC_STUB IEventWrapperProxy_Uninitialize_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IEventWrapperProxy_INTERFACE_DEFINED__ */


#ifndef __ISegments_INTERFACE_DEFINED__
#define __ISegments_INTERFACE_DEFINED__

/* interface ISegments */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_ISegments;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("F8032A01-9352-47fa-B2DE-9BE97B3B4414")
    ISegments : public IEegSegments
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE AddSegment( 
            /* [in] */ double dDateStart,
            /* [in] */ double dFractionStart,
            /* [in] */ double dDateEnd,
            /* [in] */ double dFractionEnd,
            /* [retval][out] */ long *pliSegment) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE RemoveSegment( 
            /* [in] */ long liSegment) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Clear( void) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE SegmentsFromPruneTemplate( 
            /* [in] */ IDispatch *pIPruneTemplate,
            /* [in] */ IUserEvents *pIUserEvents,
            /* [in] */ IEegSegments *pIEegSegments) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE FromNrvSegments( 
            /* [in] */ INrvSegments3 *pNrvSegments) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Clone( 
            /* [retval][out] */ ISegments **pISegments) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Copy( 
            /* [in] */ IEegSegments *pISegments) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Normalize( 
            /* [in] */ IEegSegments *pSegmentsFromFile) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ISegmentsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ISegments * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ISegments * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ISegments * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ISegments * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ISegments * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ISegments * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ISegments * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_lcSegments )( 
            ISegments * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *QuerySegmentStartTime )( 
            ISegments * This,
            /* [in] */ double dDate,
            /* [in] */ double dFraction,
            /* [out] */ double *pdDate,
            /* [out] */ double *pdFraction,
            /* [retval][out] */ long *pliSegment);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *QuerySegmentEndTime )( 
            ISegments * This,
            /* [in] */ double dDate,
            /* [in] */ double dFraction,
            /* [out] */ double *pdDate,
            /* [out] */ double *pdFraction,
            /* [retval][out] */ long *pliSegment);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *QuerySegmentStartTime2 )( 
            ISegments * This,
            /* [in] */ long liSegment,
            /* [out] */ double *pdDate,
            /* [out] */ double *pdFraction);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *QuerySegmentEndTime2 )( 
            ISegments * This,
            /* [in] */ long liSegment,
            /* [out] */ double *pdDate,
            /* [out] */ double *pdFraction);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *QuerySegmentDuration )( 
            ISegments * This,
            /* [in] */ long liSegment,
            /* [retval][out] */ double *pdDuration);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *QueryLastSegment )( 
            ISegments * This,
            /* [in] */ double dDate,
            /* [in] */ double dFraction,
            /* [out] */ long *pliSegment,
            /* [retval][out] */ VARIANT_BOOL *pbInSegment);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *QueryStartTime )( 
            ISegments * This,
            /* [out] */ double *pdDate,
            /* [out] */ double *pdFraction);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *QueryEndTime )( 
            ISegments * This,
            /* [out] */ double *pdDate,
            /* [out] */ double *pdFraction);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *QueryTotalRecSecs )( 
            ISegments * This,
            /* [retval][out] */ double *pVal);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *NrvDate2RecSec )( 
            ISegments * This,
            /* [in] */ double dDate,
            /* [in] */ double dFraction,
            /* [retval][out] */ double *pdRecSec);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *RecSec2NrvDate )( 
            ISegments * This,
            /* [in] */ double dRecSec,
            /* [out] */ double *pdDate,
            /* [out] */ double *pdFraction);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *AddSegment )( 
            ISegments * This,
            /* [in] */ double dDateStart,
            /* [in] */ double dFractionStart,
            /* [in] */ double dDateEnd,
            /* [in] */ double dFractionEnd,
            /* [retval][out] */ long *pliSegment);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *RemoveSegment )( 
            ISegments * This,
            /* [in] */ long liSegment);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *Clear )( 
            ISegments * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *SegmentsFromPruneTemplate )( 
            ISegments * This,
            /* [in] */ IDispatch *pIPruneTemplate,
            /* [in] */ IUserEvents *pIUserEvents,
            /* [in] */ IEegSegments *pIEegSegments);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *FromNrvSegments )( 
            ISegments * This,
            /* [in] */ INrvSegments3 *pNrvSegments);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *Clone )( 
            ISegments * This,
            /* [retval][out] */ ISegments **pISegments);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *Copy )( 
            ISegments * This,
            /* [in] */ IEegSegments *pISegments);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *Normalize )( 
            ISegments * This,
            /* [in] */ IEegSegments *pSegmentsFromFile);
        
        END_INTERFACE
    } ISegmentsVtbl;

    interface ISegments
    {
        CONST_VTBL struct ISegmentsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ISegments_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ISegments_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ISegments_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ISegments_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define ISegments_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define ISegments_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define ISegments_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define ISegments_get_lcSegments(This,pVal)	\
    (This)->lpVtbl -> get_lcSegments(This,pVal)

#define ISegments_QuerySegmentStartTime(This,dDate,dFraction,pdDate,pdFraction,pliSegment)	\
    (This)->lpVtbl -> QuerySegmentStartTime(This,dDate,dFraction,pdDate,pdFraction,pliSegment)

#define ISegments_QuerySegmentEndTime(This,dDate,dFraction,pdDate,pdFraction,pliSegment)	\
    (This)->lpVtbl -> QuerySegmentEndTime(This,dDate,dFraction,pdDate,pdFraction,pliSegment)

#define ISegments_QuerySegmentStartTime2(This,liSegment,pdDate,pdFraction)	\
    (This)->lpVtbl -> QuerySegmentStartTime2(This,liSegment,pdDate,pdFraction)

#define ISegments_QuerySegmentEndTime2(This,liSegment,pdDate,pdFraction)	\
    (This)->lpVtbl -> QuerySegmentEndTime2(This,liSegment,pdDate,pdFraction)

#define ISegments_QuerySegmentDuration(This,liSegment,pdDuration)	\
    (This)->lpVtbl -> QuerySegmentDuration(This,liSegment,pdDuration)

#define ISegments_QueryLastSegment(This,dDate,dFraction,pliSegment,pbInSegment)	\
    (This)->lpVtbl -> QueryLastSegment(This,dDate,dFraction,pliSegment,pbInSegment)

#define ISegments_QueryStartTime(This,pdDate,pdFraction)	\
    (This)->lpVtbl -> QueryStartTime(This,pdDate,pdFraction)

#define ISegments_QueryEndTime(This,pdDate,pdFraction)	\
    (This)->lpVtbl -> QueryEndTime(This,pdDate,pdFraction)

#define ISegments_QueryTotalRecSecs(This,pVal)	\
    (This)->lpVtbl -> QueryTotalRecSecs(This,pVal)

#define ISegments_NrvDate2RecSec(This,dDate,dFraction,pdRecSec)	\
    (This)->lpVtbl -> NrvDate2RecSec(This,dDate,dFraction,pdRecSec)

#define ISegments_RecSec2NrvDate(This,dRecSec,pdDate,pdFraction)	\
    (This)->lpVtbl -> RecSec2NrvDate(This,dRecSec,pdDate,pdFraction)


#define ISegments_AddSegment(This,dDateStart,dFractionStart,dDateEnd,dFractionEnd,pliSegment)	\
    (This)->lpVtbl -> AddSegment(This,dDateStart,dFractionStart,dDateEnd,dFractionEnd,pliSegment)

#define ISegments_RemoveSegment(This,liSegment)	\
    (This)->lpVtbl -> RemoveSegment(This,liSegment)

#define ISegments_Clear(This)	\
    (This)->lpVtbl -> Clear(This)

#define ISegments_SegmentsFromPruneTemplate(This,pIPruneTemplate,pIUserEvents,pIEegSegments)	\
    (This)->lpVtbl -> SegmentsFromPruneTemplate(This,pIPruneTemplate,pIUserEvents,pIEegSegments)

#define ISegments_FromNrvSegments(This,pNrvSegments)	\
    (This)->lpVtbl -> FromNrvSegments(This,pNrvSegments)

#define ISegments_Clone(This,pISegments)	\
    (This)->lpVtbl -> Clone(This,pISegments)

#define ISegments_Copy(This,pISegments)	\
    (This)->lpVtbl -> Copy(This,pISegments)

#define ISegments_Normalize(This,pSegmentsFromFile)	\
    (This)->lpVtbl -> Normalize(This,pSegmentsFromFile)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISegments_AddSegment_Proxy( 
    ISegments * This,
    /* [in] */ double dDateStart,
    /* [in] */ double dFractionStart,
    /* [in] */ double dDateEnd,
    /* [in] */ double dFractionEnd,
    /* [retval][out] */ long *pliSegment);


void __RPC_STUB ISegments_AddSegment_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISegments_RemoveSegment_Proxy( 
    ISegments * This,
    /* [in] */ long liSegment);


void __RPC_STUB ISegments_RemoveSegment_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISegments_Clear_Proxy( 
    ISegments * This);


void __RPC_STUB ISegments_Clear_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISegments_SegmentsFromPruneTemplate_Proxy( 
    ISegments * This,
    /* [in] */ IDispatch *pIPruneTemplate,
    /* [in] */ IUserEvents *pIUserEvents,
    /* [in] */ IEegSegments *pIEegSegments);


void __RPC_STUB ISegments_SegmentsFromPruneTemplate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISegments_FromNrvSegments_Proxy( 
    ISegments * This,
    /* [in] */ INrvSegments3 *pNrvSegments);


void __RPC_STUB ISegments_FromNrvSegments_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISegments_Clone_Proxy( 
    ISegments * This,
    /* [retval][out] */ ISegments **pISegments);


void __RPC_STUB ISegments_Clone_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISegments_Copy_Proxy( 
    ISegments * This,
    /* [in] */ IEegSegments *pISegments);


void __RPC_STUB ISegments_Copy_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISegments_Normalize_Proxy( 
    ISegments * This,
    /* [in] */ IEegSegments *pSegmentsFromFile);


void __RPC_STUB ISegments_Normalize_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ISegments_INTERFACE_DEFINED__ */


#ifndef __IStorageMan_INTERFACE_DEFINED__
#define __IStorageMan_INTERFACE_DEFINED__

/* interface IStorageMan */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IStorageMan;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("18F60508-0347-4884-9D0C-F1307B8C029E")
    IStorageMan : public IEegStorage
    {
    public:
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_bstrFileName( 
            /* [retval][out] */ BSTR *pbstrFileName) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_bModified( 
            /* [retval][out] */ VARIANT_BOOL *pbModified) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_bOnLine( 
            /* [retval][out] */ VARIANT_BOOL *pbOnLine) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_bstrFileFormatCLSID( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_bHasVideo( 
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_bstrVideoPath( 
            /* [retval][out] */ BSTR *pbstrPath) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_lPruneType( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE AddObserver( 
            /* [in] */ IStorageObserver *pIObserver) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE RemoveObserver( 
            /* [in] */ IStorageObserver *pIObserver) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IStorageManVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IStorageMan * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IStorageMan * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IStorageMan * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IStorageMan * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IStorageMan * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IStorageMan * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IStorageMan * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_bReadOnly )( 
            IStorageMan * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_bOpen )( 
            IStorageMan * This,
            /* [retval][out] */ VARIANT_BOOL *pbOpen);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetEegSegments )( 
            IStorageMan * This,
            /* [retval][out] */ IEegSegments **ppIEegSegments);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetVideoSegments )( 
            IStorageMan * This,
            /* [retval][out] */ IEegSegments **ppIEegSegments);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetEegData )( 
            IStorageMan * This,
            /* [retval][out] */ IEegData **ppIEegData);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetEvents )( 
            IStorageMan * This,
            /* [retval][out] */ IUserEvents **ppIUserEvents);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetEventWrapper )( 
            IStorageMan * This,
            /* [retval][out] */ IEventWrapper **ppIEventWrapper);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetSleepData )( 
            IStorageMan * This,
            /* [out] */ ISleepData **ppISleepData);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_bstrFileName )( 
            IStorageMan * This,
            /* [retval][out] */ BSTR *pbstrFileName);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_bModified )( 
            IStorageMan * This,
            /* [retval][out] */ VARIANT_BOOL *pbModified);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_bOnLine )( 
            IStorageMan * This,
            /* [retval][out] */ VARIANT_BOOL *pbOnLine);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_bstrFileFormatCLSID )( 
            IStorageMan * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_bHasVideo )( 
            IStorageMan * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_bstrVideoPath )( 
            IStorageMan * This,
            /* [retval][out] */ BSTR *pbstrPath);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_lPruneType )( 
            IStorageMan * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *AddObserver )( 
            IStorageMan * This,
            /* [in] */ IStorageObserver *pIObserver);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *RemoveObserver )( 
            IStorageMan * This,
            /* [in] */ IStorageObserver *pIObserver);
        
        END_INTERFACE
    } IStorageManVtbl;

    interface IStorageMan
    {
        CONST_VTBL struct IStorageManVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IStorageMan_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IStorageMan_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IStorageMan_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IStorageMan_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IStorageMan_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IStorageMan_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IStorageMan_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IStorageMan_get_bReadOnly(This,pVal)	\
    (This)->lpVtbl -> get_bReadOnly(This,pVal)

#define IStorageMan_get_bOpen(This,pbOpen)	\
    (This)->lpVtbl -> get_bOpen(This,pbOpen)

#define IStorageMan_GetEegSegments(This,ppIEegSegments)	\
    (This)->lpVtbl -> GetEegSegments(This,ppIEegSegments)

#define IStorageMan_GetVideoSegments(This,ppIEegSegments)	\
    (This)->lpVtbl -> GetVideoSegments(This,ppIEegSegments)

#define IStorageMan_GetEegData(This,ppIEegData)	\
    (This)->lpVtbl -> GetEegData(This,ppIEegData)

#define IStorageMan_GetEvents(This,ppIUserEvents)	\
    (This)->lpVtbl -> GetEvents(This,ppIUserEvents)

#define IStorageMan_GetEventWrapper(This,ppIEventWrapper)	\
    (This)->lpVtbl -> GetEventWrapper(This,ppIEventWrapper)

#define IStorageMan_GetSleepData(This,ppISleepData)	\
    (This)->lpVtbl -> GetSleepData(This,ppISleepData)


#define IStorageMan_get_bstrFileName(This,pbstrFileName)	\
    (This)->lpVtbl -> get_bstrFileName(This,pbstrFileName)

#define IStorageMan_get_bModified(This,pbModified)	\
    (This)->lpVtbl -> get_bModified(This,pbModified)

#define IStorageMan_get_bOnLine(This,pbOnLine)	\
    (This)->lpVtbl -> get_bOnLine(This,pbOnLine)

#define IStorageMan_get_bstrFileFormatCLSID(This,pVal)	\
    (This)->lpVtbl -> get_bstrFileFormatCLSID(This,pVal)

#define IStorageMan_get_bHasVideo(This,pVal)	\
    (This)->lpVtbl -> get_bHasVideo(This,pVal)

#define IStorageMan_get_bstrVideoPath(This,pbstrPath)	\
    (This)->lpVtbl -> get_bstrVideoPath(This,pbstrPath)

#define IStorageMan_get_lPruneType(This,pVal)	\
    (This)->lpVtbl -> get_lPruneType(This,pVal)

#define IStorageMan_AddObserver(This,pIObserver)	\
    (This)->lpVtbl -> AddObserver(This,pIObserver)

#define IStorageMan_RemoveObserver(This,pIObserver)	\
    (This)->lpVtbl -> RemoveObserver(This,pIObserver)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE IStorageMan_get_bstrFileName_Proxy( 
    IStorageMan * This,
    /* [retval][out] */ BSTR *pbstrFileName);


void __RPC_STUB IStorageMan_get_bstrFileName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE IStorageMan_get_bModified_Proxy( 
    IStorageMan * This,
    /* [retval][out] */ VARIANT_BOOL *pbModified);


void __RPC_STUB IStorageMan_get_bModified_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE IStorageMan_get_bOnLine_Proxy( 
    IStorageMan * This,
    /* [retval][out] */ VARIANT_BOOL *pbOnLine);


void __RPC_STUB IStorageMan_get_bOnLine_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE IStorageMan_get_bstrFileFormatCLSID_Proxy( 
    IStorageMan * This,
    /* [retval][out] */ BSTR *pVal);


void __RPC_STUB IStorageMan_get_bstrFileFormatCLSID_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE IStorageMan_get_bHasVideo_Proxy( 
    IStorageMan * This,
    /* [retval][out] */ VARIANT_BOOL *pVal);


void __RPC_STUB IStorageMan_get_bHasVideo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE IStorageMan_get_bstrVideoPath_Proxy( 
    IStorageMan * This,
    /* [retval][out] */ BSTR *pbstrPath);


void __RPC_STUB IStorageMan_get_bstrVideoPath_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE IStorageMan_get_lPruneType_Proxy( 
    IStorageMan * This,
    /* [retval][out] */ long *pVal);


void __RPC_STUB IStorageMan_get_lPruneType_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IStorageMan_AddObserver_Proxy( 
    IStorageMan * This,
    /* [in] */ IStorageObserver *pIObserver);


void __RPC_STUB IStorageMan_AddObserver_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IStorageMan_RemoveObserver_Proxy( 
    IStorageMan * This,
    /* [in] */ IStorageObserver *pIObserver);


void __RPC_STUB IStorageMan_RemoveObserver_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IStorageMan_INTERFACE_DEFINED__ */


#ifndef __IStorageManEx_INTERFACE_DEFINED__
#define __IStorageManEx_INTERFACE_DEFINED__

/* interface IStorageManEx */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IStorageManEx;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("8EEAE4ED-724F-4034-BA93-7A56D9C293CD")
    IStorageManEx : public IStorageMan
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE New( 
            /* [in] */ BSTR bstrName,
            /* [in] */ BSTR bstrCLSID) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE NewEx( 
            /* [in] */ BSTR bstrName,
            /* [in] */ IDispatch *pTest,
            /* [in] */ BSTR bstrCLSID) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Open( 
            /* [in] */ BSTR bstrName,
            /* [in] */ DWORD dwShareFlags,
            /* [in] */ VARIANT_BOOL bOnline,
            /* [in] */ VARIANT_BOOL bReadOnly) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE OpenEx( 
            /* [in] */ IDispatch *pITest,
            /* [in] */ DWORD dwShareFlags,
            /* [in] */ VARIANT_BOOL bOnline,
            /* [in] */ VARIANT_BOOL bReadOnly) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Save( void) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Close( void) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE SetPrunedSegments( 
            /* [in] */ IEegSegments *pISegments,
            /* [in] */ long lPruneType) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE EndPrune( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IStorageManExVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IStorageManEx * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IStorageManEx * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IStorageManEx * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IStorageManEx * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IStorageManEx * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IStorageManEx * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IStorageManEx * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_bReadOnly )( 
            IStorageManEx * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_bOpen )( 
            IStorageManEx * This,
            /* [retval][out] */ VARIANT_BOOL *pbOpen);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetEegSegments )( 
            IStorageManEx * This,
            /* [retval][out] */ IEegSegments **ppIEegSegments);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetVideoSegments )( 
            IStorageManEx * This,
            /* [retval][out] */ IEegSegments **ppIEegSegments);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetEegData )( 
            IStorageManEx * This,
            /* [retval][out] */ IEegData **ppIEegData);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetEvents )( 
            IStorageManEx * This,
            /* [retval][out] */ IUserEvents **ppIUserEvents);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetEventWrapper )( 
            IStorageManEx * This,
            /* [retval][out] */ IEventWrapper **ppIEventWrapper);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetSleepData )( 
            IStorageManEx * This,
            /* [out] */ ISleepData **ppISleepData);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_bstrFileName )( 
            IStorageManEx * This,
            /* [retval][out] */ BSTR *pbstrFileName);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_bModified )( 
            IStorageManEx * This,
            /* [retval][out] */ VARIANT_BOOL *pbModified);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_bOnLine )( 
            IStorageManEx * This,
            /* [retval][out] */ VARIANT_BOOL *pbOnLine);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_bstrFileFormatCLSID )( 
            IStorageManEx * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_bHasVideo )( 
            IStorageManEx * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_bstrVideoPath )( 
            IStorageManEx * This,
            /* [retval][out] */ BSTR *pbstrPath);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_lPruneType )( 
            IStorageManEx * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *AddObserver )( 
            IStorageManEx * This,
            /* [in] */ IStorageObserver *pIObserver);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *RemoveObserver )( 
            IStorageManEx * This,
            /* [in] */ IStorageObserver *pIObserver);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *New )( 
            IStorageManEx * This,
            /* [in] */ BSTR bstrName,
            /* [in] */ BSTR bstrCLSID);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *NewEx )( 
            IStorageManEx * This,
            /* [in] */ BSTR bstrName,
            /* [in] */ IDispatch *pTest,
            /* [in] */ BSTR bstrCLSID);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *Open )( 
            IStorageManEx * This,
            /* [in] */ BSTR bstrName,
            /* [in] */ DWORD dwShareFlags,
            /* [in] */ VARIANT_BOOL bOnline,
            /* [in] */ VARIANT_BOOL bReadOnly);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *OpenEx )( 
            IStorageManEx * This,
            /* [in] */ IDispatch *pITest,
            /* [in] */ DWORD dwShareFlags,
            /* [in] */ VARIANT_BOOL bOnline,
            /* [in] */ VARIANT_BOOL bReadOnly);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *Save )( 
            IStorageManEx * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *Close )( 
            IStorageManEx * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetPrunedSegments )( 
            IStorageManEx * This,
            /* [in] */ IEegSegments *pISegments,
            /* [in] */ long lPruneType);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *EndPrune )( 
            IStorageManEx * This);
        
        END_INTERFACE
    } IStorageManExVtbl;

    interface IStorageManEx
    {
        CONST_VTBL struct IStorageManExVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IStorageManEx_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IStorageManEx_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IStorageManEx_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IStorageManEx_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IStorageManEx_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IStorageManEx_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IStorageManEx_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IStorageManEx_get_bReadOnly(This,pVal)	\
    (This)->lpVtbl -> get_bReadOnly(This,pVal)

#define IStorageManEx_get_bOpen(This,pbOpen)	\
    (This)->lpVtbl -> get_bOpen(This,pbOpen)

#define IStorageManEx_GetEegSegments(This,ppIEegSegments)	\
    (This)->lpVtbl -> GetEegSegments(This,ppIEegSegments)

#define IStorageManEx_GetVideoSegments(This,ppIEegSegments)	\
    (This)->lpVtbl -> GetVideoSegments(This,ppIEegSegments)

#define IStorageManEx_GetEegData(This,ppIEegData)	\
    (This)->lpVtbl -> GetEegData(This,ppIEegData)

#define IStorageManEx_GetEvents(This,ppIUserEvents)	\
    (This)->lpVtbl -> GetEvents(This,ppIUserEvents)

#define IStorageManEx_GetEventWrapper(This,ppIEventWrapper)	\
    (This)->lpVtbl -> GetEventWrapper(This,ppIEventWrapper)

#define IStorageManEx_GetSleepData(This,ppISleepData)	\
    (This)->lpVtbl -> GetSleepData(This,ppISleepData)


#define IStorageManEx_get_bstrFileName(This,pbstrFileName)	\
    (This)->lpVtbl -> get_bstrFileName(This,pbstrFileName)

#define IStorageManEx_get_bModified(This,pbModified)	\
    (This)->lpVtbl -> get_bModified(This,pbModified)

#define IStorageManEx_get_bOnLine(This,pbOnLine)	\
    (This)->lpVtbl -> get_bOnLine(This,pbOnLine)

#define IStorageManEx_get_bstrFileFormatCLSID(This,pVal)	\
    (This)->lpVtbl -> get_bstrFileFormatCLSID(This,pVal)

#define IStorageManEx_get_bHasVideo(This,pVal)	\
    (This)->lpVtbl -> get_bHasVideo(This,pVal)

#define IStorageManEx_get_bstrVideoPath(This,pbstrPath)	\
    (This)->lpVtbl -> get_bstrVideoPath(This,pbstrPath)

#define IStorageManEx_get_lPruneType(This,pVal)	\
    (This)->lpVtbl -> get_lPruneType(This,pVal)

#define IStorageManEx_AddObserver(This,pIObserver)	\
    (This)->lpVtbl -> AddObserver(This,pIObserver)

#define IStorageManEx_RemoveObserver(This,pIObserver)	\
    (This)->lpVtbl -> RemoveObserver(This,pIObserver)


#define IStorageManEx_New(This,bstrName,bstrCLSID)	\
    (This)->lpVtbl -> New(This,bstrName,bstrCLSID)

#define IStorageManEx_NewEx(This,bstrName,pTest,bstrCLSID)	\
    (This)->lpVtbl -> NewEx(This,bstrName,pTest,bstrCLSID)

#define IStorageManEx_Open(This,bstrName,dwShareFlags,bOnline,bReadOnly)	\
    (This)->lpVtbl -> Open(This,bstrName,dwShareFlags,bOnline,bReadOnly)

#define IStorageManEx_OpenEx(This,pITest,dwShareFlags,bOnline,bReadOnly)	\
    (This)->lpVtbl -> OpenEx(This,pITest,dwShareFlags,bOnline,bReadOnly)

#define IStorageManEx_Save(This)	\
    (This)->lpVtbl -> Save(This)

#define IStorageManEx_Close(This)	\
    (This)->lpVtbl -> Close(This)

#define IStorageManEx_SetPrunedSegments(This,pISegments,lPruneType)	\
    (This)->lpVtbl -> SetPrunedSegments(This,pISegments,lPruneType)

#define IStorageManEx_EndPrune(This)	\
    (This)->lpVtbl -> EndPrune(This)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring] */ HRESULT STDMETHODCALLTYPE IStorageManEx_New_Proxy( 
    IStorageManEx * This,
    /* [in] */ BSTR bstrName,
    /* [in] */ BSTR bstrCLSID);


void __RPC_STUB IStorageManEx_New_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IStorageManEx_NewEx_Proxy( 
    IStorageManEx * This,
    /* [in] */ BSTR bstrName,
    /* [in] */ IDispatch *pTest,
    /* [in] */ BSTR bstrCLSID);


void __RPC_STUB IStorageManEx_NewEx_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IStorageManEx_Open_Proxy( 
    IStorageManEx * This,
    /* [in] */ BSTR bstrName,
    /* [in] */ DWORD dwShareFlags,
    /* [in] */ VARIANT_BOOL bOnline,
    /* [in] */ VARIANT_BOOL bReadOnly);


void __RPC_STUB IStorageManEx_Open_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IStorageManEx_OpenEx_Proxy( 
    IStorageManEx * This,
    /* [in] */ IDispatch *pITest,
    /* [in] */ DWORD dwShareFlags,
    /* [in] */ VARIANT_BOOL bOnline,
    /* [in] */ VARIANT_BOOL bReadOnly);


void __RPC_STUB IStorageManEx_OpenEx_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IStorageManEx_Save_Proxy( 
    IStorageManEx * This);


void __RPC_STUB IStorageManEx_Save_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IStorageManEx_Close_Proxy( 
    IStorageManEx * This);


void __RPC_STUB IStorageManEx_Close_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IStorageManEx_SetPrunedSegments_Proxy( 
    IStorageManEx * This,
    /* [in] */ IEegSegments *pISegments,
    /* [in] */ long lPruneType);


void __RPC_STUB IStorageManEx_SetPrunedSegments_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IStorageManEx_EndPrune_Proxy( 
    IStorageManEx * This);


void __RPC_STUB IStorageManEx_EndPrune_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IStorageManEx_INTERFACE_DEFINED__ */


#ifndef __IStorageManProxy_INTERFACE_DEFINED__
#define __IStorageManProxy_INTERFACE_DEFINED__

/* interface IStorageManProxy */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IStorageManProxy;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("0329CCF6-78E6-489a-AC02-91CF592A5934")
    IStorageManProxy : public IDispatch
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Initialize( 
            /* [in] */ IStorageMan *pIStorageMan,
            /* [in] */ VARIANT_BOOL bSetEvents,
            /* [in] */ VARIANT_BOOL bAddAsObserver) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE SetEventWrapper( 
            /* [in] */ IEventWrapper *pIEventWrapper) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Uninitialize( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IStorageManProxyVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IStorageManProxy * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IStorageManProxy * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IStorageManProxy * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IStorageManProxy * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IStorageManProxy * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IStorageManProxy * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IStorageManProxy * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *Initialize )( 
            IStorageManProxy * This,
            /* [in] */ IStorageMan *pIStorageMan,
            /* [in] */ VARIANT_BOOL bSetEvents,
            /* [in] */ VARIANT_BOOL bAddAsObserver);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetEventWrapper )( 
            IStorageManProxy * This,
            /* [in] */ IEventWrapper *pIEventWrapper);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *Uninitialize )( 
            IStorageManProxy * This);
        
        END_INTERFACE
    } IStorageManProxyVtbl;

    interface IStorageManProxy
    {
        CONST_VTBL struct IStorageManProxyVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IStorageManProxy_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IStorageManProxy_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IStorageManProxy_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IStorageManProxy_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IStorageManProxy_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IStorageManProxy_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IStorageManProxy_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IStorageManProxy_Initialize(This,pIStorageMan,bSetEvents,bAddAsObserver)	\
    (This)->lpVtbl -> Initialize(This,pIStorageMan,bSetEvents,bAddAsObserver)

#define IStorageManProxy_SetEventWrapper(This,pIEventWrapper)	\
    (This)->lpVtbl -> SetEventWrapper(This,pIEventWrapper)

#define IStorageManProxy_Uninitialize(This)	\
    (This)->lpVtbl -> Uninitialize(This)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring] */ HRESULT STDMETHODCALLTYPE IStorageManProxy_Initialize_Proxy( 
    IStorageManProxy * This,
    /* [in] */ IStorageMan *pIStorageMan,
    /* [in] */ VARIANT_BOOL bSetEvents,
    /* [in] */ VARIANT_BOOL bAddAsObserver);


void __RPC_STUB IStorageManProxy_Initialize_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IStorageManProxy_SetEventWrapper_Proxy( 
    IStorageManProxy * This,
    /* [in] */ IEventWrapper *pIEventWrapper);


void __RPC_STUB IStorageManProxy_SetEventWrapper_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IStorageManProxy_Uninitialize_Proxy( 
    IStorageManProxy * This);


void __RPC_STUB IStorageManProxy_Uninitialize_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IStorageManProxy_INTERFACE_DEFINED__ */


#ifndef __IEventWrapperCopy_INTERFACE_DEFINED__
#define __IEventWrapperCopy_INTERFACE_DEFINED__

/* interface IEventWrapperCopy */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IEventWrapperCopy;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("CC5B5C73-11CE-47b2-8EEB-323827ACC94F")
    IEventWrapperCopy : public IUnknown
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE SetEventWrapper( 
            /* [in] */ IUnknown *pIUnk) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IEventWrapperCopyVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IEventWrapperCopy * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IEventWrapperCopy * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IEventWrapperCopy * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetEventWrapper )( 
            IEventWrapperCopy * This,
            /* [in] */ IUnknown *pIUnk);
        
        END_INTERFACE
    } IEventWrapperCopyVtbl;

    interface IEventWrapperCopy
    {
        CONST_VTBL struct IEventWrapperCopyVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IEventWrapperCopy_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IEventWrapperCopy_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IEventWrapperCopy_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IEventWrapperCopy_SetEventWrapper(This,pIUnk)	\
    (This)->lpVtbl -> SetEventWrapper(This,pIUnk)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring] */ HRESULT STDMETHODCALLTYPE IEventWrapperCopy_SetEventWrapper_Proxy( 
    IEventWrapperCopy * This,
    /* [in] */ IUnknown *pIUnk);


void __RPC_STUB IEventWrapperCopy_SetEventWrapper_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IEventWrapperCopy_INTERFACE_DEFINED__ */


#ifndef __ISleepData_INTERFACE_DEFINED__
#define __ISleepData_INTERFACE_DEFINED__

/* interface ISleepData */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_ISleepData;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("E64B2F6D-7F8E-4686-A1B4-658132A011AB")
    ISleepData : public IUnknown
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Initialize( 
            /* [in] */ INrvData3 *pINrvData,
            /* [in] */ INrvSegments3 *pISegments,
            /* [in] */ long lWriteMode) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Uninitialize( void) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Add( 
            /* [in] */ BSTR bstrLabel,
            /* [in] */ long lEpochLength) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Delete( 
            /* [in] */ BSTR bstrLabel) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Rename( 
            /* [in] */ BSTR bstrLabel,
            /* [in] */ BSTR bstrNewLabel) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Duplicate( 
            /* [in] */ BSTR bstrLabelSrc,
            /* [in] */ BSTR bstrNewLabel) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Save( void) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE SwapItems( 
            /* [in] */ BSTR bstrLabelA,
            /* [in] */ BSTR bstrLabelB) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE WriteData( 
            /* [in] */ BSTR bstrLabel,
            /* [in] */ long liEpoch,
            /* [in] */ BYTE byteScore,
            /* [in] */ long lEpochLength) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE WriteData2( 
            /* [in] */ long liEpoch,
            /* [in] */ BYTE byteScore) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE ReadData2( 
            /* [in] */ BSTR bstrLabel,
            /* [in] */ long liEpoch,
            /* [out] */ BYTE *pbyteScore,
            /* [out] */ long *plLength) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE ReadData( 
            /* [in] */ BSTR bstrLabel,
            /* [in] */ long liFirstEpoch,
            /* [in] */ long lcEpochs,
            /* [size_is][out] */ BYTE *pbyteScore,
            /* [size_is][out] */ long *plLength) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetHypnogramCount( 
            /* [retval][out] */ long *plcCount) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetHypnogramNameFromIndex( 
            /* [in] */ long liIndex,
            /* [retval][out] */ BSTR *pbstrLabel) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_bstrCurrentHypnogram( 
            /* [retval][out] */ BSTR *pbstrLabel) = 0;
        
        virtual /* [helpstring][propput] */ HRESULT STDMETHODCALLTYPE put_bstrCurrentHypnogram( 
            /* [in] */ BSTR bstrLabel) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Epoch2RecSec( 
            /* [in] */ BSTR bstrLabel,
            /* [in] */ long liEpoch,
            /* [retval][out] */ double *pdRecSec) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE RecSec2Epoch( 
            /* [in] */ BSTR bstrLabel,
            /* [in] */ double dRecSec,
            /* [retval][out] */ long *pliEpoch) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_lEpochLength( 
            /* [in] */ BSTR bstrLabel,
            /* [in] */ long liEpoch,
            /* [retval][out] */ long *plEpochLength) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_lDefaultEpochLength( 
            /* [retval][out] */ long *plVal) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_lcEpochs( 
            /* [in] */ BSTR bstrLabel,
            /* [retval][out] */ long *plcEpochs) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_bSleepMode( 
            /* [retval][out] */ VARIANT_BOOL *pbMode) = 0;
        
        virtual /* [helpstring][propput] */ HRESULT STDMETHODCALLTYPE put_bSleepMode( 
            /* [in] */ VARIANT_BOOL bMode) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_lWriteMode( 
            /* [retval][out] */ long *plWriteMode) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_lType( 
            /* [retval][out] */ long *plType) = 0;
        
        virtual /* [helpstring][propput] */ HRESULT STDMETHODCALLTYPE put_lType( 
            /* [in] */ long lType) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_bDirty( 
            /* [in] */ BSTR bstrLabel,
            /* [retval][out] */ VARIANT_BOOL *pbDirty) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Segment( 
            /* [in] */ VARIANT_BOOL bIsSegment) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Prune( 
            /* [in] */ VARIANT_BOOL bStartPrune,
            /* [in] */ INrvSegments3 *pISegments) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE PrePrune( void) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Clone( 
            /* [in] */ ISleepData *pISleepDataSrc) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_bNeedsRedraw( 
            /* [in] */ BSTR bstrLabel,
            /* [retval][out] */ VARIANT_BOOL *pbMode) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ISleepDataVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ISleepData * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ISleepData * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ISleepData * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *Initialize )( 
            ISleepData * This,
            /* [in] */ INrvData3 *pINrvData,
            /* [in] */ INrvSegments3 *pISegments,
            /* [in] */ long lWriteMode);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *Uninitialize )( 
            ISleepData * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *Add )( 
            ISleepData * This,
            /* [in] */ BSTR bstrLabel,
            /* [in] */ long lEpochLength);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *Delete )( 
            ISleepData * This,
            /* [in] */ BSTR bstrLabel);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *Rename )( 
            ISleepData * This,
            /* [in] */ BSTR bstrLabel,
            /* [in] */ BSTR bstrNewLabel);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *Duplicate )( 
            ISleepData * This,
            /* [in] */ BSTR bstrLabelSrc,
            /* [in] */ BSTR bstrNewLabel);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *Save )( 
            ISleepData * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *SwapItems )( 
            ISleepData * This,
            /* [in] */ BSTR bstrLabelA,
            /* [in] */ BSTR bstrLabelB);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *WriteData )( 
            ISleepData * This,
            /* [in] */ BSTR bstrLabel,
            /* [in] */ long liEpoch,
            /* [in] */ BYTE byteScore,
            /* [in] */ long lEpochLength);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *WriteData2 )( 
            ISleepData * This,
            /* [in] */ long liEpoch,
            /* [in] */ BYTE byteScore);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *ReadData2 )( 
            ISleepData * This,
            /* [in] */ BSTR bstrLabel,
            /* [in] */ long liEpoch,
            /* [out] */ BYTE *pbyteScore,
            /* [out] */ long *plLength);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *ReadData )( 
            ISleepData * This,
            /* [in] */ BSTR bstrLabel,
            /* [in] */ long liFirstEpoch,
            /* [in] */ long lcEpochs,
            /* [size_is][out] */ BYTE *pbyteScore,
            /* [size_is][out] */ long *plLength);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetHypnogramCount )( 
            ISleepData * This,
            /* [retval][out] */ long *plcCount);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetHypnogramNameFromIndex )( 
            ISleepData * This,
            /* [in] */ long liIndex,
            /* [retval][out] */ BSTR *pbstrLabel);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_bstrCurrentHypnogram )( 
            ISleepData * This,
            /* [retval][out] */ BSTR *pbstrLabel);
        
        /* [helpstring][propput] */ HRESULT ( STDMETHODCALLTYPE *put_bstrCurrentHypnogram )( 
            ISleepData * This,
            /* [in] */ BSTR bstrLabel);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *Epoch2RecSec )( 
            ISleepData * This,
            /* [in] */ BSTR bstrLabel,
            /* [in] */ long liEpoch,
            /* [retval][out] */ double *pdRecSec);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *RecSec2Epoch )( 
            ISleepData * This,
            /* [in] */ BSTR bstrLabel,
            /* [in] */ double dRecSec,
            /* [retval][out] */ long *pliEpoch);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_lEpochLength )( 
            ISleepData * This,
            /* [in] */ BSTR bstrLabel,
            /* [in] */ long liEpoch,
            /* [retval][out] */ long *plEpochLength);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_lDefaultEpochLength )( 
            ISleepData * This,
            /* [retval][out] */ long *plVal);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_lcEpochs )( 
            ISleepData * This,
            /* [in] */ BSTR bstrLabel,
            /* [retval][out] */ long *plcEpochs);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_bSleepMode )( 
            ISleepData * This,
            /* [retval][out] */ VARIANT_BOOL *pbMode);
        
        /* [helpstring][propput] */ HRESULT ( STDMETHODCALLTYPE *put_bSleepMode )( 
            ISleepData * This,
            /* [in] */ VARIANT_BOOL bMode);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_lWriteMode )( 
            ISleepData * This,
            /* [retval][out] */ long *plWriteMode);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_lType )( 
            ISleepData * This,
            /* [retval][out] */ long *plType);
        
        /* [helpstring][propput] */ HRESULT ( STDMETHODCALLTYPE *put_lType )( 
            ISleepData * This,
            /* [in] */ long lType);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_bDirty )( 
            ISleepData * This,
            /* [in] */ BSTR bstrLabel,
            /* [retval][out] */ VARIANT_BOOL *pbDirty);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *Segment )( 
            ISleepData * This,
            /* [in] */ VARIANT_BOOL bIsSegment);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *Prune )( 
            ISleepData * This,
            /* [in] */ VARIANT_BOOL bStartPrune,
            /* [in] */ INrvSegments3 *pISegments);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *PrePrune )( 
            ISleepData * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *Clone )( 
            ISleepData * This,
            /* [in] */ ISleepData *pISleepDataSrc);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_bNeedsRedraw )( 
            ISleepData * This,
            /* [in] */ BSTR bstrLabel,
            /* [retval][out] */ VARIANT_BOOL *pbMode);
        
        END_INTERFACE
    } ISleepDataVtbl;

    interface ISleepData
    {
        CONST_VTBL struct ISleepDataVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ISleepData_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ISleepData_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ISleepData_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ISleepData_Initialize(This,pINrvData,pISegments,lWriteMode)	\
    (This)->lpVtbl -> Initialize(This,pINrvData,pISegments,lWriteMode)

#define ISleepData_Uninitialize(This)	\
    (This)->lpVtbl -> Uninitialize(This)

#define ISleepData_Add(This,bstrLabel,lEpochLength)	\
    (This)->lpVtbl -> Add(This,bstrLabel,lEpochLength)

#define ISleepData_Delete(This,bstrLabel)	\
    (This)->lpVtbl -> Delete(This,bstrLabel)

#define ISleepData_Rename(This,bstrLabel,bstrNewLabel)	\
    (This)->lpVtbl -> Rename(This,bstrLabel,bstrNewLabel)

#define ISleepData_Duplicate(This,bstrLabelSrc,bstrNewLabel)	\
    (This)->lpVtbl -> Duplicate(This,bstrLabelSrc,bstrNewLabel)

#define ISleepData_Save(This)	\
    (This)->lpVtbl -> Save(This)

#define ISleepData_SwapItems(This,bstrLabelA,bstrLabelB)	\
    (This)->lpVtbl -> SwapItems(This,bstrLabelA,bstrLabelB)

#define ISleepData_WriteData(This,bstrLabel,liEpoch,byteScore,lEpochLength)	\
    (This)->lpVtbl -> WriteData(This,bstrLabel,liEpoch,byteScore,lEpochLength)

#define ISleepData_WriteData2(This,liEpoch,byteScore)	\
    (This)->lpVtbl -> WriteData2(This,liEpoch,byteScore)

#define ISleepData_ReadData2(This,bstrLabel,liEpoch,pbyteScore,plLength)	\
    (This)->lpVtbl -> ReadData2(This,bstrLabel,liEpoch,pbyteScore,plLength)

#define ISleepData_ReadData(This,bstrLabel,liFirstEpoch,lcEpochs,pbyteScore,plLength)	\
    (This)->lpVtbl -> ReadData(This,bstrLabel,liFirstEpoch,lcEpochs,pbyteScore,plLength)

#define ISleepData_GetHypnogramCount(This,plcCount)	\
    (This)->lpVtbl -> GetHypnogramCount(This,plcCount)

#define ISleepData_GetHypnogramNameFromIndex(This,liIndex,pbstrLabel)	\
    (This)->lpVtbl -> GetHypnogramNameFromIndex(This,liIndex,pbstrLabel)

#define ISleepData_get_bstrCurrentHypnogram(This,pbstrLabel)	\
    (This)->lpVtbl -> get_bstrCurrentHypnogram(This,pbstrLabel)

#define ISleepData_put_bstrCurrentHypnogram(This,bstrLabel)	\
    (This)->lpVtbl -> put_bstrCurrentHypnogram(This,bstrLabel)

#define ISleepData_Epoch2RecSec(This,bstrLabel,liEpoch,pdRecSec)	\
    (This)->lpVtbl -> Epoch2RecSec(This,bstrLabel,liEpoch,pdRecSec)

#define ISleepData_RecSec2Epoch(This,bstrLabel,dRecSec,pliEpoch)	\
    (This)->lpVtbl -> RecSec2Epoch(This,bstrLabel,dRecSec,pliEpoch)

#define ISleepData_get_lEpochLength(This,bstrLabel,liEpoch,plEpochLength)	\
    (This)->lpVtbl -> get_lEpochLength(This,bstrLabel,liEpoch,plEpochLength)

#define ISleepData_get_lDefaultEpochLength(This,plVal)	\
    (This)->lpVtbl -> get_lDefaultEpochLength(This,plVal)

#define ISleepData_get_lcEpochs(This,bstrLabel,plcEpochs)	\
    (This)->lpVtbl -> get_lcEpochs(This,bstrLabel,plcEpochs)

#define ISleepData_get_bSleepMode(This,pbMode)	\
    (This)->lpVtbl -> get_bSleepMode(This,pbMode)

#define ISleepData_put_bSleepMode(This,bMode)	\
    (This)->lpVtbl -> put_bSleepMode(This,bMode)

#define ISleepData_get_lWriteMode(This,plWriteMode)	\
    (This)->lpVtbl -> get_lWriteMode(This,plWriteMode)

#define ISleepData_get_lType(This,plType)	\
    (This)->lpVtbl -> get_lType(This,plType)

#define ISleepData_put_lType(This,lType)	\
    (This)->lpVtbl -> put_lType(This,lType)

#define ISleepData_get_bDirty(This,bstrLabel,pbDirty)	\
    (This)->lpVtbl -> get_bDirty(This,bstrLabel,pbDirty)

#define ISleepData_Segment(This,bIsSegment)	\
    (This)->lpVtbl -> Segment(This,bIsSegment)

#define ISleepData_Prune(This,bStartPrune,pISegments)	\
    (This)->lpVtbl -> Prune(This,bStartPrune,pISegments)

#define ISleepData_PrePrune(This)	\
    (This)->lpVtbl -> PrePrune(This)

#define ISleepData_Clone(This,pISleepDataSrc)	\
    (This)->lpVtbl -> Clone(This,pISleepDataSrc)

#define ISleepData_get_bNeedsRedraw(This,bstrLabel,pbMode)	\
    (This)->lpVtbl -> get_bNeedsRedraw(This,bstrLabel,pbMode)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISleepData_Initialize_Proxy( 
    ISleepData * This,
    /* [in] */ INrvData3 *pINrvData,
    /* [in] */ INrvSegments3 *pISegments,
    /* [in] */ long lWriteMode);


void __RPC_STUB ISleepData_Initialize_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISleepData_Uninitialize_Proxy( 
    ISleepData * This);


void __RPC_STUB ISleepData_Uninitialize_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISleepData_Add_Proxy( 
    ISleepData * This,
    /* [in] */ BSTR bstrLabel,
    /* [in] */ long lEpochLength);


void __RPC_STUB ISleepData_Add_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISleepData_Delete_Proxy( 
    ISleepData * This,
    /* [in] */ BSTR bstrLabel);


void __RPC_STUB ISleepData_Delete_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISleepData_Rename_Proxy( 
    ISleepData * This,
    /* [in] */ BSTR bstrLabel,
    /* [in] */ BSTR bstrNewLabel);


void __RPC_STUB ISleepData_Rename_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISleepData_Duplicate_Proxy( 
    ISleepData * This,
    /* [in] */ BSTR bstrLabelSrc,
    /* [in] */ BSTR bstrNewLabel);


void __RPC_STUB ISleepData_Duplicate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISleepData_Save_Proxy( 
    ISleepData * This);


void __RPC_STUB ISleepData_Save_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISleepData_SwapItems_Proxy( 
    ISleepData * This,
    /* [in] */ BSTR bstrLabelA,
    /* [in] */ BSTR bstrLabelB);


void __RPC_STUB ISleepData_SwapItems_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISleepData_WriteData_Proxy( 
    ISleepData * This,
    /* [in] */ BSTR bstrLabel,
    /* [in] */ long liEpoch,
    /* [in] */ BYTE byteScore,
    /* [in] */ long lEpochLength);


void __RPC_STUB ISleepData_WriteData_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISleepData_WriteData2_Proxy( 
    ISleepData * This,
    /* [in] */ long liEpoch,
    /* [in] */ BYTE byteScore);


void __RPC_STUB ISleepData_WriteData2_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISleepData_ReadData2_Proxy( 
    ISleepData * This,
    /* [in] */ BSTR bstrLabel,
    /* [in] */ long liEpoch,
    /* [out] */ BYTE *pbyteScore,
    /* [out] */ long *plLength);


void __RPC_STUB ISleepData_ReadData2_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISleepData_ReadData_Proxy( 
    ISleepData * This,
    /* [in] */ BSTR bstrLabel,
    /* [in] */ long liFirstEpoch,
    /* [in] */ long lcEpochs,
    /* [size_is][out] */ BYTE *pbyteScore,
    /* [size_is][out] */ long *plLength);


void __RPC_STUB ISleepData_ReadData_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISleepData_GetHypnogramCount_Proxy( 
    ISleepData * This,
    /* [retval][out] */ long *plcCount);


void __RPC_STUB ISleepData_GetHypnogramCount_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISleepData_GetHypnogramNameFromIndex_Proxy( 
    ISleepData * This,
    /* [in] */ long liIndex,
    /* [retval][out] */ BSTR *pbstrLabel);


void __RPC_STUB ISleepData_GetHypnogramNameFromIndex_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE ISleepData_get_bstrCurrentHypnogram_Proxy( 
    ISleepData * This,
    /* [retval][out] */ BSTR *pbstrLabel);


void __RPC_STUB ISleepData_get_bstrCurrentHypnogram_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput] */ HRESULT STDMETHODCALLTYPE ISleepData_put_bstrCurrentHypnogram_Proxy( 
    ISleepData * This,
    /* [in] */ BSTR bstrLabel);


void __RPC_STUB ISleepData_put_bstrCurrentHypnogram_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISleepData_Epoch2RecSec_Proxy( 
    ISleepData * This,
    /* [in] */ BSTR bstrLabel,
    /* [in] */ long liEpoch,
    /* [retval][out] */ double *pdRecSec);


void __RPC_STUB ISleepData_Epoch2RecSec_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISleepData_RecSec2Epoch_Proxy( 
    ISleepData * This,
    /* [in] */ BSTR bstrLabel,
    /* [in] */ double dRecSec,
    /* [retval][out] */ long *pliEpoch);


void __RPC_STUB ISleepData_RecSec2Epoch_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE ISleepData_get_lEpochLength_Proxy( 
    ISleepData * This,
    /* [in] */ BSTR bstrLabel,
    /* [in] */ long liEpoch,
    /* [retval][out] */ long *plEpochLength);


void __RPC_STUB ISleepData_get_lEpochLength_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE ISleepData_get_lDefaultEpochLength_Proxy( 
    ISleepData * This,
    /* [retval][out] */ long *plVal);


void __RPC_STUB ISleepData_get_lDefaultEpochLength_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE ISleepData_get_lcEpochs_Proxy( 
    ISleepData * This,
    /* [in] */ BSTR bstrLabel,
    /* [retval][out] */ long *plcEpochs);


void __RPC_STUB ISleepData_get_lcEpochs_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE ISleepData_get_bSleepMode_Proxy( 
    ISleepData * This,
    /* [retval][out] */ VARIANT_BOOL *pbMode);


void __RPC_STUB ISleepData_get_bSleepMode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput] */ HRESULT STDMETHODCALLTYPE ISleepData_put_bSleepMode_Proxy( 
    ISleepData * This,
    /* [in] */ VARIANT_BOOL bMode);


void __RPC_STUB ISleepData_put_bSleepMode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE ISleepData_get_lWriteMode_Proxy( 
    ISleepData * This,
    /* [retval][out] */ long *plWriteMode);


void __RPC_STUB ISleepData_get_lWriteMode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE ISleepData_get_lType_Proxy( 
    ISleepData * This,
    /* [retval][out] */ long *plType);


void __RPC_STUB ISleepData_get_lType_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput] */ HRESULT STDMETHODCALLTYPE ISleepData_put_lType_Proxy( 
    ISleepData * This,
    /* [in] */ long lType);


void __RPC_STUB ISleepData_put_lType_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE ISleepData_get_bDirty_Proxy( 
    ISleepData * This,
    /* [in] */ BSTR bstrLabel,
    /* [retval][out] */ VARIANT_BOOL *pbDirty);


void __RPC_STUB ISleepData_get_bDirty_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISleepData_Segment_Proxy( 
    ISleepData * This,
    /* [in] */ VARIANT_BOOL bIsSegment);


void __RPC_STUB ISleepData_Segment_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISleepData_Prune_Proxy( 
    ISleepData * This,
    /* [in] */ VARIANT_BOOL bStartPrune,
    /* [in] */ INrvSegments3 *pISegments);


void __RPC_STUB ISleepData_Prune_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISleepData_PrePrune_Proxy( 
    ISleepData * This);


void __RPC_STUB ISleepData_PrePrune_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISleepData_Clone_Proxy( 
    ISleepData * This,
    /* [in] */ ISleepData *pISleepDataSrc);


void __RPC_STUB ISleepData_Clone_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE ISleepData_get_bNeedsRedraw_Proxy( 
    ISleepData * This,
    /* [in] */ BSTR bstrLabel,
    /* [retval][out] */ VARIANT_BOOL *pbMode);


void __RPC_STUB ISleepData_get_bNeedsRedraw_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ISleepData_INTERFACE_DEFINED__ */



#ifndef __FILEACCESS3Lib_LIBRARY_DEFINED__
#define __FILEACCESS3Lib_LIBRARY_DEFINED__

/* library FILEACCESS3Lib */
/* [helpstring][version][uuid] */ 


enum WRITEMODE
    {	WRITEMODE_IMMEDIATE	= 0,
	WRITEMODE_REVIEW	= WRITEMODE_IMMEDIATE + 1,
	WRITEMODE_READONLY	= WRITEMODE_REVIEW + 1
    } ;

EXTERN_C const IID LIBID_FILEACCESS3Lib;

EXTERN_C const CLSID CLSID_FileWrite3;

#ifdef __cplusplus

class DECLSPEC_UUID("BFB9D16B-512D-47e6-AD85-632B0F39FEB3")
FileWrite3;
#endif

EXTERN_C const CLSID CLSID_FileRead3;

#ifdef __cplusplus

class DECLSPEC_UUID("BE3AC01B-934F-4fbb-98E6-C977710F1627")
FileRead3;
#endif

EXTERN_C const CLSID CLSID_EventWrapper;

#ifdef __cplusplus

class DECLSPEC_UUID("D02AEFCC-B0D0-4877-8AA1-149182C9194B")
EventWrapper;
#endif

EXTERN_C const CLSID CLSID_UserEvent;

#ifdef __cplusplus

class DECLSPEC_UUID("28FC5B18-24BA-48a8-9F7E-230B10F96E8A")
UserEvent;
#endif

EXTERN_C const CLSID CLSID_TrendWrite;

#ifdef __cplusplus

class DECLSPEC_UUID("61AE14D0-D711-4e0b-96F7-BD66495471BD")
TrendWrite;
#endif

EXTERN_C const CLSID CLSID_TrendRead;

#ifdef __cplusplus

class DECLSPEC_UUID("2290B46F-421A-4149-9E36-0640BA80A230")
TrendRead;
#endif

EXTERN_C const CLSID CLSID_ImpedanceEvent;

#ifdef __cplusplus

class DECLSPEC_UUID("CF4F7DEC-FE6E-454b-B074-442D34C91C86")
ImpedanceEvent;
#endif

EXTERN_C const CLSID CLSID_EegData;

#ifdef __cplusplus

class DECLSPEC_UUID("77E268AC-1AEC-440a-BE64-192192543F16")
EegData;
#endif

EXTERN_C const CLSID CLSID_EegSegments;

#ifdef __cplusplus

class DECLSPEC_UUID("38979206-3556-426c-9423-641208C06521")
EegSegments;
#endif

EXTERN_C const CLSID CLSID_EventWrapperProxy;

#ifdef __cplusplus

class DECLSPEC_UUID("7F0B08D2-F593-4ea1-82BE-29AE946C1ADA")
EventWrapperProxy;
#endif

EXTERN_C const CLSID CLSID_SleepData;

#ifdef __cplusplus

class DECLSPEC_UUID("788BA2F9-3A62-43c7-A4F5-3B48E5BF0C61")
SleepData;
#endif

EXTERN_C const CLSID CLSID_AsciiWrite;

#ifdef __cplusplus

class DECLSPEC_UUID("9A0F9B69-DD00-4592-9192-43DAB88CD0E8")
AsciiWrite;
#endif

EXTERN_C const CLSID CLSID_Segments;

#ifdef __cplusplus

class DECLSPEC_UUID("A9BA61C9-829C-4b9b-B06F-771C0423047E")
Segments;
#endif

EXTERN_C const CLSID CLSID_StorageMan;

#ifdef __cplusplus

class DECLSPEC_UUID("57DDCF4E-D7BF-4446-957D-1F45B9D8CC62")
StorageMan;
#endif

EXTERN_C const CLSID CLSID_StorageManProxy;

#ifdef __cplusplus

class DECLSPEC_UUID("549EA9FA-3EC9-4325-B3B1-29039F118A2D")
StorageManProxy;
#endif

EXTERN_C const CLSID CLSID_EventWrapperCopy;

#ifdef __cplusplus

class DECLSPEC_UUID("F1E41323-8281-439c-8B23-9A7DD5A3A65F")
EventWrapperCopy;
#endif
#endif /* __FILEACCESS3Lib_LIBRARY_DEFINED__ */

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


