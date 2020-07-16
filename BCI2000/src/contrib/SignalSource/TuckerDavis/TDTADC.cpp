/* $BEGIN_BCI2000_LICENSE$
 * 
 * This file is part of BCI2000, a platform for real-time bio-signal research.
 * [ Copyright (C) 2000-2012: BCI2000 team and many external contributors ]
 * 
 * BCI2000 is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 * 
 * BCI2000 is distributed in the hope that it will be useful, but
 *                         WITHOUT ANY WARRANTY
 * - without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 * $END_BCI2000_LICENSE$
 */
//---------------------------------------------------------------------------
#include "TDTADC.h"
#include "BCIStream.h"

#include "PrecisionTime.h"
#include <algorithm>

using namespace std;

RegisterFilter( TDTADC, 1);

// Class constructor for TDTADC
TDTADC::TDTADC()
: mpAcquireThread( NULL ),
  acquireEventRead( NULL )
{
    mSourceCh = 0;
    mSampleBlockSize = 0;
    mSamplingRate = 0;
    nChannels = 0;
    nProcessors = 0;
    LPFfreq = 0;
    HPFfreq = 0;
    notchBW = 0;
    TDTsampleRate = 24414.0625;
    TDTgain = 1;
    blockSize = 0;
    TDTbufSize = 32768;
    curindex = 0;
    stopIndex = 0;
    indexMult = 1;
    devAddr[0] = 0;
    devAddr[1] = 0;
    ECGchannel = -1;
    ECGgain = 0;

    BEGIN_PARAMETER_DEFINITIONS
        "Source:TDT string CircuitPath= ..\\src\\contrib\\SignalSource\\TuckerDavis\\chAcquire64_RZ2.rcx 0 0 1024"
		    "//RCO circuit path (inputfile)",
        "Source:TDT int TDTBufferBlocks= 32 0 1 128 "
            "//determines the size of the TDT buffer as TDTBufferBlocks*16*blockSize",
        "Source:TDT float LPFfreq= 256 256 0 1024 "
		    "//Low Pass Filter Frequency",
        "Source:TDT float HPFfreq= 3 3 0 256"
		    "//High Pass Filter Frequency",
        "Source:TDT float notchBW= 10 10 1 30 "
		    "//60 Hz notch filter BW",
        "Source:TDT float TDTgain= 50000 1 1 100000000"
		    "//TDT pre-gain",
        "Source:TDT intlist SourceChList= 0 0 1 256 //the list of eeg channels to acquire (blank for all)",
        "Source:TDT float DigitalGain= 1 1 0 % "
            "// the gain in converting from TTL to float",
        "Source:TDT float FrontPanelGain= 1 1 0 % "
            "// the gain in converting from front panel to float",
    END_PARAMETER_DEFINITIONS


	HRESULT hr, hz;
	hr = CoInitialize(NULL);
	if (FAILED(hr)) {
		bcierr<<"Failed to initialize COM!"<<endl;
	}
	const char* rpid = "{d323a625-1d13-11d4-8858-444553540000}"; //"RPcoX.ocx"
	const char* zbid = "{79734a6c-8e6e-4998-b834-3e4e481232b0}"; //"RPcoX.ocx"
	hr = RPcoX1.CreateInstance(rpid);
	hz = ZBus.CreateInstance(zbid);
	if (FAILED(hr) || FAILED(hz)) {
		bcierr<< "Could not instantiate the TDT ActiveX controls."<<endl;
	}
	mData.clear();
}

// Class destructor
TDTADC::~TDTADC()
{
    reset();

    Halt();
}

void TDTADC::reset()
{
}
//
void TDTADC::Preflight(const SignalProperties&,	SignalProperties& outputProperties)	const
{

    int devAddrTemp[2];

    // connect to the ZBus
    ZBus->ConnectZBUS("GB");
    devAddrTemp[0] = ZBus->GetDeviceAddr(45,1);
    devAddrTemp[1] = ZBus->GetDeviceAddr(50,1);
    short tConnectType;
    if (devAddrTemp[0] > 0)
        tConnectType = 0;
    else if (devAddrTemp[1] > 0)
        tConnectType = 1;
    else
        bcierr << "There does not seem to be a compatible TDT device on the rack. Quitting."<<endl;


    string circuit = Parameter("CircuitPath");

    bciout <<"Connecting to	TDT..."<<endl;
    switch (tConnectType){
        case 0:
            if (!RPcoX1->ConnectRX5("GB",	1))
                bcierr << "Error connecting	to the RX5.	Use	the	zBuzMon	to ensure you are connected, and that you are using an RX5 Pentusa." <<endl;
            break;
        case 1:
            if (!RPcoX1->ConnectRZ2("GB",	1))
                bcierr << "Error connecting	to the RZ2.	Use	the	zBuzMon	to ensure you are connected, and that you are using an RX5 Pentusa." <<endl;
            break;
        case 2:
            bcierr << "Unknown connection (this is a software error!)"<<endl;
    }

    bciout <<"Loading RCO/RCX file..."<<endl;
	if (!RPcoX1->LoadCOF(circuit.c_str()))
    {
        bcierr << "Error loading RCO file. Check the file name and path, and that your Pentusa has 5 processors."<<endl;
        //error
    }
    int nTags = RPcoX1->GetNumOf("ParTag");
	vector<string> tagList, tmpDataTags;
	for (int i = 0; i < nTags; i++){
		string tagName = RPcoX1->GetNameOf("ParTag", i);
		if (tagName.substr(0,5).find("index") != string::npos && tagName.length() > 5)
			tagList.push_back(tagName);
		if (tagName.substr(0,4).find("data") != string::npos && tagName.length() > 4){
			tmpDataTags.push_back(tagName.substr(4));
		}
	}
	if (tmpDataTags.size() == 0)
		bcierr << "The TDT *.rcx file must contain at least one data/index tag combination (see the TDT Client documentation for details.)"<<endl;
	//for each data tag, make sure there is a corresponding "index"
	for (size_t i =0; i < tmpDataTags.size(); i++){
		bool found = false;
		string tmpTag = "index" + tmpDataTags[i];
		for (size_t j = 0; j < tagList.size() && !found; j++){
			if (tagList[j] == tmpTag)
				found = true;
		}
		if (!found)
			bcierr << "The data tag \'data"<<tmpDataTags[i]<<"\'is missing a corresponding "<<tmpTag<<". Modify the rcx file accordingly."<<endl;
	}

	RPcoX1->Halt();
    //RPcoX2->Halt();

  outputProperties = SignalProperties(Parameter( "SourceCh"	), Parameter( "SampleBlockSize" ), SignalType::float32);
}

void TDTADC::Initialize(const SignalProperties&, const SignalProperties&)
{
	//stop the acquire thread
    if (mpAcquireThread != NULL)
    {
        mpAcquireThread->Terminate();
        delete mpAcquireThread;
        mpAcquireThread = NULL;
    }
	mpAcquireThread = new AcquireThread(this);

	mSourceCh	= Parameter("SourceCh");
	mSampleBlockSize = Parameter("SampleBlockSize");
	mSamplingRate =    static_cast<int>(Parameter("SamplingRate").InHertz());
	LPFfreq	= Parameter("LPFfreq");
	HPFfreq	= Parameter("HPFfreq");
	notchBW	= Parameter("notchBW");
  blockSize =	mSampleBlockSize;
    TDTgain = Parameter("TDTgain");
	mDigitalGain = (float)Parameter("DigitalGain");
    mFrontPanelGain = (float)Parameter("FrontPanelGain");
    TDTbufBlocks = Parameter("TDTBufferBlocks");

    TDTbufSize = TDTbufBlocks*blockSize*16;

    int nSamplesPerSec = static_cast<int>(floor(TDTsampleRate / mSamplingRate));
    double nSamplingRate = TDTsampleRate / nSamplesPerSec;
    bciwarn <<"The actual sampling rate is "<<nSamplingRate <<" Hz"<<endl;
	//mOffset	= 0;

	string LPFfreqTag =	"LPFfreq";
	string HPFfreqTag =	"HPFfreq";
	string notchBWTag =	"notchBW";
	string blockSizeTag	= "blkSize";
    string TDTgainTag = "TDTgain";
    string nPerTag = "nPer";
    string digGainTag = "DigGain";
    string frontPanelGainTag = "FrontPanelGain";
    string TDTBufSizeTag = "bufSize";

    // connect to the ZBus
    ZBus->ConnectZBUS("GB");
    devAddr[0] = ZBus->GetDeviceAddr(45,1);
    devAddr[1] = ZBus->GetDeviceAddr(50,1);
    if (devAddr[0] > 0)
        connectType = 0;
    else if (devAddr[1] > 0)
        connectType = 1;
    else
        bcierr << "There does not seem to be a compatible TDT device on the rack. Quitting."<<endl;

	// set the number of channels
	// the real	number of channels should be a multiple	of four
    int	valuesToRead = mSampleBlockSize*16;

	reset();
	// set filtering stuff
	if (!RPcoX1->SetTagVal(LPFfreqTag.c_str(),	static_cast<float>(LPFfreq)))
	{
		bcierr << "Error setting LPF tag." << endl;
	}
	if (!RPcoX1->SetTagVal(HPFfreqTag.c_str(),	static_cast<float>(HPFfreq)))
	{
		bcierr << "Error setting HPF tag." << endl;
	}
	if (!RPcoX1->SetTagVal(notchBWTag.c_str(),	static_cast<float>(notchBW)))
	{
		bcierr << "Error setting notch BW tag."	<< endl;
	}
    if (!RPcoX1->SetTagVal(TDTgainTag.c_str(),	static_cast<float>(TDTgain)))
	{
		bcierr << "Error setting TDT pre-gain."	<< endl;
	}

    if (!RPcoX1->SetTagVal(nPerTag.c_str(), static_cast<float>(nSamplesPerSec)))
	{
		bcierr << "Error setting TDT sample rate." << endl;
	}

    if (!RPcoX1->SetTagVal(digGainTag.c_str(), mDigitalGain))
	{
		bciwarn << "Error setting Digital Gain gain." << endl;
	}

    if (!RPcoX1->SetTagVal(frontPanelGainTag.c_str(), mFrontPanelGain))
	{
		bciwarn << "Error setting Front Panel gain." << endl;
	}

    if (!RPcoX1->SetTagVal(TDTBufSizeTag.c_str(), static_cast<float>(TDTbufSize)))
	{
		bciwarn << "Error setting TDT Buffer Size." << endl;
	}



    // reset the hardware and all conditions


	mThreadBlock = mProcBlock = 0;
    ResetEvent( acquireEventRead );
    mpAcquireThread->Start();
}

void TDTADC::Halt()
{
	//bciout <<"Halting the TDT..."<<endl;
	if (mpAcquireThread != NULL)
    {
        mpAcquireThread->Terminate();
        delete mpAcquireThread;
        mpAcquireThread = NULL;
    }
	// Halt	the	TDT
}

// This	is the meat	of the class; it reads the data	from the TDT and returns it
void TDTADC::Process(const GenericSignal&, GenericSignal& outputSignal)
{


	while(mProcBlock >= mThreadBlock) // && acquireEventRead != NULL
        WaitForSingleObject(acquireEventRead, 1000);
	ResetEvent( acquireEventRead );

	for (int sample = 0; sample < outputSignal.Elements(); ++sample)
	{
		for (int channel = 0; channel < outputSignal.Channels(); ++ channel)
		{
			outputSignal( channel, sample ) = mpAcquireThread->getNextValue();
		}
	}
	mProcBlock++;


}
float TDTADC::AcquireThread::getNextValue()
{
    float value;

    value = mData[mBufferReadPos];
    mBufferReadPos = (mBufferReadPos+1) % mBufferSize;
    return value;
}

int TDTADC::AcquireThread::OnExecute()
{
	int	valuesToRead = amp->mSampleBlockSize*16;

    int stopIndex, curIndex, curBufPos;
	init();

	while( !this->Terminating() && this->Running())
	{
		stopIndex = mOffset + valuesToRead;
		if (amp->RPcoX1->GetStatus() < 5)
		{
			bcierr << "TDT system is not connected and/or not running!"<<endl;
			return 0;
		}
		string waitTag = string("index" + mDataTags[0]);
		curIndex = static_cast<int>(amp->RPcoX1->GetTagVal(waitTag.c_str()));

		int waitCount=0;
		if (stopIndex <= amp->TDTbufSize)
		{
			while (curIndex <= stopIndex && curIndex > mOffset)
			{
				curIndex = static_cast<int>(amp->RPcoX1->GetTagVal(waitTag.c_str()));
				Sleep(1);
			}
		}
		else
		{
			stopIndex = stopIndex % (amp->TDTbufSize);
			// this needs to be updated for the buffer wrap-around in the TDT
			bool done = false;
			while (!done)
			{
				curIndex = static_cast<int>(amp->RPcoX1->GetTagVal(waitTag.c_str()));
				if  (curIndex >= stopIndex && curIndex < (amp->TDTbufSize - valuesToRead))
					done = true;
				Sleep(1);
			}
		}

		for (size_t i = 0; i < mDataTags.size(); i++){
			string dataTag = "data" + mDataTags[i],
				indexTag = "index" + mDataTags[i];
			if (!amp->RPcoX1->ReadTag(dataTag.c_str(), mDataBuf[i], mOffset, valuesToRead))
				bcierr << "Error reading data from "<< dataTag <<"."<<endl;
		}
		mOffset = (mOffset + valuesToRead) % (amp->TDTbufSize);


		for (size_t i = 0; i < mDataBuf.size(); i++){
			for (int ch =0; ch < 16; ch++){
				int curCh = static_cast<int>( i )*16 + ch;
				if (find(mChList.begin(), mChList.end(), curCh) == mChList.end())
					continue;
				for (int sample = 0; sample < amp->mSampleBlockSize; sample++){
					curBufPos = mBufferWritePos + curCh + sample*amp->mSourceCh;
					mData[curBufPos] = mDataBuf[i][sample*16 + mChList[ch]%(16)];
					//outputSignal(curCh, sample) = mData[i][sample*16 + mChList[ch]%(16)];
				}
			}
		}
		mBufferWritePos = (mBufferWritePos+ amp->mSourceCh*amp->mSampleBlockSize) % mBufferSize;
		SetEvent( amp->acquireEventRead );
		amp->mThreadBlock++;
	}
	reset();
	return 0;
}

void TDTADC::AcquireThread::init()
{
	int	valuesToRead = amp->mSampleBlockSize*16;
	//get the number of tags
	int nTags = amp->RPcoX1->GetNumOf("ParTag");
	vector<string> tagList;
	mDataTags.clear();
	for (int i = 0; i < nTags; i++){
		string tagName = amp->RPcoX1->GetNameOf("ParTag", i);
		if (tagName.substr(0,5).find("index") != string::npos && tagName.length() > 5)
			tagList.push_back(tagName);
		if (tagName.substr(0,4).find("data") != string::npos && tagName.length() > 4){
			mDataTags.push_back(tagName.substr(4));
		}
	}
	//for each data tag, make sure there is a corresponding "index"
	for (size_t i =0; i < mDataTags.size(); i++){
		bool found = false;
		string tmpTag = "index" + mDataTags[i];
		for (size_t j = 0; j < tagList.size() && !found; j++){
			if (tagList[j] == tmpTag)
				found = true;
		}
		if (!found)
			bcierr << "The data tag \'data"<<mDataTags[i]<<"\'is missing a corresponding "<<tmpTag<<". Modify the rcx file accordingly."<<endl;
		else{
			//initialize the data buffers
			float *tmp = new float[valuesToRead];
			mDataBuf.push_back(tmp);
		}
	}

	//the channels will be read in alphabetical order
	sort(mDataTags.begin(), mDataTags.end());

	mChList.clear();
	if (amp->Parameter("SourceChList")->NumValues() == 0){
		int curChOffset = 0;
		for (int i = 0; i < amp->Parameter("SourceCh"); i++)
			mChList.push_back(i);
	}
	else{
		for (int i = 0; i < amp->Parameter("SourceChList")->NumValues(); i++)
            mChList.push_back(static_cast<int>(amp->Parameter("SourceChList")(i)-1));
	}
	mOffset = 0;

	if (mData != NULL)
		delete [] mData;

	mBufferSize = amp->TDTbufSize;
	mData = new float[mBufferSize];

	mBufferReadPos = 0;
    mBufferWritePos = 0;


	// Start TDT
	amp->RPcoX1->Run();
    amp->ZBus->zBusTrigA(0, 0, 5);
}
//---------------------------------------------------------------------------

void TDTADC::AcquireThread::reset()
{
	for (size_t i = 0; i < mDataBuf.size(); i++)
		delete [] mDataBuf[i];
	mDataBuf.clear();
	if (mData != NULL)
		delete [] mData;
	mData = NULL;
}
void TDTADC::dropSamples(GenericSignal& outputSignal)
{
    //just write zeros into the data
    for (int ch =0; ch < mSourceCh; ch++)
    {
        for (int sample = 0; sample < mSampleBlockSize; sample++)
        {
            outputSignal(ch, sample) = 0;
        }
    }
}

