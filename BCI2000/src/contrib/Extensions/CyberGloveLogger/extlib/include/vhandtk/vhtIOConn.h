#ifndef _VHT_IO_CONN_H_
#define _VHT_IO_CONN_H_

/********************************************************************
 FILE: $Id: vhtIOConn.h,v 1.12 2002/08/28 23:16:17 jsilver Exp $
 AUTHOR: Hugo DesRosiers.
 DATE: March 1st, 1999.

 DESCRIPTION: Definition of the connection parameters of a remote device
	(normally a IODaemon-based endpoint).

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

class ConfigList;
class Config;

//: A VTIDM connection.
// This class represents a VTIDM data connection.  Note that this class does
// not connect to the VTIDM, that is done by subclasses of vhtDevice.
//!vhtmodule: Device
class VH_DEVICE_DLL_EXPORT vhtIOConn {
	
	public:
		enum EntryKey {
			deviceClass, host, port, device, rate, name
				, nbrEntries
		};
		//: Connection related parameter types.

		enum DeviceType {
			glove, tracker, grasp, touch
		};
		//: VTIDM default connection devices.

		char *values[nbrEntries];
	protected:
		//char *values[nbrEntries];
		//: Connection related parameters.

	private:
		static void itoa(char *result, unsigned int anInt, unsigned int maxLength);
		static ConfigList *s_pConfigList;
		static Config *defaultConfig;

	public:		/* Instance methods. */
		vhtIOConn(const char *aDeviceClass, const char *aHost, const char *aPort,
			const char *aDevice, const char *aRate);

		char* getvalues(int a) { return values[a];}
		//: Construct a connection to VTIDM.
		// Describes a connection to a VTIDM daemon process located at aHost:aPort.
		// Host should be a resolvable internet name or address.  Port is the
		// data port at which VTIDM is listening (usually 12345 ). The device
		// class argument indicates the device type, "cyberglove", "cybergrasp",
		// "flock" or "fastrak".  The last two arguments describe how the physical
		// device is connected to aHost.  If the connection is serial, this should
		// be "com1", "com2", "com3" or "com4".  For cybergrasp, the device is
		// "/dev/servo".  The rate parameter describes the connection speed. For
		// serial connections, this should be the speed of the hardware, typically
		// "115200".  For cybergrasp, this argument is NULL.
		vhtIOConn( char *connectString );
		//: Construct a VTIDM connection.
		// Use the connection string to build the connection.

		vhtIOConn(vhtIOConn *clonee);
		//: Copy constructor.

		virtual ~vhtIOConn(void);
		//: Delete.

		/* default methods */
		static vhtIOConn *getDefault(DeviceType aParam);
		//: Get a default VTIDM connection.
		// Returns the device connection parameter aParam.
		static vhtIOConn  *getDefault( const char *key );
		//: Get a registry device connection.
		// Builds a device connection based on the registry key. Not yet implemented.

		virtual char *getEntry(EntryKey aKey);
		//: Return specified entry.
		virtual void setEntry(EntryKey aKey, const char *aValue);
		//: Set specified entry.
};


#endif		/* _VHT_IO_CONN_H_ */


