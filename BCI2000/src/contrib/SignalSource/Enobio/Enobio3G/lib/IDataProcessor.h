#ifndef IDATAPROCESSOR_H
#define IDATAPROCESSOR_H

#include "IDataConsumer.h"
#include "IDataProducer.h"


/*!
 * \class IDataProcessor IDataProcessor.h
 *
 * \brief Base class that allows using the IDataConsumer and IDataProducer
 * interfaces in order to process incoming data and pass it to other
 * IDataProcessors that might be registered.
 *
 */
class IDataProcessor
{
public:
    /*!
     * \typedef const int ID
     *
     * \brief Identification of the data producer.
     */
    typedef const int ID;

    /*!
     * Virtual destructor
     */
    virtual ~IDataProcessor () { }

    /*!
     * It obtains a data consumer of the IDataProcessor.
     *
     * \param id Identification of the data consumer.
     *
     * \return Pointer to the requested IDataConsumer object
     */
    virtual IDataConsumer * getDataConsumer (ID id) = 0;

    /*!
     * It obtains a data producer of the IDataProcessor
     *
     * \param id Identification of the data producer.
     *
     * \return Pointer to the requested IDataProducer object
     */
    virtual IDataProducer * getDataProducer (ID id) = 0;

    /*!
     * It registers an IDataConsumer implementation from another IDataProcessor
     * to an IDataProducer of the current IDataProcessor
     *
     * \param id Identification of the IDataProducer to which the provided
     * IDataConsumer is going to be registered.
     *
     * \param dataConsumer Reference to the IDataConsumer implementation to be
     * registered to the IDataProducer identified with the provided ID.
     */
    virtual bool registerConsumer (ID id, IDataConsumer& dataConsumer) = 0;

    /*!
     * It deregisters an already registered IDataConsumer implementation.
     *
     * \param id Identification of the IDataProducer to which the provided
     * IDataConsumer is already registered.
     *
     * \param dataConsumer Reference to the IDataConsumer implementation to be
     * deregistered from the IDataProducer identified with the provided ID.
     */
    virtual bool deregisterConsumer (ID id,
                                     const IDataConsumer& dataConsumer) = 0;

    /*!
     * It connects the two given IDataProcessor by registering the
     * IDataConsumer of the second processor to the IDataProducer of the first
     * one.
     *
     * \param processor1 Reference to the IDataProcessor whose IDataProducer is
     * accessed to register the provided IDataConsumer.
     *
     * \param idProducer Identification of the IDataProducer of the processor1.
     *
     * \param processor2 Reference to the IDataProcessor whose IDataConsumer is
     * going to be registered to the provided IDataProducer.
     *
     * \param idConsumer Identification of the IDataConsumer of the processor2.
     *
     * \return true if the registration between the two IDataProcessor is
     * succesfully, false otherwise.
     */
    static bool connectProcessors (IDataProcessor & processor1,
                                   ID idProducer,
                                   IDataProcessor & processor2,
                                   ID idConsumer)
    {
        IDataProducer * producer = processor1.getDataProducer(idProducer);
        if (producer == NULL)
        {
            return false;
        }
        IDataConsumer * consumer = processor2.getDataConsumer(idConsumer);
        if (consumer == NULL)
        {
            return false;
        }
        producer->addDataConsumerEndPoint(*consumer);
        return true;
    }

    /*!
     * It disconnects the two given IDataProcessor by deregistering the
     * IDataConsumer of the second processor from the IDataProducer of the
     * first one.
     *
     * \param processor1 Reference to the IDataProcessor whose IDataProducer is
     * accessed to deregister the provided IDataConsumer.
     *
     * \param idProducer Identification of the IDataProducer of the processor1.
     *
     * \param processor2 Reference to the IDataProcessor whose IDataConsumer is
     * going to be deregistered from the provided IDataProducer.
     *
     * \param idConsumer Identification of the IDataConsumer of the processor2.
     *
     * \return true if the deregistration between the two IDataProcessor is
     * succesfully, false otherwise.
     */
    static bool disconnectProcessors (IDataProcessor & processor1,
                                      ID idProducer,
                                      IDataProcessor & processor2,
                                      ID idConsumer)
    {
        IDataProducer * producer = processor1.getDataProducer(idProducer);
        if (producer == NULL)
        {
            return false;
        }
        IDataConsumer * consumer = processor2.getDataConsumer(idConsumer);
        if (consumer == NULL)
        {
            return false;
        }
        producer->removeDataConsumerEndPoint(*consumer);
        return true;
    }
};

#endif // IDATAPROCESSOR_H
