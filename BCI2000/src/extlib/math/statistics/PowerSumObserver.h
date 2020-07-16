////////////////////////////////////////////////////////////////////////////////
// $Id: PowerSumObserver.h 3798 2012-02-01 18:07:06Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A statistical observer that stores distribution information
//   in form of aging power sums. For information how to use this class, see
//   the Observer class declared in Observer.h.
//   For a justification of the aging power sums representation,
//   consider power sums of an aging histogram, i.e. an histogram with entries
//   decaying exponentially in time. Power sums computed from such a histogram
//   will be identical to power sums aging themselves.
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
#ifndef POWER_SUM_OBSERVER_H
#define POWER_SUM_OBSERVER_H

#include "ObserverBase.h"

namespace StatisticalObserver
{

class PowerSumObserver : public virtual ObserverBase
{
  // This class implements the ObserverBase interface. For documentation, see ObserverBase.h.
 public:
  enum
  {
    Supports =
      StatisticalObserver::Variance |
      StatisticalObserver::Covariance
  };
  PowerSumObserver( int config = Supports );

 protected:
  virtual void DoChange();
  virtual void DoAgeBy( unsigned int count );
  virtual void DoObserve( const Vector&, Number weight );
  virtual void DoClear();

 public:
  virtual Number PowerSum0( MemPool& ) const
    { return mPowerSum0; }
  virtual VectorPtr PowerSum1( MemPool& ) const;
  virtual VectorPtr PowerSum2Diag( MemPool& ) const;
  virtual MatrixPtr PowerSum2Full( MemPool& ) const;

 private:
  bool mCovarianceRequired;
  Number mPowerSum0;
  Vector mPowerSum1,
         mPowerSum2Diag;
  Matrix mPowerSum2Full;
};

} // namespace StatisticalObserver

#endif // POWER_SUM_OBSERVER
