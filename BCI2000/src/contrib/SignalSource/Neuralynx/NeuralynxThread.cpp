////////////////////////////////////////////////////////////////////////////////
// $Id: NeuralynxThread.cpp 3798 2012-02-01 18:07:06Z mellinger $
// Author: g.dimitriadis@donders.ru.nl s.klanke@donders.ru.nl
// Description: A thread class that interfaces to the Neuralynx DigitalLynx
//              amplifier through its reversed engineered Matlab drivers
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

#include "NeuralynxThread.h"
#include "MatlabNetComClient.h"

#include "BCIError.h"

#include <math.h>
#include <queue>

using namespace std;

NeuralynxThread::NeuralynxThread(short _samplingRateFromADC):
    samplingRateFromADC(_samplingRateFromADC),
    maxCscSamples(0),
    recordBufferSize(0),
    maxEventStringLength(0),
    keepRunning(true),
    numCSC(0),
    numEvC(0),
    mBufferSample(0),
    mIndex(0),
    indexFollower(0),
    mBufferSampleReturned(0),
    mChannelReturned(0),
    numEventsRead(0),
    numEventsWritten(0),
    mEventTtlBinaryBase(32768)
{
    SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);
}

NeuralynxThread::~NeuralynxThread()
{
    cleanup(true);
}



void NeuralynxThread::Connect(bool verbose)
{
    //Checking for old connection closing it if there and opening a new one
    if(verbose) bciout<<"Trying to connect to Neuralynx server on localhost\n";
    int r = AreWeConnected();
    if(r)
    {
       DisconnectFromServer();
       r = 0;
    }

	if(!r) r=ConnectToServer("localhost");

	if (!r) {
		bcierr<<"Connection failed\n";
		keepRunning = false;
		cleanup(true);
		return;
	}
	if(verbose) bciout<<"Connected!"<<endl<<endl;

    //Set app name, get the names and types of the channels and give some feedback
	r = SetApplicationName("Neuralynx-to-BCI2000");

	r = getObjectsAndTypes();
	if (r==-1) {
		bcierr<<"Could not get Cheetah objects and types - aborting"<<endl;
		keepRunning = false;
		cleanup(true);
		return;
	}

	recordBufferSize = GetRecordBufferSize();
	maxCscSamples = GetMaxCSCSamples();
	maxEventStringLength = GetMaxEventStringLength();

	if(verbose)
	{
        bciout<<"Number of CSC streams.......: "<<numCSC<<endl;
        bciout<<"Number of event streams.....: "<<numEvC<<endl;
        bciout<<"Record Buffer Size..........: "<<recordBufferSize<<endl;
        bciout<<"Samples per Cheetah Block...: "<<maxCscSamples<<endl;
        bciout<<"Max. length event strings...: "<<maxEventStringLength<<endl;
        bciout<<"Server PC name..............: "<<GetServerPCName()<<endl;
        bciout<<"Server IP address...........: "<<GetServerIPAddress()<<endl;
        bciout<<"Server application..........: "<<GetServerApplicationName()<<endl<<endl;
	}


	if (numCSC==0) {
		bcierr<<"No continuous channels - exiting"<<endl;
		keepRunning = false;
		cleanup(true);
		return;
	}

    //Set up the temp buffers and the ring buffer to be used in the AquireThread
    if(verbose) bciout<<endl<<"Allocating memory to buffers... "<<endl;

	retBufSamples         = new short[recordBufferSize * maxCscSamples];
	retBufTimeStamps      = new uint64_t[recordBufferSize];
	retBufChannelNumbers  = new int[recordBufferSize];
	retBufSamplingFreq    = new int[recordBufferSize];
	retBufValidSamples    = new int[recordBufferSize];
	retBufEventIDs        = new int[recordBufferSize];
	retBufTTL             = new int[recordBufferSize];
	retBufEventString     = new char*[recordBufferSize];
	for (int j=0;j<recordBufferSize;j++) {
		retBufEventString[j] = new char[maxEventStringLength+1];
	}

	for (int j=0;j<RB_SIZE;j++)
	{
		mcbRingBuf[j].status = 0; // empty
		mcbRingBuf[j].timeStamp = 0;
		mcbRingBuf[j].chansFilled = 0;
		mcbRingBuf[j].samples = new short[numCSC * maxCscSamples];

		evRingBuf[j].events = new Event[maxCscSamples];
		for(int k=0; k<maxCscSamples; ++k)
		{
		    evRingBuf[j].events[k].eventID = 0;
		    evRingBuf[j].events[k].ttl = 0;
		    evRingBuf[j].events[k].name = new char[maxEventStringLength+1];
		}
	}
}



int NeuralynxThread::getObjectsAndTypes() {
	char *objPtr[MAX_OBJ], *typPtr[MAX_OBJ];
	int n;

	for (n=0;n<MAX_OBJ;n++)
	{
		cheetahObjects[n][0] = 0;
		cheetahTypes[n][0] = 0;
		objPtr[n] = cheetahObjects[n];
		typPtr[n] = cheetahTypes[n];
	}

	numCSC = 0;
	numEvC = 0;
	char *channelNamesForChunk = NULL;

	n = GetCheetahObjectsAndTypes(objPtr, typPtr);
	if (n==0) return -1;

	channelNamesSize = 0;

	for (n=0;n<MAX_OBJ;n++) {
		if (cheetahObjects[n][0] == 0) break;

		if (!strcmp(cheetahTypes[n], "CscAcqEnt"))
		{
			cscIndex[numCSC++] = n;
			channelNamesSize += strlen(cheetahObjects[n]) + 1; // trailing 0
            continue;
		}

		if (!strcmp(cheetahTypes[n], "EventAcqEnt"))
		{
			evIndex[numEvC++] = n;
			continue;
		}
	}

	//for(int i=0; i<channelNamesSize; ++i)
	//{
	//	channelNamesForChunk[i]=NULL;
	//}
	
	channelNamesForChunk=NULL;
	if (numCSC>0)
	{
		int offset = 0;
		channelNamesForChunk = new char[channelNamesSize];
		
		for (int i=0;i<numCSC;i++) {
			const char *name = cheetahObjects[cscIndex[i]];
			int len = strlen(name);
			memcpy(channelNamesForChunk + offset, name, len+1); // includes \0
			//copy(name, name+len+1, channelNamesForChunk + offset);
			offset += len+1;
		}
	}
	return n;
}




void NeuralynxThread::CheckAmp()
{
    //Open the streams for all channels
	bciout<<"Opening and Checking CSC channels..."<<endl;
	for (int j=0;j<numCSC;j++)
	{
		int r = OpenStream(cheetahObjects[cscIndex[j]]);
		if (!r) {
			bcierr<<"Could not open CSC channel "<<cheetahObjects[cscIndex[j]]<<endl;
			keepRunning = false;
			cleanup(true);
			break;
		}
	}

	bciout<<"Opening and Checking Event channel(s)..."<<endl;
	for (int j=0;j<numEvC;j++)
	{
		int r = OpenStream(cheetahObjects[evIndex[j]]);
		if (!r) {
			bcierr<<"Could not open event channel "<<cheetahObjects[evIndex[j]];
			keepRunning = false;
			cleanup(true);
			break;
		}
	}

    //Check for sampling frequency consistency
    //by grabing some data (no other way with the current neuralynx driver)
    int nCSC = 0;
    bool checkingOn=true;
	while(checkingOn){
		int r = GetNewCSCData(cheetahObjects[cscIndex[nCSC]], &retBufTimeStamps, &retBufChannelNumbers, &retBufSamplingFreq, &retBufValidSamples, &retBufSamples, &numRecordsReturned, &numRecordsDropped);
		if (r==0)
		{
			bcierr<<"Error in getting data from channel "<<cscIndex[nCSC]<<" (in GetNewCSCData)"<<endl;
			keepRunning = false;
			checkingOn = false;
			cleanup(true);
			break;
		}
		if (numRecordsDropped > 0) {
			bcierr<<"Records dropped - cancelling operation"<<endl;
			keepRunning = false;
			checkingOn = false;
			cleanup(true);
			break;
		}

        //If we got some records for this channel then check them out
        //otherwize sleep a bit and try again for the same channel
		if (numRecordsReturned > 0)
		{
			for (int i=0;i<numRecordsReturned;i++)
			{
				int chan = retBufChannelNumbers[i];
				int freq = retBufSamplingFreq[i];
				int vals = retBufValidSamples[i];
				uint64_t ts = retBufTimeStamps[i];
                if(freq<=0 || freq>33000) break; //Sometimes the GetNewCSCData return negative and just wrong retBufSamplingFreq for a channel. Ignore this and keep trying to get a good value
                nCSC++;
				if(samplingRateFromADC!=freq)
				{
                    bcierr<<"Inconsistent input of frequency. Input freq = "<<samplingRateFromADC<<". Amp freq = "<<freq<<". Make sure you type the frequency after the subsampling using decimals."<<endl;
                    keepRunning = false;
                    checkingOn = false;
                    cleanup(true);
				    break;
				}
			}
        }else
        {
            Sleep(1);
        }

        if(nCSC==numCSC) checkingOn=false;
    }

    bciout<<"Closing CSC channels..."<<endl;
	for (int j=0;j<numCSC;j++) {
		int r = CloseStream(cheetahObjects[cscIndex[j]]);
		if (!r) {
			bcierr<<"Could not close CSC channel "<<cheetahObjects[cscIndex[j]]<<endl;
			keepRunning = false;
			cleanup(true);
			break;
		}
	}

	bciout<<"Closing event channels..."<<endl;
	for (int j=0;j<numEvC;j++)
	{
		int r = CloseStream(cheetahObjects[evIndex[j]]);
		if (!r) {
			bcierr<<"Could not close event channel "<<cheetahObjects[evIndex[j]]<<endl;
			keepRunning = false;
			cleanup(true);
			break;
		}
	}
    bciout<<"Finished Checking Amp\n"<<endl;
}





void NeuralynxThread::cleanup(bool disconnect)
{
    if(disconnect && AreWeConnected())
    {
        DisconnectFromServer();
        bciout<<"Disconnected from amplifier"<<endl;
    }

    delete[] retBufSamples;
    delete[] retBufTimeStamps;
	delete[] retBufChannelNumbers;
	delete[] retBufSamplingFreq;
	delete[] retBufValidSamples;
	delete[] retBufTTL;
	delete[] retBufEventIDs;
	for (int j=0;j<RB_SIZE;j++)
	{
	    for(int k=0; k<maxCscSamples; ++k)
	    {
	        delete[] evRingBuf[j].events[k].name;
	    }
		delete[] mcbRingBuf[j].samples;
		delete[] evRingBuf[j].events;
	}
	if (!channelNamesForChunk) {
		delete[] channelNamesForChunk;
	}
}



int NeuralynxThread::Execute()
{
	OSThread::Sleep(100);
    bciout<<"Data Collection Thread has started"<<endl;
    Connect(true);
    int index = 0;
    int headerWritten = 0;


    //Open the streams for all channels
    for (int j=0;j<numCSC;j++)
    {
        int r = OpenStream(cheetahObjects[cscIndex[j]]);
        if (!r) {
            bcierr<<"Could not open CSC channel "<<cheetahObjects[cscIndex[j]]<<endl;
            keepRunning = false;
            break;
        }
    }
	for (int j=0;j<numEvC;j++)
	{
		int r = OpenStream(cheetahObjects[evIndex[j]]);
		if (!r) {
			bcierr<<"Could not open event channel "<<cheetahObjects[evIndex[j]]<<endl;
			keepRunning = false;
            break;
		}
	}

    bciout<<"All channels have been opened"<<endl;
    //Call GetNewCSCData once for all channels to get read of channels that don't have records in the first call
    for(int b=0; b<numCSC; ++b)
    {

        GetNewCSCData(cheetahObjects[cscIndex[b]], &(retBufTimeStamps), &(retBufChannelNumbers),
                              &(retBufSamplingFreq), &(retBufValidSamples), &(retBufSamples), &(numRecordsReturned), &(numRecordsDropped));
    }
    for(int e=0; e<numEvC; ++e)
    {
        char *stream = cheetahObjects[evIndex[e]];
        GetNewEventData(stream, &retBufTimeStamps, &retBufEventIDs, &retBufTTL, retBufEventString, &numRecordsReturned, &numRecordsDropped);
    }



    int Dts = (int) (1e6* maxCscSamples / calcSamplingFreq + 0.5);
    int numOfBuffItt = 0;
    while(keepRunning)
    {
        //Get the 32 channels of voltage data
        for(int nCSC = 0; nCSC<numCSC; ++nCSC)
        {
            int r = GetNewCSCData(cheetahObjects[cscIndex[nCSC]], &(retBufTimeStamps), &(retBufChannelNumbers),
                              &(retBufSamplingFreq), &(retBufValidSamples), &(retBufSamples), &(numRecordsReturned), &(numRecordsDropped));

            //Make a max RB_SIZE long queue and write all the individual
            //timeStamps of the records collected up to that point,
            //thus knowing at what index in the buffer(position in the queue)
            //a timeStamp and thus a record should be placed into
            if (numRecordsReturned > 0)
            {
                for (int i=0;i<numRecordsReturned;i++)
                {
                    if((mIndexOfTimeStamps.empty()) || (mIndexOfTimeStamps.back() < retBufTimeStamps[i]))
                    {
                        mIndexOfTimeStamps.push(retBufTimeStamps[i]);
                    }
                    if(mIndexOfTimeStamps.front() > retBufTimeStamps[i])
                    {
                        bcierr<<"Record: "<<i<<" of channel: "<<cheetahObjects[cscIndex[nCSC]]<<" time stamp is older than the first captured TS"<<endl;
                    }
                    if(mIndexOfTimeStamps.size() > RB_SIZE)
                    {
                        while(!mIndexOfTimeStamps.empty())
                        {
                            mIndexOfTimeStamps.pop();
                        }
                        mIndexOfTimeStamps.push(retBufTimeStamps[i]);
//                        bciout<<"Buffer filled "<<++numOfBuffItt<<" times."<<endl;

                        for (int j=0;j<RB_SIZE;j++)
                        {
                            for(int k=0; k<maxCscSamples; ++k)
                            {
                                evRingBuf[j].events[k].eventID = 0;
                                evRingBuf[j].events[k].ttl = 0;
                            }
                        }
                    }
                }
            }


            if (r==0) {
                bcierr<<"Error in GetNewCSCData"<<endl;
                break;
            }

            if (numRecordsDropped > 0) {
                bcierr<<"Records dropped - cancelling operation."<<endl;
                break;
            }


            if (numRecordsReturned > 0)
            {
                for (int i=0;i<numRecordsReturned;i++)
                {
                    int chan = retBufChannelNumbers[i];
                    int freq = retBufSamplingFreq[i];
                    int vals = retBufValidSamples[i];
                    uint64_t ts = retBufTimeStamps[i];

                    if (vals != maxCscSamples) {
                        bcierr<<"Got "<<vals<<" valid samples. That is less than "<<maxCscSamples<<" samples in this record - aborting"<<endl;
                        keepRunning = false;
                        break;
                    }


                    //Find the index by comparing the timestamp of the record to the timestamps in the timestampQueue
                    queue<uint64_t> tempq(mIndexOfTimeStamps);
                    int m =0;
                    while(!tempq.empty())
                    {
                        if(tempq.front() == retBufTimeStamps[i]) index = m;
                        tempq.pop();
                        m+=1;
                    }

                    // ok, everything looks fine, add to ring buffer
                    if (mcbRingBuf[index].status == 2) {
                        bciout<<"Channel="<<chan<<" Index="<<index<<" record="<<i<<endl;
                        bcierr<<"Proccess cannot keep up with load - aborting"<<endl;
                        keepRunning = false;
                        break;
                        }

                    if (mcbRingBuf[index].status == 0)
                    {
                        mcbRingBuf[index].timeStamp = ts;
                        mcbRingBuf[index].status = 1;
                    }
                    else if (mcbRingBuf[index].timeStamp != ts)
                    {
                        bcierr<<"Inconsistent time stamps in records and internal ring buffer - aborting"<<endl;
                        bciout<<"Channel="<<chan<<" Index="<<index<<" record="<<i<<endl;
                        queue<uint64_t> tempq(mIndexOfTimeStamps);
                        while(!tempq.empty())
                        {
                            bciout<<tempq.front()<<"/";
                            tempq.pop();
                        }
                        bciout<<endl;
                        keepRunning = false;
                        break;
                    }

                    // fine, write samples at right channel
                    // have 'dest' point to proper row in ring buffer slot
                    short *dest = mcbRingBuf[index].samples + chan;
                    // have 'src' point to first sample in current record
                    short *src  = retBufSamples + i*maxCscSamples;
                    for (int n=0;n<maxCscSamples;n++)
                    {
                        dest[n*numCSC] = src[n];
                    }
                    // see if all channels are filled in this slot, and if so, write out
                    if (++mcbRingBuf[index].chansFilled == numCSC)
                     {
                        mcbRingBuf[index].chansFilled = 0;
                        mcbRingBuf[index].status = 2;
                    }
                }
            }
            else
            {
                Sleep(1);
            }

            if(!keepRunning) break;
        }


        //Call the Events channel(s) and put the event data to an evRingBuffer
        for (int nEvC = 0; nEvC < numEvC; nEvC++)
        {
			char *stream = cheetahObjects[evIndex[nEvC]];
			int r = GetNewEventData(stream, &retBufTimeStamps, &retBufEventIDs, &retBufTTL, retBufEventString, &numRecordsReturned, &numRecordsDropped);

			if (r==0)
			{
				bcierr<<"Error in GetNewEventData from stream "<<stream<<" - aborting"<<endl;
				keepRunning = false;
				break;
			}

			if (numRecordsDropped > 0)
			{
				bcierr<<"Records dropped in stream "<<stream<<" - cancelling operation"<<endl;
				keepRunning = false;
				break;
			}

			if (numRecordsReturned != 0)
			{
                for (int i=0;i<numRecordsReturned;i++)
                {
                    //Find the index by comparing the timestamp of the event to the timestamps in the timestampQueue
                    int indexBase = mIndexOfTimeStamps.size()-1;
                    int TSdifference = int(retBufTimeStamps[i] - mIndexOfTimeStamps.back());
                    index = indexBase + int(TSdifference / Dts);
                    if(index<0) index=0;
                    if(index>=RB_SIZE) index-=RB_SIZE;
                    int SamplesDifference = int(maxCscSamples * TSdifference / Dts);
                    while(SamplesDifference > maxCscSamples)
                    {
                        SamplesDifference = SamplesDifference - maxCscSamples;
                    }
                    int eventPlace = SamplesDifference;
                    if(eventPlace<0)
                    {
                        eventPlace+=maxCscSamples;
                    }
//                  strcpy(evRingBuf[index].events[eventPlace].name, retBufEventString[i]);
                    evRingBuf[index].events[eventPlace].eventID = retBufEventIDs[i];
                    int ttl = retBufTTL[i];
//                    bciout<<"IndexedTS: "<<mIndexOfTimeStamps.back()<<" eventTS: "<<retBufTimeStamps[i]<<" TSdif: "<<TSdifference<<" index: "<<index<<" eventPlace: "<<eventPlace<<" TTL: "<<ttl+32768<<endl;
                    evRingBuf[index].events[eventPlace].ttl = ttl;
                }
            }
            else
            {
                Sleep(1);
            }
        }
		if(!keepRunning) break;
    }

    //Close all streams
    bciout<<"Closing CSC channels..."<<endl;
	for (int j=0;j<numCSC;j++) {
		int r = CloseStream(cheetahObjects[cscIndex[j]]);
		if (!r) {
			bcierr<<"Could not close CSC channel "<<cheetahObjects[cscIndex[j]]<<endl;
			cleanup(true);
			break;
		}
	}

	bciout<<"Closing event channels..."<<endl;
	for (int j=0;j<numEvC;j++)
	{
		int r = CloseStream(cheetahObjects[evIndex[j]]);
		if (!r) {
			bcierr<<"Could not close event channel "<<cheetahObjects[evIndex[j]]<<endl;
			cleanup(true);
			break;
		}
	}
	//Call cleanup to disconnect and clean memory buffers
    cleanup(true);
    bciout<<"Data Collection Thread just stopped"<<endl;
    return 0;
}



short NeuralynxThread::getNextValue()
{
    short result = 0;
    while(mcbRingBuf[indexFollower].status!=2)
    {
       this->Sleep(1);
    }

    result = (mcbRingBuf[indexFollower].samples[mChannelReturned + mBufferSampleReturned*numCSC]);
    if(mChannelReturned == numCSC)
    {
        result = evRingBuf[indexFollower].events[mBufferSampleReturned].ttl;
        if(result!=0) result+=mEventTtlBinaryBase;
    }

    if(++mChannelReturned == numCSC + numEvC)
    {
        mChannelReturned = 0;
        if(++mBufferSampleReturned == maxCscSamples)
        {
            mBufferSampleReturned=0;
            mcbRingBuf[indexFollower].status=0;
            if(++indexFollower == RB_SIZE)
            {
               indexFollower=0;
            }
        }
    }

    return result;
}
