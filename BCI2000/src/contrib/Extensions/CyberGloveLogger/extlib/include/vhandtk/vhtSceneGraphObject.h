/********************************************************************
 FILE: $Id: vhtSceneGraphObject.h,v 1.6 2002/08/28 23:16:23 jsilver Exp $
 AUTHOR: Ron Carmel
 DATE: Aug 1998

 DESCRIPTION:
  - 
 HISTORY:
  -
           -- COPYRIGHT VIRTUAL TECHNOLOGIES, INC. 1998 --
********************************************************************/

#ifndef __VHTSCENEGRAPHOBJECT_H
#define __VHTSCENEGRAPHOBJECT_H

#if defined(BUILD_WIN32_VH_DLL)

#if defined __BUILD_VH_CORE_DLL__
#define VH_CORE_DLL_EXPORT __declspec(dllexport)
#else
#define VH_CORE_DLL_EXPORT __declspec(dllimport)
#endif

#else 

#define VH_CORE_DLL_EXPORT

#endif

//: A base scene graph object.
// The virtual base class for all scene graph objects.  Implements reference counting.
//!vhtmodule: Core
class VH_CORE_DLL_EXPORT vhtSceneGraphObject
{
	public:
		vhtSceneGraphObject( void );
		//: Default constructor.

		vhtSceneGraphObject(vhtSceneGraphObject& sceneGraphObject);

		//: Copy constructor.
		virtual ~vhtSceneGraphObject(void);
		//: Destructor.

		inline bool isReferenced(void) {
			return (m_refCount>0);
		}
		//: Reference flag.

	protected:

		inline void                 refDec(void)                            {m_refCount--;}
		//: Reference decrement.
		inline void                 refInc(void)                            {m_refCount++;}
		//: Reference increment.
    
	private:

		// when an object is added to a scene graph, m_refCount
		int                         m_refCount;

};

#endif



