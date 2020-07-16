//////////////////////////////////////////////////////////////////////
// $Id: Debugging.h 5356 2016-05-16 15:21:01Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: Debugging support.
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
#ifndef TINY_DEBUGGING_H
#define TINY_DEBUGGING_H

#include <string>
#include <cstdint>

#ifndef TINY_DEBUG
# ifdef NDEBUG
#  define TINY_DEBUG 0
# else
#  define TINY_DEBUG 1
# endif
#endif

#if TINY_DEBUG
# ifdef TINY_ASSERT
#  undef TINY_ASSERT
# endif
# define TINY_ASSERT 1
#endif

#if TINY_ASSERT

# define SuggestDebugging Tiny::DebuggingEvent::Create( &Tiny::SuggestDebugging_, "SuggestDebugging statement", EXCEPTION_CONTEXT_STRING_ )
# define DebugPrompt Tiny::DebuggingEvent::Create( &Tiny::DebugPrompt_ )
# define DebugLog Tiny::DebuggingEvent::Create( &Tiny::DebugLog_ )

# define Assert(x) (void) ( !!Tiny::id_(x) || (Tiny::SuggestDebugging_( "Assertion failed", (Tiny::StringUtils::String() << "Assert( " #x " )\n" << EXCEPTION_CONTEXT_).Str().c_str()),0) )
# define DebugIf(x) (void) ( !Tiny::id_(x) && (Tiny::SuggestDebugging_( "DebugIf condition met", (Tiny::StringUtils::String() << #x "\n" << EXCEPTION_CONTEXT_).Str().c_str()),0) )

#else // TINY_ASSERT

# define SuggestDebugging Tiny::NullStream()
# define DebugPrompt Tiny::NullStream()
# define DebugLog Tiny::NullStream()

# define Assert(x) (0)
# define DebugIf(x) (0)

#endif // TINY_ASSERT

#define Syscall(x) { if( !Tiny::id_(x) ) throw std_runtime_error << "System call failed: " << #x << "\nReason: " << SysError().Message() << "\n"; }

#include "Exception.h"
#include "SysError.h"
#include "NullStream.h"
#include "StringUtils.h"

namespace Tiny
{
class DebuggingEvent
{
  typedef void (*Function)( const char*, const char* );
  Function mFunction;
  String mTitle, mText, mAppendix;
  DebuggingEvent( Function f, const String& title, const String& appendix ) : mFunction( f ), mTitle( title ), mAppendix( appendix ) {}

public:
  static DebuggingEvent Create( Function f, const String& title = 0, const String& appendix = 0 ) { return DebuggingEvent( f, title, appendix ); }
  ~DebuggingEvent() { mText.append( mAppendix ); mFunction( mTitle.c_str(), mText.c_str() ); }
  template<class T> auto operator<<( const T& t ) -> decltype( mText << t ) { return mText << t; }
};

void SuggestDebugging_( const char* why, const char* where = 0 );
void SuggestDebugging_( const std::string& why, const std::string& where = "" );
void DebugPrompt_( const char*, const char* = 0 );
void DebugLog_( const char*, const char* = 0 );

namespace ThreadUtils { class ID; }
namespace Debugging
{
  void SetCurrentThreadName( const std::string& );
  std::string SymbolFromAddress( void* );
  std::string StackTrace( int depth = 32 );

  struct CPUCycles
  {
    CPUCycles& Begin();
    uint64_t End();
  };
};

namespace MemoryDebugging
{
// Avoid irrelevant warnings about unbalanced allocation for static objects.
struct Init
{
  Init();
  ~Init();
};
// In debug mode, deleting a memory block with associated data results in a debugging suggestion:
class Data
{
 public:
  Data( void* );
  void* operator=( void* );
  operator void*() const;
 private:
  void* mMemory;
};
// Called by SharedPointer when taking ownership
void NewlyShared( void* );
// Use with TINY_GUARDED_MALLOC
void SetReadonly( void*, bool );

} // namespace


// The id_() function is there to avoid "condition is always false" and
// "unreachable code" compiler warnings.
inline bool id_( bool x )
  { return x; }

} // namespace

namespace Debugging = Tiny::Debugging;
namespace MemoryDebugging = Tiny::MemoryDebugging;

#endif // TINY_DEBUGGING_H
