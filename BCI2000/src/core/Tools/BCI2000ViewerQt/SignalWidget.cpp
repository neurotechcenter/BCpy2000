#include "SignalWidget.h"
#include <QtWidgets>

SignalWidget::SignalWidget( QWidget* inParent )
: QWidget( inParent ),
  mpBackgroundLabel( new QLabel( this ) )
{
  UpdateDrawContext();
  mpBackgroundLabel->setAlignment( Qt::AlignCenter );
}

SignalDisplay&
SignalWidget::Display()
{
  return mDisplay;
}

const SignalDisplay&
SignalWidget::Display() const
{
  return mDisplay;
}

bool
SignalWidget::IsEmpty() const
{
  return mDisplay.NumSamples() == 0;
}

void
SignalWidget::SetBackgroundText( const QString& inText )
{
  mpBackgroundLabel->setText( inText );
}

QString
SignalWidget::BackgroundText() const
{
  return mpBackgroundLabel->text();
}

void
SignalWidget::AddSeparatorAction()
{
  QAction* pSeparator = new QAction( NULL );
  pSeparator->setSeparator( true );
  this->addAction( pSeparator );
}

void
SignalWidget::paintEvent( QPaintEvent* inpEvent )
{
  if( IsEmpty() )
  {
    mpBackgroundLabel->setVisible( true );
    QWidget::paintEvent( inpEvent );
  }
  else
  {
    mpBackgroundLabel->setVisible( false );
    QApplication::setOverrideCursor( Qt::WaitCursor );
    mDisplay.Paint( const_cast<void*>(
      reinterpret_cast<const void*>( &( inpEvent->region() ) ) ) );
    QApplication::restoreOverrideCursor();
  }
}

void
SignalWidget::resizeEvent( QResizeEvent* inpEvent )
{
  UpdateDrawContext();
  QWidget::resizeEvent( inpEvent );
}

void
SignalWidget::UpdateDrawContext()
{
  GUI::DrawContext dc =
  {
    { 0, 0, this->width(), this->height() },
    0
  };
  dc.device = this;
  mDisplay.SetContext( dc );
  mpBackgroundLabel->setGeometry( 0, 0, this->width(), this->height() );
}
