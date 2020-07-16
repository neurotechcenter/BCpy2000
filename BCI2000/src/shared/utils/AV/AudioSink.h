////////////////////////////////////////////////////////////////////////////////
// $Id$
// Author: juergen.mellinger@uni-tuebingen.de
// Description: An object through which multiple AudioPlayer instances may
//   render audio into a single AudioStream.
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
#ifndef AUDIO_SINK_H
#define AUDIO_SINK_H

#include "Broadcaster.h"
#include "AVDecoder.h"
#include "AudioDevice.h"
#include "Uncopyable.h"
#include <memory>

class AudioSink : public Broadcaster, Uncopyable
{
private:
  struct Private;
  struct BufferRef
  {
    explicit BufferRef( AudioSink* p = nullptr );
    BufferRef( const BufferRef& other );
    BufferRef& operator=( const BufferRef& );
    ~BufferRef();
    int Put( const AVDecoder::Chunk& inChunk, int inDupOrDrop, float inVolumeFactor );
    private: AudioSink::Private* mp; float* mpData;
  };

public:
  struct StreamRef
  {
    StreamRef( nullptr_t = nullptr );
    explicit StreamRef( AudioSink* );
    StreamRef( const StreamRef& );
    StreamRef& operator=( const StreamRef& );
    ~StreamRef();
    operator const AudioStream*() const;
    const AudioStream* operator->() const;
    const AudioStream& operator*() const;
    operator AudioStream*();
    AudioStream* operator->();
    AudioStream& operator*();
    AudioSink& Sink();
    const AudioSink& Sink() const;
    private: AudioSink::Private* mp;
  };

  enum { OnTick = 1, OnSound, OnSilence };
  AudioSink( const Rate&, int channels, int samplesInbuffer );
  ~AudioSink();

  BufferRef MixBuffer() { return BufferRef( this ); }
  StreamRef Stream() { return StreamRef( this ); }
  const String& Error() const;

private:
  Private* p;
};

struct AudioSinkRef : std::shared_ptr<AudioSink>
{
  AudioSinkRef( AudioSink* p = nullptr ) : std::shared_ptr<AudioSink>( p ) {}
};

#endif // AUDIO_SINK_H
