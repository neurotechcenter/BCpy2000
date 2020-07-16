////////////////////////////////////////////////////////////////////////////////
// $Id: $
// Authors: griffin.milsap@gmail.com
// Description: gNautilusADC header
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

#include "BufferedADC.h"
#include "PrecisionTime.h"

#include "gNEEDaccessSource.h"
#include "GDSNautilusConfig.h"
#include "GDSClient.h"

#include <queue>

class gNautilusADC : public gNEEDaccessSource
{
 public:
           gNautilusADC();
  virtual ~gNautilusADC();

 protected:
  virtual void DoAutoConfig( GDSClient &gds,
                             const std::vector< std::string > &inDeviceList,
                             const GDSDeviceMap &inDevices );

  virtual void GetConfiguration( GDSClient &gds,
                                 const std::vector< std::string > &inDeviceList,
                                 const GDSDeviceMap &inDevices, 
                                 GDSConfigList &outConfigList ) const;

  virtual void DoInitialize( GDSClient &gds,
                             const std::vector< std::string > &inDeviceList,
                             const GDSDeviceMap &inDevices, 
                             const std::vector< size_t > &channelsPerDevice,
                             size_t scanSize );

  virtual void PopulateBlock( GenericSignal& Output, 
                              std::queue< float > &Data, 
                              const std::vector< size_t > &channelsPerDevice,
                              size_t scanSize );

 private:
  mutable GDSNautilusConfig mConfiguration;
  int mSampleBlockSize;
};

#endif // INCLUDED_GNAUTILUSADC_H