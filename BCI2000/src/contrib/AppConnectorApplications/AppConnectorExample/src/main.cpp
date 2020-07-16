/***************************************************************************
 *   Copyright (C) 2007 by J. Adam Wilson   *
 *   jawilson@cae.wisc.edu   *
 *                                                                         *
 * $BEGIN_BCI2000_LICENSE$
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
 ***************************************************************************/


#include <QCoreApplication>
#include "mainUI.h"

/*
Qt programs require a main function, and will generally use a separate class to
handle any form generation. The form is usually made by inherting from a
QMainWindow or QWidget class in the Qt4 library. In our case, the form is
generated in the mainUI.h/cpp files. All the main function has to do is
create a QApplication object (app), declare our mainUI class (mainWin),
use the show function on mainWin, and then execute the app.
*/
int main(int argc, char *argv[])
{
  QApplication app(argc, argv);
  mainUI mainWin;
  mainWin.show();
  return app.exec();
}
