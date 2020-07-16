////////////////////////////////////////////////////////////////////////////////
// $Id: ColorListChooser.cpp 4608 2013-10-14 13:26:14Z mellinger $
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
#include "ColorListChooser.h"

#include <QColorDialog>
#include <algorithm>

using namespace std;

const ColorListChooser&
ColorListChooser::Display( ColorList& ioColors ) const
{
  for( int i = 0; i < min( static_cast<int>( ioColors.size() ), QColorDialog::customCount() ); ++i )
    QColorDialog::setCustomColor( i, ioColors[i].ToQRgb() );
  for( int i = static_cast<int>( ioColors.size() ); i < QColorDialog::customCount(); ++i )
    QColorDialog::setCustomColor( i, RGBColor( RGBColor::Black ).ToQRgb() );
  if( QColorDialog::getColor( Qt::white, NULL, mPrompt.c_str(), QColorDialog::DontUseNativeDialog ).isValid() )
  {
    int numUserColors = 0;
    while( numUserColors < QColorDialog::customCount()
      && QColorDialog::customColor( numUserColors ) != RGBColor::Black )
      ++numUserColors;
    if( numUserColors == 0 )
      ioColors.resize( 1, RGBColor::White );
    else
    {
      ioColors.resize( numUserColors );
      for( int i = 0; i < numUserColors; ++i )
#if QT_IS4
        ioColors[i] = QColorDialog::customColor( i );
#else
        ioColors[i] = RGBColor::FromQRgb( QColorDialog::customColor( i ).rgba() );
#endif
    }
  }
  return *this;
}
