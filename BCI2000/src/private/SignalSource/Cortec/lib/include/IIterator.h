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
#ifndef IMPLANTAPI_IITERATOR_H
#define IMPLANTAPI_IITERATOR_H

namespace cortec { namespace implantapi {
    /**
      * @brief iterator template interface
      */
    template<class Item>
    class IIterator 
    {
    public:
        virtual ~IIterator() {}

        /**
          * Set iterator to next element in the container.
          */
        virtual void next() = 0;

        /**
          * True if all elements of the container have been traversed.
          */
        virtual bool isDone() const = 0;

        /**
          * Get the element the iterator is pointing to.
          */         
        virtual Item* getCurrentItem() const = 0;

    protected:
        IIterator() {}
    };
}}

#endif //IMPLANTAPI_IITERATOR_H