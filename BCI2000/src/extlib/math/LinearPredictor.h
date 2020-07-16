////////////////////////////////////////////////////////////////////////////////
// $Id: LinearPredictor.h 4202 2012-06-28 15:44:46Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: An abstract base class for linear predictors.
//   The predictor's TransferFunction() member returns a rational transfer
//   function with the estimated coefficients in the denominator, and the
//   explained signal variance in the numerator.
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
#ifndef LINEAR_PREDICTOR_H
#define LINEAR_PREDICTOR_H

#include <valarray>
#include <complex>
#include "Polynomials.h"

template<typename T>
class LinearPredictor
{
 public:
  LinearPredictor()
    : mModelOrder( 1 )
    {}
  virtual ~LinearPredictor() {}

  LinearPredictor& SetModelOrder( int inOrder )
    { mModelOrder = inOrder; return *this; }
  int ModelOrder() const
    { return static_cast<int>( mModelOrder ); }

  virtual void TransferFunction( const std::valarray<T>&, Ratpoly<T>& ) const = 0;

 protected:
  size_t mModelOrder;
};

#endif // LINEAR_PREDICTOR_H

