/////////////////////////////////////////////////////////////////////////////
// $Id: TestLogger.cpp 5818 2018-11-09 15:50:15Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A logger component designed for stress-testing the BCI2000
//   bcievent mechanism, and thread-safe bcistreams.
//   To activate it, set the TestLoggerFrequency parameter from the command
//   line:
//   SignalGenerator --TestLoggerFrequency=1kHz
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
/////////////////////////////////////////////////////////////////////////////
#include "TestLogger.h"
#include "BCIEvent.h"
#include "BCIStream.h"

using namespace std;

Extension( TestLogger );

TestLogger::TestLogger()
: mTestLoggerFrequency( 0 ),
  mTestLoggerEmitDebug( true ),
  mTestLoggerCounter( 0 )
{
}

TestLogger::~TestLogger()
{
  Halt();
}

void
TestLogger::Publish()
{
  bool enabled = ( OptionalParameter( "TestLoggerFrequency", 0 ) > 0 );
  if( enabled )
  {
    BEGIN_PARAMETER_DEFINITIONS
      "Source float TestLoggerFrequency= 256Hz % % % // Frequency of emission for bcievents",
      "Source int   TestLoggerEmitDebug= 1 1 0 1 // Emit bcidbg messages (boolean)",
    END_PARAMETER_DEFINITIONS

    BEGIN_EVENT_DEFINITIONS
      "TestLoggerCounter 32 4294967295 0 0",
    END_EVENT_DEFINITIONS
  }
}

void
TestLogger::Preflight() const
{
  PreflightCondition( OptionalParameter( "TestLoggerFrequency", 0 ).InHertz() >= 0 );
}

void
TestLogger::Initialize()
{
  mTestLoggerFrequency = OptionalParameter( "TestLoggerFrequency", 0 ).InHertz();
  if( mTestLoggerFrequency > 0 )
    mTestLoggerEmitDebug = ( Parameter( "TestLoggerEmitDebug" ) == 1 );
}

void
TestLogger::StartRun()
{
  if( mTestLoggerFrequency > 0 )
    Thread::Start();
}

void
TestLogger::StopRun()
{
  Thread::Terminate();
}

void
TestLogger::Halt()
{
  StopRun();
}

int
TestLogger::OnExecute()
{
  if( mTestLoggerEmitDebug )
    bcidbg( 0 ) << "Started logging, counter is " << mTestLoggerCounter << endl;

  while( !Thread::Terminating() )
  {
    PrecisionTime wakeupTime = PrecisionTime::Now() + static_cast<int>( 1000 / mTestLoggerFrequency );
    bcievent << "TestLoggerCounter " << mTestLoggerCounter << flush;
    if( mTestLoggerEmitDebug )
      bcidbg( 0 ) << "Logging, counter is " << mTestLoggerCounter << endl;
    ++mTestLoggerCounter;
    ThreadUtils::SleepUntil( wakeupTime );
  }

  if( mTestLoggerEmitDebug )
    bcidbg( 0 ) << "Finished logging, counter is " << mTestLoggerCounter << endl;
  return 0;
}


