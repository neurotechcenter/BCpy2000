#ifndef IDATACONSUMER_H
#define IDATACONSUMER_H

#include "PData.h"

/*!
 * \class IDataConsumer IDataConsumer.h
 *
 * \brief Base class of a data consumer. It is registered to a data producer in
 * order to receive its data.
 *
 */
class IDataConsumer
{
public:
    /*!
     * Virtual destructor
     */
    virtual ~IDataConsumer () {}

    /*!
     * This method is called by the IDataProducer to which the IDataConsumer is
     * registered. The IDataConsumer implementation shall cast the data passed
     * as parameter accordingly to the kind of data producer to wich it is
     * registered.
     *
     * \param data PData object containing the data that comes from the
     * IDataProducer to which it is registered.
     */
    virtual void receiveData (const PData& data) = 0;
};


#endif // IDATACONSUMER_H
