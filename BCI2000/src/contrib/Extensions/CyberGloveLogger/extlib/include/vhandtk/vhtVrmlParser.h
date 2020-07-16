#ifndef _VHT_VRML_PARSER_H_
#define _VHT_VRML_PARSER_H_
/********************************************************************
 FILE: $Id: vhtVrmlParser.h,v 1.5 2002/08/28 23:16:29 jsilver Exp $
 AUTHOR: Hugo DesRosiers.

 DESCRIPTION: Definition of a node parser that transforms our minimalist
	VRML support nodes into haptic scene nodes.

 HISTORY:

           -- COPYRIGHT VIRTUAL TECHNOLOGIES, INC. 1999 --
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


#include <vhtNodeParser.h>


class vhtVrSFNode;
class vhtVrmlNode;
class vhtTransformGroup;
class vhtVrmlNode;

//: A scene graph parser for VTi's generic VRML scene graphs.
// The parsing process generates a data-neutral scene graph made of
// vhtVrmlNode instances, and and the haptic scene graph that represents
// a given generic VRML scene graph.
//!vhtmodule: Core
class VH_CORE_DLL_EXPORT vhtVrmlParser : public vhtNodeParser {

	protected:
		vhtTransformGroup *m_currXFormRoot;
		//: Keeps track during the graph descent of the last transform node seen for a sub-graph.
		vhtTransformGroup *m_sgRoot;
		//: Holds the root of the generated haptic scene graph.
		vhtVrmlNode *currentDataNode;
		//: Keeps a context of the current data-neutral instance for the descent methods.
		double m_scale;
		//: Global scaling factor applied to all points specified in the VRML graph.

	public:
		
		vhtVrmlParser(void);
		//: Construct and initialize a VRML parser instance.
		
		~vhtVrmlParser(void);
		//: Destroy a VRML parser instance.

		virtual void *preProcess(void *someData);
		//: Performs an initial processing the data before the nodes start to be visited.
		
		virtual vhtDataNode *postProcess(void *someData, vhtDataNode *aTree);
		//: Process the result of the descent, after all nodes have been visited.
		virtual vhtDataNode *descend(void *someData);
		//: Initiates the descent of the VRML graph to procude the two dual graphs.

		virtual vhtDataNode *enterNode(vhtVrSFNode *currNode, vhtNodeParser::ParseResult &aResult);
		//: Called during the descent to begin the processing of a node in the graph.
		
		virtual ParseResult exitNode(vhtVrSFNode *currNode);
		//: Called during the descent to end the processing of a node in the graph.
	
};


#endif		/* _VHT_VRML_PARSER_H_ */


