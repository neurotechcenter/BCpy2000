///////////////////////////////////////////////////////////////////////////////
// $Id$
// Author: juergen.mellinger@uni-tuebingen.de
// Description: Import functions from a DLL by patching function stubs.
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
#include "DllImport.h"
#include <Windows.h>

void DllImportNotInitialized( const char* name )
{
  const char* msg = "Function %1() was called before being initialized.\n\nTerminating process.";
  ::FormatMessageA(
    FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_STRING | FORMAT_MESSAGE_ARGUMENT_ARRAY,
    msg, 0, 0, (LPSTR)&msg, 0, (va_list*)&name
  );
  ::MessageBoxA( 0, msg, "Fatal error", MB_OK | MB_ICONHAND );
  ::ExitProcess( -1 );
}

static void PutAbsjmp( void* inAtLocation, void* inToLocation )
{
  // The jump code pushes the target address on the stack, and then uses the RET instruction
  // to pop the address from the stack and continue execution at the target.
  char* pAtLocation = static_cast<char*>(inAtLocation);
  union { void** p; char* c; } pToLocation = { &inToLocation };
  *pAtLocation++ = 0x68; // PUSH imm32
  for( int i = 0; i < 4; ++i )
    *pAtLocation++ = *pToLocation.c++;
  if( sizeof(void*) == 8 )
  {
    *pAtLocation++ = 0xc7; // MOV [rsp+4], imm32
    *pAtLocation++ = 0x44;
    *pAtLocation++ = 0x24;
    *pAtLocation++ = 0x04;
    for( int i = 0; i < 4; ++i )
      *pAtLocation++ = *pToLocation.c++;
  }
  *pAtLocation++ = 0xc3; // RET
}

bool ResolveDllImports( const char* inDllName, DllImport* inImports )
{
  HMODULE dll = ::LoadLibraryA( inDllName );
  if( !dll )
    return false;
  char* max = nullptr, *min = max - 1;
  for( const DllImport* p = inImports; p->name; ++p )
  {
    char* addr = static_cast<char*>( p->location );
    if( addr > max )
      max = addr;
    if( addr < min )
      min = addr;
  }
  const int maxJumpBytes = 16;
  ptrdiff_t size = max - min + maxJumpBytes;
  DWORD protect = PAGE_EXECUTE_READWRITE;
  ::VirtualProtect( min, size, protect, &protect );
  for( DllImport* p = inImports; p->name; ++p )
  {
    void* proc = ::GetProcAddress( dll, p->name );
    if( proc )
      PutAbsjmp( p->location, proc );
  }
  ::VirtualProtect( max, size, protect, &protect );
  return true;
}

