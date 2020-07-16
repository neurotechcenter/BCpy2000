////////////////////////////////////////////////////////////////////////////////
// $Id: Detrend.h 4219 2012-07-02 17:10:13Z mellinger $
// Authors: mcfarlan@wadsworth.org, juergen.mellinger@uni-tuebingen.de
// Description: Linear detrending functions.
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
#ifndef DETREND_H
#define DETREND_H

#include <valarray>
#include <algorithm>
#include <numeric>

namespace Detrend
{
  template<typename T>
   void MeanDetrend( const std::valarray<T>&, std::valarray<T>& );
  template<typename T>
   void LinearDetrend( const std::valarray<T>&, std::valarray<T>& );
}

// Implementation
template<typename T>
void
Detrend::MeanDetrend( const std::valarray<T>& inData, std::valarray<T>& outResult )
{
  if( outResult.size() != inData.size() )
    outResult.resize( inData.size(), 0.0 );
  if( inData.size() > 0 )
    outResult = inData - inData.sum() / inData.size();
}

template<typename T>
void
Detrend::LinearDetrend( const std::valarray<T>& inData, std::valarray<T>& outResult )
{
  int n = static_cast<int>( inData.size() );
  if( outResult.size() != inData.size() )
    outResult.resize( n );
  outResult = inData;
  if( n > 0 )  
  {
    std::valarray<T> linbuf( n );
    for( int i = 0; i < n; ++i )
      linbuf[i] = i;

    T x2 = ( ( 2 * n - 1 ) * ( n - 1 ) * n ) / 6,
      xy = std::inner_product( &linbuf[0], &linbuf[n], &outResult[0], 0.0 ),
       x = n * ( n - 1 ) / 2,
       y = inData.sum(),
       b = ( xy - x * y / n ) / ( x2 - ( x * x  / n ) ),
       a = ( y - b * x ) / n;
    outResult -= b * linbuf + a;
  }
}

#endif // DETREND_H
