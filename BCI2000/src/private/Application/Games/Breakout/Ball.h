////////////////////////////////////////////////////////////////////////////////
// $Id: Ball.h 3125 2011-02-21 02:08:27Z jhill $
// Authors: griffin.milsap@gmail.com
// Description: Defines a ball for breakout
//
// (C) 2000-2010, BCI2000 Project
// http://www.bci2000.org
////////////////////////////////////////////////////////////////////////////////
#ifndef BALL_H
#define BALL_H

#include "../CircleObject.h"
#include <qgl.h>

class Ball
{
public:
	Ball();

	void SetPosition( float x, float y ) { mX = x; mY = y; }
	void SetDirection( float dX, float dY ) { mDX = dX; mDY = dY; }
	void SetSpeed( float speed ){ mSpeed = speed; }
	void SetRadius( float radius ) { mRadius = radius; mCircle.SetRadius( radius ); }
	
	// Gets
	void Position( float &x, float &y ) { x = mX; y = mY; }
	void Direction( float &dX, float &dY ) { dX = mDX; dY = mDY; }
	float X() { return mX; }
	float Y() { return mY; }
	float DX() { return mDX; }
	float DY() { return mDY; }
	float Radius() { return mRadius; }
	float Speed() { return mSpeed; }
	bool Solid() {return mSolid; }

	void MoveX();
	void MoveY();
	void Update();

	void SetSolid(bool setting) {mSolid = setting;}
	
	static float DefaultBallSize;

private:
	CircleObject mCircle;
	GLfloat mX, mY, mRadius;
	float mDX, mDY; // Unit vector direction
	float mSpeed; // Scalar magnitude
	bool mSolid;
};

#endif // BALL_H