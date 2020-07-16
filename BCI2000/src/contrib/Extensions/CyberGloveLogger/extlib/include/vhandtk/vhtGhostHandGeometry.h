#ifndef VHT_GHOST_HAND_GEOMETRY_H
#define VHT_GHOST_HAND_GEOMETRY_H
/********************************************************************
 FILE: $Id: vhtGhostHandGeometry.h,v 1.6 2002/08/28 23:16:15 jsilver Exp $
 AUTHOR: Chris Ullrich
 DATE: March 9th, 1999.

 DESCRIPTION: Visual Ghost hand support geometry.

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

#include <vhtHandGeometry.h>
#include <vhtVector3d.h>

class vhtTransform3D;

//: A ghost hand geometry.
// Ghost hand geometry allows a user specified offset between the graphical
// hand and the haptic (ghost) hand.
//!vhtmodule: Core
class VH_CORE_DLL_EXPORT vhtGhostHandGeometry : public vhtHandGeometry
{
	public:
		vhtGhostHandGeometry(void);
		//: Constructor.
		// Set the default offset to {0,0,0}.

		virtual void palmReform( vhtTransform3D *xform );
		//: Apply an aribtrary transform to the palm.
		//!param: xform - Transformation to apply to palm.
		// Used during graphical rendering.  Will apply the current offset vector
		// to the hand geometry. xform is the current (haptic) palm transformation.
		virtual void setPalmOffset( vhtVector3d &offset );
		//: Set the offset vector.
		//!param: offset - Offset vector between physical and ghost.
		// Sets the offset between the haptic and graphical hand.

	protected:
		vhtVector3d            m_offset;
		//: Offset vector.
};

#endif


