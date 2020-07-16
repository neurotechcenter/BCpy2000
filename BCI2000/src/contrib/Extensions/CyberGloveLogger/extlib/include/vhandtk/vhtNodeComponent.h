/********************************************************************
 FILE: $Id: vhtNodeComponent.h,v 1.6 2002/08/28 23:16:19 jsilver Exp $
 AUTHOR: Ron Carmel
 DATE: Aug 1998

 DESCRIPTION:
  -
 HISTORY:
  -
           -- COPYRIGHT VIRTUAL TECHNOLOGIES, INC. 1998 --
********************************************************************/

#ifndef __VHTNODECOMPONENT_H
#define __VHTNODECOMPONENT_H

#if defined(BUILD_WIN32_VH_DLL)

#if defined __BUILD_VH_CORE_DLL__
#define VH_CORE_DLL_EXPORT __declspec(dllexport)
#else
#define VH_CORE_DLL_EXPORT __declspec(dllimport)
#endif

#else 

#define VH_CORE_DLL_EXPORT

#endif

#include <vhtSceneGraphObject.h>

class vhtShape3D;

//: A node attribute virtual base class.
// Node components are properties of node objects.  Examples of node components
// include physical attributes and geometry.  
// This class is a virtual base class for all node components.
//!vhtmodule: Core
class VH_CORE_DLL_EXPORT vhtNodeComponent: public vhtSceneGraphObject {

    friend class vhtShape3D;

	public:

		vhtNodeComponent(void);
		//: Default constructor.

		vhtNodeComponent(vhtNodeComponent &nodeComponent);
		//: Copy constructor.
    
		virtual  ~vhtNodeComponent(void);
		//: Destructor.

};

#endif



