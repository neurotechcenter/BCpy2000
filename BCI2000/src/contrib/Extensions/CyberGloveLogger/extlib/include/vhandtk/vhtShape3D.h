#ifndef __VHTSHAPE3D_H
#define __VHTSHAPE3D_H

/********************************************************************
 FILE: $Id: vhtShape3D.h,v 1.9 2002/08/28 23:16:23 jsilver Exp $
 AUTHOR: Ron Carmel
 DATE: Aug 1998

 DESCRIPTION:
  -
 HISTORY:
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

#include <vhtNode.h>

class vhtArray;
class vhtGeometry;
class vhtPhysicalAttributes;
class vhtComponent;

//: A 3 dimensional shape.
// All 3D geometries are represented in the HSG by this type of node.
// The actual geometry data is a node component of this class.  Other 
// geometry related data, such as the physical attributes, collision id
// and component (if any) are stored here.
//!vhtmodule: Core
class VH_CORE_DLL_EXPORT vhtShape3D : public vhtNode {
	public:
		vhtShape3D( void );
		//: Default constructor.
		vhtShape3D(vhtGeometry *geometry, unsigned int geometryId = 0);
		//: Build a shape with indicated geometry.
		//!param: geometry - Default geometry to use.
		//!param: geometryId - Id of default geometry.
		// Construct a vhtShape3D with a default geometry specified.

		virtual ~vhtShape3D( void );
		//: Destructor.

		virtual inline  vhtNodeType getType( void ) 
		{ 
			return VHT_SHAPE3D; 
		}
		//: HSG type info.

		vhtGeometry *getGeometry( unsigned int geometryId = 0 ) const;
		//: Get the associated geometry.
		//!param: geometryId - Id of geometry to get.

		void setGeometry(vhtGeometry *geometry, unsigned int geometryId = 0 );
		//: Set the associated geometry.
		//!param: geometry - Geometry to use.
		//!param: geometryId - Index of geometry.
   
		inline vhtPhysicalAttributes   *getPhysicalAttributes( void )
		{ 
			return m_physicalAttributes; 
		}
		//: Get the physical attributes.

		void setPhysicalAttributes(vhtPhysicalAttributes *physicalAttributes);
		//: Set the physical attributes.
		//!param: physicalAttributes - Attributes of this Shape.
		// Default attributes are vhtDynamicAttributes. This method also automatically adds/removes
		// this node from the dynamic scene thread.

		vhtShape3D *getNextDynamicNode( void );
		//: Get next dynamic vhtShape3D.
		// In threaded haptic scene graphs, this is a next pointer for the dynamic node list.

		vhtShape3D *getPrevDynamicNode( void );
		//: Get prev dynamic vhtShape3D.
		// In threaded haptic scene graphs, this is a previous pointer for the dynamic node list.

		inline int getCollisionId( void ) 
		{ 
			return m_collisionId; 
		}
		//: Get the assigned collision id.    
		
		inline void setCollisionId( int id ) 
		{ 
			m_collisionId = id; 
		}
		
		//: Set the collision id.
		//!param: id - Associated collision id.
		// User applications do not need to set this unless they have overriden the
		// collision engine.

		inline void setComponent( vhtComponent *component ) 
		{ 
			m_component = component; 
		}
		//: Set the associated component.
		//!param: component - Component to use.
		// Sets the component parent of this node.  This is done automatically when children
		// are added to the component.
		
		inline vhtComponent *getComponent( void  ) 
		{ 
			return m_component; 
		}
		//: Get the associated component.

		bool render( void );
		//: Render the geometry.

	private:

		virtual void                    refreshBoundingSphere( void );
		//: Refresh the bounding sphere.
		virtual void                    thread( void );
		//: Thread into the tree.
		virtual void                    unthread( void );
		//: Unthread from the tree.
    
		vhtArray                        *m_geometry;
		vhtPhysicalAttributes           *m_physicalAttributes;

		vhtShape3D                      *m_nextDynamicNode;
		vhtShape3D                      *m_prevDynamicNode;

		int                              m_collisionId;

		vhtComponent                    *m_component;
};

#endif


