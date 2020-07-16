////////////////////////////////////////////////////////////////////////////////
// $Id: Load3DS.cpp 5494 2016-08-05 19:24:21Z mellinger $
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
#include "Load3DS.h"
#include "buffers.h"
#include "FileUtils.h"
#include <fstream>

using namespace std;
/************************CHUNK ID DEFINITION***********************************/
//>------ Primary Chunk, at the beginning of each file
#define PRIMARY       0x4D4D

//>------ Main Chunks
#define OBJECTINFO    0x3D3D				// This gives the version of the mesh and is found right before the material and object information
#define VERSION       0x0002				// This gives the version of the .3ds file
#define EDITKEYFRAME  0xB000				// This is the header for all of the key frame info

//>------ sub defines of OBJECTINFO
#define MATERIAL	  0xAFFF				// This stored the texture info
#define OBJECT		  0x4000				// This stores the faces, vertices, etc...

//>------ sub defines of MATERIAL
#define MATNAME       0xA000				// This holds the material name
#define MATDIFFUSE    0xA020				// This holds the color of the object/material
#define MATMAP        0xA200				// This is a header for a new material
#define MATMAPFILE    0xA300				// This holds the file name of the texture

#define OBJECT_MESH   0x4100				// This lets us know that we are reading a new object

//>------ sub defines of OBJECT_MESH
#define OBJECT_VERTICES     0x4110			// The objects vertices
#define OBJECT_FACES		0x4120			// The objects faces
#define OBJECT_MATERIAL		0x4130			// This is found if the object has a material, either texture map or color
#define OBJECT_UV			0x4140			// The UV texture coordinates


//debug variable


//global varaibles
int gBuffer[50000] = {0};					          // This is used to read past unwanted data
unsigned int g_Texture[MAX_TEXTURES] = {0};	// This holds the texture info, referenced by an ID
CLoad3DS g_Load3ds;										// This is 3DS class.  This should go in a good model class.

static void ErrorMessage( const char* inText )
{
#if _WIN32
  ::MessageBoxA( 0, inText, inText, MB_OK );
#else
  std::cerr << inText << std::endl;
#endif
}

/*************************** IMPORT 3DS ***************************************/
//This is called by the Init Fucnction to open the .3ds file, read it, then clean up
bool CLoad3DS::Import3DS(t3DModel& outModel, const char *strFileName)
{
	char strMessage[255] = {0};
	tChunk currentChunk = {0};

	// Open the 3DS file
  std::string file = FileUtils::AbsolutePath( strFileName );
	m_FilePointer = fopen(file.c_str(), "rb");

	// Make sure we have a valid file pointer (we found the file)
	if(!m_FilePointer)
	{
		sprintf(strMessage, "Unable to find the file: %s!", strFileName);
		ErrorMessage(strMessage);
		return false;
	}

	// Once we have the file open, we need to read the very first data chunk
	// to see if it's a 3DS file.  That way we don't read an invalid file.
	// If it is a 3DS file, then the first chunk ID will be equal to PRIMARY (some hex num)

	// Read the first chuck of the file to see if it's a 3DS file
	ReadChunk(currentChunk);

	// Make sure this is a 3DS file
	if (currentChunk.ID != PRIMARY)
	{
		sprintf(strMessage, "Unable to load PRIMARY chunk from file: %s!", strFileName);
		ErrorMessage(strMessage);
		return false;
	}

  std::string wd = FileUtils::WorkingDirectory();
  FileUtils::ChangeDirectory( FileUtils::ExtractDirectory( file ) );
	// Now we actually start reading in the data.  ProcessNextChunk() is recursive

	// Begin loading objects, by calling this recursive function
	ProcessNextChunk(outModel, currentChunk);

	// After we have read the whole 3DS file, we want to calculate our own vertex normals.
	ComputeNormals(outModel);

	// Clean up after everything
	CleanUp();

  FileUtils::ChangeDirectory( wd );
	return true;
}

/******************************READ CHUNK **********************************/
//This function reads in a chunk ID and it's length in bytes
void CLoad3DS::ReadChunk(tChunk& outChunk)
{
	// This reads the chunk ID which is 2 bytes.
	// The chunk ID is like OBJECT or MATERIAL.  It tells what data is
	// able to be read in within the chunks section.
	outChunk.bytesRead = static_cast<unsigned int>( fread(&outChunk.ID, 1, 2, m_FilePointer) );

	// Then, we read the length of the chunk which is 4 bytes.
	// This is how we know how much to read in, or read past.
	outChunk.bytesRead += static_cast<unsigned int>( fread(&outChunk.length, 1, 4, m_FilePointer) );
}//read chunk


/************************PROCESS NEXT CHUNK************************************/
void CLoad3DS::ProcessNextChunk(t3DModel& outModel, tChunk& ioPreviousChunk)
{
	t3DObject newObject;					// This is used to add to our object list
	tMaterialInfo newTexture;				// This is used to add to our material list

	tChunk currentChunk = {0};					// The current chunk to load
	tChunk tempChunk = {0};						// A temp chunk for holding data

	// Below we check our chunk ID each time we read a new chunk.  Then, if
	// we want to extract the information from that chunk, we do so.
	// If we don't want a chunk, we just read past it.

	// Continue to read the sub chunks until we have reached the length.
	// After we read ANYTHING we add the bytes read to the chunk and then check
	// check against the length.
	while (ioPreviousChunk.bytesRead < ioPreviousChunk.length)
	{
		// Read next Chunk
		ReadChunk(currentChunk);

		// Check the chunk ID
		switch (currentChunk.ID)
		{
		case VERSION:							// This holds the version of the file

			// If the file was made in 3D Studio Max, this chunk has an int that
			// holds the file version.  Since there might be new additions to the 3DS file
			// format in 4.0, we give a warning to that problem.
			// However, if the file wasn't made by 3D Studio Max, we don't 100% what the
			// version length will be so we'll simply ignore the value

			// Read the file version and add the bytes read to our bytesRead variable
			currentChunk.bytesRead += static_cast<unsigned int>( fread(gBuffer, 1, currentChunk.length - currentChunk.bytesRead, m_FilePointer) );

			// If the file version is over 3, give a warning that there could be a problem
			if ((currentChunk.length - currentChunk.bytesRead == 4) && (gBuffer[0] > 0x03)) {
				const char* msg = "This 3DS file is over version 3 so it may load incorrectly";
#if _WIN32 
				MessageBox(NULL, msg, "Warning", MB_OK);
#endif
			}
			break;

		case OBJECTINFO:						// This holds the version of the mesh
			{
			// This chunk holds the version of the mesh.  It is also the head of the MATERIAL
			// and OBJECT chunks.  From here on we start reading in the material and object info.

			// Read the next chunk
			ReadChunk(tempChunk);

			// Get the version of the mesh
			tempChunk.bytesRead += static_cast<unsigned int>( fread(gBuffer, 1, tempChunk.length - tempChunk.bytesRead, m_FilePointer) );

			// Increase the bytesRead by the bytes read from the last chunk
			currentChunk.bytesRead += tempChunk.bytesRead;

			// Go to the next chunk, which is the object has a texture, it should be MATERIAL, then OBJECT.
			ProcessNextChunk(outModel, currentChunk);
			break;
		}
		case MATERIAL:							// This holds the material information

			// This chunk is the header for the material info chunks

			// Add a empty texture structure to our texture list.
			// If you are unfamiliar with STL's "vector" class, all push_back()
			// does is add a new node onto the list.  I used the vector class
			// so I didn't need to write my own link list functions.
			outModel.materials.push_back(newTexture);

			// Proceed to the material loading function
			ProcessNextMaterialChunk(outModel, currentChunk);
			break;

		case OBJECT:							// This holds the name of the object being read

			// This chunk is the header for the object info chunks.  It also
			// holds the name of the object.

			// Add a new tObject node to our list of objects (like a link list)
			outModel.objects.push_back(newObject);

			// Get the name of the object and store it, then add the read bytes to our byte counter.
			currentChunk.bytesRead += GetString(outModel.objects.rbegin()->strName);

			// Now proceed to read in the rest of the object information
			ProcessNextObjectChunk(outModel, *outModel.objects.rbegin(), currentChunk);
			break;

		case EDITKEYFRAME:

			// Because I wanted to make this a SIMPLE tutorial as possible, I did not include
			// the key frame information.  This chunk is the header for all the animation info.
			// In a later tutorial this will be the subject and explained thoroughly.

			//ProcessNextKeyFrameChunk(pModel, currentChunk);

			// Read past this chunk and add the bytes read to the byte counter
			currentChunk.bytesRead += static_cast<unsigned int>( fread(gBuffer, 1, currentChunk.length - currentChunk.bytesRead, m_FilePointer) );
			break;

		default:

			// If we didn't care about a chunk, then we get here.  We still need
			// to read past the unknown or ignored chunk and add the bytes read to the byte counter.
			currentChunk.bytesRead += static_cast<unsigned int>( fread(gBuffer, 1, currentChunk.length - currentChunk.bytesRead, m_FilePointer) );
			break;
		}

		// Add the bytes read from the last chunk to the previous chunk passed in.
		ioPreviousChunk.bytesRead += currentChunk.bytesRead;
	}
}//process next chunk


/*********************** PROCESS NEXT OBJECT CHUNK *****************************/
//This function handles all the information about the objects in the file
void CLoad3DS::ProcessNextObjectChunk(t3DModel& outModel, t3DObject& outObject, tChunk& ioPreviousChunk)
{
	// The current chunk to work with
	tChunk currentChunk = {0};

	// Continue to read these chunks until we read the end of this sub chunk
	while (ioPreviousChunk.bytesRead < ioPreviousChunk.length)
	{
		// Read the next chunk
		ReadChunk(currentChunk);

		// Check which chunk we just read
		switch (currentChunk.ID)
		{
		case OBJECT_MESH:					// This lets us know that we are reading a new object

			// We found a new object, so let's read in it's info using recursion
			ProcessNextObjectChunk(outModel, outObject, currentChunk);
			break;

		case OBJECT_VERTICES:				// This is the objects vertices
			ReadVertices(outObject, currentChunk);
			break;

		case OBJECT_FACES:					// This is the objects face information
			ReadVertexIndices(outObject, currentChunk);
			break;

		case OBJECT_MATERIAL:				// This holds the material name that the object has

			// This chunk holds the name of the material that the object has assigned to it.
			// This could either be just a color or a texture map.  This chunk also holds
			// the faces that the texture is assigned to (In the case that there is multiple
			// textures assigned to one object, or it just has a texture on a part of the object.
			// Since most of my game objects just have the texture around the whole object, and
			// they aren't multitextured, I just want the material name.

			// We now will read the name of the material assigned to this object
			ReadObjectMaterial(outModel, outObject, currentChunk);
			break;

		case OBJECT_UV:						// This holds the UV texture coordinates for the object

			// This chunk holds all of the UV coordinates for our object.  Let's read them in.
			ReadUVCoordinates(outObject, currentChunk);
			break;

		default:

			// Read past the ignored or unknown chunks
			currentChunk.bytesRead += static_cast<unsigned int>( fread(gBuffer, 1, currentChunk.length - currentChunk.bytesRead, m_FilePointer) );
			break;
		}

		// Add the bytes read from the last chunk to the previous chunk passed in.
		ioPreviousChunk.bytesRead += currentChunk.bytesRead;
	}
}//process next object chunk

/****************** PROCESS NEXT MATERIAL CHUNK *******************************/
//This function handles all the information about the material (Texture)
void CLoad3DS::ProcessNextMaterialChunk(t3DModel& outModel, tChunk& ioPreviousChunk)
{
	// The current chunk to work with
	tChunk currentChunk = {0};

	// Continue to read these chunks until we read the end of this sub chunk
	while (ioPreviousChunk.bytesRead < ioPreviousChunk.length)
	{
		// Read the next chunk
		ReadChunk(currentChunk);

		// Check which chunk we just read in
		switch (currentChunk.ID)
		{
		case MATNAME:							// This chunk holds the name of the material

			// Here we read in the material name
			currentChunk.bytesRead += static_cast<unsigned int>( fread(outModel.materials.rbegin()->strName, 1, currentChunk.length - currentChunk.bytesRead, m_FilePointer) );
			break;

		case MATDIFFUSE:						// This holds the R G B color of our object
			ReadColorChunk(*outModel.materials.rbegin(), currentChunk);
			break;

		case MATMAP:							// This is the header for the texture info

			// Proceed to read in the material information
			ProcessNextMaterialChunk(outModel, currentChunk);
			break;

		case MATMAPFILE:						// This stores the file name of the material

			// Here we read in the material's file name
			currentChunk.bytesRead += static_cast<unsigned int>( fread(outModel.materials.rbegin()->strFile, 1, currentChunk.length - currentChunk.bytesRead, m_FilePointer) );
			break;

		default:

			// Read past the ignored or unknown chunks
			currentChunk.bytesRead += static_cast<unsigned int>( fread(gBuffer, 1, currentChunk.length - currentChunk.bytesRead, m_FilePointer) );
			break;
		}

		// Add the bytes read from the last chunk to the previous chunk passed in.
		ioPreviousChunk.bytesRead += currentChunk.bytesRead;
	}
}//process next material chunk




// *Note*
//
// Below are some math functions for calculating vertex normals.  We want vertex normals
// because it makes the lighting look really smooth and life like.  You probably already
// have these functions in the rest of your engine, so you can delete these and call
// your own.  I wanted to add them so I could show how to calculate vertex normals.
// This computes the magnitude of a normal.   (magnitude = sqrt(x^2 + y^2 + z^2)
#define Mag(Normal) (sqrt(Normal.x*Normal.x + Normal.y*Normal.y + Normal.z*Normal.z))

// This calculates a vector between 2 points and returns the result
CVector3 Vector(CVector3 vPoint1, CVector3 vPoint2)
{
	CVector3 vVector;							// The variable to hold the resultant vector

	vVector.x = vPoint1.x - vPoint2.x;			// Subtract point1 and point2 x's
	vVector.y = vPoint1.y - vPoint2.y;			// Subtract point1 and point2 y's
	vVector.z = vPoint1.z - vPoint2.z;			// Subtract point1 and point2 z's

	return vVector;								// Return the resultant vector
}

// This adds 2 vectors together and returns the result
CVector3 AddVector(CVector3 vVector1, CVector3 vVector2)
{
	CVector3 vResult;							// The variable to hold the resultant vector

	vResult.x = vVector2.x + vVector1.x;		// Add Vector1 and Vector2 x's
	vResult.y = vVector2.y + vVector1.y;		// Add Vector1 and Vector2 y's
	vResult.z = vVector2.z + vVector1.z;		// Add Vector1 and Vector2 z's

	return vResult;								// Return the resultant vector
}

// This divides a vector by a single number (scalar) and returns the result
CVector3 DivideVectorByScaler(CVector3 vVector1, float Scaler)
{
	CVector3 vResult;							// The variable to hold the resultant vector

	vResult.x = vVector1.x / Scaler;			// Divide Vector1's x value by the scaler
	vResult.y = vVector1.y / Scaler;			// Divide Vector1's y value by the scaler
	vResult.z = vVector1.z / Scaler;			// Divide Vector1's z value by the scaler

	return vResult;								// Return the resultant vector
}

// This returns the cross product between 2 vectors
CVector3 Cross(CVector3 vVector1, CVector3 vVector2)
{
	CVector3 vCross;								// The vector to hold the cross product
												// Get the X value
	vCross.x = ((vVector1.y * vVector2.z) - (vVector1.z * vVector2.y));
												// Get the Y value
	vCross.y = ((vVector1.z * vVector2.x) - (vVector1.x * vVector2.z));
												// Get the Z value
	vCross.z = ((vVector1.x * vVector2.y) - (vVector1.y * vVector2.x));

	return vCross;								// Return the cross product
}

// This returns the normal of a vector
CVector3 Normalize(CVector3 vNormal)
{
	double Magnitude;							// This holds the magitude

	Magnitude = Mag(vNormal);					// Get the magnitude

	vNormal.x /= (float)Magnitude;				// Divide the vector's X by the magnitude
	vNormal.y /= (float)Magnitude;				// Divide the vector's Y by the magnitude
	vNormal.z /= (float)Magnitude;				// Divide the vector's Z by the magnitude

	return vNormal;								// Return the normal
}

/************************** COMPUTER NORMALS *************************************/
//This function computes the normals and vertex normals of the objects
void CLoad3DS::ComputeNormals(t3DModel& ioModel)
{
	CVector3 vVector1, vVector2, vNormal, vPoly[3];

	// If there are no objects, we can skip this part
	if(ioModel.objects.empty())
		return;

	// What are vertex normals?  And how are they different from other normals?
	// Well, if you find the normal to a triangle, you are finding a "Face Normal".
	// If you give OpenGL a face normal for lighting, it will make your object look
	// really flat and not very round.  If we find the normal for each vertex, it makes
	// the smooth lighting look.  This also covers up blocky looking objects and they appear
	// to have more polygons than they do.    Basically, what you do is first
	// calculate the face normals, then you take the average of all the normals around each
	// vertex.  It's just averaging.  That way you get a better approximation for that vertex.

	// Go through each of the objects to calculate their normals
	for(size_t index = 0; index < ioModel.objects.size(); index++)
	{
		// Get the current object
		t3DObject *pObject = &(ioModel.objects[index]);

		// Here we allocate all the memory we need to calculate the normals
		CVector3 *pNormals		= new CVector3 [pObject->numOfFaces];
		CVector3 *pTempNormals	= new CVector3 [pObject->numOfFaces];
		pObject->pNormals		= new CVector3 [pObject->numOfVerts];

		// Go though all of the faces of this object
		for(int i=0; i < pObject->numOfFaces; i++)
		{
			// To cut down LARGE code, we extract the 3 points of this face
			vPoly[0] = pObject->pVerts[pObject->pFaces[i].vertIndex[0]];
			vPoly[1] = pObject->pVerts[pObject->pFaces[i].vertIndex[1]];
			vPoly[2] = pObject->pVerts[pObject->pFaces[i].vertIndex[2]];

			// Now let's calculate the face normals (Get 2 vectors and find the cross product of those 2)

			vVector1 = Vector(vPoly[0], vPoly[2]);		// Get the vector of the polygon (we just need 2 sides for the normal)
			vVector2 = Vector(vPoly[2], vPoly[1]);		// Get a second vector of the polygon

			vNormal  = Cross(vVector1, vVector2);		// Return the cross product of the 2 vectors (normalize vector, but not a unit vector)
			pTempNormals[i] = vNormal;					// Save the un-normalized normal for the vertex normals
			vNormal  = Normalize(vNormal);				// Normalize the cross product to give us the polygons normal

			pNormals[i] = vNormal;						// Assign the normal to the list of normals
		}

		//////////////// Now Get The Vertex Normals /////////////////

		CVector3 vSum = {0.0, 0.0, 0.0};
		CVector3 vZero = vSum;
		int shared=0;

		for (int i = 0; i < pObject->numOfVerts; i++)			// Go through all of the vertices
		{
			for (int j = 0; j < pObject->numOfFaces; j++)	// Go through all of the triangles
			{												// Check if the vertex is shared by another face
				if (pObject->pFaces[j].vertIndex[0] == i ||
					pObject->pFaces[j].vertIndex[1] == i ||
					pObject->pFaces[j].vertIndex[2] == i)
				{
					vSum = AddVector(vSum, pTempNormals[j]);// Add the un-normalized normal of the shared face
					shared++;								// Increase the number of shared triangles
				}
			}

			// Get the normal by dividing the sum by the shared.  We negate the shared so it has the normals pointing out.
			pObject->pNormals[i] = DivideVectorByScaler(vSum, float(-shared));

			// Normalize the normal for the final vertex normal
			pObject->pNormals[i] = Normalize(pObject->pNormals[i]);

			vSum = vZero;									// Reset the sum
			shared = 0;										// Reset the shared
		}

		// Free our memory and start over on the next object
		delete [] pTempNormals;
		delete [] pNormals;
	}
} //ComputeNormals



/***************************** READ VERTICES *********************************/
//This function reads in the vertices for the object
void CLoad3DS::ReadVertices(t3DObject& outObject, tChunk& ioPreviousChunk)
{
	// Like most chunks, before we read in the actual vertices, we need
	// to find out how many there are to read in.  Once we have that number
	// we then fread() them into our vertice array.

	// Read in the number of vertices (int)
	ioPreviousChunk.bytesRead += static_cast<unsigned int>( fread(&(outObject.numOfVerts), 1, 2, m_FilePointer) );

	// Allocate the memory for the verts and initialize the structure
	outObject.pVerts = new CVector3 [outObject.numOfVerts];
	memset(outObject.pVerts, 0, sizeof(CVector3) * outObject.numOfVerts);

	// Read in the array of vertices (an array of 3 floats)
	ioPreviousChunk.bytesRead += static_cast<unsigned int>( fread(outObject.pVerts, 1, ioPreviousChunk.length - ioPreviousChunk.bytesRead, m_FilePointer) );

	// Now we should have all of the vertices read in.  Because 3D Studio Max
	// Models with the Z-Axis pointing up (strange and ugly I know!), we need
	// to flip the y values with the z values in our vertices.  That way it
	// will be normal, with Y pointing up.  If you prefer to work with Z pointing
	// up, then just delete this next loop.  Also, because we swap the Y and Z
	// we need to negate the Z to make it come out correctly.

	// Go through all of the vertices that we just read and swap the Y and Z values
	for(int i = 0; i < outObject.numOfVerts; i++)
	{
		// Store off the Y value
		float fTempY = outObject.pVerts[i].y;

		// Set the Y value to the Z value
		outObject.pVerts[i].y = outObject.pVerts[i].z;

		// Set the Z value to the Y value,
		// but negative Z because 3D Studio max does the opposite.
		outObject.pVerts[i].z = -fTempY;
	}
}// READ VERTICES









/****************************** GET STRING **********************************/
//This function reads in a string of characters
int CLoad3DS::GetString(char *pBuffer)
{
	int index = 0;

	// Read 1 byte of data which is the first letter of the string
	fread(pBuffer, 1, 1, m_FilePointer);

	// Loop until we get NULL
	while (*(pBuffer + index++) != 0) {

		// Read in a character at a time until we hit NULL.
		fread(pBuffer + index, 1, 1, m_FilePointer);
	}

	// Return the string length, which is how many bytes we read in (including the NULL)
	return static_cast<int>( strlen(pBuffer) + 1 );
}//GetString





/***************************READ COLOR *****************************************/
//T
void CLoad3DS::ReadColorChunk(tMaterialInfo& outMaterial, tChunk& ioChunk)
{
	tChunk tempChunk = {0};

	// Read the color chunk info
	ReadChunk(tempChunk);

	// Read in the R G B color (3 bytes - 0 through 255)
	tempChunk.bytesRead += static_cast<unsigned int>( fread(outMaterial.color, 1, tempChunk.length - tempChunk.bytesRead, m_FilePointer) );

	// Add the bytes read to our chunk
	ioChunk.bytesRead += tempChunk.bytesRead;
} //ReadColorChunk



/************************READ OBJECT MATERIAL ***********************************/
//	This function reads in the material name assigned to the object and sets the materialID
void CLoad3DS::ReadObjectMaterial(t3DModel& outModel, t3DObject& ioObject, tChunk& ioPreviousChunk)
{
	char strMaterial[255] = {0};			// This is used to hold the objects material name

	// *What is a material?*  - A material is either the color or the texture map of the object.
	// It can also hold other information like the brightness, shine, etc... Stuff we don't
	// really care about.  We just want the color, or the texture map file name really.

	// Here we read the material name that is assigned to the current object.
	// strMaterial should now have a string of the material name, like "Material #2" etc..
	ioPreviousChunk.bytesRead += GetString(strMaterial);

	// Now that we have a material name, we need to go through all of the materials
	// and check the name against each material.  When we find a material in our material
	// list that matches this name we just read in, then we assign the materialID
	// of the object to that material index.  You will notice that we passed in the
	// model to this function.  This is because we need the number of textures.
	// Yes though, we could have just passed in the model and not the object too.

	// Go through all of the textures
	for(size_t i = 0; i < outModel.materials.size(); i++)
	{
		// If the material we just read in matches the current texture name
		if(strcmp(strMaterial, outModel.materials[i].strName) == 0)
		{
			// Set the material ID to the current index 'i' and stop checking
			ioObject.materialID = static_cast<int>( i );

			// Now that we found the material, check if it's a texture map.
			// If the strFile has a string length of 1 and over it's a texture
			if(strlen(outModel.materials[i].strFile) > 0) {

				// Set the object's flag to say it has a texture map to bind.
				ioObject.bHasTexture = true;
			}
			break;
		}
		else
		{
			// Set the ID to -1 to show there is no material for this object
			ioObject.materialID = -1;
		}
	}

	// Read past the rest of the chunk since we don't care about shared vertices
	// You will notice we subtract the bytes already read in this chunk from the total length.
	ioPreviousChunk.bytesRead += static_cast<unsigned int>( fread(gBuffer, 1, ioPreviousChunk.length - ioPreviousChunk.bytesRead, m_FilePointer) );
}//ReadObjectMaterial



/***********************************CLEAN UP**************************************/
//This function cleans up our allocated memory and closes the file
void CLoad3DS::CleanUp()
{
	if (m_FilePointer) {
		fclose(m_FilePointer);					// Close the current file pointer
		m_FilePointer = NULL;
	}
}


/*********************** READ UV COORDINATES ***********************************/
//This function reads in the UV coordinates for the object
void CLoad3DS::ReadUVCoordinates(t3DObject& outObject, tChunk& ioPreviousChunk)
{
	// In order to read in the UV indices for the object, we need to first
	// read in the amount there are, then read them in.

	// Read in the number of UV coordinates there are (int)
	ioPreviousChunk.bytesRead += static_cast<unsigned int>( fread(&outObject.numTexVertex, 1, 2, m_FilePointer) );

	// Allocate memory to hold the UV coordinates
	outObject.pTexVerts = new CVector2 [outObject.numTexVertex];

	// Read in the texture coodinates (an array 2 float)
	ioPreviousChunk.bytesRead += static_cast<unsigned int>( fread(outObject.pTexVerts, 1, ioPreviousChunk.length - ioPreviousChunk.bytesRead, m_FilePointer) );
}//READ UV COORDINATES


/*************************READ VERTEX INDECES ********************************/
//This function reads in the indices for the vertex array
void CLoad3DS::ReadVertexIndices(t3DObject& outObject, tChunk& ioPreviousChunk)
{
	unsigned short index = 0;					// This is used to read in the current face index

	// In order to read in the vertex indices for the object, we need to first
	// read in the number of them, then read them in.  Remember,
	// we only want 3 of the 4 values read in for each face.  The fourth is
	// a visibility flag for 3D Studio Max that doesn't mean anything to us.

	// Read in the number of faces that are in this object (int)
	ioPreviousChunk.bytesRead += static_cast<unsigned int>( fread(&outObject.numOfFaces, 1, 2, m_FilePointer) );

	// Alloc enough memory for the faces and initialize the structure
	outObject.pFaces = new tFace [outObject.numOfFaces];
	memset(outObject.pFaces, 0, sizeof(tFace) * outObject.numOfFaces);

	// Go through all of the faces in this object
	for(int i = 0; i < outObject.numOfFaces; i++)
	{
		// Next, we read in the A then B then C index for the face, but ignore the 4th value.
		// The fourth value is a visibility flag for 3D Studio Max, we don't care about this.
		for(int j = 0; j < 4; j++)
		{
			// Read the first vertice index for the current face
			ioPreviousChunk.bytesRead += static_cast<unsigned int>( fread(&index, 1, sizeof(index), m_FilePointer) );

			if(j < 3)
			{
				// Store the index in our face structure.
				outObject.pFaces[i].vertIndex[j] = index;
			}
		}
	}
}//ReadVertexIndices


/******************************DECLARATION*************************************/
void Create3dsTexture(unsigned int textureArray[], const char* strFileName, int textureID);
void Load3dsFile();


/****************************** CREATE 3ds TEXTURE *********************************/
//This creates a texture in OpenGL that we can texture map
void Create3dsTexture(unsigned int textureArray[], const char* strFileName, int textureID)
{
	if(!strFileName)									// Return from the function if no file name was passed in
		return;

  std::vector<GLubyte> bmp;
  int width, height;
	if( !buffers::loadWindowsBitmap( strFileName, width, height, bmp ) )
		return;

	// Generate a texture with the associative texture ID stored in the array
	glGenTextures(1, &textureArray[textureID]);

	// This sets the alignment requirements for the start of each pixel row in memory.
	glPixelStorei (GL_UNPACK_ALIGNMENT, 1);

	// Bind the texture to the texture arrays index and init the texture
	glBindTexture(GL_TEXTURE_2D, textureArray[textureID]);

	// Build Mipmaps (builds different versions of the picture for distances - looks better)
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, bmp.data());

	// Lastly, we need to tell OpenGL the quality of our texture map.  GL_LINEAR_MIPMAP_LINEAR
	// is the smoothest.  GL_LINEAR_MIPMAP_NEAREST is faster than GL_LINEAR_MIPMAP_LINEAR,
	// but looks blochy and pixilated.  Good for slower computers though.

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);

}//create texture


/********************************LOAD 3DS FILE********************************/
//This function load the 3ds file into sturcture. Must call in the InitGL() function
void Load3dsFile(t3DModel &outModel, const string& fileName)
{
    /***************************LOAD 3DS FILE******************************/
    // First we need to actually load the .3DS file.  We just pass in an address to
	// our t3DModel structure and the file name string we want to load ("face.3ds").
    g_Load3ds.Import3DS(outModel, FileUtils::AbsolutePath( fileName ).c_str());			// Load our .3DS file into our model structure
    // Go through all the materials
    for(size_t i = 0; i < outModel.materials.size(); i++)
	{
		// Check to see if there is a file name to load in this material
		if(strlen(outModel.materials[i].strFile) > 0)
		{
			// Use the name of the texture file to load the bitmap, with a texture ID (i).
			// We pass in our global texture array, the name of the texture, and an ID to reference it.
			Create3dsTexture(g_Texture, outModel.materials[i].strFile, static_cast<int>( i ) );
		}
		// Set the texture ID for this material
		outModel.materials[i].textureId = static_cast<int>( i );
	}
}//Load3dsFile

void
ClearModel(t3DModel& ioModel)
{
  for( size_t i = 0; i < ioModel.objects.size(); ++i )
  {
    delete[] ioModel.objects[ i ].pVerts;
    delete[] ioModel.objects[ i ].pNormals;
    delete[] ioModel.objects[ i ].pTexVerts;
    delete[] ioModel.objects[ i ].pFaces;
  }
  ioModel.objects.clear();
  ioModel.materials.clear();
}

