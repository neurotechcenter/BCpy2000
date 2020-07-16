////////////////////////////////////////////////////////////////////////////////
// $Id: VisID.h 5281 2016-03-15 13:54:30Z mellinger $
// Author: griffin.milsap@gmail.com, juergen.mellinger@uni-tuebingen.de
// Description: A class representing a Visualization ID.
//   Visualization IDs refer to visualization windows,
//   and layers within windows. In case of windows,
//   the Visualization ID is a window ID; in case of
//   layers, the Visualization ID is a combination of
//   window ID and layer ID, separated with a colon.
//   When a colon but no layer ID is present, the
//   resulting Visualization ID refers to the window's
//   "dominating layer" which is always at the bottom
//   of the window, and receives keyboard focus.
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
#ifndef VIS_ID_H
#define VIS_ID_H

#include "EncodedString.h"

class VisID : public EncodedString
{
 public:
  VisID();
  VisID( const std::string& );
  VisID( const char* );

  VisID& SetWindowID( const std::string& );
  std::string WindowID() const;

  VisID& SetLayerID( const std::string& );
  std::string LayerID() const;

  bool IsLayer() const;
  VisID& ToLayer();
};

#endif // VIS_ID_H

