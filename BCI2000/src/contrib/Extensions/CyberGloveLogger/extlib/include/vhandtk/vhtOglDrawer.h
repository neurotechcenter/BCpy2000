#ifndef _VHT_OGL_DRAWER_H_
#define _VHT_OGL_DRAWER_H_
/********************************************************************
 FILE: $Id: vhtOglDrawer.h,v 1.9 2002/08/28 23:16:21 jsilver Exp $
 AUTHOR: Hugo DesRosiers.
 DATE: 1999/03/15.

 DESCRIPTION: Drawing context for our minimalist VRML, using OpenGL
	as the rendering engine.

 HISTORY:
 
           -- COPYRIGHT VIRTUAL TECHNOLOGIES, INC. 1998 --
********************************************************************/

#if defined(BUILD_WIN32_VH_DLL)

#if defined __BUILD_VH_CORE_DLL__
#define VH_CORE_DLL_EXPORT __declspec(dllexport)
#else
#define VH_CORE_DLL_EXPORT __declspec(dllimport)
#endif

#else 

#define VH_CORE_DLL_EXPORT

#endif

#include <vhtDrawContext.h>
#include <vhtTransform3D.h>

class vhtTransform3D;
class vhtVrShape;
class vhtVrIndexedFaceSet;
class vhtVrSFNode;
class vhtHumanHand;

//: A stack of transforms.
// An infinite depth matrix transform stack.
// For internal use only.
//!vhtmodule: Core
class VH_CORE_DLL_EXPORT vhtMatrixStack {
	public:
		VH_CORE_DLL_EXPORT enum Constants {
			heapSize= 32
		};

	protected:
		vhtMatrixStack *next, *prev;   //: Next and previous stacks.
		unsigned int nbrMatrices;      //: Number of matricies in this stack.
		vhtTransform3D *matrices;      //: Matrix array.

	public:
		vhtMatrixStack(void);
		//: Construct a matrix stack.
		// Allocates appropriate matrix stack data.
		vhtMatrixStack(vhtMatrixStack *aParent);
		//: Construct another matrix stack.
		// Builds a matrix stack and adds it to the list.
		~vhtMatrixStack(void);
		//: Delete this stack.
		void push(vhtTransform3D *aTransfo);
		//: Push a matrix onto this stack.
		void pop(void);
		//: Pop the top of this matrix stack.
		inline vhtTransform3D current(void) { return (nbrMatrices > 0) ? matrices[nbrMatrices-1] : matrices[0]; }
		//: Return the current top of this stack.
		inline bool empty(void) { return (nbrMatrices == 0); }
		//: Check if this stack is empty.
};



//: An OpenGL VRML rendering context.
// OpenGL implementation of a VRML rendering context.  Can draw Vr scene graphs using current
// OpenGL context.
//!vhtmodule: Core
class vhtOglDrawer : public vhtVrContext {
	protected:
		vhtMatrixStack *xformStack;
		//: Pre-allocated matrix viewing stack.
		
	public:
		VH_CORE_DLL_EXPORT vhtOglDrawer(void);
		//: Construct an OpenGL based rendering context.
		VH_CORE_DLL_EXPORT virtual void render(vhtTransform3D *initialMatrix, vhtVrSFNode *aNode);
		//: Render a Vr scene graph with provided initial transform.
		//!param: initialMatrix - Pre-multiplier (for camera)
		//!param: aNode - Root of Vr graph to render.
		// Renders the indicated Vr scene graph to the current OpenGL context, and
		// pre-multiplies all subsequent transformations by initialMatrix.
		VH_CORE_DLL_EXPORT virtual void renderHand( vhtTransform3D *cameraMatrix, vhtHumanHand *hand );
		//: Render the indicated human hand using the viewing transform.
		//!param: cameraMatrix - Camera pre-multiplication matrix.
		//!param: hand - Hand to render.
		// Renders the indicated vhtHumanHand and applies the cameraMatrix viewing transform
		// to the hand.  Requires a properly setup OpenGL context.
		VH_CORE_DLL_EXPORT virtual void setLocalTransform(vhtVrTransform *aTransform);
		//: Push a matrix onto the stack.
		//!param: aTransform - Transform to push.
		VH_CORE_DLL_EXPORT virtual void unsetLocalTransform(vhtVrTransform *aTransform);
		//: Pop a matrix from the stack.
		//!param: aTransform - Depreciated.
		VH_CORE_DLL_EXPORT void drawShape(vhtVrShape *aNode);
		//: Draw a particular VrShape with OpenGL.
		//!param: aNode - Node to draw.
		// Renders the indicated vhtVrShape into the current OpenGL context.
};


#endif		/* _VHT_OGL_DRAWER_H_ */



