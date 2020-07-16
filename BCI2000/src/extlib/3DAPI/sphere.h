////////////////////////////////////////////////////////////////////////////////
// $Id: sphere.h 4924 2015-07-28 12:30:32Z mellinger $
// Authors: shzeng, schalk@wadsworth.org
// Description: A 3D sphere.
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
#ifndef SPHERE_H
#define SPHERE_H

#include "primObj.h"

class sphere: public sceneObj
{
private:
        GLfloat mRadius;      //radius of the sphere

public:
        enum { primitiveID = 1 };
/*******************************CONSTRUCTOR************************************/
        sphere(Scene& inScene, GLfloat x=0, GLfloat y=0, GLfloat z=0, GLfloat rad=1)
          :sceneObj(inScene), mRadius(rad) {setOrigin(x,y,z);}

        void setRadius( GLfloat rad )                   {mRadius = rad;}
        GLfloat getRadius() const                       {return mRadius;}

        virtual testResult isConvex() const override { return true_; }
        virtual testResult contains( const CVector3& p ) const override;
        virtual testResult intersects( const Primitive& ) const override;
        virtual testResult intersectsSurface( const geomObj& ) const override;

protected:
        virtual void onRender() const;
};//sphere

#endif // SPHERE_H
