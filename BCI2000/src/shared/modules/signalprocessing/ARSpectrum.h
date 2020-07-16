////////////////////////////////////////////////////////////////////////////////
// $Id: ARSpectrum.h 4767 2014-11-26 18:35:04Z mellinger $
// Authors: mcfarlan@wadsworth.org, juergen.mellinger@uni-tuebingen.de,
//          Adam Wilson
// Description: The ARSpectrum filter fits a Maximum Entropy AR model to
//   its input data. Its output can be configured to be
//   - raw AR coefficients,
//   - the model's amplitude spectrum,
//   - the model's intensity spectrum.
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
#ifndef AR_SPECTRUM_H
#define AR_SPECTRUM_H

#include "Spectrum.h"
#include "MEMPredictor.h"
#include "TransferSpectrum.h"

class ARSpectrum : public Spectrum
{
  typedef Spectrum Super;

 protected:
  void Publish();
  void Preflight( const SignalProperties&, SignalProperties& ) const;
  void Initialize( const SignalProperties&, const SignalProperties& );
  void Process( const GenericSignal&, GenericSignal& );

 private:
  enum OutputType
  {
    SpectralAmplitude = 0,
    SpectralPower = 1,
    ARCoefficients = 2,
  };

  typedef double Real;
  typedef std::valarray<Real> DataVector;

  std::vector< MEMPredictor<Real> > mPredictors;
  std::vector< TransferSpectrum<Real> > mTransferSpectra;
  std::vector< DataVector > mInputs, mSpectra;
  int mOutputType;
};

#endif // AR_SPECTRUM_H
