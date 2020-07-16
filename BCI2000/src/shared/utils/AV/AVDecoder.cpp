////////////////////////////////////////////////////////////////////////////////
// $Id$
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A class that asynchronously decodes and buffers AV file data.
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
#include "AVDecoder.h"

#include "AVUtils.h"
#include "Lockable.h"
#include "Debugging.h"

#include <vector>

using namespace std;

namespace
{

class FrameCache : public AVUtils::DataOwner, Uncopyable
{
public:
  FrameCache();
  ~FrameCache();

  char* Realloc( size_t size );
  void SetFrameSize( int );
  void SetFramePosition( ptrdiff_t );
  ptrdiff_t FramePosition() const;
  ptrdiff_t FrameCount() const;

  void SetData( const void*, const void* );
  int LockData( int, void*&, void*& );
  void ReleaseData( void*, void* );
private:
  char* mpBegin, *mpEnd, *mpPos;
  int mFrameSize;
  Synchronized<int> mRefCount;
};

FrameCache::FrameCache()
: mpBegin( nullptr ), mpEnd( nullptr ), mpPos( nullptr ),
  mFrameSize( 1 )
{}

FrameCache::~FrameCache()
{
  Assert( mRefCount == 0 );
  ::free( mpBegin );
}

void
FrameCache::SetFrameSize( int size )
{
  mFrameSize = size;
}

void
FrameCache::SetFramePosition( ptrdiff_t pos )
{
  pos = std::min( pos, mpEnd - mpBegin );
  mpPos = mpBegin + pos * mFrameSize;
}

ptrdiff_t
FrameCache::FramePosition() const
{
  return ( mpPos - mpBegin ) / mFrameSize;
}

ptrdiff_t
FrameCache::FrameCount() const
{
  return ( mpEnd - mpBegin ) / mFrameSize;
}

char*
FrameCache::Realloc( size_t inSize )
{
  Assert( mRefCount == 0 );
  if( !inSize )
  {
    ::free( mpBegin );
    mpBegin = 0;
    mpEnd = 0;
    mpPos = 0;
    return 0;
  }
  char* p = static_cast<char*>( ::realloc( mpBegin, inSize ) );
  if( p )
  {
    mpBegin = p;
    mpEnd = p + inSize;
    if( mpPos > mpEnd )
      mpPos = mpEnd;
    if( mpPos < mpBegin )
      mpPos = mpBegin;
  }
  return p;
}

void
FrameCache::SetData( const void* inpBegin, const void* inpEnd )
{
  if( mRefCount < 1 )
  {
    const char* pBegin = reinterpret_cast<const char*>( inpBegin ),
              *pEnd = reinterpret_cast<const char*>( inpEnd );
    int size = pEnd - pBegin;
    if( Realloc( size ) )
    {
      ::memcpy( mpBegin, pBegin, size );
      mpPos = mpBegin;
    }
  }
}

int
FrameCache::LockData( int frames, void*& p1, void*& p2 )
{
  frames = std::min( frames, int( mpEnd - mpPos ) / mFrameSize );
  p1 = mpPos;
  p2 = mpPos + frames * mFrameSize;
  ++mRefCount;
  return frames;
}

void
FrameCache::ReleaseData( void* p1, void* )
{
  Assert( p1 == mpPos );
  Assert( mRefCount > 0 );
  --mRefCount;
}

} // namespace

struct AVDecoder::Data
{
  Data();
  ~Data();
  void Close();

  void SetPositionFromAVTimeStamp( int64_t, ClockTick& );
  void SetError( const std::string&, int inCode = 0 );
  void SetError( int inCode );
  void ClearState();

  void ConfigChanged();
  void UpdateConfig();
  const ClockTick& HeadPos();
  int FrameSize() const;

  int ReadAndDecodePacket();
  bool ReadOneFrame();
  bool ProcessToBuffer( AVUtils::FrameProcessor* );
  bool ProcessToBuffer();

  bool SeekFile( int64_t );
  void SeekHead( int64_t );
  int64_t HeadFrameTick();

  String mError;
  MediaType mMediaType;
  FrameCache mFrameCache;

  enum { Closed = 0, Open, EndOfStream, EndOfData, Error };
  Synchronized<int> mState, mConfigurationState;
  Synchronized<bool> mConfigChanged, mIsSeeking;
  Mutex mConfigurationLock;
  Lockable<> mDecodingLock;

  struct
  {
    AVFormatContext* pFormat;
    AVCodecContext* pCodec;
    AVStream* pStream;
    AVPacket packetFromFile, packetToDecode;
    int streamIndex, ticksPerFrame;
    bool eof, eos;
  } mAV;
  Lockable<> mAVLock;

  ClockTick mBofPos, mEofPos, mInputPos, mHeadPos;
  Rate mInputFrameRate;
  Ratio mResamplingRatio;
  AVUtils::Frame mFrame;

  Synchronized<AVUtils::FrameProcessor*> mpFrameProcessor;
  vector<int> mFrameDimensions;
};

AVDecoder::Data::Data()
: mpFrameProcessor( 0 ),
  mMediaType( None ),
  mState( Closed )
{
  static bool sInitialized = false;
  if( !sInitialized )
  {
    ::av_register_all();
    ::avcodec_register_all();
    sInitialized = true;
  }
  ::memset( &mAV, 0, sizeof( mAV ) );
  Close();
}

AVDecoder::Data::~Data()
{
  Close();
}

void
AVDecoder::Data::SetError( int inCode )
{
  SetError( "", inCode );
}

void
AVDecoder::Data::SetError( const string& inMessage, int inCode )
{
  mState = Error;
  mError = inMessage;
  if( inCode < 0 )
  {
    if( mError )
      mError += ": ";
    else
      mError = "Libav error: ";
    char buf[256];
    if( !::av_strerror( inCode, buf, sizeof(buf) ) )
      mError += buf;
    else if( inCode & (AVERROR_INPUT_CHANGED|AVERROR_OUTPUT_CHANGED) == inCode )
      mError += "Unexpected change of data format";
    else
      mError += ::strerror( AVUNERROR(inCode) );
  }
  const char* pLog = AVUtils::Log::Current();
  if( pLog && *pLog )
  {
    mError += "\nLibav log:\n";
    mError += pLog;
  }
}

void
AVDecoder::Data::ClearState()
{
  mError.clear();
  if( mAV.pStream )
    mState = Open;
  else
    mState = Closed;
}

void
AVDecoder::Data::Close()
{
  mState = Closed;
  mMediaType = None;

  WithLock( mAVLock )
  {
    if( mAV.pFormat )
    {
      ::avformat_free_context( mAV.pFormat );
      mAV.pFormat = 0;
    }
    if( mAV.packetFromFile.data )
      ::av_free_packet( &mAV.packetFromFile );
    // avcodec_close() seems to be buggy (may throw in MSVC debug mode),
    // but must be called to avoid considerable memory leaking
#if NDEBUG
    if( mAV.pCodec )
      ::avcodec_close( mAV.pCodec );
#endif
    ::memset( &mAV, 0, sizeof( mAV ) );
    mAV.streamIndex = -1;

    mHeadPos = ClockTick::Null;
    mInputPos = ClockTick::Null;
    mBofPos = ClockTick::Null;
    mEofPos = ClockTick::Null;
    mInputFrameRate = Rate( 0, 0 );
    mResamplingRatio = Ratio( 0, 0 );
    mFrameDimensions.clear();
    mFrameCache.Realloc( 0 );
    delete mpFrameProcessor.Atomic().Exchange( 0 );
  }
}

const ClockTick&
AVDecoder::Data::HeadPos()
{
  Assert( mHeadPos.count == 0 || !IsNaN(mHeadPos.rate) );
  if( !IsNaN( mHeadPos.rate ) )
  {
    AVUtils::FrameProcessor* p = mpFrameProcessor;
    if( p )
      mHeadPos.count = p->HeadTick();
    else
      mHeadPos.count = Floor( mFrameCache.FramePosition() / mResamplingRatio );
    mHeadPos.time = mBofPos.time + mHeadPos.count / mHeadPos.rate;
  }
  else if( mHeadPos.count == 0 )
    mHeadPos.time = mBofPos.time;
  SyncMemory<AfterWrite>();
  return mHeadPos;
}

int
AVDecoder::Data::FrameSize() const
{
  int frameSize = 1;
  for( size_t i = 0; i < mFrameDimensions.size(); ++i )
    frameSize *= mFrameDimensions[i];
  return frameSize;
}

void
AVDecoder::Data::ConfigChanged()
{
  if( mConfigurationState < 1 )
    throw std_runtime_error
      << "Configuration changes must be enclosed with "
      << "BeginConfiguration()/EndConfiguration()";
  mConfigChanged = true;
}

void
AVDecoder::Data::UpdateConfig()
{
  if( !( mpFrameProcessor && mFrame.p && mFrame.p->format >= 0 ) )
    return;
  if( mConfigChanged.Atomic().IfEqual( 1 ).Exchange( 0 ) == 1 )
  {
    AVUtils::Log log;
    bool needSeek = mpFrameProcessor->BuffersUsed();
    int64_t pos = mpFrameProcessor->HeadTick();
    int changed = mpFrameProcessor->Configure( &mFrame, mResamplingRatio, mFrameDimensions.data(), mFrameDimensions.size() );
    if( changed < 0 )
      SetError( "When configuring frame processor", changed );
    else if( changed && needSeek )
      SeekHead( pos );
  }
}

void
AVDecoder::Data::SetPositionFromAVTimeStamp( int64_t ts, ClockTick& tick )
{
  if( ts == AV_NOPTS_VALUE )
    tick = ClockTick::Null;
  else
  {
    tick = mBofPos;
    tick.count = ts;
    tick.time += ts / tick.rate;
  }
}

int
AVDecoder::Data::ReadAndDecodePacket()
{
  ScopedLock( mAVLock );
  if( mAV.packetToDecode.size <= 0 && !mAV.eof )
  {
    AVUtils::Log log;
    if( mAV.packetFromFile.data )
      ::av_free_packet( &mAV.packetFromFile );
    int err = ::av_read_frame( mAV.pFormat, &mAV.packetFromFile );
    if( err == AVERROR_EOF )
      mAV.eof = true;
    else if( err < 0 )
      SetError( "Error reading frame", err );
    if( err < 0 )
    {
      mAV.packetFromFile.data = 0;
      mAV.packetFromFile.size = 0;
    }
    mAV.packetToDecode = mAV.packetFromFile;
  }
  // For some codecs, obtaining final frames requires calling decode() with empty input beyond eof.
  if( mAV.packetToDecode.size <= 0 && mAV.eof && mAV.packetToDecode.pts != AV_NOPTS_VALUE )
    mAV.packetToDecode.pts += mAV.ticksPerFrame;
  SetPositionFromAVTimeStamp( mAV.packetToDecode.pts, mInputPos );

  AVUtils::Log log;
  int dataAvailable = 0;
  int used = mAV.packetToDecode.size;
  if( mAV.packetToDecode.stream_index == mAV.streamIndex && mAV.pCodec )
  {
    switch( mAV.pCodec->codec_type )
    {
    case AVMEDIA_TYPE_VIDEO:
      used = ::avcodec_decode_video2( mAV.pCodec, mFrame.p, &dataAvailable, &mAV.packetToDecode );
      break;
    case AVMEDIA_TYPE_AUDIO:
      used = ::avcodec_decode_audio4( mAV.pCodec, mFrame.p, &dataAvailable, &mAV.packetToDecode );
      break;
    default:
      ;
    }
  }
  if( mAV.eof )
    mAV.eos = !dataAvailable;
  if( used < 0 )
    SetError( "Error decoding packet", used );
  mAV.packetToDecode.data += used;
  mAV.packetToDecode.size -= used;
  if( mAV.packetToDecode.size > 0 && !mAV.packetFromFile.buf )
  {
    int err = ::av_dup_packet( &mAV.packetFromFile );
    if( err < 0 )
      SetError( err );
  }
  return dataAvailable;
}

bool
AVDecoder::Data::ReadOneFrame()
{
  bool gotFrame = false;
  while( !mAV.eos && !gotFrame && mState != Error )
    gotFrame = ReadAndDecodePacket();
  if( gotFrame )
  {
    if( mFrame.p->pkt_pts == AV_NOPTS_VALUE )
      SetPositionFromAVTimeStamp( mFrame.p->pkt_dts, mFrame.pos );
    else
      SetPositionFromAVTimeStamp( mFrame.p->pkt_pts, mFrame.pos );
  }
  return gotFrame;
}

bool
AVDecoder::Data::SeekFile( int64_t inTick )
{
  AVUtils::Log log;
  int flags = 0;
  if( inTick == 0 || inTick < mInputPos.count )
    flags |= AVSEEK_FLAG_BACKWARD;
  int err = ::av_seek_frame( mAV.pFormat, mAV.streamIndex, inTick, flags );
  if( err < 0 )
    SetError( "Seek error", err );
  else
  {
    mAV.eof = false;
    mAV.eos = false;
    ::avcodec_flush_buffers( mAV.pCodec );
    ::memset( &mAV.packetToDecode, 0, sizeof( mAV.packetToDecode ) );
    mInputPos.time = mBofPos.time + inTick / mBofPos.rate;
    mInputPos.count = inTick;
  }
  mFrame.pos = ClockTick::Null;
  return ReadOneFrame();
}

bool
AVDecoder::Data::ProcessToBuffer()
{
  AVUtils::FrameProcessor* p = mpFrameProcessor;
  if( p )
    return ProcessToBuffer( p );
  else
  {
    ptrdiff_t framesBuffered = mFrameCache.FrameCount() - mFrameCache.FramePosition();
    if( framesBuffered <= 1 )
      mState.Atomic().IfEqual( Open ).Exchange( EndOfData );
    return framesBuffered > 0;
  }
  return false;
}

bool
AVDecoder::Data::ProcessToBuffer( AVUtils::FrameProcessor* pProc )
{
  while( pProc->BuffersFree() > 0 && mState == Open )
  {
    int frameConsumed = pProc->FillBuffers( &mFrame );
    if( frameConsumed > 0 && !ReadOneFrame() )
      mState.Atomic().IfEqual( Open ).Exchange( EndOfStream );
    else if( frameConsumed < 0 )
      SetError( "Processing error", frameConsumed );
  }
  if( pProc->BuffersFree() > 0 && mState == EndOfStream )
  {
    int err = pProc->FillBuffers( 0 );
    if( err < 0 )
      SetError( "Processing error", err );
    else if( pProc->FramesBuffered() <= 1 )
      mState.Atomic().IfEqual( EndOfStream ).Exchange( EndOfData );
  }
  return pProc->BuffersUsed();
}

void
AVDecoder::Data::SeekHead( int64_t inTick )
{
  int64_t t = inTick;
  if( t < 0 )
    t = 0;
  else if( mEofPos.count > 0 && t >= mEofPos.count )
    t = mEofPos.count - 1;
  ClearState();

  if( !mpFrameProcessor )
  {
    mFrameCache.SetFramePosition( Floor( t * mResamplingRatio ) );
    ptrdiff_t frames = mFrameCache.FrameCount() - mFrameCache.FramePosition();
    if( frames > 1 )
      mState = Open;
    else
      mState = EndOfData;
    return;
  }

  ScopedLock( mAVLock );
  // First, try to find a reference frame located prior to the desired position.
  // There is a tradeoff unsuccessful seeking vs unnecessary decoding here:
  // greater seek steps and/or growth factors will save seeking effort but tend to increase
  // decoding effort.
  double seekStep = ( Time::Seconds( 0.05 ) * mBofPos.rate ).To<double>(),
         seekStepGrowth = 1.1;//2.5;

  int64_t seek = t;
  SeekFile( seek );
  while( seek > 0 && ( !mFrame.pos.type || mFrame.pos.count > seek ) )
  {
    seek -= seekStep;
    if( seek < 0 )
      seek = 0;
    seekStep *= seekStepGrowth;
    ClearState();
    SeekFile( seek );
  }
  if( !mFrame.pos.type && !mError )
    SetError( "Unexpected failure" );
  // At this point, mFrame will contain unprocessed data starting somewhere before
  // the seek tick.
  if( !mError )
  {
    // Hide frame processor buffer manipulation from rendering code, behind
    // a copy of the current content.
    HeadPos();
    void* pCacheBegin = 0, *pCacheEnd = 0;
    if( !mFrameCache.LockData( 1, pCacheBegin, pCacheEnd ) )
    {
      mFrameCache.ReleaseData( pCacheBegin, pCacheEnd );
      void* pFrameBegin, *pFrameEnd;
      mpFrameProcessor->LockData( 1, pFrameBegin, pFrameEnd );
      mFrameCache.SetData( pFrameBegin, pFrameEnd );
      mpFrameProcessor->ReleaseData( pFrameBegin, pFrameEnd );
      mFrameCache.LockData( 1, pCacheBegin, pCacheEnd );
    }
    mpFrameProcessor->FlushBuffers();
    AVUtils::FrameProcessor* pProc = mpFrameProcessor.Atomic().Exchange( 0 );
    // Send data through the frame processor until the desired tick
    // has been consumed.
    bool gotFrame = true;
    while( mFrame.pos.count <= t && gotFrame && !mError )
    {
      pProc->FlushBuffers();
      int frameConsumed = 0;
      while( !frameConsumed && pProc->BuffersFree() > 0 )
      {
        frameConsumed = pProc->FillBuffers( &mFrame );
        if( frameConsumed < 0 )
          SetError( "Processing error", frameConsumed );
      }
      if( frameConsumed == 1 )
        gotFrame = ReadOneFrame();
    }
    if( !gotFrame )
      mState = EndOfStream;
    ProcessToBuffer( pProc );
    // At this point, the desired tick should be part of the frame processor's buffer.
    int ticks = t - pProc->HeadTick(),
        didAdvance = 1;
    while( ticks > 0 && didAdvance > 0 && mState != EndOfData && mState != Error )
    {
      didAdvance = pProc->AdvanceByTicks( ticks );
      ticks -= didAdvance;
      ProcessToBuffer( pProc );
    }
    mpFrameProcessor = pProc;
    HeadPos();
    mFrameCache.ReleaseData( pCacheBegin, pCacheEnd );
  }
}

int64_t
AVDecoder::Data::HeadFrameTick()
{
  AVUtils::FrameProcessor* p = mpFrameProcessor;
  if( p )
    mHeadPos.count = p->HeadTick();
  else
    mHeadPos.count = Round( mFrameCache.FramePosition() / mResamplingRatio );
  int64_t tick = mHeadPos.count;
  Ratio inputFramesPerTick = mInputFrameRate / mBofPos.rate;
  if( !IsNaN( inputFramesPerTick ) && tick >= 0 )
    tick = Floor( Floor( tick * inputFramesPerTick ) / inputFramesPerTick );
  return tick;
}

// AVDecoder
AVDecoder::AVDecoder()
: d( new Data )
{
}

AVDecoder::~AVDecoder()
{
  delete d;
}

AVDecoder&
AVDecoder::BeginConfiguration()
{
  if( ++d->mConfigurationState == 1 )
    d->mConfigurationLock.Acquire();
  return *this;
}

AVDecoder&
AVDecoder::EndConfiguration()
{
  if( --d->mConfigurationState == 0 )
  {
    d->UpdateConfig();
    d->mConfigurationLock.Release();
  }
  return *this;
}

const String&
AVDecoder::Error() const
{
  return d->mError;
}

bool
AVDecoder::EndOfData() const
{
  switch( d->mState )
  {
  case Data::EndOfData:
  case Data::Error:
  case Data::Closed: // AudioPlayer::OnRenderingTick() depends on this
    return true;
  }
  return false;
}

AVDecoder&
AVDecoder::Open( const std::string& inFile, MediaType inType, int inWhich )
{
  Close();
  d->mError.clear();

  int mediaType = AVMEDIA_TYPE_UNKNOWN;
  switch( inType )
  {
  case Image:
  case Video:
    mediaType = AVMEDIA_TYPE_VIDEO;
    break;
  case Audio:
    mediaType = AVMEDIA_TYPE_AUDIO;
    break;
  default:
    d->mError = "Unknown MediaType";
  }

  ScopedLock( d->mAVLock );
  if( !Error() )
  {
    AVUtils::Log log;
    int err = ::avformat_open_input( &d->mAV.pFormat, inFile.c_str(), 0, 0 );
    if( err >= 0 )
      err = ::avformat_find_stream_info( d->mAV.pFormat, 0 );
    if( err < 0 )
      d->SetError( "Unknown file format", err );
  }
  AVStream* pStream = 0;
  if( !Error() )
  {
    int count = 0;
    for( size_t i = 0; pStream == 0 && i < d->mAV.pFormat->nb_streams; ++i )
    {
      AVStream* p = d->mAV.pFormat->streams[i];
      if( p->codec->codec_type == mediaType )
      {
        bool found = false;
        switch( inType )
        {
        case Audio:
          found = true;
          break;
        case Video:
          found = p->avg_frame_rate.num && p->avg_frame_rate.den;
          break;
        case Image:
          found = ( p->avg_frame_rate.num == 0 );
          break;
        default:
          found = false;
        }
        if( found && count++ == inWhich )
          pStream = d->mAV.pFormat->streams[i];
      }
    }
  }
  if( !pStream )
    d->mError = "Did not find specified stream";

  const AVCodec* pCodec = 0;
  if( !Error() )
  {
    AVUtils::Log log;
    pCodec = ::avcodec_find_decoder( pStream->codec->codec_id );
    if( !pCodec )
      d->SetError( "Could not find codec for specified stream" );
  }
  if( !Error() )
  {
    AVCodecContext* pCodecContext = 0;
    if( pStream->codec )
    {
      pCodecContext = pStream->codec;
      pCodecContext->thread_count = 1; // disable libav multithreaded decoding
      AVUtils::Log log;
      int err = ::avcodec_open2( pCodecContext, pCodec, 0 );
      if( err < 0 )
        d->SetError( "Could not initialize codec", err );
      else
        d->mAV.pCodec = pCodecContext;
    }
  }
  if( !Error() )
  {
    AVUtils::Log log;
    d->mAV.pStream = pStream;
    d->mAV.streamIndex = pStream->index;
    d->mAV.ticksPerFrame = pStream->codec->ticks_per_frame;
    Assert( d->mAV.ticksPerFrame );

    d->mBofPos = ClockTick::Null;
    d->mBofPos.type = ClockType::Recording | inType;
    d->mBofPos.rate = Rate( pStream->time_base.den, pStream->time_base.num * Time::OneSecond );
    d->mBofPos.count = 0;
    d->mBofPos.time = Time( 0 ) + Time::Seconds( 3600 * 24 ); // might refer to recording date, for now make sure there is some space left for negative offsets

    d->mEofPos = d->mBofPos;
    if( pStream->start_time != AV_NOPTS_VALUE )
      d->mEofPos.count = pStream->start_time;
    d->mEofPos.count += pStream->duration;
    d->mEofPos.time += d->mEofPos.count / d->mEofPos.rate;
    d->mInputPos = d->mBofPos;
    d->mHeadPos = d->mBofPos;

    ::av_init_packet( &d->mAV.packetFromFile );
    d->ReadOneFrame();
  }
  if( !Error() )
  {
    switch( inType )
    {
    case Image:
      d->mInputFrameRate = Rate( 0, Time::OneSecond );
      d->mFrameDimensions.push_back( d->mFrame.p->width );
      d->mFrameDimensions.push_back( d->mFrame.p->height );
      d->mpFrameProcessor = new AVUtils::VideoScaler();
      break;
    case Video:
      d->mInputFrameRate = Rate( pStream->avg_frame_rate.num, pStream->avg_frame_rate.den * Time::OneSecond );
      d->mFrameDimensions.push_back( d->mFrame.p->width );
      d->mFrameDimensions.push_back( d->mFrame.p->height );
      d->mpFrameProcessor = new AVUtils::VideoScaler();
      break;
    case Audio:
      d->mInputFrameRate = Rate( d->mFrame.p->sample_rate, Time::OneSecond );
      d->mFrameDimensions.push_back( ::av_get_channel_layout_nb_channels( d->mFrame.p->channel_layout ) );
      d->mpFrameProcessor = new AVUtils::AudioResampler();
      break;
    }
    Assert( !IsNaN( d->mInputFrameRate ) );
    Ratio r = d->mBofPos.rate / d->mInputFrameRate;
    r.Simplify( 1e-5 ); // make sure that input frames can be exactly represented by time stamps
    d->mInputFrameRate = d->mBofPos.rate / r;
    d->mResamplingRatio = Ratio( 1, 1 );
    d->mMediaType = inType;
    d->mState = Data::Open;
    d->ConfigChanged();
  }
  if( Error() )
    d->Close();
  return *this;
}

AVDecoder&
AVDecoder::Close()
{
  d->Close();
  return *this;
}

String
AVDecoder::Codec() const
{
  if( !d->mAV.pCodec )
    return 0;
  if( !d->mAV.pCodec->codec )
    return 0;
  return d->mAV.pCodec->codec->name;
}

String
AVDecoder::ContainerFormat() const
{
  if( !d->mAV.pFormat )
    return 0;
  if( !d->mAV.pFormat->iformat )
    return 0;
  return d->mAV.pFormat->iformat->name;
}

const ClockTick&
AVDecoder::BofPosition() const
{
  return d->mBofPos;
}

const ClockTick&
AVDecoder::EofPosition() const
{
  return d->mEofPos;
}

const ClockTick&
AVDecoder::InputPosition() const
{
  return d->mInputPos;
}

const ClockTick&
AVDecoder::HeadPosition() const
{
  return d->HeadPos();
}

int64_t
AVDecoder::HeadTick() const
{
  AVUtils::FrameProcessor* p = d->mpFrameProcessor;
  if( p )
    d->mHeadPos.count = p->HeadTick();
  else
    d->mHeadPos.count = Floor( d->mFrameCache.FramePosition() / d->mResamplingRatio );
  return d->mHeadPos.count;
}

int64_t
AVDecoder::HeadFrameTick() const
{
  return d->HeadFrameTick();
}

AVDecoder&
AVDecoder::SeekToTick( int64_t t )
{
  switch( d->mState )
  {
    case Data::Closed:
    case Data::Error:
      return *this;
  }
  d->mIsSeeking = true;
  if( d->mpFrameProcessor )
    d->mpFrameProcessor->AssignHeadTick( d->HeadFrameTick() );
#if 0 // Optimization: Advance buffer instead of seeking forward by small amounts.
      // Currently broken -- moves beyond EndOfData.
  bool isValidState = d->mpFrameProcessor->HeadTick() >= 0;
  if( isValidState && d->mEofPos.count > 0 )
    isValidState = ( d->mEofPos.count - d->mpFrameProcessor->HeadTick() ) / d->mBofPos.rate > Time::Seconds( 0.1 );
  if( isValidState )
  {
    int64_t delta = t - d->mpFrameProcessor->HeadTick();
    if( delta >= 0 && delta / d->mBofPos.rate < Time::Seconds( 0.1 ) )
    {
      int didAdvance = 1;
      while( delta > 0 && didAdvance > 0 && d->mState != Data::EndOfData && d->mState != Data::Error )
      {
        d->ProcessToBuffer();
        didAdvance = d->mpFrameProcessor->AdvanceByTicks( delta );
        delta -= didAdvance;
      }
    }
    d->HeadPos();
  }
#endif
  if( HeadTick() != t )
    d->SeekHead( t );
  d->mIsSeeking = false;
  return *this;
}

bool
AVDecoder::IsSeeking() const
{
  return d->mIsSeeking;
}

int
AVDecoder::AdvanceByTicks( int inCount )
{
  if( d->mIsSeeking )
    return 0;
  AVUtils::FrameProcessor* p = d->mpFrameProcessor;
  if( p )
    return p->AdvanceByTicks( inCount );
  else
  {
    int framePos = d->mFrameCache.FramePosition(),
        tickPos = Floor( framePos / d->mResamplingRatio );
    d->mFrameCache.SetFramePosition( framePos + Floor( inCount * d->mResamplingRatio ) );
    return HeadFrameTick() - tickPos;
  }
  return 0;
}

int
AVDecoder::AdvanceByFrames( int inCount )
{
  if( d->mIsSeeking )
    return 0;
  AVUtils::FrameProcessor* p = d->mpFrameProcessor;
  if( p )
    return p->AdvanceByFrames( inCount );
  else
  {
    int pos = d->mFrameCache.FramePosition();
    d->mFrameCache.SetFramePosition( pos + inCount );
    return d->mFrameCache.FramePosition() - pos;
  }
  return 0;
}

const Rate&
AVDecoder::InputFrameRate() const
{
  return d->mInputFrameRate;
}

const Ratio&
AVDecoder::ResamplingRatio() const
{
  return d->mResamplingRatio;
}

AVDecoder&
AVDecoder::SetResamplingRatio( const Ratio& r )
{
  if( r != d->mResamplingRatio )
  {
    d->mResamplingRatio = r;
    d->ConfigChanged();
  }
  return *this;
}

AVDecoder&
AVDecoder::SetFrameDimension( size_t inIdx, int inSize )
{
  if( inIdx >= d->mFrameDimensions.size() )
    d->mFrameDimensions.resize( inIdx + 1, 1 );
  if( inSize != d->mFrameDimensions[inIdx] )
  {
    d->mFrameDimensions[inIdx] = inSize;
    d->ConfigChanged();
  }
  return *this;
}

int
AVDecoder::FrameDimension( size_t inIdx ) const
{
  return inIdx < d->mFrameDimensions.size() ? d->mFrameDimensions[inIdx] : 1;
}

bool
AVDecoder::DecodeToBuffer()
{
  WithTryLock( d->mDecodingLock )
  {
    if( d->mConfigurationState > 0 )
      return false;
    if( d->mState == Data::Closed )
      return false;
    if( d->mIsSeeking )
      return false;
    return d->ProcessToBuffer();
  }
  return false;
}

bool
AVDecoder::DecodeAllIntoMemory()
{
  if( d->mConfigurationState > 0 )
    return false;
  if( d->mState == Data::Closed )
    return false;
  if( d->mIsSeeking )
    return false;
  if( !d->mpFrameProcessor )
    return false;

  WithLock( d->mDecodingLock )
  {
    AVUtils::FrameProcessor* p = d->mpFrameProcessor;
    int bytesPerFrame = p->BytesPerFrame();
    size_t size = Ceil( ( d->mEofPos.count - d->mBofPos.count ) * d->mResamplingRatio * bytesPerFrame );
    size += 1024;
    char* pBegin = d->mFrameCache.Realloc( size );
    if( !pBegin )
      return false;
    d->mFrameCache.SetFrameSize( bytesPerFrame );

    if( HeadTick() != 0 )
      SeekToTick( 0 );
    d->ProcessToBuffer();

    int remaining = size / bytesPerFrame, frames = 0;
    do
    {
      void* p1, *p2;
      frames = p->LockData( remaining, p1, p2 );
      if( frames == 0 )
      {
        p->ReleaseData( p1, p2 );
        d->ProcessToBuffer();
       frames = p->LockData( remaining, p1, p2 );
      }
      ::memcpy( pBegin, p1, frames * bytesPerFrame );
      pBegin += frames * bytesPerFrame;
      remaining -= frames;
      p->ReleaseData( p1, p2 );
      p->AdvanceByFrames( frames );
    } while( remaining > 0 && frames > 0 );
    if( !d->mFrameCache.Realloc( size - remaining * bytesPerFrame ) )
      return false;
    d->mState = Data::Open;

    d->mpFrameProcessor = nullptr;
    delete p;
  }
  return true;
}

bool
AVDecoder::AreFramesAvailable() const
{
  if( d->mIsSeeking )
    return false;

  AVUtils::FrameProcessor* p = d->mpFrameProcessor;
  if( p )
    return p->BuffersUsed() > 0;

  return d->mFrameCache.FramePosition() < d->mFrameCache.FrameCount();
}

AVDecoder::Chunk
AVDecoder::GetFrames( int inFrames )
{
  Chunk ref;
  AVUtils::FrameProcessor* p = d->mpFrameProcessor;
  if( !p || p->BuffersUsed() < 1 )
  {
    ref.frames = d->mFrameCache.LockData( inFrames, ref.pBegin, ref.pEnd );
    ref.d = &d->mFrameCache;
  }
  else
  {
    ref.frames = p->LockData( inFrames, ref.pBegin, ref.pEnd );
    ref.d = p;
  }
  return ref;
}

// Chunk
AVDecoder::Chunk::Chunk()
{
  Clear();
}

AVDecoder::Chunk::Chunk( const Chunk& other )
{
  operator=( other );
}

AVDecoder::Chunk&
AVDecoder::Chunk::operator=( const Chunk& other )
{
  ::memcpy( this, &other, sizeof(*this) );
  const_cast<Chunk&>( other ).Clear();
  return *this;
}

AVDecoder::Chunk::~Chunk()
{
  AVUtils::DataOwner* p = reinterpret_cast<AVUtils::DataOwner*>( d );
  if( p )
    p->ReleaseData( pBegin, pEnd );
}

void
AVDecoder::Chunk::Release()
{
  AVUtils::DataOwner* p = reinterpret_cast<AVUtils::DataOwner*>( d );
  if( p )
    p->ReleaseData( pBegin, pEnd );
  Clear();
}

void
AVDecoder::Chunk::Clear()
{
  ::memset( this, 0, sizeof(*this) );
}

