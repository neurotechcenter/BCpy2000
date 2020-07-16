///////////////////////////////////////////////////////////////////////////////////////
// $Id: twoDOverlay.h 3798 2012-02-01 18:07:06Z mellinger $
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
#ifndef TWO_D_OVERLAY_H
#define TWO_D_OVERLAY_H

#include "glheaders.h"
#include "primObj.h"

class twoDOverlay : public overlayObj
{
private:
        GLuint                  mPictureTexture;
        GLuint                  mTranslucencyTexture;
        std::string             mPictureFile;      //overlay picture file name
        std::string             mTranslucencyFile; //overlay transparency file name
        float                   mBrightness;

        static void drawQuad( const int* inRect );

public:
/**************************************CONTRUCTRO******************************/
        twoDOverlay(Scene& inScene):overlayObj( inScene ),
          mPictureTexture(0),mTranslucencyTexture(0),mBrightness(1)
          {setDrawingOrder(1);}
/***********************************Modifier***********************************/
        void setPicture( const std::string& pic )      {mPictureFile=pic;}
        void setTranslucency( const std::string& tra ) {mTranslucencyFile=tra;}
        void setBrightness(float bri)                  {mBrightness=bri;}

/***************************Accessor*******************************************/
        const std::string& getPicture() const          {return mPictureFile;}
        const std::string& getTranslucency() const     {return mTranslucencyFile;}
        float getBrightness() const                    {return mBrightness;}

protected:
        void onInitialize();
        void onCleanup();
        void onRender() const;
};//twoDOverlay

#endif // TWO_D_OVERLAY_H
