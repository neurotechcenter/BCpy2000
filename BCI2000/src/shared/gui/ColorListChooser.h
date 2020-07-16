////////////////////////////////////////////////////////////////////////////////
// $Id: ColorListChooser.h 3798 2012-02-01 18:07:06Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A color chooser dialog used to define a color list.
//   The dialog's "custom colors" are used to hold colors, and the first
//   black entry delimits the list of colors.
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
#ifndef COLOR_LIST_CHOOSER_H
#define COLOR_LIST_CHOOSER_H

#include <string>
#include "VersionInfo.h"
#include "Color.h"

class ColorListChooser
{
 public:
  ColorListChooser()
    {}
  ~ColorListChooser()
    {}
  std::string Prompt() const
    { return mPrompt; }
  ColorListChooser& SetPrompt( const std::string& s )
    { mPrompt = s; return *this; }
  const ColorListChooser& Display( ColorList& ) const;

 private:
  std::string mPrompt;
};

#endif // COLOR_LIST_CHOOSER_H
