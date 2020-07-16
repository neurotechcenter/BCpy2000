////////////////////////////////////////////////////////////////////////////////
// $Id: primObj.cpp 5492 2016-08-04 16:47:13Z mellinger $
// Authors: shzeng, schalk@wadsworth.org
// Description: primObj is a base class for all objects in the 3D environment.
//   Two classes are derived from primObj:
//     sceneObj representing 3D scene objects, and
//     overlayObj representing objects in the 2D overlay.
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

#include "glheaders.h"
#include "primObj.h"
#include "buffers.h"
#include "Scene.h"

using namespace std;

primObj::primObj( Scene& inScene )
: mScene( inScene ),
  mVisible( true ),
  mDrawingOrder( 0 ),
  mChanged( true )
{
  mScene.Add( this );
}

primObj::~primObj()
{
  mScene.Remove( this );
}


void
overlayObj::render() const
{
  if( getVisible() )
  {
    glDisable( GL_DEPTH_TEST );
    glDisable( GL_LIGHTING );
    glBindTexture( GL_TEXTURE_2D, 0 );
    glMatrixMode( GL_MODELVIEW );
    glPushMatrix();
    glLoadIdentity();
    glMatrixMode( GL_PROJECTION );
    glPushMatrix();
    glLoadIdentity();
    int viewPort[4] = { -1, -1, -1, -1 };
    glGetIntegerv( GL_VIEWPORT, viewPort );
    glOrtho( 0, viewPort[2], 0, viewPort[3], -1, 1 );
    glMatrixMode( GL_MODELVIEW );

    onRender();

    glEnable( GL_LIGHTING );
    glEnable( GL_DEPTH_TEST );
    glMatrixMode( GL_PROJECTION );
    glPopMatrix();
    glMatrixMode( GL_MODELVIEW );
    glPopMatrix();
  }
}


sceneObj::sceneObj( Scene& inScene )
: primObj( inScene ),
  mBrightness(1),
  mTransparency(0),
  mRColor(1),
  mBColor(1),
  mGColor(1),
  mTextureHandle(0)
{
}


// Rendering
void
sceneObj::render() const
{
  if( getVisible() )
  {
    // apply world transform
    glMatrixMode( GL_MODELVIEW );
    glPushMatrix();
    glMultMatrixf( getWorldTransform() );
    glEnable( GL_NORMALIZE );

    // bind texture
    if( mTextureHandle != 0 )  //check to see if texture date loaded successfully
    {
      glEnable( GL_TEXTURE_2D );
      glBindTexture( GL_TEXTURE_2D, mTextureHandle );
    }
    else
    {
      glDisable( GL_TEXTURE_2D );
    }

    //transparency and brightness
    GLfloat b = clamp( getBrightness() ),
            t = clamp( getTransparency() );

    glColor4f( mRColor * b, mGColor * b, mBColor * b, 1 - t );
    glEnable( GL_BLEND ); // Turn Blending On
    // Blending Function For Translucency Based On Source Alpha Value
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    onRender();

    //disable the transparency
    glDisable( GL_BLEND );      // Turn Blending Off
    glDisable( GL_TEXTURE_2D ); // Turn the 2D texture off
    // restore previous transformation matrix
    glPopMatrix();
  }
}

void
sceneObj::onInitialize()
{
  mTextureHandle = buffers::loadTexture(mTextureFile);
}

void
sceneObj::onCleanup()
{
  mTextureHandle = 0;
  buffers::releaseTexture(mTextureFile);
}

