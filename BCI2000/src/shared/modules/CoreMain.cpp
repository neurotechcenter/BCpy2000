////////////////////////////////////////////////////////////////////////////////
// $Id: CoreMain.cpp 5387 2016-05-31 15:47:36Z mellinger $
// Description: main() function definition for core modules.
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
#include "CoreModule.h"
#include "Debugging.h"
#if USE_QT
# include "CoreModule_Qt.h"
#endif

int
main( int argc, char** argv )
{
  MemoryDebugging::Init init;
#if USE_QT
  CoreModuleQt module;
#else
  CoreModule module;
#endif
  return module.Run( argc, argv ) ? 0 : -1;
}

