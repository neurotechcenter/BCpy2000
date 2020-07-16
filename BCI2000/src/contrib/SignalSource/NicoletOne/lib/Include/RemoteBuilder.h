// *************************************************************************************** //
//  Copyright (C) 2005 by VIASYS Healthcare, Inc. All Rights Reserved.
//  Last changed by: $Author: mellinger $
//  Last changed on: $Modtime:  $
//  File reference:  $Archive:  $
//  File version:    $Revision: 4536 $
//                            THIS CODE IS COMPANY CONFIDENTIAL
// *************************************************************************************** //
// RemoteBuilder.h: interface for the CRemoteBuilder class.


#if !defined(AFX_REMOTEBUILDER_H__F7A69091_8D00_4E0A_BDD3_6C3C9F18E052__INCLUDED_)
#define AFX_REMOTEBUILDER_H__F7A69091_8D00_4E0A_BDD3_6C3C9F18E052__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

namespace NREMOTESERVICELib
{
   interface IRemoteBuilder;
}


/////////////////////////////////////////////////////////////////////////////////////////////
///<class>
///<name>CRemoteBuilder</name>
///<summary>
///  <para>
///    Helper class that attempts to obtain the COREIFLib::IBuilder interface for an 
///    instance of Recorder.exe running on a remote machine. It obtains this pointer by
///    making a call into NRemoteService.exe running on the remote box.
///  </para>
///</summary>
///<remarks>
///  <para>
///    In order to successfully contact the remote machine and get a pointer, the following
///    conditions must apply:
///  </para>
///  <list>
///    <item>This machine must be on the same network for authentication (uses DCOM)</item>
///    <item>The correct version of NRemoteService.exe must be running on the target machine</item>
///    <item>The firewall on both this computer and the target must allow the traffic (DCOM)</item>
///    <item>Recorder.exe must be running on the target to get a pointer</item>
///  </list>
///</remarks>
///</class>
/////////////////////////////////////////////////////////////////////////////////////////////
class CRemoteBuilder
{
public:
   // Construction / destruction
   CRemoteBuilder();
   explicit CRemoteBuilder(const _bstr_t& bstrMachine);
   ~CRemoteBuilder();

/////////////////////////////////////////////////////////////////////////////////////////////
// Public Methods
public:
   void Create(const _bstr_t& bstrMachine);
   HRESULT TryCreate(const _bstr_t& bstrMachine);
   HRESULT LastError(BSTR* pbstrErrMessage);

   IDispatchPtr GetBuilder(const _bstr_t& bstrGuid) const;
   IDispatchPtr GetBuilder() const;
   IDispatchPtr GetServiceInterface() const; 

/////////////////////////////////////////////////////////////////////////////////////////////
// Private Declarations
private:
   // Data Members
   CComPtr<NREMOTESERVICELib::IRemoteBuilder>  m_pIRemoteBuilder;
   CComBSTR                                    m_bstrLastError;
   HRESULT                                     m_hrLastError;
}; // class CRemoteBuilder


#endif // !defined(AFX_REMOTEBUILDER_H__F7A69091_8D00_4E0A_BDD3_6C3C9F18E052__INCLUDED_)
