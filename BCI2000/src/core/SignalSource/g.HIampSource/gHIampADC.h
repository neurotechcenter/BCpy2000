////////////////////////////////////////////////////////////////////////////////
// $Id: gHIampADC.h 5240 2016-02-15 16:22:38Z mellinger $
// Authors: griffin.milsap@gmail.com
//			kaleb.goering@gmail.com
// Description: gHIampADC header
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
#ifndef INCLUDED_GHIAMPADC_H
#define INCLUDED_GHIAMPADC_H

#include "gHIampDevice.h"
#include "BufferedADC.h"

class gHIampADC : public BufferedADC
{
 public:
  gHIampADC();
  ~gHIampADC();
  void OnPublish() override;
  void OnAutoConfig() override;
  void OnPreflight( SignalProperties& Output ) const override;
  void OnInitialize( const SignalProperties& Output ) override;
  void OnStartAcquisition() override;
  void OnStopAcquisition() override;
  void DoAcquire( GenericSignal& ) override;

 private:
  struct Private;
  Private* p;
};

#endif // INCLUDED_GHIAMPADC_H
