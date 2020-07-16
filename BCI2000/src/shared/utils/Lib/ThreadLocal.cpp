//////////////////////////////////////////////////////////////////////
// $Id$
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
#include "ThreadLocal.h"

#if _WIN32
# include <Windows.h>
#else
# include <pthread.h>
#endif // _WIN32

namespace Tiny
{

#if _WIN32

# define INDEX (*reinterpret_cast<DWORD*>(const_cast<void**>(&mpData)))

ThreadLocal_::ThreadLocal_()
: mpData( 0 )
{
  INDEX = ::TlsAlloc();
}

ThreadLocal_::~ThreadLocal_()
{
  ::TlsFree( INDEX );
}

void*
ThreadLocal_::Set( const void* const_p )
{
  void* p = const_cast<void*>( const_p );
  ::TlsSetValue( INDEX, p );
  return p;
}

void*
ThreadLocal_::Get() const
{
  return ::TlsGetValue( INDEX );
}

#else // pthreads

namespace
{
  pthread_key_t& Key_( void*& p )
  {
      union { void** p; pthread_key_t* k; } keyptr = { &p };
      return *keyptr.k;
  }
}
# define KEY (Key_(const_cast<void*&>(mpData)))

ThreadLocal_::ThreadLocal_()
: mpData( 0 )
{
  pthread_key_create( &KEY, 0 );
}

ThreadLocal_::~ThreadLocal_()
{
  pthread_key_delete( KEY );
}

void*
ThreadLocal_::Set( const void* const_p )
{
  void* p = const_cast<void*>( const_p );
  pthread_setspecific( KEY, p );
  return p;
}

void*
ThreadLocal_::Get() const
{
  return pthread_getspecific( KEY );
}

#endif // _WIN32

} // namespace
