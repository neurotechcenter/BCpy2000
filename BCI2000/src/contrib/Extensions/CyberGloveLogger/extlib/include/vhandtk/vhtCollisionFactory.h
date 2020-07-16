#ifndef __VHT_COLLISION_FACTORY_H
#define __VHT_COLLISION_FACTORY_H


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
class vhtGeometry;

//: Factory abstraction for collision management.
// The vhtCollisionFactory interface defines the methods that are used to
// construct two collider specific data structures, the vhtCollisionPair and
// the vhtGeometry.  Specific colliders must inherit this class and implement
// all the pure virtual methods.  At construction time for the
// vhtCollisionEngine, a valid (implemented) vhtCollisionFactory must already
// exist.
//!vhtmodule: Core
class VH_CORE_DLL_EXPORT vhtCollisionFactory
{
	protected:
		unsigned int			m_factoryId;
		bool					m_bDoHandHand;

	public:
		vhtCollisionFactory( void );
		//: Construct a basic collision pair factory.

		virtual ~vhtCollisionFactory( void ) {;}
		//: Destruct.

		virtual vhtCollisionPair *generateCollisionPair( vhtShape3D &obj1, vhtShape3D &obj2 ) = 0;
		//: Generate a collision pair corresponding to obj1 and obj2.
		//!param: obj1 - First object of collision pair.
		//!param: obj2 - Second object of collision pair.
		// Implemented sub-classes of the collisionPairFactory must implement
		// this method.
		// Implementors should check for compatible shapes, attributes, etc. to
		// generate the appropriate collision pair.  A return value of NULL
		// indicates that the pair is invalid.  The method
		// vhtCollisionPair::isValidPair performs a number of basic checks and
		// may be sufficient for many users.

		virtual vhtGeometry	  *generateCollisionGeometry( vhtShape3D &obj ) = 0;
		//: Generate the collider dependent collision geometry.
		//!param: obj - Shape container to generate geometry for.
		// The collision geometry appropriate for each shape3D should be
		// generated in this method.

		virtual void removeCollisionPair( vhtCollisionPair *aPair );
		//: Remove the indicated pair.

		virtual void removeCollisionGeometry( vhtShape3D *obj );
		//: Remove the associated collision geometry.

		void setFactoryId( unsigned int anId );
		//: Set this factories Id.
		//!param: anId - The factory id to use.
		// The factoryId is used to index multiple factories in the vhtShape3D
		// container.  The factoryId indexes the vhtShape3D::getGeometry and
		// vhtShape3D::setGeometry methods.

		void setEnableHandHand(bool bEnable);
		//: Set whether this factory should check for collisions between the
		// shapes of HumanHand(s).
};

#endif
 			
