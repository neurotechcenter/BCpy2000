////////////////////////////////////////////////////////////////////////////////
//
// File: DASQueue.h
//
// Author: juergen.mellinger@uni-tuebingen.de
//
// Date: Sep 23, 2003
//
// Description: A class that interfaces with A/D boards supported by
//              MeasurementComputing's Universal Library. 
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
#ifndef DASQUEUEH
#define DASQUEUEH

#include <queue>
#include <windows.h>

class DASQueue : public std::queue<short>
{
  public:
    struct DASInfo
    {
      long  boardNumber,
            samplingRate,
            sampleBlockSize,
            numChannels;
      float adRangeMin, adRangeMax;
    };
    enum
    {
      ok = 0,
      initFail = 1 << 0,
      connectionFail = 1 << 1,
      memoryFail = 1 << 2,
    };
    DASQueue();
    ~DASQueue();
    void open( const DASInfo& inInfo );
    void close();
    void clear()          { c.clear(); mFailstate = ok; mShouldBeOpen = false; }
    const short& front();
    operator bool() const { return mFailstate == ok; }
    bool is_open() const  { return mShouldBeOpen && mFailstate == ok; }

  private:
    static const int cTimeoutFactor = 2;  // How many sample block durations
                                          // the timeout should be.
    static const int cBlocksInBuffer = 4; // How many sample blocks shall
                                          // fit into the sample buffer.

    int     mFailstate,
            mBoardNumber,
            mFreqMultiplier,
            mChannels,
            mHWChannels;
    USHORT* mDataBuffer;
    long    mDataBufferSize,
            mReadCursor,
            mWriteCursor;
    bool    mShouldBeOpen;
    DWORD   mTimeoutInterval;
    HANDLE  mDataAvailableEvent;

    void ReceiveData();
    bool IgnoredSample( long inIndex );
    static void CALLBACK BoardEventCallback( int inBoardNumber,
                         unsigned inEventType, unsigned inEventData, void* inUserData );
};

#endif // DASQUEUEH
