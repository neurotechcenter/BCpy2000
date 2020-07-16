////////////////////////////////////////////////////////////////////////////////
// $Id: Load3DS.h 3798 2012-02-01 18:07:06Z mellinger $
// Authors: shzeng, schalk@wadsworth.org
// Description: This class handles all of the loading code
//   to load .3ds file
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
#ifndef C_LOAD_3DS_H
#define C_LOAD_3DS_H

#include "component.h"

//------- The maximum amount of textures to load --------//
#define MAX_TEXTURES 100
extern unsigned int g_Texture[MAX_TEXTURES];

void Load3dsFile(t3DModel&, const std::string& fileName);
void ClearModel(t3DModel&);


/********************PRIMARY LOAD CODE FOR LOADING 3DS FILE********************/
// This class handles all of the loading code
class CLoad3DS
{
public:
       //    CLoad3DS();                     // This inits the data members

    // This is the function that you call to load the 3DS
    bool Import3DS(t3DModel& pModel,const char *strFileName);

private:
    // This reads in a string and saves it in the char array passed in
    int GetString(char *readin);

    // This reads the next chunk
    void ReadChunk(tChunk& myChunk);

    // This reads the next large chunk
    void ProcessNextChunk(t3DModel& pModel, tChunk&);

    // This reads the object chunks
    void ProcessNextObjectChunk(t3DModel& pModel, t3DObject& pObject, tChunk&);

    // This reads the material chunks
    void ProcessNextMaterialChunk(t3DModel& pModel, tChunk&);

    // This reads the RGB value for the object's color
    void ReadColorChunk(tMaterialInfo& pMaterial, tChunk& pChunk);

    // This reads the objects vertices
    void ReadVertices(t3DObject& pObject, tChunk&);

    // This reads the objects face information
    void ReadVertexIndices(t3DObject& pObject, tChunk&);

    // This reads the texture coodinates of the object
    void ReadUVCoordinates(t3DObject& pObject, tChunk&);

    // This reads in the material name assigned to the object and sets the materialID
    void ReadObjectMaterial(t3DModel& pModel, t3DObject& pObject, tChunk& pPreviousChunk);

    // This computes the vertex normals for the object (used for lighting)
    void ComputeNormals(t3DModel& pModel);

    // This frees memory and closes the file
    void CleanUp();

    // The file pointer
    std::FILE *m_FilePointer;
};


#endif // C_LOAD_3DS_H
