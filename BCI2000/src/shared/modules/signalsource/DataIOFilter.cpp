////////////////////////////////////////////////////////////////////////////////
// $Id: DataIOFilter.cpp 5851 2019-01-07 16:02:42Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A filter that handles data acquisition from a GenericADC,
//   storing through a GenericFileWriter, and signal calibration into muV.
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
#include "DataIOFilter.h"

#include "defines.h"
#include "GenericADC.h"
#include "GenericFileWriter.h"
#include "BCIStream.h"
#include "BCIException.h"
#include "BCIEvent.h"
#include "PrecisionTime.h"
#include "ClassName.h"
#include "MeasurementUnits.h"
#include "StringUtils.h"
#include "SimpleStatistics.h"
#include "Rate.h"

#include <fstream>
#include <iostream>
#include <iomanip>
#include <set>
#include <numeric>

using namespace std;
using namespace bci;

static double CalibrateTo = 1e-6; // muV

RegisterFilter( DataIOFilter, 0 );

enum TimingChannelIdx
{
  Block,
  BlockNom,
  BlockAvg,
  Roundtrip,
  Stimulus,
  StimTrig,
  StimDisp,
  StimLatAvg,
  StimAudio,

  NumTimingChannels
};

namespace
{

class TimingObserver
{
public:
  TimingObserver();
  void SetEnabled( bool = true );
  void SetWindowLength( float block, float stim );
  void Reset( float nominalBlock );
  void Observe( GenericSignal& );
  void ObserveStimLat( float );
private:
  int mState;
  SimpleStatistics<double> mBlock, mRoundtrip, mLatency;
};

// a-b
int32_t
SignedDiff( uint32_t a, uint32_t b, int bits )
{
  uint32_t mask = uint32_t( -1 );
  mask >>= 8*sizeof(mask) - bits;
  a &= mask;
  b &= mask;
  int32_t diff = a - b, wraparound = 1 << bits;
  if( diff >= wraparound / 2 )
    diff -= wraparound;
  else if( diff < -wraparound / 2 )
    diff += wraparound;
  return diff;
}

} // namespace

struct DataIOFilter::Private
{
  TimingObserver mTimingObserver;
  int mLastAudioPresentationTime;
  Private() : mLastAudioPresentationTime(-1) {}
};


  DataIOFilter::DataIOFilter()
: mpADC( GenericFilter::PassFilter<GenericADC>() ),
  mpSourceFilter( NULL ),
  mpFileWriter( NULL ),
  mpFileWriterInput( NULL ),
  mVisualizeSource( false ),
  mVisualizeTiming( false ),
  mVisualizeSourceDecimation( 1 ),
  mVisualizeSourceBufferSize( 1 ),
  mSourceVis( "SRCD" ),
  mTimingVis( "RNDT" ),
  mTimingSignal( NumTimingChannels, 1 ),
  mBlockCount( 0 ),
  mBlockDuration( 0 ),
  mSampleBlockSize( 0 ),
  mPrevStimulusTime( 0 ),
  mBCIEvents( 0 ),
  mWasResting( false ),
  p( new Private )
{
  BCIEvent::SetEventQueue( &mBCIEvents );

  // Find available GenericFileWriter descendants and determine which one to use.
  typedef set<GenericFileWriter*> writerSet;
  set<GenericFileWriter*> availableFileWriters;
  for( GenericFileWriter* p = GenericFilter::PassFilter<GenericFileWriter>();
       p != NULL;
       p = GenericFilter::PassFilter<GenericFileWriter>() )
    availableFileWriters.insert( p );

  if( !availableFileWriters.empty() )
  {
    string fileFormat = OptionalParameter( "FileFormat", "BCI2000" );
    fileFormat = StringUtils::ToUpper( fileFormat );
    if( fileFormat == "DAT" )
      fileFormat = "BCI2000";
    if( fileFormat == "NULL" )
      fileFormat = "Null";

    string writerName = fileFormat + "FileWriter";
    for( writerSet::const_iterator i = availableFileWriters.begin();
         mpFileWriter == NULL && i != availableFileWriters.end();
         ++i )
      if( writerName == ClassName( typeid( **i ) ) )
        mpFileWriter = *i;

    if( !mpFileWriter )
      bcierr << "Could not identify writer component for file format "
             << "\"" << fileFormat << "\""
             << endl;

    availableFileWriters.erase( mpFileWriter );
    for( writerSet::const_iterator i = availableFileWriters.begin();
         i != availableFileWriters.end();
         ++i )
      delete *i;
  }
  // Check whether the next filter in the chain is of type SourceFilter.
  // If this is the case, use it for preprocessing _before_ writing into the file.
  GenericFilter* filter = GenericFilter::GetFilter<GenericFilter>();
  if( filter && string( "SourceFilter" ) == ClassName( typeid( *filter ) ) )
    mpSourceFilter = GenericFilter::PassFilter<GenericFilter>();
}

void
DataIOFilter::Publish()
{
  if( mpADC )
    mpADC->CallPublish();

  BEGIN_PARAMETER_DEFINITIONS
    // Parameters required to interpret a data file are listed here
    // to enforce their presence:
    "Source:Signal%20Properties int SourceCh= 16 "
      "% % % // number of digitized and stored channels",
    "Source:Signal%20Properties int SampleBlockSize= 32 "
      "% % % // number of samples transmitted at a time",
    "Source:Signal%20Properties int SamplingRate= 256Hz "
      "% % % // sampling rate",
    "Source:Signal%20Properties list ChannelNames= 0 "
      "% % % // list of channel names",
    "Source:Signal%20Properties list SourceChOffset= 1 auto "
      "% % % // Offset for channels in A/D units",
    "Source:Signal%20Properties list SourceChGain= 1 auto "
      "% % % // gain for each channel (A/D units per physical unit)",

    // Storage related parameters are listed here to enforce their presence
    // even if not used by all FileWriter classes.
    "Storage:Data%20Location string DataDirectory= ..\\data ..\\data % % "
      "// path to top level data directory (directory)",
    "Storage:Session string SubjectName= Name Name % % "
      "// subject alias",
    "Storage:Session string SubjectSession= 001 001 % % "
      "// three-digit session number",
    "Storage:Session string SubjectRun= 00 00 % % "
      "// two-digit run number",
    "Storage:Documentation string ID_System= % % % % "
      "// BCI2000 System Code",
    "Storage:Documentation string ID_Amp= % % % % "
      "// BCI2000 Amp Code",
    "Storage:Documentation string ID_Montage= % % % % "
      "// BCI2000 Cap Montage Code",

    // Visualization of data as far as managed by the DataIOFilter:
    "Visualize:Timing int VisualizeTiming= 1 1 0 1 "
      "// visualize system timing (0=no, 1=yes) (boolean)",
    "Visualize:Source%20Signal int VisualizeSource= 1 1 0 1 "
      "// visualize raw brain signal (0=no, 1=yes) (boolean)",
    "Visualize:Source%20Signal int VisualizeSourceDecimation= auto auto % % "
      "// decimation factor for raw brain signal",
    "Visualize:Source%20Signal int VisualizeSourceBufferSize= auto auto % % "
      "// number of blocks to aggregate before sending to operator",
    "Visualize:Source%20Signal int VisualizeSourceTime= 2s 2s 0 % "
      "// how much time in Source visualization",
    "Visualize:Source%20Signal int SourceMin= auto % % % "
      "// raw signal vis Min Value",
    "Visualize:Source%20Signal int SourceMax= auto % % % "
      "// raw signal vis Max Value",
  END_PARAMETER_DEFINITIONS

  BEGIN_STATE_DEFINITIONS
    "Running 1 0 0 0",
    "Recording 1 1 0 0", // "Recording" must have an initial value of 1 to avoid
                         // it being set to zero by the other modules' initialization
                         // code.
    "SourceTime 16 0 0 0",
    "StimulusTime 16 0 0 0",
  END_STATE_DEFINITIONS

  if( mpADC ) // intentionally applied twice
    mpADC->CallPublish();

  if( mpFileWriter )
    mpFileWriter->CallPublish();

  if( mpSourceFilter )
    mpSourceFilter->CallPublish();
}


DataIOFilter::~DataIOFilter()
{
  Halt();
  delete mpADC;
  delete mpSourceFilter;
  delete mpFileWriter;
  BCIEvent::DenyEvents();
  BCIEvent::SetEventQueue( 0 );
  delete p;
}

bool
DataIOFilter::ParameterUnconfigured( const char* inName ) const
{
  auto param = ActualParameter( inName );
  return param->NumValues() == 1 && param( 0 ) == "auto";
}

void
DataIOFilter::AutoConfig( const SignalProperties& Input )
{
  if( !mpADC )
    bcierr << "Expected an ADC filter instance to be present";
  else
    mpADC->CallAutoConfig( Input );

  // Fix auto parameters that may not have been autoconfigured
  if( ParameterUnconfigured( "ChannelNames" ) )
    Parameter( "ChannelNames" )->SetNumValues( 0 );

  if( ParameterUnconfigured( "SourceChOffset" ) )
  {
    MutableParamRef SourceChOffset = Parameter( "SourceChOffset" );
    SourceChOffset->SetNumValues( ActualParameter( "SourceCh" ) );
    for( int i = 0; i < SourceChOffset->NumValues(); i++ )
      SourceChOffset( i ) = 0;
  }

  if( ParameterUnconfigured( "SourceChGain" ) )
  {
    MutableParamRef SourceChGain = Parameter( "SourceChGain" );
    SourceChGain->SetNumValues( ActualParameter( "SourceCh" ) );
    for( int i = 0; i < SourceChGain->NumValues(); i++ )
      SourceChGain( i ) = 1;
  }

  // Fix Remaining Parameters
  ParamList p = *Parameters;
  static const char* fixparams[] = { "SamplingRate", "SampleBlockSize" };
  for( size_t i = 0; i < sizeof( fixparams ) / sizeof( *fixparams ); ++i )
    if( !::atof( p[fixparams[i]].Value().c_str() ) )
      p[fixparams[i]].Value() = "1";

  MeasurementUnits::Initialize( p );
}

void
DataIOFilter::Preflight( const SignalProperties& Input,
                               SignalProperties& Output ) const
{
  OptionalState( "PresentationRequested" );
  OptionalState( "PresentationDisplayed" );
  OptionalState( "PresentationTime" );
  OptionalState( "AudioPresentationTime" );
  // Parameter existence and range.
  Parameter( "ListeningAddress" );
  PreflightCondition( Parameter( "SamplingRate" ).InHertz() > 0 );

  int SampleBlockSize = Parameter( "SampleBlockSize" );
  PreflightCondition( SampleBlockSize > 0 );

  bool sourceChOffsetConsistent = ( Parameter( "SourceChOffset" )->NumValues() >= Parameter( "SourceCh" ) );
  if( !sourceChOffsetConsistent )
  {
    bcierr << "The number of entries in the SourceChOffset parameter (currently "
           << Parameter( "SourceChOffset" )->NumValues()
           << ") must match the SourceCh parameter (currently "
           << Parameter( "SourceCh" )
           << ")";
  }

  bool sourceChGainConsistent = ( Parameter( "SourceChGain" )->NumValues() >= Parameter( "SourceCh" ) );
  if( !sourceChGainConsistent )
  {
    bcierr << "The number of entries in the SourceChGain parameter (currently "
           << Parameter( "SourceChGain" )->NumValues()
           << ") must match the SourceCh parameter (currently "
           << Parameter( "SourceCh" )
           << ")";
  }

  if( Parameter( "VisualizeSource" ) == 1 )
  {    
    if( Parameter( "VisualizeSourceDecimation" ) != string( "auto" ) )
    {
      int VisualizeSourceDecimation = Parameter( "VisualizeSourceDecimation" );
      if( VisualizeSourceDecimation <= 0 )
        bcierr << "The VisualizeSourceDecimation parameter must be greater 0"
               << endl;
      if( Parameter( "VisualizeSourceBufferSize" ) != string( "auto" ) )
      {
        int VisualizeSourceBufferSize = Parameter( "VisualizeSourceBufferSize" ),
            effectiveSampleBlockSize = SampleBlockSize * VisualizeSourceBufferSize;
        if( effectiveSampleBlockSize % VisualizeSourceDecimation != 0 )
          bcierr << "The VisualizeSourceDecimation parameter "
                 << "(now " << VisualizeSourceDecimation << ") "
                 << "must be a divider of VisualizeSourceDecimation times sample block size "
                 << "(now " << effectiveSampleBlockSize << ")"
                 << endl;
      }
    }
    if( Parameter( "VisualizeSourceBufferSize" ) != string( "auto" ) )
    {
      int VisualizeSourceBufferSize = Parameter( "VisualizeSourceBufferSize" );
      if( VisualizeSourceBufferSize <= 0 )
        bcierr << "The VisualizeSourceBufferSize parameter must be greater 0"
               << endl;
    }
    Parameter( "SourceMin" );
    Parameter( "SourceMax" );
  }
  else
  {
    Parameter( "VisualizeSourceDecimation" );
    Parameter( "VisualizeSourceBufferSize" );
  }

  // Channel names unique?
  set<string> names;
  for( int i = 0; i < Parameter( "ChannelNames" )->NumValues(); ++i )
  {
    string name = Parameter( "ChannelNames" )( i );
    if( names.find( name ) == names.end() )
      names.insert( name );
    else
      bcierr << "Duplicate name: \"" << name << "\" in ChannelNames parameter" << endl;
  }
  // Sub-filter preflight/signal properties.
  // The ADC and file writer filters must have a position string greater than
  // that of the DataIOFilter.
  SignalProperties adcOutput;
  AdjustProperties( adcOutput );
  // Calibration
  if( !sourceChOffsetConsistent || !sourceChGainConsistent )
    return;
  bool unitsPresent = false;
  for( int ch = 0; ch < adcOutput.Channels(); ++ch )
  {
    PhysicalUnit& u = adcOutput.ValueUnit( ch );
    try
    {
      u.SetOffset( Parameter( "SourceChOffset" )( ch ) )
       .SetGainWithSymbol( Parameter( "SourceChGain" )( ch ) );
      unitsPresent |= !u.Symbol().empty();
    }
    catch( const BCIException& e )
    {
      throw bcierr << "SourceChGain(" << ch + 1 << "): " << e.What();
    }
  }
  if( !unitsPresent )
  {
    for( int ch = 0; ch < adcOutput.Channels(); ++ch )
    {
      PhysicalUnit& u = adcOutput.ValueUnit( ch );
      u.SetGain( u.Gain() * 1e-6 ).SetSymbol( "V" );
    }
  }
  Output = adcOutput;
  for( int ch = 0; ch < adcOutput.Channels(); ++ch )
  {
    PhysicalUnit& a = adcOutput.ValueUnit( ch );
    a.SetRawMin( a.PhysicalToRawValue( -100 * CalibrateTo ) )
     .SetRawMax( a.PhysicalToRawValue( 100 * CalibrateTo ) );
    PhysicalUnit& u = Output.ValueUnit( ch );
    u.SetRawMin( a.RawMin() )
     .SetRawMax( a.RawMax() );
  }

  if( mpADC )
    mpADC->CallPreflight( adcOutput, Output );
  // Fixup Output properties without destroying changes made by the ADC filter.
  AdjustProperties( Output );
  for( int ch = 0; ch < Output.Channels(); ++ch )
  {
    PhysicalUnit& a = adcOutput.ValueUnit( ch ),
                & u = Output.ValueUnit( ch );
    u.SetOffset( a.Offset() )
     .SetGain( a.Gain() )
     .SetSymbol( a.Symbol() );
    if( !u.IsRangeSet() )
    {
      u.SetRawMin( a.RawMin() );
      u.SetRawMax( a.RawMax() );
    }
  }
  adcOutput = Output;
  mADCOutput.SetProperties( adcOutput );
  if( !mpADC->IsRealTimeSource() )
    if( OptionalParameter( "EvaluateTiming", 1 ) == 0 )
      bciwarn << "The EvaluateTiming parameter is false, so realtime operation will not be enforced" << endl;

  if( !mpSourceFilter )
  {
    mSourceFilterOutput = GenericSignal();
    mpFileWriterInput = &mADCOutput;
  }
  else
  {
    mpSourceFilter->CallAutoConfig( adcOutput );
    SignalProperties sourceFilterOutput( adcOutput );
    mpSourceFilter->CallPreflight( adcOutput, sourceFilterOutput );
    mSourceFilterOutput.SetProperties( sourceFilterOutput );
    mpFileWriterInput = &mSourceFilterOutput;
  }

  if( !mpFileWriter )
  {
    bcierr << "Expected a file writer filter instance to be present" << endl;
  }
  else
  {
    SignalProperties writerOutput;
    mpFileWriter->CallAutoConfig( mpFileWriterInput->Properties() );
    mpFileWriter->CallPreflight( mpFileWriterInput->Properties(), writerOutput );
    if( !writerOutput.IsEmpty() )
      bcierr << "Expected empty output signal from file writer" << endl;
  }

  // Signal properties.
  if( !Input.IsEmpty() )
    bcierr << "Expected empty input signal" << endl;

  // For reasons of backward compatibility, force output signal to be
  // in micro-units (i.e., multiplied by 1e6, such as to get microvolts from volts).
  for( int ch = 0; ch < Output.Channels(); ++ch )
  {
    PhysicalUnit& u = Output.ValueUnit( ch );
    double physMin = u.RawToPhysicalValue( u.RawMin() ),
           physMax = u.RawToPhysicalValue( u.RawMax() );
    u.SetOffset( 0 )
     .SetGain( CalibrateTo );
    u.SetRawMin( u.PhysicalToRawValue( physMin ) )
     .SetRawMax( u.PhysicalToRawValue( physMax ) );
  }
  Output.SetType( SignalType::float32 );

  // Range.
  if( Parameter( "SourceMin" ) != "auto" )
  {
    double rangeMin = Output.ValueUnit().PhysicalToRaw( Parameter( "SourceMin" ) );
    for( int ch = 0; ch < Output.Channels(); ++ch )
      Output.ValueUnit( ch ).SetRawMin( rangeMin );
  }
  if( Parameter( "SourceMax" ) != "auto" )
  {
    double rangeMax = Output.ValueUnit().PhysicalToRaw( Parameter( "SourceMax" ) );
    for( int ch = 0; ch < Output.Channels(); ++ch )
      Output.ValueUnit( ch ).SetRawMax( rangeMax );
  }

  double numSamplesInDisplay = Parameter( "VisualizeSourceTime" ).InSeconds() * Parameter( "SamplingRate" ).InHertz();
  if( !PhysicalUnit().SetSymbol( "s" ).IsPhysical( Parameter( "VisualizeSourceTime" ) ) )
    bciwarn << "The VisualizeSourceTime parameter specifies time without unit. "
            << "Throughout BCI2000, time specifications without unit are now consistently interpreted "
            << "as being given in sample blocks.\n"
            << "If your source display appears strange, try appending the letter \"s\" to the "
            << "VisualizeSourceTime parameters value.";
  Output.ElementUnit().SetRawMin( 0 )
                      .SetRawMax( numSamplesInDisplay - 1 );
}

void
DataIOFilter::Initialize( const SignalProperties& /*Input*/,
                          const SignalProperties& Output )
{
  mBCIEvents.SetMaxCount( Output.Elements() * 1000 );
  mOutputBuffer.SetProperties( Output );
  const SignalProperties& adcOutput = mADCOutput.Properties();
  mSampleBlockSize = adcOutput.Elements();
  mBlockDuration = 1e3 * ( Rate::FromHertz( Parameter( "SamplingRate" ).InHertz() ).Period() * mSampleBlockSize ).Seconds();

  State( "Recording" ) = 0;
  mpADC->CallInitialize( adcOutput, adcOutput );
  if( mpSourceFilter )
    mpSourceFilter->CallInitialize( adcOutput, mSourceFilterOutput.Properties() );
  mpFileWriter->CallInitialize( mpFileWriterInput->Properties(), SignalProperties( 0, 0 ) );

  // Configure visualizations.
  mVisualizeSource = ( Parameter( "VisualizeSource" ) == 1 );

  if( Parameter( "VisualizeSourceBufferSize" ) == string( "auto" ) )
  {
    // Choose visualization buffer size for an update rate of 30Hz.
    mVisualizeSourceBufferSize = static_cast<int>( ::floor( 1000.0 / mBlockDuration / 30.0 ) );
    if( mVisualizeSourceBufferSize < 1 )
      mVisualizeSourceBufferSize = 1;
    bcidbg( 5 ) << "Choosing a VisualizeSourceBufferSize of "
                << mVisualizeSourceBufferSize << endl;
  }
  else
  {
    mVisualizeSourceBufferSize = Parameter( "VisualizeSourceBufferSize" );
  }
  SignalProperties v = Output;
  v.SetElements( mVisualizeSourceBufferSize * v.Elements() );
  mVisSourceBuffer.SetProperties( v );

  if( Parameter( "VisualizeSourceDecimation" ) == string( "auto" ) )
  {
    mVisualizeSourceDecimation = static_cast<int>( ::floor( adcOutput.SamplingRate() / 256.0 ) );
    if( mVisualizeSourceDecimation < 1 )
      mVisualizeSourceDecimation = 1;
    int numSamplesBuffered = static_cast<int>( mVisualizeSourceBufferSize * mSampleBlockSize );
    while( numSamplesBuffered % mVisualizeSourceDecimation )
      --mVisualizeSourceDecimation;
    bcidbg( 5 ) << "Choosing a VisualizeSourceDecimation value of "
                << mVisualizeSourceDecimation << endl;
  }
  else
  {
    mVisualizeSourceDecimation = Parameter( "VisualizeSourceDecimation" );
  }
  mBlockCount = 0;
  mAcquired = Time::Never;

  SignalProperties d = Output;
  int newElements = Output.Elements() * mVisualizeSourceBufferSize / mVisualizeSourceDecimation;
  if( newElements < 1 )
    newElements = 1;
  double trueDecimation = ( Output.Elements() * mVisualizeSourceBufferSize ) / newElements;
  d.SetName( "Source Signal" )
   .SetElements( newElements )
   .SetType( SignalType::float32 )
   .ElementUnit().SetGain( Output.ElementUnit().Gain() * trueDecimation )
                 .SetRawMax( Output.ElementUnit().RawMax() / trueDecimation );
  mDecimatedSignal.SetProperties( d );
  if( mVisualizeSource )
    mSourceVis.Send( mDecimatedSignal.Properties() );
  mSourceVis.Send( CfgID::Visible, mVisualizeSource );

  mVisualizeTiming = ( Parameter( "VisualizeTiming" ) == 1 );

  std::string addr = Parameter( "ListeningAddress" );
  addr = addr.substr( 0, addr.find(':') );
  addr = addr.substr( 0, addr.find('.') );
  bool measureStimulus = ( addr == "127" || !::stricmp( addr.c_str(), "localhost" ) ),
       displayPresentation = measureStimulus,
       displayAudio = false;
  displayPresentation = displayPresentation && States->Exists( "PresentationTime" );
  displayPresentation = displayPresentation && States->Exists( "PresentationRequested" );
  displayPresentation = displayPresentation && States->Exists( "PresentationDisplayed" );
  displayAudio = displayPresentation && States->Exists( "AudioPresentationTime" );

  SignalProperties s = mTimingSignal.Properties();
  if(displayAudio)
    s.SetChannels( StimAudio + 1 );
  else if( displayPresentation )
    s.SetChannels( StimLatAvg + 1 );
  else if( measureStimulus )
    s.SetChannels( Stimulus + 1 );
  else
    s.SetChannels( Roundtrip + 1 );
  s.SetName( "Timing" );
  // Roundtrip values are in ms, and we want a range that is twice the value
  // of what we expect for the second signal (the time between subsequent
  // completions of the ADC's Process()).
  s.ValueUnit().SetRawMin( 0 ).SetRawMax( 2 * mBlockDuration )
               .SetOffset( 0 ).SetGain( 1e-3 ).SetSymbol( "s" );
  s.ChannelLabels()[Block] = ":Block";
  s.ChannelLabels()[BlockNom] = ":%20%20nominal";
  s.ChannelLabels()[BlockAvg] = ":%20%20average";
  s.ChannelLabels()[Roundtrip] = ":Roundtrip";
  if( measureStimulus )
    s.ChannelLabels()[Stimulus] = ":Stimulus";
  if( displayPresentation )
  {
    s.ChannelLabels()[StimTrig] = ":%20%20triggered";
    s.ChannelLabels()[StimDisp] = ":%20%20displayed";
    s.ChannelLabels()[StimLatAvg] = ":%20%20latency";
  }
  if( displayAudio )
    s.ChannelLabels()[StimAudio] = ":%20%20audio";
  s.ElementUnit().SetRawMin( 0 ).SetRawMax( 127 )
                 .SetOffset( 0 ).SetGain( mBlockDuration / 1e3 ).SetSymbol( "s" );
  mTimingSignal = GenericSignal( s, GenericSignal::NaN );
  mTimingSignal( BlockNom, 0 ) = mBlockDuration;

  p->mTimingObserver.SetWindowLength( MeasurementUnits::TimeInSampleBlocks( "10s" ), 30 );
  p->mTimingObserver.Reset( mBlockDuration );
  p->mTimingObserver.SetEnabled( mpADC->IsRealTimeSource() || ( OptionalParameter( "EvaluateTiming", 1 ) != 0 ) );

  if( mVisualizeTiming )
  {
    mTimingVis.Send( mTimingSignal.Properties() );
    ColorList colors( NumTimingChannels );
    colors[Block] = RGBColor::White;
    colors[BlockAvg] = RGBColor::Red;
    colors[BlockNom] = RGBColor::Aqua;
    colors[Roundtrip] = RGBColor::LtGray;
    colors[Stimulus] = RGBColor::DkGray;
    colors[StimTrig] = RGBColor::Yellow;
    colors[StimDisp] = RGBColor::Lime;
    colors[StimLatAvg] = RGBColor::Orange;
    colors[StimAudio] = RGBColor::Purple;

    std::string styles = "-----";
    const char* blackCircle_utf8 = "\xe2\x97\x8f";
    if( displayPresentation )
      styles.append( blackCircle_utf8 ).append( blackCircle_utf8 ).append( "-" );
    if( displayAudio )
      styles.append( blackCircle_utf8 );

    mTimingVis.Send( CfgID::ChannelColors, colors )
              .Send( CfgID::ChannelStyles, styles )
              .Send( CfgID::ShowBaselines, true )
              .Send( CfgID::ShowNumericValues, true )
              .Send( CfgID::ShowValueUnit, true )
              .Send( CfgID::AutoScale, "off" );
  }
  mTimingVis.Send( CfgID::Visible, mVisualizeTiming );
  if( mpSourceFilter )
    mpSourceFilter->CallStartRun();
}

void
DataIOFilter::StartRun()
{
  mpADC->CallStartRun();
  mpFileWriter->CallStartRun();

  mStatevectorBuffer = *Statevector;
  mStatevectorBuffer.SetStateValue( "Recording", 0 );

  State( "Recording" ) = 1;
}


void
DataIOFilter::StopRun()
{
  mpADC->CallStopRun();
  mpFileWriter->CallStopRun();

  State( "Recording" ) = 0;

  mTimingSignal( Roundtrip, 0 ) = GenericSignal::NaN;
  for( int i = Stimulus; i < mTimingSignal.Channels(); ++i )
    mTimingSignal( i, 0 ) = GenericSignal::NaN;

  p->mLastAudioPresentationTime = -1;
}

void
DataIOFilter::Process( const GenericSignal& /*Input*/,
                             GenericSignal& Output )
{
  DoProcess( Output, false );
}

void
DataIOFilter::Resting( const GenericSignal& /*Input*/,
                             GenericSignal& Output )
{
  DoProcess( Output, true );
}

void
DataIOFilter::DoProcess( GenericSignal& Output, bool inResting )
{
  PrecisionTime functionEntry = PrecisionTime::Now();
  bool recording = inResting ? false : mStatevectorBuffer.StateValue( "Recording" );
  // Moving the save-to-file code to the beginning of Process() implies
  // that the time spent on i/o operations will only reduce the
  // time spent waiting for A/D data, and thus not enter into the
  // roundtrip time.
  // In between, the signal is buffered in a data member.
  // The BCI2000 standard requires that the state vector saved with a data block
  // is the one that existed when the data left the ADC.
  // So we also need to buffer the state vector between calls to Process().
  if( recording )
    mpFileWriter->CallWrite( *mpFileWriterInput, mStatevectorBuffer );

  if( inResting && !mWasResting )
  {
    for( int i = StimTrig; i < mTimingSignal.Channels(); ++i )
      mTimingSignal( i, 0 ) = mTimingSignal.NaN;
  }
  if( !inResting )
  {
    int timeStampBits = State( "SourceTime" )->Length();
    PrecisionTime prevSourceTime = PrecisionTime(State( "SourceTime" )),
                  stimulusTime = PrecisionTime(State( "StimulusTime" ));
    mTimingSignal( Roundtrip, 0 ) = SignedDiff( functionEntry, prevSourceTime, timeStampBits );
    if( mTimingSignal.Channels() > StimTrig )
    {
      auto s = mTimingSignal.NaN;
      for( int sample = 0; sample < Statevector->Samples() - 1; ++sample )
      {
        int32_t id = State( "PresentationRequested" )( sample ).AsSigned();
        if( id >= 0 )
        {
          int diff = id & 0xff;
          if( !(diff > s) ) // min, use negation to handle NaN
            s = diff;
        }
      }
      mTimingSignal( StimTrig, 0 ) = s;
    }
    if( mTimingSignal.Channels() > StimDisp )
    {
      auto s = mTimingSignal.NaN;
      for( int sample = 0; sample < Statevector->Samples() - 1; ++sample )
      {
        int32_t id = State( "PresentationDisplayed" )( sample ).AsSigned();
        if( id >= 0 )
        {
          PrecisionTime sourceTime = id >> 8, reqDiff = id & 0xff,
            dispTime = PrecisionTime(State( "PresentationTime" )( sample ));
          int disp = SignedDiff( dispTime, sourceTime, timeStampBits - 1 );
          if( disp > 0 )
          {
            p->mTimingObserver.ObserveStimLat( disp - reqDiff );
            if( !(disp < s) ) // max, use negation to handle NaN
              s = disp;
          }
        }
      }
      mTimingSignal( StimDisp, 0 ) = s;
    }
    if( mTimingSignal.Channels() > Stimulus )
    {
      if( !mWasResting && stimulusTime != mPrevStimulusTime ) // ignore if application does not set StimulusTime
        mTimingSignal( Stimulus, 0 ) = SignedDiff( stimulusTime, prevSourceTime, timeStampBits );
      mPrevStimulusTime = stimulusTime;
    }
    if( mTimingSignal.Channels() > StimAudio )
    {
      int audioPresentationTime = State("AudioPresentationTime");
      if (audioPresentationTime != p->mLastAudioPresentationTime && p->mLastAudioPresentationTime >= 0)
      {
        int delta = PrecisionTime::SignedDiff(PrecisionTime(audioPresentationTime), prevSourceTime);
        delta += mBlockDuration;
        mTimingSignal(StimAudio, 0) = delta;
      }
      else
      {
        mTimingSignal(StimAudio, 0) = mTimingSignal.NaN;
      }
      p->mLastAudioPresentationTime = audioPresentationTime;
    }
  }
  mWasResting = inResting;

  AcquireData();
  Output = mOutputBuffer;
  Statevector->ResetToCarryover( State::EventKind );
  ProcessBCIEvents();
  mStatevectorBuffer = *Statevector;
  Statevector->ResetToCarryover( State::StateKind );
}

void
DataIOFilter::Halt()
{
  mAcquired = Time::Never;
  if( mpADC )
    mpADC->CallHalt();
  if( mpSourceFilter )
    mpSourceFilter->CallHalt();
  if( mpFileWriter )
    mpFileWriter->CallHalt();
}

void
DataIOFilter::AdjustProperties( SignalProperties& p ) const
{
  p.SetChannels( Parameter( "SourceCh" ) )
   .SetElements( Parameter( "SampleBlockSize" ) )
   .SetUpdateRate( Parameter( "SamplingRate" ).InHertz() / Parameter( "SampleBlockSize" ) );
  p.ElementUnit().SetOffset( 0 )
                 .SetGain( 1.0 / Parameter( "SamplingRate" ).InHertz() )
                 .SetSymbol( "s" );
  // Channel labels.
  LabelIndex& outputLabels = p.ChannelLabels();
  int namesFromParam = min( p.Channels(), Parameter( "ChannelNames" )->NumValues() );
  for( int i = 0; i < namesFromParam; ++i )
    outputLabels[i] = Parameter( "ChannelNames" )( i ).ToString();
  for( int i = namesFromParam; i < p.Channels(); ++i )
  {
    ostringstream oss;
    oss << "Ch" << i + 1;
    outputLabels[i] = oss.str();
  }
}

void
DataIOFilter::AcquireData()
{
  mpADC->CallProcess( mADCOutput, mADCOutput );
  if( !mpADC->SetsSourceTime() )
    State( "SourceTime" ) = PrecisionTime::Now();

  if( mpSourceFilter )
    mpSourceFilter->CallProcess( mADCOutput, *mpFileWriterInput );

  for( int ch = 0; ch < mpFileWriterInput->Channels(); ++ch )
  {
    const PhysicalUnit& inputUnit = mpFileWriterInput->Properties().ValueUnit( ch ),
                      & outputUnit = mOutputBuffer.Properties().ValueUnit( ch );
    for( int el = 0; el < mOutputBuffer.Elements(); ++el )
    {
      GenericSignal::ValueType phys = inputUnit.RawToPhysicalValue( (*mpFileWriterInput)( ch, el ) );
      mOutputBuffer( ch, el ) = outputUnit.PhysicalToRawValue( phys );
    }
  }

  CopyBlock( mOutputBuffer, mVisSourceBuffer, mBlockCount % mVisualizeSourceBufferSize );
  ++mBlockCount;

  Time now = TimeUtils::MonotonicTime();
  if( mAcquired > now )
  {
    for( int i = 0; i < mTimingSignal.Channels(); ++i )
      mTimingSignal( i, 0 ) = mTimingSignal.NaN;
  }
  else
  {
    mTimingSignal( Block, 0 ) = ( now - mAcquired ).Seconds() * 1e3;
    p->mTimingObserver.Observe( mTimingSignal );
  }
  mTimingSignal( BlockNom, 0 ) = mBlockDuration;
  mAcquired = now;

  DoVisualize();
}

void
DataIOFilter::DoVisualize()
{
  // If the visualization thread has been stalled, we repeatedly
  // send the same data to move the visualization display by the
  // correct amount of samples.
  int sendCount = mBlockCount / mVisualizeSourceBufferSize;
  mBlockCount %= mVisualizeSourceBufferSize;
  if( mVisualizeSource && sendCount > 0 )
  {
    Downsample( mVisSourceBuffer, mDecimatedSignal );
    if( sendCount > 4 )
      sendCount = 1; // If we have been stalled for so long, it makes no sense to try to keep up.
    while( --sendCount >= 0 )
      mSourceVis.Send( mDecimatedSignal );
  }
  if( mVisualizeTiming )
    mTimingVis.Send( mTimingSignal );
}

void
DataIOFilter::Downsample( const GenericSignal& Input, GenericSignal& Output )
{
  int decimationFactor = Input.Elements() / Output.Elements();
  for( int ch = 0; ch < Output.Channels(); ++ch )
    for( int outsample = 0; outsample < Output.Elements(); ++outsample )
    {
      GenericSignal::ValueType value = 0.0;
      for( int insample = outsample * decimationFactor; insample < ( outsample + 1 ) * decimationFactor; ++insample )
        value += Input( ch, insample ) / decimationFactor;
      Output( ch, outsample ) = value;
    }
}

void
DataIOFilter::CopyBlock( const GenericSignal& Input, GenericSignal& Output, int inBlock )
{
  int blockSize = Input.Elements();
  for( int ch = 0; ch < Input.Channels(); ++ch )
    for( int sample = 0; sample < Input.Elements(); ++sample )
      Output( ch, sample + blockSize * inBlock ) = Input( ch, sample );
}

void
DataIOFilter::ProcessBCIEvents()
{
  // When translating event time stamps into sample positions, we assume a block's
  // source time stamp to match the subsequent block's first sample.
  PrecisionTime sourceTime = static_cast<PrecisionTime::NumType>( Statevector->StateValue( "SourceTime" ) );

  while( !mBCIEvents.IsEmpty()
         && PrecisionTime::SignedDiff( mBCIEvents.FrontTimeStamp(), sourceTime ) <= 0 )
  {
    int offset = static_cast<int>(
      ( ( mBlockDuration - ( PrecisionTime::UnsignedDiff( sourceTime, mBCIEvents.FrontTimeStamp() ) + 1 ) ) * mSampleBlockSize ) / mBlockDuration
    );
    istringstream iss( mBCIEvents.FrontDescriptor() );
    string name;
    iss >> name;
    State::ValueType value;
    iss >> value;
    int duration;
    if( !( iss >> duration ) )
      duration = -1;

    const class State& state = States->ByName( name );
    int kind = state.Kind();
    if( kind != State::EventKind )
      throw std_invalid_argument <<
        "Trying to set state \"" << name << "\" from an event. "
        "This state was not defined as an event state. "
        "Use BEGIN_EVENT_DEFINITIONS/END_EVENT_DEFINITIONS to define states "
        "as event states.";

    offset = max( offset, 0 );
    if( duration < 0 )
    { // No duration given -- set the state at the current and following positions.
      Statevector->SetStateValue( state.Location(), state.Length(), offset, value );
    }
    else if( duration == 0 )
    { // Set the state at a single position only.
      // For zero duration events, avoid overwriting a previous event by
      // moving the current one if possible, and reposting if not.
      State::ValueType val = Statevector->CarryoverValue( state.Location(), state.Length() );
      while( offset < mSampleBlockSize && Statevector->StateValue( state.Location(), state.Length(), offset ) != val )
        ++offset;
      if( offset == mSampleBlockSize ) // Re-post the event to be processed in the next block
        mBCIEvents.PushBack( mBCIEvents.FrontDescriptor(), mBCIEvents.FrontTimeStamp() );
      else
        Statevector->SetSampleValue( state.Location(), state.Length(), offset, value );
    }
    else
    {
      bcierr__ << "Event durations > 0 are currently unsupported "
               << "(" << iss.str() << ")"
               << endl;
    }
    mBCIEvents.PopFront();
  }
}

namespace
{
// TimingObserver
TimingObserver::TimingObserver()
: mState( 0 )
{
}

void TimingObserver::SetEnabled( bool inEnabled )
{
  mState = inEnabled ? 1 : 0;
}

void TimingObserver::SetWindowLength( float block, float stim )
{
  mBlock.SetWindowLength( block );
  mRoundtrip.SetWindowLength( block );
  mLatency.SetWindowLength( stim );
}

void TimingObserver::Reset( float nom )
{
  mBlock.Reset( nom );
  mRoundtrip.Reset( 0 );
  mLatency.Reset();
}

void TimingObserver::ObserveStimLat( float lat )
{
  mLatency.Observe( lat );
}

void TimingObserver::Observe( GenericSignal& ioSignal )
{
  enum
  {
    enabled = 1,

    rtripApproaching = 2,
    rtripExceeding = 4,
    rtripTooLarge = 6,
    rtrip = rtripApproaching | rtripExceeding | rtripTooLarge,

    blockLow = 8,
    blockHigh = 16,
    block = blockLow | blockHigh,
  };
  if( ioSignal( Block, 0 ) < 0 )
  {
#if !BCIDEBUG
    bciwarn << "Time measurement appears to be unreliable on your system. "
            << "You cannot use BCI2000 time stamps for timing evaluation."
            << endl;
#endif
    mState = 0;
  }

  mBlock.Observe( ioSignal( Block, 0 ) );
  mRoundtrip.Observe( ioSignal( Roundtrip, 0 ) );
  if( ioSignal.Channels() > StimLatAvg && mLatency.Count() > 2 )
    ioSignal( StimLatAvg, 0 ) = mLatency.Mean();
  ioSignal( BlockAvg, 0 ) = mBlock.Mean();
  if( !( mState & enabled ) || mBlock.Count() < mBlock.WindowLength() )
    return;

  double avgBlockDuration = mBlock.Mean() / ioSignal( BlockNom, 0 ),
         avgRoundtrip = mRoundtrip.Mean() / ioSignal( BlockNom, 0 );

  int newState = mState & enabled;

  if( avgRoundtrip >= 1.1 )
    newState |= rtripTooLarge;
  else if( avgRoundtrip >= 1.0 )
    newState |= rtripExceeding;
  else if( avgRoundtrip >= 0.75 )
    newState |= rtripApproaching;

  if( avgBlockDuration > 1.05 )
    newState |= blockHigh;
  else if( avgBlockDuration < 0.95 )
    newState |= blockLow;

  if( (newState & rtrip) > (mState & rtrip) )
    switch( newState & rtrip )
    {
      case rtripTooLarge:
        bcierr << "Roundtrip time consistently exceeds nominal block duration (currently "
               << avgRoundtrip * 100
               << "%)"
               << endl;
        break;
      case rtripExceeding:
        bciwarn << "Roundtrip time exceeds nominal block duration (currently "
               << avgRoundtrip * 100
               << "%)"
               << endl;
        break;
      case rtripApproaching:
        bciwarn << "Roundtrip time approaches nominal block duration (currently "
               << avgRoundtrip * 100
               << "%)"
               << endl;
        break;
    }

  if( (newState & block) != (mState & block) )
    switch( newState & block )
    {
      case blockLow:
      case blockHigh:
        bciwarn << "Average block duration is off by more than 5%.\n"
                << "Nominal: " << ioSignal( BlockNom, 0 ) << "ms, Actual: " << mBlock.Mean() << "ms";
    }

  mState = newState;
}

} // namespace
