////////////////////////////////////////////////////////////////////////////////
// $Id: Paddle.cpp 4981 2015-08-26 17:04:38Z mellinger $
// Authors: griffin.milsap@gmail.com
// Description: Defines a paddle for breakout
//
// (C) 2000-2010, BCI2000 Project
// http://www.bci2000.org
////////////////////////////////////////////////////////////////////////////////
#include "Paddle.h"
#include "../ThemeManager.h"
#include <math.h>

// Constructor
Paddle::Paddle()
{
	// Construct the object
	mX = mY = 0.0f;
	mWidth = 0.3f;
	mHeight = 0.1f;
	mSolid = true;

	mPaddle.SetExtent( mX - ( mWidth / 2.0f ), mY - ( mHeight / 2.0f ), mWidth, mHeight );

	// Add this object to the Theme Manager
	ThemeManager::Instance().AddObject( &mPaddle, "Paddle" );
}

void Paddle::Update()
{
	mPaddle.Update();
}

void Paddle::SetPosition( float x, float y )
{
	// Set the paddle position such that the origin is in the center
	mX = x; mY = y;
	mPaddle.SetPosition( x - ( mWidth / 2.0f ), y - ( mHeight / 2.0f ) );
}

bool Paddle::Collide( Ball &ball )
{
	if ( !mSolid ) return false;
	if( ball.DY() < 0.0f )
		if( Intersect( ball.X(), ball.Y(), ball.Radius(), mX, mY, mPaddle.Width(), mPaddle.Height() ) )
			return true;
	return false;
}

// Helper intersection function - DUPLICATE CODE.  BAAAAAD.
// http://stackoverflow.com/questions/401847/circle-rectangle-collision-detection-intersection
bool Paddle::Intersect( float CircleX, float CircleY, float CircleRadius, float X, float Y, float Width, float Height )
{
	float circledistancex = fabs( CircleX - X );
	float circledistancey = fabs( CircleY - Y );

	// We're obviously too far away...
	if( circledistancex > ( ( Width / 2.0f ) + CircleRadius ) ) { return false; }
	if( circledistancey > ( ( Height / 2.0f ) + CircleRadius ) ) { return false; }

	// Center is within the rectangle
	if( circledistancex <= ( Width / 2.0f ) ) { return true; }
	if( circledistancey <= ( Height / 2.0f ) ) { return true; }

	// The hard case...
	float cornerdistance_sq = pow( circledistancex - ( Width / 2.0f ), 2 ) + 
		pow( circledistancey - ( Height / 2.0f ), 2 );
	
	return ( cornerdistance_sq <= pow( CircleRadius, 2 ) );
}
