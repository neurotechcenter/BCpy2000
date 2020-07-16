////////////////////////////////////////////////////////////////////////////////
// $Id$
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A stimulus that plays audio and/or video when being presented.
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
#include "AVStimulus.h"

#include "AudioPlayer.h"
#include "AudioSink.h"
#include "SpeechStimulus.h"
#include "StringUtils.h"
#include "FileUtils.h"
#include "BCIStream.h"
#include "GraphDisplay.h"

static const char cSpeechQuote = '\'';
static const char cStreamSeparator = ':';

struct AVStimulus::Data
{
  String mError, mSource;
  double mVolume;
  SpeechStimulus mSpeech;
  Stimulus* mpStimulus;
  AudioPlayer mAudioPlayer;
  bool mAbortOnConceal;
  struct : Runnable
  {
    AVStimulus::Data* pSelf;
    AVPlayer* pControl;
    void OnRun() { OnPlayerStateChange(); }
    void OnPlayerStateChange()
    {
      Assert( pControl );
      if( !pControl->IsPlaying() && pControl->FramePosition() > 0 )
        pControl->Stop();
      if( pControl->Error() && !pSelf->mError )
        pSelf->mError = pControl->Error();
    }
  } mPlayer;
  Data( AudioSinkRef* ppSink ) : mAudioPlayer( ppSink ? *ppSink : nullptr ) {}
};

AVStimulus::AVStimulus( GUI::GraphDisplay& display )
: VideoPlayer( display, AVStimulusZOrder ),
  d( new Data( display.AssociatedAudio() ) )
{
  d->mpStimulus = this;
  d->mAbortOnConceal = false;
  AVStimulus::SetVolume( 1.0 );
  VideoPlayer::SetVisible( false );
  d->mPlayer.pSelf = d;
  d->mPlayer.pControl = 0;
  if( d->mAudioPlayer.Error() )
    d->mError = d->mAudioPlayer.Error();
  else
   d->mError = VideoPlayer::Error();
}

AVStimulus::~AVStimulus()
{
  SetSource( "" );
  delete d;
}

const String&
AVStimulus::AudioDevice() const
{
  return d->mAudioPlayer.DeviceInfo();
}

const String&
AVStimulus::VideoDevice() const
{
  return VideoPlayer::DeviceInfo();
}

const String&
AVStimulus::Error() const
{
  if( !d->mError && d->mPlayer.pControl )
    d->mError = d->mPlayer.pControl->Error();
  return d->mError;
}

AVStimulus&
AVStimulus::SetSource( const std::string& inSource )
{
  if( d->mPlayer.pControl )
  {
    d->mPlayer.pControl->SetOnStateChange( 0 );
    d->mPlayer.pControl->Close();
    d->mPlayer.pControl = 0;
    VideoPlayer::Untie( &d->mAudioPlayer );
    VideoPlayer::SetClockMaster( this );
    d->mAudioPlayer.SetClockMaster( &d->mAudioPlayer );
  }
  d->mSource = StringUtils::Trim( inSource );
  d->mSpeech.SetText( "" );
  d->mError = "";
  d->mpStimulus = 0;
  if( !d->mSource.empty() )
  {
    size_t length = d->mSource.length();
    if( length >= 2 && d->mSource.at( 0 ) == cSpeechQuote && d->mSource.at( length - 1 ) == cSpeechQuote )
    {
      d->mpStimulus = &d->mSpeech;
      d->mSpeech.SetText( d->mSource.substr( 1, length - 2 ) );
    }
    else if( VideoPlayer::Open( d->mSource ) )
    {
      d->mpStimulus = this;
      if( d->mAudioPlayer.Open( d->mSource ) )
      {
        VideoPlayer::Tie( &d->mAudioPlayer );
        d->mAudioPlayer.SetClockMaster( this );
        d->mPlayer.pControl = &d->mAudioPlayer;
      }
      else
        d->mPlayer.pControl = this;
    }
    else if( d->mAudioPlayer.Open( d->mSource ) )
    {
      d->mpStimulus = this;
      d->mPlayer.pControl = &d->mAudioPlayer;
      if( d->mAudioPlayer.NativeDuration() < Time::Seconds( 5 ) )
        d->mAudioPlayer.LoadIntoMemory();
    }
    else if( d->mError.empty() )
      d->mError = d->mAudioPlayer.Error();
  }
  if( d->mPlayer.pControl )
    d->mPlayer.pControl->SetOnStateChange( &d->mPlayer );
  return *this;
}

const String&
AVStimulus::Source() const
{
  return d->mSource;
}

AVStimulus&
AVStimulus::SetVolume( double inVolume )
{
  if( inVolume < 0 || inVolume > 1 )
  {
    d->mError = "Volume out of range, must be between 0 and 1";
    return *this;
  }
  d->mVolume = inVolume;
  d->mAudioPlayer.SetVolume( inVolume );
  d->mSpeech.SetVolume( inVolume );
  return *this;
}

double
AVStimulus::Volume() const
{
  return d->mVolume;
}

AVStimulus&
AVStimulus::SetAbortOnConceal( bool b )
{
  d->mAbortOnConceal = b;
  return *this;
}

bool
AVStimulus::AbortOnConceal() const
{
  return d->mAbortOnConceal;
}

void
AVStimulus::OnPresent()
{
  if( d->mpStimulus == this )
  {
    d->mPlayer.pControl->Stop().Play();
    if( d->mPlayer.pControl->Error() && !d->mError )
      d->mError = d->mPlayer.pControl->Error();
    VideoPlayer::Show();
  }
  else if( d->mpStimulus )
    d->mpStimulus->Present();
  if( Error() )
    throw bcierr << Error();
}

void
AVStimulus::OnConceal()
{
  if( d->mpStimulus == this )
  {
    if( d->mAbortOnConceal )
      d->mPlayer.pControl->Stop();
    VideoPlayer::Hide();
  }
  else if( d->mpStimulus )
    d->mpStimulus->Conceal();
  if( Error() )
    throw bcierr << Error();
}

