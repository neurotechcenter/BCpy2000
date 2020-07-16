////////////////////////////////////////////////////////////////////////////////
// $Id: GrapeVineADC.cpp 5620 2017-02-16 22:48:28Z gmilsap $
// Author: griffin.milsap@gmail.com
// Description: ADC class to relay xipp packets from the Grapevine
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
#include "BCIStream.h"

#include "GrapeVineADC.h"

using namespace std;

// Register the source class with the framework.
RegisterFilter( GrapeVineADC, 1 );

GrapeVineADC::GrapeVineADC()
: mInput( &mStreambuf )
{
  // For this module, I'm using BCI2000's impl. of sockets/UDP
  // This means I don't need to link against any external libraries
  // And the build/deployment is cross-platform.  Win-win.
  mStreambuf.SetInput( &mInputSocket );

  // This is where BCI2000 parameters are stipulated.
  // It's probably best not to change these too much
  // The module should ideally autoconfigure itself
  // to match the Trellis configuration

  // We need one parameter to pick the data stream to listen to
  // All source modules must declare the parameters specified here
  BEGIN_PARAMETER_DEFINITIONS

    "Source:Signal%20Properties float SamplingRate= 1000Hz "
       "10000Hz 500 2000 // sample rate",

    // Although you CAN specify these, it'd be best if you let the module
    // autoconfigure them individually based on the SamplingRate.
    "Source auto SourceCh= auto ", // The number of channels to acquire
    "Source auto SampleBlockSize= auto ", // The number of samples to acquire for a "block"
    "Source auto ChannelNames= auto ", // The names of the channels

    // Signal in muV = ( raw value - SourceChOffset ) * SourceChGain
    // If possible, we would prefer to save the raw ADC values, then 
    // Populate these parameters with conversion values -- for data compression's sake
    "Source auto SourceChOffset= auto ",
    "Source auto SourceChGain= auto ",

  END_PARAMETER_DEFINITIONS
    
}

GrapeVineADC::~GrapeVineADC()
{

}

// AutoConfig is called once the SetConfig button is pressed.
void
GrapeVineADC::OnAutoConfig()
{
  bciout << "Entering AutoConfig!";
  int samplingRate = Parameter( "SamplingRate" );
  // Attempt autoconfiguration
  if( Connect() ) {
    // Resize signal source parameters accordingly
    // NOTE: IDEALLY we would send query packets to the amplifier 
    // and receive configuration information.  I'm just spitballing here
    size_t numChans = 32;
    Parameter( "SampleBlockSize" ) = 50;
    Parameter( "SourceCh" ) = numChans;
    Parameter( "SourceChOffset" )->SetNumValues( numChans );
    Parameter( "SourceChGain" )->SetNumValues( numChans );
    Parameter( "ChannelNames" )->SetNumValues( numChans );

    // Populate channel information
    // NOTE: IDEALLY we could get channel names too.  It seems trellis 
    // pretty actively discourages setting channel names :(
    for( unsigned int i = 0; i < numChans; i++ )
    {
      Parameter( "SourceChOffset" )( i ) = 0.0;
      Parameter( "SourceChGain" )( i ) = 1.0;
      Parameter( "ChannelNames" )( i ) = String() << i + 1;
    }
  } else bcierr << "Could not establish connection to Instrument Network" << endl;

  Disconnect();
}

// OnPreflight is called immediately after OnAutoConfig.
// * It exists to check parameters for errors.
// * Throwing a bcierr here displays helpful error messages.
// * After OnPreflight is finished, we know this module is
//   parameterized without errors and ***WILL RUN WITHOUT FURTHER ERRORS***
// * It's a const function because you shouldn't do anything here that modifies
//   the object -- JUST check it's parameterization for errors.
// * You might need to query some information from the API in order to do this.
void
GrapeVineADC::OnPreflight( SignalProperties& Output ) const
{
  // NOTE: This is where you check parameters for errors
  // for examples on how to do this, please see the Blackrock module
}

// OnInitialize is called immediately after OnPreflight.
// * OnInitialize exists to save parameters into member variables
// * Throwing a bcierr here CRASHES the acquisition module (on purpose)
//   you absolutely cannot have an error after this point, or 
//   BCI2000 assumes something really bad has happened that will
//   comprimise the dataset.
// * After this function is called, a NEW ACQUISITION THREAD is created
//   which immediately calls OnStartAcquisition.  The system is put into
//   Initialized state, which means it's acquiring from the amplifier
//   and will continue to do so until OnHalt() is called.  We won't be 
//   processing and/or recording the data until StartRun() is called.
void
GrapeVineADC::OnInitialize( const SignalProperties& Output )
{
  // Parameter access is expensive -- let's save some parameters
  // that we would otherwise need once per 'Process' into member variables
  mSourceCh = Parameter( "SourceCh" );
  mSamplingRate = Parameter( "SamplingRate" );
  mSampleBlockSize = Parameter( "SampleBlockSize" );
}

void
GrapeVineADC::StartRun()
{
  // Start button has just been pushed.
  // Usually don't need to do much here.
  // Recording and processing is handled automatically.
}


// The acquisition thread looks kinda like this:
// {
//    OnStartAcquisition();
//    While( !Terminating() ) {
//      GenericSignal Output;
//      DoAcquire( Output );
//      Save/ProcessDataBlock( Output );
//    }
//    OnStopAcquisition();
//  }


// As such, OnStartAcquisition is called when the 
// acquisition thread is just started.  You need to do everything
// here necessary to connect to the API and prepare for 
// streaming the data in *this* thread.
void
GrapeVineADC::OnStartAcquisition()
{
  if( !Connect() )
    bcierr << "Could not re-connect to Instrument Network";
}

const Time::Interval cReactionTime = Time::Seconds( 0.1 );

// DoAcquire is called as fast as possible.  As soon as
// one DoAcquire() returns a data block, it's called immediately again.
// It's this function's purpose to fill one GenericSignal 
// (Channels (SourceCh) X Time (SampleBlockSize))
// without occupying 100% CPU busywaiting.
// The strategy here is to rely upon the InputSocket.Wait()
// method that uses a messaging pattern to block execution until
// new data is available at the socket, then parse the packet and
// fill the data block.
void
GrapeVineADC::DoAcquire( GenericSignal& Output )
{
  Time now = TimeUtils::MonotonicTime();

  int samp = 0;
  while( samp < mSampleBlockSize )
  {
    if( mInputSocket.Wait( cReactionTime ) )
    {
      while( mInput.rdbuf()->in_avail() )
      {
        // If we haven't filled the sampleblock after a second, something bad has happened
        if( TimeUtils::MonotonicTime() - now > Time::OneSecond ) {
          bciout << "There was an issue collecting data from the Grapevine NSP.";
          mInput.clear();
          mInput.ignore( std::numeric_limits<std::streamsize>::max() );
          break;
        }

        mInput >> mCurPacket;
        //uint32_t time = mCurPacket.GetHeader().time;
        //bciout << int( mCurPacket.GetHeader().size ) << ' ' 
        //       << int( mCurPacket.GetHeader().processor ) << ' '
        //       << int( mCurPacket.GetHeader().module ) << ' '
        //       << int( mCurPacket.GetHeader().stream ) << ' '
        //       << unsigned int( time );
        if( mCurPacket.GetHeader().processor == 1 && mCurPacket.GetHeader().module != 0 )
        {
          if( mCurPacket.GetHeader().stream == 1 ) // 0 means the packet is config, 1 means the packet is "LFP 1k", 3 means the packet is "hires 2k"
          {
            uint16_t streamType = mCurPacket.as< XippDataPacket >()->streamType;
            //XippPacketFormat *pkt = mCurPacket.as< XippPacketFormat >();
            //bciout << int( pkt->payload[0] ) << " " 
            //       << int( pkt->payload[1] ) << " "
            //       << int( pkt->payload[2] ) << " "
            //       << int( pkt->payload[3] ) << " "
            //       << int( pkt->payload[4] ) << " ";
            //bciout << int( streamType );
            if( streamType == XIPP_STREAM_CONTINUOUS )
            {
              if( mCurPacket.GetHeader().module != 33 )
              {
                for( int ch = 0; ch < mSourceCh; ch++ )
                  Output( ch, samp ) = ( int16_t )( mCurPacket.as< XippContinousDataPacket >()->samples[ch] );
                samp++;
              } else {
                // Apparently this is analog input data.. ?
              }
            }
          } if( samp == mSampleBlockSize ) break;
        }
      }
    }

    // If we haven't filled the sampleblock after a second, something bad has happened
    if( TimeUtils::MonotonicTime() - now > Time::OneSecond ) {
      bciout << "There was an issue collecting data from the Grapevine NSP.";
      mInput.clear();
      mInput.ignore( std::numeric_limits<std::streamsize>::max() );
      break;
    }
  }

  // Remember that throwing bcierr after OnPreflight crashes the executable?
  // Well that comes in handy sometimes if you're inspecting Log output without
  // wanting to flood the log with multiple repetitions of DoAcquire.
  //bcierr << "DEBUGSTOP";
}

// This is called when the Acquisition thread is shutting down.
// Here, we need to shut down any communication with the API
void
GrapeVineADC::OnStopAcquisition()
{
  Disconnect();
}

void
GrapeVineADC::StopRun()
{
  // Stop run button was clicked, we are no longer in 'running state'
}

void
GrapeVineADC::OnHalt()
{
  // System is being re-configured and acquisition is stopping.
}

bool
GrapeVineADC::Connect()
{
  // So... BCI2000's default UDP socket impl. doesn't have
  // any means of supporting non-exclusive access to a port.
  // This means that as of right now, you cannot have trellis
  // and this module running at the same time.
  // It's totally possible to add that functionality to the socket code
  // but I'd defer to Juergen Mellinger (Project lead) on how best to do that.
  mInputSocket.Open( "*", XIPP_NET_DACAR_PORT );
  return mInputSocket.IsOpen();
}

void
GrapeVineADC::Disconnect()
{
  mInputSocket.Close();
}
