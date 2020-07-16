////////////////////////////////////////////////////////////////////////////////
// $Id: EDFHeader.h 4731 2014-07-03 15:18:03Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A C++ representation of a BCI2000 relevant subset of the EDF
//              data format as defined in Kemp et al, 1992, and the
//              GDF 2.10 data format as defined in Schloegl et al, 2008.
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

#ifndef EDF_HEADER_H
#define EDF_HEADER_H

#include <string>
#include <vector>
#include <iostream>
#include <cstring>

struct EDFHeader
{
  std::string Version, Patient, Recording, Startdate;
  int NumBlocks;
  double BlockDuration;

  struct ChannelInfo
  {
    std::string Label,
                TransducerType,
                PhysicalDimension,
                Filtering;
    double      PhysicalMinimum,
                PhysicalMaximum,
                DigitalMinimum,
                DigitalMaximum;
    float       LowPass,
                HighPass,
                Notch,
                ElectrodePosition[3];
    unsigned short PhysicalDimensionCode;
    long        SamplesPerRecord;
    int         ElectrodeImpedance,
                DataType;

    ChannelInfo()
      : PhysicalMinimum( 0 ), PhysicalMaximum( 0 ),
        DigitalMinimum( 0 ), DigitalMaximum( 0 ),
        LowPass( 0 ), HighPass( 0 ), Notch( 0 ),
        PhysicalDimensionCode( 0 ), SamplesPerRecord( 0 ),
        ElectrodeImpedance( 0 ), DataType( 0 )
    { ::memset( ElectrodePosition, 0, sizeof( ElectrodePosition ) ); }

  };
  struct ChannelList : std::vector<ChannelInfo>
  { int Size() const { return static_cast<int>( size() ); }
  } Channels;

  EDFHeader() : Version( "0" ), NumBlocks( -1 ), BlockDuration( 0 ) {}
  size_t Length() const;
  std::ostream& Serialize( std::ostream& ) const;
  std::istream& Unserialize( std::istream& );
};

#endif // EDF_HEADER_H
