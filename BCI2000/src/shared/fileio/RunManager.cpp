////////////////////////////////////////////////////////////////////////////////
// $Id: RunManager.cpp 5638 2017-07-03 13:57:28Z mellinger $
// Authors: schalk@wadsworth.org, juergen.mellinger@uni-tuebingen.de
// Description: A class that manages BCI2000 data file names.
//   Some of the functionality here consists of workarounds that will no longer
//   be necessary in 4.x (which will introduce publication of output file name).
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
#include "RunManager.h"
#include "FileUtils.h"
#include "FilePath.h"
#include "GenericFilter.h"
#include "BCIStream.h"

#include <iostream>
#include <iomanip>
#include <sstream>
#include <cstdlib>

using namespace std;

static const string sbTag = "${", seTag = "}";

static string
IntToString( int inNumber, int inWidth )
{
  ostringstream oss;
  oss << setfill( '0' ) << setw( inWidth ) << inNumber;
  return oss.str();
}

static int
StringToInt( const string& inString )
{
  return ::atoi( inString.c_str() );
}

RunManager::RunManager()
: mPublished( false ),
  mInWritingModule( false )
{
}

RunManager::~RunManager()
{
  if(mCurrentRunFile.Length() == 0)
    FileUtils::RemoveFile(mCurrentRun);
}

string
RunManager::CurrentSession()
{
  return CurrentSession( *Parameters );
}

const string&
RunManager::CurrentRun()
{
  if( !mInWritingModule )
    bcierr << "Cannot obtain current run outside writing module";
  else if( !mPublished )
    bcierr << "Trying to access current run file, which is undefined";
  else if( !mCurrentRunFile.IsOpen() )
    mCurrentRun = ConstructRun();
  return mCurrentRun;
}

Tiny::File&
RunManager::CurrentRunFile()
{
  CurrentRun();
  return mCurrentRunFile;
}

void
RunManager::Publish()
{
#ifdef TODO
# error Remove this test from V4
#endif
  GenericFilter::ChainInfo info = GenericFilter::GetChainInfo();
  for( GenericFilter::ChainInfo::const_iterator i = info.begin(); i != info.end(); ++i )
    mInWritingModule |= !::stricmp( i->name.c_str(), "DataIOFilter" );

  if( mInWritingModule )
  {
    bool existed = Parameters->Exists( "DataFile" );
    BEGIN_PARAMETER_DEFINITIONS
      "Storage:Data%20Location string DataFile= % % % % "
      "// name of data file relative to DataDirectory",
    END_PARAMETER_DEFINITIONS
    if( !existed )
      Parameter( "DataFile" ) = "${SubjectName}${SubjectSession}/${SubjectName}S${SubjectSession}R${SubjectRun}.${FileFormat}";
  }
  mPublished = true;
}

void
RunManager::Preflight() const
{
  if(mCurrentRunFile.Length() > 0)
    mCurrentRunFile.Close();
}

void
RunManager::Initialize()
{
}

void
RunManager::StopRun()
{
  mCurrentRunFile.Close();
}

string
RunManager::DataDirectory( ParamListView& inParams )
{
  Param& session = inParams["SubjectSession"];
  session.Value() = IntToString( StringToInt( session.Value() ), 3 );
  Param& run = inParams["SubjectRun"];
  run.Value() = IntToString( StringToInt( run.Value() ), 2 );

  Tiny::FilePath path = inParams["DataDirectory"].Value().AsString();
  path.ToAbsolute( FileUtils::InstallationDirectory() ).Simplify();
  return path.ToString() + path.NativeSeparator();
}

string
RunManager::CurrentSession( ParamListView& inParams )
{
  string dataDir = DataDirectory( inParams ),
         file = inParams["DataFile"].Value(),
         dir = FileUtils::ExtractDirectory( file ),
         runRef = sbTag + "SubjectRun" + seTag;
  file = FileUtils::ExtractBase( file );

  size_t pos;
  while( string::npos != ( pos = file.find( runRef ) ) )
  {
    size_t i = pos;
    while( i > 0 && ::isalpha( file[i-1] ) )
      --i;
    file = file.substr( 0, i ) + file.substr( pos + runRef.length() );
  }
  if( file.empty() )
    file = "Session${SubjectSession}";
  
  while( string::npos != ( pos = dir.find( runRef ) ) )
    dir = dir.substr( 0, pos ) + "_MultipleRuns_" + dir.substr( pos + runRef.length() );
  
  return dataDir + SubstituteParameters( dir + file, inParams );
}

string
RunManager::ConstructRun()
{
  ParamListView& parameters = *Parameters;
  string path = DataDirectory( parameters ),
         first, file, result;
  int run = ::atoi( parameters["SubjectRun"].Value().c_str() );
  if( --run < 0 )
    run = 0;
  bool done = false, ok = true;
  while( ok && !done )
  {
    parameters["SubjectRun"].Value() = IntToString( ++run, 2 );
    file = SubstituteParameters( parameters["DataFile"].Value(), *Parameters );
    if( first.empty() )
      first = file;
    else if( file == first )
    {
      string ext = FileUtils::ExtractExtension( file );
      file = FileUtils::StripExtension( file ) + IntToString( run, 0 ) + ext;
    }
    result = path + file;

    std::string dir = FileUtils::ExtractDirectory( result );
    if( !FileUtils::MakeDirectory( dir, true ) )
    {
      ok = false;
      bcierr << "Could not create directory: " << dir;
    }
    else
    {
      done = mCurrentRunFile.Open( result, File::out | File::exclusive );
      ok = FileUtils::Exists( result ); // ok but not done if file existed
      if( !ok )
        bcierr << "Could not create output file: " << result;
    }
  }
  if( ok )
    result = FileUtils::NormalizedPath( result );
  return result;
}

string
RunManager::SubstituteParameters( const string& inExpr, const ParamListView& inParams )
{
  set<string> occurred;

  string result = inExpr;
  size_t beg;
  while( string::npos != ( beg = result.find( sbTag ) ) )
  {
    size_t end = result.find( seTag, beg + sbTag.length() );
    if( end != string::npos )
    {
      string name = result.substr( beg, end - beg );
      name = name.substr( sbTag.length() );
      string value;
      if( occurred.find( name ) != occurred.end() )
        bcierr << "Circular reference to parameter " << name;
      else if( inParams.Exists( name ) )
        value = inParams[name].Value();
      result = result.substr( 0, beg ) + value + result.substr( end + 1 );
    }
  }
  return result;
}

