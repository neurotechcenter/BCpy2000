#ifndef __TOBII_SDK_CPP_MAINLOOP_HPP__
#define __TOBII_SDK_CPP_MAINLOOP_HPP__

#include <boost/shared_ptr.hpp>
#include <tobii/sdk/core.h>
#include <tobii/sdk/cpp/EyeTrackerException.hpp>

namespace tobii {

namespace sdk {

namespace cpp {

class MainLoop
{
public:
	MainLoop()
	{
		tobii_sdk_error_t* error = 0;
		handle_.reset(tobii_sdk_mainloop_create(&error), tobii_sdk_mainloop_destroy);
		EyeTrackerException::throwOnError(error);
	}

	MainLoop(const MainLoop& other)
		: handle_(other.handle_)
	{
	}

	MainLoop& operator = (const MainLoop& other)
	{
		handle_ = other.handle_;
		return *this;
	}

	virtual ~MainLoop() 
	{
	}

	void run()
	{
		tobii_sdk_error_t* error = 0;
		tobii_sdk_mainloop_run(handle_.get(), &error);
		EyeTrackerException::throwOnError(error);
	}

	void quit()
	{
		tobii_sdk_error_t* error = 0;
		tobii_sdk_mainloop_quit(handle_.get(), &error);
		EyeTrackerException::throwOnError(error);
	}

	tobii_sdk_mainloop_t* getHandle()
	{
		return handle_.get();
	}

private:
	boost::shared_ptr<tobii_sdk_mainloop_t> handle_;
};

} // namespace cpp

} // namespace sdk

} // namespace tobii

#endif // __TOBII_SDK_CPP_MAINLOOP_HPP__
