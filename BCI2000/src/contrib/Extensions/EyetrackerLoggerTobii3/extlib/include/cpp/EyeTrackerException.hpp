
#ifndef __TOBII_SDK_CPP_EYETRACKEREXCEPTION_HPP__
#define __TOBII_SDK_CPP_EYETRACKEREXCEPTION_HPP__

#include <tobii/sdk/core.h>

#include <stdexcept>

namespace tobii {

namespace sdk {

namespace cpp {

typedef uint32_t error_code_t;

class EyeTrackerException : public std::runtime_error {

public:
	EyeTrackerException(error_code_t code, const std::string& message) :
		std::runtime_error(message),
		errorCode_(code)
	{
	}

	error_code_t getErrorCode() const
	{
		return errorCode_;
	}

	static void throwOnError(tobii_sdk_error_t* error)
	{
		if(!error)
			return;

		error_code_t code = tobii_sdk_error_get_code(error);
		uint32_t error_is_failure = tobii_sdk_error_is_failure(error);



		if(error_is_failure)
		{
			const char* rawMessage = tobii_sdk_error_get_message(error);
			std::string message(rawMessage);
			tobii_sdk_free_string(rawMessage);
			tobii_sdk_error_destroy(error);

			throw EyeTrackerException(code, message);
		}
		tobii_sdk_error_destroy(error);
	}

private:
	error_code_t errorCode_;
};


} // namespace cpp

} // namespace sdk

} // namespace tobii

#endif // __TOBII_SDK_CPP_EYETRACKEREXCEPTION_HPP__
