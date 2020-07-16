////////////////////////////////////////////////////////////////////////////////
//  $Id: RandomFilter.cpp 4981 2015-08-26 17:04:38Z mellinger $
//  Author:      juergen.mellinger@uni-tuebingen.de
//  Description: A filter that returns zero-mean white noise multiplied by the
//               input signal's value.
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
#include "RandomFilter.h"
#include "BCIStream.h"
#include <cmath>
#include <cstdlib>

using namespace std;

RegisterFilter( RandomFilter, 2.C1 );

RandomFilter::RandomFilter()
: mRandomGenerator( this ),
  mpGetRandom( NULL )
{
  BEGIN_PARAMETER_DEFINITIONS
    "Filtering int RandomNoise= 0 0 0 1 // 0: none, 1: uniform (enumeration)",
  END_PARAMETER_DEFINITIONS
}


RandomFilter::~RandomFilter()
{
}


void
RandomFilter::Preflight( const SignalProperties& Input,
                               SignalProperties& Output ) const
{
  int outputChannels = Input.Channels();
  if( Parameter( "RandomNoise" ) > 0 )
    outputChannels *= 2;
  // Request output signal properties:
  Output = Input;
  Output.SetChannels( outputChannels );
}


void
RandomFilter::Initialize( const SignalProperties& Input,
                          const SignalProperties& Output )
{
  switch( int( Parameter( "RandomNoise" ) ) )
  {
    case none:
      break;

    case uniform:
      mpGetRandom = &RandomFilter::GetRandomUniform;
      break;
  }
}


void
RandomFilter::Process( const GenericSignal& Input,
                             GenericSignal& Output )
{
  for( int channel = 0; channel < Input.Channels(); ++channel )
    for( int sample = 0; sample < Input.Elements(); ++sample )
    {
      Output( channel, sample ) = Input( channel, sample );
      if( mpGetRandom )
        Output( channel + Input.Channels(), sample ) =
          Input( channel, sample ) * ( this->*mpGetRandom )();
    }
}


float
RandomFilter::GetRandomUniform()
{
  return ( mRandomGenerator.Random() * 2.0f / mRandomGenerator.RandMax() ) - 1.0f;
}

