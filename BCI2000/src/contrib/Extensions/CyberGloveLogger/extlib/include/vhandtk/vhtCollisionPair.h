#ifndef VHT_COLLISION_PAIR_H
#define VHT_COLLISION_PAIR_H
/********************************************************************
 FILE: $Id: vhtCollisionPair.h,v 1.14 2002/08/28 23:16:08 jsilver Exp $
 AUTHOR: C. Ullrich
 DATE: 1998

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

#include <vhtVector3d.h>
#include <vhtTransform3D.h>
#include <vhtShape3D.h>

#include <vhtCollide.h>

class vhtShape3D;
class vhtCollisionTracking;

//: Collision report for two objects.
// Report collision data about an object pair.  Objects are collided at the
// vhtShape3D level, and all information produced by collisions should be stored here.
// The vhtCollisionPair object is constructed by the associated vhtCollisionFactory
// object.  Functionality related to specific colliders that is not represented in this
// class can be added as a subclass, and built with the appropriate factory.
// As an absolute minimum, all colliders should report the MTD (see vhtCollide).
// Note that not all colliders can return all the information found in this class. As a result
// not all methods can be relied on to return sensible results for all colliders.
//!vhtmodule: Core
class VH_CORE_DLL_EXPORT vhtCollisionPair 
{
	public:
		enum FeatureType { Face, Edge, Vertex };
		
		vhtCollisionPair(void);
		//: Default constructor.
		// Everything is zero.
		vhtCollisionPair( vhtShape3D *obj1, vhtShape3D *obj2 );
		//: Construct a collision pair for two objects.
		//!param: obj1 - First collision object.
		//!param: obj2 - Second collision object.
		// Builds a collision pair data structure corresponding to 
		// obj1 and obj2. Automatically check if this pair is valid.

		virtual ~vhtCollisionPair(void);
		//: Destruct collision pair.

		inline vhtShape3D *getObject1(void) const { return m_obj1; }
		//: Get object1.
		
		inline void setObject1( vhtShape3D *obj1) { m_obj1 = obj1; m_validChecked = false; }
		//: Set object1.
		//!param: obj1 - First object.

		inline vhtShape3D *getObject2(void) const { return m_obj2; }
		//: Get object2.

		inline void setObject2( vhtShape3D *obj2) { m_obj2 = obj2; m_validChecked = false; }
		//: Set object2.
		//!param: obj2 - Second object.

		/*
		// witness points
		*/
		inline bool getWitnessValid(void) { return m_witnessValid; }
		//: Check if witness point is valid.
		// Only returns the cached witness validity.

		inline const vhtVector3d &getWitness1(void) const { return m_witness1; }
		//: Get object1 witness point.
		// Witness points are in object frame co-ordinates.  Witness points
		// are points on the surface of the object that are closest to the
		// other object.  Witness points do not have to be object vertices.
		inline void setWitness1( const vhtVector3d &witness1 ) 
		{
			if( m_lastMTD > 0.0 ) {
			m_cachedWitness1 = m_witness1;
			}
			m_witness1 = witness1; 
		}
		//: Set object1 witness point.
		//!param: witness1 - First witness point.
		// Witness points are stored in the object local reference frame.
		inline const vhtVector3d &getWitness2(void) const { return m_witness2; }
		//: Get object2 witness point.
		inline void setWitness2( const vhtVector3d &witness2 )
		{
			if( m_lastMTD > 0.0 ) {
			m_cachedWitness2 = m_witness2;
			}
			m_witness2 = witness2; 
		}

		//: Set object2 witness point.
		//!param: witness2 - Second witness point.
		// Witness points are stored in the object local reference frame.

		/*
		// distance
		*/
		inline double getMTD(void) 
		{ 
			m_lastMTD = m_collide->getMTD( *this ); 
			return m_lastMTD; 
		}
		//: Calculate the MTD for this object pair.
		// The MTD is the Minimum Translation Distance between two objects.  This distance
		// which acts like a signed proximity measure, is positive for disjoint objects
		// and negative for penetrating objects.  Not all colliders are capable of 
		// determining the full range of the MTD (i.e. some only return positive distances).
		// This method calls the collider for this pair to determine the MTD.
		inline double getLastMTD(void) const { return m_lastMTD; }
		//: Get the most recently calculated MTD value.
		// Use this method to access the MTD value stored in this object.
		inline void setLastMTD( double lastMTD) { m_lastMTD = lastMTD; }
		//: Set the most recent MTD.

		/*
		// check if the current pair is valid
		*/
		virtual bool isValidPair(void);
		//: Determine if the collision pair is valid.
		// Checks for pair validity.  Pairs are considered valid if they are members of different
		// components, isOneExternal() == true, or at least one object is dynamic.
		// Subclasses of vhtCollisionPair may want to override this behavior to produce
		// different types of valid collision pairs.
		bool isOneExternal(void);
		//: Determine if one object has external dynamics.
		// Checks if one of the objects has vhtExternalAttributes.
		// Deprecaited.

		/*
		// contact normal
		*/
		inline const vhtVector3d &getContactNormal1(void) { return m_contactNormal1; }
		//: Get the contact normal for object1.
		// Contact normals are in object co-ordinates.
		inline void setContactNormal1( const vhtVector3d &normal1 )
		{ 
			if( m_lastMTD > 0 ) {
				m_cachedNormal1 = m_contactNormal1;
			}
			m_contactNormal1 = normal1; 
		}
		//: Set the contact normal for object1.
		//!param: normal1 - Contract normal for object 1.
		// Contact normals are in object co-ordinates.

		inline const vhtVector3d &getContactNormal2(void) { return m_contactNormal2; }
		//: Get the contact normal for object2.
		// Contact normals are in object co-ordinates.
		inline void setContactNormal2( const vhtVector3d &normal2 )
		{ 
			if( m_lastMTD > 0 ) {
				m_cachedNormal2 = m_contactNormal2;
			}
			m_contactNormal2 = normal2; 
		}
		//: Set the contact normal for object2.
		//!param: normal2 - Contract normal for object 2.
		// Contact normals are in object co-ordinates.

		/*
		  // Contact features
		*/
		virtual FeatureType getFeature1(  void ) { return m_feature1; }
		//: Return the feature type for object1.
		// The feature type can be either a Face, an Edge or a Vertex.

		virtual FeatureType getFeature2(  void ) { return m_feature2; }
		//: Return the feature type for object2.
		// The feature type can be either a Face, an Edge or a Vertex.

		virtual void setFeature1( FeatureType fType ) { m_feature1 = fType; }
		//: Set the feature type for object1.
		//!param: fType - Feature type for object 1.
		// The feature type can be either a Face, an Edge or a Vertex.

		virtual void setFeature2( FeatureType fType ) { m_feature2 = fType; }
		//: Set the feature type for object2.
		//!param: fType - Feature type for object 2.
		// The feature type can be either a Face, an Edge or a Vertex.

		/* 
		// get/set the collision model for this pair
		*/
		inline void setCollisionModel( vhtCollide *collide) { m_collide = collide; }
		//: Set the collider for this object pair.
		//!param: collide - Collider to use for finding collision info about this pair.
		// The collider is used to calculate the collision information for the two objects.
		// This is usually set by the associated factory object.
		inline vhtCollide *getCollisionModel() { return m_collide; }
		//: Get the collider for this object pair.

		/*
		// get/set the collision tracking
		*/
		inline void setCollisionTracking( vhtCollisionTracking *tracking ) { m_tracking = tracking; }
		//: Set the collision tracker for this pair.
		// A value of NULL diables collision tracking.
		// Collision tracking must be supported by the associated collider.
		inline vhtCollisionTracking *getCollisionTracking(void) { return m_tracking; }
		//: Get the collision tracker for this pair.

		void setEnableHandHand(bool bEnable);
		//: Set whether to enable collisions between the shapes of HumanHand(s).

		/*
		  Get Cached values, valid for last positive MTD
		 */
		inline const vhtVector3d &getCachedWitness1( void ) { return m_cachedWitness1; }
		inline const vhtVector3d &getCachedWitness2( void ) { return m_cachedWitness2; }
		inline const vhtVector3d &getCachedNormal1( void ) { return m_cachedNormal1; }
		inline const vhtVector3d &getCachedNormal2( void ) { return m_cachedNormal2; }

		inline double getCachedMTD( void )
		{
			if( m_lastMTD > 0.0 ) {
				return m_lastMTD;
			} else {
				vhtVector3d wfWitness1 = m_cachedWitness1;
				vhtVector3d wfWitness2 = m_cachedWitness2;

				m_obj1->getLM().transform( wfWitness1 );
				m_obj2->getLM().transform( wfWitness2 );

				vhtVector3d tmp;
				tmp.sub( wfWitness1, wfWitness2 );
				return -tmp.length();
			}
		}

	protected:
		vhtShape3D	 *m_obj1;
		//: First object.
		vhtShape3D	 *m_obj2;
		//: Second object.

		bool			m_witnessValid;
		//: True if witness points are valid.
		vhtVector3d	 m_witness1;
		//: First witness point (in obj1 frame ).
		vhtVector3d	 m_witness2;
		//: Second witness point (in obj2 frame ).

		vhtVector3d	 m_contactNormal1;
		//: First normal vector (in obj1 frame ).
		vhtVector3d	 m_contactNormal2;
		//: Second normal vector (in obj2 frame ).

		double		  m_lastMTD;
		//: Cached MTD value.

		vhtCollide	 *m_collide;	
		//: Associated collider.
		vhtCollisionTracking *m_tracking;
		//: Associated collision tracker.
		
		bool			m_validChecked;
		//: True if validity has been checked.
		bool			m_valid;
		//: True if pair is valid.
		bool			m_bDoHandHand;
		//: True if hand-to-hand collision are allowed.

		FeatureType	 m_feature1;
		//: Feature type of first witness point.
		FeatureType	 m_feature2;
		//: Feature type of second witness point.

		vhtVector3d	 m_cachedWitness1;
		vhtVector3d	 m_cachedWitness2;

		vhtVector3d	 m_cachedNormal1;
		vhtVector3d	 m_cachedNormal2;

};
#endif


