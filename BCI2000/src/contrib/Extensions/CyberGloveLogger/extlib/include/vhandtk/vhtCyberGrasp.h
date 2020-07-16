#ifndef VHT_CYBER_GRASP_H
#define VHT_CYBER_GRASP_H

/********************************************************************
 FILE: $Id: vhtCyberGrasp.h,v 1.22 2002/11/01 00:17:30 dgomez Exp $
 AUTHOR: Chris Ullrich
 DATE: August 1998

 DESCRIPTION:

 HISTORY:
 
           -- COPYRIGHT VIRTUAL TECHNOLOGIES, INC. 1998 --
********************************************************************/

#if defined(BUILD_WIN32_VH_DLL)

#if defined __BUILD_VH_DEVICE_DLL__
#define VH_DEVICE_DLL_EXPORT __declspec(dllexport)
#else
#define VH_DEVICE_DLL_EXPORT __declspec(dllimport)
#endif

#else

#define VH_DEVICE_DLL_EXPORT

#endif

#include <client.h>
#include <vhtDevice.h>

class vhtContactPatch;
class vhtHapticEffect;

//: CyberGrasp hardware abstraction.
// vhtCyberGrasp is an interface to a physical CyberGrasp force-feedback
// controller.  This class only represents the feedback portion of the
// CyberGrasp system.  Glove and tracker proxies are available as vhtCyberGlove
// and vht6DofDevice respectively.  Objects of this type are usually attached to a vhtHumanHand.
// The CyberGrasp device has an associated state, which may be queried with the getMode method.
// To do force feedback, the CyberGasp must be in either GR_CONTROL_FORCE, GR_CONTROL_EFFECT or
// GR_CONTROL_IMPEDENCE mode.  Please see the users guide for further details on the operation
// of the CyberGrasp device.
// Note see also vhtCyberGlove, vhtHumanHand and vhtContactPatch.
//!vhtmodule: Device
class VH_DEVICE_DLL_EXPORT vhtCyberGrasp : public vhtDevice
{
public:
    vhtCyberGrasp(void);
    //: Construct a default CyberGrasp with no physical connection.

    vhtCyberGrasp(vhtIOConn *connDict, bool doConnect= true);
    //: Construct a physical CyberGrasp using the provided connection. 
    // Construct a physical CyberGrasp.  By default, the Grasp unit is queried,
    // but this may be disabled by specifying doConnect = false.  Currently,
    // the grasp unit will try to automatically associate itself with a 
    // local glove (local to the controller).

    vhtCyberGrasp(vhtIOConn *connDict, vhtIOConn *gloveDict, bool doConnect= true);
    //: Construct a physical CyberGrasp using the provided connection. 
    // Construct a physical CyberGrasp.  By default, the Grasp unit is queried,
    // but this may be disabled by specifying doConnect = false.
    // In addition, the grasp unit is associated with the indicated glove.  Note that
    // only gloves local to the controller are supported.

    ~vhtCyberGrasp(void);
    //: Close connection to CyberGrasp and delete.


    bool setGlove( vhtIOConn * connectionDict );
    //: Set the associated glove device
    // Attach this CyberGrasp to the glove whose server and port are specified in the given 
    // connection.

    virtual bool disconnect(void);
    //: Disconnect from the physical device.

    vhtDeviceType getType(void) 
	{
		return VHT_CYBERGRASP; 
	}
    //: Get the device type.
    // Returns VHT_CYBERGRASP.

    bool doCalibration(void);
    //: Calibrate the grasp unit.  
    // Calibrate grasp unit (depreciated).  Please use the DCU to calibrate the CyberGrasp device
    // properly.

    virtual void setForce( double fd[5] );
    //: Set the force level of the CyberGrasp.
    // Set the force level.  fd must point to an array of 5 doubles.
	// CyberGrasp force values are in the range of 0 to 1. 
    // A force level of 0 is no force, and a force level of 1 is maximum force.  
	// This command also sets the control mode to GR_CONTROL_FORCE.

    void setForceEffectActive(int finger, bool actv = true );
    //: Set force effect active or inactive.
    // The actual effect starts when the setForce or startEffect function are called

    void setForceEffectActiveAll(bool actv = true);
    //: Set force effect active or inactive for all fingers.
    // The actual effect starts when the setForce or startEffect function are called

    void startForceEffect(void);
    //: Start the force effect. The current (last force sent) force is maintained
    // if setForce is called inside the loop there is not need to use startEffect, just
    // use setForceEffectActive everytime in the loop to control wich fingers have effects

    void stopForceEffect(void);
    //: Stop the force effect. The current (last force sent) force is maintained
    // if setForce is called inside the loop there is not need to use stopEffect, just
    // use setForceEffectActive(finger, false) everytime in the loop to control wich 
    // fingers have effects

    void triggerForceEffect(void);
    //: triggers re-starts effects in all fingers
    // A convination stop-start can not be used since stop deletes the active effect list
    //   
    

    bool setMode( int mode );
    //: Set the control mode for the CyberGrasp.
    // Set the control mode. Available modes include:
    // GR_CALIBRATION_E2G_MAP - calibration mode.
    // GR_CONTROL_REWIND      - rewind mode.
    // GR_CONTROL_FORCE       - force mode.
    // GR_CONTROL_EFFECT      - effect mode.
    // GR_CONTROL_IMPEDENCE   - impedence mode.
    // More details on modes can be found in the VHT users guide.  Note that mode
    // settings are not exclusive, and other clients may also set modes asynchronously.

    inline int getMode(void) 
	{ 
		return m_currentMode; 
	}

    //: Get the current control mode.
    // Get the current control mode, see setMode for a list of available modes.


    void setContactPatch( int finger, const vhtContactPatch *patch, bool autosend = true );
    //: Set the contact patch for finger.
    // Set the contact patch for finger.  By default contact patches are sent to the
    // controller immediately.  However improved performance can be had by setting
    // contact patches for each finger and then sending them with the sendContactPatches 
    // method.  Patches are cached in the controller and remain in effect until an updated
    // patch is sent.

    void setResetPatch( vhtContactPatch *patch );
    //: Set the default reset patch.
    // Set the default reset patch.  The reset patch should be in palm co-ordinates.  This
    // method stores the reset patch locally, to set the reset patch on the controller, 
    // the method resetContactPatch should also be used.  Note that only one reset patch is
    // stored for all fingers.

    void resetContactPatch( int finger, bool autosend = true );
    //: Set the contact patch to the reset patch.
    // The reset patch can be used to provide a default 'contact free' patch situation.

    void sendContactPatches(void);
    //: Send all buffered contact patches.
    // Sends the most recently set contact patches for all fingers.


    void setEffect( int finger, vhtHapticEffect *effect );
    //: Set the impedence effect for finger.
    // Set haptic effect for finger.  This effect will remain valid until a new one is sent.  
    // Effects are activated in the corresponding contact patch. See the users guide for
    // more details on impedence effects.


    virtual double getData(unsigned int index) 
	{ 
		return 0.0;
	}

    //: Get current data.
    // Returns 0.

    virtual int getDimensionRange(void) 
	{ 
		return 0;
	}

    //: Get dimension range of data.
    // Returns 0.

    /* Depreciated methods */
    bool setGlove(char * pserver, char * pport);

    //: Set the associated glove device.
    // Attach this CyberGrasp to the glove located on pserver at pport (depreciated).

    bool setTracker(char * pserver, char * pport, char * pdevice);

    //: Set the associated tracker.
    // Attach this CyberGrasp to the tracker located on pserver, at pport with pdevice id (depreciated).

    bool rewind( void );
    //: Rewind the grasp unit.
    // Set the control mode to GR_CONTROL_REWIND (depreciated).

protected:
    // IO daemon related stuff
    int             datasz;
    char            *raw;
    int             m_numPatches;
    int             m_rawPatchSize;

    int             m_currentMode;

    // force buffer
    double          m_force[5];

    // effect active-flag array
    bool            m_forceEffectActive[5];

    // TODO should be 1/finger
    vhtContactPatch *m_resetPatch;  
};

#endif


