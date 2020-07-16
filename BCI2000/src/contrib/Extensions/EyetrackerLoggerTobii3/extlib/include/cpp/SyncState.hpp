
#ifndef __TOBII_SDK_CPP_TIME_SYNC_STATE_HPP__
#define __TOBII_SDK_CPP_TIME_SYNC_STATE_HPP__

#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <tobii/sdk/core.h>

namespace tobii {

namespace sdk {

namespace cpp {

struct SyncPoint {

	int64_t localMidpoint;
	int64_t remoteTime;
	int64_t roundtripTime;

	SyncPoint() :
		localMidpoint(0),
		remoteTime(0),
		roundtripTime(0)
	{
	}
};

class SyncState 
{
public:
	typedef boost::shared_ptr<SyncState> pointer_t;
	enum sync_state_flag_t {UNSYNCHRONIZED, STABILIZING, SYNCHRONIZED};
	
	virtual ~SyncState() {}
	
	sync_state_flag_t getSyncStateFlag()
	{
		return doGetSyncStateFlag();
	}
	
	const std::vector<SyncPoint>& getSyncPoints()
	{
		return doGetSyncPoints();
	}

private:
	virtual sync_state_flag_t doGetSyncStateFlag() = 0;
	virtual const std::vector<SyncPoint>& doGetSyncPoints() = 0;
};

class SyncStateImpl : public SyncState, boost::noncopyable
{
public:	
	SyncStateImpl(const tobii_sdk_sync_state_t* state)
	{
		// Clone sync state without taking ownership of
		// the state pointer
		tobii_sdk_error_t* error = 0;
		internalSyncState_.reset(
							tobii_sdk_sync_state_clone(state, &error),
							&tobii_sdk_sync_state_destroy);

		EyeTrackerException::throwOnError(error);
		updateSyncPoints();
	}

	SyncStateImpl(boost::shared_ptr<tobii_sdk_sync_state_t> state)
	{
		internalSyncState_ = state;
		updateSyncPoints();
	}

private:
	boost::shared_ptr<tobii_sdk_sync_state_t> internalSyncState_;
	std::vector<SyncPoint> SyncPoints_;

	sync_state_flag_t doGetSyncStateFlag()
	{
		tobii_sdk_error_t* error = 0;
		uint32_t state_flag = tobii_sdk_sync_state_get_sync_state_flag(internalSyncState_.get(), &error);
		EyeTrackerException::throwOnError(error);
		return static_cast<sync_state_flag_t>(state_flag);
	}

	const std::vector<SyncPoint>& doGetSyncPoints()
	{
		return SyncPoints_;
	}

	void updateSyncPoints()
	{
		tobii_sdk_error_t* error = 0;
		uint32_t num_points = tobii_sdk_sync_state_get_number_of_points_in_use(internalSyncState_.get(), &error);
		EyeTrackerException::throwOnError(error);

		for (uint32_t i = 0; i < num_points; ++i) {
			SyncPoint pt;
			tobii_sdk_sync_state_get_point_in_use(internalSyncState_.get(), i,
					&pt.localMidpoint, &pt.remoteTime, &pt.roundtripTime, &error);
			EyeTrackerException::throwOnError(error);
			SyncPoints_.push_back(pt);
		}
	}

};

} // namespace cpp

} // namespace sdk

} // namespace tobii

#endif  // __TOBII_SDK_CPP_TIME_SYNC_STATE_HPP__
