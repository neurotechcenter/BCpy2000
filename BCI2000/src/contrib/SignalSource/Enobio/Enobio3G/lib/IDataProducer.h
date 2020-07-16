#ifndef IDATAPRODUCER_H
#define IDATAPRODUCER_H

#include "PData.h"
#include "IDataConsumer.h"


/*!
 * \class IDataProducer IDataProducer.h
 *
 * \brief Base class of a data producer. It sends data to all the data
 * consumers that are registered to it.
 *
 */
class IDataProducer
{
public:
    /*!
     * Virtual destructor
     */
    virtual ~IDataProducer () {}

    /*!
     * It sends a piece of data to all the IDataConsumer implementations
     * already registered.
     *
     * \param data PData object that is sent to the IDataConsumer
     * implementations already registered.
     */
    virtual void sendData (const PData& data) = 0;

    /*!
     * It registers the provided IDataConsumer.
     *
     * \param dataConsumer Reference to the IDataConsumer implementation to be
     * registered.
     */
    virtual void addDataConsumerEndPoint
                            ( IDataConsumer& dataConsumer) = 0;

    /*!
     * It deregister the provided IDataConsumer.
     *
     * \param dataConsumer IDataConsumer implementation to be deregistered.
     */
    virtual void removeDataConsumerEndPoint
                            (const IDataConsumer& dataConsumer) = 0;

};

#endif // IDATAPRODUCER_H
