////////////////////////////////////////////////////////////////////////////////
// $Id: XYGauge.h 1782 2008-02-15 11:13:42Z milsapg $
// Author: griffin.milsap@gmail.com
// Description: This gauge displays two signals, one on the vertical axis and
//   one on the horizontal axis.  A cursor denotes current value of both signals
//
// (C) 2000-2008, BCI2000 Project
// http://www.bci2000.org
////////////////////////////////////////////////////////////////////////////////
#ifndef XY_GAUGE_H
#define XY_GAUGE_H

#include <string>
#include <vector>
#include "Gauge.h"
#include "Scale.h"
#include "Shapes.h"
#include "TextField.h"

class XYGauge : public Gauge
{
public:
  XYGauge( GUI::GraphDisplay &disp, int zOrder );
  ~XYGauge();

  // Virtual implementation of Gauge
  virtual void Update( float value1, float value2 );
  virtual void UpdateStatics();
  virtual void OnPaint( const GUI::DrawContext & );

private:
  TextField* mpCaption;
  VerticalScale* mpVScale;
  HorizontalScale* mpHScale;
  EllipticShape* mpCursor;
  RectangularShape* mpBounds;
};

#endif // XY_GAUGE_H
