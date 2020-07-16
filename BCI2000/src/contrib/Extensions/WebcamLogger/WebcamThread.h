/////////////////////////////////////////////////////////////////////////////
// $Id: WebcamLogger.cpp 5817 2018-11-08 16:56:17Z mellinger $
// Authors: adam.wilson@uc.edu & Alex Belsten belsten@neurotechcenter.org
//
// Description: The WebcamThread records webcam video to an AVI file
// and sets the frame number as a state value
//
// Event Variables:
//   WebcamFrame<n> - The current frame number for camera index n 
//
// (C) 2000-2008, BCI2000 Project
// http://www.bci2000.org
/////////////////////////////////////////////////////////////////////////////
#ifndef WEBCAMTHREAD_H
#define WEBCAMTHREAD_H

#include <opencv2/highgui/highgui.hpp>
#include <string>
#include <iostream>

#include "WebcamLogger.h"
#include "Thread.h"
#include "Mutex.h"
#include "PrecisionTime.h"

using namespace std;

class WebcamLogger;

class WebcamThread : public Thread
{
public:
	WebcamThread(WebcamLogger* logger, int camIndex);
	~WebcamThread();
	virtual int OnExecute() override;
	void GetFrame();
	void StartRecording();
	void StopRecording();
	bool Initalize();
	bool Connected() const { return mVCapture.isOpened(); }
	void StopThread() { this->TerminateAndWait(); }

private:
	void InitalizeText();

	WebcamLogger*		 mWebcam;
	Tiny::Mutex			 mMutex;
	cv::VideoCapture mVCapture;
	cv::VideoWriter  mVideoWriter;
	cv::Point				 mTextPoint;
	unsigned long		 mFrameNum,
									 mCount;
	int 						 mSourceWidth,
									 mSourceHeight,
									 mCameraIndex;
	float						 mTargetFps;
	bool						 mAddText;
	string					 mWinName;
};

#endif // WEBCAMTHREAD_H





