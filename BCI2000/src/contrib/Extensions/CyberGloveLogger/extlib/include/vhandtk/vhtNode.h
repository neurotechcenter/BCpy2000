/********************************************************************
 FILE: $Id: vhtNode.h,v 1.12 2002/08/28 23:16:19 jsilver Exp $
 AUTHOR: Ron Carmel
 DATE: Aug 1998

 DESCRIPTION:
  -
 HISTORY:
  -
 NOTES:
  - when adding a child to a node, it is assumed that the new child 
    is eventually going to be a part of the scene graph.  optimizations
    are based on this fact.  if two seperate or partially overlapping 
    scene graphs are built, the vhtNode::setRoot() method should be
    called to allow maximum performance optimization.
  - the scene graph is a multiply threaded tree.  every time a node is
    added to the tree it will thread itself into the appropriate thread
    (if at all) by calling it's own *::thread() function, which it
    implements (overrides the pure virtual vhtNode::thread() method)

           -- COPYRIGHT VIRTUAL TECHNOLOGIES, INC. 1998 --
********************************************************************/

#ifndef __VHTNODE_H
#define __VHTNODE_H

#if defined(BUILD_WIN32_VH_DLL)

#if defined __BUILD_VH_CORE_DLL__
#define VH_CORE_DLL_EXPORT __declspec(dllexport)
#else
#define VH_CORE_DLL_EXPORT __declspec(dllimport)
#endif

#else 

#define VH_CORE_DLL_EXPORT

#endif

#include <vhtSceneGraphObject.h>
#include <vhtBoundingSphere.h>
#include <vhtCriticalSection.h>
#include <vhtNodeTypes.h>

class vhtNode;
class vhtEngine;
class vhtGroup;
class vhtSwitch;
class vhtTransformGroup;
class vhtTransform3D;
class vhtShape3D;
class vhtDataNode;

typedef enum {
    VHT_OPENGL,
    VHT_PERFORMER,
    VHT_FAHRENHEIT
} vhtGraphicsType;

#define VHT_SCENEGRAPH_DEFAULT_TIMESTEP 0.0025
#define VHT_MAXNAMELEN 127

/*------------------------------------------------------------------------*/

//: The base class of all haptic scene graph classes.
// Implements the basic features common to all scene graph objects.  This includes sibling
// and parent access, as well as bounding sphere and LM methods.
//!vhtmodule: Core
class VH_CORE_DLL_EXPORT vhtNode: public vhtSceneGraphObject {

    /* allow a group node access members: */
    friend class vhtGroup;
    friend class vhtTransformGroup;

    /* allow a shape3d node access: */
    friend class vhtShape3D;

    
	public:
		vhtNode(void);
		//: Default constructor.
		vhtNode(vhtNode &node);
		//: Copy constructor.
		virtual ~vhtNode(void);
		//: Destructor.

		/* ///////////////////////////////////////////////////////////////////
		//
		// query functions
		*/
	    virtual inline bool getDebugMode(void)
		{ 
			return sm_debugMode; 
		}
		//: Get the global debug mode.
		// This is common to all nodes.

		virtual inline vhtNodeType getType(void) 
		{ 
			return VHT_UNKNOWN; 
		}
		//: Get the node type.
		// The default type is VHT_UNKNOWN.

		inline bool isRoot(void) const 
		{ 
			return this == sm_root; 
		}
		//: Is this node root.
		// Each tree must have a unique root node.

		virtual inline bool isLeaf(void)
		{ 
			return true; 
		}
		//: Is this a terminal node.
		// Default return value is true.
		inline virtual bool isModified(void)
		{ 
			return m_isModified; 
		}

		//: Has this node been updated.

		inline const char  * getName(void)
		{ 
			return m_name; 
		}

		//: Get node name.
		// Return the user defined node name.
		inline vhtNode * getParent(void) 
		{ 
			return m_parent; 
		}

		//: Get this nodes parent.
		inline vhtNode * getNextSibling(void)  
		{ 
			return m_nextSibling; 
		}

		//: Get the next sibling.
		// Siblings are stored in a circular list.
		inline vhtNode * getPrevSibling(void)  
		{ 
			return m_prevSibling; 
		}
		//: Get the previous sibling.
		// Siblings are stored in a circular list.

		virtual const vhtTransform3D  &getLM(void);
		//: Get the world transformation.
		// Returns a matrix that transforms from the local frame to the global frame.

		const inline vhtBoundingSphere  &getBoundingSphere(void) const 
		{ 
			return m_boundingSphere; 
		}
		//: Get bounds.
		// Return the current bounding sphere for this node. 

		vhtNode * getNextNode(vhtNode *subTreeRoot = sm_root);
		//: Get the next node in a breadth first manner.
		//!param: subTreeRoot - Root of subtree to halt traversal at.
		// The subreeRoot defines the subtree to search below.  The return value for this
		// method is NULL when the subtreeRoot node has been reached in post order.
		// This method implements an inorder (depth first) traversal of the scene graph.
	    vhtNode * getPrevNode(void);

		//: Get the previous node.
		vhtNode * getNodeByName(const char *name);
		//: Get a particular node from the sub-tree.
		//!param: name - Name to find.

		inline vhtEngine * getEngine(void)
		{ 
			return m_engine;
		}
		//: User application engine access.
		// Returns the engine associated with this node (may be NULL).

	    inline vhtDataNode *getNeutralDual(void) 
		{ 
			return neutralDual; 
		}
		//: Get neutral data node.
		// Returns the node in the neutral graph corresponding to this.

		/*
		// set functions:
		*/
	    virtual inline void setDebugMode( bool debugMode ) 
		{ 
			sm_debugMode = debugMode; 
		}
		//: Set the debug mode.
		//!param: debugMode - Set the debug mode (on/off).
    
	    void setName(const char *name);
		//: Set the nodes name.
		//!param: name - Name to use.
		// Node names are limited to VHT_MAXNAMELEN characters.

	    bool setRoot(void);
		//: Set this node as root.
		// Overrides any previous root setting.  The root property is a static global amongst
		// nodes (i.e. root must be unique).

	    inline void setFirstHandNode(void)
		{ 
			sm_firstHandNode = this;
		}
		//: Set the first hand node.
		// Used to define the head for the hand thread.

		inline void setFirstTransformGroupNode(void)  
		{ 
			sm_firstTransformGroupNode = this;
		}
		//: Set first transform group.
		// Used to define the head for the transformgroup thread.
	    inline void setFirstDynamicNode(void) 
		{ 
			sm_localFirstDynamicNode = this;
		}
		//: Set first dynamic node.
		// Used to define the head of the dynamic node thread.

	    virtual bool setLM(const vhtTransform3D &LM);
		//: Set the local->global matrix.
		//!param: LM - Local to world matrix to copy.

	    inline void setEngine( vhtEngine *engine ) 
		{ 
			m_engine = engine; 
		}
		//: Set the application engine.
		//!param: engine - vhtEngine to use.
		// Sets the engine that will manage this node.

		inline void setNeutralDual(vhtDataNode *aDual) 
		{ 
			neutralDual= aDual; 
		}
		//: Sets the neutral dual node.
		//!param: aDual - Associated neutral dual node.

		/* ///////////////////////////////////////////////////////////////////
		//
		// Utility methods
		*/
	    inline virtual void refresh(void)
		{
			refreshTransforms();
			refreshBoundingSphere();        
	    }
		//: Reresh this node.
		// Refreshes the transforms and bounding sphere for this node.

		virtual bool render(void)
		{ 
			return true;
		}    
		//: Render this node.
		// Default behavior is no action. Returns true.

		/*///////////////////////////////////////////////////////////////////
		//
		// Protected members
		*/
	protected:

		inline void setIsModified(bool val) 
		{ 
			m_isModified = val; 
		}
		//: Set modify flag.
		//!param: val - Modification flag.

		inline void setParent(vhtNode *node)       
		{ 
			m_parent = node; 
		}
		//: Set parent node.
		//!param: node - Parent node.

		inline void setNextSibling(vhtNode *node)  
		{ 
			m_nextSibling = node; 
		}
		//: Set next sibling.
		//!param: node - Next sibling node.
		
		inline void setPrevSibling(vhtNode *node)  
		{ 
			m_prevSibling = node; 
		}
		//: Set previous sibling.
		//!param: node - Previous sibling node.

		inline void setBoundingSphere(const vhtBoundingSphere &s)   
		{ 
			m_isModified = true; 
			m_boundingSphere = s; 
		}
		//: Set bounding geometry.
		//!param: s - Bounding sphere for this node.

		inline virtual void thread(void)
		{;}
		//: Threads node to the scene graph.
		// Default behavior is no action.
		
		inline virtual void unthread(void)                                      
		{;}
		//: Unthreads node to the scene graph.
		// Default behavior is no action.

		vhtDataNode *neutralDual;
		//: Neutral dual data pointer.

	private:
		/* refresh bounds/transform*/
		virtual inline void             refreshBoundingSphere(void) {;}
		virtual inline void             refreshTransforms(void) {;}

		/* node name */
		char                             m_name[VHT_MAXNAMELEN];

		/* relatives */
		vhtNode                         *m_parent;
		vhtNode                         *m_nextSibling;
		vhtNode                         *m_prevSibling;

		/* geometry bound */
		vhtBoundingSphere               m_boundingSphere;

		/* modified? */
		bool                            m_isModified;

		/* user application class */
		static vhtEngine                *m_engine;

		/* ///////////////////////////////////////////////////////////////////
		// STATIC MEMBERS: 
		*/
public:
    static vhtNode  *getLastNode(void);
    //: Get the last node in the HSG.

    static void setGraphicsType(vhtGraphicsType g);
    //: Set the graphic renderer for the HSG.
    //!param: g - Graphics type to use.
    // Currently only VHT_OPENGL is supported.


    static inline vhtNode * getRoot(void)
	{
		return sm_root;
	}

    
	//: Get the root node of the HSG.
    static inline vhtNode *getFirstHandNode(void)
	{
		return sm_firstHandNode;
	}

    
	//: Get the head of the hand thread.
    static inline vhtNode *getFirstTransformGroupNode(void)
	{
		return sm_firstTransformGroupNode;
	}
    
	//: Get the head of the transform group thread.
    static inline vhtNode *getFirstDynamicNode(void)
	{
		return sm_localFirstDynamicNode; 
	}

    //: Get the head of the dynamic node thread.

    static void sceneGraphLock(void);
/*	{ 
		sm_sceneGraphLock.lock(); 
	}
*/

    //: Lock the scene graph for writing.
    // This must be done in multithreaded applications that access the 
    // HSG asynchronously.
    
	static void sceneGraphUnlock(void);
/*	{ 
		sm_sceneGraphLock.unlock(); 
	}*/

    //: Unlock the scene graph.

    static inline double getTimeStep(void)
	{
		return sm_timeStep;
	}

    //: Get the HSG time step.
    static  inline void setTimeStep(double timeStep)
	{ 
		if(timeStep>0.0) { 
			sm_timeStep = timeStep; 
		} 
	}

    //: Set the HSG time step.
    /*///////////////////////////////////////////////////////////////////
    //
    // private STATIC members
    */
	private:
		static void                     rethread(void);

		static vhtNode                  *sm_root;

		static bool                     sm_debugMode;

		static vhtNode                  *sm_firstHandNode;   /* for accessing all the hands in the simulation */
		static vhtNode                  *sm_firstTransformGroupNode; /* for updating the graphical scene graph */
		static vhtNode                  *sm_localFirstDynamicNode; /* for local dynamic sim */

		static double                   sm_timeStep;

		static vhtCriticalSection       sm_sceneGraphLock;

		static vhtGraphicsType          sm_graphicsType;
    


};

#endif




