/********************************************************************
 FILE: $Id: vhtVertexSphere.h,v 1.7 2002/08/28 23:16:28 jsilver Exp $
 AUTHOR: C Ullrich
 DATE: Oct 1998

 DESCRIPTION:
  -
 HISTORY:
  -
           -- COPYRIGHT VIRTUAL TECHNOLOGIES, INC. 1998 --
********************************************************************/

#ifndef __VHT_VERTEX_SPHERE_H
#define __VHT_VERTEX_SPHERE_H

#if defined(BUILD_WIN32_VH_DLL)

#if defined __BUILD_VH_CORE_DLL__
#define VH_CORE_DLL_EXPORT __declspec(dllexport)
#else
#define VH_CORE_DLL_EXPORT __declspec(dllimport)
#endif

#else 

#define VH_CORE_DLL_EXPORT

#endif

class vhtMassProperties;

#include "vht.h"
#include "vhtVertexGeometry.h"

//: A vertex based sphere.
// Convenience class for representing sphere like geometries.
//!vhtmodule: Core
class VH_CORE_DLL_EXPORT vhtVertexSphere : public vhtVertexGeometry 
{
	public:
		vhtVertexSphere( double radius = 1.0, int longRez = 5, int latRez = 5 );
		// Construct a vertex sphere.
		// The resulting sphere.  The longRez parameter is the number
		// of vertices around sphere longitude.  The latRez is the number of
		// vertices around the sphere lattitude. The total number of vertices
		// on the resulting geometry will be longRes*latRex.
		inline void setRadius( double radius ) 
		{ 
			m_radius = radius; 
			updateVertices(); 
	    }
		//: Set the sphere radius.
		
		inline double getRadius( ) 
		{ 
			return m_radius; 
		} 
		//: Get the sphere radius.
    
		inline void setLattitudeResolution( int latRes ) 
		{ 
			m_lattitudeResolution = latRes; 
		}
		//: Set the lattitude vertex resoultion.

		inline int getLattitudeResolution( ) 
		{ 
			return m_lattitudeResolution; 
		}
		//: Get the lattitude vertex resoultion.

		inline void setLongitudeResolution( int longRes ) 
		{ 
			m_longitudeResolution = longRes; 
		}
		//: Set the longitude vertex resoultion.
    
		inline int getLongitudeResolution( ) 
		{ 
			return m_longitudeResolution; 
		}
		//: Set the lattitude vertex resoultion.
		
		void applyMassProperties( vhtMassProperties &massProp );
		//: Compute analytic mass properties.
		// Uses analytic expressions for the mass properties of a sphere.
	
	private:
		void updateVertices();

		double   m_radius;
		int      m_lattitudeResolution;
		int      m_longitudeResolution;
};

#endif


