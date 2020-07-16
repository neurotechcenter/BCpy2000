
#ifndef VHT_HUMAN_HAND_H
#define VHT_HUMAN_HAND_H

/********************************************************************
 FILE: $Id: vhtHumanHand.h,v 1.29 2006/08/14 21:22:10 mbakircioglu Exp $
 AUTHOR: Chris Ullrich
 DATE: August 1998

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

#include <vhtGenHandModel.h>
#include <vhtContactPatch.h>

class vhtComponent;
class vhtKinematics;
class vhtCyberGrasp;
class vhtHumanFinger;
class vhtGlove;
class vhtTracker;
class vhtHandMaster;
class vhtIOConn;
class vhtHandGeometry;
class vhtGraspControl;
class vhtPhalanx;

//: Human hand model.
// An instance of a human hand.  This class brings together all hand relate
// data sources, graphics and feedback mechanisms.  The human hand contains
// references to external devices such as vhtCyberGlove, vht6DofDevice,
// vhtCyberGrasp and vhtCyberTouch.  Once devices are registered as belonging
// to a vhtHumanHand object, their update and connection status are centrally
// managed by this class.
// There is a hand kinematic model that generates homogeneous transformations
// (vhtTransform3D) from glove and tracker data.  A complete graphic
// represention is also included as well as a haptic model for force
// feedback. In addition, objects of this type can be treated atomically by
// the collision engine.
//!vhtmodule: Core
class VH_CORE_DLL_EXPORT vhtHumanHand
{

protected:	/* Instance variables accessible by subclasses. */
	vhtComponent	*m_hapticRoot;				  //: Root of the haptic scene graph.
	vhtHumanFinger  *m_fingers[VHT_DEFAULT_FINGERS]; //: Array of finger pointers.
	vhtPhalanx		*m_palmPhalanx;				  //: Palm geometry.
	vhtHandGeometry *m_visualGeometry;			  //: The visual hand geometry.
	vhtKinematics	*m_kinematics;				  //: The hand kinematic algorithms.
	vhtHandMaster	*m_manipulator;				  //: External data sources.
	vhtCyberGrasp	*m_cyberGrasp;				  //: Force feedback mechanism.
	GHM::Handedness  handedness;				  //: The handedness (GHM::rightHand or GHM::leftHand).
	vhtContactPatch  m_handFramePatches[VHT_DEFAULT_FINGERS]; //: Retrieved patches in hand frame.
	bool			 m_externalDeviceUpdates;
	bool			 m_graspDeviceOwner;		  //: Flag to indicate who owns the grasp.

private:		/* Instance methods for this class only. */
	void initialize(double scale = 1.0);
	void setHumanHandDynamics(void);


protected:	// Instance methods modifiable by subclasses.
	virtual void buildHapticModel(vhtComponent *handRoot);
	//: Build the associated haptic model.
	//!param: handRoot - Root node of hand geometry.
	// Model data is constructed from an internal representation.  Subclasses
	// may add whatever geometry they wish here, as long as the
	// updateVisualGeometry method is also updated.
	virtual void buildGeometry(double scale =1.0);
	//: Build visual model.
	// Constructs the visual scene graph from an internal representation.

	virtual void convertToHandFrame( int fingerId, vhtContactPatch &patch );
	//: Convert patch to palm frame.
	//!param: fingerId - Id of finger that owns patch.
	//!param: patch - A contact patch in world co-ordinates.
	// In certain operating modes, the CyberGrasp controller requires contact patches
	// to be sent in palm frame.  This method manages the conversion.
	virtual void			   setFinger(GHM::Fingers fingerId,
		vhtHumanFinger *finger);
	//: Set the indicated finger pointer.
	//!param: fingerId - Id of finger to set.
	//!param: finger - Finger to set.
	// Sets the finger subgraph to the provided finger object.  The valid
	// range of finger is GHM::thumb to GHM::pinky.
	void				setVisualGeometry(vhtHandGeometry *handGeometry);
	//: Set the visual geometry.
	//!param: handGeometry - Hand geometry to use.
	// Set the visual subgraph to be used to draw the hand on the screen.
	virtual const vhtContactPatch *selectContactPatch(GHM::Fingers finger );
	//: Select the contact patch for indicated finger.
	//!param: finger - Finger to select contact patch for.
	// Will select the 'best' available contact patch from among all the phalanx.

public:		/* Instance methods for all other to use.*/
	/* Constructors. */
	vhtHumanHand(double scale =1.0, GHM::Handedness h= GHM::rightHand);
	//: Construct a default human hand.
	//!param: h - Handedness.
	// Builds all human hand data structures with no external data sources or sinks.
	vhtHumanHand(vhtIOConn *gloveDict, vhtIOConn *trackerDict, double scale =1.0, 
		GHM::Handedness h= GHM::rightHand);
	//: Construct a human hand with given tracker and glove sources.
	//!param: gloveDict - A CyberGlove connection descriptor.
	//!param: trackerDict - A tracker connection descriptor.
	//!param: h - Handedness.
	// Builds all human hand data structures with provided glove and tracker
	// data connections.
	vhtHumanHand(vhtGlove *aGlove, vhtTracker *aTracker, double scale =1.0, 
		GHM::Handedness h= GHM::rightHand);
	//: Construct a human hand with given tracker and glove.
	//!param: aGlove - An initialized glove object.
	//!param: aTracker - An initialized tracker object.
	//!param: h - Handedness.
	// Builds all human hand data structures with provided glove and tracker.
	vhtHumanHand(vhtIOConn *gloveDict, vhtIOConn *trackerDict,
		vhtIOConn *graspDict, double scale =1.0, GHM::Handedness h= GHM::rightHand);
	//: Construct a human hand with given tracker, glove and grasp sources.
	//!param: gloveDict - A CyberGlove connection descriptor.
	//!param: trackerDict - A tracker connection descriptor.
	//!param: graspDict - A CyberGrasp connection descriptor.
	//!param: h - Handedness.
	// Builds all human hand data structures with provided glove, tracker and
	// grasp data connections.
	vhtHumanHand(vhtGlove *aGlove, vhtTracker *aTracker,
		vhtCyberGrasp *aGrasp, double scale =1.0, GHM::Handedness h= GHM::rightHand);
	//: Construct a human hand with given tracker, glove and grasp.
	//!param: aGlove - An initialized glove object.
	//!param: aTracker - An initialized tracker object.
	//!param: aGrasp - An initialized grasp object.
	//!param: h - Handedness.
	// Builds all human hand data structures with provided glove, tracker and grasp.
	vhtHumanHand(vhtHandMaster *aMaster, double scale =1.0, GHM::Handedness h= GHM::rightHand);
	//: Construct a human hand with given hand master.
	//!param: aMaster - An initialized hand master.
	//!param: h - Handedness.
	// Builds all human hand data structures with glove and tracker provided
	// by the vhtHandMaster.
	vhtHumanHand(vhtHandMaster *aMaster, vhtCyberGrasp *aGrasp, double scale = 1.0, 
		GHM::Handedness h= GHM::rightHand);
	//: Construct a human hand with given hand master and grasp.
	//!param: aMaster - An initialized hand master.
	//!param: aGrasp - An initialized grasp object.
	//!param: h - Handedness.
	// Builds all human hand data structures with glove and tracker provided
	// by the vhtHandMaster.  CyberGrasp is connected to indicated vhtCyberGrasp.

	/* Destructors.*/
	virtual ~vhtHumanHand(void);
	//: Disconnect and delete.

	virtual void disconnect(void);
	//: Close all VTIDM connections.
	// Disconnect all external devices registered with the human hand.

	/* 
	   Data accessors. 
	*/
	virtual vhtHumanFinger	*getFinger(GHM::Fingers finger);
	//: Get an indicated finger.
	//!param: finger - Finger to retrieve.
	// Returns a pointer to the selected vhtHumanFinger object.  The valid
	// range of finger is GHM::thumb to GHM::pinky.
	inline vhtPhalanx		 *getPalmPhalanx( void ) { return m_palmPhalanx; }
	//: Get the palm phalanx.
	// Returns a pointer to the phalnax representing the palm.

	virtual vhtKinematics	 *getKinematics(void);
	//: Get current hand kinematics object.
	// Returns a pointer to the current vhtKinematics object.
	virtual vhtComponent	  *getHapticRoot(void);
	//: Get the root of the haptic subgraph containing the hand.
	// Returns a pointer to the root of the haptic subgraph for this human hand.
	virtual vhtHandGeometry	*getVisualGeometry(void) { return m_visualGeometry; }
	//: Get the current graphical hand.
	// Returns a pointer to the hand geometry in use.
	virtual vhtHandMaster *getHandMaster(void);
	//: Get the vhtHandMaster utility instance.
	// Returns a pointer to the instance of vhtHandMaster that contains the
	// vhtCyberGlove and vhtTracker used by the vhtHumanHand.
	virtual vhtCyberGrasp	 *getCyberGrasp(void);
	//: Get the CyberGrasp device.
	// Returns a pointer to the CyberGrasp used to send force feedback
	// information from this hand.

	/* 
	   Specialized functionality. 
	*/
	virtual const vhtContactPatch *getContactPatch( GHM::Fingers finger,
		GHM::Joints phalanx = GHM::distal ) const;
	//: Get the current contact patch for the indicated phalanx.
	//!param: finger - Finger id of patch.
	//!param: palanx - Phalanx id of patch.
	// Returns a pointer to the contact patch that is being sent to the
	// CyberGrasp object for a particular phalanx.  
	virtual void setContactPatch( GHM::Fingers finger, const vhtContactPatch *patch );
	//: Set the contact patch for an indicated finger.
	//!param: finger - Finger if of patch.
	//!param: patch - Patch to use.
	// Sets the contact patch.  By default this is the GHM::distal patch and
	// is gauranteed to be selected when the hand updates the CyberGrasp unit.
	// More precise patch control is available directly from the
	// vhtHumanFinger class.

	virtual void resetContactPatches(void);
	//: Reset all contact patches.
	// Set the contact patches for all fingers to the reset patch defined in
	// vhtCyberGrasp.
	virtual void update( bool updateVisuals = false );
	//: Update all external and internal data.
	//!param: updateVisuals - Set to true to also update the visual hand graph.
	// Call this method to make all vhtHumanHand data structures current.
	// Will get most recent data from glove and tracker, send contact patches
	// to grasp and update the kinematics.
	// The updateVisuals argument indicates that the visual sub-graph should
	// also be updated.
	virtual void updateVisualGeometry( void );
	//: Copy current kinematics to visual geometry.

	virtual void updateCyberGrasp( void );
	//: Update the CyberGrasp device.

	void setExternalDeviceUpdates( bool externalDeviceUpdates);
	//: Controls device updates.
	// Set to true if associated h/w devices will be updated by an external agency.
	// Default is false.

	/*
	  Debug.
	 */
	void renderPatches(void);
	//: Render contact patches.
	// Renders the current contact patches to the OpenGL context.

};

#endif // VHT_HUMAN_HAND_H

