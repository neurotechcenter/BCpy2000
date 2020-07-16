////////////////////////////////////////////////////////////////////////////////
// $Id: ARSpectrum.cpp 5612 2017-01-24 18:02:32Z mellinger $
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
#include "ARSpectrum.h"
#include "Exception.h"

using namespace std;

void
ARSpectrum::Publish()
{
  Super::Publish();
  BEGIN_PARAMETER_DEFINITIONS
   "Filtering:AR%20Spectral%20Estimator int ModelOrder= 16 16 0 % "
      "// AR model order",
   "Filtering:AR%20Spectral%20Estimator int EvaluationsPerBin= 15 15 1 % "
      "// Number of uniformly spaced evaluation points entering into a single bin value",
  END_PARAMETER_DEFINITIONS
 }

void
ARSpectrum::Preflight( const SignalProperties& Input,
                             SignalProperties& Output ) const
{
  if( Input.Elements() < Parameter( "ModelOrder" ) )
    bcierr << "WindowLength parameter must be large enough"
           << " for the number of samples to exceed the model order"
           << endl;

  if( Parameter( "OutputType" ) == Coefficients )
  {
    Output = Input;
    Output.SetName( "AR Coefficients" );
    Output.SetElements( Parameter( "ModelOrder" ) );
    Output.ElementUnit().SetSymbol( "" )
                        .SetOffset( 0 )
                        .SetGain( 1 )
                        .SetRawMin( 0 )
                        .SetRawMax( Output.Elements() - 1 );
  }
  else
  {
    Super::Preflight( Input, Output );
    Output.SetName( "AR " + Output.Name() );
  }
}

void
ARSpectrum::Initialize( const SignalProperties& Input, const SignalProperties& Output )
{
  mInputs.clear();
  mInputs.resize( Input.Channels(), DataVector( Input.Elements() ) );

  mPredictors.clear();
  MEMPredictor<Real> m;
  m.SetModelOrder( Parameter( "ModelOrder" ) );
  mPredictors.resize( Input.Channels(), m );

  mSpectra.clear();
  mSpectra.resize( Input.Channels(), DataVector( Output.Elements() ) );

  mTransferSpectra.clear();
  mOutputType = Parameter( "OutputType" );
  if( mOutputType != Coefficients )
  {
    TransferSpectrum<Real> s;
    s.SetFirstBinCenter( Parameter( "FirstBinCenter" ).InHertz() / Input.SamplingRate() );
    s.SetBinWidth( Parameter( "BinWidth" ).InHertz() / Input.SamplingRate() );
    s.SetNumBins( Output.Elements() );
    s.SetEvaluationsPerBin( Parameter( "EvaluationsPerBin" ) );
    mTransferSpectra.resize( Input.Channels(), s );
    mSpectra.resize( Input.Channels(), DataVector( s.NumBins() ) );
  }
}

void
ARSpectrum::Process( const GenericSignal& Input, GenericSignal& Output )
{
  Output.EnsureDeepCopy();
  #pragma omp parallel for
  for( int ch = 0; ch < Input.Channels(); ++ch )
  {
    for( size_t i = 0; i < mInputs[ch].size(); ++i )
      mInputs[ch][i] = Input( ch, i );

    Ratpoly<Real> tf;
    mPredictors[ch].TransferFunction( mInputs[ch], tf );
    tf *= ::sqrt( 2.0 ); // Multiply power by a factor of 2 to account for positive and negative frequencies.
    switch( mOutputType )
    {
      case SpectralAmplitude:
      case SpectralPower:
        mTransferSpectra[ch].Evaluate( tf, mSpectra[ch] );
        if( mOutputType == SpectralAmplitude )
          for( size_t bin = 0; bin < mSpectra[ch].size(); ++bin )
            mSpectra[ch][bin] = ::sqrt( mSpectra[ch][bin] );
        for( size_t bin = 0; bin < mSpectra[ch].size(); ++bin )
          Output( ch, bin ) = mSpectra[ch][bin];
        break;

      case ARCoefficients:
      {
        const Polynomial<Real>::Vector& coeff = tf.Denominator().Coefficients();
        for( size_t i = 1; i < coeff.size(); ++i )
          Output( ch, i - 1 ) = coeff[i];
      } break;

      default:
        throw std_logic_error << "Unknown output type";
    }
  }
}

