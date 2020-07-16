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
#ifndef IMPLANTAPI_ISTIMULATION_ATOM_H
#define IMPLANTAPI_ISTIMULATION_ATOM_H
#include <stdint.h>


namespace cortec { namespace implantapi {

    /**
      * @brief Atomic element of a stimulation function.
      *
      * Each atom has a duration which is defined in micros and a type (IStimulationAtom::AtomType) that determines its 
      * form. Atoms can be appended to a stimulation function to define the function's signal form over time.
      *
      * @see IStimulationFunction.
      */
    class IStimulationAtom 
    {
    public:
        /**
        * Enumeration with all available stimulation atom types.
        */
        enum class AtomType
        {
            AT_NOTYPE        /// Type representing no type
            , AT_RECTANGULAR /// Rectangular shaped atoms, @see CRectangularStimulationAtom 
            , AT_RECTANGULAR_4_AMPLITUDE /// Rectangular shaped atoms with multiple amplitudes, @see CRectangular4AmplitudeStimulationAtom
            , AT_COUNT       /// number of atom types
        };

        virtual ~IStimulationAtom() {}       

        /// @return duration in micros of the stimulation atom
        virtual uint64_t getDuration() const = 0;

        /// @return type of the atom
        virtual AtomType getType() const = 0;

        /**
        * Create a deep copy of the atom. Caller is responsible for deletion of the copy.
        *
        * @return An identical copy of the atom.
        */
        virtual IStimulationAtom* clone() const = 0; 

        /// @return true if the other atom defines the same stimulation signal
        virtual bool operator==(const IStimulationAtom& other) const = 0;

        /// @return true if the other atom defines not the same stimulation signal
        virtual bool operator!=(const IStimulationAtom& other) const = 0;
    };

}}

#endif //IMPLANTAPI_ISTIMULATION_ATOM_H