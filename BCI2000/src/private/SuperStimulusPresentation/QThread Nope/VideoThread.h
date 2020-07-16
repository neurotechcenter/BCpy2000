#ifndef VIDEO_THREAD_H_
#define VIDEO_THREAD_H_

#include "PCHIncludes.h"
#pragma hdrstop

#include <string>
#include "FileUtils.h"
#include "BCIError.h"
#include "NumericConstants.h"
#include "BCIStream.h"
#include "OSThread.h"
#include "GraphObject.h"
#include "GraphDisplay.h"

#include <QUrl>
#include <QFile>
#include <QList>
#include <QWidget>
#include <QBuffer>
#include <QVideoWidget>
#include <QBoxLayout>

#include <QtMultimedia\QMediaPlayer>
#include <QtMultimedia\QMediaContent>
#include <QtMultimedia\QMediaPlaylist>
#include <QtMultimedia\QAbstractVideoSurface>
#include <QtMultimedia\QVideoSurfaceFormat>

using namespace std;
using namespace GUI;

class VideoThread : public OSThread
{
 public:
  VideoThread( GraphDisplay& display );
  virtual ~VideoThread();
	
	// Property setting
  VideoThread& SetFile( const string& fileName );
	VideoThread& SetSize( const GUI::Rect rect   );
	VideoThread& SetSize( const int& width,
												const int& height      );

	// Custom Actions
	// These can't be in Execute because they must be in the main thread
	virtual void PrePlay();
	virtual void PostStop();

	// Inherited Actions
	virtual int Execute();
	virtual int Paint();
	
  // Error checking
	bool CheckMediaPlaylist          ( bool verbose, const string& fileName );
	bool CheckMediaPlayer            ( bool verbose );
	bool CheckMediaPlayerAvailability( bool verbose );
	bool CheckMediaPlayerError       ( bool verbose );
	bool CheckMediaPlayerStatus      ( bool verbose );
	bool CheckMediaPlayerBufferStatus( bool verbose );
    
 // friend class VideoStimulus;
 private:
	GraphDisplay* mpDisplay;

	// Qt5 Multimedia objects
	QMediaPlayer* mpMediaPlayer;
	QMediaPlaylist* mpMediaPlaylist;
	QVideoWidget* mpVideoWidget;
	QBoxLayout* mpBoxLayout;
	QMediaContent mMediaContent;

	// File information
	string mFileName;
	QString mFileNameQLocal;
	QFile* mpQFile;

	// Size information
	GUI::Rect mRect;
	QRect mQRect;
	QSize mSize;
	float mWidth, mHeight;
	int rate;
};

#include "VideoThread.cpp"

#endif // VIDEO_THREAD_H_