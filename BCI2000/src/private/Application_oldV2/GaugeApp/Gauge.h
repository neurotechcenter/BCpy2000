////////////////////////////////////////////////////////////////////////////////
// $Id: Gauge.h 1782 2008-02-15 11:13:42Z milsapg $
// Author: griffin.milsap@gmail.com
// Description: Handles rendering and maintenance of a gauge
//
// (C) 2000-2008, BCI2000 Project
// http://www.bci2000.org
////////////////////////////////////////////////////////////////////////////////
#ifndef GAUGE_H
#define GAUGE_H

#include <string>
#include <vector>
#include "Scale.h"
#include "Shapes.h"
#include "TextField.h"
#include "GraphObject.h"

// Gauge is the parent class for all types of gauges
class Gauge : public GUI::GraphObject
{
public:
  // Constructors
  Gauge( GUI::GraphDisplay& display, int zOrder );
  ~Gauge() {}

  // Caption Accessors
  void SetCaption( std::string caption ) { mCaption = caption; UpdateStatics(); }
  std::string Caption() { return mCaption; }
  void SetCaptionSize( float size ) { mCaptionSize = size; UpdateStatics(); }
  float CaptionSize() { return mCaptionSize; }

  // Control Signal Accessors
  void SetControlSignal1( std::string cs1 ) { mControlSignal1 = cs1; }
  std::string ControlSignal1() { return mControlSignal1; }
  void SetControlSignal1Bounds( float min, float max ) { mMin1 = min; mMax1 = max; UpdateStatics(); }
  float ControlSignal1Min() { return mMin1; }
  float ControlSignal1Max() { return mMax1; }
  void SetControlSignal2( std::string cs2 ) { mControlSignal2 = cs2; }
  std::string ControlSignal2() { return mControlSignal2; }
  void SetControlSignal2Bounds( float min, float max ) { mMin2 = min; mMax2 = max; UpdateStatics(); }
  float ControlSignal2Min() { return mMin2; }
  float ControlSignal2Max() { return mMax2; }

  // Scale Accessors
  void ShowScale( bool show ) { mShowScale = show; UpdateStatics(); }
  bool ScaleShowing() { return mShowScale; }
  void SetHScalePrecision( int precision ) { mHScalePrecision = precision; UpdateStatics(); }
  int HScalePrecision() { return mHScalePrecision; }
  void SetVScalePrecision( int precision ) { mVScalePrecision = precision; UpdateStatics(); }
  int VScalePrecision() { return mVScalePrecision; }

  // Virtual Interface
  virtual void Update( float value1, float value2 ) = 0;
  virtual void OnChange( GUI::DrawContext & );
  virtual void UpdateStatics() = 0;

protected:
  // Display Context
  GUI::GraphDisplay* mpDisplay;

  // Caption properties
  std::string mCaption;
  float mCaptionSize;

  // Positioning is in percentage of screen width/height
  float mX, mY; // Position of upper left corner
  float mWidth, mHeight;

  // Control Signals and associated values
  std::string mControlSignal1;
  float mMin1, mMax1;
  std::string mControlSignal2;
  float mMin2, mMax2;

  // Scale properties
  bool mShowScale;
  int mHScalePrecision;
  int mVScalePrecision;
};

// Helper Functions
float Normalize( float value, float min, float max );

#endif // GAUGE_H
