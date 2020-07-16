////////////////////////////////////////////////////////////////////////////////
// $Id: ComCoClass.h 5462 2016-07-19 22:29:37Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A template for a COM co-class. To declare a co-class "MyClass"
//   immediately inheriting IInterface, specify inheritance and template para-
//   meters as follows:
//
//   class MyClass : public CoClass<IInterface, &IID_Interface, MyClass, &CLSID_MyClass>
//   {
//     ...
//   };
//
//   When MyClass inherits from IDispatch, you need to provide its library's
//   TypeLib as a "TYPELIB" resource in the COM DLL.
//   In order to allow instantiation from outside the DLL, put a
//
//   ComRegisterCoClass( MyClass );
//
//   at the beginning of MyClass.cpp, and add a "REGISTRY" type resource to your
//   COM DLL. Use MyCoClass.rgs as a template for the content of that resource.
//
// $BEGIN_BCI2000_LICENSE$
//
// This file is part of BCI2000, a platform for real-time bio-signal research.
// [ Copyright (C) 2000-2012: BCI2000 team and many external contributors ]
//
// BCI2000 is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software
// Foundation, either version 3 of the License, or (at your option) any later
// version.
//
// BCI2000 is distributed in the hope that it will be useful, but
//                         WITHOUT ANY WARRANTY
// - without even the implied warranty of MERCHANTABILITY or FITNESS FOR
// A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with
// this program.  If not, see <http://www.gnu.org/licenses/>.
//
// $END_BCI2000_LICENSE$
////////////////////////////////////////////////////////////////////////////////
#ifndef COM_CO_CLASS_H
#define COM_CO_CLASS_H

#include "ComClassFactory.h"
#include "ComModule.h"

#define ComRegisterCoClass(x) static const CLSID* x##Registration_ = x::Register();

namespace com {

template<class I, const IID* IID_, class C, const CLSID* CID_>
class CoClass : public I
{
 public:
  CoClass()
  : mRefCount( 0 ),
    mpTypeInfo( nullptr )
  {
    sFactory.IncObjectCount();
    Module::GetTypeLib()->GetTypeInfoOfGuid( *IID_, mpTypeInfo.Assignee() );
  }
  virtual ~CoClass()
  {
    sFactory.DecObjectCount();
  }
  // IUnknown
  HRESULT __stdcall QueryInterface( const IID& inIID, void** outInterface ) override
  {
    *outInterface = nullptr;
    if( inIID == *IID_ )
      *outInterface = static_cast<I*>( this );
    else if( inIID == IID_IUnknown )
      *outInterface = static_cast<IUnknown*>( this );
    else if( mpTypeInfo && inIID == IID_IDispatch )
      *outInterface = static_cast<::IDispatch*>( this );
    if( *outInterface )
      this->AddRef();
    return *outInterface ? S_OK : E_NOINTERFACE;
  }
  ULONG __stdcall AddRef() override
  {
    return ::InterlockedIncrement( &mRefCount );
  }
  ULONG __stdcall Release() override
  {
    ULONG refCount = ::InterlockedDecrement( &mRefCount );
    if( refCount == 0 )
      delete this;
    return refCount;
  }
  // IDispatch
  HRESULT __stdcall GetTypeInfoCount( UINT* pOut ) override
  {
    *pOut = 1;
    return S_OK;
  }
  HRESULT __stdcall GetTypeInfo( UINT inIdx, LCID, ITypeInfo** pOut ) override
  {
    if( inIdx != 0 )
      return E_INVALIDARG;
    *pOut = mpTypeInfo;
    (*pOut)->AddRef();
    return S_OK;
  }
  HRESULT __stdcall GetIDsOfNames( const IID&, LPOLESTR* inpNames, UINT inCount, LCID, DISPID* outpIDs ) override
  {
    return mpTypeInfo ? mpTypeInfo->GetIDsOfNames( inpNames, inCount, outpIDs ) : E_NOTIMPL;
  }
  HRESULT __stdcall Invoke( DISPID inID, const IID&, LCID, WORD inFlags, DISPPARAMS* inpParams, VARIANT* outpResult, EXCEPINFO* outpExcepInfo, UINT* outpArgErr ) override
  {
    return mpTypeInfo ? mpTypeInfo->Invoke( static_cast<I*>( this ), inID, inFlags, inpParams, outpResult, outpExcepInfo, outpArgErr ) : E_NOTIMPL;
  }

 public:
  static const CLSID* Register()
  {
    return sFactory.ClassID();
  }

 private:
  volatile LONG mRefCount;
  Ptr<ITypeInfo> mpTypeInfo;
  static ClassFactory<C, CID_> sFactory;
};

template<class I, const IID* IID_, class C, const CLSID* CID_>
ClassFactory<C, CID_> CoClass<I, IID_, C, CID_>::sFactory;

} // namespace

#endif // COM_CO_CLASS_H
