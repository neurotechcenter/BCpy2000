////////////////////////////////////////////////////////////////////////////////
// $Id: Paddle.h 3127 2011-02-23 02:13:27Z jhill $
// Authors: griffin.milsap@gmail.com
// Description: Defines a paddle which can hit the ball
//
// (C) 2000-2010, BCI2000 Project
// http://www.bci2000.org
////////////////////////////////////////////////////////////////////////////////
#ifndef PADDLE_H
#define PADDLE_H

#include "../RectangleObject.h"
#include "Ball.h"
#include <qgl.h>

class Paddle
{
public:
	Paddle();

	void SetPosition( float x, float y );
	void SetWidth( float width ) { mWidth = width; mPaddle.SetWidth( width ); }
	void SetHeight( float height ) { mHeight = height; mPaddle.SetHeight( height ); }
	void SetSolid( bool value ) { mSolid = value; }

	// Gets
	void Position( float &x, float &y ) { x = mX; y = mY; }
	float Width() { return mWidth; }
	float Height() { return mHeight; }
	float X() { return mX; }
	float Y() { return mY; }
	bool Solid() { return mSolid; }

	void Update();

	// Collision
	bool Collide( Ball &ball );
	bool Intersect( float CircleX, float CircleY, float CircleRadius, float X, float Y, float Width, float Height );

private:
	RectangleObject mPaddle;
	GLfloat mX, mY;
	float mWidth, mHeight;
	bool mSolid;
};

#endif // PADDLE_H