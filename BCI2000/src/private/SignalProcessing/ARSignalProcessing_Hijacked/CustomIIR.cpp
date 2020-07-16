////////////////////////////////////////////////////////////////////////////////
// $Id: CustomIIR.cpp 3798 2012-02-01 18:07:06Z mellinger $
// Author:      jezhill@gmail.com
// Description: An IIR filter class containing a HighPass, LowPass, and Notch
//              filter.
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

#include "CustomIIR.h"
#include "FilterDesign.h"
#include <cassert>

using namespace std;

CustomIIR::CustomIIR()
: mHPCorner( 0 ),
  mLPCorner( 0 ),
  mNotchCenter( 0 )
{
}

CustomIIR::~CustomIIR()
{
}

CustomIIR&
CustomIIR::HPCorner( Real inCorner )
{
  if( inCorner != mHPCorner )
  {
    mHPCorner = inCorner;
    DesignFilter();
  }
  return *this;
}

CustomIIR::Real
CustomIIR::HPCorner() const
{
  return mHPCorner;
}

CustomIIR&
CustomIIR::LPCorner( Real inCorner )
{
  if( inCorner != mLPCorner )
  {
    mLPCorner = inCorner;
    DesignFilter();
  }
  return *this;
}

CustomIIR::Real
CustomIIR::LPCorner() const
{
  return mLPCorner;
}

CustomIIR&
CustomIIR::NotchCenter( Real inCenter )
{
  if( inCenter != mNotchCenter )
  {
    mNotchCenter = inCenter;
    DesignFilter();
  }
  return *this;
}

CustomIIR::Real
CustomIIR::NotchCenter() const
{
  return mNotchCenter;
}


void
CustomIIR::DesignFilter()
{
  typedef Ratpoly<FilterDesign::Complex> TransferFunction;
  TransferFunction tf( 1.0 );
  Real gain = 1.0;

  // Design a HP.
  if( mHPCorner > 0 && mHPCorner < 0.5 )
  {
    TransferFunction hp =
      FilterDesign::Butterworth()
      .Order( 1 )
      .Highpass( mHPCorner )
      .TransferFunction();
    tf *= hp;
    gain *= abs( hp.Evaluate( -1.0 ) ); // HF gain
  }
  // Add a LP.
  if( mLPCorner > 0 && mLPCorner < 0.5 )
  {
    TransferFunction lp =
      FilterDesign::Butterworth()
      .Order( 2 )
      .Lowpass( mLPCorner )
      .TransferFunction();
    tf *= lp;
    gain *= abs( lp.Evaluate( 1.0 ) ); // LF gain
  }
  // Add a notch filter.
  Real corner1 = 0.9 * mNotchCenter,
       corner2 = 1.1 * mNotchCenter;
  if( corner1 > 0 && corner2 < 0.5 )
  {
    TransferFunction notch =
      FilterDesign::Chebyshev()
      .Ripple_dB( -0.1 )
      .Order( 4 )
      .Bandstop( corner1, corner2 )
      .TransferFunction();
    tf *= notch;
    gain *= abs( notch.Evaluate( 1.0 ) ); // LF gain
  }

  assert( gain > 0 );
  mFilter.SetGain( 1 / gain )
         .SetZeros( tf.Numerator().Roots() )
         .SetPoles( tf.Denominator().Roots() );
  Reset();
}

void
CustomIIR::Reset()
{
  mFilter.Initialize();
}

void
CustomIIR::Process( const GenericSignal& Input, GenericSignal& Output )
{
  if( Input.Channels() != mFilter.Channels() )
    mFilter.Initialize( Input.Channels() );
  mFilter.Process( Input, Output );
}


