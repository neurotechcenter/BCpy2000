
#ifndef __TOBII_SDK_CPP_TRACKING_EYE_TRACKER_HPP__
#define __TOBII_SDK_CPP_TRACKING_EYE_TRACKER_HPP__

#include <vector>
#include <string>

#include <boost/make_shared.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/function.hpp>
#include <boost/signals2.hpp>
#include <boost/bind.hpp>

#include <tobii/sdk/core.h>
#include <tobii/sdk/cpp/EyeTrackerException.hpp>
#include <tobii/sdk/cpp/AsyncResult.hpp>
#include <tobii/sdk/cpp/ParamStack.hpp>
#include <tobii/sdk/cpp/Calibration.hpp>
#include <tobii/sdk/cpp/GazeDataItem.hpp>
#include <tobii/sdk/cpp/Types.hpp>

namespace tobii {

namespace sdk {

namespace cpp {

class EyeTracker
{
public:
	typedef boost::shared_ptr<EyeTracker> pointer_t;
	typedef boost::function<void (uint32_t)> async_callback_t;
	typedef boost::signals2::connection connection_t;
	typedef boost::signals2::signal<void (float)> framerate_changed_event_t;
	typedef boost::signals2::signal<void ()> empty_event_t;
	typedef boost::signals2::signal<void (GazeDataItem::pointer_t)> gaze_data_received_event_t;
	typedef boost::signals2::signal<void (const XConfiguration&)> x_configuration_changed_event_t;
	typedef boost::signals2::signal<void (uint32_t)> connection_error_event_t;

	virtual ~EyeTracker() {}

	void setFrameRate(float framerate)
	{
		doSetFrameRate(framerate);
	}

	float getFrameRate()
	{
		return doGetFrameRate();
	}

	AuthorizeChallenge getAuthorizeChallenge(uint32_t realmId, const std::vector<uint32_t> algorithms)
	{
		return doGetAuthorizeChallenge(realmId, algorithms);
	}

	void validateChallengeResponse(uint32_t realmId, uint32_t algorithm, Blob& responseData)
	{
		doValidateChallengeResponse(realmId, algorithm, responseData);
	}

	std::vector<float> enumerateFrameRates()
	{
		return doEnumerateFrameRates();
	}

	uint32_t getLowblinkMode()
	{
		return doGetLowBlinkMode();
	}

	void setLowblinkMode(uint32_t enabled)
	{
		doSetLowBlinkMode(enabled);
	}

	void dumpImages(uint32_t count, uint32_t frequency)
	{
		doDumpImages(count, frequency);
	}

	Blob getDiagnosticsReport(uint32_t includeImages)
	{
		return doGetDiagnosticsReport(includeImages);
	}

	void setUnitName(const std::string& name)
	{
		doSetUnitName(name);
	}
	std::string getUnitName()
	{
		return doGetUnitName();
	}

	UnitInfo::pointer_t getUnitInfo()
	{
		return doGetUnitInfo();
	}
	PayPerUseInfo getPayPerUseInfo()
	{
		return doGetPayPerUseInfo();
	}

	void startCalibration()
	{
		doStartCalibration();
	}

	void stopCalibration()
	{
		doStopCalibration();
	}

	void clearCalibration()
	{
		doClearCalibration();
	}

	void addCalibrationPoint(const Point2d& point)
	{
		doAddCalibrationPoint(point);
	}

	void addCalibrationPointAsync(const Point2d& point, const async_callback_t& completedHandler)
	{
		doAddCalibrationPointAsync(point, completedHandler);
	}

	void removeCalibrationPoint(const Point2d& point)
	{	
		doRemoveCalibrationPoint(point);
	}

	void computeCalibration()
	{
		doComputeCalibration();
	}

	void computeCalibrationAsync(const async_callback_t& completedHandler)
	{
		doComputeCalibrationAsync(completedHandler);
	}

	Calibration::pointer_t getCalibration()
	{
		return doGetCalibration();
	}

	void setCalibration(Calibration::pointer_t calibration)
	{
		doSetCalibration(calibration);
	}

	void startTracking()
	{
		doStartTracking();
	}

	void stopTracking()
	{
		doStopTracking();
	}

	TrackBox::pointer_t getTrackBox()
	{
		return doGetTrackBox();
	}

	void enableExtension(uint32_t extensionId)
	{
		doEnableExtension(extensionId);
	}

	Extension::vector_pointer_t getAvailableExtensions()
	{
		return doGetAvailableExtensions();
	}

	Extension::vector_pointer_t getEnabledExtensions()
	{
		return doGetEnabledExtensions();
	}

	void setXConfiguration(const XConfiguration& configuration)
	{
		doSetXConfiguration(configuration);
	}

	XConfiguration::pointer_t getXConfiguration()
	{
		return doGetXConfiguration();
	}

	std::string getIlluminationMode()
	{
		return doGetIlluminationMode();
	}

	void setIlluminationMode(const std::string& illuminationMode)
	{
		doSetIlluminationMode(illuminationMode);
	}

	std::vector<std::string> enumerateIlluminationModes()
	{
		return doEnumerateIlluminationModes();
	}

	connection_t addFrameRateChangedListener(const framerate_changed_event_t::slot_type& listener)
	{
		return doAddFrameRateChangedListener(listener);
	}

	connection_t addCalibrationStartedListener(const empty_event_t::slot_type& listener)
	{
		return doAddCalibrationStartedListener(listener);
	}

	connection_t addCalibrationStoppedListener(const empty_event_t::slot_type& listener)
	{
		return doAddCalibrationStoppedListener(listener);
	}

	connection_t addGazeDataReceivedListener(const gaze_data_received_event_t::slot_type& listener)
	{
		return doAddGazeDataReceivedListener(listener);
	}

	connection_t addTrackBoxChangedListener(const empty_event_t::slot_type& listener)
	{
		return doAddTrackBoxChangedListener(listener);
	}

	connection_t addXConfigChangedListener(const x_configuration_changed_event_t::slot_type& listener)
	{
		return doAddXConfigChangedListener(listener);
	}

	connection_t addConnectionErrorListener(const connection_error_event_t::slot_type& listener)
	{
		return doAddConnectionErrorListener(listener);
	}

private:
	virtual void doSetFrameRate(float framerate) = 0;
	virtual float doGetFrameRate() = 0;
	virtual AuthorizeChallenge doGetAuthorizeChallenge(uint32_t realmId, const std::vector<uint32_t> algorithms) = 0;
	virtual void doValidateChallengeResponse(uint32_t realmId, uint32_t algorithm, Blob& responseData) = 0;
	virtual std::vector<float> doEnumerateFrameRates() = 0;
	virtual uint32_t doGetLowBlinkMode() = 0;
	virtual void doSetLowBlinkMode(uint32_t enabled) = 0;
	virtual void doDumpImages(uint32_t count, uint32_t frequency) = 0;
	virtual Blob doGetDiagnosticsReport(uint32_t includeImages) = 0;
	virtual void doSetUnitName(const std::string& name) = 0;
	virtual std::string doGetUnitName() = 0;
	virtual UnitInfo::pointer_t doGetUnitInfo() = 0;
	virtual PayPerUseInfo doGetPayPerUseInfo() = 0;
	virtual void doStartCalibration() = 0;
	virtual void doStopCalibration() = 0;
	virtual void doClearCalibration() = 0;
	virtual void doAddCalibrationPoint(const Point2d& point) = 0;
	virtual void doAddCalibrationPointAsync(const Point2d& point, const async_callback_t& completedHandler) = 0;
	virtual void doRemoveCalibrationPoint(const Point2d& point) = 0;
	virtual void doComputeCalibration() = 0;
	virtual void doComputeCalibrationAsync(const async_callback_t& completedHandler) = 0;
	virtual Calibration::pointer_t doGetCalibration() = 0;
	virtual void doSetCalibration(Calibration::pointer_t calibration) = 0;
	virtual void doStartTracking() = 0;
	virtual void doStopTracking() = 0;
	virtual TrackBox::pointer_t doGetTrackBox() = 0;
	virtual void doEnableExtension(uint32_t extensionId) = 0;
	virtual Extension::vector_pointer_t doGetAvailableExtensions() = 0;
	virtual Extension::vector_pointer_t doGetEnabledExtensions() = 0;
	virtual void doSetXConfiguration(const XConfiguration& configuration) = 0;
	virtual XConfiguration::pointer_t doGetXConfiguration() = 0;
	virtual std::string doGetIlluminationMode() = 0;
	virtual void doSetIlluminationMode(const std::string& illuminationMode) = 0;
	virtual std::vector<std::string> doEnumerateIlluminationModes() = 0;
	virtual connection_t doAddFrameRateChangedListener(const framerate_changed_event_t::slot_type& listener) = 0;
	virtual connection_t doAddCalibrationStartedListener(const empty_event_t::slot_type& listener) = 0;
	virtual connection_t doAddCalibrationStoppedListener(const empty_event_t::slot_type& listener) = 0;
	virtual connection_t doAddGazeDataReceivedListener(const gaze_data_received_event_t::slot_type& listener) = 0;
	virtual connection_t doAddTrackBoxChangedListener(const empty_event_t::slot_type& listener) = 0;
	virtual connection_t doAddXConfigChangedListener(const x_configuration_changed_event_t::slot_type& listener) = 0;
	virtual connection_t doAddConnectionErrorListener(const connection_error_event_t::slot_type& listener) = 0;
};

class EyeTrackerImpl : public EyeTracker, boost::noncopyable
{
public:
	EyeTrackerImpl(tobii_sdk_message_passer_t* messagePasser) :
		mp_(messagePasser, tobii_sdk_message_passer_destroy)
	{
		tobii_sdk_error_t* error = 0;

		boost::shared_ptr<tobii_sdk_callback_connection_t> framerateChangedConn(
				tobii_sdk_message_passer_add_data_handler(mp_.get(), 1640, &EyeTrackerImpl::handleDataCallback, this, &error),
				tobii_sdk_callback_connection_destroy);
		EyeTrackerException::throwOnError(error);

		boost::shared_ptr<tobii_sdk_callback_connection_t> calibrationStartedConn(
				tobii_sdk_message_passer_add_data_handler(mp_.get(), 1040, &EyeTrackerImpl::handleDataCallback, this, &error),
				tobii_sdk_callback_connection_destroy);
		EyeTrackerException::throwOnError(error);

		boost::shared_ptr<tobii_sdk_callback_connection_t> calibrationStoppedConn(
				tobii_sdk_message_passer_add_data_handler(mp_.get(), 1050, &EyeTrackerImpl::handleDataCallback, this, &error),
				tobii_sdk_callback_connection_destroy);
		EyeTrackerException::throwOnError(error);

		boost::shared_ptr<tobii_sdk_callback_connection_t> gazedataReceivedConn(
				tobii_sdk_message_passer_add_data_handler(mp_.get(), 1280, &EyeTrackerImpl::handleDataCallback, this, &error),
				tobii_sdk_callback_connection_destroy);
		EyeTrackerException::throwOnError(error);

		boost::shared_ptr<tobii_sdk_callback_connection_t> trackboxChangedConn(
				tobii_sdk_message_passer_add_data_handler(mp_.get(), 1410, &EyeTrackerImpl::handleDataCallback, this, &error),
				tobii_sdk_callback_connection_destroy);
		EyeTrackerException::throwOnError(error);

		boost::shared_ptr<tobii_sdk_callback_connection_t> xconfigChangedConn(
				tobii_sdk_message_passer_add_data_handler(mp_.get(), 1450, &EyeTrackerImpl::handleDataCallback, this, &error),
				tobii_sdk_callback_connection_destroy);
		EyeTrackerException::throwOnError(error);

		boost::shared_ptr<tobii_sdk_callback_connection_t> errorCallbackConn(
				tobii_sdk_message_passer_add_error_handler(mp_.get(), &EyeTrackerImpl::handleErrorCallback, this, &error),
				tobii_sdk_callback_connection_destroy);
		EyeTrackerException::throwOnError(error);

		callbacks_.push_back(framerateChangedConn);
		callbacks_.push_back(calibrationStartedConn);
		callbacks_.push_back(calibrationStoppedConn);
		callbacks_.push_back(gazedataReceivedConn);
		callbacks_.push_back(trackboxChangedConn);
		callbacks_.push_back(xconfigChangedConn);
		callbacks_.push_back(errorCallbackConn);
	}

private:
	typedef boost::function<void (tobii_sdk_opcode_t, tobii_sdk_error_code_t, ParamStack&)> response_handler_function_t;
	typedef std::vector<boost::shared_ptr<tobii_sdk_callback_connection_t> > callback_connection_vector_t;

	boost::shared_ptr<tobii_sdk_message_passer_t> mp_;
	std::vector<boost::shared_ptr<tobii_sdk_callback_connection_t> >	callbacks_;
	framerate_changed_event_t framerateChangedSig_;
	empty_event_t calibrationStartedSig_;
	empty_event_t calibrationStoppedSig_;
	gaze_data_received_event_t gazeDataReceivedSig_;
	empty_event_t trackboxChangedSig_;
	x_configuration_changed_event_t xConfigurationChangedSig_;
	connection_error_event_t connectionErrorSig_;

	struct ResponseHandlerTrampolineData {
		response_handler_function_t handler;
	};

	struct EmptyData{

	};

	static void responseHandlerTrampoline(
			tobii_sdk_opcode_t opcode,
			tobii_sdk_error_code_t errorCode,
			const tobii_sdk_param_stack_t* params,
			void* userData)
	{
		boost::scoped_ptr<ResponseHandlerTrampolineData> trampolineData((ResponseHandlerTrampolineData*)userData);

		if(trampolineData) {
			ParamStack stack(params);
			trampolineData->handler(opcode, errorCode, stack);
		}
	}

	void execute(uint32_t opcode, const ParamStack& stack, const response_handler_function_t& handler)
	{
		ResponseHandlerTrampolineData* trampolineData = new ResponseHandlerTrampolineData();
		trampolineData->handler = handler;

		tobii_sdk_error_t* error = 0;
		tobii_sdk_message_passer_execute_request(mp_.get(),
				opcode,
				stack.getHandle(),
				&EyeTrackerImpl::responseHandlerTrampoline,
				trampolineData,
				&error);
		EyeTrackerException::throwOnError(error);
	}

	void handleEmptyResponse(
			tobii_sdk_error_code_t error,
			AsyncResult<EmptyData>::pointer_t result)
	{
		if(error)
		{
			result->setError(error);
		}
		else
		{
			result->setResult(EmptyData());
		}
	}

	static void handleDataCallback(tobii_sdk_opcode_t opcode,
			tobii_sdk_error_code_t,
			const tobii_sdk_param_stack_t* params,
			void* userData)
	{
		EyeTrackerImpl* self = (EyeTrackerImpl*)userData;
		ParamStack stack(params);

		switch (opcode) {
			case 1640:
				self->handleFrameRateChanged(stack);
				break;
			case 1040:
				self->handleCalibrationStarted();
				break;
			case 1050:
				self->handleCalibrationStopped();
				break;
			case 1280:
				self->handleGazeDataReceived(stack);
				break;
			case 1410:
				self->handleTrackBoxChanged();
				break;
			case 1450:
				self->handleXConfigChanged(stack);
				break;
			default:
				break;
		}
	}

	static void handleErrorCallback(tobii_sdk_error_code_t error, void* userData)
	{
		EyeTrackerImpl* self = (EyeTrackerImpl*)userData;
		self->handleErrorEvent(error);
	}

	void handleFrameRateChanged(ParamStack& stack)
	{
		float framerate = stack.getFixed15x16AsFloat32(0);
		framerateChangedSig_(framerate);
	}

	void handleCalibrationStarted()
	{
		calibrationStartedSig_();
	}

	void handleCalibrationStopped()
	{
		calibrationStoppedSig_();
	}

	void handleGazeDataReceived(ParamStack& stack)
	{
		GazeDataItem::pointer_t data(new GazeDataItemImpl(stack));
		if(data)
		{
			gazeDataReceivedSig_(data);
		}
	}

	void handleTrackBoxChanged()
	{
		trackboxChangedSig_();
	}

	void handleXConfigChanged(ParamStack& stack)
	{
		XConfiguration config;
		uint32_t index = 0;
		config.upperLeft = stack.readPoint3d(index);
		config.upperRight = stack.readPoint3d(index);
		config.lowerLeft = stack.readPoint3d(index);
		// Ignore tool data

		xConfigurationChangedSig_(config);
	}

	void handleErrorEvent(uint32_t errorCode)
	{
		connectionErrorSig_(errorCode);
	}

	AuthorizeChallenge doGetAuthorizeChallenge(uint32_t realmId, const std::vector<uint32_t> algorithms)
	{
		AsyncResult<AuthorizeChallenge>::pointer_t result(new AsyncResult<AuthorizeChallenge>());
		ParamStack stack;
		stack.pushUInt32(realmId);
		stack.pushVector(algorithms);

		execute(1900, stack, boost::bind(&EyeTrackerImpl::handleGetAuthorizeChallengeResponse, this, _2, _3, result));

		result->waitForResult();
		result->throwIfError();

		return result->getResult();
	}

	void doValidateChallengeResponse(uint32_t realmId, uint32_t algorithm, Blob& responseData)
	{
		AsyncResult<EmptyData>::pointer_t result(new AsyncResult<EmptyData>());
		ParamStack stack;
		stack.pushUInt32(realmId);
		stack.pushUInt32(algorithm);
		stack.pushBlob(responseData.getData().get(), responseData.getLength());

		execute(1910, stack, boost::bind(&EyeTrackerImpl::handleEmptyResponse, this, _2, result));

		result->waitForResult();
		result->throwIfError();
	}

	void handleGetAuthorizeChallengeResponse(
				tobii_sdk_error_code_t error,
				ParamStack& stack,
				AsyncResult<AuthorizeChallenge>::pointer_t result)
		{
			if(error)
			{
				result->setError(error);
			}
			else
			{
				try {
					AuthorizeChallenge challenge;

					challenge.realmId = stack.getUInt32(0);
					challenge.algorithm = stack.getUInt32(1);
					challenge.challengeData = stack.getBlob(2);

					result->setResult(challenge);
				}
				catch (EyeTrackerException& ex) {
					result->setError(ex.getErrorCode());
				}
			}
		}

	std::vector<float> doEnumerateFrameRates()
	{
		AsyncResult<std::vector<float> >::pointer_t result(new AsyncResult<std::vector<float> >());
		ParamStack stack;

		execute(1630, stack, boost::bind(&EyeTrackerImpl::handleEnumerateFrameRatesResponse, this, _2, _3, result));

		result->waitForResult();
		result->throwIfError();

		return result->getResult();
	}

	void handleEnumerateFrameRatesResponse(
			tobii_sdk_error_code_t error,
			ParamStack& stack,
			AsyncResult<std::vector<float> >::pointer_t result)
	{
		if(error)
		{
			result->setError(error);
		}
		else
		{
			try {
				std::vector<float> framerates(stack.getVectorFloat(0));
				result->setResult(framerates);
			}
			catch (EyeTrackerException& ex) {
				result->setError(ex.getErrorCode());
			}
		}
	}

	void doSetFrameRate(float framerate)
	{
		AsyncResult<EmptyData>::pointer_t result(new AsyncResult<EmptyData>());
		ParamStack stack;

		stack.pushFloat32AsFixed15x16(framerate);
		execute(1620, stack, boost::bind(&EyeTrackerImpl::handleEmptyResponse, this, _2, result));

		result->waitForResult();
		result->throwIfError();
	}

	float doGetFrameRate()
	{
		AsyncResult<float>::pointer_t result(new AsyncResult<float>());
		ParamStack stack;

		execute(1610, stack, boost::bind(&EyeTrackerImpl::handleGetFrameRateResponse, this, _2, _3, result));

		result->waitForResult();
		result->throwIfError();

		return result->getResult();
	}

	void handleGetFrameRateResponse(
			tobii_sdk_error_code_t error,
			ParamStack& stack,
			AsyncResult<float>::pointer_t result)
	{
		if(error)
		{
			result->setError(error);
		}
		else
		{
			try {
				float framerate = stack.getFixed15x16AsFloat32(0);
				result->setResult(framerate);
			}
			catch (EyeTrackerException& ex) {
				result->setError(ex.getErrorCode());
			}
		}
	}

	uint32_t doGetLowBlinkMode()
	{
		AsyncResult<uint32_t>::pointer_t result(new AsyncResult<uint32_t>());
		ParamStack stack;

		execute(1920, stack, boost::bind(&EyeTrackerImpl::handleGetLowblinkResponse, this, _2, _3, result));

		result->waitForResult();
		result->throwIfError();

		return result->getResult();
	}

	void doSetLowBlinkMode(uint32_t enabled)
	{
		AsyncResult<EmptyData>::pointer_t result(new AsyncResult<EmptyData>());
		ParamStack stack;
		stack.pushUInt32(enabled);

		execute(1930, stack, boost::bind(&EyeTrackerImpl::handleEmptyResponse, this, _2, result));

		result->waitForResult();
		result->throwIfError();
	}

	void handleGetLowblinkResponse(
			tobii_sdk_error_code_t error,
			ParamStack& stack,
			AsyncResult<uint32_t>::pointer_t result)
	{
		if(error)
		{
			result->setError(error);
		}
		else
		{
			try {
				uint32_t lowblink = stack.getUInt32(0);
				result->setResult(lowblink);
			}
			catch (EyeTrackerException& ex) {
				result->setError(ex.getErrorCode());
			}
		}
	}

	void doDumpImages(uint32_t count, uint32_t frequency)
	{
		AsyncResult<EmptyData>::pointer_t result(new AsyncResult<EmptyData>());
		ParamStack stack;
		stack.pushUInt32(count);
		stack.pushUInt32(frequency);

		execute(1500, stack, boost::bind(&EyeTrackerImpl::handleEmptyResponse, this, _2, result));

		result->waitForResult();
		result->throwIfError();
	}

	Blob doGetDiagnosticsReport(uint32_t includeImages)
	{
		AsyncResult<Blob>::pointer_t result(new AsyncResult<Blob>());
		ParamStack stack;
		stack.pushUInt32(includeImages);

		execute(1510, stack, boost::bind(&EyeTrackerImpl::handleGetDiagnosticsResponse, this, _2, _3, result));

		result->waitForResult();
		result->throwIfError();

		return result->getResult();
	}

	void handleGetDiagnosticsResponse(
			tobii_sdk_error_code_t error,
			ParamStack& stack,
			AsyncResult<Blob>::pointer_t result)
	{
		if(error)
		{
			result->setError(error);
		}
		else
		{
			try {
				Blob data = stack.getBlob(0);
				result->setResult(data);
			}
			catch (EyeTrackerException& ex) {
				result->setError(ex.getErrorCode());
			}
		}
	}

	void doSetUnitName(const std::string& name)
	{
		AsyncResult<EmptyData>::pointer_t result(new AsyncResult<EmptyData>());
		ParamStack stack;

		stack.pushString(name);
		execute(1710, stack, boost::bind(&EyeTrackerImpl::handleEmptyResponse, this, _2, result));

		result->waitForResult();
		result->throwIfError();
	}

	std::string doGetUnitName()
	{
		AsyncResult<std::string>::pointer_t result(new AsyncResult<std::string>());
		ParamStack stack;

		execute(1700, stack, boost::bind(&EyeTrackerImpl::handleGetUnitNameResponse, this, _2, _3, result));

		result->waitForResult();
		result->throwIfError();

		return result->getResult();
	}

	void handleGetUnitNameResponse(
			tobii_sdk_error_code_t error,
			ParamStack& stack,
			AsyncResult<std::string>::pointer_t result)
	{
		if(error)
		{
			result->setError(error);
		}
		else
		{
			try {
				std::string name = stack.getString(0);
				result->setResult(name);
			}
			catch (EyeTrackerException& ex) {
				result->setError(ex.getErrorCode());
			}
		}
	}

	UnitInfo::pointer_t doGetUnitInfo()
	{
		AsyncResult<UnitInfo::pointer_t>::pointer_t result(new AsyncResult<UnitInfo::pointer_t>());
		ParamStack stack;

		execute(1420, stack, boost::bind(&EyeTrackerImpl::handleGetUnitInfoResponse, this, _2, _3, result));

		result->waitForResult();
		result->throwIfError();

		return result->getResult();
	}

	void handleGetUnitInfoResponse(
			tobii_sdk_error_code_t error,
			ParamStack& stack,
			AsyncResult<UnitInfo::pointer_t>::pointer_t result)
	{
		if(error)
		{
			result->setError(error);
		}
		else
		{
			try {
				UnitInfo::pointer_t info(new UnitInfo);
				info->serialNumber = stack.getString(0);
				info->model = stack.getString(1);
				info->generation = stack.getString(2);
				info->firmwareVersion = stack.getString(3);
				result->setResult(info);
			}
			catch (EyeTrackerException& ex) {
				result->setError(ex.getErrorCode());
			}
		}
	}

	PayPerUseInfo doGetPayPerUseInfo()
	{
		AsyncResult<PayPerUseInfo>::pointer_t result(new AsyncResult<PayPerUseInfo>());
		ParamStack stack;

		execute(1600, stack, boost::bind(&EyeTrackerImpl::handleGetPayPerUseInfoResponse, this, _2, _3, result));

		result->waitForResult();
		result->throwIfError();

		return result->getResult();
	}

	void handleGetPayPerUseInfoResponse(
			tobii_sdk_error_code_t error,
			ParamStack& stack,
			AsyncResult<PayPerUseInfo>::pointer_t result)
	{
		if(error)
		{
			result->setError(error);
		}
		else
		{
			try {
				PayPerUseInfo p_info;
				p_info.enabled = stack.getUInt32(0) > 0;
				p_info.realm = stack.getUInt32(1);
				p_info.authorized = stack.getUInt32(2) > 0;
				result->setResult(p_info);
			}
			catch (EyeTrackerException& ex) {
				result->setError(ex.getErrorCode());
			}
		}
	}

	void doStartCalibration()
	{
		AsyncResult<EmptyData>::pointer_t result(new AsyncResult<EmptyData>());
		ParamStack stack;

		execute(1010, stack, boost::bind(&EyeTrackerImpl::handleEmptyResponse, this, _2, result));

		result->waitForResult();
		result->throwIfError();
	}

	void doStopCalibration()
	{
		AsyncResult<EmptyData>::pointer_t result(new AsyncResult<EmptyData>());
		ParamStack stack;

		execute(1020, stack, boost::bind(&EyeTrackerImpl::handleEmptyResponse, this, _2, result));

		result->waitForResult();
		result->throwIfError();
	}

	void doClearCalibration()
	{
		AsyncResult<EmptyData>::pointer_t result(new AsyncResult<EmptyData>());
		ParamStack stack;

		execute(1060, stack, boost::bind(&EyeTrackerImpl::handleEmptyResponse, this, _2, result));

		result->waitForResult();
		result->throwIfError();
	}

	void doAddCalibrationPoint(const Point2d& point)
	{
		AsyncResult<EmptyData>::pointer_t result(new AsyncResult<EmptyData>());
		ParamStack stack;
		stack.pushFloat64AsFixed22x41(point.x);
		stack.pushFloat64AsFixed22x41(point.y);
		stack.pushUInt32(3);

		execute(1030, stack, boost::bind(&EyeTrackerImpl::handleEmptyResponse, this, _2, result));

		result->waitForResult();
		result->throwIfError();
	}

	void doAddCalibrationPointAsync(const Point2d& point, const async_callback_t& completedHandler)
	{
		AsyncResult<EmptyData>::pointer_t result(new AsyncResult<EmptyData>());
		ParamStack stack;
		stack.pushFloat64AsFixed22x41(point.x);
		stack.pushFloat64AsFixed22x41(point.y);
		stack.pushUInt32(3);

		execute(1030, stack, boost::bind(completedHandler, _2));
	}


	void doRemoveCalibrationPoint(const Point2d& point)
	{
		AsyncResult<EmptyData>::pointer_t result(new AsyncResult<EmptyData>());
		ParamStack stack;
		stack.pushFloat64AsFixed22x41(point.x);
		stack.pushFloat64AsFixed22x41(point.y);
		stack.pushUInt32(3);

		execute(1080, stack, boost::bind(&EyeTrackerImpl::handleEmptyResponse, this, _2, result));

		result->waitForResult();
		result->throwIfError();
	}

	void doComputeCalibration()
	{
		AsyncResult<EmptyData>::pointer_t result(new AsyncResult<EmptyData>());
		ParamStack stack;

		execute(1070, stack, boost::bind(&EyeTrackerImpl::handleEmptyResponse, this, _2, result));

		result->waitForResult();
		result->throwIfError();
	}

	void doComputeCalibrationAsync(const async_callback_t& completedHandler)
	{
		ParamStack stack;
		execute(1070, stack, boost::bind(completedHandler, _2));
	}


	Calibration::pointer_t doGetCalibration()
	{
		AsyncResult<Calibration::pointer_t>::pointer_t result = boost::make_shared<AsyncResult<Calibration::pointer_t> >();

		ParamStack stack;
		execute(1100, stack, boost::bind(&EyeTrackerImpl::handleGetCalibrationResponse, this, _2, _3, result));

		result->waitForResult();
		result->throwIfError();

		return result->getResult();
	}

	void handleGetCalibrationResponse(
			tobii_sdk_error_code_t error,
			ParamStack& stack,
			AsyncResult<Calibration::pointer_t>::pointer_t result)
	{
		if(error)
		{
			result->setError(error);
		}
		else
		{
			try {

				Blob raw_data = stack.getBlob(0);
				Calibration::pointer_t calibration = boost::make_shared<CalibrationImpl>(raw_data);

				result->setResult(calibration);
			}
			catch (EyeTrackerException& ex) {
				result->setError(ex.getErrorCode());
			}
		}
	}

	void doSetCalibration(Calibration::pointer_t calibration)
	{
		AsyncResult<EmptyData>::pointer_t result(new AsyncResult<EmptyData>());
		ParamStack stack;
		Blob& raw_data = calibration->getRawData();
		stack.pushBlob(raw_data.getData().get(), raw_data.getLength());

		execute(1110, stack, boost::bind(&EyeTrackerImpl::handleEmptyResponse, this, _2, result));

		result->waitForResult();
		result->throwIfError();
	}

	void doStartTracking()
	{
		AsyncResult<EmptyData>::pointer_t result(new AsyncResult<EmptyData>());
		std::vector<uint32_t> emptyVec;

		ParamStack stack;
		stack.pushUInt32(1280); // 1280 is the gaze data stream
		stack.pushVector(emptyVec);
		execute(1220, stack, boost::bind(&EyeTrackerImpl::handleEmptyResponse, this, _2, result));

		result->waitForResult();
		result->throwIfError();
	}

	void doStopTracking()
	{
		AsyncResult<EmptyData>::pointer_t result(new AsyncResult<EmptyData>());
		ParamStack stack;
		stack.pushUInt32(1280); // 1280 is the gaze data stream

		execute(1230, stack, boost::bind(&EyeTrackerImpl::handleEmptyResponse, this, _2, result));

		result->waitForResult();
		result->throwIfError();
	}

	TrackBox::pointer_t doGetTrackBox()
	{
		AsyncResult<TrackBox::pointer_t>::pointer_t result(new AsyncResult<TrackBox::pointer_t>());
		ParamStack stack;

		execute(1400, stack, boost::bind(&EyeTrackerImpl::handleGetTrackBoxResponse, this, _2, _3, result));

		result->waitForResult();
		result->throwIfError();

		return result->getResult();
	}

	void handleGetTrackBoxResponse(
			tobii_sdk_error_code_t error,
			ParamStack& stack,
			AsyncResult<TrackBox::pointer_t>::pointer_t result)
	{
		if(error)
		{
			result->setError(error);
		}
		else
		{
			try {
				TrackBox::pointer_t box(new TrackBox);
				uint32_t index = 0;

				box->point1 = stack.readPoint3d(index);
				box->point2 = stack.readPoint3d(index);
				box->point3 = stack.readPoint3d(index);
				box->point4 = stack.readPoint3d(index);

				box->point5 = stack.readPoint3d(index);
				box->point6 = stack.readPoint3d(index);
				box->point7 = stack.readPoint3d(index);
				box->point8 = stack.readPoint3d(index);

				result->setResult(box);
			}
			catch (EyeTrackerException& ex) {
				result->setError(ex.getErrorCode());
			}
		}
	}

	void doEnableExtension(uint32_t extensionId)
	{
		AsyncResult<EmptyData>::pointer_t result(new AsyncResult<EmptyData>());
		ParamStack stack;
		stack.pushUInt32(extensionId);

		execute(1800, stack, boost::bind(&EyeTrackerImpl::handleEmptyResponse, this, _2, result));

		result->waitForResult();
		result->throwIfError();
	}

	Extension::vector_pointer_t doGetAvailableExtensions()
	{
		AsyncResult<Extension::vector_pointer_t>::pointer_t result(new AsyncResult<Extension::vector_pointer_t>());
		ParamStack stack;

		execute(1810, stack, boost::bind(&EyeTrackerImpl::handleExtensionVectorResponse, this, _2, _3, result));

		result->waitForResult();
		result->throwIfError();

		return result->getResult();
	}

	Extension::vector_pointer_t doGetEnabledExtensions()
	{
		AsyncResult<Extension::vector_pointer_t>::pointer_t result(new AsyncResult<Extension::vector_pointer_t>());
		ParamStack stack;

		execute(1820, stack, boost::bind(&EyeTrackerImpl::handleExtensionVectorResponse, this, _2, _3, result));

		result->waitForResult();
		result->throwIfError();

		return result->getResult();
	}

	void handleExtensionVectorResponse(
			tobii_sdk_error_code_t error,
			ParamStack& stack,
			AsyncResult<Extension::vector_pointer_t>::pointer_t result)
	{
		if(error)
		{
			result->setError(error);
		}
		else
		{
			try {
				Extension::vector_pointer_t vector(new std::vector<Extension>());

				uint32_t index = 0;
				NodeProlog prolog = stack.getNodeProlog(index);
				uint32_t length = prolog.getLength() - 1;
				index++; // Skip element type
				for(uint32_t i=0; i<length; i++)
				{
					Extension ext;
					ext.protocolVersion = stack.getUInt32(index++);
					ext.extensionId = stack.getUInt32(index++);
					ext.name = stack.getString(index++);
					ext.realm = stack.getUInt32(index++);
					vector->push_back(ext);
				}


				result->setResult(vector);
			}
			catch (EyeTrackerException& ex) {
				result->setError(ex.getErrorCode());
			}
		}
	}

	void doSetXConfiguration(const XConfiguration& configuration)
	{
		AsyncResult<EmptyData>::pointer_t result(new AsyncResult<EmptyData>());
		ParamStack stack;

		stack.pushPoint3d(configuration.upperLeft);
		stack.pushPoint3d(configuration.upperRight);
		stack.pushPoint3d(configuration.lowerLeft);

		// Create list (id 256) of empty tool data (id 12345)
		NodeProlog toolDataProlog(1, 256);
		stack.pushNodeProlog(toolDataProlog);
		stack.pushUInt32(12345);

		execute(1440, stack, boost::bind(&EyeTrackerImpl::handleEmptyResponse, this, _2, result));

		result->waitForResult();
		result->throwIfError();
	}

	XConfiguration::pointer_t doGetXConfiguration()
	{
		AsyncResult<XConfiguration::pointer_t>::pointer_t result(new AsyncResult<XConfiguration::pointer_t>());
		ParamStack stack;

		execute(1430, stack, boost::bind(&EyeTrackerImpl::handleGetXConfigurationResponse, this, _2, _3, result));

		result->waitForResult();
		result->throwIfError();

		return result->getResult();
	}

	void handleGetXConfigurationResponse(
			tobii_sdk_error_code_t error,
			ParamStack& stack,
			AsyncResult<XConfiguration::pointer_t>::pointer_t result)
	{
		if(error)
		{
			result->setError(error);
		}
		else
		{
			try {
				XConfiguration::pointer_t configuration(new XConfiguration);
				uint32_t index = 0;
				configuration->upperLeft = stack.readPoint3d(index);
				configuration->upperRight = stack.readPoint3d(index);
				configuration->lowerLeft = stack.readPoint3d(index);

				result->setResult(configuration);
			}
			catch (EyeTrackerException& ex) {
				result->setError(ex.getErrorCode());
			}
		}
	}

	std::string doGetIlluminationMode()
	{
		AsyncResult<std::string>::pointer_t result(new AsyncResult<std::string>());
		ParamStack stack;

		execute(2010, stack, boost::bind(&EyeTrackerImpl::handleGetIlluminationModeResponse, this, _2, _3, result));

		result->waitForResult();
		result->throwIfError();

		return result->getResult();
	}

	void handleGetIlluminationModeResponse(
			tobii_sdk_error_code_t error,
			ParamStack& stack,
			AsyncResult<std::string>::pointer_t result)
	{
		if(error)
		{
			result->setError(error);
		}
		else
		{
			try {
				std::string mode = stack.getString(0);
				result->setResult(mode);
			}
			catch (EyeTrackerException& ex) {
				result->setError(ex.getErrorCode());
			}
		}
	}

	void doSetIlluminationMode(const std::string& illuminationMode)
	{
		AsyncResult<EmptyData>::pointer_t result(new AsyncResult<EmptyData>());
		ParamStack stack;

		stack.pushString(illuminationMode);
		execute(2020, stack, boost::bind(&EyeTrackerImpl::handleEmptyResponse, this, _2, result));

		result->waitForResult();
		result->throwIfError();
	}

	std::vector<std::string> doEnumerateIlluminationModes()
	{
		AsyncResult<std::vector<std::string> >::pointer_t result(new AsyncResult<std::vector<std::string> >());
		ParamStack stack;

		execute(2030, stack, boost::bind(&EyeTrackerImpl::handleEnumerateIlluminationModes, this, _2, _3, result));

		result->waitForResult();
		result->throwIfError();

		return result->getResult();
	}

	void handleEnumerateIlluminationModes(
			tobii_sdk_error_code_t error,
			ParamStack& stack,
			AsyncResult<std::vector<std::string> >::pointer_t result)
	{
		if(error)
		{
			result->setError(error);
		}
		else
		{
			try {
				std::vector<std::string> modes(stack.getVectorString(0));
				result->setResult(modes);
			}
			catch (EyeTrackerException& ex) {
				result->setError(ex.getErrorCode());
			}
		}
	}

	connection_t doAddFrameRateChangedListener(const framerate_changed_event_t::slot_type& listener)
	{
		return framerateChangedSig_.connect(listener);
	}

	connection_t doAddCalibrationStartedListener(const empty_event_t::slot_type& listener)
	{
		return calibrationStartedSig_.connect(listener);
	}

	connection_t doAddCalibrationStoppedListener(const empty_event_t::slot_type& listener)
	{
		return calibrationStoppedSig_.connect(listener);
	}

	connection_t doAddGazeDataReceivedListener(const gaze_data_received_event_t::slot_type& listener)
	{
		return gazeDataReceivedSig_.connect(listener);
	}

	connection_t doAddTrackBoxChangedListener(const empty_event_t::slot_type& listener)
	{
		return trackboxChangedSig_.connect(listener);
	}

	connection_t doAddXConfigChangedListener(const x_configuration_changed_event_t::slot_type& listener)
	{
		return xConfigurationChangedSig_.connect(listener);
	}

	connection_t doAddConnectionErrorListener(const connection_error_event_t::slot_type& listener)
	{
		return connectionErrorSig_.connect(listener);
	}
};

} // namespace cpp

} // namespace sdk

} // namespace tobii


#endif  // __TOBII_SDK_CPP_TRACKING_EYE_TRACKER_HPP__
