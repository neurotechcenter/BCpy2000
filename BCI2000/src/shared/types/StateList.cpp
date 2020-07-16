////////////////////////////////////////////////////////////////////////////////
// $Id: StateList.cpp 5535 2016-09-19 16:54:55Z mellinger $
// Authors: schalk@wadsworth.org, juergen.mellinger@uni-tuebingen.de
// Description: BCI2000 type for a list of BCI2000 states (event markers).
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
#include "StateList.h"
#include "BCIException.h"
#include "Debugging.h"

#include <sstream>
#include <algorithm>
#include <cstring>

static const std::string sPaddingTag = "__pad";

static bool HasPaddingTag( const std::string& s )
{
  return s.find( sPaddingTag ) == 0;
}

State
StateList::PaddingState( int inPos, int inLength, int inId )
{
  std::ostringstream name;
  name << sPaddingTag << inId;
  State s;
  s.SetName( name.str() ).SetKind( State::Padding ).SetLocation( inPos ).SetLength( inLength );
  return s;
}

// **************************************************************************
// Function:   operator==
// Purpose:    Compare to another StateList object.
// Parameters: Reference to second StateList.
// Returns:    True if equal, false otherwise.
// **************************************************************************
bool
StateList::operator==( const StateList& s ) const
{
  bool result = true;
  auto i = mStates.begin(),
       j = s.mStates.begin();
  while( result && i != mStates.end() && j != s.mStates.end() )
    result = result && *i == *j;
  return result;
}

// **************************************************************************
// Function:   ByName
// Purpose:    Access a state by its name.
// Parameters: State name.
// Returns:    Returns a reference to a state with a given name.
// **************************************************************************
State&
StateList::ByNameMutable( const std::string& inName )
{
  mAlignment = 0;
  StateIndex::const_iterator i = mIndex.find( inName );
  if( i == mIndex.end() )
  {
    mIndex[inName] = Size();
    mStates.resize( Size() + 1 );
    i = mIndex.find( inName );
  }
  return mStates[i->second];
}

const State&
StateList::ByName( const std::string& inName ) const
{
  const State* result = &mDefaultState;
  StateIndex::const_iterator i = mIndex.find( inName );
  if( i != mIndex.end() )
    result = &mStates[i->second];
  return *result;
}

State&
StateList::ByIndexMutable( size_t idx )
{
  mAlignment = 0;
  return mStates[idx];
}

const State&
StateList::ByIndex( size_t idx ) const
{
  return mStates[idx];
}

bool
StateList::SetStateValue( const std::string& inName, State::ValueType inValue )
{
  StateIndex::const_iterator i = mIndex.find( inName );
  if( i == mIndex.end() )
    return false;
  mStates[i->second].SetValue( inValue );
  return true;
}

// **************************************************************************
// Function:   Clear
// Purpose:    Clear all entries from the state list.
// Parameters: N/A
// Returns:    N/A
// **************************************************************************
void
StateList::Clear()
{
  mAlignment = 0;
  ::memset( mKindByteBegin, 0xff, sizeof(mKindByteBegin) );
  ::memset( mKindByteEnd, 0xff, sizeof(mKindByteEnd) );
  mStates.clear();
  RebuildIndex();
}
// **************************************************************************
// Function:   BitLength
// Purpose:    Compute overall state data length in bits.
// Parameters: N/A
// Returns:    Bit length.
// **************************************************************************
int
StateList::BitLength() const
{
  if( Empty() )
    return 0;
  // A caller of this function implicitly assumes that state positions are consistent.
  // We throw an error if this assumption is violated.
  // The BitLength() function is called at setup time, so the overhead for checking
  // should not be a problem.
  typedef std::vector< std::pair<int, const State*> > UsageList;
  UsageList usage;
  for( const auto& s : mStates )
    usage.push_back( std::make_pair<int, const State*>( s.Location(), &s ) );
  sort( usage.begin(), usage.end() );
  int bad = -1;
  for( size_t i = 0; i < usage.size() - 1 && bad < 0; ++i )
    if( usage[i].first + usage[i].second->Length() > usage[i+1].first )
      bad = i + 1;
  if( bad >= 0 )
  {
    std::ostringstream oss;
    for( int i = 0; i < usage.size(); ++i )
      oss << *usage[i].second << ( i == bad ? " <<--" : "" ) << '\n';
    throw std_logic_error << "Inconsistent state positions:\n" << oss.str();
  }
  return usage.rbegin()->first + usage.rbegin()->second->Length();
}

int
StateList::ByteLength() const
{
/*
  ( BitLength() + 7 ) / 8
  would be sufficient, but a change would break interoperability with existing
  file readers and online modules
*/
  return BitLength() / 8 + 1;
}

// **************************************************************************
// Function:   Add
// Purpose:    adds a new state to the list of states
//             if a state with the same name already exists,
//             it updates the currently stored state with the provided values
// Parameters: statestring - ASCII string, as defined in project description,
//                           defining this new state
// Returns:    N/A
// **************************************************************************
bool
StateList::Add( const std::string& inDefinition )
{
  std::istringstream line( inDefinition );
  State s;
  if( line >> s )
  {
    if( Exists( s.Name() ) && !HasPaddingTag( s.Name() ) )
      ByNameMutable( s.Name() ).SetValue( s.Value() );
    else
      Add( s );
  }
  return !!line;
}

void
StateList::Add( const State& inState )
{
  int idx = -1;
  auto i = mIndex.find( inState.Name() );
  if( i == mIndex.end() )
  {
    idx = Size();
    mIndex[inState.Name()] = idx;
    mStates.resize( Size() + 1 );
  }
  else
  {
    idx = i->second;
  }
  ByIndexMutable( idx ) = inState;
  if( HasPaddingTag( inState.Name() ) )
    ByIndexMutable( idx ).SetKind( State::Padding );
}

// **************************************************************************
// Function:   Delete
// Purpose:    deletes a state of a given name in the list of states
//             it also frees the memory for this particular state
//             it does not do anything, if the state does not exist
// Parameters: name - name of the state
// Returns:    N/A
// **************************************************************************
void
StateList::Delete( const std::string& inName )
{
  auto i = mIndex.find( inName );
  if( i != mIndex.end() )
  {
    mStates.erase( mStates.begin() + i->second );
    RebuildIndex();
    mAlignment = 0;
  }
}

// **************************************************************************
// Function:   InsertInto
// Purpose:    Member function for formatted stream output of the entire
//             state list.
//             For partial output, use another instance of type StateList
//             to hold the desired subset.
//             All formatted output functions are, for consistency's sake,
//             supposed to use this function.
// Parameters: Output stream to write into.
// Returns:    Output stream written into.
// **************************************************************************
std::ostream&
StateList::InsertInto( std::ostream& os ) const
{
  for( int i = 0; i < Size(); ++i )
    os << ByIndex( i ) << '\n';
  return os;
}

// **************************************************************************
// Function:   ExtractFrom
// Purpose:    Member function for formatted stream input of the entire
//             state list. The list is cleared before reading.
//             For partial input, use another instance of type StateList
//             to hold the desired subset.
//             All formatted input functions are, for consistency's sake,
//             supposed to use this function.
// Parameters: Input stream to read from.
// Returns:    Input stream read from.
// **************************************************************************
std::istream&
StateList::ExtractFrom( std::istream& is )
{
  Clear();
  State state;
  is >> std::ws;
  while( !is.eof() && is >> state >> std::ws )
    Add( state );
  return is;
}

// **************************************************************************
// Function:   Serialize
// Purpose:    Member function for binary stream output of the entire
//             state list.
//             For partial output, use another instance of type StateList
//             to hold the desired subset.
// Parameters: Output stream to write into.
// Returns:    Output stream written into.
// **************************************************************************
std::ostream&
StateList::Serialize( std::ostream& os ) const
{
  for( int i = 0; i < Size(); ++i )
    ByIndex( i ).Serialize( os );
  return os;
}

// **************************************************************************
// Function:   Unserialize
// Purpose:    Member function for binary stream input of the entire
//             state list. The list is cleared before reading.
//             For partial input, use another instance of type StateList
//             to hold the desired subset.
// Parameters: Input stream to read from.
// Returns:    Input stream read from.
// **************************************************************************
std::istream&
StateList::Unserialize( std::istream& is )
{
  return ExtractFrom( is );
}

int
StateList::AlignedPosition( int pos ) const
{
  const int bitAlign = ByteAlignment * 8;
  return ( ( pos + bitAlign - 1 ) / bitAlign ) * bitAlign;
}

// **************************************************************************
// Function:   AssignPositions
// Purpose:    assigns positions to states contained in the list
// Parameters: none
// Returns:    N/A
// **************************************************************************
void
StateList::AssignPositions()
{
  mAlignment = 0;
  int pos = 0, dataEnd = 0;
  const int bitAlign = ByteAlignment * 8;
  // group by kind so memory is contiguous for each kind
  for( int kind = State::firstKind; kind <= State::lastKind; ++kind )
  {
    pos = AlignedPosition( pos );
    std::vector<State*> unassigned;
    for( State& s : mStates )
      if( s.Kind() == kind )
        unassigned.push_back( &s );
    std::stable_sort(
      unassigned.begin(), unassigned.end(),
      []( const State* a, const State* b )
      { return a->Length() > b->Length(); }
    );
    while( !unassigned.empty() )
    {
      auto j = unassigned.end();
      for( auto i = unassigned.begin(); i != unassigned.end() && j == unassigned.end(); ++i )
        if( ( pos + (*i)->Length() - 1 ) / bitAlign == pos / bitAlign )
          j = i;
      if( j == unassigned.end() )
      {
        pos = AlignedPosition( pos );
        j = unassigned.begin();
      }
      (*j)->SetLocation( pos );
      pos += (*j)->Length();
      dataEnd = pos;
      unassigned.erase( j );
    }
  }
  int bitLength = AlignedPosition( dataEnd );
  // For backward compatibility, state vector byte length is computed to be
  //   BitLength() / 8 + 1
  // rather than
  //   ( BitLength() + 7 ) / 8.
  // For an aligned and padded state vector, this will always result in an extra unused byte.
  // If we have a padding state at the end, we may regain that byte by reducing that padding state's
  // length by one bit.
  if( bitLength > dataEnd )
    --bitLength;
  AddPadding( bitLength / 8 + 1 );
  // assert interoperability with previous versions
  int totalBits = 0;
  for( const auto& s : mStates )
    totalBits += s.Length();
  Assert( ByteLength() == totalBits / 8 + 1 );
}

void
StateList::AddPadding( int inByteLength )
{
  // Previous versions of BCI2000 assumed there were no gaps between state locations.
  // For interoperability, padding states will be created to represent any gaps which
  // may result from alignment.

  std::vector<State> states;
  std::vector<const State*> sorted;
  states.reserve( mStates.size() );
  for( State& s : mStates )
  {
    if( s.Kind() != State::Padding && !HasPaddingTag( s.Name() ) )
    {
      states.push_back( s );
      sorted.push_back( &s );
    }
  }
  std::sort(
    sorted.begin(), sorted.end(),
    []( const State* a, const State* b ) { return a->Location() < b->Location(); }
  );
  int pos = 0, n = 0;
  for( auto pState : sorted )
  {
    if( pState->Location() < pos )
      throw std_logic_error << "Inconsistent state positions";
    if( pState->Location() > pos )
      states.push_back( PaddingState( pos, pState->Location() - pos, n++ ) );
    pos = pState->Location() + pState->Length();
  }
  int bitLength = 8 * inByteLength - 1;
  if( pos > bitLength )
    throw std_logic_error << "Inconsistent state positions";
  if( pos < bitLength )
    states.push_back( PaddingState( pos, bitLength - pos, n++ ) );

  mStates = states;
  RebuildIndex();
}

int
StateList::Alignment()
{
  if( mAlignment < 1 )
  {
    const int bitAlign = ByteAlignment * 8;
    bool aligned = true;
    for( const auto& s : mStates )
      if( s.Location() / bitAlign != ( s.Location() + s.Length() - 1 ) / bitAlign )
        aligned = false;
    mAlignment = aligned ? ByteAlignment : 1;

    bool grouped = aligned;
    if( grouped )
    {
      unsigned int posBegin[State::lastKind + 1], posEnd[State::lastKind + 1];
      ::memset( posBegin, 0xff, sizeof(posBegin) );
      ::memset( posEnd, 0, sizeof(posEnd) );
      for( const auto& s : mStates )
      {
        posBegin[s.Kind()] = std::min<unsigned int>( posBegin[s.Kind()], s.Location() );
        posEnd[s.Kind()] = std::max<unsigned int>( posEnd[s.Kind()], s.Location() + s.Length() - 1 );
      }
      for( int kind = State::firstKind; grouped && kind <= State::lastKind; ++kind )
      {
        if( posBegin[kind] > posEnd[kind] )
          posBegin[kind] = posEnd[kind];
        posBegin[kind] /= 8;
        posEnd[kind] = ( posEnd[kind] + 7 ) / 8;
      }
      for( int kind = State::firstKind; grouped && kind <= State::lastKind; ++kind )
      {
        int begin = posBegin[kind], end = posEnd[kind];
        for( int kind2 = kind + 1; kind2 <= State::lastKind; ++kind2 )
        {
          int begin2 = posBegin[kind2], end2 = posEnd[kind2];
          bool overlap = ( begin2 >= begin && begin2 < end ) || ( begin >= begin2 && begin < end2 );
          grouped = grouped && !overlap;
        }
      }
      if( grouped ) for( int kind = State::firstKind; kind <= State::lastKind; ++kind )
      {
        mKindByteBegin[kind] = posBegin[kind];
        mKindByteEnd[kind] = posEnd[kind];
      }
    }
    if( !grouped ) for( int kind = State::firstKind; kind <= State::lastKind; ++kind )
    {
      mKindByteBegin[kind] = -1;
      mKindByteEnd[kind] = -1;
    }
  }
  return mAlignment;
}

int
StateList::KindByteBegin( int kind )
{
  if( Alignment() != ByteAlignment )
    return -1;
  return mKindByteBegin[kind];
}

int
StateList::KindByteEnd( int kind )
{
  if( Alignment() != ByteAlignment )
    return -1;
  return mKindByteEnd[kind];
}

void
StateList::DeleteKind( int inKind )
{
  int prevSize = Size();
  for( auto i = mStates.begin(); i != mStates.end(); )
    if( i->Kind() == inKind )
      i = mStates.erase( i );
    else
      ++i;
  if( Size() != prevSize )
    RebuildIndex();
}

// **************************************************************************
// Function:   RebuildIndex
// Purpose:    Rebuilds the name-to-position lookup index.
//             This function must be called after any change to the state
//             container.
// Parameters: N/A
// Returns:    N/A
// **************************************************************************
void
StateList::RebuildIndex()
{
  mIndex.clear();
  for( int i = 0; i < Size(); ++i )
    mIndex[ByIndex( i ).Name()] = i;
}
