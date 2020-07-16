////////////////////////////////////////////////////////////////////////////////
// $Id: CustomFIRFilter.cpp 3896 2012-03-27 02:28:21Z jhill $
// Description: A finite impulse response (FIR) filter for temporal filtering.
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
#include "PCHIncludes.h"
#pragma hdrstop

#include "CustomFIRFilter.h"
#include <numeric>

using namespace std;

RegisterFilter( CustomFIRFilter, 2.C2 );

CustomFIRFilter::CustomFIRFilter()
{
  BEGIN_PARAMETER_DEFINITIONS
    "Filtering:FIR%20Filtering floatlist FIRCoefficients= 0 "
        " 1 % % // FIR Filter Coefficients",
  END_PARAMETER_DEFINITIONS
}


CustomFIRFilter::~CustomFIRFilter()
{
}


void
CustomFIRFilter::Preflight( const SignalProperties& Input,
                            SignalProperties& Output ) const
{
  Parameter( "FIRCoefficients" );
  // Output signal dimensions.
  Output = Input;
}


void
CustomFIRFilter::Initialize( const SignalProperties& Input, const SignalProperties& /*Output*/ )
{
  mBuffer.clear();

  ParamRef FIRCoefficients = Parameter( "FIRCoefficients" );
  int filterLength = FIRCoefficients->NumValues();
  mFilter.resize( filterLength, 0.0 );
  for( int sample = 0; sample < filterLength; ++sample )
    mFilter[sample] = FIRCoefficients( sample );

  int bufferLength = filterLength + Input.Elements() - 1;
  mBuffer.resize( Input.Channels(), DataVector( 0.0, bufferLength ) );
}


void
CustomFIRFilter::Process( const GenericSignal& Input, GenericSignal& Output )
{
  if( mBuffer.empty() || mFilter.size() == 0 )
    Output = Input;
  else for( size_t channel = 0; channel < mBuffer.size(); ++channel )
  {
    int bufferLength = static_cast<int>( mBuffer[channel].size() ),
        filterLength = static_cast<int>( mFilter.size() ),
        inputLength = Input.Elements();
    // Move buffer content towards the buffer's begin.
    for( int sample = 0; sample < bufferLength - inputLength; ++sample )
      mBuffer[channel][sample] = mBuffer[channel][sample + inputLength];
    // Copy current input into the buffer's end.
    for( int sample = 0; sample < inputLength; ++sample )
      mBuffer[channel][bufferLength - inputLength + sample] = Input( channel, sample );
    // Compute buffer's convolution with coefficient vector.
    DataVector result( 0.0, inputLength );
    for( int sample = 0; sample < inputLength; ++sample )
      result[sample] = inner_product( &mFilter[0], &mFilter[filterLength], &mBuffer[channel][sample], 0.0 );
    // Compute output.
    for( int sample = 0; sample < inputLength; ++sample )
      Output( channel, sample ) = result[sample];
  }
}

