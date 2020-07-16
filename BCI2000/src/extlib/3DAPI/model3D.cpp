////////////////////////////////////////////////////////////////////////////////
// $Id: model3D.cpp 4924 2015-07-28 12:30:32Z mellinger $
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
#include "PCHIncludes.h"
#pragma hdrstop

#include "glheaders.h"
#include "model3D.h"
#include "Load3DS.h"
#include "LCRandomGenerator.h"

model3D::~model3D()
{
  ClearModel(mModelData);
}

void
model3D::onInitialize()
{
  Load3dsFile(mModelData,mFile);
   // avoid large numbers of triangles due to our inefficient intersection test
  const int trianglesLimit = 100;
  int totalTriangles = 0;
  for( const auto& obj : mModelData.objects )
    totalTriangles += obj.numOfFaces;
  int seed = 0;
  for( const auto& ch : mFile )
    seed ^= ch;
  LCRandomGenerator rg( seed );
  for( const auto& obj : mModelData.objects )
  {
    for( int i = 0; i < obj.numOfFaces; ++i )
    {
      tFace* pFace = obj.pFaces + i;
      if( rg.Random( totalTriangles ) < trianglesLimit )
        Triangles().push_back(
          Triangle(
            obj.pVerts[pFace->vertIndex[0]],
            obj.pVerts[pFace->vertIndex[1]],
            obj.pVerts[pFace->vertIndex[2]]
          )
        );
    }
  }
  sceneObj::onInitialize();
}

void
model3D::onCleanup()
{
  ClearModel(mModelData);
  sceneObj::onCleanup();
}

void
model3D::onRender() const
{
    // Since we know how many objects our model has, go through each of them.
    for (size_t i = 0; i < mModelData.objects.size(); i++)
    {
        // Make sure we have valid objects just in case. (size() is in the vector class)
        if (mModelData.objects.empty())
            break;
        // Get the current object that we are displaying
        const t3DObject *pObject = &mModelData.objects[i];
        // Check to see if this object has a texture map, if so bind the texture to it.
        if (pObject->bHasTexture)
        {
            // Turn on texture mapping and turn off color
            glEnable(GL_TEXTURE_2D);
            // Reset the color to normal again
            glColor3ub(255, 255, 255);
            // Bind the texture map to the object by it's materialID
            glBindTexture(GL_TEXTURE_2D, g_Texture[pObject->materialID]);
        }
        else
        {
            // Turn off texture mapping and turn on color
            glDisable(GL_TEXTURE_2D);
            // Reset the color to normal again
            glColor3ub(255, 255, 255);
        }
        // This determines if we are in wireframe or normal mode
        glBegin(GL_TRIANGLES);					// Begin drawing with our selected mode (triangles or lines)
        // Go through all of the faces (polygons) of the object and draw them
        for (int j = 0; j < pObject->numOfFaces; j++)
        {
            // Go through each corner of the triangle and draw it.
            for (int whichVertex = 0; whichVertex < 3; whichVertex++)
            {
                // Get the index for each point of the face
                int index = pObject->pFaces[j].vertIndex[whichVertex];
                // Give OpenGL the normal for this vertex.
                glNormal3f(pObject->pNormals[ index ].x, pObject->pNormals[ index ].y, pObject->pNormals[ index ].z);
                // If the object has a texture associated with it, give it a texture coordinate.
                if (pObject->bHasTexture)
                {
                    // Make sure there was a UVW map applied to the object or else it won't have tex coords.
                    if (pObject->pTexVerts)
                    {
                        glTexCoord2f(pObject->pTexVerts[ index ].x, pObject->pTexVerts[ index ].y);
                    }
                }
                else
                {
                    // Make sure there is a valid material/color assigned to this object.
                    // You should always at least assign a material color to an object,
                    // but just in case we want to check the size of the material list.
                    // if the size is at least one, and the material ID != -1,
                    // then we have a valid material.
                    if (mModelData.materials.size() && pObject->materialID >= 0)
                    {
                        // Get and set the color that the object is, since it must not have a texture
                        const unsigned char *pColor = mModelData.materials[pObject->materialID].color;
                        // Assign the current color to this model
                        glColor3ub(pColor[0], pColor[1], pColor[2]);
                    }
                }
                // Pass in the current vertex of the object (Corner of current face)
                glVertex3f(pObject->pVerts[ index ].x, pObject->pVerts[ index ].y, pObject->pVerts[ index ].z);
            }
        }
        glEnd();                    // End the drawing
    }  /* for*/
}



