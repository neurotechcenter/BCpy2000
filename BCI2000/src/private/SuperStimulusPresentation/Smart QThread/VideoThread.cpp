#ifndef VIDEO_THREAD_CPP_
#define VIDEO_THREAD_CPP_

#include "VideoThread.h"

VideoThread::VideoThread( QObject * parent )
{
	mpParent = (VideoStimulus*)parent;
}

VideoThread::~VideoThread()
{

}

void
VideoThread::Play()
{
	mpParent->Play();
}


#endif // VIDEO_THREAD_CPP_