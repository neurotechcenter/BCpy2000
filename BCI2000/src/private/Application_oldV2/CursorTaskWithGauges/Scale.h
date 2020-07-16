////////////////////////////////////////////////////////////////////////////////
// $Id: Scale.h 1782 2008-02-15 11:13:42Z milsapg $
// Author: griffin.milsap@gmail.com
// Description: A general scale class for creating numerical scales for gauges
//
// (C) 2000-2008, BCI2000 Project
// http://www.bci2000.org
////////////////////////////////////////////////////////////////////////////////
#ifndef SCALE_H
#define SCALE_H

#include <vector>
#include "TextField.h"
#include "GraphDisplay.h"

class Scale
{
public:
  Scale();
  ~Scale() {}

  // Positioning Accessors
  void SetPosition( float x, float y ) { mX = x; mY = y; Invalidate(); }
  GUI::Point Position() { GUI::Point pt; pt.x = mX; pt.y = mY; return pt; }
  void SetWidth( float width ) { mWidth = width; Invalidate(); }
  float Width() { return mWidth; }
  void SetHeight( float height) { mHeight = height; Invalidate(); }
  float Height() { return mHeight; }
  void SetDisplayRect( GUI::Rect &rect );
  GUI::Rect DisplayRect();

  // Signal Property Accessors
  void SetMin( float min ) { mMin = min; Invalidate(); }
  float Min() { return mMin; }
  void SetMax( float max ) { mMax = max; Invalidate(); }
  float Max() { return mMax; }
  void SetPrecision( int precision ) { mPrecision = precision; Invalidate(); }
  int Precision() { return mPrecision; }
  void SetVisible( bool visible ) { mVisible = visible; Invalidate(); }
  bool Visible() { return mVisible; }
  void Hide() { mVisible = false; Invalidate(); }
  void Show() { mVisible = true; Invalidate(); }

  // Virtual Interface
  virtual void Invalidate() = 0;

  // Helper Function
  float round( float r ) { return ( r > -0.001f && r < 0.001f ) ? 0 : r; }

protected:
  // Positioning
  float mX, mY; // Coords of top left corner (in percentage of screen)
  float mWidth, mHeight; // (in percentage of screen)

  // Signal Properties
  float mMin, mMax;
  int mPrecision;

  // Other
  bool mVisible;
};

class VerticalScale : public Scale
{
public:
  VerticalScale( GUI::GraphDisplay &disp );
  ~VerticalScale();

  // Member Methods
  virtual void Invalidate();

private:
  GUI::GraphDisplay* mDisplay;
  std::vector< TextField* > mpDeliminations;
};

class HorizontalScale : public Scale
{
public:
  HorizontalScale( GUI::GraphDisplay &disp );
  ~HorizontalScale();

  // Member Methods
  virtual void Invalidate();

private:
  GUI::GraphDisplay* mDisplay;
  std::vector< TextField* > mpDeliminations;
};

#endif // SCALE_H
