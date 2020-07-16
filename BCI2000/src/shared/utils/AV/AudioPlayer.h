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
#ifndef AUDIO_PLAYER_H
#define AUDIO_PLAYER_H

#include "AVPlayer.h"

class AudioSinkRef;

class AudioControls
{
public:
  enum { Volume, ControlCount };
  AudioControls( void* );
  AudioControls& Set( int, float );
  AudioControls& Set( int, int, float );
  float Get( int ) const;
  float Get( int, int ) const;
private:
  void* p;
};

class AudioPlayer : public AVPlayer
{
public:
  AudioPlayer();
  AudioPlayer( AudioSinkRef );
  virtual ~AudioPlayer();

  AudioSinkRef Sink() const;

  AudioPlayer& SetVolume( float );
  float Volume() const;

  AudioControls& Controls();
  const AudioControls& Controls() const;

  int Channels() const;
  int NativeChannels() const;

  int64_t Samples() const;
  double SamplesPerSecond() const;
  double NativeSamplesPerSecond() const;

  int SampleLag() const;

protected:
  const char* OnSetFile( const std::string& );
  const char* OnRenderingTick( const ClockTick&, bool );
  const char* OnAdvance( int );
  const char* OnGetDeviceInfo( std::string& ) const;

private:
  struct Data;
  Data* d;
};

#endif // AUDIO_PLAYER_H
