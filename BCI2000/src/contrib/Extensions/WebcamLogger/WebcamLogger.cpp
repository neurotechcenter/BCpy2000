/////////////////////////////////////////////////////////////////////////////
// $Id: WebcamLogger.cpp 5830 2018-11-29 19:09:18Z abelsten $
// Authors: adam.wilson@uc.edu
// Description: The WebcamLogger records webcam video to an AVI file
// and sets the frame number as a state value
//
// Parameterization:
//   Webcam logging is enabled from the command line adding
//     --LogWebcam=1
//   As a command line option.
//
// Event Variables:
//   WebcamFrame<n> - The current frame number for camera index n 
//
// (C) 2000-2008, BCI2000 Project
// http://www.bci2000.org
/////////////////////////////////////////////////////////////////////////////
#include <cstring>
#include <ctime>
#include <iomanip>
#include <iostream>

#include "WebcamLogger.h"
#include "FileUtils.h"

#define NUM_OF_WEBCAM_EVENTS 4

using namespace std;

Extension( WebcamLogger );

WebcamLogger::WebcamLogger() :
	mWebcamEnable( false ),
	mpWebcamThread( NULL ),
	mCamNum( 0 )
{
	
}

WebcamLogger::~WebcamLogger()
{
	Halt();
}

void WebcamLogger::Publish()
{
	bool webcamEnable = false;
	webcamEnable = ( (int)OptionalParameter("LogWebcam", 0) != 0 );

	if( !webcamEnable ) 
	{
		if (mpWebcamThread) delete mpWebcamThread;
		return;
	}

	BEGIN_PARAMETER_DEFINITIONS
		"Source:Webcam int LogWebcam= 1 0 0 1"
			" // Allow logging from webcam (boolean)",

		"Source:Webcam int CameraNumber= 1 1 0 %"
			" // The webcam number for systems with multiple cameras",

		"Source:Webcam int RecordAll= 0 0 0 1"
			" // Record from all available webcams (boolean)",

		"Source:Webcam int StartIndex= 1 1 0 %"
			" // Camera index to start searching at",

		"Source:Webcam int EndIndex= 2 2 0 %"
		" // Camera index to stop searching at",

		"Source:Webcam int Decimation= 1 1 1 %"
			" // Save/display every nth frame",

		"Source:Webcam int DateTimeLocation= 0 0 0 4"
			" // Date/time text location in saved video: "
				" 0: none,"
				" 1: UpperRight,"
				" 2: UpperLeft,"
				" 3: LowerRight,"
				" 4: LowerLeft"
					" (enumeration)",

		"Source:Webcam int DisplayStream= 1 1 0 1"
			" // Stream camera data to a new window (boolean)",

		"Source:Webcam int SelectCodec= 0 0 0 2"
			" // Select codec to use: "
				" 0: Default DIVX,"
				" 1: Select through pop-up window,"
				" 2: Use CodecFOURCC parameter"
					" (enumeration)",
	
		"Source:Webcam string CodecFOURCC= DIVX % % %"
			" // FOURCC for video encoding",

	END_PARAMETER_DEFINITIONS

	// declare NUM_OF_WEBCAM_EVENTS event states
	for (int i = 0; i < NUM_OF_WEBCAM_EVENTS; i++) 
	{
		stringstream EventStrm;
		EventStrm << "WebcamFrame" << i << " 24 0 0 0";
		string EventStr = EventStrm.str();
		BEGIN_EVENT_DEFINITIONS
			EventStr.c_str(),
		END_EVENT_DEFINITIONS
	}
}

void WebcamLogger::Preflight() const
{
	if (OptionalParameter("LogWebcam", 0) == 0) return;

	if (!(bool)Parameter("RecordAll"))
	{
		cv::VideoCapture capture = cv::VideoCapture((int)Parameter("CameraNumber"));
		if (!capture.isOpened())
		{
			bcierr << "Cannot initialize webcam with index " << Parameter("CameraNumber") 
						 << ". Please make sure that the webcam is connected, and that the camera number is appropriate." << endl;
			return;
		}
		capture.release();
	}
	Parameter("SelectCodec");
	Parameter("StartIndex");
	Parameter("EndIndex");
	Parameter("DisplayStream");
	Parameter("Decimation");
	Parameter("DateTimeLocation");
	Parameter("DataDirectory");
	Parameter("SubjectName");
	Parameter("SubjectSession");
	Parameter("SubjectRun");

	string FOURCC = (string)Parameter("CodecFOURCC");
	if (FOURCC.length() > 4)
	{
		bcierr << "CodecFOURCC must have four characters or less" << endl;
	}
}

void WebcamLogger::Initialize()
{
	mWebcamEnable = ( (int)OptionalParameter( "LogWebcam", 0) != 0 );

	if (mWebcamEnable)
	{
		mCamNum        =  (int)Parameter("CameraNumber");
		mRecordAllCams = (bool)Parameter("RecordAll");
		mOpenWindow    = (bool)Parameter("DisplayStream");
		mCodecType     =  (int)Parameter("SelectCodec");
		mStartIndex    =  (int)Parameter("StartIndex");
		mEndIndex      =  (int)Parameter("EndIndex");
		mTextLocation  =  (int)Parameter("DateTimeLocation");
		mDecimation    =  (int)Parameter("Decimation");
		mStrFOURCC   = (string)Parameter("CodecFOURCC");
		mStrFOURCC.resize(4, ' ');
		std::transform(mStrFOURCC.begin(), mStrFOURCC.end(), mStrFOURCC.begin(), ::toupper);
		
		if (!mRecordAllCams)
		{
			// only recording from one camera

			if (mCamNum > NUM_OF_WEBCAM_EVENTS)
			{
				bcierr << "The max webcam index supported by this version of BCI2000 is " << NUM_OF_WEBCAM_EVENTS
							 << ". To record from more webcams or webcams of higher index, change NUM_OF_WEBCAM_EVENTS macro in WebcamLogger.cpp to the "
							 << "max index or number of webcams you want to record from and recompile." << endl;
				return;
			}

			// start thread
			if (!mpWebcamThread)
			{
				mpWebcamThread = new WebcamThread(this, mCamNum);
			}
			else
			{
				mpWebcamThread->Initalize();
			}
		}
		else {
			//record from all cameras on system
			if (mEndIndex > NUM_OF_WEBCAM_EVENTS)
			{
				bcierr << "The max number of webcams supported by this version of BCI2000 is " << NUM_OF_WEBCAM_EVENTS 
					     << ". To record from more webcams, change NUM_OF_WEBCAM_EVENTS macro in WebcamLogger.cpp to the "
							 << "number of webcams you want to record from and recompile." << endl;
				return;
			}

			if (mStartIndex >= mEndIndex)
			{
				bcierr << "Start index " << mStartIndex << " is greater than or equal to end index " 
							 << mEndIndex << ". No cameras will be connected" << endl;
				return;
			}

			// close connections with other cameras
			this->Halt();

			// try to connect to specified cameras
			for (int i = mStartIndex; i < mEndIndex+1; i++)
			{
				WebcamThread* temp = new WebcamThread(this, i);
				if (temp->Connected())
				{
					mWebcamThreads.push_back(temp);
				}
				else
				{
					temp->StopThread();
					delete temp;
					temp = NULL;
				}
			}
			bciout << "Connected to " << mWebcamThreads.size() << " Camera(s)" << endl;
		}
	}
	else 
	{
		this->Halt();
	}
}

void WebcamLogger::StartRun()
{
	if( mWebcamEnable )
	{
		// update output file name
		mOutputFile = CurrentRun();
    mOutputFile = FileUtils::ExtractDirectory( mOutputFile ) + FileUtils::ExtractBase( mOutputFile );
		
		// start recording from camera(s)
		if (mRecordAllCams)
		{
			for (int i = 0; i < mWebcamThreads.size(); i++)
			{
				mWebcamThreads[i]->StartRecording();
			}
		}
		else
		{			
			mpWebcamThread->StartRecording();
		}
		mRecord = true;
	}
}

void WebcamLogger::StopRun()
{
	if (mWebcamEnable)
	{
		mRecord = false;

		// stop recording from camera(s)
		if (mRecordAllCams)
		{
			for (int i = 0; i < mWebcamThreads.size(); i++)
			{
				mWebcamThreads[i]->StopRecording();
			}
		}
		else
		{
			mpWebcamThread->StopRecording();
		}
	}
}

void WebcamLogger::Halt()
{
	// stop all camera threads and delete threads 
	if (mRecordAllCams)
	{
		for (int i = 0; i < mWebcamThreads.size(); i++)
		{
			mWebcamThreads[i]->StopThread();
			delete mWebcamThreads[i];
			mWebcamThreads[i] = NULL;
		}
		mWebcamThreads.clear();
	}
	else
	{
		if (mpWebcamThread)
		{
			mpWebcamThread->StopThread();
			delete mpWebcamThread;
			mpWebcamThread = NULL;
		}
	}
}
