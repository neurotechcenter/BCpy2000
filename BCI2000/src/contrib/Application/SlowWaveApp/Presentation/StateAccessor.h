/////////////////////////////////////////////////////////////////////////////
//
// File: StateAccessor.h
//
// Date: Oct 25, 2001
//
// Author: Juergen Mellinger
//
// Description: A class that caches state information to avoid parsing
//              of state names during time critical tasks.
//
// Changes: Feb 02, 2004, jm: Introduced "optional state" concept to
//              to improve BCI2000 compatibility.
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

#ifndef STATEACCESSORH
#define STATEACCESSORH

#include <cassert>

class StateVector;

class TStateAccessor
{
  public:
                    // Default constructor
                    TStateAccessor();
                    // Copy constructor
                    TStateAccessor( const TStateAccessor    &inStateAccessor );
                    TStateAccessor( const char              *inStateName );
                    ~TStateAccessor();

    void            AttachState( const char *inStateName );
    void            AttachOptionalState( const char*    inStateName,
                                         unsigned short inDefaultValue );

    // Value accessors
    unsigned short  GetStateValue() const;
    void            SetStateValue( unsigned short inValue );

    static void     Initialize( StateVector *inStateVector );
    static StateVector* GetStateVector();

  private:
            std::string    mName;
            unsigned short mDefaultValue;
            bool           mStateAvailable;

    static  StateVector *stateVector;
};

inline
TStateAccessor::TStateAccessor()
: mStateAvailable( false ),
  mDefaultValue( 0 )
{
}

inline
TStateAccessor::TStateAccessor( const TStateAccessor& inStateAccessor )
: mName( inStateAccessor.mName ),
  mStateAvailable( inStateAccessor.mStateAvailable ),
  mDefaultValue( inStateAccessor.mDefaultValue )
{
}

inline
TStateAccessor::TStateAccessor( const char  *inStateName )
{
    AttachState( inStateName );
}

inline
TStateAccessor::~TStateAccessor()
{
}

inline
StateVector*
TStateAccessor::GetStateVector()
{
    assert( stateVector != NULL );
    return stateVector;
}

#endif // STATE_ACCESSOR_H
