/////////////////////////////////////////////////////////////////////////////
// $Id: WavePlayer.cpp 5592 2016-12-13 19:48:23Z mellinger $
// Authors: juergen.mellinger@uni-tuebingen.de,
//          halder@informatik.uni-tuebingen.de
// Description: A now obsolete class for playing wave format audio files.
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
//////////////////////////////////////////////////////////////////////////////
#include "WavePlayer.h"
#include "AudioPlayer.h"

using namespace std;

struct WavePlayer::Data
{
  AudioPlayer mAudioPlayer;
  WavePlayer::Error mErrorState;
  static int sNumInstances;
  Data();
  void Clone( const Data* );
};

int WavePlayer::Data::sNumInstances = 0;

WavePlayer::Data::Data()
: mErrorState( noError )
{
}

WavePlayer::WavePlayer()
: d( new Data )
{
}

WavePlayer::WavePlayer( const WavePlayer& inOther )
: d( new Data )
{
  d->Clone( inOther.d );
}

WavePlayer&
WavePlayer::operator=( const WavePlayer& inOther )
{
  if( &inOther != this )
  {
    delete d;
    d = new Data;
    d->Clone( inOther.d );
  }
  return *this;
}

WavePlayer::~WavePlayer()
{
  delete d;
}

void
WavePlayer::Data::Clone( const WavePlayer::Data* p )
{
  mAudioPlayer.SetFile( p->mAudioPlayer.File() );
  mAudioPlayer.SetVolume( p->mAudioPlayer.Volume() );
}

WavePlayer&
WavePlayer::SetFile( const string& inFileName )
{
  if( d->mAudioPlayer.SetFile( inFileName ).Error() )
    d->mErrorState = fileOpeningError;
  return *this;
}

const string&
WavePlayer::File() const
{
  return d->mAudioPlayer.File();
}

WavePlayer&
WavePlayer::Play()
{
  if( d->mAudioPlayer.Stop().Play().Error() )
    d->mErrorState = genError;
  return *this;
}

WavePlayer&
WavePlayer::SetVolume( float inVolume )
{
  if( d->mAudioPlayer.SetVolume( inVolume ).Error() )
    d->mErrorState = invalidParams;
  return *this;
}

float
WavePlayer::Volume() const
{
  return d->mAudioPlayer.Volume();
}

WavePlayer&
WavePlayer::SetPan( float inPan )
{
  if( inPan != 0 )
    d->mErrorState = invalidParams;
  return *this;
}

WavePlayer&
WavePlayer::Stop()
{
  d->mAudioPlayer.Stop();
  if(d->mAudioPlayer.Error())
    d->mErrorState = genError;
  return *this;
}

bool
WavePlayer::IsPlaying() const
{
  return d->mAudioPlayer.IsPlaying();
}

WavePlayer::Error
WavePlayer::ErrorState() const
{
  return d->mErrorState;
}

