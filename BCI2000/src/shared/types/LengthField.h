////////////////////////////////////////////////////////////////////////////////
// $Id: LengthField.h 4742 2014-10-09 16:29:42Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A template for length fields that occur in BCI2000 messages,
//   e.g. for the length of the message, or for the number of channels
//   and elements in a data message.
//
//   Some of the fixed-length fields have proven too short, so there is a
//   backward compatible extension introduced: An N-bit fixed-length entry
//   may contain a value of 2^N-1; in that case, a null-terminated ASCII
//   representation of the actual length follows.
//   If the represented value does not exceed 2^N-2, it is always transmitted
//   within in the fixed-length field, and not followed by a string.
//
//   These conventions ensure inter-operability between different versions of
//   the BCI2000 protocol as long as messages and signals have appropriate
//   sizes/dimensions.
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
#ifndef LENGTH_FIELD_H
#define LENGTH_FIELD_H

#include <iostream>

template<int NumBytes> class LengthField
{
  public:
    static const size_t EscapeValue = ( 1 << ( 8 * NumBytes ) ) - 1; // NumBytes bytes filled with 0xff.
    LengthField() : mValue( 0 ) {}
    template<typename T> LengthField( T value ) : mValue( value ) {}
    template<typename T> LengthField& operator=( T value ) { mValue = size_t( value ); return *this; }

    // Type conversion:
    operator size_t&() { return mValue; }
    operator size_t() const { return mValue; }

    // Formatted (ASCII) I/O:
    std::ostream& InsertInto( std::ostream& os ) const { return os << mValue; }
    std::istream& ExtractFrom( std::istream& is ) { return is >> mValue; }

    // Binary I/O according to the BCI2000 protocol:
    std::ostream& Serialize( std::ostream& os ) const;
    std::istream& Unserialize( std::istream& is );

  private:
    size_t mValue;
};

template<int NumBytes> std::ostream&
operator<<( std::ostream& os, const LengthField<NumBytes>& l )
{
  return l.InsertInto( os );
}

template<int NumBytes> std::istream&
operator>>( std::istream& is, LengthField<NumBytes>& l )
{
  return l.ExtractFrom( is );
}

template<int NumBytes> std::ostream&
LengthField<NumBytes>::Serialize( std::ostream& os ) const
{
  size_t value = mValue;
  if( value > EscapeValue )
    value = EscapeValue;
  // Old protocol: Write bytes of mValue in little endian order.
  // Extended protocol: Write the escape value.
  for( int i = 0; i < NumBytes; ++i )
  {
    os.put( value & 0xff );
    value >>= 8;
  }
  if( mValue >= EscapeValue )
  {
    // Extended protocol: Write a null-terminated ASCII representation of the
    // value after the escape value.
    os << std::dec << mValue;
    os.put( '\0' );
  }
  return os;
}

template<int NumBytes> std::istream&
LengthField<NumBytes>::Unserialize( std::istream& is )
{
  mValue = 0;
  // Read a little endian value according to the old protocol.
  for( int i = 0; i < NumBytes; ++i )
    mValue |= ( is.get() << ( 8 * i ) );
  // If the value matches the escape value, read a null-terminated
  // ASCII representation.
  if( mValue == EscapeValue )
  {
    is >> std::dec >> mValue;
    if( is.get() != '\0' )
      is.setstate( is.failbit );
  }
  return is;
}

#endif // LENGTH_FIELD_H

