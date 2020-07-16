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
#ifndef IMPLANTAPI_EXTERNAL_UNIT_INFO_H
#define IMPLANTAPI_EXTERNAL_UNIT_INFO_H
#include <string>
#include <memory>

namespace cortec { namespace implantapi {

    /**
      * @brief Information on the external unit.
      */
    class CExternalUnitInfo
    {
    public:
        /**
          * @param[in] implantType type of the connected implant
          * @param[in] deviceId unique identifier of the external unit
          * @param[in] firmwareVersion version of the external device's firmware
          */
        CExternalUnitInfo(const std::string& implantType, const std::string& deviceId
            , const std::string& firmwareVersion);

        CExternalUnitInfo(const CExternalUnitInfo& other);

        virtual ~CExternalUnitInfo();

        /// @return type of the connected implant
        virtual std::string getImplantType() const;

        /// @return unique identifier of the external unit.
        virtual inline std::string getDeviceId() const;

        /// @return firmware version of the external unit
        virtual inline std::string getFirmwareVersion() const;

        CExternalUnitInfo& operator=(const CExternalUnitInfo& o);

        
    private:
        // forbidden
        CExternalUnitInfo();

        class CPimpl;
        std::unique_ptr<CPimpl> m_pimpl;
    };
}}

#endif //IMPLANTAPI_EXTERNAL_UNIT_INFO_H