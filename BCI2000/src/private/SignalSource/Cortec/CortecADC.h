////////////////////////////////////////////////////////////////////////////////
// Authors: Adamek Markus
// Description: CortecADC header
////////////////////////////////////////////////////////////////////////////////

#ifndef INCLUDED_CORTECADC_H  // makes sure this header is not included more than once
#define INCLUDED_CORTECADC_H
#include "BufferedADC.h"
#include "bicapid.imports.h"
#include "Thread.h"
#include "Mutex.h"
#include <deque>

using namespace cortec::implantapi;




class CortecADC : public BufferedADC
{

public:
	CortecADC();
	~CortecADC();
	void OnPublish() override;
	void OnAutoConfig() override;
	void OnPreflight(SignalProperties& Output) const override;
	void OnInitialize(const SignalProperties& Output) override;
	void OnStartAcquisition() override;
	void DoAcquire(GenericSignal& Output) override;
	void OnStopAcquisition() override;

private:

	typedef struct _stim
	{
		float amplitude;
		float duration;
	} StimParameters;
	void ClearStimMap();
	void AddStimDefinitionToMap(int identifier, std::vector<StimParameters> params, int type);

	IStimulationFunction* GetStimDefinitonFromId(int identifier);

	IStimulationFunction* createStimulationFunction(std::vector<StimParameters>, int type);

	bool checkStimulationParameters(const double amplitude, const uint64_t pulseWidth, const int type);

	bool ConnectToImplant();
	bool IsConnected();
	void StartStimulation(int stimId);
	typedef enum { Voltage = 0, Current = 1 } StimType;



	int mNumberOfSignalChannels;
	std::map<int, IStimulationFunction*> mStimParameters;
	std::map<int, IStimulationCommand*> mStimExecutionFunction;
	std::unique_ptr<IImplantFactory> mImplantFactory;
	std::unique_ptr<IImplant> mImplant;
	std::unique_ptr<CImplantInfo> mImplantInfo;
	std::unique_ptr<IStimulationCommandFactory> mStimCommandFactory;
	int mBlockSize;
	int mSamplingRate;
	
	Mutex queueMutex;
	std::deque<float> mData;
	
	class CortecDataListener :public IImplantListener
	{
	public:
		CortecDataListener(CortecADC* parent);
	private:
		CortecADC* parent;

		void onData(const std::vector<CSample>* samples) override;

		void onStimulationStateChanged(const bool isStimulating) override;


		void onMeasurementStateChanged(const bool isMeasuring) override;


		void onConnectionStateChanged(const connection_info_t& info)override;

		void onVoltageChanged(const double voltage)override;
		void onTemperatureChanged(const double temperature) override;

		void onHumidityChanged(const double humidity) override;

		void onError(const std::exception& err) override;
	};
	friend class CortecDataListener;

	CortecDataListener mListener;



};



#endif // INCLUDED_CORTECADC_H
