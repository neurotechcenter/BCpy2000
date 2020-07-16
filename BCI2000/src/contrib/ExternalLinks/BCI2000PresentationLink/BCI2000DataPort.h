////////////////////////////////////////////////////////////////////////////////
// $Id: BCI2000DataPort.h 3918 2012-04-04 14:52:38Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: An IDataPort implementation that handles creation and
//   user configuration of IActiveDataPort objects.
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
#ifndef BCI2000_DATA_PORT_H
#define BCI2000_DATA_PORT_H

#include <vector>
#include "ComCoClass.h"
#include "ComStrings.h"
#include "Utils.h"
#include "BCI2000PresentationLink_i.h"
#include "BCI2000ActiveDataPort.h"

struct DataPortSettings : std::vector<com::DualString>
{
  enum Fields
  {
    StartBCI2000 = 0,
    OperatorPath,
    HideOperator,
    TelnetAddress,
    EventTypes,
    UseResumePause,

    NumFields
  };
  DataPortSettings( const std::wstring& = L"" );
  operator std::wstring() const;
};

// CBCI2000DataPort
class CBCI2000DataPort
: public com::CoClass<IDataPort, &IID_IDataPort, CBCI2000DataPort, &CLSID_BCI2000DataPort>
{
public:
  CBCI2000DataPort() {}

  // IDataPort Methods
public:
  STDMETHOD(getPropertiesViewable)(unsigned char * viewable)
  {
    COM_METHOD_START
    *viewable = 1;
    COM_METHOD_END
  }
  STDMETHOD(showProperties)(HWND parentWindow)
  {
    COM_METHOD_START
    EditProperties( parentWindow );
    COM_METHOD_END
  }
  STDMETHOD(setDefaults)()
  {
    COM_METHOD_START
    mSettings = DataPortSettings();
    COM_METHOD_END
  }
  STDMETHOD(getProperties)(BSTR * parameters)
  {
    COM_METHOD_START
    *parameters = ::SysAllocString( com::DualString( mSettings ) );
    COM_METHOD_END
  }
  STDMETHOD(setProperties)(BSTR parameters)
  {
    COM_METHOD_START
    mSettings = DataPortSettings( com::DualString( parameters ) );
    COM_METHOD_END
  }
  STDMETHOD(acquire)(IActiveDataPort** outDevice)
  {
    COM_METHOD_START
    com::Ptr<CBCI2000ActiveDataPort> pObject = new CBCI2000ActiveDataPort;
    pObject->Initialize( mSettings );
    *outDevice = pObject;
    (*outDevice)->AddRef();
    COM_METHOD_END
  }
  STDMETHOD(getLastError)(BSTR * error)
  {
    *error = ::SysAllocString( mLastError.c_str() );
    return S_OK;
  }

private:
  void EditProperties( HWND );

  std::wstring mLastError;
  DataPortSettings mSettings;
};

#endif // BCI2000_DATA_PORT_H
