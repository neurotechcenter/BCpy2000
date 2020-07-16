////////////////////////////////////////////////////////////////////////////////
// $Id: DylibImports.cpp 4987 2015-08-31 19:18:29Z mellinger $
// Authors: juergen.mellinger@uni-tuebingen.de
// Description: See the associated header file for details.
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
#include "PEHeader.h"
// NB: Code in this file encapsulates details about the PE format, and may be used
// on non-native OSes, and resource usage may be an issue.
// Please avoid adding code that requires #inclusion of additional header files.

namespace
{
  enum MappingType { none, file, image };
}

namespace Tiny {

PEHeader::PEHeader( void* inHeaderAddress )
: mpAddress( nullptr ),
  mpCoffFileHeader( nullptr ),
  mBits( 0 ),
  mpSections( 0 ),
  mSectionCount( 0 ),
  mpDirectory( 0 ),
  mDirEntryCount( 0 )
{
  mpOptionalHeader.p32 = nullptr;
  Init( inHeaderAddress );
}

PEHeader::~PEHeader()
{
}

void
PEHeader::Init( void* inHeaderAddress )
{
  mpAddress = reinterpret_cast<char*>( inHeaderAddress );
  if( !mpAddress )
    return;
  struct DosHeader* d = reinterpret_cast<struct DosHeader*>( mpAddress );
  if( d->e_magic != IMAGE_DOS_SIGNATURE )
    return;
  union { char* c; IMAGE_NT_HEADERS32* p; } h = { mpAddress + d->e_lfanew };
  if( h.p->Signature != IMAGE_NT_SIGNATURE )
    return;
  mpCoffFileHeader = &h.p->FileHeader;
  mpOptionalHeader.p32 = &h.p->OptionalHeader;
  union
  {
    IMAGE_OPTIONAL_HEADER32* p32;
    IMAGE_OPTIONAL_HEADER64* p64;
    char* c;
  } opt = { &h.p->OptionalHeader };
  mSectionCount = h.p->FileHeader.NumberOfSections;
  mpSections = reinterpret_cast<IMAGE_SECTION_HEADER*>( opt.c + h.p->FileHeader.SizeOfOptionalHeader );
  switch( h.p->OptionalHeader.Magic )
  {
    case IMAGE_NT_OPTIONAL_HDR32_MAGIC:
      mBits = 32;
      mpDirectory = opt.p32->DataDirectory;
      mDirEntryCount = opt.p32->NumberOfRvaAndSizes;
      break;
    case IMAGE_NT_OPTIONAL_HDR64_MAGIC:
      mBits = 64;
      mpDirectory = opt.p64->DataDirectory;
      mDirEntryCount = opt.p64->NumberOfRvaAndSizes;
      break;
  }
}

IMAGE_SECTION_HEADER*
PEHeader::SectionFromRVA( ptrdiff_t rva )
{
  for( int i = 0; i < mSectionCount; ++i )
  {
    ptrdiff_t begin = mpSections[i].VirtualAddress, end = begin + mpSections[i].Misc.VirtualSize;
    if( begin <= rva && rva < end )
      return mpSections + i;
  }
  return 0;
}

ptrdiff_t
PEHeader::RVAToFileOffset( ptrdiff_t rva )
{
  IMAGE_SECTION_HEADER* pSection = SectionFromRVA( rva );
  if( pSection )
  {
    ptrdiff_t raw = pSection->PointerToRawData,
              virt = pSection->VirtualAddress;
    return rva + raw - virt;
  }
  return 0;
}

namespace
{
  int Aligned( int i, int align )
  {
    return ( ( i + align - 1 ) / align ) * align;
  }
}

int
PEHeader::SectionAligned( int i ) const
{
  int align = ( mBits == 64 ) ? mpOptionalHeader.p64->SectionAlignment : mpOptionalHeader.p32->SectionAlignment;
  return Aligned( i, align );
}

int
PEHeader::FileAligned( int i ) const
{
  int align = ( mBits == 64 ) ? mpOptionalHeader.p64->FileAlignment : mpOptionalHeader.p32->FileAlignment;
  return Aligned( i, align );
}

bool
PEHeader::FixupRedundantFields()
{
  if( !mpAddress )
    return false;
  Init( mpAddress );
  if( !mpSections )
    return false;
  int sizeOfHeaders = sizeof(uint32_t) + 4 + sizeof(IMAGE_FILE_HEADER);
  switch( mBits )
  {
    case 32:
      sizeOfHeaders += sizeof(IMAGE_NT_HEADERS32);
      break;
    case 64:
      sizeOfHeaders += sizeof(IMAGE_NT_HEADERS64);
      break;
    default:
      return false;
  }
  sizeOfHeaders += mSectionCount * sizeof(IMAGE_SECTION_HEADER);
  int sizeOfImage = 0, sizeOfCode = 0, sizeOfInitializedData = 0, sizeOfUninitializedData = 0;
  for( IMAGE_SECTION_HEADER* p = mpSections; p < mpSections + mSectionCount; ++p )
  {
    int end = p->VirtualAddress + p->Misc.VirtualSize;
    if( end > sizeOfImage )
      sizeOfImage = end;
    if( p->Characteristics & IMAGE_SCN_CNT_CODE )
      sizeOfCode += p->Misc.VirtualSize;
    if( p->Characteristics & IMAGE_SCN_CNT_INITIALIZED_DATA )
      sizeOfInitializedData += p->Misc.VirtualSize;
    if( p->Characteristics & IMAGE_SCN_CNT_UNINITIALIZED_DATA )
      sizeOfUninitializedData += p->Misc.VirtualSize;
  }
  if( mBits == 32 )
  {
    IMAGE_OPTIONAL_HEADER32* p = mpOptionalHeader.p32;
    p->SizeOfHeaders = FileAligned( sizeOfHeaders );
    p->SizeOfImage = SectionAligned( sizeOfImage );
    p->SizeOfCode = sizeOfCode;
    p->SizeOfInitializedData = sizeOfInitializedData;
    p->SizeOfUninitializedData = sizeOfUninitializedData;
  }
  else
  {
    IMAGE_OPTIONAL_HEADER64* p = mpOptionalHeader.p64;
    p->SizeOfHeaders = FileAligned( sizeOfHeaders );
    p->SizeOfImage = SectionAligned( sizeOfImage );
    p->SizeOfCode = sizeOfCode;
    p->SizeOfInitializedData = sizeOfInitializedData;
    p->SizeOfUninitializedData = sizeOfUninitializedData;
  }
  return true;
}

bool
PEHeader::RelocateResources( ptrdiff_t delta )
{
  IMAGE_DATA_DIRECTORY* pRes = DirectoryEntry( IMAGE_DIRECTORY_ENTRY_RESOURCE );
  if( !pRes || !pRes->VirtualAddress )
    return true;
  struct
  {
    char* base;
    ptrdiff_t delta;
    void Traverse( IMAGE_RESOURCE_DIRECTORY* pNode )
    {
      IMAGE_RESOURCE_DIRECTORY_ENTRY* pBegin = reinterpret_cast<IMAGE_RESOURCE_DIRECTORY_ENTRY*>(pNode + 1),
       *pEnd = pBegin + pNode->NumberOfNamedEntries + pNode->NumberOfIdEntries;
      for( IMAGE_RESOURCE_DIRECTORY_ENTRY* pEntry = pBegin; pEntry < pEnd; ++pEntry )
      {
        char* p = base + (pEntry->OffsetToData & ~IMAGE_RESOURCE_DATA_IS_DIRECTORY);
        if( pEntry->OffsetToData & IMAGE_RESOURCE_DATA_IS_DIRECTORY )
          Traverse( reinterpret_cast<IMAGE_RESOURCE_DIRECTORY*>( p ) );
        else
          reinterpret_cast<IMAGE_RESOURCE_DATA_ENTRY*>( p )->OffsetToData += delta;
      }
    }
  } t = { FileDataAs<char*>( pRes->VirtualAddress ), delta };
  IMAGE_RESOURCE_DIRECTORY* pDir = FileDataAs<IMAGE_RESOURCE_DIRECTORY*>( pRes->VirtualAddress );
  if( !pDir )
    return false;
  t.Traverse( pDir );
  return true;
}

// PEFile
PEFile::PEFile( PEHeader& h, bool isImageSection )
: mHeader( h ), mIsImageSection( isImageSection )
{
}

template<class T> T
PEFile::DataAs( ptrdiff_t rva )
{
  if( mIsImageSection )
    return reinterpret_cast<T>( mHeader.BaseAddress() + rva );
  return mHeader.FileDataAs<T>( rva );
}

const char*
PEFile::ExportLibraryName()
{
  if( mHeader.CoffFileHeader() )
  {
    const IMAGE_DATA_DIRECTORY* pExportsLocation = mHeader.DirectoryEntry( IMAGE_DIRECTORY_ENTRY_EXPORT );
    if( pExportsLocation )
    {
       const IMAGE_EXPORT_DIRECTORY* pExports = DataAs<IMAGE_EXPORT_DIRECTORY*>( pExportsLocation->VirtualAddress );
       if( pExports )
         return DataAs<const char*>( pExports->Name );
    }
  }
  return 0;
}

bool
PEFile::NextExport( PEFile::SymbolInfo& ioInfo )
{
  ioInfo.name = nullptr;
  ioInfo.data = false;
  ioInfo.rva = 0;
  union { void** p; int* i; } pIdx = { &ioInfo.opaque };
  if( *pIdx.i >= 0 && mHeader.CoffFileHeader() )
  {
    const IMAGE_DATA_DIRECTORY* pExportsLocation = mHeader.DirectoryEntry( IMAGE_DIRECTORY_ENTRY_EXPORT );
    if( pExportsLocation )
    {
      const IMAGE_EXPORT_DIRECTORY* pExports = DataAs<IMAGE_EXPORT_DIRECTORY*>( pExportsLocation->VirtualAddress );
      if( pExports && *pIdx.i < pExports->NumberOfNames )
      {
        const DWORD* pNames = DataAs<DWORD*>( pExports->AddressOfNames );
        if( pNames )
          ioInfo.name = DataAs<const char*>( pNames[*pIdx.i] );
        const WORD* pOrdinals = DataAs<WORD*>( pExports->AddressOfNameOrdinals );
        int ordinal = -1;
        if( pOrdinals )
          ordinal = pOrdinals[*pIdx.i];
        const DWORD* pFunctions = DataAs<DWORD*>( pExports->AddressOfFunctions );
        if( ordinal >= 0 && pFunctions )
          ioInfo.rva = pFunctions[ordinal];
      }
    }
  }
  if( ioInfo.rva )
  {
    IMAGE_SECTION_HEADER* pSection = mHeader.SectionFromRVA( ioInfo.rva );
    if( pSection && !(pSection->Characteristics & IMAGE_SCN_MEM_EXECUTE) )
      ioInfo.data = true;
    ++(*pIdx.i);
  }
  return ioInfo.rva;
}

const char*
PEFile::ImportLibraryName( size_t idx )
{
  if( mHeader.CoffFileHeader() )
  {
    const IMAGE_DATA_DIRECTORY* pImportsLocation = mHeader.DirectoryEntry( IMAGE_DIRECTORY_ENTRY_IMPORT );
    if( pImportsLocation )
    {
      const IMAGE_IMPORT_DESCRIPTOR* pImports = DataAs<IMAGE_IMPORT_DESCRIPTOR*>( pImportsLocation->VirtualAddress );
      if( pImports && pImports[idx].Name )
        return DataAs<const char*>( pImports[idx].Name );
    }
  }
  return 0;
}


} // namespace
