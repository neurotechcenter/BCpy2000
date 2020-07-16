////////////////////////////////////////////////////////////////////////////////
// $Id: CallbackBase.cpp 5145 2015-11-27 12:18:20Z mellinger $
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
#include "CallbackBase.h"
#include "ThreadLocal.h"
#include "Waitable.h"
#include "BCIStream.h"
#include "BCIException.h"

using namespace std;

CallbackBase::CallbackBase()
: mOwnerThread( ThreadUtils::ID::Current() )
{
}

CallbackBase&
CallbackBase::SetCallback( int inEvent, Function inCallbackFn, void* inData, Context inContext )
{
  Def d;
  d.f = inCallbackFn;
  d.d = inData;
  d.c = inContext;
  mDefs[inEvent] = d;
  return *this;
}

bool
CallbackBase::Defined( int inEvent ) const
{
  return mDefs.find( inEvent ) != mDefs.end();
}

CallbackBase&
CallbackBase::SetCallbackFunction( int inEvent, Function inCallbackFn )
{
  mDefs[inEvent].f = inCallbackFn;
  return *this;
}

CallbackBase::Function
CallbackBase::CallbackFunction( int inEvent ) const
{
  DefMap::const_iterator i = mDefs.find( inEvent );
  return i == mDefs.end() ? 0 : i->second.f;
}

CallbackBase&
CallbackBase::SetCallbackData( int inEvent, void* inData )
{
  mDefs[inEvent].d = inData;
  return *this;
}

void*
CallbackBase::CallbackData( int inEvent ) const
{
  DefMap::const_iterator i = mDefs.find( inEvent );
  return i == mDefs.end() ? 0 : i->second.d;
}

CallbackBase&
CallbackBase::SetCallbackContext( int inEvent, Context inContext )
{
  mDefs[inEvent].c = inContext;
  return *this;
}

CallbackBase::Context
CallbackBase::CallbackContext( int inEvent ) const
{
  DefMap::const_iterator i = mDefs.find( inEvent );
  return i == mDefs.end() ? DefaultContext : i->second.c;
}

CallbackBase::Result
CallbackBase::ExecuteCallback( int inEvent )
{
  if( !Defined( inEvent ) )
    return Ignored;
  Callback0 c( mDefs[inEvent] );
  return DoExecute( c );
}

CallbackBase::Result
CallbackBase::DoExecute( Callback& inCallback )
{
  Context context = inCallback.Def().f ? inCallback.Def().c : None;
  if( context == MainThread && InOwnerThread() )
    context = CallingThread;

  switch( context )
  {
    case MainThread:
    {
      static ThreadLocalObject<WaitableEvent> sThreadWait;
      sThreadWait().Reset();
      inCallback.SetEvent( &sThreadWait() );
      mQueue.Produce( &inCallback );
      // Suspend execution of the calling thread while the callback function
      // is being executed in the main thread.
      sThreadWait().Wait();
      while( !inCallback.Executed() )
        ThreadUtils::SleepForMs( 1 );
    } break;

    case CallingThread:
      inCallback.Run();
      Assert( inCallback.Executed() );
      break;

    case None:
      break;

    default:
      throw std_logic_error << "Unknown callback execution context:" << context;
  }
  return inCallback.Result();
}

bool
CallbackBase::InOwnerThread() const
{
  return mOwnerThread == ThreadUtils::ID::Current();
}

bool
CallbackBase::CheckPendingCallback()
{
#if BCIDEBUG
  if( !InOwnerThread() )
  {
    bcierr__ << "CallbackBase::CheckPendingCallback() called from a thread that does own this object"
             << endl;
    return false;
  }
#endif // BCIDEBUG
  SynchronizedQueue<Callback*>::Consumable c = mQueue.Consume();
  if( c )
    (*c)->Run();
  return c;
}

bool
CallbackBase::CallbacksPending() const
{
  return !mQueue.Empty();
}

bool
CallbackBase::YieldToMainThread() const
{
#if 0
  if( mpExecuting )
  {
    ThreadUtils::Yield();
    return true;
  }
#endif
  return false;
}

// CallbackBase::Callback
CallbackBase::Callback::Callback( const CallbackBase::Def& d )
: mDef( d ),
  mpEvent( 0 ),
  mResult( Ignored )
{
}

CallbackBase::Callback::~Callback()
{
}

const CallbackBase::Def&
CallbackBase::Callback::Def() const
{
  return mDef;
}

void
CallbackBase::Callback::OnRun()
{
  mExecuted = true;
  if( mpEvent )
    mpEvent->Set();
}

void
CallbackBase::Callback0::OnRun()
{
  typedef CallbackBase::Result (STDCALL *Fn)( void* );
  mResult = reinterpret_cast<Fn>( mDef.f )( mDef.d );
  Callback::OnRun();
}


