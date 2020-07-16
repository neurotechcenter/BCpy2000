#ifndef	_VHT_NULLPTR_EXCEPTION_H_
#define _VHT_NULLPTR_EXCEPTION_H_
/********************************************************************
 FILE: $Id: vhtNullPtr.h,v 1.5 2002/08/28 23:16:20 jsilver Exp $
 AUTHOR: Hugo DesRosiers
 DATE: Sept 1999.

           -- COPYRIGHT VIRTUAL TECHNOLOGIES, INC. 1998 - 1999 --
********************************************************************/

#if defined(BUILD_WIN32_VH_DLL)

#if defined __BUILD_VH_DEVICE_DLL__
#define VH_DEVICE_DLL_EXPORT __declspec(dllexport)
#else
#define VH_DEVICE_DLL_EXPORT __declspec(dllimport)
#endif

#else

#define VH_DEVICE_DLL_EXPORT

#endif

#include <vhtBaseException.h>

//: The dereference of a pointer is leading to nowhere.
// This exception is to be raised when a pointer is to be dereferenced into an object
//  and the pointer is found to be containing 0 as the memory address.
//!vhtmodule: Device
class VH_DEVICE_DLL_EXPORT vhtNullPtrExcp : public vhtBaseException {
	public:
		/* Constructors. */
		vhtNullPtrExcp(void);
		//: A default constructor, for the compilers that require such a thing.
		vhtNullPtrExcp(char *aMessage);
		//: The preferred constructor.
		//!param: aMessage - A string to be returned to the caller.
		// A significant error message should be provided as the aMessage parameter.
};


#endif	    /* _VHT_NULLPTR_EXCEPTION_H_ */
