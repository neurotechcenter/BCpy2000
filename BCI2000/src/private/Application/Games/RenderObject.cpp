////////////////////////////////////////////////////////////////////////////////
// $Id: RenderObject.cpp 4981 2015-08-26 17:04:38Z mellinger $
// Authors: griffin.milsap@gmail.com
// Description: Defines a renderable object
//
// (C) 2000-2010, BCI2000 Project
// http://www.bci2000.org
////////////////////////////////////////////////////////////////////////////////
#include "RenderObject.h"
#include "BCIStream.h"

using namespace std;

// Name: Constructor
// Input: Position and Number of Vertices
// Output: N/A
// Description: Sets member variables and allocates memory for vertex and color arrays
RenderObject::RenderObject( unsigned int NumVerts )
{
	// Set member variables
	mNumVertices = NumVerts;
	mAlpha = 1.0f;
	mVisible = true;
	mTexture = 0;

	// Allocate space for vertices and color data
	mVertices = new float[ mNumVertices * 3 ];
	if( !mVertices ) bcidbg( 0 ) << "Could not allocate Vertex Array." << endl; 
	mUVs = new float[ mNumVertices * 2 ];
	if( !mUVs ) bcidbg( 0 ) << "Could not allocate UV Array." << endl;

	// The object has had new parameters set
	mChanged = true;
}

// Name: Deconstructor
// Input: N/A
// Output: N/A
// Description: Frees memory associated with vertex and color arrays
RenderObject::~RenderObject()
{
	// Free Vertices and Color array
	if( mVertices ) delete [] mVertices;
	mVertices = NULL;
	if( mUVs ) delete [] mUVs;
	mUVs = NULL;
}

// Name: Update
// Input: N/A
// Output: N/A
// Description: Constructs and draws the object
void RenderObject::Update()
{
	// Reconstruct of necessary
	if( mChanged )
		Construct();
	mChanged = false;

	// Draw the object
	if( mVisible )
		Draw();
}

// Name: SetTexture
// Input: Color
// Output: Void
// Description: Sets the color array and marks as dirty
void RenderObject::SetTexture( GLuint textureHandle )
{
	// Set the new texture handle
	mTexture = textureHandle;

	// We changed some parameters
	mChanged = true;
}

// Name: SetAlpha
// Input: Alpha value (Transparancy)
// Output: Void
// Description: Sets the transparancy of the object and marks the object dirty
void RenderObject::SetAlpha( float alpha )
{
	// Set alpha accordingly
	mAlpha = alpha;

	// We changed some parameters
	mChanged = true;
}
