////////////////////////////////////////////////////////////////////////////////
// $Id: FIRFilter.h 3798 2012-02-01 18:07:06Z mellinger $
// Description: A finite impulse response (FIR) filter for temporal filtering.
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
#ifndef FIR_FILTER_H
#define FIR_FILTER_H

#include "GenericFilter.h"
#include <valarray>
#include <vector>

class FIRFilter : public GenericFilter
{
 public:
          FIRFilter();
  virtual ~FIRFilter();
  virtual void Preflight( const SignalProperties&, SignalProperties& ) const;
  virtual void Initialize( const SignalProperties&, const SignalProperties& );
  virtual void Process( const GenericSignal& Input, GenericSignal& Output );

 private:
  enum
  {
    none = 0,
    mean,
    rms,
    max,
  };
  int mFIRIntegration;

  typedef std::valarray<double> DataVector;
  std::vector<DataVector>       mFilter,
                                mBuffer;
};
#endif // FIR_FILTER_H


