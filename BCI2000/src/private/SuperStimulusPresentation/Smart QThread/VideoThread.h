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
#include <QTemporaryFile>
#include <QtWidgets>

#include <QtMultimedia\QMediaPlayer>
#include <QtMultimedia\QMediaContent>
#include <QtMultimedia\QMediaPlaylist>
#include <QtMultimedia\QAbstractVideoSurface>
#include <QtMultimedia\QVideoSurfaceFormat>

using namespace std;
using namespace GUI;

class VideoStimulus;
#include "VideoStimulus.h"

#include <QThread>

class VideoThread : public QThread
{
 public:
	VideoThread() {}
	VideoThread( QObject * parent );
  virtual ~VideoThread();

	void Play();
	
 private:
	 QMediaPlayer* mpMediaPlayer;

	 VideoStimulus* mpParent;
};

#endif // VIDEO_THREAD_H_