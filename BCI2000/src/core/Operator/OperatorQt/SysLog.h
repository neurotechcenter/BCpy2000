//////////////////////////////////////////////////////////////////////
// $Id: SysLog.h 5298 2016-03-22 18:27:19Z mellinger $
// Authors: schalk@wadsworth.org, juergen.mellinger@uni-tuebingen.de
// Description: The Operator module's log window.
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
#ifndef SYSLOG_H
#define SYSLOG_H

#include <QString>
#include <QTextBrowser>
#include <QMutex>
#include <QDialog>

class MainWindow;

class SysLog : public QDialog
{
  Q_OBJECT

 public:
  enum LogEntryMode
  {
    logEntryNormal = 0,
    logEntryWarning,
    logEntryError,
    numLogEntryModes
  };
  SysLog( MainWindow* );
  virtual ~SysLog();
  bool HasUndisplayedErrors() { return !mErrorsDisplayedSoFar; }

 public slots:
  void AddEntry( const QString& text, int = logEntryNormal, bool = false );

 protected:
  void moveEvent( QMoveEvent* ) override;
  void showEvent( QShowEvent* ) override;
  void paintEvent( QPaintEvent* ) override;
  void closeEvent( QCloseEvent* ) override;

 private:
  bool MatchURL( const QString&, int&, int& );
  void Show( bool force );
  
  MainWindow* mpParent;
  QTextBrowser* mpLog;
  QTextCharFormat mDefaultFormat;
  QMutex mCritsec;
  bool mEmpty,
       mAttachToParent,
       mErrorsDisplayedSoFar;
  enum { normal, closeDenied, closed } mState;
  QString mLastLine;
  QStringMatcher mURLMatcher;
};

#endif // SYSLOG_H
