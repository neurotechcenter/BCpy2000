////////////////////////////////////////////////////////////////////////////////
// $Id: Cursor.h 2009 2008-06-17 11:51:23Z milsag $
// Authors: griffin.milsap@gmail.com
// Description: A Cursor Class
//
// (C) 2000-2008, BCI2000 Project
// http://www.bci2000.org
////////////////////////////////////////////////////////////////////////////////
#ifndef CURSOR_H
#define CURSOR_H

#include "Shapes.h"
#include "DisplayWindow.h"
#include "Color.h"

class Cursor
{
 public:
  // Constructor/Deconstuctor
  Cursor();
  Cursor( GUI::GraphDisplay &display );
  ~Cursor();

  // Methods
  void Init( GUI::GraphDisplay &display );
  void SetSpeed( float xSpeed, float ySpeed );
  void SetColor( RGBColor &color );
  void SetRadius( float radius );
  void SetVisible( bool visible );
  GUI::Point Update( float cX, float cY );
  void SetPosition( float x, float y );
  bool DistanceFrom( GUI::Point point );

  // Accessors
  float XSpeed()   { return mSpeedX; }
  float YSpeed()   { return mSpeedY; }
  RGBColor Color() { return mColor; }
  float Radius()   { return mRadius; }
  bool Visible()   { return mVisible; }
  float X()        { return mX; }
  float Y()        { return mY; }

 private:
  // Private Member Variables
  EllipticShape* mpCursor;
  float mSpeedX, mSpeedY, mRadius;
  float mX, mY;
  bool mVisible;
  RGBColor mColor;
  float mScalingX, mScalingY;
};
#endif // CURSOR_H



