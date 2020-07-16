/********************************************************************
 FILE: $Id: vhtSwitch.h,v 1.7 2002/08/28 23:16:24 jsilver Exp $
 AUTHOR: Ron Carmel
 DATE: Aug 1998

 DESCRIPTION:
  -
 HISTORY:
  -
           -- COPYRIGHT VIRTUAL TECHNOLOGIES, INC. 1998 --
********************************************************************/

#ifndef __VHTSWITCH_H
#define __VHTSWITCH_H

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

//: Subgraph switching group.
// This is a type of group node that forces a particular sub-graph to be
// active at any given time.  Only one child of this node may be an
// active part of the scene graph.  This functionality can be used to
// implement level of detail nodes, or to perform animated haptics. 
//!vhtmodule: Core
class vhtSwitch: public vhtGroup {
	public:
		VH_CORE_DLL_EXPORT vhtSwitch(void);
		//: Constructor.

		VH_CORE_DLL_EXPORT vhtSwitch(vhtSwitch &aSwitch);
		//: Copy constructor.
    
		VH_CORE_DLL_EXPORT virtual ~vhtSwitch(void);
		//: Destructor.
    
		VH_CORE_DLL_EXPORT virtual inline vhtNodeType getType(void) 
		{ 
			return VHT_SWITCH; 
		}
		//: Get the node type.
		// This node has type VHT_SWITCH.

		VH_CORE_DLL_EXPORT vhtNode *currentChild(void);
		//: Get the current active sub-graph.

		VH_CORE_DLL_EXPORT int getWhichChild(void);
		//: Get the index of the current active child.
    
		VH_CORE_DLL_EXPORT void setWhichChild(int child);
		//: Set the index of the current active child.
		//!param: child - Index of child to make active.

		VH_CORE_DLL_EXPORT inline void detachChild(int index) 
		{
			vhtGroup::detachChild(index);

			if(index==m_whichChild){
				m_whichChild = -1;
			}
		}

		//: Detach a sub-graph.

		VH_CORE_DLL_EXPORT inline bool isSwitch(void) 
		{ 
			return true; 
		}

		//: Boolean flag for switch type.
		VH_CORE_DLL_EXPORT virtual void refresh(void);
		//: Refresh the sub-graph.
		// Only refreshes the active child.

	    VH_CORE_DLL_EXPORT bool render(void);
		 //: Render the active child.

private:
    virtual void refreshBoundingSphere(void);

    int  m_whichChild;
};

#endif


