////////////////////////////////////////////////////////////////////////////////
// Authors:
// Description: CortecADC implementation
////////////////////////////////////////////////////////////////////////////////

#include "CortecADC.h"
#include "BCIStream.h"
#include "BCIEvent.h"



// In order to help you write a source module, exchange of information
// between amplifier and the BCI2000 source module is indicated by the use of
// macros.
// Once you are done with writing the source module, each occurrence of
// GET_FROM_AMP_API(), CALL_AMP_API(), and AMP_API_SYNC_GET_DATA() should
// have been replaced with actual calls to the amplifier API, or constants.
// By removing or disabling those macros' definitions below, you can then
// make sure that the compiler will notify you of any oversights.

// Depending on the kind of amplifier you have, occurrences of GET_FROM_AMP_API()
// may be read through the amplifier API, or obtained from documentation.

using namespace std;


// Make the source filter known to the framework.
RegisterFilter(CortecADC, 1); // ADC filters must be registered at location "1" in order to work.

CortecADC::CortecDataListener::CortecDataListener(CortecADC * p)
{
	parent = p;
}


void CortecADC::CortecDataListener::onData(const std::vector<CSample>* samples)
{
	parent->queueMutex.Acquire();
	for (int i = 0; i < samples->size(); ++i)
	{

		for (int s = 0; s < parent->mNumberOfSignalChannels; ++s)
			parent->mData.push_back(samples->at(i).getMeasurements()[s]);
	}
	delete samples;
	parent->queueMutex.Release();
}

void CortecADC::CortecDataListener::onStimulationStateChanged(const bool isStimulating)
{
}

CortecADC::CortecADC() :
	mImplantFactory(createImplantFactory()),
	mImplant(nullptr), mImplantInfo(nullptr),
	mStimCommandFactory(createStimulationCommandFactory()),
	mListener(this)
{

}

CortecADC::~CortecADC()
{
}

void
CortecADC::OnPublish()
{
	// Declare any parameters that the filter needs....

	BEGIN_PARAMETER_DEFINITIONS

		"Source:Signal%20Properties int SourceCh= auto "
		"auto 1 % // number of digitized and stored channels",

		"Source:Signal%20Properties int SampleBlockSize= auto "
		"auto 1 % // number of samples transmitted at a time",

		"Source:Signal%20Properties float SamplingRate= auto "
		"auto 0.0 % // sample rate, defined by connected device (noedit)",

		"Source:Signal%20Properties list SourceChGain= 1 auto "
		" // physical units per raw A/D unit",

		"Source:Signal%20Properties list SourceChOffset= 1 auto "
		" // raw A/D offset to subtract, typically 0",

		"Source:Signal%20Properties list ChannelNames= 1 auto "
		" // names of amplifier channels",

		//Stimulation Parameters
		"Stimulation:Stimulation int Enable= 1 1 0 1 "
		" //Enable/Disable Stimulation (boolean)",

		"Stimulation:Stimulation matrix StimulationParameters= 1 1 auto "
		"// Define Stimulation parameters as followed: StimulatonId, StimulationType (voltage/current),DurationDuration1, amplitude1,DurationDuration2, amplitude2 ... ",

		"Stimulation:Stimulation matrix StimulationTriggers= 1 1 auto "
		"// Define a trigger for stimulation, convention is: StimulatonId, trigger equation,repeats, anode electrodes, kathode electrodes",


		//================READ ONLY PARAMETERS ==========================

		//	 "Source:DeviceInformation matrix ExternalDeviceInformation= "
		//	 "// Type of connected external device (readonly)(noedit)",


		//	 "Source:DeviceInformation string ImplantedDeviceInformation= "
		//	 "// Type of connected implant (readonly)(noedit)",

		END_PARAMETER_DEFINITIONS

		// ...and likewise any state variables.

		// IMPORTANT NOTE ABOUT BUFFEREDADC AND STATES:
		// * BCI2000 States, or "state variables", are additional data channels stored alongside signal data,
		//   with a resolution of one value per source signal sample.
		// * A State is much like a source signal channel, but differs from a source signal channel in the
		//   following respects:
		//   + You choose the number of bits used to represent the State's value, up to 64.
		//   + Unlike source signals, States are transmitted through all stages of processing, and their values
		//     may be modified during processing, allowing all parts of the system to store state information in
		//     data files.
		//   + When loading a BCI2000 data file for analysis, States appear separately, with names, which is
		//     typically more useful for trigger information than being just another channel in the signal.
		//   + States may be set synchronously from inside a filter's Process() handler, or asynchronously using
		//     a "bcievent" interface.
		//   + If your amplifier provides a digital input, or another kind of "trigger" information, it makes sense
		//     to store this information in form of one or more States. From a user perspective, it is probably most
		//     useful if physically distinct amplifier input sockets correspond to States, and single bits to certain
		//     lines or values communicated through such a socket.
		//   + If the amplifier API asynchronously reports trigger information through a callback mechanism, you
		//     should register a callback that uses the "bcievent" interface to set states asynchronously.
		//     This example provides a "MyAsyncTriggers" "event state", and a sample callback function.
		//   + If the amplifier API sends trigger information in the same way as it does for signal data, you should
		//     use a "State channel" to represent it in your source module. This example provides a "MySyncTriggers"
		//     state, and writes to it when acquiring data.

}

void
CortecADC::OnAutoConfig()
{
	// The user has pressed "Set Config" and some parameters may be set to "auto",
	// indicating that they should be set from the current amplifier configuration.
	// In this handler, we behave as if any parameter were set to "auto", and the
	// framework will transparently make sure to preserve user-defined values.
	if (!ConnectToImplant())
		return;
	auto implantInfo=mImplant->getImplantInfo();
	Parameter("SamplingRate") = implantInfo->getSamplingRate();
	int channels = implantInfo->getChannelCount(); //number of channels?
	delete implantInfo;
	Parameter("SourceCh") = channels; // Set SourceCh in case of "auto"
	// If SourceCh != auto but e.g. SourceChGain != auto, we need to use the actual
	// rather than the auto-set value of SourceCh in order to consistently configure
	// SourceChGain.
	// For this purpose, an ActualParameter() call will retrieve a parameter's current
	// value, no matter whether auto-configured or user-configured.
	channels = ActualParameter("SourceCh");
	Parameter("ChannelNames")->SetNumValues(channels);
	Parameter("SourceChGain")->SetNumValues(channels);
	Parameter("SourceChOffset")->SetNumValues(channels);
	for (int i = 0; i < channels; ++i)
	{
		// For EEG amplifiers, channel names should use 10-20 naming if possible.
		const char* name = 0;
		Parameter("ChannelNames")(i) << "CH" << i; // Omit "<< i" if channel names are unique.

		double gainFactor = 1.0;
		Parameter("SourceChGain")(i) << gainFactor << "muV"; // Always provide correct physical unit for documentation and display purposes.
		// For most amplifiers, offset removal is done within the amp hardware or driver. Otherwise, you may set this to a nonzero value.
		Parameter("SourceChOffset")(i) = 0;

		Parameter("StimulationParameters")->SetNumColumns(4);
		Parameter("StimulationParameters")->SetNumRows(0);
		Parameter("StimulationTriggers")->SetNumColumns(5);
		Parameter("StimulationTriggers")->SetNumRows(0);
	}

	int samplesPerPacket = 1;
	Parameter("SampleBlockSize") = samplesPerPacket;
}

void
CortecADC::OnPreflight(SignalProperties& Output) const
{

	// Internally, signals are always represented by GenericSignal::ValueType == double.
	// Here, you choose the format in which data will be stored, which may be
	// int16, int32, or float32.
	// Typically, you will choose the format that your amplifier natively provides, in
	// order to avoid loss of precision when converting, e.g., int32 to float32.
	SignalType sigType = SignalType::float32;

	int samplesPerBlock = Parameter("SampleBlockSize");
	int numberOfSignalChannels = Parameter("SourceCh");
	Output = SignalProperties(numberOfSignalChannels, samplesPerBlock, sigType);
	// Note that the CortecADC instance itself, and its members, are read-only during the
	// Preflight phase---note the "const" at the end of the OnPreflight prototype above.
	// Any methods called by OnPreflight must also be declared as "const" in the same way.

	auto stimParams = Parameter("StimulationParameters");
	auto stimtriggers = Parameter("StimulationTriggers");
	if ((stimParams->NumRows() != 0) && (stimParams->NumColumns() >= 4))
		bcierr << "Stim Parameters must have at least 4 Columns: \n Identifier, type of stimulation, Stimulus Duration1, amplitude1" << std::endl;

	if ((stimParams->NumColumns() % 2) != 0)
		bcierr << "Stim Parameters must be an even number, check for incomplete duration/amplitude pairs" << std::endl;
	if ((stimtriggers->NumRows() != 0) && (stimtriggers->NumColumns() != 5))
		bcierr << "Stim Parameters must have 2 Columns: \n Identifier, stimulus equation, repeats, anode electrode, kathode electrode" << std::endl;


	std::set<int> identifierList;
	//check if each stimulus is unique
	for (int i = 0; i < stimParams->NumRows(); i++)
	{
		if (!identifierList.insert(stimParams(i, 0)).second)
			bcierr << "Identifier values must be unique!" << std::endl;
	}

	//check if each trigger is matched to an existing stimulus
	for (int i = 0; i < stimtriggers->NumRows(); i++)
	{
		if (identifierList.find(stimtriggers(i, 0)) == identifierList.end())
			bcierr << "Identifier must be defined!" << std::endl;
	}


}

bool CortecADC::ConnectToImplant()
{
	std::vector<CExternalUnitInfo*> externalDevices = mImplantFactory->getExternalUnitInfos();
	if (externalDevices.size() == 0)
	{
		mImplant.reset();
		bcierr << "Could not find an external Cortec Device!" << std::endl;
		return false;
	}

	if (externalDevices.size() > 1)
		bciwarn << "Found more than 1 connected external Cortec Device!, first device will be used: (ID):" << externalDevices[0]->getDeviceId() << std::endl;


	std::unique_ptr<CImplantInfo> impInfo(mImplantFactory->getImplantInfo(*externalDevices[0]));
	mImplant.reset(mImplantFactory->create(*externalDevices[0], *impInfo));
	//cleanup
	for (int i = 0; i < externalDevices.size(); ++i)
		delete externalDevices[i];

	return true;
}

bool CortecADC::IsConnected()
{
	return (mImplant != nullptr);
}

void
CortecADC::OnInitialize(const SignalProperties& Output)
{
	// The user has pressed "Set Config" and all Preflight checks have passed.
	// The system will now transition into an "Initialized" state.


	// The signal properties can no longer be modified, but the const limitation has gone, so
	// the CortecADC instance itself can be modified. Allocate any memory you need
	// store any information you need in private member variables.
	if (!IsConnected())
		bcierr << "No device connected!" << std::endl;

	
	mNumberOfSignalChannels = Parameter("SourceCh");
	auto stimParams = Parameter("StimulationParameters");
	auto stimtriggers = Parameter("StimulationTriggers");
	ClearStimMap();
	std::vector<StimParameters> stimvec;

	for (int i = 0; i < stimParams->NumRows(); i++)
	{   //(int identifier, float amplitude,float duration, int type)
		//AddStimDefinitionToMap(stimParams(i, 0), stimParams(i, 1), stimParams(i, 1), stimParams(i, 2));
		//col id=0 -> identifier for stimulus
		//col id=1 -> stimulus type
		//col id=3+2*i ->  Duration of stimulus
		//col id=2+2*(i-1) -> amplitude of stim
		for (int stid = 0; stid < (stimParams->NumRows() - 2) / 2; ++stid)
		{
			float dur = stimParams(i, 3 + stid * 2).InMilliseconds();
			float ampl = stimParams(i, 2 + (stid + 1) * 2);
			StimParameters param;
			param.amplitude = ampl;
			param.duration = dur;
			stimvec.push_back(param);

		}
		AddStimDefinitionToMap(stimParams(i, 0), stimvec, stimParams(i, 1));

	}

	for (int i = 0; i < stimtriggers->NumRows(); i++)
	{
		//StimulatonId, trigger equation, repeats, anode electrodes, kathode electrodes
		//lets assume for now that there is only one anode and one kathode!
		IStimulationFunction* stimFunction(GetStimDefinitonFromId(stimtriggers(i, 0)));
		stimFunction->setRepetitions(stimtriggers(i, 2));
		std::set<uint32_t> sourceChannels;
		std::set<uint32_t> destChannels;
		sourceChannels.insert(stimtriggers(i, 3));
		destChannels.insert(stimtriggers(i, 4));
		stimFunction->setVirtualStimulationElectrodes(sourceChannels, destChannels);

		IStimulationCommand* stimCmd = mStimCommandFactory->createStimulationCommand();
		stimCmd->append(stimFunction);
		mStimExecutionFunction.insert(std::pair<int, IStimulationCommand*>(i, stimCmd));
		//mStimExecutionFunction.insert(std::pair<int, IStimulationFunction*>(i, ));
	}


	mImplant->registerListener(&mListener);
	auto impInfo = mImplant->getImplantInfo();
	mSamplingRate = impInfo->getSamplingRate();
	
	bciout << "Device Type:" << impInfo->getDeviceType() << std::endl;
	bciout << "Device ID:" << impInfo->getDeviceId() << std::endl;
	bciout << "Firmware Version:" << impInfo->getFirmwareVersion() << std::endl;
	
	delete impInfo;

	

}

IStimulationFunction* CortecADC::GetStimDefinitonFromId(int identifier)
{
	return mStimParameters.find(identifier)->second;
}

bool CortecADC::checkStimulationParameters(const double amplitude, const uint64_t pulseWidth, const int type)
{


	if (pulseWidth < 10)
	{
		bcierr << "Pulsewidth duration minimum is 10ms" << std::endl;
		return false;
	}
	if (pulseWidth > 2550)
	{
		bcierr << "Puls cannot be longer than 2550ms" << std::endl;
		return false;
	}

	return true;
}

IStimulationFunction* CortecADC::createStimulationFunction(std::vector<StimParameters> stimvec, int type)
{

	for (int i = 0; i < stimvec.size(); ++i) //check parameters
	{
		if (!checkStimulationParameters(stimvec[i].amplitude, stimvec[i].amplitude, type))
			throw std::exception("Invalid stimulation parameters");

	}
	// Defining StimulationFunction with fixed duration of 12500micros 
	IStimulationFunction* pulseStimFunction = mStimCommandFactory->createStimulationFunction();
	//mStimCommandFactory->
	for (int i = 0; i < stimvec.size(); ++i)
	{
		IStimulationAtom* pwAtom = mStimCommandFactory->createRectStimulationAtom(stimvec[i].amplitude, stimvec[i].duration);
		pulseStimFunction->append(pwAtom);
	}

	return pulseStimFunction;
}

void CortecADC::StartStimulation(int stimId)
{
	mImplant->stopStimulation(); //safety, lets stop old stimulation before we start the new one
	bciwarn << "Not able to set stimulation mode (voltage or current)" << std::endl;
	mImplant->startStimulation(mStimExecutionFunction.at(stimId));
}

void
CortecADC::OnStartAcquisition()
{
	mImplant->startMeasurement();
}

void
CortecADC::DoAcquire(GenericSignal& Output)
{

	while (true) //wait until data is available
	{
		
		queueMutex.Acquire();
		if (mData.size() > Output.Elements()*Output.Channels())
		{
			for (int sample = 0; sample < Output.Elements(); sample++)
			{
				// just an example, unlikely to work for a certain amplifier
				Output(mNumberOfSignalChannels, sample) = mData.back();
				mData.pop_back();
			}
			return;
		}
		queueMutex.Release();
		Sleep(0);
	}

}

void
CortecADC::OnStopAcquisition()
{
	// This method is called from the acquisition thread just before it exits.  Use this method
	// to shut down the amplifier API (undoing whatever was done in OnStartAcquisition).
	// Immediately after this returns, the system will go into an un-initialized state and
	// OnHalt will be called in the main thread.
	mImplant->stopMeasurement();

}

void CortecADC::ClearStimMap()
{

#pragma message("Maybe cause memory leaks because we need to clear content of IStimulationCommand")

	for (auto it = mStimExecutionFunction.begin(); it != mStimExecutionFunction.end(); it++)
	{
		delete it->second;
	}
	mStimExecutionFunction.clear();
	for (auto it = mStimParameters.begin(); it != mStimParameters.end(); it++)
	{
		
		delete it->second; //free memory of stim parameter
	}
	mStimParameters.clear();

}

void CortecADC::AddStimDefinitionToMap(int identifier, std::vector<StimParameters> params, int type)
{
	if (type > 1)
		bcierr << "Only 1 or 0 are allowed stimulation types!" << std::endl;

	IStimulationCommand* stimCmd = mStimCommandFactory->createStimulationCommand();


	IStimulationFunction* stimFunction(createStimulationFunction(params, type));

	mStimParameters.insert(std::pair<int, IStimulationFunction*>(identifier, stimFunction));
}

void CortecADC::CortecDataListener::onMeasurementStateChanged(const bool isMeasuring)
{
}

void CortecADC::CortecDataListener::onConnectionStateChanged(const connection_info_t & info)
{
}

void CortecADC::CortecDataListener::onVoltageChanged(const double voltage)
{
}

void CortecADC::CortecDataListener::onTemperatureChanged(const double temperature)
{
}

void CortecADC::CortecDataListener::onHumidityChanged(const double humidity)
{
}

void CortecADC::CortecDataListener::onError(const std::exception & err)
{
}
