////////////////////////////////////////////////////////////////////////////////
// $Id: BCI2000DataPortExtension.h 3918 2012-04-04 14:52:38Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: An IDataPortExtension implementation that allows addition of
//   BCI2000 data port objects to Presentation experiments.
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
#ifndef BCI2000_DATA_PORT_EXTENSION_H
#define BCI2000_DATA_PORT_EXTENSION_H

#include "Resource.h"
#include "BCI2000PresentationLink_i.h"
#include "BCI2000PCLLibrary.h"
#include "BCI2000DataPort.h"
#include "Utils.h"

class CBCI2000DataPortExtension
: public com::CoClass<IDataPortExtension, &IID_IDataPortExtension, CBCI2000DataPortExtension, &CLSID_BCI2000DataPortExtension>
{
public:
  static const wchar_t* Name() { return L"BCI2000PresentationLink Data Port"; }
  static const wchar_t* Description() { return L"Allows recording of Presentation events in BCI2000."; }

  CBCI2000DataPortExtension() {}

  // IDataPortExtension Methods
public:
  STDMETHOD(getName)(BSTR * name)
  {
    COM_METHOD_START
    *name = SysAllocString( Name() );
    COM_METHOD_END
  }
  STDMETHOD(getAuthor)(BSTR * author)
  {
    COM_METHOD_START
    *author = SysAllocString( CBCI2000PCLLibrary::Author() );
    COM_METHOD_END
  }
  STDMETHOD(getVersion)(BSTR * ver)
  {
    COM_METHOD_START
    *ver = SysAllocString( CBCI2000PCLLibrary::Version() );
    COM_METHOD_END
  }
  STDMETHOD(getDescription)(BSTR * desc)
  {
    COM_METHOD_START
    *desc = SysAllocString( Description() );
    COM_METHOD_END
  }
  STDMETHOD(addPort)(IDataPort** outPort)
  {
    COM_METHOD_START
    com::Ptr<CBCI2000DataPort> pObject = new CBCI2000DataPort;
    *outPort = pObject;
    (*outPort)->AddRef();
    COM_METHOD_END
  }
  STDMETHOD(getLastError)(BSTR * error)
  {
    COM_METHOD_START
    *error = SysAllocString( mLastError.c_str() );
    COM_METHOD_END
  }

private:
  std::wstring mLastError;

};

#endif // BCI2000_DATA_PORT_EXTENSION_H
