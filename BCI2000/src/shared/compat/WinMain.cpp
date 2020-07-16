////////////////////////////////////////////////////////////////////////////////
// $Id: ProcessUtils.cpp 4878 2015-07-01 08:45:06Z mellinger $
// Authors: juergen.mellinger@uni-tuebingen.de
// Description: A WinMain() function for Windows GUI apps that calls the
//   standard main() function, and sets up stdio.
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
#if _WIN32
# include <Windows.h>

# ifndef WINDOWS_ENABLE_STDIO
#  define WINDOWS_ENABLE_STDIO HAVE_LIB_TINY
# endif

# if WINDOWS_ENABLE_STDIO
#  include "ProcessUtils.h"
# endif

int main( int, char** );

int WINAPI
WinMain( HINSTANCE, HINSTANCE, LPSTR, int )
{
# if WINDOWS_ENABLE_STDIO
  ProcessUtils::WindowsEnableStdio( __argc, __argv );
# endif
  return main( __argc, __argv );
}

#endif // _WIN32
