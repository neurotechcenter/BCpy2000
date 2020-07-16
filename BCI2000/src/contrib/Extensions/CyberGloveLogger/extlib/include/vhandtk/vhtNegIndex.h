#ifndef	_VHT_NEGINDEX_EXCEPTION_H_
#define _VHT_NEGINDEX_EXCEPTION_H_
/********************************************************************
 FILE: $Id: vhtNegIndex.h,v 1.5 2002/08/28 23:16:18 jsilver Exp $
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


//: The index supplied for an array access is negative.
// This exception is to be raised when the code is about to access an element in
//  an array and it realizes that the element index to use is negative.
//!vhtmodule: Device
class VH_DEVICE_DLL_EXPORT vhtNegIndexExcp : public vhtBaseException {
	public:
		/* Constructors. */
		vhtNegIndexExcp(void);
		//: A default constructor, for the compilers that require such a thing.
		vhtNegIndexExcp(char *aMessage);
		//: The preferred constructor.
		//!param: aMessage - A string to be returned to the caller.
		// A significant error message should be provided as the aMessage parameter.
};


#endif	    /* _VHT_NEGINDEX_EXCEPTION_H_ */
