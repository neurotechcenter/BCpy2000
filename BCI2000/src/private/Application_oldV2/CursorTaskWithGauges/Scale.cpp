////////////////////////////////////////////////////////////////////////////////
// $Id: Scale.cpp 1782 2008-02-15 11:13:42Z milsapg $
// Author: griffin.milsap@gmail.com
// Description: A general scale class for creating numerical scales for gauges
//
// (C) 2000-2008, BCI2000 Project
// http://www.bci2000.org
////////////////////////////////////////////////////////////////////////////////
#include "PCHIncludes.h"
#pragma hdrstop

#include <sstream>
#include <iomanip>
#include <iostream>
#include "Scale.h"

// Scale Constructor
Scale::Scale()
{
  // Construct
  mX = mY = 0.0f;
  mWidth = mHeight = 0.3f;
  float mWidth, mHeight;
  mMin = 0.0f;
  mMax = 1.0f;
  mPrecision = 0;
  mVisible = true;
}

void
Scale::SetDisplayRect( GUI::Rect &rect )
{
  // Translate a GUI::Rect into X, Y, Width, and Height
  mX = rect.left;
  mY = rect.top;
  mWidth = rect.right - rect.left;
  mHeight = rect.bottom - rect.top;
  Invalidate();
}

GUI::Rect
Scale::DisplayRect()
{
  // Construct a GUI::Rect and return it
  GUI::Rect rect;
  rect.left = mX;
  rect.top = mY;
  rect.right = mX + mWidth;
  rect.bottom = mY + mHeight;
  return rect;
}

// Vertical Scale
VerticalScale::VerticalScale( GUI::GraphDisplay &disp )
{
  // Construct
  mDisplay = &disp;
  mWidth = 0.1f;
  mHeight = 0.3f;

  float YInterval = mHeight / ( mPrecision + 1 );
  float range = mMax - mMin;
  float numInterval = range / ( mPrecision + 1 );

  for( int i = 0; i < 2 + mPrecision; i++ )
  {
    TextField* pField = new TextField( disp );
    GUI::Rect fieldRect;
    fieldRect.top = mY + ( ( YInterval * i ) - ( 0.04 * mHeight ) );
    fieldRect.left = mX;
    fieldRect.bottom = mY + ( (YInterval * i ) + ( 0.04 * mHeight ) );
    fieldRect.right = mX + mWidth;
    pField->SetAspectRatioMode( GUI::AspectRatioModes::AdjustNone );
    pField->SetDisplayRect( fieldRect );
    std::stringstream ss;
    ss << std::setprecision( 4 );
    ss << round( mMax - ( numInterval * i ) );
    pField->SetTextColor( RGBColor::Black );
    pField->SetText( ss.str() );
    pField->Show();
    mpDeliminations.push_back( pField );
  }
}

VerticalScale::~VerticalScale()
{
  // Deconstruct
  for( unsigned int i = 0; i < mpDeliminations.size(); i++ )
  {
    if( mpDeliminations[i] )
    {
      mpDeliminations[i]->Hide();
      delete mpDeliminations[i];
    }
  }
  mpDeliminations.clear();
}

void
VerticalScale::Invalidate()
{
  // Delete the old textfields first.
  for( unsigned int i = 0; i < mpDeliminations.size(); i++ )
  {
    if( mpDeliminations[i] )
    {
      mpDeliminations[i]->Hide();
      delete mpDeliminations[i];
    }
  }
  mpDeliminations.clear();

  if( mVisible )
  {
    float YInterval = mHeight / ( mPrecision + 1 );
    float range = mMax - mMin;
    float numInterval = range / ( mPrecision + 1 );

    for( int i = 0; i < 2 + mPrecision; i++ )
    {
      TextField* pField = new TextField( *mDisplay );
      GUI::Rect fieldRect;
      fieldRect.top = mY + ( ( YInterval * i ) - ( 0.04 * mHeight ) );
      fieldRect.left = mX;
      fieldRect.bottom = mY + ( ( YInterval * i ) + ( 0.04 * mHeight ) );
      fieldRect.right = mX + mWidth;
      pField->SetAspectRatioMode( GUI::AspectRatioModes::AdjustNone );
      pField->SetDisplayRect( fieldRect );
      std::ostringstream ss;
      ss << std::setprecision( 4 );
      ss << round( mMax - ( numInterval * i ) ) << std::endl;
      pField->SetTextColor( RGBColor::Black );
      pField->SetText( ss.str() );
      pField->Show();
      mpDeliminations.push_back( pField );
    }
  }
}


// Vertical Scale
HorizontalScale::HorizontalScale( GUI::GraphDisplay &disp )
{
  // Construct
  mDisplay = &disp;
  mWidth = 0.3f;
  mHeight = 0.1f;

  float XInterval = mWidth / ( mPrecision + 1 );
  float range = mMax - mMin;
  float numInterval = range / ( mPrecision + 1 );

  for( int i = 0; i < 2 + mPrecision; i++ )
  {
    TextField* pField = new TextField( disp );
    GUI::Rect fieldRect;
    fieldRect.top = mY;
    fieldRect.left = mX + ( ( XInterval * i ) - ( 0.08 * mWidth ) ) ;
    fieldRect.bottom = mY + mHeight;
    fieldRect.right = mX + ( ( XInterval * i ) + ( 0.08 * mWidth ) );
    pField->SetAspectRatioMode( GUI::AspectRatioModes::AdjustNone );
    pField->SetDisplayRect( fieldRect );
    std::stringstream ss;
    ss << std::setprecision( 4 );
    ss << round( mMin + numInterval * i ); //ss << mMax - ( numInterval * i );
    pField->SetTextColor( RGBColor::Black );
    pField->SetText( ss.str() );
    pField->Show();
    mpDeliminations.push_back( pField );
  }
}

HorizontalScale::~HorizontalScale()
{
  // Deconstruct
  for( unsigned int i = 0; i < mpDeliminations.size(); i++ )
  {
    if( mpDeliminations[i] )
    {
      mpDeliminations[i]->Hide();
      delete mpDeliminations[i];
    }
  }
  mpDeliminations.clear();
}

void
HorizontalScale::Invalidate()
{
  // Delete the old textfields first.
  for( unsigned int i = 0; i < mpDeliminations.size(); i++ )
  {
    if( mpDeliminations[i] )
    {
      mpDeliminations[i]->Hide();
      delete mpDeliminations[i];
    }
  }
  mpDeliminations.clear();

  if( mVisible )
  {
    float XInterval = mWidth / ( mPrecision + 1 );
    float range = mMax - mMin;
    float numInterval = range / ( mPrecision + 1 );

    for( int i = 0; i < 2 + mPrecision; i++ )
    {
      TextField* pField = new TextField( *mDisplay );
      GUI::Rect fieldRect;
      fieldRect.top = mY;
      fieldRect.left = mX + ( ( XInterval * i ) - ( 0.08 * mWidth ) ) ;
      fieldRect.bottom = mY + mHeight;
      fieldRect.right = mX + ( ( XInterval * i ) + ( 0.08 * mWidth ) );
      pField->SetAspectRatioMode( GUI::AspectRatioModes::AdjustNone );
      pField->SetDisplayRect( fieldRect );
      std::stringstream ss;
      ss << std::setprecision( 4 );
      ss << round( mMin + numInterval * i ); //ss << mMax - ( numInterval * i );
      pField->SetTextColor( RGBColor::Black );
      pField->SetText( ss.str() );
      pField->Show();
      mpDeliminations.push_back( pField );
    }
  }
}

