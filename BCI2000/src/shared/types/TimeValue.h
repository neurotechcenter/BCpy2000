////////////////////////////////////////////////////////////////////////////////
// $Id: TimeValue.h 4731 2014-07-03 15:18:03Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A class that wraps an integer value such that it reads/writes
//   itself in hexagesimal units.
//   For reading, the following formats are transparently supported:
//      hh:mm:ss (this is also used for writing)
//      mm:ss
//      plain seconds.
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
#ifndef TIME_VALUE_H
#define TIME_VALUE_H

#include <iostream>

class TimeValue
{
 public:
  TimeValue( long t = 0 ) : mValue( t ) {}
  operator long&()
    { return mValue; }
  std::ostream& InsertInto( std::ostream& os ) const
    {
      int hours = mValue / 3600,
          mins = ( mValue - 3600 * hours ) / 60,
          secs = mValue % 60;
      return os << std::setw( 2 ) << std::setfill( '0' ) << hours
                << ':' << std::setw( 2 ) << mins
                << ':' << std::setw( 2 ) << secs;
    }
  std::istream& ExtractFrom( std::istream& is )
    {
      int val1 = 0,
          val2 = 0,
          val3 = 0;
      if( is >> val1 )
      {
        if( is.peek() == ':' )
        {
          if( is.ignore() >> val2 )
          {
            if( is.peek() == ':' )
            {
              if( is.ignore() >> val3 ) // format is hh:mm:ss
                mValue = 3600 * val1 + 60 * val2 + val3;
            }
            else // format is mm:ss
              mValue = 60 * val1 + val2;
          }
        }
        else // format is plain seconds
          mValue = val1;
      }
      return is;
    }
 private:
  long mValue;
};

inline
std::ostream& operator<<( std::ostream& os, const TimeValue& t )
{ return t.InsertInto( os ); }

inline
std::istream& operator>>( std::istream& is, TimeValue& t )
{ return t.ExtractFrom( is ); }

#endif // TIME_VALUE_H

