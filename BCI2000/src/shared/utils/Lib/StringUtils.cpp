//////////////////////////////////////////////////////////////////////
// $Id: StringUtils.cpp 5722 2018-03-17 18:31:27Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: String-conversion related utility functions.
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
#include "StringUtils.h"
#include "Numeric.h"
#include "UnitTest.h"

#if _WIN32
# include <Windows.h>
#else // _WIN32
# include <locale>
#endif // _WIN32

#include <cstdint>
#include <cstring>
#include <ctime>
#include <cstdlib>
#include <sstream>
#include <functional>
#include <algorithm>

namespace
{

template<class T> std::string& FloatToString( std::string& s, T f )
{
  std::ostringstream oss;
  oss << Tiny::Pretty( f );
  return s = oss.str();
}

template<class T> T StringToFloat( const std::string& s )
{
  T t;
  std::istringstream iss( s );
  if( !( iss >> t ) || iss.peek() != EOF )
    t = Tiny::NaN( t );
  return t;
}

}

namespace Tiny
{
namespace StringUtils
{

const String String::Null;

void
String::Append( const std::ostringstream* oss )
{
  if( mMulti && !empty() )
    append("\n").append( oss->str() );
  else
    assign( oss->str() );
}

String String::Trimmed() const { return StringUtils::Trim(*this); }
std::vector<String> String::Split(const String& s) const { return StringUtils::Split<std::vector<String>>(*this, s); }
String String::ToLower() const { return StringUtils::ToLower(*this); }
String String::ToUpper() const { return StringUtils::ToUpper(*this); }

bool FromString(std::string& s2, const std::string& s1) { s2 = s1; return true; }
std::string& ToString( std::string& s2, const std::string& s1 ) { return s2 = s1; }
std::string& ToString( std::string& s, double d ) { return FloatToString( s, d ); }
std::string& ToString( std::string& s, float f ) { return FloatToString( s, f ); }
std::string& ToString( std::string& s, long double ld ) { return FloatToString( s, ld ); }
template<> double To<double>( const std::string& s ) { return StringToFloat<double>( s ); }
template<> float To<float>( const std::string& s ) { return StringToFloat<float>( s ); }
template<> long double To<long double>( const std::string& s ) { return StringToFloat<long double>( s ); }

NameValueList::Super::iterator
NameValueList::find(const String& s)
{
  return std::find_if(
    this->begin(), this->end(),
    [s](const value_type& v) { return v.first == s; }
  );
}

NameValueList::Super::const_iterator
NameValueList::find(const String& s) const
{
  return std::find_if(
    this->begin(), this->end(),
    [s](const value_type& v) { return v.first == s; }
  );
}

String&
NameValueList::operator[](const String& s)
{
  auto i = find(s);
  if(i == end())
  {
    push_back(std::make_pair(s, ""));
    return back().second;
  }
  return i->second;
}

const String&
NameValueList::operator[](const String& s) const
{
  auto i = find(s);
  if(i == end())
    return String::Null;
  return i->second;
}

std::wstring
ToWide( const char* inString )
{
#if _WIN32
  const int count = ::MultiByteToWideChar( CP_ACP, 0, inString, -1, NULL, 0 );
  wchar_t* pBuffer = new wchar_t[count];
  ::MultiByteToWideChar( CP_ACP, 0, inString, -1, pBuffer, count );
  std::wstring result( pBuffer );
  delete[] pBuffer;
#else // _WIN32
  std::locale loc;
  size_t length = ::strlen( inString );
  wchar_t* pBuffer = new wchar_t[length + 1];
  pBuffer[length] = 0;
  std::use_facet< std::ctype<wchar_t> >( loc ).widen( inString, inString + length, pBuffer );
  std::wstring result( pBuffer );
  delete[] pBuffer;
#endif // _WIN32
  return result;
}

std::string
ToNarrow( const wchar_t* inString )
{
#if _WIN32
  const int count = ::WideCharToMultiByte( CP_ACP, 0, inString, -1, NULL, 0, NULL, NULL );
  char* pBuffer = new char[count];
  ::WideCharToMultiByte( CP_ACP, 0, inString, -1, pBuffer, count, NULL, NULL );
  std::string result( pBuffer );
  delete[] pBuffer;
#else // _WIN32
  std::locale loc;
  size_t length = ::wcslen( inString );
  char* pBuffer = new char[length + 1];
  pBuffer[length] = 0;
  std::use_facet< std::ctype<wchar_t> >( loc ).narrow( inString, inString + length, '?', pBuffer );
  std::string result( pBuffer );
  delete[] pBuffer;
#endif // _WIN32
  return result;
}

const std::string WhiteSpace = " \n\t\r";

std::string
LStrip( const std::string& s, const std::string& chars )
{
  size_t pos = s.find_first_not_of( chars );
  return pos == s.npos ? "" : s.substr( pos );
}

std::string
RStrip( const std::string& s, const std::string& chars )
{
  size_t pos = s.find_last_not_of( chars );
  return pos == s.npos ? "" : s.substr( 0, pos + 1 );
}

std::string
Strip( const std::string& s, const std::string& chars )
{
  return RStrip( LStrip( s, chars ), chars );
}

std::string
Join(const std::string& j, const std::string& s1, const std::string& s2)
{
  if(s1.empty())
    return s2;
  else if(s2.empty())
    return s1;
  return s1 + j + s2;
}

std::string
ReplaceAll( const std::string& s, const std::string& what, const std::string& with )
{
  std::string r = s;
  for( size_t pos = r.find( what ); pos != std::string::npos; pos = r.find( what, pos + with.length() ) )
    r = r.substr( 0, pos ) + with + r.substr( pos + what.length() );
  return r;
}

std::wstring
ToUpper( const std::wstring& s )
{
  std::wstring result = s;
  for( std::wstring::iterator i = result.begin(); i != result.end(); ++i )
    *i = ::towupper( *i );
  return result;
}

std::wstring
ToLower( const std::wstring& s )
{
  std::wstring result = s;
  for( std::wstring::iterator i = result.begin(); i != result.end(); ++i )
    *i = ::towlower( *i );
  return result;
}

bool
CiLess( const std::wstring& a, const std::wstring& b )
{
  for( size_t i = 0; ; ++i )
    if( i == a.length() || i == b.length() )
      return a.length() < b.length();
    else
    {
      wchar_t a_ = ::towlower( a[i] ),
              b_ = ::towlower( b[i] );
      if( a_ != b_ )
        return a_ < b_;
    }
  return false;
}

bool
CiLess( const std::string& a, const std::string& b )
{
  return CiLess( ToWide( a ), ToWide( b ) );
}


static const char cBase64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";
static const uint8_t cBase64Fill = 64;
static uint8_t cInvBase64[256] = "";

static int
InitBase64()
{
  for( size_t i = 0; i < sizeof( cInvBase64 ); ++i )
    cInvBase64[i] = 255;
  for( uint8_t i = 0; i < sizeof( cBase64 ); ++i )
    cInvBase64[cBase64[i]] = i;
  return 0;
}
static int sInitBase64 = InitBase64();

std::ostream&
WriteAsBase64( std::ostream& os, const std::string& s )
{
  const uint32_t mask64 = ( 1 << 6 ) - 1;
  size_t pos = 0;
  uint32_t triplet = 0;
  bool done = false,
       atEnd = false;
  int count = 0,
      rem = 0;
  while( !done )
  {
    if( pos >= s.length() )
    {
      atEnd = true;
      done = ( count == 0 );
    }
    uint32_t c = atEnd ? 0 : static_cast<uint8_t>( s[pos++] );
    if( atEnd )
      ++rem;
    triplet <<= 8;
    triplet |= c;
    ++count %= 3;
    if( 0 == count )
    {
      for( int i = 3; i >= rem; --i )
        os.put( cBase64[( triplet >> ( 6 * i ) ) & mask64] );
      for( int i = 0; i < rem; ++i )
        os.put( cBase64[cBase64Fill] );
      triplet = 0;
    }
  }
  return os;
}

namespace {
template<typename T>
std::istream&
ReadAsBase64_( std::istream& is, std::string& s, T stopIf )
{
  s.clear();
  const uint32_t mask256 = ( 1 << 8 ) - 1;
  bool done = false,
       atEnd = false;
  uint32_t triplet = 0;
  int rem = 0, count = 0;
  while( !done )
  {
    int c = cBase64[cBase64Fill];
    if( !atEnd )
    {
      c = is.get();
      if( stopIf( c ) || c >= 256 || c < 0 )
      {
        atEnd = true;
        done = ( count == 0 );
      }
    }
    uint32_t code = cInvBase64[c];
    if( code == cBase64Fill )
    {
      ++rem;
      code = 0;
    }
    if( code < cBase64Fill )
    {
      triplet <<= 6;
      triplet |= code;
      if( ++count == 4 )
      {
        for( int i = 2; i >= rem; --i )
          s += static_cast<char>( triplet >> ( 8 * i ) & mask256 );
        count = 0;
        rem = 0;
        triplet = 0;
        done = atEnd;
      }
    }
  }
  return is;
}
struct ConstFalse
{
  bool operator()( int ) { return false; }
};
} // namespace

std::istream&
ReadAsBase64( std::istream& is, std::string& s, int (*stopIf)( int ) )
{
  return stopIf ? ReadAsBase64_( is, s, stopIf ) : ReadAsBase64_( is, s, ConstFalse() );
}

std::istream&
ReadAsBase64( std::istream& is, std::string& s, int stopAt )
{
  return ReadAsBase64_( is, s, std::bind1st( std::equal_to<int>(), stopAt ) );
}

} // namespace StringUtils
} // namespace Tiny

UnitTest( Base64Test )
{
  unsigned int seed = static_cast<unsigned int>( ::time( NULL ) );
  ::srand( seed );
  for( int i = 0; i < 100; ++i )
  {
    std::string s;
    while( rand() >= RAND_MAX / 100 )
      s += static_cast<char>( rand() % 256 );
    std::stringstream stream;
    StringUtils::WriteAsBase64( stream, s );
    std::string s2;
    StringUtils::ReadAsBase64( stream, s2 );
    TestFail_if( s != s2, "seed: " << seed << ", iteration: " << i );
  }
  ::srand( 1 );
}

