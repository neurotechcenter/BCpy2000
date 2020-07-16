

/********************************************************************
 FILE: $Id: vhtCyberGraspKin.h,v 1.14 2002/08/28 23:16:11 jsilver Exp $
 AUTHOR: Daniel Gomez
 DATE: June 1998

 DESCRIPTION:

 HISTORY:
 
           -- COPYRIGHT VIRTUAL TECHNOLOGIES, INC. 1998 --
********************************************************************/

//: Provide additional kinematic information for CyberGrasp.
// vhtGraspKinematics calculates the polar coordinates of the 
// fingertip Hard Point (HP). The coordiantes are with respect
// to the mainCam Coordinate System (CS)
 
/*
// mainCam CS
// the mainCam (MC) coordinate system has origen at the MCJ joint,
// it abducts (with the finger and cybergrasp main cam) but does not flex. 
// It z axis "coincides" with the cam and MCJ abduction axis (thus we 
// assumed that the MCJ abduction and the cam abduction axis coincide), 
// the z axis points out twards the back of the hand.
// The y axis points along the finger towards the PIJ joint.
// The mainCam to palm thasformation is given by m_mainCam[finger]
// (see my black notebook pages 23 24 25)

// About the grasp thumb model
// the two trasfomations  
//      handKPIJ =  kinematics[VHT_THUMB][VHT_MCJ] * kinematics[VHT_THUMB][VHT_PIJ];
//
//      graspKPIJ =  mainCam[VHT_THUMB] * flexedMCJ[VHT_THUMB] * thumbGraspPIJ;
//
// are such that handKPIJ = graspKPIJ. This corespondto the tranfomation
// from the palm to the pij. 
// In the grasp kinematics model the thumb roll is applied to the MCJ CS
// In this way the thumb has a kinematics model that is equivalent to those
// of the other 4 fingers.
// In grasp kinematics thumbGraspPIJ replaces kinematics[VHT_THUMB][VHT_PIJ]
*/
#ifndef _VHTGRASPKINEMATICS_
#define _VHTGRASPKINEMATICS_

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
#include <vhtKinematics.h>

//: CyberGrasp kinematics.
// This class is for interal use only.
//!vhtmodule: Core
class VH_CORE_DLL_EXPORT vhtCyberGraspKin : public vhtKinematics
{
	public:
		
		vhtCyberGraspKin( vhtHandMaster * handMaster ) :  vhtKinematics( handMaster ) 
		{
			setDefaultHardPoints();
		};

		//: Construct a CyberGrasp Kinematics with given handMaster.
		// Construct Kinematics and CyberGraspKin related data structures.
		
		vhtCyberGraspKin( vhtGlove * glove, vhtTracker * tracker ) : vhtKinematics( glove, tracker ) 
		{
			setDefaultHardPoints();

		};
  
		//: Construct a CyberGrasp Kinematics with given glove and tracker.
		// Construct Kinematics and CyberGraspKin related data structures.

    
		virtual void   update( bool updateDevices = true )
		{
			vhtKinematics::update( updateDevices );
			buildKinematics( );
			calculateFtpHpWorldTransform( );
		}

		//: Update the CyberGrasp kinematics.
		// Updates all kinematic structures using most recent data.

		vhtTransform3D getMainCam(int finger)
		{
			return( m_mainCam[ finger ] );
		}
		
		//: Get transformation to the mainCam co-ordinate system.
		// Get transformation to the mainCam co-ordinate system for specified finger.  
		// This transformation is in the world frame.
		
		double getAlpha(int finger)
		{
			return m_ftpHardPointAlpha[ finger ];
		}

		//: Get the alpha angle for finger.
		// Get the alpha angle for finger.  The alpha angle is the angle wrt the y-axis
		// in the mainCam co-ordinate system of the fingertip. An angle of 0 corresponds
		// to an unflexed finger.
		double getRadius(int finger)
		{
			return m_ftpHardPointRadius[ finger ];
		}

		//: Get the fingertip radius.
		// Returns the distance from the mainCam co-ordinates system to the fingertip.

		vhtTransform3D m_flexedMCJ[ VHT_DEFAULT_FINGERS ];

		//: The flexed MCJ transform.
		// Needs comment.

		vhtTransform3D m_thumbGraspPIJ;
		//: The thumb grasp PIJ transform.
		// Needs comment.
  
		void calculateHardPointPositions( void );
		const vhtVector3d &getHardPointWorldPosition    ( unsigned int jointIndex );
		const vhtVector3d &getPalmHardPointWorldPosition( unsigned int palmHardPointIndex );
		const vhtVector3d &getHardPointPalmPosition     ( unsigned int jointIndex );
		const vhtVector3d &getPalmHardPointPalmPosition ( unsigned int palmHardPointIndex );



	private:

		void           buildKinematics(void);

		void           setDefaultHardPoints(void);

		double         m_thumbFlex;

		vhtTransform3D m_mainCam[ VHT_DEFAULT_FINGERS ];

		vhtTransform3D m_ftpHp2MainCam[ VHT_DEFAULT_FINGERS ];

		void           calculateFtpHpPosition(void);
		void           calculateFtpHpWorldTransform( void );

		// position fo the ftp hard point in the MainCam CS
		vhtVector3d    m_ftpHardPointPosition[ VHT_DEFAULT_FINGERS ];

		// polar version of the above
		// alpha is measure from the y axis to the ftpHPP vector
		// on the y-z plane. That is, a straight finger has alpha = 0.0.
		double         m_ftpHardPointAlpha[ VHT_DEFAULT_FINGERS ];
		double         m_ftpHardPointRadius[ VHT_DEFAULT_FINGERS ];

		// the tranfmorm to the fingertip and the corresponding position vector
		vhtTransform3D m_ftpHp2WorldTransform[ VHT_DEFAULT_FINGERS ];
		vhtVector3d    m_hardPointWorldPos[ VHT_DEFAULT_FINGERS ][ VHT_DEFAULT_JOINTS ];
		vhtVector3d    m_palmHardPointWorldPos[4];
		vhtVector3d    m_hardPointPalmPos[ VHT_DEFAULT_FINGERS ][ VHT_DEFAULT_JOINTS ] ;
		vhtVector3d    m_palmHardPointPalmPos[4];

		// a cached transform for general use
		vhtTransform3D m_TempTransform;

		vhtTransform3D m_fingerHardPoint[ VHT_DEFAULT_FINGERS ][ VHT_DEFAULT_FINGERS ];
		vhtTransform3D m_palmHardPoints[ 4 ];
		double         m_fingerHardPointSize[ VHT_DEFAULT_FINGERS ][ VHT_DEFAULT_FINGERS ];
		double         m_palmHardPointSize[ 4 ];
		double         m_fingerLength[ VHT_DEFAULT_FINGERS ];
  
};

#endif






