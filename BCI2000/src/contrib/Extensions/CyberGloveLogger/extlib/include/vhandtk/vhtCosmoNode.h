#ifndef _VHT_COSMONODE_H_
#define _VHT_COSMONODE_H_
/********************************************************************
 FILE: $Id: vhtCosmoNode.h,v 1.6 2002/08/28 23:16:10 jsilver Exp $
 AUTHOR: Hugo DesRosiers.
 DATE: March 10th, 1999.

 DESCRIPTION: Specialization of vhtDataNode for CosmoCode (VRML) scene graphs.

 HISTORY:
 
           -- COPYRIGHT VIRTUAL TECHNOLOGIES, INC. 1998 --
********************************************************************/


#if defined(BUILD_WIN32_VH_DLL)

#if defined __BUILD_VH_COSMO_SUPPORT__
#define VH_COSMO_DLL_EXPORT __declspec(dllexport)
#else
#define VH_COSMO_DLL_EXPORT __declspec(dllimport)
#endif

#else 

#define VH_COSMO_DLL_EXPORT

#endif


#include <vhtDataNode.h>

class csNode;

//: A neutral cosmo node pointer.
// An implementation of the neutral scene graph specialized for
// the Cosmo/Optimizer rendering infrastructure.
//!vhtmodule: Core
class VH_COSMO_DLL_EXPORT vhtCosmoNode : public vhtDataNode {

	protected:
		csNode *cosmoDual;
		//: Cosmo node dual to this.
	
	public:
		vhtCosmoNode(void);
		//: Construct a null neutral node.
		vhtCosmoNode(vhtDataNode *aParent);
		//: Construct a node with given parent.
		//!param: aParent - Parent node.

		virtual ~vhtCosmoNode(void);
		//: Destruct.

		inline csNode *getCosmo(void) { return cosmoDual; }
		//: Get the associated cosmo node.
		inline void setCosmo(csNode *aCosmo) { cosmoDual= aCosmo; }
		//: Set the associated cosmo node.
		//!param: aCosmo - Associated cosmo node.
};

#endif		/* _VHT_COSMONODE_H_ */


