//////////////////////////////////////////////////////////////////////
// $Id: UnitTest.cpp 4651 2013-11-22 16:48:25Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A runnable descendant that performs a test when run,
//   and reports test failure.
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
#include "UnitTest.h"

#include <iostream>
#include <cctype>
#include <cstdlib>

using namespace std;
using namespace Tiny;

static bool sSelfTest = false;
vector<Test*> Test::sTests;

static void
DefaultHandler( const string& s )
{
  cerr << s << endl;
}

static string
MakeDesc( const string& inName )
{
  string desc = inName,
         lcName = inName;
  for( size_t i = 0; i < lcName.length(); ++i )
    lcName[i] = ::tolower( lcName[i] );
  if( lcName.find( "test" ) == string::npos )
    desc += " test";
  return desc;
}

Test::Test( const string& inName )
: mDesc( MakeDesc( inName ) )
{
  sTests.push_back( this );
}

ostream&
Test::FailStream_( const char* inFile, int inLine )
{
  return mFailStream << mDesc << " failed in " << inFile << ", line " << inLine << ":\n";
}

bool
Test::Run_( FailHandler_ inHandler )
{
  mFailStream.clear();
  mFailStream.str( "" );
  OnRun_();
  mFailStream.flush();
  string s = mFailStream.str();
  if( !s.empty() )
  {
    if( inHandler )
      inHandler( s );
    else
      DefaultHandler( s );
  }
  return s.empty();
}

int
Test::Parse_( int argc, char** argv, bool exitIfFound )
{
  bool found = false;
  for( int i = 0; !found && i < argc; ++i )
  {
    if( !::stricmp( argv[i], "--bcitest" ) )
      found = true;
    else if( !::stricmp( argv[i], "--bcitest=self" ) )
    {
      found = true;
      sSelfTest = true;
    }
  }
  int result = 0;
  if( found )
  {
    result = RunAll_();
    if( exitIfFound )
      ::exit( result );
  }
  return result;
}

Test::RunAll_::RunAll_( Test::FailHandler_ inHandler )
: failures( 0 )
{
#if DISABLE_UNITTEST
  const char* pMsg = "This executable was compiled with the DISABLE_UNITTEST flag set";
  if( inHandler )
    inHandler( pMsg );
  else
    DefaultHandler( pMsg );
  failures = -1;
#else // DISABLE_UNITTEST
  for( size_t i = 0; i < sTests.size(); ++i )
    if( !Test::sTests[i]->Run_( inHandler ) )
      ++failures;
#endif // DISABLE_UNITTEST
}

UnitTest( UnitTestSelfTest )
{
  TestFail_if( sSelfTest == true, "SelfTest succeeded" );
}
