////////////////////////////////////////////////////////////////////////////////
// $Id: FieldTripBufferADC.h 4536 2013-08-05 14:30:13Z mellinger $
// Author: sk@sklanke.de
// Description: A source class that interfaces to the FieldTrip buffer.
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
#ifndef FIELDTRIP_BUFFER_ADC_H
#define FIELDTRIP_BUFFER_ADC_H

#include <string>

#include "buffer.h"
#include "FtBuffer.h"

#include "GenericADC.h"

class FieldTripBufferADC : public GenericADC
{
 public:
                FieldTripBufferADC();
  virtual       ~FieldTripBufferADC();
  virtual void  Preflight( const SignalProperties&, SignalProperties& ) const;
  virtual void  Initialize( const SignalProperties&, const SignalProperties& );
  virtual void  Process( const GenericSignal&, GenericSignal& );
  virtual void  Halt();

 private:
  typedef std::set<std::string> StateSet;
  bool readHeader(int sock, headerdef_t &hdr) const;
  void ConfigureStateSet( StateSet& stateSet ) const;

  std::string  mHostName;
  int mPort;
  
  unsigned int mNumSamplesRead, 
               mNumEventsRead,
               mNumChannels;
  float mSampleFreq;
  UINT32_T mDataType;
  int mSocket;
  int isRunning;
  SimpleStorage databuf;
  StateSet mEventStates;
};

#endif // FIELDTRIP_BUFFER_ADC_H


