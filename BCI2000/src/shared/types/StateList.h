////////////////////////////////////////////////////////////////////////////////
// $Id: StateList.h 5386 2016-05-31 14:13:58Z mellinger $
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
#ifndef STATE_LIST_H
#define STATE_LIST_H

#include <vector>
#include <map>
#include <string>
#include "State.h"

class StateList
{
  typedef std::vector<State> StateContainer;
  typedef std::map<std::string, int, State::NameCmp> StateIndex;

 public:
  static const int ByteAlignment = sizeof( State::ValueType );

  StateList()
    { Clear(); }
  int  Size() const
    { return mStates.size(); }
  bool Empty() const
    { return mStates.empty(); }
  void Clear();

  bool operator==( const StateList& ) const;
  bool operator!=( const StateList& s ) const
    { return !operator==( s ); }

  const State* begin() const
    { return mStates.data(); }
  const State* end() const
    { return mStates.data() + mStates.size(); }

  int BitLength() const;
  int ByteLength() const;

  const State& ByName( const std::string& ) const;
  const State& ByIndex( size_t ) const;

  const State& operator[]( const std::string& s ) const
    { return ByName( s ); }
  const State& operator[]( int i ) const
    { return ByIndex( i ); }

  // Allows changing a state value in the list without invalidating alignment.
  bool SetStateValue( const std::string&, State::ValueType );

  bool Exists( const std::string& name ) const
    { return mIndex.find( name ) != mIndex.end(); }
  int  Index( const std::string& name ) const
    { return Exists( name ) ? mIndex.find( name )->second : Size(); };
  void Add( const State& );
  bool Add( const std::string& stateDefinition );
  void Delete( const std::string& name );
  void DeleteKind( int kind );
  void AssignPositions();

  int Alignment();
  int KindByteBegin( int kind );
  int KindByteEnd( int kind );

  std::ostream& InsertInto( std::ostream& ) const;
  std::istream& ExtractFrom( std::istream& );
  std::ostream& Serialize( std::ostream& ) const;
  std::istream& Unserialize( std::istream& );

 private:
  State& ByNameMutable( const std::string& );
  State& ByIndexMutable( size_t );

  static State PaddingState( int pos, int length, int id );
  int AlignedPosition( int ) const;
  void AddPadding( int byteLength );
  void RebuildIndex();

  int mAlignment,
      mKindByteBegin[State::lastKind + 1],
      mKindByteEnd[State::lastKind + 1];

  StateContainer mStates;
  StateIndex mIndex;

  State mDefaultState;
};

inline
std::ostream& operator<<( std::ostream& os, const StateList& s )
{ return s.InsertInto( os ); }

inline
std::istream& operator>>( std::istream& is, StateList& s )
{ return s.ExtractFrom( is ); }

#endif // STATE_LIST_H


