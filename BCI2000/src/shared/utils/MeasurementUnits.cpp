/////////////////////////////////////////////////////////////////////////////
// $Id: MeasurementUnits.cpp 4558 2013-08-29 16:54:24Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: Utilities for handling measurement units and conversions.
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
//////////////////////////////////////////////////////////////////////////////
#include "PCHIncludes.h"
#pragma hdrstop

#include "MeasurementUnits.h"
#include "BCIStream.h"
#include <limits>

using namespace std;

double MeasurementUnits::sSamplingRate = 1.0;
double MeasurementUnits::sSampleBlockSize = 1.0;
PhysicalUnit MeasurementUnits::sTimeUnit;
PhysicalUnit MeasurementUnits::sIdUnit;

typedef vector< SharedPointer<Runnable> > CallbackList;
static CallbackList sCallbacks;

double
MeasurementUnits::TimeInSampleBlocks( const std::string& value )
{
  if( sTimeUnit.IsPhysical( value ) )
    return sTimeUnit.PhysicalToRaw( value );
  return value.empty() ? 0 : sIdUnit.PhysicalToRaw( value );
}

double
MeasurementUnits::ValueIn( const std::string& unitsOf, const std::string& value )
{
  string unit = "1" + unitsOf;
  if( sTimeUnit.IsPhysical( unit ) )
    return TimeInSeconds( value ) / TimeInSeconds( unit );
  PhysicalUnit u;
  u.SetGainWithSymbol( unit );
  if( u.IsPhysical( value ) )
    return u.PhysicalToRaw( value );
  return value.empty() ? 0 : sIdUnit.PhysicalToRaw( value ) / u.Gain();
}

void
MeasurementUnits::Initialize( ParamList& inParams )
{
  if( inParams.Exists( "SamplingRate" ) )
  {
    Param& p = inParams["SamplingRate"];
    PhysicalUnit u;
    u.SetSymbol( "Hz" );
    if( !u.IsPhysical( p.Value() ) )
      p.Value() = string( p.Value() ) + "Hz";
    sSamplingRate = u.PhysicalToRaw( p.Value() );
    if( sSamplingRate <= 0.0 )
      bcierr << "Parameter SamplingRate needs to be greater zero";
  }
  if( inParams.Exists( "SampleBlockSize" ) )
  {
    string value = inParams[ "SampleBlockSize" ].Value();
    if( PhysicalUnit().SetSymbol( "s" ).IsPhysical( value ) )
      bcierr << "Parameter SampleBlockSize must be given in samples, not in seconds";
    else
    {
      sSampleBlockSize = PhysicalUnit().SetSymbol( "" ).PhysicalToRaw( value );
      if( sSampleBlockSize < 1 )
        bcierr << "Parameter SampleBlockSize needs to be >= 1";
    }
  }
  // Set the unit for raw numbers representing time to multiples of sample block duration.
  sTimeUnit.SetOffset( 0 ).SetGain( sSampleBlockSize / sSamplingRate ).SetSymbol( "s" );

  for( CallbackList::iterator i = sCallbacks.begin(); i != sCallbacks.end(); ++i )
    (*i)->Run();
}

void
MeasurementUnits::AddInitializeCallback( const SharedPointer<Runnable>& r )
{
  sCallbacks.push_back( r );
}

