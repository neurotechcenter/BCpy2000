#ifndef _VHT_GLOVE_H_
#define _VHT_GLOVE_H_

/********************************************************************
 FILE: $Id: vhtGlove.h,v 1.9 2002/08/28 23:16:15 jsilver Exp $
 AUTHOR: Hugo DesRosiers.
 DATE: 1999/03/02.

 DESCRIPTION: Abstract Virtex Glove definition.

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

#include <vhtDevice.h>
#include <vhtGenHandModel.h>

//: Abstract glove class.
// Define the virtual base methods for accessing a glove device.
//!vhtmodule: Device
class VH_DEVICE_DLL_EXPORT vhtGlove : public vhtDevice {

  public:
    vhtGlove(void);
    //: Construct a default glove.
    // Default glove, no VTIDM connection.
    vhtGlove(vhtIOConn *connDict, bool doConnect= true);
    //: Construct a glove with given connection.
    // Construct a glove using given IOConn and query the device.  See vhtIOConn::getDefault
    // for access to the device registry.

    virtual ~vhtGlove(void);
    //: Disconnect and delete.


    virtual vhtDeviceType getType(void) { return VHT_GLOVE; }
    //: Get device type.
    // Returns VHT_GLOVE.

    virtual double getAngle(GHM::Fingers aFinger, GHM::Joints aJoint);
    //: Get glove joint angle.
    // Return the measured joint angle.  The base class returns 0.0.
};


#endif		/* _VHT_GLOVE_H_ */


