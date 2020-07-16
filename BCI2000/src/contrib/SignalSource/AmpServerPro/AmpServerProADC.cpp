////////////////////////////////////////////////////////////////////////////////
// $Id: AmpServerProADC.cpp 5335 2016-04-27 19:21:38Z mellinger $
// Description: A source module that interfaces withEGI's Amp Server Pro SDK.
// Authors: Joshua Fialkoff, Robert Bell (EGI),
//          juergen.mellinger@uni-tuebingen.de
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
#include "AmpServerProADC.h"

#include "BinaryData.h"
#include "BCIStream.h"

using namespace std;

RegisterFilter( AmpServerProADC, 1 );

namespace {

class ResponseNode
{
 public:
  typedef list<ResponseNode> List;

  ResponseNode()
    {}
  explicit ResponseNode( istream& is )
    { Parse( is ); }
  const string& Name() const
    { return mName; }
  const string& Value() const
    { return mValue; }
  List Leaves() const;
  istream& Parse( istream& );
  ostream& Print( ostream& ) const;
 private:
  string mName, mValue;
  List mChildren;
};

ResponseNode::List
ResponseNode::Leaves() const
{
  List result;
  if( mChildren.empty() )
    result.push_back( *this );
  else for( List::const_iterator i = mChildren.begin(); i != mChildren.end(); ++i )
  {
    List leaves = i->Leaves();
    result.insert( result.end(), leaves.begin(), leaves.end() );
  }
  return result;
}

istream&
ResponseNode::Parse( istream& is )
{
  if( (is >> ws).peek() != '(' )
    is.setstate( ios::failbit );
  is.ignore();
  is >> mName;
  while( (is >> ws).peek() == '(' )
    mChildren.push_back( ResponseNode( is ) );
  if( mChildren.empty() )
    getline( is, mValue, ')' );
  else
  {
    if( (is >> ws).peek() != ')' )
      is.setstate( ios::failbit );
    is.ignore();
  }
  return is;
}

ostream&
ResponseNode::Print( ostream& os ) const
{
  os << "( " << mName;
  if( !mValue.empty() )
    os << "=" << mValue;
  for( List::const_iterator i = mChildren.begin(); i != mChildren.end(); ++i )
    i->Print( os << " " );
  return os << " )";
}

ostream& operator<<( ostream& os, const ResponseNode& r )
{ return r.Print( os ); }

} // namespace

AmpServerProADC::AmpServerProADC()
{
}

AmpServerProADC::~AmpServerProADC()
{
  Halt();
}

void
AmpServerProADC::Publish()
{
 BEGIN_PARAMETER_DEFINITIONS
  "Source int SourceCh= auto auto % % ",
  "Source list SourceChOffset= 1 auto % % % % ",
  "Source list SourceChGain= 1 auto % % % % ",
  "Source int SampleBlockSize= 40 40 1 % ",
  "Source int SamplingRate= auto auto % % ",
  "Source string ServerIP= localhost localhost % % // IP address of the machine running AmpServerPro",
  "Source int CommandPort= auto auto 1 65535 // port number corresponding to the command layer",
  "Source int NotificationPort= auto auto 1 65535 // port number corresponding to the notification layer",
  "Source int StreamPort= auto auto 1 65535 // port number corresponding to the stream layer",
  "Source string AmplifierID=  auto auto 0 % // the ID of the Amplifier from which data should be collected",
  "Source int AmpState= 0 0 0 1 // set amp state to: 0 DefaultAcquisitionState, 1 DefaultSignalGeneration (enumeration)",
 END_PARAMETER_DEFINITIONS
}

void
AmpServerProADC::OnAutoConfig()
{
  Parameter( "CommandPort" ) = 9877;
  Parameter( "AmplifierID" ) = 0;

  int ampID = ActualParameter( "AmplifierID" );
  mConnection.Open(
    ActualParameter( "ServerIP" ),
    ActualParameter( "CommandPort" ),
    ampID
  );
  if( mConnection.IsOpen() )
  {
    if( mConnection.Amps() < 1 )
      bcierr << "No amplifiers available";
    else if( ampID >= mConnection.Amps() || ampID < 0 )
      bcierr << "Invalid amplifier ID: " << ampID;

    Parameter( "NotificationPort" ) = mConnection.NotificationPort();
    Parameter( "StreamPort" ) = mConnection.StreamPort();

    Parameter( "SourceCh" ) = mConnection.Channels();
    Parameter( "SourceChOffset" )->SetNumValues( mConnection.Channels() );
    Parameter( "SourceChGain" )->SetNumValues( mConnection.Channels() );
    for( int ch = 0; ch < mConnection.Channels(); ++ch )
    {
      Parameter( "SourceChOffset" )( ch ) = 0;
      Parameter( "SourceChGain" )( ch ) << mConnection.Gain_muV() << "muV";
    }
    Parameter( "SamplingRate" ) << mConnection.SamplingRate() << "Hz";
    Parameter( "SampleBlockSize" ) = mConnection.SampleBlockSize();
  }
  else
  {
    bcierr << "Could not connect to AmpServerPro at " << mConnection.Address()
           << ", " << mConnection.NotificationPort()
           << ", or " << mConnection.StreamPort();
    Parameter( "SamplingRate" ) = 1;
    Parameter( "SampleBlockSize" ) = 1;
  }
}

void
AmpServerProADC::OnPreflight( SignalProperties& Output ) const
{
  const Connection& AmpServer = mConnection;
  PreflightCondition( Parameter( "SourceCh" ) <= AmpServer.Channels() );
  PreflightCondition( Parameter( "SamplingRate" ).InHertz() == AmpServer.SamplingRate() );
  for( int ch = 0; ch < Parameter( "SourceCh" ); ++ch )
  {
    PreflightCondition( Parameter( "SourceChOffset" )( ch ) == 0 );
    double diff = Parameter( "SourceChGain" )( ch ).InMicrovolts() - AmpServer.Gain_muV();
    if( ::fabs( diff ) > 1e-3 )
      bcierr << "Wrong SourceChGain: differs by more than 1/1000";
  }
  Output.SetType( SignalType::float32 );
}

void
AmpServerProADC::OnInitialize( const SignalProperties& )
{
  mConnection.SendCommand( "SetPower 1" );
}

void
AmpServerProADC::OnStartAcquisition()
{
  if( !mConnection.SendCommand( "Start" ) )
  {
    bcierr << "Unable to start the amplifier";
    return;
  }
  // Always set the amp to the default acquisition state first.
  mConnection.SendCommand( "DefaultAcquisitionState" );
  // Now send the desired start state (if not the already set default acquisition state).
  switch( int( Parameter( "AmpState" ) ) )
  {
    case 0:
      break;
    case 1:
      mConnection.SendCommand( "DefaultSignalGeneration" );
      break;
  }
  if( !mConnection.StartStreaming() )
    bcierr << "Unable to start data streaming";
}


void
AmpServerProADC::OnStopAcquisition()
{
  if( !mConnection.StopStreaming() )
    bcierr << "Unable to stop data streaming";
  else
    mConnection.SendCommand( "Stop" );
}

void
AmpServerProADC::DoAcquire( GenericSignal& Output )
{
  if( !mConnection.ReadData( Output ) )
    throw bcierr << mConnection.Error();
}

AmpServerProADC::Connection::Connection()
: mSamplesInStream( 0 ),
  mSamplesInOutput( 0 ),
  mFirstRead( false ),
  mNotificationPort( 0 ),
  mStreamPort( 0 ),
  mTimeout( 0 ),
  mInitialTimeout( 0 ),
  mAmpId( 0 ),
  mAmps( 0 ),
  mChannels( 0 ),
  mSampleBlockSize( 0 ),
  mGain_muV( 0 ),
  mSamplingRate( 0 )
{
}

AmpServerProADC::Connection::~Connection()
{
  Close();
}

bool
AmpServerProADC::Connection::Open( const std::string& inIP, int inPort, int inAmpId )
{
  Close();

  mInitialTimeout = 3000;
  mTimeout = 1000;

  ostringstream oss;
  oss << inIP << ":" << inPort;
  mAddress = oss.str();
  mCommandSocket.open( mAddress );
  mCommands.open( mCommandSocket );

  mNotificationPort = 9878;
  mNotificationSocket.open( inIP, mNotificationPort );

  mStreamPort = 9879;
  mStreamSocket.open( inIP, mStreamPort );
  mStream.open( mStreamSocket );

  mAmps = 0;
  string resultName, resultValue;
  if( SendCommand( "NumberOfAmps", resultName, resultValue ) )
    mAmps = ::atoi( resultValue.c_str() );

  mAmpId = inAmpId;

  mChannels = 280;
  mSampleBlockSize = 40;
  mSamplingRate = 1000;
  // EGI NA300 amps have a range of +/- 200,000uV that is digitized into
  // a 24bit signed value. The values received in BCI2000 are in float,
  // but use these values when converting back to uV.ss
  mGain_muV = 2e5 / ( 1 << 23 );

  return IsOpen();
}

void
AmpServerProADC::Connection::Close()
{
  mStreamSocket.close();
  mStream.close();
  mStream.clear();
  mNotificationSocket.close();
  mNotifications.close();
  mNotifications.clear();
  mCommandSocket.close();
  mCommands.close();
  mCommands.clear();
}

bool
AmpServerProADC::Connection::IsOpen() const
{
  return mCommands.is_open() && mStream.is_open();
}

string
AmpServerProADC::Connection::BuildCommand( const string& inCommand ) const
{
  string name, val1, val2;
  istringstream iss( inCommand );
  if( !(iss >> name) )
    return false;
  if( !(iss >> val1) )
    val1 = "";
  if( !(iss >> val2) )
  {
    val2 = val1;
    val1 = "";
  }
  if( val1.empty() )
    val1 = "0";
  if( val2.empty() )
    val2 = "0";

  ostringstream ampId;
  ampId << mAmpId;
  string result = "(sendCommand cmd_" + name + " " + ampId.str() + " " + val1 + " " + val2 + ")";
  bcidbg << result;
  return result;
}

bool
AmpServerProADC::Connection::SendCommand( const string& inCommand, string& outResultName, string& outResultValue )
{
  if( !( mCommands << BuildCommand( inCommand ) << endl ) )
    return false;
  if( !mCommandSocket.wait_for_read( mTimeout ) )
    return false;
  bool success = false;
  ResponseNode parsedResponse( mCommands );
  bcidbg << parsedResponse;
  if( parsedResponse.Name() == "sendCommand_return" )
  {
    ResponseNode::List leaves = parsedResponse.Leaves();
    ResponseNode::List::const_iterator i = leaves.begin();
    const ResponseNode& status = *i;
    success = ( status.Name() == "status" && status.Value() == "complete" );
    if( ++i != leaves.end() )
    {
      outResultName = i->Name();
      outResultValue = i->Value();
    }
  }
  return success;
}

bool
AmpServerProADC::Connection::StartStreaming()
{
  mFirstRead = true;
  mSamplesInStream = 0;
  mSamplesInOutput = 0;
  return !!(mStream << BuildCommand( "ListenToAmp" ) << endl);
}

bool
AmpServerProADC::Connection::StopStreaming()
{
  bool result = !!(mStream << BuildCommand( "StopListeningToAmp" ) << endl);
  while( mStream.rdbuf()->in_avail() )
    mStream.ignore();
  return result;
}

bool
AmpServerProADC::Connection::ReadData( GenericSignal& Output )
{
  if( mFirstRead )
  {
    mFirstRead = false;
    if( !mStreamSocket.wait_for_read( mInitialTimeout ) )
      mError = "AmpServer is available, but does not stream live EEG data. "
               "To enable this feature, you need to obtain an AmpServerPro SDK license from EGI, "
               "and update EGI's HASP key.\n"
               "For more information, see http://www.egi.com.";
  }
  else if( !DoRead( Output ) )
    mError = "Lost connection to AmpServerPro";
  return mError.empty();
}

bool
AmpServerProADC::Connection::DoRead( GenericSignal& Output )
{
  while( !DoRead2( Output ) )
  {
    if( !mStreamSocket.wait_for_read( mTimeout ) )
      return false;
    BinaryData<int64_t, BigEndian> ampId;
    if( !ampId.Get( mStream ) || ampId != mAmpId )
      return false;
    BinaryData<uint64_t, BigEndian> packetSize;
    if( !packetSize.Get( mStream ) )
      return false;
    mSamplesInStream = packetSize / sizeof( float32_t ) / ( Channels() + 1 );
  }
  return !!mStream;
}

bool
AmpServerProADC::Connection::DoRead2( GenericSignal& Output )
{
  BinaryData<float32_t, BigEndian> data;
  while( mSamplesInOutput < Output.Elements() && mSamplesInStream > 0 )
  {
    for( int ch = 0; ch < Output.Channels(); ++ch )
    {
      data.Get( mStream );
      Output( ch, mSamplesInOutput ) = data;
    }
    ++mSamplesInOutput;
    for( int ch = Output.Channels(); ch < this->Channels() + 1; ++ch )
      data.Get( mStream );
    --mSamplesInStream;
  }
  bool done = ( mSamplesInOutput >= Output.Elements() );
  mSamplesInOutput = 0;
  return done || !mStream;
}
