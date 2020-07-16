#ifndef VHT_OPTIMIZER_H
#define VHT_OPTIMIZER_H

#if defined(BUILD_WIN32_VH_DLL)

#if defined __BUILD_VH_CORE_DLL__
#define VH_CORE_DLL_EXPORT __declspec(dllexport)
#else
#define VH_CORE_DLL_EXPORT __declspec(dllimport)
#endif

#else 

#define VH_CORE_DLL_EXPORT

#endif

#include "vhtOptimizerView.h"
#include <vhtNodeTypes.h>
class vhtGroup;
class vhtNode;

class VH_CORE_DLL_EXPORT vhtOptimizer
{
	public:
		vhtOptimizer();
		~vhtOptimizer();

		static vhtGroup *readFile( const char *filename, double scale = 1.0 );
		static vhtNode  *getFirst( vhtGroup *root, vhtNodeType type );

};

#endif


