////////////////////////////////////////////////////////////////////////////////
// $Id: VideoStimulus.cpp 4597 2013-12-03 14:04:06Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de, josh.goldberg.819@gmail.com
// Description: A stimulus consisting of a video.
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
#ifndef VIDEO_STIMULUS_CPP
#define VIDEO_STIMULUS_CPP

#include "PCHIncludes.h"
#pragma hdrstop

#include "VideoStimulus.h"

// connect(this, &VideoStimulus::stateChanged, this, &VideoStimulus::mentionState);
// // or
// connect(this, SIGNAL(stateChanged(int)), this, SLOT(mentionState(int)) );
// // with
// emit stateChanged();
VideoStimulus::VideoStimulus( GraphDisplay& display )
: GraphObject( display, ImageStimulusZOrder )
, mFileName( "" )
, mError( "" )
, doStretch( false )
, mPresenting( false )
, count( 0 )
{
	mpVideoWidget = new QVideoWidget;
#if VIDEO_STIMULUS_PRIVATE_DISPLAY
  mpVideoWidget->setParent( display.mpWidget );
  mpVideoWidget->hide();
#else
  mpVideoWidget->show();
#endif // VIDEO_STIMULUS_PRIVATE_DISPLAY
  
  mpMediaPlayer = new QMediaPlayer( 0, QMediaPlayer::LowLatency );
  // Setting the video output here works, but causes severe lag.
#if VIDEO_STIMULUS_USE_VIDEO_WIDGET
  mpMediaPlayer->setVideoOutput( mpVideoWidget );
#endif // VIDEO_STIMULUS_USE_VIDEO_WIDGET

  mpGraphicsScene = new QGraphicsScene;

  mpGraphicsVideoItem = new QGraphicsVideoItem;
  mpGraphicsScene->addItem( mpGraphicsVideoItem );
#if VIDEO_STIMULUS_USE_GRAPHICS_DISPLAY
  // mpMediaPlayer->setVideoOutput( mpGraphicsVideoItem );
#endif // VIDEO_STIMULUS_USE_GRAPHICS_DISPLAY

  mpGraphicsView = new QGraphicsView( mpGraphicsScene );
#if VIDEOS_STIMULUS_USE_GRAPHICS_DISPLAY
  mpGraphicsView->show();
#else
  mpGraphicsView->hide();
#endif // VIDEO_STIMULUS_USE_GRAPHICS_DISPLAY

	// This will be [0,0,0,0] from SuperStimulusPresentation::Initialize and the actual size elsewhere
	SetSize(display.Context().rect);
  CheckMediaPlayerError( true );
}

VideoStimulus::~VideoStimulus()
{
	OnConceal();

	mpVideoWidget->deleteLater();
	mpMediaPlayer->deleteLater();
}


void
VideoStimulus::OnPresent()
{
  VisualStimulus::OnPresent();
	mpVideoWidget->move( 0, 0 );
	mpVideoWidget->show();
  
#if VIDEOS_STIMULUS_USE_GRAPHICS_DISPLAY
  mpGraphicsView->show();
#endif // VIDEOS_STIMULUS_USE_GRAPHICS_DISPLAY

#if VIDEO_STIMULUS_USE_GRAPHICS_DISPLAY
  mpMediaPlayer->setVideoOutput( mpGraphicsVideoItem );
#endif // VIDEO_STIMULUS_USE_GRAPHICS_DISPLAY
  
	mpMediaPlayer->play();
  CheckMediaPlayer( true );
  mPresenting = true;
}

void
VideoStimulus::OnConceal()
{
  mPresenting = false;
  VisualStimulus::OnConceal();
	mpMediaPlayer->stop();
  mpGraphicsView->hide();
	mpVideoWidget->hide();
}

void
VideoStimulus::OnPaint( const GUI::DrawContext& inDC )
{
  if( !mPresenting ) return;

#if VIDEO_STIMULUS_USE_VIDEO_WIDGET
  // This seems to render a color, which is changeable by setPalette
  mpVideoWidget->render( inDC.handle.painter );
  // For testing purposes, also render it to an external file
#if VIDEO_STIMULUS_SAVE_RENDERS
  if( ++count % 7 == 0 )
  {
    QPixmap pixmap( mpVideoWidget->size() );
    mpVideoWidget->render( &pixmap );
    pixmap.save( QString( "rendered/VideoWidget Render " ) + QString::number( count ) + QString(".jpg") );
  }
#endif // VIDEO_STIMULUS_SAVE_RENDERS
#endif // VIDEO_STIMULUS_USE_VIDEO_WIDGET
  
#if VIDEO_STIMULUS_USE_GRAPHICS_DISPLAY
  mpGraphicsScene->update();
  mpGraphicsView->update();
  mpGraphicsView->render( inDC.handle.painter );
  /*
  QStyleOptionGraphicsItem styleoption;
  mpGraphicsVideoItem->paint( 
    inDC.handle.painter,
    &styleoption,
    mpGraphicsView
  );
  */
#endif // VIDEO_STIMULUS_USE_GRAPHICS_DISPLAY

}

VideoStimulus&
VideoStimulus::SetFile( const string& name )
{
  QFile* pFile = new QFile( QString( name.c_str() ) );
  if( !pFile->exists() || !pFile->open( QIODevice::ReadOnly ) )
  {
    bcierr << name << " could not be opened.";
    return *this;
  }
  
  mpMediaPlayer->setMedia( QUrl::fromLocalFile( name.c_str() ) );
	mFileName = name;
	return *this;
}

VideoStimulus&
VideoStimulus::SetVolume( const double& volume )
{
  mpMediaPlayer->setVolume( volume * 100 );
	return *this;
}

VideoStimulus&
VideoStimulus::SetRate( const qreal& rate )
{
  mpMediaPlayer->setPlaybackRate( rate );
	return *this;
}

VideoStimulus&
VideoStimulus::SetSize()
{
  return SetSize( mWidth, mHeight );
}
VideoStimulus&
VideoStimulus::SetSize( const GUI::Rect rect )
{
  SetObjectRect( rect );
	return SetSize(rect.right - rect.left, rect.bottom - rect.top);
}
VideoStimulus&
VideoStimulus::SetSize( const int& width, const int& height )
{
	mWidth = float(width);
	mHeight = float(height);
	mSize = QSize(mWidth, mHeight);
	mQRect = QRect(0, 0, mWidth, mHeight);

  if( doStretch )
  {
    mpVideoWidget->setMinimumWidth( width );
    mpVideoWidget->setMinimumHeight( height );
    mpGraphicsView->setMinimumWidth( width );
    mpGraphicsView->setMinimumHeight( height );
  }
  else
  {
    mpVideoWidget->resize( width, height );
    mpGraphicsView->resize( width, height );
  }

	return *this;
}

VideoStimulus&
VideoStimulus::SetColor( const RGBColor color )
{
  QPalette palette;
  // palette.setColor( QPalette::Background, QColor( color.ToQRgb() ) );
  // palette.setColor( QPalette::Background, Qt::blue );
  palette.setColor( QPalette::ColorRole::Background, QColor( 0, 0, 0, 0 ) );
  mpVideoWidget->setPalette( palette );
  
  return *this;
}

VideoStimulus&
VideoStimulus::SetStretching( bool enabled )
{
  doStretch = enabled;
  mpVideoWidget->setAspectRatioMode( enabled ? Qt::IgnoreAspectRatio : Qt::KeepAspectRatio);
  return SetSize();
}

bool
VideoStimulus::CheckMediaPlayer( bool verbose = false )
{
  return CheckMediaPlayerError       ( verbose )
      && CheckMediaPlayerAvailability( verbose )
      && CheckMediaPlayerStatus      ( verbose );
}

bool
VideoStimulus::CheckMediaPlayerError( bool verbose = false )
{
  switch( mpMediaPlayer->error() )
  {
    // Not having any error is the only way to go
    case QMediaPlayer::Error::NoError:
      mError = "";
      return true;
    // Any other error is bad
    case QMediaPlayer::Error::ResourceError:
      verbose && bcierr << "A media resource couldn't be resolved. (" << mFileName << ")";
    break;
    case QMediaPlayer::Error::FormatError: 
      verbose && bcierr << "The format of a media resource isn't supported. Playback may still be possible, but without an audio or video component. (" << mFileName << ")";
    break;
    case QMediaPlayer::Error::NetworkError: 
      verbose && bcierr << "A network error occurred. (" << mFileName << ")";
    break;
    case QMediaPlayer::Error::AccessDeniedError: 
      verbose && bcierr << "There are not the appropriate permissions to play a media resource. (" << mFileName << ")";
    break;
    case QMediaPlayer::Error::ServiceMissingError:
      verbose && bcierr << "A valid playback service was not found, playback cannot proceed. (" << mFileName << ")";
    break;
  }
  mError = mpMediaPlayer->errorString().toUtf8();
	return false;
}

bool
VideoStimulus::CheckMediaPlayerAvailability( bool verbose = false )
{
  switch( mpMediaPlayer->availability() ) 
  {
    // Being completely available is the only way to go
    case QMultimedia::AvailabilityStatus::Available:
      mError = "";
      return true;
    // Any other status is bad
    case QMultimedia::AvailabilityStatus::ServiceMissing:
      verbose && bcierr << "There is no service available to provide the requested video functionality. (" << mFileName << ")";
    break;
    case QMultimedia::AvailabilityStatus::Busy:
      verbose && bcierr << "The service must wait for access to necessary video resources. (" << mFileName << ")";
    break;
    case QMultimedia::AvailabilityStatus::ResourceError:
      verbose && bcierr << "The service could not allocate resources required to play video correctly. (" << mFileName << ")";
    break;
  }
  mError = mpMediaPlayer->errorString().toUtf8();
	return false;
}

bool
VideoStimulus::CheckMediaPlayerStatus( bool verbose = false )
{
	switch( mpMediaPlayer->mediaStatus() ) {
			// Unacceptable states (unknown or no media, loading, buffering, stalled, invalid)
      case QMediaPlayer::UnknownMediaStatus: 
        verbose && bcierr << "The status of the media cannot be determined. (" << mFileName << ")";
			break;
      case QMediaPlayer::NoMedia: 
        verbose && bcierr << "There is no current media. The player is in the StoppedState. (" << mFileName << ")";
			break;
      case QMediaPlayer::LoadingMedia: 
        verbose && bcierr << "The current media is being loaded. The player may be in any state. (" << mFileName << ")";
			break;
      case QMediaPlayer::StalledMedia:
        verbose && bcierr << "Playback of the current media has stalled due to insufficient buffering or some other temporary interruption. The player is in the PlayingState or PausedState. (" << mFileName << ")";
			break;
      case QMediaPlayer::BufferingMedia:
        verbose && bcierr << "The player is buffering data but has enough data buffered for playback to continue for the immediate future. The player is in the PlayingState or PausedState. This may cause blocking during playback. (" << mFileName << ")";
			break;
      case QMediaPlayer::InvalidMedia:
        verbose && bcierr << "The current media (is invalid and) cannot be played. The player is in the StoppedState. (" << mFileName << ")";
			break;
			// Acceptable states (loaded, buffered, finished)
      default:
        mError = "";
        return true;
    }
  mError = mpMediaPlayer->errorString().toUtf8();
	return false;
}

#endif // VIDEO_STIMULUS_CPP