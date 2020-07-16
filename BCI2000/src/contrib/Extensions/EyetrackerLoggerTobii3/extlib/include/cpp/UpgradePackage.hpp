

#ifndef __TOBII_SDK_CPP_PACKAGE_HPP__
#define __TOBII_SDK_CPP_PACKAGE_HPP__

#include <boost/shared_ptr.hpp>
#include <tobii/sdk/core.h>

namespace tobii {

namespace sdk {

namespace cpp {

class UpgradePackage 
{
public:
	UpgradePackage(const uint8_t* packageData, uint32_t packageSize)
	{
		tobii_sdk_error_t* error = 0;
		handle_.reset(tobii_sdk_upgrade_package_parse(packageData, packageSize, 1, &error),
				&tobii_sdk_upgrade_package_destroy);
		EyeTrackerException::throwOnError(error);
	}

	UpgradePackage(const UpgradePackage& other)
		: handle_(other.handle_)
	{
	}

	UpgradePackage& operator = (const UpgradePackage& other)
	{
		handle_ = other.handle_;
		return *this;
	}

	virtual ~UpgradePackage()
	{
	}

	tobii_sdk_upgrade_package_t* getHandle()
	{
		return handle_.get();
	}

private:
	boost::shared_ptr<tobii_sdk_upgrade_package_t> handle_;
};

} // namespace cpp

} // namespace sdk

} // namespace tobii

#endif  // __TOBII_SDK_CPP_PACKAGE_HPP__
