////////////////////////////////////////////////////////////////////////////////
// $Id: ObserverBase.cpp 4970 2015-08-21 16:46:07Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: See the associated header file for documentation.
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

#include "ObserverBase.h"
#include "BCIException.h"
#include "Debugging.h"
#include "ClassName.h"

using namespace std;
using namespace StatisticalObserver;

// Independent function definitions
Vector
StatisticalObserver::BinEdges( Number inCenter, Number inResolution, unsigned int inNumBins )
{
  Vector binEdges;
  if( inNumBins < 1 )
    throw std_invalid_argument << "NumBins argument is " << inNumBins << ", must be > 0";
  if( ::fabs( inResolution ) < eps )
    throw std_invalid_argument << "Resolution argument is " << inResolution << ", may not be 0";
  binEdges.resize( inNumBins - 1 );
  for( size_t i = 0; i < inNumBins - 1; ++i )
    binEdges[i] = inResolution * ( i + 1 ) + inCenter - inResolution * inNumBins / 2;
  return binEdges;
}


MatrixPtr
StatisticalObserver::RSquared( const ObserverBase& inObs1, const ObserverBase& inObs2, MemPool& ioPool )
{
  if( inObs1.SampleSize() == 0 || inObs2.SampleSize() == 0 )
    throw std_runtime_error << "Trying to compute RSquared without observation";

  MatrixPtr result = ioPool.NewMatrix( inObs1.SampleSize(), inObs2.SampleSize() );

  Number n1 = inObs1.Count(),
         n2 = inObs2.Count();
  if( n1 < eps || n2 < eps )
  {
    *result = 0;
  }
  else
  {
    VectorPtr sum1 = inObs1.PowerSum1( ioPool ),
              sum2 = inObs2.PowerSum1( ioPool ),
              sqSum1 = inObs1.PowerSum2Diag( ioPool ),
              sqSum2 = inObs2.PowerSum2Diag( ioPool );
    for( size_t i = 0; i < sum1->size(); ++i )
      for( size_t j = 0; j < sum2->size(); ++j )
      {
        Number G = ( ( *sum1 )[i] + ( *sum2 )[j] ) * ( ( *sum1 )[i] + ( *sum2 )[j] ) / ( n1 + n2 ),
               numerator = ( *sum1 )[i] * ( *sum1 )[i] / n1 + ( *sum2 )[j] * ( *sum2 )[j] / n2 - G,
               denominator = ( *sqSum1 )[i] + ( *sqSum2 )[j] - G,
               rsq = 0;
        if( ::fabs( denominator ) > eps )
          rsq = ::max<Number>( numerator / denominator, 0 );
        ( *result )[i][j] = rsq;
      }
  }
  return result;
}

MatrixPtr
StatisticalObserver::ZScore( const ObserverBase& inDist, const ObserverBase& inRef, MemPool& ioPool )
{
  VectorPtr mean = inDist.Mean(),
            refMean = inRef.Mean(),
            refStd = inRef.Variance();
  refStd->transform( ::sqrt );
  MatrixPtr result = ioPool.NewMatrix( mean->size(), refMean->size() );
  for( size_t i = 0; i < mean->size(); ++i )
    for( size_t j = 0; j < refMean->size(); ++j )
      if( ::fabs( ( *refStd )[j] ) > eps )
        ( *result )[i][j] = ::fabs( ( *mean )[i] - ( *refMean )[j] ) / ( *refStd )[j];
      else
        ( *result )[i][j] = 0;
  return result;
}


// ObserverBase definitions
ObserverBase::ObserverBase( int inConfig, int inSupported )
: mConfig( ImpliedConfig( inConfig ) ),
  mAge( 0 ),
  mWindowLength( Unlimited ),
  mDecayFactor( 1 ),
  mQuantileAccuracy( Auto ),
  mActualAccuracy( 0 ),
  mSampleSize( 0 )
{
  if( mConfig & ~ImpliedConfig( inSupported ) )
    throw std_logic_error << "Unsupported function requested";
}

// Observations
ObserverBase&
ObserverBase::Observe( Number inN, Number inWeight )
{
  if( mBuffer.size() != 1 )
    mBuffer.resize( 1 );
  mBuffer[0] = inN;
  return Observe( mBuffer, inWeight );
}

ObserverBase&
ObserverBase::Observe( const Vector& inV, Number inWeight )
{
  if( mSampleSize == 0 )
  {
    mSampleSize = inV.size();
    DoChange();
  }
  if( inV.size() != mSampleSize )
    throw std_logic_error
      << "Observe(Vector) called with inconsistent sample sizes, expected: " << mSampleSize
      << ", got: " << inV.size();
  DoObserve( inV, inWeight );
  return *this;
}

ObserverBase&
ObserverBase::AgeBy( unsigned int inCount )
{
  DoAgeBy( inCount );
  mAge += inCount;
  return *this;
}

ObserverBase&
ObserverBase::Clear()
{
  mSampleSize = 0;
  mAge = 0;
  DoClear();
  return *this;
}

// Properties
ObserverBase&
ObserverBase::SetWindowLength( Number inLength )
{
  if( inLength != Unlimited && inLength < eps )
    throw std_invalid_argument << "Argument is " << inLength << ", must be > 0, or \"Unlimited\"";
  mWindowLength = inLength;
  return Change();
}

ObserverBase&
ObserverBase::SetQuantileAccuracy( Number inAccuracy )
{
  if( inAccuracy != Auto && ( inAccuracy < 0 || inAccuracy > 1 ) )
    throw std_invalid_argument << "QuantileAccuracy is " << mQuantileAccuracy << ", must be in [0,1], or \"Auto\"";
  mQuantileAccuracy = inAccuracy;
  Change();
  return *this;
}

ObserverBase&
ObserverBase::Change()
{
  if( WindowLength() > 0.5 + eps )
    mDecayFactor = ::exp( - 1 / ( WindowLength() - 0.5 ) );
  else
    mDecayFactor = 1;

  if( mQuantileAccuracy == Auto )
  {
    Number autoAccuracy = 0;
    if( WindowLength() > 0 )
      autoAccuracy = 1 / WindowLength();
    mActualAccuracy = ::max( autoAccuracy, QuantileAccuracyAutoLimit() );
  }
  else
  {
    mActualAccuracy = mQuantileAccuracy;
  }
  DoChange();
  return *this;
}

// Statistical results
#define REQUIRE( x )  \
  if( !( mConfig & StatisticalObserver::x ) ) \
    throw std_logic_error << ClassName( typeid( *this ) ) << " was not configured to compute function " << #x << "()";

Number
ObserverBase::Count( MemPool& ioPool ) const
{
  REQUIRE( Count );
  return PowerSum0( ioPool );
}

VectorPtr
ObserverBase::Mean( MemPool& ioPool ) const
{
  REQUIRE( Mean );
  VectorPtr result = PowerSum1( ioPool );
  *result /= Count( ioPool );
  return result;
}

VectorPtr
ObserverBase::Variance( MemPool& ioPool ) const
{
  REQUIRE( Variance );
  VectorPtr result = PowerSum2Diag( ioPool );
  *result /= Count( ioPool );
  VectorPtr sqMean = Mean( ioPool );
  *sqMean *= *sqMean;
  *result -= *sqMean;
  for( size_t i = 0; i < result->size(); ++i )
    if( ( *result )[i] < 0 )
      ( *result )[i] = 0;
  return result;
}

MatrixPtr
ObserverBase::Covariance( MemPool& ioPool ) const
{
  REQUIRE( Covariance );
  MatrixPtr result = PowerSum2Full( ioPool );
  *result /= Count();
  result->SubtractOuterProduct( *Mean( ioPool ) );
  return result;
}

MatrixPtr
ObserverBase::Correlation( MemPool& ioPool ) const
{
  REQUIRE( Correlation );
  MatrixPtr result = Covariance( ioPool );
  for( size_t i = 0; i < result->size(); ++i )
  {
    Number varI = ( *result )[i][i];
    ( *result )[i][i] = 1;
    for( size_t j = i + 1; j < result->size(); ++j )
    {
      Number varJ = ( *result )[j][j];
      if( varI < eps && varJ < eps )
      { // For a pair of constant values, we return 0
        // (midway between perfect correlation, and perfect anticorrelation).
        ( *result )[i][j] = 0;
      }
      else
      { // If only one of the values is constant,
        // covariance will be zero, and we return zero correlation.
        if( varI >= eps )
          ( *result )[i][j] /= ::sqrt( varI );
        if( varJ >= eps )
          ( *result )[i][j] /= ::sqrt( varJ );
      }
      ( *result )[j][i] = ( *result )[i][j];
    }
  }
  return result;
}

VectorPtr
ObserverBase::CentralMoment( unsigned int inN, MemPool& ioPool ) const
{
  REQUIRE( CentralMoment );
  VectorPtr result = ioPool.NewVector( SampleSize() );
  if( inN == 0 )
    *result = 1;
  else if( inN == 1 )
    *result = 0;
  else if( inN == 2 )
    result = Variance( ioPool );
  else
  {
    struct {
      int operator()( int n, int k )
      {
        Assert( n >= k );
        if( k > n - k )
          k = n - k;
        int r = 1,
            j = 0;
        while( j < k )
        {
          r *= ( n - j );
          r /= ++j;
        }
        return r;
      }
    } binomialCoeff;

    VectorPtr negativeMean = Mean( ioPool ),
              meanPower = ioPool.NewVector( SampleSize() );
    *negativeMean *= -1;
    *meanPower = 1;
    Number count = PowerSum0( ioPool );
    *result = 0;
    for( signed int j = inN; j >= 0; --j, *meanPower *= *negativeMean )
    {
      VectorPtr summand = PowerSumDiag( j, ioPool );
      *summand *= *meanPower;
      *summand *= binomialCoeff( inN, j ) / count;
      *result += *summand;
    }
  }
  return result;
}

VectorPtr
ObserverBase::Skewness( MemPool& ioPool ) const
{
  REQUIRE( Skewness );
  VectorPtr variance = Variance( ioPool ),
            denominator = ioPool.NewVector( SampleSize() );
  *denominator = *variance;
  *denominator *= *variance;
  *denominator *= *variance;
  denominator->transform( ::sqrt );
  VectorPtr result = CentralMoment( 3, ioPool );
  for( size_t i = 0; i < result->size(); ++i )
  {
    if( ( *denominator )[i] < eps )
      ( *result )[i] = 0; // For zero variance, skewness is 0.
    else
      ( *result )[i] /= ( *denominator )[i];
  }
  return result;
}

VectorPtr
ObserverBase::Kurtosis( MemPool& ioPool ) const
{
  REQUIRE( Kurtosis );
  VectorPtr denominator = Variance( ioPool );
  *denominator *= *denominator;
  VectorPtr result = CentralMoment( 4, ioPool );
  for( size_t i = 0; i < result->size(); ++i )
  {
    if( ( *denominator )[i] < eps )
      ( *result )[i] = 1; // The zero variance limit of the standardized 4th central moment is 1.
    else
      ( *result )[i] /= ( *denominator )[i];
  }
  *result -= 3;
  return result;
}

VectorPtr
ObserverBase::Quantile( Number inP, MemPool& ioPool ) const
{
  REQUIRE( Quantile );
  if( inP < 0 || inP > 1 )
    throw std_invalid_argument << "Argument is " << inP << ", must be in [0,1]";
  return InverseCDF( inP * PowerSum0( ioPool ), ioPool );
}

MatrixPtr
ObserverBase::QQuantiles( unsigned int inQ, MemPool& ioPool ) const
{
  REQUIRE( QQuantiles );
  MatrixPtr result = ioPool.NewMatrix( SampleSize(), inQ + 1 );
  for( size_t i = 0; i <= inQ; ++i )
  {
    VectorPtr quantile = Quantile( 1.0 * i / inQ, ioPool );
    for( size_t j = 0; j < result->size(); ++j )
      ( *result )[j][i] = ( *quantile )[j];
  }
  return result;
}

MatrixPtr
ObserverBase::Histogram( const Vector& inBinEdges, MemPool& ioPool ) const
{
  REQUIRE( Histogram );
  MatrixPtr result = ioPool.NewMatrix( SampleSize(), inBinEdges.size() + 1 );
  VectorPtr leftCDF = ioPool.NewVector( SampleSize() );
  *leftCDF = 0;
  size_t bin = 0;
  while( bin < inBinEdges.size() )
  {
    VectorPtr rightCDF = CDF( inBinEdges[bin], ioPool );
    *leftCDF -= *rightCDF;
    for( size_t i = 0; i < result->size(); ++i )
      ( *result )[i][bin] = -( *leftCDF )[i];
    ++bin;
    leftCDF = rightCDF;
  }
  Number count = Count( ioPool );
  for( size_t i = 0; i < result->size(); ++i )
    ( *result )[i][bin] = count - ( *leftCDF )[i];
  return result;
}

// Default implementations of computation functions
VectorPtr
ObserverBase::PowerSum2Diag( MemPool& ioPool ) const
{
  MatrixPtr full = PowerSum2Full( ioPool );
  VectorPtr result = ioPool.NewVector( SampleSize() );
  for( size_t i = 1; i < result->size(); ++i )
    ( *result )[i] = ( *full )[i][i];
  return result;
}

#define UNSUPPORTED throw std_logic_error << ClassName( typeid( *this ) ) << " does not support this computation";

MatrixPtr
ObserverBase::PowerSum2Full( MemPool& ioPool ) const
{
  UNSUPPORTED;
  return ioPool.NewMatrix( 0, 0 );
}

VectorPtr
ObserverBase::PowerSumDiag( unsigned int i, MemPool& ioPool ) const
{
  VectorPtr result = ioPool.NewVector( SampleSize() );
  switch( i )
  {
    case 0:
      *result = PowerSum0( ioPool );
    case 1:
      result = PowerSum1( ioPool );
    case 2:
      result = PowerSum2Diag( ioPool );
    default:
      UNSUPPORTED;
  }
  return result;
}

VectorPtr
ObserverBase::CDF( Number, MemPool& ioPool ) const
{
  UNSUPPORTED;
  return ioPool.NewVector( SampleSize() );
}


VectorPtr
ObserverBase::InverseCDF( Number, MemPool& ioPool ) const
{
  UNSUPPORTED;
  return ioPool.NewVector( SampleSize() );
}

int
ObserverBase::ImpliedConfig( int inConfig )
{ // This function describes dependencies of ObserverBase
  // functions.
  const int groups[] =
  {
    StatisticalObserver::CentralMoment |
    StatisticalObserver::Skewness |
    StatisticalObserver::Kurtosis,

    StatisticalObserver::Correlation |
    StatisticalObserver::Covariance,

    StatisticalObserver::Quantile |
    StatisticalObserver::QQuantiles |
    StatisticalObserver::Histogram,
  };

  int config = inConfig | MinConfig;
  for( size_t i = 0; i < sizeof( groups ) / sizeof( *groups ); ++i )
    if( config & groups[i] )
      config |= groups[i];

  return config;
}


