////////////////////////////////////////////////////////////////////////////////
// $Id: CoherenceFilter.cpp 4981 2015-08-26 17:04:38Z mellinger $
// Authors: juergen.mellinger@uni-tuebingen.de
// Description: A filter that computes magnitude squared coherence between its
//   input signals at a set of frequencies.
//   Output channels correspond to pairs of input channels, output elements
//   correspond to frequencies at which coherence is evaluated.
//
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
#include "CoherenceFilter.h"

#include <numeric>
#include <algorithm>

using namespace std;


RegisterFilter( CoherenceFilter, 2.C );


CoherenceFilter::CoherenceFilter()
{
 BEGIN_PARAMETER_DEFINITIONS
   "Filtering:CoherenceFilter float CohBufferLength= 3s 0 % % // length of buffer over which coherence is computed, in seconds or blocks",
   "Filtering:CoherenceFilter float CohWindowLength= 0.5s 0 % % // length of moving window for frequency analysis, in seconds or blocks",
   "Filtering:CoherenceFilter float CohWindowOverlap= 0.25s 0 % % // overlap between windows, in seconds or blocks",
   "Filtering:CoherenceFilter floatlist CohFrequencies= 2 10Hz 20Hz 0 % % // list of frequencies where coherence is analyzed, corresponds to output elements",
 END_PARAMETER_DEFINITIONS
}


CoherenceFilter::~CoherenceFilter()
{
}

void
CoherenceFilter::Preflight( const SignalProperties& Input, SignalProperties& Output ) const
{
  // Check parameter consistency.
  int bufferLength = static_cast<int>( Parameter( "CohBufferLength" ).InSampleBlocks() * Input.Elements() ),
      windowLength = static_cast<int>( Parameter( "CohWindowLength" ).InSampleBlocks() * Input.Elements() ),
      windowOverlap = static_cast<int>( Parameter( "CohWindowOverlap" ).InSampleBlocks() * Input.Elements() );
  if( bufferLength < windowLength )
    bcierr << "The CohBufferLength parameter must be greater or equal to the CohWindowLength parameter"
           << endl;
  if( windowOverlap >= windowLength )
    bcierr << "The CohWindowLength parameter must be greater than the CohWindowOverlap parameter"
           << endl;

  ParamRef CohFrequencies = Parameter( "CohFrequencies" );
  for( int bin = 0; bin < CohFrequencies->NumValues(); ++bin )
  {
    double frequency = CohFrequencies( bin ).InHertz();
    if( frequency > Input.SamplingRate() / 2.0 )
      bcierr << "Entry " << bin + 1 << " of the CohFrequencies parameter exceeds half "
             << "the input sampling rate of the CoherenceFilter"
             << endl;
    const double eps = 1e-6;
    double cycles = frequency * Parameter( "CohWindowLength" ).InSeconds();
    if( ::fabs( cycles - ::floor( cycles + 0.5 ) ) > eps )
      bcierr << "Entry " << bin + 1 << " of the CohFrequencies parameter is "
             << frequency << "Hz, corresponds to non-integer number of cycles "
             << "(" << cycles << ") in the window specified by CohWindowLength"
             << endl;
  }

  // Determine output properties.
  Output = Input;
  Output.SetChannels( Input.Channels() * ( Input.Channels() - 1 ) / 2 );
  int outChannel = 0;
  for( int ch1 = 0; ch1 < Input.Channels(); ++ch1 )
  {
    string label1 = Input.ChannelLabels()[ch1];
    for( int ch2 = 0; ch2 < ch1; ++ch2 )
    {
      string label2 = Input.ChannelLabels()[ch2],
             outLabel = string( "Coh-" ) + label1 + "-" + label2;
      Output.ChannelLabels()[outChannel++] = outLabel;
    }
  }
  Output.SetElements( CohFrequencies->NumValues() );
  for( int bin = 0; bin < CohFrequencies->NumValues(); ++bin )
    Output.ElementLabels()[bin] = string( CohFrequencies( bin ) );
  Output.ElementUnit().SetGain( 1.0 ).SetOffset( 0.0 ).SetSymbol( "" )
                      .SetRawMin( 0.0 ).SetRawMax( 1.0 );
  Output.ValueUnit().SetGain( 1.0 ).SetOffset( 0.0 ).SetSymbol( "" )
                    .SetRawMin( 0.0 ).SetRawMax( 1.0 );
}


void
CoherenceFilter::Initialize( const SignalProperties& Input, const SignalProperties& Output )
{
  // Configure the input buffer.
  mInputBuffer.clear();
  int bufferLength = static_cast<int>( Parameter( "CohBufferLength" ).InSampleBlocks() * Input.Elements() );
  mInputBuffer.resize( Input.Channels(), valarray<real>( bufferLength ) );

  // Configure FIR coefficients.
  int windowLength = static_cast<int>( Parameter( "CohWindowLength" ).InSampleBlocks() * Input.Elements() ),
      numBins = Parameter( "CohFrequencies" )->NumValues();
  mFIRCoefficients.clear();
  mFIRCoefficients.resize( numBins, valarray<complex>( windowLength ) );
  for( size_t bin = 0; bin < mFIRCoefficients.size(); ++bin )
  {
    real frequency = Parameter( "CohFrequencies" )( bin ).InHertz() / Input.SamplingRate();
    for( size_t time = 0; time < mFIRCoefficients[bin].size(); ++time )
    {
      // e^(iwt)
      mFIRCoefficients[bin][time] = polar( 1.0, 2.0 * M_PI * frequency * time );
      // Hamming window
      mFIRCoefficients[bin][time] *= 0.54 - 0.46 * cos( 2.0 * M_PI * time / mFIRCoefficients[bin].size() );
    }
  }

  // Configure FIR step size.
  int windowOverlap = static_cast<int>( Parameter( "CohWindowOverlap" ).InSampleBlocks() * Input.Elements() );
  mConvolutionStep = windowLength - windowOverlap;

  // Configure the FIR convolution buffer.
  mFIRConvolution.clear();
  mFIRConvolution.resize( Input.Channels(), vector< valarray<complex> >( numBins ) );
  for( size_t ch = 0; ch < mFIRConvolution.size(); ++ch )
  {
    for( size_t bin = 0; bin < mFIRConvolution[ch].size(); ++bin )
    {
      int numConvolutionSamples = 0;
      for( size_t sample = 0; sample < mInputBuffer[ch].size() - mFIRCoefficients[bin].size(); sample += mConvolutionStep )
        ++numConvolutionSamples;
      mFIRConvolution[ch][bin].resize( numConvolutionSamples );
    }
  }
}

void
CoherenceFilter::StartRun()
{
  for( size_t ch = 0; ch < mInputBuffer.size(); ++ch )
    mInputBuffer[ch] = 0.0;
}


void
CoherenceFilter::Process( const GenericSignal& Input, GenericSignal& Output )
{
  // Compute FIR convolution for all channels.
  for( int ch = 0; ch < Input.Channels(); ++ch )
  {
    // Buffer incoming data.
    // - Shift buffer contents back by one sample block.
    for( size_t sample = 0; sample < mInputBuffer[ch].size() - Input.Elements(); ++sample )
      mInputBuffer[ch][sample] = mInputBuffer[ch][sample + Input.Elements()];
    // - Write new samples to the end of the buffer.
    for( int sample = max( 0, Input.Elements() - static_cast<int>( mInputBuffer[ch].size() ) ); sample < Input.Elements(); ++sample )
      mInputBuffer[ch][sample + mInputBuffer[ch].size() - Input.Elements()] = Input( ch, sample );
    // Remove mean to avoid artifacts.
    valarray<real> zeroMeanBuffer = mInputBuffer[ch];
    zeroMeanBuffer -= ::accumulate( &zeroMeanBuffer[0], &zeroMeanBuffer[zeroMeanBuffer.size()], 0.0 ) / zeroMeanBuffer.size();
    // Convolve with the FIR filter using the configured step size.
    for( size_t bin = 0; bin < mFIRCoefficients.size(); ++bin )
    {
      int i = 0;
      for( size_t sample = 0; sample < zeroMeanBuffer.size() - mFIRCoefficients[bin].size(); sample += mConvolutionStep )
        mFIRConvolution[ch][bin][i++] = inner_product(
                                          &mFIRCoefficients[bin][0],
                                          &mFIRCoefficients[bin][mFIRCoefficients[bin].size()],
                                          &zeroMeanBuffer[sample],
                                          complex( 0.0 )
                                        );
    }
  }
  // Compute coherences between pairs of channels.
  for( size_t bin = 0; bin < mFIRCoefficients.size(); ++bin )
  {
    int outputCh = 0;
    for( int ch1 = 0; ch1 < Input.Channels(); ++ch1 )
    {
      real prod11 = abs( InnerProduct( mFIRConvolution[ch1][bin], mFIRConvolution[ch1][bin] ) );
      for( int ch2 = 0; ch2 < ch1; ++ch2 )
      {
        real prod22 = abs( InnerProduct( mFIRConvolution[ch2][bin], mFIRConvolution[ch2][bin] ) ),
             prod12 = abs( InnerProduct( mFIRConvolution[ch1][bin], mFIRConvolution[ch2][bin] ) ),
             coherence = 0.0;
        if( prod11 > 0.0 && prod22 > 0.0 )
          coherence = prod12 * prod12 / prod11 / prod22;
        Output( outputCh++, bin ) = coherence;
      }
    }
  }
}

CoherenceFilter::complex
CoherenceFilter::InnerProduct( const valarray<complex>& inV1, const valarray<complex>& inV2 )
{
  return ( inV1 * inV2.apply( conj ) ).sum();
}
