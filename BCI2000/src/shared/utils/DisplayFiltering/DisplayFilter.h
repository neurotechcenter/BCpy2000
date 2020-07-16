////////////////////////////////////////////////////////////////////////////////
// $Id: DisplayFilter.h 4171 2012-06-22 12:14:13Z mellinger $
// Author:      juergen.mellinger@uni-tuebingen.de
// Description: An IIR filter class containing a HighPass, LowPass, and Notch
//              filter.
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
#ifndef DISPLAY_FILTER_H
#define DISPLAY_FILTER_H

#include "GenericSignal.h"
#include "IIRFilter.h"

class DisplayFilter
{
 typedef double          Real;

 public:
  DisplayFilter();
  ~DisplayFilter();

  DisplayFilter& HPCorner( Real );
  Real           HPCorner() const;
  DisplayFilter& LPCorner( Real );
  Real           LPCorner() const;
  DisplayFilter& NotchCenter( Real );
  Real           NotchCenter() const;

  void Reset();
  void Process( const GenericSignal&, GenericSignal& );

 private:
  void DesignFilter();

 private:
  Real            mHPCorner,
                  mLPCorner,
                  mNotchCenter;
  IIRFilter<Real> mFilter;
};

#endif // DISPLAY_FILTER_H
