////////////////////////////////////////////////////////////////////////////////
// $Id: StatusBar.h 5014 2015-09-28 19:54:59Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A GraphObject consisting of two stacked text fields
//   separated with a separator bar.
//   GoalText is the top field, ResultText is the bottom field.
//   For an empty GoalText, ResultText occupies the top as well, and the
//   separator bar is not displayed.
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
#ifndef STATUS_BAR_H
#define STATUS_BAR_H

#include "GraphObject.h"

#include "Color.h"
#include <string>

class StatusBar : public GUI::GraphObject
{
 public:
  StatusBar( GUI::GraphDisplay& display );
  virtual ~StatusBar();
  // Properties
  StatusBar& SetGoalText( const std::string& );
  const std::string& GoalText() const;
  StatusBar& SetResultText( const std::string& );
  const std::string& ResultText() const;
  StatusBar& SetTextHeight( float );
  float TextHeight() const;
  StatusBar& SetTextColor( RGBColor );
  RGBColor TextColor() const;
  StatusBar& SetColor( RGBColor );
  RGBColor Color() const;

 private:
  // GraphObject event handlers
  virtual void OnChange( GUI::DrawContext& );
  virtual void OnPaint( const GUI::DrawContext& );

 private:
  struct Private;
  Private* p;
};

#endif // STATUS_BAR_H

