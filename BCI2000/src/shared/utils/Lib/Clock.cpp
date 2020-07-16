//////////////////////////////////////////////////////////////////////
// $Id: Clock.cpp 5076 2015-10-28 02:27:26Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: An object that triggers an event at regular time intervals,
//   using the most accurate timing source available on the machine.
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
#include "Clock.h"

#include "TimeUtils.h"
#include "ThreadUtils.h"
#include "Thread.h"
#include "WaitableEvent.h"

using namespace Tiny;

struct Clock::Data : Thread
{
  WaitableEvent mWaitable;
  Synchronized<Time::Interval> mInterval;

  Data();
  ~Data();
  int OnExecute();
};

Clock::Data::Data()
: mWaitable( true ),
  mInterval( 0 )
{
}

Clock::Data::~Data()
{
  Thread::TerminateAndWait();
}

Clock::Clock()
: d( new Data )
{
  Waitable::AssociateWith( d->mWaitable );
}

Clock::~Clock()
{
  Stop();
  delete d;
}

Clock&
Clock::SetInterval( Time::Interval t )
{
  d->mInterval = t;
  return *this;
}

Time::Interval
Clock::Interval() const
{
  return d->mInterval;
}

bool
Clock::Start()
{
  d->mWaitable.Reset();
  d->Start();
  return d->Running();
}

bool
Clock::Stop()
{
  d->Terminate();
  AbortWaiting();
  d->mWaitable.Reset();
  return true;
}

int
Clock::Data::OnExecute()
{
  ThreadUtils::Priority::Set( ThreadUtils::Priority::Maximum - 1 );
  Waitable::Timeout timer;
  Time nextTime = TimeUtils::MonotonicTime();
  Time::Interval waitFor = mInterval;
  while( timer.Wait( waitFor ) )
  {
    mWaitable.Set();
    nextTime += mInterval;
    waitFor = nextTime - TimeUtils::MonotonicTime();
  }
  return 0;
}

