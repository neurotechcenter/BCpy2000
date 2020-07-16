#ifndef VHT_HUMAN_HAND_ATTRIBUTES_H
#define VHT_HUMAN_HAND_ATTRIBUTES_H

#if defined(BUILD_WIN32_VH_DLL)

#if defined __BUILD_VH_CORE_DLL__
#define VH_CORE_DLL_EXPORT __declspec(dllexport)
#else
#define VH_CORE_DLL_EXPORT __declspec(dllimport)
#endif

#else 

#define VH_CORE_DLL_EXPORT

#endif

#include <vhtExternalAttributes.h>

//: Human Hand physical attributes
// Human Hand attributes apply to those nodes associated with a vhtHumanHand object.
//!vhtmodule: Core
class VH_CORE_DLL_EXPORT vhtHumanHandAttributes : public vhtExternalAttributes
{
	public:
		vhtHumanHandAttributes(void) : vhtExternalAttributes() {;}
		//: Default constructor.
    
		virtual Types getType(void) { return humanHand; }
		//: Return the attribute type.
		// See vhtPhysicalAttributes::Types.
		// For this class, the attribute type is humanHand.
};

#endif
