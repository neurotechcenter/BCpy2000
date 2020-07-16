////////////////////////////////////////////////////////////////////////////////
// $Id$
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A mechanism to run a LibTiny::Runnable in response to a
//   Qt signal. Useful/necessary because Qt's moc does not support
//   nested classes, i.e. private implementation classes would be unable
//   to receive Qt signals otherwise.
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
#ifndef QT_SIGNAL_CONNECTOR_H
#define QT_SIGNAL_CONNECTOR_H

#include "Runnable.h"
#include <QObject>
#include <vector>

class QtSignalLink : public QObject
{
Q_OBJECT
public:
  template<class T> QtSignalLink( QObject* p, const T& t ) : QObject( p ), mpRunnable( new T( t ) ) {}
  ~QtSignalLink() { delete mpRunnable; }
public slots:
  void OnSignal() { mpRunnable->Run(); }
private:
  Runnable* mpRunnable;
};

class QtSignalConnector
{
public:
  template<class T> QtSignalConnector& Connect( QObject* sender, const char* signal, const T& functionCall )
  {
    QtSignalLink* p = new QtSignalLink( &mObject, functionCall );
    p->connect( sender, signal, p, SLOT(OnSignal()) );
    return *this;
  }
private:
  QObject mObject;
};

#endif // QT_SIGNAL_CONNECTOR_H
