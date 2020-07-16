

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


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



/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 500
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif /* __RPCNDR_H_VERSION__ */

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __XLDataExportClient_h__
#define __XLDataExportClient_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IXLDataExportReceiver_FWD_DEFINED__
#define __IXLDataExportReceiver_FWD_DEFINED__
typedef interface IXLDataExportReceiver IXLDataExportReceiver;

#endif 	/* __IXLDataExportReceiver_FWD_DEFINED__ */


#ifndef ___IXLDataExportReceiverEvents_FWD_DEFINED__
#define ___IXLDataExportReceiverEvents_FWD_DEFINED__
typedef interface _IXLDataExportReceiverEvents _IXLDataExportReceiverEvents;

#endif 	/* ___IXLDataExportReceiverEvents_FWD_DEFINED__ */


#ifndef __XLDataExportReceiver_FWD_DEFINED__
#define __XLDataExportReceiver_FWD_DEFINED__

#ifdef __cplusplus
typedef class XLDataExportReceiver XLDataExportReceiver;
#else
typedef struct XLDataExportReceiver XLDataExportReceiver;
#endif /* __cplusplus */

#endif 	/* __XLDataExportReceiver_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


/* interface __MIDL_itf_XLDataExportClient_0000_0000 */
/* [local] */ 

typedef long long TStamp;

typedef long TStampLegacy;

struct XLDataExportPatientMetadata
    {
    BSTR firstName;
    BSTR lastName;
    BSTR guid;
    } ;
typedef /* [v1_enum] */ 
enum _StudyType
    {
        STUDY_EEG_TYPE	= 0,
        STUDY_SLEEP_TYPE	= ( STUDY_EEG_TYPE + 1 ) 
    } 	StudyType;

struct XLDataExportStudyMetadata
    {
    BSTR studyName;
    BSTR fileName;
    BSTR guid;
    StudyType studyType;
    long headboxType;
    long numberOfChannels;
    double samplingFreq;
    __int64 xlCreationTime;
    } ;


extern RPC_IF_HANDLE __MIDL_itf_XLDataExportClient_0000_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_XLDataExportClient_0000_0000_v0_0_s_ifspec;

#ifndef __IXLDataExportReceiver_INTERFACE_DEFINED__
#define __IXLDataExportReceiver_INTERFACE_DEFINED__

/* interface IXLDataExportReceiver */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IXLDataExportReceiver;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("18B4D726-C3C0-415A-A8F9-FC6EB680C37D")
    IXLDataExportReceiver : public IDispatch
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE StartExportData( 
            /* [in] */ struct XLDataExportPatientMetadata patient,
            /* [in] */ struct XLDataExportStudyMetadata study) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE StopExportData( void) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE PassExportData( 
            /* [in] */ TStamp samplestamp,
            /* [in] */ short numberOfChannels,
            /* [size_is][in] */ float wave_data[  ]) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IXLDataExportReceiverVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IXLDataExportReceiver * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IXLDataExportReceiver * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IXLDataExportReceiver * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *StartExportData )( 
            IXLDataExportReceiver * This,
            /* [in] */ struct XLDataExportPatientMetadata patient,
            /* [in] */ struct XLDataExportStudyMetadata study);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *StopExportData )( 
            IXLDataExportReceiver * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *PassExportData )( 
            IXLDataExportReceiver * This,
            /* [in] */ TStamp samplestamp,
            /* [in] */ short numberOfChannels,
            /* [size_is][in] */ float wave_data[  ]);
        
        END_INTERFACE
    } IXLDataExportReceiverVtbl;

    interface IXLDataExportReceiver
    {
        CONST_VTBL struct IXLDataExportReceiverVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IXLDataExportReceiver_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IXLDataExportReceiver_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IXLDataExportReceiver_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IXLDataExportReceiver_StartExportData(This,patient,study)	\
    ( (This)->lpVtbl -> StartExportData(This,patient,study) ) 

#define IXLDataExportReceiver_StopExportData(This)	\
    ( (This)->lpVtbl -> StopExportData(This) ) 

#define IXLDataExportReceiver_PassExportData(This,samplestamp,numberOfChannels,wave_data)	\
    ( (This)->lpVtbl -> PassExportData(This,samplestamp,numberOfChannels,wave_data) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IXLDataExportReceiver_INTERFACE_DEFINED__ */



#ifndef __XLDATAEXPORTCLIENTLib_LIBRARY_DEFINED__
#define __XLDATAEXPORTCLIENTLib_LIBRARY_DEFINED__

/* library XLDATAEXPORTCLIENTLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_XLDATAEXPORTCLIENTLib;

#ifndef ___IXLDataExportReceiverEvents_DISPINTERFACE_DEFINED__
#define ___IXLDataExportReceiverEvents_DISPINTERFACE_DEFINED__

/* dispinterface _IXLDataExportReceiverEvents */
/* [helpstring][uuid] */ 


EXTERN_C const IID DIID__IXLDataExportReceiverEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("D9B66E4B-899F-4400-846A-C3AD36764E7D")
    _IXLDataExportReceiverEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _IXLDataExportReceiverEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _IXLDataExportReceiverEvents * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _IXLDataExportReceiverEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _IXLDataExportReceiverEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _IXLDataExportReceiverEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _IXLDataExportReceiverEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _IXLDataExportReceiverEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _IXLDataExportReceiverEvents * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        END_INTERFACE
    } _IXLDataExportReceiverEventsVtbl;

    interface _IXLDataExportReceiverEvents
    {
        CONST_VTBL struct _IXLDataExportReceiverEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _IXLDataExportReceiverEvents_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _IXLDataExportReceiverEvents_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _IXLDataExportReceiverEvents_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _IXLDataExportReceiverEvents_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _IXLDataExportReceiverEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _IXLDataExportReceiverEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _IXLDataExportReceiverEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___IXLDataExportReceiverEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_XLDataExportReceiver;

#ifdef __cplusplus

class DECLSPEC_UUID("301D67B5-E7CA-46AD-AAEB-48FDE538CBF4")
XLDataExportReceiver;
#endif
#endif /* __XLDATAEXPORTCLIENTLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long *, unsigned long            , BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserMarshal(  unsigned long *, unsigned char *, BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserUnmarshal(unsigned long *, unsigned char *, BSTR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long *, BSTR * ); 

unsigned long             __RPC_USER  BSTR_UserSize64(     unsigned long *, unsigned long            , BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserMarshal64(  unsigned long *, unsigned char *, BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserUnmarshal64(unsigned long *, unsigned char *, BSTR * ); 
void                      __RPC_USER  BSTR_UserFree64(     unsigned long *, BSTR * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


