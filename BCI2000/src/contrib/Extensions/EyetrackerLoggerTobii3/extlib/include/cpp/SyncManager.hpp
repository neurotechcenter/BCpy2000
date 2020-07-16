
#ifndef __TOBII_SDK_CPP_TIME_SYNCMANAGER_HPP__
#define __TOBII_SDK_CPP_TIME_SYNCMANAGER_HPP__

#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <boost/signals2.hpp>
#include <tobii/sdk/core.h>
#include <tobii/sdk/cpp/MainLoop.hpp>
#include <tobii/sdk/cpp/Clock.hpp>
#include <tobii/sdk/cpp/SyncState.hpp>
#include <tobii/sdk/cpp/EyeTrackerException.hpp>

namespace tobii {

namespace sdk {

namespace cpp {

class SyncManager {
public:
	typedef boost::shared_ptr<SyncManager> pointer_t;
	typedef boost::signals2::signal<void (SyncState::pointer_t)> sync_state_changed_event_t;
	typedef boost::signals2::connection connection_t;

	virtual ~SyncManager() {}

	int64_t localToRemote(int64_t localTime)
	{
		return doLocalToRemote(localTime);
	}

	int64_t remoteToLocal(int64_t remoteTime)
	{
		return doRemoteToLocal(remoteTime);
	}

	SyncState::pointer_t getSyncState()
	{
		return doGetSyncState();
	}

	connection_t addSyncStateChangedListener(const sync_state_changed_event_t::slot_type& listener)
	{
		return doAddSyncStateChangedListener(listener);
	}

private:
	virtual int64_t doLocalToRemote(int64_t localTime) = 0;
	virtual int64_t doRemoteToLocal(int64_t remoteTime) = 0;
	virtual SyncState::pointer_t doGetSyncState() = 0;
	virtual connection_t doAddSyncStateChangedListener(const sync_state_changed_event_t::slot_type& listener) = 0;
};

class SyncManagerImpl : public SyncManager, boost::noncopyable
{
public:
	SyncManagerImpl(tobii_sdk_factory_info_t* factoryInfo, Clock& clock, MainLoop& mainLoop)
	{
		tobii_sdk_error_t* error = 0;
		handle_.reset(
				tobii_sdk_sync_manager_create(
							clock.getHandle(),
							factoryInfo,
							mainLoop.getHandle(),
							&SyncManagerImpl::SyncManagerErrorHandlerTrampoline,
							this,
							&SyncStateChangedTrampoline,
							this,
							&error
						),
				&tobii_sdk_sync_manager_destroy);
		EyeTrackerException::throwOnError(error);
	}

private:
	boost::shared_ptr<tobii_sdk_sync_manager_t> handle_;
	sync_state_changed_event_t syncStateChangedSig_;

	int64_t doLocalToRemote(int64_t localTime)
	{
		tobii_sdk_error_t* error = 0;
		int64_t remote = tobii_sdk_sync_manager_convert_from_local_to_remote(handle_.get(), localTime, &error);
		EyeTrackerException::throwOnError(error);
		return remote;
	}

	int64_t doRemoteToLocal(int64_t remoteTime)
	{
		tobii_sdk_error_t* error = 0;
		int64_t local = tobii_sdk_sync_manager_convert_from_remote_to_local(handle_.get(), remoteTime, &error);
		EyeTrackerException::throwOnError(error);
		return local;
	}

	SyncState::pointer_t doGetSyncState()
	{
		tobii_sdk_error_t* error = 0;
		boost::shared_ptr<tobii_sdk_sync_state_t> current_state(
				tobii_sdk_sync_manager_get_sync_state(handle_.get(), &error),
				tobii_sdk_sync_state_destroy);
		EyeTrackerException::throwOnError(error);

		SyncState::pointer_t state(new SyncStateImpl(current_state));
		return state;
	}

	connection_t doAddSyncStateChangedListener(const sync_state_changed_event_t::slot_type& listener)
	{
		return syncStateChangedSig_.connect(listener);
	}

	void handle_sync_state_changed(const tobii_sdk_sync_state_t* state)
	{
		SyncState::pointer_t wrapped_state(new SyncStateImpl(state));
		syncStateChangedSig_(wrapped_state);
	}

	static void SyncStateChangedTrampoline(const tobii_sdk_sync_state_t* state, void* userData)
	{
		SyncManagerImpl* self = (SyncManagerImpl*)userData;
		self->handle_sync_state_changed(state);
	}

	static void SyncManagerErrorHandlerTrampoline(tobii_sdk_error_code_t /*error*/, void* /*userData*/)
	{
		// This handler is empty since the error event
		// never fires in the current implementation
	}
};

} // namespace cpp

} // namespace sdk

} // namespace tobii

#endif  // __TOBII_SDK_CPP_TIME_SYNCMANAGER_HPP__
