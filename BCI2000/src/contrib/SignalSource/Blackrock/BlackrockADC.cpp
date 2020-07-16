////////////////////////////////////////////////////////////////////////////////
// $Id: BlackrockADC.cpp 5464 2016-07-19 22:32:03Z mellinger $
// Authors: griffin.milsap@gmail.com
// Description: Implementation of a source module for Blackrock systems
//
// This module works in a slightly nonstandard way.  The Blackrock NSP is very
// configurable using the Central application.  The initial version of this 
// source module was originally intended to configure the NSP entirely
// from BCI2000, but the device is SOOO configurable (even to the extent of 
// being able to set different sampling rates for individual channels), it
// became infeasable.  As such, this module will simply query the current
// channel configuration from the device and work with that.  This module is
// no longer capable of changing the Blackrock configuration; all configuration
// changes must be made in Central.
//
// The only really configurable parameter is the SamplingRate parameter which
// decides a sampling group to record from.  Because the device can record
// individual channels at different sampling rates, you must choose the sampling
// rate of the system, and channels that are currently being sampled at that rate
// are recorded into the BCI2000 data stream.  BCI2000 does not support per-channel
// sampling rates, so all channels must be configured to sample at the same rate.
// That is, you can still have some channels sampled at a different rate, but 
// BCI2000 will ignore them.
//
// See more documentation on the BCI2000 WIKI.
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
#include "BlackrockADC.h"
#include "BCIStream.h"

using namespace std;

// This list should REALLY be included in the CBSDK library somewhere
// There are a maximum of 8 sample groups. When you query sample groups
// from the API, they are 1-indexed (hence, GroupRates[0] = 0).  These
// sample groups correspond to the output NSx files.  NS2 files are sampled
// at 1k, ns4 at 10k, etc.  The raw sample group is cbRAWGROUP (6) and is 
// also sampled at 30k.  Currently, there is no way to query this information
// from the CBSDK API, but may change in the future.
int gGroupRates[] = { 0, 500, 1000, 2000, 10000, 30000 }; // samples per second
int gBlockSizes[] = { 0, 10,  20,   40,   200,   600   }; // samples per block (50 Hz)

// IT IS CRUCIAL THAT THE SYSTEM IS NOT RUN THROUGH MORE THAN ONE SWITCH!
// IF IT ISN'T, THERE WILL VERY LIKELY BE DROPPED PACKETS DURING THE 
// CONFIGURATION PROCESS AND CBSDK HAS UNDEFINED BEHAVIOR.
string gPktErrMsg = 
  "This error suggests an incomplete system configuration due to dropped "
  "packets early in the connection process.  Please ensure that this machine "
  "is connected to the NSP through a SINGLE switch of high commercial quality.";

// Multi-NSP Synchronization Channels
// Connect the output sync channel on ONE amp to the input sync channel on all amps.
// The analog sync input channels on all amps will be monitored and delays will be adjusted
#define NSP_SYNC_OUTPUT_CHANNEL cbFIRST_DIGOUT_CHAN + 1 // Digital Output 1 (153)
#define NSP_SYNC_INPUT_CHANNEL cbFIRST_ANAIN_CHAN + cbNUM_ANAIN_CHANS // Analog Input 16 (144)
#define NSP_SYNC_THRESHOLD 1000 // Threshold for sync pulse from Digital Output 1

RegisterFilter( BlackrockADC, 1 );

BlackrockADC::BlackrockADC()
{
 BEGIN_PARAMETER_DEFINITIONS

    "Source:Signal%20Properties float SamplingRate= 10000Hz "
       "10000Hz 1000 30000 // sample rate",

    // Although you CAN specify these, it'd be best if you let the module
    // autoconfigure them individually based on the SamplingRate.
    "Source auto SourceCh= auto ",
    "Source auto SampleBlockSize= auto ",
    "Source auto ChannelNames= auto ",
    "Source auto SourceChOffset= auto ",
    "Source auto SourceChGain= auto ",

    // Experimental Multi-NSP support
    "Source:Blackrock int NSPInstances= 1 "
      "1 1 4 // Number of synchronized NSPs to record from",

    // Digital Output expressions
    "Source:Blackrock matrix DigitalOutput= 1 { Instance Output Expression } % % % "
      "% % % // Expressions to control the digital outputs"

 END_PARAMETER_DEFINITIONS

 BEGIN_STREAM_DEFINITIONS
    "NSPSyncState 2 0 0 0",
 END_STREAM_DEFINITIONS
}

BlackrockADC::~BlackrockADC()
{

}

void
BlackrockADC::OnAutoConfig()
{
  // Attempt autoconfiguration
  int nInstances = Parameter( "NSPInstances" );
  if( Connect( nInstances ) ) {
    int group = 0;
    if( group = GetRequestedSampleGroup() )
    {
      // Suggest a good SampleBlockSize
      Parameter( "SampleBlockSize" ) = gBlockSizes[ group ];

      // Acquire the channel configuration
      vector< ChanInfo > chanConfig;
      vector< int > syncChans;
      if( GetChannelConfig( nInstances, group, chanConfig, syncChans ) )
      {
        // Resize signal source parameters accordingly
        size_t numChans = chanConfig.size();
        Parameter( "SourceCh" ) = numChans;
        Parameter( "SourceChOffset" )->SetNumValues( numChans );
        Parameter( "SourceChGain" )->SetNumValues( numChans );
        Parameter( "ChannelNames" )->SetNumValues( numChans );

        // Populate channel information
        for( unsigned int i = 0; i < numChans; i++ )
        {
          Parameter( "SourceChOffset" )( i ) = chanConfig[i].offset;
          Parameter( "SourceChGain" )( i ) = chanConfig[i].gain;
          Parameter( "ChannelNames" )( i ) = chanConfig[i].label;
        }
      } else bcierr << "Error acquiring channel configuration from NSP." << endl;
    } else bcierr << "The requested SamplingRate does not correspond to a valid NSP SampleGroup." << endl;
  } else bcierr << "Could not establish connection to cbsdk" << endl;

  Disconnect( nInstances );
}

void
BlackrockADC::OnPreflight( SignalProperties& Output ) const
{
  int nInstances = Parameter( "NSPInstances" );
  if( Connect( nInstances ) ) {
    int group = 0;
    if( group = GetRequestedSampleGroup() )
    {
      // Acquire the channel configuration
      vector< ChanInfo > chanConfig;
      vector< int > syncChans;
      if( GetChannelConfig( nInstances, group, chanConfig, syncChans ) )
      {
        bool sync_procedure_enabled = true;
        for( size_t i = 0; i < syncChans.size(); i++ )
          if( syncChans[ i ] == -1 )
            sync_procedure_enabled = false;
        if( nInstances > 1 && !sync_procedure_enabled )
          bciwarn << "Multi-NSP synchronization procedure not configured properly. "
                  << "Please ensure Analog Input 16 on all NSPs is sampled at the "
                  << "requested sampling rate and that all are connected to "
                  << "Digital Output 1 of any ONE amplifier. PROCEED WITH CAUTION!";

        int numChans = chanConfig.size();

        bool goodOffsets = true,
             goodGains   = true,
             goodNames   = true;
        string matchMessage = " parameter must match the number of channels"
                              " in the requested sample group";

        // Check the SourceCh parameter
        if( Parameter( "SourceCh" ) != numChans )
          bcierr << "The SourceCh "
                 << matchMessage
                 << " (" << numChans << ") ";

        // Check the channel offsets
        if( Parameter( "SourceChOffset" )->NumValues() != numChans )
          bcierr << "The number of values in the SourceChOffset"
                 << matchMessage
                 << " (" << numChans << ") ";
        else
          for( unsigned int i = 0; i < numChans; ++i )
          {
            double chOffset = chanConfig[i].offset;
            double prmoffset = Parameter( "SourceChOffset" )( i );
            bool same = ( 1e-3 > ::fabs( prmoffset - chOffset ) / ( chOffset ? chOffset : 1.0 ) );
            goodOffsets &= same;

            if ( !same ) bciwarn << "CBSDK suggests the offset of"
                                 << " channel " << i + 1
                                 << " is " << chOffset
                                 << " whereas the corresponding value in the"
                                 << " SourceChOffset parameter is " << prmoffset;
          }

        if( !goodOffsets )
          bcierr << "The SourceChOffset values must match the channel "
                 << "resolutions settings in the recording software.";

        // Check gains and ensure they match up with what is reported by cbsdk
        if( Parameter( "SourceChGain" )->NumValues() != numChans )
          bcierr << "The number of values in the SourceChGain"
                 << matchMessage
                 << " (" << numChans << ") ";
        else
          for( unsigned int i = 0; i < numChans; ++i )
          {
            double chGain = chanConfig[i].gain;
            double prmgain = Parameter( "SourceChGain" )( i );
            bool same = ( 1e-3 > ::fabs( prmgain - chGain ) / ( chGain ? chGain : 1.0 ) );
            goodGains &= same;

            if ( !same ) bciwarn << "CBSDK suggests the gain of"
                                 << " channel " << i + 1
                                 << " is " << chGain
                                 << " whereas the corresponding value in the"
                                 << " SourceChGain parameter is " << prmgain;
          }

        if( !goodGains )
          bcierr << "The SourceChGain values must match the channel "
                 << "resolutions settings in the recording software.";

        // Check names and ensure they match up with what is reported by cbsdk
        if( Parameter( "ChannelNames" )->NumValues() != numChans )
          bcierr << "The number of values in the ChannelNames"
                 << matchMessage
                 << " (" << numChans << ") ";
        else
          for( unsigned int i = 0; i < numChans; ++i )
          {
            string prmlabel = Parameter( "ChannelNames" )( i );
            string sdklabel = chanConfig[i].label;
            bool same = prmlabel == sdklabel;
            goodNames &= same;

            if( !same ) bciwarn << "The CBSDK says channel " << i + 1
                                << " is labeled " << sdklabel
                                << " whereas the corresponding value in the"
                                << " ChannelNames parameter is " << prmlabel;
          }

        if( !goodNames )
          bciwarn << "The ChannelNames values should ideally match the channel "
                  << "labels in Central to avoid confusion later.";

      } else bcierr << "Error acquiring channel configuration from NSP." << endl;
    } else bcierr << "The requested SamplingRate does not correspond to a valid NSP SampleGroup." << endl;

    // Check the Digital Output Expressions
    GenericSignal preflightSignal = GenericSignal( Output );
    for( int i = 0; i < Parameter( "DigitalOutput" )->NumRows(); i++ )
    {
      if( Parameter( "DigitalOutput" )( i, "Instance" ) == "" ) 
        continue;

      DigitalOutputExpression exp;
      exp.inst = Parameter( "DigitalOutput" )( i, "Instance" );
      exp.dout = Parameter( "DigitalOutput" )( i, "Output" );
      exp.exp = Expression( Parameter( "DigitalOutput" )( i, "Expression" ) );
    
      if( exp.inst < 0 || exp.inst >= nInstances )
        bcierr << "Row " << i + 1 << " of DigitalOutput specifies an invalid NSP instance (Hint: These are 0 indexed)" << endl;
      if( exp.dout <= 0 || exp.dout > cbNUM_DIGOUT_CHANS )
        bcierr << "Row " << i + 1 << " of DigitalOutput specifies an invalid Digital Output Channel. "
               << "Valid digital outputs are 1-" << cbNUM_DIGOUT_CHANS << ")." <<  endl;

      int dig_chan = cbFIRST_DIGOUT_CHAN + exp.dout;
      if( CereLinkError( cbSdkSetDigitalOutput( exp.inst, dig_chan, 0 ) ) )
        bcierr << "CBSDK digital output test failed for port " << exp.dout;
      
      bciout << "Evaluating " << Parameter( "DigitalOutput" )( i, "Expression" );
      exp.exp.Evaluate( &preflightSignal );
    }

  } else bcierr << "Could not re-establish connection to cbsdk" << endl;

  Disconnect( nInstances );
  State( "NSPSyncState" );

  // We will place no limits on SampleBlockSize because cbsdk
  // receives individual frames from the NSP anyway.
  int numberOfChannels = Parameter( "SourceCh" );
  int samplesPerBlock  = Parameter( "SampleBlockSize" );
  SignalType sigType = SignalType::int16;
  Output = SignalProperties( numberOfChannels, samplesPerBlock, sigType );
}

void
BlackrockADC::OnInitialize( const SignalProperties& Output )
{
  mSampleGroup = GetRequestedSampleGroup();
  mSampleBlockSize  = Parameter( "SampleBlockSize" );
  mNSPInstances = ( unsigned int )Parameter( "NSPInstances" );
  mDataPacketBuffers.clear();
  mPauseForSync = false;
  State( "NSPSyncState" ) = 1;

  // Populate the Digital Output Expressions
  mDigitalOutputEx.clear();
  for( int i = 0; i < Parameter( "DigitalOutput" )->NumRows(); i++ )
  {
    if( Parameter( "DigitalOutput" )( i, "Instance" ) == "" ) 
      continue;

    DigitalOutputExpression exp;
    exp.inst = Parameter( "DigitalOutput" )( i, "Instance" );
    exp.dout = Parameter( "DigitalOutput" )( i, "Output" );
    exp.exp = Expression( Parameter( "DigitalOutput" )( i, "Expression" ) );
    mDigitalOutputEx.push_back( exp );
  }
}

void
BlackrockADC::OnStartAcquisition()
{
  if( Connect( mNSPInstances ) )
  {
    // Acquire the channel configuration
    if( !GetChannelConfig( mNSPInstances, mSampleGroup, mChannelConfig, mSyncChans ) )
      bcierr << "Couldn't acquire channel configuration" << endl;

    // Determine if synchronization is enabled...
    mSyncEnabled = true;
    for( size_t i = 0; i < mSyncChans.size(); i++ )
      if( mSyncChans[ i ] == -1 )
        mSyncEnabled = false;

    mDataMutex.Acquire();
    for( int instIdx = 0; instIdx < mNSPInstances; instIdx++ )
    {
      // Ready a queue for our data packets
      mDataPacketBuffers.push_back( queue< vector< INT16 > >() );

      // Register data packet callback
      CereLinkError( cbSdkRegisterCallback( instIdx, CBSDKCALLBACK_CONTINUOUS, DataCallback, this ) );
    }
    mDataMutex.Release();
  }
  else 
  {
    // Error and ensure we disconnect from any NSPs we connected to.
    bcierr << "Could not re-establish connection to cbsdk" << endl;
    Disconnect( mNSPInstances );
  }
}

void
BlackrockADC::DoAcquire( GenericSignal& Output )
{
  // Multi-NSP Synchronization Procedure
  if( mPauseForSync )
  {
    // Set the digital output low (is currently high) and collect some data
    bciout << "Starting Multi-NSP Synchronization Procedure....";
    mCommandMutex.Acquire();
    for( int instIdx = 0; instIdx < mNSPInstances; instIdx++ )
      cbSdkSetDigitalOutput( instIdx, NSP_SYNC_OUTPUT_CHANNEL, 0 );
    mCommandMutex.Release();
    ThreadUtils::SleepForMs( 100 ); // Average Latency is about 20 ms

    // Calculate the delays that exist before the two synchronization input steps.
    for( int instIdx = 0; instIdx < mNSPInstances; instIdx++ )
    {
      int delay = 0;
      stringstream debugStream;
      for( int i = 0; i < mDataPacketBuffers[ instIdx ].size(); i++ )
      {
        int value = mDataPacketBuffers[ instIdx ].front()[ mSyncChans[ instIdx ] ];
        debugStream << value << " ";
        if( value < NSP_SYNC_THRESHOLD ) break; // Falling edge
        mDataPacketBuffers[ instIdx ].pop();
        delay++;
      }
      bcidbg << "DEBUG SYNC: Inst " << instIdx << ": " << debugStream.str();
      bciout << "Corrected Instance " << instIdx << " by " << delay << " samples.";
    }

    bciout << "Synchronization Complete.";
    mPauseForSync = false;
  }

  // Wait while there isn't enough data in the queue
  while( true )
  {
    bool dataQueued = true;
    for( int instIdx = 0; instIdx < mNSPInstances; instIdx++ )
      dataQueued &= mDataPacketBuffers[ instIdx ].size() >= mSampleBlockSize;
    if( dataQueued == true ) break;

    ThreadUtils::SleepForMs( 1 );
  }

  // Dequeue data into the signal output
  mDataMutex.Acquire();
  for( int fr_idx = 0; fr_idx < Output.Elements(); fr_idx++ ) 
  {
    for( int ch_idx = 0; ch_idx < Output.Channels(); ch_idx++ )
    {
      int inst = mChannelConfig[ ch_idx ].inst;
      int idx = mChannelConfig[ ch_idx ].idx;
      Output( ch_idx, fr_idx ) = mDataPacketBuffers[ inst ].front()[ idx ];
    }

    // Pop the recorded frame off the buffers
    for( int instIdx = 0; instIdx < mNSPInstances; instIdx++ )
      mDataPacketBuffers[ instIdx ].pop();
  }
  mDataMutex.Release();
}

void
BlackrockADC::StartRun()
{
}

void
BlackrockADC::OnProcess()
{
  // Handle state machine for synchronization procedure
  if( State( "NSPSyncState" ) == 1 )
  {
    mCommandMutex.Acquire();
    for( int instIdx = 0; instIdx < mNSPInstances; instIdx++ )
      cbSdkSetDigitalOutput( instIdx, NSP_SYNC_OUTPUT_CHANNEL, 1 );
    State( "NSPSyncState" ) = 2;
    mCommandMutex.Release();
  } 
  else if( State( "NSPSyncState" ) == 2 )
  {
    mCommandMutex.Acquire();
    if( mSyncEnabled ) mPauseForSync = true;
    else for( int instIdx = 0; instIdx < mNSPInstances; instIdx++ )
      cbSdkSetDigitalOutput( instIdx, NSP_SYNC_OUTPUT_CHANNEL, 0 );
    State( "NSPSyncState" ) = 0;
    mCommandMutex.Release();
  }
}

void
BlackrockADC::Process( const GenericSignal& Input, GenericSignal& Output )
{
  // Call Superclass!
  BufferedADC::Process( Input, Output );

  // Set digital outputs as necessary
  mCommandMutex.Acquire();
  for( size_t i = 0; i < mDigitalOutputEx.size(); i++ )
  {
    DigitalOutputExpression ex = mDigitalOutputEx[ i ];
    int dig_chan = cbFIRST_DIGOUT_CHAN + ex.dout;
    int value = int( bool( ex.exp.Evaluate( &Output ) ) );
    cbSdkSetDigitalOutput( ex.inst, dig_chan, value );
  }
  mCommandMutex.Release();
}

void
BlackrockADC::StopRun()
{
}

void
BlackrockADC::OnStopAcquisition()
{
  Disconnect( mNSPInstances );
}

void
BlackrockADC::OnHalt()
{
}

bool
BlackrockADC::Connect( int nInstances ) const
{
  if( nInstances < 1 ) nInstances = 1;
  for( int instIdx = 0; instIdx < nInstances; instIdx++ )
  {
    // Open a connection to the SDK
    cbSdkConnectionType conType = CBSDKCONNECTION_DEFAULT;
    if( CereLinkError( cbSdkOpen( instIdx, conType, cbSdkConnection() ) ) )
    {
      bcierr << "Could not establish connection to cbsdk for instance " << instIdx << ".  "
             << "CBSDK may not always connect even if your system is set up properly. "
             << "Try to 'ping " << cbNET_UDP_ADDR_CNT << "'" << endl;
      return false;
    }

    // Determine the SDK connection type
    cbSdkInstrumentType instType;
    if( CereLinkError( cbSdkGetType( instIdx, &conType, &instType ) ) )
    {
      bcierr << "Unable to determine connection type for instance " << instIdx << endl;
      return false;
    }

    // Get the NSP Version
    cbSdkVersion ver;
    if( CereLinkError( cbSdkGetVersion( instIdx, &ver ) ) )
    {
      bcierr << "Unable to get NSP version for instance " << instIdx << ".  "
             << "Is device connected and on?" << endl;
      return false;
    }

    // Assemble a debug string for the connection information
    if( conType < 0 || conType > CBSDKCONNECTION_COUNT ) conType = CBSDKCONNECTION_COUNT;
    if( instType < 0 || instType > CBSDKINSTRUMENT_COUNT ) instType = CBSDKINSTRUMENT_COUNT;
    string strConnection[] = { "Default", "Central", "UDP", "Closed", "Unknown" };
    string strInstrument[] = { "NSP", "nPlay", "Local NSP", "Remote nPlay", "Unknown" };
    bciout << "Instance " << instIdx << ": " << strConnection[ conType ] 
           << " real-time interface to " << strInstrument[ instType ]
           << "(V" << ver.nspmajor << "." << ver.nspminor << "." 
           << ver.nsprelease << "." << ver.nspbeta << ")";
  }

  return true;
}

bool
BlackrockADC::GetChannelConfig( int iNumInstances, int iGroup, 
                                vector< ChanInfo > &oChanConfig, 
                                vector< int > &oSyncChans ) const
{ // BlackrockADC::Connect must be called first!

  oChanConfig.clear();
  oSyncChans.clear();

  // Get the total number of channels in this sample group across all instances
  for( int instIdx = 0; instIdx < iNumInstances; instIdx++ )
  {
    int sync_input_ch = -1;

    // Determine the number of channels in the requested channel group on this instance.
    unsigned int nLength = 0;
    CereLinkError( cbSdkGetSampleGroupList( instIdx, 1, iGroup, &nLength, NULL ) );
    if( nLength <= 0 ) continue;

    // Get the list of channels in this sample group
    unsigned int *pGroupList = new unsigned int[ nLength ];
    CereLinkError( cbSdkGetSampleGroupList( instIdx, 1, iGroup, &nLength, pGroupList ) );

    // Populate output structures with information about this channel
    for( unsigned int i = 0; i < nLength; i++ )
    {
      cbPKT_CHANINFO* pChanInfo = new cbPKT_CHANINFO();
      if( !CereLinkError( cbSdkGetChannelConfig( instIdx, pGroupList[i], pChanInfo ) ) )
      {
        ChanInfo newChan;
        newChan.offset = 0;
        newChan.gain = ScalingToGain( pChanInfo->physcalin );
        newChan.label = string( pChanInfo->label );
        newChan.inst = instIdx;
        newChan.idx = i;
        oChanConfig.push_back( newChan );
      } else bcierr << "Bad Channel Index: " << pGroupList[i] << endl << gPktErrMsg;
      delete pChanInfo;

      // Find the Synchronization input channel index
      if( pGroupList[i] == NSP_SYNC_INPUT_CHANNEL ) sync_input_ch = i;
    }
    delete [] pGroupList;
    oSyncChans.push_back( sync_input_ch );
  }
  
  // Quick sanity check
  if( oChanConfig.size() == 0 )
  {
    bcierr << "There are no channels sampled at the desired SamplingRate." << endl;
    return false;
  }

  return true;
}

void
BlackrockADC::Disconnect( int nInstances ) const
{
  if( nInstances < 1 ) nInstances = 1;
  for( int instIdx = 0; instIdx < nInstances; instIdx++ )
    // Disconnect from the instrument
    CereLinkError( cbSdkClose( instIdx ) );
}

void BlackrockADC::DataCallback( UINT32 iInstance, const cbSdkPktType iType, const void* iData, void* iBlackrockADC )
{
  // Re-establish communication to the class
  BlackrockADC* bradc = reinterpret_cast< BlackrockADC* >( iBlackrockADC );

  switch( iType )
  {
  case cbSdkPkt_PACKETLOST:
    bcierr << "Packet loss.  Data has been lost.  Reduce system load." << endl;
    break;
  case cbSdkPkt_CONTINUOUS:
    if( bradc && iData )
    {
      // Grab the packet and ensure that it is the right sample group
      const cbPKT_GROUP *pPkt = reinterpret_cast< const cbPKT_GROUP* >( iData );
      if( pPkt->type != bradc->mSampleGroup ) break;

      // We'll just queue the data packet
      bradc->mDataMutex.Acquire();
      size_t pkt_len = sizeof( pPkt->data ) / sizeof( pPkt->data[0] );
      bradc->mDataPacketBuffers[ iInstance ].push( vector< INT16 >( pPkt->data, pPkt->data + pkt_len ) );
      bradc->mDataMutex.Release();
    }
    break;
  default:
    break;
  }
  return;
}

// Check the SamplingRate parameter and return a Blackrock group index
// If this returns 0, the sampling rate is not a valid rate.
int
BlackrockADC::GetRequestedSampleGroup() const
{
  int samplingRate = ( int )Parameter( "SamplingRate" );
  int num_rates = sizeof( gGroupRates ) / sizeof( int );
  for( int rate_idx = 0; rate_idx < num_rates; rate_idx++ )
    if( samplingRate == gGroupRates[ rate_idx ] )
      return rate_idx;
  return 0;
}

// Convert a cbSCALING parameter to a SourceChGain
double
BlackrockADC::ScalingToGain( cbSCALING scaling ) const
{
  double anaRange = scaling.anamax - scaling.anamin;
  double digRange = scaling.digmax - scaling.digmin;
  return ( anaRange / digRange );
}

// Debug channel configurations
void
BlackrockADC::OutputChannelDebugInfo( int nInstances ) const
{
  for( unsigned int instIdx = 0; instIdx < mNSPInstances; instIdx++ )
  {
    for( unsigned int ch = 0; ch < cbMAXCHANS; ch++ )
    {
      cbPKT_CHANINFO* pChanInfo = new cbPKT_CHANINFO();
      if( !CereLinkError( cbSdkGetChannelConfig( instIdx, ch, pChanInfo ) ) )
        bciout << pChanInfo->label << " -- "
               << "Inst: " << instIdx  << ", "
               << "Bank: " << pChanInfo->bank << ", "
               << "Chan: " << pChanInfo->chan;
      delete pChanInfo;
    }
  }
}

// Misc Cerelink error checking
bool
BlackrockADC::CereLinkError( cbSdkResult res )
{
  cbSdkInstrumentType instType;
  switch( res )
  {
  case CBSDKRESULT_WARNCONVERT:
    bciwarn << "File conversion is needed..." << endl;
    return false;
  case CBSDKRESULT_WARNCLOSED:
    bciwarn << "Library is already closed" << endl;
    return false;
  case CBSDKRESULT_WARNOPEN:
    bciwarn << "Library is already opened" << endl;
    return false;
  case CBSDKRESULT_SUCCESS:
    //bcidbg( 0 ) << "Success" << endl;
    return false;
  case CBSDKRESULT_NOTIMPLEMENTED:
    bcierr << "Not implemented" << endl;
    return true;
  case CBSDKRESULT_INVALIDPARAM:
    bcierr << "Invalid parameter" << endl;
    return true;
  case CBSDKRESULT_CLOSED:
    bcierr << "Interface is closed cannot do this operation" << endl;
    return true;
  case CBSDKRESULT_OPEN:
    bcierr << "Interface is open cannot do this operation" << endl;
    return true;
  case CBSDKRESULT_NULLPTR:
    bcierr << "Null pointer" << endl;
    return true;
  case CBSDKRESULT_ERROPENCENTRAL:
    bcierr << "Unable to open Central interface" << endl;
    return true;
  case CBSDKRESULT_ERROPENUDP:
    bcierr << "Unable to open UDP interface (might happen if default)" << endl;
    return true;
  case CBSDKRESULT_ERROPENUDPPORT:
    bcierr << "Unable to open UDP port" << endl;
    return true;
  case CBSDKRESULT_ERRMEMORYTRIAL:
    bcierr << "Unable to allocate RAM for trial cache data" << endl;
    return true;
  case CBSDKRESULT_ERROPENUDPTHREAD:
    bcierr << "Unable to open UDP timer thread" << endl;
    return true;
  case CBSDKRESULT_ERROPENCENTRALTHREAD:
    bcierr << "Unable to open Central communication thread" << endl;
    return true;
  case CBSDKRESULT_INVALIDCHANNEL:
    bcierr << "Invalid channel number" << endl;
    return true;
  case CBSDKRESULT_INVALIDCOMMENT:
    bcierr << "Comment too long or invalid" << endl;
    return true;
  case CBSDKRESULT_INVALIDFILENAME:
    bcierr << "Filename too long or invalid" << endl;
    return true;
  case CBSDKRESULT_INVALIDCALLBACKTYPE:
    bcierr << "Invalid callback type" << endl;
    return true;
  case CBSDKRESULT_CALLBACKREGFAILED:
    bcierr << "Callback register/unregister failed" << endl;
    return true;
  case CBSDKRESULT_ERRCONFIG:
    bcierr << "Trying to run an unconfigured method" << endl;
    return true;
  case CBSDKRESULT_INVALIDTRACKABLE:
    bcierr << "Invalid trackable id, or trackable not present" << endl;
    return true;
  case CBSDKRESULT_INVALIDVIDEOSRC:
    bcierr << "Invalid video source id, or video source not present" << endl;
    return true;
  case CBSDKRESULT_ERROPENFILE:
    bcierr << "Cannot open file" << endl;
    return true;
  case CBSDKRESULT_ERRFORMATFILE:
    bcierr << "Wrong file format" << endl;
    return true;
  case CBSDKRESULT_OPTERRUDP:
    bcierr << "Socket option error (Possibly permission issue)" << endl;
    return true;
  case CBSDKRESULT_MEMERRUDP:
    bcierr << "Socket memory assignment error" << endl
           << " Consider using sysctl -w net.core.rmem_max=8388608" << endl
           << " or sysctl -w kern.ipc.maxsockbuf=8388608" << endl;
    return true;
  case CBSDKRESULT_INVALIDINST:
    bcierr << "Invalid range or instrument address" << endl;
    return true;
  case CBSDKRESULT_ERRMEMORY:
    bcierr << "Library memory allocation error" << endl;
    return true;
  case CBSDKRESULT_ERRINIT:
    bcierr << "Library initialization error" << endl;
    return true;
  case CBSDKRESULT_TIMEOUT:
    bcierr << "Connection timeout error" << endl;
    return true;
  case CBSDKRESULT_BUSY:
    bcierr << "Resource is busy" << endl;
    return true;
  case CBSDKRESULT_ERROFFLINE:
    bcierr << "Instrument is offline" << endl;
    return true;
  case CBSDKRESULT_UNKNOWN:
  default:
    bcierr << "Unknown error.  Sorry!" << endl;
    return true;
  }
}

