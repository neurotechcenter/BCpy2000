
#ifndef __TOBII_SDK_CPP_EYETRACKER_INFO_HPP__
#define __TOBII_SDK_CPP_EYETRACKER_INFO_HPP__

#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <tobii/sdk/core.h>
#include <tobii/sdk/cpp/EyeTrackerException.hpp>
#include <tobii/sdk/cpp/EyeTrackerFactory.hpp>
#include <tobii/sdk/cpp/UpgradePackage.hpp>
#include <tobii/sdk/cpp/ProgressReporter.hpp>

namespace tobii {

namespace sdk {

namespace cpp {

class EyeTrackerInfo
{
public:
	typedef boost::shared_ptr<EyeTrackerInfo> pointer_t;

	virtual ~EyeTrackerInfo()
	{
	}

	std::string getProductId()
	{
		return doGetProductId();
	}

	std::string getGivenName()
	{
		return doGetValue("given-name");
	}

	std::string getModel()
	{
		return doGetValue("model");
	}

	std::string getGeneration()
	{
		return doGetValue("generation");
	}

	std::string getVersion()
	{
		return doGetValue("firmware-version");
	}

	std::string getStatus()
	{
		return doGetValue("status");
	}

	std::string getPlatformType()
	{
		return doGetValue("platformtype");
	}

	EyeTrackerFactory::pointer_t getEyeTrackerFactory()
	{
		return doGetEyeTrackerFactory();
	}

	ProgressReporter::pointer_t beginUpgrade(UpgradePackage& upgradePackage, MainLoop& mainLoop)
	{
		return doBeginUpgrade(upgradePackage, mainLoop);
	}

	bool isCompatible(UpgradePackage& upgradePackage, MainLoop& mainLoop, uint32_t& errorCode)
	{
		return checkIsCompatible(upgradePackage, mainLoop, errorCode);
	}

private:
	virtual std::string doGetProductId() = 0;
	virtual std::string doGetValue(const std::string& key) = 0;
	virtual EyeTrackerFactory::pointer_t doGetEyeTrackerFactory() = 0;
	virtual ProgressReporter::pointer_t doBeginUpgrade(UpgradePackage& upgradePackage, MainLoop& mainLoop) = 0;
	virtual bool checkIsCompatible(UpgradePackage& upgradePackage, MainLoop& mainLoop, uint32_t& errorCode) = 0;
};

} // namespace cpp

} // namespace sdk

} // namespace tobii

#endif // __TOBII_SDK_CPP_EYETRACKER_INFO_HPP__
