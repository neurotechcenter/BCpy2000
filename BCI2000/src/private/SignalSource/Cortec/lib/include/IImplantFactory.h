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
#ifndef IMPLANTAPI_IIMPLANT_FACTORY_H
#define IMPLANTAPI_IIMPLANT_FACTORY_H
#include "ExternalUnitInfo.h"
#include "ImplantInfo.h"
#include "IImplant.h"
#include <vector>


namespace cortec { namespace implantapi {

/**
* @brief Device discovery and factory interface creating instances of IImplant.
*
* As braincon is intended as platform technology, there will be several different hardware
* abstraction layers implementations. The implementations of this factory interface are capable to
*
* - discover all external units and implants connected to the system which is recognized by the factory
* - create an instance of a suitable hardware abstraction layer implementation for a specified
*   external unit and implant combination.
*/
class IImplantFactory
{
public:
    virtual ~IImplantFactory() {}

    /**
    * Method for discovering the external units connected to this system recognized by the concrete
    * IImplantFactory object.
    *
    * @return list of all connected external units, empty list if no external units are currently connected.
    */
    virtual std::vector<CExternalUnitInfo*> getExternalUnitInfos() = 0;

    /**
    * Method for discovering the connected implant of an external unit.
    * The ownership of CImplantInfo is passed to the callee.
    *
    * @param[in] externalUnitInfo external unit information of the device of interest
    * @return ImplantInfo details on the connected implant
    * @throws CInvalidArgumentException if the factory is not responsible for implant type
    * @throws CRuntimeException if hardware revision or firmware version are not supported by this factory or
    *         if hardware info was out of date
    */
    virtual CImplantInfo* getImplantInfo(const CExternalUnitInfo& externalUnitInfo) = 0;

    /**
    * Determine if the factory is responsible for a certain external unit
    *
    * @param[in] externalUnitInfo Information describing the external unit.
    *
    * @return true if the factory can handle the type of the external unit, false otherwise
    */
    virtual bool isResponsibleFactory(const implantapi::CExternalUnitInfo& externalUnitInfo) const = 0;

    /**
    * Creates an instance of IImplant for the devices specified by implantId and externalUnitId.
    * The ownership of IImplant is passed to the callee.
    *
    * @param[in] externalUnitInfo external unit information, which the device is connected to
    * @param[in] implantInfo implantInformation of the implant that needs to be instantiated
    * @return an implant instantiated and initialized implant
    * @throws CRuntimeException if implantId or deviceId not found or if no suitable
    *         hardware abstraction layer exists
    */
    virtual IImplant* create(const CExternalUnitInfo& externalUnitInfo, const CImplantInfo& implantInfo) = 0;
};

}}


#endif //IMPLANTAPI_IIMPLANT_FACTORY_H