////////////////////////////////////////////////////////////////////////////////
// $Id: ComClassFactory.h 3902 2012-04-04 14:15:42Z mellinger $
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
#ifndef COM_CLASS_FACTORY_H
#define COM_CLASS_FACTORY_H

#include <vector>
#include "ComPtr.h"
#include <Windows.h>

namespace com {

class ClassFactoryBase : public IClassFactory
{
 public:
  ClassFactoryBase();
  virtual const CLSID* ClassID() const;

  static HRESULT DllGetClassObject( REFCLSID inCSLID, REFIID inIID, void** outObject );
  static HRESULT DllCanUnloadNow();

  static void IncObjectCount();
  static void DecObjectCount();

 private:
  STDMETHOD( QueryInterface )( const IID& inIID, void** outInterface );
  STDMETHOD_( ULONG, AddRef )();
  STDMETHOD_( ULONG, Release )();
  STDMETHOD( LockServer )( BOOL );

 private:
  int mRefCount;
  static LONG sLockCount,
              sObjectCount;
  typedef std::vector<ClassFactoryBase*> List;
  static List& Instances();
};

template<typename T, const CLSID* U>
class ClassFactory : public ClassFactoryBase
{
 public:
  const CLSID* ClassID() const { return U; }

  STDMETHOD( CreateInstance )( IUnknown* inpUnknown, REFIID inIID, void** outObject )
  {
    if( inpUnknown )
      return CLASS_E_NOAGGREGATION;
    Ptr<T> pObject = new T;
    return pObject->QueryInterface( inIID, outObject );
  }
};

} // namespace

#endif // COM_CLASS_FACTORY_H
