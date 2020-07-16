////////////////////////////////////////////////////////////////////////////////
// $Id$
// Author: juergen.mellinger@neurotechcenter.org
// Description: Glue code for convenient interfacing of BCI2000 GUI-related
//   code with Qt.
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
#include "BitmapImage.h"
#include <QImage>

BitmapImage::BitmapImage(const QImage& inQImage)
: mWidth(inQImage.width()),
  mHeight(inQImage.height()),
  mpData(new uint16_t[mWidth*mHeight])
{
  QImage img = inQImage;
  if(inQImage.format() != QImage::Format_RGB16)
    img = inQImage.convertToFormat(QImage::Format_RGB16);
  ::memcpy(mpData, img.constBits(), mWidth * mHeight * sizeof(*mpData));
  // pixel format conversion: Qt uses RGB 565, we use ARGB 4444
  for(uint16_t* p = mpData; p < mpData + mWidth*mHeight; ++p)
  {
    uint16_t r = *p >> 12 & 0xf, g = *p >> 7 & 0xf, b = *p >> 1 & 0xf;
    *p = r << 8 | g << 4 | b;
  }
}
