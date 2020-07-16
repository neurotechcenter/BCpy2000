////////////////////////////////////////////////////////////////////////////////
// $Id: Brick.cpp 3125 2011-02-21 02:08:27Z jhill $
// Authors: jezhill@gmail.com
// Description: Defines a brick object for breakout
//
// (C) 2000-2010, BCI2000 Project
// http://www.bci2000.org
////////////////////////////////////////////////////////////////////////////////
#include "Brick.h"
#include "../ThemeManager.h"

#include <cmath>
#include <cstdio>

using namespace std;

// Helper intersection function
// http://stackoverflow.com/questions/401847/circle-rectangle-collision-detection-intersection
bool Intersect( float CircleX, float CircleY, float CircleRadius, float X, float Y, float Width, float Height );
bool Intersect( float CircleX, float CircleY, float CircleRadius, float X, float Y, float Width, float Height )
{
	// TODO: OPTIMIZE THIS SPECIALPANTS
	float centerX = X + ( Width / 2.0f );
	float centerY = Y + ( Height / 2.0f );

	float circledistancex = fabs( CircleX - centerX );
	float circledistancey = fabs( CircleY - centerY );

	// We're obviously too far away...
	if( circledistancex > ( ( Width / 2.0f ) + CircleRadius ) ) { return false; }
	if( circledistancey > ( ( Height / 2.0f ) + CircleRadius ) ) { return false; }

	// Center is within the rectangle
	if( circledistancex <= ( Width / 2.0f ) ) { return true; }
	if( circledistancey <= ( Height / 2.0f ) ) { return true; }

	// The hard case...
	float dx = circledistancex - ( Width / 2.0f );
	float dy = circledistancey - ( Height / 2.0f );

	return ( dx*dx + dy*dy <= CircleRadius*CircleRadius );
}


Brick::Brick()
{
	mHitPoints = mSaved = 0.0;
	mColliding = false;
}

Brick::~Brick()
{
	while( mRects.size() )
	{
		delete mRects.back();
		mRects.pop_back();
	}
}

void Brick::Update()
{
	if( mHitPoints <= 0.0 || mRects.size() == 0 ) return;

	int index = (int)ceil(mHitPoints) - 1;
	RectangleObject *r =  ( index < (int)mRects.size() ) ? mRects[index] : mRects.back();
	if( r->X() != X() || r->Y() != Y() ) r->SetPosition( X(), Y() );
	if( r->Width() != Width() ) r->SetWidth( Width() );
	if( r->Height() != Height() ) r->SetHeight( Height() );
	r->Update();
}

void Brick::Init(double hitPoints)
{
	char default_appearance[16] = "Block";
	char appearance[32];
	mHitPoints = hitPoints;
	int nRects = (int)ceil(mHitPoints);
	while((int)mRects.size() < nRects)
	{
		RectangleObject *r = new RectangleObject();
		mRects.push_back(r);
		sprintf(appearance, "%s_%g", default_appearance, hitPoints - nRects + mRects.size() );
		ThemeManager::Instance().AddObject(r, ThemeManager::Instance().Contains(appearance) ? appearance : default_appearance );
	}
}

int Brick::Collide(Ball &ball)
{
	if( mHitPoints <= 0.0 || mColliding )
	{
		mColliding = false;
	}
	else
	{
		mColliding = Intersect( ball.X(), ball.Y(), ball.Radius(), X(), Y(), Width(), Height() );
	}
	if(mColliding) mHitPoints--;

	return mColliding;
}
