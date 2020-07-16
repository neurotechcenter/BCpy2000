////////////////////////////////////////////////////////////////////////////////
// $Id: Cursor.cpp 2009 2008-06-17 11:51:23Z milsag $
// Authors: griffin.milsap@gmail.com
// Description: A Cursor Class
//
// (C) 2000-2008, BCI2000 Project
// http://www.bci2000.org
////////////////////////////////////////////////////////////////////////////////
#include "PCHIncludes.h"
#pragma hdrstop

#include <cmath>
#include "Cursor.h"

// **************************************************************************
// Function:   Cursor()
// Purpose:    This is the default constructor for the Cursor class
// Parameters: N/A
// Returns:    N/A
// **************************************************************************
Cursor::Cursor()
{
  mpCursor = NULL;
  mSpeedX = mSpeedY = 0.0f;
  mVisible = false;
  mRadius = 0.0f;
  mColor = ( 0, 0, 0 );
}

// **************************************************************************
// Function:   Cursor( display )
// Purpose:    This constructs and initializes the class.
// Parameters: GUI::GraphDisplay display (Display context)
// Returns:    N/A
// **************************************************************************
Cursor::Cursor( GUI::GraphDisplay &display )
{
  mpCursor = NULL;
  mSpeedX = mSpeedY = 0.0f;
  mVisible = false;
  mRadius = 0.0f;
  mColor = ( 0, 0, 0 );
  mScalingX = mScalingY = 0.0f;
  Init( display );
}

// **************************************************************************
// Function:   ~Cursor()
// Purpose:    This Deconstructs the Cursor Object, deletes the mpCursor obj.
// Parameters: N/A
// Returns:    N/A
// **************************************************************************
Cursor::~Cursor()
{
  if( mpCursor )
  {
    mpCursor->Hide();
    delete mpCursor;
    mpCursor = NULL;
  }
}

// **************************************************************************
// Function:   Init
// Purpose:    If the default constructor is called, this initialization
//             method must be called before any other methods
// Parameters: GUI::GraphDisplay (Display context)
// Returns:    N/A
// **************************************************************************
void Cursor::Init( GUI::GraphDisplay &display )
{
  // Create the Cursor Object
  if( !mpCursor )
    mpCursor = new EllipticShape( display, 1 );

  float width = display.Context().rect.right - display.Context().rect.left;
  float height = display.Context().rect.bottom - display.Context().rect.top;

  if( width > height )
  {
    mScalingX = height / width;
    mScalingY = -1.0;
  }
  else
  {
    mScalingX = 1.0;
    mScalingY = -width / height;
  }
  mpCursor->Hide();
}

// **************************************************************************
// Function:   SetSpeed
// Purpose:    This sets the speed multiplier on the cursor
// Parameters: Speed in X axis, speed in Y axis
// Returns:    N/A
// **************************************************************************
void Cursor::SetSpeed( float xSpeed, float ySpeed )
{
  // Set speeds accordingly
  mSpeedX = xSpeed;
  mSpeedY = ySpeed;
}

// **************************************************************************
// Function:   SetColor
// Purpose:    This sets the color of the cursor
// Parameters: RGBColor (r, g, b color for the cursor)(Default: 0,0,0)
// Returns:    N/A
// **************************************************************************
void Cursor::SetColor( RGBColor &color )
{
  // Set the color of the cursor
  mColor = color;
  mpCursor->SetColor( mColor );
  mpCursor->SetFillColor( mColor );
}

// **************************************************************************
// Function:   SetRadius
// Purpose:    This method sets the radius of the cursor in normalized coords.
// Parameters: float (radius in percent of screen)
// Returns:    N/A
// **************************************************************************
void Cursor::SetRadius( float radius )
{
  // Set the radius accordingly
  mRadius = radius;
  float cursorWidth = radius * 2;
  GUI::Rect cursorRect = { 0, 0, cursorWidth * mScalingX, cursorWidth * mScalingY };
  mpCursor->SetAspectRatioMode( GUI::AspectRatioModes::AdjustWidth );
  mpCursor->SetDisplayRect( cursorRect );
}

// **************************************************************************
// Function:   SetVisible
// Purpose:    Sets whether or not the cursor can be seen
// Parameters: bool (visibility)
// Returns:    N/A
// **************************************************************************
void Cursor::SetVisible( bool visible )
{
  // Set Visibility
  if( visible == true )
  {
    mpCursor->Show();
    mVisible = true;
  }
  else
  {
    mpCursor->Hide();
    mVisible = false;
  }
}

// **************************************************************************
// Function:   Update
// Purpose:    Main part of this class, updates the cursor using a speed
//             modifier and does bounds checking
// Parameters: Control Signal X, Control Signal Y (INTEGRATED OVER TIME)
// Returns:    N/A
// **************************************************************************
GUI::Point Cursor::Update( float cX, float cY )
{
  // Update Coordinates
  mX += mSpeedX * cX;
  mY += mSpeedY * cY;

  // Perform Bounds Checking
  if( mX <= 0.0f )
    mX = 0.0f;
  else if( mX >= 1.0f )
    mX = 1.0f;

  if( mY <= 0.0f )
    mY = 0.0f;
  else if( mY >= 1.0f )
    mY = 1.0f;

  // Set Cursor Position
  GUI::Point center;
  center.x = mX;
  center.y = mY;
  mpCursor->SetCenter( center );
  return center;
}

// **************************************************************************
// Function:   SetPosition
// Purpose:    Sets the current position of the cursor.  (0,0) = Top left
//                                                       (1,1) = Bottom Right
// Parameters: float x coordinate, float y coordinate
// Returns:    N/A
// **************************************************************************
void Cursor::SetPosition( float x, float y )
{
  // Set the Cursor Position
  mX = x;
  mY = y;
  GUI::Point center;
  center.x = mX;
  center.y = mY;
  mpCursor->SetCenter( center );
}

// **************************************************************************
// Function:   DistanceFrom
// Purpose:    Finds the distance from the center of the cursor to the point
// Parameters: GUI::Point
// Returns:    float distance
// **************************************************************************
bool Cursor::DistanceFrom( GUI::Point point )
{
  float xDiff = point.x - mX;
  float yDiff = point.y - mY;
  return std::sqrt( ( xDiff * xDiff ) + ( yDiff * yDiff ) );
}
