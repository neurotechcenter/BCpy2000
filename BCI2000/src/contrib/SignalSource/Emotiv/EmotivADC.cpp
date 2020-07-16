////////////////////////////////////////////////////////////////////////////////
// $Id: EmotivADC.cpp 4536 2013-08-05 14:30:13Z mellinger $
// Author: griffin.milsap@gmail.com
// Description: BCI2000 Source Module for the Emotiv device.
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

#include "EmotivADC.h"
#include "PrecisionTime.h"
#include <sstream>
#include <stdio.h>

using namespace std;

// Register the source class with the framework.
RegisterFilter( EmotivADC, 1 );

// Ensure these two arrays are of the same size
EE_DataChannel_t targetChannelList[] = {
  ED_AF3, ED_F7, ED_F3, ED_FC5, ED_T7,
  ED_P7, ED_O1, ED_O2, ED_P8, ED_T8,
  ED_FC6, ED_F4, ED_F8, ED_AF4
};

string channelNames[] =
{
  "AF3", "F7", "F3", "FC5",
  "T7", "P7", "O1", "O2",
  "P8", "T8", "FC6", "F4",
  "F8", "AF4"
};

// Ensure these are of the same size too.
// This may seem redundant, but it isn't.  One channel != One electrode.
// There are some electrodes (CMS and DRL) which have contact quality, but
// do not have any sort of logged EEG data.
EE_InputChannels_t electrodes[] =
{
  EE_CHAN_CMS, EE_CHAN_DRL, EE_CHAN_FP1, EE_CHAN_AF3, EE_CHAN_F7,
  EE_CHAN_F3, EE_CHAN_FC5, EE_CHAN_T7, EE_CHAN_P7, EE_CHAN_O1,
  EE_CHAN_O2, EE_CHAN_P8, EE_CHAN_T8, EE_CHAN_FC6, EE_CHAN_F4,
  EE_CHAN_F8, EE_CHAN_AF4, EE_CHAN_FP2
};

string electrodeNames[] =
{
  "CMS", "DRL", "FP1", "AF3", "F7", "F3", "FC5", "T7", "P7", "O1",
  "O2", "P8", "T8", "FC6", "F4", "F8", "AF4", "FP2"
};


// **************************************************************************
// Function:   EmotivADC
// Purpose:    The constructor for the EmotivADC.
// **************************************************************************
EmotivADC::EmotivADC()
: mState( NULL ),
  mEvent( NULL ),
  mDataHandle( NULL ),
  mDCOffset( 4000 ),
  mLowBatt( false )
{
  // Add Parameters
  BEGIN_PARAMETER_DEFINITIONS
    "Source int SourceCh=      14 14 14 32 "
      "// number of digitized channels total",
    "Source int SampleBlockSize= 4 4 1 128 "
      "// number of samples per block",
    "Source int SamplingRate=    128 128 128 128 "
      "// the signal sampling rate",

    // Currently, the Emotiv API does not allow raw EEG acquisition through a connection to EmoComposer
    // If this functionality ever comes back, simply uncomment associated lines
    //"Source int ConnectionType= 0 0 0 1 "
    //  "// Desired Connection Type "
    //  "0: Direct, "
    //  "1: EmoComposer, "
    //  "(enumeration)",

    //"Source string EmoComposerIP= % % % % "
    //  "// IP Address -ignored with Direct Connection Type",
  END_PARAMETER_DEFINITIONS

  // Add Gyro States
  BEGIN_STATE_DEFINITIONS
    "EmotivGyroX 16 0 0 0",
    "EmotivGyroY 16 0 0 0",
  END_STATE_DEFINITIONS

  // Allocate 4 bits for each contact quality state
  for( int i = 0; i < sizeof( electrodeNames ) / sizeof( *electrodeNames ); i++ )
  {
    string state = "EmotivCQ" + electrodeNames[i] + " 4 0 0 0";
    BEGIN_STATE_DEFINITIONS
      state.c_str(),
    END_STATE_DEFINITIONS
  }

  // Allocate memory for the Emotiv API
  mEvent = EE_EmoEngineEventCreate();
  mState = EE_EmoStateCreate();

  // Set up channel names
  // (This will fail if the number of entries in the above arrays are not the same)
  for( int i = 0; i < sizeof( targetChannelList ) / sizeof( EE_DataChannel_t ); i++ )
    mChannelNames.push_back( channelNames[i] );
}


EmotivADC::~EmotivADC()
{
  // Disconnect from the device
  Halt();

  // Deallocate the memory for the Emotiv API
  EE_EmoStateFree(mState);
  EE_EmoEngineEventFree(mEvent);
}


// **************************************************************************
// Function:   Preflight
// Purpose:    Checks parameters for availability and consistence with
//             input signal properties; requests minimally required properties
//             for the output signal; checks whether resources are available.
// Parameters: References to input and output signal properties.
// Returns:    N/A
// **************************************************************************
void EmotivADC::Preflight( const SignalProperties&,
                                   SignalProperties& Output ) const
{
  // Grab important parameters
  int sourceCh = Parameter( "SourceCh" ),
      sampleBlockSize = Parameter( "SampleBlockSize" );

  // Connect to the Emotiv Engine
  EmotivConnect();

  // Attempt to find the device
  unsigned int userID = 0;
  DataHandle hData = EE_DataCreate();
  bcidbg( 1 ) << "Waiting for Emotiv device to connect." << endl;

  // Connect to the first user
  int state	= 0;
  bool readyToCollect = false;
  PrecisionTime starttime = PrecisionTime::Now();
  int timeout = 5000; // We'll wait 5 seconds for the device to connect.
  while ( !readyToCollect && PrecisionTime::UnsignedDiff( PrecisionTime::Now(), starttime ) < timeout )
  {
    state = EE_EngineGetNextEvent( mEvent );

    if( state == EDK_OK )
    {
      EE_Event_t eventType = EE_EmoEngineEventGetType( mEvent );
      EE_EmoEngineEventGetUserId( mEvent, &userID );

      if( userID != 0 )
        bcierr << "The current Emotiv module does not support more than one Emotiv device." << endl;

      // See if we have a user connected or not
      if( eventType == EE_UserAdded )
        bcidbg( 1 ) << "Emotiv User Connected.  Waiting for Headset on..." << endl;

      // See if we've removed a user or not
      if( eventType == EE_UserRemoved )
        bcidbg( 1 ) << "Emotiv User Disconnected.  Waiting for a user connection" << endl;

      // There is a user connected, we need to check the state
      if (eventType == EE_EmoStateUpdated)
      {
        EE_EmoEngineEventGetEmoState( mEvent, mState );

        if( ES_GetHeadsetOn( mState ) )
        {
          bcidbg( 1 ) << "Emotiv device on, initiating data collection" << endl;
          readyToCollect = true;
        }
      }
    }

    // We still havn't connected.  We don't want to max out CPU, so we relinquish the time slice.
    Sleep( 0 );
  }

  if( !readyToCollect )
  {
    bcierr << "Timeout occurred while waiting for device to become ready." << endl;
    return;
  }

  // Verify that signal names are correct
  if( Parameter( "ChannelNames" )->NumValues() != sizeof( targetChannelList ) / sizeof( EE_DataChannel_t ) )
  {
    bciout << "Channel Names should have " << sizeof( targetChannelList ) / sizeof( EE_DataChannel_t ) <<
              " entries." << endl;
    bciout << "Channel Names should be: ";
    for( unsigned int i = 0; i < mChannelNames.size(); i++ )
      bciout << mChannelNames[i] << " ";
    bciout << endl;
  }
  else
    for( int i = 0; i < sizeof( targetChannelList ) / sizeof( EE_DataChannel_t ); i++ )
      if( Parameter( "ChannelNames" )( i ) != mChannelNames[i] )
        bciout << "Entry number " << (i+1) << " in ChannelNames should be " << mChannelNames[i] << endl;

  // Check the number of channels
  if( ( int )Parameter( "SourceCh" ) != sizeof( targetChannelList ) / sizeof( EE_DataChannel_t ) )
    bcierr << "Set SourceCh = " << sizeof( targetChannelList ) / sizeof( EE_DataChannel_t ) << endl;

  // Check Sampling Rate
  unsigned int samplingRate = 0;
  if( EE_DataGetSamplingRate( userID, &samplingRate ) == EDK_OK )
  {
    if( static_cast<int>( Parameter( "SamplingRate" ).InHertz() ) != samplingRate )
      bcierr << "Set SamplingRate = " << samplingRate << endl;
  }
  else
    bcierr << "Cannot retreive Emotiv sampling rate " << endl;

  // Check DC Offset List
  if( Parameter( "SourceChOffset" )->NumValues() != sizeof( targetChannelList ) / sizeof( EE_DataChannel_t ) )
    bcierr << "There should be " << sizeof( targetChannelList ) / sizeof( EE_DataChannel_t ) << " entries in SourceChOffset." << endl;
  else
    // Check individual entries
    for( int i = 0; i < sizeof( targetChannelList ) / sizeof( EE_DataChannel_t ); i++ )
      if( Parameter( "SourceChOffset" )( i ) != 0 )
        bcierr << "Entry number " << (i+1) << " in SourceChOffset should be 0." << endl;

  // Get Resolution List, Set Param
  if( Parameter( "SourceChGain" )->NumValues() != sizeof( targetChannelList ) / sizeof( EE_DataChannel_t ) )
    bcierr << "There should be " << sizeof( targetChannelList ) / sizeof( EE_DataChannel_t ) << " entries in SourceChGain." << endl;
  else
    // Check individual entries
    for( int i = 0; i < sizeof( targetChannelList ) / sizeof( EE_DataChannel_t ); i++ )
      if( fabs( Parameter( "SourceChGain" )( i ) - ( double )1 ) > 0.00001 )
        bcierr << "Entry number " << i+1 << " in SourceChGain should be 1." << endl;

  // Verify that we have a low pass filter on the output
  if( Parameter( "HighPassFilter" ) != 1 )
    bcierr << "Set HighPassFilter to 0.1Hz to filter out DCOffset" << endl;

  // Disconnect from the device
  if( hData )
  {
    EE_DataFree( hData );
    hData = NULL;
  }

  // Disconnect from Emotiv
  EE_EngineDisconnect();

  // Set the Signal Properties
  Output = SignalProperties( sourceCh, sampleBlockSize, SignalType::float32 );
}


// **************************************************************************
// Function:   Initialize
// Purpose:    This function parameterizes the NeuroSkyADC.
//             It is called each time parameters have been changed.
// Parameters: References to input (ignored) and output signal properties.
// Returns:    N/A
// **************************************************************************
void EmotivADC::Initialize( const SignalProperties&,
                              const SignalProperties& Output )
{
  // Stop all current communication
  Halt();

  // Connect to the Emotiv Engine
  EmotivConnect();

  // Create a data handle for this session
  mDataHandle = EE_DataCreate();

  // Create a one second buffer (Should be enough)
  float time = 1;
  EE_DataSetBufferSizeInSec( time );

  // Size the data buffer
  for( int i = 0; i < sizeof( targetChannelList ) / sizeof( EE_DataChannel_t ); i++ )
    mData.push_back( queue< float >() );

  // Connect to the first user
  int state	= 0;
  unsigned int userID = 0;
  bool readyToCollect = false;
  while ( !readyToCollect )
  {
    state = EE_EngineGetNextEvent( mEvent );

    if( state == EDK_OK )
    {
      EE_Event_t eventType = EE_EmoEngineEventGetType( mEvent );
      EE_EmoEngineEventGetUserId( mEvent, &userID );

      // See if we have a user connected or not
      if( eventType == EE_UserAdded )
        EE_DataAcquisitionEnable( userID, true );

      // See if we've removed a user or not
      if( eventType == EE_UserRemoved )
        EE_DataAcquisitionEnable( userID, false );

      // There is a user connected, we need to check the state
      if (eventType == EE_EmoStateUpdated)
      {
        EE_EmoEngineEventGetEmoState( mEvent, mState );

        // If we have a headset on, we're ready to go
        if( ES_GetHeadsetOn( mState ) )
          readyToCollect = true;
      }
    }
  }
}


// **************************************************************************
// Function:   Process
// Purpose:    This function is called within the data acquisition loop
//             it fills its output signal with values
//             and does not return until all data has been acquired.
// Parameters: References to input signal (ignored) and output signal.
// Returns:    N/A
// **************************************************************************
void EmotivADC::Process( const GenericSignal&, GenericSignal& Output )
{
  unsigned int userID = 0;
  int state = 0;

  float *gyrox, *gyroy;
  gyrox = new float( 0.0f );
  gyroy = new float( 0.0f );

  // Just do a quick overbuffering check
  if( mData[0].size() > 128 )
    bcierr << "System not performing fast enough, DAQ is not real time anymore." << endl;

  // While we don't have enough for a full data block we'll collect samples
  // We'll do a timeout check here as well
  PrecisionTime starttime = PrecisionTime::Now();
  int timeout = ( int )( ( 1000.0f / 128.0f ) * ( float )Output.Elements() ) * 10 ; // We'll wait 10 datablocks for the device to connect.
  while( mData[0].size() < ( unsigned int )Output.Elements() && PrecisionTime::UnsignedDiff( PrecisionTime::Now(), starttime ) < timeout )
  {
    // Check for updated states
    state = EE_EngineGetNextEvent( mEvent );

    // Verify that the updated states still allow us to collect data
    if( state == EDK_OK )
    {
      EE_Event_t eventType = EE_EmoEngineEventGetType( mEvent );
      EE_EmoEngineEventGetUserId( mEvent, &userID );

      // Check if another device has been connected
      if( userID != 0 )
        bcierr << "The current Emotiv module does not support more than one Emotiv device." << endl;

      // Check for User Removal
      if( eventType == EE_UserRemoved )
      {
        EE_DataAcquisitionEnable( userID, false );
        bcierr << "Current Emotiv User removed!" << endl;
        return;
      }

      // User is connected, but state has been updated
      if (eventType == EE_EmoStateUpdated)
      {
        EE_EmoEngineEventGetEmoState( mEvent, mState );

        // Check if device is still on
        if( !ES_GetHeadsetOn( mState ) )
        {
          bcierr << "Headset Turned Off." << endl;
          return;
        }

        // Warn about poor signal quality
        // Set up Contact Quality structure
        int numKnownChannels = sizeof( electrodeNames ) / sizeof( *electrodeNames ),
            numQueriedCh = min( numKnownChannels, ES_GetNumContactQualityChannels( mState ) );
        EE_EEG_ContactQuality_t* mpCQ = new EE_EEG_ContactQuality_t[numQueriedCh];
        int size = ES_GetContactQualityFromAllChannels( mState, mpCQ, numQueriedCh );
        for( int i = 0; i < min( size, numQueriedCh ); i++ )
          State( "EmotivCQ" + electrodeNames[i] ) = mpCQ[i];
        delete [] mpCQ;

        // Warn about poor battery life
        if( !mLowBatt )
        {
          int charge = 0, maxCharge = 0;
          ES_GetBatteryChargeLevel( mState, &charge, &maxCharge );
          if( charge >= 0 )
          { // charge may be -1, indicating "no information present"
            if( ( 100 * charge ) / maxCharge < 10 )
            {
              bciout << "Low Battery; Charge < 10%" << endl;
              mLowBatt = true;
            }
          }
        }
      }
    }

    // Update the system
    EE_DataUpdateHandle( 0, mDataHandle );

    // Find out how many samples have accrued
    unsigned int nSamplesTaken=0;
    EE_DataGetNumberOfSample( mDataHandle, &nSamplesTaken );

    if( nSamplesTaken != 0 )
    {
      // Allocate space and grab the samples for each channel
      double* data = new double[ nSamplesTaken ];
      for ( int i = 0; i < sizeof( targetChannelList ) / sizeof( EE_DataChannel_t ); i++ )
      {
        int err = EE_DataGet( mDataHandle, targetChannelList[i], data, nSamplesTaken );
        if( err != EDK_OK )
          bcierr << "Emotiv Error.  Could be overbuffering." << endl;
        for ( unsigned int sampleIdx = 0; sampleIdx < nSamplesTaken; sampleIdx++ )
          mData[i].push( float( data[ sampleIdx ] ) - mDCOffset );
      }

      // Grab the most current gyro data
      if( EE_DataGet( mDataHandle, ED_GYROX, data, nSamplesTaken ) != EDK_OK )
        bcierr << "Emotiv Error collecting GyroX Data.  Could not be calibrated..." << endl;
      *gyrox = ( float )data[ nSamplesTaken - 1 ];
      if( EE_DataGet( mDataHandle, ED_GYROY, data, nSamplesTaken ) != EDK_OK )
        bcierr << "Emotiv Error collecting GyroY Data.  Could not be calibrated..." << endl;
      *gyroy = ( float )data[ nSamplesTaken - 1 ];

      // Get CQ States
      if( EE_DataGet( mDataHandle, ED_RAW_CQ, data, nSamplesTaken ) != EDK_OK )
        bcierr << "Emotiv Error getting Contact Quality" << endl;

      unsigned long long* cq = reinterpret_cast< unsigned long long* >( data );

      // Clean up the data structure.
      delete [] data;
    }
    Sleep( 3 );
  }

  // If we timed out, we need to error out
  if( mData[0].size() < ( unsigned int )Output.Elements() )
  {
    bcierr << "Device not reporting data anymore...  Could be disconnected or off." << endl;
    return;
  }

  // Pack the Gyro Data into the states using a bitwise cast
  State( "EmotivGyroX" ) = ( int )( *gyrox * 16 );
  State( "EmotivGyroY" ) = ( int )( *gyroy * 16 );
  delete gyrox;
  delete gyroy;

  // Now we have enough data for the sample block
  for( int i = 0; i < sizeof( targetChannelList ) / sizeof( EE_DataChannel_t ); i++ )
  {
    for( int j = 0; j < Output.Elements(); j++ )
    {
      Output( i, j ) = mData[i].front();
      mData[i].pop();
    }
  }
}

// **************************************************************************
// Function:   Halt
// Purpose:    This routine shuts down data acquisition
// Parameters: N/A
// Returns:    N/A
// **************************************************************************
void EmotivADC::Halt()
{
  // End the session
  if( mDataHandle )
  {
    EE_DataFree( mDataHandle );
    mDataHandle = NULL;
  }

  // Disconnect from the Emotiv Engine
  EE_EngineDisconnect();
}

// **************************************************************************
// Function:   EmotivConnect
// Purpose:    Connects to the Emotiv Engine based on Parameter Selection
// Parameters: N/A
// Returns:    N/A
// **************************************************************************
void EmotivADC::EmotivConnect() const
{
  // Attempt a Connection
  //int connectionType = Parameter( "ConnectionType" );
  int connectionType = 0;
  switch( connectionType )
  {
    case 0:
      // We're attempting a direct connection.
      if( EE_EngineConnect() != EDK_OK )
        bcierr << "Emotiv Engine start up failed." << endl;
      break;
    case 1:
      // We're connecting through EmoComposer.  What's the IP?
      string ip = Parameter( "EmoComposerIP" );
      int composerPort = 1726;

      // If no ip is entered, we'll default to 127.0.0.1, as this is likely the case.
      if ( ip.empty() )
      ip = string( "127.0.0.1" );

      // Attempt the remote connection
      if( EE_EngineRemoteConnect( ip.c_str(), composerPort ) != EDK_OK )
        bcierr << "Cannot connect to EmoComposer on [" << ip << "]" << endl;
      break;
  }
}

