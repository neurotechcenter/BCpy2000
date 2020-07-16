#ifndef __TOBII_SDK_CPP_ASYNCRESULT_HPP__
#define __TOBII_SDK_CPP_ASYNCRESULT_HPP__

#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>
#include <boost/shared_ptr.hpp>

#include <tobii/sdk/core.h>
#include <tobii/sdk/cpp/EyeTrackerException.hpp>

namespace tobii {

namespace sdk {

namespace cpp {

template <class T>
class AsyncResult {

public:
	typedef boost::shared_ptr<AsyncResult<T> > pointer_t;

	AsyncResult() :
		resultSet_(false)
	{

	}

	static pointer_t create()
	{
		pointer_t ptr(new AsyncResult<T>());
		return ptr;
	}

	void setResult(T result)
	{
		boost::unique_lock<boost::mutex> lock(mutex_);
		if(!resultSet_)
		{
			result_ = result;
			errorCode_ = 0;
			resultSet_ = true;
			resultCondition_.notify_all();
		}
	}

	void setError(uint32_t error)
	{
		boost::unique_lock<boost::mutex> lock(mutex_);

		if(!resultSet_)
		{
			errorCode_ = error;
			resultSet_ = true;
			resultCondition_.notify_all();
		}
	}

	void waitForResult()
	{
		boost::unique_lock<boost::mutex> lock(mutex_);
		boost::system_time const timeout = boost::get_system_time()+ boost::posix_time::milliseconds(10000);
		while(!resultSet_)
		{
			if(!resultCondition_.timed_wait(lock, timeout))
			{
				throw EyeTrackerException(1, "waitForResult timeout!");
			}
		}
	}

	void throwIfError()
	{
		if(errorCode_)
		{
			throw EyeTrackerException(errorCode_, "Operation failed.");
		}
	}

	T getResult()
	{
		return result_;
	}

	uint32_t getErrorCode()
	{
		return errorCode_;
	}

private:
	T result_;
	uint32_t errorCode_;
	bool resultSet_;
	boost::mutex mutex_;
	boost::condition_variable resultCondition_;
};


} // namespace cpp

} // namespace sdk

} // namespace tobii

#endif // __TOBII_SDK_CPP_ASYNCRESULT_HPP__
