#ifndef _VHT_6DOF_DEVICE_H_
#define _VHT_6DOF_DEVICE_H_
/********************************************************************
 FILE: $Id: vht6DofDevice.h,v 1.9 2002/08/28 23:16:05 jsilver Exp $
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

class vhtTracker;
class vhtTrackerData;
class vhtTransform3D;
//: 6 degree of freedom input device abstraction.
// Device independent 6 degreee of freedom input source.  In the VHT
// objects of this class are used to represent receivers associated
// with vhtTracker classes. See the method vhtTracker::getLogicalDevice.
//!vhtmodule: Device

class VH_DEVICE_DLL_EXPORT vht6DofDevice
{
  public:		
    enum Freedom {
		xPos= 0, yPos, zPos, xAngle, yAngle, zAngle
	};
    //: Degree of freedom.

	enum AngleFormat {
		ZYXEuler = 0, ExpMap
	};

  protected:		
    vhtTracker *owner;
    //: Tracker which owns this 6DOF device.
    vhtTrackerData *data;
    //: Current tracker data.
    
    vht6DofDevice( void );    // having been called.  
    //: Null constructor.

	enum AngleFormat   m_angleFormat;
  public:		
    vht6DofDevice(vhtTracker *anOwner);
    //: Construct a 6DofDevice corresponding to a pre-existing tracker.
    //!param: anOwner - All 6Dof devices must have a specified owner.
    // All degrees of freedom are set to 0 until the first update call.

    virtual ~vht6DofDevice(void);

    // methods
    virtual void update(void);
    //: retrieve most recent data.
    // Updates the internal data buffers with the most recent data
    // from the external device. Calling this method on a receiver
    // that is associated with a tracker that has multiple receivers
    // will result in all receivers being updated.

    // Data accessors.
    virtual vhtTracker *getTracker(void);
    //: Get the corresponding tracker.
    // Get the tracker connected to this receiver.
    virtual double getRawData(Freedom aDegree);
    //: Get a particular data value.
    //!param: aDegree - The degree of freedom to retrieve.
    // Get the data value corresponding to the specified degree of freedom.
    virtual void   setRawData(Freedom aDegree, double data );
    //: Set the input data explictly.
    //!param: aDegree - The degree of freedom to set.
    //!param: data - The value of the degree of freedom.
    // Use this method to override the actual data from the external device.
    // This is useful if the device is missing or is mapped to another input device.

    virtual vhtTrackerData *getSensorArray(void);
    //: Return a full set of tracker data.
    // Return the most recently updated tracker data.  The update method must
    // be called before fresh data can be retrieved.

    virtual void getTransform( vhtTransform3D *xform, bool rotationOnly = false );
    //: Constructs the world frame tracker transform.
    //!param: xform - The vhtTransform3D that will contain the tracker frame.
    //!param: rotationOnly - Set to true if only the rotation component is needed.
    // This convenience method converts the translation and ZYX Euler parameters 
    // from the device directly into a homogeneous transformation (scale = 1). Note
    // that xform is overwritten.

	virtual enum AngleFormat getAngleFormat( void );

	virtual void setAngleFormat( enum AngleFormat _newFmt );
};


#endif		/* _VHT_6DOF_DEVICE_H_ */
