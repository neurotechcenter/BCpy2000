#ifndef VHT_GHOST_HUMAN_HAND_H
#define VHT_GHOST_HUMAN_HAND_H

#if defined(BUILD_WIN32_VH_DLL)

#if defined __BUILD_VH_CORE_DLL__
#define VH_CORE_DLL_EXPORT __declspec(dllexport)
#else
#define VH_CORE_DLL_EXPORT __declspec(dllimport)
#endif

#else 

#define VH_CORE_DLL_EXPORT

#endif

#include <vhtCollisionTracker.h>
#include <vhtHumanHand.h>
#include <vhtVector3d.h>

class vhtCollisionPair;
//: A ghosted human hand class.
// Implements the functionality necessary to constrain the virtual hand
// to lie outside all collidable objects in the scene.  For details on
// the use of this class, refer to the programmers guide.
//!vhtmodule: Core
class VH_CORE_DLL_EXPORT vhtGhostHumanHand : public vhtHumanHand
{
	protected:
	  vhtCollisionTracker        m_collisionTracker[6][3];
	  vhtVector3d                m_palmOffset;
	  vhtVector3d                m_patchOffset;
	  vhtVector3d                m_fingerOffset[5];

	  unsigned int               m_numCollisions;
	  unsigned int               m_fingerCollision[5];
	  double                     m_fingerForces[5];
  
	  double                     m_convergenceRate;
	  double                     m_jumpMagnitude;
	  bool                       m_useFingerOffset;
	  bool                       m_applyFingerOffset[5];

	  virtual void convertToHandFrame( int fingerId, vhtContactPatch &patch );
	
	public:
		/* Constructors. */
		vhtGhostHumanHand(double scale =1.0, GHM::Handedness h= GHM::rightHand);
		//: Construct a default human hand.
		// Builds all human hand data structures with no external data sources or sinks.
		vhtGhostHumanHand(vhtIOConn *gloveDict, vhtIOConn *trackerDict,double scale =1.0, GHM::Handedness h= GHM::rightHand);
		//: Construct a human hand with given tracker and glove sources.
		// Builds all human hand data structures with provided glove and tracker data connections.
		vhtGhostHumanHand(vhtGlove *aGlove, vhtTracker *aTracker,  double scale =1.0, GHM::Handedness h= GHM::rightHand);
		//: Construct a human hand with given tracker and glove.
		// Builds all human hand data structures with provided glove and tracker.
		vhtGhostHumanHand(vhtIOConn *gloveDict, vhtIOConn *trackerDict, vhtIOConn *graspDict, double scale =1.0, GHM::Handedness h= GHM::rightHand);
		//: Construct a human hand with given tracker, glove and grasp sources.
		// Builds all human hand data structures with provided glove, tracker and grasp data connections.
		vhtGhostHumanHand(vhtGlove *aGlove, vhtTracker *aTracker, vhtCyberGrasp *aGrasp, double scale =1.0, GHM::Handedness h= GHM::rightHand);
		//: Construct a human hand with given tracker, glove and grasp.
		// Builds all human hand data structures with provided glove, tracker and grasp.
		vhtGhostHumanHand(vhtHandMaster *aMaster, double scale =1.0, GHM::Handedness h= GHM::rightHand);
		//: Construct a human hand with given hand master.
		// Builds all human hand data structures with glove and tracker provided by the vhtHandMaster.
		vhtGhostHumanHand(vhtHandMaster *aMaster, vhtCyberGrasp *aGrasp, double scale =1.0, GHM::Handedness h= GHM::rightHand);
		//: Construct a human hand with given hand master and grasp.
		// Builds all human hand data structures with glove and tracker provided by the vhtHandMaster.
		// CyberGrasp is connected to indicated vhtCyberGrasp.

		/* Destructors.*/
		virtual ~vhtGhostHumanHand(void);
		//: Disconnect and delete.

		virtual void initialize(double scale = 1.0);
		//: Init the vhtGhostHand.

		virtual void setPalmOffset( vhtVector3d &offset );
		//: Set the visual geometry offset.

		virtual const vhtVector3d &getPalmOffset( void ) { return m_palmOffset; }
		//: Return the palm offset

		virtual void setContactPair( vhtCollisionPair *pair );
		//: A collision event.

		virtual void endOfCollisionProcessing( void );
		//: Post process all the collision events.

		virtual void update( bool updatevisuals = false );
		//: Update ghost hand related data.  

		virtual void renderPlanes( void );

		virtual void addOrthogonal( vhtVector3d &v1, vhtVector3d &v2 );

		virtual void setConvergenceRate( double rate ) { m_convergenceRate = rate; }
		virtual double getConvergenceRate( void ) { return m_convergenceRate; }

		virtual void setUseFingerOffset( bool useFingerOffset ) { m_useFingerOffset = useFingerOffset; }
		virtual bool getUseFingerOffset( void ) { return m_useFingerOffset; }

		virtual void setApplyFingerOffset( unsigned int fingerId, bool apply );
};

#endif
