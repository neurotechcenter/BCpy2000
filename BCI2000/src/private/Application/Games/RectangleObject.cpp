////////////////////////////////////////////////////////////////////////////////
// $Id: RectangleObject.cpp 3069 2011-01-19 23:27:37Z jhill $
// Authors: griffin.milsap@gmail.com
// Description: Defines a renderable rectangle
//
// (C) 2000-2010, BCI2000 Project
// http://www.bci2000.org
////////////////////////////////////////////////////////////////////////////////
#include "PCHIncludes.h"
#pragma hdrstop

#include "RectangleObject.h"

RectangleObject::RectangleObject()
: RenderObject( 4 ) // Rectangles have four vertices
{
	// Constructor
	mX = 0.0f;
	mY = 0.0f;
	mZ = 0.0f;

	mWidth = 0.0f;
	mHeight = 0.0f;
}

// Set the bounds of the rectangle
void RectangleObject::SetExtent( float Left, float Top, float Width, float Height )
{
	// Set the new bounds and mark as changed
	mX = Left;
	mY = Top;
	mWidth = Width;
	mHeight = Height;
	mChanged = true;
}

void RectangleObject::Construct()
{
	// Set up UVs and vertices
	const float vertices[4][3] = {
		{ mX,          mY,           mZ },
		{ mX + mWidth, mY,           mZ },
		{ mX + mWidth, mY + mHeight, mZ },
		{ mX,          mY + mHeight, mZ }
	};

	int tIdx = 0;
	int vIdx = 0;
    for( int i = 0; i < 4; i++ ) 
	{
		mUVs[ tIdx++ ] = ( i == 1 || i == 2 );
		mUVs[ tIdx++ ] = ( i == 3 || i == 2 );

		for( int j = 0; j < 3; j++ )
			mVertices[ vIdx++ ] = vertices[i][j];
	}
}

void RectangleObject::Draw()
{
	// Actually draw the rectangle
	glBindTexture( GL_TEXTURE_2D, TextureHandle() );
	glEnableClientState( GL_VERTEX_ARRAY );
	glEnableClientState( GL_TEXTURE_COORD_ARRAY );
	glVertexPointer( 3, GL_FLOAT, 0, mVertices );
	glTexCoordPointer( 2, GL_FLOAT, 0, mUVs );
		
	// Draw the quad and texture it
	glDrawArrays( GL_TRIANGLE_FAN, 0, 4 );

	// Disable the client states
	glDisableClientState( GL_TEXTURE_COORD_ARRAY );
	glDisableClientState( GL_VERTEX_ARRAY );
}
