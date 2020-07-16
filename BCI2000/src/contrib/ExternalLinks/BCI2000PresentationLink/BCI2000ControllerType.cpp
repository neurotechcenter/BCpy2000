////////////////////////////////////////////////////////////////////////////////
// $Id: BCI2000ControllerType.cpp 3918 2012-04-04 14:52:38Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: An IPCLType implementation providing type information about
//   the BCI2000Controller class.
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
#include "BCI2000ControllerType.h"
#include "BCI2000Controller.h"
#include "BCI2000PCLLibrary.h"

CBCI2000ControllerType::CBCI2000ControllerType()
{
}

STDMETHODIMP CBCI2000ControllerType::getName(BSTR* pResult)
{
  COM_METHOD_START
  *pResult = ::SysAllocString( TypeName() );
  COM_METHOD_END
}

STDMETHODIMP CBCI2000ControllerType::getParentName(BSTR * pResult)
{
  COM_METHOD_START
  *pResult = ::SysAllocString( L"" );
  COM_METHOD_END
}

STDMETHODIMP CBCI2000ControllerType::getMethods(IPCLMethodList* ioList)
{
  COM_METHOD_START
  CBCI2000Controller::getMethods( ioList );
  COM_METHOD_END
}

STDMETHODIMP CBCI2000ControllerType::createObject(long inIndex, IPCLArguments* inArgs, IPCLObject** outObject)
{
  COM_METHOD_START
  com::Ptr<CBCI2000Controller> pObject = new CBCI2000Controller;
  pObject->setType( this );
  pObject->callConstructor( inIndex, inArgs );
  *outObject = pObject;
  (*outObject)->AddRef();
  COM_METHOD_END
}

STDMETHODIMP CBCI2000ControllerType::getLibrary(IPCLLibrary** outLib)
{
  COM_METHOD_START
  *outLib = mpLibrary;
  (*outLib)->AddRef();
  COM_METHOD_END
}

STDMETHODIMP CBCI2000ControllerType::getLastError(BSTR* pResult)
{
  *pResult = ::SysAllocString( mLastError.c_str() );
  return S_OK;
}

STDMETHODIMP CBCI2000ControllerType::getDescription(BSTR* pResult)
{
  COM_METHOD_START
  *pResult = ::SysAllocString( Description() );
  COM_METHOD_END
}

