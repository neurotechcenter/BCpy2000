////////////////////////////////////////////////////////////////////////////////
// $Id: ConditionalIntegrator.cpp 3798 2012-02-01 18:07:06Z mellinger $
// Author:      juergen.mellinger@uni-tuebingen.de
// Description: A filter that integrates its input signal while a given
//              expression evaluates to true.
//              This filter is intended for offline simulations of application
//              module behavior.
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

#include "ConditionalIntegrator.h"

using namespace std;

RegisterFilter( ConditionalIntegrator, 2.F ); // Place it last in signal processing.


ConditionalIntegrator::ConditionalIntegrator()
: mPreviousConditionValue( false )
{
 BEGIN_PARAMETER_DEFINITIONS
  "ConditionalIntegrator float IntegrationCondition= Feedback % % % "
    "// Perform integration while this condition is true",
 END_PARAMETER_DEFINITIONS
}


ConditionalIntegrator::~ConditionalIntegrator()
{
}


void
ConditionalIntegrator::Preflight( const SignalProperties& Input,
                                        SignalProperties& Output ) const
{
  GenericSignal preflightSignal( Input );
  Expression( Parameter( "IntegrationCondition" ) ).Evaluate( &preflightSignal );
  // Request output signal properties:
  Output = Input;
}


void
ConditionalIntegrator::Initialize( const SignalProperties& Input,
                                   const SignalProperties& Output )
{
  mSignal = GenericSignal( Input );
  mCondition = Expression( Parameter( "IntegrationCondition" ) );
  mPreviousConditionValue = false;
}


void
ConditionalIntegrator::Process( const GenericSignal& Input,
                                      GenericSignal& Output )
{
  bool currentConditionValue = mCondition.Evaluate( &Input );
  if( currentConditionValue )
    for( int channel = 0; channel < Input.Channels(); ++channel )
      for( int sample = 0; sample < Input.Elements(); ++sample )
        mSignal( channel, sample ) += Input( channel, sample );
  if( currentConditionValue && !mPreviousConditionValue )
    for( int channel = 0; channel < Input.Channels(); ++channel )
      for( int sample = 0; sample < Input.Elements(); ++sample )
        mSignal( channel, sample ) = 0;
  mPreviousConditionValue = currentConditionValue;

  Output = mSignal;
}

