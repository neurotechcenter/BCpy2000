#ifndef _VHT_TRACKER_H_
#define _VHT_TRACKER_H_
/********************************************************************
 FILE: $Id: vhtTracker.h,v 1.14 2002/08/28 23:16:25 jsilver Exp $
 AUTHOR: Hugo DesRosiers.
 DATE: March 1st, 1999.

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

#include <vhtDevice.h>

class vhtTrackerData;
class vht6DofDevice;

//: Tracking device interface class.
// This class provides a convenient abstraction of a physical 6 degree of freedom 
// tracking device.  Multiple receivers with a single transmitter are supported.
//!vhtmodule: Device

class VH_DEVICE_DLL_EXPORT vhtTracker : public vhtDevice
{
	private:
		int    datasz;
		char  *raw;

	protected:		/* Instance variables accessible by subclasses. */
		unsigned int    nbrReceivers;	//: Nbr of receivers in 'receivers' array.
		vht6DofDevice **receivers;		//: Receiver devices.
		vhtTrackerData *data;               //: Tracker data holder.
		double          m_globalScaling;    //: global scaling factor.

	protected:	/* Instance methods accessible by subclasses. */
		vhtTrackerData *getSensorArray(int rcvr = 0);
		//: Return most recent data from indicated receiver.
		//!param: rcvr - The receiver for which data should be returned.
		// Note that requesting a receiver that is greater than the number of physical
		// receivers may have undefined results.

	public:		/* Instance methods. */
		vhtTracker(void);
		//: Construct a default tracker device with no VTIDM connection.

		vhtTracker(vhtIOConn *connDict, bool doConnect= true);
		//: Construct a physical tracker device.
		//!param: connDict - The connection dictionary for this device. May be obtained from vhtDevice::getDefault.
		//!param: doConnect - True if the constructor should also connect to the remote device.
		// Construct a tracker object. By default also connect to the device
		// manager and query the device.  This may be disabled by setting doConnect = false.
    
		virtual ~vhtTracker(void);
		//: Disconnect and delete.

		virtual vhtDeviceType getType(void) { return VHT_TRACKER; }
		//: Return the physical device type.
		// Returns VHT_TRACKER.

		virtual double getRawData(unsigned int anIndex);
		//: Get one raw data value. (depreciated)

		virtual double getData(unsigned int anIndex);
		//: Get one data value. (depreciated)

		virtual int getDimensionRange(void);
		//: Get device dimensionality.
		// This returns the dimensionality of the underlying receiver, not the number of
		// receivers.

		virtual vht6DofDevice *getLogicalDevice(unsigned int anIndex);
		//: Return a pointer to a particular logical device.
		//!param: anIndex - The id of the requested device.
		// Connects and constructs a vht6DofDevice with the indicated index.  Receivers are
		// labeled starting at 0.  

		virtual void setLogicalDevice( unsigned int anIndex, vht6DofDevice *rcvr );
		//: Set the logical device for anIndex.
		//!param: anIndex - The 6DOF device slot to set.
		//!param: rcvr - A valid 6DOf device.
		// Use this method to set inherited vht6DofDevice's in the tracker update loop.

		virtual void update(void);
		//: Synchronize recent tracker data.
		// Update all 6DOF's with the most recent tracker data from the tracking device.
    
		virtual void setMonitorSynchro(unsigned int mode);
		//: Enable monitor synchronization.
		// For magnetic trackers, this enables the CRT monitor synchronization mode.

		virtual void setGlobalScaling( double _newScaling ) { m_globalScaling = _newScaling; }
		//: Set the global scaling parameter.
		// Default scaling is 1.0;

		virtual double getGlobalScaling( void ) { return m_globalScaling; }
		//: Get the global scaling parameter.
		// Default scaling is 1.0;
};


#endif		/* _VHT_TRACKER_H_ */



