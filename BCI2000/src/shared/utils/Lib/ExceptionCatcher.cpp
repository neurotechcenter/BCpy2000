//////////////////////////////////////////////////////////////////////
// $Id: ExceptionCatcher.cpp 5642 2017-07-03 16:24:39Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A class that simplifies high-level exception catching.
//   Call its Execute() function with a functor as argument in order
//   to execute the functor in a try block, catching exceptions that
//   occur during execution of the functor.
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
#include "ExceptionCatcher.h"
#include "Exception.h"
#include "SysError.h"
#include "ClassName.h"
#include "Debugging.h"
#include "Synchronized.h"

#include <signal.h>
#include <setjmp.h>

#if _MSC_VER
# define EXCEPTION_MSVC_CPP 0xe06D7363 // MSDN kb 185294
# define CATCH_WIN32_EXCEPTIONS 1
#endif

#if CATCH_WIN32_EXCEPTIONS
# include <Windows.h>
#endif

#include <iostream>

using namespace std;
using namespace Tiny;

namespace {
#if _WIN32
  struct sigaction { void(*sa_handler)( int ); };
  void sigaction( int code, struct sigaction* pNew, struct sigaction* pOld )
  {
    if( pOld )
      pOld->sa_handler = ::signal( code, pNew->sa_handler );
    else
      ::signal( code, pNew->sa_handler );
  }
#endif // _WIN32
  static struct
  {
    const char* name;
    const int code;
    struct sigaction action;
  }
  sSignals[] =
  {
    #define SIGNAL(x) { #x, x, {} }
    SIGNAL( SIGFPE ),
    SIGNAL( SIGILL ),
    SIGNAL( SIGSEGV ),
#ifndef _WIN32
    SIGNAL( SIGBUS ),
#endif
  };
  static jmp_buf sCatchSignals;
  void SignalHandler( int inSignal )
  {
    ::longjmp( sCatchSignals, inSignal );
  }
  void InstallSignalHandlers()
  {
    struct sigaction action = { 0 };
    for( size_t i = 0; i < sizeof( sSignals ) / sizeof( *sSignals ); ++i )
    {
      action.sa_handler = &SignalHandler;
      sigaction( sSignals[i].code, &action, &sSignals[i].action );
    }
  }
  void UninstallSignalHandlers()
  {
    for( size_t i = 0; i < sizeof( sSignals ) / sizeof( *sSignals ); ++i )
      sigaction( sSignals[i].code, &sSignals[i].action, NULL );
  }
}

ExceptionCatcher::ExceptionCatcher(int flags)
: mFlags(flags)
{
}

bool
ExceptionCatcher::Run( Runnable& inRunnable )
{
  return RunWithSignalsHandled( inRunnable );
}

#if CATCH_WIN32_EXCEPTIONS
namespace Tiny
{
struct Win32Exception
{
  const struct ExceptionInfo
  {
    DWORD code;
    const char* description;
    bool bad;
  };
  const ExceptionInfo* Info() const;
  string Describe() const;
  DWORD Filter( LPEXCEPTION_POINTERS );

  DWORD code;
  EXCEPTION_RECORD exception;
  CONTEXT context;
};
}
#endif // CATCH_WIN32_EXCEPTIONS

bool
ExceptionCatcher::RunWithWin32ExceptionsHandled( Runnable& inRunnable )
{
  // This function catches Win32 "structured" exceptions.
  // Handling of those cannot coexist with C++ exception handling in the same function,
  // so we need another function RunWithCppExceptionsHandled() that handles C++ exceptions.
  bool result = false;
#if CATCH_WIN32_EXCEPTIONS
  if(mFlags & CatchWin32Exceptions)
  {
    Win32Exception winException = { 0 };
    __try
    {
      inRunnable.Run();
      result = true;
    }
    __except( winException.Filter( GetExceptionInformation() ) )
    {
      ReportWin32Exception( winException );
    }
  }
  else
#endif // CATCH_WIN32_EXCEPTIONS
  {
    inRunnable.Run();
    result = true;
  }
  return result;
}

bool
ExceptionCatcher::RunWithSignalsHandled( Runnable& inRunnable )
{
  bool result = false;
  static Synchronized<int> signalHandlingInstalled = 0;
  if( (mFlags & CatchSignals) && !signalHandlingInstalled.Atomic().Exchange( 1 ) )
  {
    InstallSignalHandlers();
    int signal = ::setjmp( sCatchSignals );
    if( signal == 0 )
      result = RunWithCppExceptionsHandled( inRunnable );
    else
      ReportSignal( signal );
    UninstallSignalHandlers();
    signalHandlingInstalled = 0;
  }
  else
  {
    result = RunWithCppExceptionsHandled( inRunnable );
  }
  return result;
}

bool
ExceptionCatcher::RunWithCppExceptionsHandled( Runnable& inRunnable )
{
  bool result = false;
  if(mFlags & CatchCppExceptions)
  {
    std::string message;
    try
    {
      result = RunWithWin32ExceptionsHandled( inRunnable );
    }
    catch( const Tiny::Exception& e )
    {
      if( e.AlreadyShown() )
        message = "Exception caught as displayed previously";
      else
        message = e.What() + e.Where();
    }
    catch( const std::exception& e )
    {
      message = "Unhandled exception of type "
                + ClassName( typeid( e ) )
                + ": "
                + e.what();
    }
    catch( ... )
    {
      message = "Unknown unhandled exception";
    }
    if( !message.empty() )
      OnReportException( message );
  }
  else
  {
    result = RunWithWin32ExceptionsHandled( inRunnable );
  }
  return result;
}

#if CATCH_WIN32_EXCEPTIONS
inline const Win32Exception::ExceptionInfo*
Win32Exception::Info() const
{
# define EXCEPTION_( x, y ) { EXCEPTION_##x, "Unhandled Win32 exception of type " #x, y },
# define EXCEPTION( x ) EXCEPTION_( x, false )
# define BAD_EXCEPTION( x ) EXCEPTION_( x, true )
  static const ExceptionInfo info[] =
  {
    EXCEPTION( ACCESS_VIOLATION )
    EXCEPTION( ARRAY_BOUNDS_EXCEEDED )
    EXCEPTION( BREAKPOINT )
    EXCEPTION( DATATYPE_MISALIGNMENT )
    EXCEPTION( FLT_DENORMAL_OPERAND )
    EXCEPTION( FLT_DIVIDE_BY_ZERO )
    EXCEPTION( FLT_INEXACT_RESULT )
    EXCEPTION( FLT_INVALID_OPERATION )
    EXCEPTION( FLT_OVERFLOW )
    EXCEPTION( FLT_STACK_CHECK )
    EXCEPTION( FLT_UNDERFLOW )
    EXCEPTION( GUARD_PAGE )
    EXCEPTION( ILLEGAL_INSTRUCTION )
    EXCEPTION( IN_PAGE_ERROR )
    EXCEPTION( INT_DIVIDE_BY_ZERO )
    EXCEPTION( INT_OVERFLOW )
    EXCEPTION( INVALID_DISPOSITION )
    EXCEPTION( INVALID_HANDLE )
    BAD_EXCEPTION( NONCONTINUABLE_EXCEPTION )
    EXCEPTION( PRIV_INSTRUCTION )
    EXCEPTION( SINGLE_STEP )
    BAD_EXCEPTION( STACK_OVERFLOW )
    { EXCEPTION_MSVC_CPP, "Unhandled MSVC C++ exception, should have been passed rather than displayed", false },
    { 0, "Unhandled Win32 exception of unknown type", true },
    { 0, 0, false }
  };
  for( const ExceptionInfo* p = info; p->description; ++p )
    if( p->code == this->code || p->code == 0 )
      return p;
  return 0;
}

string
Win32Exception::Describe() const
{
  ostringstream oss;
  oss << Info()->description
      << " (0x" << hex << this->code << ")";
  return oss.str();
}

DWORD
Win32Exception::Filter( LPEXCEPTION_POINTERS p )
{
  code = p->ExceptionRecord->ExceptionCode;
  exception = *p->ExceptionRecord;
  context = *p->ContextRecord;
  DWORD result = 0;
  switch( code )
  {
#if _MSC_VER
    case EXCEPTION_MSVC_CPP:
#endif
    case EXCEPTION_BREAKPOINT:
    case EXCEPTION_SINGLE_STEP:
      result = EXCEPTION_CONTINUE_SEARCH;
      break;
    default:
      result = EXCEPTION_EXECUTE_HANDLER;
  }
  if( result != EXCEPTION_CONTINUE_SEARCH )
    SuggestDebugging_( "Win32Exception caught", Info()->description );
  return result;
}

void
ExceptionCatcher::ReportWin32Exception( const Win32Exception& inException )
{
  OnReportException( inException.Describe() );
}
#endif // CATCH_WIN32_EXCEPTIONS

void
ExceptionCatcher::ReportSignal( int inSignal )
{
  size_t numSignals = sizeof( sSignals ) / sizeof( *sSignals ),
         i = 0;
  while( sSignals[i].code != inSignal )
    ++i;
  const char* pDescription = i < numSignals ? sSignals[i].name : "Unknown Signal";
  ostringstream oss;
  oss << "Signal caught: "
      << pDescription;
  OnReportException( oss.str() );
}

void
ExceptionCatcher::OnReportException( const std::string& inMessage )
{
  cerr << inMessage + ( mMessage.empty() ? "" : "\n" ) + mMessage << endl;
}
