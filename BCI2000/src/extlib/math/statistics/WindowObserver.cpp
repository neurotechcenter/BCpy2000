////////////////////////////////////////////////////////////////////////////////
// $Id: WindowObserver.cpp 3798 2012-02-01 18:07:06Z mellinger $
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

#include "WindowObserver.h"
#include "BCIError.h"
#include <vector>
#include <algorithm>
#include <limits>

using namespace std;
using namespace StatisticalObserver;

WindowObserver::WindowObserver( int inConfig )
: ObserverBase( inConfig, Supports ),
  mAgeLimit( 0 )
{
  static bool warned = false;
  if( !warned )
    bciout << "You are using the WindowObserver class. "
              "This class is intended for testing purposes only, and is computationally very inefficient. "
              "Consider to replace WindowObserver with CombinedObserver."
           << endl;
}

void
WindowObserver::DoChange()
{
  if( WindowLength() == Unlimited )
    mAgeLimit = numeric_limits<size_t>::max();
  else
    mAgeLimit = static_cast<size_t>( WindowLength() );
  RemoveOldSamples();

  if( !empty() && begin()->first.size() != SampleSize() )
    DoClear();
}

void
WindowObserver::DoAgeBy( unsigned int inCount )
{
  for( iterator i = begin(); i != end(); ++i )
    ++i->second.second;
  RemoveOldSamples();
}

void
WindowObserver::DoObserve( const Vector& inV, Number inWeight )
{
  push_front( make_pair( inV, make_pair( inWeight, 0 ) ) );
  RemoveOldSamples();
}

void
WindowObserver::DoClear()
{
  clear();
}

// Computational functions
Number
WindowObserver::PowerSum0( MemPool& ) const
{
  Number result = 0;
  for( const_iterator i = begin(); i != end(); ++i )
    result += i->second.first;
  return result;
}

VectorPtr
WindowObserver::PowerSum1( MemPool& ioPool ) const
{
  VectorPtr result = ioPool.NewVector( SampleSize() );
  *result = 0;
  for( const_iterator i = begin(); i != end(); ++i )
    *result += i->first * i->second.first;
  return result;
}

VectorPtr
WindowObserver::PowerSum2Diag( MemPool& ioPool ) const
{
  VectorPtr result = ioPool.NewVector( SampleSize() );
  *result = 0;
  for( const_iterator i = begin(); i != end(); ++i )
    *result += i->first * i->first * i->second.first;
  return result;
}

MatrixPtr
WindowObserver::PowerSum2Full( MemPool& ioPool ) const
{
  MatrixPtr result = ioPool.NewMatrix( SampleSize(), SampleSize() );
  *result = 0;
  for( const_iterator i = begin(); i != end(); ++i )
    for( int j = 0; j < SampleSize(); ++j )
      for( int k = 0; k < SampleSize(); ++k )
        ( *result )[j][k] += i->first[j] * i->first[k] * i->second.first;
  return result;
}

VectorPtr
WindowObserver::PowerSumDiag( unsigned int inP, MemPool& ioPool ) const
{
  VectorPtr result = ioPool.NewVector( SampleSize() );
  if( inP == 0 )
  {
    *result = PowerSum0( ioPool );
    return result;
  }

  *result = 0;
  for( const_iterator i = begin(); i != end(); ++i )
  {
    Vector v = i->first;
    for( unsigned int j = 1; j < inP; ++j )
      v *= i->first;
    *result += v * i->second.first;
  }
  return result;
}

class Index : public vector<WindowObserverDataContainer::const_iterator>
{
 public:
  Index( const WindowObserverDataContainer& c )
  {
    resize( c.size() );
    size_t j = 0;
    for( value_type i = c.begin(); i != c.end(); ++i )
      ( *this )[j++] = i;
  }
  void Sort( size_t i )
  {
    sort( begin(), end(), Compare( i ) );
  }
 private:
  struct Compare
  {
    Compare( size_t idx ) : idx( idx ) {}
    bool operator()( const Index::value_type& i, const Index::value_type& j )
    { return i->first[idx] < j->first[idx]; }
    size_t idx;
  };
};

VectorPtr
WindowObserver::CDF( Number inN, MemPool& ioPool ) const
{
  Index index( *this );
  VectorPtr result = ioPool.NewVector( SampleSize() );
  for( size_t i = 0; i < result->size(); ++i )
  {
    index.Sort( i );
    Number sum = 0;
    Index::const_iterator j = index.begin();
    while( j != index.end() && ( *j )->first[i] < inN )
      sum += ( *j++ )->second.first;
    ( *result )[i] = sum;
  }
  return result;
}

VectorPtr
WindowObserver::InverseCDF( Number inN, MemPool& ioPool ) const
{
  VectorPtr result = ioPool.NewVector( SampleSize() );
  if( empty() )
  {
    *result = 0;
    return result;
  }
  // Check whether all weights are equal. If this is the case,
  // use a standard method for comparison reasons.
  const_iterator i = begin();
  Number weight = ( i++ )->second.first;
  bool equalWeights = true;
  while( equalWeights && i != end() )
    equalWeights &= ( ( i++ )->second.first == weight );
  // The target index is only used for equal weights.
  // It is computed in accordance with the "R-1" method of computing quantiles.
  int targetIndex = static_cast<int>( ::ceil( inN / Count() * size() ) - 1 );
  targetIndex = max( targetIndex, 0 );
  targetIndex = min( targetIndex, static_cast<int>( size() ) - 1 );

  Index index( *this );
  for( size_t i = 0; i < result->size(); ++i )
  {
    index.Sort( i );
    if( equalWeights )
    { // For equal weights, we may use the pre-computed target index.
      ( *result )[i] = index[targetIndex]->first[i];
    }
    else
    { // For non-equal weights, we need to search for the appropriate
      // position.
      Number sum = 0;
      size_t j = 0;
      while( sum < inN && j < index.size() )
        sum += index[j++]->second.first;
      if( j == 0 )
        ( *result )[i] = index[0]->first[i];
      else
        ( *result )[i] = index[j-1]->first[i];
    }
  }
  return result;
}

void
WindowObserver::RemoveOldSamples()
{
  while( !empty() && back().second.second >= mAgeLimit )
    pop_back();
}
