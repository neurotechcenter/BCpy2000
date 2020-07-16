////////////////////////////////////////////////////////////////////////////////
// $Id: FFTFilter.cpp 5531 2016-09-15 02:18:12Z gmilsap $
// Description: A BCI2000 filter that applies a short-term FFT to its input
//   signal.
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
#include "FFTFilter.h"
#include "Debugging.h"
#include "Numeric.h"
#include "IndexList.h"
#include <vector>

using namespace std;

RegisterFilter( FFTFilter, 2.B1 ); // Default position after the spatial filter.

FFTFilter::FFTFilter()
: mVisualizeFFT( 0 ),
  mFFTOutputSignal( eInput ),
  mFFTWindowLength( 0 ),
  mFFTWindow( eNone )
{
 BEGIN_PARAMETER_DEFINITIONS
   "Filtering int FFTOutputSignal= 0"
     " 0 0 2 // Signal provided to the next filter: "
              "0: Input Signal, "
              "1: Power Spectra of selected Channels "
              "2: Complex Amplitudes of selected Channels "
              "(enumeration)",
   "Filtering intlist FFTInputChannels= 1 1"
     " % % % // Input Channels the FFT is performed on",
   "Filtering float FFTWindowLength= 0.5s"
      " 0.5s % % // Time window for spectrum computation",
   "Filtering int FFTWindow= 3"
     " 0 0 3 // Type of Sidelobe Suppression Window "
              "1: Hamming, "
              "2: Hann, "
              "3: Blackman "
              "(enumeration)",
   "Visualize int VisualizeFFT= 0"
     " 0 0 1 // Visualize FFT Power Spectra (boolean)",
 END_PARAMETER_DEFINITIONS
}

FFTFilter::~FFTFilter()
{
}

void
FFTFilter::Preflight( const SignalProperties& Input, SignalProperties& Output ) const
{
  
  string FFTInputChannels = FlatParameter( "FFTInputChannels" );
  IndexList idx( FFTInputChannels, Input.ChannelLabels() );
  if( !idx.Errors().empty() )
    bcierr << "Invalid FFTInputChannels: " << FFTInputChannels << ": " << idx.Errors();
  else if( idx.Empty() && !Input.IsEmpty() )
    bciwarn << "FFTInputChannels \"" << FFTInputChannels << "\" does not match any channel";

  bool fftRequired = ( int( Parameter( "FFTOutputSignal" ) ) != eInput
                       || int( Parameter( "VisualizeFFT" ) ) )
                     && ( Parameter( "FFTInputChannels" )->NumValues() > 0 );
  if( fftRequired )
  {
    if( mFFT.LibAvailable() )
    {
      RealFFT preflightFFT;
      int fftWindowLength =
        static_cast<int>( Input.Elements() * Parameter( "FFTWindowLength" ).InSampleBlocks() );
      if( !preflightFFT.Initialize( fftWindowLength ) )
        bcierr << "Requested parameters are not supported by FFT library" << endl;
    }
    else
      bcierr << "The FFT Filter could not find the " << mFFT.LibName() << " library."
             << endl;
  }

  if( int( Parameter( "VisualizeFFT" ) ) || int( Parameter( "FFTOutputSignal" ) ) == ePower )
  {
    SignalProperties temp( Input );
    DetermineSignalProperties( temp, ePower );
  }

  Output = Input;
  DetermineSignalProperties( Output, Parameter( "FFTOutputSignal" ) );
}

void
FFTFilter::Initialize( const SignalProperties& Input, const SignalProperties& /*Output*/ )
{
  mFFTOutputSignal = ( eFFTOutputSignal )( int )Parameter( "FFTOutputSignal" );
  mFFTWindowLength = static_cast<int>( Input.Elements() * Parameter( "FFTWindowLength" ).InSampleBlocks() );
  mFFTWindow = ( eFFTWindow )( int )Parameter( "FFTWindow" );

  mFFTInputChannels.clear();

  for( size_t i = 0; i < mVisualizations.size(); ++i )
    mVisualizations[ i ].Send( CfgID::Visible, false );
  mVisualizations.clear();
  mVisualizeFFT = int( Parameter( "VisualizeFFT" ) );
  if( mVisualizeFFT || mFFTOutputSignal == ePower )
  {
    mVisProperties = Input;
    DetermineSignalProperties( mVisProperties, ePower );
    PhysicalUnit elementUnit = mVisProperties.ElementUnit();
    mVisProperties.SetChannels( mVisProperties.Elements() );
    for( int i = 0; i < mVisProperties.Channels(); ++i )
      mVisProperties.ChannelLabels()[i] = elementUnit.RawToPhysical( mVisProperties.Channels() - i - 1 );
    mVisProperties.SetElements( 1 );
    mVisProperties.ElementUnit() = Input.ElementUnit();
    mVisProperties.ElementUnit().SetGain( mVisProperties.ElementUnit().Gain() * Input.Elements() );
    mPowerSpectrum = GenericSignal( mVisProperties );
  }

  IndexList idx( FlatParameter( "FFTInputChannels" ), Input.ChannelLabels() );
  for( int i = 0; i < idx.Size(); ++i )
  {
    size_t ch = static_cast<size_t>( idx[i] );
    mFFTInputChannels.push_back( ch );
    if( mVisualizeFFT )
    {
      ostringstream oss_i;
      oss_i << i + 1;
      mVisualizations.push_back( GenericVisualization( string( "FFT" ) + oss_i.str() ) );

      ostringstream oss_ch;
      oss_ch << "FFT for Ch ";
      if( Input.ChannelLabels().IsTrivial() )
        oss_ch << i + 1;
      else
        oss_ch << Input.ChannelLabels()[ch];

      mVisualizations.back().Send( mVisProperties )
                            .Send( CfgID::WindowTitle, oss_ch.str().c_str() )
                            .Send( CfgID::GraphType, CfgID::Field2d )
                            .Send( CfgID::Visible, true );
    }
  }

  mWindow.clear();
  mWindow.resize( mFFTWindowLength, 1.0 );
  float phasePerSample = Pi<float>() / mFFTWindowLength;
  // Window coefficients: None Hamming Hann Blackman
  const float a1[] = {    0,   0.46f,  0.5f, 0.5f, },
              a2[] = {    0,   0,      0,    0.08f };

  for( int i = 0; i <= mFFTWindowLength / 2; ++i ) 
  {
    mWindow[ i ] = 1.0f - a1[ mFFTWindow ] - a2[ mFFTWindow ]
                   + a1[ mFFTWindow ] * cos( ( mFFTWindowLength - ( i * 2 ) ) * phasePerSample )
                   + a2[ mFFTWindow ] * cos( ( mFFTWindowLength - ( i * 2 ) ) * 2.0f * phasePerSample );
    mWindow[ mFFTWindowLength - i - 1 ] = mWindow[ i ];
  }

  mValueBuffers.resize( mFFTInputChannels.size() );
  ResetValueBuffers( mFFTWindowLength );

  bool fftRequired = ( mVisualizeFFT || mFFTOutputSignal != eInput)
                     && mFFTInputChannels.size() > 0;
  if( !fftRequired )
    mFFTInputChannels.clear();
  else
    mFFT.Initialize( mFFTWindowLength );
}

void
FFTFilter::Process( const GenericSignal& Input, GenericSignal& Output )
{
  if( mFFTOutputSignal == eInput )
    Output = Input;

  for( size_t i = 0; i < mFFTInputChannels.size(); ++i )
  {
    // Copy input signal values to the value buffer.
    vector<float>& buffer = mValueBuffers[ i ];
    int inputSize = Input.Elements(),
        bufferSize = static_cast<int>( buffer.size() );
    // Move old values towards the beginning of the buffer, if any.
    for( int j = 0; j < bufferSize - inputSize; ++j )
      buffer[ j ] = buffer[ j + inputSize ];
    // Copy new values to the end of the buffer;
    // buffer size may be greater or less than input size.
    for( int j = ::max( 0, bufferSize - inputSize ); j < bufferSize; ++j )
      buffer[ j ] = static_cast<float>( Input( mFFTInputChannels[ i ], j + inputSize - bufferSize ) );
    // Prepare the buffer.
    if( mFFTWindow == eNone )
      for( int j = 0; j < bufferSize; ++j )
        mFFT.Input( j ) = buffer[ j ];
    else
      for( int j = 0; j < bufferSize; ++j )
        mFFT.Input( j ) = buffer[ j ] * mWindow[ j ];
    // Compute the power spectrum and visualize it if requested.
    mFFT.Compute();
    if( mVisualizeFFT || mFFTOutputSignal == ePower )
    {
      int maxIdx = mPowerSpectrum.Channels() - 1;
      double normFactor = 1.0 / bufferSize;
      mPowerSpectrum( maxIdx, 0 ) = mFFT.Output( 0 ) * mFFT.Output( 0 ) * normFactor;
      for( int k = 1; k < ( bufferSize + 1 ) / 2; ++k )
        mPowerSpectrum( maxIdx - k, 0 ) = (
          mFFT.Output( k ) * mFFT.Output( k ) +
          mFFT.Output( bufferSize - k ) * mFFT.Output( bufferSize - k ) ) * normFactor;
      if( bufferSize % 2 == 0 )
        mPowerSpectrum( maxIdx - bufferSize / 2, 0 )
          = mFFT.Output( bufferSize / 2 ) * mFFT.Output( bufferSize / 2 ) * normFactor;
    }
    if( mVisualizeFFT )
      mVisualizations[ i ].Send( mPowerSpectrum );

    if( mFFTOutputSignal == ePower )
    {
      for( int j = 0; j < Output.Elements(); ++j )
        Output( i, j ) = mPowerSpectrum( mPowerSpectrum.Channels() - 1 - j, 0 );
    }
    else if( mFFTOutputSignal == eHalfcomplex )
    {
      double normFactor = 1.0 / ::sqrt( 1.0 * bufferSize );
      for( int j = 0; j < Output.Elements(); ++j )
        Output( i, j ) = mFFT.Output( j ) * normFactor;
    }
  }
}

void
FFTFilter::StopRun()
{
  ResetValueBuffers( mFFTWindowLength );
}


void
FFTFilter::ResetValueBuffers( size_t inSize )
{
  // Clear all the value buffers so old data won't enter into the transform
  // on the next call to Process().
  for( size_t i = 0; i < mValueBuffers.size(); ++i )
  {
    mValueBuffers[ i ].clear();
    mValueBuffers[ i ].resize( inSize, 0 );
  }
}


void
FFTFilter::DetermineSignalProperties( SignalProperties& ioProperties, int inFFTType ) const
{
  IndexList idx( FlatParameter( "FFTInputChannels" ), ioProperties.ChannelLabels() );
  int numChannels = idx.Size(),
      fftWindowLength = static_cast<int>( ioProperties.Elements() * Parameter( "FFTWindowLength" ).InSampleBlocks() );
  if( numChannels > 0 && fftWindowLength == 0 )
    bcierr << "FFTWindowLength must exceed a single sample's duration" << endl;
  double freqRange = ioProperties.SamplingRate() / 2.0;

  switch( inFFTType )
  {
    case eInput:
      break;

    case ePower:
    {
      ioProperties.SetName( "FFT Power Spectrum" )
                  .SetChannels( numChannels )
                  .SetElements( fftWindowLength / 2 + 1 );
      ioProperties.ElementUnit().SetOffset( 0.0 )
                                .SetGain( freqRange / ( ioProperties.Elements() - 1 ) )
                                .SetSymbol( "Hz" );
      double amplitude = ioProperties.ValueUnit().RawMax() - ioProperties.ValueUnit().RawMin();
      ioProperties.ValueUnit().SetRawMin( 0 )
                              .SetRawMax( amplitude * amplitude );
      ioProperties.ElementUnit().SetRawMin( 0 )
                                .SetRawMax( ioProperties.Elements() - 1 );
    } break;

    case eHalfcomplex:
      ioProperties.SetName( "FFT Coefficients" )
                  .SetChannels( numChannels )
                  .SetElements( fftWindowLength );
      ioProperties.ElementUnit().SetRawMin( 0 )
                                .SetRawMax( fftWindowLength - 1 )
                                .SetOffset( 0 )
                                .SetGain( 1 )
                                .SetSymbol( "" );
      break;

    default:
      throw std_logic_error << "Unknown value of FFT type";
  }
}

string
FFTFilter::FlatParameter( const string& inName ) const
{
  ParamRef p = Parameter( inName );
  string flat = p( 0 );
  for( int i = 1; i < p->NumValues(); ++i )
    flat += " " + string( p( i ) );
  return flat;
}