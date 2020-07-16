////////////////////////////////////////////////////////////////////////////////
//  $Id: LPFilter.cpp 4981 2015-08-26 17:04:38Z mellinger $
//  Author:      juergen.mellinger@uni-tuebingen.de
//  Description: A (working) tutorial low pass filter demonstrating
//               parameter access, visualization, and unit conversion.
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
#include "LPFilter.h"

#include "BCIStream.h"
#include "defines.h"
#include <vector>
#include <cmath>
#include <limits>

using namespace std;

RegisterFilter( LPFilter, 2.B1 );

LPFilter::LPFilter()
: mDecayFactor( 0 ),
  mPreviousOutput( 0 )
{
  BEGIN_PARAMETER_DEFINITIONS

    "Filtering float LPTimeConstant= 0 16s 0 % "
      "// time constant for the low pass filter",

  END_PARAMETER_DEFINITIONS
}


LPFilter::~LPFilter()
{
}


void
LPFilter::Preflight( const SignalProperties& Input,
                           SignalProperties& Output ) const
{
  double LPTimeConstant = Parameter( "LPTimeConstant" ).InSampleBlocks();
  LPTimeConstant *= Input.Elements();
  // The PreflightCondition macro will automatically generate an error
  // message if its argument evaluates to false.
  // However, we need to make sure that its argument is user-readable
  // -- this is why we chose a variable name that matches the parameter
  // name.
  PreflightCondition( LPTimeConstant >= 0 );
  // Alternatively, we might write:
  if( LPTimeConstant < 0 )
    bcierr << "The LPTimeConstant parameter must be 0 or greater" << endl;

  // Request output signal properties:
  Output = Input;
}


void
LPFilter::Initialize( const SignalProperties& Input,
                      const SignalProperties& /*Output*/ )
{
  // Get the time constant in units of a sample block's duration:
  double timeConstant = Parameter( "LPTimeConstant" ).InSampleBlocks();
  // Convert it into units of a sample's duration:
  timeConstant *= Input.Elements();

  if( timeConstant < numeric_limits<double>::epsilon() )
    mDecayFactor = 0.0;
  else
    mDecayFactor = ::exp( -1.0 / timeConstant );

  // This will initialize elements with 0,
  // implementing the first line of the filter prescription:
  mPreviousOutput.clear();
  mPreviousOutput.resize( Input.Channels(), 0 );
}


void
LPFilter::Process( const GenericSignal& Input, GenericSignal& Output )
{
  // This implements the second line for all channels:
  for( int channel = 0; channel < Input.Channels(); ++channel )
    for( int sample = 0; sample < Input.Elements(); ++sample )
    {
      mPreviousOutput[ channel ] *= mDecayFactor;
      mPreviousOutput[ channel ] += Input( channel, sample ) * ( 1.0 - mDecayFactor );
      Output( channel, sample ) = mPreviousOutput[ channel ];
    }
}

