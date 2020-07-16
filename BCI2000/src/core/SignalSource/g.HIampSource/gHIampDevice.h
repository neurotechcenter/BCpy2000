////////////////////////////////////////////////////////////////////////////////
// $Id: gHIampDevice.h 5374 2016-05-24 16:28:43Z mellinger $
// Authors: griffin.milsap@gmail.com, juergen.mellinger@uni-tuebingen.de
// Description: A class which reqpresents a gHIamp device
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
#ifndef INCLUDED_GHIAMPDEVICE_H
#define INCLUDED_GHIAMPDEVICE_H

#include <vector>
#include <memory>
#include "GenericSignal.h"
#include "AsyncIODevice.h"
#include "Thread.h"
#include "Lockable.h"

#include "gHIamp.imports.h"

class gHIampDevice : public bci::AsyncIODevice
{
  typedef gHIampDevice Self;

 public:
  typedef std::shared_ptr<gHIampDevice> Ptr;
  typedef std::vector<Ptr> List;

  struct Config
  {
    int RefChannel, CounterChannel, IOQueueSize, SampleBlockSize;
    float SamplingRate, Notch, HPCorner, LPCorner, SignalGeneratorFreq;
    bool IsMaster, HoldEnabled, ActiveElectrodes;
    const int PODByteCount; // make sure to put all POD members before this member
    typedef std::vector< std::pair<int, int> > ChannelMap;
    ChannelMap AnalogChannels, DigitalChannels;
    
    Config();
    Config& operator=( const Config& other );
    bool operator==( const Config& other ) const;
    bool operator!=( const Config& other ) const { return !operator==( other ); }
    void AddAnalogChannel( int ampCh, int outCh ) { AnalogChannels.push_back( std::make_pair( ampCh, outCh ) ); }
    void AddDigitalChannel( int ampDigIn, int outCh ) { DigitalChannels.push_back( std::make_pair( ampDigIn, outCh ) ); }
  };

  static void DetectDevices( List&, int inMaxEnumeratorIndex = 255 );

  gHIampDevice();
  ~gHIampDevice();

  bool Open( int deviceIndex = -1 );
  bool IsOpen() const { return mDevice; }
  void Close();

  // Properties (partially renamed to match BCI2000 parameter terminology)
  static int HardwareChannels() { return MAX_NUMBER_OF_CHANNELS; }
  static int DigitalInputs() { return 16; }
  static int CounterChannel() { return 0; }
  static int MaximumBlockSize() { return 256; }
  const float HWVersion() const { return mHWVersion; }

  Self& SetId( const std::string& );
  const std::string& Id() const { return mId; }

  Self& SetTag( int i ) { mTag = i; return *this; }
  int Tag() const { return mTag; }

  const Self& VerifyConfiguration( const Config& ) const;
  Self& SetConfiguration( const Config& );
  const Config& Configuration() const { return mConfig; }
  const Time::Interval& BlockDuration() const { return mBlockDuration; }

  // Device control
  void StartAcquisition();
  void StopAcquisition();
  void MoveData( const IORequest&, GenericSignal& Output );

  void StartImpedanceAcquisition();
  void StopImpedanceAcquisition();
  void MoveImpedanceData( GenericSignal& Output );

  static std::string GetErrorMessage();

 protected:
  bool OnQueueRequest( IORequest& ) override;
  bool OnRequestCompletion( IORequest& ) override;

 private:
  GT_HIAMP_CONFIGURATION TranslateConfiguration( const Config& ) const;
  struct ImpedanceAcquisition : Thread, Lockable<>
  {
    int OnExecute() override;
    gHIampDevice* mpSelf;
    GT_HIAMP_CHANNEL_IMPEDANCES mData;
  } mImpedanceAcquisition;

  std::string mId;
  float mHWVersion;
  int mTag, mRefIdx;
  uint32_t mCounter;
  bool mFirstRequest;
  size_t mValuesPerScan, mExpectedBytes;
  HANDLE mDevice;
  Config mConfig;
  Time::Interval mBlockDuration;
  GT_HIAMP_CONFIGURATION mConfigStructCache;

  static std::vector<FILT> sNotchFilters, sBandpassFilters;
};

#endif // INCLUDED_GHIAMPDEVICE_H