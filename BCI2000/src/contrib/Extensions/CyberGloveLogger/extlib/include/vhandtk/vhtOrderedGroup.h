/********************************************************************
 FILE: $Id: vhtOrderedGroup.h,v 1.4 2002/08/28 23:16:21 jsilver Exp $
 AUTHOR: Ron Carmel
 DATE: Aug 1998

 DESCRIPTION:
  -
 HISTORY:
  -
           -- COPYRIGHT VIRTUAL TECHNOLOGIES, INC. 1998 --
********************************************************************/

#define __VHTORDEREDGROUP_H
#define __VHTORDEREDGROUP_H

#if defined(BUILD_WIN32_VH_DLL)

#if defined __BUILD_VH_CORE_DLL__
#define VH_CORE_DLL_EXPORT __declspec(dllexport)
#else
#define VH_CORE_DLL_EXPORT __declspec(dllimport)
#endif

#else 

#define VH_CORE_DLL_EXPORT

#endif

#include "vhtGroup.h"

//: Depreciated.
class VH_CORE_DLL_EXPORT vhtOrderedGroup: public vhtGroup {
	public:
		vhtOrderedGroup();
		vhtOrderedGroup(vhtOrderedGroup &orderedGroup);
	    virtual ~vhtOrderedGroup();

		virtual vhtNode cloneNode(bool forceDuplicate);
		virtual void duplicateNode(vhtNode &originalNode, bool forceDuplicate);
		virtual int  serialize(char *buf, int bufsize, int startpos=0);

};

//#endif


