////////////////////////////////////////////////////////////////////////////////
// $Id: $
// Authors: griffin.milsap@gmail.com
// Description: gNEEDaccessSource header
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
#ifndef INCLUDED_GNEEDACCESSSOURCE_H
#define INCLUDED_GNEEDACCESSSOURCE_H

#include "BufferedADC.h"
#include "PrecisionTime.h"
#include "GDSClient.h"
#include "GDSXML.h"

#include <queue>

class gNEEDaccessSource : public BufferedADC, public GDSEventObserver
{
 public:
           gNEEDaccessSource();
  virtual ~gNEEDaccessSource();

  // Interface from BufferedADC
  virtual void OnHalt();
  virtual void OnAutoConfig();
  virtual void OnPreflight( SignalProperties& Output ) const;
  virtual void OnInitialize( const SignalProperties& Output );
  virtual void OnStartAcquisition();
  virtual void DoAcquire( GenericSignal& Output );
  virtual void OnStopAcquisition();

  // Interface from GDSEventObserver
  virtual void OnDataReady();

 protected:
  // Overload this to set default values for autoconfigurable parameters
  virtual void DoAutoConfig( GDSClient &gds,
                             const std::vector< std::string > &inDeviceList,
                             const GDSDeviceMap &inDevices ) { }

  // Populate the outConfigList structure with the requested 
  // configuration and perform preflight checking
  virtual void GetConfiguration( GDSClient &gds,
                                 const std::vector< std::string > &inDeviceList,
                                 const GDSDeviceMap &inDevices, 
                                 GDSConfigList &outConfigList ) const = 0;

  // Save parameters here necessary for interpreting acquisition output
  virtual void DoInitialize( GDSClient &gds,
                             const std::vector< std::string > &inDeviceList,
                             const GDSDeviceMap &inDevices, 
                             const std::vector< size_t > &channelsPerDevice,
                             size_t scanSize ) { }

  // Given an input data buffer, populate the signal Output
  virtual void PopulateBlock( GenericSignal& Output, 
                              std::queue< float > &Data, 
                              const std::vector< size_t > &channelsPerDevice,
                              size_t scanSize ) = 0;

  // Perform error checking on GDSError output
  virtual bool GDSErrorCheck( GDSError error ) const;

 private:
  GDSClient mGDS;

  size_t mScanSize;
  int mSampleBlockSize;

  std::string mGDSAddress, mLocalAddress;

  std::queue< float > mDataQueue;
  std::vector< size_t > mChannelsPerDevice;
};

#endif // INCLUDED_GNEEDACCESSSOURCE_H
