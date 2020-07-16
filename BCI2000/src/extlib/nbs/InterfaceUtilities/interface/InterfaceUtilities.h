

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 6.00.0361 */
/* at Thu Aug 23 14:57:27 2007
 */
/* Compiler settings for InterfaceUtilities.idl:
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

#ifndef __InterfaceUtilities_h__
#define __InterfaceUtilities_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IWaveData_FWD_DEFINED__
#define __IWaveData_FWD_DEFINED__
typedef interface IWaveData IWaveData;
#endif 	/* __IWaveData_FWD_DEFINED__ */


#ifndef __IImageData_FWD_DEFINED__
#define __IImageData_FWD_DEFINED__
typedef interface IImageData IImageData;
#endif 	/* __IImageData_FWD_DEFINED__ */


#ifndef __IVariableData_FWD_DEFINED__
#define __IVariableData_FWD_DEFINED__
typedef interface IVariableData IVariableData;
#endif 	/* __IVariableData_FWD_DEFINED__ */


#ifndef __IWaveData_FWD_DEFINED__
#define __IWaveData_FWD_DEFINED__
typedef interface IWaveData IWaveData;
#endif 	/* __IWaveData_FWD_DEFINED__ */


#ifndef __IImageData_FWD_DEFINED__
#define __IImageData_FWD_DEFINED__
typedef interface IImageData IImageData;
#endif 	/* __IImageData_FWD_DEFINED__ */


#ifndef __IVariableData_FWD_DEFINED__
#define __IVariableData_FWD_DEFINED__
typedef interface IVariableData IVariableData;
#endif 	/* __IVariableData_FWD_DEFINED__ */


/* header files for imported files */
#include "unknwn.h"
#include "oaidl.h"

#ifdef __cplusplus
extern "C"{
#endif 

void * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void * ); 

/* interface __MIDL_itf_InterfaceUtilities_0000 */
/* [local] */ 

typedef /* [uuid] */  DECLSPEC_UUID("189CF5BB-CE52-44CC-B0CD-CC9F55D01E67") struct WaveFormat
    {
    unsigned long channels;
    unsigned long bytesPerSamplePerChannel;
    unsigned long samplesPerSecond;
    } 	WaveFormat;

typedef /* [uuid] */  DECLSPEC_UUID("6E6B3CF5-2FED-4C58-B395-12A9EF742F85") struct ImageParameters
    {
    unsigned long height;
    unsigned long width;
    unsigned long pitch;
    unsigned long bitsPerPixel;
    unsigned long redMask;
    unsigned long greenMask;
    unsigned long blueMask;
    } 	ImageParameters;

typedef /* [uuid] */  DECLSPEC_UUID("18072289-32DB-4C78-80DD-A5B52788550B") 
enum tagDataType
    {	DATA_TYPE_INT32	= 0,
	DATA_TYPE_FLOAT64	= 1,
	DATA_TYPE_INT8	= 2,
	DATA_TYPE_FORCE_DWORD	= 0x7fffffff
    } 	DataType;

typedef /* [uuid] */  DECLSPEC_UUID("B05F73F2-B314-4F32-80A0-4E58E47C530B") struct Palette
    {
    unsigned long colors[ 256 ];
    } 	Palette;



extern RPC_IF_HANDLE __MIDL_itf_InterfaceUtilities_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_InterfaceUtilities_0000_v0_0_s_ifspec;

#ifndef __IWaveData_INTERFACE_DEFINED__
#define __IWaveData_INTERFACE_DEFINED__

/* interface IWaveData */
/* [object][oleautomation][dual][uuid] */ 


EXTERN_C const IID IID_IWaveData;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("901FECE5-C9A1-49C8-AF07-F5F0711614CC")
    IWaveData : public IDispatch
    {
    public:
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetFormat( 
            /* [out] */ WaveFormat *format) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetSampleCount( 
            /* [out] */ long *samples) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE DataLock( 
            /* [out] */ byte **data) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Unlock( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IWaveDataVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWaveData * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWaveData * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWaveData * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IWaveData * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IWaveData * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IWaveData * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IWaveData * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetFormat )( 
            IWaveData * This,
            /* [out] */ WaveFormat *format);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetSampleCount )( 
            IWaveData * This,
            /* [out] */ long *samples);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *DataLock )( 
            IWaveData * This,
            /* [out] */ byte **data);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Unlock )( 
            IWaveData * This);
        
        END_INTERFACE
    } IWaveDataVtbl;

    interface IWaveData
    {
        CONST_VTBL struct IWaveDataVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWaveData_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IWaveData_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IWaveData_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IWaveData_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IWaveData_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IWaveData_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IWaveData_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IWaveData_GetFormat(This,format)	\
    (This)->lpVtbl -> GetFormat(This,format)

#define IWaveData_GetSampleCount(This,samples)	\
    (This)->lpVtbl -> GetSampleCount(This,samples)

#define IWaveData_DataLock(This,data)	\
    (This)->lpVtbl -> DataLock(This,data)

#define IWaveData_Unlock(This)	\
    (This)->lpVtbl -> Unlock(This)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [id] */ HRESULT STDMETHODCALLTYPE IWaveData_GetFormat_Proxy( 
    IWaveData * This,
    /* [out] */ WaveFormat *format);


void __RPC_STUB IWaveData_GetFormat_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IWaveData_GetSampleCount_Proxy( 
    IWaveData * This,
    /* [out] */ long *samples);


void __RPC_STUB IWaveData_GetSampleCount_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IWaveData_DataLock_Proxy( 
    IWaveData * This,
    /* [out] */ byte **data);


void __RPC_STUB IWaveData_DataLock_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IWaveData_Unlock_Proxy( 
    IWaveData * This);


void __RPC_STUB IWaveData_Unlock_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IWaveData_INTERFACE_DEFINED__ */


#ifndef __IImageData_INTERFACE_DEFINED__
#define __IImageData_INTERFACE_DEFINED__

/* interface IImageData */
/* [object][oleautomation][dual][uuid] */ 


EXTERN_C const IID IID_IImageData;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("B7269D32-1584-40CD-9B60-530BE165E2DC")
    IImageData : public IDispatch
    {
    public:
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetParameters( 
            /* [out] */ ImageParameters *parameters) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE DataLock( 
            /* [out] */ byte **data) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Unlock( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IImageDataVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IImageData * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IImageData * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IImageData * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IImageData * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IImageData * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IImageData * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IImageData * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetParameters )( 
            IImageData * This,
            /* [out] */ ImageParameters *parameters);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *DataLock )( 
            IImageData * This,
            /* [out] */ byte **data);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Unlock )( 
            IImageData * This);
        
        END_INTERFACE
    } IImageDataVtbl;

    interface IImageData
    {
        CONST_VTBL struct IImageDataVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IImageData_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IImageData_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IImageData_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IImageData_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IImageData_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IImageData_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IImageData_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IImageData_GetParameters(This,parameters)	\
    (This)->lpVtbl -> GetParameters(This,parameters)

#define IImageData_DataLock(This,data)	\
    (This)->lpVtbl -> DataLock(This,data)

#define IImageData_Unlock(This)	\
    (This)->lpVtbl -> Unlock(This)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [id] */ HRESULT STDMETHODCALLTYPE IImageData_GetParameters_Proxy( 
    IImageData * This,
    /* [out] */ ImageParameters *parameters);


void __RPC_STUB IImageData_GetParameters_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IImageData_DataLock_Proxy( 
    IImageData * This,
    /* [out] */ byte **data);


void __RPC_STUB IImageData_DataLock_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IImageData_Unlock_Proxy( 
    IImageData * This);


void __RPC_STUB IImageData_Unlock_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IImageData_INTERFACE_DEFINED__ */


#ifndef __IVariableData_INTERFACE_DEFINED__
#define __IVariableData_INTERFACE_DEFINED__

/* interface IVariableData */
/* [object][oleautomation][dual][uuid] */ 


EXTERN_C const IID IID_IVariableData;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("9FE2B8D7-9FD7-4EFD-9EE6-87F3B99CB56E")
    IVariableData : public IDispatch
    {
    public:
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetType( 
            /* [out] */ DataType *type) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetDimension( 
            /* [out] */ unsigned long *dimension) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetSizes( 
            /* [out] */ unsigned long *sizes) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetData( 
            /* [in] */ unsigned long *indices,
            /* [out] */ byte **data) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IVariableDataVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IVariableData * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IVariableData * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IVariableData * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IVariableData * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IVariableData * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IVariableData * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IVariableData * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetType )( 
            IVariableData * This,
            /* [out] */ DataType *type);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetDimension )( 
            IVariableData * This,
            /* [out] */ unsigned long *dimension);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetSizes )( 
            IVariableData * This,
            /* [out] */ unsigned long *sizes);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetData )( 
            IVariableData * This,
            /* [in] */ unsigned long *indices,
            /* [out] */ byte **data);
        
        END_INTERFACE
    } IVariableDataVtbl;

    interface IVariableData
    {
        CONST_VTBL struct IVariableDataVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IVariableData_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IVariableData_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IVariableData_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IVariableData_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IVariableData_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IVariableData_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IVariableData_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IVariableData_GetType(This,type)	\
    (This)->lpVtbl -> GetType(This,type)

#define IVariableData_GetDimension(This,dimension)	\
    (This)->lpVtbl -> GetDimension(This,dimension)

#define IVariableData_GetSizes(This,sizes)	\
    (This)->lpVtbl -> GetSizes(This,sizes)

#define IVariableData_GetData(This,indices,data)	\
    (This)->lpVtbl -> GetData(This,indices,data)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [id] */ HRESULT STDMETHODCALLTYPE IVariableData_GetType_Proxy( 
    IVariableData * This,
    /* [out] */ DataType *type);


void __RPC_STUB IVariableData_GetType_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IVariableData_GetDimension_Proxy( 
    IVariableData * This,
    /* [out] */ unsigned long *dimension);


void __RPC_STUB IVariableData_GetDimension_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IVariableData_GetSizes_Proxy( 
    IVariableData * This,
    /* [out] */ unsigned long *sizes);


void __RPC_STUB IVariableData_GetSizes_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IVariableData_GetData_Proxy( 
    IVariableData * This,
    /* [in] */ unsigned long *indices,
    /* [out] */ byte **data);


void __RPC_STUB IVariableData_GetData_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IVariableData_INTERFACE_DEFINED__ */



#ifndef __InterfaceUtilities_LIBRARY_DEFINED__
#define __InterfaceUtilities_LIBRARY_DEFINED__

/* library InterfaceUtilities */
/* [uuid] */ 





EXTERN_C const IID LIBID_InterfaceUtilities;
#endif /* __InterfaceUtilities_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


