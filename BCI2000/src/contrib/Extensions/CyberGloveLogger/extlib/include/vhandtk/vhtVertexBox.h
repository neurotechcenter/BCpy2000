/********************************************************************
 FILE: $Id: vhtVertexBox.h,v 1.8 2002/08/28 23:16:27 jsilver Exp $
 AUTHOR: C Ullrich
 DATE: Oct 1998

 DESCRIPTION:
  -
 HISTORY:
  -
           -- COPYRIGHT VIRTUAL TECHNOLOGIES, INC. 1998 --
********************************************************************/

#ifndef __VHT_VERTEX_BOX_H
#define __VHT_VERTEX_BOX_H

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

//: Build a vertex geometry parallelpiped.
// Use this class to build box like geometries.
//!vhtmodule: Core
class VH_CORE_DLL_EXPORT vhtVertexBox : public vhtVertexGeometry 
{
	public:
		vhtVertexBox( double length = 1.0, double width = 1.0, double height = 1.0 );
		//: Construct a box.
		// By default, the box has unit dimensions.
    
		inline void setDimensions( double length, double width, double height ) 
		{ 
			m_dimensions = vhtVector3d( length, width, height );
			updateVertices(); 
		}

		//: Set box dimensions.
		inline void setDimensions( vhtVector3d &dim ) 
		{
			m_dimensions = dim; 
		}
		//: Set box dimensions.

		// Length, width and height are specified as the three components of dim.
		inline vhtVector3d  &getDimensions( ) 
		{ 
			return m_dimensions; 
		} 
		//: Get box dimensions.

		void applyMassProperties( vhtMassProperties &massProp ); 
		//: Compute analytic mass properties.
		// Uses analytic expressions for the mass properties of a parallelpiped.
	
	private:

		void updateVertices();

    vhtVector3d  m_dimensions;
};

#endif


