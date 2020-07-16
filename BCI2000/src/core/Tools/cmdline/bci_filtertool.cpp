////////////////////////////////////////////////////////////////////
// $Id: bci_filtertool.cpp 5414 2016-07-01 19:45:56Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A BCI2000 filter wrapper that reads a BCI2000
//          compliant binary stream from standard input, applies
//          a BCI2000 filter, and writes its output to the
//          standard output as a BCI2000 compliant binary stream.
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
////////////////////////////////////////////////////////////////////
#include "bci_tool.h"
#include "Param.h"
#include "ParamList.h"
#include "State.h"
#include "StateList.h"
#include "StateVector.h"
#include "GenericVisualization.h"
#include "GenericFilter.h"
#include "MessageChannel.h"
#include "SysCommand.h"
#include "Files.h"

#define FILTER_NAME "$FILTER$"

using namespace std;

string ToolInfo[] =
{
  "",
  "Process standard input with the \"" FILTER_NAME "\" BCI2000 filter.",
  "Reads a BCI2000 compliant binary stream from standard input, applies the\n"
    FILTER_NAME " BCI2000 filter, and writes its output to standard output\n"
    "as a BCI2000 compliant binary stream.",
  "binary",
  "          --operator=<file>     Direct visualization messages to <file>"
  "          --operator=-          Direct visualization messages to stdout",
  ""
};


class FilterWrapper : private MessageChannel
{
 public:
   FilterWrapper( Streambuf&, Streambuf&, Streambuf& );
  ~FilterWrapper();

  static string FilterName();
  void Run();
  template<typename T> bool Send( const T& ) = delete;

 protected:
  bool OnParam( istream& ) override;
  bool OnState( istream& ) override;
  bool OnVisSignalProperties( istream& ) override;
  bool OnVisSignal( istream& ) override;
  bool OnStateVector( istream& ) override;

 private:
  void FinishProcessing();
  void StopRun();
  void OutputParameterChanges();
  void InitializeInputStatevector();
  void InitializeOutputStatevector();
  void DisposeStatevectors();
  void SynchronizeStatevectors();

 private:
  MessageChannel mOperator, mOutput;
  SignalProperties* mpInputProperties;
  GenericSignal mOutputSignal;
  ParamList mParamlist;
  StateList mInputStatelist,
            mOutputStatelist,
            mFilterStatelist;
  StateVector* mpInputStatevector,
             * mpOutputStatevector;
  bool      mSingleStatevector;
  Environment::Context& mEnvironment;
};


ToolResult
ToolInit()
{
  string filterName = FilterWrapper::FilterName();
  for( int i = 0; ToolInfo[i] != ""; ++i )
  {
    size_t namePos;
    while( ( namePos = ToolInfo[i].find( FILTER_NAME ) ) != string::npos )
      ToolInfo[i].replace( namePos, string( FILTER_NAME ).length(), filterName );
  }
  return noError;
}

ToolResult
ToolMain( Streambuf& in, Streambuf& out, OptionSet& arOptions )
{
  ToolResult result = noError;
  File operatorOut;
  if( arOptions.size() == 1 )
  {
    string operatorFile = arOptions.getopt( "--operator", "" );
    if( operatorFile == "" )
      return illegalOption;
    if( operatorFile == "-" )
      operatorOut.AttachToFd( ::fileno( stdout ) );
    else
      operatorOut.Open( operatorFile );
  }
  BufferedIO op;
  op.SetOutput( &operatorOut.Output() );
  FilterWrapper( in, out, op ).Run();
  if( !bcierr__.Empty() || in.Input()->Bad() || in.Input()->Failed() )
    result = illegalInput;
  return result;
}

FilterWrapper::FilterWrapper( Streambuf& in, Streambuf& out, Streambuf& op )
: MessageChannel( in ),
  mOutput( out ),
  mOperator( op ),
  mpInputProperties( nullptr ),
  mpInputStatevector( nullptr ),
  mpOutputStatevector( nullptr ),
  mSingleStatevector( true ),
  mEnvironment( *Environment::Context::GlobalInstance() )
{
  GenericVisualization::SetOutputChannel( &mOperator );
}

FilterWrapper::~FilterWrapper()
{
  DisposeStatevectors();
  delete mpInputProperties;
}

string
FilterWrapper::FilterName()
{
  string name = "<n/a>";
  ParamList paramlist;
  StateList statelist;
  Environment::Context environment;
  environment.EnterPhase( Environment::construction, &paramlist, &statelist, nullptr );
  GenericFilter::InstantiateFilters();
  environment.EnterPhase( Environment::nonaccess );
  GenericFilter* pFilter = GenericFilter::GetFilter<GenericFilter>();
  if( !pFilter )
    cerr << "Could not create filter instance.\n"
         << "Make sure there is a filter definition with a "
         << "\"RegisterFilter\" statement linked into the executable."
         << endl;
  else
    name = ClassName( typeid( *pFilter ) );
  environment.EnterPhase( Environment::destruction );
  GenericFilter::DisposeFilters();
  return name;
}

void
FilterWrapper::FinishProcessing()
{
  if( mEnvironment.Phase() == Environment::processing )
    StopRun();
  if( mEnvironment.Phase() != Environment::nonaccess )
    mEnvironment.EnterPhase( Environment::nonaccess );
}

void
FilterWrapper::Run()
{
  while( HandleMessage() && bcierr__.Empty() )
    ;
  FinishProcessing();
  mEnvironment.EnterPhase( Environment::destruction );
  GenericFilter::DisposeFilters();
}

bool
FilterWrapper::OnParam( istream& is )
{
  FinishProcessing();

  Param p;
  if( p.Unserialize( is ) )
    mParamlist[p.Name()] = p;
  return true;
}

bool
FilterWrapper::OnState( istream& is )
{
  if( mEnvironment.Phase() == Environment::processing )
    StopRun();

  if( mpInputStatevector )
  {
     mInputStatelist.Clear();
     DisposeStatevectors();
  }

  State s;
  if( s.Unserialize( is ) )
    mInputStatelist.Add( s );
  return true;
}

bool
FilterWrapper::OnStateVector( istream& is )
{
  if( !mpInputStatevector )
  {
    InitializeInputStatevector();
    for( const auto& state : mInputStatelist )
      mOutput.Send( state );
  }
  mpInputStatevector->Unserialize( is );
  SynchronizeStatevectors();
  if( !mpInputStatevector->StateValue( "Running" )
      && mEnvironment.Phase() == Environment::processing )
    StopRun();
  return true;
}

bool
FilterWrapper::OnVisSignalProperties( istream& is )
{
  delete mpInputProperties;
  mpInputProperties = nullptr;
  VisSignalProperties v;
  if( v.Unserialize( is ) && v.SourceID().empty() )
    mpInputProperties = new SignalProperties( v.SignalProperties() );
  return mpInputProperties;
}

bool
FilterWrapper::OnVisSignal( istream& is )
{
  VisSignal s;
  if( s.Unserialize( is ) && s.SourceID().empty() )
  {
    const GenericSignal& inputSignal = s;
    SignalProperties outputProperties;
    switch( mEnvironment.Phase() )
    {
      case Environment::nonaccess:
        {
          GenericFilter::DisposeFilters();

          ParamList filterParams;
          mFilterStatelist.Clear();
          mEnvironment.EnterPhase( Environment::construction, &filterParams, &mFilterStatelist );
          GenericFilter::InstantiateFilters();
          mEnvironment.EnterPhase( Environment::nonaccess );
          mEnvironment.EnterPhase( Environment::publication, &filterParams, &mFilterStatelist );
          GenericFilter::PublishFilters();
          if( !bcierr__.Empty() )
            break;
          // Make sure the filter's parameters get their properties from the filter
          // rather than the input stream.
          for( int i = 0; i < filterParams.Size(); ++i )
          {
            const string& name = filterParams[i].Name();
            if( mParamlist.Exists( name ) )
              filterParams[i].AssignValues( mParamlist[name] );
            mParamlist[name] = filterParams[i];
          }
        }
        /* no break */
      case Environment::publication:
        mEnvironment.EnterPhase( Environment::nonaccess );
        if( !mpInputStatevector )
          InitializeInputStatevector();
        InitializeOutputStatevector();
        for( const auto& state : mOutputStatelist )
          mOutput.Send( state );
        mParamlist.Add( "% int EvaluateTiming= 0" );
        mParamlist( "EvaluateTiming" ) = 0;
        MeasurementUnits::Initialize( mParamlist );
        mEnvironment.EnterPhase( Environment::preflight, &mParamlist, &mOutputStatelist, mpOutputStatevector );
        if( mpInputProperties
            && inputSignal.Channels() == mpInputProperties->Channels()
            && inputSignal.Elements() == mpInputProperties->Elements() )
        {
          mpInputProperties->SetUpdateRate( 1.0 / MeasurementUnits::SampleBlockDuration() );
          GenericFilter::PreflightFilters( *mpInputProperties, outputProperties );
        }
        else
        {
          delete mpInputProperties;
          mpInputProperties = nullptr;
          SignalProperties inputProperties( inputSignal.Properties() );
          inputProperties.SetUpdateRate( 1.0 / MeasurementUnits::SampleBlockDuration() );
          GenericFilter::PreflightFilters( inputProperties, outputProperties );
        }
        mOutputSignal.SetProperties( outputProperties );
        if( !bcierr__.Empty() )
          break;
        /* no break */
      case Environment::preflight:
        mEnvironment.EnterPhase( Environment::nonaccess );
        mEnvironment.EnterPhase( Environment::initialization, &mParamlist, &mOutputStatelist, mpOutputStatevector );
        GenericFilter::InitializeFilters();
        for( int i = 0; i < mParamlist.Size(); ++i )
          mOutput.Send( mParamlist[ i ] );
        mOutput.Send( outputProperties );
        if( !bcierr__.Empty() )
          break;
        /* no break */
      case Environment::initialization:
      case Environment::resting:
        mEnvironment.EnterPhase( Environment::nonaccess );
        mEnvironment.EnterPhase( Environment::startRun, &mParamlist, &mOutputStatelist, mpOutputStatevector );
        GenericFilter::StartRunFilters();
        /* no break */
      case Environment::processing:
      {
        mEnvironment.EnterPhase( Environment::nonaccess );
        mEnvironment.EnterPhase( Environment::processing, &mParamlist, &mOutputStatelist, mpOutputStatevector );
        GenericFilter::ProcessFilters( inputSignal, mOutputSignal );
        if( !bcierr__.Empty() )
          break;
        mOutput.Send( *mpOutputStatevector );
        mOutput.Send( mOutputSignal );
      } break;
      default:
        bcierr << "Unknown Environment phase";
    }
  }
  return !!is;
}

void
FilterWrapper::StopRun()
{
  mEnvironment.EnterPhase(Environment::nonaccess);
  mEnvironment.EnterPhase( Environment::stopRun, &mParamlist, &mOutputStatelist, mpOutputStatevector );
  GenericFilter::StopRunFilters();
  mEnvironment.EnterPhase(Environment::nonaccess);
  OutputParameterChanges();
  mEnvironment.EnterPhase( Environment::resting, &mParamlist, &mOutputStatelist, mpOutputStatevector );
  GenericFilter::RestingFilters();
  OutputParameterChanges();
}

void
FilterWrapper::OutputParameterChanges()
{
  ParamList changedParameters;
  for( int i = 0; i < mParamlist.Size(); ++i )
    if( mParamlist[i].Changed() )
      changedParameters.Add( mParamlist[i] );

  if( !changedParameters.Empty() )
  {
    bool success = mOutput.Send( changedParameters )
                && mOutput.Send( SysCommand::EndOfParameter );
    if( !success )
      bcierr << "Could not publish changed parameters" << endl;
  }
}

void
FilterWrapper::InitializeInputStatevector()
{
  delete mpInputStatevector;
  mpInputStatevector = new StateVector( mInputStatelist );
}

void
FilterWrapper::InitializeOutputStatevector()
{
  if( !mSingleStatevector )
    delete mpOutputStatevector;

  mSingleStatevector = mFilterStatelist.Empty();
  if( mSingleStatevector )
  {
    mOutputStatelist = mInputStatelist;
    mpOutputStatevector = mpInputStatevector;
  }
  else
  {
    for( const auto& state : mInputStatelist )
      mOutputStatelist.Add( state );
    for( const auto& state : mFilterStatelist )
      mOutputStatelist.Add( state );
    mOutputStatelist.AssignPositions();
    mpOutputStatevector = new StateVector( mOutputStatelist );
  }
  SynchronizeStatevectors();
  for( const auto& state : mOutputStatelist )
    mOutput.Send( state );
}

void
FilterWrapper::SynchronizeStatevectors()
{
  if( !mSingleStatevector )
    // Copy state values by name if input and output statevectors differ.
    for( const auto& state : mInputStatelist )
    {
      State::ValueType value = mpInputStatevector->StateValue( state.Name() );
      mpOutputStatevector->SetStateValue( state.Name(), value );
    }
}

void
FilterWrapper::DisposeStatevectors()
{
  delete mpInputStatevector;
  mpInputStatevector = 0;
  if( !mSingleStatevector )
    delete mpOutputStatevector;
  mpOutputStatevector = 0;
  mSingleStatevector = true;
}

