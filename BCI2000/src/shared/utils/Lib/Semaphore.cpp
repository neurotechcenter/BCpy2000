//////////////////////////////////////////////////////////////////////
// $Id: Semaphore.cpp 5700 2017-08-21 18:26:41Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A wrapper for semaphore objects.
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
#include "Semaphore.h"
#include "WaitableImplementation.h"
#include "ThreadUtils.h"

namespace Tiny {

// Semaphore
struct Semaphore::Private
{
  SynchronizedObject<int> mpCount;
  Waitable::EventCondition* mpEventCondition;
  Waitable::ConditionPtr mConditionRef;
  Private()
  : mpEventCondition( new Waitable::EventCondition( false ) ),
    mConditionRef( mpEventCondition ) {}
};

Semaphore::Semaphore( int initialCount )
: p( new Private )
{
  Waitable::AssociateWith( p->mConditionRef );
  *p->mpCount.Mutable() = initialCount;
  if( initialCount > 0 )
    p->mpEventCondition->Set();
}

Semaphore::~Semaphore()
{
  while( AbortWaiting() )
    ThreadUtils::Idle();
  delete p;
}

bool
Semaphore::Increase()
{
  WithLocked( pCount = p->mpCount.Mutable() )
    if( ++*pCount == 1 )
      p->mpEventCondition->Set();
  return true;
}

bool
Semaphore::Decrease( const Time::Interval& inTimeout )
{
  bool done = false, didDecrease = false;
  Time expires = TimeUtils::MonotonicTime() + inTimeout;
  while( !done )
  {
    WithLocked( pCount = p->mpCount.Mutable() )
    {
      if( *pCount <= 1 ) // compare <= rather than == to account for missing lock atomicity
        p->mpEventCondition->Reset();
      if( *pCount > 0 )
      {
        done = true;
        didDecrease = true;
        --*pCount;
      }
    }
    if( !done )
    {
      Time::Interval waitFor = expires - TimeUtils::MonotonicTime();
      done = ( waitFor < 0 );
      if( !done )
      {
        int idx = Waitable::Condition::Wait( &p->mConditionRef, 1, waitFor );
        done = ( idx < 0 ); // aborted wait
      }
    }
  }
  return didDecrease;
}


} // namespace Tiny


