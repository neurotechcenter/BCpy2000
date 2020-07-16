////////////////////////////////////////////////////////////////////////////////
// $Id: CallbackBase.h 5145 2015-11-27 12:18:20Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A mix-in class that maintains pointers to callback functions,
//   and allows to call them from within a descendant using
//   CallbackBase::ExecuteCallback().
//   The signature of the callback function is determined from the arguments
//   to the ExecuteCallback() function, with the first argument always being
//   a void* specified as callback data, and following arguments being taken
//   from the ExecuteCallback() function call.
//   E.g., when calling
//     ExecuteCallback( MyEvent, "some string", long( 3 ) );
//   the callback function signature will be
//     void OnMyEvent( const char*, long );
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
#ifndef CALLBACK_BASE_H
#define CALLBACK_BASE_H

#include <map>
#include "SynchronizedQueue.h"
#include "Runnable.h"
#include "Waitable.h"
#include "ThreadUtils.h"

#if _WIN32
# define STDCALL __stdcall
#else
# define STDCALL
#endif

class CallbackBase
{
 public:
  typedef int (STDCALL *Function)();

  enum
  {
    Ignored = 0,
  };
  typedef int Result;

  // Callback thread context
  //  CallingThread: The callback function is executed in the thread from
  //                 which the call is issued.
  //  MainThread:    The call is buffered, and executed when the owner thread calls
  //                 CallbackBase::CheckExternalCallback(). The calling thread is
  //                 blocked until the buffered call exits.
  enum Context
  {
    None = 0,
    CallingThread = 1,
    DefaultContext = CallingThread,
    MainThread = 2,
  };

 protected:
  CallbackBase();

 public:
  ~CallbackBase() {}

  CallbackBase& SetCallback( int event, Function function, void* data, Context = DefaultContext );

  CallbackBase& SetCallbackFunction( int event, Function function );
  Function CallbackFunction( int event ) const;
  CallbackBase& SetCallbackData( int event, void* data );
  void* CallbackData( int event ) const;
  CallbackBase& SetCallbackContext( int event, Context );
  Context CallbackContext( int event ) const;

  bool YieldToMainThread() const;

  bool CheckPendingCallback();
  bool CallbacksPending() const;

  Result ExecuteCallback( int event );
  template<class T>
   Result ExecuteCallback( int event, T );
  template<class T , class U>
   Result ExecuteCallback( int event, T, U );
  template<class T , class U, class V>
   Result ExecuteCallback( int event, T, U, V );
  template<class T , class U, class V, class W>
   Result ExecuteCallback( int event, T, U, V, W );

 private:
  struct Def
  {
    Def( Function f = 0, void* d = 0, Context c = DefaultContext ) : f( f ), d( d ), c( c ) {}
    Function f; void* d; Context c;
  };
  class Callback : public Runnable
  {
    Callback( const Callback& );
   public:
    Callback( const CallbackBase::Def& );
    ~Callback();

    const CallbackBase::Def& Def() const;
    bool Executed() const
      { return mExecuted; }
    CallbackBase::Result Result() const
      { return mResult; }
    void SetEvent( WaitableEvent* p )
      { mpEvent = p; }

  protected:
    void OnRun();

   protected:
    CallbackBase::Def mDef;
    Synchronized<CallbackBase::Result> mResult;
    Synchronized<bool> mExecuted;
    WaitableEvent* mpEvent;
  };

  // Callback descendants provide a way to accommodate a variable number of
  // arguments to callback functions.
  struct Callback0 : Callback
  {
    Callback0( const CallbackBase::Def& d ) : Callback( d ) {}
    void OnRun();
  };

  template<class T>
  struct Callback1 : Callback
  {
    Callback1( const CallbackBase::Def& d, T t ) : Callback( d ), t( t ) {}
    void OnRun();
    T t;
  };

  template<class T, class U>
  struct Callback2 : Callback
  {
    Callback2( const CallbackBase::Def& d, T t, U u ) : Callback( d ), t( t ), u( u ) {}
    void OnRun();
    T t; U u;
  };

  template<class T, class U, class V>
  struct Callback3 : Callback
  {
    Callback3( const CallbackBase::Def& d, T t, U u, V v ) : Callback( d ), t( t ), u( u ), v( v ) {}
    void OnRun();
    T t; U u; V v;
  };

  template<class T, class U, class V, class W>
  struct Callback4 : Callback
  {
    Callback4( const CallbackBase::Def& d, T t, U u, V v, W w ) : Callback( d ), t( t ), u( u ), v( v ), w( w ) {}
    void OnRun();
    T t; U u; V v; W w;
  };

  Result DoExecute( Callback& );
  bool InOwnerThread() const;
  bool Defined( int ) const;

  typedef std::map<int, Def> DefMap;
  DefMap mDefs;
  ThreadUtils::ID mOwnerThread;
  SynchronizedQueue<Callback*> mQueue;
};


template<class T>
CallbackBase::Result
CallbackBase::ExecuteCallback( int inEvent, T t )
{
  if( !Defined( inEvent ) )
    return Ignored;
  Callback1<T> c( mDefs[inEvent], t );
  return DoExecute( c );
}

template<class T, class U>
CallbackBase::Result
CallbackBase::ExecuteCallback( int inEvent, T t, U u )
{
  if( !Defined( inEvent ) )
    return Ignored;
  Callback2<T, U> c( mDefs[inEvent], t, u );
  return DoExecute( c );
}

template<class T, class U, class V>
CallbackBase::Result
CallbackBase::ExecuteCallback( int inEvent, T t, U u, V v )
{
  if( !Defined( inEvent ) )
    return Ignored;
  Callback3<T,U,V> c( mDefs[inEvent], t, u, v );
  return DoExecute( c );
}

template<class T, class U, class V, class W>
CallbackBase::Result
CallbackBase::ExecuteCallback( int inEvent, T t, U u, V v, W w )
{
  if( !Defined( inEvent ) )
    return Ignored;
  Callback4<T,U,V,W> c( mDefs[inEvent], t, u, v, w );
  return DoExecute( c );
}

template<class T>
void
CallbackBase::Callback1<T>::OnRun()
{
  typedef CallbackBase::Result (STDCALL *Fn)( void*, T );
  mResult = reinterpret_cast<Fn>( mDef.f )( mDef.d, t );
  Callback::OnRun();
}

template<class T, class U>
void
CallbackBase::Callback2<T, U>::OnRun()
{
  typedef CallbackBase::Result (STDCALL *Fn)( void*, T, U );
  mResult = reinterpret_cast<Fn>( mDef.f )( mDef.d, t, u );
  Callback::OnRun();
}

template<class T, class U, class V>
void
CallbackBase::Callback3<T, U, V>::OnRun()
{
  typedef CallbackBase::Result (STDCALL *Fn)( void*, T, U, V );
  mResult = reinterpret_cast<Fn>( mDef.f )( mDef.d, t, u, v );
  Callback::OnRun();
}

template<class T, class U, class V, class W>
void
CallbackBase::Callback4<T, U, V, W>::OnRun()
{
  typedef CallbackBase::Result (STDCALL *Fn)( void*, T, U, V, W );
  mResult = reinterpret_cast<Fn>( mDef.f )( mDef.d, t, u, v, w );
  Callback::OnRun();
}

#endif // CALLBACK_BASE_H

