#ifndef _VHT_COSMOPARSER_H_
#define _VHT_COSMOPARSER_H_
/********************************************************************
 FILE: $Id: vhtCosmoParser.h,v 1.16 2002/08/28 23:16:10 jsilver Exp $
 AUTHOR: Hugo DesRosiers.
 DATE: March 9th, 1999.

 DESCRIPTION: Data importer for the CosmoCode scenes (vrml 2).

 HISTORY:
 
           -- COPYRIGHT VIRTUAL TECHNOLOGIES, INC. 1998 --
********************************************************************/

#include <Optimizer/opTraverse.h>
#include <Cosmo3D/csNode.h>
#if defined( _WIN32 )
#include <stack>
using std::stack;
#else
#include <stack.h>
#endif


#if defined(BUILD_WIN32_VH_DLL)

#if defined __BUILD_VH_COSMO_SUPPORT__
#define VH_COSMO_DLL_EXPORT __declspec(dllexport)
#else
#define VH_COSMO_DLL_EXPORT __declspec(dllimport)
#endif

#else 

#define VH_COSMO_DLL_EXPORT

#endif

#include <vhtNodeParser.h>

class vhtComponent;
class vhtShape3D;
class vhtTransformGroup;

class vhtTransformGroup;
class opDFTravAction;
class vhtCosmoNode;
class vhtGroup;
class vhtNode;


//: A scene graph parser for cosmo scene graphs.
// This parser generates both the neutral dual graph 
// and the haptic scene graph associated with a given
// cosmo scene graph.
//!vhtmodule: Core
class VH_COSMO_DLL_EXPORT vhtCosmoParser : public vhtNodeParser {
	friend class vhtDFTraverser;

	protected:	/* Instance variables accessible by subclasses. */
		vhtComponent	*m_sgRoot;
		//: Root of the generated haptic scene graph.

		double			 m_scale;
		//: Global scaling factor.
		opDFTravAction		*traverser;
		//: Cosmo scene graph traversal class.
		vhtCosmoNode		*currentDataNode;
		//: Traversal state storage.
		vhtCosmoNode		*initialNode;
		//: The first cosmo node.

		bool                         nodeAdded;
		//: True if preNode added a new node to the tree.

	public:    /* Instance methods. */
		vhtCosmoParser(void);
		//: Construct a cosmo scene graph parser.
		virtual ~vhtCosmoParser(void);
		//: Destruct.
		inline void scaleBy(double aScale) { m_scale= aScale; }
		//: Set the global scaling.
		virtual vhtDataNode *descend(void *someData);
		//: Parse the cosmo graph.
};

//: Specialized optimizer traversal class.
// This class traverses a cosmo scene graph, using the
// optimizer depth first mechanism.  For each node, a corresponding
// data neutral node, and a haptic node is created.
// Most types of VRML geometry are supported, including
// primitives.  Generic geometry classes are converted
// into vhtVertexGeometry.
class vhtDFTraverser : public opDFTravAction
{
  protected:
    vhtCosmoParser *owner;
    //: The parser which is traversing.
    csType *shapeClass, *geoSetClass, *groupClass, *transformClass;
    //: Cached type info.
    stack<vhtNode *>            fatherStack;
    //: A traversal stack.

  public:
    vhtDFTraverser(vhtCosmoParser *myOwner);
    //: Construct a traversal class with given owner.
    virtual void begin( csNode *&, const opActionInfo & );
    //: Begin the traversal.

    virtual opTravDisp preNode( csNode *&, const opActionInfo & );
    //: Before a node is traversed.
    virtual opTravDisp postNode( csNode *&, const opActionInfo & );
    //: After a node is traversed.
    virtual opActionDisp end(csNode*&, const opActionInfo&);
    //: After tree is traversed.

    virtual vhtShape3D         *createGeometryNode( csNode *node );
    //: Create a generic geometry node.
    virtual vhtShape3D         *createSphereNode( double radius );
    //: Create a sphere.
    virtual vhtShape3D         *createBoxNode( csVec3f &size );
    //: Create a parallelpiped.
    virtual vhtComponent       *createComponent( csNode *node );
    //: Create a component group.
    virtual vhtTransformGroup  *createTransformGroup( csNode *node );
    //: Create a transform group.
};



#endif		/* _VHT_COSMOPARSER_H_ */


