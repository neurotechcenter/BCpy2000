////////////////////////////////////////////////////////////////////////////////
// $Id: CustomFIRFilter.h 3896 2012-03-27 02:28:21Z jhill $
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
#ifndef CUSTOM_FIR_FILTER_H
#define CUSTOM_FIR_FILTER_H

#include "GenericFilter.h"
#include <valarray>
#include <vector>

class CustomFIRFilter : public GenericFilter
{
 public:
          CustomFIRFilter();
  virtual ~CustomFIRFilter();
  virtual void Preflight( const SignalProperties&, SignalProperties& ) const;
  virtual void Initialize( const SignalProperties&, const SignalProperties& );
  virtual void Process( const GenericSignal& Input, GenericSignal& Output );

  typedef std::valarray<double> DataVector;
  DataVector                    mFilter;
  std::vector<DataVector>       mBuffer;
};
#endif // CUSTOM_FIR_FILTER_H


