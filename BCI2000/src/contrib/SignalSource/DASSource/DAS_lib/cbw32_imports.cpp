////////////////////////////////////////////////////////////////////////////////
//
// File: cbw32_imports.cpp
//
// Author: juergen.mellinger@uni-tuebingen.de
//
// Date: Sep 18, 2003
//
// Description: A dynamic interface to the ComputerBoards/Measurement Computing
//              Universal Library cbw32.dll.
//              By loading the functions "by hand" we avoid distribution of a
//              certain version of that
//              DLL along with the source code; the resulting
//              executable should be more stable with regard to version
//              mismatches between cbw32.dll and the cbul*.sys drivers.
//
//              By default, it will use the cbw32.dll version that has been
//              installed with the A/D driver/InstaCal program, provided
//              there is no cbw32.dll in the DLL search path (i.e. in the
//              executable directory, or in the system DLL directories).
//
//              The difference between dynamically loading the DLL and
//              using an import library created by IMPLIB
//              is that the *.lib is likely to only work with the version of
//              the DLL it was created from.
//
// Notes:       If you use a function declared in cbw.h that is not defined
//              here, linking will fail with an "unresolved external" error.
//              In this case, you may conveniently add a function body at the
//              end of the file using the DLL_WRAPPER_<N>() macro where
//              N stands for the number of arguments in the function call.
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
#pragma hdrstop

#include "cbw.h"
#include <windows.h>

HINSTANCE GetDLL()
{
  static const char dllName[] = "cbw32.dll";
  static HINSTANCE dllInstance = ::LoadLibrary( dllName );

  // The InstaCal installation program will not install the dll into the system
  // but into the InstaCal path, and add that path to the PATH variable.
  // If the user modifies the PATH variable, loading fails with the call above.
  // We try once more, using a registry key that points to the UL installation
  // directory.
  if( dllInstance == NULL )
  {
    HKEY ulKey;
    if( ::RegOpenKeyEx( HKEY_LOCAL_MACHINE, "SOFTWARE\\Universal Library", 0,
                                      KEY_QUERY_VALUE, &ulKey ) == ERROR_SUCCESS )
    {
      unsigned long valueSize = 0;
      const char valueName[] = "RootDir";
      if( ::RegQueryValueEx( ulKey, valueName, NULL, NULL,
                                             NULL, &valueSize ) == ERROR_SUCCESS )
      {
        int dllPathLen = static_cast<int>( valueSize + ::strlen( dllName ) );
        char* dllPath = new char[ dllPathLen + 1 ];
        if( ::RegQueryValueEx( ulKey, valueName, NULL, NULL,
              reinterpret_cast<LPBYTE>( dllPath ), &valueSize ) == ERROR_SUCCESS )
        {
          if( dllPath[ valueSize - 2 ] != '\\' )
            ::strncat( dllPath, "\\", dllPathLen );
          ::strncat( dllPath, dllName, dllPathLen );
          dllInstance = ::LoadLibrary( dllPath );
        }
        delete[] dllPath;
      }
    }
  }
  return dllInstance;
}

#define PTR_TYPE( name, type0 )                                                 \
typedef type0 ( __stdcall *name##Ptr )

#define RESOLVE_AND_CALL( name )                                                \
  static name##Ptr _funcPtr                                                     \
    = ::GetDLL() ? ( name##Ptr )::GetProcAddress( ::GetDLL(), #name ) : NULL;   \
  return _funcPtr ? _funcPtr /* here goes the argument list before the following macro */
#define OR_DIE( type ) :( type )NOTUSED;

#define DLL_WRAPPER_1( name, type0, type1 )                                     \
PTR_TYPE( name, type0 )( type1 );                                               \
type0 __stdcall name( type1 _arg1 )                                             \
{                                                                               \
  RESOLVE_AND_CALL( name )( _arg1 )                                             \
  OR_DIE( type0 );                                                              \
}

#define DLL_WRAPPER_2( name, type0, type1, type2 )                              \
PTR_TYPE( name, type0 )( type1, type2 );                                        \
type0 __stdcall name( type1 _arg1, type2 _arg2 )                                \
{                                                                               \
  RESOLVE_AND_CALL( name )( _arg1, _arg2 )                                      \
  OR_DIE( type0 );                                                              \
}

#define DLL_WRAPPER_4( name, type0, type1, type2, type3, type4 )                \
PTR_TYPE( name, type0 )( type1, type2, type3, type4 );                          \
type0 __stdcall name( type1 _arg1, type2 _arg2, type3 _arg3, type4 _arg4 )      \
{                                                                               \
  RESOLVE_AND_CALL( name )( _arg1, _arg2, _arg3, _arg4 )                        \
  OR_DIE( type0 );                                                              \
}

#define DLL_WRAPPER_5( name, type0, type1, type2, type3, type4, type5 )                 \
PTR_TYPE( name, type0 )( type1, type2, type3, type4, type5 );                           \
type0 __stdcall name( type1 _arg1, type2 _arg2, type3 _arg3, type4 _arg4, type5 _arg5 ) \
{                                                                                       \
  RESOLVE_AND_CALL( name )( _arg1, _arg2, _arg3, _arg4, _arg5 )                         \
  OR_DIE( type0 );                                                                      \
}

#define DLL_WRAPPER_8( name, type0, type1, type2, type3, type4, type5, type6, type7, type8 ) \
PTR_TYPE( name, type0 )( type1, type2, type3, type4, type5, type6, type7, type8 );           \
type0 __stdcall name( type1 _arg1, type2 _arg2, type3 _arg3, type4 _arg4, type5 _arg5,       \
                                                     type6 _arg6, type7 _arg7, type8 _arg8 ) \
{                                                                                            \
  RESOLVE_AND_CALL( name )( _arg1, _arg2, _arg3, _arg4, _arg5, _arg6, _arg7, _arg8 )         \
  OR_DIE( type0 );                                                                           \
}

DLL_WRAPPER_1( cbWinBufAlloc, HGLOBAL, long )
DLL_WRAPPER_1( cbWinBufFree, int, HGLOBAL )

DLL_WRAPPER_1( cbDeclareRevision, int, float* )
DLL_WRAPPER_2( cbGetRevision, int, float*, float* )
DLL_WRAPPER_2( cbGetBoardName, int, int, char* )
DLL_WRAPPER_2( cbErrHandling, int, int, int )
DLL_WRAPPER_2( cbGetErrMsg, int, int, char* )

DLL_WRAPPER_5( cbGetConfig, int, int, int, int, int, int* )
DLL_WRAPPER_5( cbSetConfig, int, int, int, int, int, int )

DLL_WRAPPER_4( cbAIn, int, int, int, int, unsigned short* )
DLL_WRAPPER_8( cbAInScan, int, int, int, int, long, long*, int, HGLOBAL, int )
DLL_WRAPPER_2( cbStopBackground, int, int, int )
DLL_WRAPPER_5( cbGetStatus, int, int, short*, long*, long*, int )

DLL_WRAPPER_5( cbEnableEvent, int, int, unsigned, unsigned, EVENTCALLBACK, void* )
DLL_WRAPPER_2( cbDisableEvent, int, int, unsigned )


