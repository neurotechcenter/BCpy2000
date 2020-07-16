////////////////////////////////////////////////////////////////////////////////
// $Id: vAmpADC.h 5403 2016-06-17 09:54:09Z ratko $
// Author: jadamwilson2@gmail.com
// Description: BCI2000 Source Module for BrainProducts V-Amp devices.
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
//
// [2016.05.12] Class inherits interface from class BufferedADC, not GenericADC any more. [Brain Products - RP]
//

#ifndef VAMP_ADC_H
#define VAMP_ADC_H

#include "BufferedADC.h"
#include "GenericVisualization.h"
#include <windows.h>
#include <vector>
#include <string>
#include <algorithm>
#include "PrecisionTime.h"
#include "FirstAmp.imports.h"
#include "vAmpChannelInfo.h"
#include "vAmpThread.h"


class vAmpADC : public BufferedADC
{
 public:
         vAmpADC();
  virtual      ~vAmpADC();  

  virtual void OnPreflight( SignalProperties& Output ) const;
  virtual void OnInitialize( const SignalProperties& Output );
  virtual void OnStartAcquisition();
  virtual void DoAcquire( GenericSignal& Output );
  virtual void OnStopAcquisition();
  virtual void OnHalt(); 

  void OnTrigger( int ); // for asynchronous triggers only

 private:
	 // new privates: Use this space to declare any vAmpADC-specific methods and member variables you'll need
  void* mDeviceHandle;
  char* mpBuffer;
  int mNumberOfSignalChannels;


  int mNumEEGchannels;
  std::vector<int> mChList;
  int mTimeoutMs;
  std::vector<int> mDevList;
  bool mHighSpeed;
  std::vector< std::vector<float> > mImpedances;
  bool mImpedanceMode;

  float getSampleRate(float);
  int getDecimation() const;
  bool IsVAmpServiceStopped() const; // check the state of VAmpService

  friend class vAmpThread;
  vAmpThread *mAcquire;
};

#endif // VAMP_ADC_H




