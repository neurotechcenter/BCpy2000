////////////////////////////////////////////////////////////////////////////////
// $Id: GenericSignal.h 5694 2017-08-17 19:06:09Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: GenericSignal is the BCI2000 type representing filter input and
//              output data.
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
#ifndef GENERIC_SIGNAL_H
#define GENERIC_SIGNAL_H

#include <vector>
#include <string>
#include <iostream>
#include "SignalType.h"
#include "SignalProperties.h"
#include "LazyArray.h"
#include "SharedMemory.h"

class GenericChannel;
class GenericElement;

class GenericSignal
{
  public:
    typedef double ValueType;
    static const ValueType NaN;

    GenericSignal();
    GenericSignal( const GenericSignal& other )
      { AssignFrom( other ); }
    GenericSignal& operator=( const GenericSignal& other )
      { return AssignFrom( other ); }
    ~GenericSignal();

    GenericSignal(
      size_t inChannels,
      size_t inMaxElements,
      SignalType::Type inType = SignalType::defaultType );
    GenericSignal(
      size_t inChannels,
      size_t inMaxElements,
      SignalType inType );
    explicit GenericSignal( const SignalProperties& );
    GenericSignal( const SignalProperties&, ValueType );

    GenericSignal& SetProperties( const SignalProperties& );
    const SignalProperties& Properties() const
                            { return mProperties; }
    GenericSignal& AssignValues( const GenericSignal& );

    // Read access to properties
    int Channels() const
      { return mProperties.Channels(); }
    int Elements() const
      { return mProperties.Elements(); }
    const SignalType& Type() const
      { return mProperties.Type(); }
    bool Empty() const
      { return Channels() < 1 || Elements() < 1; }

    // Value Accessors
    ValueType Value( size_t ch, size_t el ) const
      { return mValues[mProperties.LinearIndex( ch, el )]; }
    ValueType& Value( size_t ch, size_t el )
      { return mValues[mProperties.LinearIndex( ch, el )]; }
    GenericSignal& SetValue( size_t ch, size_t el, ValueType value )
      { Value( ch, el ) = value; return *this; }
    GenericSignal& SetAllValues( ValueType value );
    ValueType operator() ( size_t ch, size_t el ) const
      { return Value( ch, el ); }
    ValueType& operator() ( size_t ch, size_t el )
      { return Value( ch, el ); }

    void EnsureDeepCopy();
    bool ShareAcrossModules();

    // Stream i/o
    std::ostream& InsertInto( std::ostream& ) const;
    std::istream& ExtractFrom( std::istream& );
    std::ostream& WriteValueBinary( std::ostream&, size_t ch, size_t el ) const;
    std::istream& ReadValueBinary( std::istream&, size_t ch, size_t el );
    std::ostream& Serialize( std::ostream& ) const;
    std::istream& Unserialize( std::istream& );

  private:
    static void PutValue_float24( std::ostream&, ValueType );
    static ValueType GetValue_float24( std::istream& );

    GenericSignal& AssignFrom( const GenericSignal& );
    void AttachToSharedMemory( const std::string& );

    SignalProperties mProperties;
    LazyArray<ValueType> mValues;
    SharedPointer<SharedMemory> mSharedMemory;
};

class GenericChannel
{
  public:
    GenericChannel( GenericSignal& s, int ch ) : mrSignal( s ), mCh( ch ) {}
    GenericChannel& operator=( const GenericChannel& );
    int Channels() const { return 1; }
    int Elements() const { return mrSignal.Elements(); }
    size_t size() const { return Elements(); }
    GenericSignal::ValueType& operator[]( size_t el ) { return mrSignal( mCh, el ); }
    GenericSignal::ValueType& operator()( int ch, int el ) { return ( *this )[el]; }
    const GenericSignal::ValueType operator[]( size_t el ) const { return mrSignal( mCh, el ); }
    const GenericSignal::ValueType operator()( int ch, int el ) const { return ( *this )[el]; }
  private:
    GenericSignal& mrSignal;
    int mCh;
};

class GenericElement
{
  public:
    GenericElement( GenericSignal& s, int el ) : mrSignal( s ), mEl( el ) {}
    GenericElement& operator=( const GenericElement& );
    int Channels() const { return mrSignal.Channels(); }
    int Elements() const { return 1; }
    size_t size() const { return Channels(); }
    GenericSignal::ValueType& operator[]( size_t ch ) { return mrSignal( ch, mEl ); }
    GenericSignal::ValueType& operator()( int ch, int el ) { return ( *this )[ch]; }
    const GenericSignal::ValueType operator[]( size_t ch ) const { return mrSignal( ch, mEl ); }
    const GenericSignal::ValueType operator()( int ch, int el ) const { return ( *this )[ch]; }
  private:
    GenericSignal& mrSignal;
    int mEl;
};

inline
std::ostream& operator<<( std::ostream& os, const GenericSignal& s )
{ return s.InsertInto( os ); }

inline
std::istream& operator>>( std::istream& is, GenericSignal& s )
{ return s.ExtractFrom( is ); }

#endif // GENERIC_SIGNAL_H

