///////////////////////////////////////////////////////////////////////////
// $Id: BCI2000RemoteLib.cpp 4536 2013-08-05 14:30:13Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: C-style interface to the BCI2000Remote class.
//   The BCI2000Remote class allows to remotely connect to, and control,
//   the BCI2000 Operator module.
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
///////////////////////////////////////////////////////////////////////////
#include <set>

#include "BCI2000RemoteLib.h"
#include "BCI2000Remote.h"

#if _WIN32
#include <Windows.h>
#endif // _WIN32

#include <cstring>

#define INSTANCE_CAST  BCI2000Remote* pInstance = reinterpret_cast<BCI2000Remote*>( inObject );

using namespace std;

#if _WIN32
int WINAPI DllEntryPoint( HINSTANCE, unsigned long, void* )
{
  return 1;
}
#endif // _WIN32

typedef set<BCI2000Remote*> BCI2000RemoteSet;
static BCI2000RemoteSet sBCI2000RemoteInstances;

typedef set<const char*> StringSet;
static StringSet sAllocatedStrings;

// An internal helper function that allocates output string buffers.
static const char* AllocateString( const char* inString )
{
  size_t len = ::strlen( inString );
  char* pCopy = new char[len + 1];
  if( pCopy != NULL )
  {
    ::strncpy( pCopy, inString, len + 1 );
    sAllocatedStrings.insert( pCopy );
  }
  return pCopy;
}

/*
function:  BCI2000Release
purpose:   Releases a string buffer, or other object allocated by the library.
arguments: String pointer, or handle to object allocated by one of the library functions.
returns:   1 if successful, 0 otherwise.
*/
DLLEXPORT int
STDCALL BCI2000Release( void* inObject )
{
  const char* c = reinterpret_cast<const char*>( inObject );
  if( sAllocatedStrings.find( c ) != sAllocatedStrings.end() )
  {
    sAllocatedStrings.erase( c );
    delete[] c;
    return 1;
  }
  if( BCI2000Remote_Delete( inObject ) )
    return 1;

  return 0;
}


/*
function:  BCI2000Remote_New
purpose:   Creates a new BCI2000Remote object.
arguments: None.
returns:   A handle to the newly created object.
*/
DLLEXPORT void*
STDCALL BCI2000Remote_New( void )
{
  BCI2000Remote* pInstance = new BCI2000Remote;
  sBCI2000RemoteInstances.insert( pInstance );
  return pInstance;
}


/*
function:  BCI2000Remote_Delete
purpose:   Deletes a BCI2000Remote object.
arguments: Handle to an existing BCI2000Remote object.
returns:   1 if successful, 0 otherwise.
*/
DLLEXPORT int
STDCALL BCI2000Remote_Delete( void* inObject )
{
  INSTANCE_CAST
  if( sBCI2000RemoteInstances.find( pInstance ) != sBCI2000RemoteInstances.end() )
  {
    sBCI2000RemoteInstances.erase( pInstance );
    delete pInstance;
    return 1;
  }
  return 0;
}


/* The following functions are property accessors, and member functions of BCI2000Remote
   objects. For documentation, see
     http://doc.bci2000.org/index.php/Programming_Reference:BCI2000Remote_Class" on
   the BCI2000 wiki.
   In this C-style interface, each function takes a handle to an existing BCI2000Remote
   object as its first argument. Use BCI2000Remote_New() to create such objects.
   When a function returns a C string, the caller is responsible for releasing that string
   once it is done with it. This is done by calling BCI2000Release() on the string.
   Function that return a C string may return NULL to indicate failure.
*/
DLLEXPORT double
STDCALL BCI2000Remote_GetTimeout( void* inObject )
{
  INSTANCE_CAST
  return pInstance->Timeout();
}

DLLEXPORT void
STDCALL BCI2000Remote_SetTimeout( void* inObject, double inTimeout )
{
  INSTANCE_CAST
  pInstance->Timeout( inTimeout );
}


DLLEXPORT const char*
STDCALL BCI2000Remote_GetOperatorPath( void* inObject )
{
  INSTANCE_CAST
  return AllocateString( pInstance->OperatorPath().c_str() );
}

DLLEXPORT void
STDCALL BCI2000Remote_SetOperatorPath( void* inObject, const char* inPath )
{
  INSTANCE_CAST
  pInstance->OperatorPath( inPath );
}


DLLEXPORT int
STDCALL BCI2000Remote_GetWindowVisible( void* inObject )
{
  INSTANCE_CAST
  return pInstance->WindowVisible();
}

DLLEXPORT void
STDCALL BCI2000Remote_SetWindowVisible( void* inObject, int inVisible )
{
  INSTANCE_CAST
  pInstance->WindowVisible( inVisible );
}


DLLEXPORT const char*
STDCALL BCI2000Remote_GetWindowTitle( void* inObject )
{
  INSTANCE_CAST
  return AllocateString( pInstance->WindowTitle().c_str() );
}

DLLEXPORT void
STDCALL BCI2000Remote_SetWindowTitle( void* inObject, const char* inTitle )
{
  INSTANCE_CAST
  pInstance->WindowTitle( inTitle );
}


DLLEXPORT const char*
STDCALL BCI2000Remote_GetTelnetAddress( void* inObject )
{
  INSTANCE_CAST
  return AllocateString( pInstance->TelnetAddress().c_str() );
}

DLLEXPORT void
STDCALL BCI2000Remote_SetTelnetAddress( void* inObject, const char* inAddress )
{
  INSTANCE_CAST
  pInstance->TelnetAddress( inAddress );
}


DLLEXPORT int
STDCALL BCI2000Remote_GetConnected( void* inObject )
{
  INSTANCE_CAST
  return pInstance->Connected();
}


DLLEXPORT const char*
STDCALL BCI2000Remote_GetResult( void* inObject )
{
  INSTANCE_CAST
  return AllocateString( pInstance->Result().c_str() );
}


DLLEXPORT const char*
STDCALL BCI2000Remote_GetSubjectID( void* inObject )
{
  INSTANCE_CAST
  return AllocateString( pInstance->SubjectID().c_str() );
}

DLLEXPORT void
STDCALL BCI2000Remote_SetSubjectID( void* inObject, const char* inSubjectID )
{
  INSTANCE_CAST
  pInstance->SubjectID( inSubjectID );
}


DLLEXPORT const char*
STDCALL BCI2000Remote_GetSessionID( void* inObject )
{
  INSTANCE_CAST
  return AllocateString( pInstance->SessionID().c_str() );
}

DLLEXPORT void
STDCALL BCI2000Remote_SetSessionID( void* inObject, const char* inSessionID )
{
  INSTANCE_CAST
  pInstance->SessionID( inSessionID );
}


DLLEXPORT const char*
STDCALL BCI2000Remote_GetDataDirectory( void* inObject )
{
  INSTANCE_CAST
  return AllocateString( pInstance->DataDirectory().c_str() );
}

DLLEXPORT void
STDCALL BCI2000Remote_SetDataDirectory( void* inObject, const char* inDataDirectory )
{
  INSTANCE_CAST
  pInstance->DataDirectory( inDataDirectory );
}


DLLEXPORT int
STDCALL BCI2000Remote_Connect( void* inObject )
{
  INSTANCE_CAST
  return pInstance->Connect();
}

DLLEXPORT int
STDCALL BCI2000Remote_Disconnect( void* inObject )
{
  INSTANCE_CAST
  return pInstance->Disconnect();
}

DLLEXPORT const char*
STDCALL BCI2000Remote_Encode( void*, const char* inValue )
{
  return AllocateString( BCI2000Remote::EncodeValue( inValue ).c_str() );
}

DLLEXPORT int
STDCALL BCI2000Remote_Execute( void* inObject, const char* inScript, int* outpExitCode )
{
  INSTANCE_CAST
  return pInstance->Execute( inScript, outpExitCode );
}

DLLEXPORT int
STDCALL BCI2000Remote_StartupModules( void* inObject, const char* inModules )
{
  INSTANCE_CAST
  const char* p = inModules;
  vector<string> modules;
  while( *p )
  {
    modules.push_back( p );
    p += modules.back().length() + 1;
  }
  return pInstance->StartupModules( modules );
}

DLLEXPORT int
STDCALL BCI2000Remote_LoadParametersLocal( void* inObject, const char* inFile )
{
  INSTANCE_CAST
  return pInstance->LoadParametersLocal( inFile );
}

DLLEXPORT int
STDCALL BCI2000Remote_LoadParametersRemote( void* inObject, const char* inFile )
{
  INSTANCE_CAST
  return pInstance->LoadParametersRemote( inFile );
}


DLLEXPORT int
STDCALL BCI2000Remote_SetConfig( void* inObject )
{
  INSTANCE_CAST
  return pInstance->SetConfig();
}

DLLEXPORT int
STDCALL BCI2000Remote_Start( void* inObject )
{
  INSTANCE_CAST
  return pInstance->Start();
}

DLLEXPORT int
STDCALL BCI2000Remote_Stop( void* inObject )
{
  INSTANCE_CAST
  return pInstance->Stop();
}

DLLEXPORT int
STDCALL BCI2000Remote_Quit( void* inObject )
{
  INSTANCE_CAST
  return pInstance->Quit();
}


DLLEXPORT const char* 
STDCALL BCI2000Remote_GetSystemState( void* inObject )
{
  INSTANCE_CAST
  string state;
  if( pInstance->GetSystemState( state ) )
    return AllocateString( state.c_str() );
  return NULL;
}

DLLEXPORT int
STDCALL BCI2000Remote_GetControlSignal( void* inObject, int inChannel, int inElement, double* pOutValue )
{
  INSTANCE_CAST
  return pInstance->GetControlSignal( inChannel, inElement, *pOutValue );
}

DLLEXPORT int
STDCALL BCI2000Remote_SetParameter( void* inObject, const char* inName, const char* inValue )
{
  INSTANCE_CAST
  return pInstance->SetParameter( inName, inValue );
}

DLLEXPORT const char*
STDCALL BCI2000Remote_GetParameter( void* inObject, const char* inName )
{
  INSTANCE_CAST
  string value;
  if( pInstance->GetParameter( inName, value ) )
    return AllocateString( value.c_str() );
  return NULL;
}

DLLEXPORT int
STDCALL BCI2000Remote_AddStateVariable( void* inObject, const char* inName, unsigned int inBitWidth, double inInitialValue )
{
  INSTANCE_CAST
  return pInstance->AddStateVariable( inName, inBitWidth, inInitialValue );
}

DLLEXPORT int
STDCALL BCI2000Remote_SetStateVariable( void* inObject, const char* inName, double inValue )
{
  INSTANCE_CAST
  return pInstance->SetStateVariable( inName, inValue );

}

DLLEXPORT int
STDCALL BCI2000Remote_GetStateVariable( void* inObject, const char* inName, double* pOutValue )
{
  INSTANCE_CAST
  return pInstance->GetStateVariable( inName, *pOutValue );
}


DLLEXPORT int
STDCALL BCI2000Remote_SetScript( void* inObject, const char* inEvent, const char* inScript )
{
  INSTANCE_CAST
  return pInstance->SetScript( inEvent, inScript );
}

DLLEXPORT const char*
STDCALL BCI2000Remote_GetScript( void* inObject, const char* inEvent )
{
  INSTANCE_CAST
  string script;
  if( pInstance->GetScript( inEvent, script ) )
    return AllocateString( script.c_str() );
  return NULL;
}

