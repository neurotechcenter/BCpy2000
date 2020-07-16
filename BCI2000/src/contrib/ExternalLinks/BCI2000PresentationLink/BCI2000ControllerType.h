////////////////////////////////////////////////////////////////////////////////
// $Id: BCI2000ControllerType.h 3918 2012-04-04 14:52:38Z mellinger $
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
#ifndef BCI2000_CONTROLLER_TYPE
#define BCI2000_CONTROLLER_TYPE

#include <string>
#include "ComCoClass.h"
#include "BCI2000PresentationLink_i.h"

class CBCI2000ControllerType
: public com::CoClass<IPCLType, &IID_IPCLType, CBCI2000ControllerType, &CLSID_BCI2000ControllerType>
{
public:
  static const wchar_t* TypeName() { return L"controller"; }
  static const wchar_t* Description() { return L"Create an instance of this type to control BCI2000."; }

  CBCI2000ControllerType();

public:
  // IPCLType Methods
  STDMETHOD(getName)(BSTR * name);
  STDMETHOD(getDescription)(BSTR * desc);
  STDMETHOD(getParentName)(BSTR* parent);
  STDMETHOD(getMethods)(IPCLMethodList * list);
  STDMETHOD(createObject)(long index, IPCLArguments* arguments, IPCLObject** object);
  STDMETHOD(getLibrary)(IPCLLibrary** lib);
  STDMETHOD(getLastError)(BSTR * message);

  // Interface to BCI2000PCLLibrary
  void setLibrary(IPCLLibrary* pLib) { mpLibrary = pLib; }

private:
  com::Ptr<IPCLLibrary> mpLibrary;
  std::wstring mLastError;
};

#endif // BCI2000_CONTROLLER_TYPE
