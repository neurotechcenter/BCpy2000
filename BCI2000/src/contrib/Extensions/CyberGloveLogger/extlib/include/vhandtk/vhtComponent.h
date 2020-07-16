#ifndef __VHTCOMPONENT_H
#define __VHTCOMPONENT_H

/********************************************************************
 FILE: $Id: vhtComponent.h,v 1.11 2002/08/28 23:16:09 jsilver Exp $
 AUTHOR: Chris Ullrich
 DATE: 1999/03/02.

 DESCRIPTION: Rigid body abstraction for VHT. This class is a higher
  level grouping of shape3d objects.  Only objects of type component
  can be grasped using the grasp state mechanism.

 HISTORY:
 
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

#include <vhtGraspStateManager.h>
#include <vhtTransformGroup.h>

class vhtShape3D;
class vhtConstraint;

//: A non-colliding sub-graph.
// Component separators are a type of group node whose children
// are pieces of the same additive object.  Collision detection
// is not perfomed on shapes that have a common component
// separator between them and the root node.
// Components are meant to represent rigid geometric objects
// made from several different vhtShape3D nodes.
// Components are also the only types of objects that can
// be grasped by vhtHumanHand objects.  Each component has
// an associated state machine (vhtGraspStateManager) that manages 
// object grasping by a human hand.
//!vhtmodule: Core
class VH_CORE_DLL_EXPORT vhtComponent: public vhtTransformGroup
{
	public:
		vhtComponent(void);
		//: Construct a default component node.
		// By default, the associated vhtTransform3D
		// is the identity transform.

		virtual ~vhtComponent(void);
		//: Destruct.

		virtual inline vhtNodeType   getType(void) { return VHT_COMPONENT; }
		//: Get the node type.
		// vhtComponents have node type VHT_COMPONENT.

		inline vhtGraspStateManager *getGraspManager(void) { return &m_graspManager; }
		//: Return the associated grasping state machine.
		// The grasp state manager is used to constrain this
		// component to a vhtHumanHand.  See the vhtGraspStateManager
		// documentation for more details.

		virtual int addChild( vhtNode *object );
		//: Add a node to the component.
		//!param: object - Object to add as child.
		// Adds object as a child of this and sets it's component
		// to this.
		
		void setChildrenComponent( vhtNode *node = NULL );
		//: Sets all child nodes to have this component.
		//!param: node - Subgraph to control ownership of.
		// The node argument in this method can be used to control non-local
		// subgraphs.

		virtual void setIsRigid( bool isRigid );
		//: Sets this component to be rigid.
		//!param: isRigid - Set to true for a rigid component.
		// Rigid components are assumed to have no independent updates on any children's LM or
		// Transforms.  For this to be true, the object must be effectively fixed in the
		// scene. This is a performance optimization.

		virtual bool getIsRigid( void );
		//: Get the rigidity of this component.
		
		virtual void refresh(void );
		//: Refresh
	
	protected:
		vhtGraspStateManager          m_graspManager;
		bool                          m_isRigid;
};

#endif




