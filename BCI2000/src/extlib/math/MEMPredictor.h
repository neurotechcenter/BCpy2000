////////////////////////////////////////////////////////////////////////////////
// $Id: MEMPredictor.h 4202 2012-06-28 15:44:46Z mellinger $
// Authors: mcfarlan@wadsworth.org, juergen.mellinger@uni-tuebingen.de
// Description: This LinearPredictor implements the Maximum Entropy Method for
//     autoregressive spectral analysis adapted from Press et. al.
//     Numerical Recipes in C (chapter 13).
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
#ifndef MEM_PREDICTOR_H
#define MEM_PREDICTOR_H

#include "LinearPredictor.h"
#include <numeric>
#include <limits>

template<typename T>
class MEMPredictor : public LinearPredictor<T>
{
 public:
  typedef std::valarray<T> DataVector;

 public:
  MEMPredictor();
  virtual ~MEMPredictor() {}

  virtual void TransferFunction( const DataVector&, Ratpoly<T>& ) const;

 private:
  mutable DataVector mWk1,
                     mWk2;
};


// Implementation
template<typename T>
MEMPredictor<T>::MEMPredictor()
{
}

template<typename T>
void
MEMPredictor<T>::TransferFunction( const DataVector& inData, Ratpoly<T>& outResult ) const
{
  typedef double D;
  static const T eps = std::numeric_limits<T>::epsilon();
  size_t n = inData.size();

  DataVector coeff, wkm;
  coeff.resize( LinearPredictor<T>::mModelOrder + 1 );
  wkm.resize( coeff.size() );

  if( mWk1.size() != n )
    mWk1.resize( n );
  mWk1 = inData;
  if( mWk2.size() != n )
    mWk2.resize( n );
  mWk2 = inData;

  D meanPower = 0;
  for (size_t t = 0; t < n; t++)
    meanPower += (mWk1[t]*mWk1[t]);

  D den = meanPower*2;
  meanPower /= n;
  D num=0.0;
  D q = 1.0;
  coeff[0] = 1.0;
  for( size_t k = 1; k <= LinearPredictor<T>::mModelOrder; ++k )
  {
    num = 0;
    for (size_t t = 0; t < n-k; t++)
        num += mWk1[t+1]*mWk2[t];

    den = den*q - mWk1[0]*mWk1[0] - mWk2[n-k]*mWk2[n-k];

    if (den < eps){
      num = 0.5;
      den = 1.0;
    }
    else{
      if (coeff[k] >= 1 || coeff[k] <= -1){
        den = 0;
        for (size_t t = 0; t < n-k; t++)
          den += mWk1[t+1]*mWk1[t+1] + mWk2[t]*mWk2[t];
      }
    }
    coeff[k] = 2*num / den;

    q = 1.0 - coeff[k] * coeff[k];
    meanPower *= q;
    for( size_t i = 1; i < k; ++i )
      coeff[i] = wkm[i] - coeff[k] * wkm[k-i];

    if( k < LinearPredictor<T>::mModelOrder )
    {
      for( size_t i = 1; i <= k; ++i )
        wkm[i] = coeff[i];

      for( size_t j = 0; j < n-k; ++j )
      {
        mWk1[j] = mWk1[j+1] - wkm[k] * mWk2[j];
        mWk2[j] = mWk2[j] - wkm[k] * mWk1[j+1];
      }
    }
  }
  if( meanPower < 0.0 )
    meanPower = 0.0;

  for (size_t k = 1; k <= LinearPredictor<T>::mModelOrder; k++)
      coeff[k] *= -1;

  outResult = Ratpoly<T>(
               Polynomial<T>( std::sqrt( meanPower ) ),
               Polynomial<T>::FromCoefficients( coeff )
              );
}

#endif // MEM_PREDICTOR_H

