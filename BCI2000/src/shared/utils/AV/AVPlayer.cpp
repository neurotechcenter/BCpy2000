////////////////////////////////////////////////////////////////////////////////
// $Id$
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A GraphObject for video rendering.
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
#include "AVPlayer.h"

#include "AVDecoder.h"
#include "FileUtils.h"
#include "Lockable.h"
#include "ThreadLocal.h"
#include "ClockTick.h"
#include "SynchronizedQueue.h"

#include <list>
#include <algorithm>

enum { blocked = 0, playing, playingAtEnd, paused };

namespace
{

struct PlayerSet : std::list<AVPlayer*>, Lockable<Mutex>
{
  bool Add( AVPlayer* p )
  {
    if( p ) WithLock( this )
    {
      if( std::find( this->begin(), this->end(), p ) == this->end() )
      {
        this->push_back( p );
        return true;
      }
    }
    return false;
  }
  bool Remove( const AVPlayer* p )
  {
    WithLock( this )
    {
      iterator i = std::find( this->begin(), this->end(), p );
      if( i != this->end() )
      {
        this->erase( i );
        return true;
      }
    }
    return false;
  }
};

struct TaskQueue : Thread, SynchronizedQueue<Runnable*>
{
  ~TaskQueue()
  {
    TerminateAndWait();
  }
  int OnExecute()
  {
    ThreadUtils::Priority::Set( ThreadUtils::Priority::Maximum - 1 );
    while (SynchronizedQueue::Wait())
    {
      SynchronizedQueue<Runnable*>::Consumable task;
      while( task = Consume() )
      {
        SyncMemory<BeforeRead>();
        (*task)->Run();
        delete *task;
        SyncMemory<AfterWrite>();
      }
    }
    return 0;
  }
};

} // namespace

struct AVPlayer::Data : Lockable<NonrecursiveSpinLock>
{
  Data( AVPlayer* );
  ~Data();
  void CloseFile();
  void AdaptFrameRate();

  int64_t DecoderTickTarget( Time ) const;
  int64_t DecoderTickToFrame( int64_t ) const;
  int64_t FrameToDecoderTick( int64_t ) const;
  Time::Interval PlaybackLatency() const;
  void SeekToDecoderTick( int64_t );

  void OnStateChange();
  Synchronized<int> mState;
  Synchronized<Runnable*> mpOnStateChange;
  Lockable<NonrecursiveSpinLock> mSetFile_NoRecursion, mSeek_NoRecursion;

  void AsyncFunctionCall( void (Data::*)() );
  template<typename T> const char* BroadcastFunctionCall( AVPlayer& (AVPlayer::*)( T ), T );
  const char* BroadcastFunctionCall( AVPlayer& (AVPlayer::*)() );
  const char* BroadcastConfiguration();
  const char* BroadcastPlaybackTick( const ClockTick& );

  AVPlayer* PlayerWithCoarsestTimeResolution() const;
  AVPlayer* PlayerWithGreatestLatency() const;

  PlayerSet mTiedPartners, mClockSlaves;

  AVPlayer* mpSelf;
  Synchronized<AVPlayer*> mpClockMaster;

  double mFrameRateAccuracy;
  Rate mNominalFrameRate, mActualFrameRate;
  Ratio mInputFramesPerDecoderTick;
  String mFile, mFileCanonical, mContainer, mCodec, mDeviceInfo, mError;

  ClockTick mRenderingTick, mPlaybackTick, mStartDecoderTick;
  Time mStartTime;
  struct PlayPause { Time play, pause; };
  SynchronizedObject<PlayPause> mPlayPause;
  Time::Interval mInternalLatency, mExternalLatency;
  AVDecoder mDecoder;

  TaskQueue mTasks;
};

// Data
AVPlayer::Data::Data( AVPlayer* p )
: mpSelf( p ),
  mpClockMaster( p ),
  mState( blocked ),
  mInternalLatency( 0 ),
  mExternalLatency( 0 ),
  mRenderingTick( ClockTick::Null ),
  mPlaybackTick( ClockTick::Null ),
  mStartDecoderTick( ClockTick::Null ),
  mStartTime( Time::Never ),
  mInputFramesPerDecoderTick( 1, 1 )
{
  WithLocked( p = mPlayPause.Mutable() )
  {
    p->play = Time::Never;
    p->pause = Time::Never;
  }
}

AVPlayer::Data::~Data()
{
}

void
AVPlayer::Data::CloseFile()
{
  mFile = "";
  mCodec = "";
  mContainer = "";
  mState = blocked;
  mInputFramesPerDecoderTick = Ratio( 1, 1 );
  mDecoder.Close();
  mStartTime = Time::Never;
  WithLocked( p = mPlayPause.Mutable() )
  {
    p->play = Time::Never;
    p->pause = Time::Never;
  }
}

void
AVPlayer::Data::AsyncFunctionCall( void (Data::*func)() )
{
  mpSelf->AbsorbIntoTaskQueue( new MemberCall<void (Data*)>( func, this ) );
}

template<typename T>
const char*
AVPlayer::Data::BroadcastFunctionCall( AVPlayer& (AVPlayer::*func)( T ), T t )
{
  if( func && !mTiedPartners.empty() )
  {
    PlayerSet tied;
    WithLock( mTiedPartners )
      tied = mTiedPartners;
    for( PlayerSet::iterator i = tied.begin(); i != tied.end(); ++i )
      ((*i)->*func)( t );
  }
  return 0;
}

const char*
AVPlayer::Data::BroadcastFunctionCall( AVPlayer& (AVPlayer::*func)() )
{
  if( func && !mTiedPartners.empty() )
  {
    PlayerSet tied;
    WithLock( mTiedPartners )
      tied = mTiedPartners;
    for( PlayerSet::iterator i = tied.begin(); i != tied.end(); ++i )
      ((*i)->*func)();
  }
  return 0;
}

const char*
AVPlayer::Data::BroadcastConfiguration()
{
  for( PlayerSet::iterator i = mTiedPartners.begin(); i != mTiedPartners.end(); ++i )
    (*i)->SetNominalFrameRate( (*i)->NativeFrameRate() * ( mpSelf->FrameRate() / mpSelf->NativeFrameRate() ) );
  return BroadcastFunctionCall<Time::Interval>( &AVPlayer::SeekTo, mpSelf->TimePosition() );
}

AVPlayer*
AVPlayer::Data::PlayerWithCoarsestTimeResolution() const
{
  AVPlayer* p = mpSelf;
  WithLock( mTiedPartners )
    for( PlayerSet::const_iterator i = mTiedPartners.begin(); i != mTiedPartners.end(); ++i )
      if( (*i)->RenderingTick().rate < p->RenderingTick().rate )
        p = *i;
  return p;
}

AVPlayer*
AVPlayer::Data::PlayerWithGreatestLatency() const
{
  AVPlayer* p = mpSelf;
  WithLock( mTiedPartners )
    for( PlayerSet::const_iterator i = mTiedPartners.begin(); i != mTiedPartners.end(); ++i )
      if( (*i)->PlaybackLatency() > p->PlaybackLatency() )
        p = *i;
  return p;
}

const char*
AVPlayer::Data::BroadcastPlaybackTick( const ClockTick& inTick )
{
  for( PlayerSet::iterator i = mClockSlaves.begin(); i != mClockSlaves.end(); ++i )
    (*i)->SetPlaybackTick( inTick );
  return 0;
}

void
AVPlayer::Data::OnStateChange()
{
  Runnable* pOnStateChange = mpOnStateChange;
  if( pOnStateChange )
    pOnStateChange->Run();
}

int64_t
AVPlayer::Data::DecoderTickTarget( Time t ) const
{
  return mStartDecoderTick.count + Round( ( t - mStartTime ) * mStartDecoderTick.rate * mActualFrameRate / mDecoder.InputFrameRate() );
}

void
AVPlayer::Data::AdaptFrameRate()
{
  Rate outputFrameRate = mNominalFrameRate * mDecoder.ResamplingRatio();
  if( !IsNaN( outputFrameRate ) && !IsNaN( mRenderingTick.rate ) )
  {
    Ratio r = outputFrameRate / mRenderingTick.rate,
          adaptation = r.Simplified( mFrameRateAccuracy ) / r;
    mActualFrameRate = mNominalFrameRate * adaptation;
  }
}

int64_t
AVPlayer::Data::FrameToDecoderTick( int64_t frame ) const
{
  return Floor( frame / mInputFramesPerDecoderTick );
}

int64_t
AVPlayer::Data::DecoderTickToFrame( int64_t tick ) const
{
  return Floor( tick * mInputFramesPerDecoderTick );
}

void
AVPlayer::Data::SeekToDecoderTick( int64_t tick )
{
  int oldState = mState.Atomic().Exchange( blocked );
  WithLocked( p = mPlayPause.Mutable() )
  {
    p->play = Time::Never;
    p->pause = Time::Never;
  }
  const char* pErr = 0;
  WithTryLock( mSeek_NoRecursion )
  {
    pErr = mDecoder.SeekToTick( tick ).Error();
    if( !pErr )
      pErr = BroadcastFunctionCall<Time::Interval>( &AVPlayer::SeekTo, mpSelf->TimePosition() );
  }
  if( pErr )
    mpSelf->SetError( pErr );
  else
    mState = paused;
  if( oldState != paused )
    OnStateChange();
}

Time::Interval
AVPlayer::Data::PlaybackLatency() const
{
  return mInternalLatency + mExternalLatency;
}

// AVPlayer
AVPlayer::AVPlayer()
: d( new Data( this ) )
{
  SetFrameRateAccuracy( FrameRateAccuracyDefault() );
}

AVPlayer::~AVPlayer()
{
  Assert( !IsPlaying() );
  PlayerSet slaves;
  WithLock( d->mClockSlaves )
    slaves = d->mClockSlaves;
  for( PlayerSet::iterator i = slaves.begin(); i != slaves.end(); ++i )
    (*i)->SetClockMaster( 0 );

  SetClockMaster( 0 );

  PlayerSet tied;
  WithLock( d->mTiedPartners )
    tied = d->mTiedPartners;
  for( PlayerSet::iterator i = tied.begin(); i != tied.end(); ++i )
    Untie( *i );

  delete d;
}

const String&
AVPlayer::Error() const
{
  SyncMemory<BeforeRead>();
  if( !d->mError && d->mDecoder.Error() )
    d->mError = d->mDecoder.Error();
  return d->mError;
}

AVPlayer&
AVPlayer::SetError( const std::string& s )
{
  if( !s.empty() )
  {
    WithLock( d )
      d->mError = s;
    d->AsyncFunctionCall( &Data::OnStateChange );
  }
  return *this;
}

const String&
AVPlayer::File() const
{
  SyncMemory<BeforeRead>();
  return d->mFile;
}

const String&
AVPlayer::Codec() const
{
  SyncMemory<BeforeRead>();
  return d->mCodec;
}

const String&
AVPlayer::DeviceInfo() const
{
  SyncMemory<BeforeRead>();
  return d->mDeviceInfo;
}

const String&
AVPlayer::ContainerFormat() const
{
  SyncMemory<BeforeRead>();
  return d->mContainer;
}

const Rate&
AVPlayer::NativeFrameRate() const
{
  SyncMemory<BeforeRead>();
  return d->mDecoder.InputFrameRate();
}

Rate
AVPlayer::FrameRate() const
{
  SyncMemory<BeforeRead>();
  return d->mActualFrameRate;
}

AVPlayer&
AVPlayer::SetNominalFrameRate( const Rate& inRate )
{
  WithLock( d )
  {
    Ratio relativeChange = inRate / d->mNominalFrameRate;
    d->mActualFrameRate = d->mActualFrameRate * relativeChange;
    d->mNominalFrameRate = inRate;
    Ratio resamplingRatio = d->mDecoder.ResamplingRatio();
    resamplingRatio = resamplingRatio / relativeChange;
    d->mDecoder.BeginConfiguration();
    d->mDecoder.SetResamplingRatio( resamplingRatio );
    d->mDecoder.EndConfiguration();
  }
  return *this;
}

const Rate&
AVPlayer::NominalFrameRate() const
{
  SyncMemory<BeforeRead>();
  return d->mNominalFrameRate;
}

AVPlayer&
AVPlayer::SetFrameRateAccuracy( double acc )
{
  acc = std::max( acc, 0.0 );
  acc = std::min( acc, FrameRateAccuracyMaximum() );
  WithLock( d )
  {
    d->mFrameRateAccuracy = acc;
    d->AdaptFrameRate();
  }
  return *this;
}

double
AVPlayer::FrameRateAccuracy() const
{
  SyncMemory<BeforeRead>();
  return d->mFrameRateAccuracy;
}

ClockTick
AVPlayer::PlaybackTick() const
{
  SyncMemory<BeforeRead>();
  return d->mPlaybackTick;
}

ClockTick
AVPlayer::RenderingTick() const
{
  SyncMemory<BeforeRead>();
  return d->mRenderingTick;
}

int64_t
AVPlayer::FramePosition() const
{
  return d->DecoderTickToFrame( Decoder().HeadFrameTick() );
}

Time::Interval
AVPlayer::TimePosition() const
{
  return FramePosition() / Decoder().InputFrameRate();
}

Time::Interval
AVPlayer::PlaybackLatency() const
{
  return d->PlaybackLatency();
}

AVPlayer&
AVPlayer::SetInternalPlaybackLatency( Time::Interval t )
{
  Assert( t >= 0 );
  d->mInternalLatency = t;
  return *this;
}

AVPlayer&
AVPlayer::SetExternalPlaybackLatency( Time::Interval t )
{
  if( t < 0 )
    SetError( "ExternalPlaybackLatency must be >= 0" );
  else
    d->mExternalLatency = t;
  return *this;
}

AVPlayer&
AVPlayer::PlayAt( Time t )
{
  if( !IsPlaying() )
  {
    AVPlayer* pCoarse = d->PlayerWithCoarsestTimeResolution(),
             *pLatent = d->PlayerWithGreatestLatency();
    Time earliestPossible = pLatent->RenderingTick().time + pLatent->RenderingTick().rate.Period();
    earliestPossible = std::max( t, earliestPossible );
    Time closestCoarse = earliestPossible;
    if( pCoarse != pLatent )
      closestCoarse = pCoarse->RenderingTick().time + Round( ( earliestPossible - pCoarse->RenderingTick().time ) * pCoarse->RenderingTick().rate ) / pCoarse->RenderingTick().rate;

    PlayerSet tied;
    WithLock( d->mTiedPartners )
      tied = d->mTiedPartners;
    tied.push_back( this );
    for( PlayerSet::iterator i = tied.begin(); i != tied.end(); ++i )
      (*i)->d->mPlayPause.Mutable()->play = closestCoarse;
  }
  return *this;
}

AVPlayer&
AVPlayer::PauseAt( Time t )
{
  if( IsPlaying() )
    d->mPlayPause.Mutable()->pause = t;
  return *this;
}

AVPlayer&
AVPlayer::Play()
{
  return PlayAt( RenderingTick().time - RenderingTick().rate.Period() / 2 );
}

AVPlayer&
AVPlayer::Pause()
{
  return PauseAt( RenderingTick().time - RenderingTick().rate.Period() / 2 );
}

AVPlayer&
AVPlayer::Stop()
{
  SeekTo( 0 );
  return *this;
}

int64_t
AVPlayer::FrameCount() const
{
  if( !IsOpen() )
    return 0;
  if( Decoder().EofPosition().count <= 0 )
    return Maximum<int64_t>();
  return d->DecoderTickToFrame( Decoder().EofPosition().count );
}

Time::Interval
AVPlayer::NativeDuration() const
{
  if( !IsOpen() )
    return 0;
  if( Decoder().EofPosition().count <= 0 )
    return Time::Forever;
  return FrameCount() / Decoder().InputFrameRate();
}

AVPlayer&
AVPlayer::SeekTo( int64_t inNativeFrame )
{
  d->SeekToDecoderTick( d->FrameToDecoderTick( inNativeFrame ) );
  return *this;
}

AVPlayer&
AVPlayer::SeekTo( Time::Interval t )
{
  d->SeekToDecoderTick( Round( t * Decoder().BofPosition().rate ) );
  return *this;
}

bool
AVPlayer::IsPlaying() const
{
  switch( d->mState )
  {
  case playing:
  case playingAtEnd:
    return true;
  }
  return false;
}

bool
AVPlayer::IsOpen() const
{
  return !d->mFile.empty();
}

bool
AVPlayer::Eof() const
{
  return Decoder().EndOfData();
}

AVPlayer&
AVPlayer::SpendDecodingEffort()
{
  d->mDecoder.DecodeToBuffer();
  return *this;
}

AVPlayer&
AVPlayer::LoadIntoMemory()
{
  d->mDecoder.DecodeAllIntoMemory();
  return *this;
}

Runnable*
AVPlayer::OnStateChange() const
{
  return d->mpOnStateChange;
}

AVPlayer&
AVPlayer::SetOnStateChange( Runnable* p )
{
  d->mpOnStateChange = p;
  return *this;
}

AVPlayer&
AVPlayer::SetFile( const std::string& s )
{
  WithTryLock( d->mSetFile_NoRecursion )
  {
    std::string fileCanonical = s.empty() ? "" : FileUtils::CanonicalPath(s);
    if(!FileUtils::IsFile(fileCanonical))
      fileCanonical.clear();
    if(!s.empty() && fileCanonical.empty())
      return SetError( "File \"" + s + "\" does not exist" );

    if( d->mState != blocked && d->mFileCanonical == fileCanonical && !Error() )
      return *this;

    d->mState = blocked;
    d->mError.clear();
    if( s.empty() )
    {
      d->AsyncFunctionCall( &Data::OnStateChange );
      return *this;
    }

    String err;
    if( !err )
    {
      const char* pErr = OnSetFile(s);
      if( pErr )
        err = pErr;
    }
    if( !err )
    {
      d->mFile = s;
      d->mFileCanonical = fileCanonical;
      d->mNominalFrameRate = Decoder().InputFrameRate();
      d->mActualFrameRate = d->mNominalFrameRate;
      d->mInputFramesPerDecoderTick = Decoder().InputFrameRate() / Decoder().BofPosition().rate;
      d->mInputFramesPerDecoderTick.Normalize();
      d->mContainer = Decoder().ContainerFormat();
      d->mCodec = Decoder().Codec();
      Decoder().SeekToTick( 0 );
      Decoder().DecodeToBuffer();
      err = Decoder().Error();
    }
    if( !err )
      err = OnGetDeviceInfo( d->mDeviceInfo );
    if( !err )
    {
      d->mState = paused;
      d->BroadcastFunctionCall<const std::string&>( &AVPlayer::SetFile, d->mFile );
      d->BroadcastConfiguration();
      d->AsyncFunctionCall( &Data::OnStateChange );
    }
    else
      SetError( "Could not open media file \"" + s + "\": " + err );
  }
  return *this;
}

AVPlayer&
AVPlayer::SetPlaybackTick( const ClockTick& t )
{
  const char* pErr = 0;
  bool changed = IsNaN( d->mPlaybackTick.rate ) && !IsNaN( t.rate ) || d->mPlaybackTick.rate != t.rate;
  d->mPlaybackTick = t;
  SyncMemory<AfterWrite>();
  pErr = OnPlaybackTick( d->mPlaybackTick, changed );
  if( pErr )
    SetError( pErr );
  else
    pErr = d->BroadcastPlaybackTick( d->mPlaybackTick );

  if( IsPlaying() )
    pErr = OnAdvance( d->DecoderTickTarget( d->mPlaybackTick.time ) - Decoder().HeadTick() );
  if( pErr )
    SetError( pErr );
  return *this;
}

AVPlayer&
AVPlayer::SetRenderingTick( const ClockTick& t )
{
  const char* pErr = 0;
  bool changed = IsNaN( d->mRenderingTick.rate ) && !IsNaN( t.rate ) || d->mRenderingTick.rate != t.rate;
  d->mRenderingTick = t;
  d->mRenderingTick.time += d->mExternalLatency;
  if( changed )
  {
    d->AdaptFrameRate();
    pErr = d->BroadcastConfiguration();
  }
  enum { nothing, startPlayback, pausePlayback } action = nothing;
  WithLocked( p = d->mPlayPause.Mutable() )
  {
    if( d->mRenderingTick.time >= p->play && d->mRenderingTick.time >= p->pause )
    {
      if( p->play > p->pause )
        action = startPlayback;
      else
        action = pausePlayback;
      p->play = Time::Never;
      p->pause = Time::Never;
    }
    else if( d->mRenderingTick.time >= p->play )
    {
      action = startPlayback;
      p->play = Time::Never;
    }
    else if( d->mRenderingTick.time >= p->pause )
    {
      action = pausePlayback;
      p->pause = Time::Never;
    }
  }
  if( action == startPlayback && paused == d->mState.Atomic().IfEqual( paused ).Exchange( playing ) )
  {
    d->mStartTime = d->mRenderingTick.time;
    d->mStartDecoderTick = d->mDecoder.HeadPosition();
    pErr = OnAdvance( d->DecoderTickTarget( PlaybackTick().time ) - Decoder().HeadTick() );
    d->AsyncFunctionCall( &Data::OnStateChange );
  }
  if( d->mpClockMaster == this )
    SetPlaybackTick( d->mRenderingTick );

  if( !pErr )
    pErr = OnRenderingTick( d->mRenderingTick, changed );
  if( pErr )
    SetError( pErr );

  if( playingAtEnd == d->mState.Atomic().IfEqual( playingAtEnd ).Exchange( paused ) )
  {
    d->mPlayPause.Mutable()->pause = Time::Never;
    d->AsyncFunctionCall( &Data::OnStateChange );
  }
  else if( d->mDecoder.EndOfData() )
    d->mState.Atomic().IfEqual( playing ).Exchange( playingAtEnd );

  if( action == pausePlayback && playing == d->mState.Atomic().IfEqual( playing ).Exchange( paused ) )
  {
    AVPlayer* p = d->PlayerWithCoarsestTimeResolution();
    p->AsyncFunctionCall<int64_t>( &AVPlayer::SeekTo, p->FramePosition() );
  }
  return *this;
}

AVDecoder&
AVPlayer::Decoder()
{
  SyncMemory<BeforeRead>();
  return d->mDecoder;
}

const AVDecoder&
AVPlayer::Decoder() const
{
  SyncMemory<BeforeRead>();
  return d->mDecoder;
}

AVPlayer&
AVPlayer::Tie( AVPlayer* p )
{
  if( p != this && d->mTiedPartners.Add( p ) )
    p->Tie( this );
  return *this;
}

AVPlayer&
AVPlayer::Untie( AVPlayer* p )
{
  if( d->mTiedPartners.Remove( p ) )
    p->Untie( this );
  return *this;
}

AVPlayer&
AVPlayer::SetClockMaster( AVPlayer* pMaster )
{
  if( pMaster != this )
  {
    AVPlayer* p = pMaster;
    while( p && !Error() )
    {
      AVPlayer* q = p->ClockMaster();
      if( q == this )
      {
        SetError( "Circular clock master reference detected" );
        SuggestDebugging << Error();
      }
      else if( q == p )
        p = 0;
      else
        p = q;
    }
  }
  if( !Error() && pMaster != d->mpClockMaster )
  {
    if( d->mpClockMaster )
    {
      d->mpClockMaster->d->mClockSlaves.Remove( this );
      d->mpClockMaster = pMaster;
    }
    if( pMaster && pMaster != this )
      pMaster->d->mClockSlaves.Add( this );
  }
  return *this;
}

AVPlayer*
AVPlayer::ClockMaster()
{
  return d->mpClockMaster;
}

const AVPlayer*
AVPlayer::ClockMaster() const
{
  return d->mpClockMaster;
}

AVPlayer&
AVPlayer::AbsorbIntoTaskQueue( Runnable* p )
{
  d->mTasks.Produce(p);
  return *this;
}

AVPlayer&
AVPlayer::AsyncFunctionCall( AVPlayer& (AVPlayer::*func)() )
{
  Runnable* p = new MemberCall<AVPlayer& ( AVPlayer* )>( func, this );
  return AbsorbIntoTaskQueue( p );
}

