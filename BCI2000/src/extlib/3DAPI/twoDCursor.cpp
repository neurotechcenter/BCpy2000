///////////////////////////////////////////////////////////////////////////////////////
// $Id: twoDCursor.cpp 3798 2012-02-01 18:07:06Z mellinger $
// Authors: shzeng, schalk@wadsworth.org
// Description: A 2D cursor displayed on top of the 2D overlay.
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

#include "twoDCursor.h"
#include <cmath>

using namespace std;

const int cNumVertices = 35;

void
twoDCursor::onRender() const
{
  glDisable( GL_TEXTURE_2D );
  glTranslatef( mOrigin.x, mOrigin.y, 0 );
  glColor3f( mRColor, mGColor, mBColor );
  glBegin( GL_POLYGON );
  for( int i = 0; i < cNumVertices; ++i )
  {
      float angle = i * 2 * M_PI / cNumVertices,
            x = mRadius * ::cos( angle ),
            y = mRadius * ::sin( angle );
      glVertex2f( x, y );
  }//for
  glEnd();
}


