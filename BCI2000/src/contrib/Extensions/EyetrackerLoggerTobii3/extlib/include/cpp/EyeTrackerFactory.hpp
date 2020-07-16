#ifndef __TOBII_SDK_CPP_FACTORY_INFO_HPP__
#define __TOBII_SDK_CPP_FACTORY_INFO_HPP__

#include <string>
#include <boost/shared_ptr.hpp>
#include <tobii/sdk/core.h>
#include <tobii/sdk/cpp/MainLoop.hpp>
#include <tobii/sdk/cpp/EyeTracker.hpp>
#include <tobii/sdk/cpp/SyncManager.hpp>

namespace tobii {

namespace sdk {

namespace cpp {

class EyeTrackerFactory 
{
public:
	typedef boost::shared_ptr<EyeTrackerFactory> pointer_t;

	virtual ~EyeTrackerFactory()
	{
	}

	std::string getRepresentation()
	{
		return doGetRepresentation();
	}

	EyeTracker::pointer_t createEyeTracker(MainLoop& mainLoop)
	{
		return doCreateEyeTracker(mainLoop);
	}

	SyncManager::pointer_t createSyncManager(Clock& clock, MainLoop& mainLoop)
	{
		return doCreateSyncManager(clock, mainLoop);
	}

private:
	virtual std::string doGetRepresentation() = 0;
	virtual EyeTracker::pointer_t doCreateEyeTracker(MainLoop& mainLoop) = 0;
	virtual SyncManager::pointer_t doCreateSyncManager(Clock& clock, MainLoop& mainLoop) = 0;
};

} // namespace cpp

} // namespace sdk

} // namespace tobii

#endif // __TOBII_SDK_CPP_FACTORY_INFO_HPP__
