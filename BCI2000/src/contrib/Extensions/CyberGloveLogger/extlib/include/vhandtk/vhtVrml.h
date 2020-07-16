#ifndef _VHT_VRML_IMPLEMENTATION_H_
#define _VHT_VRML_IMPLEMENTATION_H_


/********************************************************************
 FILE: $Id: vhtVrml.h,v 1.11 2003/06/03 22:48:29 jsilver Exp $
 AUTHOR: Hugo DesRosiers.
 DATE: 1999/03/15.

 DESCRIPTION: Minimal definition of a VRML model in C++, for the
	visual geometry of the HumanHand.

 HISTORY:
 
           -- COPYRIGHT VIRTUAL TECHNOLOGIES, INC. 1998 --
********************************************************************/

#if defined(BUILD_WIN32_VH_DLL)

#if defined __BUILD_VH_CORE_DLL__
#define VH_CORE_DLL_EXPORT __declspec(dllexport)
#else
#define VH_CORE_DLL_EXPORT __declspec(dllimport)
#endif

#else 

#define VH_CORE_DLL_EXPORT

#endif

#include <vhtTransform3D.h>

class vhtVrGroup;
class vhtVrShape;
class vhtVrIndexedFaceSet;
class vhtVrAppearance;
class vhtVrMaterial;
class vhtVrCoordinate;


/*****************************
* Class: vhtNodeOptimizers.
* Super: None.
* Desc: Abstract class for inserting optimizing elements
*	in standard VRML nodes (for rendering).
******************************/


class vhtVrContext;
class vhtNodeOptimizers;

//: Root class of VTi's minimal VRML implementation.
// In VTi's minimal implementation of VRML, all nodes are
// derived from the vhtVrSFNode.  This class represents a
// generic set of capabilities we provide for any VRML node.
//!vhtmodule: Core
class VH_CORE_DLL_EXPORT vhtVrSFNode {
	public:

		enum VrmlClasses {
			group, 
			shape, 
			indexedFaceSet, 
			coordinate, 
			appearance, 
			material, 
			transform, 
			unknown
		};
		//: Pre-defined set of node types that we support.

	protected:	/* Instance variables. */
		vhtVrSFNode *parent;
		//: Parent node in a VRML graph.

	public:		/* Instance methode. */
		/* Constructors. */
		vhtVrSFNode(void);
		//: Creates an empty instance.

		virtual VrmlClasses getNodeClass(void) 
		{ 
			return unknown;
		}
		//: Returns the pre-defined VRML node type of the instance.

		virtual void render(vhtVrContext *aContext);
		//: Performs the visual rendering of the instance.

		inline void setParent(vhtVrSFNode *aNode) 
		{ 
			parent= aNode; 
		}
		//: Defines the parent of the node in the VRML graph.

		inline vhtVrSFNode *getParent(void) 
		{ 
			return parent; 
		}
	//: Returns the parent of the node in the VRML graph.
};


//: Minimal implementation of the GROUP node.
// A GROUP node is simply a container that has some other
// nodes as its children.
//!vhtmodule: Core
class VH_CORE_DLL_EXPORT vhtVrGroup : public vhtVrSFNode {

	protected:	/* Instance variables. */

		unsigned int nbrChildren;
		//: Number of children contained by this node.

		unsigned int arraySize;
		//: Maximum number of children that can fit in the children array.

		vhtVrSFNode **children;
		//: Array of children nodes.

	public:		/* Instance methods. */
		/* Constructor. */
		vhtVrGroup(void);
		//: Constructs an empty GROUP node.

		/* Destructor. */
		virtual ~vhtVrGroup(void);
		//: Deletes the node and all its children.

		virtual VrmlClasses getNodeClass(void);
		//: Returns the pre-defined type <em>group<\em>.

		virtual void render(vhtVrContext *aContext);
		//: Renders the node by passing the render message to all its children.

		virtual void addChild(vhtVrSFNode *aNode);
		//: Add a new child.
		// The new child is added as the last element of the array.

		virtual vhtVrSFNode *getChild(unsigned int anIndex);
		//: Returns a child with a given index.
		// The index runs from 0, and the method returns NULL if the index
		// is greater than the number of children contained by the node.
};


//: Minimal implementation of the SHAPE node.
// A SHAPE node is a leaf of a VRML graph.  Its visual representation
// is given by a geometry node (a sphere, a box, a polyhedron, etc).
//!vhtmodule: Core
class VH_CORE_DLL_EXPORT vhtVrShape : public vhtVrSFNode {

	protected:	/* Instance variables. */
	
		vhtVrIndexedFaceSet *geometry;
		//: The visual geometry of the node.  Only IndexedFaceSet nodes are currently supported.

		vhtVrAppearance *appearance;
		//: The visual attributes to apply to the geometry.

	public:		/* Instance methods. */
		vhtVrShape(vhtVrIndexedFaceSet *aGeometry, vhtVrAppearance *anAppear);
		//: Constructs a node with a given geometry and appearance.

		virtual ~vhtVrShape(void);
		//: Destroys the node, and the geometry and appearance it contains.

		inline vhtVrIndexedFaceSet *getGeometry(void) 
		{ 
			return geometry; 
		}
		//: Returns the geometry of the shape.

		virtual VrmlClasses getNodeClass(void);
		//: Returns the pre-defined type 'geometry'.

		virtual void render(vhtVrContext *aContext);
		//: Provides a visual rendering of the shape.
};


//: Minimal implementation of the INDEXED FACE SET node.
// An IndexedFaceSet represents a polyhedron geometry.  It is used
// in Shape nodes.
//!vhtmodule: Core
class VH_CORE_DLL_EXPORT vhtVrIndexedFaceSet : public vhtVrSFNode {

	protected:	/* Instance variables. */

		vhtVrCoordinate *coord;
		//: Coordinates of the polyhedron.

		bool solid;
		//: Specifies if the polyhedron is solid (true) or hollow (false).

		double creaseAngle;
		//: Maximum angle for which adjacent faces are going to be smooth-shaded by default.

		vhtNodeOptimizers *optimizer;
		//: Internal representation of the polyhedron. optimized for drawing.

	public:		/* Instance methods. */

		vhtVrIndexedFaceSet(vhtVrCoordinate *someCoord);
		//: Constructs an instance with given coordinates.

		 virtual ~vhtVrIndexedFaceSet(void);
		//: Destroys the instance and its Coordinate node.

		 virtual VrmlClasses getNodeClass(void);
		//: Returns the pre-defined type 'indexedFaceSet'.

		 inline void setSolid(bool aFlag) { solid= aFlag; }
		//: Defines the solid property.

		 inline void setCreaseAngle(double anAngle) { creaseAngle= anAngle; }
		//: Sets the crease angle.

		 inline vhtNodeOptimizers *getOptimizers(void) { return optimizer; }
		//: Returns the optimizer object for the node.

		 inline void setOptimizers(vhtNodeOptimizers *anOptim) { optimizer= anOptim; }
		//: Assigns the optimizer object for the node.

		 inline vhtVrCoordinate *getCoordinates(void) { return coord; }
		//: Returns the Coordinate node used by the instance.
};


//: Minimal implementation of the COORDINATE node.
// The Coordinate node holds points in 3d space and indices that specify
// how these points are linked to form faces of a polyhedron.
//!vhtmodule: Core
class VH_CORE_DLL_EXPORT vhtVrCoordinate : public vhtVrSFNode {

	public:		/* Instance variables. */

		unsigned int nbrPoints;
		//: Number of points (x-y-z values) in this instance.

		double (*point)[3];
		//: Array of triples, double precision.

		unsigned int nbrIndices;
		//: Number of indices that are in this instance.

		int *coordIndex;
		//: Array of indicies, with valid values between -1 and 2^31-1.

	public:		/* Instance methods. */
	
		/* Constructor. */
		 vhtVrCoordinate(double *points, unsigned int dblCount, int *indices, unsigned int intCount);

		//: Creates a new instance with given points and indices.
		// The array that are provided as arguments are duplicated internally, so they can be
		// safely deleted afterward without affected the instance.

		 virtual ~vhtVrCoordinate(void);
		//: Destroys the instance.

		 virtual VrmlClasses getNodeClass(void);
		//: Returns the pre-defined type 'coordinate'.
};


//: Minimal implementation of the APPEARANCE node.
// The Appearance node contains material and texture information
// and it is used by the Shape node. The current implementation only
// supports the material specification.
//!vhtmodule: Core
class VH_CORE_DLL_EXPORT vhtVrAppearance : public vhtVrSFNode {

	protected:		/* Instance variables. */
	
		vhtVrMaterial *material;
		//: The material definition of the appearance.

	public:			/* Instance methods. */
	
		/* Constructor. */
		vhtVrAppearance(vhtVrMaterial *aMaterial);
		//: Constructs a new instance with a given material.

		/* Destructor. */
		virtual ~vhtVrAppearance(void);
		//: Destroys the instance and its associated material.

		virtual VrmlClasses getNodeClass(void);
		//: Returns the pre-defined type 'appearance'.
};


//: Minimal implementation of the MATERIAL node.
// The Material node provides color parameters for the
// miscellaneous lighting models defined by VRML.
//!vhtmodule: Core
class VH_CORE_DLL_EXPORT vhtVrMaterial : public vhtVrSFNode {

	protected:		/* Instance variables. */

		double ambientIntensity;
		//: Amount of light reflected by ambient lights.

		double diffuseColor[3];
		//: Color reflection from light based on angle w/r to light source.

		double specularColor[3];
		//: Specular highlights (shiny spots) colors.

		double emissiveColor[3];
		//: Color emitted by the shape.

		double shininess;
		//: Specular highlights (shiny spots) shininess.

		double transparency;
		//: Amount of light that can go through the material (0.0 is opaque).

	public:	 /* Instance methods. */
		/* Constructor. */
		vhtVrMaterial(double ambient, double *diffuse, double *specular, double *emissive, double shine, double transpa);
		//: Constructs an instance with given lighting parameters.

		/* Destructor. */
		virtual ~vhtVrMaterial(void);
		//: Destroys the instance.

		virtual VrmlClasses getNodeClass(void);
		//: Returns the pre-defined type 'material'.
};


//: Minimal implementation of the TRANSFORM node.
// Transformations that can be specified are translation from current origin,
// scaling of coordinates, rotation about a given point.
//!vhtmodule: Core
class VH_CORE_DLL_EXPORT vhtVrTransform : public vhtVrSFNode {

	protected:	/* Instance variables. */
		unsigned int nbrChildren;
		//: Number of children nodes contained by the instance.

		unsigned int arraySize;
		//: Maximum number of children that can currently fit in the array.

		vhtVrSFNode **children;
		//: Array of children nodes.

		bool isDirty;
		//: Optimization flag indicating if the instance needs some internal updating.

		vhtTransform3D fastXform;
		//: Internal date for speeding up matrical computations.

	public:		/* Instance variables. */
		double center[3];
		//: Center of rotation.

		double rotation[4];
		//: Rotation (x-y-z axis + rotation angle).

		double scale[3];
		//: Scaling factor for all children coordinates (x-y-z values).

		double scaleOrientation[4];
		//: Rotation to apply before scaling (x-y-z axis + rotation angle).

		double translation[3];
		//: Offsets to apply to children (on x-y-z).


	protected:	/* Instance methods. */

		void updateFastXform(void);
		//: Performs an update of the optimized matricial data.


	public:		/* Instance methods. */
		
		/* Constructor. */
		vhtVrTransform(void);

		//: Constructs a standard new object.
		//	All data fields are initialized to their default '0' values.
		
		/* Destructor. */
		~vhtVrTransform(void);
		//: Destroys the instance and all its children.

		virtual VrmlClasses getNodeClass(void);
		//: Returns the pre-defined type 'transform'.

		virtual void render(vhtVrContext *aContext);
		//: Provides a visual rendering of all children of the node.

		virtual void addChild(vhtVrSFNode *aNode);
		//: Adds a new child to the node.
		// The new child is added as the last element of the array.

		virtual vhtVrSFNode *getChild(unsigned int anIndex);
		//: Returns a child with a given index.
		// The index runs from 0, and the method returns NULL if the index
		// is greater than the number of children contained by the node.

		inline void setCenter(double cx, double cy, double cz)
		{ 
			center[0]= cx; 
			center[1]= cy; 
			center[2]= cz; 
			isDirty= true; 
		}
		//: Defines the center of rotation.

		inline void setRotation(double rx, double ry, double rz, double angle)
		{ 
			rotation[0]= rx; 
			rotation[1]= ry; 
			rotation[2]= rz; 
			rotation[2]= angle; 
			isDirty= true; 
		}
		//: Defines the rotation values.

		inline void setScale(double sx, double sy, double sz)
		{ 
			scale[0]= sx; 
			scale[1]= sy; 
			scale[2]= sz; 
			isDirty= true; 
		}
		//: Defines the scaling values.

		inline void setTranslation(double tx, double ty, double tz)
		{ 
			translation[0]= tx; 
			translation[1]= ty; 
			translation[2]= tz; 
			isDirty= true; 
		}
		//: Defines the translation values.

		inline void setScaleOrientation(double rx, double ry, double rz, double angle)
		{ 
			scaleOrientation[0]= rx; 
			scaleOrientation[1]= ry; 
			scaleOrientation[2]= rz;
			scaleOrientation[2]= angle; 
			isDirty= true; 
		}
		//: Defines the pre-scaling rotation values.

		inline vhtTransform3D *getFastXform(void)
		{ 
			if (isDirty) { 
				updateFastXform(); 
				isDirty = false; 
			}

			return &fastXform;
		}
		//: Returns an optimized representation of the transform for matricial operations.

		inline void setFastXform(vhtTransform3D *aXform) 
		{ 
			fastXform = *aXform; 
			isDirty   = false; 
		}
		//: Defines the optimized representation of the transform.
};


#endif		/* _VHT_VRML_IMPLEMENTATION_H_ */



