///////////////////////////////////////////////////////////////////////////////////////
// $Id: twoDCursor.h 3798 2012-02-01 18:07:06Z mellinger $
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
#ifndef TWO_D_CURSOR_H
#define TWO_D_CURSOR_H

#include "glheaders.h"
#include "primObj.h"

class twoDCursor : public overlayObj
{
private:
        CVector2 mOrigin;
        float    mRadius;
        float    mRColor,
                 mGColor,
                 mBColor;

public:
/**************************************CONTRUCTRO******************************/
        twoDCursor(Scene& inScene)
          :overlayObj(inScene),mRadius(1),mRColor(0),mGColor(0),mBColor(0)
          {mOrigin.x=mOrigin.y=0;setDrawingOrder(2);}
/***********************************Modifier***********************************/
        void setRadius(float r)                  {mRadius=r;}
        void setColor(float r, float g, float b) {mRColor=r;mGColor=g;mBColor=b;}
        void setOrigin(const CVector2 v)         {mOrigin=v;}
        void setOrigin(float x, float y)         {mOrigin.x=x;mOrigin.y=y;}
/***************************Accessor*******************************************/
        float getRadius() const            {return mRadius;}
        float getRColor() const            {return mRColor;}
        float getGColor() const            {return mGColor;}
        float getBColor() const            {return mBColor;}
        const CVector2& getOrigin() const  {return mOrigin;}
        float getOriginX() const           {return mOrigin.x;}
        float getOriginY() const           {return mOrigin.y;}

protected:
        void onRender() const;

};//twoDCursor

#endif // TWO_D_CURSOR_H
