////////////////////////////////////////////////////////////////////////////////
// $Id: CoherenceFilter.h 3798 2012-02-01 18:07:06Z mellinger $
// Authors: juergen.mellinger@uni-tuebingen.de
// Description: A filter that computes magnitude squared coherence between its
//   input signals at a set of frequencies.
//   Output channels correspond to pairs of input channels, output elements
//   correspond to frequencies at which coherence is evaluated.
//   
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

#ifndef COHERENCE_FILTER_H
#define COHERENCE_FILTER_H

#include <valarray>
#include <vector>
#include <complex>

#include "GenericFilter.h"

class CoherenceFilter : public GenericFilter
{
  typedef double             real;
  typedef std::complex<real> complex;

 public:
           CoherenceFilter();
  virtual ~CoherenceFilter();
  virtual void Preflight(  const SignalProperties& Input,       SignalProperties& Output ) const;
  virtual void Initialize( const SignalProperties& Input, const SignalProperties& Output );
  virtual void StartRun();
  virtual void Process(    const GenericSignal&    Input,       GenericSignal&    Output );

 private:
   static complex InnerProduct( const std::valarray<complex>&, const std::valarray<complex>& );

   std::vector< std::valarray<real> >                   mInputBuffer;     // channels x samples
   std::vector< std::valarray<complex> >                mFIRCoefficients; // frequency bins x samples
   std::vector< std::vector< std::valarray<complex> > > mFIRConvolution;  // channels x bins x convolution samples
   int mConvolutionStep;

};

#endif // COHERENCE_FILTER_H
