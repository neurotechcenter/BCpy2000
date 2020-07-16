#ifndef VHT_OCTREE_NODE_H
#define VHT_OCTREE_NODE_H
/********************************************************************
 FILE: $Id: vhtOctreeNode.h,v 1.9 2002/08/28 23:16:20 jsilver Exp $
 AUTHOR: C Ullrich
 DATE: 1998

 DESCRIPTION:
  - a single cube in the octree
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

class vhtArray;
class vhtShape3D;
class vhtPairTable;
class vhtCollide;

//: One octree cell.
// A single cell in the collision detection octree structure.  Each cell contains pointers
// to all cells contained inside it. All octree cells are cubical.  This data structure should not 
// be used directly, instead the vhtCollisionEngine interface should be used.
//!vhtmodule: Core
class VH_CORE_DLL_EXPORT vhtOctreeNode
{ 
	public:
		vhtOctreeNode( int depth = 2);
		//: Construct an octree.
		//!param: depth - Depth of the octree to create.
		// This will create 1 + 8 + 64 cells. Note that memory usage for the
		// octree scales as the cube of the depth.

		vhtOctreeNode( const vhtOctreeNode &node );
		//: Copy an octree.

		~vhtOctreeNode( void );
		//: Destruct this node and all children.

		/* Octree sizing and location */
		void setEdgeLength( double length );

		//: Set the edge length of this octree cell.
		//!param: length - Edge length of this cell.
		// Automatically recalculates all children cells to fit the new edge length.
		inline double getEdgeLength( void ) const 
		{ 
			return m_xmax-m_xmin; 
		}
		//: Get the current edge length.

		void setCenter( vhtVector3d &center );
		//: Set the center of this octree node.
		//!param: center - New center for octree.
		// Recursively adjusts all children.

		inline const vhtVector3d &getCenter( void ) const 
		{ 
			return m_center; 
		}
		//: Get the center of this octree node.

		/* Collision parameters */
		inline void setMinEdgeScale( double alpha ) 
		{ 
			m_alpha = alpha; 
		}
		
		//: Set the edge alpha parameter.
		//!param: alpha - Edge alpha.
		inline double getMinEdgeScale( void ) const 
		{ 
			return m_alpha; 
		}
		
		//: Get the minimum edge scaling.
		inline void setMaxEdgeScale( double beta ) 
		{ 
			m_beta = beta; 
		}
		
		//: Set the edge beta parameter.
		//!param: beta - Edge beta.
		inline double getMaxEdgeScale( void ) const 
		{ 
			return m_beta; 
		}
		//: Get the beta parameter.

		/* Object mantinence */
		void addObject( vhtShape3D &object );
		//: Insert an object into this octree.
		//!param: object - Object to insert.

		void addObject( vhtShape3D &object, vhtVector3d &location );
		//: Insert an object with a given location vector.
		//!param: object - Object to insert.
		//!param: location - Location of object in world space.
		// Provided as a performance optimization.

		/* Collision data structures */
		void setPairTable( vhtPairTable *pairTable );
		//: Set the collision pairtable for this octree.
		//!param: pairTable - Associated pair table.
		// One pair table per octree is permitted.
		inline const vhtPairTable *getPairTable( void ) const  
		{ 
			return m_pairTable; 
		}
		//: Get the corresponding pairtable.

		void reset( void );
		//: Reset this octree.
		// Remove all inserted objects from this octree.

		/* Debug */
		void print( void );
		//: Print this octree to stderr.
		// Depreciated.

		void render( void );
		//: Render the octree in the OpenGL ontext.

	private:
		//
		// children
		//
		vhtOctreeNode      **m_children;
		int                  m_depth;
		void                 m_allocateChildren(void);
    
		//
		//
		double               m_alpha;
		double               m_beta;
    
		//
		// box co-ordinates
		//
		double               m_xmin;
		double               m_xmax;

		double               m_ymin;
		double               m_ymax;
    
		double               m_zmin;
		double               m_zmax;
  
		vhtVector3d          m_center;
    
		void                 m_recalculateChildren(void);
		//
		// hash bucket for object ids
		//
		int                 *m_objectIds;
		int                  m_numObjects;
		int                  m_objectIdArraySize;

		bool                 m_hasMinEntry;
    
		//
		// pair table
		//
		vhtPairTable        *m_pairTable;
};




#endif


