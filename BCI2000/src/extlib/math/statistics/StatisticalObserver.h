////////////////////////////////////////////////////////////////////////////////
// $Id: StatisticalObserver.h 3798 2012-02-01 18:07:06Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: The main include file for the StatisticalObserver library.
//   The StatisticalObserver library provides objects ("Observers") that observe
//   streams of data, and calculate statistical properties of these data.
//   An important feature of Observers is their ability to "forget" data by
//   assigning less weight to older data than to newer data.
//
//   Typically, Observers are used as follows:
//
//   #include "StatisticalObserver.h"
//   using namespace StatisticalObserver;
//   Observer observer( Correlation | Quantile ); // Specify the functions we are going to use.
//   ...
//   Vector data( 2 ); // For this example, Data must be a
//                     // StatisticalObserver::Vector of length >= 2.
//   ... // Fill data with observations.
//   observer.Observe( data ); // Call Observe() repeatedly, until all data
//                             // has been seen.
//                             // You may use matrix-valued input as well, which
//                             // is then interpreted as a sequence of observations.
//   ...
//   cout << "Correlation 12: "
//        << observer.Correlation()[0][1]
//        << endl;
//   cout << "20% Quantile of 1:
//        << observer.Quantile( 0.2 )[0]
//        << endl;
//   cout << "RSquared between 1 and 2: "
//        << RSquared( observer, observer )[0][1]
//        << endl;
//
//   For details about the Observer interface, and about how to implement your
//   own Observer class, see the ObserverBase.h header file.
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
#ifndef STATISTICAL_OBSERVER_H
#define STATISTICAL_OBSERVER_H

// Make observers available to user code including "StatisticalObserver.h".
#include "PowerSumObserver.h"
#include "HistogramObserver.h"
#include "CombinedObserver.h"

namespace StatisticalObserver
{

// User code typically uses StatisticalObserver::Observer
// unless a specific type of observer is required.
typedef CombinedObserver Observer;

} // namespace StatisticalObserver

#endif // STATISTICAL_OBSERVER_H

