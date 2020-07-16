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

#ifndef TDTADCH
#define TDTADCH

#include "GenericADC.h"
#import "RPcoX.ocx"
#import "zBUSx.ocx"
#include <stdio.h>
#include <vector>
#include <string>
#include "Thread.h"

class TDTADC : public GenericADC
{
public:
	TDTADC();
    ~TDTADC();

    //
    void Preflight(const SignalProperties&, SignalProperties&) const;
    void Initialize(const SignalProperties&, const SignalProperties&);
    void Process( const GenericSignal&, GenericSignal&);
    void Halt();

    void reset();

private:
	RPCOXLib::_DRPcoXPtr RPcoX1;
	ZBUSXLib::_DZBUSxPtr ZBus;

    void dropSamples(GenericSignal& outputSignal);

    int	mSourceCh;
    int	mSampleBlockSize;
    int mSamplingRate;

    int nChannels;
    int nProcessors;
    double LPFfreq;
    double HPFfreq;
    double notchBW;
    double TDTsampleRate;
    double TDTgain;
    int TDTbufSize;
    int blockSize;
    int TDTbufBlocks;
    short connectType;
    int curindex, stopIndex, indexMult;
    int devAddr[2];
    bool use2RX5;
    bool useECG;
    float ECGgain;
    int ECGchannel;
    int ECGoffset;
    int ECGstopIndex;
    int mUseFrontPanel;
    int mEEGchannels;

    int mFrontPanelChannels;
    float mDigitalGain;
    float mFrontPanelGain;
    FILE * logFile;
	int mThreadBlock, mProcBlock;

	std::vector<float*> mData;
	std::vector<std::string> mDataTags, mDataIndex;

	class AcquireThread;
    HANDLE acquireEventRead;
    friend class AcquireThread;
    class AcquireThread : public Thread
    {
      public:
        AcquireThread( TDTADC * parent )
        : amp( parent )
        {
            SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);
            mData = NULL;
        }
        float getNextValue();
        private:
            int OnExecute() override;
			void init();
			void reset();
            TDTADC *amp;
            int mBufferReadPos, mBufferWritePos;
            float *mData;
			int mBufferSize;
			std::vector< std::vector<HANDLE> >         m_hEvent;
			std::vector<float*> mDataBuf;
			std::vector<std::string> mDataTags;
			int mOffset;
			std::vector<int> mChList;

	} *mpAcquireThread;
};

//---------------------------------------------------------------------------
#endif
