#ifndef VHT_DEBUG_UTILITY_H
#define VHT_DEBUG_UTILITY_H

#if defined(BUILD_WIN32_VH_DLL)

#if defined __BUILD_VH_CORE_DLL__
#define VH_CORE_DLL_EXPORT __declspec(dllexport)
#else
#define VH_CORE_DLL_EXPORT __declspec(dllimport)
#endif

#else 

#define VH_CORE_DLL_EXPORT

#endif

#include <vhtNode.h>

//: Debug utility.
// Various debugging utilities.
//!vhtmodule: Core
class VH_CORE_DLL_EXPORT vhtDebugUtility
{
	public:

		static void printSubTree( vhtNode *subTree, int indentLength = 0 );
		//: Print a sub-tree to stdout.
		//!param: subTree - Subtree to print.
		//!param: indentLength - Number of spaces to prepend.

		static void printHSGToFile( vhtNode *hsg, const char *filename );
};

#endif


