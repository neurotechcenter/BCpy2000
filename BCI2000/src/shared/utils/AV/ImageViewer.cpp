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
#include "ImageViewer.h"

#include "AVDecoder.h"
#include "FileUtils.h"
#include "Lockable.h"

#if USE_QT
# include <QPainter>
#endif

struct ImageViewer::Data : Lockable<NonrecursiveSpinLock>
{
  String mFile, mError;
  AVDecoder mDecoder;
  int mNativeWidth, mNativeHeight;
};

// ImageViewer
ImageViewer::ImageViewer( GUI::GraphDisplay& display, int zOrder )
: GraphObject( display, zOrder ),
  d( new Data )
{
  WithLock( d )
  {
    d->mNativeWidth = 0;
    d->mNativeHeight = 0;
  }
}

ImageViewer::~ImageViewer()
{
  DestructorEntered();
  Close();
  delete d;
}

int
ImageViewer::NativeWidth() const
{
  SyncMemory<BeforeRead>();
  return d->mNativeWidth;
}

int
ImageViewer::NativeHeight() const
{
  SyncMemory<BeforeRead>();
  return d->mNativeHeight;
}

const String&
ImageViewer::Error() const
{
  SyncMemory<BeforeRead>();
  if( !d->mError && d->mDecoder.Error() )
    d->mError = d->mDecoder.Error();
  return d->mError;
}

const String&
ImageViewer::File() const
{
  SyncMemory<BeforeRead>();
  return d->mFile;
}

String
ImageViewer::Codec() const
{
  SyncMemory<BeforeRead>();
  return d->mDecoder.Codec();
}

String
ImageViewer::ContainerFormat() const
{
  SyncMemory<BeforeRead>();
  return d->mDecoder.ContainerFormat();
}

bool
ImageViewer::IsOpen() const
{
  SyncMemory<BeforeRead>();
  return !d->mFile.empty();
}

ImageViewer&
ImageViewer::SetFile( const std::string& s )
{
  WithLock( d )
  {
    std::string file = s.empty() ? "" : FileUtils::RealPath( s );
    if( !file.empty() && !FileUtils::IsFile(file) )
    {
      d->mDecoder.Close();
      d->mError = "File \"" + file + "\" does not exist";
      return *this;
    }

    if( File() == file && !Error() )
      return *this;

    d->mDecoder.Close();
    d->mError.clear();

    if( file.empty() )
      return *this;

    d->mDecoder.BeginConfiguration();
    if( !d->mDecoder.Open( s, AVDecoder::Image, 0 ).Error() )
    {
      d->mNativeWidth = d->mDecoder.FrameDimension( 0 );
      d->mNativeHeight = d->mDecoder.FrameDimension( 1 );
      d->mFile = file;
      Change();
    }
    d->mDecoder.EndConfiguration();
    if( d->mDecoder.Error() )
      d->mError = "Could not open media file \"" + file + "\": " + d->mDecoder.Error();
    else
      d->mDecoder.DecodeToBuffer();
  }
  return *this;
}

// GUI::GraphObject
void
ImageViewer::OnResize( GUI::DrawContext& c )
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

  WithLock( d )
    d->mDecoder.BeginConfiguration()
               .SetFrameDimension( 0, c.rect.Width() )
               .SetFrameDimension( 1, c.rect.Height() )
               .EndConfiguration();
}

void
ImageViewer::OnPaint( const GUI::DrawContext& c )
{
  GUI::Rect r = c.rect;
  AVDecoder::Chunk frame;
  WithTryLock( d )
    frame = d->mDecoder.GetFrames( 1 );
  const uint32_t* p, *pEnd;
  int pixelCount = frame.GetData( p, pEnd );
  if( !frame || pixelCount != r.Width() * r.Height() )
    return;
#if USE_QT
  QImage img( reinterpret_cast<const uchar*>( p ), r.Width(), r.Height(), QImage::Format_RGB32 );
  QPainter* pPainter = c.handle.dc;
  pPainter->drawImage( QPoint( r.left, r.top ), img );
#endif // USE_QT
}
