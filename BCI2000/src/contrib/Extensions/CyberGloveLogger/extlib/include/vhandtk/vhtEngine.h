#ifndef _VHT_ENGINE_H_
#define _VHT_ENGINE_H_

/********************************************************************
 FILE: $Id: vhtEngine.h,v 1.13 2002/08/28 23:16:13 jsilver Exp $
 AUTHOR: Hugo DesRosiers.
 DATE: March 5th, 1999.

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

#include <vhtThread.h>


class vhtCollisionEngine;
class vhtHumanHand;
class vhtGroup;
class vhtDataNode;
class vhtNodeParser;
class vhtSimulation;
class vhtNodeHolder;
class vhtNode;

//: Simulation management.
// The vhtEngine class is intended to be an easy to use framework for constructing
// user simulations.  Primarily, the engine runs a vhtSimulation created by
// the user in it's own execution thread.  In addition to this, the engine is
// the central repositry for all human hands in a particular simulation as well
// as the haptic scene graph.
//!vhtmodule: Core
class VH_CORE_DLL_EXPORT vhtEngine {

	private:
		enum Constants {
			maxNbrHands= 4
		};

	protected:	/* Instance variables accessible to subclasses. */

		unsigned int              nbrHands;			
		//: Number of hands in this engine.
		vhtHumanHand             *hands[maxNbrHands];	
		//: The hands that will manipulate the haptic scene.
		vhtGroup                 *hapticScene;		
		//: The root of the haptic scene.
		vhtThread                 simulationThread; 
		//: Thread ptr for simulation loop.
		vhtSimulation            *simulation;
		//: The user simulation.
		vhtDataNode				 *neutralGraph;		
		//: Graph mapping haptic nodes and graphic nodes, for updates.
		vhtNodeHolder			 *fastUpdateList;	
		//: List of all transformGroup haptic nodes in neutral graph.


	public:	/* Instance methods. */
		vhtEngine(bool autoBoot = true);
		//: Construct an engine.
		//!param: autoBoot - Set to true to automatically boot.
		// By default, the engine is booted.

		virtual ~vhtEngine(void);
		//: Destructor.

		virtual bool boot(void);
		//: Create a default haptic scene graph.
		// The boot procedure constructs a rooted haptic scene graph and stores the
		// result in hapticScene.
		
		virtual bool start(void);
		//: Begin the simulation.
		// Spawn a seperate execution thread and run the simulation object's methods.
		// Will run until stop or pause is called.  The return value indicates
		// success of the thread creation.

		virtual bool stop(void);
		//: Stop the current simulation.
		// Terminates the execution thread.  Returns the sucess of this.
		virtual bool pause(void);
		//: Pause the simulation thread.
		// Not currently implemented.

		/* 
		   Data accessors. 
		*/
		virtual unsigned int getHandCount(void);
		//: Return the number of hands.
		// Returns the number of registered hands in this simulation.

		virtual vhtHumanHand *getHand(unsigned int handID);
		//: Get a particular hand.
		//!param: handID - Id of hand to retrieve.
		// Returns the vhtHumanHand stored in position handID. The valid range of handID is 0 to 
		// getHandCount(). Invalid handID's return NULL.

		virtual vhtDataNode *getNeutralGraph(void);
		//: Get the root of the neutral scene graph.
    
		virtual vhtNodeHolder *getComponentUpdaters(void);
		//: Get the update thread list.
		// Returns the head of the update transform list.  This is a list of the nodes that
		// must be synchronized by the neutral scene graph once per frame.

		void removeComponentUpdaters(void);
		//: Remove the list of component updaters.

		/* 
		   Add information to the engine. 
		*/
		virtual bool registerHand(vhtHumanHand *aHand);
		//: Register a human hand with the engine.
		//!param: aHand - Hand to register.

		virtual vhtDataNode *registerVisualGraph(void *aNode, vhtNodeParser *parserFct, bool dynamicGraph= true);
		//: Register a visual scene graph.
		//!param: aNode - Root node of 3rd party data structure.
		//!param: parserFct - Node parser for aNode.
		//!param: dynamicGraph - Force all generated VHT nodes to be dynamic.
		// User applications can add arbitrary scene graphs to the engine by passing
		// the scene graph root and a traversal function.  Scene graphs with the
		// dynamicGraph flag set to true are given vhtDynamicAttributes.

		virtual void updateTransformList(vhtDataNode *aNode);
		//: Update the transformation thread.
		//!param: aNode - Root of neutral scene graph.
		// Rebuilds the fastUpdateList from the indicated neutral scene graph.

		void flagGraphExternal(vhtNode *rootNode);
		//: Set the attributes of a haptic graph to external.
		//!param: rootNode - Root of nodes to be flagged.
		// Sets all the children of rootNode to have vhtExternalAttributes.

		virtual void setHapticSceneGraph( vhtGroup *root );
		//: Set the root of the haptic scene graph.
		//!param: root - Root of the haptic scene graph.
		virtual vhtGroup *getHapticSceneGraph(void);
		//: Get the current root of the haptic scene graph.

		void useSimulation( vhtSimulation *sim );
		//: Sets the simulation to use.
		//!param: sim - Simulation to use.
		// Users can inherit vhtSimulation into their own application.  This may then be 
		// passed as the simulation to use.

		static unsigned int __cdecl simulationFct( void *param );
		//: Simulation callback method.
};

#endif		/* _VHT_ENGINE_H_ */


