//
// (C) VIRTUAL TECHNOLOGIES, INC. 1998-2001
//

#ifndef	_VHT_BASEEXCEPTION_H_
#define _VHT_BASEEXCEPTION_H_


#if defined(BUILD_WIN32_VH_DLL)

#if defined __BUILD_VH_DEVICE_DLL__
#define VH_DEVICE_DLL_EXPORT __declspec(dllexport)
#else
#define VH_DEVICE_DLL_EXPORT __declspec(dllimport)
#endif

#else

#define VH_DEVICE_DLL_EXPORT

#endif

/**
 * The base class for error management through exception throwing.
 * This class defines the generic behavior of exceptions used in the VHT.
 * It is not intended to be used by itself, and it should be considered as
 * an abstract base class.  Error codes are not used in the VHT, instead,
 * methods may throw exceptions derived from this class (only).  Methods
 * which throw exceptions are documented in the reference manual.
 */
class VH_DEVICE_DLL_EXPORT vhtBaseException
{
  protected:
	char *message;
	//: An error message detailing the context of the exception.

  public:
	/** A default constructor, for the compilers that require such a thing. */
	vhtBaseException(void);

	/** The preferred constructor.
	  @param aMessage Error message describing the cause.
	  A significant error message should be provided as the aMessage parameter. */
	vhtBaseException(const char *aMessage);

	/** The content of the instance variable message will also be deleted when
	 * the destructor is invoked.
	 */
	virtual ~vhtBaseException(void);

	/** Data accessor: Returns a pointer to the message detailing the exception. */
	virtual const char *getMessage(void);
};

#endif	/* _VHT_BASEEXCEPTION_H_ */

