////////////////////////////////////////////////////////////////////////////////
//
// File: DASQueue.cpp
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
#include "DASQueue.h"

#include "DAS_lib/cbw.h"
#include "DAS_lib/DASUtils.h"
#include "BCIStream.h"
#include <sstream>
#include <typeinfo>

using namespace std;

DASQueue::DASQueue()
: mFailstate( ok ),
  mBoardNumber( NOTUSED ),
  mFreqMultiplier( 1 ),
  mChannels( 0 ),
  mHWChannels( 0 ),
  mDataBuffer( NULL ),
  mDataBufferSize( 0 ),
  mReadCursor( 0 ),
  mWriteCursor( 0 ),
  mShouldBeOpen( false ),
  mTimeoutInterval( 0 ),
  mDataAvailableEvent( ::CreateEvent( NULL, FALSE, FALSE, NULL ) )
{
  float currentRevNum = static_cast<float>( CURRENTREVNUM );
  ::cbDeclareRevision( &currentRevNum );
  ::cbErrHandling( DONTPRINT, DONTSTOP );
}

DASQueue::~DASQueue()
{
  close();
  ::CloseHandle( mDataAvailableEvent );
}

void
DASQueue::open( const DASInfo& inInfo )
{
  if( mFailstate != ok )
    return;

  if( is_open() )
    close();

  mShouldBeOpen = true;
  mBoardNumber = inInfo.boardNumber;
  mTimeoutInterval = cTimeoutFactor * ( 1000 * inInfo.sampleBlockSize ) / inInfo.samplingRate;
  if( mTimeoutInterval < 1 )
    mTimeoutInterval = 1;

  // Check whether there are version conflicts between DLL and board driver.
  float ignored;
  int result = ::cbGetRevision( &ignored, &ignored );
  if( result == NOERRORS )
  {
    int adRange = DASUtils::GetADRangeCode( inInfo.adRangeMin, inInfo.adRangeMax ),
        hwRange = NOTUSED;
    result = ::cbGetConfig( BOARDINFO, mBoardNumber, 0, BIRANGE, &hwRange );
    if( result == NOERRORS )
    {
      if( !DASUtils::ADRangeCompatible( hwRange, inInfo.adRangeMin, inInfo.adRangeMax ) )
      {
        bcierr << "The specified A/D range does not match the associated"
                  " hardware switch setting" << endl;
        adRange = hwRange;
      }

      mHWChannels = 0;
      mChannels = inInfo.numChannels;
      result = ::cbGetConfig( BOARDINFO, mBoardNumber, 0, BINUMADCHANS, &mHWChannels );
      if( result == NOERRORS )
      {
        if( mHWChannels < mChannels )
        {
          bcierr << "Requested number of channels exceeds number of available"
                 << " hardware channels" << endl;
          mChannels = mHWChannels;
        }

        long hwBlockSize;
        result = DASUtils::GetTransferBlockSize( mBoardNumber, hwBlockSize );
        if( result != NOERRORS )
          bcierr << "Could not measure the board's transfer block size" << endl;
        else if( !DASUtils::IsPowerOf2( hwBlockSize ) )
          bciwarn << "The board's transfer block size ("
                  << hwBlockSize << ") is not a power of 2.\n"
                  << "For non-demo boards this may indicate a problem"
                  << " with the method used to measure the block size"
                  << endl;

        // For large hardware buffers, we need to increase the sampling rate to
        // obtain the block update rate implied by the parameters.
        mFreqMultiplier = hwBlockSize / DASUtils::GreatestCommonDivisor(
                             hwBlockSize, mHWChannels * inInfo.sampleBlockSize );
        mDataBufferSize = cBlocksInBuffer * hwBlockSize;

        long hwSamplingRate = inInfo.samplingRate * mFreqMultiplier;
        int  options = CONTINUOUS | BACKGROUND;
        while( hwSamplingRate > 0
          && BADRATE == ( result = DASUtils::GetBoardOptions(
                                             mBoardNumber, mHWChannels,
                                             mDataBufferSize, hwSamplingRate,
                                             adRange, options ) ) )
        {
          hwSamplingRate /= 2;
          mFreqMultiplier = ( 2 * hwSamplingRate + inInfo.samplingRate ) / ( 2 * inInfo.samplingRate );
        }

        if( result == NOERRORS )
        {
          // The sampling rate may have changed.
          if( hwSamplingRate / mFreqMultiplier != inInfo.samplingRate )
          {
            // hwBlockSize is typically a power of 2, so increasing the software
            // block size to contain an additional factor of two appears a useful
            // strategy to fix the problem.
            // Factor the software block size into powers of 2 and remaining factors:
            int swBlockSize = inInfo.sampleBlockSize,
                powersOf2 = 1;
            while( ( swBlockSize & powersOf2 ) == 0 )
              powersOf2 <<= 1;
            // Increase the remaining part by 1 to introduce an additional factor of 2:
            swBlockSize += powersOf2;

            bciwarn << "Sampling rate/block size combination not optimally supported by A/D board"
                    << " (try a block size of " << swBlockSize << ")."
                    << " The actual sampling rate is now "
                    << hwSamplingRate / mFreqMultiplier << "/s"
                    << endl;
          }

          // A memory allocation failure will trigger an error below.
          mDataBuffer = ( USHORT* )::cbWinBufAlloc( mDataBufferSize );
          mReadCursor = 0;
          mWriteCursor = 0;
          result = DASUtils::BackgroundScan( mBoardNumber, 0, mHWChannels - 1,
                                             mDataBufferSize, hwSamplingRate,
                                             adRange, mDataBuffer, options,
                                             BoardEventCallback, this );
          if( result == NOERRORS )
            ReceiveData();
        }
      }
    }
  }
  if( result != NOERRORS )
  {
    mFailstate |= initFail;
    bcierr << "A/D error: "
           << DASUtils::GetErrorMessage( result )
           << endl;
  }
}

void
DASQueue::close()
{
  DASUtils::StopBackground( mBoardNumber );
  ::cbWinBufFree( mDataBuffer );
  mDataBuffer = NULL;
  mReadCursor = 0;
  mWriteCursor = 0;
  mShouldBeOpen = false;
}

const short&
DASQueue::front()
{
  while( empty() && mFailstate == ok )
    ReceiveData();
  if( empty() )
  {
    static short null = 0;
    return null;
  }
  return std::queue<short>::front();
}

// This is a _blocking_ function that waits until at least one sample arrived.
// We modify the queue here and not directly in the callback because the
// callback runs in a different thread and thus might access the queue data
// concurrently.
void
DASQueue::ReceiveData()
{
  if( ::WaitForSingleObject( mDataAvailableEvent, mTimeoutInterval ) == WAIT_OBJECT_0 )
  {
    for( ; mReadCursor != mWriteCursor; mReadCursor = ( mReadCursor + 1 ) % mDataBufferSize )
      if( !IgnoredSample( mReadCursor ) )
        push( mDataBuffer[ mReadCursor ] - ( 1 << 15 ) );
  }
}

inline
bool
DASQueue::IgnoredSample( long inIndex )
{
  bool notSWChannel = ( inIndex % mHWChannels >= mChannels ),
       notSWSample = ( ( inIndex / mHWChannels ) % mFreqMultiplier != 0 );
  return notSWChannel || notSWSample;
}

// This is not a member function but we get an instance pointer in the
// user data argument and cast it to a quasi-this pointer.
void
CALLBACK
DASQueue::BoardEventCallback( int      inBoardNumber,
                              unsigned inEventType,
                              unsigned inEventData,
                              void*    inUserData )
{
  DASQueue* _this = static_cast<DASQueue*>( inUserData );
  bool saneArguments = ( _this != NULL && inBoardNumber == _this->mBoardNumber );
  if( saneArguments )
  {
    switch( inEventType )
    {
      case ON_DATA_AVAILABLE:
        _this->mWriteCursor = inEventData % _this->mDataBufferSize;
        ::SetEvent( _this->mDataAvailableEvent );
        break;
      case ON_SCAN_ERROR:
        {
          bcierr << "A/D error: "
                 << DASUtils::GetErrorMessage( inEventData )
                 << endl;
          DASUtils::StopBackground( inBoardNumber );
        }
        break;
      default:
        saneArguments = false;
    }
  }
  if( !saneArguments )
    bcierr << "A/D callback function called with inconsistent arguments"
           << endl;
}

