#ifndef __vhtTrackerEmulator_h
#define __vhtTrackerEmulator_h

#if defined(BUILD_WIN32_VH_DLL)

#if defined __BUILD_VH_DEVICE_DLL__
#define VH_DEVICE_DLL_EXPORT __declspec(dllexport)
#else
#define VH_DEVICE_DLL_EXPORT __declspec(dllimport)
#endif

#else

#define VH_DEVICE_DLL_EXPORT

#endif


#include <vht6DofDevice.h>
#include <vhtTracker.h>

//: A software tracker emulator.
// Emulate the behavior of a 6 degree of freedom tracking device from software.
// The tracker emulator class can be used to simulate a tracker from software, or to
// apply user defined transformations to an existing tracker.  In addition to the
// usual get methods of the vhtTracker class, the emulator includes set methods for all
// the sensor data.
// Note that currently, the vhtTrackerEmulator is limited to 1 6DOF device.
//!vhtmodule: Device
class VH_DEVICE_DLL_EXPORT vhtTrackerEmulator : public vhtTracker {

	public:

		enum {
			X_POS = 0,
			Y_POS = 1,
			Z_POS = 2,
			X_ROT = 3,
			Y_ROT = 4,
			Z_ROT = 5,
			SENSOR_COUNT = 6
	    };

		vhtTrackerEmulator( void );
		//:	Construct a tracker emulator.  All data fields are initialized to 0.

		~vhtTrackerEmulator( void );
		//: Destructor.
    
		inline void setTrackerPosition(double x, double y, double z)
		{
			m_data[X_POS] = x;
			m_data[Y_POS] = y;
			m_data[Z_POS] = z;
			
			m_rcvr->setRawData( vht6DofDevice::xPos, x );
			m_rcvr->setRawData( vht6DofDevice::yPos, y );
			m_rcvr->setRawData( vht6DofDevice::zPos, z );
	    }

		//: Set the position fields for the tracker.
		// Sets the data fields for tracker position to the provided co-ordinates.
		inline void	setTrackerOrientation(double x, double y, double z)
		{
			m_data[X_ROT] = x;
			m_data[Y_ROT] = y;
			m_data[Z_ROT] = z;
			
			m_rcvr->setRawData( vht6DofDevice::xAngle, x );
			m_rcvr->setRawData( vht6DofDevice::yAngle, y );
			m_rcvr->setRawData( vht6DofDevice::zAngle, z );
	    }

		//: Set the rotation fields for the tracker.
		// Sets the data fields for tracker rotation to the provided co-ordinates.  The angles are 
		// XYZ Euler angles.


		double getRawData(unsigned int anIndex)
		{
			return m_rcvr->getRawData( (vht6DofDevice::Freedom)anIndex);
		}
		//: Return the specified data from the internal buffer.
		// Returns data that was set previously using setTrackerPosition and setTrackerOrientation.

		vht6DofDevice *getLogicalDevice(unsigned int anIndex)
		{
			return m_rcvr;
		}

		//: Return a pointer to the 6DofDevice associated with this tracker. 
		// Return a pointer to the 6DofDevice associated with this tracker. Currently only one
		// 6Dof device is supported.
		unsigned int getLogicalDevicesCount(void)
		{
			return 1;
		}

		//: Return the number of 6DofDevices available.
		// Always returns 1.

		double getData(unsigned int index)
		{
			return m_data[index];
		}

		//: Return the specified data from the internal buffer.
		// Returns data that was set previously using setTrackerPosition and setTrackerOrientation.

		int getDimensionRange(void)
		{
			return SENSOR_COUNT;
		}

		//: Return the number of degrees of freedom available.
		//  Return the number of degrees of freedom available.  Currently 6.
		bool connect(void)
		{
			return true;
		}

		//: Connect to software device.
		// Always returns true.

		bool disconnect(void)
		{
			return true;
		}

		//: Disconnect from software device.
		// Always returns true.

		void update(void)
		{
			// do nothing

		}

		//: Update internal data.
		// This method does no action.
		virtual Status getConnectStatus(void) 
		{
			return vhtDevice::connected;
		}	

		//: Provide the remote node connection status.
		// Return the connection status using Status values.
	protected:
		double          m_data[SENSOR_COUNT];
		vht6DofDevice  *m_rcvr;
};

#endif


