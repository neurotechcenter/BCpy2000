#ifndef __VHT_DYNAMIC_ATTRIBUTES_H
#define __VHT_DYNAMIC_ATTRIBUTES_H
/**********************************************************************

FILE:    $Id: vhtDynamicAttributes.h,v 1.7 2002/08/28 23:16:13 jsilver Exp $
AUTHOR:  C. Ullrich

<Header>
DESCRIPTION: Dynamic attributes.

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

#include <vhtPhysicalAttributes.h>
#include <vhtMassProperties.h>

//: Dynamic object attributes.
// Dynamic attributes are a type of physical attribute for dynamic objects.
// This attribute can be used to store mass and contact properties related to an object.
//!vhtmodule: Core
class VH_CORE_DLL_EXPORT vhtDynamicAttributes : public vhtPhysicalAttributes
{
	public:
		vhtDynamicAttributes(void);
		//: Default dynamic attributes.

		inline virtual bool isDynamic(void) { return true; }
		//: Always true for this class.

		inline virtual Types getType(void) { return dynamic; }
		//: Return attribute type.
		// See vhtPhysicalAttributes::Types.
		
		// Mass properties.
		inline const vhtMassProperties &getMassProperties(void) const
		{ 
			return m_massProperties; 
		}

		//: Return mass properties.	
		inline void setMassProperties( const vhtMassProperties &massProp )
		{ 
			m_massProperties = massProp;
			m_massPropertiesChanged = true; 
		}
		
		//: Set mass properties.
		//!param: massProp - Mass properties.
		// Set the mass properties for the associated node.
		inline bool getMassPropertiesChanged() const
		{ 
			return m_massPropertiesChanged; 
		}
		
		//: True if mass properties have changed.
		inline void setMassPropertiesChanged( bool mcf ) 
		{ 
			m_massPropertiesChanged = mcf; 
		}
		
		//: Acknowledge mass property change.

		// Object contact properties.
		inline void setStiffness( const double stiffness ) 
		{ 
			m_stiffness = stiffness; 
		}
		//: Set the patch normalized stiffness.
		//!param: stiffness - Stiffness of patch.
		// Stiffness is normalized to [0..1], where 0 is no stiffness,
		// and 1 is the maximum stiffness of the device.

		inline double getStiffness(void) const { return m_stiffness; }
		//: Get the patch normalized stiffness.
		// Default is .5.

		inline void setDamping( const double damping ) { m_damping = damping; }
		//: Set the patch normalized damping.
		//!param: damping - Damping of patch.
		// Damping is normalized to [0..1], where 0 is no damping,
		// and 1 is the maximum damping of the device.

		double getDamping(void) const { return m_damping; }
		//: Get the patch normalized damping.
		// Default is .5.

	protected:
		vhtMassProperties      m_massProperties;
		//: Mass properties.
		bool                   m_massPropertiesChanged;
		//: Mass properties changed flag.
		double                 m_stiffness;
		//: Object stiffness.
		double                 m_damping;
		//: Object damping.
};
#endif


