////////////////////////////////////////////////////////////////////////////////
// $Id: main.cpp 4659 2013-12-03 16:22:46Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: The main() function of the BCI2000Viewer tool.
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
#include <QApplication>
#include "ExceptionCatcher.h"
#include "Settings.h"
#include "BCI2000Viewer.h"

int main(int argc, char *argv[])
{
  QApplication a( argc, argv );
  a.setOrganizationName( "BCI2000" );
  a.setOrganizationDomain( "bci2000.org" );
  a.setApplicationName( "BCI2000Viewer" );
  BCI2000Viewer w;
  w.show();

  std::string message = "Aborting ";
  message += a.applicationName().toLocal8Bit().constData();
  FunctionCall< int() >
    call( &QApplication::exec );
  bool finished = ExceptionCatcher()
    .SetMessage( message )
    .Run( call );
  return finished ? call.Result() : -1;
}
