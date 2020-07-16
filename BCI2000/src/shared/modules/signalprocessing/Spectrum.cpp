////////////////////////////////////////////////////////////////////////////////
// $Id: Spectrum.cpp 4826 2015-04-29 00:40:23Z mellinger $
// Authors: juergen.mellinger@uni-tuebingen.de
// Description: A base class for spectral estimator threads that centralizes
//   common parameters, and Preflight() functionality.
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
#include "Spectrum.h"
#include <limits>

using namespace std;

void
Spectrum::Publish() const
{
 BEGIN_PARAMETER_DEFINITIONS
  "Filtering:Spectral%20Estimation float FirstBinCenter= 0Hz 0Hz % % "
      "// Center of first frequency bin (in Hz)",
  "Filtering:Spectral%20Estimation float LastBinCenter= 30Hz 30Hz % % "
      "// Center of last frequency bin (in Hz)",
  "Filtering:Spectral%20Estimation float BinWidth= 3Hz 3Hz % % "
      "// Width of spectral bins (in Hz)",
  "Filtering:Spectral%20Estimation int OutputType= 0 0 0 2 "
      "// 0: Spectral Amplitude,"
        " 1: Spectral Power,"
        " 2: Coefficients"
        " (enumeration)",
 END_PARAMETER_DEFINITIONS
}

int
Spectrum::NumberOfBins( const SignalProperties& Input ) const
{
  double firstBinCenter = Parameter( "FirstBinCenter" ).InHertz() / Input.SamplingRate(),
          lastBinCenter = Parameter( "LastBinCenter" ).InHertz() / Input.SamplingRate(),
          binWidth = Parameter( "BinWidth" ).InHertz() / Input.SamplingRate();
  if( firstBinCenter > 0.5 || firstBinCenter < 0
      || lastBinCenter > 0.5 || lastBinCenter < 0 )
    bcierr << "FirstBinCenter and LastBinCenter must be greater zero and"
            << " less than half the sampling rate"
            << endl;
  if( firstBinCenter > lastBinCenter )
    bcierr << "FirstBinCenter cannot be greater than LastBinCenter" << endl;
  int numBins = 0;
  if( binWidth <= 0 )
    bcierr << "BinWidth must be greater zero" << endl;
  else
    numBins = Floor( ( lastBinCenter - firstBinCenter + Epsilon( firstBinCenter ) ) / binWidth + 1 );
  return numBins;
}

void
Spectrum::Preflight( const SignalProperties& Input,
                           SignalProperties& Output ) const
{
  Output = Input;
  if( Input.SamplingRate() < Epsilon( Input.SamplingRate() ) )
  {
    bcierr << "Zero input sampling rate" << endl;
  }
  else
  {
    Output.SetElements( NumberOfBins( Input ) );
    Output.ElementUnit().SetOffset( -Parameter( "FirstBinCenter" ).InHertz() / Parameter( "BinWidth" ).InHertz() )
                        .SetGain( Parameter( "BinWidth" ).InHertz() )
                        .SetSymbol( "Hz" );
    Output.ValueUnit().SetRawMin( 0 );
    double inputAmplitude = ( Input.ValueUnit().RawMax() - Input.ValueUnit().RawMin() ) / 2,
           expectedPower = inputAmplitude * inputAmplitude / 3; // Constant spectral power of uniform white noise with input amplitude.
    expectedPower *= 10; // Account for a peaked spectrum.
    switch( int( Parameter( "OutputType" ) ) )
    {
      case SpectralAmplitude:
        Output.ValueUnit().SetOffset( 0 )
                          .SetGain( 1 )
                          .SetSymbol( "muV" )
                          .SetRawMax( ::sqrt( expectedPower ) );
        Output.SetName( "Amplitude Spectrum" );
        break;

      case SpectralPower:
        Output.ValueUnit().SetOffset( 0 )
                          .SetGain( 1 )
                          .SetSymbol( "muV^2" )
                          .SetRawMax( expectedPower );
        Output.SetName( "Power Spectrum" );
        break;

      case Coefficients:
        Output.ValueUnit().SetOffset( 0 )
                          .SetGain( 1 )
                          .SetSymbol( "muV" )
                          .SetRawMax( ::sqrt( expectedPower / 2 ) )
                          .SetRawMin( -::sqrt( expectedPower / 2 ) );
        Output.SetName( "Coefficients" );
        break;
    }
  }
  Output.ElementUnit().SetRawMin( 0 )
                      .SetRawMax( Output.Elements() - 1 );
}
