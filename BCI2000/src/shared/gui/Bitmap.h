////////////////////////////////////////////////////////////////////////////////
// $Id$
// Author: juergen.mellinger@uni-tuebingen.de
// Description: An extension of the BitmapImage class to handle interfacing
//   with GUI objects.
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
#ifndef GUI_BITMAP_H
#define GUI_BITMAP_H

#include "GUI.h"
#include "BitmapImage.h"

namespace GUI
{

class OpenGLTexture;

class Bitmap : public BitmapImage
{
public:
  Bitmap( int inWidth = 0, int inHeight = 0 );
  Bitmap( const BitmapImage& );
  Bitmap( int inWidth, int inHeight, uint16_t* inpData );
  Bitmap& operator=( const BitmapImage& );
  Bitmap& CopyFrom( const OpenGLTexture* );
#if 0
  Bitmap& GrabFrom( void* window, const GUI::Rect* = 0 );
#endif
};

} // namespace GUI

#endif // GUI_BITMAP_H

