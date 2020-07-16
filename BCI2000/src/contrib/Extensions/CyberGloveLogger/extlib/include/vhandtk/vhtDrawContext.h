#ifndef _VHT_VR_CONTEXT_H_
#define _VHT_VR_CONTEXT_H_


/********************************************************************
 FILE: $Id: vhtDrawContext.h,v 1.6 2002/08/28 23:16:13 jsilver Exp $
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


class vhtTransform3D;
class vhtVrGroup;
class vhtVrTransform;
class vhtVrShape;
class vhtVrSFNode;


//: VRML drawing context.
//  Generic drawing context for the minimalist implementation
//  of VRML support.
//!vhtmodule: Core
class VH_CORE_DLL_EXPORT vhtVrContext {

  public:
	  virtual void drawShape(vhtVrShape *aNode)= 0;
	  //: Draw given VRML shape.
	  // Pure virtual method to draw a given VRML shape.

	  virtual void setLocalTransform(vhtVrTransform *aTransform)= 0;
	  //: Set (push) the local model transform.
	  // Pushes aTransform onto the current modelling stack.
	  
	  virtual void unsetLocalTransform(vhtVrTransform *aTransform)= 0;
	  //: Unset (pop) the local model transform.
	  // Pops a transform off the current modelling stack.
};

#endif		/* _VHT_VR_CONTEXT_H_ */



