////////////////////////////////////////////////////////////////////////////////
// $Id$
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A GraphObject intended for testing purposes.
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
#ifndef DISPLAY_STATISTICS_FIELD_H
#define DISPLAY_STATISTICS_FIELD_H

#include "TextField.h"
#include "DisplayWindow.h"

class DisplayStatisticsField : public TextField
{
 public:
  DisplayStatisticsField( GUI::GraphDisplay&, int = MessageZOrder );
  ~DisplayStatisticsField();
  bool MayBePresented() const override { return false; }

 protected:
  // GraphObject event handlers
  void OnTick( const ClockTick& ) override;
  void OnChange(GUI::DrawContext&) override;

 private:
  std::string mInfo;
  GUI::DisplayWindow* mpWindow;
};

#endif // DISPLAY_STATISTICS_FIELD_H

