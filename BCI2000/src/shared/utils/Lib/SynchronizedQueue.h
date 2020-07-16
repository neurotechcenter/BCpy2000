//////////////////////////////////////////////////////////////////////
// $Id$
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A queue that implements synchronization between
//   any number of producer and consumer threads.
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
#ifndef TINY_SYNCHRONIZED_QUEUE_H
#define TINY_SYNCHRONIZED_QUEUE_H

#include "Synchronized.h"
#include "Lockable.h"
#include "Thread.h"
#include "Semaphore.h"
#include "WaitableEvent.h"
#include <functional>

namespace Tiny
{

template<class T> class SynchronizedQueue
: public Waitable, Uncopyable
{
 private:
  struct Ref;

 public:
  typedef T ValueType;
  SynchronizedQueue()
    : mpHead( 0 ), mpTail( 0 ), mSize( 0 )
    { AssociateWith( mMayConsume ); }
  ~SynchronizedQueue()
    { Clear(); }

  bool Empty() const
    { return mSize == 0; }
  int Size() const
    { return mSize; }
  void Clear();
  void Produce( const T& t )
    { Push( t ); mMayConsume.Increase(); }
  Ref Consume()
    { return AwaitConsumption( 0 ); }
  Ref AwaitConsumption( const Time::Interval& = Time::Forever );

 private:
  struct Element
  {
    Element( const T& t )
    : data( t ), pNext( 0 )
    {}
    Element* pNext;
    T data;
  };
  void Push( const T& );
  Element* Pop();

  Synchronized<int> mSize;
  SynchronizedObject<Element*> mpHead, mpTail;
  Semaphore mMayConsume;

 public:
  friend class Consumable;
  class Consumable
  {
   public:
    Consumable()
      : mp( 0 ) {}
    Consumable( const Ref& r )
      : mp( r ? r->Pop() : 0 ) {}
    Consumable& operator=( const Ref& r )
      { delete mp; mp = r ? r->Pop() : 0; return *this; }
    Consumable( Consumable&& c )
      { mp = c.mp; c.mp = 0; }
    ~Consumable()
      { delete mp; }
    T* operator->()
      { return Data(); }
    const T* operator->() const
      { return Data(); }
    operator T*()
      { return Data(); }
    operator const T*() const
      { return Data(); }
    T& operator *()
      { return *Data(); } 
    const T& operator *() const
      { return *Data(); } 
   private:
    T* Data() const
      { return mp ? &mp->data : 0; }
    typename SynchronizedQueue<T>::Element* mp;
  };
};

// SynchronizedQueue Implementation
template<class T>
struct SynchronizedQueue<T>::Ref
{
  Ref( SynchronizedQueue* q ) : p( q ) {}
  operator void*() const { return p; }
  SynchronizedQueue* operator->() const { return p; }
  SynchronizedQueue* p;
};

template<class T> void
SynchronizedQueue<T>::Clear()
{
  while( mMayConsume.Decrease( 0 ) )
    delete Pop();
}

template<class T> typename SynchronizedQueue<T>::Ref
SynchronizedQueue<T>::AwaitConsumption( const Time::Interval& timeout )
{
  return mMayConsume.Decrease( timeout ) ? this : 0;
}

template<class T> void
SynchronizedQueue<T>::Push( const T& t )
{
  Element* pNew = new Element( t );
  WithLocked( p = mpTail.Mutable() )
  {
    if( *p )
      (*p)->pNext = pNew;
    *p = pNew;
  }
  WithLocked( p = mpHead.Mutable() )
  {
    if( ++mSize == 1 )
      *p = pNew;
  }
}

template<class T> typename SynchronizedQueue<T>::Element*
SynchronizedQueue<T>::Pop()
{
  Element* pHead = 0;
  WithLocked( p = mpHead.Mutable() )
  {
    if( *p )
    {
      pHead = *p;
      *p = (*p)->pNext;
      if( --mSize == 0 )
        *p = 0;
    }
  }
  WithLocked( p = mpTail.Mutable() )
  {
    if( *p && *p == pHead )
      *p = (*p)->pNext;
  }
  return pHead;
}

template<class T, class U = void>
class AsyncQueue : private Thread
{
 protected:
  typedef void (*F)( U*, const T& );

 public:
  AsyncQueue( U*, F );
  ~AsyncQueue();
  bool Async() const;
  bool SetAsync( bool );
  void Produce( const T& );
  bool WaitAndConsume( const Time::Interval& );
  Waitable& Empty();
 private:
  int OnExecute();
  void Apply( const T& );

  SynchronizedQueue<T> mQueue;
  WaitableEvent mEmpty;
  U* mpUserData;
  F mFunc;
};

// AsyncQueue implementation
template<class T, class U>
AsyncQueue<T, U>::AsyncQueue( U* u, F f )
: mpUserData( u ), mFunc( f )
{
  mEmpty.Set();
}

template<class T, class U>
AsyncQueue<T, U>::~AsyncQueue()
{
  SetAsync( false );
}

template<class T, class U>
bool
AsyncQueue<T, U>::SetAsync( bool b )
{
  bool async = Async();
  if( b && !async )
    Thread::Start();
  else if( !b )
    Thread::TerminateAndWait();
  return async;
}

template<class T, class U>
bool
AsyncQueue<T, U>::Async() const
{
  return Thread::Running();
}


template<class T, class U>
void
AsyncQueue<T, U>::Apply( const T& t )
{
  mFunc( mpUserData, t );
  if( mQueue.Empty() ) 
    mEmpty.Set();
}

template<class T, class U>
void
AsyncQueue<T, U>::Produce( const T& t )
{
  mEmpty.Reset();
  if( Async() )
    mQueue.Produce( t );
  else
    Apply( t );
}

template<class T, class U>
bool
AsyncQueue<T, U>::WaitAndConsume( const Time::Interval& timeout )
{
  typename SynchronizedQueue<T>::Consumable c = mQueue.AwaitConsumption( timeout );
  if( c )
    Apply( *c );
  return c;
}

template<class T, class U>
Waitable&
AsyncQueue<T, U>::Empty()
{
  return mEmpty;
}

template<class T, class U>
int
AsyncQueue<T, U>::OnExecute()
{
  while( WaitAndConsume( Time::Forever ) )
    ;
  while( WaitAndConsume( 0 ) )
    ;
  return 0;
}

} // namespace

using Tiny::SynchronizedQueue;
using Tiny::AsyncQueue;

#endif // TINY_SYNCHRONIZED_QUEUE_H
