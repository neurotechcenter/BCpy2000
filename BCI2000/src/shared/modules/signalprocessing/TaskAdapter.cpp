////////////////////////////////////////////////////////////////////////////////
// $Id: TaskAdapter.cpp 3798 2012-02-01 18:07:06Z mellinger $
// Author:      juergen.mellinger@uni-tuebingen.de
// Description: A filter that takes a unit variance, zero mean signal and
//              scales it according to a desired minimum trial duration, and
//              system update rate.
//              The purpose of this filter is to connect old-style application
//              modules to the new normalizer filter without changing their
//              behavior.
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
#include "PCHIncludes.h" // Make the compiler's Pre-Compiled Headers feature happy
#pragma hdrstop

#include "TaskAdapter.h"

using namespace std;

RegisterFilter( TaskAdapter, 2.99999 ); // Place it immediately before the task filter.

TaskAdapter::TaskAdapter()
{
 BEGIN_PARAMETER_DEFINITIONS
  // MinFeedbackDuration is a more intuitive replacement for the X/YPixelsPerSec parameters.
  "UsrTask float MinFeedbackDuration= 3s 0 0 0 "
    "// Time for the cursor to cross the task display for a unit control signal",
 END_PARAMETER_DEFINITIONS
}


TaskAdapter::~TaskAdapter()
{
}


void
TaskAdapter::Preflight( const SignalProperties& input,
                              SignalProperties& output ) const
{
  double MinFeedbackDuration = Parameter( "MinFeedbackDuration" ).InSampleBlocks();
  PreflightCondition( MinFeedbackDuration > 0 );
  // Request output signal properties:
  output = input;
}


void
TaskAdapter::Initialize( const SignalProperties& input,
                         const SignalProperties& output )
{
  mGains.clear();
  mGains.resize( ::max( 2, input.Channels() ), 1.0 );

  double MinFeedbackDuration = Parameter( "MinFeedbackDuration" ).InSampleBlocks();
  mGains[ 0 ] = double( 0x7fff ) / MinFeedbackDuration;
  mGains[ 1 ] = mGains[ 0 ];
}


void
TaskAdapter::Process( const GenericSignal& input, GenericSignal& output )
{
  for( int channel = 0; channel < input.Channels(); ++channel )
    for( int sample = 0; sample < input.Elements(); ++sample )
      output( channel, sample ) = input( channel, sample ) * mGains[ channel ];
}


