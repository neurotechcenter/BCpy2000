/**********************************************************************
* Copyright 2015-2018, CorTec GmbH
* All rights reserved.
*
* Redistribution, modification, adaptation or translation is not permitted.
*
* CorTec shall be liable a) for any damage caused by a willful, fraudulent or grossly 
* negligent act, or resulting in injury to life, body or health, or covered by the 
* Product Liability Act, b) for any reasonably foreseeable damage resulting from 
* its breach of a fundamental contractual obligation up to the amount of the 
* licensing fees agreed under this Agreement. 
* All other claims shall be excluded. 
* CorTec excludes any liability for any damage caused by Licensee’s 
* use of the Software for regular medical treatment of patients.
**********************************************************************/
#ifndef BICAPI_BIC3232CONSTANTS_H
#define BICAPI_BIC3232CONSTANTS_H

#include <stdint.h>

#ifdef BIC_EXPORTS
#define BIC_API __declspec(dllexport)
#else
#define BIC_API __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C"
{
#endif
/// Struct holding constants specific to implants of type BIC3232.
struct BIC3232Constants
{
    /**
    * Channel index for ground electrode. This constants can be used to set the ground electrode as destination in
    * a stimulation function.
    *
    * @see IStimulationFunction::setVirtualElectrodes()
    */
    BIC_API static const ::uint32_t c_groundElectrode;
};


#ifdef __cplusplus
}
#endif

#endif // BICAPI_BIC3232CONSTANTS_H