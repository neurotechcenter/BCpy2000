//////////////////////////////////////////////////////////////////////
// $Id: main.cpp 5818 2018-11-09 15:50:15Z mellinger $
// Authors: juergen.mellinger@uni-tuebingen.de
// Description: A demo program using the OperatorLib to load a
//   parameter file and execute a run before exiting.
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
///////////////////////////////////////////////////////////////////////
#include "BCI_OperatorLib.h"
#include "ThreadUtils.h"

#include <iostream>
#include <sstream>

using namespace std;

void STDCALL OnConnect( void* );
void STDCALL OnSetConfig( void* );
void STDCALL OnSuspend( void* );
void STDCALL OnLogMessage( void*, const char* );
void STDCALL OnWarningMessage( void*, const char* );
void STDCALL OnErrorMessage( void*, const char* );

struct ProgramData
{
  const char* parameterfile;
  bool terminate;
};


int
main( int argc, char* argv[] )
{
  struct ProgramData data =
  {
    NULL,
    false
  };

  if( argc > 1 )
    data.parameterfile = argv[1];

  BCI_Initialize();

  BCI_SetExternalCallback( BCI_OnConnect, BCI_Function( OnConnect ), &data );
  BCI_SetExternalCallback( BCI_OnSetConfig, BCI_Function( OnSetConfig ), &data );
  BCI_SetExternalCallback( BCI_OnSuspend, BCI_Function( OnSuspend ), &data );

  BCI_SetExternalCallback( BCI_OnLogMessage, BCI_Function( OnLogMessage ), &data );
  BCI_SetExternalCallback( BCI_OnWarningMessage, BCI_Function( OnWarningMessage ), &data );
  BCI_SetExternalCallback( BCI_OnErrorMessage, BCI_Function( OnErrorMessage ), &data );
  BCI_SetExternalCallback( BCI_OnScriptError, BCI_Function( OnErrorMessage ), &data );

  BCI_Startup( "SignalSource:4000 SignalProcessing:4001 Application:4002" );

  while( !data.terminate )
  {
    BCI_CheckPendingCallback();
    ThreadUtils::SleepForMs( 50 );

    switch( BCI_GetStateOfOperation() )
    {
      case BCI_StateInitialization:
        /* update status information */
        break;

      case BCI_StateResting:
        /* update status information */
        break;

      case BCI_StateSuspended:
        /* update status information */
        break;

      case BCI_StateRunning:
        /* update status information */
        break;
    }
  }

  while( BCI_CheckPendingCallback() );
  BCI_Shutdown();
  BCI_Dispose();
}

void STDCALL
OnConnect( void* inData )
{
  ProgramData* pData = static_cast<ProgramData*>( inData );
  if( pData->parameterfile )
  {
    ostringstream oss;
    oss << "Load Parameterfile " << pData->parameterfile;
    BCI_ExecuteScript( oss.str().c_str() );
  }
  if( !pData->terminate )
    BCI_SetConfig();
}

void STDCALL
OnSetConfig( void* )
{
  BCI_StartRun();
}

void STDCALL
OnSuspend( void* inData )
{
  ProgramData* pData = static_cast<ProgramData*>( inData );
  pData->terminate = true;
}

void STDCALL
OnLogMessage( void*, const char* s )
{
  cout << s << endl;
}

void STDCALL
OnWarningMessage( void*, const char* s )
{
  cout << s << endl;
}

void STDCALL
OnErrorMessage( void* inData, const char* s )
{
  cout << s << endl;

  ProgramData* pData = static_cast<ProgramData*>( inData );
  pData->terminate = true;
}

