#ifndef VHT_HAND_MASTER_H
#define VHT_HAND_MASTER_H

/********************************************************************
 FILE: $Id: vhtHandMaster.h,v 1.7 2002/08/28 23:16:16 jsilver Exp $
 AUTHOR: Chris Ullrich.
 DATE: August 1998

 DESCRIPTION:

 HISTORY:
	1999/03/03: Rewritten for a more general model of glove/tracker utilization.
 
           -- COPYRIGHT VIRTUAL TECHNOLOGIES, INC. 1998 --
********************************************************************/

#if defined(BUILD_WIN32_VH_DLL)

#if defined __BUILD_VH_CORE_DLL__
#define VH_CORE_DLL_EXPORT __declspec(dllexport)
#else
#define VH_CORE_DLL_EXPORT __declspec(dllimport)
#endif

#else 

#define VH_CORE_DLL_EXPORT

#endif


class vhtGlove;
class vhtTracker;
class vhtIOConn;
class vht6DofDevice;

//: HandMaster abstracts a glove/tracker combination.
// HandMaster is a container class for a glove and tracker pair.
// Many other classes take a vhtHandMaster argument to simplify device
// management.
//!vhtmodule: Device
class VH_CORE_DLL_EXPORT vhtHandMaster {

	private:
		vhtGlove      *glove;
		vht6DofDevice *trackerRcvr;
		vhtTracker    *m_tracker;
		bool           m_isDeviceOwner;

	public:	
		vhtHandMaster(void);
		//: Construct a default HandMaster with no connection.
		vhtHandMaster(vhtIOConn *gloveConn, vhtIOConn *trackerConn);
		//: Construct a HandMaster using specified glove and tracker.
		// Construct a HandMaster with glove and tracker connections
		// specified.
		vhtHandMaster(vhtGlove *, vhtTracker *);
		//: Construct a HandMaster using pre-existing glove and tracker.
		// Construct a HandMaster with already connected glove and tracker.
		vhtHandMaster(vhtGlove *, vht6DofDevice *);
		//: Construct a HandMaster using pre-existing glove and receiver.
		// Construct a HandMaster with already connected glove and receiver.

		~vhtHandMaster(void);
		//: Disconnect and delete.

		virtual vhtGlove * getGlove(void);
		//: Get current glove.
		virtual void setGlove(vhtGlove *);
		//: Set current glove.
		virtual vht6DofDevice *getReceiver(void);
		//: Get current receiver.
		virtual vhtTracker *getTracker(void);
		//: Get current tracker.
		virtual void setReceiver( vht6DofDevice *rcvr );
		//: Set current receiver.
		virtual void setTracker(vhtTracker *);
		//: Set current tracker.
		// By default, the first receiver is used.

		virtual void update(void);
		//: Update glove and tracker from devices.
		// Updates the internal data for both glove and
		// tracker by calling their update methods.
};

#endif


