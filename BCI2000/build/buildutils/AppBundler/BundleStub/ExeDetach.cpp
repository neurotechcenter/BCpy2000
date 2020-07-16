/////////////////////////////////////////////////////////////////////////////
// $Id$
// Author: juergen.mellinger@uni-tuebingen.de
// Description: Detach the current in-memory executable from its on-disk
//   executable file.
//
//   Duplicates the executable's in-memory image and continues execution
//   inside the copy. From there, it unloads the original in-memory image,
//   and restores it from the copy at its original memory location.
//   After jumping back into the restored code, the copy is freed, and
//   execution continues as normal.
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
////////////////////////////////////////////////////////////////////
#include <Windows.h>

static bool Unload( void* original, void* copy, size_t );
static void* memcpy_( void*, const void*, size_t );

bool ExeDetach()
{
  union { HMODULE h; void* p; char* c; }
    base = { ::GetModuleHandleA( nullptr ) },
    region = base;
  MEMORY_BASIC_INFORMATION info = { 0 };
  info.AllocationBase = base.p;
  while( info.AllocationBase == base.p )
  {
    info.AllocationBase = nullptr;
    ::VirtualQuery( region.p, &info, sizeof(info) );
    if( info.AllocationBase == base.p )
    {
      region.p = info.BaseAddress;
      region.c += info.RegionSize;
    }
  }
  ptrdiff_t size = region.c - base.c;
  if( size <= 0 )
    return false;
  union { void* p; char* c; }
    copy = { ::VirtualAlloc( nullptr, size, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE ) };
  if( !copy.p )
    return false;
  info.AllocationBase = base.p;
  region = base;
  bool ok = true;
  while( info.AllocationBase == base.p )
  {
    info.AllocationBase = nullptr;
    ::VirtualQuery( region.p, &info, sizeof(info) );
    if( info.AllocationBase == base.p )
    {
      region.p = info.BaseAddress;
      char* dest = copy.c + (region.c - base.c);
      memcpy_( dest, region.c, info.RegionSize );
      DWORD protect = info.Protect;
      protect &= 0xff;
      switch( protect )
      {
      case PAGE_WRITECOPY:
        protect = PAGE_READWRITE;
        break;
      case PAGE_EXECUTE_WRITECOPY:
        protect = PAGE_EXECUTE_READWRITE;
        break;
      }
      ok = ok && ::VirtualProtect( dest, info.RegionSize, protect, &protect );
      region.c += info.RegionSize;
    }
  }
  union { decltype(&Unload) f; char* c; } CallUnload = { &Unload };
  CallUnload.c += (copy.c - base.c);
  ok = ok && CallUnload.f( base.p, copy.p, size );
  ::VirtualFree( copy.p, 0, MEM_RELEASE );
  return ok;
}

static bool Unload( void* original, void* copy, size_t size )
{ // This function can not rely on any static variables or import tables,
  // so it needs all required api functions as pointers on the stack
  HMODULE kernel32 = ::LoadLibraryA("kernel32.dll");
#define LOADAPI(x) decltype(&x) x##_ = reinterpret_cast<decltype(&x)>(::GetProcAddress(kernel32, #x)); if(!x##_) return false;
  LOADAPI( VirtualAlloc );
  LOADAPI( VirtualProtect );
  LOADAPI( VirtualQuery );

  if( !::UnmapViewOfFile( original ) )
    return false;
  void* p = VirtualAlloc_( original, size, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE );
  if( p != original )
    return false; // argh!

  union { void* p; char* c; } source = { copy }, region = source, dest = { original };
  MEMORY_BASIC_INFORMATION info = { 0 };
  info.AllocationBase = source.p;
  bool ok = true;
  while( info.AllocationBase == source.p )
  {
    info.AllocationBase = nullptr;
    VirtualQuery_( region.p, &info, sizeof(info) );
    if( info.AllocationBase == source.p )
    {
      region.p = info.BaseAddress;
      char* pDest = dest.c + (region.c - source.c);
      for( size_t i = 0; i < info.RegionSize; ++i )
        *pDest++ = *region.c++; // avoid memcpy() intrinsic vs import complication
      DWORD ignore;
      ok = ok && VirtualProtect_( info.BaseAddress, info.RegionSize, info.Protect, &ignore );
    }
  }
  return ok;
}

void* memcpy_( void* dest, const void* src, size_t count )
{
  char* pDest = static_cast<char*>( dest );
  const char* pSrc = static_cast<const char*>( src );
  for( size_t i = 0; i < count; ++i )
    *pDest++ = *pSrc++;
  return dest;
}

