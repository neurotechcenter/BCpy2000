
#ifndef __TOBII_SDK_CPP_LIBRARY_HPP__
#define __TOBII_SDK_CPP_LIBRARY_HPP__

#include <tobii/sdk/core.h>
#include <tobii/sdk/cpp/EyeTrackerException.hpp>

namespace tobii {

namespace sdk {

namespace cpp {

class Library 
{
public:
	static inline void init()
	{
		tobii_sdk_error_t* error = 0;
		tobii_sdk_init(&error);
		EyeTrackerException::throwOnError(error);
	}

private:
	// the constructor is private because this class should never be instantiated.
	Library()
	{
	}
};

} // namespace cpp

} // namespace sdk

} // namespace tobii

#endif // __TOBII_SDK_CPP_LIBRARY_HPP__
