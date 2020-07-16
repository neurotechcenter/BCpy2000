#ifndef _VHT_TRIANGLE_GEOMETRY_H
#define _VHT_TRIANGLE_GEOMETRY_H

#if defined(BUILD_WIN32_VH_DLL)

#if defined __BUILD_VH_CORE_DLL__
#define VH_CORE_DLL_EXPORT __declspec(dllexport)
#else
#define VH_CORE_DLL_EXPORT __declspec(dllimport)
#endif

#else 

#define VH_CORE_DLL_EXPORT

#endif

#include <vhtVertexGeometry.h>

//: A triangle based geometry.
// Uses a set of triangles to describe a geometry.  Vertices can still be accessed
// as per the vhtVertexGeometry representation.
class VH_CORE_DLL_EXPORT vhtTriangleGeometry : public vhtVertexGeometry
{
	public:
		vhtTriangleGeometry( void );
		//: Construct an empty triangle geometry.
		~vhtTriangleGeometry( void );
		//: Delete.

		virtual GeometryType getGeometryType( void ) 
		{ 
			return triangleGeometry; 
		}
		//: Get the geometry type.

		void  setTriangles( vhtArray &vertexArray, vhtArray &triangleArray );
		//: Set the triangle array.

		const vhtArray &getTriangles( void );
		// Get the triangle array.

	protected:
		vhtArray                      m_triangleArray;

};

#endif
