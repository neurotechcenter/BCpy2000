#ifndef VHT_MASS_PROPERTIES_H
#define VHT_MASS_PROPERTIES_H

/**********************************************************************

FILE:    $Id: vhtMassProperties.h,v 1.10 2002/08/28 23:16:18 jsilver Exp $
AUTHOR:  C. Ullrich

<Header>
DESCRIPTION: 
  mass properties of VClip geometries. This class automatically
  computes the center of mass and the moment of inertia matrix of
  a VClip convex hull.

HISTORY:

</Header>

              -- COPYRIGHT VIRTUAL TECHNOLOGIES, INC. 1998 --
 **********************************************************************/

#if defined(BUILD_WIN32_VH_DLL)

#if defined __BUILD_VH_CORE_DLL__
#define VH_CORE_DLL_EXPORT __declspec(dllexport)
#else
#define VH_CORE_DLL_EXPORT __declspec(dllimport)
#endif

#else 

#define VH_CORE_DLL_EXPORT

#endif

#include <vhtNodeComponent.h>
#include <vhtVector3d.h>

class vhtShape3D;

//: Mass properties.
// Storage of mass, center of mass vector and moment of inertia matrix
// for vhtNode objects.  A 6x6 inverse mass matrix is also available. 
//!vhtmodule: Core
class VH_CORE_DLL_EXPORT vhtMassProperties : public vhtNodeComponent
{
	public:
		vhtMassProperties(void);
		//: Construct default mass properties.
		vhtMassProperties( const vhtMassProperties &mass);
		//: Copy constructor.
		~vhtMassProperties(void);
		//: Destructor.


		inline double getMass(void) const
		{ 
			return m_mass; 
		}
		//: Get object mass.

		inline void setMass( double mass ) 
		{ 
			m_mass = mass;
			m_invMassUpToDate = false; 
		}
		//: Set object mass.
		//!param: mass - Mass of object.
 
		const inline vhtVector3d &getCenterOfMass() const
		{ 
			return m_cm; 
		}
		//: Get center of mass.
		// Center of mass is in object local co-ordinates.
	    inline void setCenterOfMass( const vhtVector3d &cm ) 
		{ 
			m_cm = cm; 
		}
		//: Set center of mass.
		//!param: cm - Center of mass in object frame.
		// Center of mass is in object local co-ordinates.

		inline void getMomentOfInertia( double MOI[3][3] ) const
		{ 
			for( unsigned int i =0; i < 3 ; i++ ) {
				for( unsigned int j =0; j < 3 ; j++ ) {
					MOI[i][j] = m_MOI[i][j];
				}
		    }
		}

		//: Get MOI matrix.
		//!param: MOI - Moment of Inertia matrix.
		// Return the 3x3 moment of inertia matrix for this object.  The MOI matrix is
		// in the object local co-ordinate frame.


		inline void setMomentOfInertia( double MOI[3][3] ) 
		{ 
			for( unsigned int i =0; i < 3 ; i++ ) {
				for( unsigned int j =0; j < 3 ; j++ ) {
					m_MOI[i][j] = MOI[i][j];
				}
			}
		}
	    //: Set the MOI matrix.
		//!param: MOI - Moment of Inertia matrix.

		void  computeHomogeneousMassProp( vhtShape3D *shape );
		//: Compute mass properties based on shape.
		//!param: shape - Associated shape.
		// Assumes that the associated geometry is vhtVertexGeometry.

	private:
		double              m_mass;            // total mass 
		vhtVector3d         m_cm;              // center of mass 
		double              m_MOI[3][3];       // moment of inertia 
		bool                m_invMassUpToDate;
};

#endif


