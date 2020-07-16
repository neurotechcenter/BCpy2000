/********************************************************************
 FILE: $Id: vhtVertexCylinder.h,v 1.7 2002/08/28 23:16:28 jsilver Exp $
 AUTHOR: C Ullrich
 DATE: Oct 1998

 DESCRIPTION:
  -
 HISTORY:
  -
           -- COPYRIGHT VIRTUAL TECHNOLOGIES, INC. 1998 --
********************************************************************/

#ifndef __VHT_VERTEX_CYLINDER_H
#define __VHT_VERTEX_CYLINDER_H

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

class vhtMassProperties;

//: A vertex based cylinder geometry.
// Convenience class for representing cylinder like geometries.
//!vhtmodule: Core
class VH_CORE_DLL_EXPORT vhtVertexCylinder : public vhtVertexGeometry 
{
	public:

	    vhtVertexCylinder( double radius = 1.0, double height = 1.0, int longRes=10 );
		//: Construct a vertex cylinder.
		// The cylinder will be oriented along the y-axis.  The height parameter
		// specifies the y axis length. The longRes parameter is the number
		// of vertices around the circumference of the ends.
    
		inline void setRadius( double radius ) 
		{ 
			m_radius = radius; 
			updateVertices(); 
		}
	    //: Set the cylinder radius.
    
		inline double getRadius( ) 
		{ 
			return m_radius; 
		} 
		//: Get the cylinder radius.

		inline void setHeight( double height ) 
		{
			m_height = height;
			updateVertices();
	    }
		//: Set the cylinder height.
    
		inline double getHeight( ) 
		{ 
			return m_height; 
		}
		//: Get the cylinder height.

		inline void setLongitudeResolution( int longRes ) 
		{ 
			m_longitudeResolution = longRes; 
			updateVertices();    
		}
		//: Set the longitude resolution.
    
		// This is the number of vertices around the ends.
		inline int getLongitudeResolution( ) 
		{ 
			return m_longitudeResolution; 
		}
		//: Get the longitude resolution.

		void applyMassProperties( vhtMassProperties &massProp );
		//: Compute analytic mass properties.
		// Uses analytic expressions for the mass properties of a cylinder.

	private:
		void updateVertices();

		double   m_radius;
		double   m_height;
		int      m_longitudeResolution;
};

#endif


