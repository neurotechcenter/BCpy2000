////////////////////////////////////////////////////////////////////////////////
// $Id: BCI2000SessionReader.cpp 3798 2012-02-01 18:07:06Z mellinger $
// Authors: schalk@wadsworth.org, juergen.mellinger@uni-tuebingen.de
// Description: Class that provides an interface to the data stored in a
//              BCI2000 data file.
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

#include "BCI2000SessionReader.h"
#include <dirent.h>

using namespace std;

// **************************************************************************
// Function:   BCI2000SessionReader
// Purpose:    The constructor for the BCI2000SessionReader object
// Parameters: N/A
// Returns:    N/A
// **************************************************************************
BCI2000SessionReader::BCI2000SessionReader()
{
  Reset();
}

BCI2000SessionReader::BCI2000SessionReader( const char* inFileName )
{
  Reset();
  Open( inFileName );
}

// **************************************************************************
// Function:   ~BCI2000SessionReader
// Purpose:    The destructor for the BCI2000SessionReader object
// Parameters: N/A
// Returns:    N/A
// **************************************************************************
BCI2000SessionReader::~BCI2000SessionReader()
{
}

// **************************************************************************
// Function:   Reset
// Purpose:    Resets file related data members to a defined state.
// Parameters: N/A
// Returns:    N/A
// **************************************************************************
void
BCI2000SessionReader::Reset()
{
  BCI2000FileReader::Reset();
  mInitialized = false;
  mBufSize = 0;
  mRunNames.clear();
  mNumSamples.clear();
  mCurRun = 0;
  mErrorState = NoError;
}

int
BCI2000SessionReader::ErrorState() const
{
  return mErrorState == NoError ? BCI2000FileReader::ErrorState() : mErrorState;
}

// **************************************************************************
// Function:   Open
// Purpose:    This method initializes the object by reading in
//             the header and creating a list of all parameters and states
// Parameters: filename - name of the session of interest
//             buf_size - size of input buffer to use
// **************************************************************************
BCI2000SessionReader&
BCI2000SessionReader::Open( const char* inFileName, int inBufSize )
{
  Reset();
  mRunNames.push_back( "" );
  mNumSamples.push_back( 0 );
  mBufSize = inBufSize;

  // Obtain the input file's signal properties and state vector size.
  BCI2000FileReader::Open( inFileName, 0 );
  ::SignalProperties signalProperties = SignalProperties();
  ::StateList stateList = StateList();

  // Extract the file name's session part.
  string sessionName = inFileName;
  int runPos = sessionName.find_last_of( "rR", sessionName.rfind( '.' ) );
  sessionName = sessionName.substr( 0, runPos );
  int sepPos = sessionName.find_last_of( "/\\" );
  string path = ".";
  if( sepPos != string::npos )
  {
    path = sessionName.substr( 0, sepPos );
    sessionName = sessionName.substr( sepPos + 1 );
  }
  // Find all files in the directory matching the session name:
  DIR* dir = ::opendir( path.c_str() );
  if( dir != NULL )
  {
    struct dirent* entry;
    while( NULL != ( entry = ::readdir( dir ) ) )
    {
      string curFile = entry->d_name;
      if( curFile.find( sessionName ) == 0 )
      {
        string fullPath = path + "/" + curFile;
        BCI2000FileReader::Open( fullPath.c_str(), 0 );
        if( BCI2000FileReader::ErrorState() == BCI2000FileReader::NoError )
        {
          if( signalProperties != SignalProperties() || stateList != StateList() )
            mErrorState = InconsistentFiles;
          else
          {
            mRunNames.push_back( fullPath );
            mNumSamples.push_back( NumSamples() + BCI2000FileReader::NumSamples() );
          }
        }
      }
    }
    ::closedir( dir );
  }
  if( mRunNames.size() > 1 )
  {
    BCI2000FileReader::Open( mRunNames[ 1 ].c_str(), mBufSize );
    if( BCI2000FileReader::IsOpen() )
    {
      mCurRun = 1;
      mInitialized = true;
    }
  }

  return *this;
}

// **************************************************************************
// Function:   RawValue
// Purpose:    Returns the sample value for a given sample and channel number
//             that is, the sample in the current run
// Parameters: channel - channel number
//             sample - sample number
// Returns:    value requested
// **************************************************************************
GenericSignal::ValueType
BCI2000SessionReader::RawValue( int inChannel, long inSample )
{
  return BCI2000FileReader::RawValue( inChannel, SetSampleRun( inSample ) );
}

// **************************************************************************
// Function:   ReadStateVector
// Purpose:    reads the statevector for a given sample
//             the results are in a statevector pointed to by "statevector"
// Parameters: sample - sample number
// Returns:    N/A
// **************************************************************************
BCI2000SessionReader&
BCI2000SessionReader::ReadStateVector( long inSample )
{
  BCI2000FileReader::ReadStateVector( SetSampleRun( inSample ) );
  return *this;
}

// **************************************************************************
// Function:   SetSampleRun
// Purpose:    Determines which run is associated with a given sample,
//             and switches to this run.
// Parameters: global sample number
// Returns:    sample number within the sample's run
// **************************************************************************
long
BCI2000SessionReader::SetSampleRun( long inSample )
{
  if( mNumSamples.empty() || inSample > *mNumSamples.rbegin() )
    return 0;

  int sampleRun = 1;
  while( mNumSamples[ sampleRun ] < inSample )
    ++sampleRun;

  if( mCurRun != sampleRun )
  {
    BCI2000FileReader::Open( mRunNames[ sampleRun ].c_str(), mBufSize );
    mCurRun = sampleRun;
  }

  return inSample - mNumSamples[ sampleRun - 1 ];
}


