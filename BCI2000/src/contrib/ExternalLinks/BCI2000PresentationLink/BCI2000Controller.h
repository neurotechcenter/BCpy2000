////////////////////////////////////////////////////////////////////////////////
// $Id: BCI2000Controller.h 3975 2012-04-27 10:44:16Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: An IPCLObject implementation representing an instantiated
//  object in Presentation's PCL language. A BCI2000Controller object forwards
//  the member functions of BCI2000Remote to PCL programs, allowing to
//  control BCI2000 from PCL.
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
#ifndef BCI2000_CONTROLLER
#define BCI2000_CONTROLLER

#include <string>
#include "ComCoClass.h"
#include "BCI2000PresentationLink_i.h"
#include "BCI2000Remote.h"
#include "BCI2000ActiveDataPort.h"
#include "Utils.h"

class CBCI2000Controller
: public com::CoClass<IPCLObject, &IID_IPCLObject, CBCI2000Controller, &CLSID_BCI2000Controller>
{
public:
  CBCI2000Controller() {}

public:
  // IPCLObject Methods
  STDMETHOD(callMethod)(long index, IPCLArguments*);
  STDMETHOD(getType)(IPCLType**);
  STDMETHOD(getLastError)(BSTR*);

  // Interface for ControllerType
  void setType( IPCLType* p ) { mpType = p; }
  void callConstructor(long index, IPCLArguments*);
  static void getMethods(IPCLMethodList*);

  // PCL methods
  void constructor_void( ArgList& );
  void constructor_data_port_ext( ArgList& );
  void constructor_string( ArgList& );

  void hide_window( ArgList& );
  void show_window( ArgList& );
  void set_window_title( ArgList& );
  void set_subject( ArgList& );
  void set_session( ArgList& );
  void set_data_directory( ArgList& );

  void startup_modules( ArgList& );
  void set_config( ArgList& );
  void start( ArgList& );
  void stop( ArgList& );

  void load_parameters_local( ArgList& );
  void load_parameters_remote( ArgList& );
  void get_parameter( ArgList& );
  void set_parameter( ArgList& );

  void add_state_variable( ArgList& );
  void get_state_variable( ArgList& );
  void set_state_variable( ArgList& );

  void get_system_state( ArgList& );
  void get_control_signal( ArgList& );

  void execute( ArgList& );
  void set_script( ArgList& );
  void get_script( ArgList& );

private:
  std::wstring mLastError;
  com::Ptr<IPCLType> mpType;
  com::Ptr<CBCI2000ActiveDataPort> mpDataPort;
  BCI2000Remote mBCI2000;
};

#endif // BCI2000_CONTROLLER
