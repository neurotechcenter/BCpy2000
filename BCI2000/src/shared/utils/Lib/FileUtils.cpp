//////////////////////////////////////////////////////////////////////
// $Id: FileUtils.cpp 5833 2018-12-04 15:10:28Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: File-system related utility functions.
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
///////////////////////////////////////////////////////////////////////
#if _WIN32
# include <Windows.h>
# define S_ISLNK(x) ( false )
# if _MSC_VER
#  include <direct.h>
#  define S_ISREG(x) ( (x) & S_IFREG )
#  define S_ISDIR(x) ( (x) & S_IFDIR )
# else // _MSC_VER
#  include <dir.h>
# endif // _MSC_VER
#elif __APPLE__
# include <mach-o/dyld.h>
#endif // _WIN32

#if _MSC_VER
# include "../compat/dirent_win.h"
#else // _MSC_VER
# include <dirent.h>
#endif // _MSC_VER

#ifndef _WIN32
# include <cstdio>
# include <unistd.h>
#endif // _WIN32

#include <sys/stat.h>
#include <cstdlib>
#include <cerrno>

#include "FileUtils.h"
#include "FilePath.h"
#include "StringUtils.h"
#include "ProcessUtils.h"
#include "ThreadLocal.h"
#include "LCRandomGenerator.h"
#include "Debugging.h"

using namespace std;
using namespace Tiny::FileUtils;
using Tiny::FilePath;

namespace {

  struct WD_ : std::string
  {
    WD_()
      : std::string( EnsureSeparator( ProcessUtils::InitialProcessWD() ) ) {}
    std::string& operator=( const std::string& s )
      { return std::string::operator=( s ); }
  };
  ThreadLocalObject<WD_> ThreadWD;

#if _WIN32

  FilePath& NormalizePathElements(FilePath& path)
  {
    int i = 0;
    std::string r;
    if (path.HasRoot())
    {
      if (path.RootIsDrive())
        path.SetRoot(StringUtils::ToUpper(path.Root()));
      if (path.Kind() == FilePath::ambiguous)
        r = FilePath(WorkingDirectory()).Left(0) + path.NativeSeparator();
    }
    else
    {
      while (path.IsRelativeElement(i))
        ++i;
      r = WorkingDirectory();
    }
    while (i < path.ElementCount())
    {
      std::string s = r + path.Left(i + 1);
      WIN32_FIND_DATAW findFileData = { 0 };
      HANDLE h = ::FindFirstFileW(StringUtils::ToWide(s).c_str(), &findFileData);
      if(h == INVALID_HANDLE_VALUE)
        break;
      ::FindClose(h);
      path.SetElement(i, StringUtils::ToNarrow(findFileData.cFileName));
      ++i;
    }
    return path;
  }

#else // WIN32

  FilePath& NormalizePathElements( FilePath& path )
  {
    int i = 0;
    std::string r;
    if( path.HasRoot() )
    {
      if( path.RootIsDrive() )
        path.SetRoot( StringUtils::ToUpper( path.Root() ) );
    }
    else
    {
      while( path.IsRelativeElement( i ) )
        ++i;
      r = WorkingDirectory();
    }
    std::string s = r + path.Left( i );
    bool done = false;
    while( !done && i < path.ElementCount() && IsDirectory( s ) )
    {
      // This is inefficient because we need to obtain the canonical path for half the 
      // siblings on each level but we may get away with it on efficiently implemented
      // file systems.
      List names;
      ListDirectory( s, names );
      s += path.NativeSeparator();
      std::string c = CanonicalPath( s + path.Element( i ) );
      done = c.empty();
      if( !done )
      {
        for( const auto& name : names )
        {
          if( CanonicalPath( s + name ) == c )
          {
            path.SetElement( i, name );
            break;
          }
        }
        s = r + path.Left( ++i );
      }
    }
    return path;
  }
#endif // WIN32

  string
  GetExecutablePath()
  {
    string path;
#if _WIN32
    char* pFileName = NULL;
    DWORD size = 1024; // Must be large enough to hold path in WinXP.
    DWORD result = 0;
    do {
      delete[] pFileName;
      pFileName = new char[size];
      result = ::GetModuleFileNameA( NULL, pFileName, size );
      size *= 2;
    } while( result != 0 && ::GetLastError() == ERROR_INSUFFICIENT_BUFFER );
    if( result != 0 )
      path = pFileName;
    delete[] pFileName;
#else // _WIN32
# if _GNU_SOURCE
    path = program_invocation_name;
# elif __APPLE__
    uint32_t size = 0;
    ::_NSGetExecutablePath( NULL, &size );
    char buf[size];
    buf[0] = '\0';
    ::_NSGetExecutablePath( buf, &size );
    path = buf;
# else
#  error Don't know how to obtain the executable's path on this platform.
# endif
    path = FilePath( path ).ToAbsolute( ProcessUtils::InitialProcessWD() ).ToString();
#endif // _WIN32
    path = NormalizedPath( path );
    if( path.empty() )
      throw std_runtime_error << "Could not determine executable path";
    return path;
  }

  string GetInstallationDirectory()
  {
    return ParentDirectory( ExecutablePath() );
  }

  string GetTemporaryDirectory()
  {
    string dir;
#if _WIN32
    int length = ::GetTempPathA( 0, nullptr );
    std::vector<char> buf( length );
    if( ::GetTempPathA( buf.size(), buf.data() ) )
      dir = CanonicalPath( ExtractDirectory( buf.data() ) );
#else
    char* p = ::getenv( "TMPDIR" );
    if( p )
      dir = CanonicalPath( p );
#endif
    if( dir.empty() )
      throw std_runtime_error << "Could not get temporary directory";
    return dir;
  };
}

string
FileUtils::ExecutablePath()
{
  return StaticBuffer<string, &GetExecutablePath>();
}

std::string
FileUtils::InstallationDirectory()
{
  return StaticBuffer<string, &GetInstallationDirectory>();
}

string
FileUtils::TemporaryDirectory()
{
  return StaticBuffer<string, &GetTemporaryDirectory>();
}

string
FileUtils::EnsureSeparator( const string& inDir, const std::string& inSep )
{
  if( inDir.empty() || FilePath::IsDirSeparator( inDir.back() ) )
    return inDir;
  if( inSep.empty() )
    return inDir + FilePath::DirSeparator;
  return inDir + inSep;
}

std::string
FileUtils::StripSeparator( const std::string& inDir )
{
  if( inDir.empty() || !FilePath::IsDirSeparator( inDir.back() ) )
    return inDir;
  int i = inDir.length() - 1;
  while( i > 0 && FilePath::IsDirSeparator( inDir[i - 1] ) )
    --i;
  return inDir.substr( 0, i );
}

string
FileUtils::ApplicationTitle()
{
  string baseName = ExtractBase( ExecutablePath() ),
         title;
  bool wasSpace = true,
       wasUpper = false;
  for( string::const_iterator i = baseName.begin(); i != baseName.end(); ++i )
  {
    if( ::isspace( *i ) )
    {
      wasSpace = true;
      wasUpper = false;
    }
    else
    {
      if( ::isupper( *i ) && !wasSpace && !wasUpper )
        title += " ";
      if( ::isupper( *i ) )
        wasUpper = true;
      else
        wasUpper = false;
      wasSpace = false;
    }
    title += *i;
  }
  return title;
}

string
FileUtils::WorkingDirectory()
{
  return *ThreadWD;
}

bool
FileUtils::ChangeDirectory( const string& inDir )
{
  std::string dir = AbsolutePath( inDir );
  if( !IsDirectory( dir ) )
    return false;
  *ThreadWD = EnsureSeparator( dir );
  return true;
}

string
FileUtils::AbsolutePath( const string& inPath )
{
  if( inPath.empty() )
    return inPath;
  FilePath path( inPath );
  if( path.Kind() == FilePath::absolute )
    return inPath;
#if _WIN32
  if( path.Kind() == FilePath::ambiguous )
    return FilePath( WorkingDirectory() ).Root() + inPath;
#else
  if( path.Kind() == FilePath::ambiguous )
    return inPath;
#endif
  return WorkingDirectory() + inPath;
}

string
FileUtils::CanonicalPath( const std::string& inPath )
{
  if( inPath.empty() )
    return inPath;

  FilePath path( inPath );
  path.ToAbsolute( WorkingDirectory() ).Simplify();
  string result;
#if _WIN32
  // GetFullPathNameW() would return \\?\ when given \\?\C:\.. as input,
  // but we expect \\?\C:\
  // So let's avoid GetFullPathName() altogether, and do our own path normalization
  // before calling Get*PathName().
  wstring wpath = StringUtils::ToWide( "\\\\?\\" + path.ToString( "\\" ) );
  DWORD size = ::GetShortPathNameW( wpath.c_str(), 0, 0 );
  if( size == 0 )
  {
    wpath += L'\\';
    size = ::GetLongPathNameW( wpath.c_str(), 0, 0 );
  }
  if( size > 0 )
  {
    std::vector<wchar_t> buf( size + 1 );
    if( ::GetShortPathNameW( wpath.c_str(), buf.data(), size ) )
      wpath = buf.data();
    result = StringUtils::ToNarrow( wpath ).substr( string( "\\\\?\\" ).length() );
  }
  result = StringUtils::ToUpper( result );
#elif _GNU_SOURCE
  char* pPath = ::canonicalize_file_name( path.ToString( "/" ).c_str() );
  if( pPath )
  {
    result = pPath;
    ::free( pPath );
  }
#elif __APPLE__
  char buffer[PATH_MAX + 1] = "";
  if( ::realpath( path.ToString( "/" ).c_str(), buffer ) )
    result = buffer;
#else
# error Don`t know how to canonicalize a path on the current target OS.
#endif
  if( IsDirectory( result ) )
    result = EnsureSeparator( result );
  return result;
}

std::string
FileUtils::ExecutableCanonicalPath(const std::string& inExecutable)
{
  std::vector<std::string> extensions;
#if _WIN32
  std::string ext = StringUtils::ToLower(FileUtils::ExtractExtension(inExecutable));
  extensions = StringUtils::Split<std::vector<std::string>>(";", ::getenv("PATHEXT"));
  if(extensions.empty())
  {
    extensions.push_back(".cmd");
    extensions.push_back(".exe");
    extensions.push_back(".bat");
  }
#endif
  extensions.push_back("");
  if(FilePath(inExecutable).Kind() != FilePath::absolute)
  {
    std::string pathvar = ::getenv("PATH");
#if _WIN32
    std::string sep = ";";
#else
    std::string sep = ":";
#endif
    auto paths = StringUtils::Split<std::vector<std::string>>(sep, pathvar);
    for(const auto& path : paths)
    {
      for(const auto& ext : extensions)
      {
        auto executable = CanonicalPath(path + "/" + inExecutable + ext);
        if(!executable.empty())
          return executable;
      }
    }
  }
  return CanonicalPath(inExecutable);
}

string
FileUtils::RealPath( const std::string& inPath )
{
  return NormalizedPath( CanonicalPath( inPath ) );
}

string
FileUtils::NormalizedPath( const std::string& inPath )
{
  if( inPath.empty() )
    return inPath;

  bool haveSeparator = FilePath::IsDirSeparator( inPath.back() );
  FilePath path( inPath );
  path.Simplify();
  NormalizePathElements( path );
  std::string s = path.ToString( "/" );
  if( haveSeparator )
    s = EnsureSeparator( s, "/" );
  return s;
}

std::string
FileUtils::ParentDirectory( const std::string& inPath )
{
  if( IsFile( inPath ) )
    return FileUtils::ExtractDirectory( inPath );
  return EnsureSeparator( inPath ) + "..";
}

std::string
FileUtils::ExtractDirectory( const std::string& inPath )
{
  if( inPath.empty() )
    return inPath;
  size_t pos = inPath.find_last_of( FilePath::SeparatorSet() );
  if( pos == string::npos )
    return "";
  return inPath.substr( 0, pos + 1 );
}

string
FileUtils::ExtractName( const std::string& inPath )
{
  size_t pos = inPath.find_last_of( FilePath::SeparatorSet() );
  if( pos == string::npos )
    return inPath;
  return inPath.substr( pos + 1 );
}

string
FileUtils::ExtractBase( const std::string& inPath )
{
  string file = ExtractFile( inPath );
  size_t pos = file.find_last_of( "." );
  if( pos == string::npos )
    return file;
  return file.substr( 0, pos );
}

string
FileUtils::ExtractExtension( const std::string& inPath )
{
  string file = ExtractFile( inPath );
  size_t pos = file.find_last_of( "." );
  if( pos == string::npos )
    return "";
  return file.substr( pos );
}

bool
FileUtils::IsFile( const std::string& inPath )
{
  struct stat s;
  return !::stat( AbsolutePath( inPath ).c_str(), &s ) && ( S_ISREG( s.st_mode ) );
}

bool
FileUtils::IsDirectory( const std::string& inPath )
{
  struct stat s;
  std::string abs = StripSeparator( AbsolutePath( inPath ) );
  bool exists = !::stat( abs.c_str(), &s )  || !::stat( EnsureSeparator( abs ).c_str(), &s );
  return exists && ( S_ISDIR( s.st_mode ) );
}

bool
FileUtils::IsSymbolicLink( const std::string& inPath )
{
  struct stat s;
  std::string abs = StripSeparator( AbsolutePath( inPath ) );
  return !::stat( abs.c_str(), &s ) && ( S_ISLNK( s.st_mode ) );
}

bool
FileUtils::Exists( const std::string& inPath )
{
  struct stat s;
  std::string abs = StripSeparator( AbsolutePath( inPath ) );
  return !::stat( abs.c_str(), &s ) || !::stat( EnsureSeparator( abs ).c_str(), &s );
}

bool
FileUtils::IsWritableFile( const string& inPath )
{
  std::string abs = AbsolutePath( inPath );
  return IsFile( abs ) && std::ofstream( abs.c_str(), ios::out | ios::app ).is_open();
}

bool
FileUtils::IsWritableDirectory( const string& inPath )
{
  if( !IsDirectory( inPath ) )
    return false;
  string testfile = WorkingDirectory() + "." + ExtractFile( ExecutablePath() ) + ".readtest";
  while( Exists( testfile ) )
  {
    static StaticObject<LCRandomGenerator> rg;
    testfile += rg().RandomCharacter();
  }
  if( !std::ofstream( testfile.c_str(), ios::out | ios::app ).is_open() )
    return false;
  RemoveFile( testfile );
  return true;
}

bool
FileUtils::IsWritableLocation( const string& inPath )
{
  if( IsFile( inPath ) )
    return IsWritableFile( inPath );
  string dir = ParentDirectory( inPath );
  while( !Exists( dir ) && dir != ParentDirectory( dir ) )
    dir = ParentDirectory( dir );
  if( IsFile( dir ) )
    return false;
  return IsWritableDirectory( dir );
}

Time
FileUtils::ModificationTime( const std::string& inPath )
{
  struct stat s;
  std::string abs = AbsolutePath( inPath );
  if( !::stat( abs.c_str(), &s ) )
    return Time::FromUnixTime( s.st_mtime );
  return Time::Never;
}

int64_t
FileUtils::Length( const std::string& inPath )
{
  struct stat s;
  std::string abs = AbsolutePath( inPath );
  if( !::stat( abs.c_str(), &s ) )
    return s.st_size;
  return -1;
}

bool
FileUtils::MakeDirectory( const string& inName, bool inForce )
{
  bool result = false;
  std::string abs = AbsolutePath( inName );
#ifdef _WIN32
  result = !::mkdir( abs.c_str() );
#else
  const int rwxr_xr_x = 0755;
  result = !::mkdir( abs.c_str(), rwxr_xr_x );
#endif
  if( inForce )
    result = result || IsDirectory( abs );
  if( !result && inForce )
  {
    auto parent = FilePath(ParentDirectory(abs)).Simplify().ToString();
    if(parent != abs)
      result = MakeDirectory(parent, true) && MakeDirectory( abs, false );
  }
  return result;
}

bool
FileUtils::RemoveDirectory( const string& inName, bool inForce )
{
  bool success = true;
  string name = CanonicalPath( inName );
  if( inForce )
  {
    List entries;
    success = ListDirectory( name, entries );
    for( size_t i = 0; success && i < entries.size(); ++i )
    {
      string path = name + FilePath::DirSeparator + entries[i];
      if( IsFile( path ) || IsSymbolicLink( path ) )
        success = RemoveFile( path );
      else if( IsDirectory( path ) )
        success = RemoveDirectory( path, true );
    }
  }
  if( success )
    success = !::rmdir( name.c_str() );
  return success;
}

bool
FileUtils::Rename( const string& inName, const string& inNewName )
{
  std::string abs1 = AbsolutePath( inName ), abs2 = AbsolutePath( inNewName );
  return !::rename( abs1.c_str(), abs2.c_str() );
}

bool
FileUtils::RemoveFile( const string& inName )
{
  return inName.empty() || !::unlink( AbsolutePath( inName ).c_str() );
}

bool
FileUtils::Copy( const string& inSource, const string& inDest )
{
  std::ifstream source( AbsolutePath( inSource ).c_str(), ios::binary );
  if( !source.is_open() )
    return false;
  std::ofstream dest( AbsolutePath( inDest ).c_str(), ios::binary );
  return !!( dest << source.rdbuf() );
}

bool
FileUtils::ListDirectory( const string& inPath, List& outList )
{
  bool success = true;
  outList.clear();
  DIR* dir = ::opendir( CanonicalPath( inPath ).c_str() );
  success = ( dir != NULL );
  if( success )
  {
    struct dirent* entry;
    while( NULL != ( entry = ::readdir( dir ) ) )
    {
      string name = entry->d_name;
      if( name != "." && name != ".." )
        outList.push_back( name );
    }
    ::closedir( dir );
  }
  return success;
}

std::string
FileUtils::MakeUniqueDirectory( const std::string& inName )
{
  if( !IsWritableDirectory( ParentDirectory( inName ) ) )
    return "";
  std::string name = inName;
  int i = 0;
  while( !FileUtils::MakeDirectory( name ) )
  {
    std::ostringstream oss;
    oss << "." << ++i;
    name = inName + oss.str();
  }
  return name;
}

FileUtils::TemporaryFile::TemporaryFile( const string& inExt )
: mpFile( new File )
{
  for( int trial = 0; trial < 20 && !is_open(); ++trial )
  {
    mpFile->name = GenerateName() + inExt;
    open( mpFile->name.c_str(), ios::in | ios::out | ios::binary | ios::trunc );
  }
  if( !is_open() )
    throw std_runtime_error << "Could not create temporary file: " << mpFile->name;
}

namespace
{
  struct TempPrefix : string
  { TempPrefix() : string( ExtractBase( ExecutablePath() ) + "~" ) {} };
}

string
FileUtils::TemporaryFile::GenerateName()
{
  static StaticObject<TempPrefix, string> prefix;
  static StaticObject<LCRandomGenerator> rg;
  return TemporaryDirectory() + prefix() + rg().RandomName( 6 );
}

bool
FileUtils::TemporaryFile::Open()
{
  open( mpFile->name.c_str(), ios::in | ios::out | ios::binary );
  return is_open();
}

string
FileUtils::SearchURL( const string& inSearchTerms )
{
#ifndef WEBSEARCH_DOMAIN
  return "";
#else
  string url = "http://" WEBSEARCH_DOMAIN "/search?q=",
         searchTerms = inSearchTerms;
  if( searchTerms.empty() )
    searchTerms = ApplicationTitle();
  string site;
#ifdef PROJECT_DOMAIN
  if( !searchTerms.empty() )
    site = PROJECT_DOMAIN;
#endif
#ifdef PROJECT_NAME
  if( searchTerms.empty() )
    searchTerms = PROJECT_NAME;
#endif
  if( searchTerms.empty() )
    return "";
  if( !site.empty() )
    searchTerms = "site%3a" + site + " " + searchTerms;

  istringstream iss( searchTerms );
  string term;
  iss >> ws >> term;
  url += term;
  while( iss >> ws >> term )
    url += "+" + term;
  return url;
#endif // WEBSEARCH_DOMAIN
}

