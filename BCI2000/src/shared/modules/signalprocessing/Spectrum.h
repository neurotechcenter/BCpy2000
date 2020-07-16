////////////////////////////////////////////////////////////////////////////////
// $Id: Spectrum.h 4826 2015-04-29 00:40:23Z mellinger $
// Authors: juergen.mellinger@uni-tuebingen.de
// Description: A base class for spectral estimator threads that centralizes
//   common parameters, and Preflight() functionality.
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
#ifndef SPECTRUM_H
#define SPECTRUM_H

#include "GenericFilter.h"

class Spectrum : public GenericFilter
{
 public:
  enum OutputType
  {
    SpectralAmplitude = 0,
    SpectralPower = 1,
    Coefficients = 2,
  };

 protected:
  void Publish() const;
  void Preflight( const SignalProperties&, SignalProperties& ) const;
  int NumberOfBins( const SignalProperties& ) const;
};

#endif // SPECTRUM_H
