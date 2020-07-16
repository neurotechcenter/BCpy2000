////////////////////////////////////////////////////////////////////////////////
// $Id: BarGauge.h 1782 2008-02-15 11:13:42Z milsapg $
// Author: griffin.milsap@gmail.com
// Description: Displays one vertical bar for a single signal.
//
// (C) 2000-2008, BCI2000 Project
// http://www.bci2000.org
////////////////////////////////////////////////////////////////////////////////
#ifndef BAR_GAUGE_H
#define BAR_GAUGE_H

#include <string>
#include <vector>
#include "Gauge.h"
#include "Scale.h"
#include "Shapes.h"
#include "TextField.h"

class BarGauge : public Gauge
{
public:
  BarGauge( GUI::GraphDisplay &disp, int zOrder );
  ~BarGauge();

  // Virtual implementation of Gauge
  virtual void Update( );
  virtual void UpdateStatics();
  virtual void OnPaint( const GUI::DrawContext & );

private:
  RectangularShape* mpBar;
  RectangularShape* mpBorder;
  VerticalScale* mpScale;
  TextField* mpCaption;
};

#endif // BAR_GAUGE_H
