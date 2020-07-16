////////////////////////////////////////////////////////////////////////////////
// $Id: Gauge.cpp 1782 2008-02-15 11:13:42Z milsapg $
// Author: griffin.milsap@gmail.com
// Description: Handles rendering and maintenance of a gauge
//
// (C) 2000-2008, BCI2000 Project
// http://www.bci2000.org
////////////////////////////////////////////////////////////////////////////////
#include "PCHIncludes.h"
#pragma hdrstop

#include "Gauge.h"

// Gauge Constructor
Gauge::Gauge( GUI::GraphDisplay& display, int zOrder )
: GUI::GraphObject( display, zOrder )
{
  // Keep a non-const display reference
  // GUI::GraphObject gives us a const accessor, but we need non-const
  // in order to create child graphobjects...  Consider revising?
  mpDisplay = &display;
  mCaptionSize = 0.5f;
  mX = mY = 0.0f;
  mWidth = mHeight = 0.0f;
  mMin1 = 0.0f;
  mMax1 = 1.0f;
  mMin2 = 0.0f;
  mMax2 = 1.0f;
  mShowScale = true;
  mHScalePrecision = 0;
  mVScalePrecision = 0;
}

void
Gauge::OnChange( GUI::DrawContext& )
{
  mX = DisplayRect().left;
  mY = DisplayRect().top;
  mWidth = DisplayRect().right - DisplayRect().left;
  mHeight = DisplayRect().bottom - DisplayRect().top;
}

// Signal normalization helper function
// Returns a value from 0 to 1 for value between min and max
float
Normalize( float value, float min, float max )
{
  float range = ( max - min );
  float retval = ( value - min ) / range;
  if( retval <= 0.0f )
    return 0.0f;
  if( retval >= 1.0f )
    return 1.0f;
  else
    return retval;
}

