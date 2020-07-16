////////////////////////////////////////////////////////////////////////////////
// $Id$
// Author: juergen.mellinger@uni-tuebingen.de
// Description: MediaPlayer is a loose combination of an AudioPlayer and a
//   VideoPlayer which provides a single control interface for all supported
//   media combinations (Audio and Video, Audio only, Video only).
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
#ifndef MEDIA_PLAYER_H
#define MEDIA_PLAYER_H

#include "VideoPlayer.h"
#include "AudioPlayer.h"

class MediaPlayer
{
public:
  enum
  {
    OpenAny = 0,
    OpenVideo = 1,
    OpenAudio = 2,
    OpenBoth = OpenVideo | OpenAudio,
  };

  MediaPlayer( GUI::GraphDisplay& w ) : mVideo( w ), mpControl( 0 ) {}
  ~MediaPlayer() {}

  VideoPlayer& Video() { return mVideo; }
  const VideoPlayer& Video() const { return mVideo; }
  AudioPlayer& Audio() { return mAudio; }
  const AudioPlayer& Audio() const { return mAudio; }
  AVPlayer& Control() { if( mpControl ) return *mpControl; else return mAudio; }
  const AVPlayer& Control() const { if( mpControl ) return *mpControl; else return mAudio; }

  AVPlayer& Open( const std::string file, int mode = OpenAny )
  {
    Close();
    switch( mode )
    {
    case OpenBoth:
      mVideo.Open( file );
      mAudio.Open( file );
      if( mVideo && !mAudio )
        mpControl = &mAudio;
      else if( !mVideo )
        mpControl = &mVideo;
      break;
    case OpenVideo:
      mVideo.Open( file );
      mpControl = &mVideo;
      break;
    case OpenAudio:
      mAudio.Open( file );
      mpControl = &mAudio;
      break;
    default:
      mVideo.Open( file );
      mAudio.Open( file );
      if( !mVideo && mAudio )
        mpControl = &mAudio;
      else if( mVideo )
        mpControl = &mVideo;
      else
        mpControl = &mAudio;
    }
    if( mVideo && mAudio )
    {
      mAudio.Tie( &mVideo );
      mAudio.SetClockMaster( &mVideo );
      mpControl = &mVideo;
    }
    Assert( mpControl );
    return *mpControl;
  }
  MediaPlayer& Close()
  {
    mVideo.Close();
    mAudio.Close();
    mAudio.Untie( &mVideo );
    mAudio.SetClockMaster( &mAudio );
    mVideo.SetClockMaster( &mVideo );
    mpControl = 0;
    return *this;
  }

private:
  VideoPlayer mVideo;
  AudioPlayer mAudio;
  AVPlayer* mpControl;
};

#endif // MEDIA_PLAYER_H
