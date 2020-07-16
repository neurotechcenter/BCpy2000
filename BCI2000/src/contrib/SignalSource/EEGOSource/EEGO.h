////////////////////////////////////////////////////////////////////////////////
// $Id: $
// Authors:
// Description: EEGO header
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

#ifndef INCLUDED_EEGO_H  // makes sure this header is not included more than once
#define INCLUDED_EEGO_H

#include "BufferedADC.h"

#include <eemagine\sdk\stream.h>
#include <eemagine\sdk\amplifier.h>

class EEGO : public BufferedADC
{
 public:
           EEGO();
  virtual ~EEGO();

  virtual void AutoConfig(const SignalProperties&);
  virtual void OnHalt();
  virtual void OnPreflight( SignalProperties& Output ) const;
  virtual void OnInitialize( const SignalProperties& Output );
  virtual void StartRun();
  virtual void OnStartAcquisition();
  virtual void DoAcquire( GenericSignal& Output );
  virtual void OnStopAcquisition();
  virtual void StopRun();

 private:
	 struct Ranges
	 {
		 double reference;
		 double bipolar;
	 };

   // Filter the given list of channels for only the specific type and return their indices.
   std::vector<unsigned> GetChannelIndicesOfType(const std::vector<eemagine::sdk::channel>& list, const eemagine::sdk::channel::channel_type type) const;
   // Get the referential and bipolar ranges supported by this amplifier.
   Ranges GetRanges(const eemagine::sdk::amplifier& amp) const;
   // Return the lowest range in the list of ranges which includes the range specified in the setting stored under paramName. 
   double FindSignalRange(const std::vector<double> ranges, const std::string paramName) const;
   // SDK helpee
   eemagine::sdk::factory GetFactory() const;

 private:
  // The currently open stream
  eemagine::sdk::stream* mpCurrentOpenStream;
  // The current amplifier in use
  eemagine::sdk::amplifier* mpCurrentAmplifier;

  // Stream
  eemagine::sdk::buffer mCurrentBuffer;
  size_t mCurrentBufferPosition;

  // Trigger 
  unsigned mTriggerIndex;
  unsigned mLastTriggerLevel;
  unsigned mCurrentTriggerState;
};

#endif // INCLUDED_EEGO_H
