////////////////////////////////////////////////////////////////////////////////
// $Id: BlackrockADC.h 5818 2018-11-09 15:50:15Z mellinger $
// Authors: griffin.milsap@gmail.com
// Description: Implementation of a source module for Blackrock systems
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

#ifndef INCLUDED_BLACKROCKADC_H
#define INCLUDED_BLACKROCKADC_H

#include "BufferedADC.h"
#include "PrecisionTime.h"
#include "Mutex.h"
#include "Expression.h"

#include "cbsdk.imports.h"

#include <vector>
#include <queue>

class BlackrockADC : public BufferedADC
{
 public:
           BlackrockADC();
  virtual ~BlackrockADC();
  virtual void OnAutoConfig();
  virtual void OnHalt();
  virtual void OnPreflight( SignalProperties& Output ) const;
  virtual void OnInitialize( const SignalProperties& Output );
  virtual void OnProcess();
  virtual void StartRun();
  virtual void OnStartAcquisition();
  virtual void DoAcquire( GenericSignal& Output );
  virtual void OnStopAcquisition();
  virtual void StopRun();

  // Bit of a hack necessary for digital output based on expressions
  virtual void Process( const GenericSignal& Input, GenericSignal& Output );

 private:
  struct ChanInfo
  {
    unsigned int inst;
    unsigned int idx;
    double gain;
    double offset;
    std::string label;
  };

  bool Connect( int nInstances = 1 ) const;
  void Disconnect( int nInstances = 1 ) const;
  bool GetChannelConfig( int iNumInstances, int iGroup,
                         std::vector< ChanInfo > &oChanConfig,
                         std::vector< int > &oSyncChans ) const;
  static void DataCallback( UINT32 iInstance, const cbSdkPktType iType, const void* iData, void* iBlackrockADC );
  static bool CereLinkError( cbSdkResult res );
  int GetRequestedSampleGroup() const;
  double ScalingToGain( cbSCALING scaling ) const;

  // Debug Functionality
  void OutputChannelDebugInfo( int nInstances ) const;

  Mutex mDataMutex, mCommandMutex;
  std::vector< std::queue< std::vector< INT16 > > > mDataPacketBuffers;
  std::vector< ChanInfo > mChannelConfig;
  std::vector< int > mSyncChans;
  int mSampleGroup;
  unsigned int mNSPInstances;
  unsigned int mSampleBlockSize;
  bool mSyncEnabled, mPauseForSync;

  struct DigitalOutputExpression
  {
    unsigned int inst;
    unsigned int dout;
    Expression exp;
  };
  std::vector< DigitalOutputExpression > mDigitalOutputEx;

};

#endif // INCLUDED_BLACKROCKADC_H
