////////////////////////////////////////////////////////////////////////////////
//  $Id$
//  Author: juergen.mellinger@uni-tuebingen.de
//  Description: A filter that continuously estimates the dominant frequency of
//    its input signal, similar to a frequency counter.
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
#include "FrequencyEstimator.h"

#include "BCIStream.h"
#include "Numeric.h"
#include "Debugging.h"

using namespace std;

RegisterFilter( FrequencyEstimator, 2.B0.1 );

FrequencyEstimator::FrequencyEstimator()
: mBinVisualization( "FEBN" )
{
  BEGIN_PARAMETER_DEFINITIONS
    "Filtering int FEResolution= 1e-2 1e-2 % % "
      "// relative change between neighboring bins",
    "Filtering floatlist FERange= 2 0.5Hz 40Hz % % % "
      "// range of frequencies",
    "Filtering float FEVisualizeSpectrum= 0 0 0 1 "
      "// visualize power spectrum (boolean)",
  END_PARAMETER_DEFINITIONS
}


FrequencyEstimator::~FrequencyEstimator()
{
}


void
FrequencyEstimator::Preflight( const SignalProperties& Input,
                                     SignalProperties& Output ) const
{
  PreflightCondition( Parameter( "FEResolution" ) > 0 );
  PreflightCondition( Parameter( "FERange" )( 0 ).InHertz() > 0 );
  PreflightCondition( Parameter( "FERange" )( 1 ).InHertz() > 0 );

  Data data;
  Configure( Input, data );
  Output = Input;
  Output.ValueUnit() = data.Power.Properties().ElementUnit();
}


void
FrequencyEstimator::Initialize( const SignalProperties& Input,
                               const SignalProperties& /*Output*/ )
{
  Configure( Input, mData );
  mBuffer.resize( mData.Power.Elements() );

  if( mVisualizeBins = ( Parameter( "FEVisualizeSpectrum" ) > 0 ) )
    mBinVisualization.Send( mData.Power.Properties() ).Send( mData.Power );
  mBinVisualization.Send( CfgID::Visible, mVisualizeBins );
}


void
FrequencyEstimator::Process( const GenericSignal& Input, GenericSignal& Output )
{
  for( int ch = 0; ch < Input.Channels(); ++ch )
  {
    for( int el = 0; el < Input.Elements(); ++el )
    {
      mData.Bins[ch] *= mData.TimeshiftOperator;
      mData.Bins[ch] += Input( ch, el );
      for( int bin = 0; bin < mData.Power.Elements(); ++bin )
        mData.Power( ch, bin ) = norm( mData.Bins[ch][bin] ) * mData.Frequencies[bin];
      Output( ch, el ) = GetMaxPosition( ch );
    }
  }
  if( mVisualizeBins )
    mBinVisualization.Send( mData.Power );
}

void
FrequencyEstimator::Configure( const SignalProperties& Input, Data& outData ) const
{
  double f1 = Parameter( "FERange" )( 0 ).InHertz() / Input.SamplingRate(),
         f2 = Parameter( "FERange" )( 1 ).InHertz() / Input.SamplingRate();
  f1 = min( 0.5, f1 );
  f2 = min( 0.5, f2 );
  double binMin = ::log( min( f1, f2 ) ),
         binMax = ::log( max( f1, f2 ) ),
         resolution = Parameter( "FEResolution" );
  size_t numBins = Ceil( ( binMax - binMin ) / resolution ) + 1;

  SignalProperties s = Input;
  s.SetName( "FE Power Spectrum" );
  s.SetElements( numBins );
  PhysicalUnit u;
  u.SetRawMin( 0 ).SetRawMax( numBins - 1 )
   .SetSymbol( "" ).SetGain( resolution ).SetOffset( 0 );
  u.SetOffset( -u.PhysicalToRawValue( binMin ) );
  s.ElementUnit() = u;

  outData.Frequencies.resize( numBins );
  outData.TimeshiftOperator.resize( numBins );
  Real h = Parameter( "FEResolution" );
  Complex dp = Complex( h, 2 * Pi() );
  for( size_t i = 0; i < numBins; ++i )
  {
    double f = ::exp( s.ElementUnit().RawToPhysicalValue( i ) );
    outData.Frequencies[i] = f;
    outData.TimeshiftOperator[i] = exp( -f * dp );
  }

  s.ValueUnit() = Input.ValueUnit() * Input.ValueUnit();
  s.ValueUnit().SetRawMin( 0 );
  s.ValueUnit().SetRawMax( s.ValueUnit().RawMax() * h );
  outData.Power = GenericSignal( s, NaN<GenericSignal::ValueType>() );
  outData.Bins.resize( 0 );
  outData.Bins.resize( Input.Channels(), ComplexVector( numBins ) );
}


double
FrequencyEstimator::GetMaxPosition( int ch ) const
{
  const GenericSignal& s = mData.Power;
  int idx = 0;
  double max = -Inf( max );
  for( int i = 0; i < s.Elements(); ++i )
    if( s( ch, i ) > max )
      max = s( ch, i ), idx = i;

  double sum = idx * s( ch, idx ),
         mass = s( ch, idx );
  if( idx > 0 )
  {
    sum += ( idx-1 ) * s( ch, idx-1 );
    mass += s( ch, idx-1 );
  }
  if( idx < s.Elements() - 1 )
  {
    sum += ( idx+1 ) * s( ch, idx+1 );
    mass += s( ch, idx+1 );
  }
  return sum / mass;
}

