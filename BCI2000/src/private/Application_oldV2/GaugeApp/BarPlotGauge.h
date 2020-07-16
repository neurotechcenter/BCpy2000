////////////////////////////////////////////////////////////////////////////////
// $Id: BarPlotGauge.h 1782 2008-02-15 11:13:42Z milsapg $
// Author: griffin.milsap@gmail.com
// Description: This gauge displays one signal in multiple vertical bars,
//   displaying the relative frquency of each sub-division defined in precision.
//
// (C) 2000-2008, BCI2000 Project
// http://www.bci2000.org
////////////////////////////////////////////////////////////////////////////////
#ifndef BAR_PLOT_GAUGE_H
#define BAR_PLOT_GAUGE_H

#include <string>
#include <vector>
#include "Gauge.h"
#include "Scale.h"
#include "Shapes.h"
#include "TextField.h"

class BarPlotGauge : public Gauge
{
public:
  BarPlotGauge( GUI::GraphDisplay &disp, int zOrder );
  ~BarPlotGauge();

  // Virtual implementation of Gauge
  virtual void Update( float value1, float value2 );
  virtual void UpdateStatics();
  virtual void Reset();
  virtual void OnPaint( const GUI::DrawContext & );
  virtual bool OnClick( const GUI::Point & );

private:
  std::vector< RectangularShape* > mpBars;
  VerticalScale* mpVScale;
  HorizontalScale* mpHScale;
  TextField* mpCaption;
  std::vector< long > mCounts;
  long mNumSamples;
};

#endif // BAR_PLOT_GAUGE_H
