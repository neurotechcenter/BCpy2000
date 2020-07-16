///////////////////////////////////////////////////////////////////////////////
// $Id$
// Author: juergen.mellinger@uni-tuebingen.de
// Description: Entry function and TLS callback for a bundled application.
//   When prepended to a zip archive, the stub will make archive contents
//   available as if on a read-only file system, and will then load and run the
//   bundle's main executable.
//   The bundle's main executable has been stored with its entry point set to
//   zero, and the DLL flag set in its PE header, so it can be loaded as a DLL,
//   and can be executed without the overhead of creating a new process.
//   Also, if possible, files are extracted into memory rather than to
//   temporary files, so loading a bundled application may be faster than
//   loading the same application and its dependencies from disk uncompressed.
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
#include "Stub.h"
#include "Bundle.h"
#include "ZipFileSystem.h"
#include "MiniCRT.h"
#include "PEHeader.h"
#include "Utils.h"

template<class T>
static T* PathFindFileName_( T* p ) // don't want to link to shlwapi.dll
{
  T* f = p;
  while( *f ) ++f;
  while( f > p && *(f-1) != L'\\' && *(f-1) != L'/' )
    --f;
  return f;
}

static struct
{
  ZipFileSystem* pFileSystem;
  bool hooksInstalled, writable;
  WString* path;
} sBundle;

static struct
{
  HMODULE handle;
  DWORD error, exception;
  WString* detail;
} sMainExecutable;

static void MountBundle()
{
  sBundle.hooksInstalled = NtHookFs::Install();
  // NOTE: To notify an attached debugger about module loading, a true executable file
  // must be loaded, i.e. we need to use temporary files.
  // Reloading module information inside the debugger should always work, however
  // in order for a breakpoint to be hit reliably inside a memory-only module, that module
  // must be known to the debugger from start.
  // Thus, we prefer temporary files if a debugger is attached at load time, and avoid
  // them otherwise if possible.
  BOOL beingDebugged = false;
  if( !::CheckRemoteDebuggerPresent( ::GetCurrentProcess(), &beingDebugged ) )
    beingDebugged = ::IsDebuggerPresent();
  bool useTempFiles = !sBundle.hooksInstalled || beingDebugged,
       dumpToTemp = !sBundle.hooksInstalled;
  wchar_t name[MAX_PATH + 1];
  if( ::GetModuleFileNameW( 0, name, MAX_PATH + 1 ) )
  {
    sBundle.pFileSystem = new ZipFileSystem( name );
    int flags = NtHookFs::MountFlags::Opaque;
    if( useTempFiles )
      flags |= NtHookFs::MountFlags::UseTempFiles;
    if( dumpToTemp )
      flags |= NtHookFs::MountFlags::DumpToTemp;
    if( sBundle.writable )
      flags |= NtHookFs::MountFlags::Writable;
    // We need to place our virtual file system into an 8.3 compatible DOS path
    // inside an existing local drive, or we may get into trouble with long vs 
    // short file names, UNC naming, MAX_PATH limit applying or not, ...
    const wchar_t* path = sBundle.pFileSystem->Mount( L"C:\\current.app\\", flags );
    sBundle.path = new WString( path );
  }
}

static void LoadMainExecutable()
{
  if( sBundle.path )
  {
    WString exe = *sBundle.path + Bundle::MainExecutableW();
    ::SetDllDirectoryW( sBundle.path->c_str() );
    if( sBundle.hooksInstalled )
    {
      ::SetErrorMode( ::SetErrorMode( 0 ) | SEM_FAILCRITICALERRORS | SEM_NOOPENFILEERRORBOX );
      NtHookFs::EnableFileTracking();
    }
    sMainExecutable.handle = ::LoadLibraryExW( exe.c_str(), 0, LOAD_WITH_ALTERED_SEARCH_PATH );
    if( !sMainExecutable.handle )
    {
      sMainExecutable.error = ::GetLastError();
      const wchar_t* file = NtHookFs::DisableFileTracking();
      sMainExecutable.detail = new WString( file );
    }
  }
}

static void RunMainExecutable()
{
  if( sMainExecutable.handle && !sMainExecutable.error )
  {
    PEHeader exe( sMainExecutable.handle );
    ptrdiff_t entryPointRva = exe.AddressOfEntryPoint();
    if( !entryPointRva )
    {
      entryPointRva = exe.DosHeader()->e_reserved;
      if( entryPointRva )
      {
        union { char* c; void (WINAPI *f)(); } entryPoint = { exe.BaseAddress() + entryPointRva };
        entryPoint.f(); // will not return
      }
    }
  }
}

static void ExitWithError()
{
  wchar_t buf[MAX_PATH + 1];
  const int bufsize = sizeof(buf)/sizeof(*buf);
  ::GetModuleFileNameW( 0, buf, bufsize );
  ::GetLongPathNameW( buf, buf, bufsize );

  wchar_t* title = nullptr, *text = nullptr;
  const wchar_t* file = ::PathFindFileName_( sMainExecutable.detail->c_str() );
  const void* args[] = { file, ::PathFindFileName_( buf ), (void*)(size_t)sMainExecutable.exception, L"", L"", L"", L"", nullptr };
  DWORD flags = FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_ARGUMENT_ARRAY;
  ::FormatMessageW( flags | FORMAT_MESSAGE_FROM_STRING, L"Error running Bundled Application %2", 0, 0, (LPWSTR)&title, 0, (va_list*)args );
  if( !title )
    title = L"Error running Bundled Application";

  wchar_t* msg = nullptr;
  switch( sMainExecutable.error )
  {
    case ERROR_MOD_NOT_FOUND:
      if( *file )
      {
        msg = L"Executable module %1\nis required to run %2.\n\n"
              L"It is not located inside the bundle or in the system search path.";
        file = nullptr;
      }
      break;
    case ERROR_BAD_EXE_FORMAT:
      // Original system error message "%1 is not a valid Win32 application."
      // is misleading in case of DLL executables.
      if( *file )
        msg = L"Module %1 has an invalid executable format.", file = nullptr;
      else
        msg = L"Invalid executable format.";
      break;
    case ERROR_UNHANDLED_EXCEPTION:
      msg = L"Unhandled exception 0x%3!08x! when loading bundle.";
      break;
    case NO_ERROR:
      msg = L"Unexpected error.\nApplication bundle may be corrupted.";
      break;
  }
  flags |= msg ? FORMAT_MESSAGE_FROM_STRING : FORMAT_MESSAGE_FROM_SYSTEM;
  wchar_t* buf1 = nullptr;
  ::FormatMessageW( flags, msg, sMainExecutable.error, 0, (wchar_t*)&buf1, 0, (va_list*)args );
  text = buf1 ? buf1 : msg;
  union { int i; void* p; } err = { sMainExecutable.error };
  args[0] = err.p;
  args[1] = text;
  args[2] = file;
  flags = FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_ARGUMENT_ARRAY | FORMAT_MESSAGE_FROM_STRING;
  wchar_t* buf2 = nullptr;
  msg = file && *file ? L"System error 0x%1!08x!:\n%2This error may be related to file %3." : L"System error 0x%1!08x!:\n\n%2";
  ::FormatMessageW( flags | 80, msg, 0, 0, (wchar_t*)&buf2, 0, (va_list*)args );
  ::LocalFree( buf1 );
  text = buf2 ? buf2 : text;
  ::MessageBoxW( 0, text, title, MB_OK | MB_ICONERROR );
  ::LocalFree( buf2 );
  ::ExitProcess( -1 );
}

static LONG CALLBACK UncaughtExceptionHandler( EXCEPTION_POINTERS* p )
{
  sMainExecutable.error = ERROR_UNHANDLED_EXCEPTION;
  sMainExecutable.exception = p->ExceptionRecord->ExceptionCode;
  ExitWithError(); // will not return
  return 0;
}

void WINAPI StubEntryPoint()
{
  void* handler = ::AddVectoredExceptionHandler( 0, &UncaughtExceptionHandler );
  MiniCRT_init();
  MountBundle();
  LoadMainExecutable();
  if( !::RemoveVectoredExceptionHandler( handler ) )
    sMainExecutable.error = ::GetLastError();
  RunMainExecutable(); // will not return if successful
  ExitWithError();
}

static void Cleanup()
{
  if( sMainExecutable.handle )
  {
    ::FreeLibrary( sMainExecutable.handle );
    sMainExecutable.handle = 0;
  }
  if( sBundle.pFileSystem )
  {
    delete sBundle.pFileSystem;
    sBundle.pFileSystem = 0;
  }
}

// TlsCallback is guaranteed to run no matter how a process terminates,
// so we use it to make sure any temporary files are cleaned up.
void WINAPI StubTlsCallback( LPVOID, DWORD reason, LPVOID )
{
  switch( reason )
  {
    case DLL_PROCESS_DETACH:
      Cleanup();
      break;
  }
}

