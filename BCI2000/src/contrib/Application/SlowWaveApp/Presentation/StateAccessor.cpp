/////////////////////////////////////////////////////////////////////////////
//
// File: StateAccessor.cpp
//
// Date: Oct 25, 2001
//
// Author: Juergen Mellinger
//
// Description: A class that caches state information to avoid parsing
//              of state names during time critical tasks.
//              Most methods are defined inline in StateAccessor.h.
//
// Changes: Feb 02, 2004, jm: Introduced "optional state" concept to
//              to improve BCI2000 compatibility. Note that optional states
//              cannot be written to.
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
//////////////////////////////////////////////////////////////////////////////

#ifdef __BORLANDC__
#include "PCHIncludes.h"
#pragma hdrstop
#endif // __BORLANDC__

#include "StateAccessor.h"
#include "PresErrors.h"
#include "State.h"
#include "StateList.h"
#include "StateVector.h"

StateVector*    TStateAccessor::stateVector = NULL;

void
TStateAccessor::Initialize( StateVector *inStateVector )
{
    stateVector = inStateVector;
}

void
TStateAccessor::AttachState( const char *inStateName )
{
    mName = inStateName;
    mStateAvailable = stateVector->StateList().Exists( mName );
    if( !mStateAvailable )
      gPresErrors << "State '" << inStateName << "' inaccessible."
                  << std::endl;
}

void
TStateAccessor::AttachOptionalState( const char *inStateName, unsigned short inDefaultValue )
{
    mName = inStateName;
    mDefaultValue = inDefaultValue;
    mStateAvailable = stateVector->StateList().Exists( mName );
}

unsigned short
TStateAccessor::GetStateValue() const
{
    assert( stateVector != NULL );
    return mStateAvailable ?
            stateVector->StateValue( mName.c_str() ) :
            mDefaultValue;
}

void
TStateAccessor::SetStateValue( unsigned short inValue )
{
    assert( stateVector != NULL );
    assert( mStateAvailable );
    stateVector->SetStateValue( mName.c_str(), inValue );
}

