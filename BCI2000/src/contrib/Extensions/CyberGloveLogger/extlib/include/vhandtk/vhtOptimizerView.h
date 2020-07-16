#ifndef VHT_OPTIMIZER_VIEW
#define VHT_OPTIMIZER_VIEW

#if defined(BUILD_WIN32_VH_DLL)

#if defined __BUILD_VH_CORE_DLL__
#define VH_CORE_DLL_EXPORT __declspec(dllexport)
#else
#define VH_CORE_DLL_EXPORT __declspec(dllimport)
#endif

#else 

#define VH_CORE_DLL_EXPORT

#endif

#include <Optimizer/opTraverse.h>
#include <Cosmo3D/csNode.h>

class vhtGroup;
class vhtTransformGroup;

//: Depreciated.
// The optimizer view class performs inorder (depth first) traversals
// of Cosmo scene graphs.
class VH_CORE_DLL_EXPORT vhtOptimizerView : public opDFTravAction
{
	public:
		virtual void begin( csNode *&, const opActionInfo & );

		virtual opTravDisp preNode( csNode *&, const opActionInfo & );
		virtual opTravDisp postNode( csNode *&, const opActionInfo & );
		virtual opActionDisp end(csNode*&, const opActionInfo&);


		inline vhtGroup *getVHTSceneGraph() { return (vhtGroup *)m_sgRoot; }

		inline void setScale( double scale ) { m_scale = scale; }

	private:
		vhtTransformGroup      *m_currXFormRoot;
		vhtTransformGroup      *m_sgRoot;
		double                  m_scale;
};


#endif


