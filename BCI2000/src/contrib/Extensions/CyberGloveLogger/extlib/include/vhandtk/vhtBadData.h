#ifndef VHT_BAD_DATA_EXCP_H
#define VHT_BAD_DATA_EXCP_H
/********************************************************************
 FILE: $Id: vhtBadData.h,v 1.6 2002/08/28 23:16:06 jsilver Exp $
 AUTHOR: Chris Ullrich.
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

//: Unsupported or unknown data type encountered.
// This exception is to be raised when an unknown data type, a data
// structure with unknown or unsupported formats during processing.
//!vhtmodule: Device
class VH_DEVICE_DLL_EXPORT vhtBadData : public vhtBaseException
{
 public:
    vhtBadData( void );
    //: Null constructor.
    vhtBadData( const char *aMessage );
    //: Preferred constructor.
    //!param: aMessage - An informative message string.
};

#endif
