////////////////////////////////////////////////////////////////////////////////
// $Id: CfgID.h 5649 2017-07-14 17:34:26Z mellinger $
// Description: Visualization Configuration IDs and their conversion from
//   constant into text form, and back.
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
#ifndef CFG_ID_H
#define CFG_ID_H

#include <string>
#include "defines.h"

class CfgID
{
 public:

   typedef uint8_t IDType;

  enum CfgID_ // Must have a name to be used as a template argument.
  {
    None = 251, // -5

    Top = 252, // -4
    Left = 253, // -3
    Width = 254, // -2
    Height = 255, // -1
    WindowTitle = 1,

      // Graph options
      MinValue,
      MaxValue,
      NumSamples,
      Reserved_1, // was XAxisLabel
      Reserved_2, // was YAxisLabel
      ChannelGroupSize,
      GraphType,
        // Graph types
        Polyline,
          // Polyline options
          ShowBaselines,
          ChannelColors,
        Field2d,

      // Units
      SampleUnit,
      ChannelUnit,
      ValueUnit,

      // Memo options
      NumLines,

      // Label lists
      ChannelLabels,
      GroupLabels,
      XAxisLabels,
      YAxisLabels,
      // Marker lists
      XAxisMarkers,
      YAxisMarkers,
      // Miscellaneous
      ShowSampleUnit,
      ShowChannelUnit,
      ShowValueUnit,
      SampleOffset,

      Visible,
      InvertedDisplay,
      // Set filter property to "off" to disable a filter
      HPFilter,
      LPFilter,
      NotchFilter,

      ShowNumericValues,
      AutoScale,
      ChannelStyles,
      FixedScale,
      ValueColors,
      FontSize,
      WindowFrame,

      // When adding new values, don't forget to add them to the string list in CfgID.cpp as well.
  };

 public:
  CfgID( IDType i )
    : mID( i )
    {}
  CfgID( const std::string& );

  operator IDType() const
    { return mID; }
  operator std::string() const;

 private:
  IDType mID;
};

#endif // CFG_ID_H
