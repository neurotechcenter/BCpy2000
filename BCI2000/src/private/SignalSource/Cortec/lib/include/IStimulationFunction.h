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
#ifndef IMPLANTAPI_ISTIMULATION_FUNCTION_H
#define IMPLANTAPI_ISTIMULATION_FUNCTION_H
#include "IIterator.h"
#include "IStimulationAtom.h"
#include <set>


namespace cortec { namespace implantapi {

    /**
      * @brief A IStimulationFunction defines what stimulation signal is applied to which electrodes.
      *
      * An stimulation function consists of
      * - a sequence of IStimulationAtoms that can be repeated a number of times.
      * - and the information which electrodes to use for stimulation.
      *
      * Stimulation is usually applied between two points: one source electrode and one destination electrode (e.g., a 
      * ground electrode). Here, this concept is generalized to so-called 'virtual electrodes'. A virtual electrode is a
      * non-empty set of electrodes. It allows more degrees of freedom to shape the electric field of a stimulation.
      * These two sets have to be disjunct (because an electrode is either a source or a destination electrode, not 
      * both) and non-empty (because at least two electrodes are needed for stimulation).
      *
      * Each electrode is adressed with a positive integer index (i.e., uint32_t) and virtual electrodes are defined 
      * as sets of uint16_t (i.e., std::set<uint32_t>). Depending on the actual implant used, these indices may 
      * vary. In addition, depending on the actually used implant, the electrode indices and the allowed combinations of
      * electrodes into virtual electrodes may differ. Please refer to the documentation of the used implant for more 
      * details on this.
      *
      * Typical usage:
      * 1. create IStimulationFunction instance with an IStimulationCommandFactory.
      * 2. Add an atom to the stimulation function.
      * 3. Repeat 2. until all atoms are added.
      * 4. Set repetitions using setRepetitions().
      *
      * Please note that one function may only contain atoms of the same type (e.g., only rectangular atoms).
      *
      * @see IStimulationCommandFactory
      * @see IStimulationAtom
      */
    class IStimulationFunction
    {
    public:
        virtual ~IStimulationFunction() {}

        /**
          * Append a stimulation atom to the end of the stimulation function.
          * Only valid atoms may be appended. An atom is considered invalid if one of the following statement holds:
          * - IStimulationAtom::TYPE == AT_NOTYPE
          * - The atom has a different type than the other atoms in the function
          *
          * @param[in] stimulationAtom IStimulationFunction takes ownership of the atom
          * \if INTERN_IMPLANTAPI
          *     @throws CInvalidArgumentException if stimulationAtom is nullptr or the atom is invalid
          * \else
          *     @throws std::exception if stimulationAtom is nullptr or the atom is invalid
          * \endif
          * 
          */
        virtual void append(IStimulationAtom* stimulationAtom) = 0;

        /**
          * @return Iterator to the first atom in this function. The ownership of the iterator is passed to the caller.
          */
        virtual IIterator<IStimulationAtom>* getAtomIterator() const = 0;

        /**
          * Set the number of times the sequence of atoms defined in the function will be repeated.
          *
          * \if INTERN_IMPLANTAPI
          *     @throws CInvalidArgumentException if repetitions < 1
          * \else
          *     @throws std::exception if repetitions < 1
          * \endif
          
          */
        virtual void setRepetitions(const uint32_t repetitions) = 0;

        /**
          * @return the number of times the sequence of atoms in a function will be repeated.           
          */
        virtual uint32_t getRepetitions() const = 0;

        /**
          * Set the name of the function.
          */
        virtual void setName(const std::string& functionName) = 0;

        /**
          * @return the set name of the function. If the function name was not set, empty string is returned.
          */
        virtual std::string getName() const = 0;

        /**
          * @return the total duration in micros defined by the function including the time required for all repetitions.
          */
        virtual uint64_t getDuration() const = 0;

        /**
          * The period is the duration of one repetition, i.e. the sum of all atom durations. If (repetitions == 1) 
          * getPeriod() and getDuration() return the same result.
          *
          * @return Duration of one period in micros. 
          * 
          */
        virtual uint64_t getPeriod() const = 0;

        /**
          * Set the virtual electrodes for the stimulation function.
          *
          * @param[in] sourceChannels      Set of electrodes where the function is applied to.
          * @param[in] destinationChannels Set of electrodes that are used as ground electrodes by this function.
          *
          * \if INTERN_IMPLANTAPI
          *   @throws CInvalidArgumentException if the sets are not disjunct or if destination set is empty.
          * \else
          *   @throws std::exception if the sets are not disjunct or if destination set is empty.
          * \endif
          */
        virtual void setVirtualStimulationElectrodes
            (const std::set<uint32_t>& sourceChannels, const std::set<uint32_t>& destinationChannels) = 0;

        /**
          * Returns the virtual electrodes configuration for this stimulation function.
          *
          * @return pair of sets for source electrodes (first) and destination electrodes (second)
          */
        virtual std::pair<std::set<uint32_t>, std::set<uint32_t>> getVirtualStimulationElectrodes() const = 0;

        /**
          * Return a deep copy of the stimulation function. Caller is responsible for deletion of the copy.
          *
          * @return Identical copy of the stimulation function.
          */
        virtual IStimulationFunction* clone() const = 0; 

        /**
          * Check if another function has the same form (i.e., consist of the same number of atoms and the atoms are 
          * pairwise equal). The number of repetitions may be different.
          * 
          * @param[in] other The stimulation function to compare the signal form to.
          *
          * @return true if both functions have the same signal form.
          */
        virtual bool hasEqualSignalForm(const IStimulationFunction& other) const = 0;

        /**
          * Check if another function has the same virtual electrodes.
          * 
          * @param[in] other The stimulation function to compare the virtual electrodes to.
          *
          * @return true if the source electrodes and the destination electrodes are the same for both functions
          */
        virtual bool hasEqualVirtualStimulationElectrodes(const IStimulationFunction& other) const = 0;
    };
}}

#endif //IMPLANTAPI_ISTIMULATION_FUNCTION_H