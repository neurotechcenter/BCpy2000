

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Wed Mar 20 15:00:21 2013
 */
/* Compiler settings for BCI2000Automation.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 7.00.0555 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

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

#ifndef __BCI2000Automation_h__
#define __BCI2000Automation_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IBCI2000Remote_FWD_DEFINED__
#define __IBCI2000Remote_FWD_DEFINED__
typedef interface IBCI2000Remote IBCI2000Remote;
#endif 	/* __IBCI2000Remote_FWD_DEFINED__ */


#ifndef __BCI2000Remote_FWD_DEFINED__
#define __BCI2000Remote_FWD_DEFINED__

#ifdef __cplusplus
typedef class BCI2000Remote BCI2000Remote;
#else
typedef struct BCI2000Remote BCI2000Remote;
#endif /* __cplusplus */

#endif 	/* __BCI2000Remote_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IBCI2000Remote_INTERFACE_DEFINED__
#define __IBCI2000Remote_INTERFACE_DEFINED__

/* interface IBCI2000Remote */
/* [object][helpstring][oleautomation][dual][uuid] */ 


EXTERN_C const IID IID_IBCI2000Remote;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("D2A5DB6C-DE5B-4a1f-A042-3F4CEFB88F4F")
    IBCI2000Remote : public IDispatch
    {
    public:
        virtual /* [helpstring][propget] */ HRESULT __stdcall get_Timeout( 
            /* [retval][out] */ double *seconds) = 0;
        
        virtual /* [helpstring][propput] */ HRESULT __stdcall put_Timeout( 
            /* [in] */ double seconds) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT __stdcall get_OperatorPath( 
            /* [retval][out] */ BSTR *path) = 0;
        
        virtual /* [helpstring][propput] */ HRESULT __stdcall put_OperatorPath( 
            /* [in] */ BSTR path) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT __stdcall get_TelnetAddress( 
            /* [retval][out] */ BSTR *address) = 0;
        
        virtual /* [helpstring][propput] */ HRESULT __stdcall put_TelnetAddress( 
            /* [in] */ BSTR address) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT __stdcall get_WindowVisible( 
            /* [retval][out] */ int *visible) = 0;
        
        virtual /* [helpstring][propput] */ HRESULT __stdcall put_WindowVisible( 
            /* [in] */ int visible) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT __stdcall get_WindowTitle( 
            /* [retval][out] */ BSTR *title) = 0;
        
        virtual /* [helpstring][propput] */ HRESULT __stdcall put_WindowTitle( 
            /* [in] */ BSTR title) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT __stdcall get_Result( 
            /* [retval][out] */ BSTR *result) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT __stdcall get_SubjectID( 
            /* [retval][out] */ BSTR *subjectID) = 0;
        
        virtual /* [helpstring][propput] */ HRESULT __stdcall put_SubjectID( 
            /* [in] */ BSTR subjectID) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT __stdcall get_SessionID( 
            /* [retval][out] */ BSTR *sessionID) = 0;
        
        virtual /* [helpstring][propput] */ HRESULT __stdcall put_SessionID( 
            /* [in] */ BSTR sessionID) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT __stdcall get_DataDirectory( 
            /* [retval][out] */ BSTR *directory) = 0;
        
        virtual /* [helpstring][propput] */ HRESULT __stdcall put_DataDirectory( 
            /* [in] */ BSTR directory) = 0;
        
        virtual /* [helpstring] */ HRESULT __stdcall Connect( 
            /* [retval][out] */ VARIANT_BOOL *success) = 0;
        
        virtual /* [helpstring] */ HRESULT __stdcall Disconnect( 
            /* [retval][out] */ VARIANT_BOOL *success) = 0;
        
        virtual /* [helpstring] */ HRESULT __stdcall StartupModules( 
            /* [out][in] */ SAFEARRAY * *modules,
            /* [retval][out] */ VARIANT_BOOL *success) = 0;
        
        virtual /* [helpstring] */ HRESULT __stdcall SetConfig( 
            /* [retval][out] */ VARIANT_BOOL *success) = 0;
        
        virtual /* [helpstring] */ HRESULT __stdcall Start( 
            /* [retval][out] */ VARIANT_BOOL *success) = 0;
        
        virtual /* [helpstring] */ HRESULT __stdcall Stop( 
            /* [retval][out] */ VARIANT_BOOL *success) = 0;
        
        virtual /* [helpstring] */ HRESULT __stdcall LoadParametersLocal( 
            /* [in] */ BSTR file,
            /* [retval][out] */ VARIANT_BOOL *success) = 0;
        
        virtual /* [helpstring] */ HRESULT __stdcall LoadParametersRemote( 
            /* [in] */ BSTR file,
            /* [retval][out] */ VARIANT_BOOL *success) = 0;
        
        virtual /* [helpstring] */ HRESULT __stdcall GetParameter( 
            /* [in] */ BSTR name,
            /* [out][in] */ BSTR *value,
            /* [retval][out] */ VARIANT_BOOL *success) = 0;
        
        virtual /* [helpstring] */ HRESULT __stdcall SetParameter( 
            /* [in] */ BSTR name,
            /* [in] */ BSTR value,
            /* [retval][out] */ VARIANT_BOOL *success) = 0;
        
        virtual /* [helpstring] */ HRESULT __stdcall AddStateVariable( 
            /* [in] */ BSTR stateName,
            /* [in] */ int bitWidth,
            /* [in] */ double initialValue,
            /* [retval][out] */ VARIANT_BOOL *success) = 0;
        
        virtual /* [helpstring] */ HRESULT __stdcall GetStateVariable( 
            /* [in] */ BSTR stateName,
            /* [out][in] */ double *value,
            /* [retval][out] */ VARIANT_BOOL *success) = 0;
        
        virtual /* [helpstring] */ HRESULT __stdcall SetStateVariable( 
            /* [in] */ BSTR stateName,
            /* [in] */ double value,
            /* [retval][out] */ VARIANT_BOOL *success) = 0;
        
        virtual /* [helpstring] */ HRESULT __stdcall GetSystemState( 
            /* [out][in] */ BSTR *state,
            /* [retval][out] */ VARIANT_BOOL *success) = 0;
        
        virtual /* [helpstring] */ HRESULT __stdcall GetControlSignal( 
            /* [in] */ int channel,
            /* [in] */ int element,
            /* [out][in] */ double *value,
            /* [retval][out] */ VARIANT_BOOL *success) = 0;
        
        virtual /* [helpstring] */ HRESULT __stdcall Encode( 
            /* [in] */ BSTR rawValue,
            /* [retval][out] */ BSTR *encodedValue) = 0;
        
        virtual /* [helpstring] */ HRESULT __stdcall Execute( 
            /* [in] */ BSTR command,
            /* [out][in][optional] */ VARIANT *exitCode,
            /* [retval][out] */ VARIANT_BOOL *success) = 0;
        
        virtual /* [helpstring] */ HRESULT __stdcall SetScript( 
            /* [in] */ BSTR eventName,
            /* [in] */ BSTR script,
            /* [retval][out] */ VARIANT_BOOL *success) = 0;
        
        virtual /* [helpstring] */ HRESULT __stdcall GetScript( 
            /* [in] */ BSTR eventName,
            /* [out][in] */ BSTR *script,
            /* [retval][out] */ VARIANT_BOOL *success) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IBCI2000RemoteVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IBCI2000Remote * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IBCI2000Remote * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IBCI2000Remote * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IBCI2000Remote * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IBCI2000Remote * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IBCI2000Remote * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IBCI2000Remote * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][propget] */ HRESULT ( __stdcall *get_Timeout )( 
            IBCI2000Remote * This,
            /* [retval][out] */ double *seconds);
        
        /* [helpstring][propput] */ HRESULT ( __stdcall *put_Timeout )( 
            IBCI2000Remote * This,
            /* [in] */ double seconds);
        
        /* [helpstring][propget] */ HRESULT ( __stdcall *get_OperatorPath )( 
            IBCI2000Remote * This,
            /* [retval][out] */ BSTR *path);
        
        /* [helpstring][propput] */ HRESULT ( __stdcall *put_OperatorPath )( 
            IBCI2000Remote * This,
            /* [in] */ BSTR path);
        
        /* [helpstring][propget] */ HRESULT ( __stdcall *get_TelnetAddress )( 
            IBCI2000Remote * This,
            /* [retval][out] */ BSTR *address);
        
        /* [helpstring][propput] */ HRESULT ( __stdcall *put_TelnetAddress )( 
            IBCI2000Remote * This,
            /* [in] */ BSTR address);
        
        /* [helpstring][propget] */ HRESULT ( __stdcall *get_WindowVisible )( 
            IBCI2000Remote * This,
            /* [retval][out] */ int *visible);
        
        /* [helpstring][propput] */ HRESULT ( __stdcall *put_WindowVisible )( 
            IBCI2000Remote * This,
            /* [in] */ int visible);
        
        /* [helpstring][propget] */ HRESULT ( __stdcall *get_WindowTitle )( 
            IBCI2000Remote * This,
            /* [retval][out] */ BSTR *title);
        
        /* [helpstring][propput] */ HRESULT ( __stdcall *put_WindowTitle )( 
            IBCI2000Remote * This,
            /* [in] */ BSTR title);
        
        /* [helpstring][propget] */ HRESULT ( __stdcall *get_Result )( 
            IBCI2000Remote * This,
            /* [retval][out] */ BSTR *result);
        
        /* [helpstring][propget] */ HRESULT ( __stdcall *get_SubjectID )( 
            IBCI2000Remote * This,
            /* [retval][out] */ BSTR *subjectID);
        
        /* [helpstring][propput] */ HRESULT ( __stdcall *put_SubjectID )( 
            IBCI2000Remote * This,
            /* [in] */ BSTR subjectID);
        
        /* [helpstring][propget] */ HRESULT ( __stdcall *get_SessionID )( 
            IBCI2000Remote * This,
            /* [retval][out] */ BSTR *sessionID);
        
        /* [helpstring][propput] */ HRESULT ( __stdcall *put_SessionID )( 
            IBCI2000Remote * This,
            /* [in] */ BSTR sessionID);
        
        /* [helpstring][propget] */ HRESULT ( __stdcall *get_DataDirectory )( 
            IBCI2000Remote * This,
            /* [retval][out] */ BSTR *directory);
        
        /* [helpstring][propput] */ HRESULT ( __stdcall *put_DataDirectory )( 
            IBCI2000Remote * This,
            /* [in] */ BSTR directory);
        
        /* [helpstring] */ HRESULT ( __stdcall *Connect )( 
            IBCI2000Remote * This,
            /* [retval][out] */ VARIANT_BOOL *success);
        
        /* [helpstring] */ HRESULT ( __stdcall *Disconnect )( 
            IBCI2000Remote * This,
            /* [retval][out] */ VARIANT_BOOL *success);
        
        /* [helpstring] */ HRESULT ( __stdcall *StartupModules )( 
            IBCI2000Remote * This,
            /* [out][in] */ SAFEARRAY * *modules,
            /* [retval][out] */ VARIANT_BOOL *success);
        
        /* [helpstring] */ HRESULT ( __stdcall *SetConfig )( 
            IBCI2000Remote * This,
            /* [retval][out] */ VARIANT_BOOL *success);
        
        /* [helpstring] */ HRESULT ( __stdcall *Start )( 
            IBCI2000Remote * This,
            /* [retval][out] */ VARIANT_BOOL *success);
        
        /* [helpstring] */ HRESULT ( __stdcall *Stop )( 
            IBCI2000Remote * This,
            /* [retval][out] */ VARIANT_BOOL *success);
        
        /* [helpstring] */ HRESULT ( __stdcall *LoadParametersLocal )( 
            IBCI2000Remote * This,
            /* [in] */ BSTR file,
            /* [retval][out] */ VARIANT_BOOL *success);
        
        /* [helpstring] */ HRESULT ( __stdcall *LoadParametersRemote )( 
            IBCI2000Remote * This,
            /* [in] */ BSTR file,
            /* [retval][out] */ VARIANT_BOOL *success);
        
        /* [helpstring] */ HRESULT ( __stdcall *GetParameter )( 
            IBCI2000Remote * This,
            /* [in] */ BSTR name,
            /* [out][in] */ BSTR *value,
            /* [retval][out] */ VARIANT_BOOL *success);
        
        /* [helpstring] */ HRESULT ( __stdcall *SetParameter )( 
            IBCI2000Remote * This,
            /* [in] */ BSTR name,
            /* [in] */ BSTR value,
            /* [retval][out] */ VARIANT_BOOL *success);
        
        /* [helpstring] */ HRESULT ( __stdcall *AddStateVariable )( 
            IBCI2000Remote * This,
            /* [in] */ BSTR stateName,
            /* [in] */ int bitWidth,
            /* [in] */ double initialValue,
            /* [retval][out] */ VARIANT_BOOL *success);
        
        /* [helpstring] */ HRESULT ( __stdcall *GetStateVariable )( 
            IBCI2000Remote * This,
            /* [in] */ BSTR stateName,
            /* [out][in] */ double *value,
            /* [retval][out] */ VARIANT_BOOL *success);
        
        /* [helpstring] */ HRESULT ( __stdcall *SetStateVariable )( 
            IBCI2000Remote * This,
            /* [in] */ BSTR stateName,
            /* [in] */ double value,
            /* [retval][out] */ VARIANT_BOOL *success);
        
        /* [helpstring] */ HRESULT ( __stdcall *GetSystemState )( 
            IBCI2000Remote * This,
            /* [out][in] */ BSTR *state,
            /* [retval][out] */ VARIANT_BOOL *success);
        
        /* [helpstring] */ HRESULT ( __stdcall *GetControlSignal )( 
            IBCI2000Remote * This,
            /* [in] */ int channel,
            /* [in] */ int element,
            /* [out][in] */ double *value,
            /* [retval][out] */ VARIANT_BOOL *success);
        
        /* [helpstring] */ HRESULT ( __stdcall *Encode )( 
            IBCI2000Remote * This,
            /* [in] */ BSTR rawValue,
            /* [retval][out] */ BSTR *encodedValue);
        
        /* [helpstring] */ HRESULT ( __stdcall *Execute )( 
            IBCI2000Remote * This,
            /* [in] */ BSTR command,
            /* [out][in][optional] */ VARIANT *exitCode,
            /* [retval][out] */ VARIANT_BOOL *success);
        
        /* [helpstring] */ HRESULT ( __stdcall *SetScript )( 
            IBCI2000Remote * This,
            /* [in] */ BSTR eventName,
            /* [in] */ BSTR script,
            /* [retval][out] */ VARIANT_BOOL *success);
        
        /* [helpstring] */ HRESULT ( __stdcall *GetScript )( 
            IBCI2000Remote * This,
            /* [in] */ BSTR eventName,
            /* [out][in] */ BSTR *script,
            /* [retval][out] */ VARIANT_BOOL *success);
        
        END_INTERFACE
    } IBCI2000RemoteVtbl;

    interface IBCI2000Remote
    {
        CONST_VTBL struct IBCI2000RemoteVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IBCI2000Remote_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IBCI2000Remote_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IBCI2000Remote_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IBCI2000Remote_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IBCI2000Remote_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IBCI2000Remote_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IBCI2000Remote_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IBCI2000Remote_get_Timeout(This,seconds)	\
    ( (This)->lpVtbl -> get_Timeout(This,seconds) ) 

#define IBCI2000Remote_put_Timeout(This,seconds)	\
    ( (This)->lpVtbl -> put_Timeout(This,seconds) ) 

#define IBCI2000Remote_get_OperatorPath(This,path)	\
    ( (This)->lpVtbl -> get_OperatorPath(This,path) ) 

#define IBCI2000Remote_put_OperatorPath(This,path)	\
    ( (This)->lpVtbl -> put_OperatorPath(This,path) ) 

#define IBCI2000Remote_get_TelnetAddress(This,address)	\
    ( (This)->lpVtbl -> get_TelnetAddress(This,address) ) 

#define IBCI2000Remote_put_TelnetAddress(This,address)	\
    ( (This)->lpVtbl -> put_TelnetAddress(This,address) ) 

#define IBCI2000Remote_get_WindowVisible(This,visible)	\
    ( (This)->lpVtbl -> get_WindowVisible(This,visible) ) 

#define IBCI2000Remote_put_WindowVisible(This,visible)	\
    ( (This)->lpVtbl -> put_WindowVisible(This,visible) ) 

#define IBCI2000Remote_get_WindowTitle(This,title)	\
    ( (This)->lpVtbl -> get_WindowTitle(This,title) ) 

#define IBCI2000Remote_put_WindowTitle(This,title)	\
    ( (This)->lpVtbl -> put_WindowTitle(This,title) ) 

#define IBCI2000Remote_get_Result(This,result)	\
    ( (This)->lpVtbl -> get_Result(This,result) ) 

#define IBCI2000Remote_get_SubjectID(This,subjectID)	\
    ( (This)->lpVtbl -> get_SubjectID(This,subjectID) ) 

#define IBCI2000Remote_put_SubjectID(This,subjectID)	\
    ( (This)->lpVtbl -> put_SubjectID(This,subjectID) ) 

#define IBCI2000Remote_get_SessionID(This,sessionID)	\
    ( (This)->lpVtbl -> get_SessionID(This,sessionID) ) 

#define IBCI2000Remote_put_SessionID(This,sessionID)	\
    ( (This)->lpVtbl -> put_SessionID(This,sessionID) ) 

#define IBCI2000Remote_get_DataDirectory(This,directory)	\
    ( (This)->lpVtbl -> get_DataDirectory(This,directory) ) 

#define IBCI2000Remote_put_DataDirectory(This,directory)	\
    ( (This)->lpVtbl -> put_DataDirectory(This,directory) ) 

#define IBCI2000Remote_Connect(This,success)	\
    ( (This)->lpVtbl -> Connect(This,success) ) 

#define IBCI2000Remote_Disconnect(This,success)	\
    ( (This)->lpVtbl -> Disconnect(This,success) ) 

#define IBCI2000Remote_StartupModules(This,modules,success)	\
    ( (This)->lpVtbl -> StartupModules(This,modules,success) ) 

#define IBCI2000Remote_SetConfig(This,success)	\
    ( (This)->lpVtbl -> SetConfig(This,success) ) 

#define IBCI2000Remote_Start(This,success)	\
    ( (This)->lpVtbl -> Start(This,success) ) 

#define IBCI2000Remote_Stop(This,success)	\
    ( (This)->lpVtbl -> Stop(This,success) ) 

#define IBCI2000Remote_LoadParametersLocal(This,file,success)	\
    ( (This)->lpVtbl -> LoadParametersLocal(This,file,success) ) 

#define IBCI2000Remote_LoadParametersRemote(This,file,success)	\
    ( (This)->lpVtbl -> LoadParametersRemote(This,file,success) ) 

#define IBCI2000Remote_GetParameter(This,name,value,success)	\
    ( (This)->lpVtbl -> GetParameter(This,name,value,success) ) 

#define IBCI2000Remote_SetParameter(This,name,value,success)	\
    ( (This)->lpVtbl -> SetParameter(This,name,value,success) ) 

#define IBCI2000Remote_AddStateVariable(This,stateName,bitWidth,initialValue,success)	\
    ( (This)->lpVtbl -> AddStateVariable(This,stateName,bitWidth,initialValue,success) ) 

#define IBCI2000Remote_GetStateVariable(This,stateName,value,success)	\
    ( (This)->lpVtbl -> GetStateVariable(This,stateName,value,success) ) 

#define IBCI2000Remote_SetStateVariable(This,stateName,value,success)	\
    ( (This)->lpVtbl -> SetStateVariable(This,stateName,value,success) ) 

#define IBCI2000Remote_GetSystemState(This,state,success)	\
    ( (This)->lpVtbl -> GetSystemState(This,state,success) ) 

#define IBCI2000Remote_GetControlSignal(This,channel,element,value,success)	\
    ( (This)->lpVtbl -> GetControlSignal(This,channel,element,value,success) ) 

#define IBCI2000Remote_Encode(This,rawValue,encodedValue)	\
    ( (This)->lpVtbl -> Encode(This,rawValue,encodedValue) ) 

#define IBCI2000Remote_Execute(This,command,exitCode,success)	\
    ( (This)->lpVtbl -> Execute(This,command,exitCode,success) ) 

#define IBCI2000Remote_SetScript(This,eventName,script,success)	\
    ( (This)->lpVtbl -> SetScript(This,eventName,script,success) ) 

#define IBCI2000Remote_GetScript(This,eventName,script,success)	\
    ( (This)->lpVtbl -> GetScript(This,eventName,script,success) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IBCI2000Remote_INTERFACE_DEFINED__ */



#ifndef __BCI2000AutomationLib_LIBRARY_DEFINED__
#define __BCI2000AutomationLib_LIBRARY_DEFINED__

/* library BCI2000AutomationLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_BCI2000AutomationLib;

EXTERN_C const CLSID CLSID_BCI2000Remote;

#ifdef __cplusplus

class DECLSPEC_UUID("F880B1BC-3511-4b06-8A79-895477300608")
BCI2000Remote;
#endif
#endif /* __BCI2000AutomationLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long *, unsigned long            , BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserMarshal(  unsigned long *, unsigned char *, BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserUnmarshal(unsigned long *, unsigned char *, BSTR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long *, BSTR * ); 

unsigned long             __RPC_USER  LPSAFEARRAY_UserSize(     unsigned long *, unsigned long            , LPSAFEARRAY * ); 
unsigned char * __RPC_USER  LPSAFEARRAY_UserMarshal(  unsigned long *, unsigned char *, LPSAFEARRAY * ); 
unsigned char * __RPC_USER  LPSAFEARRAY_UserUnmarshal(unsigned long *, unsigned char *, LPSAFEARRAY * ); 
void                      __RPC_USER  LPSAFEARRAY_UserFree(     unsigned long *, LPSAFEARRAY * ); 

unsigned long             __RPC_USER  VARIANT_UserSize(     unsigned long *, unsigned long            , VARIANT * ); 
unsigned char * __RPC_USER  VARIANT_UserMarshal(  unsigned long *, unsigned char *, VARIANT * ); 
unsigned char * __RPC_USER  VARIANT_UserUnmarshal(unsigned long *, unsigned char *, VARIANT * ); 
void                      __RPC_USER  VARIANT_UserFree(     unsigned long *, VARIANT * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


