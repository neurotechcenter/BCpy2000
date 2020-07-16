////////////////////////////////////////////////////////////////////////////////
// $Id: Brackets.h 3798 2012-02-01 18:07:06Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: This helper class defines what we accept as delimiting
//   single-character symbol pairs for index lists and sub-parameters in a
//   parameter line.
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
#ifndef BRACKETS_H
#define BRACKETS_H

#include <string>

class Brackets
{
 public:
  static bool IsOpening( char c )
  {
    return ClosingMatch( c ) != '\0';
  }
  static char ClosingMatch( char c )
  {
    size_t pos = BracketPairs().find( c );
    if( pos != std::string::npos && !( pos & 1 ) ) // opening brackets are at even positions
      return BracketPairs()[ pos + 1 ];
    return '\0';
  }
  static const std::string& BracketPairs();
  // A bracket pair that is used for writing.
  static const char OpeningDefault = '{';
  static const char ClosingDefault = '}';
};

#endif // BRACKETS_H

