//////////////////////////////////////////////////////////////////////
// $Id: ConnectionInfo.h 5291 2016-03-17 18:26:36Z mellinger $
// Authors: schalk@wadsworth.org, juergen.mellinger@uni-tuebingen.de
// Description: The Operator module's connection info dialog.
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
#ifndef CONNECTIONINFO_H
#define CONNECTIONINFO_H

#include <QDialog>

class ConnectionInfo : public QDialog
{
  Q_OBJECT
 public:
  ConnectionInfo( QWidget* parent = 0 );
  ~ConnectionInfo();

  void UpdateDisplay();

 protected:
  void showEvent( QShowEvent* ) override;

 private slots:
  void OnUpdateDisplay();

 private:
  struct Private;
  Private* p;
};

#endif // CONNECTIONINFO_H
