#ifndef VIDEO_STIMULUS_WIDGET_CPP_
#define VIDEO_STIMULUS_WIDGET_CPP_

#include "VideoStimulusWidget.h"

VideoStimulusWidget::VideoStimulusWidget( QWidget *parent )
: QVideoWidget( parent )
, mpContext( NULL )
{
  // setAutoFillBackground( false );
  // setAttribute( Qt::WA_NoSystemBackground, true );
  // setAttribute( Qt::WidgetAttribute::WA_OpaquePaintEvent, true );
  // setAttribute( Qt::WidgetAttribute::WA_PaintOnScreen, true );
  // setAttribute( Qt::WidgetAttribute::WA_StaticContents, true );

  QPalette palette = this->palette();
  palette.setColor( QPalette::Background, Qt::black );
  setPalette( palette );

  setSizePolicy( QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding );
}

VideoStimulusWidget::~VideoStimulusWidget()
{
}

VideoStimulusWidget&
VideoStimulusWidget::SetContext( const GUI::DrawContext& inDC )
{
  mpContext = &inDC;
  CheckDrawContext( true );
  return *this;
}

void
VideoStimulusWidget::OnPaint( const GUI::DrawContext& inDC )
{
  // Test code
  inDC.handle.painter->fillRect( 0, 0, width(), height(), Qt::blue );

  bool test1 = true,
       test2 = true;
  
  if( test1 )
  {
    // Solution one
    this->render( inDC.handle.painter );
  }

  if( test2 )
  {
    // Solution two
    inDC.handle.painter->drawPixmap(
      ::Floor( inDC.rect.left ),
      ::Floor( inDC.rect.top ),
      width(), height(),
      this->grab() );
  }
}

bool
VideoStimulusWidget::CheckDrawContext( bool verbose = false )
{
  if( !mpContext )
    verbose && bcierr << "A BCI2000 DrawContext was not provided to the video widget.";
  else if( !mpContext->handle.device )
    verbose && bcierr << "The BCI2000 DrawContext does not have a valid device for the painter.";
  // else if( !mpContext->handle.glContext )
  //   verbose && bcierr << "The BCI2000 DrawContext does not have a valid context GL widget.";
  // else if( !mpContext->handle.painter )
  //   verbose && bcierr << "The BCI2000 DrawContext does not have a valid painter.";
  else
    return true;

  return false;
}

#endif // VIDEO_STIMULUS_WIDGET_CPP_