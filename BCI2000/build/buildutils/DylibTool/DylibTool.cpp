////////////////////////////////////////////////////////////////////
// $Id$
// Author: juergen.mellinger@uni-tuebingen.de
// Description: Build tool to automatically create glue code that
//   allows importing functions from a dynamic library without 
//   linking to an import library.
//   In the code that uses imported functions, you will need to
//   include the created header file rather than the original one.
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
#include <iostream>
#include <iomanip>
#include <map>

#include "StringUtils.h"
#include "FileUtils.h"
#include "FileMapping.h"
#include "ProcessUtils.h"
#include "Resources.h"
#if _WIN32
# include "PEHeader.h"
#elif __APPLE__
# include "MachOHeader.h"
#endif

static std::string Hash( const std::string&, size_t );
static std::string Varexpand( const std::string&, const std::map<std::string, std::string>& );
static bool PromptForExistingFile( const std::string& inPrompt, std::string& outFile );
static bool FindSymbolName( std::string& ioName, const std::string& data );
static bool IsCdeclName( const std::string& );

static bool sInteractive = false;
static const int cHashLength = 4;

int
DylibToolMain( int argc, char** argv )
{
  std::string toolname = FileUtils::ExtractBase( FileUtils::ExecutablePath() );
  bool help = false, listexports = false, cdecl_only = false;
  int createglue = -1;
  
  std::string libheader, dylib, options;
  for( int i = 1; i < argc; ++i )
  {
    std::string arg = argv[i];
    if( arg == "-h" || arg == "--help" || arg == "/?" )
      help = true;
    else if( arg == "--list-exports" )
      listexports = true;
    else if( arg == "--create-glue" )
      createglue = 1;
    else if( arg == "--cdecl-only" )
      cdecl_only = true;
    else if( dylib.empty() )
      dylib = arg;
    else if( libheader.empty() )
      libheader = arg;
    else
      std::cerr << "Ignoring unknown option: " << arg << std::endl;
  }
  if( createglue < 0 && listexports )
    createglue = 0;
  if( help )
  {
    std::cout << toolname << " creates glue code for custom runtime-loading of dynamic libraries.\n"
         << "Run without arguments to use it interactively.\n"
         << "In batch mode, usage is\n"
         << toolname << " <dylib file> [<header file>] <options>\n"
         << "\nOptions:\n"
         << "--cdecl-only: Assume the cdecl calling convention for symbols with unmangled names, and ignore all others.\n"
         << "--list-exports: List function names from <dylib file>'s export table, and quit.\n"
         << "\nExamples:\n\n"
         << toolname << " acmegadget.dll\n"
         << "will create glue code, identifying library imports by matching "
         << "symbol names exported from acmegadget.dll against function declarations in an "
         << "acmegadget.h header file.\n\n"
         << toolname << " acmegadget.dll ../acme/gadget.h\n"
         << "same as above, but will use specified header file.\n\n"
         << toolname << " acmegadget.dll --cdecl-only\n"
         << "will create glue code without using a header file, omitting any symbols with mangled names.\n\n";
    return 0;
  }

  sInteractive = dylib.empty();
  if( sInteractive )
    std::cout << toolname << ": creates glue code for custom runtime-loading of dynamic libraries"
              << std::endl;

  while( dylib.empty() )
    PromptForExistingFile( "Dynamic library: ", dylib );
  if( libheader.empty() && createglue )
  {
    libheader = FileUtils::ExtractDirectory( dylib ) + FileUtils::ExtractBase( dylib ) + ".h";
    if( !FileUtils::IsFile( libheader ) )
      libheader.clear();
  }
  if( libheader.empty() && !cdecl_only )
  {
    PromptForExistingFile( "Library header file: ", libheader );
    if( libheader.empty() )
    {
      cdecl_only = true;
      std::cout << "Without a header file, any non-cdecl functions will be ignored" << std::endl;
    }
  }

  FileMapping dylibdata( dylib );
#if _WIN32
  PEHeader pe( dylibdata.BaseAddress() );
  PEFile dylibfile( pe );
  PEFile::SymbolInfo info = { 0 };
#elif __APPLE__
  MachOHeader mh( dylibdata.BaseAddress() );
  MachOFile dylibfile( pe );
  MachOFile::SymbolInfo info = { 0 };
#endif
  const char* p = dylibfile.ExportLibraryName();
  if( !p )
  {
    std::cerr << "Could not open " << dylib << " as a dynamic library" << std::endl;
    return -1;
  }
  std::string libname = FileUtils::ExtractBase( p );

  std::vector<std::string> fsymbols, dsymbols;
  while( dylibfile.NextExport(info) )
  {
    if( cdecl_only && !IsCdeclName( info.name ) )
        continue;

    if( !info.data )
    {
        if( listexports )
            std::cout << "function " << info.name << ": 0x" << std::hex << info.rva << std::endl;
        fsymbols.push_back( info.name );
    }
    else
    {
        if( listexports )
            std::cout << "data " << info.name << ": 0x" << std::hex << info.rva << std::endl;
        dsymbols.push_back( info.name );
    }
  }
  if( !createglue )
    return 0;

  if( !cdecl_only )
  {
    std::string headercontent;
    if( !std::getline( std::ifstream( libheader ), headercontent, '\0' ) )
    {
      std::cerr << "Could not read contents of " << libheader << std::endl;
      return -1;
    }
    for( auto i = fsymbols.begin(); i != fsymbols.end(); )
    {
      if( !FindSymbolName( *i, headercontent ) )
      {
        std::cerr << "Warning: Function " << *i << " appears in library but not in header file, omitting" << std::endl;
        i = fsymbols.erase( i );
      }
      else
        ++i;
    }
    for( auto i = dsymbols.begin(); i != dsymbols.end(); )
    {
      if( !FindSymbolName( *i, headercontent ) )
      {
        std::cerr << "Warning: Data symbol " << *i << " appears in library but not in header file, omitting" << std::endl;
        i = dsymbols.erase( i );
      }
      else
        ++i;
    }
  }
  ProcessUtils::TemporaryProcessWD wd( FileUtils::ExtractDirectory( libheader ) );
  std::map<std::string, std::string> vars;
  vars["toolname"] = toolname;
  vars["headerfile"] = libheader;
  vars["headername"] = FileUtils::ExtractFile( libheader );
  vars["libfile"] = dylib;
  vars["libname"] = libname;
  for( auto& c : libname )
    if( !::isalnum( c ) )
      c = '_';
  vars["libname_n"] = libname;
  vars["libname_nu"] = StringUtils::ToUpper( libname );
  time_t now = std::time( nullptr );
  char* pTime = std::ctime( &now ); // has a trailing newline
  vars["datetime"] = std::string( pTime, ::strlen( pTime ) - 1 );

  struct Template { const bci::Resource* r; std::string file; };
  const struct Template templates[] =
  {
    { &Resources::HeaderTemplate, FileUtils::ExtractDirectory( libheader ) + libname + ".imports.h" },
    { &Resources::CppTemplate, FileUtils::ExtractDirectory( libheader ) + libname + ".imports.cpp" },
    { nullptr, "" },
    { &Resources::CdeclTemplate, FileUtils::ExtractDirectory( dylib ) + libname + ".imports.cpp" },
    { nullptr, "" },
  };
  
  for( const Template* tmpl = cdecl_only ? templates + 3 : templates; tmpl->r; ++tmpl )
  {
    std::string tmplData( tmpl->r->data, tmpl->r->length );
    size_t pos = 0, listCount = 0;
    const std::string listtagopen = "$@", listtagclose = "@$";
    while( ( pos = tmplData.find( listtagopen, pos ) ) != std::string::npos )
    {
      size_t end = tmplData.find( listtagclose, pos + listtagopen.length() );
      if( end != std::string::npos )
      {
        std::string listTmpl = tmplData.substr( pos + listtagopen.length(), end - pos - listtagopen.length() );
        std::ostringstream listName;
        listName << "__list" << listCount++ << "__";
        tmplData = tmplData.substr( 0, pos ) + "$" + listName.str() + "$" + tmplData.substr( end + listtagclose.length() );

        std::string& s = vars[listName.str()];
        s.clear();
        if( listTmpl.find( "$fname$" ) != std::string::npos )
          for( const auto& fsymbol : fsymbols )
          {
            vars["fname"] = fsymbol;
            vars["hash"] = Hash( fsymbol, cHashLength );
            s += Varexpand( listTmpl, vars );
          }
        else if( listTmpl.find( "$dname$" ) != std::string::npos )
          for( const auto& dsymbol : dsymbols )
          {
            vars["dname"] = dsymbol;
            vars["hash"] = Hash( dsymbol, cHashLength );
            s += Varexpand( listTmpl, vars );
          }
      }
      pos += listtagopen.length();
    }
    vars.erase( "fname" );
    vars.erase( "dname" );
    vars.erase( "hash" );
    std::string expanded = Varexpand( tmplData, vars );
    ;
    if( !std::ofstream( tmpl->file ).write( expanded.data(), expanded.size() ) )
    {
      std::cerr << "Error writing file " << tmpl->file << std::endl;
      return -1;
    }
    std::cout << "Wrote " << tmpl->file << std::endl;
  }
  return 0;
}

int main( int argc, char** argv )
{
  int result = DylibToolMain( argc, argv );
  if( result == 0 )
    std::cout << "\nDone";
  else
    std::cout << "\nErrors occurred";
  if( sInteractive )
  {
    std::cout << ", press enter to quit" << std::flush;
    std::cin.get();
  }
  else
    std::cout << "." << std::endl;
  return result;
}

std::string
Varexpand( const std::string& s, const std::map<std::string, std::string>& vars )
{
  std::string r = s;
  size_t pos = 0;
  for( const auto& var : vars )
    while( ( pos = r.find( "$" + var.first + "$" ) ) != std::string::npos )
      r = r.substr( 0, pos ) + var.second + r.substr( pos + var.first.length() + 2 );
  return r;
}

std::string Hash( const std::string& data, size_t hashLength )
{
  std::vector<char> val( hashLength, 0 );
  for( int i = 0; i < data.length(); ++i )
    val[i % val.size()] ^= data[i];
  std::ostringstream oss;
  for( int i = 0; i < val.size(); ++i )
    oss << std::setfill( '0' ) << std::setw( 2 ) << std::hex << int( val[i] );
  return oss.str();
}

bool PromptForExistingFile( const std::string& inPrompt, std::string& outFile )
{
#if _WIN32
  static bool displayed = false;
  if( !displayed )
  {
    displayed = true;
    std::cout << "\nNOTES:\n"
         << "To abort, press <ctrl>-c.\n"
         << "Instead of typing a file name, "
            "you may drag a file's icon from an explorer shell window, "
            "and drop it into this console window.\n\n";
  }
#endif // _WIN32
  outFile.clear();
  std::cout << inPrompt << std::flush;
  std::string file;
  std::getline( std::cin, outFile );
  outFile = StringUtils::Trim( outFile );
  if( outFile.size() >= 2 && outFile.front() == '"' )
    outFile = outFile.substr( 1, outFile.length() - 2 );
  if( FileUtils::IsFile( outFile ) )
    return true;
  if( !outFile.empty() )
    std::cout << "\"" << outFile << "\" is not an existing file.\n";
  outFile.clear();
  return false;
}

bool
FindSymbolName( std::string& ioName, const std::string& header )
{
  if( ioName.empty() )
    return false;
  if( ioName.find( '@' ) != std::string::npos )
  {
    ioName = ioName.substr( 0, ioName.rfind( '@' ) );
    if( ioName.find( '@' ) != std::string::npos )
      ioName = ioName.substr( ioName.find( '@' ) + 1 );
  }
  size_t pos = header.find( ioName );
  if(pos != std::string::npos)
    return true;
  if(ioName.front() == '_')
    ioName = ioName.substr(1);
  pos = header.find(ioName);
  return pos != std::string::npos;
}

bool
IsCdeclName( const std::string& name )
{
  if( name.empty() )
    return false;
  if( !::isalpha(name.front()) && name.front() != '_' )
    return false;
  for( auto c : name )
    if( !::isalnum(c) && c != '_' )
      return false;
  return true;
}
