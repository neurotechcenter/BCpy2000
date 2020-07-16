////////////////////////////////////////////////////////////////////////////////
// $Id: NeuralynxThread.h 3798 2012-02-01 18:07:06Z mellinger $
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
#ifndef NEURALYNX_THREAD_H
#define NEURALYNX_THREAD_H

#define MAX_OBJ  5000
#define MAX_LEN  100
#define RB_SIZE  50

#include "GenericADC.h"
#include "OSThread.h"
#include <queue>

typedef unsigned long long uint64_t;

using namespace std;


class NeuralynxThread : public OSThread
{
public:
    NeuralynxThread(short samplingRateFromADC);
    ~NeuralynxThread();


    int getObjectsAndTypes();//Get the objects that the Cheetah software is currently using
    void Connect(bool);//Connect to the amplifier
    void CheckAmp();//Check the amplifier's setup and compare to the Parameters set by the user
    void cleanup(bool disconnect); //Disconnects from amp if disconnect==true and cleans all memeory


    short getNextValue();
    bool bufferFilled, keepRunning;
    int numCSC, numEvC, calcSamplingFreq, samplingRateFromADC;

private:
    int maxCscSamples, recordBufferSize, maxEventStringLength;
    char cheetahObjects[MAX_OBJ][MAX_LEN];
    char cheetahTypes[MAX_OBJ][MAX_LEN];
    int cscIndex[MAX_OBJ];
    int evIndex[MAX_OBJ];


    int numEventsRead, numEventsWritten;
    int mEventTtlBinaryBase;

    int numRecordsReturned, numRecordsDropped;
    int mIndex;
    int mBufferSample, mBufferSampleReturned, mChannelReturned, indexFollower, eventFollower;
    queue<uint64_t> mIndexOfTimeStamps;

protected:

    char *channelNamesForChunk;
    int channelNamesSize;
    short *retBufSamples;
    uint64_t *retBufTimeStamps;
    int *retBufChannelNumbers;
    int *retBufValidSamples;
    int *retBufSamplingFreq;

    int *retBufEventIDs;
    int *retBufTTL;
    char **retBufEventString;

    typedef struct
    {
        char *name;
        int eventID;
        int ttl;
    } Event;

    typedef struct
    {
        int status; 	// 0=empty, 1=being filled, 2=being sent
        int chansFilled;
        uint64_t timeStamp;
        short *samples; /* numChannels x maxCscSamples */
        Event *events;
    } MultiChannelBlock;


    typedef struct {
        Event *events;
    } EventBlock;


    MultiChannelBlock mcbRingBuf[RB_SIZE];
    EventBlock evRingBuf[RB_SIZE];

    private:
        int Execute();

};




#endif //NEURALYNX_THREAD_H

