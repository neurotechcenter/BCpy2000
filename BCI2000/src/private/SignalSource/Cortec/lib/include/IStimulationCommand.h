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
#ifndef IMPLANTAPI_ISTIMULATION_COMMAND_H
#define IMPLANTAPI_ISTIMULATION_COMMAND_H
#include "IIterator.h"
#include "IStimulationFunction.h"
#include <stdint.h>


namespace cortec { namespace implantapi {

    /**
      * @brief Interface for classes that can be sent to an implant to elicit electrical stimulation.
      *
      * This is a generic interface that is intended to be used with different types of implants with different 
      * stimulation capabilities each. Stimulation capabilities may vary regarding the number of channels used for 
      * stimulation, stimulation amplitude, form of the stimulation signal over time. To reflect this, a stimulation
      * command consists of a sequence of stimulation functions. A stimulation function can be composed arbitrarily
      * complex.
      *
      * Typical usage of an IStimulationCommand:
      * 1. Create an empty stimulation command instance.
      * 2. Repeatedly add IStimulationFunction instances.
      * 3. Send IStimulationCommand instance to implant by calling IImplant::startStimulation().
      *
      * @see IImplant
      * @see IStimulationFunction
      */
    class IStimulationCommand
    {
    public:
        virtual ~IStimulationCommand() {}

        /**
          * Append a stimulation function. The sequence of appends defines the sequence of execution.
          * The ownership of IStimulationFunction is passed to the IStimulationCommand if this method call succeeds 
          * (i.e., does not throw and exception).
          *
          * \if INTERN_IMPLANTAPI
          *     @throws CInvalidArgumentException if duration is 0 microseconds or function is a nullptr
          * \else
          *     @throws std::exception if duration is 0 or function is a nullptr
          * \endif
          */
        virtual void append(IStimulationFunction* function) = 0;

        /**
          * Return iterator that can be used to iterate through all functions currently contained in the 
          * IStimulationCommand.
          *
          * The ownership of the iterator is passed to the caller.
          *
          * @return iterator pointing to the first IStimulationFunction.
          */
        virtual IIterator<IStimulationFunction>* getFunctionIterator() const = 0;

        /**
          * Return function iterator that can be used to iterate through all functions. In contrast
          * to getFunctionIterator the iterator is fully aware of repetitions. For example, if a function has n 
          * repetitions, then the iterator will return the next stimulation function after n calls to getNext().
          *
          * The ownership of the iterator is passed to the caller.
          * 
          * @return repetition aware iterator pointing to the first IStimulationFunction
          */
        virtual IIterator<IStimulationFunction>* getRepetitionAwareFunctionIterator() const = 0;

        /**
          * @return the total duration of the stimulation command in microseconds.
          */
        virtual uint64_t getDuration() const = 0;

        /**
        * Set the name of the command.
        */
        virtual void setName(const std::string& commandName) = 0;

        /**
        * @return the set name of the command. If the command name was not set, empty string is returned.
        */
        virtual std::string getName() const = 0;

        /**
          * Return a deep copy of the command. Caller is responsible for deletion of the cloned command.
          *
          * @return An identical deep copy of the command.
          */
        virtual IStimulationCommand* clone() const = 0;
    };
}}


#endif //IMPLANTAPI_ISTIMULATION_COMMAND_H