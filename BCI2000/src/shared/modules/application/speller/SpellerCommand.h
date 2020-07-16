////////////////////////////////////////////////////////////////////////////////
// $Id: SpellerCommand.h 4731 2014-07-03 15:18:03Z mellinger $
// Authors: juergen.mellinger@uni-tuebingen.de
// Description: A class that represents a speller command (given in <>) which
//   can read and write itself from/to a stream.
//   For plain strings without command codes, the Code property is empty, and
//   the Value property contains the string.
//   Grammar is
//    plain_string | < code value >
//   with optional white space within brackets.
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
#ifndef SPELLER_COMMAND_H
#define SPELLER_COMMAND_H

#include <iostream>
#include <string>

class SpellerCommand
{
 public:
  static const char cOpenChar = '<',
                    cCloseChar = '>';

  SpellerCommand()
    {}
  SpellerCommand( const std::string& );
  virtual ~SpellerCommand()
    {}
  // Properties
  SpellerCommand& SetCode( std::string& s )
    { mCode = s; return *this; }
  const std::string& Code() const
    { return mCode; }
  SpellerCommand& SetValue( std::string& s )
    { mValue = s; return *this; }
  const std::string& Value() const
    { return mValue; }
  // Formatted IO
  std::istream& ExtractFrom( std::istream& );
  std::ostream& InsertInto( std::ostream& ) const;

 private:
  std::string mCode,
              mValue;
};

inline
std::istream& operator>>( std::istream& is, SpellerCommand& s )
{ return s.ExtractFrom( is ); }
inline
std::ostream& operator<<( std::ostream& os, const SpellerCommand& s )
{ return s.InsertInto( os ); }

#endif // SPELLER_COMMAND_H
