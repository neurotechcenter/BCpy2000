//////////////////////////////////////////////////////////////////////
// $Id: ThreadLocal.h 5429 2016-07-14 00:41:42Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A template for thread-local storage.
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
#ifndef TINY_THREAD_LOCAL_H
#define TINY_THREAD_LOCAL_H

#include "Uncopyable.h"
#include "StaticObject.h"
#include "Runnable.h"
#include "Thread.h"

namespace Tiny
{

template<typename T = void*> class ThreadLocal;

class ThreadLocal_ : public Uncopyable
{
 protected:
  ThreadLocal_();
  ~ThreadLocal_();
  void* Set( const void* );
  void* Get() const;
 private:
  void* mpData;
};

template<typename T> class ThreadLocal: ThreadLocal_
{
 public:
  ThreadLocal() {}
  ThreadLocal( T t ) { Set( t ); }
  T operator=( T t ) { return Set( t ); }
  operator T() { return Get(); }
 private:
  T Set( T t ) { ThreadLocal_::Set( reinterpret_cast<void*>( t ) ); return t; }
  T Get() const { return reinterpret_cast<T>( ThreadLocal_::Get() ); }
};

template<typename T>
class ThreadLocal<T*> : ThreadLocal_
{
 public:
  ThreadLocal() {}
  ThreadLocal( T* t ) { Set( t ); }
  T* operator=( T* t ) { return Set( t ); }
  T& operator*() const { return *Get(); }
  operator T*() const { return Get(); }
  T* operator->() const { return Get(); }
 private:
  T* Set( T* t ) { ThreadLocal_::Set( t ); return t; }
  T* Get() const { return reinterpret_cast<T*>( ThreadLocal_::Get() ); }
};

template<>
class ThreadLocal<void*> : ThreadLocal_
{
 public:
  ThreadLocal() {}
  ThreadLocal( void* t ) { Set( t ); }
  void* operator=( void* t ) { return Set( t ); }
  operator void*() const { return Get(); }
};

template<typename T>
class ThreadLocalObject
{
 public:
  ThreadLocalObject() : mpMainObject( 0 ) {}
  ~ThreadLocalObject() { delete mpMainObject; }
  T& operator()() { return *Get(); }
  T* operator->() { return Get(); }
  T& operator*() { return *Get(); }
  operator T*() { return Get(); }

 private:
  struct Object : Runnable
  {
    T t;
    void OnRun() { delete this; }
  };
  ThreadLocal<Object*> mpThreadLocal;
  Object* mpMainObject;
  
  T* Get()
  {
    if( !mpThreadLocal )
    {
      mpThreadLocal = new Object;
      if( !Thread::Notify( Thread::WhenFinished, mpThreadLocal ) && !mpMainObject )
        mpMainObject = mpThreadLocal;
    }
    return &mpThreadLocal->t;
  }
};

} // namespace

using Tiny::ThreadLocal;
using Tiny::ThreadLocalObject;

#endif // TINY_THREAD_LOCAL_H
