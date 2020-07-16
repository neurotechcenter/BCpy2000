////////////////////////////////////////////////////////////////////////////////
// $Id$
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A base class for video, audio, and other media playing
//   components.
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
#ifndef AV_PLAYER_H
#define AV_PLAYER_H

#include "Runnable.h"
#include "StringUtils.h"
#include "ClockTick.h"
#include "Lockable.h"
#include "Rate.h"

class AVDecoder;

class AVPlayer
{
public:
  AVPlayer();
  virtual ~AVPlayer();

  operator bool() const { return IsOpen(); }
  AVPlayer& Open( const std::string& s ) { return SetFile( s ); }
  AVPlayer& Close() { return SetFile( "" ); }

  // Properties
  AVPlayer& SetFile( const std::string& );
  const String& File() const;
  bool IsOpen() const;
  const String& Error() const;
  const String& ContainerFormat() const;
  const String& Codec() const;
  const String& DeviceInfo() const;

  Runnable* OnStateChange() const;
  AVPlayer& SetOnStateChange( Runnable* );

  // Audio: 1 frame == 1 audio sample x N audio channels
  // Video: 1 frame == 1 video frame == N x M video pixels
  AVPlayer& SetNominalFrameRate( const Rate& );
  const Rate& NominalFrameRate() const;
  const Rate& NativeFrameRate() const;
  Rate FrameRate() const;

  int64_t FrameCount() const;
  Time::Interval NativeDuration() const;
  // Estimated time interval between a frame being read from the decoder's
  // buffer head, and the same frame being physically realized.
  // Includes the ExternalPlaybackLatency value.
  Time::Interval PlaybackLatency() const;
  // Additional latency not visible to software (e.g., physical delays).
  // Must be > 0.
  AVPlayer& SetExternalPlaybackLatency( Time::Interval );

  // Tying two players means to synchronize position and state between them.
  // A tie between players is symmetric, i.e. synchronization operates in
  // both directions.
  AVPlayer& Tie( AVPlayer* );
  AVPlayer& Untie( AVPlayer* );
  // A player uses two distinct types of clocks:
  // 1) The Playback clock's progress determines how the player's frame position
  //    proceeds in time during playback.
  // 2) The Rendering clock represents the rendering sink's demand for data.
  // A player uses its "clock master's" playback tick as its own playback tick.
  // If a player is its own clock master (as by default), it uses its rendering
  // tick as its playback tick.
  // Due to drift and/or limitations in resampling options, synchronous playback
  // of multiple media streams requires defining a clock master in addition
  // to synchronizing position and state by tying the streams' players together.
  AVPlayer& SetClockMaster( AVPlayer* );
  AVPlayer* ClockMaster();
  const AVPlayer* ClockMaster() const;
  ClockTick PlaybackTick() const;
  ClockTick RenderingTick() const;
  // Position: Estimate of currently rendered frame, corrected for latency.
  int64_t FramePosition() const;
  Time::Interval TimePosition() const;
  // Property-style aliases for SeekTo().
  AVPlayer& SetFramePosition( int64_t frame ) { return SeekTo( frame ); }
  AVPlayer& SetTimePosition( Time::Interval time ) { return SeekTo( time ); }

  // FrameRateAccuracy: Relative tolerance when matching native framerate to playback rate.
  // E.g., set to 0.05 in order to allow frame rates between 95% and 105% of
  // the specified one.
  // Tolerance values will be silently clamped to the range between 0 and maximum.
  AVPlayer& SetFrameRateAccuracy( double );
  double FrameRateAccuracy() const;
  static double FrameRateAccuracyDefault() { return 2e-3; /* allow NTSC 29.97 fps roundup to 30 fps */ }
  static double FrameRateAccuracyMaximum() { return 0.03; /* = 3%, about half a musical semitone step */ }

  // Playback state
  AVPlayer& Play();
  AVPlayer& Pause();
  AVPlayer& Stop();
  AVPlayer& PlayAt( Time );
  AVPlayer& PauseAt( Time );
  AVPlayer& SeekTo( int64_t toFrame );
  AVPlayer& SeekTo( Time::Interval );
  bool IsPlaying() const;
  bool Eof() const;

  // Decode input as long as both input data and output buffers are available.
  // Not necessary to call unless you want to preferably spend decoding effort from inside
  // a certain thread or at a certain time.
  AVPlayer& SpendDecodingEffort();
  // Load all into a memory buffer in order to avoid disk access and decoding during playback.
  // Don't do this for large files, or a large number of files.
  AVPlayer& LoadIntoMemory();

protected:
  // Queue a function for asynchronous execution.
  AVPlayer& AsyncFunctionCall( AVPlayer& (AVPlayer::*)() );
  template<class T> AVPlayer& AsyncFunctionCall( AVPlayer& (AVPlayer::*func)( T ), T arg )
  { return AbsorbIntoTaskQueue( new MemberCall<AVPlayer& ( AVPlayer*, T )>( func, this, arg ) ); }

  AVDecoder& Decoder();
  const AVDecoder& Decoder() const;

  AVPlayer& SetError( const std::string& );
  AVPlayer& SetInternalPlaybackLatency( Time::Interval );
  AVPlayer& SetPlaybackTick( const ClockTick& );
  AVPlayer& SetRenderingTick( const ClockTick& );

  virtual const char* OnSetFile( const std::string& ) = 0;
  virtual const char* OnAdvance( int decoderTicks ) = 0;
  virtual const char* OnPlaybackTick( const ClockTick&, bool changed ) { return 0; }
  virtual const char* OnRenderingTick( const ClockTick&, bool changed ) = 0;
  virtual const char* OnGetDeviceInfo( std::string& outInfo ) const { outInfo.clear(); return 0; }

 private:
  AVPlayer& AbsorbIntoTaskQueue( Runnable* );
  struct Data;
  Data* d;
};

#endif // AV_PLAYER_H
