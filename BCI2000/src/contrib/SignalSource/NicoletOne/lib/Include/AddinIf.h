
#pragma warning( disable: 4049 )  /* more than 64k source lines */

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 6.00.0347 */
/* at Tue Nov 11 18:06:07 2003
 */
/* Compiler settings for C:\development\NERVUS 3.1\Include\AddInIF.idl:
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

#ifndef __AddInIF_h__
#define __AddInIF_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IProcess_FWD_DEFINED__
#define __IProcess_FWD_DEFINED__
typedef interface IProcess IProcess;
#endif 	/* __IProcess_FWD_DEFINED__ */


#ifndef __INodeSetup_FWD_DEFINED__
#define __INodeSetup_FWD_DEFINED__
typedef interface INodeSetup INodeSetup;
#endif 	/* __INodeSetup_FWD_DEFINED__ */


#ifndef __INBroadcaster_FWD_DEFINED__
#define __INBroadcaster_FWD_DEFINED__
typedef interface INBroadcaster INBroadcaster;
#endif 	/* __INBroadcaster_FWD_DEFINED__ */


#ifndef __INSubscriber_FWD_DEFINED__
#define __INSubscriber_FWD_DEFINED__
typedef interface INSubscriber INSubscriber;
#endif 	/* __INSubscriber_FWD_DEFINED__ */


#ifndef __INrvEvents3_FWD_DEFINED__
#define __INrvEvents3_FWD_DEFINED__
typedef interface INrvEvents3 INrvEvents3;
#endif 	/* __INrvEvents3_FWD_DEFINED__ */


#ifndef __INrvData3_FWD_DEFINED__
#define __INrvData3_FWD_DEFINED__
typedef interface INrvData3 INrvData3;
#endif 	/* __INrvData3_FWD_DEFINED__ */


#ifndef __INrvSegments3_FWD_DEFINED__
#define __INrvSegments3_FWD_DEFINED__
typedef interface INrvSegments3 INrvSegments3;
#endif 	/* __INrvSegments3_FWD_DEFINED__ */


#ifndef __IPanelItem_FWD_DEFINED__
#define __IPanelItem_FWD_DEFINED__
typedef interface IPanelItem IPanelItem;
#endif 	/* __IPanelItem_FWD_DEFINED__ */


#ifndef __IReport_FWD_DEFINED__
#define __IReport_FWD_DEFINED__
typedef interface IReport IReport;
#endif 	/* __IReport_FWD_DEFINED__ */


#ifndef __IReportGenerator_FWD_DEFINED__
#define __IReportGenerator_FWD_DEFINED__
typedef interface IReportGenerator IReportGenerator;
#endif 	/* __IReportGenerator_FWD_DEFINED__ */


#ifndef __ICalcNode_FWD_DEFINED__
#define __ICalcNode_FWD_DEFINED__
typedef interface ICalcNode ICalcNode;
#endif 	/* __ICalcNode_FWD_DEFINED__ */


#ifndef __IAcquisitionTime_FWD_DEFINED__
#define __IAcquisitionTime_FWD_DEFINED__
typedef interface IAcquisitionTime IAcquisitionTime;
#endif 	/* __IAcquisitionTime_FWD_DEFINED__ */


#ifndef __IDetectionsUI_FWD_DEFINED__
#define __IDetectionsUI_FWD_DEFINED__
typedef interface IDetectionsUI IDetectionsUI;
#endif 	/* __IDetectionsUI_FWD_DEFINED__ */


#ifndef __IAmplifier_FWD_DEFINED__
#define __IAmplifier_FWD_DEFINED__
typedef interface IAmplifier IAmplifier;
#endif 	/* __IAmplifier_FWD_DEFINED__ */


#ifndef __IRulerSlave_FWD_DEFINED__
#define __IRulerSlave_FWD_DEFINED__
typedef interface IRulerSlave IRulerSlave;
#endif 	/* __IRulerSlave_FWD_DEFINED__ */


#ifndef __IEventNotify_FWD_DEFINED__
#define __IEventNotify_FWD_DEFINED__
typedef interface IEventNotify IEventNotify;
#endif 	/* __IEventNotify_FWD_DEFINED__ */


#ifndef __IEventFilter_FWD_DEFINED__
#define __IEventFilter_FWD_DEFINED__
typedef interface IEventFilter IEventFilter;
#endif 	/* __IEventFilter_FWD_DEFINED__ */


#ifndef __IProtocolClient_FWD_DEFINED__
#define __IProtocolClient_FWD_DEFINED__
typedef interface IProtocolClient IProtocolClient;
#endif 	/* __IProtocolClient_FWD_DEFINED__ */


#ifndef __IQuerySize_FWD_DEFINED__
#define __IQuerySize_FWD_DEFINED__
typedef interface IQuerySize IQuerySize;
#endif 	/* __IQuerySize_FWD_DEFINED__ */


#ifndef __IEditor_FWD_DEFINED__
#define __IEditor_FWD_DEFINED__
typedef interface IEditor IEditor;
#endif 	/* __IEditor_FWD_DEFINED__ */


#ifndef __IProgressSinkOld_FWD_DEFINED__
#define __IProgressSinkOld_FWD_DEFINED__
typedef interface IProgressSinkOld IProgressSinkOld;
#endif 	/* __IProgressSinkOld_FWD_DEFINED__ */


#ifndef __IProgressSink_FWD_DEFINED__
#define __IProgressSink_FWD_DEFINED__
typedef interface IProgressSink IProgressSink;
#endif 	/* __IProgressSink_FWD_DEFINED__ */


#ifndef __IPagerNotify_FWD_DEFINED__
#define __IPagerNotify_FWD_DEFINED__
typedef interface IPagerNotify IPagerNotify;
#endif 	/* __IPagerNotify_FWD_DEFINED__ */


#ifndef __IStorageObserver_FWD_DEFINED__
#define __IStorageObserver_FWD_DEFINED__
typedef interface IStorageObserver IStorageObserver;
#endif 	/* __IStorageObserver_FWD_DEFINED__ */


#ifndef __IDetectionNode_FWD_DEFINED__
#define __IDetectionNode_FWD_DEFINED__
typedef interface IDetectionNode IDetectionNode;
#endif 	/* __IDetectionNode_FWD_DEFINED__ */


#ifndef __IConnectNotify_FWD_DEFINED__
#define __IConnectNotify_FWD_DEFINED__
typedef interface IConnectNotify IConnectNotify;
#endif 	/* __IConnectNotify_FWD_DEFINED__ */


#ifndef __ICacheObserver_FWD_DEFINED__
#define __ICacheObserver_FWD_DEFINED__
typedef interface ICacheObserver ICacheObserver;
#endif 	/* __ICacheObserver_FWD_DEFINED__ */


#ifndef __IEventHeight_FWD_DEFINED__
#define __IEventHeight_FWD_DEFINED__
typedef interface IEventHeight IEventHeight;
#endif 	/* __IEventHeight_FWD_DEFINED__ */


#ifndef __IAlarmObserver_FWD_DEFINED__
#define __IAlarmObserver_FWD_DEFINED__
typedef interface IAlarmObserver IAlarmObserver;
#endif 	/* __IAlarmObserver_FWD_DEFINED__ */


#ifndef __ITrendExport2_FWD_DEFINED__
#define __ITrendExport2_FWD_DEFINED__
typedef interface ITrendExport2 ITrendExport2;
#endif 	/* __ITrendExport2_FWD_DEFINED__ */


#ifndef __IAlarmLogics_FWD_DEFINED__
#define __IAlarmLogics_FWD_DEFINED__
typedef interface IAlarmLogics IAlarmLogics;
#endif 	/* __IAlarmLogics_FWD_DEFINED__ */


#ifndef __ISleepNotifySubscriber_FWD_DEFINED__
#define __ISleepNotifySubscriber_FWD_DEFINED__
typedef interface ISleepNotifySubscriber ISleepNotifySubscriber;
#endif 	/* __ISleepNotifySubscriber_FWD_DEFINED__ */


#ifndef __IConfigRegistryNotify_FWD_DEFINED__
#define __IConfigRegistryNotify_FWD_DEFINED__
typedef interface IConfigRegistryNotify IConfigRegistryNotify;
#endif 	/* __IConfigRegistryNotify_FWD_DEFINED__ */


#ifndef __IDoniCallback2_FWD_DEFINED__
#define __IDoniCallback2_FWD_DEFINED__
typedef interface IDoniCallback2 IDoniCallback2;
#endif 	/* __IDoniCallback2_FWD_DEFINED__ */


#ifndef __IProcess_FWD_DEFINED__
#define __IProcess_FWD_DEFINED__
typedef interface IProcess IProcess;
#endif 	/* __IProcess_FWD_DEFINED__ */


#ifndef __INodeSetup_FWD_DEFINED__
#define __INodeSetup_FWD_DEFINED__
typedef interface INodeSetup INodeSetup;
#endif 	/* __INodeSetup_FWD_DEFINED__ */


#ifndef __INSubscriber_FWD_DEFINED__
#define __INSubscriber_FWD_DEFINED__
typedef interface INSubscriber INSubscriber;
#endif 	/* __INSubscriber_FWD_DEFINED__ */


#ifndef __INrvEvents3_FWD_DEFINED__
#define __INrvEvents3_FWD_DEFINED__
typedef interface INrvEvents3 INrvEvents3;
#endif 	/* __INrvEvents3_FWD_DEFINED__ */


#ifndef __INrvData3_FWD_DEFINED__
#define __INrvData3_FWD_DEFINED__
typedef interface INrvData3 INrvData3;
#endif 	/* __INrvData3_FWD_DEFINED__ */


#ifndef __INrvSegments3_FWD_DEFINED__
#define __INrvSegments3_FWD_DEFINED__
typedef interface INrvSegments3 INrvSegments3;
#endif 	/* __INrvSegments3_FWD_DEFINED__ */


#ifndef __IPanelItem_FWD_DEFINED__
#define __IPanelItem_FWD_DEFINED__
typedef interface IPanelItem IPanelItem;
#endif 	/* __IPanelItem_FWD_DEFINED__ */


#ifndef __IReport_FWD_DEFINED__
#define __IReport_FWD_DEFINED__
typedef interface IReport IReport;
#endif 	/* __IReport_FWD_DEFINED__ */


#ifndef __IReportGenerator_FWD_DEFINED__
#define __IReportGenerator_FWD_DEFINED__
typedef interface IReportGenerator IReportGenerator;
#endif 	/* __IReportGenerator_FWD_DEFINED__ */


#ifndef __ICalcNode_FWD_DEFINED__
#define __ICalcNode_FWD_DEFINED__
typedef interface ICalcNode ICalcNode;
#endif 	/* __ICalcNode_FWD_DEFINED__ */


#ifndef __IDetectionsUI_FWD_DEFINED__
#define __IDetectionsUI_FWD_DEFINED__
typedef interface IDetectionsUI IDetectionsUI;
#endif 	/* __IDetectionsUI_FWD_DEFINED__ */


#ifndef __IAcquisitionTime_FWD_DEFINED__
#define __IAcquisitionTime_FWD_DEFINED__
typedef interface IAcquisitionTime IAcquisitionTime;
#endif 	/* __IAcquisitionTime_FWD_DEFINED__ */


#ifndef __IAmplifier_FWD_DEFINED__
#define __IAmplifier_FWD_DEFINED__
typedef interface IAmplifier IAmplifier;
#endif 	/* __IAmplifier_FWD_DEFINED__ */


#ifndef __IRulerSlave_FWD_DEFINED__
#define __IRulerSlave_FWD_DEFINED__
typedef interface IRulerSlave IRulerSlave;
#endif 	/* __IRulerSlave_FWD_DEFINED__ */


#ifndef __IEventNotify_FWD_DEFINED__
#define __IEventNotify_FWD_DEFINED__
typedef interface IEventNotify IEventNotify;
#endif 	/* __IEventNotify_FWD_DEFINED__ */


#ifndef __IProtocolClient_FWD_DEFINED__
#define __IProtocolClient_FWD_DEFINED__
typedef interface IProtocolClient IProtocolClient;
#endif 	/* __IProtocolClient_FWD_DEFINED__ */


#ifndef __IQuerySize_FWD_DEFINED__
#define __IQuerySize_FWD_DEFINED__
typedef interface IQuerySize IQuerySize;
#endif 	/* __IQuerySize_FWD_DEFINED__ */


#ifndef __IEditor_FWD_DEFINED__
#define __IEditor_FWD_DEFINED__
typedef interface IEditor IEditor;
#endif 	/* __IEditor_FWD_DEFINED__ */


#ifndef __IProgressSinkOld_FWD_DEFINED__
#define __IProgressSinkOld_FWD_DEFINED__
typedef interface IProgressSinkOld IProgressSinkOld;
#endif 	/* __IProgressSinkOld_FWD_DEFINED__ */


#ifndef __IProgressSink_FWD_DEFINED__
#define __IProgressSink_FWD_DEFINED__
typedef interface IProgressSink IProgressSink;
#endif 	/* __IProgressSink_FWD_DEFINED__ */


#ifndef __IEventFilter_FWD_DEFINED__
#define __IEventFilter_FWD_DEFINED__
typedef interface IEventFilter IEventFilter;
#endif 	/* __IEventFilter_FWD_DEFINED__ */


#ifndef __IPagerNotify_FWD_DEFINED__
#define __IPagerNotify_FWD_DEFINED__
typedef interface IPagerNotify IPagerNotify;
#endif 	/* __IPagerNotify_FWD_DEFINED__ */


#ifndef __IStorageObserver_FWD_DEFINED__
#define __IStorageObserver_FWD_DEFINED__
typedef interface IStorageObserver IStorageObserver;
#endif 	/* __IStorageObserver_FWD_DEFINED__ */


#ifndef __IDetectionNode_FWD_DEFINED__
#define __IDetectionNode_FWD_DEFINED__
typedef interface IDetectionNode IDetectionNode;
#endif 	/* __IDetectionNode_FWD_DEFINED__ */


#ifndef __INBroadcaster_FWD_DEFINED__
#define __INBroadcaster_FWD_DEFINED__
typedef interface INBroadcaster INBroadcaster;
#endif 	/* __INBroadcaster_FWD_DEFINED__ */


#ifndef __IConnectNotify_FWD_DEFINED__
#define __IConnectNotify_FWD_DEFINED__
typedef interface IConnectNotify IConnectNotify;
#endif 	/* __IConnectNotify_FWD_DEFINED__ */


#ifndef __ICacheObserver_FWD_DEFINED__
#define __ICacheObserver_FWD_DEFINED__
typedef interface ICacheObserver ICacheObserver;
#endif 	/* __ICacheObserver_FWD_DEFINED__ */


#ifndef __IEventHeight_FWD_DEFINED__
#define __IEventHeight_FWD_DEFINED__
typedef interface IEventHeight IEventHeight;
#endif 	/* __IEventHeight_FWD_DEFINED__ */


#ifndef __IAlarmObserver_FWD_DEFINED__
#define __IAlarmObserver_FWD_DEFINED__
typedef interface IAlarmObserver IAlarmObserver;
#endif 	/* __IAlarmObserver_FWD_DEFINED__ */


#ifndef __ITrendExport2_FWD_DEFINED__
#define __ITrendExport2_FWD_DEFINED__
typedef interface ITrendExport2 ITrendExport2;
#endif 	/* __ITrendExport2_FWD_DEFINED__ */


#ifndef __IAlarmLogics_FWD_DEFINED__
#define __IAlarmLogics_FWD_DEFINED__
typedef interface IAlarmLogics IAlarmLogics;
#endif 	/* __IAlarmLogics_FWD_DEFINED__ */


#ifndef __ISleepNotifySubscriber_FWD_DEFINED__
#define __ISleepNotifySubscriber_FWD_DEFINED__
typedef interface ISleepNotifySubscriber ISleepNotifySubscriber;
#endif 	/* __ISleepNotifySubscriber_FWD_DEFINED__ */


#ifndef __IDoniCallback2_FWD_DEFINED__
#define __IDoniCallback2_FWD_DEFINED__
typedef interface IDoniCallback2 IDoniCallback2;
#endif 	/* __IDoniCallback2_FWD_DEFINED__ */


#ifndef ___IBannerEvents_FWD_DEFINED__
#define ___IBannerEvents_FWD_DEFINED__
typedef interface _IBannerEvents _IBannerEvents;
#endif 	/* ___IBannerEvents_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"
#include "PacketDef.h"
#include "NrvDate.h"

#ifdef __cplusplus
extern "C"{
#endif 

void * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void * ); 

#ifndef __IProcess_INTERFACE_DEFINED__
#define __IProcess_INTERFACE_DEFINED__

/* interface IProcess */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IProcess;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("BF6AF2A0-53C5-11D2-A86C-080009D8F187")
    IProcess : public IUnknown
    {
    public:
        virtual /* [helpstring][local] */ HRESULT STDMETHODCALLTYPE ProcessData( 
            /* [in] */ const PACKET pPacket) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetName( 
            /* [retval][out] */ BSTR *pbstrName) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetNodeInfo( 
            /* [retval][out] */ BSTR *pbstrInfo) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IProcessVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IProcess * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IProcess * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IProcess * This);
        
        /* [helpstring][local] */ HRESULT ( STDMETHODCALLTYPE *ProcessData )( 
            IProcess * This,
            /* [in] */ const PACKET pPacket);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetName )( 
            IProcess * This,
            /* [retval][out] */ BSTR *pbstrName);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetNodeInfo )( 
            IProcess * This,
            /* [retval][out] */ BSTR *pbstrInfo);
        
        END_INTERFACE
    } IProcessVtbl;

    interface IProcess
    {
        CONST_VTBL struct IProcessVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IProcess_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IProcess_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IProcess_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IProcess_ProcessData(This,pPacket)	\
    (This)->lpVtbl -> ProcessData(This,pPacket)

#define IProcess_GetName(This,pbstrName)	\
    (This)->lpVtbl -> GetName(This,pbstrName)

#define IProcess_GetNodeInfo(This,pbstrInfo)	\
    (This)->lpVtbl -> GetNodeInfo(This,pbstrInfo)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][local] */ HRESULT STDMETHODCALLTYPE IProcess_ProcessData_Proxy( 
    IProcess * This,
    /* [in] */ const PACKET pPacket);


void __RPC_STUB IProcess_ProcessData_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IProcess_GetName_Proxy( 
    IProcess * This,
    /* [retval][out] */ BSTR *pbstrName);


void __RPC_STUB IProcess_GetName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IProcess_GetNodeInfo_Proxy( 
    IProcess * This,
    /* [retval][out] */ BSTR *pbstrInfo);


void __RPC_STUB IProcess_GetNodeInfo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IProcess_INTERFACE_DEFINED__ */


#ifndef __INodeSetup_INTERFACE_DEFINED__
#define __INodeSetup_INTERFACE_DEFINED__

/* interface INodeSetup */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_INodeSetup;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("28F82D41-77CC-11d2-9931-00104B6F4F70")
    INodeSetup : public IUnknown
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE SetDataReceiver( 
            /* [in] */ IProcess *pProcess) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE SetName( 
            /* [in] */ BSTR bstrName) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetName( 
            /* [retval][out] */ BSTR *pbstrName) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE ReleaseNode( 
            /* [in] */ BSTR bstrNode) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE ReleaseAllNodes( void) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE ResetNode( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct INodeSetupVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            INodeSetup * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            INodeSetup * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            INodeSetup * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetDataReceiver )( 
            INodeSetup * This,
            /* [in] */ IProcess *pProcess);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetName )( 
            INodeSetup * This,
            /* [in] */ BSTR bstrName);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetName )( 
            INodeSetup * This,
            /* [retval][out] */ BSTR *pbstrName);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *ReleaseNode )( 
            INodeSetup * This,
            /* [in] */ BSTR bstrNode);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *ReleaseAllNodes )( 
            INodeSetup * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *ResetNode )( 
            INodeSetup * This);
        
        END_INTERFACE
    } INodeSetupVtbl;

    interface INodeSetup
    {
        CONST_VTBL struct INodeSetupVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define INodeSetup_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define INodeSetup_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define INodeSetup_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define INodeSetup_SetDataReceiver(This,pProcess)	\
    (This)->lpVtbl -> SetDataReceiver(This,pProcess)

#define INodeSetup_SetName(This,bstrName)	\
    (This)->lpVtbl -> SetName(This,bstrName)

#define INodeSetup_GetName(This,pbstrName)	\
    (This)->lpVtbl -> GetName(This,pbstrName)

#define INodeSetup_ReleaseNode(This,bstrNode)	\
    (This)->lpVtbl -> ReleaseNode(This,bstrNode)

#define INodeSetup_ReleaseAllNodes(This)	\
    (This)->lpVtbl -> ReleaseAllNodes(This)

#define INodeSetup_ResetNode(This)	\
    (This)->lpVtbl -> ResetNode(This)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring] */ HRESULT STDMETHODCALLTYPE INodeSetup_SetDataReceiver_Proxy( 
    INodeSetup * This,
    /* [in] */ IProcess *pProcess);


void __RPC_STUB INodeSetup_SetDataReceiver_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE INodeSetup_SetName_Proxy( 
    INodeSetup * This,
    /* [in] */ BSTR bstrName);


void __RPC_STUB INodeSetup_SetName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE INodeSetup_GetName_Proxy( 
    INodeSetup * This,
    /* [retval][out] */ BSTR *pbstrName);


void __RPC_STUB INodeSetup_GetName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE INodeSetup_ReleaseNode_Proxy( 
    INodeSetup * This,
    /* [in] */ BSTR bstrNode);


void __RPC_STUB INodeSetup_ReleaseNode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE INodeSetup_ReleaseAllNodes_Proxy( 
    INodeSetup * This);


void __RPC_STUB INodeSetup_ReleaseAllNodes_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE INodeSetup_ResetNode_Proxy( 
    INodeSetup * This);


void __RPC_STUB INodeSetup_ResetNode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __INodeSetup_INTERFACE_DEFINED__ */


#ifndef __INBroadcaster_INTERFACE_DEFINED__
#define __INBroadcaster_INTERFACE_DEFINED__

/* interface INBroadcaster */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_INBroadcaster;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("2C051D54-88EE-4776-8B00-169492824CB3")
    INBroadcaster : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE AddSubscriber( 
            /* [in] */ IDispatch *pISubscriber) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE RemoveSubscriber( 
            /* [in] */ IDispatch *pISubscriber) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct INBroadcasterVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            INBroadcaster * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            INBroadcaster * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            INBroadcaster * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            INBroadcaster * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            INBroadcaster * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            INBroadcaster * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            INBroadcaster * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *AddSubscriber )( 
            INBroadcaster * This,
            /* [in] */ IDispatch *pISubscriber);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *RemoveSubscriber )( 
            INBroadcaster * This,
            /* [in] */ IDispatch *pISubscriber);
        
        END_INTERFACE
    } INBroadcasterVtbl;

    interface INBroadcaster
    {
        CONST_VTBL struct INBroadcasterVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define INBroadcaster_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define INBroadcaster_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define INBroadcaster_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define INBroadcaster_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define INBroadcaster_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define INBroadcaster_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define INBroadcaster_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define INBroadcaster_AddSubscriber(This,pISubscriber)	\
    (This)->lpVtbl -> AddSubscriber(This,pISubscriber)

#define INBroadcaster_RemoveSubscriber(This,pISubscriber)	\
    (This)->lpVtbl -> RemoveSubscriber(This,pISubscriber)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE INBroadcaster_AddSubscriber_Proxy( 
    INBroadcaster * This,
    /* [in] */ IDispatch *pISubscriber);


void __RPC_STUB INBroadcaster_AddSubscriber_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE INBroadcaster_RemoveSubscriber_Proxy( 
    INBroadcaster * This,
    /* [in] */ IDispatch *pISubscriber);


void __RPC_STUB INBroadcaster_RemoveSubscriber_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __INBroadcaster_INTERFACE_DEFINED__ */


#ifndef __INSubscriber_INTERFACE_DEFINED__
#define __INSubscriber_INTERFACE_DEFINED__

/* interface INSubscriber */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_INSubscriber;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("2B60B9C4-7D30-4545-9A9A-62A426A7C133")
    INSubscriber : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE NotifySubscriber( 
            /* [in] */ BSTR bstrNotify,
            /* [in] */ VARIANT varArgument) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct INSubscriberVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            INSubscriber * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            INSubscriber * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            INSubscriber * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            INSubscriber * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            INSubscriber * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            INSubscriber * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            INSubscriber * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *NotifySubscriber )( 
            INSubscriber * This,
            /* [in] */ BSTR bstrNotify,
            /* [in] */ VARIANT varArgument);
        
        END_INTERFACE
    } INSubscriberVtbl;

    interface INSubscriber
    {
        CONST_VTBL struct INSubscriberVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define INSubscriber_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define INSubscriber_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define INSubscriber_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define INSubscriber_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define INSubscriber_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define INSubscriber_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define INSubscriber_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define INSubscriber_NotifySubscriber(This,bstrNotify,varArgument)	\
    (This)->lpVtbl -> NotifySubscriber(This,bstrNotify,varArgument)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE INSubscriber_NotifySubscriber_Proxy( 
    INSubscriber * This,
    /* [in] */ BSTR bstrNotify,
    /* [in] */ VARIANT varArgument);


void __RPC_STUB INSubscriber_NotifySubscriber_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __INSubscriber_INTERFACE_DEFINED__ */


#ifndef __INrvEvents3_INTERFACE_DEFINED__
#define __INrvEvents3_INTERFACE_DEFINED__

/* interface INrvEvents3 */
/* [unique][helpstring][uuid][local][object] */ 


EXTERN_C const IID IID_INrvEvents3;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("8888216B-533D-4edf-8276-4B7019E7AA37")
    INrvEvents3 : public IUnknown
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE LoadEvent( 
            /* [in] */ PACKET pPacket) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetEventCount( 
            /* [retval][out] */ long *plcEvents) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetEvent( 
            /* [in] */ long liEvent,
            /* [out] */ PACKET *ppPacket) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct INrvEvents3Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            INrvEvents3 * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            INrvEvents3 * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            INrvEvents3 * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *LoadEvent )( 
            INrvEvents3 * This,
            /* [in] */ PACKET pPacket);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetEventCount )( 
            INrvEvents3 * This,
            /* [retval][out] */ long *plcEvents);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetEvent )( 
            INrvEvents3 * This,
            /* [in] */ long liEvent,
            /* [out] */ PACKET *ppPacket);
        
        END_INTERFACE
    } INrvEvents3Vtbl;

    interface INrvEvents3
    {
        CONST_VTBL struct INrvEvents3Vtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define INrvEvents3_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define INrvEvents3_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define INrvEvents3_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define INrvEvents3_LoadEvent(This,pPacket)	\
    (This)->lpVtbl -> LoadEvent(This,pPacket)

#define INrvEvents3_GetEventCount(This,plcEvents)	\
    (This)->lpVtbl -> GetEventCount(This,plcEvents)

#define INrvEvents3_GetEvent(This,liEvent,ppPacket)	\
    (This)->lpVtbl -> GetEvent(This,liEvent,ppPacket)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring] */ HRESULT STDMETHODCALLTYPE INrvEvents3_LoadEvent_Proxy( 
    INrvEvents3 * This,
    /* [in] */ PACKET pPacket);


void __RPC_STUB INrvEvents3_LoadEvent_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE INrvEvents3_GetEventCount_Proxy( 
    INrvEvents3 * This,
    /* [retval][out] */ long *plcEvents);


void __RPC_STUB INrvEvents3_GetEventCount_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE INrvEvents3_GetEvent_Proxy( 
    INrvEvents3 * This,
    /* [in] */ long liEvent,
    /* [out] */ PACKET *ppPacket);


void __RPC_STUB INrvEvents3_GetEvent_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __INrvEvents3_INTERFACE_DEFINED__ */


#ifndef __INrvData3_INTERFACE_DEFINED__
#define __INrvData3_INTERFACE_DEFINED__

/* interface INrvData3 */
/* [unique][helpstring][uuid][local][object] */ 


EXTERN_C const IID IID_INrvData3;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("E8AE1D3B-A714-4865-9CC1-59646EDFB24D")
    INrvData3 : public IUnknown
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE New( 
            /* [in] */ BSTR bstrFileName) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Open( 
            /* [in] */ BSTR bstrFileName,
            /* [in] */ DWORD dwOpenFlags,
            /* [out][in] */ BOOL *pbReadOnly) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Close( void) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetFileName( 
            /* [retval][out] */ BSTR *pbstrFileName) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_bReadOnly( 
            /* [retval][out] */ BOOL *pval) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Initialize( 
            /* [in] */ long lcMaxChannels) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE IsInitialized( 
            /* [retval][out] */ BOOL *pVal) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetMaxChannelCount( 
            /* [retval][out] */ long *plcMaxChannels) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE StartSampling( 
            /* [in] */ NRVDATE nrvDate,
            /* [in] */ PACKET pTSInfo) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE StopSampling( void) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_bSampling( 
            /* [retval][out] */ BOOL *pVal) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE ReadStaticInfoPacket( 
            /* [in] */ GUID guid,
            /* [in] */ long lPacketSize,
            /* [size_is][out] */ BYTE *pPacket) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE WriteStaticInfoPacket( 
            /* [in] */ PACKET pPacket) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE QueryStaticInfoPacketSize( 
            /* [in] */ GUID guid,
            /* [retval][out] */ long *plSize) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE CanWriteStaticInfoPacket( 
            /* [in] */ GUID guid,
            /* [retval][out] */ BOOL *pbVal) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE WriteInfoPacket( 
            /* [in] */ PACKET pPacket) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE ReadInfoPacket( 
            /* [in] */ long liChange,
            /* [in] */ long lPacketSize,
            /* [size_is][out] */ BYTE *pPacket) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE WriteData( 
            /* [in] */ long liChannel,
            /* [in] */ long lcSamples,
            /* [size_is][in] */ short *psSamples) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Flush( void) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE ReadData( 
            /* [in] */ long liChannel,
            /* [in] */ NRVDATE nrvDate,
            /* [in] */ double dDuration,
            /* [out] */ short *psSamples) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetDataPacket( 
            /* [in] */ NRVDATE nrvDate,
            /* [in] */ double dDuration,
            /* [in] */ BYTE *pPacket) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_bSupportsReadData( 
            /* [retval][out] */ BOOL *pVal) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetChannelCount( 
            /* [in] */ NRVDATE nrvDate,
            /* [retval][out] */ long *plcChannels) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetSampleCount( 
            long liChannel,
            /* [in] */ NRVDATE nrvDate,
            /* [in] */ double dDuration,
            /* [retval][out] */ long *plSamples) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetSampleCount2( 
            /* [in] */ NRVDATE nrvDate,
            /* [in] */ double dDuration,
            /* [in] */ long lcChannels,
            /* [size_is][out] */ long *plSamples) = 0;
        
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
            /* [in] */ long liChange,
            /* [out] */ NRVDATE *pnrvDate,
            /* [out] */ long *plPacketSize,
            /* [retval][out] */ GUID *pguidInfoPacket) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_lcInfoChange( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE QueryLastInfoChange( 
            /* [in] */ GUID guidInfoChange,
            /* [in] */ NRVDATE nrvDate,
            /* [out] */ long *plPacketSize,
            /* [retval][out] */ long *pliInfoChange) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE WriteEvents( 
            /* [in] */ INrvEvents3 *pINrvEvents3) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetEvents( 
            /* [in] */ INrvEvents3 *pINrvEvents3) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE AddEvent( 
            /* [in] */ PACKET pPacket) = 0;
        
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

    typedef struct INrvData3Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            INrvData3 * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            INrvData3 * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            INrvData3 * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *New )( 
            INrvData3 * This,
            /* [in] */ BSTR bstrFileName);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *Open )( 
            INrvData3 * This,
            /* [in] */ BSTR bstrFileName,
            /* [in] */ DWORD dwOpenFlags,
            /* [out][in] */ BOOL *pbReadOnly);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *Close )( 
            INrvData3 * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetFileName )( 
            INrvData3 * This,
            /* [retval][out] */ BSTR *pbstrFileName);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_bReadOnly )( 
            INrvData3 * This,
            /* [retval][out] */ BOOL *pval);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *Initialize )( 
            INrvData3 * This,
            /* [in] */ long lcMaxChannels);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *IsInitialized )( 
            INrvData3 * This,
            /* [retval][out] */ BOOL *pVal);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetMaxChannelCount )( 
            INrvData3 * This,
            /* [retval][out] */ long *plcMaxChannels);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *StartSampling )( 
            INrvData3 * This,
            /* [in] */ NRVDATE nrvDate,
            /* [in] */ PACKET pTSInfo);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *StopSampling )( 
            INrvData3 * This);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_bSampling )( 
            INrvData3 * This,
            /* [retval][out] */ BOOL *pVal);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *ReadStaticInfoPacket )( 
            INrvData3 * This,
            /* [in] */ GUID guid,
            /* [in] */ long lPacketSize,
            /* [size_is][out] */ BYTE *pPacket);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *WriteStaticInfoPacket )( 
            INrvData3 * This,
            /* [in] */ PACKET pPacket);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *QueryStaticInfoPacketSize )( 
            INrvData3 * This,
            /* [in] */ GUID guid,
            /* [retval][out] */ long *plSize);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *CanWriteStaticInfoPacket )( 
            INrvData3 * This,
            /* [in] */ GUID guid,
            /* [retval][out] */ BOOL *pbVal);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *WriteInfoPacket )( 
            INrvData3 * This,
            /* [in] */ PACKET pPacket);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *ReadInfoPacket )( 
            INrvData3 * This,
            /* [in] */ long liChange,
            /* [in] */ long lPacketSize,
            /* [size_is][out] */ BYTE *pPacket);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *WriteData )( 
            INrvData3 * This,
            /* [in] */ long liChannel,
            /* [in] */ long lcSamples,
            /* [size_is][in] */ short *psSamples);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *Flush )( 
            INrvData3 * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *ReadData )( 
            INrvData3 * This,
            /* [in] */ long liChannel,
            /* [in] */ NRVDATE nrvDate,
            /* [in] */ double dDuration,
            /* [out] */ short *psSamples);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetDataPacket )( 
            INrvData3 * This,
            /* [in] */ NRVDATE nrvDate,
            /* [in] */ double dDuration,
            /* [in] */ BYTE *pPacket);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_bSupportsReadData )( 
            INrvData3 * This,
            /* [retval][out] */ BOOL *pVal);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetChannelCount )( 
            INrvData3 * This,
            /* [in] */ NRVDATE nrvDate,
            /* [retval][out] */ long *plcChannels);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetSampleCount )( 
            INrvData3 * This,
            long liChannel,
            /* [in] */ NRVDATE nrvDate,
            /* [in] */ double dDuration,
            /* [retval][out] */ long *plSamples);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetSampleCount2 )( 
            INrvData3 * This,
            /* [in] */ NRVDATE nrvDate,
            /* [in] */ double dDuration,
            /* [in] */ long lcChannels,
            /* [size_is][out] */ long *plSamples);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *QueryNextInfoChange )( 
            INrvData3 * This,
            /* [in] */ NRVDATE nrvDate,
            /* [out] */ NRVDATE *pnrvDate,
            /* [out] */ GUID *pguidInfoPacket,
            /* [out] */ long *plPacketSize,
            /* [retval][out] */ long *pliChange);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *QueryNextInfoChange2 )( 
            INrvData3 * This,
            /* [in] */ GUID guidInfoPacket,
            /* [in] */ long liChange,
            /* [out] */ long *plPacketSize,
            /* [retval][out] */ long *pliChange);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *QueryInfoChange )( 
            INrvData3 * This,
            /* [in] */ long liChange,
            /* [out] */ NRVDATE *pnrvDate,
            /* [out] */ long *plPacketSize,
            /* [retval][out] */ GUID *pguidInfoPacket);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_lcInfoChange )( 
            INrvData3 * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *QueryLastInfoChange )( 
            INrvData3 * This,
            /* [in] */ GUID guidInfoChange,
            /* [in] */ NRVDATE nrvDate,
            /* [out] */ long *plPacketSize,
            /* [retval][out] */ long *pliInfoChange);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *WriteEvents )( 
            INrvData3 * This,
            /* [in] */ INrvEvents3 *pINrvEvents3);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetEvents )( 
            INrvData3 * This,
            /* [in] */ INrvEvents3 *pINrvEvents3);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *AddEvent )( 
            INrvData3 * This,
            /* [in] */ PACKET pPacket);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetTestID )( 
            INrvData3 * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *ReadExtraData )( 
            INrvData3 * This,
            /* [in] */ GUID guidType,
            /* [in] */ BSTR bstrName,
            /* [in] */ LONGLONG llPos,
            /* [in] */ ULONG lcSamples,
            /* [size_is][out] */ BYTE *pSamples,
            /* [retval][out] */ ULONG *plSamplesRead);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *WriteExtraData )( 
            INrvData3 * This,
            /* [in] */ GUID guidType,
            /* [in] */ BSTR bstrName,
            /* [in] */ LONGLONG llPos,
            /* [in] */ ULONG lcSamples,
            /* [size_is][in] */ BYTE *pSamples,
            /* [retval][out] */ ULONG *plSamplesWritten);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *CreateExtraData )( 
            INrvData3 * This,
            /* [in] */ GUID guidType,
            /* [in] */ BSTR bstrName);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetExtraDataSize )( 
            INrvData3 * This,
            /* [in] */ GUID guidType,
            /* [in] */ BSTR bstrName,
            /* [retval][out] */ ULONGLONG *pllSize);
        
        END_INTERFACE
    } INrvData3Vtbl;

    interface INrvData3
    {
        CONST_VTBL struct INrvData3Vtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define INrvData3_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define INrvData3_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define INrvData3_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define INrvData3_New(This,bstrFileName)	\
    (This)->lpVtbl -> New(This,bstrFileName)

#define INrvData3_Open(This,bstrFileName,dwOpenFlags,pbReadOnly)	\
    (This)->lpVtbl -> Open(This,bstrFileName,dwOpenFlags,pbReadOnly)

#define INrvData3_Close(This)	\
    (This)->lpVtbl -> Close(This)

#define INrvData3_GetFileName(This,pbstrFileName)	\
    (This)->lpVtbl -> GetFileName(This,pbstrFileName)

#define INrvData3_get_bReadOnly(This,pval)	\
    (This)->lpVtbl -> get_bReadOnly(This,pval)

#define INrvData3_Initialize(This,lcMaxChannels)	\
    (This)->lpVtbl -> Initialize(This,lcMaxChannels)

#define INrvData3_IsInitialized(This,pVal)	\
    (This)->lpVtbl -> IsInitialized(This,pVal)

#define INrvData3_GetMaxChannelCount(This,plcMaxChannels)	\
    (This)->lpVtbl -> GetMaxChannelCount(This,plcMaxChannels)

#define INrvData3_StartSampling(This,nrvDate,pTSInfo)	\
    (This)->lpVtbl -> StartSampling(This,nrvDate,pTSInfo)

#define INrvData3_StopSampling(This)	\
    (This)->lpVtbl -> StopSampling(This)

#define INrvData3_get_bSampling(This,pVal)	\
    (This)->lpVtbl -> get_bSampling(This,pVal)

#define INrvData3_ReadStaticInfoPacket(This,guid,lPacketSize,pPacket)	\
    (This)->lpVtbl -> ReadStaticInfoPacket(This,guid,lPacketSize,pPacket)

#define INrvData3_WriteStaticInfoPacket(This,pPacket)	\
    (This)->lpVtbl -> WriteStaticInfoPacket(This,pPacket)

#define INrvData3_QueryStaticInfoPacketSize(This,guid,plSize)	\
    (This)->lpVtbl -> QueryStaticInfoPacketSize(This,guid,plSize)

#define INrvData3_CanWriteStaticInfoPacket(This,guid,pbVal)	\
    (This)->lpVtbl -> CanWriteStaticInfoPacket(This,guid,pbVal)

#define INrvData3_WriteInfoPacket(This,pPacket)	\
    (This)->lpVtbl -> WriteInfoPacket(This,pPacket)

#define INrvData3_ReadInfoPacket(This,liChange,lPacketSize,pPacket)	\
    (This)->lpVtbl -> ReadInfoPacket(This,liChange,lPacketSize,pPacket)

#define INrvData3_WriteData(This,liChannel,lcSamples,psSamples)	\
    (This)->lpVtbl -> WriteData(This,liChannel,lcSamples,psSamples)

#define INrvData3_Flush(This)	\
    (This)->lpVtbl -> Flush(This)

#define INrvData3_ReadData(This,liChannel,nrvDate,dDuration,psSamples)	\
    (This)->lpVtbl -> ReadData(This,liChannel,nrvDate,dDuration,psSamples)

#define INrvData3_GetDataPacket(This,nrvDate,dDuration,pPacket)	\
    (This)->lpVtbl -> GetDataPacket(This,nrvDate,dDuration,pPacket)

#define INrvData3_get_bSupportsReadData(This,pVal)	\
    (This)->lpVtbl -> get_bSupportsReadData(This,pVal)

#define INrvData3_GetChannelCount(This,nrvDate,plcChannels)	\
    (This)->lpVtbl -> GetChannelCount(This,nrvDate,plcChannels)

#define INrvData3_GetSampleCount(This,liChannel,nrvDate,dDuration,plSamples)	\
    (This)->lpVtbl -> GetSampleCount(This,liChannel,nrvDate,dDuration,plSamples)

#define INrvData3_GetSampleCount2(This,nrvDate,dDuration,lcChannels,plSamples)	\
    (This)->lpVtbl -> GetSampleCount2(This,nrvDate,dDuration,lcChannels,plSamples)

#define INrvData3_QueryNextInfoChange(This,nrvDate,pnrvDate,pguidInfoPacket,plPacketSize,pliChange)	\
    (This)->lpVtbl -> QueryNextInfoChange(This,nrvDate,pnrvDate,pguidInfoPacket,plPacketSize,pliChange)

#define INrvData3_QueryNextInfoChange2(This,guidInfoPacket,liChange,plPacketSize,pliChange)	\
    (This)->lpVtbl -> QueryNextInfoChange2(This,guidInfoPacket,liChange,plPacketSize,pliChange)

#define INrvData3_QueryInfoChange(This,liChange,pnrvDate,plPacketSize,pguidInfoPacket)	\
    (This)->lpVtbl -> QueryInfoChange(This,liChange,pnrvDate,plPacketSize,pguidInfoPacket)

#define INrvData3_get_lcInfoChange(This,pVal)	\
    (This)->lpVtbl -> get_lcInfoChange(This,pVal)

#define INrvData3_QueryLastInfoChange(This,guidInfoChange,nrvDate,plPacketSize,pliInfoChange)	\
    (This)->lpVtbl -> QueryLastInfoChange(This,guidInfoChange,nrvDate,plPacketSize,pliInfoChange)

#define INrvData3_WriteEvents(This,pINrvEvents3)	\
    (This)->lpVtbl -> WriteEvents(This,pINrvEvents3)

#define INrvData3_GetEvents(This,pINrvEvents3)	\
    (This)->lpVtbl -> GetEvents(This,pINrvEvents3)

#define INrvData3_AddEvent(This,pPacket)	\
    (This)->lpVtbl -> AddEvent(This,pPacket)

#define INrvData3_GetTestID(This,pVal)	\
    (This)->lpVtbl -> GetTestID(This,pVal)

#define INrvData3_ReadExtraData(This,guidType,bstrName,llPos,lcSamples,pSamples,plSamplesRead)	\
    (This)->lpVtbl -> ReadExtraData(This,guidType,bstrName,llPos,lcSamples,pSamples,plSamplesRead)

#define INrvData3_WriteExtraData(This,guidType,bstrName,llPos,lcSamples,pSamples,plSamplesWritten)	\
    (This)->lpVtbl -> WriteExtraData(This,guidType,bstrName,llPos,lcSamples,pSamples,plSamplesWritten)

#define INrvData3_CreateExtraData(This,guidType,bstrName)	\
    (This)->lpVtbl -> CreateExtraData(This,guidType,bstrName)

#define INrvData3_GetExtraDataSize(This,guidType,bstrName,pllSize)	\
    (This)->lpVtbl -> GetExtraDataSize(This,guidType,bstrName,pllSize)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring] */ HRESULT STDMETHODCALLTYPE INrvData3_New_Proxy( 
    INrvData3 * This,
    /* [in] */ BSTR bstrFileName);


void __RPC_STUB INrvData3_New_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE INrvData3_Open_Proxy( 
    INrvData3 * This,
    /* [in] */ BSTR bstrFileName,
    /* [in] */ DWORD dwOpenFlags,
    /* [out][in] */ BOOL *pbReadOnly);


void __RPC_STUB INrvData3_Open_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE INrvData3_Close_Proxy( 
    INrvData3 * This);


void __RPC_STUB INrvData3_Close_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE INrvData3_GetFileName_Proxy( 
    INrvData3 * This,
    /* [retval][out] */ BSTR *pbstrFileName);


void __RPC_STUB INrvData3_GetFileName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE INrvData3_get_bReadOnly_Proxy( 
    INrvData3 * This,
    /* [retval][out] */ BOOL *pval);


void __RPC_STUB INrvData3_get_bReadOnly_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE INrvData3_Initialize_Proxy( 
    INrvData3 * This,
    /* [in] */ long lcMaxChannels);


void __RPC_STUB INrvData3_Initialize_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE INrvData3_IsInitialized_Proxy( 
    INrvData3 * This,
    /* [retval][out] */ BOOL *pVal);


void __RPC_STUB INrvData3_IsInitialized_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE INrvData3_GetMaxChannelCount_Proxy( 
    INrvData3 * This,
    /* [retval][out] */ long *plcMaxChannels);


void __RPC_STUB INrvData3_GetMaxChannelCount_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE INrvData3_StartSampling_Proxy( 
    INrvData3 * This,
    /* [in] */ NRVDATE nrvDate,
    /* [in] */ PACKET pTSInfo);


void __RPC_STUB INrvData3_StartSampling_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE INrvData3_StopSampling_Proxy( 
    INrvData3 * This);


void __RPC_STUB INrvData3_StopSampling_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE INrvData3_get_bSampling_Proxy( 
    INrvData3 * This,
    /* [retval][out] */ BOOL *pVal);


void __RPC_STUB INrvData3_get_bSampling_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE INrvData3_ReadStaticInfoPacket_Proxy( 
    INrvData3 * This,
    /* [in] */ GUID guid,
    /* [in] */ long lPacketSize,
    /* [size_is][out] */ BYTE *pPacket);


void __RPC_STUB INrvData3_ReadStaticInfoPacket_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE INrvData3_WriteStaticInfoPacket_Proxy( 
    INrvData3 * This,
    /* [in] */ PACKET pPacket);


void __RPC_STUB INrvData3_WriteStaticInfoPacket_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE INrvData3_QueryStaticInfoPacketSize_Proxy( 
    INrvData3 * This,
    /* [in] */ GUID guid,
    /* [retval][out] */ long *plSize);


void __RPC_STUB INrvData3_QueryStaticInfoPacketSize_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE INrvData3_CanWriteStaticInfoPacket_Proxy( 
    INrvData3 * This,
    /* [in] */ GUID guid,
    /* [retval][out] */ BOOL *pbVal);


void __RPC_STUB INrvData3_CanWriteStaticInfoPacket_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE INrvData3_WriteInfoPacket_Proxy( 
    INrvData3 * This,
    /* [in] */ PACKET pPacket);


void __RPC_STUB INrvData3_WriteInfoPacket_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE INrvData3_ReadInfoPacket_Proxy( 
    INrvData3 * This,
    /* [in] */ long liChange,
    /* [in] */ long lPacketSize,
    /* [size_is][out] */ BYTE *pPacket);


void __RPC_STUB INrvData3_ReadInfoPacket_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE INrvData3_WriteData_Proxy( 
    INrvData3 * This,
    /* [in] */ long liChannel,
    /* [in] */ long lcSamples,
    /* [size_is][in] */ short *psSamples);


void __RPC_STUB INrvData3_WriteData_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE INrvData3_Flush_Proxy( 
    INrvData3 * This);


void __RPC_STUB INrvData3_Flush_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE INrvData3_ReadData_Proxy( 
    INrvData3 * This,
    /* [in] */ long liChannel,
    /* [in] */ NRVDATE nrvDate,
    /* [in] */ double dDuration,
    /* [out] */ short *psSamples);


void __RPC_STUB INrvData3_ReadData_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE INrvData3_GetDataPacket_Proxy( 
    INrvData3 * This,
    /* [in] */ NRVDATE nrvDate,
    /* [in] */ double dDuration,
    /* [in] */ BYTE *pPacket);


void __RPC_STUB INrvData3_GetDataPacket_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE INrvData3_get_bSupportsReadData_Proxy( 
    INrvData3 * This,
    /* [retval][out] */ BOOL *pVal);


void __RPC_STUB INrvData3_get_bSupportsReadData_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE INrvData3_GetChannelCount_Proxy( 
    INrvData3 * This,
    /* [in] */ NRVDATE nrvDate,
    /* [retval][out] */ long *plcChannels);


void __RPC_STUB INrvData3_GetChannelCount_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE INrvData3_GetSampleCount_Proxy( 
    INrvData3 * This,
    long liChannel,
    /* [in] */ NRVDATE nrvDate,
    /* [in] */ double dDuration,
    /* [retval][out] */ long *plSamples);


void __RPC_STUB INrvData3_GetSampleCount_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE INrvData3_GetSampleCount2_Proxy( 
    INrvData3 * This,
    /* [in] */ NRVDATE nrvDate,
    /* [in] */ double dDuration,
    /* [in] */ long lcChannels,
    /* [size_is][out] */ long *plSamples);


void __RPC_STUB INrvData3_GetSampleCount2_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE INrvData3_QueryNextInfoChange_Proxy( 
    INrvData3 * This,
    /* [in] */ NRVDATE nrvDate,
    /* [out] */ NRVDATE *pnrvDate,
    /* [out] */ GUID *pguidInfoPacket,
    /* [out] */ long *plPacketSize,
    /* [retval][out] */ long *pliChange);


void __RPC_STUB INrvData3_QueryNextInfoChange_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE INrvData3_QueryNextInfoChange2_Proxy( 
    INrvData3 * This,
    /* [in] */ GUID guidInfoPacket,
    /* [in] */ long liChange,
    /* [out] */ long *plPacketSize,
    /* [retval][out] */ long *pliChange);


void __RPC_STUB INrvData3_QueryNextInfoChange2_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE INrvData3_QueryInfoChange_Proxy( 
    INrvData3 * This,
    /* [in] */ long liChange,
    /* [out] */ NRVDATE *pnrvDate,
    /* [out] */ long *plPacketSize,
    /* [retval][out] */ GUID *pguidInfoPacket);


void __RPC_STUB INrvData3_QueryInfoChange_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE INrvData3_get_lcInfoChange_Proxy( 
    INrvData3 * This,
    /* [retval][out] */ long *pVal);


void __RPC_STUB INrvData3_get_lcInfoChange_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE INrvData3_QueryLastInfoChange_Proxy( 
    INrvData3 * This,
    /* [in] */ GUID guidInfoChange,
    /* [in] */ NRVDATE nrvDate,
    /* [out] */ long *plPacketSize,
    /* [retval][out] */ long *pliInfoChange);


void __RPC_STUB INrvData3_QueryLastInfoChange_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE INrvData3_WriteEvents_Proxy( 
    INrvData3 * This,
    /* [in] */ INrvEvents3 *pINrvEvents3);


void __RPC_STUB INrvData3_WriteEvents_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE INrvData3_GetEvents_Proxy( 
    INrvData3 * This,
    /* [in] */ INrvEvents3 *pINrvEvents3);


void __RPC_STUB INrvData3_GetEvents_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE INrvData3_AddEvent_Proxy( 
    INrvData3 * This,
    /* [in] */ PACKET pPacket);


void __RPC_STUB INrvData3_AddEvent_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE INrvData3_GetTestID_Proxy( 
    INrvData3 * This,
    /* [retval][out] */ BSTR *pVal);


void __RPC_STUB INrvData3_GetTestID_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE INrvData3_ReadExtraData_Proxy( 
    INrvData3 * This,
    /* [in] */ GUID guidType,
    /* [in] */ BSTR bstrName,
    /* [in] */ LONGLONG llPos,
    /* [in] */ ULONG lcSamples,
    /* [size_is][out] */ BYTE *pSamples,
    /* [retval][out] */ ULONG *plSamplesRead);


void __RPC_STUB INrvData3_ReadExtraData_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE INrvData3_WriteExtraData_Proxy( 
    INrvData3 * This,
    /* [in] */ GUID guidType,
    /* [in] */ BSTR bstrName,
    /* [in] */ LONGLONG llPos,
    /* [in] */ ULONG lcSamples,
    /* [size_is][in] */ BYTE *pSamples,
    /* [retval][out] */ ULONG *plSamplesWritten);


void __RPC_STUB INrvData3_WriteExtraData_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE INrvData3_CreateExtraData_Proxy( 
    INrvData3 * This,
    /* [in] */ GUID guidType,
    /* [in] */ BSTR bstrName);


void __RPC_STUB INrvData3_CreateExtraData_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE INrvData3_GetExtraDataSize_Proxy( 
    INrvData3 * This,
    /* [in] */ GUID guidType,
    /* [in] */ BSTR bstrName,
    /* [retval][out] */ ULONGLONG *pllSize);


void __RPC_STUB INrvData3_GetExtraDataSize_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __INrvData3_INTERFACE_DEFINED__ */


#ifndef __INrvSegments3_INTERFACE_DEFINED__
#define __INrvSegments3_INTERFACE_DEFINED__

/* interface INrvSegments3 */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_INrvSegments3;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("40084763-DB80-4a65-8157-4D8A03839E37")
    INrvSegments3 : public IUnknown
    {
    public:
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_lcSegments( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE QueryLastSegment( 
            /* [in] */ NRVDATE nrvDate,
            /* [out] */ long *pliSegment,
            /* [retval][out] */ BOOL *pbInSegment) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE QuerySegmentStartTime( 
            /* [in] */ long liSegment,
            /* [out] */ NRVDATE *pnrvDate) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE QuerySegmentEndTime( 
            /* [in] */ long liSegment,
            /* [out] */ NRVDATE *pnrvDate) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE NrvDate2RecSec( 
            /* [in] */ NRVDATE nrvDate,
            /* [retval][out] */ double *pdRecSec) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE RecSec2NrvDate( 
            /* [in] */ double dRecSec,
            /* [out] */ NRVDATE *pnrvDate) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct INrvSegments3Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            INrvSegments3 * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            INrvSegments3 * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            INrvSegments3 * This);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_lcSegments )( 
            INrvSegments3 * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *QueryLastSegment )( 
            INrvSegments3 * This,
            /* [in] */ NRVDATE nrvDate,
            /* [out] */ long *pliSegment,
            /* [retval][out] */ BOOL *pbInSegment);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *QuerySegmentStartTime )( 
            INrvSegments3 * This,
            /* [in] */ long liSegment,
            /* [out] */ NRVDATE *pnrvDate);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *QuerySegmentEndTime )( 
            INrvSegments3 * This,
            /* [in] */ long liSegment,
            /* [out] */ NRVDATE *pnrvDate);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *NrvDate2RecSec )( 
            INrvSegments3 * This,
            /* [in] */ NRVDATE nrvDate,
            /* [retval][out] */ double *pdRecSec);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *RecSec2NrvDate )( 
            INrvSegments3 * This,
            /* [in] */ double dRecSec,
            /* [out] */ NRVDATE *pnrvDate);
        
        END_INTERFACE
    } INrvSegments3Vtbl;

    interface INrvSegments3
    {
        CONST_VTBL struct INrvSegments3Vtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define INrvSegments3_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define INrvSegments3_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define INrvSegments3_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define INrvSegments3_get_lcSegments(This,pVal)	\
    (This)->lpVtbl -> get_lcSegments(This,pVal)

#define INrvSegments3_QueryLastSegment(This,nrvDate,pliSegment,pbInSegment)	\
    (This)->lpVtbl -> QueryLastSegment(This,nrvDate,pliSegment,pbInSegment)

#define INrvSegments3_QuerySegmentStartTime(This,liSegment,pnrvDate)	\
    (This)->lpVtbl -> QuerySegmentStartTime(This,liSegment,pnrvDate)

#define INrvSegments3_QuerySegmentEndTime(This,liSegment,pnrvDate)	\
    (This)->lpVtbl -> QuerySegmentEndTime(This,liSegment,pnrvDate)

#define INrvSegments3_NrvDate2RecSec(This,nrvDate,pdRecSec)	\
    (This)->lpVtbl -> NrvDate2RecSec(This,nrvDate,pdRecSec)

#define INrvSegments3_RecSec2NrvDate(This,dRecSec,pnrvDate)	\
    (This)->lpVtbl -> RecSec2NrvDate(This,dRecSec,pnrvDate)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE INrvSegments3_get_lcSegments_Proxy( 
    INrvSegments3 * This,
    /* [retval][out] */ long *pVal);


void __RPC_STUB INrvSegments3_get_lcSegments_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE INrvSegments3_QueryLastSegment_Proxy( 
    INrvSegments3 * This,
    /* [in] */ NRVDATE nrvDate,
    /* [out] */ long *pliSegment,
    /* [retval][out] */ BOOL *pbInSegment);


void __RPC_STUB INrvSegments3_QueryLastSegment_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE INrvSegments3_QuerySegmentStartTime_Proxy( 
    INrvSegments3 * This,
    /* [in] */ long liSegment,
    /* [out] */ NRVDATE *pnrvDate);


void __RPC_STUB INrvSegments3_QuerySegmentStartTime_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE INrvSegments3_QuerySegmentEndTime_Proxy( 
    INrvSegments3 * This,
    /* [in] */ long liSegment,
    /* [out] */ NRVDATE *pnrvDate);


void __RPC_STUB INrvSegments3_QuerySegmentEndTime_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE INrvSegments3_NrvDate2RecSec_Proxy( 
    INrvSegments3 * This,
    /* [in] */ NRVDATE nrvDate,
    /* [retval][out] */ double *pdRecSec);


void __RPC_STUB INrvSegments3_NrvDate2RecSec_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE INrvSegments3_RecSec2NrvDate_Proxy( 
    INrvSegments3 * This,
    /* [in] */ double dRecSec,
    /* [out] */ NRVDATE *pnrvDate);


void __RPC_STUB INrvSegments3_RecSec2NrvDate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __INrvSegments3_INTERFACE_DEFINED__ */


#ifndef __IPanelItem_INTERFACE_DEFINED__
#define __IPanelItem_INTERFACE_DEFINED__

/* interface IPanelItem */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IPanelItem;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("77136353-BD06-4b5d-879B-9C574DD56CF4")
    IPanelItem : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetBuilder( 
            /* [in] */ IDispatch *pIBuilder) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Close( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IsAvailable( 
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE StateChanged( 
            /* [in] */ long lNewState) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetTitle( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetSizingFlags( 
            /* [retval][out] */ DWORD *pdwFlags) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IPanelItemVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IPanelItem * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IPanelItem * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IPanelItem * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IPanelItem * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IPanelItem * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IPanelItem * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IPanelItem * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetBuilder )( 
            IPanelItem * This,
            /* [in] */ IDispatch *pIBuilder);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Close )( 
            IPanelItem * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *IsAvailable )( 
            IPanelItem * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *StateChanged )( 
            IPanelItem * This,
            /* [in] */ long lNewState);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetTitle )( 
            IPanelItem * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetSizingFlags )( 
            IPanelItem * This,
            /* [retval][out] */ DWORD *pdwFlags);
        
        END_INTERFACE
    } IPanelItemVtbl;

    interface IPanelItem
    {
        CONST_VTBL struct IPanelItemVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPanelItem_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IPanelItem_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IPanelItem_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IPanelItem_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IPanelItem_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IPanelItem_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IPanelItem_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IPanelItem_SetBuilder(This,pIBuilder)	\
    (This)->lpVtbl -> SetBuilder(This,pIBuilder)

#define IPanelItem_Close(This)	\
    (This)->lpVtbl -> Close(This)

#define IPanelItem_IsAvailable(This,pVal)	\
    (This)->lpVtbl -> IsAvailable(This,pVal)

#define IPanelItem_StateChanged(This,lNewState)	\
    (This)->lpVtbl -> StateChanged(This,lNewState)

#define IPanelItem_GetTitle(This,pVal)	\
    (This)->lpVtbl -> GetTitle(This,pVal)

#define IPanelItem_GetSizingFlags(This,pdwFlags)	\
    (This)->lpVtbl -> GetSizingFlags(This,pdwFlags)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPanelItem_SetBuilder_Proxy( 
    IPanelItem * This,
    /* [in] */ IDispatch *pIBuilder);


void __RPC_STUB IPanelItem_SetBuilder_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPanelItem_Close_Proxy( 
    IPanelItem * This);


void __RPC_STUB IPanelItem_Close_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPanelItem_IsAvailable_Proxy( 
    IPanelItem * This,
    /* [retval][out] */ VARIANT_BOOL *pVal);


void __RPC_STUB IPanelItem_IsAvailable_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPanelItem_StateChanged_Proxy( 
    IPanelItem * This,
    /* [in] */ long lNewState);


void __RPC_STUB IPanelItem_StateChanged_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPanelItem_GetTitle_Proxy( 
    IPanelItem * This,
    /* [retval][out] */ BSTR *pVal);


void __RPC_STUB IPanelItem_GetTitle_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPanelItem_GetSizingFlags_Proxy( 
    IPanelItem * This,
    /* [retval][out] */ DWORD *pdwFlags);


void __RPC_STUB IPanelItem_GetSizingFlags_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IPanelItem_INTERFACE_DEFINED__ */


#ifndef __IReport_INTERFACE_DEFINED__
#define __IReport_INTERFACE_DEFINED__

/* interface IReport */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IReport;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("231D8C29-5159-4ab2-991B-02799F9A547B")
    IReport : public IDispatch
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE SetStorage( 
            /* [in] */ IDispatch *pDisp) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE SetStudy( 
            /* [in] */ IDispatch *pStudy,
            /* [in] */ VARIANT_BOOL bStudyRoom) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GenerateReport( 
            /* [in] */ BSTR bstrTemplatePath) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE SetBuilder( 
            /* [in] */ IDispatch *pDisp) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE IsAvailable( 
            /* [retval][out] */ VARIANT_BOOL *pbAvailable) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IReportVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IReport * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IReport * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IReport * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IReport * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IReport * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IReport * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IReport * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetStorage )( 
            IReport * This,
            /* [in] */ IDispatch *pDisp);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetStudy )( 
            IReport * This,
            /* [in] */ IDispatch *pStudy,
            /* [in] */ VARIANT_BOOL bStudyRoom);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GenerateReport )( 
            IReport * This,
            /* [in] */ BSTR bstrTemplatePath);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetBuilder )( 
            IReport * This,
            /* [in] */ IDispatch *pDisp);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *IsAvailable )( 
            IReport * This,
            /* [retval][out] */ VARIANT_BOOL *pbAvailable);
        
        END_INTERFACE
    } IReportVtbl;

    interface IReport
    {
        CONST_VTBL struct IReportVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IReport_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IReport_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IReport_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IReport_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IReport_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IReport_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IReport_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IReport_SetStorage(This,pDisp)	\
    (This)->lpVtbl -> SetStorage(This,pDisp)

#define IReport_SetStudy(This,pStudy,bStudyRoom)	\
    (This)->lpVtbl -> SetStudy(This,pStudy,bStudyRoom)

#define IReport_GenerateReport(This,bstrTemplatePath)	\
    (This)->lpVtbl -> GenerateReport(This,bstrTemplatePath)

#define IReport_SetBuilder(This,pDisp)	\
    (This)->lpVtbl -> SetBuilder(This,pDisp)

#define IReport_IsAvailable(This,pbAvailable)	\
    (This)->lpVtbl -> IsAvailable(This,pbAvailable)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring] */ HRESULT STDMETHODCALLTYPE IReport_SetStorage_Proxy( 
    IReport * This,
    /* [in] */ IDispatch *pDisp);


void __RPC_STUB IReport_SetStorage_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IReport_SetStudy_Proxy( 
    IReport * This,
    /* [in] */ IDispatch *pStudy,
    /* [in] */ VARIANT_BOOL bStudyRoom);


void __RPC_STUB IReport_SetStudy_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IReport_GenerateReport_Proxy( 
    IReport * This,
    /* [in] */ BSTR bstrTemplatePath);


void __RPC_STUB IReport_GenerateReport_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IReport_SetBuilder_Proxy( 
    IReport * This,
    /* [in] */ IDispatch *pDisp);


void __RPC_STUB IReport_SetBuilder_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IReport_IsAvailable_Proxy( 
    IReport * This,
    /* [retval][out] */ VARIANT_BOOL *pbAvailable);


void __RPC_STUB IReport_IsAvailable_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IReport_INTERFACE_DEFINED__ */


#ifndef __IReportGenerator_INTERFACE_DEFINED__
#define __IReportGenerator_INTERFACE_DEFINED__

/* interface IReportGenerator */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IReportGenerator;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("C4340326-CEB3-454f-8C12-1921E31294DA")
    IReportGenerator : public IDispatch
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE SetStorage( 
            /* [in] */ IDispatch *pDisp) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE SetStudy( 
            /* [in] */ IDispatch *pStudy) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE SetBuilder( 
            /* [in] */ IDispatch *pDisp) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE SetTemplate( 
            /* [in] */ BSTR bstrTemplatePath) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE IsAvailable( 
            /* [retval][out] */ VARIANT_BOOL *pbAvailable) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetDocumentTypeID( 
            /* [retval][out] */ BSTR *pbstrID) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetDocumentTitle( 
            /* [retval][out] */ BSTR *pbstrTitle) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetExtension( 
            /* [retval][out] */ BSTR *pbstrExt) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetFileNameBase( 
            /* [retval][out] */ BSTR *pbstrBase) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GenerateReport( 
            /* [in] */ BSTR bstrPath) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IReportGeneratorVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IReportGenerator * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IReportGenerator * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IReportGenerator * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IReportGenerator * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IReportGenerator * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IReportGenerator * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IReportGenerator * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetStorage )( 
            IReportGenerator * This,
            /* [in] */ IDispatch *pDisp);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetStudy )( 
            IReportGenerator * This,
            /* [in] */ IDispatch *pStudy);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetBuilder )( 
            IReportGenerator * This,
            /* [in] */ IDispatch *pDisp);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetTemplate )( 
            IReportGenerator * This,
            /* [in] */ BSTR bstrTemplatePath);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *IsAvailable )( 
            IReportGenerator * This,
            /* [retval][out] */ VARIANT_BOOL *pbAvailable);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetDocumentTypeID )( 
            IReportGenerator * This,
            /* [retval][out] */ BSTR *pbstrID);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetDocumentTitle )( 
            IReportGenerator * This,
            /* [retval][out] */ BSTR *pbstrTitle);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetExtension )( 
            IReportGenerator * This,
            /* [retval][out] */ BSTR *pbstrExt);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetFileNameBase )( 
            IReportGenerator * This,
            /* [retval][out] */ BSTR *pbstrBase);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GenerateReport )( 
            IReportGenerator * This,
            /* [in] */ BSTR bstrPath);
        
        END_INTERFACE
    } IReportGeneratorVtbl;

    interface IReportGenerator
    {
        CONST_VTBL struct IReportGeneratorVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IReportGenerator_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IReportGenerator_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IReportGenerator_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IReportGenerator_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IReportGenerator_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IReportGenerator_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IReportGenerator_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IReportGenerator_SetStorage(This,pDisp)	\
    (This)->lpVtbl -> SetStorage(This,pDisp)

#define IReportGenerator_SetStudy(This,pStudy)	\
    (This)->lpVtbl -> SetStudy(This,pStudy)

#define IReportGenerator_SetBuilder(This,pDisp)	\
    (This)->lpVtbl -> SetBuilder(This,pDisp)

#define IReportGenerator_SetTemplate(This,bstrTemplatePath)	\
    (This)->lpVtbl -> SetTemplate(This,bstrTemplatePath)

#define IReportGenerator_IsAvailable(This,pbAvailable)	\
    (This)->lpVtbl -> IsAvailable(This,pbAvailable)

#define IReportGenerator_GetDocumentTypeID(This,pbstrID)	\
    (This)->lpVtbl -> GetDocumentTypeID(This,pbstrID)

#define IReportGenerator_GetDocumentTitle(This,pbstrTitle)	\
    (This)->lpVtbl -> GetDocumentTitle(This,pbstrTitle)

#define IReportGenerator_GetExtension(This,pbstrExt)	\
    (This)->lpVtbl -> GetExtension(This,pbstrExt)

#define IReportGenerator_GetFileNameBase(This,pbstrBase)	\
    (This)->lpVtbl -> GetFileNameBase(This,pbstrBase)

#define IReportGenerator_GenerateReport(This,bstrPath)	\
    (This)->lpVtbl -> GenerateReport(This,bstrPath)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring] */ HRESULT STDMETHODCALLTYPE IReportGenerator_SetStorage_Proxy( 
    IReportGenerator * This,
    /* [in] */ IDispatch *pDisp);


void __RPC_STUB IReportGenerator_SetStorage_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IReportGenerator_SetStudy_Proxy( 
    IReportGenerator * This,
    /* [in] */ IDispatch *pStudy);


void __RPC_STUB IReportGenerator_SetStudy_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IReportGenerator_SetBuilder_Proxy( 
    IReportGenerator * This,
    /* [in] */ IDispatch *pDisp);


void __RPC_STUB IReportGenerator_SetBuilder_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IReportGenerator_SetTemplate_Proxy( 
    IReportGenerator * This,
    /* [in] */ BSTR bstrTemplatePath);


void __RPC_STUB IReportGenerator_SetTemplate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IReportGenerator_IsAvailable_Proxy( 
    IReportGenerator * This,
    /* [retval][out] */ VARIANT_BOOL *pbAvailable);


void __RPC_STUB IReportGenerator_IsAvailable_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IReportGenerator_GetDocumentTypeID_Proxy( 
    IReportGenerator * This,
    /* [retval][out] */ BSTR *pbstrID);


void __RPC_STUB IReportGenerator_GetDocumentTypeID_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IReportGenerator_GetDocumentTitle_Proxy( 
    IReportGenerator * This,
    /* [retval][out] */ BSTR *pbstrTitle);


void __RPC_STUB IReportGenerator_GetDocumentTitle_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IReportGenerator_GetExtension_Proxy( 
    IReportGenerator * This,
    /* [retval][out] */ BSTR *pbstrExt);


void __RPC_STUB IReportGenerator_GetExtension_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IReportGenerator_GetFileNameBase_Proxy( 
    IReportGenerator * This,
    /* [retval][out] */ BSTR *pbstrBase);


void __RPC_STUB IReportGenerator_GetFileNameBase_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IReportGenerator_GenerateReport_Proxy( 
    IReportGenerator * This,
    /* [in] */ BSTR bstrPath);


void __RPC_STUB IReportGenerator_GenerateReport_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IReportGenerator_INTERFACE_DEFINED__ */


#ifndef __ICalcNode_INTERFACE_DEFINED__
#define __ICalcNode_INTERFACE_DEFINED__

/* interface ICalcNode */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_ICalcNode;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("91AC64D5-7974-4289-9D12-864643332935")
    ICalcNode : public IUnknown
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE SetUniqueGUID( 
            /* [in] */ BSTR bstrUniqueGUID) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE SetParameters( 
            /* [in] */ BSTR bstrParameters) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE SetTimeResolution( 
            /* [in] */ double dTimeResolution) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ICalcNodeVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ICalcNode * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ICalcNode * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ICalcNode * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetUniqueGUID )( 
            ICalcNode * This,
            /* [in] */ BSTR bstrUniqueGUID);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetParameters )( 
            ICalcNode * This,
            /* [in] */ BSTR bstrParameters);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetTimeResolution )( 
            ICalcNode * This,
            /* [in] */ double dTimeResolution);
        
        END_INTERFACE
    } ICalcNodeVtbl;

    interface ICalcNode
    {
        CONST_VTBL struct ICalcNodeVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ICalcNode_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ICalcNode_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ICalcNode_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ICalcNode_SetUniqueGUID(This,bstrUniqueGUID)	\
    (This)->lpVtbl -> SetUniqueGUID(This,bstrUniqueGUID)

#define ICalcNode_SetParameters(This,bstrParameters)	\
    (This)->lpVtbl -> SetParameters(This,bstrParameters)

#define ICalcNode_SetTimeResolution(This,dTimeResolution)	\
    (This)->lpVtbl -> SetTimeResolution(This,dTimeResolution)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring] */ HRESULT STDMETHODCALLTYPE ICalcNode_SetUniqueGUID_Proxy( 
    ICalcNode * This,
    /* [in] */ BSTR bstrUniqueGUID);


void __RPC_STUB ICalcNode_SetUniqueGUID_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ICalcNode_SetParameters_Proxy( 
    ICalcNode * This,
    /* [in] */ BSTR bstrParameters);


void __RPC_STUB ICalcNode_SetParameters_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ICalcNode_SetTimeResolution_Proxy( 
    ICalcNode * This,
    /* [in] */ double dTimeResolution);


void __RPC_STUB ICalcNode_SetTimeResolution_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ICalcNode_INTERFACE_DEFINED__ */


#ifndef __IAcquisitionTime_INTERFACE_DEFINED__
#define __IAcquisitionTime_INTERFACE_DEFINED__

/* interface IAcquisitionTime */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IAcquisitionTime;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("CFE7CCDE-08F7-446c-BD23-42AB7F21F6F6")
    IAcquisitionTime : public IDispatch
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetAcqTime( 
            /* [out] */ double *pdDate,
            /* [out] */ double *pdFraction) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IAcquisitionTimeVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IAcquisitionTime * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IAcquisitionTime * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IAcquisitionTime * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IAcquisitionTime * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IAcquisitionTime * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IAcquisitionTime * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IAcquisitionTime * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetAcqTime )( 
            IAcquisitionTime * This,
            /* [out] */ double *pdDate,
            /* [out] */ double *pdFraction);
        
        END_INTERFACE
    } IAcquisitionTimeVtbl;

    interface IAcquisitionTime
    {
        CONST_VTBL struct IAcquisitionTimeVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IAcquisitionTime_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IAcquisitionTime_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IAcquisitionTime_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IAcquisitionTime_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IAcquisitionTime_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IAcquisitionTime_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IAcquisitionTime_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IAcquisitionTime_GetAcqTime(This,pdDate,pdFraction)	\
    (This)->lpVtbl -> GetAcqTime(This,pdDate,pdFraction)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring] */ HRESULT STDMETHODCALLTYPE IAcquisitionTime_GetAcqTime_Proxy( 
    IAcquisitionTime * This,
    /* [out] */ double *pdDate,
    /* [out] */ double *pdFraction);


void __RPC_STUB IAcquisitionTime_GetAcqTime_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IAcquisitionTime_INTERFACE_DEFINED__ */


#ifndef __IDetectionsUI_INTERFACE_DEFINED__
#define __IDetectionsUI_INTERFACE_DEFINED__

/* interface IDetectionsUI */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IDetectionsUI;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("D2FFF2DC-6BE8-470D-9425-9B837A5AE414")
    IDetectionsUI : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetDetection( 
            /* [in] */ IDispatch *pIDispatch,
            /* [in] */ BSTR bstrDetectionID) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDetectionsUIVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDetectionsUI * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDetectionsUI * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDetectionsUI * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDetectionsUI * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDetectionsUI * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDetectionsUI * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDetectionsUI * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetDetection )( 
            IDetectionsUI * This,
            /* [in] */ IDispatch *pIDispatch,
            /* [in] */ BSTR bstrDetectionID);
        
        END_INTERFACE
    } IDetectionsUIVtbl;

    interface IDetectionsUI
    {
        CONST_VTBL struct IDetectionsUIVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDetectionsUI_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IDetectionsUI_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IDetectionsUI_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IDetectionsUI_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IDetectionsUI_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IDetectionsUI_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IDetectionsUI_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IDetectionsUI_SetDetection(This,pIDispatch,bstrDetectionID)	\
    (This)->lpVtbl -> SetDetection(This,pIDispatch,bstrDetectionID)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IDetectionsUI_SetDetection_Proxy( 
    IDetectionsUI * This,
    /* [in] */ IDispatch *pIDispatch,
    /* [in] */ BSTR bstrDetectionID);


void __RPC_STUB IDetectionsUI_SetDetection_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IDetectionsUI_INTERFACE_DEFINED__ */


#ifndef __IAmplifier_INTERFACE_DEFINED__
#define __IAmplifier_INTERFACE_DEFINED__

/* interface IAmplifier */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IAmplifier;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("7144012C-E7B9-4e8f-8C0C-DA5E37A1C7EF")
    IAmplifier : public IAcquisitionTime
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE IsDeblockAvailable( 
            /* [retval][out] */ VARIANT_BOOL *pbAvail) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Deblock( void) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE IsImpedanceTest( 
            /* [retval][out] */ VARIANT_BOOL *pbImpTest) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE IsCalibrationAvailable( 
            /* [retval][out] */ VARIANT_BOOL *pbAvail) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE IsCalibrationMode( 
            /* [retval][out] */ VARIANT_BOOL *pbCal) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE CalibrationMode( 
            /* [in] */ VARIANT_BOOL bCalibration) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE SetCalibrationParameters( 
            /* [in] */ float fCalRate,
            /* [in] */ LONG lCalLevel) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE IsConnected( 
            /* [retval][out] */ VARIANT_BOOL *pbConn) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE SetNeutral( 
            /* [in] */ VARIANT_BOOL bOn) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE IsRunning( 
            /* [retval][out] */ VARIANT_BOOL *pbRunning) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE SetDigitalOut( 
            /* [in] */ LONG lOutput,
            /* [in] */ VARIANT_BOOL bHigh) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE SetRecLogics( 
            /* [in] */ IDispatch *pIRecLogics) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE CheckStatus( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IAmplifierVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IAmplifier * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IAmplifier * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IAmplifier * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IAmplifier * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IAmplifier * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IAmplifier * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IAmplifier * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetAcqTime )( 
            IAmplifier * This,
            /* [out] */ double *pdDate,
            /* [out] */ double *pdFraction);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *IsDeblockAvailable )( 
            IAmplifier * This,
            /* [retval][out] */ VARIANT_BOOL *pbAvail);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *Deblock )( 
            IAmplifier * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *IsImpedanceTest )( 
            IAmplifier * This,
            /* [retval][out] */ VARIANT_BOOL *pbImpTest);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *IsCalibrationAvailable )( 
            IAmplifier * This,
            /* [retval][out] */ VARIANT_BOOL *pbAvail);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *IsCalibrationMode )( 
            IAmplifier * This,
            /* [retval][out] */ VARIANT_BOOL *pbCal);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *CalibrationMode )( 
            IAmplifier * This,
            /* [in] */ VARIANT_BOOL bCalibration);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetCalibrationParameters )( 
            IAmplifier * This,
            /* [in] */ float fCalRate,
            /* [in] */ LONG lCalLevel);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *IsConnected )( 
            IAmplifier * This,
            /* [retval][out] */ VARIANT_BOOL *pbConn);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetNeutral )( 
            IAmplifier * This,
            /* [in] */ VARIANT_BOOL bOn);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *IsRunning )( 
            IAmplifier * This,
            /* [retval][out] */ VARIANT_BOOL *pbRunning);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetDigitalOut )( 
            IAmplifier * This,
            /* [in] */ LONG lOutput,
            /* [in] */ VARIANT_BOOL bHigh);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetRecLogics )( 
            IAmplifier * This,
            /* [in] */ IDispatch *pIRecLogics);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *CheckStatus )( 
            IAmplifier * This);
        
        END_INTERFACE
    } IAmplifierVtbl;

    interface IAmplifier
    {
        CONST_VTBL struct IAmplifierVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IAmplifier_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IAmplifier_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IAmplifier_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IAmplifier_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IAmplifier_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IAmplifier_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IAmplifier_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IAmplifier_GetAcqTime(This,pdDate,pdFraction)	\
    (This)->lpVtbl -> GetAcqTime(This,pdDate,pdFraction)


#define IAmplifier_IsDeblockAvailable(This,pbAvail)	\
    (This)->lpVtbl -> IsDeblockAvailable(This,pbAvail)

#define IAmplifier_Deblock(This)	\
    (This)->lpVtbl -> Deblock(This)

#define IAmplifier_IsImpedanceTest(This,pbImpTest)	\
    (This)->lpVtbl -> IsImpedanceTest(This,pbImpTest)

#define IAmplifier_IsCalibrationAvailable(This,pbAvail)	\
    (This)->lpVtbl -> IsCalibrationAvailable(This,pbAvail)

#define IAmplifier_IsCalibrationMode(This,pbCal)	\
    (This)->lpVtbl -> IsCalibrationMode(This,pbCal)

#define IAmplifier_CalibrationMode(This,bCalibration)	\
    (This)->lpVtbl -> CalibrationMode(This,bCalibration)

#define IAmplifier_SetCalibrationParameters(This,fCalRate,lCalLevel)	\
    (This)->lpVtbl -> SetCalibrationParameters(This,fCalRate,lCalLevel)

#define IAmplifier_IsConnected(This,pbConn)	\
    (This)->lpVtbl -> IsConnected(This,pbConn)

#define IAmplifier_SetNeutral(This,bOn)	\
    (This)->lpVtbl -> SetNeutral(This,bOn)

#define IAmplifier_IsRunning(This,pbRunning)	\
    (This)->lpVtbl -> IsRunning(This,pbRunning)

#define IAmplifier_SetDigitalOut(This,lOutput,bHigh)	\
    (This)->lpVtbl -> SetDigitalOut(This,lOutput,bHigh)

#define IAmplifier_SetRecLogics(This,pIRecLogics)	\
    (This)->lpVtbl -> SetRecLogics(This,pIRecLogics)

#define IAmplifier_CheckStatus(This)	\
    (This)->lpVtbl -> CheckStatus(This)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring] */ HRESULT STDMETHODCALLTYPE IAmplifier_IsDeblockAvailable_Proxy( 
    IAmplifier * This,
    /* [retval][out] */ VARIANT_BOOL *pbAvail);


void __RPC_STUB IAmplifier_IsDeblockAvailable_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IAmplifier_Deblock_Proxy( 
    IAmplifier * This);


void __RPC_STUB IAmplifier_Deblock_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IAmplifier_IsImpedanceTest_Proxy( 
    IAmplifier * This,
    /* [retval][out] */ VARIANT_BOOL *pbImpTest);


void __RPC_STUB IAmplifier_IsImpedanceTest_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IAmplifier_IsCalibrationAvailable_Proxy( 
    IAmplifier * This,
    /* [retval][out] */ VARIANT_BOOL *pbAvail);


void __RPC_STUB IAmplifier_IsCalibrationAvailable_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IAmplifier_IsCalibrationMode_Proxy( 
    IAmplifier * This,
    /* [retval][out] */ VARIANT_BOOL *pbCal);


void __RPC_STUB IAmplifier_IsCalibrationMode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IAmplifier_CalibrationMode_Proxy( 
    IAmplifier * This,
    /* [in] */ VARIANT_BOOL bCalibration);


void __RPC_STUB IAmplifier_CalibrationMode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IAmplifier_SetCalibrationParameters_Proxy( 
    IAmplifier * This,
    /* [in] */ float fCalRate,
    /* [in] */ LONG lCalLevel);


void __RPC_STUB IAmplifier_SetCalibrationParameters_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IAmplifier_IsConnected_Proxy( 
    IAmplifier * This,
    /* [retval][out] */ VARIANT_BOOL *pbConn);


void __RPC_STUB IAmplifier_IsConnected_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IAmplifier_SetNeutral_Proxy( 
    IAmplifier * This,
    /* [in] */ VARIANT_BOOL bOn);


void __RPC_STUB IAmplifier_SetNeutral_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IAmplifier_IsRunning_Proxy( 
    IAmplifier * This,
    /* [retval][out] */ VARIANT_BOOL *pbRunning);


void __RPC_STUB IAmplifier_IsRunning_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IAmplifier_SetDigitalOut_Proxy( 
    IAmplifier * This,
    /* [in] */ LONG lOutput,
    /* [in] */ VARIANT_BOOL bHigh);


void __RPC_STUB IAmplifier_SetDigitalOut_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IAmplifier_SetRecLogics_Proxy( 
    IAmplifier * This,
    /* [in] */ IDispatch *pIRecLogics);


void __RPC_STUB IAmplifier_SetRecLogics_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IAmplifier_CheckStatus_Proxy( 
    IAmplifier * This);


void __RPC_STUB IAmplifier_CheckStatus_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IAmplifier_INTERFACE_DEFINED__ */


#ifndef __IRulerSlave_INTERFACE_DEFINED__
#define __IRulerSlave_INTERFACE_DEFINED__

/* interface IRulerSlave */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IRulerSlave;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("DB1FE6E1-6797-11d3-90CE-00104B6F4F70")
    IRulerSlave : public IUnknown
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Seek( 
            /* [in] */ NRVDATE nrvDate) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE StopPlay( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IRulerSlaveVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IRulerSlave * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IRulerSlave * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IRulerSlave * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *Seek )( 
            IRulerSlave * This,
            /* [in] */ NRVDATE nrvDate);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *StopPlay )( 
            IRulerSlave * This);
        
        END_INTERFACE
    } IRulerSlaveVtbl;

    interface IRulerSlave
    {
        CONST_VTBL struct IRulerSlaveVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IRulerSlave_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IRulerSlave_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IRulerSlave_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IRulerSlave_Seek(This,nrvDate)	\
    (This)->lpVtbl -> Seek(This,nrvDate)

#define IRulerSlave_StopPlay(This)	\
    (This)->lpVtbl -> StopPlay(This)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring] */ HRESULT STDMETHODCALLTYPE IRulerSlave_Seek_Proxy( 
    IRulerSlave * This,
    /* [in] */ NRVDATE nrvDate);


void __RPC_STUB IRulerSlave_Seek_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IRulerSlave_StopPlay_Proxy( 
    IRulerSlave * This);


void __RPC_STUB IRulerSlave_StopPlay_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IRulerSlave_INTERFACE_DEFINED__ */


#ifndef __IEventNotify_INTERFACE_DEFINED__
#define __IEventNotify_INTERFACE_DEFINED__

/* interface IEventNotify */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IEventNotify;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("4924443B-4203-4ba8-8641-924C46D1270C")
    IEventNotify : public IDispatch
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE NotifyEventChange( 
            /* [in] */ IDispatch *pIUserEvent,
            /* [in] */ LONG lHint,
            /* [in] */ VARIANT_BOOL bLive) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE NotifyReset( 
            /* [in] */ IDispatch *pIUserEvents) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE NotifyEventTypes( 
            /* [in] */ IDispatch *pIEventCache) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IEventNotifyVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IEventNotify * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IEventNotify * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IEventNotify * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IEventNotify * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IEventNotify * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IEventNotify * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IEventNotify * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *NotifyEventChange )( 
            IEventNotify * This,
            /* [in] */ IDispatch *pIUserEvent,
            /* [in] */ LONG lHint,
            /* [in] */ VARIANT_BOOL bLive);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *NotifyReset )( 
            IEventNotify * This,
            /* [in] */ IDispatch *pIUserEvents);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *NotifyEventTypes )( 
            IEventNotify * This,
            /* [in] */ IDispatch *pIEventCache);
        
        END_INTERFACE
    } IEventNotifyVtbl;

    interface IEventNotify
    {
        CONST_VTBL struct IEventNotifyVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IEventNotify_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IEventNotify_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IEventNotify_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IEventNotify_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IEventNotify_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IEventNotify_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IEventNotify_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IEventNotify_NotifyEventChange(This,pIUserEvent,lHint,bLive)	\
    (This)->lpVtbl -> NotifyEventChange(This,pIUserEvent,lHint,bLive)

#define IEventNotify_NotifyReset(This,pIUserEvents)	\
    (This)->lpVtbl -> NotifyReset(This,pIUserEvents)

#define IEventNotify_NotifyEventTypes(This,pIEventCache)	\
    (This)->lpVtbl -> NotifyEventTypes(This,pIEventCache)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring] */ HRESULT STDMETHODCALLTYPE IEventNotify_NotifyEventChange_Proxy( 
    IEventNotify * This,
    /* [in] */ IDispatch *pIUserEvent,
    /* [in] */ LONG lHint,
    /* [in] */ VARIANT_BOOL bLive);


void __RPC_STUB IEventNotify_NotifyEventChange_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IEventNotify_NotifyReset_Proxy( 
    IEventNotify * This,
    /* [in] */ IDispatch *pIUserEvents);


void __RPC_STUB IEventNotify_NotifyReset_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IEventNotify_NotifyEventTypes_Proxy( 
    IEventNotify * This,
    /* [in] */ IDispatch *pIEventCache);


void __RPC_STUB IEventNotify_NotifyEventTypes_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IEventNotify_INTERFACE_DEFINED__ */


#ifndef __IEventFilter_INTERFACE_DEFINED__
#define __IEventFilter_INTERFACE_DEFINED__

/* interface IEventFilter */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IEventFilter;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("E9AC575D-CF4C-4A17-A7EF-2C15C46EE33F")
    IEventFilter : public IUnknown
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Filter( 
            /* [in] */ BYTE *pPacket,
            /* [retval][out] */ VARIANT_BOOL *pbIs) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IEventFilterVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IEventFilter * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IEventFilter * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IEventFilter * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *Filter )( 
            IEventFilter * This,
            /* [in] */ BYTE *pPacket,
            /* [retval][out] */ VARIANT_BOOL *pbIs);
        
        END_INTERFACE
    } IEventFilterVtbl;

    interface IEventFilter
    {
        CONST_VTBL struct IEventFilterVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IEventFilter_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IEventFilter_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IEventFilter_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IEventFilter_Filter(This,pPacket,pbIs)	\
    (This)->lpVtbl -> Filter(This,pPacket,pbIs)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring] */ HRESULT STDMETHODCALLTYPE IEventFilter_Filter_Proxy( 
    IEventFilter * This,
    /* [in] */ BYTE *pPacket,
    /* [retval][out] */ VARIANT_BOOL *pbIs);


void __RPC_STUB IEventFilter_Filter_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IEventFilter_INTERFACE_DEFINED__ */


#ifndef __IProtocolClient_INTERFACE_DEFINED__
#define __IProtocolClient_INTERFACE_DEFINED__

/* interface IProtocolClient */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IProtocolClient;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("9816547B-9AC5-480d-9DC4-B3CF19BAF1ED")
    IProtocolClient : public IUnknown
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE ProtocolChanging( 
            /* [in] */ DWORD dwHint,
            /* [in] */ IUnknown *piUnk) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE ProtocolChanged( 
            /* [in] */ DWORD dwHint,
            /* [in] */ IUnknown *piUnk) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IProtocolClientVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IProtocolClient * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IProtocolClient * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IProtocolClient * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *ProtocolChanging )( 
            IProtocolClient * This,
            /* [in] */ DWORD dwHint,
            /* [in] */ IUnknown *piUnk);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *ProtocolChanged )( 
            IProtocolClient * This,
            /* [in] */ DWORD dwHint,
            /* [in] */ IUnknown *piUnk);
        
        END_INTERFACE
    } IProtocolClientVtbl;

    interface IProtocolClient
    {
        CONST_VTBL struct IProtocolClientVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IProtocolClient_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IProtocolClient_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IProtocolClient_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IProtocolClient_ProtocolChanging(This,dwHint,piUnk)	\
    (This)->lpVtbl -> ProtocolChanging(This,dwHint,piUnk)

#define IProtocolClient_ProtocolChanged(This,dwHint,piUnk)	\
    (This)->lpVtbl -> ProtocolChanged(This,dwHint,piUnk)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring] */ HRESULT STDMETHODCALLTYPE IProtocolClient_ProtocolChanging_Proxy( 
    IProtocolClient * This,
    /* [in] */ DWORD dwHint,
    /* [in] */ IUnknown *piUnk);


void __RPC_STUB IProtocolClient_ProtocolChanging_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IProtocolClient_ProtocolChanged_Proxy( 
    IProtocolClient * This,
    /* [in] */ DWORD dwHint,
    /* [in] */ IUnknown *piUnk);


void __RPC_STUB IProtocolClient_ProtocolChanged_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IProtocolClient_INTERFACE_DEFINED__ */


#ifndef __IQuerySize_INTERFACE_DEFINED__
#define __IQuerySize_INTERFACE_DEFINED__

/* interface IQuerySize */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IQuerySize;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("BE042193-C83D-11d3-9FB4-00104B6F5247")
    IQuerySize : public IUnknown
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE QueryWantedSize( 
            /* [out] */ DWORD *pdwCx,
            /* [out] */ DWORD *pdwCy) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IQuerySizeVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IQuerySize * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IQuerySize * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IQuerySize * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *QueryWantedSize )( 
            IQuerySize * This,
            /* [out] */ DWORD *pdwCx,
            /* [out] */ DWORD *pdwCy);
        
        END_INTERFACE
    } IQuerySizeVtbl;

    interface IQuerySize
    {
        CONST_VTBL struct IQuerySizeVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IQuerySize_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IQuerySize_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IQuerySize_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IQuerySize_QueryWantedSize(This,pdwCx,pdwCy)	\
    (This)->lpVtbl -> QueryWantedSize(This,pdwCx,pdwCy)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring] */ HRESULT STDMETHODCALLTYPE IQuerySize_QueryWantedSize_Proxy( 
    IQuerySize * This,
    /* [out] */ DWORD *pdwCx,
    /* [out] */ DWORD *pdwCy);


void __RPC_STUB IQuerySize_QueryWantedSize_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IQuerySize_INTERFACE_DEFINED__ */


#ifndef __IEditor_INTERFACE_DEFINED__
#define __IEditor_INTERFACE_DEFINED__

/* interface IEditor */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IEditor;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("EEC31098-5AC7-48ff-8358-4E771EF14EA3")
    IEditor : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Initialize( 
            /* [in] */ IDispatch *pIDisp,
            /* [in] */ VARIANT_BOOL bEnableApply) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE QueryClose( 
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IEditorVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IEditor * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IEditor * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IEditor * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IEditor * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IEditor * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IEditor * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IEditor * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Initialize )( 
            IEditor * This,
            /* [in] */ IDispatch *pIDisp,
            /* [in] */ VARIANT_BOOL bEnableApply);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *QueryClose )( 
            IEditor * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        END_INTERFACE
    } IEditorVtbl;

    interface IEditor
    {
        CONST_VTBL struct IEditorVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IEditor_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IEditor_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IEditor_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IEditor_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IEditor_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IEditor_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IEditor_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IEditor_Initialize(This,pIDisp,bEnableApply)	\
    (This)->lpVtbl -> Initialize(This,pIDisp,bEnableApply)

#define IEditor_QueryClose(This,pVal)	\
    (This)->lpVtbl -> QueryClose(This,pVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IEditor_Initialize_Proxy( 
    IEditor * This,
    /* [in] */ IDispatch *pIDisp,
    /* [in] */ VARIANT_BOOL bEnableApply);


void __RPC_STUB IEditor_Initialize_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IEditor_QueryClose_Proxy( 
    IEditor * This,
    /* [retval][out] */ VARIANT_BOOL *pVal);


void __RPC_STUB IEditor_QueryClose_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IEditor_INTERFACE_DEFINED__ */


#ifndef __IProgressSinkOld_INTERFACE_DEFINED__
#define __IProgressSinkOld_INTERFACE_DEFINED__

/* interface IProgressSinkOld */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IProgressSinkOld;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("44A75D7C-D9D8-11d1-8483-0800091B5BDA")
    IProgressSinkOld : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetSteps( 
            /* [in] */ long lSteps) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Step( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Info( 
            /* [in] */ long lType,
            /* [in] */ BSTR bstrInfo) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IProgressSinkOldVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IProgressSinkOld * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IProgressSinkOld * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IProgressSinkOld * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IProgressSinkOld * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IProgressSinkOld * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IProgressSinkOld * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IProgressSinkOld * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetSteps )( 
            IProgressSinkOld * This,
            /* [in] */ long lSteps);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Step )( 
            IProgressSinkOld * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Info )( 
            IProgressSinkOld * This,
            /* [in] */ long lType,
            /* [in] */ BSTR bstrInfo);
        
        END_INTERFACE
    } IProgressSinkOldVtbl;

    interface IProgressSinkOld
    {
        CONST_VTBL struct IProgressSinkOldVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IProgressSinkOld_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IProgressSinkOld_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IProgressSinkOld_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IProgressSinkOld_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IProgressSinkOld_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IProgressSinkOld_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IProgressSinkOld_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IProgressSinkOld_SetSteps(This,lSteps)	\
    (This)->lpVtbl -> SetSteps(This,lSteps)

#define IProgressSinkOld_Step(This)	\
    (This)->lpVtbl -> Step(This)

#define IProgressSinkOld_Info(This,lType,bstrInfo)	\
    (This)->lpVtbl -> Info(This,lType,bstrInfo)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IProgressSinkOld_SetSteps_Proxy( 
    IProgressSinkOld * This,
    /* [in] */ long lSteps);


void __RPC_STUB IProgressSinkOld_SetSteps_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IProgressSinkOld_Step_Proxy( 
    IProgressSinkOld * This);


void __RPC_STUB IProgressSinkOld_Step_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IProgressSinkOld_Info_Proxy( 
    IProgressSinkOld * This,
    /* [in] */ long lType,
    /* [in] */ BSTR bstrInfo);


void __RPC_STUB IProgressSinkOld_Info_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IProgressSinkOld_INTERFACE_DEFINED__ */


#ifndef __IProgressSink_INTERFACE_DEFINED__
#define __IProgressSink_INTERFACE_DEFINED__

/* interface IProgressSink */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IProgressSink;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("78CF4418-5F95-4bd9-8BE6-316D630C7CE2")
    IProgressSink : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetSteps( 
            /* [in] */ long lSteps) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Step( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Info( 
            /* [in] */ long lType,
            /* [in] */ BSTR bstrInfo,
            /* [in] */ IDispatch *pStudy) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IProgressSinkVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IProgressSink * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IProgressSink * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IProgressSink * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IProgressSink * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IProgressSink * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IProgressSink * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IProgressSink * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetSteps )( 
            IProgressSink * This,
            /* [in] */ long lSteps);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Step )( 
            IProgressSink * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Info )( 
            IProgressSink * This,
            /* [in] */ long lType,
            /* [in] */ BSTR bstrInfo,
            /* [in] */ IDispatch *pStudy);
        
        END_INTERFACE
    } IProgressSinkVtbl;

    interface IProgressSink
    {
        CONST_VTBL struct IProgressSinkVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IProgressSink_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IProgressSink_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IProgressSink_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IProgressSink_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IProgressSink_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IProgressSink_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IProgressSink_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IProgressSink_SetSteps(This,lSteps)	\
    (This)->lpVtbl -> SetSteps(This,lSteps)

#define IProgressSink_Step(This)	\
    (This)->lpVtbl -> Step(This)

#define IProgressSink_Info(This,lType,bstrInfo,pStudy)	\
    (This)->lpVtbl -> Info(This,lType,bstrInfo,pStudy)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IProgressSink_SetSteps_Proxy( 
    IProgressSink * This,
    /* [in] */ long lSteps);


void __RPC_STUB IProgressSink_SetSteps_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IProgressSink_Step_Proxy( 
    IProgressSink * This);


void __RPC_STUB IProgressSink_Step_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IProgressSink_Info_Proxy( 
    IProgressSink * This,
    /* [in] */ long lType,
    /* [in] */ BSTR bstrInfo,
    /* [in] */ IDispatch *pStudy);


void __RPC_STUB IProgressSink_Info_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IProgressSink_INTERFACE_DEFINED__ */


#ifndef __IPagerNotify_INTERFACE_DEFINED__
#define __IPagerNotify_INTERFACE_DEFINED__

/* interface IPagerNotify */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IPagerNotify;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("F281F19B-0EE1-46EA-8826-A4746398F306")
    IPagerNotify : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE NotifyPagerSubscriber( 
            /* [in] */ VARIANT varArgument) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IPagerNotifyVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IPagerNotify * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IPagerNotify * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IPagerNotify * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IPagerNotify * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IPagerNotify * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IPagerNotify * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IPagerNotify * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *NotifyPagerSubscriber )( 
            IPagerNotify * This,
            /* [in] */ VARIANT varArgument);
        
        END_INTERFACE
    } IPagerNotifyVtbl;

    interface IPagerNotify
    {
        CONST_VTBL struct IPagerNotifyVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPagerNotify_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IPagerNotify_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IPagerNotify_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IPagerNotify_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IPagerNotify_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IPagerNotify_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IPagerNotify_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IPagerNotify_NotifyPagerSubscriber(This,varArgument)	\
    (This)->lpVtbl -> NotifyPagerSubscriber(This,varArgument)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPagerNotify_NotifyPagerSubscriber_Proxy( 
    IPagerNotify * This,
    /* [in] */ VARIANT varArgument);


void __RPC_STUB IPagerNotify_NotifyPagerSubscriber_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IPagerNotify_INTERFACE_DEFINED__ */


#ifndef __IStorageObserver_INTERFACE_DEFINED__
#define __IStorageObserver_INTERFACE_DEFINED__

/* interface IStorageObserver */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IStorageObserver;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("6EF5C1AC-E3DA-4bfb-B6D5-6AE12D6A3B46")
    IStorageObserver : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE NotifyOpen( 
            /* [in] */ IDispatch *pIStorageMan) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE NotifyPrune( 
            /* [in] */ VARIANT_BOOL bStarting) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IStorageObserverVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IStorageObserver * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IStorageObserver * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IStorageObserver * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IStorageObserver * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IStorageObserver * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IStorageObserver * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IStorageObserver * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *NotifyOpen )( 
            IStorageObserver * This,
            /* [in] */ IDispatch *pIStorageMan);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *NotifyPrune )( 
            IStorageObserver * This,
            /* [in] */ VARIANT_BOOL bStarting);
        
        END_INTERFACE
    } IStorageObserverVtbl;

    interface IStorageObserver
    {
        CONST_VTBL struct IStorageObserverVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IStorageObserver_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IStorageObserver_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IStorageObserver_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IStorageObserver_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IStorageObserver_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IStorageObserver_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IStorageObserver_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IStorageObserver_NotifyOpen(This,pIStorageMan)	\
    (This)->lpVtbl -> NotifyOpen(This,pIStorageMan)

#define IStorageObserver_NotifyPrune(This,bStarting)	\
    (This)->lpVtbl -> NotifyPrune(This,bStarting)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IStorageObserver_NotifyOpen_Proxy( 
    IStorageObserver * This,
    /* [in] */ IDispatch *pIStorageMan);


void __RPC_STUB IStorageObserver_NotifyOpen_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IStorageObserver_NotifyPrune_Proxy( 
    IStorageObserver * This,
    /* [in] */ VARIANT_BOOL bStarting);


void __RPC_STUB IStorageObserver_NotifyPrune_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IStorageObserver_INTERFACE_DEFINED__ */


#ifndef __IDetectionNode_INTERFACE_DEFINED__
#define __IDetectionNode_INTERFACE_DEFINED__

/* interface IDetectionNode */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IDetectionNode;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("4417C4A6-B221-47d4-A722-53AB0612EC51")
    IDetectionNode : public IUnknown
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE SetParams( 
            /* [in] */ IDispatch *pIDetectionTemplate,
            /* [in] */ long liElement) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE SetPatient( 
            /* [in] */ IDispatch *pIPatient) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDetectionNodeVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDetectionNode * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDetectionNode * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDetectionNode * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetParams )( 
            IDetectionNode * This,
            /* [in] */ IDispatch *pIDetectionTemplate,
            /* [in] */ long liElement);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetPatient )( 
            IDetectionNode * This,
            /* [in] */ IDispatch *pIPatient);
        
        END_INTERFACE
    } IDetectionNodeVtbl;

    interface IDetectionNode
    {
        CONST_VTBL struct IDetectionNodeVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDetectionNode_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IDetectionNode_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IDetectionNode_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IDetectionNode_SetParams(This,pIDetectionTemplate,liElement)	\
    (This)->lpVtbl -> SetParams(This,pIDetectionTemplate,liElement)

#define IDetectionNode_SetPatient(This,pIPatient)	\
    (This)->lpVtbl -> SetPatient(This,pIPatient)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring] */ HRESULT STDMETHODCALLTYPE IDetectionNode_SetParams_Proxy( 
    IDetectionNode * This,
    /* [in] */ IDispatch *pIDetectionTemplate,
    /* [in] */ long liElement);


void __RPC_STUB IDetectionNode_SetParams_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IDetectionNode_SetPatient_Proxy( 
    IDetectionNode * This,
    /* [in] */ IDispatch *pIPatient);


void __RPC_STUB IDetectionNode_SetPatient_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IDetectionNode_INTERFACE_DEFINED__ */


#ifndef __IConnectNotify_INTERFACE_DEFINED__
#define __IConnectNotify_INTERFACE_DEFINED__

/* interface IConnectNotify */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IConnectNotify;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("173E7B73-F5F3-47bb-A4AE-84DF478B8104")
    IConnectNotify : public IDispatch
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE NotifyConnected( void) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE NotifyConnectionClosed( 
            /* [in] */ BSTR bstrCause) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IConnectNotifyVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IConnectNotify * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IConnectNotify * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IConnectNotify * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IConnectNotify * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IConnectNotify * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IConnectNotify * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IConnectNotify * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *NotifyConnected )( 
            IConnectNotify * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *NotifyConnectionClosed )( 
            IConnectNotify * This,
            /* [in] */ BSTR bstrCause);
        
        END_INTERFACE
    } IConnectNotifyVtbl;

    interface IConnectNotify
    {
        CONST_VTBL struct IConnectNotifyVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IConnectNotify_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IConnectNotify_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IConnectNotify_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IConnectNotify_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IConnectNotify_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IConnectNotify_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IConnectNotify_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IConnectNotify_NotifyConnected(This)	\
    (This)->lpVtbl -> NotifyConnected(This)

#define IConnectNotify_NotifyConnectionClosed(This,bstrCause)	\
    (This)->lpVtbl -> NotifyConnectionClosed(This,bstrCause)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring] */ HRESULT STDMETHODCALLTYPE IConnectNotify_NotifyConnected_Proxy( 
    IConnectNotify * This);


void __RPC_STUB IConnectNotify_NotifyConnected_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IConnectNotify_NotifyConnectionClosed_Proxy( 
    IConnectNotify * This,
    /* [in] */ BSTR bstrCause);


void __RPC_STUB IConnectNotify_NotifyConnectionClosed_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IConnectNotify_INTERFACE_DEFINED__ */


#ifndef __ICacheObserver_INTERFACE_DEFINED__
#define __ICacheObserver_INTERFACE_DEFINED__

/* interface ICacheObserver */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_ICacheObserver;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("0936EDEE-0DE1-4092-B052-108FF200F4B4")
    ICacheObserver : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE NotifyCacheChanging( 
            /* [in] */ IDispatch *pDispCache,
            /* [in] */ DWORD dwHint) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE NotifyCacheChanged( 
            /* [in] */ IDispatch *pDispCache,
            /* [in] */ DWORD dwHint) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE NotifyCacheSaved( 
            /* [in] */ IDispatch *pDispCache,
            /* [in] */ DWORD dwHint) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ICacheObserverVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ICacheObserver * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ICacheObserver * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ICacheObserver * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ICacheObserver * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ICacheObserver * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ICacheObserver * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ICacheObserver * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *NotifyCacheChanging )( 
            ICacheObserver * This,
            /* [in] */ IDispatch *pDispCache,
            /* [in] */ DWORD dwHint);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *NotifyCacheChanged )( 
            ICacheObserver * This,
            /* [in] */ IDispatch *pDispCache,
            /* [in] */ DWORD dwHint);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *NotifyCacheSaved )( 
            ICacheObserver * This,
            /* [in] */ IDispatch *pDispCache,
            /* [in] */ DWORD dwHint);
        
        END_INTERFACE
    } ICacheObserverVtbl;

    interface ICacheObserver
    {
        CONST_VTBL struct ICacheObserverVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ICacheObserver_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ICacheObserver_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ICacheObserver_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ICacheObserver_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define ICacheObserver_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define ICacheObserver_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define ICacheObserver_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define ICacheObserver_NotifyCacheChanging(This,pDispCache,dwHint)	\
    (This)->lpVtbl -> NotifyCacheChanging(This,pDispCache,dwHint)

#define ICacheObserver_NotifyCacheChanged(This,pDispCache,dwHint)	\
    (This)->lpVtbl -> NotifyCacheChanged(This,pDispCache,dwHint)

#define ICacheObserver_NotifyCacheSaved(This,pDispCache,dwHint)	\
    (This)->lpVtbl -> NotifyCacheSaved(This,pDispCache,dwHint)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ICacheObserver_NotifyCacheChanging_Proxy( 
    ICacheObserver * This,
    /* [in] */ IDispatch *pDispCache,
    /* [in] */ DWORD dwHint);


void __RPC_STUB ICacheObserver_NotifyCacheChanging_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ICacheObserver_NotifyCacheChanged_Proxy( 
    ICacheObserver * This,
    /* [in] */ IDispatch *pDispCache,
    /* [in] */ DWORD dwHint);


void __RPC_STUB ICacheObserver_NotifyCacheChanged_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ICacheObserver_NotifyCacheSaved_Proxy( 
    ICacheObserver * This,
    /* [in] */ IDispatch *pDispCache,
    /* [in] */ DWORD dwHint);


void __RPC_STUB ICacheObserver_NotifyCacheSaved_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ICacheObserver_INTERFACE_DEFINED__ */


#ifndef __IEventHeight_INTERFACE_DEFINED__
#define __IEventHeight_INTERFACE_DEFINED__

/* interface IEventHeight */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IEventHeight;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("337A9472-25AE-4834-9E27-E9039FA351A9")
    IEventHeight : public IDispatch
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE SetScreen( 
            /* [in] */ long lWidthMM,
            /* [in] */ long lHeightMM) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE SetEventHeight( 
            /* [in] */ double dEventHeight) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IEventHeightVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IEventHeight * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IEventHeight * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IEventHeight * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IEventHeight * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IEventHeight * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IEventHeight * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IEventHeight * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetScreen )( 
            IEventHeight * This,
            /* [in] */ long lWidthMM,
            /* [in] */ long lHeightMM);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetEventHeight )( 
            IEventHeight * This,
            /* [in] */ double dEventHeight);
        
        END_INTERFACE
    } IEventHeightVtbl;

    interface IEventHeight
    {
        CONST_VTBL struct IEventHeightVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IEventHeight_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IEventHeight_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IEventHeight_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IEventHeight_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IEventHeight_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IEventHeight_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IEventHeight_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IEventHeight_SetScreen(This,lWidthMM,lHeightMM)	\
    (This)->lpVtbl -> SetScreen(This,lWidthMM,lHeightMM)

#define IEventHeight_SetEventHeight(This,dEventHeight)	\
    (This)->lpVtbl -> SetEventHeight(This,dEventHeight)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring] */ HRESULT STDMETHODCALLTYPE IEventHeight_SetScreen_Proxy( 
    IEventHeight * This,
    /* [in] */ long lWidthMM,
    /* [in] */ long lHeightMM);


void __RPC_STUB IEventHeight_SetScreen_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IEventHeight_SetEventHeight_Proxy( 
    IEventHeight * This,
    /* [in] */ double dEventHeight);


void __RPC_STUB IEventHeight_SetEventHeight_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IEventHeight_INTERFACE_DEFINED__ */


#ifndef __IAlarmObserver_INTERFACE_DEFINED__
#define __IAlarmObserver_INTERFACE_DEFINED__

/* interface IAlarmObserver */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IAlarmObserver;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("80520EC8-C803-4cbf-8593-866A22BCF540")
    IAlarmObserver : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Alarm( 
            /* [in] */ IDispatch *pEventDisp,
            /* [in] */ BSTR bstrStudyID,
            /* [in] */ VARIANT_BOOL bOpenDuration) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE AlarmEnd( 
            /* [in] */ IDispatch *pEventDisp,
            BSTR bstrStudyID) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IAlarmObserverVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IAlarmObserver * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IAlarmObserver * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IAlarmObserver * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IAlarmObserver * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IAlarmObserver * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IAlarmObserver * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IAlarmObserver * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Alarm )( 
            IAlarmObserver * This,
            /* [in] */ IDispatch *pEventDisp,
            /* [in] */ BSTR bstrStudyID,
            /* [in] */ VARIANT_BOOL bOpenDuration);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *AlarmEnd )( 
            IAlarmObserver * This,
            /* [in] */ IDispatch *pEventDisp,
            BSTR bstrStudyID);
        
        END_INTERFACE
    } IAlarmObserverVtbl;

    interface IAlarmObserver
    {
        CONST_VTBL struct IAlarmObserverVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IAlarmObserver_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IAlarmObserver_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IAlarmObserver_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IAlarmObserver_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IAlarmObserver_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IAlarmObserver_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IAlarmObserver_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IAlarmObserver_Alarm(This,pEventDisp,bstrStudyID,bOpenDuration)	\
    (This)->lpVtbl -> Alarm(This,pEventDisp,bstrStudyID,bOpenDuration)

#define IAlarmObserver_AlarmEnd(This,pEventDisp,bstrStudyID)	\
    (This)->lpVtbl -> AlarmEnd(This,pEventDisp,bstrStudyID)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IAlarmObserver_Alarm_Proxy( 
    IAlarmObserver * This,
    /* [in] */ IDispatch *pEventDisp,
    /* [in] */ BSTR bstrStudyID,
    /* [in] */ VARIANT_BOOL bOpenDuration);


void __RPC_STUB IAlarmObserver_Alarm_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IAlarmObserver_AlarmEnd_Proxy( 
    IAlarmObserver * This,
    /* [in] */ IDispatch *pEventDisp,
    BSTR bstrStudyID);


void __RPC_STUB IAlarmObserver_AlarmEnd_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IAlarmObserver_INTERFACE_DEFINED__ */


#ifndef __ITrendExport2_INTERFACE_DEFINED__
#define __ITrendExport2_INTERFACE_DEFINED__

/* interface ITrendExport2 */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_ITrendExport2;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("12D4D09F-B7E9-42dc-8273-4D8792B97CA3")
    ITrendExport2 : public IUnknown
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetInfo( 
            /* [in] */ BSTR bstrLabel,
            /* [out] */ long *plcPoints,
            /* [out] */ long *plcSeries,
            /* [out] */ VARIANT_BOOL *pbLogScale,
            /* [out] */ BSTR *bstrColDesc) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetTimeStamps( 
            /* [in] */ BSTR bstrLabel,
            /* [out] */ long *plBufSize,
            /* [size_is][size_is][out] */ double **ppdData) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetValues( 
            /* [in] */ BSTR bstrLabel,
            /* [out] */ long *plBufSize,
            /* [size_is][size_is][out] */ float **ppfData) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ITrendExport2Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ITrendExport2 * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ITrendExport2 * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ITrendExport2 * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetInfo )( 
            ITrendExport2 * This,
            /* [in] */ BSTR bstrLabel,
            /* [out] */ long *plcPoints,
            /* [out] */ long *plcSeries,
            /* [out] */ VARIANT_BOOL *pbLogScale,
            /* [out] */ BSTR *bstrColDesc);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetTimeStamps )( 
            ITrendExport2 * This,
            /* [in] */ BSTR bstrLabel,
            /* [out] */ long *plBufSize,
            /* [size_is][size_is][out] */ double **ppdData);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetValues )( 
            ITrendExport2 * This,
            /* [in] */ BSTR bstrLabel,
            /* [out] */ long *plBufSize,
            /* [size_is][size_is][out] */ float **ppfData);
        
        END_INTERFACE
    } ITrendExport2Vtbl;

    interface ITrendExport2
    {
        CONST_VTBL struct ITrendExport2Vtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ITrendExport2_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ITrendExport2_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ITrendExport2_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ITrendExport2_GetInfo(This,bstrLabel,plcPoints,plcSeries,pbLogScale,bstrColDesc)	\
    (This)->lpVtbl -> GetInfo(This,bstrLabel,plcPoints,plcSeries,pbLogScale,bstrColDesc)

#define ITrendExport2_GetTimeStamps(This,bstrLabel,plBufSize,ppdData)	\
    (This)->lpVtbl -> GetTimeStamps(This,bstrLabel,plBufSize,ppdData)

#define ITrendExport2_GetValues(This,bstrLabel,plBufSize,ppfData)	\
    (This)->lpVtbl -> GetValues(This,bstrLabel,plBufSize,ppfData)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring] */ HRESULT STDMETHODCALLTYPE ITrendExport2_GetInfo_Proxy( 
    ITrendExport2 * This,
    /* [in] */ BSTR bstrLabel,
    /* [out] */ long *plcPoints,
    /* [out] */ long *plcSeries,
    /* [out] */ VARIANT_BOOL *pbLogScale,
    /* [out] */ BSTR *bstrColDesc);


void __RPC_STUB ITrendExport2_GetInfo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ITrendExport2_GetTimeStamps_Proxy( 
    ITrendExport2 * This,
    /* [in] */ BSTR bstrLabel,
    /* [out] */ long *plBufSize,
    /* [size_is][size_is][out] */ double **ppdData);


void __RPC_STUB ITrendExport2_GetTimeStamps_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ITrendExport2_GetValues_Proxy( 
    ITrendExport2 * This,
    /* [in] */ BSTR bstrLabel,
    /* [out] */ long *plBufSize,
    /* [size_is][size_is][out] */ float **ppfData);


void __RPC_STUB ITrendExport2_GetValues_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ITrendExport2_INTERFACE_DEFINED__ */


#ifndef __IAlarmLogics_INTERFACE_DEFINED__
#define __IAlarmLogics_INTERFACE_DEFINED__

/* interface IAlarmLogics */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IAlarmLogics;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("71DD2C46-522C-4521-B73B-CBA96E69D5AA")
    IAlarmLogics : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE AddObserver( 
            /* [in] */ IDispatch *pObserverDisp) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE RemoveObserver( 
            /* [in] */ IDispatch *pObserverDisp) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IAlarmLogicsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IAlarmLogics * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IAlarmLogics * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IAlarmLogics * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IAlarmLogics * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IAlarmLogics * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IAlarmLogics * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IAlarmLogics * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *AddObserver )( 
            IAlarmLogics * This,
            /* [in] */ IDispatch *pObserverDisp);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *RemoveObserver )( 
            IAlarmLogics * This,
            /* [in] */ IDispatch *pObserverDisp);
        
        END_INTERFACE
    } IAlarmLogicsVtbl;

    interface IAlarmLogics
    {
        CONST_VTBL struct IAlarmLogicsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IAlarmLogics_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IAlarmLogics_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IAlarmLogics_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IAlarmLogics_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IAlarmLogics_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IAlarmLogics_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IAlarmLogics_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IAlarmLogics_AddObserver(This,pObserverDisp)	\
    (This)->lpVtbl -> AddObserver(This,pObserverDisp)

#define IAlarmLogics_RemoveObserver(This,pObserverDisp)	\
    (This)->lpVtbl -> RemoveObserver(This,pObserverDisp)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IAlarmLogics_AddObserver_Proxy( 
    IAlarmLogics * This,
    /* [in] */ IDispatch *pObserverDisp);


void __RPC_STUB IAlarmLogics_AddObserver_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IAlarmLogics_RemoveObserver_Proxy( 
    IAlarmLogics * This,
    /* [in] */ IDispatch *pObserverDisp);


void __RPC_STUB IAlarmLogics_RemoveObserver_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IAlarmLogics_INTERFACE_DEFINED__ */


#ifndef __ISleepNotifySubscriber_INTERFACE_DEFINED__
#define __ISleepNotifySubscriber_INTERFACE_DEFINED__

/* interface ISleepNotifySubscriber */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_ISleepNotifySubscriber;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("B2E6A19E-6629-42cb-BD29-8061D777355F")
    ISleepNotifySubscriber : public IDispatch
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE SomethingChanged( 
            /* [in] */ long lEvent) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ISleepNotifySubscriberVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ISleepNotifySubscriber * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ISleepNotifySubscriber * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ISleepNotifySubscriber * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ISleepNotifySubscriber * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ISleepNotifySubscriber * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ISleepNotifySubscriber * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ISleepNotifySubscriber * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *SomethingChanged )( 
            ISleepNotifySubscriber * This,
            /* [in] */ long lEvent);
        
        END_INTERFACE
    } ISleepNotifySubscriberVtbl;

    interface ISleepNotifySubscriber
    {
        CONST_VTBL struct ISleepNotifySubscriberVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ISleepNotifySubscriber_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ISleepNotifySubscriber_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ISleepNotifySubscriber_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ISleepNotifySubscriber_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define ISleepNotifySubscriber_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define ISleepNotifySubscriber_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define ISleepNotifySubscriber_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define ISleepNotifySubscriber_SomethingChanged(This,lEvent)	\
    (This)->lpVtbl -> SomethingChanged(This,lEvent)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISleepNotifySubscriber_SomethingChanged_Proxy( 
    ISleepNotifySubscriber * This,
    /* [in] */ long lEvent);


void __RPC_STUB ISleepNotifySubscriber_SomethingChanged_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ISleepNotifySubscriber_INTERFACE_DEFINED__ */


#ifndef __IConfigRegistryNotify_INTERFACE_DEFINED__
#define __IConfigRegistryNotify_INTERFACE_DEFINED__

/* interface IConfigRegistryNotify */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IConfigRegistryNotify;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("D661138B-F4DD-4a81-BA4F-FE0C0CFF7433")
    IConfigRegistryNotify : public IUnknown
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE NotifyChanged( 
            /* [in] */ BSTR bstrName,
            /* [in] */ VARIANT varValue) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IConfigRegistryNotifyVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IConfigRegistryNotify * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IConfigRegistryNotify * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IConfigRegistryNotify * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *NotifyChanged )( 
            IConfigRegistryNotify * This,
            /* [in] */ BSTR bstrName,
            /* [in] */ VARIANT varValue);
        
        END_INTERFACE
    } IConfigRegistryNotifyVtbl;

    interface IConfigRegistryNotify
    {
        CONST_VTBL struct IConfigRegistryNotifyVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IConfigRegistryNotify_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IConfigRegistryNotify_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IConfigRegistryNotify_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IConfigRegistryNotify_NotifyChanged(This,bstrName,varValue)	\
    (This)->lpVtbl -> NotifyChanged(This,bstrName,varValue)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring] */ HRESULT STDMETHODCALLTYPE IConfigRegistryNotify_NotifyChanged_Proxy( 
    IConfigRegistryNotify * This,
    /* [in] */ BSTR bstrName,
    /* [in] */ VARIANT varValue);


void __RPC_STUB IConfigRegistryNotify_NotifyChanged_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IConfigRegistryNotify_INTERFACE_DEFINED__ */


#ifndef __IDoniCallback2_INTERFACE_DEFINED__
#define __IDoniCallback2_INTERFACE_DEFINED__

/* interface IDoniCallback2 */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IDoniCallback2;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("703D93CF-9B3A-4b69-BB2D-E023E76BD01B")
    IDoniCallback2 : public IUnknown
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE DoniPacketHandler( 
            /* [in] */ long lMonId,
            /* [in] */ long lcSize,
            /* [size_is][in] */ BYTE *pdatex_record) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE DoniPatientHandler( 
            /* [in] */ long lMonId,
            /* [in] */ long lEvent,
            /* [in] */ long lStatus) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE DoniServerHandler( 
            /* [in] */ long lServerId,
            /* [in] */ long lEvent) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE DoniWaveStatusHandler( 
            /* [in] */ long lMonID,
            /* [in] */ long lcWaves,
            /* [size_is][in] */ BYTE *pc,
            /* [in] */ long lchanged) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDoniCallback2Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDoniCallback2 * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDoniCallback2 * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDoniCallback2 * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *DoniPacketHandler )( 
            IDoniCallback2 * This,
            /* [in] */ long lMonId,
            /* [in] */ long lcSize,
            /* [size_is][in] */ BYTE *pdatex_record);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *DoniPatientHandler )( 
            IDoniCallback2 * This,
            /* [in] */ long lMonId,
            /* [in] */ long lEvent,
            /* [in] */ long lStatus);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *DoniServerHandler )( 
            IDoniCallback2 * This,
            /* [in] */ long lServerId,
            /* [in] */ long lEvent);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *DoniWaveStatusHandler )( 
            IDoniCallback2 * This,
            /* [in] */ long lMonID,
            /* [in] */ long lcWaves,
            /* [size_is][in] */ BYTE *pc,
            /* [in] */ long lchanged);
        
        END_INTERFACE
    } IDoniCallback2Vtbl;

    interface IDoniCallback2
    {
        CONST_VTBL struct IDoniCallback2Vtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDoniCallback2_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IDoniCallback2_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IDoniCallback2_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IDoniCallback2_DoniPacketHandler(This,lMonId,lcSize,pdatex_record)	\
    (This)->lpVtbl -> DoniPacketHandler(This,lMonId,lcSize,pdatex_record)

#define IDoniCallback2_DoniPatientHandler(This,lMonId,lEvent,lStatus)	\
    (This)->lpVtbl -> DoniPatientHandler(This,lMonId,lEvent,lStatus)

#define IDoniCallback2_DoniServerHandler(This,lServerId,lEvent)	\
    (This)->lpVtbl -> DoniServerHandler(This,lServerId,lEvent)

#define IDoniCallback2_DoniWaveStatusHandler(This,lMonID,lcWaves,pc,lchanged)	\
    (This)->lpVtbl -> DoniWaveStatusHandler(This,lMonID,lcWaves,pc,lchanged)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring] */ HRESULT STDMETHODCALLTYPE IDoniCallback2_DoniPacketHandler_Proxy( 
    IDoniCallback2 * This,
    /* [in] */ long lMonId,
    /* [in] */ long lcSize,
    /* [size_is][in] */ BYTE *pdatex_record);


void __RPC_STUB IDoniCallback2_DoniPacketHandler_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IDoniCallback2_DoniPatientHandler_Proxy( 
    IDoniCallback2 * This,
    /* [in] */ long lMonId,
    /* [in] */ long lEvent,
    /* [in] */ long lStatus);


void __RPC_STUB IDoniCallback2_DoniPatientHandler_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IDoniCallback2_DoniServerHandler_Proxy( 
    IDoniCallback2 * This,
    /* [in] */ long lServerId,
    /* [in] */ long lEvent);


void __RPC_STUB IDoniCallback2_DoniServerHandler_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IDoniCallback2_DoniWaveStatusHandler_Proxy( 
    IDoniCallback2 * This,
    /* [in] */ long lMonID,
    /* [in] */ long lcWaves,
    /* [size_is][in] */ BYTE *pc,
    /* [in] */ long lchanged);


void __RPC_STUB IDoniCallback2_DoniWaveStatusHandler_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IDoniCallback2_INTERFACE_DEFINED__ */



#ifndef __ADDINIFLib_LIBRARY_DEFINED__
#define __ADDINIFLib_LIBRARY_DEFINED__

/* library ADDINIFLib */
/* [helpstring][version][uuid] */ 

































#define	DISPID_CLICKCLOSE	( 1 )

#define	DISPID_CLICKOK	( 2 )

#define	DISPID_CLICKSAVE	( 3 )


EXTERN_C const IID LIBID_ADDINIFLib;

#ifndef ___IBannerEvents_DISPINTERFACE_DEFINED__
#define ___IBannerEvents_DISPINTERFACE_DEFINED__

/* dispinterface _IBannerEvents */
/* [helpstring][uuid] */ 


EXTERN_C const IID DIID__IBannerEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("D79133A3-4963-40d0-8DFE-CA72D2634E48")
    _IBannerEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _IBannerEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _IBannerEvents * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _IBannerEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _IBannerEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _IBannerEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _IBannerEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _IBannerEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _IBannerEvents * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _IBannerEventsVtbl;

    interface _IBannerEvents
    {
        CONST_VTBL struct _IBannerEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _IBannerEvents_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define _IBannerEvents_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define _IBannerEvents_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define _IBannerEvents_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define _IBannerEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define _IBannerEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define _IBannerEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___IBannerEvents_DISPINTERFACE_DEFINED__ */

#endif /* __ADDINIFLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long *, unsigned long            , BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserMarshal(  unsigned long *, unsigned char *, BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserUnmarshal(unsigned long *, unsigned char *, BSTR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long *, BSTR * ); 

unsigned long             __RPC_USER  VARIANT_UserSize(     unsigned long *, unsigned long            , VARIANT * ); 
unsigned char * __RPC_USER  VARIANT_UserMarshal(  unsigned long *, unsigned char *, VARIANT * ); 
unsigned char * __RPC_USER  VARIANT_UserUnmarshal(unsigned long *, unsigned char *, VARIANT * ); 
void                      __RPC_USER  VARIANT_UserFree(     unsigned long *, VARIANT * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


