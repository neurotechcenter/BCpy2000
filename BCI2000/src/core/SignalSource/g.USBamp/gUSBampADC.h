////////////////////////////////////////////////////////////////////////////////
// $Id: gUSBampADC.h 5200 2016-01-12 21:01:06Z mellinger $
// Author: schalk@wadsworth.org
// Description: BCI2000 Source Module for gUSBamp devices.
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
#ifndef GUSBAMP_ADC_H
#define GUSBAMP_ADC_H

#include "GenericADC.h"
#include "GenericVisualization.h"

#include <vector>
#include <string>
#include <memory>
#include "Win32Defs.h"
#include "Thread.h"
#include "PrecisionTime.h"
#include "Expression/Expression.h"

class gUSBampADC : public GenericADC
{
 public:
  gUSBampADC();
  ~gUSBampADC();
  void Publish();
  void AutoConfig( const SignalProperties& ) override;
  void Preflight( const SignalProperties&, SignalProperties& ) const override;
  void Initialize( const SignalProperties&, const SignalProperties& ) override;
  void Process( const GenericSignal&, GenericSignal& ) override;
  void Halt() override;
  bool SetsSourceTime() const override { return true; }

 private:
  struct Device;
  typedef std::shared_ptr<Device> DevicePtr;
  typedef std::vector<DevicePtr> DeviceList;
  DeviceList mDevicesDetected, mDevicesUsed;

  bool EnumerateDevices( DeviceList& ) const;
  bool MeasureImpedance( const DevicePtr& );
  int DetermineFilterNumber() const;
  int DetermineNotchNumber() const;
  GenericVisualization mVis;
  float m_filterhighpass, m_filterlowpass, m_notchhighpass, m_notchlowpass; // at the moment, only one filter setting for all channels and all devices
  int m_filtermodelorder, m_filtertype, m_notchmodelorder, m_notchtype;
  int m_blockduration_ms;
  int m_digitalOutputMode;
  int m_acqmode; enum { normal, calibrate, impedance, counter };
  bool m_digitalOut1;
  int mEnableDigitalInput;

  std::map<int, Expression> mDigOutputMatrix;
  HANDLE mSourceBufferSemaphore, mWakeupEvent;
  int mReadCursor, mIOQueueLength;

  struct
  {
    bool on;
    double lastSampleCount;
  } mDetectDataLoss;

  class AcquireThread;
  friend class AcquireThread;
  class AcquireThread : public Thread
  {
   public:
    AcquireThread( gUSBampADC * parent ) : amp( parent ) {}
    ~AcquireThread() { TerminateAndWait(); }
   private:
    int OnExecute() override;
    gUSBampADC *amp;
  } mAcquireThread;
};

#endif // GUSBAMP_ADC_H




