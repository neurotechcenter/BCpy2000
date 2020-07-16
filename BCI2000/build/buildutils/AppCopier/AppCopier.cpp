//////////////////////////////////////////////////////////////////////////////
// $Id$
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A tool that packages a main executable together
//   with its dependencies into a target directory.
//
// $BEGIN_BCI2000_LICENSE$
//
// This file is part of BCI2000, a platform for real-time bio-signal research.
// [ Copyright (C) 2000-2018: BCI2000 team and many external contributors ]
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
#include "SysError.h"
#include "FileUtils.h"
#include "ExceptionCatcher.h"
#include <iostream>
#include <vector>
#include <string>

using namespace std;

int main( int argc, char** argv )
{
  int help = 0, verbosity = 0;
  const struct { std::string name; int* pValue; } options[] =
  {
    { "--help", &help },
    { "--verbosity=", &verbosity },
  };
  std::string targetPath, exePath;
  std::vector<std::pair<std::string, std::string>> files;
  for( int i = 1; i < argc; ++i )
  {
    if( *argv[i] == '-' )
    {
      bool found = false;
      for( size_t j = 0; !found && j < sizeof(options)/sizeof(*options); ++j )
      {
        if( string( argv[i] ).find( options[j].name ) == 0 )
        {
          found = true;
          if( options[j].name.back() == '=' )
            *options[j].pValue = ::atoi( argv[i] + options[j].name.length() );
          else
            *options[j].pValue = 1;
        }
      }
      if( !found )
      {
        cerr << "Unknown option: " << argv[i] << endl;
        return -1;
      }
    }
    else if( targetPath.empty() )
      targetPath = argv[i];
    else if( exePath.empty() )
      exePath = argv[i];
    else
    {
      std::string file = argv[i];
      size_t pos = file.find('=');
      if(pos == std::string::npos)
        files.push_back(std::make_pair(file, FileUtils::ExtractFile(file)));
      else
        files.push_back(std::make_pair(file.substr(0, pos), file.substr(pos + 1)));
    }
  }
  if( help || exePath.empty() )
  {
    cerr << "Usage:\nAppCopier [--verbosity={0..10}] <target executable> <main executable> [<file 1>[=<new name file 1>] ... <file n>[=<new name file n>]]" << endl;
    return help ? 0 : -1;
  }
  if (verbosity < 1)
    std::clog.rdbuf(nullptr);

  const string ext = ".exe";
  if( FileUtils::ExtractExtension( targetPath ) != ext )
    targetPath += ext;
  if( FileUtils::ExtractExtension( exePath ) != ext )
    exePath += ext;

  const string targetDir = FileUtils::ExtractDirectory( targetPath );
  FileUtils::MakeDirectory( targetDir, true );

  files.insert( files.begin(), std::make_pair( exePath, FileUtils::ExtractFile( targetPath ) ) );
  for( const auto& file : files )
  {
    std::string name = FileUtils::ExtractFile( file.first );
    if( !FileUtils::Exists( file.first ) )
    {
      cerr << file.first << " does not exist, aborting" << std::endl;
      exit( -1 );
    }
    std::string target = targetDir + file.second;
    std::string dir = FileUtils::ExtractDirectory(target);
    if (!FileUtils::MakeDirectory(dir, true))
    {
      cerr << "Could not create directory: " << dir << ", aborting" << std::endl;
      exit(-1);
    }
    clog << "Copying " << name << " ..." << std::endl;
    if (!FileUtils::Copy(file.first, target))
    {
      cerr << "Could not copy " << file.first << " to " << target << std::endl;
      exit(-1);
    }
  }
  return 0;
}

