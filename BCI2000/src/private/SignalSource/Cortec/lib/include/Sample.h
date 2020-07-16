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
#ifndef IMPLANTAPI_SAMPLE_H
#define IMPLANTAPI_SAMPLE_H
#include <stdint.h>
#include <memory>


namespace cortec { namespace implantapi {

    /**
      * @brief Class for measurement data read by the implant at one point in time, i.e. for one sample.
      */
    class CSample
    {
    public:
        /// Creates an empty sample.
        CSample();
        
        /**
        * @param[in] numberOfMeasurements number of channels used for measuring. Array measurements must be of this
        *                                 size.
        * @param[in] measurements         array of measurements. This array must be of size numberOfMeasurements. Note: 
        *                                 The ownership of measurements is passed to CSample
        * @param[in] supplyVoltage        supply voltage in mV.
        * @param[in] isConnected          true if the implant is connected (all channels available)
        * @param[in] stimulationNumber    id of the stimulation which starts with this sample. If no stimulation started
        *                                 with this sample, then the stimulationNumber is Sample::c_noStimuation.
        * @param[in] m_stimulationActive  true if the stimulation is started
        */
        CSample(const uint16_t numberOfMeasurements
            , double* measurements
            , const uint32_t supplyVoltage
            , const bool isConnected
            , const uint16_t stimulationNumber
            , const bool m_stimulationActive);

        /**
         * @param other The other sample to copy from
         */
        CSample(const CSample& other);

        virtual ~CSample();

        /// @return Number of elements in measurements
        virtual uint16_t getNumberOfMeasurements() const;

        /**
        * @return array of measurements in system units. Caller is responsible for deletion of this pointer.
        */
        virtual double* getMeasurements() const;

        /// @return supply voltage
        virtual uint32_t getSupplyVoltage() const;

        /// @return true if the implant is connected
        virtual bool isConnected() const;

        /** 
          * Id of the stimulation which starts with this sample. If no stimulation started with this sample, then the 
          * stimulationNumber is Sample::c_noStimuation.
          */
        virtual uint16_t getStimulationId() const;

        /// @return true during on going stimulation
        virtual bool isStimulationActive() const;

        /// value of m_stimulationNumber for samples during which no stimulation starts.
        static const uint16_t c_noStimulation;

    private:
        // prohibited        
        CSample& operator=(const CSample& other);

        class CPimpl;
        std::unique_ptr<CPimpl> m_pimpl;
    };

}}


#endif //IMPLANTAPI_SAMPLE_H