////////////////////////////////////////////////////////////////////////////////
// $Id: vAmpADC.cpp 5818 2018-11-09 15:50:15Z mellinger $
// Author: jadamwilson2@gmail.com
// Description: BCI2000 Source Module for BrainProducts V-Amp devices.
//
// $BEGIN_BCI2000_LICENSE$
// 
// This file is part of BCI2000, a platform for real-time bio-signal research.
// [ Copyright (C) 2000-2012: BCI2000 team and many external contributors ]
// 
// BCI2000 is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software
// Foundation, either version 3 of the License, or (at your option) any later
// version.
// 
// BCI2000 is distributed in the hope that it will be useful, but
//                         WITHOUT ANY WARRANTY
// - without even the implied warranty of MERCHANTABILITY or FITNESS FOR
// A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License along with
// this program.  If not, see <http://www.gnu.org/licenses/>.
// 
// $END_BCI2000_LICENSE$
////////////////////////////////////////////////////////////////////////////////
#include "PCHIncludes.h"
#pragma hdrstop

#include "vAmpADC.h"
#include <stdlib.h>

#include "defines.h"
#include "BufferedADC.h"

#include <algorithm>
using namespace std;

// Register the source class with the framework.
RegisterFilter( vAmpADC,  1 );

// **************************************************************************
// Function:   vAmpADC
// Purpose:    The constructor for the vAmpADC
// **************************************************************************
vAmpADC::vAmpADC()
: mNumEEGchannels( 0 ),
 mTimeoutMs( 0 ),
 mHighSpeed( false ),
 mImpedanceMode( false ),
 mAcquire(NULL)
{
  // add all the parameters that this ADC requests to the parameter list
  BEGIN_PARAMETER_DEFINITIONS
   "Source int SourceCh=      19 16 1 128 "
     "// number of digitized channels total",
   "Source floatlist SourceChOffset= 19 "
      "0 0 0 0 "
      "0 0 0 0 "
      "0 0 0 0 "
      "0 0 0 0 "
      "0 0 0 "
      "0 % % // Offset for channels in A/D units",
   "Source floatlist SourceChGain= 19 "
      "1 1 1 1 "
      "1 1 1 1 "
      "1 1 1 1 "
      "1 1 1 1 "
      "1 1 1 "
      "1 % % // gain for each channel",
   "Source int SampleBlockSize= 100 5 1 20000 "
     "// number of samples per block",
   "Source int SamplingRate=    2000 2000 100 20000 "
     "// the signal sampling rate",
   "Source intlist SourceChList= 0 0 1 128 "
     "// list of channels to digitize",
   "Source float HPFilterCorner= 0.1 0 0 % "
      "// high-pass filter corner (use 0 or leave blank to disable)",
   "Source int AcquisitionMode=      0 0 0 4 "
        "// data acquisition mode: "
      " 0: analog signal acquisition,"
      " 1: high-speed signal acquisition,"
      " 2: calibration,"
      " 3: impedance,"
      " 4: high-speed calibration"
      " (enumeration)",
   "Source matrix Impedances= 1 1 not%20measured % % % "
        "// impedances as measured by the amplifier(s)"
        "--rows represent amplifiers, columns represent channels",
  END_PARAMETER_DEFINITIONS
}


vAmpADC::~vAmpADC()
{
  Halt();
}


// **************************************************************************
// Function:   Preflight
// Purpose:    Checks parameters for availability and consistence with
//             input signal properties; requests minimally needed properties for
//             the output signal; checks whether resources are available.
// Parameters: Input and output signal properties pointers.
// **************************************************************************
void vAmpADC::OnPreflight(SignalProperties& outSignalProperties ) const
{
  // Requested output signal properties.
  bool tHighSpeed = Parameter("AcquisitionMode") == 1 || Parameter("AcquisitionMode") == 4;
  SignalType signalType = SignalType::float32;
  outSignalProperties = SignalProperties(
    Parameter( "SourceCh" ), Parameter( "SampleBlockSize" ), signalType );

  // Parameter consistency checks: Existence/Ranges and mutual Ranges.
  if ( Parameter("SourceChList")->NumValues() > 0 )
  {
    if (Parameter("SourceChList")->NumValues() != Parameter("SourceCh"))
    {
      bcierr << "# elements in SourceChList must match total # channels (SourceCh)" <<endl;
      return;
    }
  }
  if( Parameter("SourceChGain")->NumValues() != Parameter("SourceCh"))
  {
    bcierr << "# elements in SourceChGain has to match total # channels" << endl;
    return;
  }
  if( Parameter("SourceChOffset")->NumValues() != Parameter("SourceCh"))
  {
    bcierr << "# elements in SourceChOffset has to match total # channels" << endl;
    return;
  }

  bool goodSourceChGain = true,
     goodSourceChOffset = true;
  for (int ch=0; ch<Parameter("SourceCh"); ch++)
  {
    goodSourceChGain = goodSourceChGain && ( Parameter("SourceChGain")(ch) > 0 );
    goodSourceChOffset = goodSourceChOffset && ( fabs(float(Parameter("SourceChOffset")(ch))) < 0.0001 );
  }
  if( !goodSourceChGain )
    bcierr << "SourceChGain is not supposed to be zero" << endl;
  if( !goodSourceChOffset )
    bcierr << "SourceChOffset is supposed to be zero" << endl;

	//get the number of devices
	int tNumDevices = FA_ID_INVALID;
	tNumDevices = faGetCount();
	if (tNumDevices < 1) {
	  bcierr <<"No vAmp devices were found."<<endl;
	  return;
	}
	if (tNumDevices > 1) {
		bcierr << "Only one device can be present on the system at a time."<<endl;
		return;     
	}

  //get device ID
	int tDevIds[1];
	t_faInformation tDeviceInfo[1];
	
	tDevIds[0] = faGetId(0);
	if (tDevIds[0] == FA_ID_INVALID)
	{
		bcierr << "Invalid device ID reported: "<< tDevIds[0] << endl;
	}
	faGetInformation(tDevIds[0], &(tDeviceInfo[0]));
	bciout << "Device found with serial: "<< tDeviceInfo[0].SerialNumber << endl;
	
	
	if(!IsVAmpServiceStopped()){
		bcierr << "There is \"VAmpService\" service running in background! It must be stopped before using V-Amp amplifier.\n" 
		       << "More detailed information is available in chapter \"VAmp Service Troubleshooting\" at: http://www.bci2000.org/wiki/index.php/Contributions:vAmpADC \n";
		return; 
	}

  if (Parameter("SamplingRate").InHertz() < 200)
  {
    bcierr << "The sample rate must be >= 200 Hz (2000Hz in high-speed)" << endl;
    return;
  }
  int dectmp = getDecimation();
  if (dectmp == 0)
  {
    if (!tHighSpeed)
    {
      bcierr << "Invalid sample rate. Valid values are 2000, 1000, "
             << float(2000.0/3)
             << ", 500, 400, "
             << 2000.0/6 << ", "
             << 2000.0/7 << ", "
             << 2000.0/8 << ", "
             << 2000.0/9 << ", or 200"
             << endl;
      return;
    }
    else
    {
      bcierr << "Invalid sample rate. Valid values in high-speed mode are 20000, 10000, "
             << float(20000.0/3)
             << ", 5000, 4000, "
             << 20000.0/6 << ", "
             << 20000.0/7 << ", "
             << 20000.0/8 << ", "
             << 20000.0/9 << ", or 2000"
             << endl;
      return;
    }
  }
  
  for (int dev=0; dev<tNumDevices; dev++)
  {
    if (tHighSpeed)
    {
      if (Parameter("SourceChList")(dev) > 5)
      {
        bcierr << "In high-speed acquisition mode, devices can have at most 5 channels (4 EEG + 1 Dig)." <<endl;
        return;
      }
    }
  }
  int totalnumchannels = Parameter( "SourceChList" )->NumValues();
  if( (totalnumchannels!=0) && (Parameter("SourceCh") != totalnumchannels) )
  {
    bcierr << "# total channels ("<< totalnumchannels<<") has to equal the SourceCh parameter, "
           << "or must be left blank for default list."
           << " If the digital input is turned on, you have to take this into account."
           << endl;
    return;
  }

  //check for consistency between sourcechdevices and sourcechlist per device
  if( Parameter( "SourceChList" )->NumValues() != 0 )
  {
    vector<int> tmpChList;
    int maxCh = tHighSpeed ? 5 : 19;
    for( int i = 0; i < Parameter( "SourceChList" )->NumValues(); ++i )
    {
      int curCh = Parameter( "SourceChList" )( i );
      if (curCh < 1 || curCh > maxCh)
      {
        bcierr << "SourceChList values must be within the range of 1 to "<< maxCh <<endl;
        return;
      }
      if (find(tmpChList.begin(), tmpChList.end(), curCh) != tmpChList.end())
      {
        bcierr << "SourceChList may not contain duplicate values for an individual device"<<endl;
        return;
      }
      tmpChList.push_back(curCh);
    }
  }

  Parameter( "Impedances" );
  State("Running");
}

int vAmpADC::getDecimation() const
{
  bool tHighSpeed = Parameter("AcquisitionMode") == 1 || Parameter("AcquisitionMode") == 4;
  float reqFs = Parameter("SamplingRate").InHertz();
  if (mHighSpeed){
    int dec = int(20000.0/reqFs);
    if (abs(float(dec) - 20000.0/reqFs) < .02)
      return dec;
    else
      return 0;
  }
  else{
    int dec = int(2000.0/reqFs);
    if (abs(float(dec) - 2000.0/reqFs) < .02)
      return dec;
    else
      return 0;
  }

}

// **************************************************************************
// Function:   Initialize
// Purpose:    This function parameterizes the vAmpADC
// **************************************************************************
void vAmpADC::OnInitialize(const SignalProperties& output)
{
  this->OnHalt();
  mHighSpeed = Parameter("AcquisitionMode") == 1 || Parameter("AcquisitionMode") == 4;
    mImpedanceMode = ( Parameter( "AcquisitionMode" ) == 3 );
	int tNumDevices = FA_ID_INVALID;
	tNumDevices = faGetCount();
	if (tNumDevices < 1) {
		bcierr <<"No vAmp devices were found."<<endl;
	}
	if (tNumDevices >  1) {
		bcierr << "A maximum of " <<  1 << " devices can be present on the system at a time."<<endl;
		return;     
	}
	t_faInformation tDeviceInfo[1];
	
	faGetInformation(faGetId(0), &(tDeviceInfo[0]));
	

	int sourceChListOffset = 0;
	mChList.clear();
	mDevList.clear();
	
	
	
	mDevList.push_back(faGetId(0));
	if( tNumDevices == 1 && mDevList.empty() )
	{
		bciout << "No device Id found"	<< endl;
		return;
		
	}	
	
	if (Parameter("SourceChList")->NumValues() == 0)
	{
		int nChsTmp = 0;
		switch (tDeviceInfo[0].Model)
		{
			case FA_MODEL_8:
				nChsTmp = FA_MODEL_8_CHANNELS_MAIN + FA_MODEL_8_CHANNELS_AUX;
				break;
			case FA_MODEL_16:
				nChsTmp = FA_MODEL_16_CHANNELS_MAIN + FA_MODEL_16_CHANNELS_AUX;
				break;
			default:
				if (mHighSpeed) nChsTmp = 5;
				break;
		}		
		for (int i = 0; i < nChsTmp; i++)
		{
			mChList.push_back(i);
		}
	}
	else
	{
		for (int i = 0; i < Parameter("SourceChList")->NumValues(); i++)
		{
			int curCh = Parameter("SourceChList")(i);
			mChList.push_back(curCh-1);
		}
	}
	

  MutableParamRef Impedances = Parameter( "Impedances" );
  if( mImpedances.empty())
  {
    Impedances->SetDimensions( 1, 1 );
    Impedances = "not measured";
  }
  else
  {
		size_t maxChannels = 0;		
		if( mImpedances[0].size() > maxChannels ) maxChannels = mImpedances[0/*i*/].size();		
        Impedances->SetDimensions( 1, mImpedances[0].size());
		
		for( size_t j = 0; j < mImpedances[0].size(); ++j )
		{
			float value = mImpedances[0][j];
			ostringstream oss;
			if( value < 1e3 )
				oss << value << "Ohm";
			else if( value < 1e6 )
			    oss << value / 1e3 << "kOhm";
			else
				oss << ">1MOhm";
			Impedances( 0, j ) = oss.str();
		}
		for( size_t j = mImpedances[0].size(); j < maxChannels; ++j )
		{
			Impedances( 0, j ) = "";
		}    
  }

  int decimate = 1;
  float fs = Parameter("SamplingRate").InHertz();
  decimate= getDecimation();
  float hpCorner = 0;
  if (Parameter("HPFilterCorner")->NumValues() == 1)
    hpCorner = Parameter("HPFilterCorner");
    
  mAcquire = new vAmpThread(
            Parameter("SampleBlockSize"),
            fs,
            decimate,
            mChList,
						mDevList,
            Parameter("AcquisitionMode"),
            hpCorner/fs);
  if (!mAcquire->ok())
  {
    bcierr << mAcquire->GetLastErr() << endl;
    mAcquire->Terminate();
    delete mAcquire;
    return;
  }

  ResetEvent( mAcquire->acquireEventRead );
	mAcquire->Start();
}


// **************************************************************************
// Function:   Process
// Purpose:    This function reads from the data buffer that is written to
//              in the acquisition thread Execute function
//
// Parameters: References to input signal (ignored) and output signal
// Returns:    N/A
// **************************************************************************
void vAmpADC::DoAcquire( GenericSignal& signal )
{
	if (mAcquire == NULL)
	{
	bcierr << "Invalid acquisition thread! " << endl;
	State("Running") = 0;
	return;
	}
	if (!mAcquire->ok())
	{
	bcierr << mAcquire->GetLastErr() << endl;
	State("Running") = 0;
	return;
	}

    string s = mAcquire->GetLastWarning();
    if(!s.empty())
      bciout << s << flush;

    if( mImpedanceMode )
	{
		mImpedances = mAcquire->GetImpedances();
	}
	if (mAcquire->Terminating())
	{
		for (int sample = 0; sample < signal.Elements(); sample++)
			for (int ch = 0; ch < signal.Channels(); ch++)
				signal(ch, sample) = 0;
		ResetEvent( mAcquire->acquireEventRead );
		return;
	}

	if (WaitForSingleObject(mAcquire->acquireEventRead, 1000) != WAIT_OBJECT_0)
	{
	for (int sample = 0; sample < signal.Elements(); sample++)
		for (int ch = 0; ch < signal.Channels(); ch++)
		signal(ch, sample) = 0;
	ResetEvent( mAcquire->acquireEventRead );
	return;
	}

	mAcquire->Lock();
	for (int sample = 0; sample < signal.Elements(); sample++)
	{
		for (int ch = 0; ch < signal.Channels(); ch++)
		{
			signal(ch, sample) = mAcquire->ExtractData(ch, sample);
		}
	}
	mAcquire->AdvanceReadBlock();
	mAcquire->Unlock();
	ResetEvent( mAcquire->acquireEventRead );
}


void vAmpADC::OnStartAcquisition( )
{
}

void vAmpADC::OnStopAcquisition()
{
}

// **************************************************************************
// Function:   Halt
// Purpose:    This routine shuts down data acquisition
// Parameters: N/A
// Returns:    N/A
// **************************************************************************

void vAmpADC::OnHalt()
{
	if (mAcquire != NULL)
	{
		mAcquire->Terminate();
		delete mAcquire;
		mAcquire = NULL;
	}
}

bool vAmpADC::IsVAmpServiceStopped() const
// checks the state of VAmpService. It returns true when the Service is stopped, otherwise returns false.
{
	bool success = true;

	// open the service control manager
	SC_HANDLE hSCM = OpenSCManager(NULL, NULL, GENERIC_READ);
	if(hSCM==NULL)
	{
		//Error: could not open SCManager
		bciout << " CheckVAmpService::OpenSCManager() failed, error: " << GetLastError() << endl;
		success = false;
	}

	// open the service
	SC_HANDLE hService = OpenService(hSCM, "VAmpService", GENERIC_READ);
	if(hService==NULL)
	{
		DWORD err = GetLastError();

		if(err != 1060) // service unknown
	    {
			//Error: could not open service
			bciout <<  "OpenService() failed, error: " <<  err << endl;
			success = false;
		}
		else
		{
			bciout << " Service not installed " <<  err << endl;
			success = true;  // returns true because if there is no Service, it is the same as: "it is not running"!
		}

		CloseServiceHandle(hSCM);
		return success;
	}

	SERVICE_STATUS ss;
	memset(&ss, 0, sizeof(ss));
	BOOL b = QueryServiceStatus(hService, &ss);

	if(!b)
	{
		bciout << " CheckVAmpService::QueryServiceStatus failed! " << endl;		
		success = false;
	}

	if(success && ss.dwCurrentState != SERVICE_STOPPED)
	{
		bciout << " CheckVAmpService => (dwCurrentState != SERVICE_STOPPED) => Service VAmpService.exe must be stopped!!! " << endl;		
		success = false;
	}
	else //if(success && ss.dwCurrentState == SERVICE_RUNNING)
	{
	}

	// close the service handle
	CloseServiceHandle(hService);

	// close the service control manager handle
	CloseServiceHandle(hSCM);

	return success;
}