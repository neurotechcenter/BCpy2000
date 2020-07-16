#ifndef _VHT_CYBER_TOUCH_H_
#define _VHT_CYBER_TOUCH_H_

/********************************************************************
 FILE: $Id: vhtCyberTouch.h,v 1.9 2002/08/28 23:16:12 jsilver Exp $
 AUTHOR: Chris Ullrich
 DATE: 1999/03/02.

 DESCRIPTION: CyberTouch interface class

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

#include <vhtCyberGlove.h>

//: CyberTouch device abstraction.
// Hardware device interface class for CyberTouch device.  This subclass of vhtCyberGlove
// implements the feedback functionality needed to set vibration amplitudes in a
// CyberTouch glove.
//!vhtmodule: Device
class VH_DEVICE_DLL_EXPORT vhtCyberTouch : public vhtCyberGlove
{
  public:

	vhtCyberTouch(void);
    //: Construct a default CyberTouch with no connection.

    vhtCyberTouch(vhtIOConn *connDict, bool doConnect= true); 
    //: Construct a CyberTouch with given connection.
    //!param: connDict - The connection dictionary for this device. May be obtained from vhtDevice::getDefault.
    //!param: doConnect - True if the constructor should also connect to the remote device.
    // Construct a touch object. By default also connect to the device
    // manager and query the device.  This may be disabled by setting doConnect = false.

    void setVibrationAmplitude( double *va );
    //: Set vibration for all actuators.
    //!param: va - An array of 6 doubles.
    // Set the vibration level for all actuators.  The amplitude range is 0 to 1 with
    // 0 being no vibration and 1 being maximum vibration.


    void setVibrationAmplitude( GHM::Fingers aFinger, double va );
    //: Set an individual actuator on a finger.
    //!param: aFinger - A vhtGenHandModel finger identifier.
    // Set the vibration amplitude for a given actuator. The amplitude range is 0 to 1 with
    // 0 being no vibration and 1 being maximum vibration.  To reduce communication latency,
    // it is recommended to set all vibration amplitudes at once.


    void setVibrationAmplitude( GHM::HandParts aPart, double va );
    //: Set an individual non-finger actuator.
    //!param: aPart - A vhtGenHandModel hand part identifier.
    // Set the vibration amplitude for a given actuator. The amplitude range is 0 to 1 with
    // 0 being no vibration and 1 being maximum vibration.  To reduce communication latency,
    // it is recommended to set all vibration amplitudes at once.
};

#endif


