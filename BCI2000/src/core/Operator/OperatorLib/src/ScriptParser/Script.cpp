////////////////////////////////////////////////////////////////////////
// $Id: Script.cpp 4970 2015-08-21 16:46:07Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A class that represents a parsed script.
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
////////////////////////////////////////////////////////////////////////
#include "PCHIncludes.h"
#pragma hdrstop

#include "Script.h"
#include "CommandInterpreter.h"
#include "BCIException.h"

using namespace std;

Script::Script( const string& inScript, const string& inName )
: mLine( 0 ),
  mInputStream( inScript ),
  mName( inName ),
  mpRootNode( NULL )
{
}

Script::~Script()
{
  Cleanup();
}

Script&
Script::Compile()
{
  Cleanup();
  mLine = 1;
  static const string shebang = "#!";
  streampos start = mInputStream.tellg();
  string line;
  if( getline( mInputStream, line ) )
  {
    if( line.find( shebang ) == string::npos )
    {
      mInputStream.clear();
      mInputStream.seekg( start );
    }
    else
    {
      ++mLine;
      start = mInputStream.tellg();
      if( getline( mInputStream, line ) )
      {
        if( line.find( shebang ) == string::npos )
        {
          mInputStream.clear();
          mInputStream.seekg( start );
        }
        else
          ++mLine;
      }
    }
  }
  if( ScriptParser::yyparse( this ) )
    ReportError( mError );
  return *this;
}

Script&
Script::Execute( class CommandInterpreter& inInterpreter )
{
  try
  {
    if( !dynamic_cast<ScriptParser::Command*>( mpRootNode ) )
      inInterpreter.Execute( "" ); // make sure to clear interpreter's state
    if( mpRootNode )
      mpRootNode->Execute( inInterpreter );
  }
  catch( const bci::Exception& e )
  {
    ReportError( e.What() );
  }
  return *this;
}

void
Script::ReportError( const string& inError ) const
{
  if( mName.empty() )
    throw bciexception << inError;
  else
    throw bciexception << mName << ", line " << mLine << ": " << inError;
}

void
Script::Cleanup()
{
  for( size_t i = 0; i < mNodes.size(); ++i )
    delete mNodes[i];
}
