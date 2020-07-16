////////////////////////////////////////////////////////////////////////////////
// $Id: ScaleObservationFilter.h 4171 2012-06-22 12:14:13Z mellinger $
// Author:      juergen.mellinger@uni-tuebingen.de
// Description: A filter that observes the scale of the data it sees.
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
#ifndef SCALE_OBSERVATION_FILTER_H
#define SCALE_OBSERVATION_FILTER_H

#include "GenericSignal.h"

class ScaleObservationFilter
{
 public:
  ScaleObservationFilter();
  ~ScaleObservationFilter() {}

  ScaleObservationFilter& TimeConstant( double inTimeConstant );
  double TimeConstant() const { return mTimeConstant; }

  double Count() const { return mCount; }
  double Mean() const { return mSum / mCount; }
  double Max() const { return mMax; }
  double Min() const { return mMin; }

  void Reset();
  void Process( const GenericSignal& );

 private:
  double mTimeConstant, mDecayFactor,
         mCount, mSum, mMax, mMin;
};

#endif // SCALE_OBSERVATION_FILTER_H
