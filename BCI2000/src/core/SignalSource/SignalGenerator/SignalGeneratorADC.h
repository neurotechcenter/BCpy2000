////////////////////////////////////////////////////////////////////////////////
// $Id: SignalGeneratorADC.h 4630 2013-10-30 20:49:31Z mellinger $
// Author: schalk@wadsworth.org, juergen.mellinger@uni-tuebingen.de
// Description: An ADC class for testing purposes.
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
#ifndef SIGNAL_GENERATOR_ADC_H
#define SIGNAL_GENERATOR_ADC_H

#include "GenericADC.h"
#include "Clock.h"
#include "Expression.h"
#include "RandomGenerator.h"
#include <vector>
#include <valarray>

class SignalGeneratorADC : public GenericADC
{
 public:
  SignalGeneratorADC();
  ~SignalGeneratorADC();

  void Publish();
  void AutoConfig( const SignalProperties& );
  void Preflight( const SignalProperties&, SignalProperties& ) const;
  void Initialize( const SignalProperties&, const SignalProperties& );
  void StartRun();
  void Process( const GenericSignal&, GenericSignal& );
  void Halt();

  bool IsRealTimeSource() const { return false; } // permits --EvaluateTiming=0, to launch without realtime checking

 private:
  // Configuration
  double mNoiseAmplitude,
         mDCOffset;
  Expression mOffsetMultiplier,
             mAmplitudeMultiplier;
  typedef std::valarray<double> Vector;
  typedef std::vector<Vector> Matrix;
  Vector mSourceFrequencies,
         mSourceAmplitudes,
         mSourcePhases,
         mSourceValues;
  Matrix mMixingMatrix;

  int    mSineChannelX,
         mSineChannelY,
         mSineChannelZ;
  bool   mModulateAmplitude;
  // Internal State
  double mAmplitudeX,
         mAmplitudeY,
         mAmplitudeZ;
  Clock mClock;
  RandomGenerator mRandomGenerator;
};

#endif // SIGNAL_GENERATOR_ADC_H

