/////////////////////////////////////////////////////////////////////////////
// $Id: WebcamLogger.h 2118 6/30/2010
// Authors: adam.wilson@uc.edu
// Description: The Webcam Logger logs video data from a standard webcams,
// saves the videos in a compressed video format, and stores the frame numbers
// as states
//
// Parameterization:
//   Webcam logging is enabled from the command line adding
//     --LogWebcam=1
//
// Event Variables:
//   WebcamFrame<n> - The current frame number for camera index n 
//
// (C) 2000-2008, BCI2000 Project
// http://www.bci2000.org
/////////////////////////////////////////////////////////////////////////////
#ifndef WEBCAMLOGGER_H
#define WEBCAMLOGGER_H

#include <string>
#include <iostream>

#include "WebcamThread.h"
#include "Environment.h"
#include "GenericVisualization.h"

using namespace std;

class WebcamLogger : public EnvironmentExtension
{
public:

	friend class WebcamThread;

	//Constructors and virtual interface
	WebcamLogger();
	virtual ~WebcamLogger();
	virtual void Publish();
	virtual void Preflight() const;
	virtual void Initialize();
	virtual void StartRun();
	virtual void StopRun();
	virtual void Halt();

private:
	bool							 mWebcamEnable,
										 mRecordAllCams,
										 mOpenWindow;
	int								 mCamNum,
										 mStartIndex,
										 mCodecType,
										 mEndIndex,
										 mDecimation,
										 mTextLocation;
	string						 mOutputFile,
										 mStrFOURCC;
	Synchronized<bool> mRecord;
	WebcamThread*      mpWebcamThread;

	std::vector<WebcamThread*> mWebcamThreads;
};

#endif // WEBCAM_LOGGER_H





