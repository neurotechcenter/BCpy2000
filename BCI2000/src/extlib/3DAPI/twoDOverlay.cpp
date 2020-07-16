///////////////////////////////////////////////////////////////////////////////////////
// $Id: twoDOverlay.cpp 3798 2012-02-01 18:07:06Z mellinger $
// Authors: shzeng, schalk@wadsworth.org
// Description: This "twoDOverlay" class resembles the 2D overlay on top of the
//   3D environment.
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

#include "twoDOverlay.h"
#include "glheaders.h"
#include "buffers.h"

void
twoDOverlay::onInitialize()
{
  mPictureTexture = buffers::loadTexture( mPictureFile );
  mTranslucencyTexture = buffers::loadTexture( mTranslucencyFile );
  overlayObj::onInitialize();
}

void
twoDOverlay::onCleanup()
{
  buffers::releaseTexture( mPictureFile );
  mPictureTexture = 0;
  buffers::releaseTexture( mTranslucencyFile );
  mTranslucencyTexture = 0;
  overlayObj::onCleanup();
}

void
twoDOverlay::drawQuad( const int* inRect )
{
  glBegin( GL_QUADS );
  // Display the top left point of the 2D image
  glTexCoord2f( 0, 1 );
  glVertex2f( inRect[0], inRect[1] );
  // Display the bottom left point of the 2D image
  glTexCoord2f( 0, 0 );
  glVertex2f( inRect[0], inRect[1] + inRect[3] );
  // Display the bottom right point of the 2D image
  glTexCoord2f( 1, 0 );
  glVertex2f( inRect[0] + inRect[2], inRect[1] + inRect[3] );
  // Display the top right point of the 2D image
  glTexCoord2f( 1, 1 );
  glVertex2f( inRect[0] + inRect[2], inRect[1] );
  // Stop drawing
  glEnd();
}

void
twoDOverlay::onRender() const
{
  int viewPort[4] = { -1, -1, -1, -1 };
  // Push the current projection matrix
  glGetIntegerv( GL_VIEWPORT, viewPort );
  if( mTranslucencyTexture )
  {
    glColor4f( 1, 1, 1, 1 );
    glEnable( GL_TEXTURE_2D );
    glEnable( GL_BLEND );
    glBlendFunc( GL_ZERO, GL_SRC_COLOR );
    //Bind the mask texture to our new 2D quad
    glBindTexture( GL_TEXTURE_2D, mTranslucencyTexture );
    // Display a 2D quad with the translucency mask
    drawQuad( viewPort );
  }
  if( mPictureTexture )
  {
    glColor4f( mBrightness, mBrightness, mBrightness, 1 );
    glEnable( GL_TEXTURE_2D );
    glEnable( GL_BLEND );
    glBlendFunc( GL_ONE, GL_ONE );
    // Bind the scope texture to this next 2D quad
    glBindTexture( GL_TEXTURE_2D, mPictureTexture );
    // Display a 2D quad with our scope texture
    drawQuad( viewPort );
  }
  glDisable( GL_BLEND );
}


