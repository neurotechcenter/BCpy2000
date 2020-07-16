////////////////////////////////////////////////////////////////////////////////
// $Id$
// Authors: juergen.mellinger@uni-tuebingen.de
// Description: A template for graphic resource data.
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
#ifndef GRAPHIC_RESOURCE_H
#define GRAPHIC_RESOURCE_H

#include "GUI.h"
#include "Exception.h"

#if USE_QT
# include <QPainter>
# include <QImage>
#endif

namespace GUI
{

namespace GraphicResource
{
  template<class T>
   int Width( const T& t )
    { return t[ 0 ].count; }
  template<class T>
   int Height( const T& t )
    { return t[ 1 ].count; }
  template<int Mode, class T>
   void Render( const T&, const DrawContext& );
};

} // namespace GUI

template<int Mode, class T>
void
GUI::GraphicResource::Render( const T& inGraphic, const GUI::DrawContext& inDC )
{
#if USE_QT
  if( inDC.handle.dc )
  {
    if( Mode != RenderingMode::Opaque )
      throw std_runtime_error << "Unsupported rendering mode: " << Mode;

    int width = Width( inGraphic ),
        height = Height( inGraphic );
    QImage image( width, height, QImage::Format_RGB888 );
    int run = 2,
        x = 0,
        y = 0;
    while( inGraphic[ run ].count > 0 )
    {
      int grayValue = inGraphic[ run ].color;
      for( int i = 0; i < inGraphic[ run ].count; ++i )
      {
        image.setPixel( x, y, qRgb( grayValue, grayValue, grayValue ) );
        if( ++x >= width )
        {
          ++y;
          x = 0;
        }
      }
      ++run;
    }
    QRect targetRect
    (
      inDC.rect.left,
      inDC.rect.top,
      inDC.rect.right - inDC.rect.left,
      inDC.rect.bottom - inDC.rect.top
    );
    inDC.handle.dc->drawImage( targetRect, image );
  }
#endif
}

#endif // GRAPHIC_RESOURCE_H
