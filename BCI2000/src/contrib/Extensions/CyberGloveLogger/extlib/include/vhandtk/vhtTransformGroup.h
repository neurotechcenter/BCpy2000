/********************************************************************
 FILE: $Id: vhtTransformGroup.h,v 1.9 2002/08/28 23:16:26 jsilver Exp $
 AUTHOR: Ron Carmel
 DATE: Aug 1998

 DESCRIPTION:
  vhtTransform group is a grouping node that applies a homogeneous
  transformation to its subgraph.  The transformation can be set
  either as a world relative matrix or as a parent relative matrix.
  Parent relative matricies are efficient for tree structured 
  objects.

 HISTORY:
  -
           -- COPYRIGHT VIRTUAL TECHNOLOGIES, INC. 1998 --
********************************************************************/

#ifndef __VHTTRANSFORMGROUP_H
#define __VHTTRANSFORMGROUP_H

#if defined(BUILD_WIN32_VH_DLL)

#if defined __BUILD_VH_CORE_DLL__
#define VH_CORE_DLL_EXPORT __declspec(dllexport)
#else
#define VH_CORE_DLL_EXPORT __declspec(dllimport)
#endif

#else 

#define VH_CORE_DLL_EXPORT

#endif

#include <vhtGroup.h>
#include <vhtTransform3D.h>

//: A group node with attached transformation.
// This type of group node applies a homogeneous vhtTransform3D to all 
// it's children.  This transform can be accessed using the get/setTransform
// method.  The transform is in local to parent format.
//!vhtmodule: Core
class VH_CORE_DLL_EXPORT vhtTransformGroup: public vhtGroup 
{
	public:
		vhtTransformGroup(void);
		//: Construct a default transform group.
		// The default transformation is the identity. 

		vhtTransformGroup(vhtTransform3D &t1);
		//: Copy constructor.
				 
		virtual ~vhtTransformGroup(void);
		//: Destructor.

		virtual inline vhtNodeType getType(void) 
		{ 
			return VHT_TRANSFORMGROUP; 
		}
		//: Get the node type.
		// This returns VHT_TRANSFORMGROUP.
    
		const inline vhtTransform3D &getTransform(void)   
		{ 
			return m_transform; 
		}

		//: Get the associated transformtion.
		// Returns the vhtTransform3D that this node applies to
		// all it's children.

		bool setTransform(const vhtTransform3D &t1);
		//: Set the associated transformtion.
		//!param: t1 - Transform to set.
		// This transform will be applied to all children on the next
		// refresh. The transform should represent the child to parent transformation.

		const inline vhtTransform3D &getLM(void) 
		{ 
			return m_LM; 
		}
		//: Get the world transform.
		// Returns the global transformation from world co-ordinates to
		// the local frame.

		bool setLM(const vhtTransform3D &LM);
		//: Set the world transform.
		//!param: LM - Local to world transformation.
		// Sets the global transform from local to world.  Note that
		// only one call to either setLM or setTransform is permitted
		// in each refresh cycle. Trying to set both will result in a
		// vhtBadLogicExcp being thrown.

	    virtual bool render(void);
		//: Render to OpenGL context.

	protected:
		virtual void setChildTransformModified( void );
		//: Sets the transform modified flag for all children.

	private:
		virtual void                 refreshTransforms(void);
		//: Refresh all transforms.

		virtual void                 thread(void);
		//: Thread into transformGroup list.
		virtual void                 unthread(void);
		//: Unthread from transformGroup list.

		vhtTransform3D               m_LM;
		//: Global xform
		bool                         m_LMModified;
		//: Xform modification flag.

		vhtTransform3D               m_transform;
		//: Local transform.
		bool                         m_transformModified;
		//: Xform modification flag.

		vhtTransformGroup           *m_nextTransformGroup;
		//: Next xform group in xform thread.
		vhtTransformGroup           *m_prevTransformGroup;
		//: Prev xform group in xform thread.
};

#endif


