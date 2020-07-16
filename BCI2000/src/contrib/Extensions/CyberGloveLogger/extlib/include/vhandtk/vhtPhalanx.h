#ifndef __VHT_PHALANX_H
#define __VHT_PHALANX_H
/********************************************************************
 FILE: $Id: vhtPhalanx.h,v 1.9 2002/08/30 21:24:14 dgomez Exp $
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


#include <vhtGenHandModel.h>
#include <vhtTransformGroup.h>

class vhtContactPatch;
class vhtHumanHand;
class vhtHumanFinger;

//: A single human hand phalanx.
// Methods for management of a single hand phalanx.  Phalanx are exentensions
// of haptic transform group nodes.  Each finger has a sub-tree of phalanx. 
//!vhtmodule: Core
class VH_CORE_DLL_EXPORT vhtPhalanx : public vhtTransformGroup
{
 public:
    vhtPhalanx(void);
    //: Construct a default phalanx.
    vhtPhalanx(vhtHumanHand *aHand, GHM::Fingers finger, GHM::Joints joint);
    //: Construct a specific phalanx.
    //!param: aHand - Associated hand object.
    //!param: finger - Finger owner type.
    //!param: joint - Joint type for this.
    // Construct a phalanx attached to aHand for finger and joint.

    ~vhtPhalanx(void);
    //: Destruct.
    // Deletes all children.

    /*
      Hand access
     */
    inline vhtHumanHand *getHand(void) { return m_hand; }
    //: Get the associated human hand.
    inline void setHand(vhtHumanHand *hand) { m_hand = hand; }
    //: Set the associated human hand.
    //!param: hand - Associated hand object.

    /*
      Type specifiers
     */
    inline void setFingerType( GHM::Fingers fingerType ) { m_fingerType = fingerType; }
    //: Set the finger type.
    //!param: fingerType - Finger owner type.
    inline void setJointType( GHM::Joints jointType ) { m_jointType = jointType; }
    //: Set the joint type.
    //!param: jointType - Joint type for this.

    inline GHM::Fingers getFingerType(void) { return m_fingerType; }
    //: Get the finger type.
    inline GHM::Joints getJointType(void) { return m_jointType; }
    //: Get the joint type.

    /*
      Contact patches
     */
    void setContactPatch( const vhtContactPatch *patch );
    //: Set the contact patch for this phalanx.
    //!param: patch - Patch to use.
    // Patches are cached until a call to setContactPatch, or to resetContactPatch.
    inline const vhtContactPatch *getContactPatch(void) { return m_patch; }
    //: Get the most recent contact patch.

    inline bool isPatchValid(void) { return m_isPatchValid; }
    //: Is current patch valid.
    inline void resetContactPatch(void) 
	{ 
	    m_NumPatches = 0;
	    m_isPatchValid = false; 
	}
    //: Reset the contact patch.

    void computeAggregatePatch( void );
    //: Compute the aggregate patch.

	double getPenetration( void );
	void setPenetration( double p );

protected:
    void initialize( void );

private:
    GHM::Fingers              m_fingerType;
    GHM::Joints               m_jointType;

    vhtHumanHand    * m_hand;

    void             copyPatch( const vhtContactPatch *patch );
    bool             m_isPatchValid;
    vhtContactPatch *m_patch;

    void             _allocatePatches( void );
    //: Allocate contact patch array.
    vhtArray         m_PatchList;
    int              m_NumPatches;

	double m_penetrationDistance;

};

#endif


