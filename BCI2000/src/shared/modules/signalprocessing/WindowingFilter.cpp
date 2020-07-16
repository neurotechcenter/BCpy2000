////////////////////////////////////////////////////////////////////////////////
// $Id: WindowingFilter.cpp 5612 2017-01-24 18:02:32Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: The WindowingFilter provides
//  * Buffering of the signal into time windows that may be larger than
//    SampleBlockSize,
//  * Detrending options (mean or linear),
//  * Window functions typically used with FFT (Hann, Hamming, Blackman).
//  Typically, the Windowing filter provides its output to a spectral
//  estimator (AR, FFT).
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
#include "WindowingFilter.h"
#include "Detrend.h"
#include "Exception.h"
#include "Numeric.h"

using namespace std;
using namespace Detrend;

WindowingFilter::WindowingFilter()
: mDetrend( None ),
  mWindowFunction( Rectangular )
{
}

void
WindowingFilter::Publish()
{
 BEGIN_PARAMETER_DEFINITIONS
  "Filtering:Windowing int WindowLength= 0.5s 0.5s % % "
      "// Length of window",
  "Filtering:Windowing int Detrend= 0 0 0 2 "
      "// Detrend data? "
         "0: no, "
         "1: mean, "
         "2: linear "
         "(enumeration)",
  "Filtering:Windowing int WindowFunction= 0 0 0 3 "
      "// Window function "
         "0: Rectangular, "
         "1: Hamming, "
         "2: Hann, "
         "3: Blackman "
         "(enumeration)",
 END_PARAMETER_DEFINITIONS
}


void
WindowingFilter::Preflight( const SignalProperties& Input,
                                    SignalProperties& Output ) const
{
  Output = Input;
  double windowLength = Parameter( "WindowLength" ).InSampleBlocks();
  int numSamples = static_cast<int>( windowLength * Input.Elements() );
  if( numSamples < 0 )
  {
    bcierr << "WindowLength parameter must be >= 0" << endl;
    numSamples = 0;
  }
  Output.SetElements( numSamples )
        .SetIsStream( false )
        .ElementUnit().SetRawMin( 0 )
                      .SetRawMax( Output.Elements() - 1 );
}


void
WindowingFilter::Initialize( const SignalProperties& Input,
                             const SignalProperties& Output )
{
  size_t numSamples = Output.Elements();
  mBuffers.clear();
  mBuffers.resize( Input.Channels(), DataVector( numSamples ) );

  mDetrendBuffers.clear();
  mDetrend = Parameter( "Detrend" );
  if( mDetrend != None )
    mDetrendBuffers.resize( Input.Channels(), DataVector( numSamples ) );

  mWindowFunction = Parameter( "WindowFunction" );
  mWindow.resize( numSamples );
  Real phasePerSample = Pi<Real>() / numSamples;
  // Window coefficients: Rect Hamming Hann Blackman
  const Real a1[] = {    0,   0.46,  0.5, 0.5, },
             a2[] = {    0,   0,     0,   0.08 };
  for( int i = 0; i <= numSamples / 2; ++i ) 
  {
    mWindow[ i ] = 1.0f - a1[ mWindowFunction ] - a2[ mWindowFunction ]
                   + a1[ mWindowFunction ] * cos( ( numSamples - ( i * 2 ) ) * phasePerSample )
                   + a2[ mWindowFunction ] * cos( ( numSamples - ( i * 2 ) ) * 2.0f * phasePerSample );
    mWindow[ numSamples - i - 1 ] = mWindow[ i ];
  }
}

void
WindowingFilter::Process( const GenericSignal& Input, GenericSignal& Output )
{
  Output.EnsureDeepCopy();
  #pragma omp parallel for
  for( int ch = 0; ch < Input.Channels(); ++ch )
  {
    // Move existing data towards the beginning of the buffer.
    size_t i = 0,
           j = Input.Elements();
    while( j < mBuffers[ch].size() )
      mBuffers[ch][i++] = mBuffers[ch][j++];

    // Fill the rightmost part of the buffer with new input:
    i = max<ptrdiff_t>( 0, mBuffers[ch].size() - Input.Elements() );
    j = 0;
    while( i < mBuffers[ch].size() )
      mBuffers[ch][i++] = Input( ch, j++ );

    DataVector* pDetrendedData = NULL;
    switch( mDetrend )
    {
      case None:
        pDetrendedData = &mBuffers[ch];
        break;

      case Mean:
        MeanDetrend( mBuffers[ch], mDetrendBuffers[ch] );
        pDetrendedData = &mDetrendBuffers[ch];
        break;

      case Linear:
        LinearDetrend( mBuffers[ch], mDetrendBuffers[ch] );
        pDetrendedData = &mDetrendBuffers[ch];
        break;

      default:
        throw std_logic_error << "Unknown detrend option";
    }

    if( mWindowFunction == Rectangular )
      for( size_t i = 0; i < pDetrendedData->size(); ++i )
        Output( ch, i ) = ( *pDetrendedData )[i];
    else
      for( size_t i = 0; i < pDetrendedData->size(); ++i )
        Output( ch, i ) = mWindow[i] * ( *pDetrendedData )[i];
  }
}

void
WindowingFilter::StartRun()
{
  for( size_t ch = 0; ch < mBuffers.size(); ++ch )
    mBuffers[ch] = 0;
}
