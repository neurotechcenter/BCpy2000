/////////////////////////////////////////////////////////////////////////////
// $Id: MeasurementUnits.h 4529 2013-08-05 13:56:53Z mellinger $
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
#ifndef MEASUREMENT_UNITS_H
#define MEASUREMENT_UNITS_H

#include "PhysicalUnit.h"
#include "SignalProperties.h"
#include "ParamList.h"
#include "Runnable.h"
#include "SharedPointer.h"
#include <string>

// This class converts strings such as "123.3" or "12ms" to plain numbers that
// represent values in global units.
class MeasurementUnits
{
  public:
    // Use these functions to convert values forth and back into "natural" BCI2000 units:
    static double TimeInSampleBlocks( const std::string& );
    static double TimeInSeconds( const std::string& value )
    { return TimeInSampleBlocks( value ) * sSampleBlockSize / sSamplingRate; }
    static double TimeInMilliseconds( const std::string& value )
    { return TimeInSeconds( value ) * 1e3; }

    static std::string TimeUnit()
    { return sTimeUnit.RawToPhysical( 1 ); }

    static double SamplingRate()
    { return sSamplingRate; }
    static int SampleBlockSize()
    { return static_cast<int>( sSampleBlockSize ); }
    static double SampleBlockDuration()
    { return sSampleBlockSize / sSamplingRate; }

    static double ValueIn( const std::string& unitsOf, const std::string& );

    static void Initialize( ParamList& );
    static void AddInitializeCallback( const SharedPointer<Runnable>& );

  private:
    static double sSamplingRate;
    static double sSampleBlockSize;
    static PhysicalUnit sTimeUnit, sIdUnit;
};

#endif // MEASUREMENT_UNITS_H
