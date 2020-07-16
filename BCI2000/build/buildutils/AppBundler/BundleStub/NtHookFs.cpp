////////////////////////////////////////////////////////////////////
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
#include "NtInterface.h"
#include "NtHookFs.h"
#include "PELoader.h"
#include "Utils.h"
#include "FunctionHook.h"

namespace NtInterface {
NTSTATUS(NTAPI *NtQueryAttributesFile)(POBJECT_ATTRIBUTES, PFILE_BASIC_INFORMATION);
NTSTATUS(NTAPI *NtQueryFullAttributesFile)(POBJECT_ATTRIBUTES, PFILE_NETWORK_OPEN_INFORMATION);
NTSTATUS(NTAPI *NtQueryVirtualMemory)(HANDLE, PVOID, MEMORY_INFORMATION_CLASS, PVOID, SIZE_T, PSIZE_T);
NTSTATUS(NTAPI *NtOpenFile)(PHANDLE, ACCESS_MASK, POBJECT_ATTRIBUTES, PIO_STATUS_BLOCK, ULONG, ULONG);
NTSTATUS(NTAPI *NtCreateFile)(PHANDLE, ACCESS_MASK, POBJECT_ATTRIBUTES, PIO_STATUS_BLOCK, PLARGE_INTEGER, ULONG, ULONG, ULONG, ULONG, PVOID, ULONG);
NTSTATUS(NTAPI *NtClose)(HANDLE);
NTSTATUS(NTAPI *NtReadFile)(HANDLE, HANDLE, PIO_APC_ROUTINE, PVOID, PIO_STATUS_BLOCK, PVOID, ULONG, PLARGE_INTEGER, PULONG);
NTSTATUS(NTAPI *NtWriteFile)(HANDLE, HANDLE, PIO_APC_ROUTINE, PVOID, PIO_STATUS_BLOCK, PVOID, ULONG, PLARGE_INTEGER, PULONG);
NTSTATUS(NTAPI *NtQueryDirectoryFile)(HANDLE, HANDLE, PIO_APC_ROUTINE, PVOID, PIO_STATUS_BLOCK, PVOID, ULONG, FILE_INFORMATION_CLASS, BOOLEAN, PUNICODE_STRING, BOOLEAN);
NTSTATUS(NTAPI *NtQueryDirectoryFileEx)(HANDLE, HANDLE, PIO_APC_ROUTINE, PVOID, PIO_STATUS_BLOCK, PVOID, ULONG, FILE_INFORMATION_CLASS, ULONG, PUNICODE_STRING);
NTSTATUS(NTAPI *NtQueryInformationFile)(HANDLE, PIO_STATUS_BLOCK, PVOID, ULONG, FILE_INFORMATION_CLASS);
NTSTATUS(NTAPI *NtSetInformationFile)(HANDLE, PIO_STATUS_BLOCK, PVOID, ULONG, FILE_INFORMATION_CLASS);
NTSTATUS(NTAPI *NtCreateSection)(PHANDLE, ACCESS_MASK, POBJECT_ATTRIBUTES, PLARGE_INTEGER, ULONG, ULONG, HANDLE);
NTSTATUS(NTAPI *NtQuerySection)(HANDLE, SECTION_INFORMATION_CLASS, PVOID, ULONG, PULONG);
NTSTATUS(NTAPI *NtMapViewOfSection)(HANDLE, HANDLE, PVOID*, ULONG_PTR, SIZE_T, PLARGE_INTEGER, PSIZE_T, SECTION_INHERIT, ULONG, ULONG);
NTSTATUS(NTAPI *NtUnmapViewOfSection)(HANDLE, PVOID);
}

namespace {

enum { STATUS_HOOKFS_UNHANDLED = 0xffff0001, STATUS_HOOKFS_PATH_HIDDEN, };
NTSTATUS FsQueryBasicOrFullAttributesFile(POBJECT_ATTRIBUTES, PFILE_BASIC_INFORMATION, PFILE_NETWORK_OPEN_INFORMATION, bool hookPre);
NTSTATUS FsQueryVirtualMemory( HANDLE, PVOID, MEMORY_INFORMATION_CLASS, PVOID, SIZE_T, PSIZE_T );
NTSTATUS FsCreateFile(PHANDLE, ACCESS_MASK, POBJECT_ATTRIBUTES, PIO_STATUS_BLOCK, PLARGE_INTEGER, ULONG, ULONG, ULONG, ULONG, PVOID, ULONG, bool hookPre);
NTSTATUS FsClose( HANDLE );
NTSTATUS FsReadFile( HANDLE, HANDLE, PIO_APC_ROUTINE, PVOID, PIO_STATUS_BLOCK, PVOID, ULONG, PLARGE_INTEGER, PULONG );
NTSTATUS FsWriteFile(HANDLE, HANDLE, PIO_APC_ROUTINE, PVOID, PIO_STATUS_BLOCK, PVOID, ULONG, PLARGE_INTEGER, PULONG);
NTSTATUS FsQueryDirectoryFile( HANDLE, HANDLE, PIO_APC_ROUTINE, PVOID, PIO_STATUS_BLOCK, PVOID, ULONG, FILE_INFORMATION_CLASS, BOOLEAN, PUNICODE_STRING, BOOLEAN );
NTSTATUS FsQueryDirectoryFileEx(HANDLE, HANDLE, PIO_APC_ROUTINE, PVOID, PIO_STATUS_BLOCK, PVOID, ULONG, FILE_INFORMATION_CLASS, ULONG, PUNICODE_STRING);
NTSTATUS FsQueryInformationFile( HANDLE, PIO_STATUS_BLOCK, PVOID, ULONG, FILE_INFORMATION_CLASS );
NTSTATUS FsSetInformationFile( HANDLE, PIO_STATUS_BLOCK, PVOID, ULONG, FILE_INFORMATION_CLASS );
NTSTATUS FsCreateSection( PHANDLE, ACCESS_MASK, POBJECT_ATTRIBUTES, PLARGE_INTEGER, ULONG, ULONG, HANDLE );
NTSTATUS FsQuerySection( HANDLE, SECTION_INFORMATION_CLASS, PVOID, ULONG, PULONG );
NTSTATUS FsMapViewOfSection( HANDLE, HANDLE, PVOID*, ULONG_PTR, SIZE_T, PLARGE_INTEGER, PSIZE_T, SECTION_INHERIT, ULONG, ULONG );
NTSTATUS FsUnmapViewOfSection( HANDLE, PVOID );

struct
{
  bool active, paused;
  wchar_t name[MAX_PATH + 1];
} sFileTracking;

void
TrackFile( POBJECT_ATTRIBUTES attr )
{
  if( sFileTracking.active && !sFileTracking.paused && attr && attr->ObjectName )
  {
    const wchar_t* p = attr->ObjectName->Buffer;
    int bytes = attr->ObjectName->Length;
    if( bytes >= sizeof(sFileTracking.name) )
    {
      p += (bytes - sizeof(sFileTracking.name))/2 + 1;
      bytes = sizeof(sFileTracking.name) - 2;
    }
    ::memcpy( sFileTracking.name, p, bytes );
    sFileTracking.name[bytes/2] = 0;
  }
}

NTSTATUS NTAPI
NtQueryAttributesFileHook( POBJECT_ATTRIBUTES _0, PFILE_BASIC_INFORMATION _1 )
{
  NTSTATUS status = FsQueryBasicOrFullAttributesFile( _0, _1, nullptr, true );
  switch( status )
  {
  case STATUS_OBJECT_PATH_NOT_FOUND:
      status = NtQueryAttributesFile( _0, _1 );
      break;
  }
  switch( status )
  {
  case STATUS_HOOKFS_PATH_HIDDEN:
      status = STATUS_OBJECT_PATH_NOT_FOUND;
      break;
  case STATUS_OBJECT_NAME_INVALID:
  case STATUS_OBJECT_NAME_NOT_FOUND:
  case STATUS_OBJECT_PATH_INVALID:
  case STATUS_OBJECT_PATH_NOT_FOUND:
      status = FsQueryBasicOrFullAttributesFile( _0, _1, nullptr, false );
      break;
  }
  TrackFile( _0 );
  return status;
}

NTSTATUS NTAPI
NtQueryFullAttributesFileHook( POBJECT_ATTRIBUTES _0, PFILE_NETWORK_OPEN_INFORMATION _1 )
{
  NTSTATUS status = FsQueryBasicOrFullAttributesFile( _0, nullptr, _1, true );
  switch( status )
  {
  case STATUS_OBJECT_PATH_NOT_FOUND:
      status = NtQueryFullAttributesFile( _0, _1 );
      break;
  }
  switch( status )
  {
  case STATUS_HOOKFS_PATH_HIDDEN:
      status = STATUS_OBJECT_PATH_NOT_FOUND;
      break;
  case STATUS_OBJECT_NAME_INVALID:
  case STATUS_OBJECT_NAME_NOT_FOUND:
  case STATUS_OBJECT_PATH_INVALID:
  case STATUS_OBJECT_PATH_NOT_FOUND:
      status = FsQueryBasicOrFullAttributesFile( _0, nullptr, _1, false );
      break;
  }
  TrackFile( _0 );
  return status;
}

NTSTATUS NTAPI
NtQueryVirtualMemoryHook( HANDLE _0, PVOID _1, MEMORY_INFORMATION_CLASS _2, PVOID _3, SIZE_T _4, PSIZE_T _5 )
{
  NTSTATUS status = FsQueryVirtualMemory( _0, _1, _2, _3, _4, _5 );
  if( status == STATUS_HOOKFS_UNHANDLED )
    status = NtQueryVirtualMemory( _0, _1, _2, _3, _4, _5 );
  return status;
}

NTSTATUS NTAPI
NtOpenFileHook( PHANDLE _0, ACCESS_MASK _1, POBJECT_ATTRIBUTES _2, PIO_STATUS_BLOCK _3, ULONG _4, ULONG _5 )
{
  TrackFile( _2 );
  NTSTATUS status = FsCreateFile( _0, _1, _2, _3, 0, FILE_ATTRIBUTE_NORMAL, _4, FILE_OPEN, _5, 0, 0, true );
  switch( status )
  {
  case STATUS_OBJECT_PATH_NOT_FOUND:
  case STATUS_OBJECT_NAME_NOT_FOUND:
      status = NtInterface::NtOpenFile( _0, _1, _2, _3, _4, _5 );
      break;
  }
  switch( status )
  {
  case STATUS_HOOKFS_PATH_HIDDEN:
      status = STATUS_OBJECT_PATH_NOT_FOUND;
      break;
  case STATUS_OBJECT_PATH_NOT_FOUND:
  case STATUS_OBJECT_NAME_NOT_FOUND:
      NTSTATUS status = FsCreateFile(_0, _1, _2, _3, 0, FILE_ATTRIBUTE_NORMAL, _4, FILE_OPEN, _5, 0, 0, false);
      break;
  }
  return status;
}

NTSTATUS NTAPI
NtCreateFileHook( PHANDLE _0, ACCESS_MASK _1, POBJECT_ATTRIBUTES _2, PIO_STATUS_BLOCK _3, PLARGE_INTEGER _4, ULONG _5, ULONG _6, ULONG _7, ULONG _8, PVOID _9, ULONG _10 )
{
  TrackFile( _2 );
  NTSTATUS status = FsCreateFile(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, true);
  switch (status)
  {
  case STATUS_OBJECT_PATH_NOT_FOUND:
  case STATUS_OBJECT_NAME_NOT_FOUND:
    status = NtInterface::NtCreateFile(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10);
    break;
  }
  switch (status)
  {
  case STATUS_HOOKFS_PATH_HIDDEN:
    status = STATUS_OBJECT_PATH_NOT_FOUND;
    break;
  case STATUS_OBJECT_PATH_NOT_FOUND:
  case STATUS_OBJECT_NAME_NOT_FOUND:
    status = FsCreateFile(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, true);
    break;
  }
  return status;
}

NTSTATUS NTAPI
NtCloseHook( HANDLE _0 )
{
  NTSTATUS status = FsClose( _0 );
  if( status == STATUS_HOOKFS_UNHANDLED )
    status = NtInterface::NtClose( _0 );
  return status;
}

NTSTATUS NTAPI
NtReadFileHook( HANDLE _0, HANDLE _1, PIO_APC_ROUTINE _2, PVOID _3, PIO_STATUS_BLOCK _4, PVOID _5, ULONG _6, PLARGE_INTEGER _7, PULONG _8 )
{
  NTSTATUS status = FsReadFile( _0, _1, _2, _3, _4, _5, _6, _7, _8 );
  if( status == STATUS_HOOKFS_UNHANDLED )
    status = NtReadFile( _0, _1, _2, _3, _4, _5, _6, _7, _8 );
  return status;
}

NTSTATUS NTAPI
NtWriteFileHook(HANDLE _0, HANDLE _1, PIO_APC_ROUTINE _2, PVOID _3, PIO_STATUS_BLOCK _4, PVOID _5, ULONG _6, PLARGE_INTEGER _7, PULONG _8)
{
  NTSTATUS status = FsWriteFile(_0, _1, _2, _3, _4, _5, _6, _7, _8);
  if (status == STATUS_HOOKFS_UNHANDLED)
    status = NtWriteFile(_0, _1, _2, _3, _4, _5, _6, _7, _8);
  return status;
}

NTSTATUS NTAPI
NtQueryDirectoryFileHook( HANDLE _0, HANDLE _1, PIO_APC_ROUTINE _2, PVOID _3, PIO_STATUS_BLOCK _4, PVOID _5, ULONG _6, FILE_INFORMATION_CLASS _7, BOOLEAN _8, PUNICODE_STRING _9, BOOLEAN _10 )
{
  NTSTATUS status = FsQueryDirectoryFile( _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10 );
  if( status == STATUS_HOOKFS_UNHANDLED )
    status = NtQueryDirectoryFile( _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10 );
  return status;
}

NTSTATUS NTAPI
NtQueryDirectoryFileExHook(HANDLE _0, HANDLE _1, PIO_APC_ROUTINE _2, PVOID _3, PIO_STATUS_BLOCK _4, PVOID _5, ULONG _6, FILE_INFORMATION_CLASS _7, ULONG _8, PUNICODE_STRING _9)
{
	NTSTATUS status = FsQueryDirectoryFileEx(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9);
	if (status == STATUS_HOOKFS_UNHANDLED)
		status = NtQueryDirectoryFileEx(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9);
	return status;
}

NTSTATUS NTAPI
NtQueryInformationFileHook( HANDLE _0, PIO_STATUS_BLOCK _1, PVOID _2, ULONG _3, FILE_INFORMATION_CLASS _4 )
{
  NTSTATUS status = FsQueryInformationFile( _0, _1, _2, _3, _4 );
  if( status == STATUS_HOOKFS_UNHANDLED )
    status = NtQueryInformationFile( _0, _1, _2, _3, _4 );
  return status;
}

NTSTATUS NTAPI
NtSetInformationFileHook( HANDLE _0, PIO_STATUS_BLOCK _1, PVOID _2, ULONG _3, FILE_INFORMATION_CLASS _4 )
{
  NTSTATUS status = FsSetInformationFile( _0, _1, _2, _3, _4 );
  if( status == STATUS_HOOKFS_UNHANDLED )
    status = NtSetInformationFile( _0, _1, _2, _3, _4 );
  return status;
}

NTSTATUS NTAPI
NtCreateSectionHook( PHANDLE _0, ACCESS_MASK _1, POBJECT_ATTRIBUTES _2, PLARGE_INTEGER _3, ULONG _4, ULONG _5, HANDLE _6 )
{
  TrackFile( _2 );
  NTSTATUS status = FsCreateSection( _0, _1, _2, _3, _4, _5, _6 );
  if( status == STATUS_HOOKFS_UNHANDLED )
    status = NtCreateSection( _0, _1, _2, _3, _4, _5, _6 );
  return status;
}

NTSTATUS NTAPI
NtQuerySectionHook( HANDLE _0, SECTION_INFORMATION_CLASS _1, PVOID _2, ULONG _3, PULONG _4 )
{
  NTSTATUS status = FsQuerySection( _0, _1, _2, _3, _4 );
  if( status == STATUS_HOOKFS_UNHANDLED )
    status = NtQuerySection( _0, _1, _2, _3, _4 );
  return status;
}

NTSTATUS NTAPI
NtMapViewOfSectionHook( HANDLE _0, HANDLE _1, PVOID* _2, ULONG_PTR _3, SIZE_T _4, PLARGE_INTEGER _5, PSIZE_T _6, SECTION_INHERIT _7, ULONG _8, ULONG _9 )
{
  NTSTATUS status = FsMapViewOfSection( _0, _1, _2, _3, _4, _5, _6, _7, _8, _9 );
  if( status == STATUS_HOOKFS_UNHANDLED )
    status = NtMapViewOfSection( _0, _1, _2, _3, _4, _5, _6, _7, _8, _9 );
  return status;
}

NTSTATUS NTAPI
NtUnmapViewOfSectionHook( HANDLE _0, PVOID _1 )
{
  NTSTATUS status = FsUnmapViewOfSection( _0, _1 );
  if( status == STATUS_HOOKFS_UNHANDLED )
    status = NtUnmapViewOfSection( _0, _1 );
  return status;
}

static struct Patch
{
  void* addr;
  void* hook;
  void **orig;
}* sPatches;

bool
UninstallHooks();

bool
InstallHooks()
{
  HMODULE ntdll = ::LoadLibraryA( "ntdll" );
  static Patch patches[] =
  {
#define PATCH(x) { ::GetProcAddress( ntdll, #x ), &x##Hook, (void**)&NtInterface::x },
    PATCH( NtQueryAttributesFile )
    PATCH( NtQueryFullAttributesFile )
    PATCH( NtQueryVirtualMemory )
    PATCH( NtOpenFile )
    PATCH( NtCreateFile )
    PATCH( NtClose )
    PATCH( NtReadFile )
    PATCH( NtWriteFile )
    PATCH( NtQueryDirectoryFile )
	PATCH( NtQueryDirectoryFileEx )
	PATCH( NtQueryInformationFile )
    PATCH( NtSetInformationFile )
    PATCH( NtCreateSection )
    PATCH( NtQuerySection )
    PATCH( NtMapViewOfSection )
    PATCH( NtUnmapViewOfSection )
#undef PATCH
    { nullptr, nullptr, nullptr }
  };
  sPatches = patches;
  bool ok = ntdll;
  for( Patch* p = sPatches; p && p->orig; ++p )
	  if(p->addr)
	    ok = ok && FunctionHook::Apply( p->addr, p->hook, p->orig );
  if( !ok )
    UninstallHooks();
  return ok;
}

bool
UninstallHooks()
{
  bool ok = true;
  for( Patch* p = sPatches; p && p->orig; ++p )
    if( *p->orig )
      if( !FunctionHook::Unapply( p->addr, *p->orig ) )
        ok = false;
  return ok;
}

// Windows kernel handles never have their two lowest-order bits set,
// and these bits are ignored by the OS, so they may be used as a mark.
HANDLE Spiced( HANDLE h ) { return HANDLE( ptrdiff_t( h ) | 3 ); }
HANDLE Raw( HANDLE h ) { return HANDLE( ptrdiff_t( h ) & ~ptrdiff_t( 3 ) ); }
bool IsSpiced( HANDLE h ) { return ( ptrdiff_t( h ) & 3 ) == 3; }

template<class T> T** InsertObject( T** at, T* object )
{
  MutexLock lock( T::mutex );
  object->next = *at;
  *at = object;
  return at;
}

template<class T> T** PushBackObject( T** at, T* object )
{
  MutexLock lock( T::mutex );
  while( *at )
    at = &(*at)->next;
  object->next = nullptr;
  *at = object;
  return at;
}

template<class T> T* UnlinkObject( T** at )
{
  MutexLock lock( T::mutex );
  T* p = *at;
  if( p )
  {
    *at = p->next;
    p->next = nullptr;
  }
  return p;
}

template<class T> T** FindObject( T** from, void* h )
{
  T** p = from;
  while( *p && (*p)->handle != h )
    p = &(*p)->next;
  if( *p && (*p)->handle == h )
    return p;
  return nullptr;
}

template<class T> bool FindAndClearObject( T** from, void* h )
{
  MutexLock lock( T::mutex );
  T** p = FindObject( from, h );
  if( p )
    delete UnlinkObject( p );
  return p;
}

template<class T> struct Object
{
  T* next; void* handle;
  static HANDLE mutex;
  Object() : next( nullptr ), handle( nullptr ) {}
};

template<class T> struct FakeHandleObject : Object<T>
{
  FakeHandleObject( HANDLE hTemplate )
  {
    if( !::DuplicateHandle( ::GetCurrentProcess(), hTemplate, ::GetCurrentProcess(), &handle, 0, false, DUPLICATE_SAME_ACCESS ) )
      handle = INVALID_HANDLE_VALUE;
  }
  ~FakeHandleObject()
  {
    if( handle != INVALID_HANDLE_VALUE )
      ::CloseHandle( handle );
  }
};

struct FileObject : FakeHandleObject<FileObject>
{
  NtHookFs::Stream* stream;
  struct FsMount* mount;
  int fsIndex;
  FileObject( FsMount* m, int index, NtHookFs::Stream* s, HANDLE hTemplate )
  : FakeHandleObject( hTemplate ), mount( m ), fsIndex( index ), stream( s ) {}
  ~FileObject() { stream->Release(); }
} *spFileObjects;
HANDLE FileObject::mutex;

struct DirObject : FakeHandleObject<DirObject>
{
  wchar_t* dirname, *dirnameEnd, *pattern, *patternEnd;
  NtHookFs* pFs;
  int idxOut, idxIn;
  DirObject( const wchar_t* inName, const wchar_t* inEnd, NtHookFs* pFs, HANDLE hTemplate )
  : FakeHandleObject( hTemplate ), pFs( pFs ),
    dirname( nullptr ), dirnameEnd( nullptr ),
    pattern( nullptr ), patternEnd( nullptr ),
    idxOut( -1 ), idxIn( 0 )
  {
    if( inEnd > inName )
    {
      dirname = new wchar_t[inEnd - inName + 2];
      ::memcpy( dirname, inName, (inEnd - inName) * sizeof(*dirname) );
      dirnameEnd = dirname + (inEnd - inName);
      *dirnameEnd++ = L'\\';
      *dirnameEnd = 0;
    }
  }
  ~DirObject()
  {
    delete[] dirname;
    delete[] pattern;
  }
  void SetPattern( UNICODE_STRING* p )
  {
    UNICODE_STRING asterisk = { 2, 2, L"*" };
    if( !p )
      p = &asterisk;
    int count = p->Length / sizeof(*p->Buffer);
    pattern = new wchar_t[count];
    patternEnd = pattern + count;
    ::memcpy( pattern, p->Buffer, p->Length );
  }
} *spDirObjects;
HANDLE DirObject::mutex;

struct ImageSection : Object<ImageSection>
{
  struct ImageView* view;
  NtHookFs::Stream* stream;
  ImageSection( FileObject*, void* );
  ~ImageSection();
} *spImageSections;
HANDLE ImageSection::mutex;

struct ImageView : Object<ImageView>
{
  ImageSection* section;
  ImageView( ImageSection* s, void* p ) : section( s )
  {
    section->view = this;
    handle = p;
  }
  ~ImageView()
  {
    if( section && section->view == this )
      section->view = nullptr;
  }
} *spImageViews;
HANDLE ImageView::mutex;

ImageSection::ImageSection( FileObject* p, void* h )
: view( nullptr ),
  stream( p->stream )
{
  stream->Retain();
  handle = h;
}

ImageSection::~ImageSection()
{
  if( view && view->section == this )
    view->section = nullptr;
  stream->Release();
  ::CloseHandle( Raw( handle ) );
}

struct FsMount : Object<FsMount>
{
  static void Initialize( bool );
  static void Cleanup();

  FsMount( NtHookFs*, const wchar_t*, int flags );
  ~FsMount();
  NTSTATUS FsMount::OpenFile(HANDLE*, ACCESS_MASK, const wchar_t*, const wchar_t*, IO_STATUS_BLOCK*, ULONG, ULONG, ULONG, ULONG);
  bool CreateTemporaryFile( NtHookFs::Stream*, HANDLE* = nullptr );
  bool StatByName( const wchar_t*, const wchar_t*, FILE_BOTH_DIR_INFORMATION* );
  bool StatByIndex( int, FILE_BOTH_DIR_INFORMATION* );

  static FsMount* FindFile( const wchar_t*&, const wchar_t*&, bool isPreHook );
  static FsMount** Find( NtHookFs*, const wchar_t* );
  static void Clear( FsMount** );

  static HANDLE mutex;
  NtHookFs* mpFs;
  struct { HANDLE h; LARGE_INTEGER id; } mTemplate;

  WString mMountPoint;
  int mMountFlags;

  struct TempObject: Object<TempObject> { WString name; }* mpTempObjects;
  TempObject mTempDir;
  static TempObject* spTempDir;

}* spFsMounts;

FsMount::TempObject* FsMount::spTempDir;
HANDLE FsMount::mutex;
HANDLE FsMount::TempObject::mutex;

namespace FileNames
{

const wchar_t win32DevPrefix[] = L"\\\\.\\", ntDosDevAlias[] = L"\\??\\";
const int prefixLen = sizeof(win32DevPrefix)/sizeof(*win32DevPrefix) - 1;

void
Win32ToNtPath( wchar_t* p )
{
  if( ::lstrlenW( p ) >= prefixLen && !::memcmp( p, win32DevPrefix, prefixLen * sizeof(*p) ) )
    ::memcpy( p, ntDosDevAlias, prefixLen * sizeof(*p) );
}

void
NtToWin32Path( wchar_t* p )
{
  if( ::lstrlenW( p ) >= prefixLen && !::memcmp( p, ntDosDevAlias, prefixLen * sizeof(*p) ) )
    ::memcpy( p, win32DevPrefix, prefixLen * sizeof(*p) );
}

wchar_t
UnicodeFullWidth( wchar_t c )
{
  return (c + 0xe0) | 0xff00;
}

void
EnsureValidPath( wchar_t* p, int count, bool allowSeparators )
{
  wchar_t* end = p + count;
  while( p < end )
  {
    switch( *p )
    {
      case L'/':
      case L'\\':
        if( allowSeparators )
        {
          *p = L'\\';
          break;
        }
        /* fall through */
      case L':':
      case L'?':
      case L'*':
      case L'"':
      case L'<':
      case L'>':
      case L'|':
        *p = UnicodeFullWidth( *p );
        break;
      case 0:
        *p = UnicodeFullWidth( L'0' );
        break;
    }
    ++p;
  }
}

WString
PathToName( WString inPath )
{
  // Windows hard limit on long file names is 255 characters, so we
  // truncate the beginning of the path if necessary.
  size_t len = Min<size_t>( inPath.length(), 255 );
  WString name( inPath.data() + inPath.length() - len, len );
  FileNames::EnsureValidPath( name.data(), name.length(), false );
  return name;
}

} // namespace FileNames

FsMount::FsMount( NtHookFs* pFs, const wchar_t* pMountPoint, int flags )
: mpFs( pFs ),
  mMountFlags( flags ),
  mMountPoint( pMountPoint ),
  mpTempObjects( nullptr )
{
  mTemplate.h = pFs->TemplateHandle( pFs );
  mTemplate.id.QuadPart = -1;
  if( NtQueryInformationFile )
  {
    IO_STATUS_BLOCK ioStatusBlock = { 0 };
    FILE_INFORMATION_CLASS infoClass = FILE_INFORMATION_CLASS(FileInternalInformation);
    FILE_INTERNAL_INFORMATION info;
    NTSTATUS status = NtQueryInformationFile( mTemplate.h, &ioStatusBlock, &info, sizeof(info), infoClass );
    if( NT_SUCCESS(status) )
      mTemplate.id = info.IndexNumber;
  }

  bool useTempDir = flags & NtHookFs::MountFlags::UseTempFiles;
  Initialize( useTempDir );
  this->handle = mpFs;
  ::memset( &mTempDir, 0, sizeof( mTempDir ) );
  if( useTempDir )
  { // For each mount, create an individual temporary subdirectory with its name derived from the
    // mount point string, and a file system id.
    mTempDir.name = spTempDir->name + L"\\" + FileNames::PathToName( mMountPoint + mpFs->InstanceId() );
    ::CreateDirectoryW( mTempDir.name.c_str(), nullptr );
    mTempDir.name += L"\\";
    // Keeping an open handle to our subdirectory protects it against being deleted.
    mTempDir.handle = ::CreateFileW( mTempDir.name.c_str(), 0, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, 0 );
    if( mMountFlags & NtHookFs::MountFlags::DumpToTemp )
    {
      int i = 0;
      NtHookFs::Stream* pStream = mpFs->OpenByIndex( mpFs, i, NtHookFs::Access::ReadData );
      while( pStream && CreateTemporaryFile( pStream ) )
        pStream = mpFs->OpenByIndex( mpFs, ++i, NtHookFs::Access::ReadData );
    }
  }
}

FsMount::~FsMount()
{
  // If temporary objects exist, we delete them here.
  // On premature program termination, this destructor will not be
  // called, and temporary files/directories may remain, depending on
  // the mode of operation.
  TempObject* p = nullptr;
  while( p = UnlinkObject( &mpTempObjects ) )
  {
    DWORD attr = 0;
    if( p->handle != INVALID_HANDLE_VALUE )
    {
      BY_HANDLE_FILE_INFORMATION info = { 0 };
      if( ::GetFileInformationByHandle( p->handle, &info ) )
        attr = info.dwFileAttributes;
    }
    if( attr & FILE_ATTRIBUTE_DIRECTORY )
    {
      ::CloseHandle( p->handle );
      ::RemoveDirectoryW( p->name.c_str() );
    }
    else
    {
      // At this point, DLLs should be officially unloaded.
      // This does not imply that they will be unmapped as well, so
      // we need to force unmapping in order to allow deletion.
      wchar_t* pBasename = p->name.data() + p->name.length();
      while( pBasename > p->name.data() && *( pBasename - 1 ) != L'\\' )
        --pBasename;
      HMODULE mapped = ::GetModuleHandleW( pBasename );
      if( mapped )
        ::UnmapViewOfFile( mapped );
      if( p->handle == INVALID_HANDLE_VALUE )
        ::DeleteFileW( p->name.data() );
      else
        ::CloseHandle( p->handle ); // has FILE_ATTRIBUTE_DELETE_ON_CLOSE
    }
    delete p;
  }
  if( mTempDir.handle != INVALID_HANDLE_VALUE )
    ::CloseHandle( mTempDir.handle );
  if( !mTempDir.name.empty() )
    ::RemoveDirectoryW( mTempDir.name.c_str() );
  if( !spFsMounts )
    Cleanup();
}

NTSTATUS
FsMount::OpenFile(
  HANDLE* outHandle, ACCESS_MASK inDesiredAccess,
  const wchar_t* inPath, const wchar_t* inPathEnd,
  IO_STATUS_BLOCK* outSb,
  ULONG inFileAttributes, ULONG inShareAccess,
  ULONG inCreateDisposition, ULONG inCreateOptions )
{
  static const struct { int flag; ACCESS_MASK mask; } accessmap[] =
  {
    { NtHookFs::Access::ReadData, GENERIC_ALL | GENERIC_READ | GENERIC_EXECUTE | FILE_READ_DATA | FILE_EXECUTE },
    { NtHookFs::Access::WriteData, GENERIC_ALL | GENERIC_WRITE | FILE_WRITE_DATA | FILE_APPEND_DATA },
    { NtHookFs::Access::ReadDirEntry, GENERIC_ALL | GENERIC_READ | FILE_READ_ATTRIBUTES | FILE_READ_EA },
    { NtHookFs::Access::WriteDirEntry, GENERIC_ALL | GENERIC_WRITE | FILE_WRITE_ATTRIBUTES | FILE_WRITE_EA },
    { NtHookFs::Access::Delete, DELETE },
  };
  int mode = NtHookFs::Access::None;
  for (size_t i = 0; i < sizeof(accessmap) / sizeof(*accessmap); ++i)
  {
    if(inDesiredAccess & accessmap[i].mask)
      mode |= accessmap[i].flag;
  }

  NTSTATUS status = STATUS_SUCCESS;
  outSb->Information = 0;
  FILE_BOTH_DIR_INFORMATION info;
  if(!mpFs->StatByName( mpFs, inPath, inPathEnd, &info ))
    status = STATUS_OBJECT_NAME_NOT_FOUND;
  if( NT_SUCCESS(status) )
  {
    static const int reqWritableMount = NtHookFs::Access::Write | NtHookFs::Access::Delete;
    if( (inCreateOptions & FILE_DIRECTORY_FILE) && !(info.FileAttributes & FILE_ATTRIBUTE_DIRECTORY) )
      status = STATUS_NOT_A_DIRECTORY;
    else if( (inCreateOptions & FILE_NON_DIRECTORY_FILE) && (info.FileAttributes & FILE_ATTRIBUTE_DIRECTORY) )
      status = STATUS_FILE_IS_A_DIRECTORY;
    else if( (mode & reqWritableMount) && !(mMountFlags & NtHookFs::MountFlags::Writable) )
      status = STATUS_ACCESS_DENIED;
  }
  if(NT_SUCCESS(status)) switch (inCreateDisposition)
  {
    case FILE_OPEN:
    case FILE_OPEN_IF:
      outSb->Information = FILE_OPENED;
      break;
    case FILE_SUPERSEDE:
      outSb->Information = FILE_SUPERSEDED;
      break;
    case FILE_OVERWRITE:
    case FILE_OVERWRITE_IF:
      outSb->Information = FILE_OVERWRITTEN;
      break;
    case FILE_CREATE:
      outSb->Information = FILE_EXISTS;
      status = STATUS_UNSUCCESSFUL;
      break;
  }
  else switch (inCreateDisposition)
  {
    case FILE_OPEN:
    case FILE_OVERWRITE:
      outSb->Information = FILE_DOES_NOT_EXIST;
      break;
    case FILE_SUPERSEDE:
    case FILE_CREATE:
    case FILE_OPEN_IF:
    case FILE_OVERWRITE_IF:
      outSb->Information = FILE_CREATED;
      status = STATUS_SUCCESS;
      break;
  }
  if (outSb->Information == FILE_CREATED)
  {
    int attrib = 0;
    if(inCreateOptions & FILE_DIRECTORY_FILE)
      attrib |= FILE_ATTRIBUTE_DIRECTORY;
    if(!mpFs->Create(mpFs, inPath, inPathEnd, attrib))
    {
      outSb->Information = 0;
      status = STATUS_UNSUCCESSFUL;
    }
  }
  if( NT_SUCCESS(status) )
  {
    if( info.FileAttributes & FILE_ATTRIBUTE_DIRECTORY )
    {
      DirObject* pObject = new DirObject( inPath, inPathEnd, mpFs, mTemplate.h );
      if( pObject->handle != INVALID_HANDLE_VALUE )
      {
        *outHandle = pObject->handle;
        InsertObject( &spDirObjects, pObject );
      }
      else
      {
        delete pObject;
        status = STATUS_INSUFFICIENT_RESOURCES;
      }
    }
    else
    {
      NtHookFs::Stream* pStream = mpFs->OpenByIndex( mpFs, info.FileIndex, mode );
      if(!pStream)
        pStream = mpFs->OpenByName(mpFs, inPath, inPathEnd, mode);
      if(!pStream)
        status = STATUS_OBJECT_NAME_NOT_FOUND;
      else if (mMountFlags & NtHookFs::MountFlags::DumpToTemp)
      { // extract to a temporary file, and return an ordinary file handle to that file
        if (!CreateTemporaryFile(pStream, outHandle))
          status = STATUS_INSUFFICIENT_RESOURCES;
      }
      else
      { // extract into memory, and return a spiced handle to an existing file
        FileObject* pObject = new FileObject( this, info.FileIndex, pStream, mTemplate.h );
        if( pObject->handle != INVALID_HANDLE_VALUE )
        {
          *outHandle = pObject->handle;
          InsertObject( &spFileObjects, pObject );
        }
        else
        {
          delete pObject;
          status = STATUS_INSUFFICIENT_RESOURCES;
        }
      }
    }
  }
  return status;
}

bool
FsMount::CreateTemporaryFile( NtHookFs::Stream* pStream, HANDLE* outpHandle )
{
  WString tempFile = mTempDir.name + pStream->Name( pStream );
  FileNames::EnsureValidPath( tempFile.data() + mTempDir.name.length(), tempFile.length() - mTempDir.name.length(), true );
  for( wchar_t* p = tempFile.data() + mTempDir.name.length(); *p; ++p )
  {
    if( *p == L'\\' )
    {
      *p = 0;
      if( ::CreateDirectoryW( tempFile.data(), nullptr ) )
      {
        HANDLE h = ::CreateFileW( tempFile.data(), 0, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, 0 );
        if( h != INVALID_HANDLE_VALUE )
        {
          TempObject* p = new TempObject;
          p->handle = h;
          p->name = tempFile.data();
          PushBackObject( &mpTempObjects, p );
        }
      }
      else if( ::GetLastError() != ERROR_ALREADY_EXISTS )
        return false;
      *p = L'\\';
    }
  }
  int access = GENERIC_READ | GENERIC_WRITE,
      share = FILE_SHARE_READ | FILE_SHARE_DELETE,
      attr = FILE_ATTRIBUTE_NORMAL;
  if( outpHandle ) // used with a hook
    attr = FILE_ATTRIBUTE_TEMPORARY | FILE_FLAG_DELETE_ON_CLOSE;
  HANDLE hNewFile = ::CreateFileW( tempFile.data(), access, share, nullptr, CREATE_ALWAYS, attr, 0 );
  bool newTempFileCreated = ( hNewFile != INVALID_HANDLE_VALUE );
  if( newTempFileCreated )
  {
    LARGE_INTEGER size;
    size.QuadPart = pStream->EofPosition( pStream );
    if( ::SetFilePointerEx( hNewFile, size, 0, FILE_BEGIN ) && ::SetEndOfFile( hNewFile ) )
    {
      HANDLE hMapping = ::CreateFileMappingW( hNewFile, 0, PAGE_READWRITE, size.HighPart, size.LowPart, 0 );
      if( hMapping )
      {
        void* pData = ::MapViewOfFile( hMapping, FILE_MAP_WRITE, 0, 0, size.QuadPart );
        ::CloseHandle( hMapping );
        if( pData )
        {
          pStream->SeekTo( pStream, 0 );
          pStream->Read( pStream, pData, size.QuadPart );
          pStream->Release();
          ::UnmapViewOfFile( pData );
        }
      }
    }
  }
  if( outpHandle )
  {
    int access = GENERIC_READ | GENERIC_EXECUTE,
        share =  FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
        attr = FILE_ATTRIBUTE_TEMPORARY;
    *outpHandle = ::CreateFileW( tempFile.c_str(), access, share, nullptr, OPEN_EXISTING, attr, 0 );
  }
  if( newTempFileCreated )
  {
    TempObject* p = new TempObject;
    p->name = tempFile;
    if( outpHandle )
    { // With hooks installed, we are able to manipulate a caller's open mode.
      // This allows us to keep a handle with FILE_FLAG_DELETE_ON_CLOSE open,
      // such that temp files will be deleted even without a clean program exit.
      p->handle = hNewFile;
    }
    else
    { // Without hooking, we must make sure that temp files can be opened in
      // whatever mode a caller desires, so we cannot keep a handle open.
      // If the program does not exit cleanly, temp files will not be removed.
      ::CloseHandle( hNewFile );
      p->handle = INVALID_HANDLE_VALUE;
    }
    PushBackObject( &mpTempObjects, p );
  }
  return outpHandle ? ( *outpHandle != INVALID_HANDLE_VALUE ) : newTempFileCreated;
}

bool
FsMount::StatByName( const wchar_t* name, const wchar_t* nameEnd, FILE_BOTH_DIR_INFORMATION* stat )
{
  bool ok = mpFs->StatByName( mpFs, name, nameEnd, stat );
  if( ok && !(mMountFlags & NtHookFs::MountFlags::Writable) && stat )
    stat->FileAttributes |= FILE_ATTRIBUTE_READONLY;
  return ok;
}

bool
FsMount::StatByIndex( int index, FILE_BOTH_DIR_INFORMATION* stat )
{
  bool ok = mpFs->StatByIndex( mpFs, index, nullptr, stat );
  if( ok && !(mMountFlags & NtHookFs::MountFlags::Writable) && stat )
    stat->FileAttributes |= FILE_ATTRIBUTE_READONLY;
  return ok;
}

FsMount*
FsMount::FindFile( const wchar_t*& ioName, const wchar_t*& ioNameEnd, bool inIsPreHook )
{
  FsMount* pResult = nullptr;
  if( ioNameEnd > ioName && *(ioNameEnd-1) == L'\\' )
    --ioNameEnd;
  for( FsMount* pMount = spFsMounts; !pResult && pMount; pMount = pMount->next )
  {
    bool testAtPreHook = (pMount->mMountFlags & (NtHookFs::MountFlags::OverlayUpper|NtHookFs::MountFlags::Opaque));
    if( testAtPreHook == inIsPreHook )
    {
      int len = pMount->mMountPoint.length();
      const wchar_t* pName = nullptr;
      if( ioNameEnd - ioName > len && ioName[len] == L'\\' )
        pName = ioName + len + 1;
      else if( ioNameEnd - ioName == len )
        pName = ioName + len;
      if( pName && ::_wcsnicmp( pMount->mMountPoint.c_str(), ioName, len ) )
        pName = nullptr;
      if( pName )
      {
        bool exists = pMount->StatByName( pName, ioNameEnd, nullptr );
        if( exists )
        {
          ioName = pName;
          pResult = pMount;
        }
        else if( pMount->mMountFlags & NtHookFs::MountFlags::Opaque )
        {
          ioName = nullptr;
          pResult = pMount;
        }
      }
    }
  }
  return pResult;
}

FsMount**
FsMount::Find( NtHookFs* fs, const wchar_t* inMountPoint )
{
  for( FsMount** p = &spFsMounts; *p; p = &(*p)->next )
    if( (*p)->mpFs == fs && !::lstrcmpiW( inMountPoint, (*p)->mMountPoint.c_str() ) )
      return p;
  return nullptr;
}

void
FsMount::Clear( FsMount** p )
{
  delete UnlinkObject( p );
}

void
FsMount::Initialize( bool inWithTempDir )
{
  if( !FileObject::mutex )
  {
    FileObject::mutex = ::CreateMutexA( nullptr, false, nullptr );
    ImageSection::mutex = ::CreateMutexA( nullptr, false, nullptr );
    ImageView::mutex = ::CreateMutexA( nullptr, false, nullptr );
    FsMount::mutex = ::CreateMutexA( nullptr, false, nullptr );
    FsMount::TempObject::mutex = ::CreateMutexA( nullptr, false, nullptr );
  }
  if( inWithTempDir && !spTempDir )
  {
    spTempDir = new TempObject;
    spTempDir->handle = INVALID_HANDLE_VALUE;
    wchar_t temp[MAX_PATH + 1] = L"";
    ::GetTempPathW( sizeof(temp)/sizeof(*temp), temp );
    wchar_t module[MAX_PATH + 5] = L"";
    ::GetModuleFileNameW( 0, module, sizeof(module)/sizeof(*module) );
    wchar_t* p = module + ::lstrlenW( module );
    while( p > module && *(p-1) != L'\\' )
      --p;
    WString& tempdir = spTempDir->name;
    tempdir = L"\\\\?\\";
    tempdir.append( temp ).append( p ).append( L".hkfs.***" );
    int maxCount = 1, places = 0;
    p = tempdir.data() + tempdir.length();
    while( p > tempdir.data() && *(p-1) == L'*' )
      --p, ++places, maxCount *= 10;
    int n = 0, err = 0;
    while( !err && n < maxCount && spTempDir->handle == INVALID_HANDLE_VALUE )
    {
      wchar_t* q = p;
      for( int i = 0; i < places; ++i )
      {
        int d = n;
        for( int j = 1; j < places - i; ++j )
          d /= 10;
        *q++ = L'0' + d % 10;
      }
      *q++ = L'\\';
      *q = 0;
      ++n;
      ::CreateDirectoryW( tempdir.c_str(), nullptr );
      WString lockname = tempdir + L"\\.lock";
      spTempDir->handle = ::CreateFileW( lockname.c_str(), GENERIC_ALL, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_DELETE_ON_CLOSE, 0 );
      *q = 0;
      err = ::GetLastError();
      if( err == ERROR_ALREADY_EXISTS )
        err = 0;
    }
  }
}

void
FsMount::Cleanup()
{
  if( spTempDir )
  {
    ::CloseHandle( spTempDir->handle );
    ::RemoveDirectoryW( spTempDir->name.c_str() );
    delete spTempDir;
    spTempDir = nullptr;
  }
}

} // namespace

static int sRefCount;
static bool sInstalled;

bool
NtHookFs::Install()
{
  ++sRefCount;
  if(!sInstalled)
    sInstalled = InstallHooks();
  return sInstalled;
}

bool
NtHookFs::Uninstall()
{
  if(--sRefCount == 0)
    sInstalled = !UninstallHooks();
  return !sInstalled;
}

void
NtHookFs::EnableFileTracking()
{
  sFileTracking.active = true;
  *sFileTracking.name = 0;
}

const wchar_t*
NtHookFs::DisableFileTracking()
{
  sFileTracking.active = false;
  return sFileTracking.name;
}

bool
NtHookFs::PauseFileTracking(bool b)
{
  bool wasPaused = sFileTracking.paused;
  sFileTracking.paused = b;
  return wasPaused;
}

NtHookFs::NtHookFs() :
  OpenByName(&OpenByName_default),
  OpenByIndex(&OpenByIndex_default),
  StatByName(&StatByName_default),
  StatByIndex(&StatByIndex_default),
  Create(&Create_default),
  TemplateHandle(&TemplateHandle_default)
{
  static int sSerialNumber;
  ::memset(mInstanceId, 0, sizeof(mInstanceId));
  wchar_t* p = mInstanceId + sizeof(mInstanceId) / sizeof(*mInstanceId);
  *--p = 0;
  int n = sSerialNumber++;
  while(p > mInstanceId)
  {
    *--p = L'0' + n % 10;
    n /= 10;
  }
}

const wchar_t*
NtHookFs::Mount( const wchar_t* inMountPoint, int inFlags )
{
  bool inUseTempDir = inFlags & MountFlags::UseTempFiles,
       inDump = inFlags & MountFlags::DumpToTemp;
  WString mountPoint;
  if( *inMountPoint != L'\\' )
    mountPoint += L"\\\\.\\";
  else if( *(inMountPoint+1) != L'\\' )
    mountPoint += L"\\\\.";
  mountPoint += inMountPoint;
  FileNames::Win32ToNtPath( mountPoint.data() );
  wchar_t* pLast = mountPoint.data() + mountPoint.length();
  while( pLast > mountPoint.data() && *--pLast == L'\\' )
    *pLast = 0;
  FsMount** p = FsMount::Find( this, mountPoint.c_str() );
  if( !p )
    p = InsertObject( &spFsMounts, new FsMount( this, mountPoint.c_str(), inFlags ) );
  return (inUseTempDir && inDump) ? (*p)->mTempDir.name.c_str() : inMountPoint;
}

bool
NtHookFs::Unmount()
{
  bool result = false;
  FsMount** p = FindObject( &spFsMounts, this );
  while( p )
  {
    result = true;
    FsMount::Clear( p );
    p = FindObject( &spFsMounts, this );
  }
  return result;
}

NtHookFs::Stream::Stream()
{
  char* p = mRefCountStorage;
  while( ( p - static_cast<char*>( nullptr ) ) & 4 )
    ++p;
  mpRefCount = reinterpret_cast<LONG*>( p );
  *mpRefCount = 1;
}

void
NtHookFs::Stream::Retain()
{
  ::InterlockedIncrement( mpRefCount );
}

void
NtHookFs::Stream::Release()
{
  if( ::InterlockedDecrement( mpRefCount ) == 0 )
    Destroy( this );
}

namespace
{

// fast pre-check to avoid performance impact
bool MaybeFakeHandle( HANDLE h )
{
  if( !spFsMounts )
    return false;
  IO_STATUS_BLOCK ioStatusBlock = { 0 };
  FILE_INFORMATION_CLASS infoClass = FILE_INFORMATION_CLASS(FileInternalInformation);
  FILE_INTERNAL_INFORMATION info;
  NTSTATUS status = NtQueryInformationFile( h, &ioStatusBlock, &info, sizeof(info), infoClass );
  if( NT_ERROR(status) )
    return true;
  for( FsMount* p = spFsMounts; p; p = p->next )
  {
    bool mayRefer = p->mTemplate.h != h; // don't accept the template itself as a match
    mayRefer = mayRefer && ( p->mTemplate.id.QuadPart == info.IndexNumber.QuadPart );
    if( mayRefer )
      return true;
  }
  return false;
}

NTSTATUS
FsQueryBasicOrFullAttributesFile( POBJECT_ATTRIBUTES _0, PFILE_BASIC_INFORMATION _1b, PFILE_NETWORK_OPEN_INFORMATION _1f, bool isPreHook )
{
  NTSTATUS status = STATUS_OBJECT_PATH_NOT_FOUND;
  if( !_0->RootDirectory )
  {
    const wchar_t* pName = _0->ObjectName->Buffer,
                 *pNameEnd = pName + _0->ObjectName->Length / 2;
    FsMount* pMount = FsMount::FindFile( pName, pNameEnd, isPreHook );
    if( pMount )
    {
      status = STATUS_HOOKFS_PATH_HIDDEN;
      if( pName )
      {
        FILE_BOTH_DIR_INFORMATION stat;
        if( pMount && pMount->StatByName( pName, pNameEnd, &stat ) )
        {
          if(_1b)
          {
            ::memset( _1b, 0, sizeof(*_1b) );
            _1b->FileAttributes = stat.FileAttributes;
            _1b->CreationTime = stat.CreationTime;
            _1b->ChangeTime = stat.ChangeTime;
            _1b->LastAccessTime = stat.LastAccessTime;
            _1b->LastWriteTime = stat.LastWriteTime;
          }
          if(_1f)
          {
            ::memset(_1f, 0, sizeof(*_1f));
            _1f->FileAttributes = stat.FileAttributes;
            _1f->CreationTime = stat.CreationTime;
            _1f->ChangeTime = stat.ChangeTime;
            _1f->LastAccessTime = stat.LastAccessTime;
            _1f->LastWriteTime = stat.LastWriteTime;
            _1f->EndOfFile = stat.EndOfFile;
            _1f->AllocationSize = stat.AllocationSize;
          }
          status = STATUS_SUCCESS;
        }
      }
    }
  }
  return status;
}

NTSTATUS
FsQueryVirtualMemory( HANDLE _0, PVOID _1, MEMORY_INFORMATION_CLASS _2, PVOID _3, SIZE_T _4, PSIZE_T _5 )
{
  NTSTATUS status = STATUS_HOOKFS_UNHANDLED;
  if( _0 == ::GetCurrentProcess() && _2 == MemoryBasicInformation && _4 >= sizeof( MEMORY_BASIC_INFORMATION ) )
  {
    status = NtQueryVirtualMemory( _0, _1, _2, _3, _4, _5 );
    if( NT_SUCCESS( status ) )
    {
      MEMORY_BASIC_INFORMATION* pInfo = static_cast<MEMORY_BASIC_INFORMATION*>( _3 );
      if( FindObject( &spImageViews, pInfo->AllocationBase ) )
        pInfo->Type = MEM_IMAGE; // important for safe structured exception handlers on x86
    }
  }
  return status;
}

NTSTATUS FsCreateFile(PHANDLE _0, ACCESS_MASK _1, POBJECT_ATTRIBUTES _2, PIO_STATUS_BLOCK _3, PLARGE_INTEGER _4, ULONG _5, ULONG _6, ULONG _7, ULONG _8, PVOID _9, ULONG _10, bool isPreHook)
{
  NTSTATUS status = STATUS_OBJECT_PATH_NOT_FOUND;
  if (!_2->RootDirectory)
  {
    const wchar_t* pName = _2->ObjectName->Buffer,
      *pNameEnd = pName + _2->ObjectName->Length / 2;
    FsMount* pMount = FsMount::FindFile(pName, pNameEnd, isPreHook);
    if (pMount && !pName)
      status = STATUS_HOOKFS_PATH_HIDDEN;
    else if (pMount)
      status = pMount->OpenFile(_0, _1, pName, pNameEnd, _3, _5, _6, _7, _8);
  }
  return status;
}

NTSTATUS
FsClose( HANDLE _0 )
{
  NTSTATUS status = STATUS_HOOKFS_UNHANDLED;
  if( IsSpiced( _0 ) )
  {
    if( FindAndClearObject( &spImageSections, _0 ) )
      status = STATUS_SUCCESS;
  }
  else if( MaybeFakeHandle( _0 ) )
  {
    if( FindAndClearObject( &spFileObjects, Raw( _0 ) ) )
      status = STATUS_SUCCESS;
    else if( FindAndClearObject( &spDirObjects, Raw( _0 ) ) )
      status = STATUS_SUCCESS;
  }
  return status;
}

NTSTATUS
FsReadFile( HANDLE _0, HANDLE _1, PIO_APC_ROUTINE _2, PVOID _3, PIO_STATUS_BLOCK _4, PVOID _5, ULONG _6, PLARGE_INTEGER _7, PULONG _8 )
{
  NTSTATUS status = STATUS_HOOKFS_UNHANDLED;
  if( MaybeFakeHandle( _0 ) )
  {
    FileObject** p = FindObject( &spFileObjects, Raw( _0 ) );
    if( p && (*p)->stream )
    {
      status = STATUS_SUCCESS;
      NtHookFs::Stream* s = (*p)->stream;
      if( _7 && _7->HighPart != -1 && _7->LowPart != FILE_USE_FILE_POINTER_POSITION )
        if( s->SeekTo( s, _7->QuadPart ) != _7->QuadPart )
          status = STATUS_END_OF_FILE;
      _4->Information = (*p)->stream->Read( (*p)->stream, _5, _6 );
      if( _4->Information == 0 && _6 > 0 )
        status = STATUS_END_OF_FILE;
      _4->Status = status;
      if( _1 )
        ::SetEvent( _1 );
    }
  }
  return status;
}

NTSTATUS
FsWriteFile(HANDLE _0, HANDLE _1, PIO_APC_ROUTINE _2, PVOID _3, PIO_STATUS_BLOCK _4, PVOID _5, ULONG _6, PLARGE_INTEGER _7, PULONG _8)
{
  NTSTATUS status = STATUS_HOOKFS_UNHANDLED;
  if (MaybeFakeHandle(_0))
  {
    FileObject** p = FindObject(&spFileObjects, Raw(_0));
    if (p && (*p)->stream)
    {
      status = STATUS_SUCCESS;
      NtHookFs::Stream* s = (*p)->stream;
      if (_7 && _7->HighPart != -1 && _7->LowPart != FILE_USE_FILE_POINTER_POSITION)
        if (s->SeekTo(s, _7->QuadPart) != _7->QuadPart)
          status = STATUS_END_OF_FILE;
      _4->Information = (*p)->stream->Write((*p)->stream, _5, _6);
      if (_4->Information == 0 && _6 > 0)
        status = STATUS_DISK_FULL;
      _4->Status = status;
      if (_1)
        ::SetEvent(_1);
    }
  }
  return status;
}

bool
FsFilenameMatch( const wchar_t* begin, const wchar_t* end, const wchar_t* pat, const wchar_t* patEnd )
{ // No attempt at implementing weird Windows wildcard matching rules, no portable program will rely on those anyway.
  // * matches any sequence of characters, ? matches exactly one arbitrary character,
  // never match subdirectory entries, match case.
  const wchar_t* pName = begin, *pPat = pat;
  while( pName < end && pPat < patEnd )
  {
    if( *pName == L'\\' )
      return false;
    switch( *pPat )
    {
      case L'*':
        if( FsFilenameMatch( pName + 1, end, pPat + 1, patEnd ) )
          return true;
        break;
      default:
        if( *pPat != L'?' && *pName != *pPat )
          return false;
        ++pPat;
    }
    ++pName;
  }
  return pName == end && pPat == patEnd;
}

// TODO: enumerate actual objects in case of overlay mount
NTSTATUS
FsQueryDirectoryFile( HANDLE _0, HANDLE _1, PIO_APC_ROUTINE _2, PVOID _3, PIO_STATUS_BLOCK _4, PVOID _5, ULONG _6, FILE_INFORMATION_CLASS _7, BOOLEAN _8, PUNICODE_STRING _9, BOOLEAN _10 )
{
  NTSTATUS status = STATUS_HOOKFS_UNHANDLED;
  if( MaybeFakeHandle( _0 ) )
  {
    DirObject** pp = FindObject( &spDirObjects, Raw( _0 ) );
    if( pp && *pp )
    {
      DirObject* p = *pp;
      switch( _7 )
      {
      case FileDirectoryInformation:
      case FileFullDirectoryInformation:
      case FileBothDirectoryInformation:
        status = STATUS_SUCCESS;
        break;
      default:
        status = STATUS_NOT_IMPLEMENTED;
      }
      if( status == STATUS_SUCCESS ) switch( p->idxOut )
      {
      case -2:
        status = STATUS_NO_MORE_FILES;
        break;
      case -1:
        p->idxOut = 0;
        p->idxIn = 0;
        p->SetPattern( _9 );
        break;
      default:
        if( _10 ) // RestartScan
        {
          p->idxOut = 0;
          p->idxIn = 0;
        }
      }
      union { void* p; char* c; FILE_BOTH_DIR_INFORMATION* f; } outBegin = { _5 },
        outEnd = outBegin, out = outBegin;
      outEnd.c += _6;
      if( status == STATUS_SUCCESS )
      {
        ULONG* pOffsetLocation = nullptr;
        wchar_t* name = nullptr;
        FILE_BOTH_DIR_INFORMATION info;
        ptrdiff_t baseLength = 0;
        switch( _7 )
        {
        case FileBothDirectoryInformation:
          baseLength = (char*)info.FileName - (char*)&info;
          break;
        case FileFullDirectoryInformation:
          baseLength = (char*)&info.ShortNameLength - (char*)&info;
          break;
        case FileDirectoryInformation:
          baseLength = (char*)&info.EaSize - (char*)&info;
          break;
        };
        ::memset( outBegin.c, 0, Min( baseLength, outEnd.c - outBegin.c ) );
        for( int i = p->idxIn; p->pFs->StatByIndex( p->pFs, i, &name, &info ); ++i )
        {
          p->idxIn = i;
          int namelen = ::wcslen( name ), dirnamelen = p->dirnameEnd - p->dirname;
          if( namelen > dirnamelen && !::memcmp( name, p->dirname, dirnamelen ) )
          {
            const wchar_t* filename = name + dirnamelen, *nameEnd = name + namelen;
            if( *(nameEnd-1) == L'\\' )
              --nameEnd;
            if( !FsFilenameMatch( filename, nameEnd, p->pattern, p->patternEnd ) )
              p->idxIn = i + 1;
            else
            {
              int nameBytes = (nameEnd - filename) * sizeof(*nameEnd);
              WCHAR* nameLoc = (WCHAR*)(out.c + baseLength);
              if( out.c + Align<8>( baseLength + nameBytes ) < outEnd.c )
              {
                ::memcpy( out.c, &info, baseLength );
                if( pOffsetLocation )
                  *pOffsetLocation = out.c - outBegin.c;
                pOffsetLocation = &out.f->NextEntryOffset;
                out.f->FileNameLength = nameBytes;
                ::memcpy( nameLoc, filename, out.f->FileNameLength );
                out.c += Align<8>( baseLength + nameBytes );
                ++p->idxOut;
                p->idxIn = i + 1;
                if( _8 ) // ReturnSingleEntry
                  break;
              }
              else if( !pOffsetLocation && out.c + baseLength < outEnd.c )
              {
                ::memcpy( out.c, &info, baseLength );
                out.f->FileIndex = 0;
                out.f->FileNameLength = outEnd.c - out.c - baseLength;
                ::memcpy( nameLoc, filename, out.f->FileNameLength );
                out.c += out.f->FileNameLength;
                status = STATUS_BUFFER_OVERFLOW;
                break;
              }
              else break;
            }
          }
          delete[] name;
        }
        if( pOffsetLocation )
          *pOffsetLocation = 0;
        if( out.c == outBegin.c && status == STATUS_SUCCESS )
        {
          if( p->idxOut == 0 )
            status = STATUS_NO_SUCH_FILE;
          else
            status = STATUS_NO_MORE_FILES;
          p->idxOut = -2; // any further call will return STATUS_NO_MORE_FILES
        }
      }
      _4->Information = out.c - outBegin.c;
      _4->Status = status;
      if( _1 )
        ::SetEvent( _1 );
    }
  }
  return status;
}

NTSTATUS
FsQueryDirectoryFileEx(HANDLE _0, HANDLE _1, PIO_APC_ROUTINE _2, PVOID _3, PIO_STATUS_BLOCK _4, PVOID _5, ULONG _6, FILE_INFORMATION_CLASS _7, ULONG _8, PUNICODE_STRING _9)
{
	NTSTATUS status = FsQueryDirectoryFile(_0, _1, _2, _3, _4, _5, _6, _7, _8 & SL_RETURN_SINGLE_ENTRY, _9, _8 & SL_RESTART_SCAN);
	if (status != STATUS_HOOKFS_UNHANDLED && (_8 & SL_INDEX_SPECIFIED))
	{
		status = STATUS_NOT_IMPLEMENTED;
		_4->Status = status;
	}
	return status;
}

NTSTATUS
FsQueryInformationFile( HANDLE _0, PIO_STATUS_BLOCK _1, PVOID _2, ULONG _3, FILE_INFORMATION_CLASS _4 )
{
  NTSTATUS status = STATUS_HOOKFS_UNHANDLED;
  if( MaybeFakeHandle( _0 ) )
  {
    FileObject** p = FindObject( &spFileObjects, Raw( _0 ) );
    if( p ) switch( _4 )
    {
    case FilePositionInformation:
      if( _3 < sizeof( FILE_POSITION_INFORMATION ) )
        status = STATUS_BUFFER_TOO_SMALL;
      else
      {
        FILE_POSITION_INFORMATION* pFilePosition = static_cast<FILE_POSITION_INFORMATION*>( _2 );
        pFilePosition->CurrentByteOffset.QuadPart = (*p)->stream->Position( (*p)->stream );
        status = STATUS_SUCCESS;
      }
      break;
    case FileBasicInformation:
      if( _3 < sizeof( FILE_BASIC_INFORMATION ) )
        status = STATUS_BUFFER_TOO_SMALL;
      else
      {
        FILE_BASIC_INFORMATION* pInfo = static_cast<FILE_BASIC_INFORMATION*>( _2 );
        FILE_BOTH_DIR_INFORMATION info = { 0 };
        if( (*p)->mount->StatByIndex( (*p)->fsIndex, &info ) )
        {
          pInfo->ChangeTime = info.ChangeTime;
          pInfo->CreationTime = info.CreationTime;
          pInfo->FileAttributes = info.FileAttributes;
          pInfo->LastAccessTime = info.LastAccessTime;
          pInfo->LastWriteTime = info.LastWriteTime;
          status = STATUS_SUCCESS;
        }
      }
      break;
    case FileStandardInformation:
      if( _3 < sizeof( FILE_STANDARD_INFORMATION ) )
        status = STATUS_BUFFER_TOO_SMALL;
      else
      {
        FILE_STANDARD_INFORMATION* pInfo = static_cast<FILE_STANDARD_INFORMATION*>( _2 );
        FILE_BOTH_DIR_INFORMATION info = { 0 };
        if( (*p)->mount->StatByIndex( (*p)->fsIndex, &info ) )
        {
          pInfo->AllocationSize = info.AllocationSize;
          pInfo->EndOfFile = info.EndOfFile;
          pInfo->Directory = false;
          pInfo->NumberOfLinks = 0;
          pInfo->DeletePending = false;
          status = STATUS_SUCCESS;
        }
      }
      break;
    }
  }
  return status;
}

NTSTATUS
FsSetInformationFile( HANDLE _0, PIO_STATUS_BLOCK _1, PVOID _2, ULONG _3, FILE_INFORMATION_CLASS _4 )
{
  NTSTATUS status = STATUS_HOOKFS_UNHANDLED;
  if( MaybeFakeHandle( _0 ) )
  {
    if (_4 == FileBasicInformation)
    {
      if(_3 < sizeof(FILE_BASIC_INFORMATION))
        status = STATUS_INVALID_PARAMETER;
      else // TODO: store attributes
        status = STATUS_SUCCESS;
    }
    else if( _4 == FilePositionInformation )
    {
      if( _3 < sizeof( FILE_POSITION_INFORMATION ) )
        status = STATUS_INVALID_PARAMETER;
      else
      {
        FILE_POSITION_INFORMATION* pFilePosition = static_cast<FILE_POSITION_INFORMATION*>( _2 );
        FileObject** p = FindObject( &spFileObjects, Raw( _0 ) );
        if( p && (*p)->stream )
        {
          NtHookFs::Stream* s = (*p)->stream;
          if( s->SeekTo( s, pFilePosition->CurrentByteOffset.QuadPart ) != pFilePosition->CurrentByteOffset.QuadPart )
            status = STATUS_END_OF_FILE;
          else
            status = STATUS_SUCCESS;
        }
      }
    }
  }
  return status;
}

NTSTATUS
FsCreateSection( PHANDLE _0, ACCESS_MASK _1, POBJECT_ATTRIBUTES _2, PLARGE_INTEGER _3, ULONG _4, ULONG _5, HANDLE _6 )
{
  NTSTATUS status = STATUS_HOOKFS_UNHANDLED;
  if( MaybeFakeHandle( _6 ) )
  {
    FileObject** p = FindObject( &spFileObjects, Raw( _6 ) );
    if( p && (_5 & SEC_IMAGE) )
    {
      if( (*p)->mount->mTempDir.name.empty() )
      {
        IMAGE_NT_HEADERS hdr;
        status = PELoader::ReadImageHeader( (*p)->stream, &hdr );
        if( NT_SUCCESS( status ) )
        {
          LARGE_INTEGER size;
          if( _3 )
            size = *_3;
          else
            size.QuadPart = hdr.OptionalHeader.SizeOfImage;
          status = NtCreateSection( _0, _1, _2, &size, PAGE_EXECUTE_READWRITE, SEC_RESERVE, 0 );
        }
        if( NT_SUCCESS( status ) )
        {
          ImageSection* pSection = new ImageSection( *p, Spiced( *_0 ) );
          InsertObject( &spImageSections, pSection );
          *_0 = pSection->handle;
        }
      }
      else
      {
        HANDLE h = NULL;
        (*p)->stream->Retain();
        if( (*p)->mount->CreateTemporaryFile( (*p)->stream, &h ) )
        {
          status = NtCreateSection( _0, _1, _2, _3, _4, _5, h );
          ::CloseHandle(h);
        }
      }
    }
    else if( p && (*p)->stream )
    {
      NtHookFs::Stream* s = (*p)->stream;
      LARGE_INTEGER size;
      if( _3 )
        size = *_3;
      else
        size.QuadPart = s->EofPosition( s );
      status = NtCreateSection( _0, _1, _2, &size, PAGE_EXECUTE_READWRITE, SEC_COMMIT, 0 );
      if( NT_SUCCESS( status ) )
      {
        void* pMemory = nullptr;
        SIZE_T viewSize = 0;
        status = NtMapViewOfSection( *_0, ::GetCurrentProcess(), &pMemory, 0, 0, nullptr, &viewSize, ViewUnmap, 0, PAGE_READWRITE );
        if( NT_SUCCESS( status ) )
        {
          s->SeekTo( s, 0 );
          s->Read( s, pMemory, viewSize );
          NtUnmapViewOfSection( ::GetCurrentProcess(), pMemory );
        }
      }
    }
  }
  return status;
}

NTSTATUS
FsQuerySection( HANDLE _0, SECTION_INFORMATION_CLASS _1, PVOID _2, ULONG _3, PULONG _4 )
{
  NTSTATUS status = STATUS_HOOKFS_UNHANDLED;
  if( IsSpiced( _0 ) )
  {
    status = NtQuerySection( _0, _1, _2, _3, _4 );
    ImageSection** p = FindObject( &spImageSections, _0 );
    if( NT_SUCCESS( status ) &&  _1 == SectionBasicInformation && _3 >= sizeof(SECTION_BASIC_INFORMATION) )
    {
      int attr = SEC_FILE;
      if( p )
        attr |= SEC_IMAGE;
          static_cast<SECTION_BASIC_INFORMATION*>( _2 )->AllocationAttributes |= attr;
    }
    else if( status == STATUS_SECTION_NOT_IMAGE && p )
    {
      ::memset( _2, 0, _3 );
      status = STATUS_SUCCESS;
    }
  }
  return status;
}

NTSTATUS
FsMapViewOfSection( HANDLE _0, HANDLE _1, PVOID* _2, ULONG_PTR _3, SIZE_T _4, PLARGE_INTEGER _5, PSIZE_T _6, SECTION_INHERIT _7, ULONG _8, ULONG _9 )
{
  NTSTATUS status = STATUS_HOOKFS_UNHANDLED;
  if( IsSpiced( _0 ) )
  {
    ImageSection** p = FindObject( &spImageSections, _0 );
    if( p )
    {
      if( (*p)->view )
        status = STATUS_IMAGE_ALREADY_LOADED;
      else
      {
        IMAGE_NT_HEADERS hdr;
        status = PELoader::ReadImageHeader( (*p)->stream, &hdr );
        union { size_t n; void* p; } addr = { hdr.OptionalHeader.ImageBase };
        SIZE_T size = hdr.OptionalHeader.SizeOfImage;
        if( NT_SUCCESS( status ) )
        {
          status = NtMapViewOfSection( _0, _1, &addr.p, _3, size, _5, &size, ViewUnmap, 0, PAGE_EXECUTE_READWRITE );
          if( status == STATUS_CONFLICTING_ADDRESSES )
          {
            addr.n = 0;
            status = NtMapViewOfSection( _0, _1, &addr.p, _3, size, _5, &size, ViewUnmap, 0, PAGE_EXECUTE_READWRITE );
          }
        }
        if( NT_SUCCESS( status ) )
        {
          status = PELoader::ReadImageData( (*p)->stream, addr.p );
          PELoader::ApplyExePatch( addr.p );
        }
        if( NT_SUCCESS( status ) )
          status = PELoader::ApplyRelocation( addr.p );
        if( NT_SUCCESS( status ) )
        {
          NTSTATUS s = PELoader::ApplyMemoryProtection( addr.p );
          if( NT_ERROR(s) )
            status = s;
        }
        if( NT_SUCCESS( status ) )
        {
          *_2 = addr.p;
          *_6 = hdr.OptionalHeader.SizeOfImage;
          if( _5 )
            _5->QuadPart = 0;
          InsertObject( &spImageViews, new ImageView( *p, *_2 ) );
        }
      }
    }
  }
  return status;
}

NTSTATUS
FsUnmapViewOfSection( HANDLE _0, PVOID _1 )
{
  NTSTATUS status = STATUS_HOOKFS_UNHANDLED;
  if( FindAndClearObject( &spImageViews, _1 ) )
    status = NtUnmapViewOfSection( _0, _1 );
  return status;
}

} // namespace
