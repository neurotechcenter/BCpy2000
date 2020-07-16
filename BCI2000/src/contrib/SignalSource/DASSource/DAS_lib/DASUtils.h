////////////////////////////////////////////////////////////////////////////////
//
// File: DASUtils.h
//
// Author: juergen.mellinger@uni-tuebingen.de
//
// Date: Sep 22, 2003
//
// Description: Functions that one would like to be part of MMC's Universal
//              Library but that unfortunately aren't.
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
#ifndef DASUtilsH
#define DASUtilsH

#include "cbw.h"
#include <string>
#include <map>

// These constants should actually be in cbw.h.
enum
{
  PC_CARD_DAS16_16 = 56,
  CIO_DAS1402_16 = 3590,
  PCM_DAS16S_16 = 16393,
 };

class DASUtils
{
 public:
  // Convert a numerical range to the corresponding UL range code, if possible.
  static int GetADRangeCode( float inMin,
                             float inMax );

  // Check whether a numerical range is compatible with an UL range code.
  // Considers UNIPOLAR and BIPOLAR range codes.
  static bool ADRangeCompatible( int   inRangeCode,
                                 float inMin,
                                 float inMax );

  // Measure the size of the chunks in which data are transferred from the
  // board. In most cases, this should equal half the size of the board's
  // FIFO buffer.
  static int GetTransferBlockSize( int   inBoardNumber,
                                   long& outBlockSize );

  // Check whether a board supports certain options, and return the combination
  // we consider 'best'.
  static int GetBoardOptions( int   inBoardNumber,
                              int   inChannels,
                              long& ioCount,
                              long& ioSamplingRate,
                              int   inADRange,
                              int&  ioOptions );

  // Check whether a board supports the event/callback mechanism.
  // If not, the BackgroundScan() function will still simulate event callbacks.
  static bool BoardSupportsEvents( int inBoardNumber );

  // This is a version of cbAInScan() for continuous background
  // scans that simulates events for boards that don't support cbEnableEvent().
  static int BackgroundScan( int           inBoardNumber,
                             int           inLowChan,
                             int           inHighChan,
                             long          inDataCount,
                             long&         ioRate,
                             int           inADRange,
                             HGLOBAL       inDataBuffer,
                             int&          ioOptions,
                             EVENTCALLBACK inNotificationCallback,
                             void*         inUserData );

  // If using BackgroundScan(), use this instead of cbStopBackground().
  static int StopBackground( int inBoardNumber );

  // A more convenient way to get error messages.
  static std::string GetErrorMessage( int inError );

  // The following utility functions might go elsewhere.
  template<typename T> static T GreatestCommonDivisor( T m, T n )
  {
    T remainder = 0;
    while( 0 != ( remainder = m % n ) )
    {
      m = n;
      n = remainder;
    }
    return n;
  }

  // The next two functions are useful to check the result of GetTransferBlockSize() for sanity.

  // Check whether an integer is a power of 2.
  template<typename T> static bool IsPowerOf2( T inNumber )
  {
    return BinaryCrossSum( inNumber ) == 1;
  }

  // Compute the cross sum of an integer.
  template<typename T> static unsigned BinaryCrossSum( T inNumber )
  {
    unsigned crossSum = 0;
    while( inNumber != 0 )
    {
      crossSum += ( inNumber & 1 );
      inNumber >>= 1;
    }
    return crossSum;
  }

 private:
  // Constants used in GetTransferBlockSize().
  //
  // The sampling rate used for measuring the transfer block size.
  // If it is too low, the test measurement takes quite long for large
  // FIFOs.
  static const int testSamplingRate = 10000;

  // This is the size of the ring buffer used for measuring the transfer
  // block size. If this is below the FIFO size for a board, the test will
  // fail.
  static const int testSampleCount = 8192;

  static struct ADRangeEntry
  {
    int code;
    float min, max;
  } ranges[];

  static const int tryOptions[];

  struct LoopData
  {
    int           boardNumber,
                  dataSize;
    EVENTCALLBACK callback;
    void*         userData;
  };

  // This is a polling function used to simulate hardware interrupts for
  // boards that don't support the cbEnableEvent() function.
  // It runs inside its own thread and calls the callback function from
  // there.
  static DWORD WINAPI DataLoop( LPVOID );
  static std::map<int, HANDLE> threadHandles;
};

#endif // DASUtilsH
