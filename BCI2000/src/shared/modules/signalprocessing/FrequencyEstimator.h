////////////////////////////////////////////////////////////////////////////////
//  $Id$
//  Author: juergen.mellinger@uni-tuebingen.de
//  Description: A filter that continuously estimates the dominant frequency of
//    its input signal, similar to a frequency counter.
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
#ifndef FREQUENCY_ESTIMATOR_H
#define FREQUENCY_ESTIMATOR_H

#include "GenericFilter.h"
#include <valarray>
#include <vector>
#include <complex>

class FrequencyEstimator : public GenericFilter
{
 public:
  FrequencyEstimator();
  ~FrequencyEstimator();

  void Preflight( const SignalProperties&, SignalProperties& ) const;
  void Initialize( const SignalProperties&, const SignalProperties& );
  void Process( const GenericSignal&, GenericSignal& );

 private:
  typedef double Real;
  typedef std::complex<Real> Complex;
  typedef std::valarray<Complex> ComplexVector;
  typedef std::vector<Real> RealVector;

  struct Data
  {
    ComplexVector TimeshiftOperator;
    std::vector<ComplexVector> Bins;
    RealVector Frequencies;
    GenericSignal Power;
  } mData;
  RealVector mBuffer;
  void Configure( const SignalProperties&, Data& ) const;
  double GetMaxPosition( int ) const;

  bool mVisualizeBins;
  GenericVisualization mBinVisualization;
};

#endif // FREQUENCY_ESTIMATOR_H
