////////////////////////////////////////////////////////////////////////////////
//
// File: DASUtils.cpp
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
#include "PCHIncludes.h"
#pragma hdrstop

#include "DASUtils.h"
#include "cbw.h"
#include <limits.h>
#include <mmsystem.h>
#include <sstream>

std::map<int, HANDLE> DASUtils::threadHandles;

DASUtils::ADRangeEntry DASUtils::ranges[] =
{
  { BIP10VOLTS,   -10.0f,  10.0f   }, { UNI10VOLTS,    0.0f, 10.0f  },
  { BIP5VOLTS,     -5.0f,   5.0f   }, { UNI5VOLTS,     0.0f,  5.0f  },
  { BIP2PT5VOLTS,  -2.5f,   2.5f   }, { UNI2PT5VOLTS,  0.0f,  2.5f  },
                                      { UNI2VOLTS,     0.0f,  2.0f  },
  { BIP1PT25VOLTS, -1.25f,  1.25f  }, { UNI1PT25VOLTS, 0.0f,  1.25f },
  { BIP1PT67VOLTS, -1.67f,  1.67f  }, { UNI1PT67VOLTS, 0.0f,  1.67f },
  { BIP1VOLTS,     -1.0f,   1.0f   }, { UNI1VOLTS,     0.0f,  1.0f  },
  { BIPPT625VOLTS, -0.625f, 0.625f },
  { BIPPT5VOLTS,   -0.5f,   0.5f   },
  { BIPPT1VOLTS,   -0.1f,   0.1f   }, { UNIPT1VOLTS,   0.0f,  0.1f  },
  { BIPPT05VOLTS,  -0.05f,  0.05f  },
                                      { UNIPT02VOLTS,  0.0f,  0.02f },
  { BIPPT01VOLTS,  -0.01f,  0.01f  }, { UNIPT01VOLTS,  0.0f,  0.01f },
  { BIPPT005VOLTS, -0.005f, 0.005f },
};

// Try a number of options in the order of preferability.
const int DASUtils::tryOptions[] =
{
  BLOCKIO | BURSTMODE | CONVERTDATA,
  BLOCKIO | CONVERTDATA,
  BLOCKIO | BURSTMODE,
  BLOCKIO,
  BURSTMODE | CONVERTDATA,
  CONVERTDATA,
  BURSTMODE,
  0,
};

int
DASUtils::GetADRangeCode( float rangeMin, float rangeMax )
{
  // cf ComputerBoards Universal Library Function Reference, p13
  int ADRange = NOTUSED;
  for( size_t i = 0; ADRange == NOTUSED && i < sizeof( ranges ) / sizeof( *ranges ); ++i )
    if( ranges[ i ].min == rangeMin && ranges[ i ].max == rangeMax )
      ADRange = ranges[ i ].code;
  return ADRange;
}

bool
DASUtils::ADRangeCompatible( int inRange, float rangeMin, float rangeMax )
{
  int rangeCode = GetADRangeCode( rangeMin, rangeMax ),
      polarity = ( rangeMin == 0 ? UNIPOLAR : BIPOLAR );
  return ( inRange == NOTUSED ) || ( inRange == rangeCode ) || ( inRange == polarity );
}

int
DASUtils::GetTransferBlockSize( int inBoardNumber, long& outBlockSize )
{
  // Try looking up the transfer block size in a table.
  // This will speed up initialization.
  bool boardInTable = true;
  int boardType = 0;
  if( NOERRORS == ::cbGetConfig( BOARDINFO, inBoardNumber, 0, BIBOARDTYPE, &boardType ) )
  {
    switch( boardType )
    {
      case CIO_DAS1402_16:
        outBlockSize = 512;
        break;
      case PC_CARD_DAS16_16:
        outBlockSize = 2048;
        break;
      case PCM_DAS16S_16:
        outBlockSize = 256;
        break;
      default:
        boardInTable = false;
    }
  }
  if( boardInTable )
    return NOERRORS;

  // The board is not in the table -- try measuring the block size.

  // Determine a ADRange parameter supported by the board.
  int result = BADRANGE,
      ADRange = NOTUSED;
  unsigned short data;
  for( size_t i = 0; result == BADRANGE && i < sizeof( ranges ) / sizeof( *ranges ); ++i )
  {
    result = ::cbAIn( inBoardNumber, 0, ranges[ i ].code, &data );
    ADRange = ranges[ i ].code;
  }

  // Let's measure how many new samples trigger a change in the result of the
  // cbGetStatus() function.
  if( result == NOERRORS )
  {
    result = ::cbStopBackground( inBoardNumber, AIFUNCTION );
    if( result == NOERRORS )
    {
      int numADChans = 0;
      result = ::cbGetConfig( BOARDINFO, inBoardNumber, 0, BINUMADCHANS, &numADChans );
      if( result == NOERRORS )
      {
        HGLOBAL data = ::cbWinBufAlloc( testSampleCount );
        long samplingRate = testSamplingRate;
        do
        {
          result = ::cbAInScan( inBoardNumber, 0, numADChans - 1, testSampleCount,
                   &samplingRate, ADRange, data, CONTINUOUS | BACKGROUND );
        } while( result != NOERRORS && ( samplingRate /= 2 ) > 0 );

        if( result == NOERRORS )
        {
          unsigned long timeout = ( 1000 * testSampleCount ) / ( samplingRate * numADChans );
          if( timeout < 10 )
            timeout = 10;
          long lastCount = 0,
               minDiff = LONG_MAX,
               curIndex;
          short status;
          result = ::cbGetStatus( inBoardNumber, &status, &lastCount, &curIndex, AIFUNCTION );
          DWORD beginTime = ::timeGetTime();
          while( result == NOERRORS && ( ::timeGetTime() - beginTime ) < timeout )
          {
            long curCount;
            result = ::cbGetStatus( inBoardNumber, &status, &curCount, &curIndex, AIFUNCTION );
            if( curCount != lastCount )
            {
              long diff = curCount - lastCount;
              if( ( diff > 0 ) && ( diff < minDiff ) )
                minDiff = diff;
            }
            lastCount = curCount;
          }
          if( result == NOERRORS )
          {
            if( minDiff < LONG_MAX )
              outBlockSize = minDiff ;
            else
              result = NOTUSED;
          }
        }
        ::cbStopBackground( inBoardNumber, AIFUNCTION );
        ::cbWinBufFree( data );
      }
    }
  }
  return result;
}

// This tries buffer sizes and options to obtain allowed values that are as
// close as possible.
int
DASUtils::GetBoardOptions( int   inBoardNumber,
                           int   inChannels,
                           long& ioCount,
                           long& ioSamplingRate,
                           int   inADRange,
                           int&  ioOptions )
{
  const int maxCount = 1 << 20;

  // Try options in the order given in tryOptions.
  // Options independent of the ones given in tryOptions
  // will not be touched.
  int numTryOptions = sizeof( tryOptions ) / sizeof( *tryOptions ),
  tryMask = 0;
  for( int i = 0; i < numTryOptions; ++i )
    tryMask |= tryOptions[ i ];

  int result = ::cbStopBackground( inBoardNumber, AIFUNCTION );
  if( result == NOERRORS )
  {
    int i;
    HGLOBAL bufMem = ::cbWinBufAlloc( ioCount );
    for( i = 0, result = BADOPTION;
         i < numTryOptions && ( result == BADOPTION || result == BADSAMPLEMODE );
         ++i )
    {
      ioOptions &= ~tryMask;
      ioOptions |= tryOptions[ i ];
      // An allocation failure will show up as an error in cbAInScan.
      result = ::cbAInScan( inBoardNumber , 0, inChannels - 1, ioCount,
                                &ioSamplingRate, inADRange, bufMem, ioOptions );
      ::cbStopBackground( inBoardNumber, AIFUNCTION );
    }
    ::cbWinBufFree( bufMem );
    for( i = ioCount, result = BADCOUNT;
         i < maxCount && ( result == BADCOUNT || result == CONTINUOUSCOUNT );
         ++i )
    {
      HGLOBAL bufMem = ::cbWinBufAlloc( i );
      // An allocation failure will show up as an error in cbAInScan.
      result = ::cbAInScan( inBoardNumber , 0, inChannels - 1, i,
                                &ioSamplingRate, inADRange, bufMem, ioOptions );
      ::cbStopBackground( inBoardNumber, AIFUNCTION );
      ::cbWinBufFree( bufMem );
    }
    ioCount = i;
  }
  return result;
}

bool
DASUtils::BoardSupportsEvents( int inBoardNumber )
{
  int result = ::cbDisableEvent( inBoardNumber, ALL_EVENT_TYPES );
  ::cbStopBackground( inBoardNumber, AIFUNCTION );
  return result != BADBOARDTYPE;
}

int
DASUtils::BackgroundScan( int inBoardNumber, int inLowChan, int inHighChan,
                          long inDataCount, long& ioRate, int inADRange,
                          HGLOBAL inDataBuffer, int& ioOptions,
                          EVENTCALLBACK inNotificationCallback, void* inUserData )
{

  int result = ::cbStopBackground( inBoardNumber, AIFUNCTION );
  if( result == NOERRORS )
  {
    result = ::cbEnableEvent( inBoardNumber, ON_DATA_AVAILABLE | ON_SCAN_ERROR, 1,
                                                inNotificationCallback, inUserData );
    bool simulateEvents = false;
    if( result == BADBOARDTYPE )
    {
      simulateEvents = true;
      result = ::cbStopBackground( inBoardNumber, AIFUNCTION );
    }
    if( result == NOERRORS )
    {
      ioOptions |= CONTINUOUS | BACKGROUND;
      result = ::cbAInScan( inBoardNumber, inLowChan, inHighChan, inDataCount,
                                       &ioRate, inADRange, inDataBuffer, ioOptions );
    }
    if( result == NOERRORS && simulateEvents )
    {
      LoopData _loopData =
      {
        inBoardNumber,
        inDataCount,
        inNotificationCallback,
        inUserData
      },
      *loopData = new LoopData( _loopData ); // The delete operator will be called
                                             // from the created thread.
      threadHandles[ inBoardNumber ] = ::CreateThread( NULL, 0, DataLoop, loopData, 0, NULL );
      if( threadHandles[ inBoardNumber ] == NULL )
      {
        delete loopData;
        result = NOTUSED;
      }
    }
  }
  if( result != NOERRORS )
  {
    ::cbDisableEvent( inBoardNumber, ALL_EVENT_TYPES );
    ::cbStopBackground( inBoardNumber, AIFUNCTION );
  }
  return result;
}

int
DASUtils::StopBackground( int inBoardNumber )
{
  ::cbDisableEvent( inBoardNumber, ALL_EVENT_TYPES );
  int result = ::cbStopBackground( inBoardNumber, AIFUNCTION );
  if( threadHandles[ inBoardNumber ] != NULL )
  {
    ::WaitForSingleObject( threadHandles[ inBoardNumber ], 500 );
    ::CloseHandle( threadHandles[ inBoardNumber ] );
    threadHandles[ inBoardNumber ] = NULL;
  }
  return result;
}

// This is a polling thread function used to simulate hardware interrupts for
// boards that don't support the cbEnableEvent() function.
DWORD
WINAPI
DASUtils::DataLoop( LPVOID inArg )
{
  LoopData* data = static_cast<LoopData*>( inArg );
  if( data->callback == NULL || data->dataSize == 0 )
    return 0;

  int   result = NOERRORS;
  short status = RUNNING;
  long  lastCount = 0,
        curCount = 0,
        curIndex = 0;

  while( status == RUNNING && result == NOERRORS )
  {
    while( status == RUNNING && result == NOERRORS && curCount == lastCount )
    {
      ::Sleep( 1 );
      result = ::cbGetStatus( data->boardNumber, &status, &curCount, &curIndex, AIFUNCTION );
      curCount %= data->dataSize;
    }
    // If the board has just become idle, we should not assume valid user data,
    // i.e. we should not call the callback function.
    if(  status == RUNNING )
    {
      if( result == NOERRORS  )
        data->callback( data->boardNumber, ON_DATA_AVAILABLE, curCount, data->userData );
      else
        data->callback( data->boardNumber, ON_SCAN_ERROR, result, data->userData );
    }
    lastCount = curCount;
  }
  delete data;
  return 0;
}

std::string
DASUtils::GetErrorMessage( int inError )
{
  std::ostringstream errorMessage;
  switch( inError )
  {
    case NOTUSED:
      errorMessage << "Shared library cbw32.dll not found or too old";
      break;
    default:
      char buffer[ ERRSTRLEN + 1 ] = "Error when obtaining error message";
      ::cbGetErrMsg( inError, buffer );
      errorMessage << buffer
                   << " (Error #" << inError << ")";
  }
  return errorMessage.str();
}

