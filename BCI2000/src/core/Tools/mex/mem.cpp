///////////////////////////////////////////////////////////////////////////////
// $Id: mem.cpp 5378 2016-05-26 14:44:06Z mellinger $
// Authors: mcfarlan@wadsworth.org, schalk@wadsworth.org,
//          juergen.mellinger@uni-tuebingen.de
// Description: A Matlab (mex) subroutine that applies the MEM spectral
//  estimator available in the BCI2000 AR signal processing module.
//  For the calling syntax, see the USAGE macro below.
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
///////////////////////////////////////////////////////////////////////////////
#include "mex.h"

#include "mexutils.h"
#include "ARFilter.h"
#include "StandaloneFilters.h"
#include "BCIStream.h"
#include "BCIException.h"
#include <limits>

#define USAGE \
"The mem command estimates a power spectrum using Burg's MEM algorithm.\n" \
"The calling syntax is:\n" \
        "[spectrum, frequencies] = mem(signal, parms)\n" \
        " with <signal> having dimensions (values x channels),\n" \
        " <spectrum> having dimensions (values x channels x blocks)\n" \
        " and with <parms> being a vector of parameter values:\n" \
        " model order,\n" \
        " first bin center,\n" \
        " last bin center,\n" \
        " bin width,\n" \
        " evaluations per bin,\n" \
        " detrend option (optional, 0: none, 1: mean, 2: linear; defaults to none),\n" \
        " frequency (optional, defaults to 1),\n" \
        " sample block size (optional, defaults to size(signal,1)),\n" \
        " window length in sample blocks (optional, defaults to 1)\n"

using namespace std;

const double eps = numeric_limits<double>::epsilon();

void
BCIMexFunction( int nlhs, mxArray* plhs[],
                int nrhs, const mxArray* prhs[] )
{
  if( PrintVersion( __FILE__, nrhs, prhs ) )
    return;

  const mxArray* inSignalArray = prhs[0],
               * inParmsArray = prhs[1];

  // Check for proper number of arguments
  if( nrhs != 2 )
    throw bciexception << "Two input arguments required -- " USAGE;
  if( nlhs > 2 )
    throw bciexception << "Too many output arguments -- " USAGE;

  int numSamples  = static_cast<int>( ::mxGetM( inSignalArray ) ),
      numChannels = static_cast<int>( ::mxGetN( inSignalArray ) );
  mwSize numParms = static_cast<mwSize>( ::mxGetNumberOfElements( inParmsArray ) );

  if( !::mxIsDouble( inSignalArray ) || ::mxIsComplex( inSignalArray ) )
    throw bciexception << "Expected real double input signal -- " USAGE ;
  if( !::mxIsDouble( inParmsArray ) || ::mxIsComplex( inParmsArray ) )
    throw bciexception << "Expected real double parameter vector -- " USAGE;
  if ( numParms < 5 )
    throw bciexception << "Expected at least 5 MEM parameter values -- " USAGE;

  double* inSignal = ::mxGetPr( inSignalArray ),
        * inParms  = ::mxGetPr( inParmsArray ),
        * p = inParms;
  double  modelOrder        = *p++,
          firstBinCenter    = *p++,
          lastBinCenter     = *p++,
          binWidth          = *p++,
          evaluationsPerBin = *p++,
          detrendOption     = numParms > ( p - inParms ) ? *p++ : WindowingFilter::Detrend::None,
          frequency         = numParms > ( p - inParms ) ? *p++ : 1.0,
          sampleBlockSize   = numParms > ( p - inParms ) ? *p++ : numSamples,
          windowLength      = numParms > ( p - inParms ) ? *p++ : 1;

  if( modelOrder >= numSamples )
    throw bciexception << "The number of input samples must exceed the model order.";
  if( binWidth <= 0.0 || binWidth > frequency/2 )
    throw bciexception << "Bin width must be between 0 and half the sampling rate.";
  if( firstBinCenter < 0.0 || firstBinCenter > frequency/2 )
    throw bciexception << "First bin center must be between 0 and half the sampling rate.";
  if( lastBinCenter < firstBinCenter || lastBinCenter > frequency/2 )
    throw bciexception << "Last bin center must be between firstBinCenter and half the sampling rate.";
  if( evaluationsPerBin < 1 )
    throw bciexception << "There must be at least 1 evaluation per bin.";
  if( frequency < eps )
    throw bciexception << "Frequency must be > 0.";
  if( sampleBlockSize < 1 )
    throw bciexception << "Sample block size must be >= 1.";
  if( windowLength * sampleBlockSize < modelOrder )
    throw bciexception << "Window must contain more samples than the model order.";
  if( windowLength * sampleBlockSize > numSamples )
    throw bciexception << "Window may not exceed the number of samples.";
  switch( static_cast<int>( detrendOption ) )
  {
    case WindowingFilter::Detrend::None:
    case WindowingFilter::Detrend::Mean:
    case WindowingFilter::Detrend::Linear:
      break;
    default:
      throw bciexception << "Unknown detrend option.";
  }
  StandaloneFilter<ARFilter> filter;
  filter.Parameters().Add( "% int EvaluateTiming= 0" );
  filter.Parameter( "WindowLength" ) = windowLength;
  filter.Parameter( "Detrend" ) = detrendOption;
  filter.Parameter( "ModelOrder" ) = modelOrder;
  filter.Parameter( "FirstBinCenter" ) = firstBinCenter;
  filter.Parameter( "LastBinCenter" ) = lastBinCenter;
  filter.Parameter( "BinWidth" ) = binWidth;
  filter.Parameter( "EvaluationsPerBin" ) = evaluationsPerBin;
  filter.Parameter( "OutputType" ) = Spectrum::OutputType::SpectralPower;

  int iSampleBlockSize = static_cast<int>( sampleBlockSize );
  SignalProperties inputProperties( numChannels, iSampleBlockSize );
  inputProperties.ElementUnit().SetGain( 1.0 / frequency ).SetSymbol( "s" );
  filter.SetConfig( inputProperties );

  filter.Start();
  int numBins = filter.Output().Elements(),
      samplesInWindow = windowLength * sampleBlockSize,
      numBlocks = ( numSamples - samplesInWindow + sampleBlockSize ) / sampleBlockSize;
  const mwSize dims[] =
  {
      static_cast<size_t>(numBins),
      static_cast<size_t>(numChannels), 
      static_cast<size_t>(numBlocks)
  };
  plhs[0] = ::mxCreateNumericArray( 3, dims, mxDOUBLE_CLASS, mxREAL );
  double* outSpectrum = ::mxGetPr( plhs[0] );
  if( nlhs > 1 )
  {
    plhs[1] = ::mxCreateDoubleMatrix( numBins, 1, mxREAL );
    double* outBinFreqs = ::mxGetPr( plhs[1] );
    for( int bin = 0; bin < numBins; ++bin )
      outBinFreqs[bin] = ( firstBinCenter + bin * binWidth );
  }

  GenericSignal input( inputProperties );

  int start = 0;
  while( start < ( samplesInWindow - iSampleBlockSize ) && start <= numSamples - iSampleBlockSize )
  { // Ignore filter output until its internal buffer has been filled with data.
    for( int ch = 0; ch < numChannels; ++ch )
      for( int s = 0; s < iSampleBlockSize; ++s )
        input( ch, s ) = inSignal[ch*numSamples + s + start];
    filter.Process( input );
    start += iSampleBlockSize;
  }

  int blockNum = 0;
  while( start <= numSamples - iSampleBlockSize )
  {
    for( int ch = 0; ch < numChannels; ++ch )
      for( int s = 0; s < iSampleBlockSize; ++s )
        input( ch, s ) = inSignal[ch*numSamples + s + start];
    filter.Process( input );
    for( int ch = 0; ch < numChannels; ++ch )
      for( int bin = 0; bin < numBins; ++bin )
        outSpectrum[bin + ch*numBins + blockNum*numBins*numChannels] = filter.Output()( ch, bin );
    start += iSampleBlockSize;
    ++blockNum;
  }
  filter.Stop();

}
