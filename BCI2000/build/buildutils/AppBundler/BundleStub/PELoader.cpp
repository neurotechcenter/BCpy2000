////////////////////////////////////////////////////////////////////
// $Id$
// Author: juergen.mellinger@uni-tuebingen.de
// Description: Load a PE executable into memory.
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
#include <ntstatus.h>
#define WIN32_NO_STATUS
#include <Windows.h>
#include <winternl.h>

#include "PELoader.h"
#include "PEHeader.h"
#include <cassert>

NTSTATUS
PELoader::ReadImageHeader( NtHookFs::Stream* pStream, IMAGE_NT_HEADERS* pHdr )
{
  pStream->SeekTo( pStream, 0 );
  struct PEHeader::DosHeader dh;
  if( pStream->Read( pStream, &dh, sizeof(dh) ) < sizeof(dh) )
    return STATUS_INVALID_IMAGE_NOT_MZ;
  if( dh.e_magic != IMAGE_DOS_SIGNATURE )
    return STATUS_INVALID_IMAGE_NOT_MZ;
  if( pStream->SeekTo( pStream, dh.e_lfanew ) != dh.e_lfanew )
    return STATUS_INVALID_IMAGE_FORMAT;
  if( pStream->Read( pStream, pHdr, sizeof(*pHdr) ) < sizeof(*pHdr) )
    return STATUS_INVALID_IMAGE_FORMAT;
  if( pHdr->Signature != IMAGE_NT_SIGNATURE )
    return STATUS_INVALID_IMAGE_FORMAT;
  if( pHdr->OptionalHeader.Magic != IMAGE_NT_OPTIONAL_HDR_MAGIC )
    return STATUS_IMAGE_MACHINE_TYPE_MISMATCH;
  return STATUS_SUCCESS;
}

NTSTATUS
PELoader::GetImageSizes( NtHookFs::Stream* pStream, int* pHeaderSize, LARGE_INTEGER* pImageSize )
{
  IMAGE_NT_HEADERS hdr;
  NTSTATUS status = ReadImageHeader( pStream, &hdr );
  if( pHeaderSize )
    *pHeaderSize = hdr.OptionalHeader.SizeOfHeaders;
  if( pImageSize )
    pImageSize->QuadPart = hdr.OptionalHeader.SizeOfImage;
  return STATUS_SUCCESS;
}

NTSTATUS
PELoader::ReadImageData( NtHookFs::Stream* pStream, void* pAddr )
{
  union { void* p; char* c; size_t n; IMAGE_NT_HEADERS* h; } memory = { pAddr };
  int headerSize = 0;
  NTSTATUS status = GetImageSizes( pStream, &headerSize, nullptr );
  if( NT_SUCCESS( status ) )
  {
    pStream->SeekTo( pStream, 0 );
    pStream->Read( pStream, memory.p, headerSize );
    PEHeader h( memory.p );
    if( !h.Bits() )
      status = STATUS_INVALID_IMAGE_FORMAT;
    else if( h.Bits() != sizeof(void*) * 8 )
      status = STATUS_IMAGE_MACHINE_TYPE_MISMATCH;
    for( int i = 0; NT_SUCCESS( status ) && i < h.SectionCount(); ++i )
    {
      IMAGE_SECTION_HEADER* pSection = h.Section( i );
      int size = pSection->SizeOfRawData;
      if( pSection->Misc.VirtualSize > 0 && pSection->Misc.VirtualSize < size )
        size = pSection->Misc.VirtualSize;
      pStream->SeekTo( pStream, pSection->PointerToRawData );
      int bytesRead = pStream->Read( pStream, memory.c + pSection->VirtualAddress, size );
      if( bytesRead < size )
      {
#ifndef NDEBUG // in debug builds, make sure we detect any inconsistencies which may be due to bugs
        status = STATUS_INVALID_IMAGE_FORMAT;
#else // in production builds, behave like Windows PE loader
        ::memset( memory.c + pSection->VirtualAddress + bytesRead, 0, size - bytesRead );
#endif
      }
    }
  }
  return status;
}

NTSTATUS
PELoader::ApplyExePatch(void* pAddr)
{
  NTSTATUS status = STATUS_UNSUCCESSFUL;
  PEHeader h(pAddr);
  if (!h.AddressOfEntryPoint())
    status = STATUS_ENTRYPOINT_NOT_FOUND;
  else if(h.CoffFileHeader()->Characteristics & IMAGE_FILE_DLL)
    status = STATUS_IMAGE_ALREADY_LOADED_AS_DLL;
  else
  {
    h.DosHeader()->e_reserved = h.AddressOfEntryPoint();
    h.AddressOfEntryPoint() = 0;
    h.CoffFileHeader()->Characteristics |= IMAGE_FILE_DLL;
    status = STATUS_SUCCESS;
  }
  return status;
}

NTSTATUS
PELoader::ApplyRelocation( void* pAddr )
{
  NTSTATUS status = STATUS_SUCCESS;
  PEHeader h( pAddr );
  union { void* p; char* c; size_t n; } memory = { pAddr };
  ptrdiff_t delta = memory.n - h.OptionalHeader().pNative->ImageBase;
  if( delta )
  {
    status = STATUS_IMAGE_AT_DIFFERENT_BASE;
    IMAGE_DATA_DIRECTORY* p = h.DirectoryEntry( IMAGE_DIRECTORY_ENTRY_BASERELOC );
    union { char* c; IMAGE_BASE_RELOCATION* r; } pReloc = { memory.c + p->VirtualAddress };
    while( pReloc.c < memory.c + p->VirtualAddress + p->Size )
    {
      union { void* p; char* c; uint16_t* s; } pEntry = { pReloc.r + 1 };
      while( pEntry.c < pReloc.c + pReloc.r->SizeOfBlock )
      {
        union { char* c; uint16_t* i16; uint32_t* i32; uint64_t* i64; } addr =
          { memory.c + pReloc.r->VirtualAddress + (*pEntry.s & 0x0fff) };
        switch( *pEntry.s >> 12 )
        {
          case IMAGE_REL_BASED_ABSOLUTE:
            break;
          case IMAGE_REL_BASED_HIGH:
            *addr.i16 += HIWORD(delta);
            break;
          case IMAGE_REL_BASED_LOW:
            *addr.i16 += LOWORD(delta);
            break;
          case IMAGE_REL_BASED_HIGHLOW:
            *addr.i32 += delta;
            break;
          case IMAGE_REL_BASED_DIR64:
            *addr.i64 += delta;
            break;
          default:
            status = STATUS_INVALID_IMAGE_FORMAT;
        }
        ++pEntry.s;
      }
      pReloc.c += pReloc.r->SizeOfBlock;
    }
  }
  return status;
}

NTSTATUS
PELoader::ApplyMemoryProtection( void* pAddr )
{
  NTSTATUS status = STATUS_SUCCESS;
  PEHeader h( pAddr );
  union { void* p; char* c; size_t n; } memory = { pAddr };
  DWORD protect = PAGE_READONLY;
  if( !::VirtualProtect( memory.c, h.OptionalHeader().pNative->SizeOfImage, protect, &protect ) )
    status = STATUS_UNSUCCESSFUL;
  for( int i = 0; NT_SUCCESS( status ) && i < h.SectionCount(); ++i )
  {
    IMAGE_SECTION_HEADER* pSection = h.Section( i );
    protect = 0;
    int flags = pSection->Characteristics & (IMAGE_SCN_MEM_READ | IMAGE_SCN_MEM_WRITE | IMAGE_SCN_MEM_EXECUTE);
    if( flags & IMAGE_SCN_MEM_WRITE )
      protect = PAGE_READWRITE;
    else if( flags & IMAGE_SCN_MEM_EXECUTE )
      protect = PAGE_EXECUTE_READ;
    else if( flags & IMAGE_SCN_MEM_READ )
      protect = PAGE_READONLY;
    char* addr = memory.c + pSection->VirtualAddress;
    if( !::VirtualProtect( addr, pSection->Misc.VirtualSize, protect, &protect ) )
      status = STATUS_UNSUCCESSFUL;
  }
  return status;
}


