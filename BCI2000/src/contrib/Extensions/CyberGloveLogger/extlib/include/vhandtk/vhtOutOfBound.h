#ifndef	_VHT_OUTOFBOUND_EXCEPTION_H_
#define _VHT_OUTOFBOUND_EXCEPTION_H_
/********************************************************************
 FILE: $Id: vhtOutOfBound.h,v 1.5 2002/08/28 23:16:22 jsilver Exp $
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


//: Array access with an index that is either too low or too high.
// This exception is to be raised when the code detects that an index used to access
// an array element is not within the range of accessible values in the array.
//!vhtmodule: Device
class VH_DEVICE_DLL_EXPORT vhtOutOfBoundExcp : public vhtBaseException {
public:
    /* Constructors. */
    vhtOutOfBoundExcp(void);
    //: A default constructor, for the compilers that require such a thing.
    vhtOutOfBoundExcp(char *aMessage);
    //: The preferred constructor.
    //!param: aMessage - A string to be returned to the caller.
    // A significant error message should be provided as the aMessage parameter.
};


#endif	    /* _VHT_OUTOFBOUND_EXCEPTION_H_ */
