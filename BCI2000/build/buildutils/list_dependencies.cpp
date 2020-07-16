////////////////////////////////////////////////////////////////////
// $Id$
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A tool that lists static DLL dependencies of an
//   executable.
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
#include <Psapi.h>
#include <iostream>
#include "PEHeader.h"
#include "FileMapping.h"
#include "FileUtils.h"
#include "ProcessUtils.h"
#include "StringUtils.h"
#include "EnvVariable.h"
#include "DylibImports.h"
#include "buildenv.h"

bool FindFiles( const std::string&, const std::string&, FileUtils::List& );
bool FindRedistributables( const std::vector<std::string>&, std::vector<std::string>&, int );
bool ListLoadedModules( std::vector<std::string>& );
bool ListDependencies( const std::string&, std::vector<std::string>& );
bool ListFailedDependencies( const std::string&, std::vector<std::string>& );

using namespace std;

int main( int argc, char** argv )
{
  if( argc < 2 )
  {
    const char* usage = "Usage:\nListDependencies [--redist] [--add-redist-dir <dir>] [--import-table] <executable file>";
    cout << usage << endl;
    return -1;
  }
  string executable;
  bool redist = false, importtable = false;
  vector<string> redistdirs;
  for( int i = 1; i < argc; ++i )
  {
    if( !strcmp( argv[i], "--redist" ) )
      redist = true;
    else if( !strcmp( argv[i], "--add-redist-dir" ) && i + 1 < argc )
      redistdirs.push_back( argv[++i] );
    else if( !strcmp( argv[i], "--import-table" ) )
      importtable = true;
    else if( *argv[i] == '-' )
    {
      cerr << "Unknown option: " << argv[i] << endl;
      return -1;
    }
    else
      executable = FileUtils::AbsolutePath( argv[i] );
  }
  if( !FileUtils::IsFile( executable ) )
  {
    cerr << "File does not exist: " << executable << endl;
    return -1;
  }
  string path = FileUtils::ExtractDirectory( executable );

  vector<string> dependencies;
  if( !ListDependencies( executable, dependencies ) )
  {
    cerr << "Cout not read import table" << endl;
    return -1;
  }
  if( !importtable )
  {
    FileUtils::TemporaryFile tmp( ".dll" );
    string tmpname = tmp.Name();
    if( !tmp.IsOpen() )
    {
      cerr << "Cout not create temporary file: " << tmpname << endl;
      return -1;
    }
    tmp.Close();
    tmpname = FileUtils::CanonicalPath( tmpname );
    FileMapping fm( executable );
    if( fm.SaveAs( tmpname ) )
    {
      PEHeader dll = fm.BaseAddress();
      dll.AddressOfEntryPoint() = 0;
      dll.CoffFileHeader()->Characteristics |= IMAGE_FILE_DLL;
      fm.Close();
    }
    if( !fm )
    {
      cerr << "Could not save dll-ized copy of executable: " << SysError().Message() << endl;
      return -1;
    }
    vector<string> modules1;
    if( !ListLoadedModules( modules1 ) )
    {
      cerr << "Could not get list of modules: " << SysError().Message() << endl;
      return -1;
    }
    if( !::SetDllDirectory( path.c_str() ) )
    {
      cerr << "Could not set DLL directory: " << SysError().Message() << endl;
      return -1;
    }
    EnvVariable pathvar( "PATH" );
    string pathstr;
    for( auto& s : redistdirs )
    {
      s = FileUtils::CanonicalPath( FileUtils::EnsureSeparator( s ) );
      if( s.empty() )
      {
        cerr << "Cannot find redist dir: " << s << endl;
        return -1;
      }
      pathstr += ";" + s;
    }
    pathvar.Set( pathstr.empty() ? "" : pathstr.substr( 1 ) );
    ::SetErrorMode( ::SetErrorMode( 0 ) | SEM_FAILCRITICALERRORS );
    HMODULE h = ::LoadLibraryA( tmpname.c_str() );
    if( !h )
    {
      SysError err;
      if( err.Code() == ERROR_MOD_NOT_FOUND )
      {
        ProcessUtils::TemporaryProcessWD wd( path );
        vector<string> dependencies;
        if( ListFailedDependencies( tmpname, dependencies ) && !dependencies.empty() )
        {
          cerr << "Cannot list dependencies, executable depends on modules that failed to load:" << endl;
          for( const auto& s : dependencies )
            cerr << " " << s << endl;
          return -1;
        }
      }
      cerr << "Could not load dll-ized copy of executable: " << err.Message() << endl;
      return -1;
    }
    vector<string> modules2;
    if( !ListLoadedModules( modules2 ) )
    {
      cerr << "Could not get list of modules: " << SysError().Message() << endl;
      ::FreeLibrary( h );
      return -1;
    }
    ::FreeLibrary( h );
    for( auto& import : dependencies )
      import = StringUtils::ToUpper( import );
    vector<string> modules;
    for( auto& module : modules1 )
    {
      string file = StringUtils::ToUpper( FileUtils::ExtractFile( module ) );
      if( std::find( dependencies.begin(), dependencies.end(), file ) == dependencies.end() )
        modules.push_back( FileUtils::CanonicalPath( module ) );
    }
    dependencies.clear();
    modules1 = modules;
    for( auto& module : modules2 )
    {
      module = FileUtils::CanonicalPath( module );
      if( module != tmpname && std::find( modules1.begin(), modules1.end(), module ) == modules1.end() )
        dependencies.push_back( module );
    }
  }
  if( redist )
  {
    int machine = PEHeader( FileMapping( executable ).BaseAddress() ).CoffFileHeader()->Machine;
    vector<string> redistributables;
    if( !FindRedistributables( dependencies, redistributables, machine ) )
      return -1;
    for( const auto& module : dependencies )
    {
      string path = FileUtils::CanonicalPath( FileUtils::ParentDirectory( module ) );
      if( std::find( redistdirs.begin(), redistdirs.end(), path ) != redistdirs.end() )
        if( std::find( redistributables.begin(), redistributables.end(), module ) == redistributables.end() )
          redistributables.push_back( module );
    }
    dependencies = redistributables;
  }
  for( const auto& module : dependencies )
    cout << module << "\n";
  cout.flush();
  return 0;
}

bool ListLoadedModules( std::vector<std::string>& outModules )
{
  DWORD needed = 0;
  if( !::EnumProcessModules( ::GetCurrentProcess(), 0, 0, &needed ) )
    return false;
  int count = needed / sizeof(HMODULE);
  HMODULE* pModules = new HMODULE[count];
  if( !::EnumProcessModules( ::GetCurrentProcess(), pModules, needed, &needed ) )
    return false;
  for( int i = 0; i < count; ++i )
  {
    char path[MAX_PATH + 1] = { 0 };
    if( ::GetModuleFileNameA( pModules[i], path, sizeof(path) ) )
      outModules.push_back( path );
  }
  delete[] pModules;
  return true;
}

bool FindRedistributables( const vector<string>& modules, vector<string>& redist, int inMachine )
{
  if( ::strcmp( CMAKE_C_COMPILER_ID, "MSVC" ) )
  {
    cerr << "Unsupported compiler: " << CMAKE_C_COMPILER_ID << endl;
    return false;
  }
  string rpath, cpath = FileUtils::ExtractDirectory( CMAKE_C_COMPILER );
  bool abort = false;
  do
  {
    string s = FileUtils::CanonicalPath( FileUtils::ParentDirectory( cpath ) );
    abort = ( cpath == s );
    cpath = s;
    rpath = cpath + "redist";
  } while( !abort && !FileUtils::IsDirectory( rpath ) );
  if( abort )
  {
    cerr << "Could not find redistributable directory for compiler " << CMAKE_C_COMPILER << endl;
    return false;
  }
  rpath = FileUtils::EnsureSeparator( rpath );
  for( const auto& module : modules )
  { // NOTE: For reliable comparison, we used canonicalized paths, but to identify
    // a library by its import name, we need the normalized file name.
    string moduleName = FileUtils::ExtractFile( FileUtils::NormalizedPath(module) );
    FileUtils::List files;
    FindFiles( rpath, moduleName, files );
    string redistFile;
    for( const auto& file : files )
    {
      FileMapping fm( file );
      PEHeader pe( fm.BaseAddress() );
      if( pe.CoffFileHeader() && pe.CoffFileHeader()->Machine == inMachine )
        if(redistFile.empty() || redistFile.length() > file.length())
          redistFile = file;
    }
    if(!redistFile.empty())
      redist.push_back(redistFile);
  }
  return true;
}

bool FindFiles( const std::string& inDir, const std::string& inName, FileUtils::List& outFiles )
{
  outFiles.clear();
  if( !FileUtils::IsDirectory( inDir ) )
    return false;
  struct
  {
    void Recurse( const string& inDir, const string& inName, FileUtils::List& outFiles )
    {
      string path = inDir + inName;
      if( FileUtils::IsFile(path) )
        outFiles.push_back( path );
      else
      {
        FileUtils::List dir;
        FileUtils::ListDirectory( inDir, dir );
        for( const auto& entry : dir )
        {
          path = inDir + entry;
          if( FileUtils::IsDirectory( path ) )
            Recurse( FileUtils::EnsureSeparator(path), inName, outFiles );
        }
      }
    }
  } r;
  r.Recurse( inDir, inName, outFiles );
  return true;
}

bool
ListDependencies( const std::string& inName, std::vector<std::string>& outDependencies )
{
  bool result = false;
  outDependencies.clear();
  FileMapping file( inName );
  PEHeader pe( file.BaseAddress() );
  if( !pe.CoffFileHeader() )
    return false;
  const char* dep = PEFile( pe ).ImportLibraryName( 0 );
  for( int i = 0; dep; dep = PEFile( pe ).ImportLibraryName( ++i ) )
    outDependencies.push_back( dep );
  return pe.Bits();
}

bool ListFailedDependencies( const string& inModule, vector<string>& outDependencies )
{
  struct 
  {
    bool Recurse( const string& inModule, vector<string>& outDependencies )
    {
      HMODULE h = ::LoadLibraryA( inModule.c_str() );
      if( !h )
      {
        if( find( outDependencies.begin(), outDependencies.end(), inModule ) == outDependencies.end() )
        {
          outDependencies.push_back( inModule );
          vector<string> dep;
          ListDependencies( inModule, dep );
          for( const auto& s : dep )
            if( !Recurse( s, outDependencies ) )
              return false;
        }
      }
      else
        ::FreeLibrary( h );
      return true;
    }
  } r;
  outDependencies.clear();
  vector<string> dep;
  ListDependencies( inModule, dep );
  for( const auto& s : dep )
    if( !r.Recurse( s, outDependencies ) )
      return false;
  return true;
}

