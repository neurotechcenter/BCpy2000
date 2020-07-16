////////////////////////////////////////////////////////////////////////////////
// $Id: ImageStimulus.cpp 5453 2016-07-19 01:09:01Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A stimulus consisting of an image.
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
#include "PCHIncludes.h"
#pragma hdrstop

#include "ImageStimulus.h"

#include "FileUtils.h"
#include "BCIStream.h"
#include "NumericConstants.h"

#if USE_QT
# include <QPixmap>
# include <QImage>
# include <QPainter>
# include <QBitmap>
#endif

using namespace std;
using namespace GUI;

#if USE_QT
struct ImageStimulusPrivateData
{
  QImage* mpImage;
  QPixmap* mpImageBufferNormal,
         * mpImageBufferHighlighted;

  ImageStimulusPrivateData()
  : mpImage( 0 ),
    mpImageBufferNormal( 0 ),
    mpImageBufferHighlighted( 0 )
  {}
  ~ImageStimulusPrivateData()
  {
    delete mpImage;
    delete mpImageBufferNormal;
    delete mpImageBufferHighlighted;
  }
};

static QPixmap*
NewBufferFromImage( QImage& inImage, bool inTransparent )
{
  QPixmap* pBuffer = new QPixmap( QPixmap::fromImage( inImage ) );
  if( inTransparent && !inImage.hasAlphaChannel() )
  {
    QRgb c = inImage.pixel( 0, 0 );
    QImage mask = inImage.createMaskFromColor( c, Qt::MaskInColor );
    pBuffer->setMask( QBitmap::fromImage( mask ) );
  }
  return pBuffer;
}

#endif // USE_QT

ImageStimulus::ImageStimulus( GraphDisplay& display )
: GraphObject( display, ImageStimulusZOrder ),
  mRenderingMode( RenderingMode::Opaque ),
  mpData( new ImageStimulusPrivateData )
{
}

ImageStimulus::~ImageStimulus()
{
  DestructorEntered();
  delete mpData;
}

ImageStimulus&
ImageStimulus::SetFile( const string& inName )
{
  bool errorOccurred = false;

  // Attempt to load the image
#if USE_QT
  delete mpData->mpImage;
  mpData->mpImage = new QImage();
  errorOccurred = !mpData->mpImage->load( QString( FileUtils::AbsolutePath( inName ).c_str() ) );
#endif

  // An error occurred while loading the image
  if( errorOccurred )
  {
    bcierr << "Could not load image from file \"" << inName << "\"" << endl;
    delete mpData->mpImage;
    mpData->mpImage = NULL;
    mFile = "";
  }
  else
    mFile = inName;

  Change();
  return *this;
}

const string&
ImageStimulus::File() const
{
  return mFile;
}

ImageStimulus&
ImageStimulus::SetRenderingMode( int inMode )
{
  if( inMode != mRenderingMode )
  {
    mRenderingMode = inMode;
    Change();
  }
  return *this;
}

int
ImageStimulus::RenderingMode() const
{
  return mRenderingMode;
}

#if USE_QT
int
ImageStimulus::NativeWidth() const
{
  return mpData->mpImage ? mpData->mpImage->width() : 0;
}

int
ImageStimulus::NativeHeight() const
{
  return mpData->mpImage ? mpData->mpImage->height() : 0;
}

void
ImageStimulus::OnPaint( const DrawContext& inDC )
{
  // Draw the proper buffered image using the given DrawContext
  QPixmap* pBuffer = BeingPresented() ?
                     mpData->mpImageBufferHighlighted :
                     mpData->mpImageBufferNormal;
  if( pBuffer != NULL )
  {
    int width = ::Floor( inDC.rect.Width() ),
        height = ::Floor( inDC.rect.Height() );
    if( width == pBuffer->width() && height == pBuffer->height() )
      inDC.handle.dc->drawPixmap(
        ::Floor( inDC.rect.left ),
        ::Floor( inDC.rect.top ),
        *pBuffer );
    else
      inDC.handle.dc->drawPixmap(
        ::Floor( inDC.rect.left ),
        ::Floor( inDC.rect.top ),
        width, height,
        *pBuffer );
  }
}
#endif

void
ImageStimulus::OnResize( DrawContext& ioDC )
{
  if( AspectRatioMode() == GUI::AspectRatioModes::AdjustBoth )
    OnMove( ioDC );
  else
    OnChange( ioDC );
}

void
ImageStimulus::OnMove( DrawContext& ioDC )
{
  AdjustRect( ioDC.rect );
}

void
ImageStimulus::OnChange( DrawContext& ioDC )
{
  delete mpData->mpImageBufferNormal;
  mpData->mpImageBufferNormal = NULL;
  delete mpData->mpImageBufferHighlighted;
  mpData->mpImageBufferHighlighted = NULL;

  AdjustRect( ioDC.rect );
  int width = ioDC.rect.Width(),
      height = ioDC.rect.Height();

#if USE_QT

  if( mpData->mpImage != NULL )
  {
    bool storeScaled = width * height < 2 * OriginalWidth() * OriginalHeight();
    storeScaled |= width < OriginalWidth();
    storeScaled |= height < OriginalHeight();
    QImage img = storeScaled ? mpData->mpImage->scaled( width, height ) : *mpData->mpImage;
    // Create the normal pixmap
    if( PresentationMode() != ShowHide )
      mpData->mpImageBufferNormal = NewBufferFromImage( img, mRenderingMode == GUI::RenderingMode::Transparent );
    // Create the highlighted pixmap by modifying img
    switch( PresentationMode() )
    {
      case ShowHide:
        delete mpData->mpImageBufferNormal;
        mpData->mpImageBufferNormal = NULL;
        break;

      case Intensify:
        img = img.convertToFormat( QImage::Format_Indexed8 );
        for( int i = 0; i < img.colorCount(); ++i )
          img.setColor( i, QColor( img.color( i ) ).lighter( static_cast<int>( 100 * DimFactor() ) ).rgb() );
        break;

      case Grayscale:
        img = img.convertToFormat( QImage::Format_Indexed8 );
        for( int i = 0; i < img.colorCount(); ++i )
          img.setColor( i, QColor( img.color( i ) ).value() );
        break;

      case Invert:
        img.invertPixels();
        break;

      case Dim:
        img = img.convertToFormat( QImage::Format_Indexed8 );
        for( int i = 0; i < img.colorCount(); ++i )
          img.setColor( i, QColor( img.color( i ) ).darker( static_cast<int>( 100 * DimFactor() ) ).rgb() );
        break;

	  case HideShow:
        break;
    }
    if ( PresentationMode() != HideShow )
	{
		mpData->mpImageBufferHighlighted = NewBufferFromImage( img, mRenderingMode == GUI::RenderingMode::Transparent );
	}
	else
	{
        delete mpData->mpImageBufferHighlighted;
        mpData->mpImageBufferHighlighted = NULL;
	}
  }

#endif // USE_QT

}

void
ImageStimulus::AdjustRect( GUI::Rect& ioRect ) const
{
  if( mpData->mpImage != NULL )
  {
    int width = ioRect.Width(),
        height = ioRect.Height(),
        hCenter = ( ioRect.left + ioRect.right ) / 2,
        vCenter = ( ioRect.bottom + ioRect.top ) / 2,
        imageWidth = NativeWidth(),
        imageHeight = NativeHeight();

    switch( AspectRatioMode() )
    {
      case GUI::AspectRatioModes::AdjustWidth:
        width = ( imageWidth * height ) / imageHeight;
        ioRect.left = hCenter - width / 2;
        ioRect.right = ioRect.left + width;
        break;

      case GUI::AspectRatioModes::AdjustHeight:
        height = ( imageHeight * width ) / imageWidth;
        ioRect.top = vCenter - height / 2;
        ioRect.bottom = ioRect.top + height;
        break;

      case GUI::AspectRatioModes::AdjustBoth:
        width = imageWidth;
        height = imageHeight;
        ioRect.left = hCenter - width / 2;
        ioRect.right = ioRect.left + width;
        ioRect.top = vCenter - height / 2;
        ioRect.bottom = ioRect.top + height;
        break;

      case GUI::AspectRatioModes::AdjustNone:
      default:
        ;
    }
  }
}

