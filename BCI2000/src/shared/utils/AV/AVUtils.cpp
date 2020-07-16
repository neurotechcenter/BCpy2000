////////////////////////////////////////////////////////////////////////////////
// $Id$
// Author: juergen.mellinger@uni-tuebingen.de
// Description: Helper classes for interfacing with LibAV.
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
#include "AVUtils.h"

#include "Debugging.h"
#include "Lockable.h"
#include "StringUtils.h"
#include "ThreadLocal.h"
#include "BCIStream.h"
#include <cstdlib>

#ifndef AVDEBUG
# define AVDEBUG 0
#endif

#if _MSC_VER
# define snprintf _snprintf
#endif

using namespace AVUtils;

namespace
{

FracTick& operator+=( FracTick& t, const Ratio& r )
{
  t.intpart += r.Floor();
  t.fracpart += r.Frac();
  if( t.fracpart >= 1 )
    t.fracpart -= 1, t.intpart += 1;
  return t;
}

FracTick& operator-=( FracTick& t, const Ratio& r )
{
  t.intpart -= r.Floor();
  t.fracpart -= r.Frac();
  if( t.fracpart < 0 )
    t.fracpart += 1, t.intpart -= 1;
  return t;
}

FracTick operator+( const FracTick& t, const Ratio& r )
{
  FracTick result = t;
  return result += r;
}

FracTick operator-( const FracTick& t, const Ratio& r )
{
  FracTick result = t;
  return result -= r;
}

double operator-( const FracTick& t1, const FracTick& t2 )
{
  return 1.0 * ( t1.intpart - t2.intpart ) + ( t1.fracpart - t2.fracpart );
}

struct LogData { LogData* pNext; std::string content; int level; };
ThreadLocal<LogData*> stpLog;

void AVLog( void* inContext, int inLevel, const char* inFormat, va_list inArgs )
{
  if( stpLog )
  {
    char buf[1024];
    int len = sizeof(buf), pos = 0;
    if (inContext)
      pos += ::snprintf( buf + pos, len - pos, "%s: ", ::av_default_item_name(inContext));
    if (inFormat)
      pos += ::vsnprintf( buf + pos, len - pos, inFormat, inArgs );
    buf[pos] = 0;
    stpLog->content.append( StringUtils::Trim( buf ) );
#if AVDEBUG
    bciout << "Libav: " << buf;
    ::av_log_set_level( INT_MAX );
#endif
  }
};

int Init()
{
#if AVDEBUG
  ::av_log_set_level( INT_MAX );
#else
  ::av_log_set_level( AV_LOG_QUIET );
#endif
  ::av_log_set_callback( &AVLog );
  return 1;
}
int init_ = Init();

} // namespace

// Log
struct Log::Data : LogData
{
};

Log::Log( int level )
: d( new Data )
{
  d->level = level;
  d->pNext = stpLog;
  stpLog = d;
  ::av_log_set_level( level );
}

Log::~Log()
{
  if( d->pNext )
  {
#if !AVDEBUG
    ::av_log_set_level( d->pNext->level );
#endif
    d->pNext->content.append( d->content );
  }
  else
  {
#if !AVDEBUG
    ::av_log_set_level( AV_LOG_QUIET );
#endif
  }
  stpLog = d->pNext;
  delete d;
}

const char*
Log::Current()
{
  return stpLog ? stpLog->content.c_str() : 0;
}

// Frame
Frame::Frame()
: p( ::av_frame_alloc() ), pos( ClockTick::Null )
{}

Frame::~Frame()
{
  ::av_frame_free( &p );
}

// FrameProcessor
template<class T>
FrameProcessor::FrameProcessor( T* objs, size_t count )
: mBuffersUsed( 0 ),
  mWriteBufferIdx( 0 ),
  mReadBufferIdx( 0 )
{
  for( size_t i = 0; i < count; ++i )
  {
    Buffer b = { 0 };
    b.pBufferObject = objs + i;
    mBuffers.push_back( b );
  }
}

FrameProcessor::~FrameProcessor()
{
  WithLock( mAccess )
    for( size_t i = 0; i < mBuffers.size(); ++i )
      while( mBuffers[i].refCount > 0 )
        ThreadUtils::SleepForMs( 1 );
}

int
FrameProcessor::BuffersUsed() const
{
  return mBuffersUsed;
}

int
FrameProcessor::BuffersFree() const
{
  SyncMemory<BeforeRead>();
  return mBuffers.size() - mBuffersUsed;
}

int
FrameProcessor::FramesBuffered() const
{
  SyncMemory<BeforeRead>();
  int bytes = 0;
  for( size_t i = 0; i < mBuffersUsed; ++ i )
  {
    const Buffer& b = mBuffers[(mReadBufferIdx + i) % mBuffers.size()];
    bytes += ( b.pEnd - b.pRead );
  }
  return bytes / BytesPerFrame();
}

int64_t
FrameProcessor::HeadTick() const
{
  SyncMemory<BeforeRead>();
  const Buffer& b = mBuffers[mReadBufferIdx];
  return b.tick.intpart + Round( b.tick.fracpart );
}

void
FrameProcessor::AssignHeadTick( int64_t inTick )
{
  Buffer& b = mBuffers[mReadBufferIdx];
  b.tick.intpart = inTick;
  b.tick.fracpart = 0;
  SyncMemory<AfterWrite>();
}

int
FrameProcessor::Configure( const Frame* inpFrame, Ratio& ioResamplingRatio, const int* inFrameDimensions, size_t inDimensionCount )
{
  if( !inpFrame || inpFrame->p->format < 0 )
    return AVERROR(EINVAL);

  int changed = false;
  WithLock( mAccess )
    changed = OnConfigure( inpFrame, ioResamplingRatio, inFrameDimensions, inDimensionCount );
  if( changed > 0 )
    FlushBuffers();
  mResamplingRatio = ioResamplingRatio;
  SyncMemory<AfterWrite>();
  return changed;
}

int
FrameProcessor::FillBuffers( const Frame* inpFrame )
{
  int count = 0, consumed = 0;
  if( BuffersFree() > 0 )
  {
    count = Process( inpFrame );
    consumed = ( count > 0 );
  }
  return count < 0 ? count : consumed;
}

int
FrameProcessor::Process( const Frame* inpFrame )
{
  WithLock( mAccess )
  {
    Buffer& b = mBuffers[mWriteBufferIdx];
    if( b.pRead != b.pEnd )
      return 0;
    while( b.refCount.Atomic().IfEqual( 0 ).Exchange( 1 ) )
      ;
    int count = OnProcess( inpFrame, mWriteBufferIdx );
    if( count > 0 )
    {
      b.pRead = reinterpret_cast<uint8_t*>( b.pBufferObject->Data() );
      b.pEnd = b.pRead + b.pBufferObject->Frames() * BytesPerFrame();
      if( b.tick.intpart < 0 && inpFrame )
      {
        b.tick.intpart = inpFrame->pos.count;
        b.tick.fracpart = 0;
      }
      if( b.tick.intpart < 0 && mBuffersUsed > 0 )
      {
        const Buffer& prev = mBuffers[( mWriteBufferIdx + mBuffers.size() - 1 ) % mBuffers.size()];
        b.tick = prev.tick + GuessTicksInBuffer( prev );
      }
      if( mWriteBufferIdx.Atomic().IfEqual( mBuffers.size() - 1 ).Exchange( 0 ) != mBuffers.size() - 1 )
        ++mWriteBufferIdx;
      ++mBuffersUsed;
      Assert( mBuffersUsed <= mBuffers.size() );
    }
    --b.refCount;
    return count;
  }
  return 0;
}

Ratio
FrameProcessor::GuessTicksInBuffer( const Buffer& b ) const
{
  SyncMemory<BeforeRead>();
  int frames = ( b.pEnd - b.pRead ) / BytesPerFrame();
  return frames / mResamplingRatio;
}

int
FrameProcessor::LockData( int inFrames, void*& outpBegin, void*& outpEnd )
{
  Buffer& b = mBuffers[mReadBufferIdx];
  int frames = std::min<int>( inFrames, ( b.pEnd - b.pRead ) / BytesPerFrame() );
  ++b.refCount;
  outpBegin = b.pRead;
  outpEnd = b.pRead + frames * BytesPerFrame();
  return frames;
}

void
FrameProcessor::ReleaseData( void* inpBegin, void* /*inpEnd*/ )
{
  Buffer& b = mBuffers[mReadBufferIdx];
  --b.refCount;
  Assert( b.pRead == inpBegin );
}

int
FrameProcessor::AdvanceByBuffers( int inCount )
{
  if( inCount < 1 || mBuffersUsed < 1 )
    return 0;

  SyncMemory<BeforeRead>();
  Buffer& b = mBuffers[mReadBufferIdx];
  if( b.refCount > 0 )
  {
    const int maxCount = Ceil( Time::FromMilliseconds( 100 ) / ThreadUtils::TimeSlice::Duration() );
    for( int i = 0; b.refCount > 0 && i < maxCount; ++i )
      ThreadUtils::Idle();
  }
  Assert( b.refCount == 0 );
  if( --mBuffersUsed == 0 )
  {
    b.tick += GuessTicksInBuffer( b );
    mWriteBufferIdx = mReadBufferIdx;
  }
  else
  {
    b.tick.intpart = -1;
    b.tick.fracpart = 0;
    mReadBufferIdx = (mReadBufferIdx + 1) % mBuffers.size();
  }
  b.pRead = 0;
  b.pEnd = 0;
  SyncMemory<AfterWrite>();
  return 1;
}

int
FrameProcessor::AdvanceByFrames( int inCount )
{ 
  if( inCount < 1 )
    return 0;

  SyncMemory<BeforeRead>();
  Assert( mBuffersUsed > 0 );
  Buffer& b = mBuffers[mReadBufferIdx];
  int bytesPerFrame = BytesPerFrame(),
      framesInBuffer = ( b.pEnd - b.pRead ) / bytesPerFrame;
  int frames = std::min( inCount, framesInBuffer );
  b.pRead += frames * bytesPerFrame;
  if( b.pRead == b.pEnd )
    return frames * AdvanceByBuffers( 1 );

  if( mBuffersUsed > 1 )
  {
    const Buffer& next = mBuffers[(mReadBufferIdx + 1) % mBuffers.size()];
    b.tick = next.tick - ( framesInBuffer - frames ) / mResamplingRatio;
  }
  else
    b.tick += frames / mResamplingRatio;
  SyncMemory<AfterWrite>();
  return frames;
}

int
FrameProcessor::AdvanceByTicks( int inTicks )
{
  SyncMemory<BeforeRead>();
  Assert( mBuffersUsed > 0 );
  Buffer& b = mBuffers[mReadBufferIdx];
  if( inTicks < 1 )
    return 0;

  if( mBuffersUsed > 1 )
  {
    Buffer& b2 = mBuffers[( mReadBufferIdx + 1 ) % mBuffers.size()];
    int ticksInBuffer = Round( b2.tick - b.tick );
    if( inTicks >= ticksInBuffer )
      return ticksInBuffer * AdvanceByBuffers( 1 );
  }

  int framesInBuffer = ( b.pEnd - b.pRead ) / BytesPerFrame(),
      advanceFrames = Round( inTicks * mResamplingRatio );
  if( advanceFrames >= framesInBuffer )
    advanceFrames = framesInBuffer - 1;
  b.pRead += advanceFrames * BytesPerFrame();
  b.tick.intpart += inTicks;
  SyncMemory<AfterWrite>();
  return inTicks;
}

void
FrameProcessor::FlushBuffers()
{
  WithLock( mAccess )
    OnFlushBuffers();
  for( size_t i = 0; i < mBuffers.size(); ++i )
  {
    Buffer& b = mBuffers[i];
    while( b.refCount > 0 )
      ThreadUtils::Idle();
    b.pRead = 0;
    b.pEnd = 0;
    b.tick.intpart = -1;
    b.tick.fracpart = 0;
  }
  mReadBufferIdx = 0;
  mWriteBufferIdx = 0;
  mBuffersUsed = 0;
}

// AudioResampler
AudioResampler::AudioBuffer::AudioBuffer()
: pObject( ::av_frame_alloc() )
{
}

AudioResampler::AudioBuffer::~AudioBuffer()
{
  ::av_frame_free( &pObject );
}

void
AudioResampler::AudioBuffer::Clear()
{
  pObject->nb_samples = 0;
}

void*
AudioResampler::AudioBuffer::Data()
{
  return pObject->data[0];
}

int
AudioResampler::AudioBuffer::Frames() const
{
  return pObject->nb_samples;
}

AudioResampler::AudioResampler()
: FrameProcessor( mBufferObjects, sizeof(mBufferObjects)/sizeof(*mBufferObjects) ),
  mpContext( 0 )
{
  ::memset( &mParams, 0, sizeof( mParams ) );
}

AudioResampler::~AudioResampler()
{
  Destroy();
}

void
AudioResampler::Destroy()
{
  WithLocked( ppCtx = mpContext.Mutable() ) if( *ppCtx )
  {
    ::avresample_close( *ppCtx );
    ::avresample_free( &*ppCtx );
    *ppCtx = nullptr;
  }
}

int
AudioResampler::BytesPerFrame() const
{
  SyncMemory<BeforeRead>();
  return mParams.channels * sizeof(float);
}

int
AudioResampler::OnConfigure( const Frame* inpFrame, Ratio& ioResamplingRatio, const int* inFrameDimensions, size_t inDimensionCount )
{
  bool changed = false;
  int numChannels = 1;
  if( inDimensionCount > 0 )
    numChannels = inFrameDimensions[0];

  uint64_t layout = inpFrame->p->channel_layout;
  if( ::av_get_channel_layout_nb_channels( layout ) != numChannels )
    layout = ::av_get_default_channel_layout( numChannels );

  ioResamplingRatio.Normalize();
  ioResamplingRatio.Simplify( 1e-5 );
  Assert( ioResamplingRatio.Numerator() <= INT_MAX && ioResamplingRatio.Denominator() <= INT_MAX );

  Params params =
  {
    ioResamplingRatio, numChannels,
    inpFrame->p->channel_layout, layout,
    AVSampleFormat( inpFrame->p->format ), AV_SAMPLE_FMT_FLT,
  };
  if( ::memcmp( &mParams, &params, sizeof( Params ) ) )
  {
    mParams = params;
    Destroy();
  }
  WithLocked( ppCtx = mpContext.Mutable() ) if( !*ppCtx )
  {
    changed = true;
    *ppCtx = ::avresample_alloc_context();
    for( int i = 0; i < sizeof(mBufferObjects)/sizeof(*mBufferObjects); ++i )
    {
      AVFrame* p = mBufferObjects[i].pObject;
      p->sample_rate = mParams.resamplingRatio.Numerator();
      p->channel_layout = mParams.layout;
      p->format = mParams.format;
    }
    inpFrame->p->sample_rate = mParams.resamplingRatio.Denominator();
    int err = 0;
    if( !err )
      err = ::avresample_config( *ppCtx, mBufferObjects[0].pObject, inpFrame->p );
#if 0
    if( !err )
      err = ::av_opt_set_int( *ppCtx, "force_resampling", 1, 0 );
#endif
    if( !err )
      err = ::av_opt_set_int( *ppCtx, "dither_method", AV_RESAMPLE_DITHER_NONE, 0 );
    if( !err )
      err = ::av_opt_set_int( *ppCtx, "linear_interp", 1, 0 );
    if( !err )
      err = ::avresample_open( *ppCtx );
    SyncMemory<AfterWrite>();
    if( err )
      return err;
  }
  return changed ? 1 : 0;
}

int
AudioResampler::OnProcess( const Frame* inpFrame, size_t inBufferIdx )
{
  // With Libav 11.1, this did not work as expected when using avresample_convert_frame().
  // So we use the lower-level avresample_convert() function instead.
  int err = 0;
  uint8_t** inData = 0;
  int inLinesize = 0,
      inSamples = 0;
  if( inpFrame && inpFrame->p )
  {
    inData = inpFrame->p->extended_data;
    inLinesize = inpFrame->p->linesize[0];
    inSamples = inpFrame->p->nb_samples;
  }
  WithLocked( ppCtx = mpContext.Mutable() ) if( *ppCtx )
  {
    int avail = ::avresample_get_out_samples( *ppCtx, inSamples );
    if( avail < 1 )
      return avail;
    AVFrame* pOut = mBufferObjects[inBufferIdx].pObject;
    pOut->nb_samples = pOut->linesize[0] / BytesPerFrame();
    if( pOut->nb_samples < avail && pOut->linesize[0] > 0 )
      ::av_frame_unref( pOut );
    if( pOut->linesize[0] == 0 )
    {
      pOut->format = mParams.format;
      pOut->channel_layout = mParams.layout;
      pOut->nb_samples = avail;
      err = ::av_frame_get_buffer( pOut, sizeof(float) );
    }
    if( !err )
    {
      int count = ::avresample_convert( *ppCtx, pOut->extended_data, pOut->linesize[0], pOut->nb_samples, inData, inLinesize, inSamples );
      if( count < 0 )
      {
        err = count;
        pOut->nb_samples = 0;
      }
      else
      {
        pOut->nb_samples = count;
      }
    }
    if( err < 0 )
      return err;
    return pOut->nb_samples;
  }
  return 0;
}

void
AudioResampler::OnFlushBuffers()
{
  WithLocked( ppCtx = mpContext.Mutable() )
  {
    if( *ppCtx && ::avresample_is_open( *ppCtx ) )
    {
      ::avresample_close( *ppCtx );
      ::avresample_open( *ppCtx );
    }
  }
}

// VideoScaler
VideoScaler::PictureBuffer::PictureBuffer()
: allocated( false )
{
}

VideoScaler::PictureBuffer::~PictureBuffer()
{
  if( allocated )
    ::avpicture_free( &picture );
}

void
VideoScaler::PictureBuffer::Clear()
{
}

void*
VideoScaler::PictureBuffer::Data()
{
  return picture.data[0];
}

int
VideoScaler::PictureBuffer::Frames() const
{
  return 1;
}

VideoScaler::VideoScaler()
: FrameProcessor( mBufferObjects, sizeof(mBufferObjects)/sizeof(*mBufferObjects) ),
  mpContext( 0 )
{
  ::memset( &mParams, 0, sizeof( mParams ) );
}

VideoScaler::~VideoScaler()
{
  if( mpContext )
    ::sws_freeContext( mpContext );
}

int
VideoScaler::BytesPerFrame() const
{
  return mParams.height * mParams.width * sizeof(uint32_t);
}

int
VideoScaler::OnProcess( const Frame* inpFrame, size_t inWriteBufferIdx )
{
  Assert( mpContext );
  if( !inpFrame )
    return 0;
  AVPicture* pPicture = &mBufferObjects[inWriteBufferIdx].picture;
  int height = ::sws_scale( mpContext, inpFrame->p->data, inpFrame->p->linesize, 0, inpFrame->p->height, pPicture->data, pPicture->linesize );
  if( height < 0 )
    return height;
  Assert( height == mParams.height );
  return 1;
}

int
VideoScaler::OnConfigure( const Frame* inpFrame, Ratio&, const int* inFrameDimensions, size_t inDimensionCount )
{
  bool changed = false;
  Params params =
  {
    inpFrame->p->width, 1,
    inpFrame->p->height, 1,
    AVPixelFormat( inpFrame->p->format ), AV_PIX_FMT_RGB32,
  };
  if( inDimensionCount > 0 )
    params.width = inFrameDimensions[0];
  if( inDimensionCount > 1 )
    params.height = inFrameDimensions[1];

  if( ::memcmp( &mParams, &params, sizeof( Params ) ) )
  {
    mParams = params;
    if( mpContext )
    {
      ::sws_freeContext( mpContext );
      mpContext = 0;
    }
  }
  if( !mpContext )
  {
    changed = true;
    for( int i = 0; i < mBuffers.size(); ++i )
    {
      PictureBuffer& b = mBufferObjects[i];
      if( b.allocated )
        ::avpicture_free( &b.picture );
      ::avpicture_alloc( &b.picture, mParams.format, mParams.width, mParams.height );
      b.allocated = true;
    }
    mpContext = ::sws_getContext(
      mParams.srcWidth, mParams.srcHeight, mParams.srcFormat,
      mParams.width, mParams.height, mParams.format, SWS_FAST_BILINEAR, 0, 0, 0
    );
    if( !mpContext )
      return AVERROR(ENOMEM);
  }
  return changed ? 1 : 0;
}

void
VideoScaler::OnFlushBuffers()
{
}


