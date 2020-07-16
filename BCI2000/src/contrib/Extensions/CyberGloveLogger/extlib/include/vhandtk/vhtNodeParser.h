#ifndef _VHT_NODEPARSER_H_
#define _VHT_NODEPARSER_H_
/********************************************************************
 FILE: $Id: vhtNodeParser.h,v 1.6 2002/08/28 23:16:19 jsilver Exp $
 AUTHOR: Hugo DesRosiers.
 DATE: March 9th, 1999.

 DESCRIPTION: Global easy-to-use environment for VHT visualization
	environment.

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


class vhtDataNode;


/**************************
* Class: vhtNodeParser.
* Super-class: None.
* Desc:
* Methods:
*	- preProcess: void * => void *: takes the raw data and does an initial processing on it.
*				The processed data is returned by the function.
*	- postProcess: void * x vhtNodeData * => vhtNodeData *: takes the data and the dual graph
*				and do any additional processing on the pair, returning the final form of the
*				dual graph.
*	- descend: void * => vhtNodeData *: takes the raw data and produce a dual graph in
*				vhtNodeData format.
***************************/


//: Generic parser to import scene graphs into the VHT environment.
// A node parser is a generic approach in the VHT to read a scene graph
// (normally, a visual scene) from an external format and create an 
//	haptic equivalent.  Further more, the parser must generate a data-neutral
// graph provide the links between the visual and the haptic nodes.
//!vhtmodule: Core
class VH_CORE_DLL_EXPORT vhtNodeParser {
	public:
		enum ParseFlags {
			ready, 
			finished, 
			aborted
		};
		//: The states of a parser.
  
		enum ParseResult {
			prContinue, 
			prAbort, 
			prRetry, 
			prFinished
		};
		//: Operation states that occur during the descent.
  
	protected:
		ParseFlags status;
		//: Current state of the parser.

	public:    /* Instance methods. */
		/* Constructors. */
		vhtNodeParser(void);
		//: Constructs an empty instance.
  
		/* Destructors. */
		virtual ~vhtNodeParser(void);
		//: Destroys the instance.

		virtual vhtDataNode *parse(void *aNode);
		//: Initiates the descent into the external graph.
		//!param: aNode - Root of subgraph to parse.
		// The parse method takes an unspecified type of data (subclass specific),
		// and produces the equivalent data-neutral subgraph.  The haptic nodes
		// are embedded in the data-neutral nodes (there is no need for either an
		// onto or one-to-one equivalence).

		virtual void *preProcess(void *someData);
		//: First processing sequence during the parse, before the descent.
		//!param: someData - Raw data to preprocess.
		// This method returns the processed node.

		virtual vhtDataNode *postProcess(void *someData, vhtDataNode *aTree);
		//: Last processing sequence during the parse, after the descent.
		//!param: someData - Raw data to postprocess.
		//!param: aTree - Dual graph node for someData.
		// This method returns the processed node.

		virtual vhtDataNode *descend(void *someData)= 0;
		//: The graph descent operation.
		//!param: someData - Raw data to parse.
		// Returns the root of the neutral graph.
};


#endif		/* _VHT_NODEPARSER_H_ */



