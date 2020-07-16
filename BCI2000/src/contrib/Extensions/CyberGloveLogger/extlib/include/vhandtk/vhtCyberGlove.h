#ifndef _VHT_CYBER_GLOVE_H_
#define _VHT_CYBER_GLOVE_H_

/********************************************************************
 FILE: $Id: vhtCyberGlove.h,v 1.14 2002/08/28 23:16:11 jsilver Exp $
 AUTHOR: 
 DATE: 

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

#include <vhtGlove.h>

class vhtGloveData;

//: CyberGlove device abstraction.
// Represents a physical CyberGlove connection.  Objects of this class should be
// used to get both calibrated and uncalibrated data from a physical CyberGlove
// device.  This class is a proxy for a remote device so that accessing data is
// a two stage process. First the local data must be updated by calling the update
// method, then data may be accessed with getAngle.  For uncalibrated data,
// no update call is required, simply call getRawData to get the most recent
// data.
// Calibration coeffients may also be set through this interface, however the
// recommended calibration procedure is to use the DCU software.
//!vhtmodule: Device
class VH_DEVICE_DLL_EXPORT vhtCyberGlove : public vhtGlove {
private:	/* Instance variables. */
    /* IO daemon related stuff. */
    int datasz;
    char *raw;
    bool bRightHand;
    
    /* Internal data holder.*/
    vhtGloveData * data;

protected:	/* Instance methods for subclasses. */
    vhtGloveData * getSensorArray( void );
    //: Get a full field of calibrated glove data.
    // Return the most recently updated glove data.

public:	/* Instance methods.*/
    vhtCyberGlove(void);
    //: Construct a default CyberGlove object with no connection.
    // Default constructor.  Does not connect to any devices.
    vhtCyberGlove(vhtIOConn *connDict, bool doConnect= true);
    //: Construct a physical CyberGlove object.
    //!param: connDict - Connection dictionary.  See vhtIOConn::getDefault.
    //!param: doConnect - True if connection should be established during construction.
    // Construct a CyberGlove object associated with the specified IO connection.
    // By default, the device is queried immediately. This behavior can be
    // disabled by specifing doConnect = false.

    virtual ~vhtCyberGlove(void);
    //: Disconnect and delete CyberGlove connection.
    // Delete CyberGlove connection.

    virtual bool connect( void );
    //: Connect to CyberGlove device.
    // Connects to device and sets the handedness based on the physical device.

    virtual double getRawData(unsigned int anIndex);
    //: Get raw (uncalibrated) sensor data from the glove. 
    //!param: anIndex - Index into raw glove data.
    // Directly access the raw data from the device, no update call is required. 
    // The index must be in the valid range of the glove sensors.  For sensor assignments, see the
    // Users manual. 

    virtual double getRawData(GHM::Fingers aFinger, GHM::Joints aJoint);
    //: Get raw (uncalibrated) sensor data from finger related sensors
    //!param: aFinger - The vhtGenHandModel finger identifier.
    //!param: aJoint - The vhtGenHandModel joint identifier.
    // Directly access the raw data from the device, no update call is required. 
    
    virtual double getRawData(GHM::HandParts handPart, GHM::Joints aJoint);
    //: Get raw (uncalibrated) sensor data from palm related sensors
    //!param: handPart - The vhtGenHandModel hand part identifier.
    //!param: aJoint - The vhtGenHandModel joint1 identifier.
    // Directly access the raw data from the device, no update call is required. 

    virtual double getData(unsigned int anIndex);
    //: Get calibrated sensor data from the glove.
    //!param: anIndex - Index into raw glove data.
    // Get the most recently updated calibrated glove data. The index must be
    // in the valid range of the glove sensors.

    virtual double getData(GHM::Fingers aFinger, GHM::Joints aJoint);
    //: Get calibrated sensor data from finger related sensors
    //!param: aFinger - The vhtGenHandModel finger identifier.
    //!param: aJoint - The vhtGenHandModel joint identifier.
    // Get the most recently updated calibrated glove data. 

    virtual double getData(GHM::HandParts handPart, GHM::Joints aJoint);
    //: Get calibrated sensor data from palm related sensor
    //!param: handPart - The vhtGenHandModel hand part identifier.
    //!param: aJoint - The vhtGenHandModel joint1 identifier.
    // Get the most recently updated calibrated glove data

    virtual double getAngle(GHM::Fingers aFinger, GHM::Joints aJoint);
    //: Get joint angle in radians.
    //!param: aFinger - The vhtGenHandModel finger identifier.
    //!param: aJoint - The vhtGenHandModel joint identifier.
    // Get the most recently updated calibrated joint angle data.  The return
    // value is in radians.
    virtual int getDimensionRange(void);
    //: Get number of glove sensors.
    // Return the valid range of the glove sensors.  Returns 22.

    virtual bool getSwitchStatus(void);
    //: Return the current (on/off) status of the glove switch.
    // Returns true for enabled switch and false for disabled. No update call is requried.

    virtual void update(void);
    //: Update glove data.
    // Update internal data buffers with the most recent data from the
    // physical device.
 
    virtual void setSensorGain(GHM::Fingers aFinger, GHM::Joints aJoint, double gain);
    //: Set the gain of a CyberGlove sensor.
    //!param: aFinger - The vhtGenHandMode finger identifier.
    //!param: aJoint - The vhtGenHandMode joint identifier.
    //!param: gain - The gain to apply.
    // Sets the gain of a CyberGlove sensor and writes it to the device manager. The
    // calibration mapping is ja = g*(r-o), where ja is the joint angle in radians,
    // g is the real valued gain, r is the raw sensor value (0-255) and o is the 
    // offset (0-255).
    // This method throws a vhtBadLogic exception if the call cannot be completed.

    virtual void setSensorOffset(GHM::Fingers aFinger, GHM::Joints aJoint, int offset);
    //: Set the offset of a CyberGlove sensor.
    //!param: aFinger - The vhtGenHandMode finger identifier.
    //!param: aJoint - The vhtGenHandMode joint identifier.
    //!param: offset - The offset to apply.
    // Sets the offset of a CyberGlove sensor and writes it to the device manager
    // The calibration mapping is ja = g*(r-o), where ja is the joint angle in radians,
    // g is the real valued gain, r is the raw sensor value (0-255) and o is the 
    // offset (0-255).
    // This method throws a vhtBadLogic exception if the call cannot be completed.

};


#endif		/* _VHT_CYBER_GLOVE_H_ */



