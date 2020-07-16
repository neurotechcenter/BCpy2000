#ifndef VHT_CF_HUMAN_HAND_H
#define VHT_CF_HUMAN_HAND_H


#if defined(BUILD_WIN32_VH_DLL)

#if defined __BUILD_VH_CORE_DLL__
#define VH_CORE_DLL_EXPORT __declspec(dllexport)
#else
#define VH_CORE_DLL_EXPORT __declspec(dllimport)
#endif

#else 

#define VH_CORE_DLL_EXPORT

#endif

#include "vhtHumanHand.h"
#include "vhtGenHandModel.h"
#include "vhtContactPatch.h"
#include <vhtHumanFinger.h>
#include <vhtPhalanx.h>


class vhtHandMaster;
class vhtCyberGrasp;


class VH_CORE_DLL_EXPORT vhtCFHumanHand : public vhtHumanHand
{

public:
    vhtCFHumanHand( vhtHandMaster *aMaster, vhtCyberGrasp *aGrasp, double scale=1.0, GHM::Handedness h= GHM::rightHand )  : vhtHumanHand( aMaster, aGrasp, scale,h ) {};

    virtual const vhtContactPatch * selectContactPatch( GHM::Fingers finger );
    virtual void                    updateCyberGrasp( void );
protected:

    virtual void  removeHapticRoot( vhtContactPatch *_aPatch );
};

#endif
