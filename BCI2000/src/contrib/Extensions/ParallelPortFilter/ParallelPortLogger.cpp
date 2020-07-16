////////////////////////////////////////////////////////////////////////////////
// $Id$
// Author: kaleb.goering@gmail.com, juergen.mellinger@uni-tuebingen.de
// Description: Logs a parallel port's data lines into a state variable.
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
#include "ParallelPortLogger.h"

#include "Thread.h"
#include "WaitableEvent.h"
#include "BCIEvent.h"
#include "ParallelPort.h"

Extension( ParallelPortLogger );

struct ParallelPortLogger::Private : Thread
{
  Private();
  ~Private();
  int OnExecute();

  std::vector<std::string> mPortNames;
  struct Input
  {
    ParallelPort::Ptr port;
    std::string event;
    uint16_t value;
  };
  std::vector<Input> mInputs;
};

ParallelPortLogger::Private::Private()
: Thread( "ParallelPortLogger" )
{
}

ParallelPortLogger::Private::~Private()
{
  TerminateAndWait();
}

int
ParallelPortLogger::Private::OnExecute()
{
  for( auto& input : mInputs )
    input.value = 0xffff;

  WaitableTimeout timer;
  Time::Interval pause = Time::Seconds( 5e-3 );
  while( timer.Wait( pause ) )
  {
    for( auto& input : mInputs )
    {
      uint8_t data = 0;
      if( !input.port->Read( ParallelPort::Data, data ) )
        bciwarn << "Could not read from " << input.port->Name() << ": " << SysError().Message();
      if( data != input.value )
      {
        input.value = data;
        bcievent << input.event << " " << input.value;
      }
    }
  }
  return 0;
}

// ParallelPortLogger
ParallelPortLogger::ParallelPortLogger()
: p( new Private )
{
}

ParallelPortLogger::~ParallelPortLogger()
{
  delete p;
}

void
ParallelPortLogger::Publish()
{
  if( Parameters->Exists( "LogParallelPort" ) )
  {
    BEGIN_PARAMETER_DEFINITIONS
      "ParallelPort:Input string LogParallelPort= % % % %"
        " // comma-separated list of parallel port device names to log from (noedit)",
    END_PARAMETER_DEFINITIONS

    std::istringstream iss( Parameter( "LogParallelPort" ).c_str() );
    std::string name;
    while( std::getline( iss >> std::ws, name, ',' ) )
    {
      p->mPortNames.push_back( name );
      std::string state = "ParallelPort" + name + " 8 0";
      BEGIN_EVENT_DEFINITIONS
        state.c_str(),
      END_EVENT_DEFINITIONS
    }
  }
}

void
ParallelPortLogger::AutoConfig()
{
  p->mInputs.clear();
  if( !p->mPortNames.empty() )
  {
    ParallelPort::List ports;
    ParallelPort::Enumerate( ports );
    for( const auto& name : p->mPortNames )
    {
      auto port = ports.Find( name );
      if( !port )
        bcierr << "Parallel port device \"" << name << "\" is not present in the system";
      else
        port->Open();
      Private::Input input = { port, "ParallelPort" + port->Name(), 0xffff };
      p->mInputs.push_back( input );
    }
  }
}

void
ParallelPortLogger::Initialize()
{
  for( const auto& input : p->mInputs )
  {
    static const uint8_t inputMode = 0x20;
    input.port->Write( ParallelPort::DCR, inputMode );
    uint8_t value = 0;
    input.port->Read( ParallelPort::DCR, value );
    if( value & inputMode != inputMode )
      bcierr << "Could not switch " << input.port->Name() << " to input mode";
  }
}

void
ParallelPortLogger::StartRun()
{
  p->Start();
}

void
ParallelPortLogger::StopRun()
{
  Halt();
}

void
ParallelPortLogger::Halt()
{
  p->Terminate();
}

