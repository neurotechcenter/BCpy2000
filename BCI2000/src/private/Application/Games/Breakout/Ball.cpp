////////////////////////////////////////////////////////////////////////////////
// $Id: Ball.cpp 3125 2011-02-21 02:08:27Z jhill $
// Authors: griffin.milsap@gmail.com
// Description: Defines a ball for breakout
//
// (C) 2000-2010, BCI2000 Project
// http://www.bci2000.org
////////////////////////////////////////////////////////////////////////////////
#include "PCHIncludes.h"
#pragma hdrstop

#include "Ball.h"
#include "../ThemeManager.h"

float Ball::DefaultBallSize = 0.05f;

// Constructor
Ball::Ball()
{
	// Construct the object
	mX = mY = mDX = mDY = 0.0f;
	mSpeed = 0.0f;
	mSolid = true;
	mRadius = DefaultBallSize;
	mCircle.SetRadius( mRadius );

	// Add this object to the Theme Manager
	ThemeManager::Instance().AddObject( &mCircle, "Ball" );
}

void Ball::Update()
{
	mCircle.Update();
}

void Ball::MoveX()
{
	// Update the ball
	mX += mDX * mSpeed;
	mCircle.SetPosition( mX, mY );
}

void Ball::MoveY()
{
	// Update the ball
	mY += mDY * mSpeed;
	mCircle.SetPosition( mX, mY );
}
