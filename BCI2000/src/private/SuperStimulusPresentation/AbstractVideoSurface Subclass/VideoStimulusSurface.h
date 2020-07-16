#ifndef VIDEO_STIMULUS_SURFACE_H_
#define VIDEO_STIMULUS_SURFACE_H_

#include <QAbstractVideoSurface>
#include <QVideoSurfaceFormat>
#include <QImage>
#include <QRect>
#include <QVideoFrame>
#include <QPainter>

#include "BCIStream.h"
#include <string>

class VideoStimulusSurface : public QAbstractVideoSurface
{
 public:
  VideoStimulusSurface( QObject *parent = 0 );
  QList<QVideoFrame::PixelFormat> supportedPixelFormats(
    QAbstractVideoBuffer::HandleType handleType = QAbstractVideoBuffer::NoHandle
  ) const;
  bool isFormatSupported( const QVideoSurfaceFormat &format ) const;

  bool start( const QVideoSurfaceFormat &format );
  void stop();
  bool present( const QVideoFrame &frame );

  void updateVideoRect();
  void paint(QPainter *painter);
  
 private:
  QImage::Format imageFormat;
  QRect targetRect;
  QSize imageSize;
  QRect sourceRect;
  QVideoFrame currentFrame;
};

#endif // VIDEO_STIMULUS_SURFACE_H_