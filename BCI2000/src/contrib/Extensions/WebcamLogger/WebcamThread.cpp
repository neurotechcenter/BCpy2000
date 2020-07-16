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
#include <cstring>
#include <ctime>
#include <iomanip>
#include <iostream>

#include "WebcamThread.h"
#include <opencv2/opencv.hpp>
#include "BCIEvent.h"

using namespace std;

static time_t Now()
{ return ::time( 0 ); }

static string TimeToString( time_t inTime )
{ // format is "MM/dd/yy hh:mm:ss"
  struct ::tm t = { 0 },
             *p = ::localtime( &inTime );
  if( !p )
    return "<n/a>";

  t = *p;
  ostringstream oss;
  oss << setfill( '0' )
      << setw( 2 ) << t.tm_mon + 1 << '/'
      << setw( 2 ) << t.tm_mday << '/'
      << setw( 2 ) << t.tm_year % 100 << ' '
      << setw( 2 ) << t.tm_hour << ':'
      << setw( 2 ) << t.tm_min << ':'
      << setw( 2 ) << t.tm_sec;
  return oss.str();
}

//==========================================================================================
// WebcamThread member function implementaion
//==========================================================================================
WebcamThread::WebcamThread(WebcamLogger* logger, int camIndex) :
	mWebcam(logger),
	mSourceWidth(0),
	mSourceHeight(0),
	mAddText(true),
	mCameraIndex(camIndex)
{
	bool success;
	success = this->Initalize();
	if (success)
	{
		// Start the thread
		this->Start();
	}
}

bool WebcamThread::Initalize()
{
	// start mutex so existing thread doesn't attempt to use camera while initalizing and getting FPS
	mMutex.Acquire();

	// open the webcam 
	mVCapture.open(mCameraIndex);
	if (!mVCapture.isOpened()) 
	{
		mMutex.Release();
		return false;
	}
	// get FPS over 60 frames
	int nFrames = 60;
	PrecisionTime t1 = PrecisionTime::Now();
	for (int i = 0; i < nFrames; i++)
	{
		cv::Mat Frame;
		mVCapture >> Frame;
	}
	PrecisionTime t2 = PrecisionTime::UnsignedDiff(PrecisionTime::Now(), t1);
	int fps = 1000.0f / (float(t2) / nFrames);
	mTargetFps = fps / mWebcam->mDecimation;
	bciout << "Camera " << mCameraIndex << " Target FPS: " << mTargetFps << endl;

	mMutex.Release();

	// store source size
	mSourceWidth = mVCapture.get(cv::CAP_PROP_FRAME_WIDTH);
	mSourceHeight = mVCapture.get(cv::CAP_PROP_FRAME_HEIGHT);
		
	mWinName = "Camera " + to_string(mCameraIndex);

	// initalize the location of date/time text 
	this->InitalizeText();
	return true;
}

WebcamThread::~WebcamThread()
{
	cv::destroyAllWindows();
}

void WebcamThread::StartRecording()
{
	// open video recorder
	string outputFileName = mWebcam->mOutputFile + "_" + to_string(mCameraIndex) + "_vid.mp4";
	int fourcc;
	switch (mWebcam->mCodecType) {
	case 0: // Defualt DIVX
		fourcc = cv::VideoWriter::fourcc('D', 'I', 'V', 'X');
		break;
	case 1: // Pop-up window, functionality provided by openCV
		bciout << "Using external pop-up window for FOURCC selection" << endl;
		fourcc = -1;
		break;
	case 2: // Other (user supplied string)
		bciout << "User supplied FOURCC: \"" << mWebcam->mStrFOURCC << "\"" << endl;
		fourcc = cv::VideoWriter::fourcc(mWebcam->mStrFOURCC[0], mWebcam->mStrFOURCC[1], mWebcam->mStrFOURCC[2], mWebcam->mStrFOURCC[3]);
		break;
	default:
		bcierr << "Invalid CodecFOURCC parameter. Recording canceled" << endl;
		return;
	}
	mVideoWriter.open(outputFileName, fourcc, mTargetFps, cv::Size(mSourceWidth, mSourceHeight), true);

	if (!mVideoWriter.isOpened())
	{
		bciwarn << "Could not open file for recording camera " << mCameraIndex << " video." 
						<< " Trying a different codec may resolve this issue" << endl;
		return;
	}

	bciout << "Started Recording Camera " << mCameraIndex << endl;

	mFrameNum = 0;
	mCount = 0;
}

void WebcamThread::StopRecording()
{
	if (mVideoWriter.isOpened())
	{
		bciout << "Stopped Recording Camera " << mCameraIndex << endl;
	}
	mVideoWriter.release();
}

void WebcamThread::InitalizeText()
{
	string currentTime = TimeToString(Now());
	int baseline;

	// get the size of bounding box of text
	cv::Size textSize = cv::getTextSize(currentTime, cv::FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseline);

	// find appropriate point to put the text
	mAddText = true;
	switch (mWebcam->mTextLocation) {
	case 1: // Upper Right
		mTextPoint = cv::Point(mSourceWidth - 10 - textSize.width, 10 + textSize.height);
		break;
	case 2: // Upper Left
		mTextPoint = cv::Point(10, 10 + textSize.height);
		break;
	case 3: // Lower Right
		mTextPoint = cv::Point(mSourceWidth - 10 - textSize.width, mSourceHeight - 10 - baseline);
		break;
	case 4: // Lower Left
		mTextPoint = cv::Point(10, mSourceHeight - 10 - baseline);
		break;
	default:
		mAddText = false;
		mTextPoint = cv::Point(10, 10);
		break;
	}
}

void WebcamThread::GetFrame()
{
	mCount++;
	if ((mCount % mWebcam->mDecimation) == 0)
	{
		// get new frame
		cv::Mat Frame;
		mVCapture >> Frame;

		if (mAddText)
		{
			// add text to the image
			cv::putText(Frame, TimeToString(Now()), mTextPoint, cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 255));
		}

		if (mWebcam->mOpenWindow)
		{
			// display image to window
			cv::imshow(mWinName, Frame);
			cv::waitKey(5);
		}
	
		if (mWebcam->mRecord)
		{
			// write image to file
			mVideoWriter << Frame;
			bcievent << "WebcamFrame" + to_string(mCameraIndex) + " " << ++mFrameNum;
		}
	}
	else
	{
		if (mWebcam->mRecord)
			bcievent << "WebcamFrame" + to_string(mCameraIndex) + " " << 0;
	}
}

int WebcamThread::OnExecute()
{
	mFrameNum = 0;
	bciout << "Camera " << mCameraIndex << " thread started" << endl;
	while (!this->Terminating())
	{
		if (mVCapture.isOpened())
			this->GetFrame();
		else
			bcierr << "Camera not opened" << endl;
	}
	bciout << "Camera " << mCameraIndex << " thread ended" << endl;
	return 0;
}
