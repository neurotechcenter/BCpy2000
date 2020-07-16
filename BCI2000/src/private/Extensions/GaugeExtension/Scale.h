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

#include <QFont>
#include <QString>
#include <string>
#include "GraphObject.h"

class QPainter;

class Scale : public GUI::GraphObject
{
public:
  Scale( GUI::GraphDisplay& inDisplay, float zOrder = TextStimulusZOrder );
  ~Scale();

  // Signal Property Accessors
  void SetMin( float min ) { mMin = min; Change(); }
  float GetMin() { return mMin; }
  void SetMax( float max ) { mMax = max; Change(); }
  float GetMax() { return mMax; }
  void SetPrecision( int precision ) { mPrecision = precision; Change(); }
  int GetPrecision() { return mPrecision; }
  void SetNumDeliminations( int numDelims ) { mNumDelims = numDelims; Change(); }
  int GetNumDeliminations() { return mNumDelims; }
  void SetFormat( std::string fmt ) { mFormat = fmt; }

  virtual void OnPaint( const GUI::DrawContext& ) = 0;

protected:
  // Signal Properties
  float mMin, mMax;
  int mPrecision;
  int mNumDelims;
  std::string mFormat;
  QFont mFont;

  // Helper function
  QString CalculateFontSize( int width, int height, QPainter* painter );
};

class VerticalScale : public Scale
{
public:
  VerticalScale( GUI::GraphDisplay& inDisplay, float zOrder = TextStimulusZOrder ) : 
    Scale( inDisplay, zOrder ) { }

  // Member Methods
  virtual void OnPaint( const GUI::DrawContext& inDC );
};

class HorizontalScale : public Scale
{
public:
  HorizontalScale( GUI::GraphDisplay& inDisplay, float zOrder = TextStimulusZOrder ) :
    Scale( inDisplay, zOrder ) { }

  // Member Methods
  virtual void OnPaint( const GUI::DrawContext& inDC );
};

#endif // SCALE_H
