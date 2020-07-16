#ifndef _VHT_KINEMATICS_H_
#define _VHT_KINEMATICS_H_

/********************************************************************
 FILE: $Id: vhtKinematics.h,v 1.19 2006/08/14 21:23:12 mbakircioglu Exp $
 AUTHOR: 
 DATE: 

 DESCRIPTION:

 HISTORY:
 
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


#include <vhtTransform3D.h>
#include <vhtVector3d.h>
#include <vhtGenHandModel.h>


class vhtHandMaster;
class vhtTracker;
class vhtGlove;

//: Hand kinematics class.
// This class is for internal use only.
//!vhtmodule: Core
class VH_CORE_DLL_EXPORT vhtKinematics
{
	protected:	
		//
		// Instance variables accessible by subclasses.
		//

		// Hand events data source.
		vhtHandMaster *m_master;    
    
		// tracker values
		vhtVector3d    m_trackerPosition;
		vhtVector3d    m_trackerRotation;
		vhtVector3d    m_trackerOffset;
    
		// Internal constants.
		vhtVector3d    m_zAxisV;	

		// Kinematics transforms.
		vhtTransform3D m_kinematics[ VHT_DEFAULT_FINGERS ][ VHT_DEFAULT_LINKS +1 ];
		vhtTransform3D m_wristXForm;
		vhtTransform3D m_world2Palm;
		vhtTransform3D m_tracker2Palm;

		// Geometry,
		vhtTransform3D m_geometry         [ VHT_DEFAULT_FINGERS ][ VHT_DEFAULT_LINKS +1 ];
		vhtVector3d    m_geometryVector   [ VHT_DEFAULT_FINGERS ][ VHT_DEFAULT_LINKS +1 ];
		double         m_geometryThickness[ VHT_DEFAULT_FINGERS ][ VHT_DEFAULT_LINKS +1 ];
		vhtVector3d    m_thumbRollVector;	
		vhtVector3d    m_thumbMCJVector; // TR axis
		double         m_thumbRollAngle;
		double         m_thumbUnflexedAbduction;
		vhtVector3d    m_wristOffset;


		vhtTransform3D  m_handGeoTransform;
		GHM::Handedness m_handedness;
		double m_handedness_factor;

		bool            m_useGTP;
		bool            m_useHWS;
		
		char pad[2];

		double m_scale;


	protected:		
		//
		// Instance methods accessible by subclasses.
		//

		// Build/maintain default kinematic structure.
		virtual void setDefaultGeometryVectors(void);
		virtual void setDefaultGeometryTransforms(void);
		virtual void updateDefaultKinematics(void);
		virtual void updateTrackerKinematics( void );

	public:
		//
		// Instance methods
		//

		// Constructors.
		vhtKinematics(vhtHandMaster *, GHM::Handedness = GHM::rightHand, double scale =1.0);
		vhtKinematics(vhtGlove *, vhtTracker *, GHM::Handedness = GHM::rightHand, double scale = 1.0);
 
		// Destructors.
		~vhtKinematics(void);

		// Data accessors.
		virtual vhtTransform3D & getKinematics(int finger, int joint = 0 );
		virtual vhtTransform3D & getWristTransform( void );
		virtual vhtTransform3D & getGeometry(int finger, int joint = 0 );
		
		virtual const vhtVector3d & getTrackerPosition(void);	// tracker offset
		virtual const vhtVector3d & getTrackerOffset(void);

		virtual vhtHandMaster  * getHandMaster(void);
		
		virtual void setTrackerOffset( const vhtVector3d &offset );

		double getThumbRoll(void){return m_thumbRollAngle;};

		// set stuff
		virtual void setHandedness(GHM::Handedness h);

		// Specialized functionality.
		virtual void update( bool updateDevices = true );

		// get fingertip angle wrt mcp base
		double getAlpha(int finger);

		// tracker location
		virtual void useGraspTrackerPosition( bool useGTP );

		// Haptic WorkStation
		virtual void useHapticWorkStation( bool useHWS );
};


#endif		/* _VHT_KINEMATICS_H_ */



