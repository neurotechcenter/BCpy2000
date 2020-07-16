////////////////////////////////////////////////////////////////////////////////
// $Id: DecimationFilter.cpp 4536 2013-08-05 14:30:13Z mellinger $
// Author:      juergen.mellinger@uni-tuebingen.de
// Description: A filter that downsamples a signal after applying a moving
//   average filter.
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
#include "PCHIncludes.h"
#pragma hdrstop

#include "DecimationFilter.h"

using namespace std;

DecimationFilter::DecimationFilter()
: mDecimation( 1 ),
  mCurDecimationCount( 0 )
{
}

DecimationFilter::~DecimationFilter()
{
}

DecimationFilter& 
DecimationFilter::Decimation( int inDecimation )
{
  if( inDecimation != mDecimation )
  {
    mDecimation = inDecimation;
    Reset();
  }
  return *this;
}

int            
DecimationFilter::Decimation() const
{
  return mDecimation;
}

void 
DecimationFilter::Process( const GenericSignal& input, GenericSignal& output )
{
  if( mDecimation == 1 )
  {
    output = input;
    return;
  }
  if( mValues.size() != input.Channels() )
  {
    mValues.resize( input.Channels(), 0.0 );
    mCurDecimationCount = 0;
  }
  int outputSamples = ( input.Elements() + mCurDecimationCount ) / mDecimation;
  output.SetProperties( SignalProperties( input.Channels(), outputSamples ) );
  for( int i = 0; i < input.Elements(); ++i, ++mCurDecimationCount %= mDecimation )
  { 
    for( int ch = 0; ch < input.Channels(); ++ch )
    {
      mValues[ch] += input( ch, i ) / mDecimation;
      if( mCurDecimationCount == mDecimation - 1 )
      {
        output( ch, i / mDecimation ) = mValues[ch];
        mValues[ch] = 0.0;
      }
    }
  }
}

void
DecimationFilter::Reset()
{
  mValues.clear();
  mCurDecimationCount = 0;
}
