/********************************************************************
 FILE: $Id: vhtBoundingSphere.h,v 1.7 2002/08/28 23:16:07 jsilver Exp $
 AUTHOR: Ron Carmel
 DATE: Aug 1998

 DESCRIPTION:
  - Bounding sphere for a node component.

 HISTORY:
  -
           -- COPYRIGHT VIRTUAL TECHNOLOGIES, INC. 1998 --
********************************************************************/

#ifndef __VHTBOUNDINGSPHERE_H
#define __VHTBOUNDINGSPHERE_H

#if defined(BUILD_WIN32_VH_DLL)

#if defined __BUILD_VH_CORE_DLL__
#define VH_CORE_DLL_EXPORT __declspec(dllexport)
#else
#define VH_CORE_DLL_EXPORT __declspec(dllimport)
#endif

#else 

#define VH_CORE_DLL_EXPORT

#endif

#include <vhtVector3d.h>
#include <vhtNodeComponent.h>

//: Bounding sphere for a haptic sub-graph or leaf node.
// Bounding spheres are used for a variety of optimizations in the VHT.  Bounding
// spheres are automatically computed during scene graph updates.
//!vhtmodule: Core
class VH_CORE_DLL_EXPORT vhtBoundingSphere : public vhtNodeComponent {
	public:

		vhtBoundingSphere(void);
		//: Default constructor.
		// Sets center to the origin and radius to 0.

		vhtBoundingSphere(const vhtVector3d &center, double radius);
		//: Construct a sphere with given center and radius.
		//!param: center - Center of sphere.
		//!param: radius - Radius of sphere.

		~vhtBoundingSphere(void);
		//: destructor

		void combine(const vhtBoundingSphere &boundsObject);
		//: Merge this and boundsObject into a larger sphere.
		//!param: boundsObject - Sphere to combine.
		// Combines this with bounds and stores the result in this.

		void combine(const vhtVector3d &point);
		//: Incorporate point into bounding sphere.
		//!param: point - Point to incorporate.
		
		void combine(int numSpheres, vhtBoundingSphere **spheres);
		//: Merge numSpheres into this.
		//!param: numSpheres - Number of bounding spheres in array.
		//!param: spheres - Array of bounding spheres.
		// Finds smallest sphere that contains all sphers in the array.
    

		inline const vhtVector3d    &getCenter(void) const {return m_center;}
		//: Get sphere center.

		inline double getRadius(void) const {return m_radius;}
		//: Get sphere radius.

		void set(const vhtBoundingSphere &boundingSphere);
		//: Set bounding sphere.
		//!param: boundingSphere - Sphere to set.
		// Replace this with the bounding sphere.

		void setCenter(const vhtVector3d &center);
		//: Set sphere center.
		//!param: center - Center of sphere.

		void setRadius(double r);
		// Set sphere radius.
		//!param: r - Radius of sphere.

		bool intersect(vhtBoundingSphere &boundsObject);
		//: Intersection test for two spheres.
		//!param: boundsObject - Second sphere.
		// Returns true if spheres intersect.

private:

    vhtVector3d                 m_center;
    double                      m_radius;
    bool                        m_isChanged;
};

#endif



