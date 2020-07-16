/////////////////////////////////////////////////////////////////////////////
// $Id: PrecisionTime.cpp 4731 2014-07-03 15:18:03Z mellinger $
// Authors: schalk@wadsworth.org, juergen.mellinger@uni-tuebingen.de
// Description: A class wrapper for the system's high precision timer.
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
/////////////////////////////////////////////////////////////////////////////
#include "PrecisionTime.h"

#include "TimeUtils.h"

PrecisionTime
PrecisionTime::Now()
{
  return static_cast<NumType>( TimeUtils::MonotonicTime().ToMilliseconds() );
}

// **************************************************************************
// Function:   UnsignedDiff
// Purpose:    calculates the difference between two times (i.e., time1-time2)
//             takes roll-over into account (in case time1 < time2)
// Parameters: time1, time2 - two 16 bit integers
// Returns:    time1-time2, if time1-time2 >= 0
//             or time1-time2+65536, if time1-time2 < 0
// **************************************************************************
PrecisionTime::NumType
PrecisionTime::UnsignedDiff( NumType time1, NumType time2 )
{
  const int maxdiffPlusOne = 1 << ( 8 * sizeof( time1 ) );
  return ( ( time1 + maxdiffPlusOne ) - time2 ) % maxdiffPlusOne;
}

// **************************************************************************
// Function:   SignedDiff
// Purpose:    calculates the signed difference between two times,
//             taking roll-over into account.
// Parameters: time1, time2 - two 16 bit integers
// Returns:    signed difference
// **************************************************************************
int
PrecisionTime::SignedDiff( NumType time1, NumType time2 )
{
  const int wraparound = 1 << ( 8 * sizeof( time1 ) );
  int diff = time1;
  diff -= time2;
  if( diff >= wraparound / 2 )
    diff -= wraparound;
  else if( diff < -wraparound / 2 )
    diff += wraparound;
  return diff;
}

