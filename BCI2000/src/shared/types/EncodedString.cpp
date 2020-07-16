//////////////////////////////////////////////////////////////////////
// $Id: EncodedString.cpp 4731 2014-07-03 15:18:03Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A string class that allows for transparent handling of
//   character codes using the '%' character.
//
//   InsertInto() will always output white space in encoded
//   hexadecimal form.
//   Additional characters may be listed in the "encodeThese"
//   parameter.
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
#include "PCHIncludes.h"
#pragma hdrstop

#include "EncodedString.h"
#include <sstream>

using namespace std;

static const char cEscapeChar = '%';

// **************************************************************************
// Function:   ExtractFrom
// Purpose:    Member function for formatted stream input of a single
//             string value.
//             All formatted input functions are, for consistency's sake,
//             supposed to use this function.
// Parameters: Input stream to read from.
// Returns:    Input stream read from.
// **************************************************************************
istream&
EncodedString::ExtractFrom( istream& is )
{
  string newContent;
  if( is >> newContent )
  {
    size_t pos = newContent.find( cEscapeChar, 0 );
    while( pos != npos )
    {
      newContent.erase( pos, 1 );

      size_t numDigits = 0;
      char curDigit;
      int hexValue = 0;
      while( pos + numDigits < newContent.size() && numDigits < 2
             && ::isxdigit( curDigit = newContent[ pos + numDigits ] ) )
      {
        if( !::isdigit( curDigit ) )
          curDigit = ::toupper( curDigit ) - 'A' + 10;
        else
          curDigit -= '0';
        hexValue = ( hexValue << 4 ) + curDigit;
        ++numDigits;
      }
      newContent.erase( pos, numDigits );
      if( hexValue > 0 )
        newContent.insert( pos, 1, static_cast<char>( hexValue ) );

      pos = newContent.find( cEscapeChar, pos + 1 );
    }
    *this = newContent;
  }
  return is;
}

// **************************************************************************
// Function:   InsertInto
// Purpose:    Member function for formatted stream output of a single
//             encoded string value.
//             All formatted output functions are, for consistency's sake,
//             supposed to use this function.
// Parameters: Output stream to write into; list of characters that may not
//             appear in the output.
// Returns:    Stream written into.
// **************************************************************************
ostream&
EncodedString::InsertInto( ostream& os, const string& forbiddenChars ) const
{
  if( empty() )
    os << cEscapeChar;
  else
  {
    const string& self = *this;
    ostringstream oss;
    oss << hex;
    for( size_t pos = 0; pos < size(); ++pos )
    {
      if( self[ pos ] >= 0
          && ::isprint( self[ pos ] )
          && !::isspace( self[ pos ] )
          && forbiddenChars.find( self[ pos ] ) == npos )
      {
        oss << self[ pos ];
        if( self[ pos ] == cEscapeChar )
          oss << cEscapeChar;
      }
      else
        oss << cEscapeChar
            << ( int )( ( self[ pos ] >> 4 ) & 0x0f )
            << ( int )( self[ pos ] & 0x0f );
    }
    os << oss.str();
  }
  return os;
}



