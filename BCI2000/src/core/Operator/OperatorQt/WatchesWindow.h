//////////////////////////////////////////////////////////////////////
// $Id$
// Authors: juergen.mellinger@uni-tuebingen.de
// Description: A window displaying user-editable expressions.
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
#ifndef WATCHES_WINDOW_H
#define WATCHES_WINDOW_H

#include <QWidget>

class WatchesWindow : public QWidget
{
  Q_OBJECT
 public:
  static void AddWatch( const QString& );
  static void Initialize( class MainWindow* );
  static void Deinitialize();

 protected:
  void timerEvent( class QTimerEvent* ) override;
  void closeEvent( class QCloseEvent* ) override;

 private:
  WatchesWindow( class MainWindow* );
  ~WatchesWindow();
  void Update();
  struct Watch;

 private slots:
  void OnItemChanged( class QTableWidgetItem* );
  void OnValuesReceived( void*, QString, double );
  void OnHeaderDoubleClick( int );
  void AddEmptyLine();
  void RemoveLastLine();
  void SetHistoryLengthSeconds( double );
  void SaveWatches();
  void RestoreWatches();
  void ClearWatches();

 private:
  class QTableWidget* mpTable;
  class QAction* mpMenuEntry;
  double mHistoryLengthSeconds, mUpdateInterval, mLastUpdate;
};

#endif // WATCHES_WINDOW_H
