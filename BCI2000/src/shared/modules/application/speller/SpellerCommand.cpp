////////////////////////////////////////////////////////////////////////////////
// $Id: SpellerCommand.cpp 4731 2014-07-03 15:18:03Z mellinger $
// Authors: juergen.mellinger@uni-tuebingen.de
// Description: A class that represents a speller command (given in <>) which
//   can read and write itself from/to a stream.
//   For plain strings without command codes, the Code property is empty, and
//   the Value property contains the string.
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

#include "SpellerCommand.h"
#include <sstream>
#include <cstdio>

using namespace std;

SpellerCommand::SpellerCommand( const std::string& inString )
{
  istringstream iss( inString );
  if( !( iss >> *this ) )
  {
    mCode = "";
    mValue = "";
  }
}

// Formatted IO
std::istream&
SpellerCommand::ExtractFrom( std::istream& is )
{
  mValue = "";
  mCode = "";
  while( is.peek() != EOF && is.peek() != cOpenChar )
    mValue += is.get();
  if( is && mValue.empty() )
  {
    is.ignore();
    getline( is, mCode, cCloseChar );
    string::iterator i = mCode.begin();
    while( i != mCode.end() && ::isalpha( *i ) )
    {
      *i = ::toupper( *i );
      ++i;
    }
    mValue = string( i, mCode.end() );
    mCode = string( mCode.begin(), i );
  }
  return is;
}

std::ostream&
SpellerCommand::InsertInto( std::ostream& os ) const
{
  if( mCode.empty() )
    os << mValue;
  else
   os << cOpenChar << mCode << ' ' << mValue << cCloseChar;
  return os;
}

