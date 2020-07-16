////////////////////////////////////////////////////////////////////////////////
// $Id: component.h 5494 2016-08-05 19:24:21Z mellinger $
// Authors: shzeng, schalk@wadsworth.org
// Description: This is the object classes that store all the data structure
//   needed to load .3ds file
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
#ifndef COMPONENT_H
#define COMPONENT_H

#include "glheaders.h"

inline float clamp( float f )
  { return std::max<float>( 0, std::min<float>( 1, f ) ); }


/*************************CHUNK OF THE 3DS FILE********************************/
// This holds the chunk info
struct tChunk
{
    unsigned short int ID;                  // The chunk's ID
    unsigned int length;                    // The length of the chunk
    unsigned int bytesRead;                 // The amount of bytes read within that chunk
};


/********************************3D POINT CLASS********************************/
// This is our 3D point class.  This will be used to store the vertices of our model.
class CVector3
{
public:
    float x, y, z;
    float* data() { return &x; }
    const float* data() const { return &x; }
    float sqLength() const { return x*x + y*y + z*z; }
};

inline CVector3 operator+( const CVector3& v1, const CVector3& v2 )
  { CVector3 v = { v1.x + v2.x, v1.y + v2.y, v1.z + v2.z }; return v; }
inline CVector3 operator-( const CVector3& v1, const CVector3& v2 )
  { CVector3 v = { v1.x - v2.x, v1.y - v2.y, v1.z - v2.z }; return v; }
inline CVector3 operator+( const CVector3& v )
  { return v; }
inline CVector3 operator-( const CVector3& v )
  { CVector3 r = { -v.x, -v.y, -v.z }; return r; }
inline float operator*( const CVector3& v1, const CVector3& v2 )
  { return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z; }
inline CVector3 operator*( const CVector3& v, float s )
  { CVector3 r = { s * v.x, s * v.y, s * v.z }; return r; }
inline CVector3 operator/( const CVector3& v, float s )
  { CVector3 r = { v.x / s, v.y / s, v.z / s }; return r; }
inline CVector3 operator*( float s, const CVector3& v )
  { return v * s; }
inline float Length( const CVector3& v )
  { return std::sqrt( v.sqLength() ); }
inline CVector3 CrossProd( const CVector3& u, const CVector3& v )
  { CVector3 r = { u.y*v.z-u.z*v.y, u.z*v.x-u.x*v.z, u.x*v.y-u.y*v.x }; return r; }

/********************************2D POINT CLASS********************************/
// This is our 2D point class.  This will be used to store the UV coordinates.
class CVector2
{
public:
    float x, y;
};


/*******************************FACE STRUCTURE*********************************/
// This is our face structure.  This is is used for indexing into the vertex
// and texture coordinate arrays.  From this information we know which vertices
// from our vertex array go to which face, along with the correct texture coordinates.
struct tFace
{
    tFace()
    {
      vertIndex[0] = vertIndex[1] = vertIndex[2]
      = coordIndex[0] = coordIndex[1] = coordIndex[2]
      = 0;
    }
    int vertIndex[3];           // indices for the verts that make up this triangle
    int coordIndex[3];          // indices for the tex coords to texture this face
};

/***************************MATERIALINFO***************************************/
// This holds the information for a material.  It may be a texture map of a color.
// Some of these are not used, but I left them because you will want to eventually
// read in the UV tile ratio and the UV tile offset for some models.
struct tMaterialInfo
{
    tMaterialInfo()
      : textureId( 0 ), uTile( 0 ), vTile( 0 ), uOffset( 0 ), vOffset( 0 )
      { strName[ 0 ] = '\0'; strFile[ 0 ] = '\0'; color[0]=color[1]=color[2]=0; }
    char  strName[255];         // The texture name
    char  strFile[255];         // The texture file name (If this is set it's a texture map)
    unsigned char  color[3];    // The color of the object (R, G, B)
    int   textureId;            // the texture ID
    float uTile;                // u tiling of texture  (Currently not used)
    float vTile;                // v tiling of texture  (Currently not used)
    float uOffset;              // u offset of texture  (Currently not used)
    float vOffset;              // v offset of texture  (Currently not used)
} ;


/*******************************OBJECT3D***************************************/
// This holds all the information for our model/scene.
// You should eventually turn into a robust class that
// has loading/drawing/querying functions like:
// LoadModel(...); DrawObject(...); DrawModel(...); DestroyModel(...);
struct t3DObject
{
    t3DObject()
      : numOfVerts( 0 ), numOfFaces( 0 ), numTexVertex( 0 ),
        materialID( 0 ), bHasTexture( false ),
        pVerts( NULL ), pNormals( NULL ), pTexVerts( NULL ), pFaces( NULL )
      { strName[ 0 ] = '\0'; }
    int  numOfVerts;            // The number of verts in the model
    int  numOfFaces;            // The number of faces in the model
    int  numTexVertex;          // The number of texture coordinates
    int  materialID;            // The texture ID to use, which is the index into our texture array
    bool bHasTexture;           // This is TRUE if there is a texture map for this object
    char strName[255];          // The name of the object
    CVector3  *pVerts;          // The object's vertices
    CVector3  *pNormals;        // The object's normals
    CVector2  *pTexVerts;       // The texture's UV coordinates
    tFace *pFaces;              // The faces information of the object
};

/***********************************MODEL3D************************************/
// This holds our model information.
struct t3DModel
{
    std::vector<tMaterialInfo> materials;   // The list of material information (Textures and colors)
    std::vector<t3DObject> objects;          // The object list for our model
};

#endif // COMPONENT_H
