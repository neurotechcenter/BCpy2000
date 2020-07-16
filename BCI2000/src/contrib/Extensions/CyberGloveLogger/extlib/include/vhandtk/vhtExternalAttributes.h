#ifndef __VHT_EXTERNAL_ATTRIBUTES_H
#define __VHT_EXTERNAL_ATTRIBUTES_H
/**********************************************************************

FILE:    $Id: vhtExternalAttributes.h,v 1.7 2002/08/28 23:16:13 jsilver Exp $
AUTHOR:  C. Ullrich

<Header>
DESCRIPTION: external attributes.

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

//: External physical attributes.
// External attributes apply to nodes whose state is dictated by some
// external method.  Examples include sensors, files and derived states.  
//!vhtmodule: Core
class VH_CORE_DLL_EXPORT vhtExternalAttributes : public vhtPhysicalAttributes
{
	public:
		vhtExternalAttributes(void) : vhtPhysicalAttributes( ) {;}
		//: Default constructor.

		inline virtual Types getType(void) { return external; }
		//: Return attribute type.
		// See vhtPhysicalAttributes::Types.
		// For this class, the attribute type is external.
};


#endif


