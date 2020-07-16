////////////////////////////////////////////////////////////////////////////////
// $Id: FileSystemTypes.cpp 5732 2018-03-21 20:56:07Z mellinger $
// Authors: juergen.mellinger@uni-tuebingen.de
// Description: CommandInterpreter types for files and directories.
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
#include "PCHIncludes.h"
#pragma hdrstop

#include "FileSystemTypes.h"
#include "CommandInterpreter.h"
#include "WildcardMatch.h"
#include "FileUtils.h"
#include "ProcessUtils.h"
#include "BCIException.h"

using namespace std;
using namespace Interpreter;

// DirectoryType
DirectoryType DirectoryType::sInstance;
const ObjectType::MethodEntry DirectoryType::sMethodTable[] =
{
  METHOD( Extract ), { "Get", &Extract },
  METHOD( Parent ),
  METHOD( Exists ), { "Is", &Exists },
  METHOD( Current ),
  METHOD( Change ),
  METHOD( Make ), { "Create", &Make },
  METHOD( List ),
  METHOD( Rename ),
  METHOD( Remove ),
  METHOD( ForceRemove ),
  END
};

bool
DirectoryType::Extract( CommandInterpreter& inInterpreter )
{
  inInterpreter.Out() << FileUtils::ExtractDirectoryS( inInterpreter.GetToken() );
  return true;
}

bool
DirectoryType::Parent( CommandInterpreter& inInterpreter )
{
  inInterpreter.Out() << FileUtils::ParentDirectoryS( inInterpreter.GetToken() );
  return true;
}

bool
DirectoryType::Exists( CommandInterpreter& inInterpreter )
{
  bool result = FileUtils::IsDirectory( inInterpreter.GetToken() );
  inInterpreter.Out() << ( result ? "true" : "false" );
  return true;
}

bool
DirectoryType::Current( CommandInterpreter& inInterpreter )
{
  inInterpreter.Out() << FileUtils::WorkingDirectoryS();
  return true;
}

bool
DirectoryType::Change( CommandInterpreter& inInterpreter )
{
  string dir = inInterpreter.GetToken();
  if( !FileUtils::ChangeDirectory( dir ) )
    inInterpreter.Out() << "Could not change current directory to \"" << dir << "\"";
  return true;
}

bool
DirectoryType::Make( CommandInterpreter& inInterpreter )
{
  string dir = inInterpreter.GetToken();
  if( !FileUtils::MakeDirectory( dir ) )
    inInterpreter.Out() << "Could not make directory \"" << dir << "\"";
  return true;
}

bool
DirectoryType::Rename( CommandInterpreter& inInterpreter )
{
  string dir = inInterpreter.GetToken(),
         newName = inInterpreter.GetToken();
  if( !FileUtils::IsDirectory( dir ) )
    throw bciexception << "There is no directory named \"" << dir << "\"";
  if( !FileUtils::Rename( dir, newName ) )
    throw bciexception << "Could not rename \"" << dir << "\" to \"" << newName << "\"";
  return true;
}

bool
DirectoryType::Remove( CommandInterpreter& inInterpreter )
{
  string dir = inInterpreter.GetToken();
  if( !FileUtils::IsDirectory( dir ) )
    throw bciexception << "There is no directory named \"" << dir << "\"";
  if( !FileUtils::RemoveDirectory( dir, false ) )
    throw bciexception << "Could not remove directory \"" << dir << "\"";
  return true;
}

bool
DirectoryType::ForceRemove( CommandInterpreter& inInterpreter )
{
  string dir = inInterpreter.GetToken();
  if( !FileUtils::IsDirectory( dir ) )
    throw bciexception << "There is no directory named \"" << dir << "\"";
  if( !FileUtils::RemoveDirectory( dir, true ) )
    throw bciexception << "Could not remove directory \"" << dir << "\"";
  return true;
}

bool
DirectoryType::List( CommandInterpreter& inInterpreter )
{
  string args;
#if _WIN32
  args = "/n ";
#else
  args = "-l ";
#endif
  string remainder = inInterpreter.GetRemainingTokens();
  args += remainder;
  inInterpreter.Out() << ListDirectory( args );
  return true;
}

string
DirectoryType::ListDirectory( const string& inArgs )
{
  string command;
#if _WIN32
  command = "dir " + inArgs;
#else
  command = "ls " + inArgs;
#endif
  int exitCode = 0;
  stringstream oss;
  if( !ProcessUtils::SystemExecuteSynchronously( command, oss, exitCode ) )
    throw bciexception << "Could not get directory listing";
#if _WIN32
  string listing;
  char c;
  while( oss.get( c ) )
    if( c != '\r' )
      listing += c;
  return listing;
#else // _WIN32
  return oss.str();
#endif // _WIN32
}

bool
DirectoryType::ListSelection( CommandInterpreter& inInterpreter, const string& inDir, const string& inWildcard, bool (*inSelector)( const string& ) )
{
  string dir = inDir.empty() ? "." : inDir,
         wildcard = inWildcard.empty() ? "*" : inWildcard;
  dir = FileUtils::NormalizedPath( dir );
  FileUtils::List list;
  if( !FileUtils::ListDirectory( dir, list ) )
    throw bciexception << "Could not list directory \"" << dir << "\"";
  for( size_t i = 0; i < list.size(); ++i )
    if( WildcardMatch( wildcard, list[i], false ) && inSelector( FileUtils::EnsureSeparator( dir ) + list[i] ) )
      inInterpreter.Out() << list[i] << '\n';
  return true;
}

// DirectoriesType
DirectoriesType DirectoriesType::sInstance;
const ObjectType::MethodEntry DirectoriesType::sMethodTable[] =
{
  METHOD( List ),
  END
};

bool
DirectoriesType::List( CommandInterpreter& inInterpreter )
{
  string dir = inInterpreter.GetOptionalToken(),
         wildcard = inInterpreter.GetOptionalToken();
  return DirectoryType::ListSelection( inInterpreter, dir, wildcard, &FileUtils::IsDirectory );
}

// FileType
FileType FileType::sInstance;
const ObjectType::MethodEntry FileType::sMethodTable[] =
{
  METHOD( Extract ), { "Get", &Extract },
  METHOD( Exists ), { "Is", &Exists },
  METHOD( List ),
  METHOD( Rename ), { "Move", &Rename },
  METHOD( Remove ), { "Delete", &Remove },
  END
};

bool
FileType::Extract( CommandInterpreter& inInterpreter )
{
  string file = inInterpreter.GetToken();
  if( !::stricmp( file.c_str(), "Base" ) )
    inInterpreter.Out() << FileUtils::ExtractBase( inInterpreter.GetToken() );
  else
    inInterpreter.Out() << FileUtils::ExtractFile( file );
  return true;
}

bool
FileType::Exists( CommandInterpreter& inInterpreter )
{
  bool result = FileUtils::IsFile( inInterpreter.GetToken() );
  inInterpreter.Out() << ( result ? "true" : "false" );
  return true;
}

bool
FileType::List( CommandInterpreter& inInterpreter )
{
  return DirectoryType::ListSelection( inInterpreter, ".", inInterpreter.GetToken(), &FileUtils::IsFile );
}

bool
FileType::Remove( CommandInterpreter& inInterpreter )
{
  string file = inInterpreter.GetToken();
  if( !FileUtils::IsFile( file ) )
    throw bciexception << "There is no file named \"" << file << "\"";
  if( !FileUtils::RemoveFile( file ) )
    throw bciexception << "Could not remove file \"" << file << "\"";
  return true;
}

bool
FileType::Rename( CommandInterpreter& inInterpreter )
{
  string file = inInterpreter.GetToken(),
         newName = inInterpreter.GetToken();
  if( !FileUtils::IsFile( file ) )
    throw bciexception << "There is no file named \"" << file << "\"";
  if( !FileUtils::Rename( file, newName ) )
    throw bciexception << "Could not rename file \"" << file << "\" to \"" << newName << "\"";
  return true;
}

// FilesType
FilesType FilesType::sInstance;
const ObjectType::MethodEntry FilesType::sMethodTable[] =
{
  METHOD( List ),
  END
};

bool
FilesType::List( CommandInterpreter& inInterpreter )
{
  string dir = inInterpreter.GetOptionalToken(),
         wildcard = inInterpreter.GetOptionalToken();
  return DirectoryType::ListSelection( inInterpreter, dir, wildcard, &FileUtils::IsFile );
}

// PathType
PathType PathType::sInstance;
const ObjectType::MethodEntry PathType::sMethodTable[] =
{
  METHOD( Canonicalize ), { "Canonical", &Canonicalize },
  METHOD( Normalize ), { "Normalized", &Normalize },
  METHOD( Real ),
  METHOD( Exists ), { "Is", &Exists },
  END
};

bool
PathType::Canonicalize( CommandInterpreter& inInterpreter )
{
  inInterpreter.Out() << FileUtils::CanonicalPath( inInterpreter.GetToken() );
  return true;
}

bool
PathType::Normalize( CommandInterpreter& inInterpreter )
{
  inInterpreter.Out() << FileUtils::NormalizedPath( inInterpreter.GetToken() );
  return true;
}

bool
PathType::Real( CommandInterpreter& inInterpreter )
{
  inInterpreter.Out() << FileUtils::RealPath( inInterpreter.GetToken() );
  return true;
}

bool
PathType::Exists( CommandInterpreter& inInterpreter )
{
  string path = inInterpreter.GetToken();
  bool result = FileUtils::IsFile( path ) || FileUtils::IsDirectory( path );
  inInterpreter.Out() << ( result ? "true" : "false" );
  return true;
}
