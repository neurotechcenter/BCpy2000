#ifndef VIDEO_STIMULUS_WIDGET_H_
#define VIDEO_STIMULUS_WIDGET_H_

#include "BCIStream.h"
#include <string>

#include "GraphObject.h"
#include "GraphDisplay.h"
#include "VisualStimulus.h"
#include "FileUtils.h"
#include "BCIError.h"
#include "BCIStream.h"
#include "NumericConstants.h"

#include <QObject>
#include <QFile>
#include <QMediaContent>
#include <QMediaPlayer>
#include <QVideoWidget>

#include <QMediaPlaylist>
#include <QByteArray>
#include <QBuffer>
#include <QPaintEngine>

using namespace std;
using namespace GUI;

class VideoStimulusWidget : public QVideoWidget
{
  Q_OBJECT

 public:
  VideoStimulusWidget( QWidget *parent = 0 );
  ~VideoStimulusWidget();

  VideoStimulusWidget& SetContext( const GUI::DrawContext& inDC );

 private:
   const GUI::DrawContext* mpContext;
   QPixmap* mpPixelMap;

 friend class VideoStimulus;
  virtual void OnPaint( const GUI::DrawContext& );


 public:
  // Error checking
  // Returns true on success and false on failure
  // Verbose indicates it should throw a corresponding bcierr of problems
  bool CheckDrawContext( bool verbose );
};

#endif // VIDEO_STIMULUS_WIDGET_H_