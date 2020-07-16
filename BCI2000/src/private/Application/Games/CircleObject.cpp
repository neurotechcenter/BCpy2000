////////////////////////////////////////////////////////////////////////////////
// $Id: CircleObject.cpp 4981 2015-08-26 17:04:38Z mellinger $
// Authors: griffin.milsap@gmail.com
// Description: Defines a renderable circle
//
// (C) 2000-2010, BCI2000 Project
// http://www.bci2000.org
////////////////////////////////////////////////////////////////////////////////
#include "CircleObject.h"
#include <math.h>

using namespace std;

CircleObject::CircleObject()
: RenderObject( CIRCLE_RESOLUTION + 2 )
{
	// Constructor
	mX = mY = 0.0f;
	mRadius = 0.0f;
}

void CircleObject::SetRadius( float Radius )
{
	// Set the radius and mark the circle as changed
	mRadius = Radius;
	mChanged = true;
}

void CircleObject::SetPosition( float x, float y )
{
	mX = x; mY = y;
	mChanged = true;
}

void CircleObject::Construct()
{
	// Set up UVs and vertices
	int vPos = 0, uvPos = 0;
	mVertices[ vPos++ ] = mX;
	mVertices[ vPos++ ] = mY;
	mVertices[ vPos++ ] = 0;

	mUVs[ uvPos++ ] = 0.5f;
	mUVs[ uvPos++ ] = 0.5f;

	double da = ( ( 2 * M_PI ) / CIRCLE_RESOLUTION );
	double a = 0;
	while( vPos < ( CIRCLE_RESOLUTION + 2 ) * 3 )
	{
		mVertices[ vPos++ ] = mX + ( cos( a ) * mRadius );
		mVertices[ vPos++ ] = mY + ( sin( a ) * mRadius );
		mVertices[ vPos++ ] = 0;

		mUVs[ uvPos++ ] = ( cos( a ) + 1.0f ) / 2.0f;
		mUVs[ uvPos++ ] = ( sin( a ) + 1.0f ) / 2.0f;

		a += da;
	}
}

void CircleObject::Draw()
{
	// Draw the circle
	glBindTexture( GL_TEXTURE_2D, TextureHandle() );
	glVertexPointer( 3, GL_FLOAT, 0, mVertices );
	glTexCoordPointer( 2, GL_FLOAT, 0, mUVs );
	glEnableClientState( GL_VERTEX_ARRAY );
	glEnableClientState( GL_TEXTURE_COORD_ARRAY );
		
	// Draw the quad and texture it
	glDrawArrays( GL_TRIANGLE_FAN, 0, CIRCLE_RESOLUTION + 2 );

	// Disable the client states
	glDisableClientState( GL_VERTEX_ARRAY );
	glDisableClientState( GL_TEXTURE_COORD_ARRAY );
}