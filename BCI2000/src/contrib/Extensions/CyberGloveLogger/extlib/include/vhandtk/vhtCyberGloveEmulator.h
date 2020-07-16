#ifndef __vhtCyberGloveEmulator_h
#define __vhtCyberGloveEmulator_h

#if defined(BUILD_WIN32_VH_DLL)

#if defined __BUILD_VH_DEVICE_DLL__
#define VH_DEVICE_DLL_EXPORT __declspec(dllexport)
#else
#define VH_DEVICE_DLL_EXPORT __declspec(dllimport)
#endif

#else

#define VH_DEVICE_DLL_EXPORT

#endif

#include <memory.h>
#include <vhtGlove.h>

//: Software CyberGlove emulator.
// Emulate the behavior of a CyberGlove.  Allows software control/simulation of
// a glove without a physical device.  This class implements all necessary features
// of a glove, but without a device connection.  Set methods can be used to control
// the sensor values directly.
//!vhtmodule: Device
class VH_DEVICE_DLL_EXPORT vhtCyberGloveEmulator : public vhtGlove {

public:
    vhtCyberGloveEmulator(void);
    //: Construct a CyberGlove emulator.
    ~vhtCyberGloveEmulator(void);
    //: Delete emulator.

    inline double getAngle(GHM::Fingers aFinger, GHM::Joints aJoint){
		return m_jointAngle[aFinger][aJoint];
    }
    //: Get joint angle in radians.
    //!param: aFinger - A vhtGenHandModel finger identifier.
    //!param: aJoint - A vhtGenHandModel joint identifier.
    // Get joint angle corresponding the a specified finger and joint (in radians).

    inline void	setAngle(GHM::Fingers aFinger, GHM::Joints aJoint, double angle)
	{
		m_jointAngle[aFinger][aJoint] = angle;
    }
    //: Set a joint angle in radians.
    //!param: aFinger - A vhtGenHandModel finger identifier.
    //!param: aJoint - A vhtGenHandModel joint identifier.
    //!param: angle - The joint angle to set in radians.
    // Set joint angle corresponding the a specified finger and joint (in radians).

    inline void	setAngles( double * angles )
	{
		memcpy(m_jointAngle, angles, sizeof(double) * ( GHM::nbrFingers + 1 ) * ( GHM::nbrLinks +1 ) );
    }
    //: Set all joint angles.
    //!param: angles - An array of (GHM::nbrFingers+1)*(GHM::nbrLinks+1) doubles.
    // Set all the joint angles at once.

    double getData(unsigned int index)
	{ 
		return 0.0;
    }

    //: Get calibrated data.
    //!param: index - The index of the joint angle to retrieve.
    // Get calibrated data.  Currently this method always returns 0.

    int getDimensionRange(void)
	{
		return 22;
	}

    //: Get the dimension range of the glove emulator.
    //  Get the dimension range of the glove emulator.  This is always 22.

    bool connect(void)
	{
		return true;
	}

    //: Connect to physical device.  
    // Connect to physical device.  This always returns true.

    bool disconnect(void)
	{
		return true;
	}

    //: Disconnect from physical device.  
    // Disconnect from physical device.  This always returns true.

    void update(void)
	{
		// do nothing
	}
    
	//: Update internal data.
    // No action is performed.

    virtual Status getConnectStatus(void) 
	{
		return vhtDevice::connected;
    }	

    //: Provide the remote node connection status.
    // Return the connection status using Status values.

protected:
    double         m_jointAngle[ GHM::nbrFingers + 1 ][ GHM::nbrLinks + 1 ];
};

#endif


