//////////////////////////////////////////////////////////////////////
// $Id: Waitable.h 4657 2013-11-29 12:21:30Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A wrapper for event objects.
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
#ifndef TINY_WAITABLE_IMPLEMENTATION_H
#define TINY_WAITABLE_IMPLEMENTATION_H

#include "Waitable.h"
#include "Synchronized.h"
#include "sockstream.h"

namespace Tiny
{

enum WaitFlags
{
  WaitNullResult = 1 << 0,
  WaitRetry = 1 << 1,

  WaitAllowAbort = 1 << 8,
  WaitAbortNow = 1 << 9,
  WaitRestart = 1 << 10,
};

enum SignaledMask
{
  Readable = 1, Writable = 2,
  Signaled = Readable,
};

class Waitable::Condition
{
public:
  Condition();
  virtual ~Condition();

public:
  Condition& SetFd( fdio::fd_type fd ) { mFd = fd; return *this; }
  fdio::fd_type Fd() const { return mFd; }

  Condition& SetMask( int mask ) { mMask = mask; return *this; }
  int Mask() const { return mMask; }

  Condition& SetEnabled( bool b ) { mEnabled = true; return *this; }
  bool Enabled() const { return mEnabled; }

  bool TestForWaitResult();
  bool AbortWaiting( bool restart = false );

  void Lock();
  void Unlock();

  static const Waitable* Wait( const Waitable* const*, size_t, const Time::Interval& );
  static int Wait( Waitable::ConditionPtr*, int, Time::Interval& );

protected:
  virtual void OnIsWaitResult() {}

private:
  bool mEnabled;
  int mMask;
  fdio::fd_type mFd;
  void* mpMutex;
};

class Waitable::EventCondition : public Waitable::Condition
{
public:
  EventCondition( bool autoReset );
  ~EventCondition();
  void Set();
  void Reset();
protected:
  void OnIsWaitResult() override;
private:
  struct Private;
  Private* p;
};

} // namespace

#endif // TINY_WAITABLE_IMPLEMENTATION_H
