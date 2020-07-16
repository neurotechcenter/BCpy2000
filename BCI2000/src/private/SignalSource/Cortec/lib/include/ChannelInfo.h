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
#ifndef IMPLANTAPI_CHANNEL_INFO_H
#define IMPLANTAPI_CHANNEL_INFO_H

namespace cortec { namespace implantapi {

    /**
      * Information on a single channel of an electrode.
      *
      * @see ChannelInfoEx for hardware-specific informations on channel.
      */
    class CChannelInfo
    {
    public:
        /**
          * Type of the electrical unit
          */
        enum class UnitType
        {
            UT_NO_UNIT = 0,
            UT_CURRENT,
            UT_VOLTAGE,
            UT_COUNT // number of units
        };

        /**
          * @param[in] canMeasure is true if the channel has measurement capabilities
          * @param[in] measureValueMin minimum value the channel can measure in voltage [V]
          * @param[in] measureValueMax maximum value the channel can measure in voltage [V]
          * @param[in] canStimulate is true if the channel has stimulation capabilities
          * @param[in] stimulationUnit physical SI unit of the stimulation value
          * @param[in] stimValueMin minimum in UnitType the electrode can emit for stimulation
          * @param[in] stimValueMax maximum in UnitType the electrode can emit for stimulation
          * @param[in] canMeasureImpedance is true if the channel has impedance measurement capabilities
          * \if INTERN_IMPLANTAPI
          *     @throw CInvalidArgumentException if measureValueMin > measureValueMax
          *     @throw CInvalidArgumentException if stimValueMin > stimValueMax
          * \else
          *     @throw std::exception if measureValueMin > measureValueMax
          *     @throw std::exception if stimValueMin > stimValueMax
          * \endif
          * @throw CInvalidArgumentException if measureValueMin > measureValueMax
          * @throw CInvalidArgumentException if stimValueMin > stimValueMax
          */
        CChannelInfo(const bool canMeasure, const double measureValueMin, const double measureValueMax,
                     const bool canStimulate, const UnitType stimulationUnit, const double stimValueMin,
                     const double stimValueMax, const bool canMeasureImpedance);

        /**
          * Copy constructor.
          */
        CChannelInfo(const CChannelInfo& o);

        virtual ~CChannelInfo() {}

        /// @return true if the electrode is capable of measure electrical signals
        virtual inline bool canMeasure() const { return m_canMeasure; }

        /// @return minimum value in voltage [V] the electrode can measure
        virtual inline double getMeasureValueMin() const { return m_measureValueMin; }

        /// @return maximum value in voltage [V] the electrode can measure
        virtual inline double getMeasureValueMax() const { return m_measureValueMax; }

        /// @return true if the electrode is capable of stimulation
        virtual inline bool canStimulate() const { return m_canStimulate; }

        /// @return unit of the stimulation value
        virtual inline UnitType getStimulationUnit() const { return m_stimulationUnit; }

        /// @return minimum value in UnitType the electrode can emit for stimulation
        virtual inline double getStimValueMin() const { return m_stimValueMin; }

        /// @return maximum value in UnitType the electrode can emit for stimulation
        virtual inline double getStimValueMax() const { return m_stimValueMax; }

        /// @return true if the electrode is capable of measure it's impedance
        virtual inline bool canMeasureImpedance() const { return m_canMeasureImpedance; }

    protected:
        bool m_canMeasure;
        double m_measureValueMin;
        double m_measureValueMax;
        bool m_canStimulate;
        UnitType m_stimulationUnit;
        double m_stimValueMin;
        double m_stimValueMax;
        bool m_canMeasureImpedance;

    private:
        // forbidden
        CChannelInfo();
    };

}}

#endif //IMPLANTAPI_CHANNEL_INFO_H