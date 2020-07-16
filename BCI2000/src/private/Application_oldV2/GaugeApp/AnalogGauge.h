////////////////////////////////////////////////////////////////////////////////
// $Id: AnalogGauge.h 1782 2008-02-15 11:13:42Z milsapg $
// Author: griffin.milsap@gmail.com
// Description: This gauge displays a signal in a traditional analog gauge
//   format.
//
// (C) 2000-2008, BCI2000 Project
// http://www.bci2000.org
////////////////////////////////////////////////////////////////////////////////
#ifndef ANALOG_GAUGE_H
#define ANALOG_GAUGE_H

#include <string>
#include <vector>
#include "Gauge.h"
#include "Scale.h"
#include "Shapes.h"
#include "TextField.h"

class AnalogGauge : public Gauge
{
public:
  AnalogGauge( GUI::GraphDisplay &disp, int zOrder );
  ~AnalogGauge();

  // Virtual implementation of Gauge
  virtual void Update( float value1, float value2 );
  virtual void OnPaint( const GUI::DrawContext &inDC );
  virtual void UpdateStatics();

private:
  HorizontalScale* mpScale;
  TextField* mpCaption;
  float mCurVal; // Normalized (0-1)
};

#endif // ANALOG_GAUGE_H
