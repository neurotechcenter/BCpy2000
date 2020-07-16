//////////////////////////////////////////////////////////////////////
// $Id: ThreadUtils.h 5455 2016-07-19 01:12:55Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: Thread-related utility functions.
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
#ifndef TINY_THREAD_UTILS_H
#define TINY_THREAD_UTILS_H

#include "Synchronized.h"
#include "TimeUtils.h"

#define WithThreadPriority( x ) for( bool b__ = true; b__; ) \
  for( ThreadUtils::Priority::TemporaryChange c__( x ); b__; b__ = false )

#define WithThreadTask( x, y ) for( bool b__ = true; b__; ) \
  for( ThreadUtils::Task::Performing p__( x, y ); b__; b__ = false )

namespace Tiny
{

namespace ThreadUtils
{

bool InMainThread();

// Thread continuation depends on a change in state
// to be performed by a different thread.
// To avoid lockup, Idle() makes sure that all other threads
// can run.
void Idle();

void SleepForMs( int ms );
void SleepFor( Time::Interval );
int NumberOfProcessors();

class ID
{
 public:
  static const ID& None();
  static const ID& Main();
  static ID Current();

  ~ID();
  bool operator==( const ID& ) const;
  bool operator!=( const ID& ) const;
  bool operator<( const ID& ) const;
  operator bool() const;

  std::ostream& Print( std::ostream& ) const;

 protected:
  ID();
  ID( bool );
  const void* OSData() const;
 private:
  Synchronized<void*> mData;
};

// In a target thread, raise a signal to be handled by the C signal mechanism.
bool RaiseSignal( int, const ID&, Time::Interval = Time::Forever );

namespace Priority
{
  bool Set( int );
  bool Increase( int = 1 );
  bool Decrease( int = 1 );
  bool ChangeBy( int );

  static const int Minimum = -( 1 << 16 ),
                   Current = 0,
                   Maximum = 1 << 16;
  class TemporaryChange
  {
  public:
    explicit TemporaryChange( int );
    ~TemporaryChange();
  private:
    int mSaved;
  };
}

class Task
{
 public:
  Task( const std::string& );
  ~Task();
  class Performing
  {
  public:
    Performing( Task&, int priority );
    Performing( Task*, int priority );
    Performing( const std::string&, int priority );
    ~Performing();
  private:
    Task* mpOwnedTask;
    void* mpData;
  };
 private:
  struct Private;
  Private* p;
};

class TimeSlice
{
 public:
  TimeSlice( Time::Interval );
  ~TimeSlice();
  static Time::Interval Duration();
 private:
  struct Data;
  Data* d;
};

} // namespace ThreadUtils

} // namespace Tiny

inline std::ostream& operator<<( std::ostream& os, const Tiny::ThreadUtils::ID& id ) { return id.Print( os ); }

namespace ThreadUtils = Tiny::ThreadUtils;

#endif // TINY_THREAD_UTILS_H

