//////////////////////////////////////////////////////////////////////
// $Id: Lockable.h 4657 2013-11-29 12:21:30Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: Classes/templates related to locking.
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
#include "Lockable.h"
#include "ThreadLocal.h"

#if _MSC_VER
# include <intrin.h>
# define RETURN_ADDRESS _ReturnAddress()
#else
# define RETURN_ADDRESS __builtin_return_address(0)
#endif

namespace Tiny
{

#if TINY_DEBUG_LOCKS

namespace
{

class Data
{
 public:
  void* operator new( size_t s )
    { return ::malloc( s ); }
  void operator delete( void* p ) throw()
    { if( p ) ::free( p ); }

  Data();
  enum { ctor = 0, dtor, test, NumFuncs };
  void Log( int idx, void* p )
    { mCalls[idx] = reinterpret_cast<const char*>( p ); }
  void Clear()
    { ::memset( mCalls, 0, sizeof( mCalls ) ); }
  void Evaluate() const;

  static Data& Instance()
  {
    static ThreadLocal<Data*> spData;
    if( !spData )
      spData = new Data;
    return *spData;
  }

  private:
   const char* mCalls[NumFuncs];
   static Data* spEmptyLoop;
};
Data* Data::spEmptyLoop;

Data::Data()
{
  static int initialized = 0;
  if( !Atomic_::Value( initialized ).Exchange( 1 ) )
  {
    Instance();
    struct NullLock : LockableObject
    {
      bool OnLock() const { return true; }
      bool OnUnlock() const { return true; }
      bool OnTryLock() const { return true; }
    };
    char memory[sizeof( NullLock )];
    NullLock* p = new (memory) NullLock;
    // Create example loop -- Lock argument must not appear as a local variable.
    WithLock( p )
      /* empty */;
    p->~NullLock();
  }
  Clear();
}

void
Data::Evaluate() const
{
  if( mCalls[ctor] == mCalls[dtor] )
  {
    if( !spEmptyLoop )
    {
      spEmptyLoop = reinterpret_cast<Data*>( -1 );
      DebugLog <<
        "Due to compiler optimizations, it is not possible to detect programming errors related to locking.\n"
        "Define TINY_DEBUG_LOCKS=0 when building to avoid this warning message.";
   }
    return;
  }

  if( !spEmptyLoop && mCalls[test] )
  {
    spEmptyLoop = new Data;
    *spEmptyLoop = *this;
    return;
  }
  if( mCalls[test] - mCalls[dtor] == spEmptyLoop->mCalls[test] - spEmptyLoop->mCalls[dtor] )
    SuggestDebugging <<
      "The body of a \"WithLock\" statement appears to be empty, suggesting a programming error.\n"
      "If it is your actual intent to lock and immediately unlock, rather than executing code while "
      "holding the lock, then use the lock's Toggle() function instead of a WithLock statement.";
}

} // namespace

LockableObject::LockObject::LockObject( const LockableObject& obj, bool tryLock )
: mp( &obj ), mFlag( false )
{
  Data::Instance().Log( Data::ctor, RETURN_ADDRESS );
  Construct( tryLock );
}

LockableObject::LockObject::LockObject( const LockableObject* pObj, bool tryLock )
: mp( pObj ), mFlag( false )
{
  Data::Instance().Log( Data::ctor, RETURN_ADDRESS );
  Construct( tryLock );
}

void
LockableObject::LockObject::Construct( bool tryLock )
{
  if( !tryLock )
  {
    mp->Lock();
  }
  else if( !mp->TryLock() )
  {
    mp = 0;
    mFlag = true;
  }
}

LockableObject::LockObject::~LockObject()
{
  Data::Instance().Log( Data::dtor, RETURN_ADDRESS );
  if( mp )
    mp->Unlock();
  Data::Instance().Evaluate();
}

bool
LockableObject::LockObject::ToggleFlag()
{
  Data::Instance().Log( Data::test, RETURN_ADDRESS );
  return mFlag = !mFlag;
}

#else // TINY_DEBUG_LOCKS

namespace { int dummy; } // suppress LNK4221

#endif // TINY_DEBUG_LOCKS


} // namespace