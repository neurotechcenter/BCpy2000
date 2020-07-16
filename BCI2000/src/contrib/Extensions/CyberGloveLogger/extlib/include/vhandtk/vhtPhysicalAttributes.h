
#ifndef VHT_PHYSICAL_ATTRIBUTES_H
#define VHT_PHYSICAL_ATTRIBUTES_H

#if defined(BUILD_WIN32_VH_DLL)

#if defined __BUILD_VH_CORE_DLL__
#define VH_CORE_DLL_EXPORT __declspec(dllexport)
#else
#define VH_CORE_DLL_EXPORT __declspec(dllimport)
#endif

#else 

#define VH_CORE_DLL_EXPORT

#endif

/**********************************************************************
FILE:    $Id: vhtPhysicalAttributes.h,v 1.10 2002/08/28 23:16:22 jsilver Exp $
AUTHOR:  C. Ullrich

<Header>
DESCRIPTION: Abstract physical entity class.  All physical objects should
 inherit from this class
</Header>
               -- COPYRIGHT VIRTUAL TECHNOLOGIES, INC. 1998 --
 **********************************************************************/

#include <vhtNode.h>
#include <vhtNodeComponent.h>

//: Associated physical attributes.
// Shape3D nodes may have associated physical attributes.  This class
// defines the base for these attributes.  The basic physical attributes
// are collision related.
//   Physical attributes are used to provide an interaction mechanism for
// objects in the scene graph.  Physical attributes do not include mass
// properties or state evolution, which are found in the dynamic attributes
// child class. This property set should be used for immobile objects in the
// scene.
//!vhtmodule: Core
class VH_CORE_DLL_EXPORT vhtPhysicalAttributes : public vhtNodeComponent
{
	public:
		enum Types 
		{
			humanHand,
			external,
			dynamic,
			nondynamic
		};
		//: Basic physical attribute types.

		vhtPhysicalAttributes(void);
		//: Defualt constructor.
		vhtPhysicalAttributes( const vhtPhysicalAttributes &p);
		//: Copy constructor.

		inline virtual bool isDynamic(void) { return false; }
		//: Is a dynamic object?
		// Dynamic objects have application driven behavior.  Non dynamic
		// objects have either no behavior or are driven by some external
		// source (ie. a hand ).

		inline virtual Types getType(void) { return nondynamic; }
		//: Return dynamic type.

		bool getIsCollidable(void) const { return m_isCollidable; }
		//: Get isCollidable flag.
		// Only objects with this flag set to true can be collided.

		void setIsCollidable( bool isCollidable ) 
		{
			m_isCollidable = isCollidable;
		}
		//: Set isCollidable flag.
		//!param: isCollidable - True if this node is collidable.
		// The default setting is false.

		vhtNode *getNode(void) const { return m_node; }
		//: Get the parent node.
		// Returns the node that these attributes refer to.

		void setNode(vhtNode *node ) { m_node = node; }
		//: Set the parent node.

////////////////////////////////////////////////////
	protected:

		bool                   m_isCollidable;
		//: Collidable flag.
		vhtNode               *m_node;
		//: Refering node.
};

#endif


