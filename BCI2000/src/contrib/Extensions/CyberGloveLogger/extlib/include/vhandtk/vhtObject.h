#ifndef __VHT_OBJECT_H_
#define __VHT_OBJECT_H_

/********************************************************************
 FILE: $Id: vhtObject.h,v 1.7 2002/08/28 23:16:20 jsilver Exp $
 AUTHOR: Ron Carmel
 DATE: Sep 1998
<header>

 DESCRIPTION:
  -
 HISTORY:
  -
</header>
           -- COPYRIGHT VIRTUAL TECHNOLOGIES, INC. 1998 --
********************************************************************/

//: The root of all VHT scene graph objects.
// Provide virtual base class functionality that all descendents must contain.
//!vhtmodule: Core

#if defined(BUILD_WIN32_VH_DLL)

#if defined __BUILD_VH_CORE_DLL__
#define VH_CORE_DLL_EXPORT __declspec(dllexport)
#else
#define VH_CORE_DLL_EXPORT __declspec(dllimport)
#endif

#else 

#define VH_CORE_DLL_EXPORT

#endif

class VH_CORE_DLL_EXPORT vhtObject {
	public:
		vhtObject() {}
		//: Constructor.
		vhtObject(vhtObject &obj) {}
		//: Copy constructor.
		virtual ~vhtObject() {}
		//: Destructor.
};


#endif