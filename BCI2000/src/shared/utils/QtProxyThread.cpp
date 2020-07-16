////////////////////////////////////////////////////////////////////////////////
// $Id$
// Author: mellinger@neurotechcenter.org
// Description: A QThread descendant integrated with Tiny::Thread.
//   Some Qt code requires to run within a QThread. Rather than making all
//   threads QThreads, we run the respective pieces of code inside an instance
//   of QtProxyThread.
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
#include "QtProxyThread.h"
#include "QtSignalConnector.h"
#include "Thread.h"
#include "ThreadUtils.h"
#include "WaitableEvent.h"
#include "ExceptionCatcher.h"
#include "Debugging.h"
#include <QThread>
#include <QApplication>

struct QtProxyThread::Private : QThread
{
  QtProxyThread* mpSelf;
  Synchronized<Runnable*> mpRunnable;
  Synchronized<Tiny::Thread*> mpTinyThread;
  WaitableEvent mRun, mDone;

  Private( QtProxyThread* p )
  : QThread( QApplication::instance() ),
    mRun( true ), mDone( false ),
    mpSelf( p )
  {
    QThread::start();
  }
  ~Private()
  {
    Halt();
  }
  void Halt()
  {
    mpSelf->p = nullptr;
    if( mpTinyThread )
      mpTinyThread->Terminate();
    mRun.AbortWaiting();
    mDone.Set();
    QThread::wait();
  }
  bool Run( Runnable* pRunnable )
  {
    Assert( pRunnable );
    bool wait = true;
    while( wait )
    {
      if( !mDone.Wait() )
        return false;
      if( !mpTinyThread )
        return false;
      wait = mpRunnable.Atomic().IfEqual( nullptr ).Exchange( pRunnable ) != nullptr;
    }
    mDone.Reset();
    mRun.Set();
    if( !mDone.Wait() )
      Halt();
    mpRunnable = nullptr;
    return true;
  }
  void run() override
  {
    MemberCall<void(QtProxyThread::Private*)>
      threadFunc( &QtProxyThread::Private::ThreadFunc, this );
    ExceptionCatcher().Run( threadFunc );
  }
  void ThreadFunc()
  {
    mpTinyThread = Tiny::Thread::AdoptAs( "QtProxyThread" );
    mDone.Set();
    WithThreadPriority( ThreadUtils::Priority::Maximum )
      while( mRun.Wait() )
      {
        Runnable* p = mpRunnable;
        if( p )
          p->Run();
        mDone.Set();
      }
    delete mpTinyThread.Atomic().Exchange( nullptr );
  }
};

QtProxyThread::QtProxyThread()
: p( new Private( this ) )
{
}

QtProxyThread::~QtProxyThread()
{
}

bool
QtProxyThread::Run( Runnable* pRunnable )
{
  Private* pPriv = p;
  return pPriv && pPriv->Run( pRunnable );
}

