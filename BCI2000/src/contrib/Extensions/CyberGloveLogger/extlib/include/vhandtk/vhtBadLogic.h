//
// (C) VIRTUAL TECHNOLOGIES, INC. 1998-2001
//

#ifndef	_VHT_BADLOGIC_EXCEPTION_H_
#define _VHT_BADLOGIC_EXCEPTION_H_

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

/**
 * Internal error resulting from an incoherent and unrecoverable logical
 * state.  This exception is to be raised when the code detects an incoherent
 * situation and that there is no way to recover gracefully from that state.
 * A catch sequence should try to reset the environment that caused the
 * exception.
 */
class VH_DEVICE_DLL_EXPORT vhtBadLogicExcp : public vhtBaseException
{
public:
	/** A default constructor, for the compilers that require such a thing. */
	vhtBadLogicExcp(void);

	/** The preferred constructor.
	  @param aMessage Error message describing the cause.
	  A significant error message should be provided as the aMessage parameter. */
	vhtBadLogicExcp(char *aMessage);
};

#endif	    /* _VHT_BADLOGIC_EXCEPTION_H_ */
