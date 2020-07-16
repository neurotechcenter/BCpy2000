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
#include "AudioSink.h"

#include "AudioDevice.h"
#include "Runnable.h"
#include "Synchronized.h"
#include "LCRandomGenerator.h"
#include "Debugging.h"

struct AudioSink::Private
{
  Private( AudioSink* );

  void OnStreamTick();
  MemberCall<void(AudioSink::Private*)> mOnStreamTick;
  void OnStreamAbort();
  MemberCall<void(AudioSink::Private*)> mOnStreamAbort;

  void AddStreamRef();
  void ReleaseStream();
  Synchronized<int> mStreamRefCount;

  AudioSink* mpSelf;
  AudioStream mStream;
  LCRandomGenerator mRand;
  std::vector<float> mMixBuffer;
  bool mSilent;
  int mSamplesInBuffer;
};

AudioSink::StreamRef::StreamRef( nullptr_t ) : mp( nullptr ) {}
AudioSink::StreamRef::StreamRef( AudioSink* p ) : mp( p->p ) { if( mp ) mp->AddStreamRef(); }
AudioSink::StreamRef::StreamRef( const StreamRef& other ) : mp( other.mp ) { mp->AddStreamRef(); }
AudioSink::StreamRef& AudioSink::StreamRef::operator=( const StreamRef& other ) { if( mp ) mp->ReleaseStream(); mp = other.mp; if( mp ) mp->AddStreamRef(); return *this; }
AudioSink::StreamRef::~StreamRef() { if( mp ) mp->ReleaseStream(); mp = nullptr; }
AudioSink::StreamRef::operator const AudioStream*() const { return mp ? &mp->mStream : nullptr; }
const AudioStream* AudioSink::StreamRef::operator->() const { Assert( mp ); return &mp->mStream; }
const AudioStream& AudioSink::StreamRef::operator*() const { Assert( mp ); return mp->mStream; }
AudioSink::StreamRef::operator AudioStream*() { return mp ? &mp->mStream : nullptr; }
AudioStream* AudioSink::StreamRef::operator->() { Assert( mp ); return &mp->mStream; }
AudioStream& AudioSink::StreamRef::operator*() { Assert( mp ); return mp->mStream; }
AudioSink& AudioSink::StreamRef::Sink() { Assert( mp ); return *mp->mpSelf; }
const AudioSink& AudioSink::StreamRef::Sink() const { Assert( mp ); return *mp->mpSelf; }

AudioSink::BufferRef::BufferRef( AudioSink* p ) : mp( p->p ), mpData( mp ? mp->mMixBuffer.data() : nullptr ) { mp->AddStreamRef(); }
AudioSink::BufferRef::BufferRef( const BufferRef& other ) : mp( other.mp ), mpData( other.mpData ) { mp->AddStreamRef(); }
AudioSink::BufferRef::~BufferRef() { if( mp ) mp->ReleaseStream(); }

int
AudioSink::BufferRef::Put( const AVDecoder::Chunk& inChunk, int inDupOrDrop, float inVolumeFactor )
{
  const float* pBegin, *pEnd;
  if( !inChunk.GetData( pBegin, pEnd ) )
    return 0;
  const float* pBegin1 = pBegin, *pEnd1 = pEnd, *pBegin2 = pEnd1, *pEnd2 = pBegin2;
  int framesToWrite = inChunk.FrameCount() + inDupOrDrop;
  if( framesToWrite < 1 )
    return 0;
  if( inDupOrDrop != 0 )
  {
    int channels = mp->mStream.PlaybackChannels(),
        cut = mp->mRand( inChunk.FrameCount() );
    if( inDupOrDrop > 0 )
      cut += 1;
    pEnd1 = pBegin1 + channels * cut;
    pBegin2 = pEnd1 - inDupOrDrop * channels;
    pEnd2 = pBegin2 +  channels * (framesToWrite - cut);
  }
#if 0
  Assert( pEnd1 >= pBegin1 );
  Assert( pEnd2 >= pBegin2 );
  Assert( pBegin1 <= pBegin2 );
  Assert( pBegin1 >= pBegin && pEnd1 <= pEnd );
  Assert( pBegin2 >= pBegin && pEnd2 <= pEnd );
  Assert( mpMixBufferWrite + (pEnd1-pBegin1) + (pEnd2-pBegin2) <= mMixBuffer.data() + mMixBuffer.size() );
#endif
  for( const float* pSrc = pBegin1; pSrc < pEnd1; ++pSrc )
    *mpData++ += *pSrc * inVolumeFactor;
  for( const float* pSrc = pBegin2; pSrc < pEnd2; ++pSrc )
    *mpData++ += *pSrc * inVolumeFactor;
  return framesToWrite;
}

AudioSink::Private::Private( AudioSink* p )
: mpSelf( p ),
  mOnStreamTick( &AudioSink::Private::OnStreamTick, this ),
  mOnStreamAbort( &AudioSink::Private::OnStreamAbort, this ),
  mSilent( true )
{
}

void
AudioSink::Private::AddStreamRef()
{
  if( ++mStreamRefCount == 1 )
  {
    int samples = mSamplesInBuffer;
    mStream.SetSamplesInBuffer( samples );
    mStream.Start();
    if( mStream.Error() )
      mStream.Destroy();
  }
}

void
AudioSink::Private::ReleaseStream()
{
  if( --mStreamRefCount == 0 )
  {
    mStream.Stop();
    mSilent = true;
  }
}

void
AudioSink::Private::OnStreamTick()
{
  mMixBuffer.resize( mStream.SamplesInBuffer() * mStream.PlaybackChannels() );
  ::memset( mMixBuffer.data(), 0, mMixBuffer.size() * sizeof(*mMixBuffer.data()) );
  ClockTick tick = mStream.PlaybackTick();
  mpSelf->Broadcaster::Emit( OnTick, &tick );
  mStream.Write( mMixBuffer.data(), mStream.SamplesInBuffer() );
  if( mSilent )
  {
    const auto* p = mMixBuffer.data(), *pEnd = p + mMixBuffer.size();
    while( p < pEnd && *p == 0 )
      ++p;
    if( p < pEnd )
    {
      mSilent = false;
      tick.time += tick.rate.Period() * Ratio( p - mMixBuffer.data(), mMixBuffer.size() );
      mpSelf->Broadcaster::Emit( OnSound, &tick );
    }
  }
  else
  {
    const auto* pBegin = mMixBuffer.data(), *pEnd = pBegin + mMixBuffer.size(), *p = pEnd;
    while( p > pBegin && *(p - 1) == 0 )
      --p;
    if( p < pEnd )
    {
      mSilent = true;
      tick.time += tick.rate.Period() * Ratio( p - mMixBuffer.data(), mMixBuffer.size() );
      mpSelf->Broadcaster::Emit( OnSilence, &tick );
    }
  }
}

void
AudioSink::Private::OnStreamAbort()
{
  std::cerr << "Audio stream aborted with error: " << mStream.Error() << std::endl;
}


AudioSink::AudioSink( const Rate& r, int ch, int s )
: p( new Private( this ) )
{
  p->mSamplesInBuffer = s;
  p->mStream.SetSampleRate( r )
            .SetPlaybackChannels( ch )
            .SetRecordingChannels( 0 );
  p->mStream.AddListener( AudioStream::OnTick, &p->mOnStreamTick );
  p->mStream.AddListener( AudioStream::OnAbort, &p->mOnStreamAbort );
}

AudioSink::~AudioSink()
{
  p->mStream.RemoveListener( AudioStream::OnAbort, &p->mOnStreamAbort );
  p->mStream.RemoveListener( AudioStream::OnTick, &p->mOnStreamTick );
  p->mStream.Stop();
  delete p;
}

const String&
AudioSink::Error() const
{
  return p->mStream.Error();
}
