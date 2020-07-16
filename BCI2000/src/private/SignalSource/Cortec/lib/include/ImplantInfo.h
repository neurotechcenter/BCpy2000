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
#ifndef IMPLANTAPI_IMPLANT_INFO_H
#define IMPLANTAPI_IMPLANT_INFO_H
#include "ChannelInfo.h"
#include <vector>
#include <string>
#include <stdint.h>


namespace cortec { namespace implantapi {

    /**
      * @brief Information on the implant including channel info.
      */
    class CImplantInfo
    {
    public:
        /**
          * @param[in] deviceType type of the implant
          * @param[in] deviceId is the unique identifier of the implant
          * @param[in] firmwareVersion version of the implant's firmware
          * @param[in] samplingRate of the measurement
          * @param[in] channelInfo provides information of the capabilities of each channel
          *            the ownership is passed to CImplantInfo
          * @throws CInvalidArgumentException if channelInfo contains one or more nullptr elements
          */
        CImplantInfo(const std::string& deviceType
            , const std::string& deviceId
            , const std::string& firmwareVersion
            , const uint32_t samplingRate
            , const std::vector<CChannelInfo*>& channelInfo
            );

        /**
          * Copy constructor
          */
        CImplantInfo(const CImplantInfo& o); 

        virtual ~CImplantInfo();

        /// @return firmware version
        virtual std::string getFirmwareVersion() const;

        /// @return type of the implant
        virtual std::string getDeviceType() const;

        /// @return unique implant identifier
        virtual std::string getDeviceId() const;

        /// @return information of the capabilities of each channel
        virtual const std::vector<CChannelInfo*>& getChannelInfo() const;

        /// @return total number of channels 
        virtual size_t getChannelCount() const;

        /// @return number of channels with measurement capabilities
        virtual size_t getMeasurementChannelCount() const;

        /// @return number of channels with stimulation capabilities
        virtual size_t getStimulationChannelCount() const;

        /// @return measurement sampling rate
        virtual uint32_t getSamplingRate() const;

        CImplantInfo& operator=(const CImplantInfo& other);

    protected:
        class CPimpl;
        CPimpl* m_pimpl;

    private:
        CImplantInfo();
    };

}}

#endif //IMPLANTAPI_IMPLANT_INFO_H