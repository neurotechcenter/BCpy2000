////////////////////////////////////////////////////////////////////////////////
// $Id: BCI2000Controller.cpp 4970 2015-08-21 16:46:07Z mellinger $
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
#include "BCI2000Controller.h"
#include "BCI2000DataPort.h"
#include "BCI2000ActiveDataPort.h"
#include <string>

#define METHOD( x ) &CBCI2000Controller::x, OLESTR( #x )
static const struct Method<CBCI2000Controller> sConstructors[] =
{
  {
    METHOD( constructor_void ),
    L"Creates a controller connecting to the first BCI2000 data port, "
    L"or to the BCI2000 installation in which BCI2000PresentationLink resides.",
    {},
  },
#if 0 // PCL does not (yet?) accept data_port_ext as a valid argument type.
  {
    METHOD( constructor_data_port_ext ),
    L"Creates a controller connecting to the specified BCI2000 data port.",
    {
      {
        L"data_port", L"data_port_ext", 0, true,
        L"a BCI2000 data port reference, as returned by program_output_port_manager::get_data_port()"
      },
    },
  },
#endif
  {
    METHOD( constructor_string ),
    L"Creates a controller connecting to a BCI2000 Operator module, or to a network address.",
    {
      {
        L"location", L"string", 0, false,
        L"path to a BCI2000 Operator module, or network address <ip>:<port>"
      },
    },
  },
};

static const struct Method<CBCI2000Controller> sMethods[] =
{
  {
    METHOD( hide_window ),
    L"Hides the BCI2000 Operator main window.",
    {},
  },
  {
    METHOD( show_window ),
    L"Shows the BCI2000 Operator main window.",
    {},
  },
  {
    METHOD( set_window_title ),
    L"Sets the title of the BCI2000 Operator main window.",
    {
      { L"title", L"string", 0, false, L"window title" },
    },
  },
  {
    METHOD( set_subject ),
    L"Sets a subject ID for the BCI2000 recording.",
    {
      { L"subject_id", L"string", 0, false, L"subject ID" },
    },
  },
  {
    METHOD( set_session ),
    L"Sets a session ID for the BCI2000 recording.",
    {
      { L"session_id", L"string", 0, false, L"session ID" },
    },
  },
  {
    METHOD( set_data_directory ),
    L"Sets a data directory for the BCI2000 recording, relative to the BCI2000 prog directory. "
    L"Data files will be placed into a subject and session specific directory below the data directory.",
    {
      { L"data_directory", L"string", 0, false, L"absolute or relative path of data directory" },
    },
  },
  {
    METHOD( startup_modules ),
    L"Starts up BCI2000 core modules.",
    {
      {
        L"list_of_modules", L"string", 1, false,
        L"array containing module names (typically, Source, Signal Processing, and Application module)"
      },
    },
  },
  {
    METHOD( set_config ),
    L"Applies current set of parameters to the BCI2000 system.",
    {},
  },
  {
    METHOD( start ),
    L"Starts operation.",
    {},
  },
  {
    METHOD( stop ),
    L"Suspends operation.",
    {},
  },
  {
    METHOD( get_parameter ),
    L"Returns the value of a BCI2000 parameter.",
    {
      { L"name", L"string", 0, false, L"parameter name" },
    },
    { L"string", 0, L"Parameter value" }
  },
  {
    METHOD( set_parameter ),
    L"Sets the value of a BCI2000 parameter.",
    {
      { L"name", L"string", 0, false, L"parameter name" },
      { L"value", L"string", 0, false, L"parameter value" },
    },
  },
  {
    METHOD( load_parameters_local ),
    L"Loads a parameter file relative to Presentation's working directory.",
    {
      { L"parameter_file", L"string", 0, false, L"path to parameter file" },
    },
  },
  {
    METHOD( load_parameters_remote ),
    L"Loads a parameter file relative to the BCI2000/prog directory.",
    {
      { L"parameter_file", L"string", 0, false, L"path to parameter file" },
    },
  },
  {
    METHOD( add_state_variable ),
    L"Adds a BCI2000 state variable.",
    {
      { L"name", L"string", 0, false, L"name of state variable" },
      { L"bit_width", L"int", 0, false, L"number of bits in the state variable" },
      { L"initial_value", L"int", 0, false, L"initial value of the state variable" },
    },
  },
  {
    METHOD( get_state_variable ),
    L"Returns the value of a BCI2000 state variable.",
    {
      { L"name", L"string", 0, false, L"name of state variable" },
    },
    { L"int", 0, L"State variable value" }
  },
  {
    METHOD( set_state_variable ),
    L"Sets the value of a BCI2000 state variable.",
    {
      { L"name", L"string", 0, false, L"name of state variable" },
      { L"value", L"int", 0, false, L"state variable value" },
    },
  },
  {
    METHOD( get_control_signal ),
    L"Returns the value of an element in the control signal.",
    {
      { L"channel", L"int", 0, false, L"one-based channel index" },
      { L"element", L"int", 0, false, L"one-based element index" },
    },
    { L"double", 0, L"control signal value" }
  },
  {
    METHOD( execute ),
    L"Executes a BCI2000 Operator scripting command.",
    {
      { L"command", L"string", 0, false, L"command to execute" },
      { L"output", L"string", 0, true, L"command output" },
    },
    { L"int", 0, L"command exit code, or 0" }
  },
  {
    METHOD( set_script ),
    L"Associates BCI2000 Operator scripting commands with an event.",
    {
      { L"event_name", L"string", 0, false, L"name of event" },
      { L"script", L"string", 0, false, L"sequence of scripting commands" },
    },
  },
  {
    METHOD( get_script ),
    L"Returns BCI2000 Operator scripting commands associated with an event.",
    {
      { L"event_name", L"string", 0, false, L"name of event" },
    },
    { L"string", 0, L"scripting commands" },
  },
};

static const int sMaxArguments = sizeof( sConstructors[0].arguments ) / sizeof( *sConstructors[0].arguments );
static const int sNumConstructors = sizeof( sConstructors ) / sizeof( *sConstructors );
static const int sNumMethods = sizeof( sMethods ) / sizeof( *sMethods );

//---------------------------------------------------------------------------
// CBCI2000Controller

STDMETHODIMP CBCI2000Controller::callMethod(long inIndex, IPCLArguments* pArgs)
{
  COM_METHOD_START
  if( inIndex < 0 || inIndex >= sNumMethods )
    throw bciexception << "Invalid function index: " << inIndex << " in CBCI2000Controller::callMethod";

  ArgList args( sMethods[inIndex], pArgs );
  (this->*sMethods[inIndex].function)( args );
  COM_METHOD_END
}

STDMETHODIMP CBCI2000Controller::getType(IPCLType** outType)
{
  COM_METHOD_START
  *outType = mpType;
  (*outType)->AddRef();
  COM_METHOD_END
}

STDMETHODIMP CBCI2000Controller::getLastError(BSTR* pResult)
{
  *pResult = ::SysAllocString( mLastError.c_str() );
  return S_OK;
}

void CBCI2000Controller::callConstructor(long inIndex, IPCLArguments* pArgs)
{
  if( inIndex < 0 || inIndex >= sNumConstructors )
    throw bciexception << "Invalid index: " << inIndex << " in CBCI2000Controller::callConstructor";
  ArgList args( sConstructors[inIndex], pArgs );
  (this->*sConstructors[inIndex].function)( args );
}

void
CBCI2000Controller::getMethods(IPCLMethodList* ioList)
{
  com::Ptr<IPCLMethodList> pList = ioList;
  for( int i = 0; i < sNumConstructors; ++i )
  {
    com::Ptr<IPCLMethod> pMethod = NULL;
    pList->addNew( pMethod.Assignee() );
    pMethod->setDescription( com::BString( sConstructors[i].description ) );
    for( int j = 0; j < sMaxArguments && sConstructors[i].arguments[j].name != NULL; ++j )
    {
      const Method<CBCI2000Controller>::Argument& arg = sConstructors[i].arguments[j];
      pMethod->addArgument( com::BString( arg.name ), com::BString( arg.type ), arg.rank, arg.isRef, com::BString( arg.description ) );
    }
  }
  for( int i = 0; i < sNumMethods; ++i )
  {
    com::Ptr<IPCLMethod> pMethod = NULL;
    pList->addMethod( pMethod.Assignee() );
    pMethod->setName( com::BString( sMethods[i].name ) );
    pMethod->setDescription( com::BString( sMethods[i].description ) );
    for( int j = 0; j < sMaxArguments && sMethods[i].arguments[j].name != NULL; ++j )
    {
      const Method<CBCI2000Controller>::Argument& arg = sMethods[i].arguments[j];
      pMethod->addArgument( com::BString( arg.name ), com::BString( arg.type ), arg.rank, arg.isRef, com::BString( arg.description ) );
    }
    const Method<CBCI2000Controller>::Result& result = sMethods[i].result;
    if( result.type != NULL )
      pMethod->setReturnValue( com::BString( result.type ), result.rank, com::BString( result.description ) );
  }
}

//---------------------------------------------------------------------------
// Controller functionality
void
CBCI2000Controller::constructor_void( ArgList& )
{
  mpDataPort = CBCI2000ActiveDataPort::FirstInstance();
  if( !mpDataPort )
  {
    mpDataPort = new CBCI2000ActiveDataPort;
    mpDataPort->Initialize( DataPortSettings() );
  }
  mBCI2000.Connect( mpDataPort->BCI2000() );
}

void
CBCI2000Controller::constructor_data_port_ext( ArgList& ioArgs )
{
  com::Ptr<IActiveDataPort> pInterface;
  ioArgs[1]->QueryInterface( IID_IActiveDataPort, pInterface.Assignee() );
  if( !pInterface )
    throw bciexception << "Argument is not an IActiveDataPort descendant";
  mpDataPort = dynamic_cast<CBCI2000ActiveDataPort*>( static_cast<IActiveDataPort*>( pInterface ) );
  if( !mpDataPort )
    throw bciexception << "Argument does not refer to a BCI2000PresentationLink data port";
  mBCI2000.Connect( mpDataPort->BCI2000() );
}

void
CBCI2000Controller::constructor_string( ArgList& ioArgs )
{
  DataPortSettings settings;
  std::wstring arg = ioArgs.GetString( 1 );
  bool isNetworkAddress = ( arg.find_first_of( L"\\/" ) == std::wstring::npos );
  if( isNetworkAddress )
  {
    settings[DataPortSettings::StartBCI2000] = L"";
    settings[DataPortSettings::TelnetAddress] = arg;
  }
  else
  {
    settings[DataPortSettings::StartBCI2000] = L"1";
    settings[DataPortSettings::OperatorPath] = arg;
  }
  mpDataPort = new CBCI2000ActiveDataPort;
  mpDataPort->Initialize( settings );
  mBCI2000.Connect( mpDataPort->BCI2000() );
}

#undef CALL
#define CALL(x) if( !mBCI2000.x ) throw bciexception << mBCI2000.Result()

void
CBCI2000Controller::hide_window( ArgList& )
{
  mBCI2000.WindowVisible( false );
}

void
CBCI2000Controller::show_window( ArgList& )
{
  mBCI2000.WindowVisible( true );
}

void
CBCI2000Controller::set_window_title( ArgList& ioArgs )
{
  mBCI2000.WindowTitle( ioArgs.GetString( 1 ) );
}

void
CBCI2000Controller::set_subject( ArgList& ioArgs )
{
  mBCI2000.SubjectID( ioArgs.GetString( 1 ) );
}

void
CBCI2000Controller::set_session( ArgList& ioArgs )
{
  mBCI2000.SessionID( ioArgs.GetString( 1 ) );
}

void
CBCI2000Controller::set_data_directory( ArgList& ioArgs )
{
  mBCI2000.DataDirectory( ioArgs.GetString( 1 ) );
}

void
CBCI2000Controller::startup_modules( ArgList& ioArgs )
{
  std::vector<com::DualString> modules_ = ioArgs.GetStringArray( 1 );
  std::vector<std::string> modules( modules_.begin(), modules_.end() );
  CALL( StartupModules( modules ) );
}

void
CBCI2000Controller::set_config( ArgList& )
{
  CALL( SetConfig() );
}

void
CBCI2000Controller::start( ArgList& )
{
  CALL( Start() );
}

void
CBCI2000Controller::stop( ArgList& )
{
  CALL( Stop() );
}

void
CBCI2000Controller::set_parameter( ArgList& ioArgs )
{
  CALL( SetParameter( ioArgs.GetString( 1 ), ioArgs.GetString( 2 ) ) );
}

void
CBCI2000Controller::get_parameter( ArgList& ioArgs )
{
  std::string result;
  CALL( GetParameter( ioArgs.GetString( 1 ), result ) );
  ioArgs.SetString( 0, result );
}

void
CBCI2000Controller::load_parameters_local( ArgList& ioArgs )
{
  CALL( LoadParametersLocal( ioArgs.GetString( 1 ) ) );
}

void
CBCI2000Controller::load_parameters_remote( ArgList& ioArgs )
{
  CALL( LoadParametersRemote( ioArgs.GetString( 1 ) ) );
}

void
CBCI2000Controller::add_state_variable( ArgList& ioArgs )
{
  CALL( AddStateVariable( ioArgs.GetString( 1 ), ioArgs.GetInt( 2 ), ioArgs.GetInt( 3 ) ) );
}

void
CBCI2000Controller::set_state_variable( ArgList& ioArgs )
{
  CALL( SetStateVariable( ioArgs.GetString( 1 ), ioArgs.GetInt( 2 ) ) );
}

void
CBCI2000Controller::get_state_variable( ArgList& ioArgs )
{
  double result = 0;
  CALL( GetStateVariable( ioArgs.GetString( 1 ), result ) );
  ioArgs.SetInt( 0, static_cast<unsigned int>( result ) );
}

void
CBCI2000Controller::get_control_signal( ArgList& ioArgs )
{
  double result = 0;
  CALL( GetControlSignal( ioArgs.GetInt( 1 ), ioArgs.GetInt( 2 ), result ) );
  ioArgs.SetDouble( 0, result );
}

void
CBCI2000Controller::execute( ArgList& ioArgs )
{
  std::string command = ioArgs.GetString( 1 );
  int exitCode = 0,
      result = mBCI2000.Execute( command, &exitCode );
  if( ioArgs.size() > 2 )
    ioArgs.SetInt( 1, exitCode );
  ioArgs.SetString( 2, com::DualString( mBCI2000.Result() ).ToWin() );
  ioArgs.SetInt( 0, result );
}

void
CBCI2000Controller::set_script( ArgList& ioArgs )
{
  CALL( SetScript( ioArgs.GetString( 1 ), ioArgs.GetString( 2 ) ) );
}

void
CBCI2000Controller::get_script( ArgList& ioArgs )
{
  std::string result;
  CALL( GetScript( ioArgs.GetString( 1 ), result ) );
  ioArgs.SetString( 0, result );
}

