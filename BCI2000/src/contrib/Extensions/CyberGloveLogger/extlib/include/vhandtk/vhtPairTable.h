#ifndef VHT_PAIR_TABLE_H
#define VHT_PAIR_TABLE_H
/********************************************************************
 FILE: $Id: vhtPairTable.h,v 1.11 2002/08/28 23:16:22 jsilver Exp $
 AUTHOR: C. Ullrich
 DATE: 1998

 DESCRIPTION:
  - a data structure for managing all possible collision pairs
 TODO:
  -
           -- COPYRIGHT VIRTUAL TECHNOLOGIES, INC. 1999 --
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

#include <vhtArray.h>
#include <vhtSymmetricTable.h>

class vhtShape3D;
class vhtCollide;
class vhtCollisionFactory;

//: Collision pair management.
// This class stores all possible collision pairs for a particular
// collision engine. Pairs are validated using the isValidPair method
// of the vhtCollisionPair class.  This class should never be used 
// directly, only the vhtCollisionEngine interface should be used.
//!vhtmodule: Core
class VH_CORE_DLL_EXPORT vhtPairTable
{
	public: 
		vhtPairTable( void );
		//: Construct a pair table.
		~vhtPairTable( void );
		//: Destruct.

		void setHandObjectOnly( bool handObjectOnly );
		//: Turn on or off object-object collision detection.
		//!param: handObjectOnly - Enable/disable object-object collisions.
		// Passing true to this method causes the collision detection engine to only
		// consider collisions between hands and objects.  The default is false.
		// This method is depreciated.

		void  addObject( vhtShape3D *object, bool forceRebuild = true );
		//: Add an object to the pair table.
		//!param: object - Object to add to pairTable.
		//!param: forceRebuild - Force the table to be rebuilt.
		void  removeObject( vhtShape3D *object, bool forceRebuild = true );
		//: Remove an object.
		//!param: object - Remove an object from the table.
		// Not implemented.

		void clearTable( void ); 
		//: Empty the pair table.

		void deletePairTable( void );
		//: Delete all entries in the pair table.

		void  buildTable( void);
		//: Build all necessary data structures. 
		// Builds the pair table corresponding to the current list of
		// added objects.

		void setCollisionFactory( vhtCollisionFactory *aCollisionFactory );
		//: Set the collision factory.
		//!param: aCollisionFactory - The associated collision factory.

		inline vhtArray *getCollisionList( void )  { return &m_collisions; }
		//: Get the collision list.
		// Return a list of objects that are less than the collision epslilon
		// apart. Do not delete the entries in this list.


		inline void setCollisionEpsilon( double epsilon ) 
		{ 
			if( epsilon >= 0.0 ) { 
				m_collisionEpsilon = epsilon; 
			} 
		}
		//: Set collision reporting threshold.
		//!param: epsilon - Collision proximity epsilon.
		// Set the distance below which collisions are reported. Must be greater than zero.
		// This value is only used if the associated collider supports distance
		// information.
		inline double getCollisionEpsilon( void ) 
		{ 
			return m_collisionEpsilon; 
		}
		//: Get the collision reporting threshold.

		void  reset( void );
		//: Reset the collision list.

		void  collisionCheck( int object1CollisionId, int object2CollisionId );
		//: Do explicit collision check between two objects.
		//!param: object1CollisionId - CollisionId of first object.
		//!param: object2CollisionId - CollisionId of second object.

	private:
		vhtArray                m_object;
		vhtSymmetricTable       m_pairs;
		vhtSymmetricTable       m_pairCheckCounter;
		vhtArray                m_collisions;

		double                  m_collisionEpsilon;
		int			    m_count;

		bool                    m_handObjectOnly;

		vhtCollisionFactory    *m_collisionFactory;

		int                     m_ValidPairCount;
};

#endif


