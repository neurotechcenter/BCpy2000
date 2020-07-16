////////////////////////////////////////////////////////////////////////////////
// $Id
// Author: juergen.mellinger@uni-tuebingen.de
//  Description: A filter that allows to compute and visualize various
//    statistics of its input signal.
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
#include "StatisticsFilter.h"

#include "ChannelSetSource.h"
#include "ExpressionSource.h"
#include "FunctionSource.h"
#include "ObserverSource.h"
#include "ViewSource.h"
#include "WildcardMatch.h"
#include "IndexList.h"
#include "BCIException.h"
#include "PrecisionTime.h"

using namespace std;
using namespace bci;

RegisterFilter( StatisticsFilter, 2.C2 );

StatisticsFilter::StatisticsFilter()
{
  mContext.type = Context::process;
  mContext.signal = NULL;
  mContext.variables = &mVariables;
  mContext.sources = &mSources;
  mContext.dependencies = &mDependencies;
  mContext.views = &mViews;
  mContext.outputView = &mpOutputView;
  mContext.scripts = &mScripts;
  mContext.visualizations = &mVisualizations;

 BEGIN_PARAMETER_DEFINITIONS
  "Statistics matrix ChannelSets= { AllChannels } 1 "
    " : "
    " % % % //"
    " Rows represent channel sets, defined as space-separated lists of channel names. "
    " Use row labels to name channel sets. Channel names may contain * and ? wildcards, "
    " and character ranges enclosed in []; wildcard patterns may be negated by an exclamation mark. "
    " Ranges of channels may be specified using : or - to separate begin from end.",
  "Statistics matrix Observers= "
    "{ Target1 Target2 Baseline Continuous } "
    "{ Observe%20what Observe%20when Observe%20over Observe%20weighted%20(boolean) Reset%20when } "
    "  AllChannels              TargetCode==1 30s  % % "
    "  AllChannels              TargetCode==2 30s  % % "
    "  AllChannels              TargetCode==0 30s  % % "
    "  TargetCode%20AllChannels true          30s  % % "
    " % % % //"
    " Rows represent observers. In the first column, you may specify channel sets, views, or expressions. "
    " Specify multiple entries in the first column in order to do multivariate statistics.",
  "Statistics matrix Views= "
    "{ Target1%20Baseline Target2%20Baseline Target1%20Target2 TargetCode%20Correlation } "
    "{ Output1 } "
    " RSquared(Target1,Baseline) RSquared(Target2,Baseline) RSquared(Target1,Target2) Continuous.Correlation()^2 "
    " % % % //"
    " Rows represent views. Views are defined by arithmetic expressions that may contain "
    " statistical functions, and properties of observers. Data from multiple "
    " columns will be concatenated in the output.",
  "Statistics string OutputView= "
    " %"
    " % % % //"
    " Specify a view to be copied into the filter's output signal. When empty, "
    " the filter's input will be used as an output.",
  "Statistics stringlist VisualizeViews= 0 "
    " % % % //"
    " Wildcard expressions matching views to be displayed. Enter a \"*\" to display all views",
  "Statistics matrix EventScripts= "
    "{ OnStartRun OnStopRun OnInitialize OnProcess } 1 "
    " Count:=0; % % Count:=Count+1; "
    " % % % //"
    " Specify statements to be executed during StartRun, StopRun, Initialize, or Process.",
 END_PARAMETER_DEFINITIONS
}


StatisticsFilter::~StatisticsFilter()
{
  Clear( mContext );
}


void
StatisticsFilter::Preflight( const SignalProperties& Input,
                                   SignalProperties& Output ) const
{
  DataSource::SetAbortFlag( false );
  Context context;
  context.type = Context::preflight;
  GenericSignal signal( Input );
  context.signal = &signal;
  Expression::VariableContainer variables;
  context.variables = &variables;
  DataSource::SourceList sources;
  context.sources = &sources;
  DataSource::SourceList dependencies;
  context.dependencies = &dependencies;
  ViewContainer views;
  context.views = &views;
  ViewSource* pOutputView = NULL;
  context.outputView = &pOutputView;
  ScriptContainer scripts;
  context.scripts = &scripts;
  context.visualizations = NULL;
  Configure( context );
  if( bcierr.Empty() )
  {
    const double numBlocks = 2;
    double allowedTime = numBlocks * MeasurementUnits::SampleBlockDuration();
    AbortThread abortThread( static_cast<int>( 1e3 * allowedTime ) );
    abortThread.Start();
    ProcessSources( context );
    abortThread.Terminate();
    if( abortThread.Result() )
      bcierr << "Data processing consumes more time than the limit of "
             << numBlocks << " sample block durations. "
             << "Reduce dimensionality of observers and views."
             << endl;

  }
  if( pOutputView )
    Output = pOutputView->Signal().Properties();
  else
    Output = Input;
  Clear( context );
}


void
StatisticsFilter::Initialize( const SignalProperties& Input,
                              const SignalProperties& Output )
{
  Clear( mContext );
  GenericSignal signal( Input );
  mContext.signal = &signal;
  Configure( mContext );
  mScripts[OnInitialize].Execute();
  InitializeVisualizations( mContext );
}


void
StatisticsFilter::Process( const GenericSignal& Input, GenericSignal& Output )
{
  mScripts[OnProcess].Execute( &Input );
  mContext.signal = &Input;
  ProcessSources( mContext );
  ProcessVisualizations( mContext );
  if( mpOutputView )
    Output = mpOutputView->Signal();
  else
    Output = Input;
}

void
StatisticsFilter::StartRun()
{
  mScripts[OnStartRun].Execute();
}

void
StatisticsFilter::StopRun()
{
  mScripts[OnStopRun].Execute();
}

// Helper functions
void
StatisticsFilter::Configure( const Context& ioContext ) const
{
  try
  {
    LoadChannelSets( ioContext );
    LoadViews( ioContext );
    LoadObservers( ioContext );
    LoadScripts( ioContext );
    DependSources( ioContext );
    InitializeSources( ioContext );
  }
  catch( const bad_alloc& )
  {
    Clear( ioContext );
    bcierr << "Out of memory when allocating objects."
           << " Reduce dimensionality of observers and views"
           << endl;
  }
}

void
StatisticsFilter::LoadChannelSets( const Context& ioContext ) const
{
  typedef ChannelSetSource::ChannelSet ChannelSet;
  typedef map<string, ChannelSet> ChannelSetContainer;
  ChannelSetContainer sets;
  const SignalProperties& Input = ioContext.signal->Properties();

  const ParamRef& ChannelSets = Parameter( "ChannelSets" );
  for( int i = 0; i < ChannelSets->NumRows(); ++i )
  {
    string name = ChannelSets->RowLabels()[i],
           definition;
    for( int j = 0; j < ChannelSets->NumColumns(); ++j )
      definition += string( ChannelSets( i, j ) ) + " ";
    istringstream iss( definition );
    string entry;
    ChannelSet channelSet;
    while( iss >> ws >> entry )
    {
      if( sets.find( entry ) != sets.end() )
      { // A channel list with that name exists, copy it.
        for( size_t i = 0; i < sets[entry].size(); ++i )
          channelSet.push_back( sets[entry][i] );
      }
      else
      {
        IndexList s( entry, Input.ChannelLabels(), Input.ChannelUnit() );
        string errors = s.Errors();
        if( errors.empty() && s.Empty() )
          errors = "entry \"" + entry + "\" does not match any channel";
        if( errors.empty() )
          for( int i = 0; i < s.Size(); ++i )
            channelSet.push_back( static_cast<int>( s[i] ) );
        else
          bcierr << "Channel set \"" << name << "\": " << errors;
      }
    }
    if( channelSet.empty() )
      bcierr << "Empty channel set \"" << name << "\"" << endl;
    else
      sets[name] = channelSet;
  }
  for( ChannelSetContainer::const_iterator i = sets.begin(); i != sets.end(); ++i )
    ioContext.sources->Add( new ChannelSetSource( i->first, i->second ) );
}

void
StatisticsFilter::LoadViews( const Context& ioContext ) const
{
  BCIStream::ContextFrame frame( "Views" );
  const ParamRef& Views = Parameter( "Views" );
  for( int row = 0; row < Views->NumRows(); ++row )
  {
    const string& name = Views->RowLabels()[row];
    if( ioContext.sources->Find( name ) )
      bcierr << name << " view definition: Names must be unique across channel sets, views and observers" << endl;
    ViewSource* pView = new ViewSource( name );
    for( int col = 0; col < Views->NumColumns(); ++col )
    {
      string expression = Views( row, col );
      if( !expression.empty() )
      {
        DataSource* pSource = ioContext.sources->Find( expression );
        if( !pSource || dynamic_cast<ExpressionSource*>( pSource ) )
        {
          pSource = new ExpressionSource( expression );
          ioContext.sources->Add( pSource );
        }
        pView->Add( pSource );
      }
    }
    ioContext.sources->Add( pView );
    ioContext.views->push_back( pView );
  }
}

void
StatisticsFilter::LoadObservers( const Context& ioContext ) const
{
  BCIStream::ContextFrame frame( "Observers" );
  enum { observeWhat, observeWhen, observeOver, observeWeighted, resetWhen, numCols };
  const ParamRef& Observers = Parameter( "Observers" );
  if( Observers->NumColumns() < numCols )
    bcierr << "The Observers parameter must have " << numCols << " columns" << endl;
  else
  {
    for( int row = 0; row < Observers->NumRows(); ++row )
    {
      const string& name = Observers->RowLabels()[row];
      if( ioContext.sources->Find( name ) )
        bcierr << name << " observer definition: Names must be unique across channel sets, views and observers" << endl;
      double window = MeasurementUnits::TimeInSampleBlocks( Observers( row, observeOver ) );
      window *= MeasurementUnits::SampleBlockSize();
      if( window <= 0 )
        bcierr << name << " observer: value in \"observe over\" row must be positive" << endl;
      bool weighted = Expression( Observers( row, observeWeighted ) ).Evaluate();
      ObserverSource* pObserver = new ObserverSource( name, Observers( row, observeWhen ), Observers( row, resetWhen ), window, weighted );
      istringstream iss( string( Observers( row, observeWhat ) ) );
      EncodedString sourceExpression;
      while( iss >> sourceExpression )
      {
        DataSource* pSource = ioContext.sources->Find( sourceExpression );
        if( !pSource || dynamic_cast<ExpressionSource*>( pSource ) )
        {
          pSource = new ExpressionSource( sourceExpression );
          ioContext.sources->Add( pSource );
        }
        pObserver->Add( pSource );
      }
      ioContext.sources->Add( pObserver );
    }
  }
}

void
StatisticsFilter::LoadScripts( const Context& ioContext ) const
{
  BCIStream::ContextFrame frame( "EventScripts" );
  ioContext.scripts->clear();
  ioContext.scripts->resize( NumEvents );
  const ParamRef& EventScripts = Parameter( "EventScripts" );
  if( EventScripts->NumColumns() > 1 )
    bciout << "EventScripts: Ignoring extra columns (use semicolons to concatenate statements)" << endl;
  #define EVENT( x ) { #x, x },
  const struct { const char* name; int index; }
  events[] =
  {
    EVENT( OnInitialize )
    EVENT( OnStartRun )
    EVENT( OnProcess )
    EVENT( OnStopRun )
  };
  for( size_t i = 0; i < sizeof( events ) / sizeof( *events ); ++i )
  {
    ( *ioContext.scripts )[events[i].index] = Expression( EventScripts( events[i].name, 0 ) );
    ( *ioContext.scripts )[events[i].index].Compile( ioContext.variables );
  }
}

void
StatisticsFilter::DependSources( const Context& ioContext ) const
{
  const ParamRef& VisualizeViews = Parameter( "VisualizeViews" );
  ViewContainer visViews;
  string OutputView = Parameter( "OutputView" );
  for( size_t i = 0; i < ioContext.views->size(); ++i )
  {
    ViewSource* pView = ( *ioContext.views )[i];
    if( pView->Name() == OutputView )
      *ioContext.outputView = pView;
    for( int i = 0; i < VisualizeViews->NumValues(); ++i )
      if( WildcardMatch( VisualizeViews( i ), pView->Name() ) )
        visViews.push_back( pView );
  }
  if( *ioContext.outputView == NULL && !OutputView.empty() )
    bcierr << "No view name matches the OutputView parameter: " << OutputView << endl;
  if( visViews.empty() && VisualizeViews->NumValues() > 0 )
    bciout << "No view name matches any of the pattern(s) specified in the VisualizeViews parameter" << endl;
  *ioContext.views = visViews;
  if( *ioContext.outputView )
    ( *ioContext.outputView )->Depends( ioContext, *ioContext.dependencies );
  for( size_t i = 0; i < ioContext.views->size(); ++i )
    ( *ioContext.views )[i]->Depends( ioContext, *ioContext.dependencies );
}

void
StatisticsFilter::InitializeSources( const Context& ioContext ) const
{
  for( size_t i = 0; i < ioContext.dependencies->size(); ++i )
    ( *ioContext.dependencies )[i]->Initialize( ioContext );
}

void
StatisticsFilter::InitializeVisualizations( const Context& ioContext ) const
{
  for( size_t i = 0; i < ioContext.visualizations->size(); ++i )
    ( *ioContext.visualizations )[i].Send( CfgID::Visible, false );
  ioContext.visualizations->clear();
  for( size_t i = 0; i < ioContext.views->size(); ++i )
  {
    ioContext.visualizations->push_back( GenericVisualization( ( *ioContext.views )[i]->Name() ) );
    const SignalProperties& properties = ( *ioContext.views )[i]->Signal().Properties();
    GenericSignal signal( properties );
    for( int ch = 0; ch < signal.Channels(); ++ch )
      for( int el = 0; el < signal.Elements(); ++el )
        signal( ch, el ) = numeric_limits<GenericSignal::ValueType>::quiet_NaN();
    ioContext.visualizations->back().Send( CfgID::Visible, true )
                                    .Send( CfgID::GraphType, properties.IsStream() ? CfgID::Polyline : CfgID::Field2d )
                                    .Send( CfgID::AutoScale, properties.IsStream() ? "off" : "10" )
                                    .Send( properties )
                                    .Send( signal );
  }
}

void
StatisticsFilter::ProcessSources( const Context& ioContext ) const
{
  for( size_t i = 0; i < ioContext.dependencies->size(); ++i )
    ( *ioContext.dependencies )[i]->Process( ioContext );
}

void
StatisticsFilter::ProcessVisualizations( const Context& inContext ) const
{
  for( size_t i = 0; i < inContext.visualizations->size(); ++i )
    ( *inContext.visualizations )[i].Send( ( *inContext.views )[i]->Signal() );
}

void
StatisticsFilter::Clear( const Context& ioContext ) const
{
  for( size_t i = 0; i < ioContext.sources->size(); ++i )
    delete ( *ioContext.sources )[i];
  ioContext.sources->clear();
  ioContext.dependencies->clear();
  ioContext.views->clear();
  *ioContext.outputView = NULL;
  ioContext.variables->clear();
}

int
StatisticsFilter::AbortThread::OnExecute()
{
  if( mTimeoutMs < 0 )
    return 0;

  if( mResolutionMs < 0 )
    throw std_range_error << "Resolution is " << mResolutionMs << ", must be >= 0";

  int remainingTime = mTimeoutMs;
  PrecisionTime last = PrecisionTime::Now();
  while( !Terminating() && remainingTime > 0 )
  {
    ThreadUtils::SleepForMs( mResolutionMs );
    PrecisionTime current = PrecisionTime::Now();
    mTimeoutMs -= PrecisionTime::UnsignedDiff( current, last );
    last = current;
  }
  bool timeoutOccurred = ( remainingTime <= 0 );
  if( timeoutOccurred )
    DataSource::SetAbortFlag( true );
  return timeoutOccurred;
}

