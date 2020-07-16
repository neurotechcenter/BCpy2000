////////////////////////////////////////////////////////////////////////////////
// $Id$
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A GraphObject for video rendering.
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
#include "VideoPlayer.h"
#include "AudioPlayer.h"
#include "FileUtils.h"
#include "Lockable.h"
#include "AVDecoder.h"

#if USE_QT
# include <QPainter>
#endif

struct VideoPlayer::Data
{
  int mNativeWidth, mNativeHeight;
  int64_t mRenderedPos;
};

// VideoPlayer
VideoPlayer::VideoPlayer( GUI::GraphDisplay& display, int zOrder )
: GraphObject( display, zOrder ),
  d( new Data )
{
  d->mNativeWidth = 0;
  d->mNativeHeight = 0;
  d->mRenderedPos = -1;
}

VideoPlayer::~VideoPlayer()
{
  DestructorEntered();
  Close();
  SetOnStateChange( 0 );
  delete d;
}

int
VideoPlayer::NativeWidth() const
{
  SyncMemory<BeforeRead>();
  return d->mNativeWidth;
}

int
VideoPlayer::NativeHeight() const
{
  SyncMemory<BeforeRead>();
  return d->mNativeHeight;
}

double
VideoPlayer::NativeFramesPerSecond() const
{
  return ( NativeFrameRate() * Time::OneSecond ).To<double>();
}

double
VideoPlayer::FramesPerSecond() const
{
  return ( FrameRate() * Time::OneSecond ).To<double>();
}

// AVPlayer
const char*
VideoPlayer::OnSetFile( const std::string& inFile )
{
  d->mRenderedPos = -1;
  Decoder().BeginConfiguration();
  if( !Decoder().Open( inFile, AVDecoder::Video, 0 ).Error() )
  {
    d->mNativeWidth = Decoder().FrameDimension( 0 );
    d->mNativeHeight = Decoder().FrameDimension( 1 );
  }
  Change();
  Decoder().EndConfiguration();
  return Decoder().Error();
}

const char*
VideoPlayer::OnAdvance( int inDecoderTicks )
{
  int ticksRemaining = inDecoderTicks,
      didAdvance = inDecoderTicks;
  while( ticksRemaining > 0 && didAdvance > 0 )
  {
    if( !Decoder().AreFramesAvailable() )
      SpendDecodingEffort();
    if( Decoder().AreFramesAvailable() )
      didAdvance = Decoder().AdvanceByTicks( ticksRemaining );
    else
      didAdvance = 0;
    ticksRemaining -= didAdvance;
  }
  SpendDecodingEffort();
  return Decoder().Error();
}

const char*
VideoPlayer::OnRenderingTick( const ClockTick& t, bool changed )
{
  if( changed )
    SetInternalPlaybackLatency( t.rate.Period() );
  SyncMemory<BeforeRead>();
  if( AVPlayer::IsOpen() && Decoder().HeadFrameTick() != d->mRenderedPos )
    Invalidate();
  return 0;
}

// GUI::GraphObject
void
VideoPlayer::OnResize( GUI::DrawContext& c )
{
  SyncMemory<BeforeRead>();
  float w = c.rect.Width(),
        h = c.rect.Height(),
        x = c.rect.CenterX(),
        y = c.rect.CenterY(),
        nativeW = NativeWidth(),
        nativeH = NativeHeight();

  switch( ScalingMode() )
  {
    case GUI::ScalingMode::AdjustWidth:
      if( nativeH > 0 )
        w = ( nativeW * h ) / nativeH;
      else
        w = 0;
      break;

    case GUI::ScalingMode::AdjustHeight:
      if( nativeW > 0 )
        h = ( nativeH * w ) / nativeW;
      else
        h = 0;
      break;

    case GUI::ScalingMode::AdjustBoth:
      w = nativeW;
      h = nativeH;
      break;

    case GUI::ScalingMode::AdjustNone:
    default:
      ;
  }
  x = Round( x );
  y = Round( y );
  w = Ceil( w );
  h = Ceil( h );
  c.rect.left = x - w / 2;
  c.rect.right = x + w / 2;
  c.rect.top = y - h / 2;
  c.rect.bottom = y + h / 2;

  Decoder().BeginConfiguration()
           .SetFrameDimension( 0, c.rect.Width() )
           .SetFrameDimension( 1, c.rect.Height() )
           .EndConfiguration();
  SyncMemory<AfterWrite>();
}

void
VideoPlayer::OnTick( const ClockTick& t )
{
  SetRenderingTick( t );
}

void
VideoPlayer::OnPaint( const GUI::DrawContext& c )
{
  GUI::Rect r = c.rect;
  int64_t tick = Decoder().HeadFrameTick();
  AVDecoder::Chunk frame = Decoder().GetFrames( 1 );
  const uint32_t* p, *pEnd;
  if( !frame || frame.GetData( p, pEnd ) != r.Width() * r.Height() )
    return;
  d->mRenderedPos = tick;
  SyncMemory<AfterWrite>();
#if USE_QT
  QImage img( reinterpret_cast<const uchar*>( p ), r.Width(), r.Height(), QImage::Format_RGB32 );
  QPainter* pPainter = c.handle.dc;
  pPainter->drawImage( QPoint( r.left, r.top ), img );
#endif // USE_QT
}

