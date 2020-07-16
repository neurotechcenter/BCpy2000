////////////////////////////////////////////////////////////////////////////////
// $Id: StateVector.cpp 5427 2016-07-14 00:36:04Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: BCI2000 type for the the binary representation of states
//  (event markers) for an entire data block.
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
#include "StateVector.h"

#include "StateList.h"
#include "Debugging.h"

#include <sstream>
#include <iomanip>
#include <type_traits>
#include <cstdint>
#include <cstring>

using namespace std;

StateVector::StateVector( size_t inByteLength, size_t inSamples )
: mpStateList( nullptr ),
  mpData( nullptr ),
  mSamples( 0 ),
  mByteLength( 0 ),
  mCreatorThreadId( std::this_thread::get_id() )
{
  Allocate( inByteLength, inSamples );
  ::memset( mpData, 0, mByteLength * mSamples );
}

StateVector::StateVector( class StateList& inList, size_t inSamples )
: mpStateList( &inList ),
  mpData( nullptr ),
  mSamples( 0 ),
  mByteLength( 0 ),
  mCreatorThreadId( std::this_thread::get_id() )
{
  Allocate( mpStateList->ByteLength(), inSamples );
  // initialize the content in the state vector, according to the content
  // of the current states in the state list
  for( const auto& s : *mpStateList )
    SetStateValue( s.Name(), s.Value() );
}

StateVector::StateVector( const StateVector& inOther )
: mpStateList( nullptr ),
  mpData( nullptr ),
  mSamples( 0 ),
  mByteLength( 0 ),
  mCreatorThreadId( std::this_thread::get_id() )
{
  *this = inOther;
}

StateVector::~StateVector()
{
  Allocate( 0, 0 );
}

StateVector&
StateVector::operator=( const StateVector& inOther )
{
  Allocate( inOther.Length(), inOther.Samples() );
  mpStateList = inOther.mpStateList;
  ::memcpy( mpData, inOther.mpData, inOther.Length() * inOther.Samples() );
  return *this;
}

// **************************************************************************
// Function:   StateValue
// Purpose:    returns a state's value from the state vector block
// Parameters: statename - the name of a state
// Returns:    the value of the state
//             0 on error (e.g., state not found)
// **************************************************************************
State::ValueType
StateVector::StateValue( const string& inName, size_t inSample ) const
{
  State::ValueType result = 0;
  if( mpStateList && mpStateList->Exists( inName ) )
  {
    const State& s = mpStateList->ByName( inName );
    result = StateValue( s.Location(), s.Length(), inSample );
  };
  return result;
}

// **************************************************************************
// Function:   StateValue
// Purpose:    returns a state's value, based upon the state's location and
//             size
// Parameters: location ... bit location of the state
//             length   ... bit length of the state
//             sample   ... sample position for which to return the state's
//                          value
// Returns:    the value of the state
// **************************************************************************
State::ValueType
StateVector::StateValue( size_t inLocation, size_t inLength, size_t inSample ) const
{
  if( inLength > 8 * sizeof( State::ValueType ) )
    throw std_range_error << "Invalid state length: " << inLength;
  if( inLocation + inLength > 8 * mByteLength )
    throw std_range_error << "Accessing non-existent state vector data, location: " << inLocation;
  if( inSample >= mSamples )
    throw std_range_error << "Accessing non-existent state vector data, sample: " << inSample;
#if 0
  if( Aligned() )
    Assert( GetValueAligned( Data( inSample ), inLocation, inLength ) == GetValueUnaligned( Data( inSample ), inLocation, inLength ) );
#endif
  return Aligned() ? GetValueAligned( Data( inSample ), inLocation, inLength )
                   : GetValueUnaligned( Data( inSample ), inLocation, inLength );
}

// **************************************************************************
// Function:   SetStateValue
// Purpose:    sets a state's value in the state vector block
// Parameters: statename - name of the state
// Returns:    N/A
// **************************************************************************
void
StateVector::SetStateValue( const string& inName, size_t inSample, State::ValueType inValue )
{
  if( mpStateList && mpStateList->Exists( inName ) )
  {
    const State& s = mpStateList->ByName( inName );
    SetStateValue( s.Location(), s.Length(), inSample, inValue );
  }
}

void
StateVector::SetStateValue( const string& inName, State::ValueType inValue )
{
  if( mpStateList && mpStateList->Exists( inName ) )
  {
    const State& s = mpStateList->ByName( inName );
    SetStateValue( s.Location(), s.Length(), 0, inValue );
  }
}

void
StateVector::SetSampleValue( size_t inLocation, size_t inLength, size_t inSample, State::ValueType inValue )
{
  if( Aligned() )
    SetValueAligned( Data( inSample ), inLocation, inLength, inValue );
  else
    SetValueUnaligned( Data( inSample ), inLocation, inLength, inValue );
}

// **************************************************************************
// Function:   SetStateValue
// Purpose:    sets a state's value in the state vector
// Parameters: location ... bit location of the state
//             length   ... bit length of the state
//             sample   ... sample position from which to set the state value
//             value    ... value of the state
// Returns:    N/A
// **************************************************************************
void
StateVector::SetStateValue( size_t inLocation, size_t inLength, size_t inSample, State::ValueType inValue )
{
  if( std::this_thread::get_id() != mCreatorThreadId )
    throw std_runtime_error << "Trying to set a state value from a secondary thread.\n"
                            << "NB: BufferedADC provides special means to deal with additional synchronous channels.\n"
                            << "    Also, the bcievent accessor allows to asynchronously write to streams.";
  State::ValueType valueMask = ( ~State::ValueType( 0 ) >> ( 8 * sizeof( State::ValueType ) - inLength ) );
  if( inValue < 0 || ( inValue & valueMask ) != inValue )
    throw std_range_error
      << "Illegal value "
      << inValue
      << " was passed to "
      << inLength
      << "-bit state at address "
      << inLocation;
  if( inLength > 8 * sizeof( State::ValueType ) )
    throw std_range_error << "Invalid state length of " << inLength;
  if( inLocation + inLength > 8 * mByteLength )
    throw std_range_error << "Accessing non-existent state vector data, location: " << inLocation;
  if( inSample >= mSamples )
    throw std_range_error << "Accessing non-existent state vector data, sample: " << inSample;

  for( int i = inSample; i < Samples(); ++i )
    SetSampleValue( inLocation, inLength, i, inValue );
}

void
StateVector::SetStateValue( size_t inLocation, size_t inLength, State::ValueType inValue )
{
  SetStateValue( inLocation, inLength, 0, inValue );
}
// **************************************************************************
// Function:   PostStateChange
// Purpose:    Have a state changed upon the next call to CommitStateChanges().
// Parameters: Name of the state to be changed; requested value
// Returns:    N/A
// **************************************************************************
void
StateVector::PostStateChange( const string& inName, State::ValueType inValue )
{
  if( !mpStateList || !mpStateList->Exists( inName ) )
    throw std_runtime_error << "Undeclared state " << inName;

  mpStateList->SetStateValue( inName, inValue ); // We use State::value as a buffer.
}

// **************************************************************************
// Function:   CommitStateChanges
// Purpose:    Have all states commit their changes, if any.
// Parameters: N/A
// Returns:    N/A
// **************************************************************************
void
StateVector::CommitStateChanges()
{
  if( mpStateList )
    for( const auto& s : *mpStateList )
      s.Commit( this );
}

// **************************************************************************
// Function:   InsertInto
// Purpose:    Member function for formatted output of a state vector block
//             into a stream.
// Parameters: Output stream to write into.
// Returns:    Output stream.
// **************************************************************************
ostream&
StateVector::InsertInto( ostream& os ) const
{
  streamsize indent = os.width();
  if( mpStateList == nullptr )
  {
    for( int i = 0; i < Length(); ++i )
    {
      os << '\n' << setw( indent ) << ""
         << i << ":";
      for( int j = 0; j < Samples(); ++j )
        os << " " << int( Data(j)[i] );
    }
  }
  else
  {
    for( const auto& state : *mpStateList )
    {
      os << '\n' << setw( indent ) << ""
         << state.Name() << ":";
      for( int j = 0; j < Samples(); ++j )
        os << " " << StateValue( state.Location(), state.Length(), j );
    }
  }
  return os;
}

// **************************************************************************
// Function:   Unserialize
// Purpose:    Member function for input of a state vector block
//             from a binary stream, as in a state vector message.
// Parameters: Input stream to read from.
// Returns:    Input stream.
// **************************************************************************
istream&
StateVector::Unserialize( istream& is )
{
  int byteLength, samples;
  ( is >> byteLength ).get();
  ( is >> samples ).get();
  Allocate( byteLength, samples );
  is.read( mpData, byteLength * samples );
  return is;
}

// **************************************************************************
// Function:   Serialize
// Purpose:    Member function for output of a state vector block
//             into a binary stream, as in a state vector message.
// Parameters: Output stream to write into.
// Returns:    Output stream.
// **************************************************************************
ostream&
StateVector::Serialize( ostream& os ) const
{
  ( os << mByteLength ).put( '\0' );
  ( os << mSamples ).put( '\0' );
  os.write( mpData, mSamples * mByteLength );
  return os;
}

char*
StateVector::Data( size_t inSample )
{
  Assert( inSample < mSamples );
  return mpData + mByteLength * inSample;
}

const char*
StateVector::Data( size_t inSample ) const
{
  Assert( inSample < mSamples );
  return mpData + mByteLength * inSample;
}

void
StateVector::Allocate( int byteLength, int samples )
{
  int oldSize = mByteLength * mSamples,
      newSize = byteLength * samples;
  if( oldSize != newSize )
  {
    delete[] mpData;
    mpData = newSize ? new char[newSize] : nullptr;
  }
  mByteLength = byteLength;
  mSamples = samples;
}

State::ValueType
StateVector::CarryoverValue( size_t location, size_t length ) const
{
  int carryOver = Samples() - 1;
  return StateValue( location, length, carryOver );
}

void
StateVector::ResetToCarryover( int inKind )
{ // For all states of the given kind, reset values to the value at the carry over position.
  Assert( mpStateList );
  int carryOver = Samples() - 1;
  int byteBegin = mpStateList->KindByteBegin( inKind );
  if( byteBegin >= 0 )
  {
    int byteEnd = mpStateList->KindByteEnd( inKind );
    for( int sample = 0; sample < carryOver; ++sample )
      ::memcpy( Data( sample ) + byteBegin, Data( carryOver ) + byteBegin, byteEnd - byteBegin );
  }
  else for( const auto& s : *mpStateList )
  {
    if( s.Kind() == inKind )
    {
      int location = s.Location(),
          length = s.Length();
      State::ValueType value = StateValue( location, length, carryOver );
      SetStateValue( location, length, value );
    }
  }
}

State::ValueType
StateVector::GetValueUnaligned( const char* inData, int inLocation, int inLength )
{
  State::ValueType result = 0;
  for( int bitIndex = inLocation + inLength - 1; bitIndex >= inLocation; --bitIndex )
  {
    result <<= 1;
    if( inData[ bitIndex / 8 ] & ( 1 << ( bitIndex % 8 ) ) )
      result |= 1;
  }
  return result;
}

void
StateVector::SetValueUnaligned( char* inData, int inLocation, int inLength, State::ValueType inValue )
{
  State::ValueType value = inValue;
  for( size_t bitIndex = inLocation; bitIndex < inLocation + inLength; ++bitIndex )
  {
    unsigned char mask = 1 << ( bitIndex % 8 );
    if( value & 1 )
      inData[ bitIndex / 8 ] |= mask;
    else
      inData[ bitIndex / 8 ] &= ~mask;
    value >>= 1;
  }
}

// Alignment guarantees that state data may be accessed using a single load/store operation.
static_assert( !std::is_signed<State::ValueType>::value, "State::ValueType must be unsigned" );
static const int sBitsPerValue = sizeof(State::ValueType) * 8;

static State::ValueType BitMask( int location, int length )
{ // NB: Shifting by >= bitsizeof(op) invokes undefined behavior
  State::ValueType mask = State::ValueType( -1 );
  mask >>= sBitsPerValue - length;
  mask <<= location % sBitsPerValue;
  return mask;
}

State::ValueType
StateVector::GetValueAligned( const char* inData, int inLocation, int inLength )
{
  union { const char* c; const State::ValueType* v; } pData = { inData };
  State::ValueType value = pData.v[inLocation / sBitsPerValue] & BitMask( inLocation, inLength );
  value >>= inLocation % sBitsPerValue;
  return value;
}

void
StateVector::SetValueAligned( char* inData, int inLocation, int inLength, State::ValueType inValue )
{
  union { char* c; State::ValueType* v; } pData = { inData };
  State::ValueType& value = pData.v[inLocation / sBitsPerValue];
  value &= ~BitMask( inLocation, inLength );
  value |= inValue << ( inLocation % sBitsPerValue );
}

bool
StateVector::Aligned() const
{
  return mpStateList && mpStateList->Alignment() == sizeof( State::ValueType );
}


