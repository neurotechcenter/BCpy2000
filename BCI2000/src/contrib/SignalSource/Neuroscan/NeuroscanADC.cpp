////////////////////////////////////////////////////////////////////////////////
// $Id: NeuroscanADC.cpp 4970 2015-08-21 16:46:07Z mellinger $
// Authors: schalk@wadsworth.org, juergen.mellinger@uni-tuebingen.de
// Description: A source module connecting to a Neuroscan Acquire server.
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

#include "NeuroscanADC.h"
#include "EDFHeader.h"
#include "Blob.h"
#include "ThreadUtils.h"
#include "StringUtils.h"
#include "BCIException.h"
#include "BCIStream.h"

using namespace std;

static const int cTimeout = 2000; // ms

RegisterFilter( NeuroscanADC, 1 );

NeuroscanADC::NeuroscanADC()
: mEventChannels( 0 )
{
}

NeuroscanADC::~NeuroscanADC()
{
  Halt();
}

void
NeuroscanADC::Publish()
{
 BEGIN_PARAMETER_DEFINITIONS
   "Source string ServerAddress= localhost:3998 % % % "
       "// IP address and port of the Neuroscan Acquire server",
   "Source blob AcquisitionSetupFile= % % % % "
       "// auxiliary acquisition information (noedit)",

   "Source int SampleBlockSize= auto",
   "Source float SamplingRate= auto",

   "Source int SourceCh= auto",
   "Source list SourceChOffset= 1 auto",
   "Source list SourceChGain= 1 auto",
   "Source stringlist ChannelNames= 1 auto",
 END_PARAMETER_DEFINITIONS

 BEGIN_STATE_DEFINITIONS
   "NeuroscanEvent1 8 0 0 0",
 END_STATE_DEFINITIONS
}

void
NeuroscanADC::AutoConfig( const SignalProperties& )
{
  try
  { // Connect to the server and gather basic info to configure BCI2000 parameter settings
    string address = Parameter( "ServerAddress" );
    mSocket.open( address.c_str() );
    mServer.open( mSocket );
    if( !mServer.is_open() )
      throw bcierr
         << "Could not connect to server at " << address << ". "
         << "Make sure Acquire is running and the server is enabled at the correct port.";

    // Basic info
    NscInfoRequest().Serialize( mServer );
    AwaitResponse();
    NscPacketHeader header;
    bool done = false;
    while( mServer && mServer.is_open() && !done )
    {
      header.Unserialize( mServer );
      done = (
        header.Id() == HeaderIdData
        && header.Code() == DataType_InfoBlock
        && header.Value() == InfoType_BasicInfo
      );
      if( mServer && !done )
        mServer.ignore( header.DataSize() );
    }
    if( mServer && done )
      mAcqSettings.Unserialize( mServer );
    if( !done || !mServer )
      throw bciexception << "Did not receive BasicInfo block";

    mEventChannels = mAcqSettings.EventChannels();
    Parameter( "SamplingRate" ) << mAcqSettings.SamplingRate() << "Hz";
    Parameter( "SampleBlockSize" ) = mAcqSettings.SamplesInBlock();
    Parameter( "SourceCh" ) = mAcqSettings.EEGChannels();
    Parameter( "SourceChOffset" )->SetNumValues( mAcqSettings.EEGChannels() );
    Parameter( "SourceChGain" )->SetNumValues( mAcqSettings.EEGChannels() );
    for( int ch = 0; ch < mAcqSettings.EEGChannels(); ++ch )
    {
      Parameter( "SourceChOffset" )( ch ) = 0;
      Parameter( "SourceChGain" )( ch ) << fixed << mAcqSettings.Resolution() << "muV";
    }
    // EDF header
    NscEDFHeaderRequest().Serialize( mServer );
    AwaitResponse();
    header.Unserialize( mServer );
    if( header.Id() != HeaderIdData
        || header.Code() != DataType_InfoBlock
        || header.Value() != InfoType_EdfHeader )
    {
      mServer.ignore( header.DataSize() );
      bciwarn << "Server " << address << " does not provide EDF header information";
    }
    else
    {
      EDFHeader h;
      h.Unserialize( mServer );
      if( h.Channels.size() != mAcqSettings.EEGChannels() )
      {
        bciwarn << "Ignoring inconsistent EDF header";
      }
      else
      {
        for( size_t i = 0; i < h.Channels.size(); ++i )
        {
          const EDFHeader::ChannelInfo& ch = h.Channels[i];
          bool good = false;
          double range = ch.DigitalMaximum - ch.DigitalMinimum,
                 gain = 0,
                 offset = 0;
          if( range != 0 )
          {
            gain = ( ch.PhysicalMaximum - ch.PhysicalMinimum ) / range;
            if( gain != 0 )
            {
              offset = ch.PhysicalMaximum / gain - ch.DigitalMaximum;
              offset = ::floor( offset + 0.5 ); // EDF implies integer data
            }
          }
          if( gain != 0 )
          {
            Parameter( "SourceChGain" )( i ) << fixed << gain << StringUtils::Strip( ch.PhysicalDimension );
            Parameter( "SourceChOffset" )( i ) = offset;
          }
        }
        Parameter( "ChannelNames" )->SetNumValues( h.Channels.size() );
        for( size_t i = 0; i < h.Channels.size(); ++i )
          Parameter( "ChannelNames" )( i ) = StringUtils::Strip( h.Channels[i].Label );
      }
    }
    // Setup file
    NscSetupFileRequest().Serialize( mServer );
    AwaitResponse();
    header.Unserialize( mServer );
    if( header.Id() != HeaderIdFile
        || header.Code() != SetupFile )
    {
      mServer.ignore( header.DataSize() );
      bciwarn << "Server " << address << " does not provide setup file information";
    }
    else
    {
      string format = "unknown";
      switch( header.Value() )
      {
        case NeuroscanASTFormat:
          format = "ast";
          break;
        case CtfDSFormat: // BCI2000-specific extension
          format = "ds.tgz";
          break;
        default:
          bciwarn << "Unknown setup file format ID: " << header.Value();
      }
      char* pData = new char[header.DataSize()];
      mServer.read( pData, header.DataSize() );
      Parameter( "AcquisitionSetupFile" ) << Blob( pData, header.DataSize(), format );
    }
  }
  catch( const bci::Exception& e )
  {
    bcierr << e.What();
  }
}

void
NeuroscanADC::Preflight( const SignalProperties&, SignalProperties& Output ) const
{
  PreflightCondition( Parameter( "SourceCh" ) == mAcqSettings.EEGChannels() );
  PreflightCondition( Parameter( "SampleBlockSize" ) == mAcqSettings.SamplesInBlock() );
  PreflightCondition( Parameter( "SamplingRate" ).InHertz() == mAcqSettings.SamplingRate() );
  ParamRef SourceChGain = Parameter( "SourceChGain" ),
           SourceChOffset = Parameter( "SourceChOffset" );
  for( int ch = 0; ch < Parameter( "SourceCh" )->NumValues(); ++ch )
  {
    PreflightCondition( SourceChOffset( ch ) == 0 );
    double gain = SourceChGain( ch );
    if( fabs( gain - mAcqSettings.Resolution() ) > 1e-3 )
      bcierr << "SourceChGain(" << SourceChGain->RowLabels()[ch] << ") == "
             << gain << " != " << mAcqSettings.Resolution();
  }

  SignalType outType;
  switch( mAcqSettings.DataDepth() )
  {
    case 2:
      outType = SignalType::int16;
      break;
    case 4:
      outType = SignalType::int32;
      break;
    default:
      bcierr << "Server reports unsupported data size "
             << "(" << mAcqSettings.DataDepth() * 8 << " bits per sample)";
  }
  Output = SignalProperties( Parameter( "SourceCh" ), Parameter( "SampleBlockSize" ), outType );
}

void
NeuroscanADC::Initialize( const SignalProperties&, const SignalProperties& )
{
  NscStartAcquisition().Serialize( mServer );
  NscStartDataRequest().Serialize( mServer );
  mServer.flush();
}

void
NeuroscanADC::Process( const GenericSignal&, GenericSignal& Output )
{
  NscPacketHeader header;
  while( mServer && ( header.Id() != HeaderIdData || header.Code() != DataType_EegData ) )
  {
    mServer.ignore( header.DataSize() );
    header.Unserialize( mServer );
  }
  if( mServer )
    switch( header.Value() )
    {
      case DataTypeRaw16bit:
        ReadData<int16_t>( Output );
        break;
      case DataTypeRaw32bit:
        ReadData<int32_t>( Output );
        break;
      default:
        bcierr << "Unknown data format" << endl;
    }
  if( !mServer )
    bcierr << "Lost connection to Acquire server" << endl;
}

void
NeuroscanADC::Halt()
{
  if( mServer.is_open() )
  {
    NscCloseRequest().Serialize( mServer ).flush();
    mSocket.wait_for_read( cTimeout );
    mServer.close();
  }
  mSocket.close();
  mServer.clear();
}

void
NeuroscanADC::AwaitResponse()
{
  mServer.flush();
  if( !mServer || !mServer.is_open() )
    throw bcierr << "Lost connection to Acquire server";
  if( !mSocket.wait_for_read( cTimeout ) )
    throw bcierr
      << "Did not receive response from Acquire server after "
      << cTimeout / 1e3 << "s of waiting.";
}

template<typename T>
void
NeuroscanADC::ReadData( GenericSignal& Output )
{
  for( int sample = 0; sample < Output.Elements(); ++sample )
  {
    T value;
    for( int channel = 0; channel < Output.Channels(); ++channel )
    {
      LittleEndianData::get( mServer, value );
      Output( channel, sample ) = value;
    }
    if( mEventChannels > 0 )
    {
      LittleEndianData::get( mServer, value );
      State( "NeuroscanEvent1" )( sample ) = value & 0xff;
      mServer.ignore( sizeof( T ) * ( mEventChannels - 1 ) );
    }
  }
}
