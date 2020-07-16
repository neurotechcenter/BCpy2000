////////////////////////////////////////////////////////////////////////////////
// $Id: CfgID.cpp 5649 2017-07-14 17:34:26Z mellinger $
// Description: Visualization Configuration IDs and their conversion from
//   constant into text form), and back.
//
// $BEGIN_BCI2000_LICENSE$
//
// This file is part of BCI2000), a platform for real-time bio-signal research.
// [ Copyright (C) 2000-2012: BCI2000 team and many external contributors ]
//
// BCI2000 is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software
// Foundation), either version 3 of the License), or (at your option) any later
// version.
//
// BCI2000 is distributed in the hope that it will be useful), but
//                         WITHOUT ANY WARRANTY
// - without even the implied warranty of MERCHANTABILITY or FITNESS FOR
// A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with
// this program.  If not), see <http://www.gnu.org/licenses/>.
//
// $END_BCI2000_LICENSE$
////////////////////////////////////////////////////////////////////////////////
#include "CfgID.h"

#include <cstring>

#define CFGID( x ) { #x, CfgID::x }
static struct { const char* name; int id; } sCfgIDs[] =
{
  CFGID( None ),

  CFGID( Top ),
  CFGID( Left ),
  CFGID( Width ),
  CFGID( Height ),
  CFGID( WindowTitle ),

    // Graph options
    CFGID( MinValue ),
    CFGID( MaxValue ),
    CFGID( NumSamples ),
    CFGID( Reserved_1 ), // was XAxisLabel
    CFGID( Reserved_2 ), // was YAxisLabel
    CFGID( ChannelGroupSize ),
    CFGID( GraphType ),
      // Graph types
      CFGID( Polyline ),
        // Polyline options
        CFGID( ShowBaselines ),
        CFGID( ChannelColors ),
      CFGID( Field2d ),

    // Units
    CFGID( SampleUnit ),
    CFGID( ChannelUnit ),
    CFGID( ValueUnit ),

    // Memo options
    CFGID( NumLines ),

    // Label lists
    CFGID( ChannelLabels ),
    CFGID( GroupLabels ),
    CFGID( XAxisLabels ),
    CFGID( YAxisLabels ),
    // Marker lists
    CFGID( XAxisMarkers ),
    CFGID( YAxisMarkers ),
    // Miscellaneous
    CFGID( ShowSampleUnit ),
    CFGID( ShowChannelUnit ),
    CFGID( ShowValueUnit ),
    CFGID( SampleOffset ),

    CFGID( Visible ),
    CFGID( InvertedDisplay ),
    // Set filter property to "off" to disable a filter
    CFGID( HPFilter ),
    CFGID( LPFilter ),
    CFGID( NotchFilter ),

    CFGID( ShowNumericValues ),
    CFGID( AutoScale ),
    CFGID( ChannelStyles ),
    CFGID( FixedScale ),
    CFGID( ValueColors ),
    CFGID( FontSize ),

    CFGID( WindowFrame ),
};

static const int sCfgListEntries = sizeof( sCfgIDs ) / sizeof( *sCfgIDs );

CfgID::CfgID( const std::string& inName )
: mID( CfgID::None )
{
  for( int i = 0; i < sCfgListEntries; ++i )
    if( 0 == ::stricmp( inName.c_str(), sCfgIDs[i].name ) )
    {
      mID = sCfgIDs[i].id;
      break;
    }
}

CfgID::operator std::string() const
{
  for( int i = 0; i < sCfgListEntries; ++i )
    if( mID == sCfgIDs[i].id )
      return sCfgIDs[i].name;
  return "None";
}
