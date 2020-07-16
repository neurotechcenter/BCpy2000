////////////////////////////////////////////////////////////////////////////////
// $Id: FileWriterBase.cpp 5490 2016-08-04 16:40:42Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A base class that implements functionality common to all
//              file writer classes that output into a file.
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
#include "FileWriterBase.h"
#include "BCIStream.h"
#include "FileUtils.h"
#include "ClassName.h"

#include <fstream>
#include <iostream>
#include <iomanip>

using namespace std;

static const int bufferSize = 65536;
static const char* bciParameterExtension = ".prm";

static string
ParameterFile( const string& inDataFile )
{
  return FileUtils::ExtractDirectory( inDataFile )
       + FileUtils::ExtractBase( inDataFile )
       + bciParameterExtension;
}


FileWriterBase::FileWriterBase( GenericOutputFormat& inOutputFormat )
: mrOutputFormat( inOutputFormat ), mStreambuf( 0, bufferSize ),
  mOutputFile( &mStreambuf )
{
}

FileWriterBase::~FileWriterBase()
{
  Halt();
}

void
FileWriterBase::Publish()
{
  mrOutputFormat.Publish();

  string ext = mrOutputFormat.DataFileExtension();
  size_t i = 0;
  while( i < ext.length() && ::ispunct( ext[i] ) )
    ++i;
  string formatName = ext.substr( i );

  if( Parameters->Exists( "FileFormat" ) )
    Parameters->Delete( "FileFormat" );
  string def = "Storage string FileFormat= " + formatName + " % % % // format of data file (readonly)";
  BEGIN_PARAMETER_DEFINITIONS
    def.c_str(),
  END_PARAMETER_DEFINITIONS

  if( OptionalParameter( "SavePrmFile" ) != 0 )
  {
    BEGIN_PARAMETER_DEFINITIONS
      "Storage:Documentation int SavePrmFile= 0 1 0 1 "
        "// save additional parameter file for each run (0=no, 1=yes) (boolean)",
    END_PARAMETER_DEFINITIONS
  }

  BEGIN_PARAMETER_DEFINITIONS
    "Storage:Documentation string StorageTime= % % % % "
      "// time of beginning of data storage",
  END_PARAMETER_DEFINITIONS
}

void
FileWriterBase::Preflight( const SignalProperties& Input,
                                 SignalProperties& Output ) const
{
  mrOutputFormat.Preflight( Input, *Statevector );

  // State availability.
  State( "Recording" );

  // File accessibility.
  string dataFile = CurrentRun();
  if( OptionalParameter( "SavePrmFile" ) == 1 )
  {
    string paramFile = ParameterFile( dataFile );
    if( FileUtils::Exists( paramFile ) )
      bcierr << "Parameter file \"" << paramFile << "\" already exists, "
             << "will not be touched.";
    else if( !FileUtils::IsWritableLocation( paramFile ) )
      bcierr << "Cannot write to file location \"" << paramFile << "\"";
  }

  if( !string( Parameter( "StorageTime" ) ).empty() )
    bciout << "The StorageTime parameter will be overwritten with the"
           << " recording's actual date and time"
           << endl;

  Output = SignalProperties( 0, 0 );
}


void
FileWriterBase::Initialize( const SignalProperties& Input,
                            const SignalProperties& /*Output*/ )
{
  mrOutputFormat.Initialize( Input, *Statevector );
}


void
FileWriterBase::StartRun()
{
  time_t now = ::time( NULL );
  struct tm* timeinfo = ::localtime( &now );
  char buffer[20];
  ::strftime( buffer, sizeof( buffer ), "%Y-%m-%dT%H:%M:%S", timeinfo );
  Parameter( "StorageTime" ) = buffer;

  mFileName = CurrentRun();
  mOutputFile.clear();
  mStreambuf.SetOutput( &CurrentRunFile().Output() );

  if( OptionalParameter( "SavePrmFile" ) == 1 )
  {
    string paramFile = ParameterFile( mFileName );
    ofstream file( paramFile.c_str() );
    if( !( file << *Parameters << flush ) )
      bcierr << "Error writing parameters to file "
             << paramFile;
  }

  mrOutputFormat.StartRun( mOutputFile, mFileName );
  mQueue.Clear();
  Thread::Start();
}


void
FileWriterBase::StopRun()
{
  Halt();
  mrOutputFormat.StopRun( mOutputFile );
  mStreambuf.SetOutput( nullptr );
  mOutputFile.clear();

  if( !mQueue.Empty() )
    bcierr << "Nonempty buffering queue" << endl;
}

void
FileWriterBase::Halt()
{
  Thread::Terminate();
}

void
FileWriterBase::Write( const GenericSignal& Signal,
                       const StateVector&   Statevector )
{
  mQueue.Produce( make_pair( Signal, Statevector ) );
}

int FileWriterBase::OnExecute()
{
  Queue::Consumable c;
  while( mOutputFile && ( c = mQueue.AwaitConsumption( Time::Forever ) ) )
  {
    mrOutputFormat.Write( mOutputFile, c->first, c->second );
    mOutputFile.flush();
  }
  while( c = mQueue.Consume() )
    mrOutputFormat.Write( mOutputFile, c->first, c->second );
  mOutputFile.flush();
  if( !mOutputFile )
    bcierr << "Error writing to file \"" << mFileName << "\"";
  return 0;
}
