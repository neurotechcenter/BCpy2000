////////////////////////////////////////////////////////////////////////////////
// $Id: FFTFilter.h 5381 2016-05-27 19:27:20Z gmilsap $
// Description: A BCI2000 filter that applies a short-term FFT to its input
//   signal.
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
#ifndef FFT_FILTER_H
#define FFT_FILTER_H

#include <vector>
#include "GenericVisualization.h"
#include "GenericFilter.h"

#include "FFTLibWrap.h"

class FFTFilter : public GenericFilter
{
 public:
  FFTFilter();
  virtual ~FFTFilter();
  virtual void Preflight( const SignalProperties&, SignalProperties& ) const;
  virtual void Initialize( const SignalProperties&, const SignalProperties& );
  virtual void Process( const GenericSignal&, GenericSignal& );
  virtual void StopRun();

 private:
  bool mVisualizeFFT;
  enum eFFTOutputSignal
  {
    eInput = 0,
    ePower,
    eHalfcomplex,
  } mFFTOutputSignal;
  std::vector<size_t> mFFTInputChannels;
  int mFFTWindowLength;
  enum eFFTWindow
  {
    eNone = 0,
    eHamming,
    eHann,
    eBlackman,
  } mFFTWindow;

  void ResetValueBuffers( size_t );
  void DetermineSignalProperties( SignalProperties&, int ) const;
  std::string FlatParameter( const std::string& inName ) const;

  SignalProperties                  mVisProperties;
  GenericSignal                     mPowerSpectrum;
  std::vector<GenericVisualization> mVisualizations;
  std::vector<std::vector<float> >  mValueBuffers;
  std::vector<float>                mWindow;

  RealFFT mFFT;
};

#endif // FFT_FILTER_H
