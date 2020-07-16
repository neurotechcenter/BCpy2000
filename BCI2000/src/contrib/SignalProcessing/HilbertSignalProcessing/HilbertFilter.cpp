////////////////////////////////////////////////////////////////////////////////
// $Id: HilbertFilter.cpp 4982 2015-08-26 20:01:03Z mellinger $
// Authors: Cristhian Potes - Jeremy Hill
// Description: HilbertFilter implementation
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
#include "HilbertFilter.h"
#include "BCIStream.h"
#include <numeric>
#include <math.h>

using namespace std;


RegisterFilter( HilbertFilter, 2.C3 );

HilbertFilter::HilbertFilter()
{
	BEGIN_PARAMETER_DEFINITIONS
   "Filtering:Analytic%20Signal int OutputSignal= 1"
     " 1 0 4 // Signal provided to the next filter: "
              "0: Input Signal, "
              "1: Hilbert envelope "
              "2: Hilbert phase "
			  "3: Real part - delayed original signal "
			  "4: Imaginary part - Hilbert transform "
              "(enumeration)",

   "Filtering:Analytic%20Signal float Delay= 0.1s"
      " 0.1s % % // Delay for convolution with FIR Hilbert transformer",
  
 END_PARAMETER_DEFINITIONS

}


HilbertFilter::~HilbertFilter()
{
}


void
HilbertFilter::Preflight( const SignalProperties& Input, SignalProperties& Output ) const
{
	int offset = static_cast<int>( 0.5 + Input.Elements() * (double)Parameter( "Delay" ).InSampleBlocks() );
	int minOffset = 100; // corresponding to mFilterLength = 201
	if ( offset < minOffset ) 
		bciwarn << "Delay translates to " << offset << "samples, but for good estimation of "
			"the magnitude or phase, it should be at least " << minOffset << " samples" << endl;

    switch( ( int )Parameter( "OutputSignal" ) )
	{
	  case eInput:
	  case eMagnitude:
	  case ePhase:
	  case eRealPart:
	  case eImaginaryPart:
		  break;
	  default:
		  bcierr << "Unsupported OutputSignal value" << endl;
	}

	Output = Input; // this simply passes information through about SampleBlock dimensions, etc....  
}


void
HilbertFilter::Initialize( const SignalProperties& Input, const SignalProperties& Output )
{
    mBuffer.clear();
	int numChannels = Input.Channels();
	int offset =  static_cast<int>( 0.5 + Input.Elements() * Parameter( "Delay" ).InSampleBlocks() );
	mFilterLength = 2*offset+1;
	
	mFilter.resize( mFilterLength, 0.0 );
	for( int sample = 1; sample < mFilterLength; sample+=2 )
		mFilter[sample] = ( sample==offset ? 0.0 : 2.0/(Pi()*double(sample-offset)) ); 
	
	int bufferLength = mFilterLength + Input.Elements() - 1;
	mBuffer.resize( numChannels, DataVector( 0.0, bufferLength ) );
	
    mOutputSignal = ( eOutputSignal )( int )Parameter( "OutputSignal" );
}

void
HilbertFilter::Process( const GenericSignal& Input, GenericSignal& Output )
{

  if( mBuffer.empty() )
    Output = Input;
  else for( size_t channel = 0; channel < mBuffer.size(); ++channel )
  {
    int bufferLength = static_cast<int>( mBuffer[channel].size() ),
		offset  = (mFilterLength-1)/2, 
        inputLength = Input.Elements();

    // Move buffer content towards the buffer's begin.
    for( int sample = 0; sample < bufferLength - inputLength; ++sample )
		mBuffer[channel][sample] = mBuffer[channel][sample + inputLength];
    // Copy current input into the buffer's end.
    for( int sample = 0; sample < inputLength; ++sample )
		mBuffer[channel][bufferLength - inputLength + sample] = Input( channel, sample );
    // Compute buffer's convolution with coefficient vector.
    
	// Compute output (either magnitude or phase).
	if (mOutputSignal == eInput)
		Output = Input;

	else for (int sample = 0; sample < inputLength; ++sample)
	{
		double real;
		double imag = inner_product( &mFilter[0], &mFilter[mFilterLength], &mBuffer[channel][sample], 0.0 );

		real = mBuffer[channel][ mFilterLength + sample - 1 - offset];

		if ( mOutputSignal == eMagnitude )
			Output( channel, sample ) = sqrt( real*real + imag*imag  );
		if ( mOutputSignal == ePhase)
			Output( channel, sample ) = -atan2(imag, real);
		if ( mOutputSignal == eRealPart )
			Output( channel, sample ) = real;
		if ( mOutputSignal == eImaginaryPart )
			Output( channel, sample ) = imag;
	}

  }
}