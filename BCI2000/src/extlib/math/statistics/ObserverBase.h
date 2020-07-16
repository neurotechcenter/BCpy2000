////////////////////////////////////////////////////////////////////////////////
// $Id: ObserverBase.h 4780 2015-04-03 17:24:51Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: ObserverBase provides an interface to observe multi-variate sample
//   distributions, and to compute statistical measures over these distributions,
//   such as mean, (co)variance, r squared, and z score.
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
#ifndef OBSERVER_BASE_H
#define OBSERVER_BASE_H

#include "ObserverMemory.h"
#include <limits>

namespace StatisticalObserver
{

const Number Unlimited = -1;
const Number Auto = -1;
const Number eps = std::numeric_limits<Number>::epsilon();

enum
{
  // Use the following values, ORed together, as a constructor argument to
  // indicate which functions you are going to use, or "AllFunctions"
  // to request all functions (this may entail performance penalties).
  None = 0,
  Count = 1 << 0,
  Mean = 1 << 1,
  Variance = 1 << 2,
  RSquared_ = 1 << 3,
  ZScore_ = 1 << 4,
  Covariance = 1 << 5,
  Correlation = 1 << 6,
  CentralMoment = 1 << 7,
  Skewness = 1 << 8,
  Kurtosis = 1 << 9,
  Quantile = 1 << 10,
  QQuantiles = 1 << 11,
  Histogram = 1 << 12,
  // Add any new values immediately before the ConfigMax entry, and make sure
  // there are no gaps in the sequence of bits.
  ConfigMax,

  MinConfig = Count | Mean | Variance | RSquared_ | ZScore_,
  DefaultConfig = MinConfig,
  FullConfig = ( ConfigMax - 2 ) | ( ConfigMax - 1 ),
  AllFunctions = FullConfig,
};

// The base class for all Observers.
class ObserverBase;

// Computes a bin edges vector from parameters. To be used in conjunction with Histogram().
Vector BinEdges( Number center, Number resolution, unsigned int numberOfBins );
// Computes pairwise r squared values between channels in the first and second observer.
MatrixPtr RSquared( const ObserverBase&, const ObserverBase&, MemPool& = GlobalMemPool() );
// Computes pairwise z scores between channels in the first and second observer,
// using the second observer as a reference.
MatrixPtr ZScore( const ObserverBase&, const ObserverBase& reference, MemPool& = GlobalMemPool() );


class ObserverBase
{
 public:
  // Derived classes need to provide their own Supports constant.
   enum { Supports = None };

 protected:
  // When creating an observer object, specify the functions you want to call.
  // As a second argument, the observer object specifies its Supports constant.
  ObserverBase( int, int );

 public:
  virtual ~ObserverBase() {}

  // Methods, observation interface to the user
  //
  ObserverBase& Observe( Number, Number weight = 1 );
  ObserverBase& Observe( const Vector&, Number weight = 1 );
  //  AgeBy(n) ages the observer by the equivalent of n sample durations.
  //  Typically, you call AgeBy(1) prior to Observe().
  ObserverBase& AgeBy( unsigned int count );
  ObserverBase& Clear();

  // Properties, observation interface to the user
  //
  //  The number of dimensions in a data sample.
  int SampleSize() const
    { return static_cast<int>( mSampleSize ); }
  //
  //  Age represents the observer's age in terms of sample durations.
  Number Age() const
    { return mAge; }
  //
  //  The WindowLength property specifies the asymptotic number of past samples
  //  entering into the observer's statistics.
  //  A value of "Unlimited" corresponds to the limit WindowLength -> +inf, representing
  //  a non-aging observer. This is also the default.
  //
  //  For an observer that buffers samples, the buffer's length should
  //  match the WindowLength property.
  //  For an observer that ages sample contribution weights, the aging factor
  //  should be exp(-1/(WindowLength-0.5)). Count() will then approximately approach
  //  the value of WindowLength. For convenience, an appropriate decay factor is
  //  provided in the DecayFactor property.
  ObserverBase& SetWindowLength( Number );
  Number WindowLength() const
    { return mWindowLength; }
 protected:
  Number DecayFactor() const
    { return mDecayFactor; }

  // Methods, statistics interface to the user
 public:
  Number Count( MemPool& = GlobalMemPool() ) const;
  VectorPtr Mean( MemPool& = GlobalMemPool() ) const;
  VectorPtr Variance( MemPool& = GlobalMemPool() ) const;   // Having no definition of unbiased (co)variance for aging observers,
  MatrixPtr Covariance( MemPool& = GlobalMemPool() ) const; // this is always the biased (co)variance of the distribution.
  MatrixPtr Correlation( MemPool& = GlobalMemPool() ) const;// Note that not all observers support computation of covariance.
                             // Calling the Covariance() function will then result in a BCIException
                             // which may be caught by the caller to avoid program termination.
  // Higher moments of marginal distributions.
  VectorPtr CentralMoment( unsigned int, MemPool& = GlobalMemPool() ) const; // Not all observers support computation of general central moments.
  VectorPtr Skewness( MemPool& = GlobalMemPool() ) const; // Depends on CentralMoment(3).
  VectorPtr Kurtosis( MemPool& = GlobalMemPool() ) const; // Excess Kurtosis, depends on CentralMoment(4).

  // The Quantile() function returns a vector of values corresponding to the specified argument of
  // the marginal distributions' quantile functions. Quantile( 0 ) returns a vector of minima.
  VectorPtr Quantile( Number, MemPool& = GlobalMemPool() ) const;
  // The QQuantiles() function returns a number of vectors representing q-quantiles for
  // marginal distributions, with the minimum added at the beginning, and the maximum added at the end.
  MatrixPtr QQuantiles( unsigned int q, MemPool& = GlobalMemPool() ) const;
  // The QuantileAccuracy property describes the accuracy of the quantile
  // function in terms of its argument, i.e. a QuantileAccuracy of 0.05 means
  // Q(0.875) <= Quantile(0.9) < Q(0.925) where Q is the true quantile function
  // of the observed distribution.
  // When the QuantileAccuracy property is set to "Auto", it is chosen to match
  // the relative weight of new data in relation to existing data as determined
  // by the WindowLength property. This relative weight is the inverse of
  // WindowLength (because Count() asymptotically approaches the value of
  // WindowLength, and a new data point has a weight of 1).
  // To avoid performance issues for large values of WindowLength, there is a limit for
  // automatically chosen accuracies. This limit may be queried by the
  // QuantileAccuracyAutoLimit property. A reasonable value for it is 0.005, which should
  // be precise enough for practical applications, and which corresponds to a storage
  // requirement of about 200 data points.
  ObserverBase& SetQuantileAccuracy( Number );
  Number QuantileAccuracy() const
    { return mActualAccuracy; }
  Number QuantileAccuracyAutoLimit() const
    { return 0.005; }
  // The Histogram() function returns vectors representing histograms of marginal distributions,
  // resampled to specified bins in the value domain. When bin edges are specified, bins may have
  // any width. In the result, the second index is a bin index.
  // Use the BinEdges( center, resolution, numberOfBins ) function to create a bin edge vector from parameters.
  MatrixPtr Histogram( const Vector& binEdges, MemPool& = GlobalMemPool() ) const; // Returns binEdges.size() + 1 bins.

  // Interface to be implemented by descendants
 protected:
  virtual void DoChange() {}                      // A property of the ObserverBase class has changed.
  //  Observation-related methods
  virtual void DoAgeBy( unsigned int count ) = 0; // Age existing data.
  virtual void DoObserve( const Vector&, Number weight ) = 0; // Add new data, without aging.
  virtual void DoClear() = 0;                     // Remove existing data.

 public:
  //  Computation-related methods

  //   PowerSumX() should return the weighted sum of data points taken to the Xth power.
  virtual Number PowerSum0( MemPool& ) const = 0;
  virtual VectorPtr PowerSum1( MemPool& ) const = 0;
  virtual VectorPtr PowerSum2Diag( MemPool& ) const; // Return only the diagonal elements of the 2nd power.
  virtual MatrixPtr PowerSum2Full( MemPool& ) const; // Return the full matrix of the 2nd power.
                                                     // An observer should provide at least one of the two PowerSum2 functions.
  virtual VectorPtr PowerSumDiag( unsigned int, MemPool& ) const; // The general diagonal power sum is not mandatory.
  //   Cumulated distribution functions of marginal distributions.
  //   These are not mandatory but either both or none should be implemented by an observer.
  virtual VectorPtr CDF( Number, MemPool& ) const;        // The sum of weights below the value given as argument.
  virtual VectorPtr InverseCDF( Number, MemPool& ) const; // The inverse cumulated distribution function for each dimension (non-normalized argument).

  // Implied dependencies between computations.
  static int ImpliedConfig( int ); // Returns configuration with implied dependencies set.

 private:
  ObserverBase& Change();

  int    mConfig;
  Number mWindowLength,
         mDecayFactor;
  Number mAge;
  Number mQuantileAccuracy,
         mActualAccuracy;

  size_t mSampleSize;
  Vector mBuffer;
};

} // namespace StatisticalObserver

#endif // OBSERVER_BASE_H
