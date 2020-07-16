////////////////////////////////////////////////////////////////////////////////
// $Id: LinearClassifier.h 3798 2012-02-01 18:07:06Z mellinger $
// Authors: mcfarlan@wadsworth.org, juergen.mellinger@uni-tuebingen.de
// Description: The LinearClassifier applies a matrix multiplication to its
//   input data.
//   Input data has 2 indices (N channels x M elements), and output data
//   has a single index (C channels x 1 element), thus the linear classifier
//   acts as a N x M x C matrix, determining the output after summation over
//   N and M.
//
//   The Classifier parameter is a sparse matrix definition in which each row
//   corresponds to a single matrix entry.
//   Columns correspond to
//   1) input channel,
//   2) input element (bin in the spectral case, time offset in the ERP case),
//   3) output channel,
//   4) weight (value of the matrix entry).
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
#ifndef LINEAR_CLASSIFIER_H
#define LINEAR_CLASSIFIER_H

#include "GenericFilter.h"

class LinearClassifier : public GenericFilter
{
 public:
          LinearClassifier();
  virtual ~LinearClassifier();
  virtual void Preflight( const SignalProperties&, SignalProperties& ) const;
  virtual void Initialize( const SignalProperties&, const SignalProperties& );
  virtual void Process( const GenericSignal&, GenericSignal& );

 private:
  std::string DescribeEntry( int row, int col ) const;
  static int Round( double );
  
  std::vector<size_t>  mInputChannels,
                       mInputElements,
                       mOutputChannels;
  std::vector<double>  mWeights;
};

#endif // LINEAR_CLASSIFIER_H


