////////////////////////////////////////////////////////////////////////////////
// $Id: MessageBox.cpp 4214 2012-06-30 14:15:25Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A program that displays a message box. This is to allow for
//   asynchronous message boxes in windows that do not hinder program
//   termination. On Unix systems, there is always a terminal to display
//   error messages.
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
#include <Windows.h>
#include <sstream>
#include <vector>
#include <cstdlib>
#include "ParserToken.h"

#if _MSC_VER
# define strtoull _strtoui64
#endif

using namespace std;

int WINAPI
WinMain( HINSTANCE, HINSTANCE, LPSTR inCmdLine, int )
{
  istringstream iss( inCmdLine );
  vector<string> args;
  ParserToken arg;
  while( iss >> arg )
    args.push_back( arg );
  string message, title = "Message";
  HWND hwnd = NULL;
  int flags = 0;
  switch( args.size() )
  {
    case 1:
      message = args[0];
      break;
    case 2:
      message = args[0];
      title = args[1];
      break;
    case 3:
      message = args[0];
      title = args[1];
      flags = ::strtol( args[2].c_str(), NULL, 0 );
      break;
    case 4:
      hwnd = reinterpret_cast<HWND>( ::strtoull( args[0].c_str(), NULL, 0 ) );
      message = args[1];
      title = args[2];
      flags = ::strtol( args[3].c_str(), NULL, 0 );
      break;
    default:
      return -1;
  }
  return ::MessageBoxA( hwnd, message.c_str(), title.c_str(), flags );
}
