/********************************************************************
 FILE: $Id: vhtGraspStateManager.h,v 1.17 2002/10/30 18:54:56 cullrich Exp $
 AUTHOR: Chris Ullrich
 DATE: Aug 1998

 DESCRIPTION:
  -
 HISTORY:
  -
 NOTES:
           -- COPYRIGHT VIRTUAL TECHNOLOGIES, INC. 1998 --
********************************************************************/
#ifndef VHT_GRASP_STATE_MANAGER_H
#define VHT_GRASP_STATE_MANAGER_H

#if defined(BUILD_WIN32_VH_DLL)

#if defined __BUILD_VH_CORE_DLL__
#define VH_CORE_DLL_EXPORT __declspec(dllexport)
#else
#define VH_CORE_DLL_EXPORT __declspec(dllimport)
#endif

#else 

#define VH_CORE_DLL_EXPORT

#endif

#include <vhtVector3d.h>
#include <vhtTransform3D.h>

class vhtPhalanx;
class vhtNode;
class vhtComponent;
class vhtHumanHand;

typedef enum 
{
    VHT_GRASP_NONE,
    VHT_GRASP_ONE,
    VHT_GRASP_TWO,
    VHT_GRASP_THREE,
    VHT_GRASP_G2,
    VHT_GRASP_G3,
    VHT_GRASP_RELEASE
} vhtGraspState;

//: Manage the grasping state.
// This class implements a state machine to allow components to be grasped
// by a human hand.  The VHT limits one grasp state manager for each vhtComponent
// in a haptic scene graph.
//
// To use this state machine, set the all contact normals for all phalanges at each
// time step.  The state manager will recalculate the component state after each
// contact normal is set.  If a grasping situation is detected, the associated
// vhtHumanHand will automatically be informed and the object will be constrained
// to the hand.  Note that even if an object is being grasped, the grasping conditions
// must be fufilled at each subsequent time step.
//
// A grasp state is achieved by having at least two contact normals that are separated by
// a user specified minimum normal angle.  At least one of the contact normals must correspond
// to the thumb, and only one is used per finger.
//!vhtmodule: Core
class VH_CORE_DLL_EXPORT vhtGraspStateManager
{
	public:
		vhtGraspStateManager(void);
		//: Construct a default state manager.

		~vhtGraspStateManager(void);
		//: Destructor.

		inline vhtGraspState getState(void)
		{ 
			return m_state; 
		}

		//: Get current state.
		// The state may be queried at any time.


		inline void setMinNormalAngle( double angle ) 
		{ 
			m_minNormalAngle = angle; 
			m_stateChanged = true; 
		}
		//: Set minimum normal separation angle (radians).
		//!param: angle - Minimum normal separation angle.
		// Set the minimum separation of normals.  All normals must have at least
		// this angular separation from all other normals.  The angle is specified in
		// radians and should be in the range 0 to Pi.

		inline double getMinNormalAngle(void) 
		{ 
			return m_minNormalAngle; 
		}

		//: Get the current minimum normal separation angle.

		inline void setGraspObject( vhtComponent *graspObject ) 
		{ 
			m_graspObject = graspObject; 
		}

		//: Set the associated grasp component.
		//!param: graspObject - Component to manage.
		// Assigns this state machine to the graspObject component.

		inline vhtComponent *getGraspObject(void) 
		{ 
			return m_graspObject; 
		}

		//: Get the currently assigned grasp component.

		bool addNormal( const vhtVector3d &normal, vhtNode *owner );
		//: Add a normal from a user object.
		//!param: normal - Contact normal.
		//!param: owner - Node which generated the contact normal.
		// Add a contact normal from a user specified HSG node.  This method should be not be
		// used in conjunction with addPhalanxNormal. This allows objects to grasp each other.
		// Note that normals must be in the global frame to use one finger grasping.

		void addPhalanxNormal( const vhtVector3d &normal, vhtPhalanx *phalanx );
		//: Add a normal from a hand.
		//!param: normal - Contact normal.
		//!param: palanx - Planax associated with this normal.
		// Add a hand contact normal.  Use this method to grasp objects with a human hand.
		// Note that normals must be in the global frame to use one finger grasping.

		void constrain(void);
		//: Constrain the object.
		// If the object is grasped, set it's transformation matrix to correspond to the
		// grasping component ( or human hand).

		void quickUpdate(void);
		//: Update and refresh object.
		// Updates the grasped components transformation and refresh it's subgraph.

		void recalculateState(void);
		//: Determine current state/transisitions.
		// Determine the state using current normals. This is done automatically for
		// hand grasping normals.

		inline int  getNumDropFingers(void) 
		{ 
			return m_numDropFingers; 
		}
		//: Get the number of drop fingers.
		// Returns the number of fingers needed to transition to the drop state.
		inline void setNumDropFingers( int numFingers ) 
		{ 
			m_numDropFingers = numFingers; 
			m_stateChanged = true; 
		}

		//: Set the number of fingers required for dropping objects.  
		//!param: numFingers - Number of fingers to allow.
		// This is the number of fingers that need to be unflexed (straight) to drop a
		// grasped object.  Once this number of fingers has been attained, no further
		// grasping is possible until the object is not colliding with the hand.

		inline double getReleaseAngle(void) 
		{ 
			return m_dropAngle; 
		}
		
		//: Get the alpha tolerance for transistion to release.
		inline void setReleaseAngle( double angle ) 
		{ 
			m_dropAngle = angle; 
			m_stateChanged = true; 
		}
		//: Set the alpha tolerance for transistion to release.
		//!param: angle - Release angle.
		// The alpha transition angle is an angle measured from the unflexed hand in the
		// direction of the palm.  If a fingertip has alpha angle less than this value,
		// it's considered to be a drop finger.  See setNumDropFingers.

		inline void resetNormals(void) 
		{ 
			m_numNormals = 0; 
		}

		//: Reset the normals.
		// Set the number of grasp normals to zero.
		void reset(void);
		//: Reset the state machine.

		void setState( vhtGraspState state );
		//: Set the current grasp state.
		//!param: state - Explicit state.
		// Force a particular grasping state.  This will be overwritten on the next call
		// to reset or recalculateState.

		inline void setGraspHand( vhtHumanHand *hand ) { m_graspHand = hand; }
		//: Set the associated human hand.
		//!param: hand - Associated hand.
		// This is automatic if contact normals are added for phalanx.

		inline vhtHumanHand *getGraspHand( void ) { return m_graspHand; }
		//: Get the associated human hand.
		//!return: The associated human hand.

		void setUseGraspOneState( bool useGraspOne );
		//: Allow the grasp state manager to use or not use the VHT_GRASP_ONE state.
		//!param: useGraspOne - Set to true to enable one finger grasping.
		// Turn on or off the grasp one state transition.  
		bool getUseGraspOneState( void );
		//: Get the current availaiblity of the VHT_GRASP_ONE state transition.

		void setGraspOneConeSlope( double slope );
		//: Set the VHT_GRASP_ONE cone slope.
		//!param: slope - Slope of contact cone.
		// The cone slope is the value below which the dot product of the relative motion
		// of the contact finger and the initial contact normal must lie.

		double getGraspOneConeSlope( void );
		//: Get the VHT_GRASP_ONE cone slope.

	private:

		// get number of small alpha angles
		int                        getNumSmallAlphas(void);

		vhtGraspState              m_state;
		vhtGraspState              m_nextState;
		bool                       m_stateChanged;

		int                        m_numNormals;
		vhtVector3d                m_normals[3];
		int                        m_fingers[3];

		double                     m_minNormalAngle;
		vhtVector3d                m_graspLocationVector;
		vhtTransform3D             m_graspLocation;
		vhtNode                   *m_graspParent;

		vhtComponent              *m_graspObject;

		vhtHumanHand              *m_graspHand;
		int                        m_numDropFingers;
		double                     m_dropAngle;

		bool                       m_useGraspOneState;
		double                     m_graspOneConeSlope;
};

#endif


