#ifndef _VHT_VRML_NODE_H_
#define _VHT_VRML_NODE_H_
/********************************************************************
 FILE: $Id: vhtVrmlNode.h,v 1.5 2002/08/28 23:16:29 jsilver Exp $
 AUTHOR: Hugo DesRosiers.

 DESCRIPTION: Extension of the vhtDataNode for handling of our minimalist
	VRML nodes.

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


#include <vhtDataNode.h>


class vhtVrSFNode;

//: Extension of the data-neutral nodes to contain VTi's generic VRML nodes.
// Following the data-neutral scene graph model, the vhtVrmlNode provides the
// dual of haptic nodes that are visually represented by VRML data.
//!vhtmodule: Core
class VH_CORE_DLL_EXPORT vhtVrmlNode : public vhtDataNode {

	protected:
		vhtVrSFNode *vrmlDual;
		//: A VRML node that is the visual dual of an haptic node.

	public:	/* Constructors. */
	
		vhtVrmlNode(void);
		//: Constructs an empty instance.
		
		vhtVrmlNode(vhtDataNode *aParent);
		//: Constructs an instance as the sub-node of an other.

				/* Destructors. */
		virtual ~vhtVrmlNode(void);
		//: Destroy the instance, but not its VRML content.

				/* Data accessors. */
		inline vhtVrSFNode *getVrml(void) 
		{ 
			return vrmlDual; 
		}
		//: Returns the associated VRML content.

		inline void setVrml(vhtVrSFNode *aVrml) 
		{ 
			vrmlDual= aVrml; 
		}
		//: Sets the associated VRML content.
};


#endif		/* _VHT_VRML_NODE_H_ */


