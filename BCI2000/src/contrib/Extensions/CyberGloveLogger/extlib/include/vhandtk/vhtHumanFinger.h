#ifndef __VHT_HUMAN_FINGER_H
#define __VHT_HUMAN_FINGER_H
/********************************************************************
 FILE: $Id: vhtHumanFinger.h,v 1.7 2002/08/30 21:23:53 dgomez Exp $
 AUTHOR: Chris Ullrich
 DATE: Feb 1999
 DESCRIPTION:

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

#include <vhtGroup.h>
#include <vhtGenHandModel.h>

class vhtContactPatch;
class vhtHumanHand;
class vhtPhalanx;
class vhtTransform3D;

//: A human finger.
// This class is used to represent a single human finger.  It contains direct
// pointers to each of it's phalanges.  In addition, managment of the contact
// patch for the current finger is supported.  Although human fingers can
// be instantiated directly in user applications, if they do not have a
// human hand pointer, their behavior is undefined.
//!vhtmodule: Core
class VH_CORE_DLL_EXPORT vhtHumanFinger : public vhtGroup
{
	private:
	  vhtHumanHand *m_hand;
	  vhtPhalanx   *m_distal;
	  vhtPhalanx   *m_proximal;
	  vhtPhalanx   *m_metacarpal;
	  GHM::Fingers  m_fingerType;
  
	protected:
	  vhtHumanFinger(void);
	  //: Default contstructor.
 
	public:
	  vhtHumanFinger( GHM::Fingers fingerId, vhtHumanHand *hand );
	  //: Construct a particular finger.
	  //!param: fingerId - Id of this finger.
	  //!param: hand - Associated human hand.
	  // Constructs a finger, including all the phalanx, and attaches to 
	  // the provided human hand object.
	  ~vhtHumanFinger(void);
	  //: Destruct.
	  // Deletes the entire sub-graph also.

	  /*
		Phalanx access.
	   */
	  virtual vhtPhalanx *getDistalPhalanx(void);
	  //: Get the distal phalanx.
	  virtual vhtPhalanx *getProximalPhalanx(void);
	  //: Get the proximal phalanx.
	  virtual vhtPhalanx *getMetacarpalPhalanx(void);
	  //: Get the metacarpal phalanx.

	  virtual vhtPhalanx *getPhalanx( GHM::Joints joint );
	  //: Get a particular phalanx.
    
	  /*
		Phalanx control
	   */
	  virtual void setDistalPhalanx( vhtPhalanx *dip );
	  //: Set the distal phalanx.
	  //!param: dip - Distal phalanx to use.
	  virtual void setProximalPhalanx( vhtPhalanx *pip );
	  //: Set the proximal phalanx.
	  //!param: pip - Proximal phalanx to use.
	  virtual void setMetacarpalPhalanx( vhtPhalanx *mcp );
	  //: Set the metacarpal phalanx.
	  //!param: mcp - Metacarpal phalanx to use.

	  /*
		Type specification
	   */
	  virtual GHM::Fingers getFingerType(void);
	  //: Get the finger type.
	  // Finger type will be GHM::Fingers, see vhtGenHandModel.
	  virtual void setFingerType( GHM::Fingers fingerType );
	  //: Set the finger type.
	  //!param: fingerType - GHM::Fingers type to use.
    
	  /*
		Hand
	   */
	  virtual vhtHumanHand *getHand(void);
	  //: Get the associated human hand.
	  virtual void setHand( vhtHumanHand *hand);
	  //: Set the associated human hand.
	  //!param: hand - Associated hand.
 
	  /*
		Contact patches
	   */
	  void setContactPatch( const vhtContactPatch *patch, GHM::Joints phalanx );
	  //: Set the contact patch for a particular phalanx.
	  //!param: patch - Contact patch to set.
	  //!param: palanx - Phalanx to set patch for.
	  const vhtContactPatch *getContactPatch( GHM::Joints phalanx );
	  //: Get the contact patch for a particular phalanx.
	  //!param: phalanx - Phalanx to retrieve patch from.
	  virtual void resetContactPatches(void);
	  //: Reset all contact patches for this finger.
	  // Contact patches for all phalanges are cleared.

	  virtual void setTransform( GHM::Joints joint, vhtTransform3D *xform );
	  //: Transform convenience functions.
	  //!param: joint - Joint to set.
	  //!param: xform - Transformation to set.
	  // Allow direct access to the transformations for each of the joints.
};

#endif


