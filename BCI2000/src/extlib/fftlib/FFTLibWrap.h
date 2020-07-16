////////////////////////////////////////////////////////////////////////////////
// $Id: FFTLibWrap.h 5285 2016-03-16 16:15:07Z mellinger $
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
#ifndef FFT_LIB_WRAP_H
#define FFT_LIB_WRAP_H

#include <complex>
#include "Debugging.h"

class FFTLibWrapper
{
  public:
    typedef double Real;
    typedef std::complex<Real> Complex;

    enum FFTDirection
    {
      FFTForward = -1,
      FFTBackward = 1,
    };

    enum FFTOptimization
    {
      Measure = 0,
      Estimate = 1U << 6,
    };

    int Size() const { return mFFTSize; }
    void Compute();

    static bool LibAvailable();
    static const char* LibName();

  protected:
    FFTLibWrapper();
    virtual ~FFTLibWrapper();
    void Cleanup();

    int     mFFTSize;
    void*   mpInputData,
        *   mpOutputData;
    void*   mLibPrivateData;

  private:
    static int sNumInstances;
};

class RealFFT : public FFTLibWrapper
{
  public:
    bool Initialize( int FFTSize, FFTDirection = FFTForward, FFTOptimization = Estimate );
    Real& Input( int index );
    const Real& Output( int index ) const;
};

class ComplexFFT : public FFTLibWrapper
{
  public:
    bool Initialize( int FFTSize, FFTDirection = FFTForward, FFTOptimization = Estimate );
    Complex& Input( int index );
    const Complex& Output( int index ) const;
};

inline
FFTLibWrapper::Real&
RealFFT::Input( int index )
{
  Assert( mpInputData != 0 && index < mFFTSize );
  return static_cast<Real*>( mpInputData )[index];
}

inline
const FFTLibWrapper::Real&
RealFFT::Output( int index ) const
{
  Assert( mpOutputData != 0 && index < mFFTSize );
  return static_cast<Real*>( mpOutputData )[index];
}

inline
FFTLibWrapper::Complex&
ComplexFFT::Input( int index )
{
  Assert( mpInputData != 0 && index < mFFTSize );
  return static_cast<Complex*>( mpInputData )[index];
}

inline
const FFTLibWrapper::Complex&
ComplexFFT::Output( int index ) const
{
  Assert( mpOutputData != 0 && index < mFFTSize );
  return static_cast<Complex*>( mpOutputData )[index];
}

#endif // FFT_LIB_WRAP_H
