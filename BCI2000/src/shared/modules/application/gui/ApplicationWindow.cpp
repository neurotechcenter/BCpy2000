////////////////////////////////////////////////////////////////////////////////
// $Id: ApplicationWindow.cpp 5865 2019-01-11 16:19:22Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A class for application windows. For more information, see
//   the associated header file.
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
#include "ApplicationWindow.h"
#include "PrecisionTime.h"
#include "Bitmap.h"
#include "BCIStream.h"
#include "SelfPipeQueue.h"
#include "WaitableEvent.h"
#include "Thread.h"

#include "SpinningWheel.h"
#include "Shapes.h"
#include "DisplayStatisticsField.h"
#include "AudioSink.h"

using namespace std;
using namespace GUI;

string ApplicationWindow::DefaultName = "Application";

struct ApplicationWindow::Data
{
  Data( ApplicationWindow*, const std::string& );
  ~Data();
  void ClearObjects();

  ApplicationWindow* mpSelf;
  std::string mName;
  struct
  {
    std::string Left,
                Top,
                Width,
                Height,
                BackgroundColor,
                Info,
                Visualize,
                SpatialDecimation,
                TemporalDecimation;
  } mParamNames;

  AudioSinkRef mpAudio;
  AudioSink::StreamRef mpStream; // will make sure the stream is running

  void OnUpdateReceived();
  void OnUpdateDisplayed();
  MemberCall<void(Data*)> mOnUpdateReceived, mOnUpdateDisplayed;
  SelfPipeQueue<RenderState, 128> mPresentations;

  void OnAudioSound();
  MemberCall<void(Data*)> mOnAudioSound;
  struct AudioPresentation { Time buffered; ClockTick presented; };
  SelfPipeQueue<AudioPresentation, 64> mAudioPresentations;

  int mFrameCountWrap, mStimulusIdWrap;
  std::vector<GraphObject*> mObjects;

  bool mDoVisualize;
  struct Visualization : BitmapVisualization, Thread
  {
    Visualization( ApplicationWindow*, const std::string& );
    ~Visualization();
    void Invalidate() { ++mInvalid; }
    void OnDisplayTick() { mTickEvent.Set(); }
    void SenderThread();

    ApplicationWindow* mpWindow;
    int mWidth, mHeight, mTemporalDecimation;
    Synchronized<int> mInvalid;
    WaitableEvent mTickEvent;

    MemberCall<void( Visualization* )> mOnDisplayTick;
    MemberCall<void( Visualization* )> mOnUpdateDisplayed;
    MemberCall<void( Visualization* )> mSenderThread;
  } mVis;
};

// Visualization
ApplicationWindow::Data::Visualization::Visualization( ApplicationWindow* pWindow, const std::string& name )
: BitmapVisualization( name ), Thread( false, &mSenderThread ),
  mpWindow( pWindow ), mWidth( 0 ), mHeight( 0 ), mTickEvent( true ),
  mOnDisplayTick( &Visualization::OnDisplayTick, this ),
  mOnUpdateDisplayed( &Visualization::Invalidate, this ),
  mSenderThread( &Visualization::SenderThread, this )
{
  mpWindow->AddListener( GraphDisplay::OnTick, &mOnDisplayTick );
  mpWindow->AddListener( DisplayWindow::OnUpdateDisplayed, &mOnUpdateDisplayed );
}

ApplicationWindow::Data::Visualization::~Visualization()
{
  mpWindow->RemoveListener( GraphDisplay::OnTick, &mOnDisplayTick );
  mpWindow->RemoveListener( DisplayWindow::OnUpdateDisplayed, &mOnUpdateDisplayed );
  Thread::TerminateAndWait();
}

void
ApplicationWindow::Data::Visualization::SenderThread()
{
  SendReferenceFrame( mpWindow->DisplayWindow::BitmapData( mWidth, mHeight ) );
  mInvalid = 0;
  int64_t lastTickSent = 0;
  WithThreadPriority( -1 ) while( mTickEvent.Wait() )
  {
    int64_t tick = mpWindow->Tick().count;
    if( tick >= lastTickSent + mTemporalDecimation && mInvalid.Atomic().Exchange( 0 ) )
    {
      lastTickSent = tick;
      SendDifferenceFrame( mpWindow->DisplayWindow::BitmapData( mWidth, mHeight ) );
    }
  }
  SendReferenceFrame( mpWindow->DisplayWindow::BitmapData( mWidth, mHeight ) );
}

// Data
ApplicationWindow::Data::Data( ApplicationWindow* pSelf, const std::string& inName )
: mpSelf( pSelf ),
  mName( inName ),
  mOnUpdateReceived( &Data::OnUpdateReceived, this ),
  mOnUpdateDisplayed( &Data::OnUpdateDisplayed, this ),
  mOnAudioSound( &Data::OnAudioSound, this ),
  mVis( pSelf, inName + "Window" ),
  mDoVisualize( false )
{
  if( mName == DefaultName )
  {
    mParamNames.Left = "WindowLeft";
    mParamNames.Top = "WindowTop";
    mParamNames.Width = "WindowWidth";
    mParamNames.Height = "WindowHeight";
    mParamNames.BackgroundColor = "WindowBackgroundColor";
    mParamNames.Info = "WindowInfo";
    mParamNames.Visualize = "VisualizeApplicationWindow";
    mParamNames.SpatialDecimation = "AppWindowSpatialDecimation";
    mParamNames.TemporalDecimation = "AppWindowTemporalDecimation";
  }
  else
  {
    mParamNames.Left = mName + "WindowLeft";
    mParamNames.Top = mName + "WindowTop";
    mParamNames.Width = mName + "WindowWidth";
    mParamNames.Height = mName + "WindowHeight";
    mParamNames.BackgroundColor = mName + "WindowBackgroundColor";
    mParamNames.Info = mName + "WindowInfo";
    mParamNames.Visualize = string( "Visualize" ) + mName + "Window";
    mParamNames.SpatialDecimation = mName + "WindowSpatialDecimation";
    mParamNames.TemporalDecimation = mName + "WindowTemporalDecimation";
    mpSelf->SetTitle( string( "BCI2000 " ) + mName );
  }
}

ApplicationWindow::Data::~Data()
{
  ClearObjects();
}

void
ApplicationWindow::Data::OnUpdateReceived()
{
  const RenderState& r = *Message().Argument<RenderState*>();
  if( !IsNaN( r.requested ) && r.presentation )
    mPresentations.Push( r );
}

void
ApplicationWindow::Data::OnUpdateDisplayed()
{
  const RenderState& r = *Message().Argument<RenderState*>();
  if( r.presentation )
    mPresentations.Push( r );
}

void
ApplicationWindow::Data::OnAudioSound()
{
  const ClockTick& tick = *Message().Argument<ClockTick*>();
  AudioPresentation p = { TimeUtils::MonotonicTime(), tick };
  mAudioPresentations.Push(p);
}

void
ApplicationWindow::Data::ClearObjects()
{
  for( int i = 0; i < mObjects.size(); ++i )
    delete mObjects[i];
  mObjects.clear();
}

// ApplicationWindow
ApplicationWindow::ApplicationWindow( const std::string& inName )
: d( new Data( this, inName ) )
{
  if( Windows().find( inName ) != Windows().end() )
    bcierr << "A window with name \"" << inName 
           << "\" already exists in application module"
           << endl;
  else
    Windows()[inName] = this;
  GUI::GraphDisplay::SetAssociatedAudio( &d->mpAudio );
}

ApplicationWindow::~ApplicationWindow()
{
  DisplayWindow::Hide();
  if( Windows()[d->mName] == this )
    Windows().erase( d->mName );
  delete d;
}

void
ApplicationWindow::Publish()
{
  struct { const char* id; const std::string& value; } variables[] =
  {
    { "$name$", d->mName },
    { "$width$", d->mParamNames.Width },
    { "$height$", d->mParamNames.Height },
    { "$left$", d->mParamNames.Left },
    { "$top$", d->mParamNames.Top },
    { "$info$", d->mParamNames.Info },
    { "$background$", d->mParamNames.BackgroundColor },
    { "$visualize$", d->mParamNames.Visualize },
    { "$spatialdecimation$", d->mParamNames.SpatialDecimation },
    { "$temporaldecimation$", d->mParamNames.TemporalDecimation },
  };
  const char* parameters[] =
  {
    "Application:$name$%20Window int $width$= 640 640 0 % "
      " // width of $name$ window",
    "Application:$name$%20Window int $height$= 480 480 0 % "
      " // height of $name$ window",
    "Application:$name$%20Window int $left$= 0 0 % % "
      " // screen coordinate of $name$ window's left edge",
    "Application:$name$%20Window int $top$= 0 0 % % "
      " // screen coordinate of $name$ window's top edge",
    "Application:$name$%20Window string $background$= 0x000000 0x505050 % % "
      "// $name$ window background color (color)",
    "Application:$name$%20Window matrix $info$= 1 1 auto "
      "// $name$ window information (readonly)(noedit)",

    "Visualize:$name$%20Window int $visualize$= 0 0 0 1 "
      "// Display miniature copy of $name$ window (boolean)",
    "Visualize:$name$%20Window int $spatialdecimation$= 8 8 1 % "
      "// $name$ window decimation (shrinking) factor",
    "Visualize:$name$%20Window int $temporaldecimation$= 4 16 1 % "
      "// $name$ window time decimation factor",
  };
  for( size_t i = 0; i < sizeof( parameters ) / sizeof( *parameters ); ++i )
  {
    string param = parameters[i];
    for( size_t j = 0; j < sizeof( variables ) / sizeof( *variables ); ++j )
    {
      size_t pos;
      do
      {
        pos = param.find( variables[j].id );
        if( pos != string::npos )
          param = param.substr( 0, pos ) + variables[j].value + param.substr( pos + ::strlen( variables[j].id ) );
      } while( pos != string::npos );
    }
    BEGIN_PARAMETER_DEFINITIONS
      param.c_str(),
    END_PARAMETER_DEFINITIONS
  }

  if( Parameters->Exists( "ShowDisplayStatistics" ) )
  {
    BEGIN_PARAMETER_DEFINITIONS
      "Application:Window int ShowDisplayStatistics= 0 0 0 1 "
        " // add an overlay with statistical information",
    END_PARAMETER_DEFINITIONS
  }
  if( Parameters->Exists( "UpdateRateDecimation" ) )
  {
    BEGIN_PARAMETER_DEFINITIONS
      "Application:Window int UpdateRateDecimation= 1 1 0 % "
        " // only update stimulus display on every nth frame, 0 for adaptive",
    END_PARAMETER_DEFINITIONS
  }
  BEGIN_PARAMETER_DEFINITIONS
    "Application:AudioPlayback float AudioPlaybackRate= auto "
      " // audio playback sampling rate",
    "Application:AudioPlayback int AudioPlaybackChannels= auto "
      " // number of playback output channels",
    "Application:AudioPlayback float AudioPlaybackBufferDuration= auto "
      " // duration of playback buffer",
  END_PARAMETER_DEFINITIONS

  BEGIN_STATE_DEFINITIONS
    "PreStimulusTime 16 0",
    "StimulusTime 16 0",

    "PresentationRequested 32 0",
    "PresentationDisplayed 32 0",
    "PresentationTime 16 0",
    "PresentationFrame 12 0",

    "AudioBufferTime 16 0",
    "AudioPresentationTime 16 0",
  END_STATE_DEFINITIONS
}

void
ApplicationWindow::AutoConfig()
{
  d->mVis.Terminate();
  DisplayWindow::SetLeft( Parameter( d->mParamNames.Left ) );
  DisplayWindow::SetTop( Parameter( d->mParamNames.Top ) );
  DisplayWindow::SetWidth( Parameter( d->mParamNames.Width ) );
  DisplayWindow::SetHeight( Parameter( d->mParamNames.Height ) );
  DisplayWindow::SetColor( RGBColor( Parameter( d->mParamNames.BackgroundColor ) ) );
  DisplayWindow::SetUpdateRateDecimation( OptionalParameter( "UpdateRateDecimation", 1 ) );
  DisplayWindow::Show();
  auto info = DisplayWindow::Info();
  DisplayWindow::Hide();
  auto WindowInfo = Parameter(d->mParamNames.Info);
  WindowInfo->SetDimensions(info.size(), 1);
  int row = 0;
  for(const auto& entry : info)
  {
    WindowInfo->RowLabels()[row] = entry.first;
    WindowInfo(row, 0) = entry.second;
    ++row;
  }

  Parameter( "AudioPlaybackRate" ) = "44.1kHz";
  Parameter( "AudioPlaybackChannels" ) = "2";
  Parameter( "AudioPlaybackBufferDuration" ) = "0";
  float rate = ActualParameter( "AudioPlaybackRate" ).InHertz(),
        buffer = ActualParameter( "AudioPlaybackBufferDuration" ).InSeconds();
  int channels = ActualParameter( "AudioPlaybackChannels" );
  d->mpStream = nullptr;
  d->mpAudio = new AudioSink( Rate::FromHertz( rate ), channels, Ceil( buffer * rate ) );
  AudioSink::StreamRef pStream = d->mpAudio->Stream();
  if( pStream->Error() )
    throw bcierr << pStream->Error();
  d->mpStream = pStream; // will avoid multiple starts and stops

  Parameter( "AudioPlaybackRate" ) << d->mpStream->SampleRate().Hertz() / 1000 << "kHz";
  Parameter( "AudioPlaybackChannels" ) << d->mpStream->PlaybackChannels();
  Parameter( "AudioPlaybackBufferDuration" ) << 1000.0 * d->mpStream->SamplesInBuffer() / d->mpStream->SampleRate().Hertz() << "ms";

  rate = ActualParameter( "AudioPlaybackRate" ).InHertz();
  buffer = ActualParameter( "AudioPlaybackBufferDuration" ).InSeconds();
  channels = ActualParameter( "AudioPlaybackChannels" );
  if( rate != d->mpStream->SampleRate().Hertz() )
    bciwarn << "Audio playback sample rate was requested to be " << rate << "Hz, but is " <<  d->mpStream->SampleRate().Hertz() << "Hz";
  float actualBuffer = d->mpStream->SamplesInBuffer() / d->mpStream->SampleRate().Hertz();
  if( Round( buffer * 500 ) != Round( actualBuffer * 500 ) )
    bciwarn << "Audio playback buffer duration was requested to be " << buffer * 1000 << "ms, but is " << actualBuffer * 1000 << "ms";
  if( channels != d->mpStream->PlaybackChannels() )
    bciwarn << "Number of audio playback channels was requested to be " << channels << ", but is " << d->mpStream->PlaybackChannels();
}

void
ApplicationWindow::Preflight() const
{
  Parameter( "SamplingRate" );
  Parameter( "SampleBlockSize" );
  OptionalParameter( "SpinningWheel" );
  OptionalParameter( "ShowDisplayStatistics" );

  PreflightCondition( State( "PresentationRequested" )->Length() == State( "PresentationDisplayed" )->Length() );
  State( "SourceTime" );
}

void
ApplicationWindow::Initialize()
{
  d->ClearObjects();
  d->mFrameCountWrap = 1 << ( State( "PresentationFrame" )->Length() - 1 );
  d->mStimulusIdWrap = 1 << ( State( "PresentationRequested" )->Length() - 1 );

  if( OptionalParameter( "SpinningWheel", "" ) != string( "" ) )
  {
    SpinningWheel* p = new SpinningWheel( *this, -100 );
    p->SetVelocity( Parameter( "SpinningWheel" ) )
      .SetCenterX( 0.5 ).SetCenterY( 0.5 )
      .SetWidth( 1 ).SetHeight( 1 );
    d->mObjects.push_back( p );
  }
  if( OptionalParameter( "ShowDisplayStatistics" ) != 0 )
  {
    DisplayStatisticsField* p = new DisplayStatisticsField( *this, -101 );
    GUI::Rect r = { 0, 0.9, 1, 1 };
    p->SetColor( RGBColor::None )
      .SetTextColor( RGBColor::Green )
      .SetTextHeight( 0.3 )
      .SetObjectRect( r )
      .SetVisible( true );
    d->mObjects.push_back( p );
  }
  d->mDoVisualize = ( Parameter( d->mParamNames.Visualize ) > 0 );
  int width = 0, height = 0;
  if( d->mDoVisualize )
  {
    d->mVis.mTemporalDecimation = Parameter( d->mParamNames.TemporalDecimation );
    int spatialDecimation = Parameter( d->mParamNames.SpatialDecimation );
    width = DisplayWindow::Context().rect.Width() / spatialDecimation,
    height = DisplayWindow::Context().rect.Height() / spatialDecimation;
    if( width < 1 || height < 1 )
    {
      bciwarn << "Visualization decimation exceeds window size, will not visualize application window";
      d->mDoVisualize = false;
    }
    else
    {
      d->mVis.mWidth = width;
      d->mVis.mHeight = height;
      d->mVis.Send( CfgID::WindowTitle, d->mName + " Window" );
      d->mVis.Send( CfgID::Visible, true );
    }
  }
  DisplayWindow::ConfigureBitmapBuffer( width, height );
  d->mVis.Send( CfgID::Visible, d->mDoVisualize );
}

void
ApplicationWindow::PostInitialize()
{
  DisplayWindow::SetAsyncRendering( true );
  DisplayWindow::Show();
  if( d->mDoVisualize )
    d->mVis.Start();
}

void
ApplicationWindow::StartRun()
{
  PrecisionTime now =  PrecisionTime::Now();
  State( "PreStimulusTime" ) = now;
  State( "StimulusTime" ) = now;
  State( "PresentationRequested" ) = d->mStimulusIdWrap;
  State( "PresentationDisplayed" ) = d->mStimulusIdWrap;
  State( "PresentationFrame" ) = d->mFrameCountWrap;
  State( "PresentationTime" ) = now;
  State( "AudioBufferTime" ) = now;
  State( "AudioPresentationTime" ) = now;
  d->mPresentations.Clear();
  AddListener( DisplayWindow::OnUpdateReceived, &d->mOnUpdateReceived );
  AddListener( DisplayWindow::OnUpdateDisplayed, &d->mOnUpdateDisplayed );
  d->mAudioPresentations.Clear();
  if(d->mpStream)
    d->mpStream.Sink().AddListener( AudioSink::OnSound, &d->mOnAudioSound );
}

void
ApplicationWindow::PostStopRun()
{
  if(d->mpStream)
    d->mpStream.Sink().RemoveListener( AudioSink::OnSound, &d->mOnAudioSound );
  d->mAudioPresentations.Clear();
  RemoveListener( DisplayWindow::OnUpdateReceived, &d->mOnUpdateReceived );
  RemoveListener( DisplayWindow::OnUpdateDisplayed, &d->mOnUpdateDisplayed );
  d->mPresentations.Clear();
  DisplayWindow::Update();
  d->mVis.Invalidate();
}

void
ApplicationWindow::Process()
{
  State("PreStimulusTime") = PrecisionTime::Now();
}

void
ApplicationWindow::PostProcess()
{
  union { uint32_t i; void* p; } sourceTime = { State( "SourceTime" ) | d->mStimulusIdWrap };
  DisplayWindow::Update( sourceTime.p );

  int reqSample = 0, dispSample = 0;
  for( RenderState r; d->mPresentations.Pop( r ); )
  {
    bool displayed = ( r.displayFrame >= 0 );
    uint32_t id = d->mStimulusIdWrap;
    if( r.userData )
    { // Presentation ID encodes both SourceTime state, and request time relative to SourceTime:
      // PresentationID = SourceTime << 8 | ( requestTime - SourceTime ) & 0xff
      // This makes it both unique (within the range of a minute), and allows to extract timing
      // information.
      union { void* p; uint32_t t; } sourceTime = { r.userData };
      sourceTime.t &= ~d->mStimulusIdWrap;
      id = PrecisionTime::UnsignedDiff( PrecisionTime( r.requested.ToMilliseconds() ), PrecisionTime( sourceTime.t ) );
      id &= 0xff;
      id |= sourceTime.t << 8;
    }
    if( r.userData && !displayed && reqSample < Statevector->Samples() - 1 )
    {
      State( "PresentationRequested" )( reqSample ) = id % d->mStimulusIdWrap;
      ++reqSample;
    }
    if( displayed && dispSample < Statevector->Samples() - 1 )
    {
      if( r.userData && !IsNaN( r.requested ) )
        State( "PresentationDisplayed" )( dispSample ) = id % d->mStimulusIdWrap;
      else
        State( "PresentationDisplayed" )( dispSample ) = d->mStimulusIdWrap;
      State( "PresentationFrame" )( dispSample ) = r.displayFrame % d->mFrameCountWrap;
      State( "PresentationTime" )( dispSample ) = PrecisionTime( r.displayed.ToMilliseconds() );
      ++dispSample;
    }
  }
  State( "PresentationRequested" )( reqSample ) = d->mStimulusIdWrap;
  State( "PresentationDisplayed" )( dispSample ) = d->mStimulusIdWrap;
  State( "PresentationFrame" )( dispSample ) = d->mFrameCountWrap;

  int sample = 0;
  for (Data::AudioPresentation p; d->mAudioPresentations.Pop(p); )
  {
    if (sample < Statevector->Samples() - 2)
    {
      State("AudioBufferTime")(sample) = PrecisionTime(p.buffered.ToMilliseconds());
      State("AudioPresentationTime")(sample) = PrecisionTime(p.presented.time.ToMilliseconds());
      ++sample;
    }
  }
  State("StimulusTime") = PrecisionTime::Now();
}

void
ApplicationWindow::Halt()
{
  d->mVis.Terminate();
  d->mpStream = nullptr;
  DisplayWindow::Hide();
}

const ::VisID&
ApplicationWindow::VisID() const
{
  return d->mVis.VisID();
}

ApplicationWindowList&
ApplicationWindow::Windows()
{
  static ApplicationWindowList instance;
  return instance;
}

// ApplicationWindowList definitions
ApplicationWindow*&
ApplicationWindowList::operator[]( const std::string& inName )
{
  return map<string, ApplicationWindow*>::operator[]( inName );
}

ApplicationWindow*
ApplicationWindowList::operator[]( const std::string& inName ) const
{
  const_iterator i = find( inName );
  return i == end() ? NULL : i->second;
}

ApplicationWindow*
ApplicationWindowList::operator[]( int inIdx ) const
{
  const_iterator i = begin();
  int j = 0;
  while( j < inIdx && i != end() )
    ++j, ++i;
  return i == end() ? NULL : i->second;
}

// ApplicationWindowClient definitions
const ApplicationWindowList* const ApplicationWindowClient::Windows = &ApplicationWindow::Windows();

ApplicationWindowClient::ApplicationWindowClient()
{
}

ApplicationWindowClient::~ApplicationWindowClient()
{
  for( WindowSet::const_iterator i = mWindowsAccessed.begin(); i != mWindowsAccessed.end(); ++i )
  {
    ( *i )->UnregisterUser( this );
    if( ( *i )->Users() == 0 )
      delete *i;
  }
}

ApplicationWindow&
ApplicationWindowClient::Window( const string& inName ) const
{
  string name = inName;
  if( name.empty() )
    name = ApplicationWindow::DefaultName;

  ApplicationWindow* pWindow = ( *Windows )[name];
  if( pWindow == NULL )
  { // New windows are legally created on access during the construction
    // phase. Outside the construction phase, we report an error, but still
    // return a valid reference to allow the caller to proceed.
    pWindow = new ApplicationWindow( name );
    if( Environment::Phase() != Environment::construction )
      bcierr_ << "Access to non-existent application window \""
              << name
              << "\""
              << endl;
  }
  pWindow->RegisterUser( this );

  switch( Environment::Phase() )
  {
    case Environment::construction:
    case Environment::preflight:
      break;

    default:
      if( mWindowsAccessed.find( pWindow ) == mWindowsAccessed.end() )
        bcierr_ << "Application window \""
                << name
                << "\" was neither declared during construction, "
                << "nor tested for existence during preflight phase."
                << endl;
  }
  mWindowsAccessed.insert( pWindow );

  return *pWindow;
}
