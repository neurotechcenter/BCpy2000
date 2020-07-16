#ifndef _VHT_SIMULATION_
#define _VHT_SIMULATION_

#if defined(BUILD_WIN32_VH_DLL)

#if defined __BUILD_VH_CORE_DLL__
#define VH_CORE_DLL_EXPORT __declspec(dllexport)
#else
#define VH_CORE_DLL_EXPORT __declspec(dllimport)
#endif

#else 

#define VH_CORE_DLL_EXPORT

#endif

class vhtGroup;
class vhtEngine;

//: Generic user simulation.
// vhtSimulation provides a standard mechanism for user based haptic applications.
// User applications should extend this class to incorporate functionality specific
// to their need.  Once a user simulation has been created, it can be passed into
// a vhtEngine and will then be executed automatically.
// The vhtSimulation can be viewed as a haptic main loop similar to the 
// main loop in openInventor, or Glut.  It takes care of updating the associated
// haptic scene graph (by calling refresh), updating all associated hands, and
// calling the update sequence.  Children of this class should override
// the appropriate methods (preLockProcess, handleConstraints,
// postLockProcess) to provide access to their application specific code.
//!vhtmodule: Core
class VH_CORE_DLL_EXPORT vhtSimulation
{
	public:
		vhtSimulation(void);
		//: Construct a default simulation.
		// There is no associated scene graph.
		vhtSimulation(vhtGroup *root);
		//: Construct a simulation for a given scene graph.
		// The created simulation will manage the sub-graph below root.
		~vhtSimulation(void);
		//: Destruct.

		virtual void setHapticSceneGraph( vhtGroup *root ) { m_hapticSceneGraph = root; }
		//: Set the haptic scene.
		//!param: root - Root of haptic scene graph.
		// The simulation object will manage the sub-graph below root.
		virtual vhtGroup *getHapticSceneGraph(void) { return m_hapticSceneGraph; }
		//: Get the haptic scene.


		virtual void addSubgraph( vhtGroup *aGroup );
		//: Add a haptic subgraph.
		//!param: aGroup - Subgraph to add.
		// By default this does no action. Subclasses of this should
		// implement this method in a manner consistent with their application.

		virtual void setEngine( vhtEngine *engine ) { m_engine = engine; }
		//: Set the vhtEngine.
		//!param: engine - Engine to control this.
		// Sets the engine that will run this simulation.  This should be
		// automatically taken care of by vhtEngine.setSimulation().
		virtual vhtEngine *getEngine(void) { return m_engine; }
		//: Get the vhtEngine.

		virtual void doSimulation(void);
		//: Perform one loop of the simulation code.
		// Lock the scene graph, call handleConstraints, refresh hands and scene graph, then unlock the scene graph.

		/*
		  User virtual methods.
		 */
		virtual void preLockProcess(void) {;}
		//: User function.
		// Called before hand and scene graph are updated.
		virtual void handleConstraints(void) {;}
		//: User function.
		//: Called after hand and scene graph are updated.
		virtual void postLockProcess(void) {;}

		/*
		  Simulation control.
		 */
		virtual void startSimulation(void);
		//: Indicate that the simulation is ready to run.
		virtual void stopSimulation(void);
		//: Request a halt to the simulation.
		// Due to multithreading issues, the simulation may not
		// stop immediately.  
		virtual bool stopRequested(void) { return m_stopRequested; }
		//: Check if a simulation stop has been requested.

#if !defined(_WIN32)
		virtual long long getFrameCount( void ) { return m_frameCounter; }
		//: Return the number of frames executed.
#else
		virtual long getFrameCount( void ) { return m_frameCounter; }
		//: Return the number of frames executed.
#endif

protected:
		vhtGroup    *m_hapticSceneGraph;
		//: The associated haptic sub-graph.
		bool         m_stopRequested;
		//: A stop has been requested.
		vhtEngine   *m_engine;
		//: The associated engine.
#if !defined(_WIN32)
		long long    m_frameCounter;
#else
		long    m_frameCounter;
#endif
    //: Count the number of frames.
};

#endif


