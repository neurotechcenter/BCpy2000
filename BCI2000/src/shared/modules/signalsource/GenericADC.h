////////////////////////////////////////////////////////////////////////////////
// $Id: GenericADC.h 4640 2013-11-18 12:18:30Z mellinger $
// Description: GenericADC defines the virtual function interface for signal
//   source filters, and thus is the base class for all ADC filter classes.
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
#ifndef GENERIC_ADC_H
#define GENERIC_ADC_H

#include "GenericFilter.h"

class GenericADC : public GenericFilter
{
 protected:
  GenericADC() {}

 public:
  virtual ~GenericADC() {}
  // GenericFilter inherited functions, forcing implementation of Halt().
  virtual void AutoConfig( const SignalProperties& ) {}
  virtual void Preflight( const SignalProperties&,
                                SignalProperties& ) const = 0;
  virtual void Initialize( const SignalProperties&,
                           const SignalProperties& ) = 0;
  virtual void StartRun() {}
  virtual void StopRun() {}
  virtual void Process(   const GenericSignal&,
                                GenericSignal& ) = 0;
  virtual void Halt() = 0;

  virtual bool AllowsVisualization() const { return false; }
  virtual bool TimedCalls() const { return false; }
  // A function to specify whether data is generated in real time.
  // Should return false when data is replayed from a file, or otherwise
  // released faster/slower than prescribed by the SamplingRate parameter.
  virtual bool IsRealTimeSource() const { return true; }
  // A function to specify whether the SourceTime time stamp is set
  // by the filter.
  virtual bool SetsSourceTime() const { return false; }
};

#endif // GENERIC_ADC_H

