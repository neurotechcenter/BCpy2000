////////////////////////////////////////////////////////////////////////////////
// $Id: $
// Authors: 
// Description: eegoRTADC header
//   
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

#ifndef INCLUDED_eegoRTADC_H  // makes sure this header is not included more than once
#define INCLUDED_eegoRTADC_H

#include "BufferedADC.h"

#define EEGO_SDK_BIND_DYNAMIC // How to bind
#include <eemagine/sdk/factory.h> // SDK header
#include <eemagine/sdk/wrapper.cc> // Wrapper code to be compiled.

using namespace eemagine::sdk;

class eegoRTADC : public BufferedADC
{
 public:
           eegoRTADC();
  virtual ~eegoRTADC();
  virtual void OnPublish();
  virtual void OnAutoConfig();
  virtual void OnPreflight( SignalProperties& Output ) const;
  virtual void OnInitialize( const SignalProperties& Output );
  virtual void OnStartAcquisition();
  virtual void DoAcquire( GenericSignal& Output );
  virtual void OnStopAcquisition();

  void OnTrigger( int ); // for asynchronous triggers only

 private:
  // Use this space to declare any eegoRTADC-specific methods and member variables you'll need
  void* mDeviceHandle;
  char* mpBuffer;
  int mNumberOfSignalChannels;

  amplifier* amp;

};

#endif // INCLUDED_eegoRTADC_H
