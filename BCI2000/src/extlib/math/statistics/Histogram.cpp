////////////////////////////////////////////////////////////////////////////////
// $Id: Histogram.cpp 4970 2015-08-21 16:46:07Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: See the header file for a description.
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

#include "Histogram.h"
#include "BCIException.h"

using namespace std;
using namespace StatisticalObserver;

class Histogram&
Histogram::operator*=( Number inFactor )
{
  for( iterator i = begin(); i != end(); ++i )
    i->second *= inFactor;
  return *this;
}

class Histogram&
Histogram::Add( Number inValue, Number inWeight )
{
  iterator i = begin();
  while( i != end() && i->first < inValue )
    ++i;
  if( i == end() )
    push_back( make_pair( inValue, inWeight ) );
  else if( i->first == inValue )
    i->second += inWeight;
  else
    insert( i, make_pair( inValue, inWeight ) );
  return *this;
}

class Histogram&
Histogram::Prune( Number inWeightThreshold, Number inDistThreshold )
{
  if( inWeightThreshold > 0 && size() < 1 / inWeightThreshold )
    return *this;

  iterator i = begin(),
           j = i;
  while( i != end() && ++j != end() )
  {
    if( i->second < eps )
    {
      erase( i++ );
    }
    else if( j->second < eps )
    {
      ++i;
    }
    else
    {
      Number weight;
      while( j != end() && ( ( weight = i->second + j->second ) < inWeightThreshold || j->first - i->first < inDistThreshold ) )
      {
        Number value = ( i->first * i->second + j->first * j->second ) / weight;
        i->first = value;
        i->second = weight;
        j = erase( j );
      }
      ++i;
    }
    j = i;
  }
  return *this;
}

class Histogram&
Histogram::Clear()
{
  clear();
  return *this;
}

Number
Histogram::PowerSum( unsigned int inPower ) const
{
  Number result = 0;
  for( const_iterator i = begin(); i != end(); ++i )
  {
    Number value = 1;
    for( size_t j = 0; j < inPower; ++j )
      value *= i->first;
    result += value * i->second;
  }
  return result;
}

Number
Histogram::CDF( Number inValue ) const
{
  Number sum = 0;
  const_iterator i = begin();
  while( i != end() && i->first < inValue  )
    sum += ( i++ )->second;
  return sum;
}

Number
Histogram::InverseCDF( Number inCumulatedWeight ) const
{
  if( empty() )
    throw std_runtime_error << "Trying to compute inverse cumulated weight without observation";

  Number sum = 0;
  const_iterator i = begin();
  while( sum < inCumulatedWeight && i != end() )
    sum += ( i++ )->second;

  if( i == begin() )
    return i->first;
  return ( --i )->first;
}

