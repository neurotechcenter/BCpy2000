/********************************************************************
 FILE: $Id: vhtGeometry.h,v 1.11 2002/08/28 23:16:14 jsilver Exp $
 AUTHOR: Ron Carmel
 DATE: Aug 1998

 DESCRIPTION:
  -
 HISTORY:
  -
           -- COPYRIGHT VIRTUAL TECHNOLOGIES, INC. 1998 --
********************************************************************/

#ifndef __VHTGEOMETRY_H
#define __VHTGEOMETRY_H

#if defined(BUILD_WIN32_VH_DLL)

#if defined __BUILD_VH_CORE_DLL__
#define VH_CORE_DLL_EXPORT __declspec(dllexport)
#else
#define VH_CORE_DLL_EXPORT __declspec(dllimport)
#endif

#else 
#define VH_CORE_DLL_EXPORT
#endif



#include <vhtNodeComponent.h>
#include <vhtVector3d.h>

class vhtOGLDisplayList;

//: Haptic geometry.
// vhtGeometry is a node component that specifies the geometric description
// of an object.  The class is intended as a base class for specific types
// of geometry such as vertex based or NURBS, and as such is primarily
// indended as an inherited place holder.  The main method of this
// class is the acess to the geometry bounds.
//!vhtmodule: Core
class VH_CORE_DLL_EXPORT vhtGeometry: public vhtNodeComponent {

	public:
		enum GeometryType { 
			geometry=0,
			vertexGeometry, 
			triangleGeometry, 
			numGeometryTypes 
		};
		//: Enumerate the geometry types.

		/* Constructors */
		vhtGeometry( void );
		//: Construct a default geometry.
		
		virtual ~vhtGeometry( void );
		//: Destructor.

		virtual GeometryType getGeometryType( void ) 
		{ 
			return geometry; 
		}

		//: Get the geometry type.

		/* get bounding info (virtual )*/
		virtual double getBoundingRadius(void) 
		{ 
			return 0.0;
		} 


		//: Get the bounding radius.
		// Returns the maximum distance from the object center of the bool m_freshHull;
		// object geometry.  The default return value is 0.
		
		virtual const vhtVector3d  &getBoundingCenter(void) 
		{ 
			static vhtVector3d nullCenter(0.0,0.0,0.0); 
			return nullCenter;
		}

		//: Get the bounding center.
		// Returns the center of the bounding sphere.  This vector is in the object
		// frame.

	    virtual void render(void);
		//: Render this node.
};

#endif



