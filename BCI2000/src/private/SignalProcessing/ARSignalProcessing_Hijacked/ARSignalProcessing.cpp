/* $BEGIN_BCI2000_LICENSE$
 * 
 * This file is part of BCI2000, a platform for real-time bio-signal research.
 * [ Copyright (C) 2000-2012: BCI2000 team and many external contributors ]
 * 
 * BCI2000 is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 * 
 * BCI2000 is distributed in the hope that it will be useful, but
 *                         WITHOUT ANY WARRANTY
 * - without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 * $END_BCI2000_LICENSE$
 */
#include "PCHIncludes.h"
#pragma hdrstop

#ifdef __BORLANDC__

#include <vcl.h>
#include "CoreModuleVCL.h"

#else // __BORLANDC__

// Not using borland, we'll use QT instead
#include "CoreModule_Qt.h"

#endif // __BORLANDC__

#ifdef _WIN32

int WINAPI
WinMain( HINSTANCE, HINSTANCE, LPSTR, int )
{
#ifdef __BORLANDC__

  try
  {
	Application->Initialize();
	CoreModuleVCL().Run( _argc, _argv );
  }
  catch (Exception &exception)
  {
	Application->ShowException(&exception);
  }
  return 0;

#else // __BORLANDC__

  bool success = CoreModuleQT().Run( __argc, __argv );
  return success ? 0 : -1;

#endif // __BORLANDC__
}

#else // _WIN32

int main( int argc, char *argv[] )
{
  bool success = CoreModuleQT().Run( argc, argv );
  return success ? 0 : -1;
}

#endif // _WIN32

