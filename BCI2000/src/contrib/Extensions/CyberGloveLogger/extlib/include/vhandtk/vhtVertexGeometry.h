/********************************************************************
 FILE: $Id: vhtVertexGeometry.h,v 1.10 2002/08/28 23:16:28 jsilver Exp $
 AUTHOR: C Ullrich
 DATE: Sep 1998

 DESCRIPTION:
  This class contains a vertex based geometry.  Vertices should be added using the
  setVertices methods.  Once added, this class automatically computes the convex
  hull of the geometry, which is available from getConvexHull.

 HISTORY:
  -
           -- COPYRIGHT VIRTUAL TECHNOLOGIES, INC. 1998 --
********************************************************************/

#ifndef __VHT_VERTEX_GEOMETRY_H
#define __VHT_VERTEX_GEOMETRY_H

#if defined(BUILD_WIN32_VH_DLL)

#if defined __BUILD_VH_CORE_DLL__
#define VH_CORE_DLL_EXPORT __declspec(dllexport)
#else
#define VH_CORE_DLL_EXPORT __declspec(dllimport)
#endif

#else 

#define VH_CORE_DLL_EXPORT

#endif

#include <vhtGeometry.h>
#include <vhtArray.h>

class vhtVector3d;

//: A vertex based geometry.
// Vertices should be added using the setVertices methods.  This class can be used as a
// template by the collision geometry factory to generate the appropriate collision geometry.
//!vhtmodule: Core
class VH_CORE_DLL_EXPORT vhtVertexGeometry : public vhtGeometry 
{
	public:
		vhtVertexGeometry(void);
		//: Constructor.

		virtual ~vhtVertexGeometry(void);
		//: Destructor.

		virtual GeometryType getGeometryType( void ) 
		{ 
			return vertexGeometry; 
		}
		//: Get the geometry type.

		void setVertices( vhtVector3d *vertexArray, int numVerticies );
		//: Set the vertices.
		//!param: vertexArray - Array of vertices.
		//!param: numVertices - Number of vertices in the array.
		// Given an array of vhtVector3d's, construct the corresponding vertex geometry.  The
		// vertexArray is assumed to have numVertices entries.
		// The resulting geometry will be the convex hull of the vertices.
	    void setVertices( vhtArray &vertexArray );
		//: Set the vertices.
		//!param: vertexArray - Array of vertex pointers.
		// Given an array of vhtVector3d pointers, construct the corresponding vertex geometry.  
		// The resulting geometry will be the convex hull of the vertices.

		inline vhtArray *getVertices(void) 
		{ 
			return &m_vertexArray; 
		}
		//: Get vertices.
		// Returns an array of pointers to vhtVector3d objects.  Do not delete any of these objects.

		inline double getBoundingRadius(void) 
		{ 
			return m_boundingRadius; 
		}
		//: Get bounding info, computed from vertices.
		// Returns the distance from the local origin (centroid) to the furthest vertex.

	    inline const vhtVector3d &getBoundingCenter(void) 
		{ 
			return m_boundingCenter; 
		}
		//: Get the centroid.
		// Returns the centroid of the vertices.
  

		virtual void render( void );
	   //: Render the vertices.

		void reference( void );
		//: Reference this object.

		void  unReference( void );
		//: Unreference.

	protected:
		void                          computeBound(void);

		double                        m_boundingRadius;
		vhtVector3d                   m_boundingCenter;
		vhtArray                      m_vertexArray;
};
#endif


