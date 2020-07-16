////////////////////////////////////////////////////////////////////////////////
// $Id: WindowObserver.h 3798 2012-02-01 18:07:06Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A statistical observer that maintains a sliding window of past
//   samples. Samples enter into statistics with equal weights.
//   This observer is quite inefficient in terms of computation, and is intended
//   as a reference when testing other observer classes.
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
#ifndef WINDOW_OBSERVER_H
#define WINDOW_OBSERVER_H

#include "ObserverBase.h"
#include <deque>

namespace StatisticalObserver
{

typedef std::deque< std::pair< Vector, std::pair<Number, size_t> > > WindowObserverDataContainer;

class WindowObserver : public virtual ObserverBase, private WindowObserverDataContainer
{
 public:
   enum
   {
     Supports = 
       StatisticalObserver::Variance |
       StatisticalObserver::Covariance |
       StatisticalObserver::Quantile |
       StatisticalObserver::CentralMoment
   };
  WindowObserver( int config = Supports );

  // The Observer interface: For documentation, see Observer.h
 protected:
  virtual void DoChange();
  virtual void DoAgeBy( unsigned int count );
  virtual void DoObserve( const Vector&, Number weight );
  virtual void DoClear();

 public:
  virtual Number PowerSum0( MemPool& ) const;
  virtual VectorPtr PowerSum1( MemPool& ) const;
  virtual VectorPtr PowerSum2Diag( MemPool& ) const;
  virtual MatrixPtr PowerSum2Full( MemPool& ) const;
  virtual VectorPtr PowerSumDiag( unsigned int, MemPool& ) const;
  virtual VectorPtr CDF( Number, MemPool& ) const;
  virtual VectorPtr InverseCDF( Number, MemPool& ) const;

 private:
  void RemoveOldSamples();

  size_t mAgeLimit;
};

} // namespace StatisticalObserver

#endif // WINDOW_OBSERVER_H
