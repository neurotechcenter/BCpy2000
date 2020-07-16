////////////////////////////////////////////////////////////////////
// $Id: bci_tool.cpp 5414 2016-07-01 19:45:56Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A small framework for platform independent command
//   line tools.
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
#include "bci_tool.h"
#include "ExceptionCatcher.h"
#include "Files.h"
#include "VersionInfo.h"
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <fcntl.h>

// Do we need to switch stdin/stdout into text mode?
#if defined( _WIN32 ) || defined( CYGWIN )
# define HAVE_BIN_MODE 1
#endif

#if HAVE_BIN_MODE
# include <io.h>
#endif

using namespace std;

int main( int argc, char** argv )
{
  if( ToolInfo[name] == "" )
  {
    size_t nameBegin = string( argv[0] ).find_last_of( "/\\" );
    ToolInfo[name] = ( nameBegin == string::npos ? argv[0] : argv[0] + nameBegin + 1 );
    size_t extensionBegin = ToolInfo[name].rfind( "." );
    if( extensionBegin != string::npos )
      ToolInfo[name].erase( extensionBegin );
  }

  FunctionCall< ToolResult() > callInit( ToolInit );
  bool finished = ExceptionCatcher()
                 .SetMessage( "Aborting " + ToolInfo[name] )
                 .Run( callInit );
  if( !finished )
    return genericError;
  ToolResult result = callInit.Result();
  if( result != noError )
  {
    cerr << ToolInfo[name] << ": Initialization error" << endl;
    return result;
  }
  OptionSet toolOptions;
  for( int i = 1; i < argc; ++i )
    toolOptions.push_back( argv[i] );

  struct options
  {
    bool execute;
    bool help;
    bool version;
    bool bcitest;
    string inputFile;
    string outputFile;
    int bufferSize;
  } options =
  {
    true,
    false,
    false,
    false,
    "",
    "",
    0,
  };

  if( toolOptions.findopt( "-h|-H|--help|-?" ) )
  {
    options.help = true;
    options.execute = false;
  }
  if( toolOptions.findopt( "-v|-V|--version" ) )
  {
    options.version = true;
    options.execute = false;
  }
  options.inputFile = toolOptions.getopt( "-i|-I|--input", "" );
  options.outputFile = toolOptions.getopt( "-o|-O|--output", "" );
  string buffer = toolOptions.getopt( "-b|-B|--buffer", "4096" );
  if( buffer.empty() )
  {
    options.execute = false;
    options.help = true;
  }
  options.bufferSize = ::atoi( buffer.c_str() );
  if( options.bufferSize < 0 )
    options.bufferSize *= -1;

  File inputFile;
  if( options.inputFile.empty() || options.inputFile == "-" )
  {
    inputFile.AttachToFd( ::fileno( stdin ) );
  }
  else if( !inputFile.Open( options.inputFile, ios_base::in ) )
  {
    cerr << "Could not open " << options.inputFile << " for input" << endl;
    result = fileIOError;
  }

  File outputFile;
  if( options.outputFile.empty() || options.outputFile == "-" )
  {
    outputFile.AttachToFd( ::fileno( stdout ) );
  }
  else if( !outputFile.Open( options.outputFile, ios_base::out ) )
  {
    cerr << "Could not open " << options.outputFile << " for output" << endl;
    result = fileIOError;
  }

#if HAVE_BIN_MODE
  if( result == noError )
  {
    ::setmode( inputFile.Fd(), O_BINARY );
    if( ToolInfo[outputType].empty() || ToolInfo[outputType][0] != 't' )
      ::setmode( outputFile.Fd(), O_BINARY );
    else
      ::setmode( outputFile.Fd(), O_TEXT );
  }
#endif

  if( result == noError && options.execute )
  {
    BufferedIO in( options.bufferSize, 0 ), out( 0, options.bufferSize );
    in.SetInput( &inputFile.Input() );
    out.SetOutput( &outputFile.Output() );
    FunctionCall< ToolResult( Streambuf&, Streambuf&, OptionSet& ) >
      callMain( ToolMain, in, out, toolOptions );
    bool finished = ExceptionCatcher()
                   .SetMessage( "Aborting " + ToolInfo[ name ] )
                   .Run( callMain );
    if( !finished )
      result = genericError;
    else
      result = callMain.Result();
  }

  options.help |= ( result == illegalOption );
  if( options.help )
  {
    ostream& out = ( result == noError ? cout : cerr );
    out << "Usage: " << ToolInfo[ name ] << " [OPTION]\n"
        << "Options are:\n"
        << "\t-h,       --help                Display this help\n"
        << "\t-v,       --version             Output version information\n"
        << "\t-i<file>, --input=<file>        Get input from <file>\n"
        << "\t-o<file>, --output=<file>       Write output to <file>\n"
        << "\t-b<size>, --buffer=<size>       Set IO buffer to <size>\n"
        << "\tWhen a minus character is given as a file name, it refers"
        << "\tto standard input/output";
    for( int i = firstOption; ToolInfo[ i ] != ""; ++i )
      out << '\t' << ToolInfo[ i ] << '\n';
    out << '\n' << ToolInfo[ description ] << '\n';
    out.flush();
  }
  if( options.version )
  {
    cout << ToolInfo[name] << " ";
    VersionInfo::Current.InsertInto( cout, true );
  }

  if( !cout )
  {
    cerr << "Error writing to output" << endl;
    result = genericError;
  }
  return result;
}

string OptionSet::getopt( const string& optionNames, const string& optionDefault )
{
  string result = optionDefault;
  istringstream is( optionNames );
  string token;
  while( getline( is, token, synonymSeparator ) )
  {
    iterator i = begin();
    while( i != end() )
      if( i->find( token ) == 0 )
      {
        string optionValue = i->substr( token.length() );
        i = erase( i );
        if( !optionValue.empty() && optionValue[0] == '=' )
          result = optionValue.substr( 1 );
        else
          result = optionValue;
      }
      else
        ++i;
  }
  return result;
}

bool OptionSet::findopt( const string& optionNames )
{
  bool result = false;
  istringstream is( optionNames );
  string token;
  while( getline( is, token, synonymSeparator ) )
  {
    iterator i = begin();
    while( i != end() )
      if( i->find( token ) == 0 )
      {
        i = erase( i );
        result = true;
      }
      else
        ++i;
  }
  return result;
}

