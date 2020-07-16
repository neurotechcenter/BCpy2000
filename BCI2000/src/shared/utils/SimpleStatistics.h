////////////////////////////////////////////////////////////////////////////////
// $Id$
// Author: mellinger@neurotechcenter.org
// Description: A simple power sum observer for continuously updated
//   statistics on scalar values.
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
#ifndef SIMPLE_STATISTICS_H
#define SIMPLE_STATISTICS_H

#include <Numeric.h>
template<class T> class SimpleStatistics
{
public:
  SimpleStatistics( T initialMean = 0, T initialError = 0, T window = Inf<T>() )
  {
    Reset( initialMean, initialError ).SetWindowLength( window );
  }
  SimpleStatistics& SetWindowLength( T length )
  {
    mWindowLength = length;
    mDecay = ::exp( -1.0 / length );
    return *this;
  }
  T WindowLength() const
  {
    return mWindowLength;
  }
  SimpleStatistics& Reset( T initialMean, T initialError = 0 )
  {
    mPow[0] = 1;
    mPow[1] = initialMean;
    mPow[2] = initialMean * initialMean * ( 1 + initialError * initialError );
    return *this;
  }
  SimpleStatistics& Reset()
  {
    mPow[0] = 0;
    mPow[1] = 0;
    mPow[2] = 0;
    return *this;
  }
  void Observe( T data )
  {
    T d = 1.0;
    for( auto& val : mPow )
    {
      val *= mDecay;
      val += d;
      d *= data;
    }
  }
  T Count() const
  {
    return mPow[0];
  }
  T Mean() const
  {
    return mPow[1] / mPow[0];
  }
  T Variance() const
  {
    T val = std::max<T>( 0, mPow[2] - ( mPow[1] * mPow[1] ) / mPow[0] );
    return val / mPow[0];
  }
private:
  T mWindowLength, mDecay, mPow[3];
};

#endif // SIMPLE_STATISTICS_H

