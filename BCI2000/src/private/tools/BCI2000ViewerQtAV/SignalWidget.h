#ifndef SIGNAL_WIDGET_H
#define SIGNAL_WIDGET_H

#include <QWidget>
#include <QLabel>
#include "SignalDisplay.h"

class SignalWidget : public QWidget
{
  Q_OBJECT

 public:
  SignalWidget( QWidget* = NULL );
  SignalDisplay& Display();
  const SignalDisplay& Display() const;
  bool IsEmpty() const;
  void SetBackgroundText( const QString& inText );
  QString BackgroundText() const;
  void AddSeparatorAction();

 protected:
  // Qt event handlers
  virtual void paintEvent( QPaintEvent* );
  virtual void resizeEvent( QResizeEvent* );

 private:
  void UpdateDrawContext();

  SignalDisplay mDisplay;
  QLabel*       mpBackgroundLabel;
};

#endif // SIGNAL_WIDGET_H
