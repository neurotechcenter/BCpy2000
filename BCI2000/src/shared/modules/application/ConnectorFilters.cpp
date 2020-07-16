////////////////////////////////////////////////////////////////////////////////
// $Id: ConnectorFilters.cpp 5384 2016-05-31 13:31:54Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A pair of filters that send/receive states and signals over a
//         UDP connection.
//
//         Data transmission is done via UDP socket connections.
//         Messages consist of a name and a value, separated by white space
//         and terminated with a single newline '\n' character.
//
//         Names may identify
//         -- BCI2000 states by name, and are then followed
//            by an integer value in decimal ASCII representation;
//         -- Signal elements in the form Signal(<channel>,<element>), and are
//            then followed by a float value in decimal ASCII representation.
//
//         Examples:
//           Running 0
//           ResultCode 2
//           Signal(1,2) 1e-8
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
#include "PCHIncludes.h"
#pragma hdrstop

#include "ConnectorFilters.h"
#include "BCIException.h"
#include "WildcardMatch.h"

#define SECTION "Connector"

using namespace std;
using namespace bci;

RegisterFilter( ConnectorInput,  2.9999 ); // Place the input filter
                                           // immediately before the task
                                           // filter.
RegisterFilter( ConnectorOutput, 3.9999 ); // Place the output filter
                                           // last in the application module.

ConnectorInput::ConnectorInput()
: mConnectorInputAddress( "" ),
  mConnection( &mStreambuf )
{
  mStreambuf.SetIO( &mSocket );

  BEGIN_PARAMETER_DEFINITIONS
    SECTION " list ConnectorInputFilter= 0 "
      "% % % // list of state names or signal elements to allow, "
      "\"*\" for any, signal elements as in \"Signal(1,0)\"",
    SECTION " string ConnectorInputAddress= % "
      "localhost:20320 % % // IP address/port to read from, e.g. localhost:20320",
  END_PARAMETER_DEFINITIONS
}

ConnectorInput::~ConnectorInput()
{
}

void
ConnectorInput::Preflight( const SignalProperties& inSignalProperties,
                                 SignalProperties& outSignalProperties ) const
{
  string connectorInputAddress = Parameter( "ConnectorInputAddress" );
  if( connectorInputAddress != "" )
  {
    receiving_udpsocket preflightSocket( connectorInputAddress.c_str() );
    if( !preflightSocket.is_open() )
      bcierr << "Could not connect to " << connectorInputAddress << endl;
  }
  Parameter( "ConnectorInputFilter" );

  // Pre-flight access each state in the list.
  for( const auto& state : *States )
    State( state.Name() );

  outSignalProperties = inSignalProperties;
}

void
ConnectorInput::Initialize( const SignalProperties&, const SignalProperties& )
{
  mConnectorInputAddress = string( Parameter( "ConnectorInputAddress" ) );
  mInputFilters.clear();
  ParamRef ConnectorInputFilter = Parameter( "ConnectorInputFilter" );
    for( int i = 0; i < ConnectorInputFilter->NumValues(); ++i )
      mInputFilters.push_back( ConnectorInputFilter( i ) );
}

void
ConnectorInput::StartRun()
{
  if( mConnectorInputAddress != "" )
  {
    mSocket.Open( mConnectorInputAddress );
    if( !mSocket.IsOpen() )
      bcierr << "Could not open " << mConnectorInputAddress;
  }
}

void
ConnectorInput::StopRun()
{
  mConnection.clear();
  mSocket.Close();
}

void
ConnectorInput::Process( const GenericSignal& Input, GenericSignal& Output )
{
  try
  {
    Output = Input;
    string buffer;
    while( mConnection && mConnection.rdbuf()->in_avail() > 0 )
      buffer += mConnection.get();
    istringstream iss( buffer );
    string name;
    while( iss >> name )
    {
      double value;
      string remainder;
      if( !std::getline( iss >> value, remainder ) || !remainder.empty() )
        throw bcierr << "Malformed input, expected state name, followed by a single number as a value,"
                        "space-separated, and terminated with a newline character";
      bool match = false;
      for( vector<string>::const_iterator i = mInputFilters.begin(); i != mInputFilters.end() && !match; ++i )
        match = match || WildcardMatch( *i, name, false );
      if( match )
      {
        if( name.find( "Signal(" ) == 0 )
        {
          istringstream iss( name.substr( name.find( '(' ) ) );
          char ignore;
          int channel = 0,
              element = 0;
          if( !( iss >> ignore >> channel >> ignore >> element >> ignore ) )
            throw bcierr << "Incorrect Signal index syntax: " << name;
          if( channel >= Input.Channels() || element >= Input.Elements() )
            throw bcierr << "Received signal index out-of-bounds: " << name;
          Output( channel, element ) = value;
        }
        else
        {
          if( !States->Exists( name ) )
            throw bcierr << "Ignoring value for non-existent " << name << " state";
          State( name.c_str() ) = static_cast<int>( value );
        }
      }
    }
  }
  catch( const bci::Exception& e )
  {
    bciout << e.What() << endl;
  }
}


// ConnectorOutput
ConnectorOutput::ConnectorOutput()
{
  BEGIN_PARAMETER_DEFINITIONS
    SECTION " string ConnectorOutputAddress= % "
      "localhost:20321 % % // one or more IP:Port combinations, e.g. localhost:20321",
  END_PARAMETER_DEFINITIONS
}

ConnectorOutput::~ConnectorOutput()
{
  DeleteConnections();
}

void
ConnectorOutput::Preflight( const SignalProperties& inSignalProperties,
                                  SignalProperties& outSignalProperties ) const
{
  string connectorOutputAddresses = string( Parameter( "ConnectorOutputAddress" ) );
  istringstream iss( connectorOutputAddresses );
  string address;
  while( iss >> address )
  {
    sending_udpsocket preflightSocket( address.c_str() );
    if( !preflightSocket.is_open() )
      bcierr << "Could not connect to " << address << endl;
  }
  // Pre-flight access each state in the list.
  for( const auto& state : *States )
    State( state.Name() );

  outSignalProperties = inSignalProperties;
}

void
ConnectorOutput::Initialize( const SignalProperties&, const SignalProperties& )
{
  mConnectorOutputAddresses = string( Parameter( "ConnectorOutputAddress" ) );
}

void
ConnectorOutput::StartRun()
{
  istringstream iss( mConnectorOutputAddresses );
  string address;
  while( iss >> address )
  {
    mConnections.push_back( new Connection( address ) );
    if( !mConnections.back()->IsOpen() )
      bcierr << "Could not connect to " << address << endl;
    else
      mConnections.back()->Start();
  }
}

void
ConnectorOutput::StopRun()
{
  DeleteConnections();
}

void
ConnectorOutput::Process( const GenericSignal& Input, GenericSignal& Output )
{
  Output = Input;
  for( vector<Connection*>::iterator i = mConnections.begin(); i != mConnections.end(); ++i )
  {
    WithLock( **i )
    {
      ( **i ).Clear();
      for( const auto& state : *States )
        **i << state.Name() << ' ' << State( state.Name() ) << '\n';
      for( int channel = 0; channel < Input.Channels(); ++channel )
        for( int element = 0; element < Input.Elements(); ++element )
          **i << "Signal(" << channel << "," << element << ") "
              << Input( channel, element )
              << '\n';
      ( **i ).flush();
    }
    ( **i ).WaitableEvent::Set();
  }
}

void
ConnectorOutput::DeleteConnections()
{
  for( vector<Connection*>::iterator i = mConnections.begin(); i != mConnections.end(); ++i )
    delete *i;
  mConnections.clear();
}

int
ConnectorOutput::Connection::OnExecute()
{
  const int cReactionTimeMs = 100;
  while( !Terminating() )
  {
    if( WaitableEvent::Wait( Time::FromIntTimeout( cReactionTimeMs ) ) )
    {
      Reset();
      bool result = true;
      while( result && !Terminating() )
      {
        string message;
        WithLock( this )
          result = !!std::getline( *this, message );
        if( result && !Terminating() )
        {
          message += "\n";
          mSocket.Write( message.c_str(), message.length() );
        }
      }
    }
  }
  return 0;
}

