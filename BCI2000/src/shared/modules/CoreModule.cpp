////////////////////////////////////////////////////////////////////////////////
// $Id: CoreModule.cpp 5707 2017-08-23 19:04:11Z mellinger $
// Authors: schalk@wadsworth.org, juergen.mellinger@uni-tuebingen.de
// Description: A class that represents functionality common to all BCI2000
//          core modules.
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
#include "CoreModule.h"

#include "GenericFilter.h"
#include "GenericSignal.h"
#include "GenericVisualization.h"
#include "ProtocolVersion.h"
#include "SysCommand.h"
#include "Status.h"
#include "MeasurementUnits.h"
#include "Sockets.h"
#include "BCIStream.h"
#include "VersionInfo.h"
#include "FileUtils.h"
#include "ProcessUtils.h"
#include "StringUtils.h"
#include "ExceptionCatcher.h"
#include "ProcessUtils.h"

#include <string>
#include <sstream>
#include <cstdlib>

#undef bcierr
#define bcierr (bcierr_(sModuleName))

using namespace std;

static const string sModuleName = FileUtils::ExtractBase( FileUtils::ExecutablePath() );

CoreModule::CoreModule()
: mFiltersInitialized( false ),
  mTerminating( false ),
  mRunning( false ),
  mActiveResting( false ),
  mStartRunPending( false ),
  mStopRunPending( false ),
  mNeedStopRun( false ),
  mReceivingNextModuleInfo( false ),
  mGlobalID( NULL ),
  mOperatorBackLink( false ),
  mAutoConfig( false ),
  mOperator( *this ),
  mPreviousModule( *this ),
  mNextModule( *this ),
  mEnvironment( *Environment::Context::GlobalInstance() )
{
  mOperatorSocket.SetFlushAfterWrite( true );
  mOperator.SetIO( &mOperatorSocket );
  mNextModuleSocket.SetFlushAfterWrite( true );
  mNextModule.SetIO( &mNextModuleSocket );
  mPreviousModuleSocket.SetFlushAfterWrite( true );
  mPreviousModule.SetIO( &mPreviousModuleSocket );

  SharedPointer<Runnable> p( new MemberCall<void(CoreModule*)>( &CoreModule::InitializeStatevector, this ) );
  MeasurementUnits::AddInitializeCallback( p );
}

CoreModule::~CoreModule()
{
}

void
CoreModule::DoRun( int& ioArgc, char** ioArgv )
{
  if( Initialize( ioArgc, ioArgv ) )
    MainMessageLoop();
}

bool
CoreModule::Run( int& ioArgc, char** ioArgv )
{
  ExceptionCatcher catcher;
  catcher.SetMessage( "Terminating " + sModuleName + " module" );
  catcher.Run( MemberCall<void( CoreModule*, int&, char** )>( &CoreModule::DoRun, this, ioArgc, ioArgv ) );
  catcher.Run( MemberCall<void( CoreModule* )>( &CoreModule::ShutdownSystem, this ) );
  return ( bcierr__.Flushes() == 0 );
}

void
CoreModule::Terminate()
{
  mTerminating = true;
  if( mNeedStopRun )
    StopRunFilters();
  GenericFilter::HaltFilters();
  mEnvironment.EnterPhase( Environment::destruction );
}

// Internal functions.
bool
CoreModule::IsLastModule() const
{
#if IS_LAST_MODULE
  return true;
#else
  return false;
#endif
}

bool
CoreModule::Initialize( int& ioArgc, char** ioArgv )
{
  // Make sure there is only one instance of each module running at a time.
  if( !ProcessUtils::AssertSingleInstance( ioArgc, ioArgv, THISMODULE "Module", 5000 ) )
  {
    bcierr << "Another " THISMODULE " Module is currently running.\n"
           << "Only one instance of each module may run at a time.\n"
           << "Adding --AllowMultipleInstances as a command line parameter will disable this behavior.\n";
    return false;
  }
  ProcessUtils::InitializeDesktopIntegration( "org.bci2000.OnlineModules", FileUtils::ApplicationTitle() );
  OnInitialize( ioArgc, ioArgv );

  string operatorAddress;
  bool   executeTests = false,
         printVersion = false,
         printHelp = false;
  int i = 1;
  while( i < ioArgc )
  {
    string curArg( ioArgv[ i ] );
    if( curArg == "--version" || curArg == "-v" )
    {
      printVersion = true;
    }
    else if( curArg == "--help" || curArg == "-?" || curArg == "?" )
    {
      printHelp = true;
    }
    else if( curArg == "--local" )
    {
      if( !mParamlist.Exists( "ListeningAddress" ) )
        mParamlist.Add( "System string ListeningAddress= 127.0.0.1 // " );
    }
    else if( curArg == "--AllowMultipleInstances" )
    { // Do not add to parameter list
    }
    else if( curArg.find( "--" ) == 0 )
    {
      string paramDef = curArg.substr( 2 );
      size_t nameEnd = paramDef.find_first_of( "-=:" );
      string paramName = paramDef.substr( 0, nameEnd ),
             paramValue;
      if( paramName == THISMODULE "IP" )
        paramName = "ListeningAddress";
      if( nameEnd != string::npos )
        paramValue = paramDef.substr( nameEnd + 1 );
      Param param( paramName,
                   "System:Command Line Arguments",
                   "variant",
                   paramValue,
                   paramValue,
                   paramValue,
                   paramValue
                 );
      mParamlist.Add( param );
    }
    else if( curArg == "AUTOSTART" )
    { /* do nothing */
    }
    else if( operatorAddress.empty() )
    {
      operatorAddress = curArg;
    }
    else
    {
      printHelp = true;
    }
    ++i;
  }

  if( printVersion )
  {
    const VersionInfo& info = VersionInfo::Current;
    cout << "BCI2000 " << sModuleName << " \n\n"
         << " Framework: " << info[VersionInfo::VersionID] << " \n";
    if( !info[VersionInfo::Revision].empty() )
      cout << " Revision: " << info[VersionInfo::Revision]
               <<          ", " << info[VersionInfo::SourceDate] << " \n";
    if( !info[VersionInfo::Config].empty() )
      cout << " Config: " << info[VersionInfo::Config] << " \n";
    cout << " Build: " << info[VersionInfo::Build] << " \n";
    cout << flush;
    return true;
  }
  if( printHelp )
  {
    string executableName = FileUtils::ExtractBase( FileUtils::ExecutablePath() );
    cout << "Usage:\n"
         << executableName << " [<address>:<port>] [--<option>[=<value>]] ...\n"
         << " address  IP address of operator module (default 127.0.0.1)\n"
         << " port     operator port (default " THISOPPORT ")\n"
         << "Options are:\n"
         << " --help    display this help\n"
         << " --version display version information\n"
         << " --local   listen on 127.0.0.1 only -- recommended,\n"
         << "           avoids triggering firewall dialog\n"
         << " --ListeningAddress=<ip address> listen on specified address\n"
         << " --AllowMultipleInstances allows multiple modules of the same kind\n"
         << "           to run simultaneously\n"
         << "Any additional options will appear as parameters in the System section.\n"
         << std::endl;
    return false;
  }

  int priority = +1;
  if( mParamlist.Exists( "ProcessPriority" ) )
    priority = ::atoi( mParamlist["ProcessPriority"].Value().c_str() );
  ProcessUtils::Priority::Set( ProcessUtils::Priority::Normal );
  if( !ProcessUtils::Priority::ChangeBy( priority ) )
    bcierr << "Could not adjust process priority by " << std::showpos << priority << " relative to normal";

  if( mParamlist.Exists( "OperatorIP" ) )
    operatorAddress = mParamlist["OperatorIP"].Value().ToString();

  if( operatorAddress.empty() )
    operatorAddress = "127.0.0.1";
  if( operatorAddress.find( ":" ) == string::npos )
    operatorAddress += ":" THISOPPORT;

  BCIStream::RedirectFromStdio();
  InitializeOperatorConnection( operatorAddress );

  return ( bcierr__.Flushes() == 0 );
}

// This function contains the main event handling loop.
// It will be entered once when the program starts,
// and only be left when the program exits.
void
CoreModule::MainMessageLoop()
{
  // max time a GUI event needs to wait before it gets processed
  const Time::Interval bciMessageTimeout = Time::Seconds( 0.1 );
  Waitables inputs;
  inputs.Add( mOperator )
        .Add( mPreviousModule );

  while( !mTerminating )
  {
    inputs.Wait( bciMessageTimeout );
    ProcessBCIAndGUIMessages();
    if( mOperator.Bad() || !mOperatorSocket.Connected() )
      Terminate();
  }
}

void
CoreModule::ProcessBCIAndGUIMessages()
{
  bool repeat = true;
  while( repeat )
  {
    mPreviousModule.HandleMessages();
    mOperator.HandleMessages();

    repeat = false;
    if( mActiveResting && !mTerminating )
    {
      ProcessFilters();
      mActiveResting &= bcierr__.Empty();
      repeat |= mActiveResting;
    }
    // Allow for the GUI to process messages from its message queue if there are any.
    OnProcessGUIMessages();
    repeat |= OnGUIMessagesPending();
    repeat = repeat && mOperator.Good();

    if( mOperator.Bad() )
      bcierr << "Lost connection to operator: " << mOperator.DescribeIOState();
    if( mPreviousModule.Bad() )
      bcierr << "Lost connection to " PREVMODULE ": " << mPreviousModule.DescribeIOState();
    if( mNextModule.Bad() )
      bcierr << "Lost connection to " NEXTMODULE ": " << mNextModule.DescribeIOState();
  }
}

void
CoreModule::InitializeOperatorConnection( const string& inOperatorAddress )
{
  // creating connection to the operator
  mOperatorSocket.Open( inOperatorAddress );
  if( !mOperatorSocket.IsOpen() )
  { // wait if connection to operator module fails
    ThreadUtils::SleepFor( Time::Seconds( 2 ) );
    mOperatorSocket.Open( inOperatorAddress );
  }
  if( !mOperatorSocket.Connected() )
  {
    bcierr << "Could not connect to operator module" << endl;
    return;
  }

  mParamlist.Delete( "OperatorAddress" );
  mParamlist.Add( "System:Core%20Connections string OperatorAddress= % // (noedit)" );
  mParamlist[ "OperatorAddress" ].Value() = String() << inOperatorAddress;

  if( mOperatorSocket.Input().Wait( Time::Seconds( 0.5 ) ) )
  { // By immediately sending a neutral message, the operator module indicates that it
    // is able to deal with more recent versions of the protocol.
    mOperator.HandleMessages();
    mOperator.Send( ProtocolVersion::Current() );
  }
  else
  {
    bciwarn << "Operator module did not indicate support for protocol negotiation.\n\n"
            << "If you experience problems, please make sure that the Operator module's version "
            << "matches the most recent core module.";
    mOperator.Send( ProtocolVersion::Current().Major() );
  }
  mOperator.SetIsLocal( mOperatorSocket.Connected() == Socket::local );
  BCIStream::SetOutputChannel( &mOperator );
  GenericVisualization::SetOutputChannel( &mOperator );

  if( mParamlist.Exists( "ListeningAddress" ) )
    mServerSocket.Open( mParamlist["ListeningAddress"].Value().ToString() );
  else
    mServerSocket.Open( Socket::LocalIPs().back() );
  mOperator.SetIO( &mOperatorSocket, CoreConnection::AsyncReceive );
  mParamlist.Add( "System string ModuleName= " THISMODULE );

  mParamlist.Delete( "ListeningAddress" );
  mParamlist.Add( "System string ListeningAddress= %" );
  mParamlist["ListeningAddress"].Value() = String() << mServerSocket.IP() << ":" << mServerSocket.Port();

  mParamlist.Add( // internally used
    "System:Core%20Connections string OperatorIP= x"
    " 127.0.0.1 % % // the Operator module's IP (noedit)(readonly)" );
  mParamlist["OperatorIP"].Value() = mOperatorSocket.IP();

  mEnvironment.EnterPhase( Environment::construction, &mParamlist, &mStatelist );
  GenericFilter::InstantiateFilters();
  mEnvironment.EnterPhase( Environment::nonaccess );
  mEnvironment.EnterPhase( Environment::publication, &mParamlist, &mStatelist );
  GenericFilter::PublishFilters();
  mEnvironment.EnterPhase( Environment::nonaccess );
  if( bcierr__.Flushes() > 0 )
    return;

  { // Version info
    const VersionInfo& info = VersionInfo::Current;
    mParamlist.Add(
      "System:Version matrix " THISMODULE "Version= "
        "{ Framework Revision Build Config Protocol } 1 "
        " % % % % % // " THISMODULE " version information (noedit)(readonly)" );
    Param& p = mParamlist[THISMODULE "Version"];
    p.Value( "Framework" ) = info[VersionInfo::VersionID];
    std::string revision = info[VersionInfo::Revision] + ", " +  info[VersionInfo::SourceDate];
    if( revision.front() == ',' )
      revision = revision.substr( 2 );
    p.Value( "Revision" ) = revision;
    p.Value( "Build" ) = info[VersionInfo::Build];
    p.Value( "Config" ) = info[VersionInfo::Config];
    ostringstream oss;
    oss << ProtocolVersion::Current();
    p.Value( "Protocol" ) = oss.str();
  }
  { // Filter chain documentation
    mParamlist.Add(
      "System:Configuration matrix " THISMODULE "FilterChain= "
        "0 { Filter%20Name Position%20String } "
        " % % % // " THISMODULE " filter chain (noedit)(readonly)" );
    Param& p = mParamlist[ THISMODULE "FilterChain" ];
    const GenericFilter::ChainInfo& chain = GenericFilter::GetChainInfo();
    p.SetNumRows( chain.size() );
    for( size_t row = 0; row < chain.size(); ++row )
    {
      p.Value( row, "Filter Name" ) = chain[ row ].name;
      p.Value( row, "Position String" ) = chain[ row ].position;
    }
  }
  { // Filter directory documentation
    mParamlist.Add( "System:Configuration matrix Filters= 0 1 % % % // Filter Directory (noedit)(readonly)" );
    AppendFilterDirectory( mParamlist["Filters"] );
  }

  // Now, publish parameters ...
  mParamlist.Sort();
  mOperator.Send( mParamlist );
  mOperator.Send( SysCommand::EndOfParameter );
  // ... and states.
  mOperator.Send( mStatelist );
  mOperator.Send( SysCommand::EndOfState );

  mOperator.Send( Status( "Waiting for configuration ...", Status::plainMessage ) );
}


void
CoreModule::InitializeCoreConnections()
{
  string nextModuleAddress, address = "NextModuleAddress";
  if( mNextModuleInfo.Exists( address ) )
    nextModuleAddress = mNextModuleInfo[address].Value();
  else
  {
    string ip = NEXTMODULE "IP";
    if( mParamlist.Exists( ip ) )
    {
      ip = mParamlist[ip].Value();
      string port = NEXTMODULE "Port";
      if( mParamlist.Exists( port ) )
      {
        port = mParamlist[port].Value();
        nextModuleAddress = ip + ":" + port;
      }
    }
  }
  if( nextModuleAddress.empty() )
  {
    bcierr << "Next module's IP/Port parameters not available";
    return;
  }
  mNextModuleSocket.Open( nextModuleAddress );
  if( !mNextModuleSocket.IsOpen() )
  {
    bcierr << "Could not make a connection to the next module at " << nextModuleAddress;
    return;
  }
  mNextModule.SetIsLocal( mNextModuleSocket.Connected() == Socket::local );

  if( !mServerSocket.WaitForAccept( mPreviousModuleSocket, cInitialConnectionTimeout ) )
    bcierr << "Connection to previous module timed out after "
           << float( cInitialConnectionTimeout ) / 1e3 << "s";
  mServerSocket.Close();
}


void
CoreModule::ShutdownSystem()
{
  BCIStream::SetOutputChannel( 0 );
  GenericVisualization::SetOutputChannel( 0 );
  mOperatorSocket.Close();
  mPreviousModuleSocket.Close();
  mNextModuleSocket.Close();
  GenericFilter::DisposeFilters();
}

void
CoreModule::InitializeStatevector()
{
  int sampleBlockSize = ::atoi( mParamlist["SampleBlockSize"].Value().c_str() );
  if( sampleBlockSize > 0 && sampleBlockSize + 1 != mStatevector.Samples() )
  {
    // The state vector holds an additional sample which is used to initialize
    // the subsequent state vector at the beginning of a new block.
    mStatevector = StateVector( mStatelist, sampleBlockSize + 1 );
    mInitialStatevector = mStatevector;
    mStatevector.CommitStateChanges();
  }
}

void
CoreModule::ResetStatevector()
{
  State::ValueType sourceTime = mStatevector.StateValue( "SourceTime" );
  mStatevector = mInitialStatevector;
  mStatevector.SetStateValue( "SourceTime", sourceTime );
  mStatevector.CommitStateChanges();
}

namespace
{
  struct AppendFilter
  {
    bool operator()( const Directory::Node* pNode )
    {
      if( pNode->Children().empty() )
      {
        int idx = param.NumRows();
        param.SetNumRows( idx + 1 );
        param.Value( idx, 0 ) = pNode->Path();
      }
      return true;
    }
    Param& param;
  };
}

void
CoreModule::AppendFilterDirectory( Param& p ) const
{
  AppendFilter append = { p };
  Directory::Traverse( GenericFilter::Directory(), append );
}

void
CoreModule::InitializeFilterChain( const SignalProperties& Input )
{
  bcierr__.Clear();
  GenericFilter::HaltFilters();
  InitializeInputSignal( Input );

  // Does the Operator module accept the AutoConfig protocol?
  if( mParamlist.Exists( "AutoConfig" ) )
    mAutoConfig = ::atoi( mParamlist["AutoConfig"].Value().c_str() );

  AutoConfigFilters();
  if( bcierr__.Empty() )
    InitializeFilters();
}

void
CoreModule::InitializeInputSignal( const SignalProperties& Input )
{
#ifdef TODO
# error The inputPropertiesFixed variable may be removed once property messages contain an UpdateRate field.
#endif // TODO
  SignalProperties inputFixed( Input );
  if( !Input.IsEmpty() )
  {
    MeasurementUnits::Initialize( mParamlist );
    inputFixed.SetUpdateRate( 1.0 / MeasurementUnits::SampleBlockDuration() );
  }
  mInputSignal = GenericSignal( inputFixed );
}

void
CoreModule::AutoConfigFilters()
{
  for( int i = 0; i < mParamlist.Size(); ++i )
    mParamlist[i].Unchanged();

  ParamList restoreParams;
  if( !mAutoConfig )
  {
    restoreParams = mParamlist;
    if( restoreParams.Exists( "SubjectRun" ) )
      restoreParams.Delete( "SubjectRun" ); // special case for backward compatibility reasons
  }

  mEnvironment.EnterPhase( Environment::preflight, &mParamlist, &mStatelist );
  SignalProperties Output( 0, 0 );
  GenericFilter::PreflightFilters( mInputSignal.Properties(), Output );
  mEnvironment.EnterPhase( Environment::nonaccess );
  mOutputSignal = GenericSignal( Output );

  if( !IsLastModule() )
  {  // sharing the output signal in the last module might result in a race condition between operator
     // and source module, so we never share the last module's output signal
    if( mNextModule.IsLocal() && mNextModule.Protocol().Provides( ProtocolVersion::SharedSignalStorage ) )
      mOutputSignal.ShareAcrossModules();
  }
  for( int i = 0; i < restoreParams.Size(); ++i )
  {
    const Param& r = restoreParams[i];
    Param& p = mParamlist[r.Name()];
    if( p.Changed() )
    {
      p = r;
      p.Unchanged();
    }
  }
  if( mParamlist.Exists( "DebugLevel" ) && ::atoi( mParamlist["DebugLevel"].Value().c_str() ) )
  {
    for( int i = 0; i < mParamlist.Size(); ++i )
      if( mParamlist[i].Changed() )
        bciout << "AutoConfig: " << mParamlist[i];
  }
  if( mAutoConfig && bcierr__.Empty() )
  {
    BroadcastParameterChanges();
    if( !mOperator.Send( Output ) )
      bcierr << "Could not send output properties to Operator module" << endl;
  }
}

void
CoreModule::InitializeFilters()
{
  mLargeSignals.clear();
  if( bcierr__.Empty() )
  {
    if( mParamlist.Exists( "OperatorBackLink" ) )
      mOperatorBackLink = ::atoi( mParamlist["OperatorBackLink"].Value().c_str() );
    if( !mAutoConfig )
    {
      if( IsLastModule() )
      {
        if( mOperatorBackLink && !mOperator.Send( mOutputSignal.Properties() ) )
          bcierr << "Could not send output properties to Operator module" << endl;
      }
      else
      {
        if( !mNextModule.Send( mOutputSignal.Properties() ) )
          bcierr << "Could not send output properties to " NEXTMODULE " module" << endl;
      }
    }
    mEnvironment.EnterPhase( Environment::initialization, &mParamlist, &mStatelist, &mStatevector );
    GenericFilter::InitializeFilters();
    mEnvironment.EnterPhase( Environment::nonaccess );
  }
  if( bcierr__.Empty() )
  {
    mOperator.Send( Status( sModuleName + " initialized", Status::firstInitializedMessage + MODTYPE - 1 ) );
    mFiltersInitialized = true;
    mActiveResting = mInputSignal.Properties().IsEmpty();
  }
}


void
CoreModule::StartRunFilters()
{
  mStartRunPending = false;
  mActiveResting = false;
  mEnvironment.EnterPhase( Environment::startRun, &mParamlist, &mStatelist, &mStatevector );
  GenericFilter::StartRunFilters();
  mEnvironment.EnterPhase( Environment::nonaccess );
  mNeedStopRun = true;
  if( bcierr__.Empty() )
    mOperator.Send( Status( sModuleName + " running", Status::firstRunningMessage + 2 * ( MODTYPE - 1 ) ) );
}


void
CoreModule::StopRunFilters()
{
  mStopRunPending = false;
  mEnvironment.EnterPhase( Environment::stopRun, &mParamlist, &mStatelist, &mStatevector );
  GenericFilter::StopRunFilters();
  mEnvironment.EnterPhase( Environment::nonaccess );
  mNeedStopRun = false;
  ResetStatevector();
  if( bcierr__.Empty() && !mTerminating )
  {
    BroadcastParameterChanges();
    if( mInputSignal.Properties().IsEmpty() )
      mOperator.Send( SysCommand::Suspend );
    mOperator.Send( Status( sModuleName + " suspended", Status::firstSuspendedMessage + 2 * ( MODTYPE - 1 ) ) );
  }
  mEnvironment.EnterPhase( Environment::resting, &mParamlist, &mStatelist, &mStatevector );
  GenericFilter::RestingFilters();
  mEnvironment.EnterPhase( Environment::nonaccess );
  mActiveResting = mInputSignal.Properties().IsEmpty();
}

void
CoreModule::BroadcastParameterChanges()
{
  ParamList changedParameters;
  for( int i = 0; i < mParamlist.Size(); ++i )
    if( mParamlist[ i ].Changed() )
      changedParameters.Add( mParamlist[ i ] );

  if( !changedParameters.Empty() )
  {
    mOperator.Send( changedParameters );
    if( !mOperator.Send( SysCommand::EndOfParameter ) )
      bcierr << "Could not publish changed parameters" << endl;
  }
}

void
CoreModule::ProcessFilters()
{
  bool wasRunning = mRunning;
  StateUpdate();
  if( mStartRunPending )
    StartRunFilters();
  mEnvironment.EnterPhase( Environment::processing, &mParamlist, &mStatelist, &mStatevector );
  GenericFilter::ProcessFilters( mInputSignal, mOutputSignal, !( mRunning || wasRunning ) );
  mEnvironment.EnterPhase( Environment::nonaccess );
  if( bcierr__.Empty() && ( mRunning || wasRunning ) )
    SendOutput();
  if( bcierr__.Empty() && mStopRunPending )
    StopRunFilters();
  if( !bcierr__.Empty() )
    Terminate();
}

void
CoreModule::SendOutput()
{
  if( IsLastModule() && mOperatorBackLink )
  {
    mOperator.Send( mStatevector );
    mOperator.Send( mOutputSignal );
  }
  mNextModule.Send( mStatevector );
  if( !IsLastModule() )
    mNextModule.Send( mOutputSignal );
}

void
CoreModule::StateUpdate()
{
  mStatevector.CommitStateChanges();
  bool running = mStatevector.StateValue( "Running" );
  mStopRunPending |= mRunning && !running;
  mStartRunPending |= running && !mRunning;
  mRunning = running;
}

void
CoreModule::OnReceive( CoreConnection&, const Param& p )
{
  if( mRunning )
    bcierr << "Unexpected Param message" << endl;

  ParamList& list = mReceivingNextModuleInfo ? mNextModuleInfo : mParamlist;
  list[p.Name()] = p;
}


void
CoreModule::OnReceive( CoreConnection&, const State& s )
{
  if( mStatevector.Length() > 0 )
  {
    // Changing a state's value via mStatevector.PostStateChange()
    // will buffer the change, and postpone it until the next call to
    // mStatevector.CommitStateChanges(). That call happens
    // after arrival of a StateVector message to make sure that
    // changes are not overwritten with the content of the previous
    // state vector when it arrives from the application module.
    mStatevector.PostStateChange( s.Name(), s.Value() );
  }
  else
  {
#ifdef TODO
# error Remove saving of State::Kind once the protocol has been modified
#endif // TODO
    int kind = State::StateKind;
    if( mStatelist.Exists( s.Name() ) )
      kind = mStatelist.ByName( s.Name() ).Kind();
    mStatelist.Delete( s.Name() );
    State state = s;
    state.SetKind( kind );
    mStatelist.Add( state );
  }
}

void
CoreModule::OnReceive( CoreConnection&, const VisSignal& s )
{
  if( s.SourceID() == "" )
  {
    mInputSignal.AssignValues( s );
    if( !mFiltersInitialized )
      bcierr << "Unexpected VisSignal message";
    else
      ProcessFilters();
  }
}


void
CoreModule::OnReceive( CoreConnection&, const VisSignalProperties& s )
{
  if( s.SourceID().empty() )
  {
    if( !mFiltersInitialized )
      InitializeFilterChain( s.SignalProperties() );
  }
}


bool
CoreModule::OnStateVector( CoreConnection&, istream& is )
{
  mStatevector.Unserialize( is );
  if( mInputSignal.Properties().IsEmpty() )
    ProcessFilters();
  return true;
}

void
CoreModule::OnReceive( CoreConnection&, const SysCommand& s )
{
  if( s == SysCommand::EndOfState )
  {
    InitializeCoreConnections();
  }
  else if( s == SysCommand::EndOfParameter )
  {
    mReceivingNextModuleInfo = false;
    mFiltersInitialized = false;
    mActiveResting = false;
  }
  else if( s == SysCommand::Start )
  {
    /* do nothing */
  }
  else if( s == SysCommand::Reset )
  {
    Terminate();
  }
  else
  {
    bcierr << "Unexpected SysCommand";
  }
}

void
CoreModule::OnReceive( CoreConnection&, const ProtocolVersion& p )
{
  if( mOperator.Protocol().Provides( ProtocolVersion::NextModuleInfo ) )
  {
    mReceivingNextModuleInfo = true;
    mNextModule.SetProtocol( p );
  }
}

bool
CoreModule::OnSend( CoreConnection& inConnection, const VisSignal& inSignal )
{
  const GenericSignal* pSignal = &inSignal.Signal();
  if( pSignal->Channels() * pSignal->Elements() * sizeof(float) > 2048 )
  {
    if( ++mLargeSignals[pSignal] == 2 )
    {
      bool share = inConnection.IsLocal();
      share &= inConnection.Protocol().Provides( ProtocolVersion::SharedSignalStorage );
      if( share )
        const_cast<GenericSignal*>( pSignal )->ShareAcrossModules();
    }
  }
  return true;
}
