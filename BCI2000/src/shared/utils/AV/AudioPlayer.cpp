////////////////////////////////////////////////////////////////////////////////
// $Id$
// Author: juergen.mellinger@uni-tuebingen.de
// Description: An object that plays audio content into an AudioSink.
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
#include "AudioPlayer.h"

#include "AudioSink.h"
#include "AVDecoder.h"
#include "FileUtils.h"
#include "Lockable.h"
#include "LCRandomGenerator.h"
#include "StaticObject.h"

namespace
{

static const int cDupsOrDropsPerSecond = 10;

AudioSinkRef DefaultSink()
{
  static AudioSinkRef instance( new AudioSink( Rate::FromHertz( 44100 ), 2, 441 ) );
  return instance;
}

} // namespace

struct AudioPlayer::Data
{
  Data( AudioPlayer* );
  ~Data();
  void Initialize( AudioSinkRef );

  void OnSinkTick();
  MemberCall<void(AudioPlayer::Data*)> mOnSinkTick;
  void OnStreamAbort();
  MemberCall<void(AudioPlayer::Data*)> mOnStreamAbort;
  const AudioStream* EnsureStreamStarted();

  AudioPlayer* mpSelf;
  int mNativeChannels, mNativeSampleLag;
  SynchronizedObject<ClockTick> mPositionTick;
  LCRandomGenerator mRand;
  AudioSinkRef mpSink;
  AudioSink::StreamRef mpStream;

  struct AudioSettings
  {
    AudioSettings();
    void Update();

    float mVolumeFactor;
    AudioControls mControls;
    float mValues[AudioControls::ControlCount];
    int32_t mChanged;
  } mSettings;
};

// Data
AudioPlayer::Data::Data( AudioPlayer* p )
: mpSelf( p ),
  mNativeChannels( 0 ),
  mNativeSampleLag( 0 ),
  mPositionTick( ClockTick::Null ),
  mOnSinkTick( &AudioPlayer::Data::OnSinkTick, this ),
  mOnStreamAbort( &AudioPlayer::Data::OnStreamAbort, this )
{
}

void
AudioPlayer::Data::Initialize( AudioSinkRef pSink )
{
  mpSink = pSink ? pSink : DefaultSink();
  mpSink->AddListener( AudioSink::OnTick, &mOnSinkTick );
  mpSink->Stream()->AddListener( AudioStream::OnAbort, &mOnStreamAbort );
}

AudioPlayer::Data::~Data()
{
  mpSink->Stream()->RemoveListener( AudioStream::OnAbort, &mOnStreamAbort );
  mpSink->RemoveListener( AudioSink::OnTick, &mOnSinkTick );
}

AudioPlayer::Data::AudioSettings::AudioSettings()
: mControls( &mValues ),
  mChanged( true )
{
  ::memset( &mValues, 0, sizeof(mValues) );
  mValues[AudioControls::Volume] = 1;
}

void
AudioPlayer::Data::OnSinkTick()
{
  SyncMemory<BeforeRead>();
  mpSelf->SetRenderingTick( *Broadcaster::Message().Argument<const ClockTick*>() );
}

void
AudioPlayer::Data::OnStreamAbort()
{
  mpStream = nullptr;
}

void
AudioPlayer::Data::AudioSettings::Update()
{
  SyncMemory<BeforeRead>();
  if( mChanged )
  {
    mChanged = false;
    float& f = mValues[AudioControls::Volume];
    if( f > 1 )
      f = 1;
    if( f < 0 )
      f = 0;
    if( f == 1 || f == 0 )
      mVolumeFactor = f;
    else
    {
      float dB = -96 * ( 1 - f );
      mVolumeFactor = ::pow( 10.f, dB / 20.f );
    }
    SyncMemory<AfterWrite>();
  }
}

const AudioStream*
AudioPlayer::Data::EnsureStreamStarted()
{
  SyncMemory<BeforeRead>();
  auto pStream = mpSink->Stream();
  if( pStream->Error() )
  {
    String err = pStream->DeviceInfo();
    if( !err )
      err = "Error opening audio output";
    err += ": " + pStream->Error();
    mpSelf->AVPlayer::SetError( err );
    pStream = nullptr;
  }
  mpStream = pStream;
  SyncMemory<AfterWrite>();
  return pStream;
}

// AudioPlayer
AudioPlayer::AudioPlayer()
: d( new Data( this ) )
{
  d->Initialize( nullptr );
  SetFrameRateAccuracy( 1e-6 );
}

AudioPlayer::AudioPlayer( AudioSinkRef pSink )
: d( new Data( this ) )
{
  d->Initialize( pSink );
  SetFrameRateAccuracy( 1e-6 );
}

AudioPlayer::~AudioPlayer()
{
  Close();
  SetOnStateChange( 0 );
  delete d;
}

int
AudioPlayer::NativeChannels() const
{
  SyncMemory<BeforeRead>();
  return d->mNativeChannels;
}

int
AudioPlayer::Channels() const
{
  SyncMemory<BeforeRead>();
  return d->mpSink->Stream()->PlaybackChannels();
}

double
AudioPlayer::SamplesPerSecond() const
{
  return ( FrameRate() * Time::OneSecond ).To<double>();
}

double
AudioPlayer::NativeSamplesPerSecond() const
{
  return ( NativeFrameRate() * Time::OneSecond ).To<double>();
}

AudioControls&
AudioPlayer::Controls()
{
  return d->mSettings.mControls;
}

const AudioControls&
AudioPlayer::Controls() const
{
  return d->mSettings.mControls;
}

AudioPlayer&
AudioPlayer::SetVolume( float f )
{
  Controls().Set( AudioControls::Volume, f );
  return *this;
}

float
AudioPlayer::Volume() const
{
  return Controls().Get( AudioControls::Volume );
}

int
AudioPlayer::SampleLag() const
{
  return d->mNativeSampleLag;
}

const char*
AudioPlayer::OnSetFile( const std::string& inFile )
{
  const AudioStream* pStream = nullptr;
  Decoder().BeginConfiguration();
  if( !Decoder().Open( inFile, AVDecoder::Audio, 0 ).Error() )
  {
    d->mNativeChannels = Decoder().FrameDimension( 0 );
    if( !inFile.empty() )
      pStream = d->EnsureStreamStarted();
    if( pStream )
    {
      SetInternalPlaybackLatency( pStream->PlaybackLatency() );
      Decoder().SetFrameDimension( 0, pStream->PlaybackChannels() );
      Decoder().SetResamplingRatio( pStream->SampleRate() / Decoder().InputFrameRate() );
    }
  }
  if( !pStream )
    d->mpStream = nullptr;
  Decoder().EndConfiguration();
  return AVPlayer::Error() ? AVPlayer::Error() : Decoder().Error();
}

const char*
AudioPlayer::OnRenderingTick( const ClockTick&, bool )
{
  auto pStream = d->mpSink->Stream();
  if( pStream->Error() )
    return pStream->Error();

  int dupOrDrop = 0;
  if( IsPlaying() )
  {
    int64_t renderingDecoderTick = Decoder().HeadTick() + Round( pStream->SamplesInBuffer() / Decoder().ResamplingRatio() );
    int64_t nativeSampleDelta;
    Time::Interval timeDelta;
    WithLocked( t = d->mPositionTick.Const() )
    {
      nativeSampleDelta =  t->count - renderingDecoderTick;
      timeDelta = t->time - RenderingTick().time;
    }
    int sampleLag = Round( nativeSampleDelta * Decoder().ResamplingRatio() ) - Round( timeDelta * pStream->SampleRate() );
    d->mNativeSampleLag = Round( sampleLag / Decoder().ResamplingRatio() );
    static const Time::Interval tolerance = Time::Interval::FromMilliseconds( 10 );
    if( ::abs( sampleLag ) >= Ceil( pStream->SampleRate() * tolerance ) )
    {
      int castsPerSecond = RenderingTick().rate.Hertz();
      if( d->mRand( castsPerSecond ) < cDupsOrDropsPerSecond )
        dupOrDrop = ( sampleLag < 0 ? 1 : -1 );
    }
  }
  if( IsPlaying() )
  {
    auto mixBuffer = d->mpSink->MixBuffer();
    int remaining = pStream->SamplesInBuffer();
    while( IsPlaying() && remaining > 0 && !Decoder().EndOfData() )
    {
      if( remaining - dupOrDrop < 1 )
        dupOrDrop = 0;
      while( !Decoder().AreFramesAvailable() && !Decoder().EndOfData() )
        SpendDecodingEffort();
      AVDecoder::Chunk chunk = Decoder().GetFrames( remaining - dupOrDrop );
      d->mSettings.Update();
      remaining -= mixBuffer.Put( chunk, dupOrDrop, d->mSettings.mVolumeFactor );
      dupOrDrop = 0;
      int frames = chunk.FrameCount();
      chunk.Release();
      while( frames > 0 && !Decoder().EndOfData() )
      {
        if( !Decoder().AreFramesAvailable() )
          SpendDecodingEffort();
        frames -= Decoder().AdvanceByFrames( frames );
      }
    }
  }
  if( !IsPlaying() )
  {
    d->mNativeSampleLag = 0;
    d->mPositionTick.Mutable()->time = Time::Never;
  }
  //SpendDecodingEffort(); // race condition when opening files
  return Decoder().Error();
}

const char*
AudioPlayer::OnAdvance( int inDecoderTicks )
{
  Time time = PlaybackTick().time;
  int64_t count = Decoder().HeadTick() + inDecoderTicks;
  WithLocked( t = d->mPositionTick.Mutable() )
  {
    t->time = time;
    t->count = count;
  }
  return 0;
}

const char*
AudioPlayer::OnGetDeviceInfo( std::string& outInfo ) const
{
  outInfo = d->mpSink->Stream()->DeviceInfo();
  if( outInfo.empty() )
    return "Not connected to a device";
  return 0;
}

// AudioControls
AudioControls::AudioControls( void* p )
: p( p )
{
}

AudioControls&
AudioControls::Set( int idx, float f )
{
  if( idx >= 0 && idx < ControlCount )
  {
    union { void* v; float* f; int32_t* i; } values = { p };
    values.f[idx] = f;
    values.i[ControlCount] = 1;
    SyncMemory<AfterWrite>();
  }
  return *this;
}

float
AudioControls::Get( int idx ) const
{
  if( idx >= 0 && idx < ControlCount )
  {
    union { void* v; float* f; int32_t* i; } values = { p };
    SyncMemory<BeforeRead>();
    return values.f[idx];
  }
  return 0;
}

