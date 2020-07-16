#ifndef _VHT_HAND_GEOMETRY_H_
#define _VHT_HAND_GEOMETRY_H_
/********************************************************************
 FILE: $Id: vhtHandGeometry.h,v 1.12 2006/08/14 21:21:34 mbakircioglu Exp $
 AUTHOR: Hugo DesRosiers.
 DATE: 1999/03/15.

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

#include <vhtGenHandModel.h>

class vhtVrTransform;
class vhtVrShape;
class vhtTransform3D;

//: Visual hand geometry class.
// Store the visual representation of the hand geometry.  Hand geometry data
// is stored in a VRML subgraph and is accessable/modifiable by 
// decendants.
//!vhtmodule: Core
class VH_CORE_DLL_EXPORT vhtHandGeometry {
	public:
		enum Constants {
			maxNbrFingers= 5
		};
		//: Geometry related constants.
 

	private:
		static vhtVrTransform *buildPalm(double scale = 1.0);
		static vhtVrTransform *buildFingers(double scale = 1.0);
	   
	protected:
		vhtVrTransform *palm;
		//: Pointer to palm transform group.
		// Access the hand sub-graph from the palm node.
		unsigned int nbrFingers;
		//: Number of fingers.
		vhtVrTransform *fingers[maxNbrFingers];
		//: Pointers to the finger sub-graphs.
		// Access to each fingers geometry.
		bool rightGeometry;
		//: Flag for right/left hand.
		// rightGeometry = true implies a right hand geometry.

	public:
		vhtHandGeometry();
		//: Construct hand geometry.
		// Initialize data structures to default values.
		void useDefault(double scale =1);
		//: Use the default hand geometry.
		// Construct all geometry sub-graphs based on defaults.

		inline vhtVrTransform *getPalm(void) 
		{ 
			return palm; 
		}
		//: Return the palm sub-graph.
		// Return the palm transform group.

		inline unsigned int getFingerCount(void) 
		{ 
			return nbrFingers; 
		}
		//: Number of fingers.
		
		inline vhtVrTransform *getFinger(GHM::Fingers i) 
		{ 
			return ((unsigned int) i < (unsigned int) nbrFingers) ? fingers[i] : 0; 
		}
		//: Return the specified finger sub-graph.
		//!param: i - Finger id for sub-graph.
		// Returns the transform group at the top of the specified finger sub-graph.

		virtual void palmReform(vhtTransform3D *xform);
		//: Apply an arbitrary transform to the palm.
		//!param: xform - Apply a palm transformation.
		// Sets the palm transform to the specified xform.
		virtual void fingerReform(GHM::Fingers finger, GHM::Joints joint, vhtTransform3D *xform);
		//: Apply an arbitrary transform to the joint.
		//!param: finger - Id of finger to modify.
		//!param: joint - Id of joint to modify.
		//!param: xform - Transform to apply.
		// Sets the joint transform to the specified xform.

		virtual void makeLeft(void);
		//: Make this a left hand.
		// Re-constructs the geometry for a left hand.
};


#endif		/* _VHT_HAND_GEOMETRY_H_ */



