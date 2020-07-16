////////////////////////////////////////////////////////////////////////////////
// $Id: threeDText.cpp 3798 2012-02-01 18:07:06Z mellinger $
// Authors: shzeng, schalk@wadsworth.org
// Description: This "threeDText" class resembles the 3D text in the 3D
//   environment.
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
#include "PCHIncludes.h"
#pragma hdrstop

#include "threeDText.h"
#include "buffers.h"

using namespace std;

void
threeDText::onInitialize()
{
#if _WIN32
  mFontID = buffers::loadFont3D( mFont );
#else // _WIN32
	mFontID = 0;
#endif // _WIN32
  sceneObj::onInitialize();
}

void
threeDText::onCleanup()
{
#if _WIN32
  buffers::releaseFont3D( mFont );
#endif // _WIN32
  mFontID = 0;
  sceneObj::onCleanup();
}

void
threeDText::onRender() const
{
#if _WIN32
  if( !mCaption.empty() )
  {  //print the 3D text
    const GLYPHMETRICSFLOAT* gmf = buffers::getFontData3D(mFontID);
    if( gmf != NULL )
    {
      GLfloat height = gmf['X'].gmfBlackBoxY,
              width = 0;
      //resize the font
      GLfloat ratio = mFontSize / height;
      glScalef( ratio, ratio, ratio );
      for( size_t i = 0; i < mCaption.length(); ++i )
        width += gmf[ mCaption[i] ].gmfCellIncX;
      // Center our text on the screen
      glTranslatef( -width/2, -height/2, 0 );

      glPushAttrib( GL_LIST_BIT );  // Pushes the display list bits
      glListBase( mFontID );        // Sets the base character to 0
      glCallLists( static_cast<GLsizei>( mCaption.length() ), GL_UNSIGNED_BYTE, mCaption.data() ); // Draws the display list text
      glPopAttrib();                // Pops the display list bits
    }
  }
#endif // _WIN32
}


