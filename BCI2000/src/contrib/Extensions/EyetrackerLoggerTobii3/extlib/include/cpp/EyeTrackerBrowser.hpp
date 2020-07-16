#ifndef __TOBII_SDK_CPP_EYETRACKER_BROWSER_HPP__
#define __TOBII_SDK_CPP_EYETRACKER_BROWSER_HPP__

#include <boost/shared_ptr.hpp>
#include <boost/signals2.hpp>
#include <tobii/sdk/cpp/EyeTrackerInfo.hpp>

namespace tobii {

namespace sdk {

namespace cpp {

class EyeTrackerBrowser
{
public:
	enum event_type_t {TRACKER_FOUND, TRACKER_UPDATED, TRACKER_REMOVED};

	typedef boost::shared_ptr<EyeTrackerBrowser> pointer_t;
	typedef boost::signals2::signal<void (event_type_t, EyeTrackerInfo::pointer_t)> browser_event_t;
	typedef boost::signals2::connection connection_t;

	virtual ~EyeTrackerBrowser() 
	{
	}

	void start()
	{
		doStart();
	}

	void stop()
	{
		doStop();
	}

	connection_t addEventListener(const browser_event_t::slot_type& listener)
	{
		return doAddEventListener(listener);
	}

private:
	virtual void doStart() = 0;
	virtual void doStop() = 0;
	virtual connection_t doAddEventListener(const browser_event_t::slot_type& listener) = 0;
};

} // namespace cpp

} // namespace sdk

} // namespace tobii

#endif // __TOBII_SDK_CPP_EYETRACKER_BROWSER_HPP__
