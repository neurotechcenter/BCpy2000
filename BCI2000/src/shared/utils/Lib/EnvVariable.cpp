////////////////////////////////////////////////////////////////////////////////
// $Id: EnvVariable.cpp 5513 2016-08-23 20:05:52Z mellinger $
// Authors: juergen.mellinger@uni-tuebingen.de
// Description: A RAII class that makes temporary changes to environment
//   variables. Also encapsulates OS specific access to environment variables.
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
#include "EnvVariable.h"

#if _WIN32
# include <Windows.h>
#else // _WIN32
# include <cstdlib>
#endif // _WIN32

using namespace std;
using namespace Tiny;

// Class methods
bool
EnvVariable::Set( const string& inName, const string& inValue )
{
  bool success = false;
#if _WIN32
  success = ::SetEnvironmentVariableA( inName.c_str(), inValue.c_str() );
#else // _WIN32
  success = ( 0 == ::setenv( inName.c_str(), inValue.c_str(), 1 ) );
#endif // _WIN32
  return success;
}

bool
EnvVariable::Get( const string& inName, string& outValue )
{
#if _WIN32
  bool success = false;
  int length = ::GetEnvironmentVariableA( inName.c_str(), NULL, 0 );
  if( length == 0 && ::GetLastError() != ERROR_ENVVAR_NOT_FOUND )
  {
  success = true;
    outValue = "";
  }
  else if( length > 0 )
  {
  success = true;
    char* pBuffer = new char[length];
    ::GetEnvironmentVariable( inName.c_str(), pBuffer, length );
    outValue = pBuffer;
    delete[] pBuffer;
  }
  return success;
#else // _WIN32
  const char* pValue = ::getenv( inName.c_str() );
  if( pValue )
    outValue = pValue;
  return pValue;
#endif // _WIN32
}

bool
EnvVariable::Clear( const string& inName )
{
  bool success = false;
#if _WIN32
  success = ::SetEnvironmentVariableA( inName.c_str(), NULL );
#else // _WIN32
  success = ( 0 == ::unsetenv( inName.c_str() ) );
#endif // _WIN32
  return success;
}

// Instance methods
EnvVariable::EnvVariable( const string& inName )
: mName( inName ),
  mExisted( false )
{
  mExisted = Get( mName, mInitialValue );
}

EnvVariable::~EnvVariable()
{
  if( mExisted )
    Set( mName, mInitialValue );
  else
    Clear( mName );
}

bool
EnvVariable::Exists() const
{
  std::string ignored;
  return Get( ignored );
}

bool
EnvVariable::Set( const string& inValue )
{
  return Set( mName, inValue );
}

bool
EnvVariable::Get( string& outValue ) const
{
  return Get( mName, outValue );
}

std::string
EnvVariable::Get() const
{
  std::string value;
  Get( mName, value );
  return value;
}

