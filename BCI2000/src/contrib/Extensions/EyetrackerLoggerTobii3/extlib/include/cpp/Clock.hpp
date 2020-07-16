
#ifndef __TOBII_SDK_CPP_TIME_CLOCK_HPP__
#define __TOBII_SDK_CPP_TIME_CLOCK_HPP__

#include <boost/shared_ptr.hpp>
#include <tobii/sdk/core.h>
#include <tobii/sdk/cpp/EyeTrackerException.hpp>

namespace tobii {

namespace sdk {

namespace cpp {

class Clock 
{
public:
	Clock()
	{
		tobii_sdk_error_t* error = 0;
		handle_.reset(tobii_sdk_clock_get_default(&error), &tobii_sdk_clock_destroy);
		EyeTrackerException::throwOnError(error);
	}

	Clock(const Clock& other)
		: handle_(other.handle_)
	{
	}

	Clock& operator = (const Clock& other)
	{
		handle_ = other.handle_;
		return *this;
	}

	virtual ~Clock()
	{
	}
	
	int64_t getTime()
	{
		tobii_sdk_error_t* error = 0;
		int64_t time = tobii_sdk_clock_get_time(handle_.get(), &error);
		EyeTrackerException::throwOnError(error);
		return time;
	}
	
	int64_t getResolution()
	{
		tobii_sdk_error_t* error = 0;
		int64_t resolution = tobii_sdk_clock_get_resolution(handle_.get(), &error);
		EyeTrackerException::throwOnError(error);
		return resolution;
	}
	
	tobii_sdk_clock_t* getHandle()
	{
		return handle_.get();
	}

private:
	boost::shared_ptr<tobii_sdk_clock_t> handle_;
};

} // namespace cpp

} // namespace sdk

} // namespace tobii

#endif  // __TOBII_SDK_CPP_TIME_CLOCK_HPP__
