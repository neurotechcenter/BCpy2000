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
#ifndef BICAPI_BICAPI_H
#define BICAPI_BICAPI_H

#include <windows.h>
#include "IImplantFactory.h"
#include "IStimulationCommandFactory.h"

#ifdef BIC_EXPORTS
#define BIC_API __declspec(dllexport)
#else
#define BIC_API __declspec(dllimport)
#endif


#ifdef __cplusplus
extern "C" {
#endif
    /// @return a factory for creation of IImplant instances. Caller takes ownership of the created pointer.
    BIC_API cortec::implantapi::IImplantFactory* APIENTRY createImplantFactory();
    
    /// @return a factory for creation of stimulation-related objects. Caller takes ownership of the created pointer.
    BIC_API cortec::implantapi::IStimulationCommandFactory*  APIENTRY createStimulationCommandFactory();
    
    /**
    * Function to query a string containing a unique identifier for the software version of this library.
    * After call succeeded, the version string is written as zero-terminated string into versionString.
    *
    * @param[in,out] versionString Pointer to a char array of length len. It has to be allocated by the caller.
    * @param[in]     len           Length of the character array 'versionString'. A length of 100 is sufficient, because
    *                              version strings are guaranteed to be shorter than 100 characters.
    *
    * @return -1 if error occurred (e.g., length of versionString > len or versionString == nullptr), 
    *            otherwise length of versionString (excluding zero terminator).
    */
    BIC_API ::int16_t APIENTRY getLibraryVersion(char* versionString, const ::uint16_t len);

#ifdef __cplusplus
}
#endif

#endif // BICAPI_BICAPI_H