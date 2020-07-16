/********************************************************************
 FILE: $Id: vhtConvexHull.h,v 1.9 2002/08/28 23:16:10 jsilver Exp $
 AUTHOR: C Ullrich
 DATE: 1998

 DESCRIPTION:
  -
 HISTORY:
  -

           -- COPYRIGHT VIRTUAL TECHNOLOGIES, INC. 1998 --
********************************************************************/
#ifndef VHT_CONVEX_HULL_H
#define VHT_CONVEX_HULL_H


#if defined(BUILD_WIN32_VH_DLL)

#if defined __BUILD_VH_CORE_DLL__
#define VH_CORE_DLL_EXPORT __declspec(dllexport)
#else
#define VH_CORE_DLL_EXPORT __declspec(dllimport)
#endif

#else 

#define VH_CORE_DLL_EXPORT

#endif

class vhtArray;

//: A three dimensional convex hull geometry.
// Convex hulls are bounded three dimensional polytopes.  Hulls
// are used to improve collision detection performance.  
// Currently, all vhtShape3D nodes in a haptic scene graph
// must be convex hulls.
// This class is a virtual base class.  Inheritance must
// be done for most collision checkers.
//!vhtmodule: Core
class VH_CORE_DLL_EXPORT vhtConvexHull 
{
	public:
		vhtConvexHull();
		//: Build a default hull.
		// Empty geometry.
		~vhtConvexHull();
		//: Destructor.

		virtual void setVertices( vhtArray &vertices );
		//: Set the vertex array.
		// The vertex array is a set of points in 3-D from
		// which the hull will be calculated.  Note that
		// points must not be co-planar.virtual const int  check(void );
		//: Check the euler condition.
		// For correctly formatted geometry, this should return 0.

		virtual int check( void );
		//: Check the Euler characteristic.
		// Calculate numFaces - numEdges + numVertices - 2.  For
		// well defined, closed geometries, this should be 0.
};
#endif


