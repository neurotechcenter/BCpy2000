/********************************************************************
 FILE: $Id: vhtContactPatch.h,v 1.21 2002/08/28 23:16:09 jsilver Exp $
 AUTHOR: 
 DATE: 

 DESCRIPTION:

 HISTORY:
 
           -- COPYRIGHT VIRTUAL TECHNOLOGIES, INC. 1998 --
********************************************************************/
#ifndef VHT_CONTACT_PATCH_H
#define VHT_CONTACT_PATCH_H

#if defined(BUILD_WIN32_VH_DLL)

#if defined __BUILD_VH_DEVICE_DLL__
#define VH_DEVICE_DLL_EXPORT __declspec(dllexport)
#else
#define VH_DEVICE_DLL_EXPORT __declspec(dllimport)
#endif

#else

#define VH_DEVICE_DLL_EXPORT

#endif

#include <vhtVector3d.h>

//: A tangent plane with surface features.
// Contact patches are used to communicate local surface information
// to secondary applications.  Contact patches are infinte planes 
// with an associated stiffness, damping and inertia.
// One example of this is sending the 
// current contact region near a fingertip to the grasp controller.
// Both the vhtCyberGrasp and vhtHumanHand classes allows user applications 
// to set a contact patch for each finger.  If a CyberGrasp unit is connected
// to the application, these patches are sent to the controller.
//!vhtmodule: Device
class VH_DEVICE_DLL_EXPORT vhtContactPatch
{
public:
    enum PatchFrame { palm, hand, world };
    enum GraspedPatch { isGrasped, notGrasped };

public:

    vhtContactPatch(void); 
    //: Construct a default contact patch.
    vhtContactPatch( const vhtVector3d &offset, const vhtVector3d &normal );
    //: Construct a particular contact patch.
    //!param: offset - Patch offset vector.
    //!param: normal - Patch normal vector.
    // A default patch is constructed with the user provided
    // plane.  All vectors are assumed to be in world co-ordinates and with
    // units of cm.

    inline ~vhtContactPatch(void) {;}
    //: Destruct.

    inline const vhtVector3d &getOffset(void) const { return m_offset; }
    //: Get the tangent plane origin.
    // Get the vector that joins the global frame to this plane.
    inline void setOffset( const vhtVector3d &offset ) { m_offset = offset; }
    //: Set the tangent plane origin.
    //!param: offset - Patch offset vector.
    // Set the vector that joins the global frame to this plane.
    // Offset must be in global frame.

    inline const vhtVector3d &getNormal(void) const { return m_normal; }
    //: Get  the tangent plane normal.
    // Returns the normal to the plane.
    inline void setNormal( const vhtVector3d &normal ) { m_normal = normal; }
    //: Set  the tangent plane normal.
    //!param: normal - Patch normal vector.
    // Normal must be in global frame.


    void setTangentPlane( const vhtVector3d &offset, const vhtVector3d &normal );
    //: Set the tangent plane.
    //!param: offset - Patch offset vector.
    //!param: normal - Patch normal vector.
    // Convenience routine for setting the tangent plane.
    // All vectors are assumed to be in world co-ordinates.

    void setStiffness( const double stiffness );
    //: Set the patch normalized stiffness.
    //!param: stiffness - Stiffness of patch.
    // Stiffness is normalized to [0..1], where 0 is no stiffness,
    // and 1 is the maximum stiffness of the device.

    double getStiffness(void) const;
    //: Get the patch normalized stiffness.

    void setDamping( const double damping );
    //: Set the patch normalized damping.
    //!param: damping - Damping of patch.
    // Damping is normalized to [0..1], where 0 is no damping,
    // and 1 is the maximum damping of the device.

    double getDamping(void) const;
    //: Get the patch normalized damping.

    inline void setInertia( const double inertia ) { m_inertia = inertia; }
    //: Set the plane inertia.
    //!param: inertia - Inertia of the patch.
    // Future expansion.
    inline double getInertia( void ) const { return m_inertia; }
    //: Get the plane inertia.
    // Future expansion.    

    inline void setDistance( const double distance ) { m_mtd = distance; }
    //: Set the most recent plane finger distance.
    //!param: distance - Last MTD for this contact patch.
    // Used to find closest patch for a finger if multiple
    // patches are set.
    inline double getDistance(void ) const { return m_mtd; }
    //: Get the patch finger distance.
   
    inline bool getEffectActive(void) const { return m_effectActive; }
    //: True if an effect is active for this patch.
    inline void setEffectActive( bool active ) { m_effectActive = active; }
    //: Set the haptic effect active for this patch.
    //!param: active - Set to true to enable impedance effects.
    // Haptic effects can be played on contact patches.  For further
    // information, see vhtHapticEffect.


    inline bool getAttenuationEnvelope( void ) const { return m_attenuationEnvelope; }
    //: True if the orientation envelope is activated for contact patches.
    inline void setAttenuationEnvelope( bool active ) { m_attenuationEnvelope = active; }
    //: Set the orienation envelope.
    //!param: active - Set to true to enable attenuation.
    // Default is true, so the envelope is always applied. This envelope attenuates
    // the patch stiffness as a function of the fingertip orientation.

    inline PatchFrame getPatchFrame( void ) const { return m_patchFrame; }
    //: Get the preferred frame for this patch.
    inline void setPatchFrame( PatchFrame frame ) { m_patchFrame = frame; }
    //: Set the preferred frame.

    inline GraspedPatch getGraspedPatch( void ) const { return m_graspedPatch; }
    //: State variable for the grasp state of the object associated with this patch.
    inline void setGraspedPatch( GraspedPatch gPatch ) { m_graspedPatch = gPatch; }
    //: Set the preferred frame.

private:
    bool           m_effectActive;
    bool           m_attenuationEnvelope;

    double         m_stiffness;      // [N/m]
    double         m_damping;        // [Ns/m]
    double         m_inertia;        // [kg]

    vhtVector3d    m_offset;         // [cm]
    vhtVector3d    m_normal; 
    double         m_mtd;            // distance
    
    PatchFrame     m_patchFrame;   // frame of the patch
    GraspedPatch   m_graspedPatch;   // state of patch
};

#endif


