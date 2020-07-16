#ifndef VHT_COLLIDE_H
#define VHT_COLLIDE_H
/********************************************************************
 FILE: $Id: MATH_NAMESPACE.h,v 1.11 2002/08/28 23:16:07 jsilver Exp $
 AUTHOR: C. Ullrich
 DATE: 1998

 DESCRIPTION:
  - virtual base class for local collision detection between object 
    pairs.
 TODO:
  -
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

class vhtCollisionPair;
class vhtShape3D;
class vhtTransform3D;
class vhtBoundingSphere;

//: Local collision model virtual base class.
// This class defines the basic operations that any pairwise distance computation
// algorithm must provide.  Subclasses of this should provide the method getMTD
// which takes a vhtCollisionPair argument.
// See the class vhtCollisionPair for details on the type of information that
// a collider should provide.  The vhtCollisionFactory mechanism can be used to
// precisely control the vhtCollide sub-class that is associated with each 
// vhtCollisionPair.
// Note that this class should not be used directly, but by the vhtCollisionEngine
// framework.
//!vhtmodule: Core
class VH_CORE_DLL_EXPORT vhtCollide
{
public:
    vhtCollide( unsigned int geometrySlot  );
    //: Default constructor.
    //!param: geometrySlot - Geometry slot associated with this collider.
    // The geometry slot is an index into the getGeometry method of vhtShape3D, and
    // is the location of the associated collision geometry.

    double getMTD( const vhtTransform3D &LM1, double radius1, const vhtTransform3D &LM2, double radius2 );
    //: Get MTD for two spheres.
    //!param: LM1 - Local->World transform for sphere 1.
    //!param: radius1 - Radius of sphere 1.
    //!param: LM2 - Local->World transform for sphere 2.
    //!param: radius2 - Radius of sphere 2.
    // Explicitly calculate the distance between two spheres.

    double getMTD( const vhtBoundingSphere &sphere1, const vhtBoundingSphere &sphere2 );
    //: Get MTD for two bounding spheres.
    //!param: sphere1 - Bounding sphere 1.
    //!param: sphere2 - Bounding sphere 2.
    // Explicitly calculate the distance between two vhtBoundingSphere objects.

    double getMTD( vhtShape3D &obj1, vhtShape3D &obj2 );
    //: MTD for two objects.
    //!param: obj1 - First object.
    //!param: obj2 - Second object.
    // Generates a temporary vhtCollisionPair object and calls getMTD with that.

    virtual double getMTD( vhtCollisionPair &pair) { return 999999999999999.0;}
    //: MTD for collision pair.
    // Find the minimum translation distance for the indicated collision pair.  This
    // distance is positive if the two objects are not touching and indicates their
    // minimum separation.  For objects that ar just touching, the MTD is zero.  For
    // penetrating objects, the MTD is the smallest translation that makes the
    // two objects just touch.
    // This method should also fill the relevant fields of the vhtCollisionPair
    // object (i.e. witness points, normals, etc. ).
    
protected:
	vhtCollide( void );
    //: Default constructor.

    unsigned int            m_geometrySlot;
    //: Location of this collision goemetry in vhtShape3D.

};

#endif



