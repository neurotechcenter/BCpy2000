////////////////////////////////////////////////////////////////////////////////
// $Id: model3D.h 4924 2015-07-28 12:30:32Z mellinger $
// Authors: shzeng, schalk@wadsworth.org
// Description: A primObj representing a 3D model.
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
#ifndef MODEL_3D_H
#define MODEL_3D_H

#include "primObj.h"
#include "component.h"

class model3D : public sceneObj
{
private:
        //3ds file name
        std::string mFile;
        t3DModel mModelData;

public:
        enum { primitiveID = 9 };
        model3D( Scene& inScene, const std::string& modelFile )
          : sceneObj( inScene ), mFile( modelFile ) {}
        virtual ~model3D();
        const std::string& getModelFile() const
          { return mFile; }

        virtual void onInitialize();
        virtual void onCleanup();
        virtual void onRender() const;
};

#endif // MODEL_3D_H
