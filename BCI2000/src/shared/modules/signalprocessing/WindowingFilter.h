////////////////////////////////////////////////////////////////////////////////
// $Id: WindowingFilter.h 5265 2016-03-02 20:23:27Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: The WindowingFilter provides
//  * Buffering of the signal into time windows that may be larger than
//    SampleBlockSize,
//  * Detrending options (mean or linear),
//  * Windowing options typically used with FFT (Hann, Hamming, Blackman).
//  Typically, the WindowingFilter provides its output to a spectral
//  estimator (AR, FFT).
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
#ifndef WINDOWING_FILTER_H
#define WINDOWING_FILTER_H

#include "GenericFilter.h"
#include <vector>
#include <valarray>

class WindowingFilter : public GenericFilter
{
 public:
  enum Detrend { None = 0, Mean, Linear };
  enum Window { Rectangular = 0, Hamming, Hann, Blackman };

 public:
  WindowingFilter();

 protected:
  void Publish();
  void Preflight( const SignalProperties&, SignalProperties& ) const;
  void Initialize( const SignalProperties&, const SignalProperties& );
  void Process( const GenericSignal&, GenericSignal& );
  void StartRun();

 private:
  typedef GenericSignal::ValueType Real;
  typedef std::valarray<Real> DataVector;

  std::vector<DataVector> mBuffers, mDetrendBuffers;
  DataVector mWindow;
  int mDetrend, mWindowFunction;
};

#endif // WINDOWING_FILTER_H
