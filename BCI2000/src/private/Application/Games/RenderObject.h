////////////////////////////////////////////////////////////////////////////////
// $Id: RenderObject.h 3069 2011-01-19 23:27:37Z jhill $
// Authors: griffin.milsap@gmail.com
// Description: Defines a renderable object
//
// (C) 2000-2010, BCI2000 Project
// http://www.bci2000.org
////////////////////////////////////////////////////////////////////////////////
#ifndef RENDER_OBJECT_H
#define RENDER_OBJECT_H

#include <qgl.h>

// The RenderObject class defines a renderable object
// We request the number of vertices in construction so that memory can be managed by
// the base class and the Java garbage collector doesn't get in our face.
// RenderObject does not use VBOs due to the volitile nature of the RenderObject.
// Perhaps a future class will be implemented which uses VBO based rendering, but
// for now, the overhead used in creating new buffers kills perfrormance.
class RenderObject
{
public:
	// Constructor/Deconstructor
	explicit RenderObject( unsigned int NumVerts );
	virtual ~RenderObject();

	// Calls Construct and Draw
	void Update();

	// Gets
	GLuint TextureHandle() { return mTexture; }
	float Alpha() { return mAlpha; }
	bool Visible() { return mVisible; }

	// Sets
	virtual void SetTexture( GLuint );
	virtual void SetAlpha( float alpha );
	virtual void Show() { mVisible = true; }
	virtual void Hide() { mVisible = false; }

protected:
	// Virtual Draw and Construct methods which MUST be implemented
	virtual void Draw() = 0;
	virtual void Construct() = 0;

	// All children can modify these variables
	unsigned int mNumVertices;
	float* mVertices;
	float* mUVs;
	bool mChanged;

private:
	// Objects managed by the RenderObject
	GLuint mTexture;
	bool mVisible;
	float mAlpha;

};

#endif // RENDER_OBJECT_H