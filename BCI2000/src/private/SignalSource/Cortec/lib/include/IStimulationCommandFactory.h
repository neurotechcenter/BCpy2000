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
#ifndef IMPLANTAPI_ISTIMULATION_COMMAND_FACTORY_H
#define IMPLANTAPI_ISTIMULATION_COMMAND_FACTORY_H
#include "IStimulationCommand.h"
#include "IStimulationFunction.h"
#include "IStimulationAtom.h"
#include <stdint.h>


namespace cortec { namespace implantapi {

    /**
      * @brief Factory interface for creation of stimulation commands.
      *
      * An instance of IStimulationCommandFactory can be used to compose stimulation commands and stimulation functions.
      *
      * Typical usage: 
      * 1. Create empty stimulation command.
      * 2. Create empty stimulation function. Set repetition parameter.
      * 3. Create stimulation atoms and append them to function.
      * 4. Append stimulation function to command.
      * 5. Repeat steps 2. - 4. until all functions are created.
      *
      * @see IStimulationCommand
      * @see IStimulationFunction
      * @see IStimulationAtom
      */
    class IStimulationCommandFactory
    {
    public:
        virtual ~IStimulationCommandFactory() {}

        /**
          * @return Empty stimulation command. Caller is responsible for deletion of returned pointer.
          */
        virtual IStimulationCommand* createStimulationCommand() = 0;

        /**
          * @return Empty stimulation function with repetition count 1. Caller is responsible for deletion of returned 
          *         pointer.
          */
        virtual IStimulationFunction* createStimulationFunction() = 0;

        /** 
          * Creates a "Rectangular" stimulation atom. Each atom represents a constant amplitude for a fixed time 
          * duration.
          * 
          * Consider an implant that does voltage stimulation. Then createRectStimulationAtom(12, 2000000) would return 
          * a stimulation atom that stimulates for 2 seconds with an amplitude of 12 Volts. However, if this atom would
          * be sent to an implant that does current stimulation, this atom would elicit a stimulation with 12 
          * Milliampere for 2 seconds.
          *
          * @param[in] amplitude    Stimulation amplitude, either in V or mA.
          * @param[in] duration Duration in micros of the stimulation.
          * @return Initialized stimulation atom. Caller is responsible for deletion of returned pointer.
          */
        virtual IStimulationAtom* createRectStimulationAtom(const double amplitude, const uint64_t duration) = 0;

        /**
        * Creates a "Rectangular" stimulation atom with four different amplitudes. Each atom represents four constant
        * amplitudes for a fixed time duration.
        *
        * @param[in] amplitude0 first stimulation amplitude, either in V or mA.
        * @param[in] amplitude1 second stimulation amplitude, either in V or mA.
        * @param[in] amplitude2 third stimulation amplitude, either in V or mA.
        * @param[in] amplitude3 fourth stimulation amplitude, either in V or mA.
        * @param[in] duration   Duration in micros of the stimulation.
        * @return Initialized stimulation atom. Caller is responsible for deletion of returned pointer.
        */
        virtual IStimulationAtom* createRect4AmplitudeStimulationAtom(const double amplitude0, const double amplitude1,
            const double amplitude2, const double amplitude3, const uint64_t duration) = 0;
    };

}}

#endif //IMPLANTAPI_ISTIMULATION_COMMAND_FACTORY_H