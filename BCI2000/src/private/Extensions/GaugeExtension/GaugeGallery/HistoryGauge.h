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
  virtual void Update( );
  virtual void UpdateStatics();
  virtual void OnPaint( const GUI::DrawContext & );
  virtual bool OnClick( const GUI::Point & ); 

  struct Entry
  {
    float value;
    unsigned short timestamp;
    Entry() { value = 0.0f; timestamp = 0; }
  };
  typedef std::list< Entry > EntryList;

  // Public Methods
  void ClearHistory();
  void SetHistoryLength( float seconds ) { mHistoryLength = seconds; Invalidate(); }
  EntryList & GetHistory( unsigned int whichDim ) { while( whichDim >= mHistories.size() ) mHistories.push_back( EntryList() ); return mHistories[ whichDim ]; }

private:
  VerticalScale* mpVScale;
  HorizontalScale* mpHScale;
  RectangularShape* mpBounds;
  TextField* mpCaption;
  TextField* mpTimeLabel;
  std::vector< EntryList > mHistories;
  float mHistoryLength; // Seconds
};

#endif // HISTORY_GAUGE_H
