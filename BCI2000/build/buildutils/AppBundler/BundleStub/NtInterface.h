////////////////////////////////////////////////////////////////////
// $Id$
// Author: juergen.mellinger@uni-tuebingen.de
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
#ifndef NT_INTERFACE_H
#define NT_INTERFACE_H
#include <ntstatus.h>
#define WIN32_NO_STATUS
#include <Windows.h>
#include <winternl.h>

namespace NtInterface {

// from wdm.h

typedef struct _FILE_BASIC_INFORMATION {
  LARGE_INTEGER CreationTime;
  LARGE_INTEGER LastAccessTime;
  LARGE_INTEGER LastWriteTime;
  LARGE_INTEGER ChangeTime;
  ULONG         FileAttributes;
} FILE_BASIC_INFORMATION, *PFILE_BASIC_INFORMATION;

typedef struct _FILE_NETWORK_OPEN_INFORMATION {
    LARGE_INTEGER CreationTime;
    LARGE_INTEGER LastAccessTime;
    LARGE_INTEGER LastWriteTime;
    LARGE_INTEGER ChangeTime;
    LARGE_INTEGER AllocationSize;
    LARGE_INTEGER EndOfFile;
    ULONG         FileAttributes;
} FILE_NETWORK_OPEN_INFORMATION, *PFILE_NETWORK_OPEN_INFORMATION; 
  
typedef struct _FILE_STANDARD_INFORMATION {
  LARGE_INTEGER AllocationSize;
  LARGE_INTEGER EndOfFile;
  ULONG         NumberOfLinks;
  BOOLEAN       DeletePending;
  BOOLEAN       Directory;
} FILE_STANDARD_INFORMATION, *PFILE_STANDARD_INFORMATION;

typedef struct _FILE_INTERNAL_INFORMATION {
  LARGE_INTEGER IndexNumber;
} FILE_INTERNAL_INFORMATION, *PFILE_INTERNAL_INFORMATION;

typedef struct _FILE_POSITION_INFORMATION {
  LARGE_INTEGER CurrentByteOffset;
} FILE_POSITION_INFORMATION, *PFILE_POSITION_INFORMATION;

/*typedef*/ enum {
  // FileDirectoryInformation = 1,
  FileFullDirectoryInformation = 2,
  FileBothDirectoryInformation = 3,
  FileBasicInformation = 4,
  FileStandardInformation = 5,
  FileInternalInformation = 6,
  FilePositionInformation = 14,
} /*FILE_INFORMATION_CLASS*/;

enum {
  FILE_USE_FILE_POINTER_POSITION = -2,
};

typedef enum {
  MemoryBasicInformation = 0,
} MEMORY_INFORMATION_CLASS;

typedef struct _SECTIONBASICINFO {
    PVOID BaseAddress;
    ULONG AllocationAttributes;
    LARGE_INTEGER MaximumSize;
} SECTION_BASIC_INFORMATION, *PSECTION_BASIC_INFORMATION;

typedef enum {
  SectionBasicInformation = 0,
  SectionImageInformation = 1,
} SECTION_INFORMATION_CLASS;

typedef enum {
  ViewShare = 0,
  ViewUnmap = 1
} SECTION_INHERIT;

enum {
	SL_RESTART_SCAN = 0x01,
	SL_RETURN_SINGLE_ENTRY = 0x02,
	SL_INDEX_SPECIFIED = 0x04,
	SL_RETURN_ON_DISK_ENTRIES_ONLY = 0x08
};

extern NTSTATUS (NTAPI *NtQueryAttributesFile)( POBJECT_ATTRIBUTES, PFILE_BASIC_INFORMATION);
extern NTSTATUS (NTAPI *NtQueryFullAttributesFile)( POBJECT_ATTRIBUTES, PFILE_NETWORK_OPEN_INFORMATION );
extern NTSTATUS (NTAPI *NtQueryVirtualMemory)( HANDLE, PVOID, MEMORY_INFORMATION_CLASS, PVOID, SIZE_T, PSIZE_T );
extern NTSTATUS (NTAPI *NtOpenFile)( PHANDLE, ACCESS_MASK, POBJECT_ATTRIBUTES, PIO_STATUS_BLOCK, ULONG, ULONG );
extern NTSTATUS (NTAPI *NtCreateFile)( PHANDLE, ACCESS_MASK, POBJECT_ATTRIBUTES, PIO_STATUS_BLOCK, PLARGE_INTEGER, ULONG, ULONG, ULONG, ULONG, PVOID, ULONG );
extern NTSTATUS (NTAPI *NtClose)( HANDLE );
extern NTSTATUS (NTAPI *NtReadFile)( HANDLE, HANDLE, PIO_APC_ROUTINE, PVOID, PIO_STATUS_BLOCK, PVOID, ULONG, PLARGE_INTEGER, PULONG );
extern NTSTATUS (NTAPI *NtWriteFile)(HANDLE, HANDLE, PIO_APC_ROUTINE, PVOID, PIO_STATUS_BLOCK, PVOID, ULONG, PLARGE_INTEGER, PULONG);
extern NTSTATUS (NTAPI *NtQueryDirectoryFile)( HANDLE, HANDLE, PIO_APC_ROUTINE, PVOID, PIO_STATUS_BLOCK, PVOID, ULONG, FILE_INFORMATION_CLASS, BOOLEAN, PUNICODE_STRING, BOOLEAN );
extern NTSTATUS (NTAPI *NtQueryDirectoryFileEx)(HANDLE, HANDLE, PIO_APC_ROUTINE, PVOID, PIO_STATUS_BLOCK, PVOID, ULONG, FILE_INFORMATION_CLASS, ULONG, PUNICODE_STRING);
extern NTSTATUS (NTAPI *NtQueryInformationFile)( HANDLE, PIO_STATUS_BLOCK, PVOID, ULONG, FILE_INFORMATION_CLASS );
extern NTSTATUS (NTAPI *NtSetInformationFile)( HANDLE, PIO_STATUS_BLOCK, PVOID, ULONG, FILE_INFORMATION_CLASS );
extern NTSTATUS (NTAPI *NtCreateSection)( PHANDLE, ACCESS_MASK, POBJECT_ATTRIBUTES, PLARGE_INTEGER, ULONG, ULONG, HANDLE );
extern NTSTATUS (NTAPI *NtQuerySection)( HANDLE, SECTION_INFORMATION_CLASS, PVOID, ULONG, PULONG );
extern NTSTATUS (NTAPI *NtMapViewOfSection)( HANDLE, HANDLE, PVOID*, ULONG_PTR, SIZE_T, PLARGE_INTEGER, PSIZE_T, SECTION_INHERIT, ULONG, ULONG );
extern NTSTATUS (NTAPI *NtUnmapViewOfSection)( HANDLE, PVOID );

} // namespace

using namespace NtInterface;

#endif // NT_INTERFACE_H

