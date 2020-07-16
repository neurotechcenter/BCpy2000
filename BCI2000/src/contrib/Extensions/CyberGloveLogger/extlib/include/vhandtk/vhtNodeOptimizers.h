#ifndef _VHT_NODE_OPTIMIZERS_
#define _VHT_NODE_OPTIMIZERS_


/********************************************************************
 FILE: $Id: vhtNodeOptimizers.h,v 1.4 2002/08/28 23:16:19 jsilver Exp $
 AUTHOR: Hugo DesRosiers.
 DATE: 1999/03/15.

 DESCRIPTION: Generic drawing context for the minimalist implementation
		of VRML support.

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

class vhtVrSFNode;

//: Optimization abstract class for VTI's minimal implementation of VRML.
// The VRML nodes are defined for flexibility, but not for speed.  The VHT
// adds optimizing instances that can be paired with certain VRML nodes
// so that rendering occurs much faster for a given context.
//!vhtmodule: Core
class VH_CORE_DLL_EXPORT vhtNodeOptimizers {

  public:
	virtual void draw(vhtVrSFNode *node)= 0;
	//: Draws the node as fast as possible.
	// This is the only method of the class: the current model for
	// optimization only specify the drawing entry point, all other
	// operations are at the discretion of the implementation.
};


#endif		/* _VHT_NODE_OPTIMIZERS_ */



