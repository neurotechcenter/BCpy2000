////////////////////////////////////////////////////////////////////////////////
// $Id: Histogram.h 3798 2012-02-01 18:07:06Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A histogram that stores data points associated with weights
//   in a std::map<Value, Weight> (which requires logarithmic time for addition/
//   removal of values).
//   Using the Prune() function after addition of new data, the Histogram class
//   may be used to implement an adaptive histogram using an asymptotically
//   constant amount of memory.
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
#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include "ObserverBase.h"
#include <list>

namespace StatisticalObserver
{

// For implementing the histogram, we choose a std::list<>
// because it is more efficient to traverse than a std::map<>, and a map's
// overhead is likely to weigh out the list's linear insertion time for
// practical applications (N<1000).
class Histogram : private std::list< std::pair<Number, Number> >
{
 public:
  // Multiply weights.
  Histogram& operator*=( Number );
  // Add an observation with a certain weight.
  Histogram& Add( Number, Number );
  // Collapse neighboring data points when their combined weight is below a threshold,
  // or when their distance is below a threshold.
  Histogram& Prune( Number weightThreshold, Number distThreshold );
  // Clear contents.
  Histogram& Clear();
  // Compute a power sum over weighted data points.
  Number PowerSum( unsigned int ) const;
  // Determine the sum of weights up to a given value.
  Number CDF( Number ) const;
  // Determine the value corresponding to the given cumulated weight.
  // NB: There are a number of methods available to determine quantiles of empirical distributions.
  // With the exception of the inverse CDF method, these do not generalize easily to the case
  // of weighted data points.
  Number InverseCDF( Number ) const;
};

} // namespace StatisticalObserver

#endif // HISTOGRAM_H
