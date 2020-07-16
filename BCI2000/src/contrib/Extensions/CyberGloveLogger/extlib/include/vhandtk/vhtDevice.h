#ifndef _VHT_DEVICE_H_
#define _VHT_DEVICE_H_

/********************************************************************
 FILE: $Id: vhtDevice.h,v 1.11 2002/08/28 23:16:12 jsilver Exp $
 AUTHOR: Hugo DesRosiers.
 DATE: 1999/03/01.

 DESCRIPTION:  Generic device root class.

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

#include <vhtDeviceTypes.h>

class vhtIOConn;
struct _VTDID_t;

//: Generic vtidm device class.
// Parent class for all devices connected through a VTIDM.  All sub-classes
// of vhtDevice represent physical or emulated devices.  The device
// mechanism is setup as a proxy for the actual data source (in most cases,
// using the vtidm), so getting fresh data involves two steps, first the
// device must be updated, then the data can be queried. Queries for the
// data without calling update will only return the locally cached data.
// See also vhtIOConn.
//!vhtmodule: Device
class VH_DEVICE_DLL_EXPORT vhtDevice
{
	public:
		enum Status {
			disconnected= 0, connected= 1, paused= 2
		};
		//: Device connection status.
		// Current status of the device connection.
        vhtIOConn *connectionDict; 
	private:
		Status connectionStatus;
		//: Current connection status.		

	protected:	/* Instace variables accessible from subclasses. */
		_VTDID_t *m_did;
		//: VTIDM internal data structure
		//vhtIOConn *connectionDict;   
		//: Current connection dictionnary for the device.

	public:
		vhtDevice(void);
		//: Default device constructor.  No action.
		vhtDevice(vhtIOConn *connDict, bool doConnect= true);
		//: Construct a device with specified IO connection.
		//!param: connDict - The connection dictionary for this device.  Can be
		// obtained from vhtIOConn::getDefault.
		//!param: doConnect - If true, automatically connect to the remote device.
		//  Construct a device with specified IO connection. By default
		// the device is queried.  This may be disabled by specifying doConnect = false.

		virtual ~vhtDevice(void);
		//: Close connection and delete.

		virtual vhtDeviceType getType(void) { return VHT_DEVICE; }
		//: Return the device type.
    
		virtual double getData(unsigned int index) { return 0.0;}
		//: Get device specific data.
		//!param: index - The index of the data, must be less than the dimension range.
		// Return the most recently update device data.

		virtual int getDimensionRange(void) { return 0;}
		//: Get range of the index parameter for getData calls.
		//  Get range of the index parameter for getData calls. Default is 0.
		virtual Status getConnectStatus(void);	
		//: Provide the remote node connection status.
		// Return the connection status using Status values.

		inline _VTDID_t *getDID( void ) { return m_did; }
		//: Get VTIDM data.
		// Return the VTIDM specific connection data.

		virtual bool connect(void);		
		//: Connect to the remote node.
		// Establish a connection to the device.  A return value of false indicates
		// connection failure.
		virtual bool disconnect(void);
		//: Disconnect from the remote node.
		// Discoonect from device. A return value of false indicates failure to
		// disconnect.
		virtual void update(void);    
		//: Synchronize internal data with device.
		// Read the most recent data available from the device into local buffers.

		double getLastUpdateTime( void );
		//: Return the last time of update in seconds.
};


#endif		/* _VHT_DEVICE_H_ */


