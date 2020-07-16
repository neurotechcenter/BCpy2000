#ifndef VIDEO_THREAD_CPP_
#define VIDEO_THREAD_CPP_

#include "VideoThread.h"

#define VIDEO_PLAYS_IMMEDIATELY false

VideoThread::VideoThread( GraphDisplay& display )
: rate(16)
{
	mpMediaPlayer = new QMediaPlayer(0, QMediaPlayer::StreamPlayback);
	mpMediaPlaylist = new QMediaPlaylist;
	mpMediaPlayer->setPlaylist(mpMediaPlaylist);

	mpDisplay = &display;
	
	// QWidget*   GraphDisplay::mpWidget  is private...
	// QGLWidget* GraphDisplay::Context() is const
	// mpVideoWidget = new QVideoWidget(display.mpWidget); // This may be null!
	// mpVideoWidget = new QVideoWidget((QWidget*)display.Context().handle.glContext);
	mpVideoWidget = new QVideoWidget;
	
	// This will be [0,0,0,0] in Initialize
	SetSize(display.Context().rect);

	mpMediaPlayer->setVideoOutput(mpVideoWidget);
}

VideoThread::~VideoThread()
{
  bciout << "Deleting a video thread.";
}

VideoThread&
VideoThread::SetFile( const string& fileName )
{
	QString qFileName( fileName.c_str() ); 
	QUrl qFileUrl( QUrl::fromLocalFile( qFileName ) );
	mFileNameQLocal = qFileUrl.toLocalFile();
	qFileUrl = QUrl::fromLocalFile(mFileNameQLocal);

	// Test for file existence
	mpQFile = new QFile( mFileNameQLocal );
	if( !mpQFile->exists() )
	{
		bcierr << "Could not find video file " << fileName << ".";
		delete mpQFile;
		return *this;
	}

	// Prep the playlist
	mpMediaPlaylist->clear();
	mpMediaPlaylist->addMedia(qFileUrl);
	mpMediaPlaylist->setCurrentIndex(1);
	mpMediaPlaylist->load(qFileUrl);
	mMediaContent = mpMediaPlayer->currentMedia();

	// Run preliminary tests
	CheckMediaPlaylist( true, fileName );
	// CheckMediaPlayer( true );
	CheckMediaPlayerError( true );

	if( VIDEO_PLAYS_IMMEDIATELY )
	{
		mpMediaPlayer->setVideoOutput( (QVideoWidget*)0 );
		mpMediaPlayer->setVolume( 0 );
		mpMediaPlayer->play();
	}

  return *this;
}

VideoThread&
VideoThread::SetSize( const int& width, const int& height )
{
	// Store the size in various BCI2000 and Qt forms
	mWidth = float(width);
	mHeight = float(height);
	mSize = QSize(mWidth, mHeight);
	mQRect = QRect(0, 0, mWidth, mHeight);

	// The video widget needs to know to make itself that size
	mpVideoWidget->setMinimumSize(mSize);
	mpVideoWidget->setMaximumSize(mSize);

	return *this;
}
VideoThread&
VideoThread::SetSize( const GUI::Rect rect)
{
	return SetSize(rect.right - rect.left, rect.bottom - rect.top);
}

void
VideoThread::PrePlay()
{
	// CheckMediaPlayer( true );
	// mpVideoWidget->setParent( mpDisplay->mpWidget );
	// mpVideoWidget->move(0, 0); 
	// mpVideoWidget->show();
	if( VIDEO_PLAYS_IMMEDIATELY )
	{
		mpMediaPlayer->setPosition( 0 );
		mpMediaPlayer->stop();
		mpMediaPlayer->setVolume( 100 );
		mpMediaPlayer->setVideoOutput(mpVideoWidget);
	}
}

void
VideoThread::PostStop()
{
	mpVideoWidget->hide();
	mpMediaPlayer->stop();
	mpMediaPlayer->setPosition(0);
}

int
VideoThread::Execute()
{
	mpMediaPlayer->play();

	while( !IsTerminating() )
	{
		bciout << "Painting " << mFileName;
		// Paint();
		Sleep( rate );
	}

	return 0;
}

bool
VideoThread::CheckMediaPlaylist( bool verbose, const string& fileName )
{
	switch( mpMediaPlaylist->error() )
	{
		// Only NoError should be allowed
		case QMediaPlaylist::Error::NoError: return true;
		// Many systems will report a 'format error' despite there being none
		case QMediaPlaylist::Error::FormatError: return true;
			// verbose && bcierr << "The video file '" + fileName + "' has a format error.";
		// break;
		// Any other error is bad
		case QMediaPlaylist::Error::FormatNotSupportedError:
			verbose && bcierr << "The video file '" + fileName + "' format is not supported on this machine.";
		break;
		case QMediaPlaylist::Error::NetworkError:
			verbose && bcierr << "Network error; cannot load video file '" + fileName + "'.";
		break;
		case QMediaPlaylist::Error::AccessDeniedError:
			verbose && bcierr << "Access to video file '" + fileName + "'was denied.";
		break;
	}
	QString qerror = mpMediaPlaylist->errorString();
	string error = string(qerror.toLatin1());
	bciout << "The error was '" << error << "'.";
	return false;
}

bool
VideoThread::CheckMediaPlayer( bool verbose = false )
{
  return CheckMediaPlayerAvailability( verbose )
      && CheckMediaPlayerError( verbose )
      && CheckMediaPlayerStatus( verbose )
	    && CheckMediaPlayerBufferStatus( verbose );
}

bool
VideoThread::CheckMediaPlayerAvailability( bool verbose = false )
{
  switch( mpMediaPlayer->availability() )
  {
    // Being available is the only way to return true
    case QMultimedia::AvailabilityStatus::Available: return true;
    // Any other status is bad
    case QMultimedia::AvailabilityStatus::ServiceMissing: verbose && bcierr << "There is no service available to provide the requested video functionality."; break;
    case QMultimedia::AvailabilityStatus::ResourceError: verbose && bcierr << "The service could not allocate resource required to play video correctly."; break;
    case QMultimedia::AvailabilityStatus::Busy: verbose && bcierr << "The service must wait for access to necessary video resources."; break;
  }
	return false;
}

bool
VideoThread::CheckMediaPlayerError( bool verbose = false )
{
	switch( mpMediaPlayer->error() )
  {
			// Not having an error is the only way to return true
			case QMediaPlayer::Error::NoError: return true;
		  // Any other error, including unknown, is bad
			case QMediaPlayer::Error::ResourceError	      : verbose && bcierr << "A media resource couldn't be resolved." << " buffer: " << mpMediaPlayer->bufferStatus(); break;
      case QMediaPlayer::Error::FormatError	        : verbose && bcierr << "The format of a media resource isn't (fully) supported. Playback may still be possible, but without an audio or video component."; break;
      case QMediaPlayer::Error::NetworkError	      : verbose && bcierr << "A network error occurred."; break;
      case QMediaPlayer::Error::AccessDeniedError   : verbose && bcierr << "There are not the appropriate permissions to play a media resource."; break;
      case QMediaPlayer::Error::ServiceMissingError : verbose && bcierr << "A valid playback service was not found, playback cannot proceed."; break;
			default                                       : verbose && bcierr << "An unknown media player error occured."; break;
  }
	return false;
}

bool
VideoThread::CheckMediaPlayerStatus( bool verbose = false )
{
	switch( mpMediaPlayer->mediaStatus() ) {
			// Acceptable states (loaded, buffering, buffered, finished)
      case QMediaPlayer::LoadedMedia: 
        verbose && bciout << "The current media has been loaded. The player is in the StoppedState.";
				return true;
      case QMediaPlayer::BufferingMedia:
        verbose && bciout << "The player is buffering data but has enough data buffered for playback to continue for the immediate future. The player is in the PlayingState or PausedState.";
				return true;
       case QMediaPlayer::BufferedMedia:
        verbose && bciout << "The player has fully buffered the current media. The player is in the PlayingState or PausedState.";
        return true;
      case QMediaPlayer::EndOfMedia:
        verbose && bciout << "Playback has reached the end of the current media. The player is in the StoppedState.";
				return true;
			// Unacceptable states (unknown or no media, loading, stalled, invalid)
      case QMediaPlayer::UnknownMediaStatus: 
        verbose && bciwarn << "The status of the media cannot be determined. Does the file exist?";
				return false;
      case QMediaPlayer::NoMedia: 
        verbose && bciwarn << "There is no current media. The player is in the StoppedState.";
				return false;
      case QMediaPlayer::LoadingMedia: 
        verbose && bciwarn << "The current media is being loaded. The player may be in any state.";
				return false;
      case QMediaPlayer::StalledMedia:
        verbose && bciwarn << "Playback of the current media has stalled due to insufficient buffering or some other temporary interruption. The player is in the PlayingState or PausedState.";
				return false;
      case QMediaPlayer::InvalidMedia:
        verbose && bciwarn << "The current media (is invalid and) cannot be played. The player is in the StoppedState.";
				return false;
    }
	return false;
}

bool
VideoThread::CheckMediaPlayerBufferStatus( bool verbose = false )
{
	// Complain if the file isn't done buffering
	if( int(mpMediaPlayer->bufferStatus()) < 100 )
	{
		verbose && bcierr << "The file is not done buffering (" << mpMediaPlayer->bufferStatus() << "/100)";
		return false;
	}
	return true;
}


#endif // VIDEO_THREAD_CPP_