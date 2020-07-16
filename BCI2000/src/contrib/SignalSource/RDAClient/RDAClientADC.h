////////////////////////////////////////////////////////////////////////////////
// $Id: RDAClientADC.h 4731 2014-07-03 15:18:03Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A source class that interfaces to the BrainAmp RDA socket
//              interface.
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
#ifndef RDA_CLIENT_ADC_H
#define RDA_CLIENT_ADC_H

#include <string>
#include "BufferedADC.h"
#include "RDAProtocol.h"

class RDAClientADC : public BufferedADC
{
 public:
  RDAClientADC();
  ~RDAClientADC();
  void OnAutoConfig();
  void OnPreflight( SignalProperties& ) const;
  void OnInitialize( const SignalProperties& );
  void OnStartAcquisition();
  void OnStopAcquisition();
  void DoAcquire( GenericSignal& );

 private:
  std::string      mHostName;
  bool             mAddMarkerChannel;
  RDA::Connection  mConnection;
};

#endif // RDA_CLIENT_ADC_H


