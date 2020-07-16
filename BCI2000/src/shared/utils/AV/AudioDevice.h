////////////////////////////////////////////////////////////////////////////////
// $Id$
// Author: juergen.mellinger@uni-tuebingen.de
// Description: Classes for playback/recording from/to a physical audio device.
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
#ifndef AUDIO_DEVICE_H
#define AUDIO_DEVICE_H

#include "ClockTick.h"
#include "Rate.h"
#include "Broadcaster.h"
#include "Uncopyable.h"
#include "StringUtils.h"

class AudioDevice
{
public:
  static bool Rescan();
  static AudioDevice First()
    { return AudioDevice( 0 ); }
  AudioDevice Next() const
    { return AudioDevice( mIndex + 1 ); }

  AudioDevice( int index = -1 );
  bool Exists() const;
  bool Active() const;

  String Name() const;
  bool SupportsRecording() const;
  bool SupportsPlayback() const;

private:
  int mIndex;
  friend class AudioStream;
};

class AudioStream : public Broadcaster, Uncopyable
{
public:
  explicit AudioStream( AudioDevice& = AudioDevice( -1 ) );
  ~AudioStream();

  const String& Error() const;
  const String& DeviceInfo() const;

  int RecordingChannels() const;
  AudioStream& SetRecordingChannels( int );

  int PlaybackChannels() const;
  AudioStream& SetPlaybackChannels( int );

  const Rate& SampleRate() const;
  AudioStream& SetSampleRate( const Rate& );

  // When set to zero, buffer size and tick interval
  // might not be constant across callback calls.
  int SamplesInBuffer() const;
  AudioStream& SetSamplesInBuffer( int );

  AudioStream& Create();
  AudioStream& Destroy();

  AudioStream& Start( Time::Interval timeout = Time::OneSecond );
  AudioStream& Stop();
  bool Active() const;

  // Tick with time at which the current buffer's oldest sample was sampled
  ClockTick RecordingTick() const;
  // Tick with time at which current buffer will start to be played
  ClockTick PlaybackTick() const;

  Time::Interval RecordingLatency() const;
  AudioStream& SetRecordingLatency( Time::Interval );
  Time::Interval PlaybackLatency() const;
  AudioStream& SetPlaybackLatency( Time::Interval );

  enum Events { OnTick = 1, OnAbort };
  bool OnTickTimedOut() const;

  int Read( float*, int samples );
  int Write( const float*, int samples );

private:
  struct Data;
  Data* d;
};

#endif // AUDIO_DEVICE_H
