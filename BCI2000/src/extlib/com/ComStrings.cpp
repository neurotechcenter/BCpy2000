////////////////////////////////////////////////////////////////////////////////
// $Id: ComStrings.cpp 3902 2012-04-04 14:15:42Z mellinger $
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
#include "ComStrings.h"

using namespace com;

std::wstring
DualString::FromNarrow( const char* inString )
{
  const int count = ::MultiByteToWideChar( CP_ACP, 0, inString, -1, NULL, 0 );
  wchar_t* pBuffer = new wchar_t[count];
  ::MultiByteToWideChar( CP_ACP, 0, inString, -1, pBuffer, count );
  std::wstring result( pBuffer );
  delete[] pBuffer;
  return result;
}

std::string
DualString::ToNarrow( const wchar_t* inString )
{
  const int count = ::WideCharToMultiByte( CP_ACP, 0, inString, -1, NULL, 0, NULL, NULL );
  char* pBuffer = new char[count];
  ::WideCharToMultiByte( CP_ACP, 0, inString, -1, pBuffer, count, NULL, NULL );
  std::string result( pBuffer );
  delete[] pBuffer;
  return result;
}

DualString
DualString::ToWin() const
{
  std::wstring result;
  for( const_iterator i = begin(); i != end(); ++i )
  {
    switch( *i )
    {
      case '\n':
        result += L"\r\n";
        break;
      default:
        result += *i;
    }
  }
  return result;
}
