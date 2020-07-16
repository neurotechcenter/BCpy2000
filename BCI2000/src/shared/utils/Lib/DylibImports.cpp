////////////////////////////////////////////////////////////////////////////////
// $Id: DylibImports.cpp 5617 2017-02-13 16:38:50Z mellinger $
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
#include "DylibImports.h"
#include "FileUtils.h"
#include "ProcessUtils.h"
#include "Exception.h"
#include "Debugging.h"
#include "FileMapping.h"
#include <vector>
#include <regex>

#if _WIN32
# include <Windows.h>
# include <WinNT.h>
# include "PEHeader.h"
#else
# if __APPLE__
#  include <mach-o/dyld.h>
#  include "MachOHeader.h"
# else
//#  include "ELFHeader.h"
#  include <cstring>
# endif
# include <unistd.h>
# include <dlfcn.h>
# include <sys/mman.h>
#endif

using namespace std;
using namespace Dylib;

static size_t sArchBits = 8 * sizeof( void* );

#if _WIN32
#define OS_WOW6432 30
#define IsOS_ORDINAL 437
static BOOL (WINAPI *IsOS_)( DWORD ) = 0;
static void* LoadLibrary_( const char* s )
{
  HMODULE h = 0;
  UINT errorMode =::SetErrorMode( SEM_FAILCRITICALERRORS );
  if( !IsOS_ )
  {
    HMODULE lib = ::LoadLibraryA( "shlwapi" );
    if( lib )
    {
      const char* name = reinterpret_cast<const char*>( IsOS_ORDINAL );
      *reinterpret_cast<void**>( &IsOS_ ) = ::GetProcAddress( lib, name );
    }
  }
  try
  {
    h = ::LoadLibraryA( s );
    if( !h )
    {
      string path = FileUtils::CanonicalPath( s );
      if( path.empty() )
        path = FileUtils::CanonicalPath( string( s ) + ".dll" );
      if( !path.empty() )
      {
        ProcessUtils::TemporaryProcessWD wd( FileUtils::ExtractDirectory( path ) );
        h = ::LoadLibraryA( path.c_str() );
      }
    }
  }
  catch( ... )
  {
  }
  ::SetErrorMode( errorMode );
  return h;
}
#undef LoadLibrary
#define LoadLibraryA *dontuse*
#define LoadLibraryW *dontuse*

#endif // _WIN32

static void*
LoadDylib( string& ioName )
{
  void* handle = 0;
#if _WIN32
  handle = LoadLibrary_( ioName.c_str() );
  if( !handle )
  {
    ostringstream oss;
    oss << ioName << sArchBits;
    handle = LoadLibrary_( oss.str().c_str() );
    if( !handle )
    {
      oss.str( "" );
      oss << ioName << "_" << sArchBits;
      handle = LoadLibrary_( oss.str().c_str() );
    }
  }
  if( !handle && sArchBits == 32 )
    handle = LoadLibrary_( ( ioName + "_x86" ).c_str() );
  if( !handle && sArchBits == 64 )
    handle = LoadLibrary_( ( ioName + "_x64" ).c_str() );

  ioName.clear();
  if( handle )
  {
    char buf[MAX_PATH + 1] = { 0 };
    if( ::GetModuleFileNameA( HMODULE( handle ), buf, sizeof(buf) - 1 ) )
      ioName = buf;
  }    
#else
# if __APPLE__
  ioName += ".dylib";
# else
  ioName += ".so";
# endif
  handle = ::dlopen( ioName.c_str(), RTLD_LAZY | RTLD_LOCAL );
#endif // _WIN32
  return handle;
}

static void
UnloadDylib( void* inHandle )
{
#if _WIN32
  if( inHandle )
    ::FreeLibrary( HMODULE( inHandle ) );
#else
  if( inHandle )
    ::dlclose( inHandle );
#endif
}

struct Unprotect
{
  Unprotect( void* addr, size_t length )
  : mLength( length ), mOriginal( 0 )
  {
    mAddr.p = addr;
#if _WIN32
    DWORD protect = PAGE_EXECUTE_READWRITE; // might be located within the same page as this code
    ::VirtualProtect( mAddr.p, mLength, protect, &protect );
    mOriginal = protect;
#else
    long size = ::sysconf(_SC_PAGESIZE);
    mAddr.n = (mAddr.n / size) * size;
    mLength = ((length + size - 1) / size) * size;
    ::mprotect( mAddr.p, mLength, PROT_WRITE | PROT_EXEC);
    mOriginal = PROT_READ | PROT_EXEC;
#endif
  }
  ~Unprotect()
  {
#if _WIN32
    DWORD protect = mOriginal;
    ::VirtualProtect( mAddr.p, mLength, protect, &protect );
    ::FlushInstructionCache( ::GetCurrentProcess(), mAddr.p, mLength );
#else
    ::mprotect( mAddr.p, mLength, mOriginal );
#endif
  }
  union { void* p; size_t n; } mAddr;
  size_t mLength;
  int mOriginal;
};

static const struct Absjmp
{
  const char* code;
  size_t size;
  int targetLo, targetHi;
}
sAbsjmp[/* indices matching Dylib::Arch::Type enum */] =
{
  { // x86
    "\x68\0\0\0\0" // push imm32
    "\xC3", // ret
    6, 1, -1
  },
  { // amd64
    "\x68\0\0\0\0" // push imm32 (will extend value to 64 bits)
    "\xC7\x44\x24\x04\0\0\0\0" // mov [rsp+4], imm32
    "\xC3", // ret
    5 + 8 + 1, 1, 9
  },
};

void
WriteJumpInstruction( void* inStub, uint64_t inTargetAddress, int inMachine )
{
  const Absjmp& absjmp = sAbsjmp[inMachine];
  Unprotect allowWrite( inStub, absjmp.size );
  ::memcpy( inStub, absjmp.code, absjmp.size );
  union { void* p; char* c; uint32_t* i; } pData = { inStub };
  pData.c += absjmp.targetLo;
  *pData.i = inTargetAddress & 0xffffffff;
  if( absjmp.targetHi >= 0 )
  {
    pData.p = inStub;
    pData.c += absjmp.targetHi;
    *pData.i = inTargetAddress >> 32;
  }
}

static void*
ResolveExport( void* inHandle, const std::string& inName )
{
#if _WIN32
  return ::GetProcAddress( HMODULE( inHandle ), inName.c_str() );
#else
  return ::dlsym( inHandle, inName.c_str() );
#endif
}

static int
ListExports( void* inHandle, std::vector<std::string>& outExports )
{
  int result = 0;
  outExports.clear();
#if _WIN32
  PEHeader pe( inHandle );
  PEFile file( pe, true );
  PEFile::SymbolInfo info = { 0 };
  while( file.NextExport( info ) )
    if( info.name )
      outExports.push_back( info.name );
  result = pe.Bits();
#endif // _WIN32
  return result;
}

// Library class
Library::Library( const std::string& lib )
{
  mNames.push_front( lib );
  Init();
}

Library::Library( const std::string& lib, const Dylib::Names& names )
: mNames( names )
{
  if( mNames.empty() || mNames.front() != lib )
    mNames.push_front( lib );
  Init();
}

Library::~Library()
{
  UnloadDylib( mHandle );
}

void
Library::Init()
{
  mHandle = 0;
  mState = none;
  mArch = Arch::unknown;
  string name;
  for( Names::const_iterator i = mNames.begin(); !mHandle && i != mNames.end(); ++i )
  {
    name = *i;
    mHandle = LoadDylib( name );
  }
  if( !mHandle && mError.empty() )
    mError = "Could not load library " + Name();
  mState = ( mHandle ? found : notFound );
#if _WIN32
  if (mHandle) switch (PEHeader(mHandle).CoffFileHeader()->Machine)
  {
    case IMAGE_FILE_MACHINE_I386:
      mArch = Arch::x86;
      break;
    case IMAGE_FILE_MACHINE_AMD64:
      mArch = Arch::amd64;
      break;
  }
#else // only handle native case on other systems
  mArch = sizeof(void*) == 4 ? Arch::x86 : Arch::amd64;
#endif
}

string
Library::Name() const
{
  string name = "<n/a>";
  if( !mNames.empty() )
  {
    name = "\"" + *mNames.begin() + "\"";
    for( Dylib::Names::const_iterator i = ++mNames.begin(); i != mNames.end(); ++i )
      name += " alias \"" + *i + "\"";
  }
  return name;
}

int
Library::Find( const char* inName, int inOptions, void** outResult, int* outStdcallStackBytes )
{
  if(outStdcallStackBytes)
    *outStdcallStackBytes = -1;
  void* p = ResolveExport( mHandle, inName );
  int count = p ? 1 : 0;
#if _WIN32
  if(!p && !(inOptions & Import::cppMangled) && mArch == Arch::x86)
  { // might be extern "C" and _stdcall
    if(mExports.empty() && !ListExports(mHandle, mExports))
      throw std_runtime_error << "Could not list exports";
    std::regex pattern("_" + std::string(inName) + "(@[0-9]+)?", std::regex::optimize);
    std::smatch match;
    for( auto i = mExports.begin(); !p && i != mExports.end(); ++i )
    {
      if( std::regex_match( *i, match, pattern ) )
      {
        ++count;
        p = ResolveExport( mHandle, *i );
        if(outStdcallStackBytes && match.size() > 1)
          *outStdcallStackBytes = ::atoi(i->c_str() + match.position(1) + 1);
      }
    }
  }
#endif
  if( !p && (inOptions & Import::cppMangled) )
  {
    if( mExports.empty() && !ListExports( mHandle, mExports ) )
      throw std_runtime_error << "C++ mangled names are not supported on this platform";
    std::regex pattern( ".*[^a-zA-Z]" + std::string(inName) + "[^a-zA-Z].*", std::regex::optimize );
    for( auto i = mExports.begin(); !p && i != mExports.end(); ++i )
      if( std::regex_match( *i, pattern ) )
        p = ResolveExport( mHandle, *i ), ++count;
  }
  if(outResult)
    *outResult = p;
  return count;
}

bool
Library::Resolve( const Import* inImports, int inCount )
{
  if( mState != found )
    return false;

  bool foundAll = true, foundSome = false;
  mError.clear();
  const Import* p = inImports;
  while( p->name && ( inCount < 1 || p < inImports + inCount ) )
  {
    void* addr = 0;
    int matches = Find( p->name, p->options, &addr );
    if( matches > 1 && ( p->options & Import::cppMangled ) )
    {
      mError += "C++ function ";
      mError += p->name;
      mError += " is overloaded, cannot decide which to use\n";
      addr = 0;
    }
    else if( matches > 1 )
    {
      mError += "Ambiguous import ";
      mError += p->name;
      mError += "\n";
      addr = 0;
    }
    else if( !( p->options & Import::optional ) && !addr )
    {
      mError += "Function ";
      mError += p->name;
      mError += " not found in library\n";
    }
    if( addr )
      foundSome = true;
    else
      foundAll = false;
    if( p->options & Import::functionStub )
    {
      union { void* p; size_t s; } targetAddr = { addr };
      WriteJumpInstruction(p->pointer, targetAddr.s, mArch);
    }
    else
    {
      Assert( *p->pointer == 0 );
      *p->pointer = addr;
    }
    ++p;
  }
  if( foundAll )
    mState = resolvedAll;
  else if( foundSome )
    mState = resolvedSome;
  else
    mState = resolvedNone;
  return mError.empty();
}

// StartupLoader class
StartupLoader::StartupLoader( const char* inLib, const Import* inImports )
: Library( inLib )
{
  if( Library::Error().empty() )
    Library::Resolve( inImports, 0 );
}

StartupLoader::StartupLoader( const char* inLib, const char* inAliases, const Import* inImports, const char* inMsg, const char* inUrl, ThrowFunc inF )
: Library( inLib, ParseAliases( inAliases ) )
{
  if( !Library::Error().empty() )
  {
    string exe = FileUtils::ExtractBase( FileUtils::ExecutablePath() ),
           msg = inMsg ? inMsg : "",
           url = inUrl ? inUrl : "";
    if( msg.empty() )
    {
      ostringstream oss;
      oss << sArchBits << "-bit Library "
          << Library::Name()
          << " could not be loaded, but is necessary for "
          << exe
          << " to run.";
      bool isWow = false;
#if _WIN32
      isWow = ( IsOS_ && IsOS_( OS_WOW6432 ) );
      if( isWow )
        oss << "\nNOTE: You are running a 32-bit executable on a 64-bit Windows installation. "
            << "This requires a 32-bit version of the "
            << Library::Name()
            << " DLL to be available. "
            << "32-bit system DLLs must reside in the SysWOW64 (yes, actually \"64\") "
            << "subdirectory of your Windows system directory.";
#endif // _WIN32
      std::regex sourcemodule( ".*(source|adc).*", std::regex::icase );
      if( std::regex_match( exe, sourcemodule ) )
      {
        oss << "\nYou may need to install ";
        if( isWow )
          oss << "the 32-bit version of ";
        oss << "the driver software that came with your amplifier.";
      }
      msg = oss.str();
    }
    BuildMessage( msg, url );
  }
  else if( !Library::Resolve( inImports, 0 ) )
  {
    string msg = "Could not load functions from dynamic library " + Library::Name()
               + " due to an error: " + Library::Error();
    if( Library::State() == resolvedSome )
      msg += "An update to that library/driver may be necessary.";
    BuildMessage( msg, "" );
  }
  if( !mMessage.empty() )
    for( const Import* p = inImports; p->name; ++p )
      if( !*p->pointer && !(p->options & Import::functionStub) )
        *p->pointer = reinterpret_cast<void*>( inF );
}

Dylib::Names
StartupLoader::ParseAliases( const char* inAliases )
{
  Dylib::Names names;
  const char* p = inAliases;
  if( p ) do
  {
    string name;
    while( *p && *p != '|' )
      name += *p++;
    names.push_back( name );
  } while( *p++ );
  return names;
}

void
StartupLoader::BuildMessage( const string& inMsg, const string& inUrl )
{
  mMessage = inMsg;
  string url = inUrl;
  if( url.empty() && !Library::Names().empty() )
  {
    Dylib::Names::const_iterator i = Library::Names().begin();
    string names = *i++;
    for( ; i != Library::Names().end(); ++i )
      names += "%20OR%20" + *i;
    url = FileUtils::SearchURL( names );
  }
  if( !url.empty() )
    mMessage += " More information may be available at:\n" + url;
}

void
StartupLoader::UninitializedFunctionExport( const char* inFunctionName ) const
{ // This function will normally not be executed.
  // Each function stub contains a call to this function, and thus
  // enforces initialization of the associated library object before
  // any of the library's functions are called.
  // Initialization of the library, in turn, patches the stub, replacing
  // the call to this function with a jump into the imported libary function.
  if( State() == none )
    throw std_runtime_error
      << "Function " << inFunctionName << " from dynamic library "
      << Library::Name() << " was called at initialization time, "
      << "before the library was loaded.";
  if( mMessage.empty() )
    throw std_runtime_error
      << "The current program requires function \"" << inFunctionName
      << "\" to be present in dynamic library " << Library::Name()
      << " but it was not found in the library.\n"
      << "Updating " << Library::Name() << " to a newer version"
      << " might solve this problem.";
  throw std_runtime_error << mMessage;
}

void*
StartupLoader::ResolveDataExport( const char* inName, int inFlags )
{
  void* addr = 0;
  if( Library::Error().empty() )
  {
    bool found = Library::Find( inName, Import::exactMatch, &addr );
    if( !found && !(inFlags & Import::optional) )
      throw std_runtime_error
        << "Could not resolve data symbol " << inName
        << " from dynamic library" << Library::Name();
  }
  return addr;
}

void
StartupLoader::ThrowError() const
{
  throw std_runtime_error << mMessage;
}

