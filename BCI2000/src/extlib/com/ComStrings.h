////////////////////////////////////////////////////////////////////////////////
// $Id: ComStrings.h 3922 2012-04-05 14:26:20Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: Two string classes that facilitate interoperation with COM
//   objects.
//
//   BString is a class that holds a COM-type BSTR pointer, and automatically
//   frees and allocates memory on assignment.
//
//   DualString is a std::wstring descendant that conveniently transforms
//   itself into a std::string, and vice versa.
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
#ifndef COM_STRINGS_H
#define COM_STRINGS_H

#include <Windows.h>
#include <string>

namespace com {

class BString
{
 public:
  BString() : mp( NULL ) {}
  BString( const wchar_t* p ) : mp( Copy( p ) ) {}
  ~BString() { Free(); }
  BString& operator=( const wchar_t* p ) { Free(); mp = Copy( p ); return *this; }
  operator wchar_t*() { return mp; }
  operator const wchar_t*() const { return mp; }
  wchar_t** operator&() { Free(); return &mp; }
  bool operator==( const wchar_t* p ) { return ( !p && !mp ) || ( p && mp && !::wcscmp( mp, p ) ); }
  bool operator!=( const wchar_t* p ) { return !operator==( p ); }

 private:
  wchar_t* Copy( const wchar_t* p ) { return ::SysAllocString( p ); }
  void Free() { if( mp ) ::SysFreeString( mp ); mp = NULL; }
  wchar_t* mp;
};

class DualString : public std::wstring
{
 public:
  DualString() {}
  DualString( const std::wstring& w ) : std::wstring( w ) {}
  DualString( const wchar_t* p ) : std::wstring( p ) {}
  DualString( const std::string& s ) : std::wstring( FromNarrow( s.c_str() ) ) {}
  DualString( const char* p ) : std::wstring( FromNarrow( p ) ) {}
  DualString& operator=( const wchar_t* p ) { std::wstring::operator=( p ); return *this; }
  DualString& operator=( const char* p ) { return *this = FromNarrow( p ); }
  operator std::string() const { return ToNarrow( c_str() ); }
  operator const wchar_t*() const { return c_str(); }
  DualString ToWin() const;

 private:
  static std::wstring FromNarrow( const char* );
  static std::string ToNarrow( const wchar_t* );
};

} // namespace

#endif // COM_STRINGS_H
