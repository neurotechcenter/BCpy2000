
#ifndef __TOBII_SDK_CPP_PROGRESS_REPORTER_HPP__
#define __TOBII_SDK_CPP_PROGRESS_REPORTER_HPP__

#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <boost/signals2.hpp>
#include <tobii/sdk/core.h>
#include <tobii/sdk/cpp/EyeTrackerException.hpp>

namespace tobii {

namespace sdk {

namespace cpp {

class ProgressReporter
{
public:
	struct Progress
	{
		uint32_t currentStep;
		uint32_t numberOfSteps;
		uint32_t percent;

		Progress() :
			currentStep(0),
			numberOfSteps(0),
			percent(0)
		{
		}
	};

	typedef boost::shared_ptr<ProgressReporter> pointer_t;
	typedef boost::signals2::signal<void (uint32_t)> upgrade_completed_event_t;
	typedef boost::signals2::signal<void (Progress)> upgrade_progress_event_t;
	typedef boost::signals2::signal<void (bool)> can_cancel_changed_event_t;
	typedef boost::signals2::connection connection_t;

	virtual ~ProgressReporter()
	{
	}

	bool canCancel()
	{
		return checkCanCancel();
	}

	bool cancel()
	{
		return doCancel();
	}

	Progress getProgress()
	{
		return doGetProgress();
	}

	connection_t addUpgradeCompletedListener(const upgrade_completed_event_t::slot_type& listener)
	{
		return doAddUpgradeCompletedListener(listener);
	}

	connection_t addProgressChangedListener(const upgrade_progress_event_t::slot_type& listener)
	{
		return doAddProgressChangedListener(listener);
	}

	connection_t addCanCancelChangedListener(const can_cancel_changed_event_t::slot_type& listener)
	{
		return doAddCanCancelChangedListener(listener);
	}

private:
	virtual bool checkCanCancel() = 0;
	virtual bool doCancel() = 0;
	virtual Progress doGetProgress() = 0;
	virtual connection_t doAddUpgradeCompletedListener(const upgrade_completed_event_t::slot_type& listener) = 0;
	virtual connection_t doAddProgressChangedListener(const upgrade_progress_event_t::slot_type& listener) = 0;
	virtual connection_t doAddCanCancelChangedListener(const can_cancel_changed_event_t::slot_type& listener) = 0;
};

class ProgressReporterImpl : public ProgressReporter, boost::noncopyable
{
public:
	ProgressReporterImpl()
	{
		tobii_sdk_error_t* error = 0;
		handle_.reset(
			tobii_sdk_upgrade_progress_reporter_create(
				&completionHandlerTrampoline,
				&progressHandlerTrampoline,
				&canCancelChangedTrampoline,
				this,
				&error),
			&tobii_sdk_upgrade_progress_reporter_destroy);
		EyeTrackerException::throwOnError(error);
	}

	tobii_sdk_upgrade_progress_reporter_t* getHandle()
	{
		return handle_.get();
	}

private:
	boost::shared_ptr<tobii_sdk_upgrade_progress_reporter_t> handle_;
	upgrade_completed_event_t upgradeCompletedSig_;
	upgrade_progress_event_t upgradeProgressSig_;
	can_cancel_changed_event_t canCancelChangedSig_;

	bool checkCanCancel()
	{
		tobii_sdk_error_t* error = 0;
		uint32_t can_cancel = tobii_sdk_upgrade_progress_get_can_cancel(handle_.get(), &error);
		EyeTrackerException::throwOnError(error);
		return can_cancel > 0;
	}

	bool doCancel()
	{
		tobii_sdk_error_t* error = 0;
		uint32_t upgrade_cancelled = tobii_sdk_upgrade_progress_cancel(handle_.get(), &error);
		EyeTrackerException::throwOnError(error);
		return upgrade_cancelled > 0;
	}

	Progress doGetProgress()
	{
		Progress progress;
		tobii_sdk_error_t* error = 0;
		tobii_sdk_upgrade_progress_reporter_get_progress(handle_.get(),
				&progress.currentStep,
				&progress.numberOfSteps,
				&progress.percent,
				&error);
		EyeTrackerException::throwOnError(error);
		return progress;
	}

	connection_t doAddUpgradeCompletedListener(const upgrade_completed_event_t::slot_type& listener)
	{
		return upgradeCompletedSig_.connect(listener);
	}

	connection_t doAddProgressChangedListener(const upgrade_progress_event_t::slot_type& listener)
	{
		return upgradeProgressSig_.connect(listener);
	}

	connection_t doAddCanCancelChangedListener(const can_cancel_changed_event_t::slot_type& listener)
	{
		return canCancelChangedSig_.connect(listener);
	}

	void handleUpgradeCompleted(uint32_t error)
	{
		upgradeCompletedSig_(error);
	}

	void handleProgressChanged(Progress& progress)
	{
		upgradeProgressSig_(progress);
	}

	void handleCanCancelChanged(bool can_cancel)
	{
		canCancelChangedSig_(can_cancel);
	}

	static void completionHandlerTrampoline(
			tobii_sdk_upgrade_progress_reporter_t*,
			tobii_sdk_error_t* error,
			void* user_data)
	{
		ProgressReporterImpl* self = (ProgressReporterImpl*)user_data;
		uint32_t error_code = tobii_sdk_error_get_code(error);
		self->handleUpgradeCompleted(error_code);
	}

	static void progressHandlerTrampoline(
			tobii_sdk_upgrade_progress_reporter_t*,
			uint32_t currentStep,
			uint32_t numberOfSteps,
			uint32_t percentage,
			void* userData)
	{
		Progress progress;
		progress.currentStep = currentStep;
		progress.numberOfSteps = numberOfSteps;
		progress.percent = percentage;

		ProgressReporterImpl* self = (ProgressReporterImpl*)userData;
		self->handleProgressChanged(progress);
	}

	static void canCancelChangedTrampoline(
			tobii_sdk_upgrade_progress_reporter_t*,
			uint32_t canCancel,
			void* userData)
	{
		ProgressReporterImpl* self = (ProgressReporterImpl*)userData;
		self->handleCanCancelChanged(canCancel > 0);
	}
};

} // namespace cpp

} // namespace sdk

} // namespace tobii


#endif // __TOBII_SDK_CPP_PROGRESS_REPORTER_HPP__
