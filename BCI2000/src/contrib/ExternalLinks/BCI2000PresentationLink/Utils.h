////////////////////////////////////////////////////////////////////////////////
// $Id: Utils.h 4970 2015-08-21 16:46:07Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: Utilities for defining PCL functions, and for accessing
//   arguments from inside PCL functions.
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
#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <sstream>
#include "ComPtr.h"
#include "ComStrings.h"
#include "BCIException.h"
#include "PCLExtension.h"

#define OLESTR_(x) OLESTR(x)

#define COM_METHOD_START try {
#define COM_METHOD_END \
} catch( const std::exception& e ) { \
  mLastError = com::DualString( e.what() ); return E_FAIL;\
} catch( ... ) { \
  mLastError = L"Unknown exception in "; \
  mLastError += com::DualString( __FUNCTION__ ); \
  return E_FAIL;\
} return S_OK;

template<class T>
struct Method
{
  void (T::*function)( struct ArgList& );
  const wchar_t* name,
               * description;
  struct Argument
  {
    const wchar_t* name,
                 * type;
    int rank;
    bool isRef;
    const wchar_t* description;
  } arguments[3];
  struct Result
  {
    const wchar_t* type;
    int rank;
    const wchar_t* description;
  } result;
};

struct ArgList : std::vector< com::Ptr<IPCLArgument> >
{
  template<class T>
    ArgList( const struct Method<T>&, IPCLArguments* );

  com::DualString GetString( int );
  void SetString( int, const com::DualString& );
  std::vector<com::DualString> GetStringArray( int );
  long GetInt( int );
  void SetInt( int, const long& );
  double GetDouble( int );
  void SetDouble( int, const double& );
};

template<class T>
ArgList::ArgList( const struct Method<T>& inMethod, IPCLArguments* pArgs )
{
  com::Ptr<IPCLArgument> pArg;
  pArgs->getReturnValue( pArg.Assignee() );
  push_back( pArg );
#ifdef BCIDEBUG
  std::string methodName = com::DualString( inMethod.name );
  if( inMethod.result.type )
  {
    com::BString type;
    pArg->getType( &type );
    bool match = ( type == inMethod.result.type );
    long rank;
    pArg->getRank( &rank );
    match = match && rank == inMethod.result.rank;
    if( !match )
      throw bciexception << "Return type does not match when calling " << methodName;
  }
#endif // BCIDEBUG

  long numArgs = -1;
  pArgs->getCount( &numArgs );
#ifdef BCIDEBUG
  int maxArgs = sizeof( inMethod.arguments ) / sizeof( *inMethod.arguments ),
      methodArgs = 0;
  while( methodArgs < maxArgs && inMethod.arguments[methodArgs].name != NULL )
    ++methodArgs;
  if( numArgs != methodArgs )
    throw bciexception << "Number of arguments does not match when calling " << methodName;
#endif // BCIDEBUG

  for( long i = 0; i < numArgs; ++i )
  {
    pArgs->getArgument( i, pArg.Assignee() );
    push_back( pArg );
#ifdef BCIDEBUG
    com::BString type;
    pArg->getType( &type );
    bool match = ( type == inMethod.arguments[i].type );
    long rank;
    pArg->getRank( &rank );
    match = match && rank == inMethod.arguments[i].rank;
    if( !match )
      throw bciexception
        << "Type of argument #" << i + 1 << " is \""
        << std::string( com::DualString( type ) )
        << ( rank > 0 ? " array" : "" )
        << "\", does not match when calling " << methodName;
#endif // BCIDEBUG
  }
}

#endif // UTILS_H
