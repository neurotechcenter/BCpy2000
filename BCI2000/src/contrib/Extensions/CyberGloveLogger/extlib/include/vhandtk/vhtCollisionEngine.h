#ifndef VHT_COLLISION_ENGINE_H
#define VHT_COLLISION_ENGINE_H
/********************************************************************
 FILE: $Id: vhtCollisionEngine.h,v 1.17 2002/08/28 23:16:08 jsilver Exp $
 AUTHOR: Chris Ullrich
 DATE: 1998
<header>

 DESCRIPTION:
  - main user interface to collision detection functionality
 HISTORY:
  -
 NOTES:
  -
</header>
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


class vhtOctreeNode;
class vhtCollide;
class vhtPairTable;
class vhtTransformGroup;
class vhtGroup;
class vhtShape3D;
class vhtCollisionFactory;
class vhtArray;


//: Manage collision detection.
// The vhtCollisionEngine class is designed to simplify and unify all collision detection
// for a user specified scene graph.  The collision engine builds and maintains all
// needed collision detection data structures and manages all calls to low level 
// collision detection routines.  Users simply specify the haptic scene graph (or sub-graph) 
// that they wish this engine to manage. After that, the user need only call collisionCheck
// to get the most recent list of collisions.
//!vhtmodule: Core
class VH_CORE_DLL_EXPORT vhtCollisionEngine
{
	public:
		vhtCollisionEngine( vhtCollisionFactory *aFactory, int octreeDepth = 2);
		//: Default constructor.
		//!param: aFactory - A collision factory object.
		//!param: octreeDepth - The depth of the octree to use for spatial culling.
		// Builds a default octree, and pair table.
		vhtCollisionEngine( vhtGroup *root, vhtCollisionFactory *aFactory, int octreeDepth = 2 );
		//: Construct a collision engine for root.
		//!param: root - The root node of the haptic scene graph that will be collided.
		//!param: aFactory - A collision factory object.
		//!param: octreeDepth - The depth of the octree to use for spatial culling.
		// Builds a collision engine, including octree and pair table for the scene graph pointed
		// to by root.  All vhtShape3D objects under root are parsed, and appropriate collision
		// data structures are created.

		~vhtCollisionEngine(void);
		// Destructor.

		/* Collision management */
		vhtArray *collisionCheck(void);
		//: Calculate collisions.
		// Scan the collision objects for intersections and epsilon proximit. All pairs
		// with MTD < collision epsilon are reported.  The report format is an array of
		// vhtCollisionPair objects (see vhtArray).  The user application should NOT delete the
		// entries of this list.  The collision pairs returned are of the same type as those
		// produced by the associated vhtCollisionFactory.

		vhtArray *getCollisionList(void) const;
		//: Return list of current collisions.
		// Generates a list of collision for objects that have not been updated.  This method
		// simply returns the most recently generated collision list, and performs no
		// actual collision detection.

		void regenerateDataStructures(void);
		//: Rebuild all collision data.
		// Rebuilds all the collisiond data structures to reflect some change in the scene
		// graph.  This method uses the current scene graph as specified in setHapticSceneGraph, or in
		// the appropriate constuctor.

		/* Engine parameters */
		void setCollisionEpsilon( double epsilon );
		//: Set the collision tolerance.
		//!param: epsilon - Collision tolerance value.
		// This is the distance below which collisions are reported.  Setting this to 0 causes
		// only intersections to be reported.  Certain non-intersecting collisions below this 
		// tolerance may not be reported due to octree aliasing.

		double getCollisionEpsilon(void) const; 
		//: Get the collision tolerance.
		// Return the current setting for the collision epsilon.

		void setHandObjectOnly( bool handObjectOnly );
		//: Turn on or off object-object collision detection.
		// Passing true to this method causes the collision detection engine to only
		// consider collisions between hands and objects.  The default is false.

		/* Scene registration */
		void setHapticSceneGraph( vhtGroup *root );
		//: Set the haptic scene graph.
		//!param: root - The root node of the haptic scene graph that will be collided.
		// Sets the haptic scene graph to the graph pointed to by root. Re-initializies all
		// data structres for root.  Setting a root node that differs from an already set
		// root will result in complete re-initialization of the collision engine.

		void registerCollisionObject( vhtShape3D *object );   
		//: Add a particular object to the collision list.
		//!param: object - Specific object to be added to collision engine.
		// Use this method to add specific objects to the collision engine.  The object does not need
		// to be in the haptic scene graph for this method to work.  It is recommended to
		// use the version of this method that takes a vhtGroup node.

		void registerCollisionObject( vhtGroup *objRoot );
		//: Add a particular sub-tree to the collision list.
		//!param: objRoot - The root of a sub-tree.
		// All vhtShape3D nodes below objRoot are added. It is recommended to use setHapticSceneGraph
		// instead of this method.

		void removeCollisionObject( vhtShape3D *object );
		//: Remove a particular object from the collision manager.
		//!param: object - object to be removed.
		// Removes the object from teh collision manager and deletes any collision geometry.

		void removeCollisionObject( vhtGroup *objRoot, bool rebuild = true );
		//: Remove a particular sub-tree from the collision manager.
		//!param: objRoot - root of the sub-tree to remove.
		// All vhtShape3D's below objRoot will be removed from collision processing and
		// have their collision geometry deleted.

		/* Scene management */
		void updateObject( vhtShape3D *object );
		//: Update a particular object's position.
		//!param: object - Specific object to update.

		void updateObject( vhtGroup *objRoot, bool updateOctree = true );
		//: Update a sub-tree's position.
		//!param: objRoot - Sub-graph to update.
		//!param: updateOctree - Update the octree to reflect the new scene position and size.
		// Updates all vhtShape3D nodes below objRoot.  If updateOctree is true, the
		// size of the octree is adjusted to reflect the sub-tree.

		void updateOctreeSize( void );
		//: Update the octree dimensions.
		// Update the octree for any changes in the size of root.

		/* Debug */
		void render( bool renderAll = false );
		//: Render the collision informatin to the current OpenGL context.
		//!param: renderAll - Set to true to render all collisions.
		// Default behavior is to render external-object collisions only.

		void print(void) const; 
		//: Print debugging information.
		// Depreciated.


protected:
    void setUserReset( bool userReset );
    //: Set the reset condition,
    //!param: userReset - Set to true to manage the collision resets explicitly.
    // On calls to collisionCheck, the octree and pair table will be 
    // reset if this flag is false. Otherwise, the user must reset
    // these structures appropriately (via the reset method).

    void initialize(void);
    //: Initialize all data structures.
    // Computes pair table, and octree.

    void reset(void);
    //: Reset data structures.
    // Resets pair table and octree.

    void uninitialize(void);
    //: Clear all data structures.
    // Clear the pair table of all entries. No objects will collide after this call, until
    // new objects are added.

private:
    vhtOctreeNode          *m_octree;
    vhtPairTable           *m_pairTable;
    vhtGroup               *m_hsgRoot;
    bool                    m_userReset;
};



#endif		/* VHT_COLLISION_ENGINE_H */



