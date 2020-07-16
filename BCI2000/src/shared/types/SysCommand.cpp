////////////////////////////////////////////////////////////////////////////////
// $Id: SysCommand.cpp 4731 2014-07-03 15:18:03Z mellinger $
// Authors: schalk@wadsworth.org, juergen.mellinger@uni-tuebingen.de
// Description: BCI2000 type for system commands.
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

#include "SysCommand.h"

#include <string>
#include <iostream>

using namespace std;

const SysCommand SysCommand::EndOfState( "EndOfState" );
const SysCommand SysCommand::EndOfParameter( "EndOfParameter" );
const SysCommand SysCommand::Start( "Start" );
const SysCommand SysCommand::Reset( "Reset" );
const SysCommand SysCommand::Suspend( "Suspend" );


ostream&
SysCommand::InsertInto( ostream& os ) const
{
  string::const_iterator p = mBuffer.begin();
  while( p != mBuffer.end() )
  {
    if( *p == '}' )
      os.put( '\\' );
    os.put( *p++ );
  }
  return os;
}

istream&
SysCommand::Unserialize( istream& is )
{
  return std::getline( is, mBuffer, '\0' );
}

ostream&
SysCommand::Serialize( ostream& os ) const
{
  os.write( mBuffer.data(), mBuffer.size() );
  os.put( '\0' );
  return os;
}

bool
SysCommand::operator<( const SysCommand& s ) const
{
  return mBuffer < s.mBuffer;
}

bool
SysCommand::operator==( const SysCommand& s ) const
{
  return mBuffer == s.mBuffer;
}
