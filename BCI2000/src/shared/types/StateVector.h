////////////////////////////////////////////////////////////////////////////////
// $Id: StateVector.h 5384 2016-05-31 13:31:54Z mellinger $
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
#ifndef STATE_VECTOR_H
#define STATE_VECTOR_H

#include "State.h"
#include <iostream>
#include <thread>

class StateVector
{
 public:
  explicit StateVector( size_t length = 0, size_t numSamples = 1 );
  explicit StateVector( class StateList& list, size_t numSamples = 1 );
  StateVector( const StateVector& );
  ~StateVector();

  StateVector& operator=( const StateVector& );
  StateVector& CopyFromMasked( const StateVector&, const StateVector& mask );

  int Length() const { return mByteLength; }
  int Samples() const { return mSamples; }
  char* Data( size_t sample = 0 );
  const char* Data( size_t sample = 0 ) const;
  const class StateList& StateList() const { return *mpStateList; }

  State::ValueType StateValue( const std::string& name, size_t sample = 0 ) const;
  State::ValueType StateValue( size_t location, size_t length, size_t sample = 0 ) const;
  void SetStateValue( const std::string& name, State::ValueType value );
  void SetStateValue( const std::string& name, size_t sample, State::ValueType value );
  void SetStateValue( size_t location, size_t length, State::ValueType value );
  void SetStateValue( size_t location, size_t length, size_t sample, State::ValueType value );
  void PostStateChange( const std::string& name, State::ValueType value );
  void CommitStateChanges();

  State::ValueType CarryoverValue( size_t location, size_t length ) const;
  void ResetToCarryover( int stateKind );

  void SetSampleValue( size_t location, size_t length, size_t sample, State::ValueType value );

  std::ostream& InsertInto( std::ostream& ) const;
  std::istream& ExtractFrom( std::istream& );
  std::ostream& Serialize( std::ostream& ) const;
  std::istream& Unserialize( std::istream& );

 private:
  void Allocate( int byteLength, int samples );
  bool Aligned() const;
  static State::ValueType GetValueUnaligned( const char*, int location, int length );
  static void SetValueUnaligned( char*, int location, int length, State::ValueType );
  static State::ValueType GetValueAligned( const char*, int location, int length );
  static void SetValueAligned( char*, int location, int length, State::ValueType );

  class StateList* mpStateList;
  char* mpData;
  int mSamples, mByteLength;
  std::thread::id mCreatorThreadId;
};


inline
std::ostream& operator<<( std::ostream& os, const StateVector& s )
{ return s.InsertInto( os ); }

inline
std::istream& operator>>( std::istream& is, StateVector& s )
{ return s.ExtractFrom( is ); }

#endif // STATE_VECTOR_H

