///////////////////////////////////////////////////////////////////////////////
// $Id$
// Author: juergen.mellinger@uni-tuebingen.de
// Description: Small footprint minimal C runtime library.
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
///////////////////////////////////////////////////////////////////////////////
#include "MiniCRT.h"
#include "DllImport.h"
#include <Windows.h>

static HANDLE sHeap = 0;
static char* sCmdLine = 0;
static char* sArgv[16];

#define UNEXPECTED(x) \
  ::MessageBoxA( 0, "Unexpected call to unimplemented function " #x, "MiniCRT error:", MB_OK ); \
  ::ExitProcess( -1 ); return 0;

extern "C" int fgetc() { UNEXPECTED( fgetc ) }
extern "C" int sprintf() { UNEXPECTED( sprintf ) }
extern "C" void* __iob_func() { return nullptr; };
extern "C" void* __acrt_iob_func() { return nullptr; }

extern "C" void* malloc( size_t size )
{
  return ::HeapAlloc( sHeap ? sHeap : ::GetProcessHeap(), 0, size );
}

extern "C" void* realloc( void* p, size_t size )
{
  HANDLE heap = sHeap ? sHeap : ::GetProcessHeap();
  return p ? ::HeapReAlloc( heap, 0, p, size ) : ::HeapAlloc( heap, 0, size );
}

extern "C" void free( void* p )
{
  if( p )
    ::HeapFree( sHeap ? sHeap : ::GetProcessHeap(), 0, p );
}

extern "C" void __cdecl _wassert( const wchar_t* msg, const wchar_t*, unsigned )
{
  ::MessageBoxW( 0, msg, L"Assertion failed:", MB_OK );
  ::ExitProcess( -1 );
}

void* operator new( size_t size ) { return ::malloc( size ); }
void operator delete( void* p ) { return ::free( p ); }

bool MiniCRT_init( int* pArgc, char*** pArgv )
{
  extern DllImport NtDllImports[];
  static bool sImportsInitialized = ResolveDllImports( "ntdll.dll", NtDllImports );

  if( !sHeap )
    sHeap = ::HeapCreate( 0, 0, 0 );
  if( pArgc && pArgv && !*sArgv )
  {
    int argc = 0;
    const char* pCmdLine = ::GetCommandLineA();
    sCmdLine = static_cast<char*>( ::malloc( ::strlen( pCmdLine ) + 2 ) );
    ::strcpy( sCmdLine, pCmdLine );
    char* p = sCmdLine;
    int quotes = 0;
    enum { ws, quote, other };
    int last = ws;
    bool done = false;
    while( !done )
    {
      int cur = other;
      switch( *p )
      {
        case 0:
          done = true;
          // fall through
        case ' ':
        case '\t':
        case '\r':
        case '\n':
          if( quotes % 2 == 0 )
            cur = ws;
          else
            cur = other;
          break;
        case '"':
          ++quotes;
          cur = quote;
          break;
      }
      if( last == ws && cur != last )
      {
        if( argc < sizeof(sArgv)/sizeof(*sArgv) - 1 )
          sArgv[argc++] = (cur == quote) ? p + 1 : p;
      }
      else if( cur == ws && cur != last )
      {
        if( last == quote )
          *(p-1) = 0;
        else
          *p = 0;
      }
      last = cur;
      ++p;
    }
    *pArgc = argc;
    *pArgv = sArgv;
  }
  return true;
}

