////////////////////////////////////////////////////////////////////////////////
// $Id: HistoryGauge.h 1782 2008-02-15 11:13:42Z milsapg $
// Author: griffin.milsap@gmail.com
// Description: The history gauge displays the value of a signal on the vertical
//   axis and previous values of said signal for different values in recent
//   history.
//
// (C) 2000-2008, BCI2000 Project
// http://www.bci2000.org
////////////////////////////////////////////////////////////////////////////////
#ifndef HISTORY_GAUGE_H
#define HISTORY_GAUGE_H

#include <string>
#include <list>
#include "Gauge.h"
#include "Scale.h"
#include "Shapes.h"
#include "TextField.h"

class HistoryGauge : public Gauge
{
public:
  HistoryGauge( GUI::GraphDisplay &disp, int zOrder );
  ~HistoryGauge();

  // Virtual implementation of Gauge
  virtual void Update( float value1, float value2 );
  virtual void UpdateStatics();
  virtual void OnPaint( const GUI::DrawContext & );
  virtual bool OnClick( const GUI::Point & ); 

  // Public Methods
  void ClearHistory();
  void SetHistoryLength( float seconds ) { mHistoryLength = seconds; Invalidate(); }

  struct Entry
  {
    float value;
    unsigned short timestamp;
    Entry() { value = 0.0f; timestamp = 0; }
  };

private:
  VerticalScale* mpVScale;
  HorizontalScale* mpHScale;
  RectangularShape* mpBounds;
  TextField* mpCaption;
  TextField* mpTimeLabel;
  std::list< Entry > mHistory;
  std::list< Entry > mOldHistory;
  float mHistoryLength; // Seconds
};

#endif // HISTORY_GAUGE_H
