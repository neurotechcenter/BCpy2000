/********************************************************************
 FILE: $Id: vhtVertexCone.h,v 1.7 2002/08/28 23:16:27 jsilver Exp $
 AUTHOR: C Ullrich
 DATE: Oct 1998

 DESCRIPTION:
  -
 HISTORY:
  -
           -- COPYRIGHT VIRTUAL TECHNOLOGIES, INC. 1998 --
********************************************************************/

#ifndef __VHT_VERTEX_CONE_H
#define __VHT_VERTEX_CONE_H

#if defined(BUILD_WIN32_VH_DLL)

#if defined __BUILD_VH_CORE_DLL__
#define VH_CORE_DLL_EXPORT __declspec(dllexport)
#else
#define VH_CORE_DLL_EXPORT __declspec(dllimport)
#endif

#else 

#define VH_CORE_DLL_EXPORT

#endif

#include "vht.h"
#include "vhtVertexGeometry.h"
#include "vhtMassProperties.h"

//: A vertex based cone geometry.
// Convenience class for representing cone like geometries.
//!vhtmodule: Core
class VH_CORE_DLL_EXPORT vhtVertexCone : public vhtVertexGeometry 
{
	public:
		vhtVertexCone( double radius = 1.0, double height = 1.0, int longRes = 10 );
		//: Construct a cone.
		// Radius is the radius of the cone base. Height is the distance from the 
		// center of the base to the tip (along the y axis). LongRes is the number of 
		// vertices placed around the base. 

		inline void setRadius( double radius ) 
		{ 
			m_radius = radius; 
			updateVertices(); 
	    }
		//: Set the radius of the cone base;

		inline double getRadius( ) 
		{ 
			return m_radius; 
		}		
		//: Get the radius.

		inline void setHeight( double height ) 
		{
			m_height = height;
			updateVertices();
	    }
		//: Set the height.

		// Height is the distance from the center of the base to the tip.
		inline double getHeight( ) 
		{ 
			return m_height; 
		}
		//: Get the height.

		inline void setLongitudeResolution( int longRes ) 
		{ 
			m_longitudeResolution = longRes;
			updateVertices();
		}
		//: Set the longitude resolution.
		// Longitude resolution is the number of vertices placed around the base.

		inline int getLongitudeResolution( ) 
		{ 
			return m_longitudeResolution; 
		}
		//: Get the longitude resolution.

		void applyMassProperties( vhtMassProperties &massProp );
		//: Compute analytic mass properties.
		// Uses analytic expressions for the mass properties of a cone.

	private:
		void updateVertices();

		double   m_radius;
		double   m_height;
		int      m_longitudeResolution;
};

#endif


