

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 6.00.0361 */
/* at Thu Feb 24 21:01:51 2011
 */
/* Compiler settings for DataPortExtension.idl:
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

#ifndef __DataPortExtension_h__
#define __DataPortExtension_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IActiveDataPort_FWD_DEFINED__
#define __IActiveDataPort_FWD_DEFINED__
typedef interface IActiveDataPort IActiveDataPort;
#endif 	/* __IActiveDataPort_FWD_DEFINED__ */


#ifndef __IDataPort_FWD_DEFINED__
#define __IDataPort_FWD_DEFINED__
typedef interface IDataPort IDataPort;
#endif 	/* __IDataPort_FWD_DEFINED__ */


#ifndef __IDataPortExtension_FWD_DEFINED__
#define __IDataPortExtension_FWD_DEFINED__
typedef interface IDataPortExtension IDataPortExtension;
#endif 	/* __IDataPortExtension_FWD_DEFINED__ */


#ifndef __IActiveDataPort_FWD_DEFINED__
#define __IActiveDataPort_FWD_DEFINED__
typedef interface IActiveDataPort IActiveDataPort;
#endif 	/* __IActiveDataPort_FWD_DEFINED__ */


#ifndef __IDataPort_FWD_DEFINED__
#define __IDataPort_FWD_DEFINED__
typedef interface IDataPort IDataPort;
#endif 	/* __IDataPort_FWD_DEFINED__ */


#ifndef __IDataPortExtension_FWD_DEFINED__
#define __IDataPortExtension_FWD_DEFINED__
typedef interface IDataPortExtension IDataPortExtension;
#endif 	/* __IDataPortExtension_FWD_DEFINED__ */


/* header files for imported files */
#include "unknwn.h"
#include "oaidl.h"

#ifdef __cplusplus
extern "C"{
#endif 

void * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void * ); 

#ifndef __IActiveDataPort_INTERFACE_DEFINED__
#define __IActiveDataPort_INTERFACE_DEFINED__

/* interface IActiveDataPort */
/* [object][oleautomation][dual][version][uuid] */ 


EXTERN_C const IID IID_IActiveDataPort;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("199E48B8-1479-4ecc-B28B-2E6500E24B51")
    IActiveDataPort : public IDispatch
    {
    public:
        virtual /* [id] */ HRESULT __stdcall getName( 
            /* [retval][out] */ BSTR *name) = 0;
        
        virtual /* [id] */ HRESULT __stdcall getServerTime( 
            /* [out] */ double *timeMicro,
            /* [out] */ double *uncertaintyMicro) = 0;
        
        virtual /* [id] */ HRESULT __stdcall sendData( 
            /* [in] */ BSTR eventType,
            /* [in] */ BSTR eventCode,
            /* [in] */ double serverTimeMicro,
            /* [in] */ double uncertaintyMicro,
            /* [in] */ BSTR other) = 0;
        
        virtual /* [id] */ HRESULT __stdcall processMessage( 
            /* [in] */ BSTR message,
            /* [retval][out] */ BSTR *result) = 0;
        
        virtual /* [id] */ HRESULT __stdcall getLastError( 
            /* [retval][out] */ BSTR *error) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IActiveDataPortVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IActiveDataPort * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IActiveDataPort * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IActiveDataPort * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IActiveDataPort * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IActiveDataPort * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IActiveDataPort * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IActiveDataPort * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [id] */ HRESULT ( __stdcall *getName )( 
            IActiveDataPort * This,
            /* [retval][out] */ BSTR *name);
        
        /* [id] */ HRESULT ( __stdcall *getServerTime )( 
            IActiveDataPort * This,
            /* [out] */ double *timeMicro,
            /* [out] */ double *uncertaintyMicro);
        
        /* [id] */ HRESULT ( __stdcall *sendData )( 
            IActiveDataPort * This,
            /* [in] */ BSTR eventType,
            /* [in] */ BSTR eventCode,
            /* [in] */ double serverTimeMicro,
            /* [in] */ double uncertaintyMicro,
            /* [in] */ BSTR other);
        
        /* [id] */ HRESULT ( __stdcall *processMessage )( 
            IActiveDataPort * This,
            /* [in] */ BSTR message,
            /* [retval][out] */ BSTR *result);
        
        /* [id] */ HRESULT ( __stdcall *getLastError )( 
            IActiveDataPort * This,
            /* [retval][out] */ BSTR *error);
        
        END_INTERFACE
    } IActiveDataPortVtbl;

    interface IActiveDataPort
    {
        CONST_VTBL struct IActiveDataPortVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IActiveDataPort_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IActiveDataPort_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IActiveDataPort_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IActiveDataPort_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IActiveDataPort_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IActiveDataPort_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IActiveDataPort_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IActiveDataPort_getName(This,name)	\
    (This)->lpVtbl -> getName(This,name)

#define IActiveDataPort_getServerTime(This,timeMicro,uncertaintyMicro)	\
    (This)->lpVtbl -> getServerTime(This,timeMicro,uncertaintyMicro)

#define IActiveDataPort_sendData(This,eventType,eventCode,serverTimeMicro,uncertaintyMicro,other)	\
    (This)->lpVtbl -> sendData(This,eventType,eventCode,serverTimeMicro,uncertaintyMicro,other)

#define IActiveDataPort_processMessage(This,message,result)	\
    (This)->lpVtbl -> processMessage(This,message,result)

#define IActiveDataPort_getLastError(This,error)	\
    (This)->lpVtbl -> getLastError(This,error)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [id] */ HRESULT __stdcall IActiveDataPort_getName_Proxy( 
    IActiveDataPort * This,
    /* [retval][out] */ BSTR *name);


void __RPC_STUB IActiveDataPort_getName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT __stdcall IActiveDataPort_getServerTime_Proxy( 
    IActiveDataPort * This,
    /* [out] */ double *timeMicro,
    /* [out] */ double *uncertaintyMicro);


void __RPC_STUB IActiveDataPort_getServerTime_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT __stdcall IActiveDataPort_sendData_Proxy( 
    IActiveDataPort * This,
    /* [in] */ BSTR eventType,
    /* [in] */ BSTR eventCode,
    /* [in] */ double serverTimeMicro,
    /* [in] */ double uncertaintyMicro,
    /* [in] */ BSTR other);


void __RPC_STUB IActiveDataPort_sendData_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT __stdcall IActiveDataPort_processMessage_Proxy( 
    IActiveDataPort * This,
    /* [in] */ BSTR message,
    /* [retval][out] */ BSTR *result);


void __RPC_STUB IActiveDataPort_processMessage_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT __stdcall IActiveDataPort_getLastError_Proxy( 
    IActiveDataPort * This,
    /* [retval][out] */ BSTR *error);


void __RPC_STUB IActiveDataPort_getLastError_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IActiveDataPort_INTERFACE_DEFINED__ */


#ifndef __IDataPort_INTERFACE_DEFINED__
#define __IDataPort_INTERFACE_DEFINED__

/* interface IDataPort */
/* [object][oleautomation][dual][version][uuid] */ 


EXTERN_C const IID IID_IDataPort;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("D8A27F2F-BC12-4206-8A34-5CB9ED9CEAE9")
    IDataPort : public IDispatch
    {
    public:
        virtual /* [id] */ HRESULT __stdcall getPropertiesViewable( 
            /* [retval][out] */ unsigned char *viewable) = 0;
        
        virtual /* [id] */ HRESULT __stdcall showProperties( 
            HWND parentWindow) = 0;
        
        virtual /* [id] */ HRESULT __stdcall setDefaults( void) = 0;
        
        virtual /* [id] */ HRESULT __stdcall getProperties( 
            /* [retval][out] */ BSTR *parameters) = 0;
        
        virtual /* [id] */ HRESULT __stdcall setProperties( 
            /* [in] */ BSTR parameters) = 0;
        
        virtual /* [id] */ HRESULT __stdcall acquire( 
            /* [retval][out] */ IActiveDataPort **device) = 0;
        
        virtual /* [id] */ HRESULT __stdcall getLastError( 
            /* [retval][out] */ BSTR *error) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDataPortVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDataPort * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDataPort * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDataPort * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDataPort * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDataPort * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDataPort * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDataPort * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [id] */ HRESULT ( __stdcall *getPropertiesViewable )( 
            IDataPort * This,
            /* [retval][out] */ unsigned char *viewable);
        
        /* [id] */ HRESULT ( __stdcall *showProperties )( 
            IDataPort * This,
            HWND parentWindow);
        
        /* [id] */ HRESULT ( __stdcall *setDefaults )( 
            IDataPort * This);
        
        /* [id] */ HRESULT ( __stdcall *getProperties )( 
            IDataPort * This,
            /* [retval][out] */ BSTR *parameters);
        
        /* [id] */ HRESULT ( __stdcall *setProperties )( 
            IDataPort * This,
            /* [in] */ BSTR parameters);
        
        /* [id] */ HRESULT ( __stdcall *acquire )( 
            IDataPort * This,
            /* [retval][out] */ IActiveDataPort **device);
        
        /* [id] */ HRESULT ( __stdcall *getLastError )( 
            IDataPort * This,
            /* [retval][out] */ BSTR *error);
        
        END_INTERFACE
    } IDataPortVtbl;

    interface IDataPort
    {
        CONST_VTBL struct IDataPortVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDataPort_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IDataPort_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IDataPort_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IDataPort_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IDataPort_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IDataPort_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IDataPort_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IDataPort_getPropertiesViewable(This,viewable)	\
    (This)->lpVtbl -> getPropertiesViewable(This,viewable)

#define IDataPort_showProperties(This,parentWindow)	\
    (This)->lpVtbl -> showProperties(This,parentWindow)

#define IDataPort_setDefaults(This)	\
    (This)->lpVtbl -> setDefaults(This)

#define IDataPort_getProperties(This,parameters)	\
    (This)->lpVtbl -> getProperties(This,parameters)

#define IDataPort_setProperties(This,parameters)	\
    (This)->lpVtbl -> setProperties(This,parameters)

#define IDataPort_acquire(This,device)	\
    (This)->lpVtbl -> acquire(This,device)

#define IDataPort_getLastError(This,error)	\
    (This)->lpVtbl -> getLastError(This,error)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [id] */ HRESULT __stdcall IDataPort_getPropertiesViewable_Proxy( 
    IDataPort * This,
    /* [retval][out] */ unsigned char *viewable);


void __RPC_STUB IDataPort_getPropertiesViewable_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT __stdcall IDataPort_showProperties_Proxy( 
    IDataPort * This,
    HWND parentWindow);


void __RPC_STUB IDataPort_showProperties_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT __stdcall IDataPort_setDefaults_Proxy( 
    IDataPort * This);


void __RPC_STUB IDataPort_setDefaults_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT __stdcall IDataPort_getProperties_Proxy( 
    IDataPort * This,
    /* [retval][out] */ BSTR *parameters);


void __RPC_STUB IDataPort_getProperties_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT __stdcall IDataPort_setProperties_Proxy( 
    IDataPort * This,
    /* [in] */ BSTR parameters);


void __RPC_STUB IDataPort_setProperties_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT __stdcall IDataPort_acquire_Proxy( 
    IDataPort * This,
    /* [retval][out] */ IActiveDataPort **device);


void __RPC_STUB IDataPort_acquire_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT __stdcall IDataPort_getLastError_Proxy( 
    IDataPort * This,
    /* [retval][out] */ BSTR *error);


void __RPC_STUB IDataPort_getLastError_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IDataPort_INTERFACE_DEFINED__ */


#ifndef __IDataPortExtension_INTERFACE_DEFINED__
#define __IDataPortExtension_INTERFACE_DEFINED__

/* interface IDataPortExtension */
/* [object][oleautomation][dual][version][uuid] */ 


EXTERN_C const IID IID_IDataPortExtension;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("5318AE38-6B92-4406-9178-51EAA811F9C5")
    IDataPortExtension : public IDispatch
    {
    public:
        virtual /* [id] */ HRESULT __stdcall getName( 
            /* [retval][out] */ BSTR *name) = 0;
        
        virtual /* [id] */ HRESULT __stdcall getAuthor( 
            /* [retval][out] */ BSTR *author) = 0;
        
        virtual /* [id] */ HRESULT __stdcall getVersion( 
            /* [retval][out] */ BSTR *ver) = 0;
        
        virtual /* [id] */ HRESULT __stdcall getDescription( 
            /* [retval][out] */ BSTR *desc) = 0;
        
        virtual /* [id] */ HRESULT __stdcall addPort( 
            /* [retval][out] */ IDataPort **port) = 0;
        
        virtual /* [id] */ HRESULT __stdcall getLastError( 
            /* [retval][out] */ BSTR *error) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDataPortExtensionVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDataPortExtension * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDataPortExtension * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDataPortExtension * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDataPortExtension * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDataPortExtension * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDataPortExtension * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDataPortExtension * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [id] */ HRESULT ( __stdcall *getName )( 
            IDataPortExtension * This,
            /* [retval][out] */ BSTR *name);
        
        /* [id] */ HRESULT ( __stdcall *getAuthor )( 
            IDataPortExtension * This,
            /* [retval][out] */ BSTR *author);
        
        /* [id] */ HRESULT ( __stdcall *getVersion )( 
            IDataPortExtension * This,
            /* [retval][out] */ BSTR *ver);
        
        /* [id] */ HRESULT ( __stdcall *getDescription )( 
            IDataPortExtension * This,
            /* [retval][out] */ BSTR *desc);
        
        /* [id] */ HRESULT ( __stdcall *addPort )( 
            IDataPortExtension * This,
            /* [retval][out] */ IDataPort **port);
        
        /* [id] */ HRESULT ( __stdcall *getLastError )( 
            IDataPortExtension * This,
            /* [retval][out] */ BSTR *error);
        
        END_INTERFACE
    } IDataPortExtensionVtbl;

    interface IDataPortExtension
    {
        CONST_VTBL struct IDataPortExtensionVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDataPortExtension_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IDataPortExtension_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IDataPortExtension_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IDataPortExtension_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IDataPortExtension_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IDataPortExtension_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IDataPortExtension_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IDataPortExtension_getName(This,name)	\
    (This)->lpVtbl -> getName(This,name)

#define IDataPortExtension_getAuthor(This,author)	\
    (This)->lpVtbl -> getAuthor(This,author)

#define IDataPortExtension_getVersion(This,ver)	\
    (This)->lpVtbl -> getVersion(This,ver)

#define IDataPortExtension_getDescription(This,desc)	\
    (This)->lpVtbl -> getDescription(This,desc)

#define IDataPortExtension_addPort(This,port)	\
    (This)->lpVtbl -> addPort(This,port)

#define IDataPortExtension_getLastError(This,error)	\
    (This)->lpVtbl -> getLastError(This,error)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [id] */ HRESULT __stdcall IDataPortExtension_getName_Proxy( 
    IDataPortExtension * This,
    /* [retval][out] */ BSTR *name);


void __RPC_STUB IDataPortExtension_getName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT __stdcall IDataPortExtension_getAuthor_Proxy( 
    IDataPortExtension * This,
    /* [retval][out] */ BSTR *author);


void __RPC_STUB IDataPortExtension_getAuthor_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT __stdcall IDataPortExtension_getVersion_Proxy( 
    IDataPortExtension * This,
    /* [retval][out] */ BSTR *ver);


void __RPC_STUB IDataPortExtension_getVersion_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT __stdcall IDataPortExtension_getDescription_Proxy( 
    IDataPortExtension * This,
    /* [retval][out] */ BSTR *desc);


void __RPC_STUB IDataPortExtension_getDescription_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT __stdcall IDataPortExtension_addPort_Proxy( 
    IDataPortExtension * This,
    /* [retval][out] */ IDataPort **port);


void __RPC_STUB IDataPortExtension_addPort_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT __stdcall IDataPortExtension_getLastError_Proxy( 
    IDataPortExtension * This,
    /* [retval][out] */ BSTR *error);


void __RPC_STUB IDataPortExtension_getLastError_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IDataPortExtension_INTERFACE_DEFINED__ */



#ifndef __DataPortExtensionLibrary_LIBRARY_DEFINED__
#define __DataPortExtensionLibrary_LIBRARY_DEFINED__

/* library DataPortExtensionLibrary */
/* [uuid] */ 





EXTERN_C const IID LIBID_DataPortExtensionLibrary;
#endif /* __DataPortExtensionLibrary_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long *, unsigned long            , BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserMarshal(  unsigned long *, unsigned char *, BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserUnmarshal(unsigned long *, unsigned char *, BSTR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long *, BSTR * ); 

unsigned long             __RPC_USER  HWND_UserSize(     unsigned long *, unsigned long            , HWND * ); 
unsigned char * __RPC_USER  HWND_UserMarshal(  unsigned long *, unsigned char *, HWND * ); 
unsigned char * __RPC_USER  HWND_UserUnmarshal(unsigned long *, unsigned char *, HWND * ); 
void                      __RPC_USER  HWND_UserFree(     unsigned long *, HWND * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


