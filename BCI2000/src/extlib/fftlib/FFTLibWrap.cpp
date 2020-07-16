////////////////////////////////////////////////////////////////////////////////
// $Id: FFTLibWrap.cpp 5285 2016-03-16 16:15:07Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: An FFT library wrapper.
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
#include "FFTLibWrap.h"

#define __declspec(x)
#include "fftw3.h"

namespace Dylib { bool fftw3_Loaded(); }

int FFTLibWrapper::sNumInstances = 0;

bool
FFTLibWrapper::LibAvailable()
{
  return Dylib::fftw3_Loaded();
}

const char*
FFTLibWrapper::LibName()
{
  return "fftw3";
}

FFTLibWrapper::FFTLibWrapper()
: mFFTSize( 0 ),
  mpInputData( NULL ),
  mpOutputData( NULL ),
  mLibPrivateData( NULL )
{
  ++sNumInstances;
}

FFTLibWrapper::~FFTLibWrapper()
{
  --sNumInstances;
  if( Dylib::fftw3_Loaded() )
  {
    Cleanup();
    if( sNumInstances < 1 )
      ::fftw_cleanup();
  }
}

void
FFTLibWrapper::Compute()
{
  ::fftw_execute( static_cast<fftw_plan>( mLibPrivateData ) );
}

void
FFTLibWrapper::Cleanup()
{
  if( mLibPrivateData )
  {
    ::fftw_destroy_plan( static_cast<fftw_plan>( mLibPrivateData ) );
    mLibPrivateData = NULL;
  }
  if( mpInputData )
  {
    ::fftw_free( mpInputData );
    mpInputData = NULL;
  }
  if( mpOutputData )
  {
    ::fftw_free( mpOutputData );
    mpOutputData = NULL;
  }
  mFFTSize = 0;
}

bool
RealFFT::Initialize( int inFFTSize, FFTDirection inDirection, FFTOptimization inOptimization )
{
  Cleanup();
  mFFTSize = inFFTSize;
  mpInputData = ::fftw_malloc( mFFTSize * sizeof( Real ) );
  mpOutputData = ::fftw_malloc( mFFTSize * sizeof( Real ) );
  fftw_r2r_kind kind = inDirection > 0 ? FFTW_R2HC : FFTW_HC2R;
  mLibPrivateData = ::fftw_plan_r2r_1d( mFFTSize, static_cast<Real*>( mpInputData ), static_cast<Real*>( mpOutputData ), kind, inOptimization );
  return mpInputData && mpOutputData && mLibPrivateData;
}

bool
ComplexFFT::Initialize( int inFFTSize, FFTDirection inDirection, FFTOptimization inOptimization )
{
  Cleanup();
  mFFTSize = inFFTSize;
  mpInputData = ::fftw_malloc( mFFTSize * sizeof( Complex ) );
  mpOutputData = ::fftw_malloc( mFFTSize * sizeof( Complex ) );
  mLibPrivateData = ::fftw_plan_dft_1d( mFFTSize, static_cast<fftw_complex*>( mpInputData) , static_cast<fftw_complex*>( mpOutputData ), inDirection, inOptimization );
  return mpInputData && mpOutputData && mLibPrivateData;
}
