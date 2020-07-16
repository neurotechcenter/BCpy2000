////////////////////////////////////////////////////////////////////////////////
// $Id: BCI2000PCLLibrary.cpp 4970 2015-08-21 16:46:07Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: An IPCLLibrary implementation that provides information about
//   BCI2000PresentationLink data types.
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
#include "BCI2000PCLLibrary.h"
#include "BCI2000ControllerType.h"
#include "VersionInfo.h"
#include "Utils.h"

ComRegisterCoClass( CBCI2000PCLLibrary );

namespace
{
  template <class T>
  com::Ptr<IPCLType>
  InstantiateType( IPCLLibrary* inLib )
  {
    com::Ptr<T> pInstance = new T;
    pInstance->setLibrary( inLib );
    return static_cast<T*>( pInstance );
  }
} // unnamed namespace

#undef TYPE
#define TYPE(x) mTypeInstantiators.push_back( &InstantiateType<x> )

CBCI2000PCLLibrary::CBCI2000PCLLibrary()
{
  TYPE( CBCI2000ControllerType );
}

STDMETHODIMP CBCI2000PCLLibrary::getName(BSTR* pResult)
{
  COM_METHOD_START
  *pResult = ::SysAllocString( Name() );
  COM_METHOD_END
}

STDMETHODIMP CBCI2000PCLLibrary::getAuthor(BSTR* pResult)
{
  COM_METHOD_START
  *pResult = ::SysAllocString( Author() );
  COM_METHOD_END
}

STDMETHODIMP CBCI2000PCLLibrary::getVersion(BSTR* pResult)
{
  COM_METHOD_START
  *pResult = ::SysAllocString( Version() );
  COM_METHOD_END
}

STDMETHODIMP CBCI2000PCLLibrary::getDescription(BSTR* pResult)
{
  COM_METHOD_START
  *pResult = ::SysAllocString( Description() );
  COM_METHOD_END
}

STDMETHODIMP CBCI2000PCLLibrary::getPresentationMinimumVersion(double* pResult)
{
  COM_METHOD_START
  /// If you use PCL extension features released after the initial release,
  /// change this to the Presentation version number required
  /// (otherwise leave 0)
  *pResult = 0;
  COM_METHOD_END
}

STDMETHODIMP CBCI2000PCLLibrary::getNamespaceName(BSTR* pResult)
{
  COM_METHOD_START
  *pResult = ::SysAllocString( NamespaceName() );
  COM_METHOD_END
}

STDMETHODIMP CBCI2000PCLLibrary::getTypeCount(long* pResult)
{
  COM_METHOD_START
  *pResult = static_cast<long>( mTypeInstantiators.size() );
  COM_METHOD_END
}

STDMETHODIMP CBCI2000PCLLibrary::getType(long inIndex, IPCLType** outType)
{
  COM_METHOD_START
  size_t index = static_cast<size_t>( inIndex );
  if( index < 0 || index >= mTypeInstantiators.size() )
    throw bciexception << "Invalid type index: " << inIndex << " sent to extension.";
  com::Ptr<IPCLType> pType = mTypeInstantiators[inIndex]( this );
  *outType = pType;
  ( *outType )->AddRef();
  COM_METHOD_END
}

STDMETHODIMP CBCI2000PCLLibrary::getLastError(BSTR* pResult)
{
  *pResult = ::SysAllocString( mLastError.c_str() );
  return S_OK;
}

const wchar_t*
CBCI2000PCLLibrary::Version()
{
  static std::wstring sVersion;
  if( sVersion.empty() )
  {
    const VersionInfo& info = VersionInfo::Current;
    std::ostringstream oss;
    oss << "BCI2000 " << info[VersionInfo::VersionID];
    if( !info[VersionInfo::Revision].empty() )
      oss << " r" << info[VersionInfo::Revision];
    sVersion = com::DualString( oss.str() );
  }
  return sVersion.c_str();
}

