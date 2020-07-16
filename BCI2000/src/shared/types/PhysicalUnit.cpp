////////////////////////////////////////////////////////////////////////////////
// $Id: PhysicalUnit.cpp 5765 2018-05-22 15:23:46Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: PhysicalUnit is a class that represents a linear mapping from
//   physical units to raw numbers.
//   Apart from the obvious use in conjunction with measured values, this
//   class is also used for signal element or channel indexing when appropriate,
//   e.g. to map a frequency value to a spectral bin (element index).
//   Consistently with the definition of the SourceChOffset and SourceChGain
//   parameters, the relation between raw and physical value is
//     PhysicalValue = ( RawValue - offset ) * gain * symbol
//
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
#include "PhysicalUnit.h"
#include "ArithmeticExpression.h"
#include "BCIException.h"
#include <limits>
#include <cmath>
#include <sstream>
#include <iomanip>
#include <algorithm>

namespace {
const struct Prefix
{
  const char* name;
  PhysicalUnit::ValueType value;
  int log10value;
} Prefixes[] =
{
  { "f", 1e-15, -15 },
  { "p", 1e-12, -12 },
  { "n", 1e-9, -9 },
  { "mu", 1e-6, -6 }, { "u", 1e-6, -6 }, { "\xc2\xb5", 1e-6, -6 },
  { "m", 1e-3, -3 },
  { "",  1.0, 0 },
  { "k", 1e3, 3 },
  { "M", 1e6, 6 },
  { "G", 1e9, 9 },
  { "T", 1e12, 12 },
},
*PrefixesEnd = Prefixes + sizeof( Prefixes ) / sizeof( *Prefixes );

} // namespace

bool
PhysicalUnit::TokenizePhysical( const std::string& inPhysical, size_t& outPrefixPos, size_t& outSymbolPos ) const
{
  outPrefixPos = inPhysical.find_first_not_of( "0123456789.:Ee+-*/^() " );
  if( outPrefixPos == std::string::npos )
    outPrefixPos = inPhysical.length();
  std::ptrdiff_t symbolPos = inPhysical.length() - Symbol().length();
  bool ok = symbolPos > 0 && inPhysical.substr( symbolPos ) == Symbol();
  outSymbolPos = ok ? symbolPos : inPhysical.length();
  return ok;
}

bool
PhysicalUnit::SexagesimalAllowed() const
{
  const char* allowSexagesimal[] = { "", "s", "deg" };
  size_t count = sizeof( allowSexagesimal ) / sizeof( *allowSexagesimal );
  size_t i = 0;
  while( i < count && allowSexagesimal[i] != mSymbol )
    ++i;
  return i < count;
}

bool
PhysicalUnit::ParseNumber( const std::string& inNumber, PhysicalUnit::ValueType& outValue ) const
{
  outValue = 0;
  bool valid = true;
  int count = 0;
  for( size_t beginPos = 0; beginPos < inNumber.length(); )
  {
    ++count;
    size_t endPos = inNumber.find( ':', beginPos );
    if( endPos == std::string::npos )
      endPos = inNumber.length();
    else if( !SexagesimalAllowed() )
      return false;
    size_t length = endPos - beginPos;
    valid &= ( length > 0 );
    ValueType value = ArithmeticExpression( inNumber.substr( beginPos, length ) ).Evaluate();
    if( beginPos != 0 )
    {
      valid &= ( value >= 0 && value < 60 );
      outValue *= 60;
    }
    outValue += value;
    beginPos = endPos;
  }
  valid &= ( count > 0 && count <= 3 );
  return valid;
}

bool
PhysicalUnit::ApplyPrefix( const std::string& inPrefix, PhysicalUnit::ValueType& ioValue ) const
{
  for( const Prefix* p = Prefixes; p < PrefixesEnd; ++p )
    if( inPrefix == p->name )
    {
      ioValue *= p->value;
      return true;
    }
  return false;
}

void
PhysicalUnit::ExtractPrefix( PhysicalUnit::ValueType inRange, PhysicalUnit::ValueType& ioValue, std::string& outPrefix ) const
{
  int orderOfMagnitude = Floor( ::log10( inRange ) );
  for( const Prefix* p = Prefixes; p != PrefixesEnd; ++p )
    if( p->log10value >= orderOfMagnitude - 2 )
    {
      ioValue /= p->value;
      outPrefix = p->name;
      return;
    }
}

void
PhysicalUnit::ToEngNotation( PhysicalUnit::ValueType inRange, PhysicalUnit::ValueType& ioValue, std::string& outPrefix ) const
{
  int n = 0;
  PhysicalUnit::ValueType f = 1;
  if( inRange < Eps( inRange ) )
    return;

  while( f * inRange < 0.01 )
  {
    f *= 1e3;
    n -= 3;
  }
  while( f * inRange >= 1e3 )
  {
    f *= 1e-3;
    n += 3;
  }
  ioValue *= f;
  if( n != 0 )
  {
    std::ostringstream oss;
    oss << "e" << n;
    outPrefix = oss.str();
  }
}

PhysicalUnit::ValueType
PhysicalUnit::RawToPhysicalValue( ValueType inRawValue ) const
{
  return ( inRawValue - mOffset ) * mGain;
}

PhysicalUnit::ValueType
PhysicalUnit::PhysicalToRawValue( ValueType inPhysicalValue ) const
{
  return ( inPhysicalValue / mGain ) + mOffset;
}

PhysicalUnit&
PhysicalUnit::SetOffset( ValueType inOffset )
{
  mOffset = inOffset;
  return *this;
}

PhysicalUnit&
PhysicalUnit::SetGain( ValueType inGain )
{
  mGain = inGain;
  return *this;
}

PhysicalUnit&
PhysicalUnit::SetGainWithSymbol( const std::string& inGain )
{
  if( inGain.empty() )
    throw bciexception << "Value is empty";

  mSymbol.clear();
  size_t pos = 0, ignored = 0;
  TokenizePhysical( inGain, pos, ignored );
  ValueType gain = 0;
  if( !ParseNumber( inGain.substr( 0, pos ), gain ) )
    throw bciexception << "Invalid number format: " << inGain;
  std::string prefix = inGain.substr( pos );
  while( !prefix.empty() && !ApplyPrefix( prefix, gain ) )
    prefix.erase( prefix.length() - 1 );
  if( gain == 0 )
    throw bciexception << "Zero gain specification: " << inGain;
  SetGain( gain );
  SetSymbol( inGain.substr( pos + prefix.length() ) );
  return *this;
}

PhysicalUnit&
PhysicalUnit::SetSymbol( const std::string& inSymbol, double inPower )
{
  mSymbolPowers.clear();
  mSymbolPowers[inSymbol] = inPower;
  mSymbol = mSymbolPowers.SingleSymbol();
  return *this;
}

PhysicalUnit&
PhysicalUnit::SetRawMin( ValueType v )
{
  mRawMin = v;
  mIsRangeSet = true;
  return *this;
}

PhysicalUnit&
PhysicalUnit::SetRawMax( ValueType v )
{
  mRawMax = v;
  mIsRangeSet = true;
  return *this;
}

int
PhysicalUnit::RawRange() const
{
  return Floor( ::fabs( mRawMax - mRawMin ) + 1 );
}

bool
PhysicalUnit::IsPhysical( const std::string& inValue ) const
{
  if( Symbol().empty() )
    return false;

  size_t prefixPos = 0, symbolPos = 0;
  if( !TokenizePhysical( inValue, prefixPos, symbolPos ) )
    return false;

  ValueType value = 0;
  if( !ParseNumber( inValue.substr( 0, prefixPos ), value ) )
    return false;

  if( !ApplyPrefix( inValue.substr( prefixPos, symbolPos - prefixPos ), value ) )
    return false;

  return true;
}

PhysicalUnit::ValueType
PhysicalUnit::PhysicalToRaw( const std::string& s ) const
{
  ValueType value = 0;
  size_t beginPos = 0,
         prefixPos = 0,
         symbolPos = 0;
  bool unitOK = TokenizePhysical( s, prefixPos, symbolPos );
  std::string number = s.substr( beginPos, prefixPos );
  if( !ParseNumber( number, value ) )
    throw bciexception << "Invalid number format \"" << number << "\" in " << s;
  if( value != 0 ) // zero times whatever is identical to zero
  {
	 std::string prefix = s.substr( prefixPos, symbolPos - prefixPos );
    if( !ApplyPrefix( prefix, value ) )
      throw bciexception << "Invalid unit prefix \"" << prefix << "\" in " << s;
    if( !unitOK )
    {
	  std::string symbol = s.substr( symbolPos ),
             message;
      if( symbol.empty() )
        message += "Missing measurement unit";
      else
        message += "Unexpected measurement unit \"" + symbol + "\"";
      message += " in \"" + s + "\"";
      if( !Symbol().empty() )
      {
        message += " , expected \"" + Symbol() + "\"";
        if( !symbol.empty() )
          message += " instead";
      }
      throw bciexception << message;
    }
  }
  return PhysicalToRawValue( value );
}

PhysicalUnit::Pair
PhysicalUnit::RawToPhysical( ValueType inRawValue, ValueType inRangeMax ) const
{
  Pair result = { RawToPhysicalValue( inRawValue ), "" };
  ValueType range = 0;
  if( !IsNaN( inRangeMax ) )
    range = RawToPhysicalValue( inRangeMax );
  if( range == 0 )
    range = ::fabs( result.value );
  
  if( range <= Eps( range ) )
  {
    result.value = 0;
    return result;
  }
  else if( range == Inf( range ) || IsNaN( result.value ) )
    return result;

  if( Symbol().empty() )
  {
    ToEngNotation( range, result.value, result.unit );
  }
  else
  {
    ExtractPrefix( range, result.value, result.unit );
    result.unit += Symbol();
  }
  return result;
}

bool
PhysicalUnit::operator==( const PhysicalUnit& inUnit ) const
{
  return mSymbol == inUnit.mSymbol
      && mOffset == inUnit.mOffset
      && mGain == inUnit.mGain;
}

PhysicalUnit&
PhysicalUnit::operator*=( const PhysicalUnit& inUnit )
{
  if( mOffset != 0 || inUnit.mOffset != 0 )
    throw std_logic_error << "Cannot multiply physical units with offsets";
  mSymbolPowers *= inUnit.mSymbolPowers;
  mSymbol = mSymbolPowers.SingleSymbol();
  mGain *= inUnit.mGain;
  ValueType max1max2 = mRawMax * inUnit.mRawMax,
            max1min2 = mRawMax * inUnit.mRawMin,
            min1max2 = mRawMin * inUnit.mRawMax,
            min1min2 = mRawMin * inUnit.mRawMin;
  mRawMax = std::max( std::max( max1max2, max1min2 ), std::max( min1max2, min1min2 ) );
  mRawMin = std::min( std::min( max1max2, max1min2 ), std::min( min1max2, min1min2 ) );
  return *this;
}

PhysicalUnit
PhysicalUnit::operator*( const PhysicalUnit& inOther ) const
{
  return PhysicalUnit( *this ) *= inOther;
}

PhysicalUnit&
PhysicalUnit::Combine( const PhysicalUnit& inUnit )
{
  if( *this != inUnit )
  {
    mGain = 1;
    mOffset = 0;
    mSymbol = "";
  }
  mRawMax = std::max( mRawMax, inUnit.mRawMax );
  mRawMin = std::min( mRawMin, inUnit.mRawMin );
  return *this;
}

std::ostream&
PhysicalUnit::InsertInto(std::ostream& os ) const
{
  return os << std::setprecision( 10 )
            << mOffset << ' '
            << mGain << ' '
            << mSymbol << ' '
            << mRawMin << ' '
            << mRawMax;
}

std::istream&
PhysicalUnit::ExtractFrom(std::istream& is )
{
  EncodedString symbol;
  is >> mOffset
     >> mGain
     >> symbol
     >> mRawMin
     >> mRawMax;
  SetSymbol( symbol );
  return is;
}

PhysicalUnit::SymbolPowers&
PhysicalUnit::SymbolPowers::operator*=( const SymbolPowers& inP )
{
  for( const_iterator i = inP.begin(); i != inP.end(); ++i )
    ( *this )[i->first] += i->second;
  return *this;
}

std::string
PhysicalUnit::SymbolPowers::SingleSymbol() const
{
  std::ostringstream oss;
  for( const_iterator i = begin(); i != end(); ++i )
    if( i->second != 0 )
    {
      oss << i->first;
      if( i->second != 1 )
        oss << '^' << i->second;
    }
  return oss.str();
}

PhysicalUnit::Pair::operator EncodedString() const
{
  return operator std::string();
}

PhysicalUnit::Pair::operator std::string() const
{
  std::ostringstream oss;
  oss << *this;
  return oss.str();
}

static std::string ToSuperscript( char c )
{
  std::string s;
  switch( c )
  {
    case '1':
      s = "\xc2\xb9";
      break;
    case '2':
      s = "\xc2\xb2";
      break;
    case '3':
      s = "\xc2\xb3";
      break;
    case '0':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
      s ="\xe2\x81\xb0";
      s.back() += ( c - '0' );
      break;
    case '+':
      s ="\xe2\x81\xba";
      break;
    case '-':
      s ="\xe2\x81\xbb";
      break;
    default:
      s = c;
  }
  return s;
}

static std::string ToSuperscript( const std::string& s )
{
  std::string r;
  for( char c : s )
    r += ToSuperscript( c );
  return r;
}

PhysicalUnit::Pair
PhysicalUnit::Pair::Utf8() const
{
  if( unit.length() < 2 )
    return *this;
  if( unit[0] == 'e' && !::isalpha( unit[1] ) )
  {
    Pair p;
    p.value = value;
    p.unit = "\xe2\x8b\x85" /* UTF-8 DOT OPERATOR */ "10" + ToSuperscript( unit.substr( 1 ) );
    return p;
  }
  else if( unit.find( "mu" ) == 0 )
  {
    Pair p;
    p.value = value;
    p.unit = "\xc2\xb5" /* UTF-8 MICRO SIGN */ + unit.substr( 2 );
    return p;
  }
  return *this;
}
