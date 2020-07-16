////////////////////////////////////////////////////////////////////////////////
// $Id: DylibImports.cpp 4987 2015-08-31 19:18:29Z mellinger $
// Authors: juergen.mellinger@uni-tuebingen.de
// Description: A class representing a PE (Windows) executable file header.
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
#ifndef PE_HEADER_H
#define PE_HEADER_H

#include <cstdint>
#if 1
#include <Windows.h>
#include <Winnt.h>
#endif

namespace WinNT_ // copied from WinNT.h
{
#ifndef IMAGE_NT_SIGNATURE
  typedef char CHAR;
  typedef uint8_t BYTE;
  typedef uint16_t WORD;
  typedef uint32_t DWORD;
  typedef uint64_t ULONGLONG;

  struct IMAGE_FILE_HEADER
  {
    WORD    Machine;
    WORD    NumberOfSections;
    DWORD   TimeDateStamp;
    DWORD   PointerToSymbolTable;
    DWORD   NumberOfSymbols;
    WORD    SizeOfOptionalHeader;
    WORD    Characteristics;
  };
  struct IMAGE_DATA_DIRECTORY
  {
    DWORD   VirtualAddress;
    DWORD   Size;
  };
  enum
  {
    IMAGE_DOS_SIGNATURE = 0x5A4D,
    IMAGE_NT_SIGNATURE = 0x00004550,
    IMAGE_NT_OPTIONAL_HDR32_MAGIC = 0x10b,
    IMAGE_NT_OPTIONAL_HDR64_MAGIC = 0x20b,
  };
  enum
  {
   IMAGE_DIRECTORY_ENTRY_EXPORT         = 0,
   IMAGE_DIRECTORY_ENTRY_IMPORT         = 1,
   IMAGE_DIRECTORY_ENTRY_RESOURCE       = 2,
   IMAGE_DIRECTORY_ENTRY_EXCEPTION      = 3,
   IMAGE_DIRECTORY_ENTRY_SECURITY       = 4,
   IMAGE_DIRECTORY_ENTRY_BASERELOC      = 5,
   IMAGE_DIRECTORY_ENTRY_DEBUG          = 6,
   IMAGE_DIRECTORY_ENTRY_ARCHITECTURE   = 7,
   IMAGE_DIRECTORY_ENTRY_GLOBALPTR      = 8,
   IMAGE_DIRECTORY_ENTRY_TLS            = 9,
   IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG    = 10,
   IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT   = 11,
   IMAGE_DIRECTORY_ENTRY_IAT            = 12,
   IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT   = 13,
   IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR = 14,

   IMAGE_NUMBEROF_DIRECTORY_ENTRIES = 16,
  };
  struct IMAGE_OPTIONAL_HEADER32
  {
      WORD    Magic;
      BYTE    MajorLinkerVersion;
      BYTE    MinorLinkerVersion;
      DWORD   SizeOfCode;
      DWORD   SizeOfInitializedData;
      DWORD   SizeOfUninitializedData;
      DWORD   AddressOfEntryPoint;
      DWORD   BaseOfCode;
      DWORD   BaseOfData;

      DWORD   ImageBase;
      DWORD   SectionAlignment;
      DWORD   FileAlignment;
      WORD    MajorOperatingSystemVersion;
      WORD    MinorOperatingSystemVersion;
      WORD    MajorImageVersion;
      WORD    MinorImageVersion;
      WORD    MajorSubsystemVersion;
      WORD    MinorSubsystemVersion;
      DWORD   Win32VersionValue;
      DWORD   SizeOfImage;
      DWORD   SizeOfHeaders;
      DWORD   CheckSum;
      WORD    Subsystem;
      WORD    DllCharacteristics;
      DWORD   SizeOfStackReserve;
      DWORD   SizeOfStackCommit;
      DWORD   SizeOfHeapReserve;
      DWORD   SizeOfHeapCommit;
      DWORD   LoaderFlags;
      DWORD   NumberOfRvaAndSizes;
      IMAGE_DATA_DIRECTORY DataDirectory[IMAGE_NUMBEROF_DIRECTORY_ENTRIES];
  };
  struct IMAGE_OPTIONAL_HEADER64
  {
      WORD        Magic;
      BYTE        MajorLinkerVersion;
      BYTE        MinorLinkerVersion;
      DWORD       SizeOfCode;
      DWORD       SizeOfInitializedData;
      DWORD       SizeOfUninitializedData;
      DWORD       AddressOfEntryPoint;
      DWORD       BaseOfCode;

      ULONGLONG   ImageBase;
      DWORD       SectionAlignment;
      DWORD       FileAlignment;
      WORD        MajorOperatingSystemVersion;
      WORD        MinorOperatingSystemVersion;
      WORD        MajorImageVersion;
      WORD        MinorImageVersion;
      WORD        MajorSubsystemVersion;
      WORD        MinorSubsystemVersion;
      DWORD       Win32VersionValue;
      DWORD       SizeOfImage;
      DWORD       SizeOfHeaders;
      DWORD       CheckSum;
      WORD        Subsystem;
      WORD        DllCharacteristics;
      ULONGLONG   SizeOfStackReserve;
      ULONGLONG   SizeOfStackCommit;
      ULONGLONG   SizeOfHeapReserve;
      ULONGLONG   SizeOfHeapCommit;
      DWORD       LoaderFlags;
      DWORD       NumberOfRvaAndSizes;
      IMAGE_DATA_DIRECTORY DataDirectory[IMAGE_NUMBEROF_DIRECTORY_ENTRIES];
  };
  struct IMAGE_NT_HEADERS32
  {
    DWORD Signature;
    IMAGE_FILE_HEADER FileHeader;
    IMAGE_OPTIONAL_HEADER32 OptionalHeader;
  };
  enum
  {
    IMAGE_FILE_RELOCS_STRIPPED = 0x0001,
    IMAGE_FILE_EXECUTABLE_IMAGE = 0x0002,
    IMAGE_FILE_LINE_NUMS_STRIPPED = 0x0004,
    IMAGE_FILE_LOCAL_SYMS_STRIPPED = 0x0008,
    IMAGE_FILE_AGGRESIVE_WS_TRIM = 0x0010,
    IMAGE_FILE_LARGE_ADDRESS_AWARE = 0x0020,
    IMAGE_FILE_BYTES_REVERSED_LO = 0x0080,
    IMAGE_FILE_32BIT_MACHINE = 0x0100,
    IMAGE_FILE_DEBUG_STRIPPED = 0x0200,
    IMAGE_FILE_REMOVABLE_RUN_FROM_SWAP = 0x0400,
    IMAGE_FILE_NET_RUN_FROM_SWAP = 0x0800,
    IMAGE_FILE_SYSTEM = 0x1000,
    IMAGE_FILE_DLL = 0x2000,
    IMAGE_FILE_UP_SYSTEM_ONLY = 0x4000,
    IMAGE_FILE_BYTES_REVERSED_HI = 0x8000,
  };
  struct IMAGE_SECTION_HEADER
  {
    BYTE    Name[8];
    union
    {
      DWORD   PhysicalAddress;
      DWORD   VirtualSize;
    } Misc;
    DWORD   VirtualAddress;
    DWORD   SizeOfRawData;
    DWORD   PointerToRawData;
    DWORD   PointerToRelocations;
    DWORD   PointerToLinenumbers;
    WORD    NumberOfRelocations;
    WORD    NumberOfLinenumbers;
    DWORD   Characteristics;
  };
  enum
  {
    IMAGE_SCN_CNT_CODE                   = 0x00000020,
    IMAGE_SCN_CNT_INITIALIZED_DATA       = 0x00000040,
    IMAGE_SCN_CNT_UNINITIALIZED_DATA     = 0x00000080,
    IMAGE_SCN_LNK_INFO                   = 0x00000200,
    IMAGE_SCN_LNK_REMOVE                 = 0x00000800,
    IMAGE_SCN_LNK_COMDAT                 = 0x00001000,
    IMAGE_SCN_NO_DEFER_SPEC_EXC          = 0x00004000,
    IMAGE_SCN_GPREL                      = 0x00008000,
    IMAGE_SCN_MEM_FARDATA                = 0x00008000,
    IMAGE_SCN_MEM_PURGEABLE              = 0x00020000,
    IMAGE_SCN_MEM_16BIT                  = 0x00020000,
    IMAGE_SCN_MEM_LOCKED                 = 0x00040000,
    IMAGE_SCN_MEM_PRELOAD                = 0x00080000,

    IMAGE_SCN_ALIGN_MASK                 = 0x00F00000,

    IMAGE_SCN_LNK_NRELOC_OVFL            = 0x01000000,
    IMAGE_SCN_MEM_DISCARDABLE            = 0x02000000,
    IMAGE_SCN_MEM_NOT_CACHED             = 0x04000000,
    IMAGE_SCN_MEM_NOT_PAGED              = 0x08000000,
    IMAGE_SCN_MEM_SHARED                 = 0x10000000,
    IMAGE_SCN_MEM_EXECUTE                = 0x20000000,
    IMAGE_SCN_MEM_READ                   = 0x40000000,
    IMAGE_SCN_MEM_WRITE                  = 0x80000000,
  };

  struct IMAGE_EXPORT_DIRECTORY
  {
    DWORD   Characteristics;
    DWORD   TimeDateStamp;
    WORD    MajorVersion;
    WORD    MinorVersion;
    DWORD   Name;
    DWORD   Base;
    DWORD   NumberOfFunctions;
    DWORD   NumberOfNames;
    DWORD   AddressOfFunctions;
    DWORD   AddressOfNames;
    DWORD   AddressOfNameOrdinals;
  };

  struct IMAGE_IMPORT_BY_NAME
  {
    WORD    Hint;
    CHAR   Name[1];
  };

  struct IMAGE_RESOURCE_DIRECTORY
  {
      DWORD   Characteristics;
      DWORD   TimeDateStamp;
      WORD    MajorVersion;
      WORD    MinorVersion;
      WORD    NumberOfNamedEntries;
      WORD    NumberOfIdEntries;
  };

  enum
  {
  IMAGE_RESOURCE_NAME_IS_STRING       = 0x80000000,
  IMAGE_RESOURCE_DATA_IS_DIRECTORY    = 0x80000000,
  };

  struct IMAGE_RESOURCE_DIRECTORY_ENTRY
  {
    union
    {
      DWORD   Name;
      WORD    Id;
    };
    DWORD   OffsetToData;
  };

  struct IMAGE_RESOURCE_DATA_ENTRY
  {
      DWORD   OffsetToData;
      DWORD   Size;
      DWORD   CodePage;
      DWORD   Reserved;
  };
#endif // defined IMAGE_NT_SIGNATURE
} // namespace WinNT_
using namespace WinNT_;

namespace Tiny {

class PEHeader
{
public:
  struct DosHeader
  {
    uint16_t e_magic;
    char _1[46];
    uint64_t e_reserved;
    uint32_t _2;
    uint32_t e_lfanew;
  };
  typedef union
  {
    IMAGE_OPTIONAL_HEADER32* p32;
    IMAGE_OPTIONAL_HEADER64* p64;
    IMAGE_OPTIONAL_HEADER* pNative;
  } OptionalHeaderPtr;

  PEHeader( void* inHeaderAddress = 0 );
  ~PEHeader();

  int Bits() const
    { return mBits; }
  char* BaseAddress()
    { return mpAddress; }
  DosHeader* DosHeader()
    { return reinterpret_cast<struct DosHeader*>( mpAddress ); }
  IMAGE_FILE_HEADER* CoffFileHeader()
    { return mpCoffFileHeader; }
  OptionalHeaderPtr OptionalHeader()
    { return mpOptionalHeader; }
  DWORD& AddressOfEntryPoint()
    { return mpOptionalHeader.p32->AddressOfEntryPoint; }

  int SectionAligned( int ) const;
  int FileAligned( int ) const;
  bool FixupRedundantFields();

  IMAGE_SECTION_HEADER* Section( size_t idx )
    { return idx < mSectionCount ? mpSections + idx : 0; }
  int SectionCount() const
    { return mSectionCount; }
  IMAGE_DATA_DIRECTORY* DirectoryEntry( size_t idx )
    { return idx < mDirEntryCount ? mpDirectory + idx : 0; }

  IMAGE_SECTION_HEADER* SectionFromRVA( ptrdiff_t );
  ptrdiff_t RVAToFileOffset( ptrdiff_t );

  template<class T> T FileDataAs( ptrdiff_t rva )
    { ptrdiff_t offs = RVAToFileOffset( rva ); return offs ? reinterpret_cast<T>( BaseAddress() + offs ) : 0; }

  bool RelocateResources( ptrdiff_t );

private:
  void Init( void* inHeaderAddress );

  char* mpAddress;
  IMAGE_FILE_HEADER* mpCoffFileHeader;
  OptionalHeaderPtr mpOptionalHeader;
  int mBits;
  IMAGE_SECTION_HEADER* mpSections;
  size_t mSectionCount;
  IMAGE_DATA_DIRECTORY* mpDirectory;
  size_t mDirEntryCount;
};

class PEFile
{
public:
  explicit PEFile( PEHeader&, bool isImageSection = false );

  const char* ImportLibraryName( size_t idx );
  const char* ExportLibraryName();

  struct SymbolInfo { const char* name; int rva; bool data; void* opaque; };
  bool NextExport( SymbolInfo& );

private:
  template<class T> T DataAs( ptrdiff_t rva );
  PEHeader& mHeader;
  bool mIsImageSection;
};

} // namespace

using Tiny::PEHeader;
using Tiny::PEFile;

#endif // PE_HEADER_H
