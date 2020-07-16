///////////////////////////////////////////////////////////////////////////////////////
// $Id: threeDText.h 3798 2012-02-01 18:07:06Z mellinger $
// Authors: shzeng, schalk@wadsworth.org
// Description: This "threeDText" class resembles the 3D text in the 3D environment.
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
#ifndef THREE_D_TEXT_H
#define THREE_D_TEXT_H

#include "primObj.h"
#include <map>
#include <string>

class threeDText : public sceneObj
{
private:
        std::string mCaption,
                    mFont;
        GLfloat     mFontSize;

        CVector3    mDir;
        GLuint      mFontID;   // 3D font handle

public:
        enum { primitiveID = 4 };
        threeDText(Scene& inScene)
          :sceneObj(inScene),mFontSize(12),mFontID(0){mDir.x=mDir.y=mDir.z=0;}
        void threeDTextsetAll();

        void setFont(const std::string& f)      {mFont = f;}
        void setFontSize(GLfloat size)          {mFontSize = size;}
        void setCaption(const std::string& cap) {mCaption = cap;}

        void setDirection(GLfloat myDX, GLfloat myDY, GLfloat myDZ)
                                                {CVector3 c={myDX,myDY,myDZ}; mDir=c;}

        //GLuint getFontID() const                {return mThreeDfontID;}
        const std::string& getFont() const      {return mFont;}
        GLfloat getFontSize() const             {return mFontSize;}
        const std::string& getCaption() const   {return mCaption;}

        GLfloat getDirX()                       {return mDir.x;}
        GLfloat getDirY()                       {return mDir.y;}
        GLfloat getDirZ()                       {return mDir.z;}

protected:
        virtual void onInitialize();
        virtual void onCleanup();
        virtual void onRender() const;

};//threeDText

#endif // THREE_D_TEXT_H
