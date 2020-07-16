#ifndef VHT_FEEDBACK_GLOVE_H
#define VHT_FEEDBACK_GLOVE_H

/********************************************************************
 FILE: $Id: vhtFeedbackGlove.h,v 1.4 2002/08/28 23:16:14 jsilver Exp $
 AUTHOR: Chris Ullrich
 DATE: August 1998

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

#include "vht.h"
#include "vhtCyberGlove.h"

//: Placeholder feedback glove class.
//!vhtmodule: Device
class VH_CORE_DLL_EXPORT vhtFeedbackGlove : public vhtCyberGlove
{   
	public:
		inline vhtFeedbackGlove() {;}

		virtual inline vhtNodeType getType() { return VHT_FEEDBACK_GLOVE; }
};


#endif


