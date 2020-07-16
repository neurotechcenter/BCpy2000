#ifndef	_VHT_UNIMPL_EXCEPTION_H_
#define _VHT_UNIMPL_EXCEPTION_H_
/********************************************************************
 FILE: $Id: vhtUnimpl.h,v 1.5 2002/08/28 23:16:27 jsilver Exp $
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


//: An unimplemented method was invoked.
// This exception is to be raised by methods which are defined but not yet
//  implemented.
//!vhtmodule: Device
class VH_DEVICE_DLL_EXPORT vhtUnimplExcp : public vhtBaseException {

	public:
		/* Constructors. */
		vhtUnimplExcp(void);
		//: A default constructor, for the compilers that require such a thing.
		vhtUnimplExcp(char *aMessage);
		//: The preferred constructor.
		//!param: aMessage - A string to be returned to the caller.
		// A significant error message should be provided as the aMessage parameter.
};


#endif	    /* _VHT_UNIMPL_EXCEPTION_H_ */
