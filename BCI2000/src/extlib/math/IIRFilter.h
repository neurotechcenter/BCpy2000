////////////////////////////////////////////////////////////////////////////////
// $Id: IIRFilter.h 5280 2016-03-14 18:49:29Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: An IIR filter implementation.
//   The filter's behavior is determined by the Gain, Poles, and Zeros
//   properties which define a rational transfer function in terms of
//   its numerator roots (Zeros), denominator roots (Poles), and a
//   multiplicative factor (Gain); use the FilterDesign class to calculate
//   Zeros, Poles, and Gain from desired filter properties.
//
//   Due to its implementation as a sequence of order 1 stages in DF I form,
//   the filter is numerically stable regardless of filter order.
//
//   The filter's Process() method computes an output signal from an input
//   signal, and saves its internal state (delays) for the next call to
//   Process(). Thus, continuous processing may be achieved by calling
//   Process() repeatedly.
//   Process() is templatized for its argument signal type T. This type must
//   provide the following member functions:
//     T::Channels() to return the number of channels,
//     T::Elements() to return the number of elements (samples),
//     T::operator()(channel, sample) for read/write access,
//     T::operator=() to copy one instance of T into another.
//
//   The filter's Initialize() method adapts the number of internal delays
//   to the number of channels provided, and resets all delays to zero.
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
#ifndef IIR_FILTER_H
#define IIR_FILTER_H

#include "FilterDesign.h"
#include "Debugging.h"
#include "NumericConstants.h"

template<typename Real>
class IIRFilter
{
 public:
  typedef FilterDesign::ComplexVector ComplexVector;

  IIRFilter()
    : mGain( 1 )
    {}
  ~IIRFilter()
    {}

  // Properties
  const ComplexVector& Zeros() const
    { return mZeros; }
  IIRFilter& SetZeros( const ComplexVector& z )
    { mZeros = z; return Initialize(); }
  const ComplexVector& Poles() const
    { return mPoles; }
  IIRFilter& SetPoles( const ComplexVector& p )
    { mPoles = p; return Initialize(); }
  const Real& Gain() const
    { return mGain; }
  IIRFilter& SetGain( const Real& g )
    { mGain = g; return Initialize(); }
  int Channels() const
    { return static_cast<int>( mDelays.size() ); }
  IIRFilter& SetChannels( int c )
    { return Initialize( c ); }
  bool NanStalled() const;

  // Methods
  IIRFilter& Initialize();
  IIRFilter& Initialize( size_t inChannels );
  IIRFilter& ClearNans();
  template<typename T>
   IIRFilter& Process( const T&, T& );

 private:
  Real                       mGain;
  ComplexVector              mZeros,
                             mPoles;
  std::vector<ComplexVector> mDelays;
};


template<typename Real>
bool
IIRFilter<Real>::NanStalled() const
{
  for( size_t ch = 0; ch < mDelays.size(); ++ch )
    if( !mDelays[ch].empty() && IsNaN( mDelays[ch][0].real() ) )
      return true;
  return false;
}

template<typename Real>
IIRFilter<Real>&
IIRFilter<Real>::ClearNans()
{
  for( size_t ch = 0; ch < mDelays.size(); ++ch )
    if( !mDelays[ch].empty() && IsNaN( mDelays[ch][0].real() ) )
      for( auto& delay : mDelays[ch] )
        delay = 0.0;
  return *this;
}

template<typename Real>
inline IIRFilter<Real>&
IIRFilter<Real>::Initialize()
{
  size_t channels = mDelays.size();
  return Initialize( channels );
}

template<typename Real>
inline IIRFilter<Real>&
IIRFilter<Real>::Initialize( size_t inChannels )
{
  mDelays.clear();
  mDelays.resize( inChannels, ComplexVector( mZeros.size() + 1, 0 ) );
  return *this;
}

template<typename Real>
template<typename T>
inline IIRFilter<Real>&
IIRFilter<Real>::Process( const T& Input, T& Output )
{
  Assert( mZeros.size() == mPoles.size() );
  const size_t numStages = mZeros.size();
  const int decimation = Input.Elements() / Output.Elements();
  if( numStages == 0 && mGain == 1 && decimation == 1 )
  {
    Output = Input;
  }
  else
  {
    const int channels = Input.Channels(),
              outSamples = Output.Elements();

    for( int ch = 0; ch < channels; ++ch )
    {
      Assert( mDelays[ch].size() == numStages + 1 );
      int inSample = 0;
      for( int outSample = 0; outSample < outSamples; ++outSample )
      {
        Real value = 0;
        for( int i = 0; i < decimation; ++i )
        {
          // Implementing the filter as a sequence of complex-valued order 1
          // stages in DF I form will give us higher numerical stability and
          // lower code complexity than a sequence of real-valued order 2 stages.
          // - Numerical stability: Greatest for lowest order stages.
          // - Code complexity: Poles and zeros immediately translate into complex
          //    coefficients, and need not be grouped into complex conjugate pairs
          //    as would be the case for real-valued order 2 stages.
          FilterDesign::Complex stageOutput = Input( ch, inSample++ ) * mGain;
          for( size_t stage = 0; stage < numStages; ++stage )
          {
            FilterDesign::Complex stageInput = stageOutput;
            stageOutput = stageInput
              - mZeros[stage] * mDelays[ch][stage]
              + mPoles[stage] * mDelays[ch][stage+1];
            mDelays[ch][stage] = stageInput;
          }
          mDelays[ch][numStages] = stageOutput;
          value += real( stageOutput );
        }
        Output( ch, outSample ) = value / decimation;
      }
    }
  }
  return *this;
}

#endif // IIR_FILTER_H
