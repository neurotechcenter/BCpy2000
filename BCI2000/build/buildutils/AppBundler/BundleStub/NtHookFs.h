///////////////////////////////////////////////////////////////////////////////
// $Id$
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A virtual file system that is realized by
//   hooking into an application's copy of the ntdll native API library.
//   After a file system has been instantiated, its Mount() function makes
//   its contents accessible under a pseudo-path.
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
#ifndef NT_HOOK_FS_H
#define NT_HOOK_FS_H

#include <cstdint>
#include <Windows.h>
// wdm.h
typedef struct _FILE_BOTH_DIR_INFORMATION {
  ULONG NextEntryOffset;
  ULONG FileIndex;
  LARGE_INTEGER CreationTime;
  LARGE_INTEGER LastAccessTime;
  LARGE_INTEGER LastWriteTime;
  LARGE_INTEGER ChangeTime;
  LARGE_INTEGER EndOfFile;
  LARGE_INTEGER AllocationSize;
  ULONG FileAttributes;
  ULONG FileNameLength;
  ULONG EaSize;
  CCHAR ShortNameLength;
  WCHAR ShortName[12];
  WCHAR FileName[1];
} FILE_BOTH_DIR_INFORMATION, *PFILE_BOTH_DIR_INFORMATION;

class NtHookFs
{
public:
  static bool Install();
  static bool Uninstall();

  static void EnableFileTracking(); // not thread-safe, make sure to disable before entering main executable
  static const wchar_t* DisableFileTracking(); // returns most recently occurring file name
  static bool PauseFileTracking(bool);

  NtHookFs();

  struct MountFlags
  { enum {
    OverlayLower = 0, // combine virtual with actual file system, actual files override virtual ones
    OverlayUpper = 1, // combine virtual with actual file system, virtual files override actual ones
    Opaque = 2,       // hide any actual file system objects which may exist below the mount point
    Writable = 4,     // allow writing to virtual file system
    UseTempFiles = 8, // native API hooking but no fake handles, useful for debugging
    DumpToTemp = 16,  // does not require hooking the native API, may be used as a fallback solution
  }; };
  struct Access
  { enum { 
    None = 0,
    ReadData = 1, WriteData = 2,
    ReadDirEntry = 4, WriteDirEntry = 8,
    Delete = 16,
    Read = ReadData | ReadDirEntry,
    Write = WriteData | WriteDirEntry,
  }; };
  const wchar_t* Mount( const wchar_t*, int flags = 0 );
  bool Unmount();
  const wchar_t* InstanceId() const { return mInstanceId; }

  struct Stream
  { // Using true virtual functions would require linking to C++ library
  public:
    Stream();
    void Retain();
    void Release();
    const wchar_t* (*Name)( Stream* );
    int64_t (*Read)( Stream*, void*, int64_t );
    int64_t (*Write)(Stream*, const void*, int64_t);
    int64_t (*Position)( const Stream* );
    int64_t (*EofPosition)( const Stream* );
    int64_t (*SeekTo)( Stream*, int64_t );
  protected:
    void (*Destroy)( Stream* );
  private:
    long* mpRefCount;
    char mRefCountStorage[7];
  };
  Stream* (*OpenByName)( NtHookFs*, const wchar_t*, const wchar_t*, int );
  Stream* (*OpenByIndex)( NtHookFs*, int, int );
  bool (*StatByName)( NtHookFs*, const wchar_t*, const wchar_t*, FILE_BOTH_DIR_INFORMATION* );
  bool (*StatByIndex)( NtHookFs*, int, wchar_t** /* free with delete[] */, FILE_BOTH_DIR_INFORMATION* );
  bool (*Create)( NtHookFs*, const wchar_t*, const wchar_t*, int attr );
  // A pseudo file system implementation must provide some Nt/Win32 file handle here.
  // This handle will be duplicated whenever a file handle must be synthesized.
  // A handle to the current executable file will be a good choice in most cases.
  void* (*TemplateHandle)( NtHookFs* );

protected:
  static Stream* OpenByName_default(NtHookFs*, const wchar_t*, const wchar_t*, int) { return nullptr; }
  static Stream* OpenByIndex_default(NtHookFs*, int, int) { return nullptr; }
  static bool StatByName_default(NtHookFs*, const wchar_t*, const wchar_t*, FILE_BOTH_DIR_INFORMATION*) { return false; }
  static bool StatByIndex_default(NtHookFs*, int, wchar_t** /* free with delete[] */, FILE_BOTH_DIR_INFORMATION*) { return false; }
  static bool Create_default(NtHookFs*, const wchar_t*, const wchar_t*, int attr) { return false; }
  static void* TemplateHandle_default(NtHookFs*) { return nullptr; }
private:
  wchar_t mInstanceId[4];
};

#endif // NT_HOOK_FS_H

