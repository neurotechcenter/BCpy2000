//////////////////////////////////////////////////////////////////////
// $Id: EventQueue.h 4731 2014-07-03 15:18:03Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A thread-safe event queue.
//   An event is defined by a string description, and a time stamp.
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
#ifndef EVENT_QUEUE_H
#define EVENT_QUEUE_H

#include "SynchronizedQueue.h"
#include "PrecisionTime.h"

class EventQueue
{
  typedef SynchronizedQueue< std::pair<std::string, PrecisionTime> > Queue;

 public:
  EventQueue( int maxCount )
    : mEventsAllowed( false ),
      mCount( 0 ),
      mMaxCount( maxCount )
    {}
  void SetMaxCount( int maxCount )
    { mMaxCount = maxCount; }
  void AllowEvents()
    { mEventsAllowed = true; }
  void DenyEvents()
    { mEventsAllowed = false; }
  bool IsEmpty();
  void PushBack( const std::string& inDescriptor, PrecisionTime );
  void PopFront();
  const std::string& FrontDescriptor() const;
  PrecisionTime FrontTimeStamp() const;

 private:
  Queue mQueue;
  mutable Queue::Consumable mFront;
  Synchronized<bool> mEventsAllowed;
  Synchronized<int> mCount, mMaxCount;
};

#endif // EVENT_QUEUE_H
