#ifndef __TOBII_SDK_CPP_EYE_TRACKER_BROWSER_FACTORY_HPP__
#define __TOBII_SDK_CPP_EYE_TRACKER_BROWSER_FACTORY_HPP__

#include <string>

#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <boost/make_shared.hpp>
#include <boost/signals2.hpp>
#include <tobii/sdk/core.h>
#include <tobii/sdk/cpp/MainLoop.hpp>
#include <tobii/sdk/cpp/EyeTrackerException.hpp>
#include <tobii/sdk/cpp/EyeTrackerBrowser.hpp>
#include <tobii/sdk/cpp/EyeTrackerFactory.hpp>

namespace tobii {

namespace sdk {

namespace cpp {

class EyeTrackerBrowserFactory
{
public:
	static EyeTrackerBrowser::pointer_t createBrowser(MainLoop& mainLoop)
	{
		return EyeTrackerBrowser::pointer_t(new EyeTrackerBrowserImpl(mainLoop));
	}

	static EyeTrackerFactory::pointer_t createEyeTrackerFactoryByIpAddressOrHostname(const std::string& ipAddressOrHostname, uint32_t tetserverPort, uint32_t syncPort)
	{
		return EyeTrackerFactory::pointer_t(new EyeTrackerFactoryImpl(ipAddressOrHostname, tetserverPort, syncPort));
	}

private:
	// the constructor is private because this class should never be instantiated.
	EyeTrackerBrowserFactory()
	{
	}

	//
	// nested class EyeTrackerFactoryImpl
	//
	class EyeTrackerFactoryImpl : public EyeTrackerFactory, boost::noncopyable
	{
	public:
		EyeTrackerFactoryImpl(tobii_sdk_device_info_t* deviceInfoHandle)
		{
			tobii_sdk_error_t* error = 0;
			handle_.reset(
				tobii_sdk_device_info_get_factory_info(deviceInfoHandle, &error),
				&tobii_sdk_factory_info_destroy);
			EyeTrackerException::throwOnError(error);
		}

		EyeTrackerFactoryImpl(const std::string& ipAddressOrHostname, uint32_t tetserverPort, uint32_t syncPort)
		{
			tobii_sdk_error_t* error = 0;
			handle_.reset(
				tobii_sdk_factory_info_create_for_networked_eyetracker(
					ipAddressOrHostname.c_str(),
					tetserverPort, 
					syncPort, 
					&error),
				&tobii_sdk_factory_info_destroy);
			EyeTrackerException::throwOnError(error);
		}

	private:
		boost::shared_ptr<tobii_sdk_factory_info_t> handle_;

		std::string doGetRepresentation()
		{
			tobii_sdk_error_t* error = 0;
			boost::shared_ptr<const char> cValue(
				tobii_sdk_factory_info_get_representation(handle_.get(), &error), 
				&tobii_sdk_free_string);
			EyeTrackerException::throwOnError(error);
			if (cValue == 0)
				return "";

			std::string value(cValue.get());
			return value;
		}

		EyeTracker::pointer_t doCreateEyeTracker(MainLoop& mainLoop)
		{
			AsyncResult<tobii_sdk_message_passer_t*> result;
			tobii_sdk_error_t* error = 0;
			tobii_sdk_message_passer_get(
				handle_.get(),
				mainLoop.getHandle(),
				&eyetracker_created_trampoline,
				&result,
				&error);
			EyeTrackerException::throwOnError(error);

			result.waitForResult();
			result.throwIfError();

			return EyeTracker::pointer_t(new EyeTrackerImpl(result.getResult()));
		}

		static void eyetracker_created_trampoline(tobii_sdk_error_t* error,
				tobii_sdk_message_passer_t* mpi,
				void* userData)
		{
			AsyncResult<tobii_sdk_message_passer_t*>* result_ptr = (AsyncResult<tobii_sdk_message_passer_t*>*)userData;

			if(tobii_sdk_error_is_failure(error))
			{
				uint32_t error_code = tobii_sdk_error_get_code(error);
				result_ptr->setError(error_code);
			}
			else
			{
				result_ptr->setResult(mpi);
			}
		}

		SyncManager::pointer_t doCreateSyncManager(Clock& clock, MainLoop& mainLoop)
		{
			return SyncManager::pointer_t(new SyncManagerImpl(handle_.get(), clock, mainLoop));
		}
	};

	//
	// nested class EyeTrackerInfoImpl
	//
	class EyeTrackerInfoImpl : public EyeTrackerInfo, boost::noncopyable
	{
	public:
		EyeTrackerInfoImpl(tobii_sdk_device_info_t* deviceInfoHandle)
		{
			if(!deviceInfoHandle) {
				throw std::runtime_error("dev_info was null pointer");
			}

			tobii_sdk_error_t* error = 0;
			handle_.reset(
				tobii_sdk_device_info_clone(deviceInfoHandle, &error), 
				&tobii_sdk_device_info_destroy);
			EyeTrackerException::throwOnError(error);
		}

	private:
		boost::shared_ptr<tobii_sdk_device_info_t> handle_;

		std::string doGetValue(const std::string& key)
		{
			tobii_sdk_error_t* error = 0;
			boost::shared_ptr<const char> cValue(
				tobii_sdk_device_info_get(handle_.get(), key.c_str(), &error), 
				&tobii_sdk_free_string);
			EyeTrackerException::throwOnError(error);
			if (cValue == 0)
				return "";

			std::string value(cValue.get());
			return value;
		}

		std::string doGetProductId()
		{
			tobii_sdk_error_t* error = 0;
			boost::shared_ptr<const char> cProductId(
				tobii_sdk_device_info_get_product_id(handle_.get(), &error), 
				&tobii_sdk_free_string);
			EyeTrackerException::throwOnError(error);

			if (!cProductId)
				return "";

			std::string productId(cProductId.get());
			return productId;
		}

		EyeTrackerFactory::pointer_t doGetEyeTrackerFactory()
		{
			return EyeTrackerFactory::pointer_t(new EyeTrackerFactoryImpl(handle_.get()));
		}

		ProgressReporter::pointer_t doBeginUpgrade(UpgradePackage& upgradePackage, MainLoop& mainLoop)
		{
			ProgressReporter::pointer_t progress(new ProgressReporterImpl());
			tobii_sdk_error_t* error = 0;
			tobii_sdk_upgrade_begin(
				mainLoop.getHandle(),
				upgradePackage.getHandle(), 
				handle_.get(), 
				((ProgressReporterImpl*)progress.get())->getHandle(), 
				&error);
			EyeTrackerException::throwOnError(error);
			return progress;
		}

		bool checkIsCompatible(UpgradePackage& upgradePackage, MainLoop& mainLoop, uint32_t& errorCode)
		{
			tobii_sdk_error_t* error = 0;
			uint32_t compatible = tobii_sdk_upgrade_package_is_compatible_with_device(
				mainLoop.getHandle(),
				upgradePackage.getHandle(),
				handle_.get(),
				&error);

			if(compatible)
			{
				errorCode = 0;
				return true;
			}
			else
			{
				if(error)
				{
					errorCode = tobii_sdk_error_get_code(error);
					tobii_sdk_error_destroy(error);
				}
				else
				{
					errorCode = 0;
				}

				return false;
			}
		}
	};

	//
	// nested class EyeTrackerBrowserImpl
	//
	class EyeTrackerBrowserImpl : public EyeTrackerBrowser, boost::noncopyable
	{
	public:
		EyeTrackerBrowserImpl(MainLoop& mainLoop)
			: mainLoop_(mainLoop)
		{
		}

	private:
		MainLoop& mainLoop_;
		browser_event_t browserSig_;
		boost::shared_ptr<tobii_sdk_device_browser_t> handle_; // NOTE must be the first to be destructed.

		void doStart()
		{
			if (!handle_)
			{
				tobii_sdk_error_t* error = 0;
				handle_.reset(tobii_sdk_device_browser_create(
						mainLoop_.getHandle(),
						&EyeTrackerBrowserImpl::callbackTrampoline,
						this,
						&error),
					&tobii_sdk_device_browser_destroy);
				EyeTrackerException::throwOnError(error);
			}
		}

		void doStop()
		{
			handle_.reset();
		}

		connection_t doAddEventListener(const browser_event_t::slot_type& listener)
		{
			return browserSig_.connect(listener);
		}
		
		void handleEyeTrackerInfoEvent(event_type_t type, EyeTrackerInfo::pointer_t info)
		{
			browserSig_(type, info);
		}

		static void callbackTrampoline(tobii_sdk_on_device_browser_event_t event,
					tobii_sdk_device_info_t* deviceInfo,
					void* userData)
		{
			EyeTrackerInfo::pointer_t info = boost::make_shared<EyeTrackerInfoImpl>(deviceInfo);

			EyeTrackerBrowserImpl* self = (EyeTrackerBrowserImpl*)userData;
			self->handleEyeTrackerInfoEvent(static_cast<event_type_t>(event), info);
		}
	};
};

} // namespace cpp

} // namespace sdk

} // namespace tobii

#endif // __TOBII_SDK_CPP_EYE_TRACKER_BROWSER_FACTORY_HPP__
