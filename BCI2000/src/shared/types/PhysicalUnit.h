////////////////////////////////////////////////////////////////////////////////
// $Id: PhysicalUnit.h 5688 2017-08-15 19:14:13Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: PhysicalUnit is a class that represents a linear mapping from
//   physical units to raw numbers.
//   Apart from the obvious use in conjunction with measured values, this
//   class is also used for signal element or channel indexing when appropriate,
//   e.g. to map a frequency value to a spectral bin (element index).
//   Consistently with the definition of the SourceChOffset and SourceChGain
//   parameters, the relation between raw and physical value is
//     PhysicalValue = ( RawValue - offset ) * gain * unit
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
#ifndef PHYSICAL_UNIT_H
#define PHYSICAL_UNIT_H

#include <iostream>
#include <map>
#include <string>
#include "EncodedString.h"
#include "Numeric.h"

class PhysicalUnit
{
 public:
  typedef double ValueType;

  struct Pair
  {
    ValueType value;
    std::string unit;
    operator std::string() const;
    operator EncodedString() const;
    Pair Utf8() const;
  };

  PhysicalUnit() : mOffset( 0 ), mGain( 1 ), mRawMin( -1 ), mRawMax( 1 ), mIsRangeSet( false ) {}
  ~PhysicalUnit() {}

  PhysicalUnit& Clear()
                { return ( *this = PhysicalUnit() ); }

  ValueType     Offset() const
                { return mOffset; }
  PhysicalUnit& SetOffset( ValueType );

  ValueType     Gain() const
                { return mGain; }
  std::string   GainWithSymbol() const
                { return RawToPhysical(1 + mOffset); }
  PhysicalUnit& SetGain( ValueType );
  PhysicalUnit& SetGainWithSymbol( const std::string& );

  const std::string& Symbol() const
                { return mSymbol; }
  PhysicalUnit& SetSymbol( const std::string&, double power = 1 );

  ValueType     RawMin() const
                { return mRawMin; }
  PhysicalUnit& SetRawMin( ValueType );

  ValueType     RawMax() const
                { return mRawMax; }
  PhysicalUnit& SetRawMax( ValueType );

  bool          IsRangeSet() const
                { return mIsRangeSet; }

#if 0
  int           Size() const
                { return RawRange(); }
#endif
  int           RawRange() const;

  ValueType     PhysicalToRawValue( ValueType ) const;
  ValueType     RawToPhysicalValue( ValueType ) const;

  bool          IsPhysical( const std::string& ) const;
  ValueType     PhysicalToRaw( const std::string& ) const;
  Pair RawToPhysical( ValueType, ValueType range = NaN<ValueType>() ) const;

  bool          operator==( const PhysicalUnit& ) const;
  bool          operator!=( const PhysicalUnit& u ) const
                { return !( *this == u ); }
  PhysicalUnit& operator*=( const PhysicalUnit& );
  PhysicalUnit operator*( const PhysicalUnit& u ) const;

  PhysicalUnit& Combine( const PhysicalUnit& );

  std::ostream& InsertInto( std::ostream& os ) const;
  std::istream& ExtractFrom( std::istream& is );

 private:
  bool TokenizePhysical( const std::string&, size_t&, size_t& ) const;
  bool SexagesimalAllowed() const;
  bool ParseNumber( const std::string&, ValueType& ) const;
  bool ApplyPrefix( const std::string&, ValueType& ) const;
  void ExtractPrefix( ValueType, ValueType&, std::string& ) const;
  void ToEngNotation( ValueType, ValueType&, std::string& ) const;

  struct SymbolPowers : std::map<std::string, double>
  {
    SymbolPowers& operator*=( const SymbolPowers& );
    std::string SingleSymbol() const;
  };

  EncodedString mSymbol;
  SymbolPowers  mSymbolPowers;
  ValueType     mOffset,
                mGain,
                mRawMin,
                mRawMax;
  bool mIsRangeSet;
};

inline
std::ostream& operator<<( std::ostream& os, const PhysicalUnit& u )
{ return u.InsertInto( os ); }

inline
std::istream& operator>>( std::istream& is, PhysicalUnit& u )
{ return u.ExtractFrom( is ); }

inline
std::ostream& operator<<( std::ostream& os, const PhysicalUnit::Pair& p )
{ return os << Tiny::Pretty( p.value ) << p.unit; }

#endif // PHYSICAL_UNIT_H

