////////////////////////////////////////////////////////////////////////////////
// $Id: RectangleObject.h 3069 2011-01-19 23:27:37Z jhill $
// Authors: griffin.milsap@gmail.com
// Description: Defines a renderable rectangle
//
// (C) 2000-2010, BCI2000 Project
// http://www.bci2000.org
////////////////////////////////////////////////////////////////////////////////
#ifndef RECTANGLE_OBJECT_H
#define RECTANGLE_OBJECT_H

#include "RenderObject.h"

class RectangleObject : public RenderObject
{
public:
	explicit RectangleObject();

	// Sets
	void SetPosition( float Left, float Top ) { SetExtent( Left, Top, mWidth, mHeight ); }
	void SetWidth( float Width ) { SetExtent( mX, mY, Width, mHeight ); }
	void SetHeight( float Height ) { SetExtent( mX, mY, mWidth, Height ); }
	void SetSize( float Width, float Height ) { SetExtent( mX, mY, Width, Height ); }
	void SetExtent( float Left, float Top, float Width, float Height );
	void SetZ( float Z ) { mZ = Z; }

	// Gets
	void Position( float &X, float &Y ) { X = mX; Y = mY; }
	float Width() { return mWidth; }
	float Height() { return mHeight; }
	float X() { return mX; }
	float Y() { return mY; }
	float Z() { return mZ; }

protected:	
	// Virtual Interface
	virtual void Construct();
	virtual void Draw();

private:
	GLfloat mX, mY, mZ, mWidth, mHeight; // Default to 0	
};

#endif // RECTANGLE_OBJECT_H