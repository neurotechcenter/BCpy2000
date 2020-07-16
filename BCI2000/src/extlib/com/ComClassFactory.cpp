////////////////////////////////////////////////////////////////////////////////
// $Id: ComClassFactory.cpp 3902 2012-04-04 14:15:42Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A base class, and a template for providing one IClassFactory
//   descendant per CoClass. Use ComRegisterCoClass() defined in CoClass.h
//   to statically instantiate a ClassFactory associated with a CoClass.
//   In addition to instantiating CoClasses on request, ClassFactoryBase
//   is also responsible for object and lock counting.
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
#include "ComClassFactory.h"

using namespace com;

LONG ClassFactoryBase::sLockCount = 0;
LONG ClassFactoryBase::sObjectCount = 0;

ClassFactoryBase::List& ClassFactoryBase::Instances()
{
  static List list;
  return list;
}

ClassFactoryBase::ClassFactoryBase()
: mRefCount( 0 )
{
  Instances().push_back( this );
}

const CLSID*
ClassFactoryBase::ClassID() const
{
  return NULL;
}

HRESULT
ClassFactoryBase::DllGetClassObject( REFCLSID inCSLID, REFIID inIID, void** outObject )
{
  List::const_iterator i = Instances().begin();
  while( i != Instances().end() )
  {
    if( inCSLID == *( *i )->ClassID() )
      return ( *i )->QueryInterface( inIID, outObject );
    ++i;
  }
  *outObject = NULL;
  return CLASS_E_CLASSNOTAVAILABLE;
}

HRESULT
ClassFactoryBase::DllCanUnloadNow()
{
  return ( sObjectCount <= 0 && sLockCount <= 0 ) ? S_OK : S_FALSE;
}

void
ClassFactoryBase::IncObjectCount()
{
  ::InterlockedIncrement( &sObjectCount );
}

void
ClassFactoryBase::DecObjectCount()
{
  ::InterlockedDecrement( &sObjectCount );
}

HRESULT
ClassFactoryBase::QueryInterface( const IID& inIID, void** outInterface )
{
  if( inIID == IID_IClassFactory || inIID == IID_IUnknown )
  {
    *outInterface = this;
    this->AddRef();
    return NOERROR;
  }
  return E_NOINTERFACE;
}

  // we use statically allocated class factories
ULONG STDMETHODCALLTYPE
ClassFactoryBase::AddRef()
{
  IncObjectCount(); // Make sure the DLL is not unloaded while someone holds a pointer.
  return ++mRefCount;
}

ULONG STDMETHODCALLTYPE
ClassFactoryBase::Release()
{
  DecObjectCount();
  return --mRefCount;
}

HRESULT
ClassFactoryBase::LockServer( BOOL inLock )
{
  if( inLock )
    ::InterlockedIncrement( &sLockCount );
  else
    ::InterlockedDecrement( &sLockCount );
  return S_OK;
}

