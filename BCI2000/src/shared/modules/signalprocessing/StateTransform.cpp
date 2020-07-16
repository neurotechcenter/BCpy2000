////////////////////////////////////////////////////////////////////////////////
// $Id: StateTransform.cpp 3798 2012-02-01 18:07:06Z mellinger $
// Author:      juergen.mellinger@uni-tuebingen.de
// Description: A filter that transforms state values according to rules.
//              Whenever a given state's value changes, it replaces the new
//              value by a user-defined expression.
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

#include "StateTransform.h"

using namespace std;

RegisterFilter( StateTransform, 4 );


StateTransform::StateTransform()
{
 BEGIN_PARAMETER_DEFINITIONS
  "StateTransform matrix StateTransforms= 1 2 ResultCode (ResultCode>0)*(1+(Signal(0,0)>0)) % % % "
    "// Replace the state from the first column with the expression from the second",
 END_PARAMETER_DEFINITIONS
}


StateTransform::~StateTransform()
{
}


void
StateTransform::Preflight( const SignalProperties& Input,
                                 SignalProperties& Output ) const
{
  GenericSignal preflightSignal( Input );
  ParamRef StateTransforms = Parameter( "StateTransforms" );
  for( int i = 0; i < StateTransforms->NumRows(); ++i )
  {
    State( string( StateTransforms( i, 0 ) ) );
    Expression( StateTransforms( i, 1 ) ).Evaluate( &preflightSignal );
  }
  // Request output signal properties:
  Output = Input;
}


void
StateTransform::Initialize( const SignalProperties& Input,
                            const SignalProperties& Output )
{
  mStateNames.clear();
  mExpressions.clear();
  mPreviousInputStateValues.clear();
  mPreviousOutputStateValues.clear();

  ParamRef StateTransforms = Parameter( "StateTransforms" );
  for( int i = 0; i < StateTransforms->NumRows(); ++i )
  {
    mStateNames.push_back( StateTransforms( i, 0 ) );
    mExpressions.push_back( Expression( StateTransforms( i, 1 ) ) );
  }
  mPreviousInputStateValues.resize( mStateNames.size(), -1 );
  mPreviousOutputStateValues.resize( mStateNames.size(), -1 );
}


void
StateTransform::Process( const GenericSignal& Input,
                               GenericSignal& Output )
{
  for( size_t i = 0; i < mStateNames.size(); ++i )
  {
    int currentInputStateValue = State( mStateNames[ i ].c_str() );
    if( currentInputStateValue != mPreviousInputStateValues[ i ] )
      mPreviousOutputStateValues[ i ] = static_cast<int>( mExpressions[ i ].Evaluate( &Input ) );
    State( mStateNames[ i ].c_str() ) = mPreviousOutputStateValues[ i ];
    mPreviousInputStateValues[ i ] = currentInputStateValue;
  }
  Output = Input;
}
