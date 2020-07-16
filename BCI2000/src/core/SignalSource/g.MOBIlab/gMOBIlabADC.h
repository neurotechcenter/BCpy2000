////////////////////////////////////////////////////////////////////////////////
// $Id: gMOBIlabADC.h 5301 2016-03-23 21:04:46Z mellinger $
// Author: schalk@wadsworth.org
// Description: BCI2000 Source Module for gMOBIlab devices.
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
#ifndef GMOBILAB_ADC_H
#define GMOBILAB_ADC_H

#include <string>
#include "BufferedADC.h"
#include "Expression.h"
#include "defines.h"
#include <Windows.h>

class gMOBIlabADC : public BufferedADC
{
 public:
  gMOBIlabADC();
  ~gMOBIlabADC();

 private:
  // Data acquisition interface defined by BufferedADC
  void OnPublish() override;
  void OnAutoConfig() override;
  void OnPreflight( SignalProperties& ) const override;
  void OnInitialize( const SignalProperties& ) override;
  void OnProcess() override;
  void OnStartAcquisition() override;
  void OnStopAcquisition() override;
  void DoAcquire( GenericSignal& ) override;
  void OnHalt() override;

 private:
  std::vector<uint8_t> mBuffer;
  int mTimeoutMs;
  bool mEnableDigOut, mEnableExpression;
  Expression mDigExpression;

  std::string mComPort;
  HANDLE mDevice;
  OVERLAPPED mOverlapped;
};

#endif // GMOBILAB_ADC_H

