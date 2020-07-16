////////////////////////////////////////////////////////////////////////////////
// $Id: Status.cpp 4731 2014-07-03 15:18:03Z mellinger $
// Authors: schalk@wadsworth.org, juergen.mellinger@uni-tuebingen.de
// Description: BCI2000 type for module status messages.
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

#include "Status.h"

#include <string>
#include <sstream>

using namespace std;

const Status Status::Fail( "Error", fail );

Status::Status()
: mCode( 0 )
{
}

Status::Status( const string& inMessage, int inCode )
: mMessage( inMessage ),
  mCode( inCode )
{
}


Status::ContentType
Status::Content() const
{
  ContentType result = unknown;
  if( Code() == debugMessage )
    result = debug;
  else if( ( Code() >= firstWarningCode ) && ( Code() <= lastWarningCode ) )
    result = warning;
  else if( ( Code() >= firstErrorCode ) && ( Code() <= lastErrorCode ) )
    result = error;
  else switch( Code() )
  {
    case sourceInitialized:
    case sigprocInitialized:
    case appInitialized:
      result = initialized;
      break;
    case sourceRunning:
    case sigprocRunning:
    case appRunning:
      result = running;
      break;
    case sourceSuspended:
    case sigprocSuspended:
    case appSuspended:
      result = suspended;
      break;
  }
  return result;
}

ostream&
Status::InsertInto( ostream& os ) const
{
  ostringstream oss;
  oss << mCode << ": " << mMessage;
  for( size_t p = oss.str().find( '}' ); p != string::npos; p = oss.str().find( '}', p ) )
    oss.str().replace( p, 1, "\\}" );
  return os << oss.str();
}

istream&
Status::Unserialize( istream& is )
{
  is >> mCode;
  is.get();
  is >> ws;
  getline( is, mMessage, '\0' );
  return is;
}

ostream&
Status::Serialize( ostream& os ) const
{
  os << mCode << ": " << mMessage;
  os.put( 0 );
  return os;
}


