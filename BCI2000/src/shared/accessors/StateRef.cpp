//////////////////////////////////////////////////////////////////////
// $Id: StateRef.h 4036 2012-05-22 14:02:34Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A class that holds references to state values, and
//         allows for convenient automatic type
//         conversions when accessing state values.
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
///////////////////////////////////////////////////////////////////////
#include "StateRef.h"
#include "StateVector.h"
#include "BCIStream.h"

StateRef::StateRef()
: mpState( NULL ),
  mpStateVector( NULL ),
  mSample( 0 ),
  mDefaultValue( 0 )
{
}

StateRef::StateRef( const State* inState,
                    StateVector* inStateVector,
                    int inSample,
                    uint32_t inDefaultValue )
: mpState( inState ),
  mpStateVector( inStateVector ),
  mSample( inSample ),
  mDefaultValue( inDefaultValue )
{
}


StateRef&
StateRef::operator=( const StateRef& s )
{
  *this = uint32_t( s );
  return *this;
}


const StateRef&
StateRef::operator=( uint32_t inValue )
{
  if( mpState && mpStateVector )
    mpStateVector->SetStateValue( mpState->Location(), mpState->Length(), mSample, inValue );
  return *this;
}


StateRef::operator uint32_t() const
{
  uint32_t value = mDefaultValue;
  if( mpStateVector )
    value = mpStateVector->StateValue( mpState->Location(), mpState->Length(), mSample );
  return value;
}


StateRef
StateRef::operator()( size_t inOffset ) const
{
  return StateRef( mpState, mpStateVector, static_cast<int>( mSample + inOffset ), mDefaultValue );
}

const State*
StateRef::operator->() const
{
  return mpState;
}

StateRef&
StateRef::Reset( const StateRef& s )
{
  mpState = s.mpState;
  mpStateVector = s.mpStateVector;
  mSample = s.mSample;
  mDefaultValue = s.mDefaultValue;
  return *this;
}

StateRefFloat
StateRef::AsFloat()
{
  return StateRefFloat( *this );
}


StateRefSigned
StateRef::AsSigned()
{
  return StateRefSigned( *this );
}


StateRef&
StateRef::AsUnsigned()
{
  return *this;
}

StateRef::SampleRef::SampleRef( StateRef& ref, int sampleIdx )
: mrStateRef( ref ),
  mSample( sampleIdx )
{
}


StateRef::SampleRef
StateRef::Sample( int idx )
{
  if( mpState->Kind() != State::StreamKind )
    throw bcierr << "State " << mpState->Name() << " must be defined as a stream if used with the Sample() accessor";
  return SampleRef( *this, idx );
}


const StateRef::SampleRef&
StateRef::SampleRef::operator=( uint32_t val )
{
  int location = mrStateRef.mpState->Location(),
      length = mrStateRef.mpState->Length();
  mrStateRef.mpStateVector->SetSampleValue( location, length, mSample, val );
  return *this;
}


StateRef::SampleRef::operator uint32_t() const
{
  int location = mrStateRef.mpState->Location(),
      length = mrStateRef.mpState->Length();
  return mrStateRef.mpStateVector->StateValue( location, length, mSample );
}


StateRefFloat::StateRefFloat( StateRef& inStateRef )
: mrStateRef( inStateRef )
{}


const StateRefFloat&
StateRefFloat::operator=( float inF )
{
  union { float f; uint32_t i; } value;
  value.f = inF;
  mrStateRef = value.i;
  return *this;
}


StateRefFloat::operator float() const
{
  union { float f; uint32_t i; } value;
  value.i = mrStateRef;
  return value.f;
}


StateRefSigned::StateRefSigned( StateRef& inStateRef )
: mrStateRef( inStateRef )
{}


const StateRefSigned&
StateRefSigned::operator=( int32_t inL )
{
  const int bitsPerByte = 8;
  if( mrStateRef->Length() == static_cast<int>( sizeof( 1L ) * bitsPerByte ) )
    mrStateRef = inL;
  else
    mrStateRef = ( inL & ( ( 1L << mrStateRef->Length() ) - 1L ) );
  return *this;
}


StateRefSigned::operator int32_t() const
{
  int32_t result = mrStateRef;
  // Interpret the most significant bit as a sign bit, and
  // extend it to all leading bits in the result.
  if( mrStateRef & 1L << ( mrStateRef->Length() - 1 ) )
    result |= -1L << mrStateRef->Length();
  return result;
}

