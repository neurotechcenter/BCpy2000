////////////////////////////////////////////////////////////////////////////////
// $Id: HistogramObserver.cpp 3798 2012-02-01 18:07:06Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: See the header file for documentation.
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

#include "HistogramObserver.h"
#include "BCIException.h"

using namespace std;
using namespace StatisticalObserver;

HistogramObserver::HistogramObserver( int inConfig )
: ObserverBase( inConfig, Supports ),
  mPowerSum0( 0 )
{
}

void
HistogramObserver::DoChange()
{
  if( mHistograms.size() != SampleSize() )
    DoClear();
}

void
HistogramObserver::DoAgeBy( unsigned int inCount )
{
  if( DecayFactor() == 1 )
    return;

  Number factor = ::pow( DecayFactor(), static_cast<int>( inCount ) );
  mPowerSum0 *= factor;
  for( size_t i = 0; i < mHistograms.size(); ++i )
  {
    mHistograms[i] *= factor;
    Number PowerSum1 = mHistograms[i].PowerSum( 1 ),
           PowerSum2 = mHistograms[i].PowerSum( 2 ),
           sdev = ::sqrt( ( PowerSum2 - PowerSum1 * PowerSum1 / mPowerSum0 ) / mPowerSum0 );
    mHistograms[i].Prune( QuantileAccuracy(), sdev * QuantileAccuracy() );
  }
}

void
HistogramObserver::DoObserve( const Vector& inV, Number inWeight )
{
  mPowerSum0 += inWeight;
  for( size_t i = 0; i < mHistograms.size(); ++i )
    mHistograms[i].Add( inV[i], inWeight );
}

void
HistogramObserver::DoClear()
{
  mPowerSum0 = 0;
  mHistograms.clear();
  mHistograms.resize( SampleSize() );
}

VectorPtr
HistogramObserver::PowerSumDiag( unsigned int inPower, MemPool& ioPool ) const
{
  VectorPtr result = ioPool.NewVector( mHistograms.size() );
  for( size_t i = 0; i < mHistograms.size(); ++i )
    ( *result )[i] = mHistograms[i].PowerSum( inPower );
  return result;
}

VectorPtr
HistogramObserver::CDF( Number inN, MemPool& ioPool ) const
{
  VectorPtr result = ioPool.NewVector( mHistograms.size() );
  for( size_t i = 0; i < mHistograms.size(); ++i )
    ( *result )[i] = mHistograms[i].CDF( inN );
  return result;
}

VectorPtr
HistogramObserver::InverseCDF( Number inN, MemPool& ioPool ) const
{
  VectorPtr result = ioPool.NewVector( mHistograms.size() );
  for( size_t i = 0; i < mHistograms.size(); ++i )
    ( *result )[i] = mHistograms[i].InverseCDF( inN );
  return result;
}

