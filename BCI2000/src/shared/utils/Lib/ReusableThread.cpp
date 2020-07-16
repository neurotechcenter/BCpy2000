//////////////////////////////////////////////////////////////////////
// $Id: ReusableThread.cpp 5268 2016-03-04 14:54:03Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A class that wraps a thread, and allows clients to
//   run code inside that thread. Unlike OSThread, which starts a new
//   thread each time its Start() function is called, an instance of
//   ReusableThread is bound to a single thread during its lifetime, and
//   re-uses that thread for each call to Run(). After calling Run()
//   for a Runnable, call Wait() to wait until execution of the
//   Runnable has finished.
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
#include "ReusableThread.h"

#include "Runnable.h"
#include "Thread.h"
#include "WaitableEvent.h"
#include "Exception.h"

struct ReusableThread::Private : Thread
{
  ~Private() { Thread::TerminateAndWait(); }
  int OnExecute();

  WaitableEvent mStartEvent, mFinishedEvent;
  Synchronized<bool> mAlive;
  Synchronized<Runnable*> mpRunnable;
};

ReusableThread::ReusableThread()
: p( new Private )
{
  p->mAlive = true;
  p->mFinishedEvent.Set();
  Waitable::AssociateWith( p->mFinishedEvent );
  p->Thread::Start();
}

ReusableThread::~ReusableThread()
{
  p->mStartEvent.AbortWaiting();
  delete p;
}

bool
ReusableThread::Run( Runnable& inRunnable )
{
  if( !p->mAlive )
    throw std_runtime_error << "Thread is no longer available for execution";
  if( p->mpRunnable.Atomic().IfNull().Exchange( &inRunnable ) )
    return false;
  p->mFinishedEvent.Reset();
  p->mStartEvent.Set();
  return true;
}

bool
ReusableThread::Alive() const
{
  return p->mAlive;
}

int
ReusableThread::Private::OnExecute()
{
  while( mStartEvent.Wait() )
  {
    mStartEvent.Reset();
    try
    {
      mpRunnable->Run();
    }
    catch( ... )
    {
      mAlive = false;
      mFinishedEvent.Set();
      throw;
    }
    mpRunnable = 0;
    mFinishedEvent.Set();
  }
  mFinishedEvent.AbortWaiting();
  mAlive = false;
  return 0;
}

