////////////////////////////////////////////////////////////////////////////////
// $Id: FFTSpectrum.cpp 5612 2017-01-24 18:02:32Z mellinger $
// Authors: mcfarlan@wadsworth.org, juergen.mellinger@uni-tuebingen.de,
//          Adam Wilson
// Description: The ARSpectrum filter fits a Maximum Entropy AR model to
//   its input data. Its output can be configured to be
//   - raw AR coefficients,
//   - the model's amplitude spectrum,
//   - the model's intensity spectrum.
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
#include "FFTSpectrum.h"

#include "Numeric.h"

using namespace std;

void
FFTSpectrum::Publish()
{
  Super::Publish();
}

void
FFTSpectrum::Preflight( const SignalProperties& Input,
                              SignalProperties& Output ) const
{
  if( !ComplexFFT::LibAvailable() )
    bcierr << "Could not find the " << ComplexFFT::LibName() << " library."
           << endl;
  Super::Preflight( Input, Output );
  if( Parameter( "OutputType" ) == Coefficients )
  {
    Output.SetElements( 2 * NumberOfBins( Input ) );
    Output.ElementUnit() = PhysicalUnit();
    Output.ElementUnit().SetRawMin( 0 )
                        .SetRawMax( Output.Elements() - 1 );
    int fftSize = 2 * Ceil( Input.SamplingRate() / 2 / Parameter( "BinWidth" ).InHertz() ) + 1;
    if( fftSize < Input.Elements() )
      bciwarn << "Multiple FFTs will be performed per input window, and "
              << "resulting coefficients will be summed up, such that some "
              << "components of the input signal will be lost. "
              << "It is recommended that you reduce either the BinWidth, or the WindowLength parameter";
  }
  Output.SetName( "FFT " + Output.Name() );
}


void
FFTSpectrum::Initialize( const SignalProperties& Input, const SignalProperties& Output )
{
  mOutputType = Parameter( "OutputType" );
  double binWidthHz = Parameter( "BinWidth" ).InHertz(),
         firstBinCenterHz = Parameter( "FirstBinCenter" ).InHertz();
  mFFTs.clear();
  mFFTs.resize( Input.Channels() );
  int fftSize = 2 * Ceil( Input.SamplingRate() / 2 / binWidthHz ) + 1;
  for( size_t i = 0; i < mFFTs.size(); ++i )
    mFFTs[i].Initialize( fftSize );
  mShiftCarrier.resize( Input.Elements() );
  double carrierOmega = -2 * Pi<Real>() * firstBinCenterHz / Input.SamplingRate();
  for( int t = 0; t < Input.Elements(); ++t )
    mShiftCarrier[t] = polar<Real>( 1, t * carrierOmega );
  // Normalization is applied to energy, and chosen such that input energy per second matches output energy in all bins.
  mNormalizationFactor = 1;
  // Output is per duration of input data in seconds.
  mNormalizationFactor /= ( 1.0 * Input.Elements() / Input.SamplingRate() );
  // Output is per bin width.
  mNormalizationFactor /= binWidthHz;
  // An extra factor of 1/fftSize to achieve proper FFT normalization.
  mNormalizationFactor /= fftSize;
}

void
FFTSpectrum::Process( const GenericSignal& Input, GenericSignal& Output )
{
  Output.EnsureDeepCopy();
  #pragma omp parallel for
  for( int ch = 0; ch < Input.Channels(); ++ch )
  {
    for( size_t i = 0; i < Output.Elements(); ++i )
      Output( ch, i ) = 0;

    int sample = 0;
    while( sample < Input.Elements() )
    {
      for( int i = 0; i < mFFTs[ch].Size() && sample < Input.Elements(); ++i, ++sample )
        mFFTs[ch].Input( i ) = Input( ch, sample ) * mShiftCarrier[sample];
      for( int i = sample; i < mFFTs[ch].Size(); ++i )
        mFFTs[ch].Input( i ) = 0;
      mFFTs[ch].Compute();

      switch( mOutputType )
      {
        case SpectralAmplitude:
        case SpectralPower:
          for( size_t i = 0; i < Output.Elements(); ++i )
            Output( ch, i ) += norm( mFFTs[ch].Output( i ) );
          break;
        case Coefficients:
          for( size_t i = 0; i < Output.Elements() / 2; ++i )
            Output( ch, i ) += mFFTs[ch].Output( i ).real();
          for( size_t i = 0; i < Output.Elements() / 2; ++i )
            Output( ch, i + Output.Elements() / 2 ) += mFFTs[ch].Output( i ).imag();
          break;
      }
    }
    switch( mOutputType )
    {
      case SpectralAmplitude:
        for( int i = 0; i < Output.Elements(); ++i )
          Output( ch, i ) = ::sqrt( Output( ch, i ) * mNormalizationFactor );
        break;
      case SpectralPower:
        for( int i = 0; i < Output.Elements(); ++i )
          Output( ch, i ) *= mNormalizationFactor;
        break;
      case Coefficients:
        for( int i = 0; i < Output.Elements(); ++i )
          Output( ch, i ) *= ::sqrt( mNormalizationFactor );
        break;
    }
  }
}

