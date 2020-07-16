#ifndef _VHT_FACE_SET_OPTIMAL_H
#define _VHT_FACE_SET_OPTIMAL_H
/********************************************************************
 FILE: $Id: vhtFaceSetOptimal.h,v 1.6 2002/08/28 23:16:14 jsilver Exp $
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

#include <vhtNodeOptimizers.h>

class vhtVrSFNode;
class vhtVrIndexedFaceSet;
class vhtVrSFNode;

//: Optimized drawing for VTi's IndexedFaceSet minimalist implementation.
//!vhtmodule: Core
class VH_CORE_DLL_EXPORT vhtFaceSetOptimal : public vhtNodeOptimizers {
	protected:		/* Instance variables. */
		unsigned int nbrFaces;
		//: Number of faces in the polyhedron.
		unsigned int *faceSize;
		//: Array of point counts per face.
		bool regularFaces;
		//: Flag indicating if all faces are identical in point count.
		unsigned int **facesPtr;
		//: Array of face's point indices.
		double (*normals)[3];
		//: Array of normals for the faces.

	public: /* Instance methods. */

		/* Constructor. */
		vhtFaceSetOptimal(vhtVrIndexedFaceSet *aFaceSet);
		//: Constructs an instance based on a given IndexedFaceSet.
		virtual ~vhtFaceSetOptimal(void);
		//: Destroys the instance.
		virtual void draw(vhtVrSFNode *aNode);
		//: Renders the IndexedFaceSet at optimal speed.
};


#endif		/* _VHT_FACE_SET_OPTIMAL_ */



