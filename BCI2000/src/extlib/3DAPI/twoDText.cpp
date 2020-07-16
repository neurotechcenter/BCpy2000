///////////////////////////////////////////////////////////////////////////////////////
// $Id: twoDText.cpp 3798 2012-02-01 18:07:06Z mellinger $
// Authors: shzeng, schalk@wadsworth.org
// Description: This "twoDText" class holds the variables of the 2D text that is
//   going to lay on the top of the 2D overlay
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
///////////////////////////////////////////////////////////////////////////////////////
#include "PCHIncludes.h"
#pragma hdrstop

#include "twoDText.h"
#include "buffers.h"
#include <sstream>

using namespace std;

void
twoDText::onInitialize()
{
#if _WIN32
  fontID = buffers::loadFont2D( textFont, textSize );
#else // _WIN32
	fontID = 0;
#endif // _WIN32
  overlayObj::onInitialize();
}

void
twoDText::onCleanup()
{
#if _WIN32
  buffers::releaseFont2D( textFont, textSize );
#endif // _WIN32
  fontID = 0;
  overlayObj::onCleanup();
}

void
twoDText::onRender() const
{
#if _WIN32
  glDisable( GL_TEXTURE_1D );
  glDisable( GL_TEXTURE_2D );
  glColor3f( textColorR, textColorG, textColorB );
  glMatrixMode( GL_MODELVIEW );

  const ABCFLOAT* metrics = buffers::getFontData2D( fontID );
  if( metrics != NULL )
  {
    istringstream iss( caption );
    string line;
    int i = 0;
    while( getline( iss, line ) )
    {
      GLfloat delta = 0;
      for( size_t j = 0; j < line.length(); ++j )
        delta += metrics[line[j]].abcfA
               + metrics[line[j]].abcfB
               + metrics[line[j]].abcfC;
      switch( alignment )
      {
        case twoDText::left:
          delta = 0;
          break;
        case twoDText::right:
          delta = -delta;
          break;
        case twoDText::center:
          delta = -delta / 2;
          break;
      }
      glRasterPos2f( textX + delta,  textY - i * textSize ); // Set the drawing position
      // Now, before we set the list base, we need to save off the current one.
      glPushAttrib(GL_LIST_BIT);                    // This saves the list base information
      // Then we want to set the list base to the font's list base, which should be 1 in our case.
      // That way when we call our display list it will start from the font's lists'.
      glListBase(fontID);                           // This sets the lists base
      // Now comes the actually rendering.  We pass in the length of the string,
      // then the data types (which are characters so its a UINT), then the actually char array.
      // This will then take the ASCII value of each character and associate it with a bitmap.
      glCallLists( static_cast<GLsizei>( line.length() ), GL_UNSIGNED_BYTE, line.data() );
      glPopAttrib();
      ++i;
    }
  }
#endif // _WIN32
}

