////////////////////////////////////////////////////////////////////////////////
// $Id: SpectralEstimator.h 3833 2012-02-29 16:13:10Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: The SpectralEstimator filter performs windowing, followed by
//   AR or FFT-based spectral estimation.
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
#ifndef SPECTRAL_ESTIMATOR_H
#define SPECTRAL_ESTIMATOR_H

#include "FilterCombination.h"
#include "ChoiceCombination.h"
#include "IdentityFilter.h"
#include "WindowingFilter.h"
#include "ARSpectrum.h"
#include "FFTSpectrum.h"

struct SpectralEstimatorChoice : ChoiceCombination
{
  SpectralEstimatorChoice()
    : ChoiceCombination( "SpectralEstimator" )
    {
      BEGIN_PARAMETER_DEFINITIONS
        "Filtering:Spectral%20Estimation int SpectralEstimator= 1 1 0 2 "
        "// Choice of spectral estimation algorithm, 0: None, 1: AR, 2: FFT (enumeration)",
      END_PARAMETER_DEFINITIONS
      // Filters should be instantiated in reverse order for parameters to appear in
      // the order of choices.
      AddChoice<FFTSpectrum>( 2 );
      AddChoice<ARSpectrum>( 1 );
      AddChoice<IdentityFilter>( 0 );
    }
};

struct SpectralEstimator
: LinearCombination<WindowingFilter, SpectralEstimatorChoice>
{
};

#endif // SPECTRAL_ESTIMATOR_H
