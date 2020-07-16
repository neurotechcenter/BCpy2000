/********************************************************************
 FILE: $Id: vhtGroup.h,v 1.12 2002/08/28 23:16:16 jsilver Exp $
 AUTHOR: Ron Carmel
 DATE: Aug 1998

 DESCRIPTION:
  vhtGroup is the fundamental non-terminal node type in the haptic
  scene graph.  This node and it's decendents control the grouping
  behaviour of objects in the scene.

 HISTORY:
  -

           -- COPYRIGHT VIRTUAL TECHNOLOGIES, INC. 1998 --
********************************************************************/

#ifndef __VHTGROUP_H
#define __VHTGROUP_H

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
#include <vhtArray.h>

//: A generic grouping scene graph node.
//  vhtGroup is the fundamental non-terminal node type in the haptic
//  scene graph.  This node and it's decendents control the grouping
//  behaviour of objects in the scene.
//!vhtmodule: Core
class VH_CORE_DLL_EXPORT vhtGroup: public vhtNode {
	public:
		vhtGroup(void);
		//: Default constructor.

		vhtGroup(vhtGroup &group);
		//: Copy constructor.
    
		virtual ~vhtGroup(void);
		//: Destructor.

		virtual inline vhtNodeType getType(void) 
		{ 
			return VHT_GROUP; 
		}
		//: Return node type info.

		inline bool isLeaf(void) 
		{ 
			return false; 
		}
    
		//: Is a leaf node.
		// Always returns false.
		virtual inline bool isSwitch(void) 
		{ 
			return false; 
		}
		//: Is a switch node.
		// By default, groups are not switching.

		virtual int addChild(vhtNode *child);
		//: Add a child node.
		//!param: child - Child node to add.

		bool setChild(vhtNode *child, int index);
		//: Set a particular child.
		//!param: child - Child node to insert.
		//!param: index - Index of child node.
		// Sets the index'th child of this to child. There is no gaurantee that
		// children before or after this index are defined.


	    inline int numChildren(void) 
		{ 
			return m_numChildren;
		}
		//: Number of children.
		// Returns the index of highest indexed child.

	    vhtNode *getChild(int index);
		//: Get child with index.
		//!param: index - Index of the child to get.
		// Get's the child at index location. If the index is outside the
		// range of children a NULL pointer will be returned.

	    void detachChild(int index);
		//: Detach child.
		//!param: index - Index of the child to detach.
		// Detaches the child at index from the group.  The child is also updated and dereferenced.
		// Note that if the child is only referenced by this group, the child will be deleted.

		void detachChild( vhtNode *child );
		//: Detach child.
		//!param: child - Child node to delete.
		// Detaches the child from the group.  The child is also updated. If the node pointer passed
		// is not a valid child, no action is taken.

		virtual void refresh(void);
		//: Refresh transforms/child properties.
		// Refresh the sub-graph below this group node. See individual refresh methods
		// for other node types for further information.

		virtual bool render(void);
		//: Render subgraph.
		// Renders the sub-graph to the current OpenGL context.  Requries that geometry nodes
		// have associated OpenGL render methods.

	private:
		// refresh bounds
		virtual void               refreshBoundingSphere(void);
		vhtBoundingSphere        **m_spheres;
    
		// children
		int                        m_numChildren;
		vhtArray                   m_children;
};

#endif



