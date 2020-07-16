#ifndef _VHT_GEN_HANDMODEL_H_
#define _VHT_GEN_HANDMODEL_H_

/********************************************************************
 FILE: $Id: vhtGenHandModel.h,v 1.13 2002/08/28 23:16:14 jsilver Exp $
 AUTHOR: Hugo DesRosiers.
 DATE: 1999/03/03.

 DESCRIPTION: Definitions that make up our generic hand model.

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

#define VHT_DEFAULT_FINGERS	    GHM::nbrFingers
#define VHT_DEFAULT_LINKS	    GHM::nbrLinks
#define VHT_DEFAULT_JOINTS	    GHM::nbrJoints


//: Generic Hand Model class.
// Collect various enumerated types and indices into one central location.  Users
// of the VHT should always use the types defined in this class to index hand
// parts, and transforms.
//!vhtmodule: Device

class VH_CORE_DLL_EXPORT GHM {
	public:
		
		enum Handedness { rightHand, leftHand };
		//: The handedness of the human hand.

		enum Quantities {
			nbrFingers= 5, 
			nbrLinks= 3, 
			nbrJoints= 3
		};
		//: Basic human specific hand data.

		enum Fingers {
			thumb= 0, index, middle, ring, pinky
		};
		
		//: Each of the fingers.
		enum HandParts {
			palm= pinky+1
		};
		//: The palm is an additional hand part.
		
		enum Joints {
			metacarpal=0,
			proximal=1, 
			distal=2, 
			fingerTip=3, 
			abduct= fingerTip, 
			palmJoint=-1,
			palmArch = metacarpal,
			wristFlexion = proximal,
			wristAbduction = distal
		};
		
		//: The various joints.
		enum wrist {
			wristPitch=1, wristYaw=2
		};
		//: Components of the wrist transformation.
		
		enum Rotations {
			yaw, pitch, roll
		};
		//: Euler rotation types.
 
		enum Columns {
			u, v, w, p
		};
		//: Homogeneous matrix column ids.

		enum Components {
			x, y, z
		};
		//: Cartesian components.
};


#endif		/* _VHT_GEN_HANDMODEL_H_ */


