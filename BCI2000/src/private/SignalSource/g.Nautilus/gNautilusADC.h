////////////////////////////////////////////////////////////////////////////////
// $Id: $
// Authors: griffin.milsap@gmail.com
// Description: gNautilus Acquisition using g.NEEDaccess
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

#ifndef INCLUDED_GNAUTILUSADC_H
#define INCLUDED_GNAUTILUSADC_H

#include <GDSClientAPI_gNautilus.h>

#include "BufferedADC.h"
#include "PrecisionTime.h"
#include "OSEvent.h"

class gNautilusADC : public BufferedADC
{
 public:
           gNautilusADC();
  virtual ~gNautilusADC();
  virtual void OnAutoConfig();
  virtual void OnHalt();
  virtual void OnPreflight( SignalProperties& Output ) const;
  virtual void OnInitialize( const SignalProperties& Output );
  virtual void StartRun();
  virtual void OnStartAcquisition();
  virtual void DoAcquire( GenericSignal& Output );
  virtual void OnStopAcquisition();
  virtual void StopRun();

 private:
  void SetupConfig( GDS_GNAUTILUS_CONFIGURATION* config, unsigned int samplingRate,
    unsigned int numChans, unsigned int networkChannel ) const;
  void ApplyConfig( GDS_HANDLE connectionHandle, const char ( &deviceName )[ DEVICE_NAME_LENGTH_MAX ], 
    GDS_GNAUTILUS_CONFIGURATION* config ) const;
  void PrintConfig( GDS_GNAUTILUS_CONFIGURATION* config ) const; // Debug functionality
  static void __stdcall AcquisitionErrorCallback( GDS_HANDLE connectionHandle, GDS_RESULT result, void* );
  static void __stdcall ServerDiedCallback( GDS_HANDLE connectionHandle, void* usrData );
  static void __stdcall DataReadyCallback( GDS_HANDLE connectionHandle, void* usrData );
  bool GDSError( GDS_RESULT result, std::string msg = "GDS ERROR: " ) const;
  std::string DigitalInputStateName( uint32_t i ) const;

  int mSampleBlockSize;
  std::string mHostIP, mLocalIP;
  uint16_t mHostPort, mLocalPort;
  GDS_HANDLE mHandle;
  GDS_GNAUTILUS_CONFIGURATION* mpConfig;
  char mDeviceName[ DEVICE_NAME_LENGTH_MAX ];
  OSEvent mDataReady;
  float *mpDataBuffer;
  size_t mDataBufferSize;
};

#endif // INCLUDED_gNautilusADC_H
