////////////////////////////////////////////////////////////////////////////////
// $Id$
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
#include "SignalSharingDemoClientApp.h"
#include "SignalSharingDemoWidget.h"
#include "QtMain.h"

#include <QApplication>

MainWindow::MainWindow()
{
  QString address = "localhost:1879";
  auto args = qApp->arguments();
  if(args.size() > 1)
    address = args.back();
  setCentralWidget(new SignalSharingDemoWidget(address));
}

int main(int argc, char** argv)
{
  QtApplication app(argc, argv);
  MainWindow w;
  w.show();
  return app.Run();
}


