////////////////////////////////////////////////////////////////////////////////
// $Id: DiffFilter.cpp 4982 2015-08-26 20:01:03Z mellinger $
// Authors: Jeremy Hill <jezhill@gmail.com>
// Description: DiffFilter implementation
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
#include "DiffFilter.h"
#include "BCIStream.h"
#include <cmath>


using namespace std;


RegisterFilter( DiffFilter, 2.C4 );

DiffFilter::DiffFilter()
{
	
 BEGIN_PARAMETER_DEFINITIONS

   "Filtering:Derivative int   DiffOrder= 0 0 0 % // Order of derivative (0 disables)",
   "Filtering:Derivative float UnwrapDiff= 0 0 0 % // Unwrapping period (NB: 2*pi=6.283185307179586232;  0 disables)",
   "Filtering:Derivative int   DivideByDeltaTime= 1 1 0 1 // 0:No - return raw numeric diffs, 1:Yes - return derivative  (enumeration)",

 END_PARAMETER_DEFINITIONS
  
}

DiffFilter::~DiffFilter()
{
  Halt();
}

void
DiffFilter::Halt()
{
  // De-allocate any memory reserved in Initialize, stop any threads, etc.
  // Good practice is to write the Halt() method such that it is safe to call it even *before*
  // Initialize, and safe to call it twice (e.g. make sure you do not delete [] pointers that
  // have already been deleted:  set them to NULL after deletion).
}

void
DiffFilter::Preflight( const SignalProperties& Input, SignalProperties& Output ) const
{
  Parameter( "DiffOrder" );
  Parameter( "UnwrapDiff" );
  Parameter( "DivideByDeltaTime" );
  Output = Input; // this simply passes information through about SampleBlock dimensions, etc....
}


void
DiffFilter::Initialize( const SignalProperties& Input, const SignalProperties& Output )
{
  mOrder = Parameter( "DiffOrder" );
  mUnwrap = Parameter( "UnwrapDiff" );
  if( Parameter( "DivideByDeltaTime" ) == 1 )
    mDenom = MeasurementUnits::TimeInSeconds("1") / (double)Input.Elements();
  else
    mDenom = 1.0;
  SignalProperties prev = Input;
  prev.SetElements( mOrder );
  mPrevious.SetProperties( prev );
}

void
DiffFilter::StartRun()
{
  mFirstBlock = true;
}


void
DiffFilter::Process( const GenericSignal& Input, GenericSignal& Output )
{
  Output = Input;
  GenericSignal prev = mPrevious;
  for( int order = mOrder; order > 0; order-- )
  {
    for( int channel = 0; channel < Output.Channels(); channel++ )
    {
      for( int sample = Output.Elements()-1; sample >= 1-order; sample-- )
      {
        double second = ( sample >= 0 ) ? Output( channel, sample )     : mFirstBlock ? 0.0 : prev( channel, sample + prev.Elements() );
        double first  = ( sample >= 1 ) ? Output( channel, sample - 1 ) : mFirstBlock ? 0.0 : prev( channel, sample + prev.Elements() - 1 );
		double delta;
		
		if( mFirstBlock && sample <= mOrder )
          delta = 0.0;
        else if( mUnwrap && order == mOrder )
        {
          delta = fmod(second, mUnwrap) - fmod(first, mUnwrap);
          if( delta < -mUnwrap/2.0 ) delta += mUnwrap;
          if( delta > mUnwrap/2.0 )  delta -= mUnwrap;
        }
        else
          delta = second - first;
        if( sample >= 0 )
          Output( channel, sample ) = delta / mDenom;
        else if (!mFirstBlock)
          prev( channel, sample + prev.Elements() ) = delta / mDenom;
      }
    }
  }
  mFirstBlock = false;
  for( int channel = 0; channel < Output.Channels(); channel++ )
  {
    for( int sample = 0; sample < mPrevious.Elements() - Input.Elements(); sample++ )
      mPrevious( channel, sample ) = mPrevious( channel, sample + Input.Elements() );
    for( int sample = 0; sample < Input.Elements() && sample < mPrevious.Elements(); sample++ )
      mPrevious( channel, mPrevious.Elements() - sample - 1 ) = Input( channel, Input.Elements() - sample - 1 );
  }
}

void
DiffFilter::StopRun()
{
  
}

