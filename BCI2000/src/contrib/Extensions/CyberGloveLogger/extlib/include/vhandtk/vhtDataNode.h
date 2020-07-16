#ifndef _VHT_DATANODE_H_
#define _VHT_DATANODE_H_

/********************************************************************
 FILE: $Id: vhtDataNode.h,v 1.13 2002/08/28 23:16:12 jsilver Exp $
 AUTHOR: Hugo DesRosiers.
 DATE: March 9th, 1999.

 DESCRIPTION: Neutral graph nodes, for managing the relations between the
	other specialized graphs (haptic, visual, etc).

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

class vhtNode;



//: Neutral graph node.
// Neutral graph nodes, for managing the relations between the
// other specialized graphs (haptic, visual, etc).  User applications
// needing to interface with a 3rd party scene graph API should subclass
// this node to map the VHS into the 3rd party API.
// The neutral scene graph provides only minimal support structure for
// management.
//!vhtmodule: Core
class VH_CORE_DLL_EXPORT vhtDataNode {
	protected:
		vhtDataNode *prev, *next;
		//: Siblings.
		vhtDataNode *parent, *children;
		//: Ancestors.
		unsigned char dataType;
		//: Node type.
		vhtNode *hapticDual;
		//: Haptic scene graph node pointer.

	public:
		vhtDataNode(void);
		//: Default neutral node.
		vhtDataNode(vhtDataNode *aParent);
		//: Neutral node with given parent.
		//!param: aParent - Parent node of this.
		vhtDataNode(vhtNode *anHaptic);
		//: Neutral node with given haptic node.
		//!param: anHaptic - Associated haptic node.

		virtual ~vhtDataNode(void);
		//: Destruct.

		/*
		  Neighbor access
		 */
		inline vhtDataNode *getPrevious(void) { return prev; }
		//: Return previous neutral node.
		// A return value of NULL indicates no more previous siblings.
		inline vhtDataNode *getNext(void) { return next; }
		//: Return next neutral node.
		// A return value of NULL indicates no more next siblings.
		inline vhtDataNode *getParent(void) { return parent; }
		//: Return parent neutral node.
		inline vhtDataNode *getChildren(void) { return children; }
		//: Return first child neutral node.

		/*
		  VHS access
		 */
		inline vhtNode *getHaptic(void) { return hapticDual; }
		//: Return corresponding haptic node.
		virtual void setHaptic(vhtNode *anHaptic);
		//: Set the corresponding haptic node.
		//!param: anHaptic - Associated haptic node.

		/*
		  Management
		 */   

		virtual void addChild(vhtDataNode *aNode);
		//: Add a neutral node child.
		//!param: aNode - Add a child node.
		virtual void linkBefore(vhtDataNode *aNode);
		//: Link this node before the indicated neutral node.
		//!param: aNode - Sibling to insert before.
};

//: Data node holder. 
// Used to implement data threads in the neutral scene graph. Node holders
// can be used to construct lists of arbitrary collections of neutral nodes
// independent of their neutral scene graph connectivity.
//!vhtmodule: Core
class VH_CORE_DLL_EXPORT vhtNodeHolder {
	protected:
		vhtNodeHolder *prev, *next;
		//: Prev, next node holders.
		vhtDataNode *data;
		//: Node that is held.
	
	public:
		vhtNodeHolder(vhtDataNode *aNode);
		//: Cconstruct a node container.
		//!param: aNode - Node to contain.
		~vhtNodeHolder(void) 
		{
			if (next != 0) next->prev = 0;
		}

		//: Construct a node holder for aNode.

		inline void linkAfter(vhtNodeHolder *aHolder)
		{
			if (aHolder != 0) {
				aHolder->next= this;
			}
	    
			this->prev= aHolder;
		}
		//: Append this node holder to aHolder.
		//!param: aHolder - List item to insert after.

		inline void linkBefore(vhtNodeHolder *aHolder)
		{
			if (aHolder != 0) {
				aHolder->prev= this;
			}

			this->next= aHolder;
		}
		//: Prepend this node holder to aHolder.
		//!param: aHolder - List item to insert before.

		inline vhtNodeHolder *getNext(void) 
		{ 
			return next; 
		}
		//: Get next node holder.

		inline vhtNodeHolder *getPrevious(void) 
		{ 
			return prev; 
		}
		//: Get previous node holder.

		inline vhtDataNode *getData(void)
		{ 
			return data; 
		}
		//: Get held data.
};


#endif		/* _VHT_DATANODE_H_ */


