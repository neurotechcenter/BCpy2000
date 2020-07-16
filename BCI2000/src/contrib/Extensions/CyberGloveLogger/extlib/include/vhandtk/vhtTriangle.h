#ifndef _VHT_TRIANGLE_H
#define _VHT_TRIANGLE_H

#if defined(BUILD_WIN32_VH_DLL)

#if defined __BUILD_VH_CORE_DLL__
#define VH_CORE_DLL_EXPORT __declspec(dllexport)
#else
#define VH_CORE_DLL_EXPORT __declspec(dllimport)
#endif

#else 

#define VH_CORE_DLL_EXPORT

#endif

#include "vhtVector3d.h"

//: An implied triangle.
// 
class VH_CORE_DLL_EXPORT vhtTriangle
{
	public:
		vhtTriangle( void );
		~vhtTriangle( void );

		unsigned int  v1, v2, v3;
		vhtVector3d   normal;
};

#endif
