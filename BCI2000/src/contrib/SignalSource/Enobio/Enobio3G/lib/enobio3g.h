#ifndef ENOBIO3G_H
#define ENOBIO3G_H

/*!
 * \mainpage
 *
 * The Enobio API consists on a set of classes that permit the access to the
 * Enobio device and the data that it provides.
 *
 * The main class to access the device is Enobio3G. This class provides methods
 * for opening the device, starting and stopping the data streaming and
 * configuring the device.
 *
 * The access to both the data streaming and the device status is made through
 * a producer-consumer protocol. The Enobio raw data streaming is accessed
 * through the producer Enobio3G::ENOBIO_DATA. The device status is accessed
 * through the producer Enobio3G::STATUS. By registering an implementation of
 * the IDataConsumer virtual class with the Enobio3G::registerConsumer method
 * the link between the poducer and the consumer is made. Thus the Enobio class
 * will call the IDataConsumer::receiveData method of all registered
 * IDataConsumer implementations every time a new sample is available or the
 * device status changes.
 *
 * The actual data or status is passed as a parameter to the
 * IDataConsumer::receiveData implementation through the PData class. The
 * pointer returned by the PData::getData method shall be casted to the
 * ChannelData or StatusData class depending on whether the implementation of
 * the IDataConsumer is registered to the Enobio3G::ENOBIO_DATA producer or the
 * Enobio3G::STATUS one.
 *
 * The implementation of the IDataConsumer::receiveData method shall take into
 * consideration that it will be executed from a different thread than the
 * Enobio one. So the operations done within the implementation shall be
 * thread-safe.
 *
 * Another consideration that the IDataConsumer::receiveData implementation
 * shall take into account is that the data accessed through the PData::getData
 * method is not accessible out of the scope of that method. That data is
 * deleted inside the Enobio class when all the IDataConsumer implementations
 * are called.
 *
 * The API can calculate the power of the signals provided by Eenobio in eight
 * different bands: Delta (from 0.1 to 3.5 Hz), Theta (from 4.0 to 7.5 Hz),
 * Alpha (from 8.0 to 13.0), Beta (from 14.0 to 30.0 Hz), Gamma (from 30.0 to
 * 100.0 Hz), SMR band (from 12.0 to 15.0 Hz) and two user defined bands. To
 * access these values, eight different producers are provided:
 * Enobio3G::POWER_DELTA, Enobio3G::POWER_ALPHA, Enobio3G::POWER_ALPHA,
 * Enobio3G::POWER_BETA, Enobio3G::POWER_GAMMA, Enobio3G::POWER_SMR,
 * Enobio3G::POWER_USERDEFINED_1 and Enobio3G::POWER_USERDEFINED_2. The rate
 * these producers provide a new power calculation can be controlled with the
 * method Enobio3G::setPowerCalculationRate. The number of samples that are
 * taken into consideration for the power calculation is controlled with the
 * method Enobio3G::setPowerComputingLength. The user defined bands are
 * selected by calling the methods Enobio3G::setUserDefinedBand1 and
 * Enobio3G::setUserDefinedBand2.
 *
 * The accelerometer is activated/deactivated by calling the
 * Enobio3G::activateAccelerometer method before starting the EEG streaming.
 * The data is received through the Enobio3G::ACCELEROMETER producer when the
 * EEG streaming is ON.
 *
 * The API can also configure up to five different power combination features.
 * The combination of the power in some specific frequency bands for a set of
 * channels placed on specific locations might provide relevant features (i.e.
 * information regarding affective state). The
 * Enobio3G::configurePowerCombinationFeatures method allows selecting up to
 * two sets of channels that will be used to extract the feature of interest as
 * well as the frequency bands for the two sets of channels and the
 * mathematical operation to combine the power from the set of channels.
 *
 * As an example, to extract information regarding valence the following
 * configuration might be used: Perform the subtraction operation over the
 * channel placed on F3 on the Alpha band and the channel placed on F4 filtered
 * on the Alpha band too.
 *
 * The length of the raw EEG signal which is used to perform the computations
 * is defined by the Enobio3G::setPowerComputingLength method.
 *
 * The obtained results are delivered through the
 * Enobio3G::POWER_COMBINATION_FEATURE_X producers (being X a number from 0 to
 * 4). The IDataConsumer::receiveData implementation connected to the producers
 * shall cast the pointer obtained through PData::getData method to the
 * ProcessedData class. The result of the combination is a single value stored
 * on channel 0 of the class.
 *
 * The results are delivered at rate defined by the
 * Enobio3G::setPowerCalculationRate method.
 */

#include "Enobio3GAPI_global.h"
#include "IDataProcessor.h"

/*! \class Enobio3G Enobio3G.h
 *
 * \brief This class provides access to the Enobio device and its data
 * streaming.
 *
 * This class provides methods for opening the device, starting and stopping
 * the data streaming and configuring the device.
 *
 * It inheritates from the base class IDataProcessor so it provides producer
 * implementations to which IDataConsumer implementations can register to
 * access both streaming data and device status.
 */

class ENOBIO3GAPISHARED_EXPORT Enobio3G : public IDataProcessor {
public:

    /*!
     * Version id string
     */
    static const char * ENOBIO_API_VERSION;

    /*!
     * Constructor of the Enobio3G object.
     */
    Enobio3G();

    /*!
     * Enobio destructor.
     */
    virtual ~Enobio3G ();

    /*!
     * The ids of the available producers.
     */
    enum producerIDs
    {
        ENOBIO_DATA=1,
        STATUS=2,
        ACCELEROMETER=3,
        POWER_DELTA=4,
        POWER_THETA=5,
        POWER_ALPHA=6,
        POWER_BETA=7,
        POWER_GAMMA=8,
        POWER_SMR=9,
        POWER_USERDEFINED_1=10,
        POWER_USERDEFINED_2=11,
        POWER_COMBINATION_FEATURE_0=12,
        POWER_COMBINATION_FEATURE_1=13,
        POWER_COMBINATION_FEATURE_2=14,
        POWER_COMBINATION_FEATURE_3=15,
        POWER_COMBINATION_FEATURE_4=16
    };

    /*!
     * The ids of the frequency bands used for power extraction features
     */
    enum bandsIds
    {
        DELTA_BAND=0, // From 0.1 to 3.5 Hz
        THETA_BAND=1, // From 4.0 to 7.5 Hz
        ALPHA_BAND=2, // From 8.0 to 13.0 Hz
        BETA_BAND=3,  // From 14.0 to 30.0 Hz
        GAMMA_BAND=4, // From 30.0 to 100.0 Hz
        SMR_BAND=5,   // From 12.0 to 15.0 Hz
        USER_DEFINED_1_BAND=6,
        USER_DEFINED_2_BAND=7
    };

    /*!
     * The ids of the operations for the power combinations
     */
    enum operations
    {
        NO_OPERATION=0,
        ADDITION_OPERATION=1,
        SUBTRACTION_OPERATION=2,
        MULTIPLICATION_OPERATION=3,
        DIVISION_OPERATION=4
    };

    enum normalizationTypes
    {
        NO_NORMALIZATION=0,
        MINMAX_NORMALIZATION=1, // Normalization to the absolut recevied max
                                // and min values.
        HISTORICAL_NORMALIZATION=2, // Normalization to the max and min of the
                                    // previous 30 values.
        SIGMOID_NORMALIZATION=3 // Normalization using a sigmoid function.
    };

    /*!
     * It performs the operations required to initialize the hardware.
     */
    bool openDevice (unsigned char * macAddress);

    /*!
     * It performs the operations required to close the hardware device.
     */
    void closeDevice ();

    /*!
     * It starts the sampling at the predefined sampling rate.
     */
    void startStreaming ();

    /*!
     * It stops the sampling.
     */
    void stopStreaming ();

    /*!
     * It activates/deactivates the accelerometer. When it is activated the
     * data is received along with the EEG data by registering to the
     * Enobio3G::ACCELEROMETER data producer. It has no effect if the EEG
     * streaming is already on, the new value takes effect when the streaming
     * is stopped and started again.
     *
     * \param b The accelerometer is activated when b is true, otherwise the
     * accelerometer is deactivated.
     */
    void activateAccelerometer (bool b);

    /*!
     * It sets the application en demo mode so the signals reported by the data
     * producer are the ones present in a file pass as a parameter.
     *
     * \param b If true the application turns to demo mode, otherwise it backs
     * to normal operation.
     *
     * \param signalFile File with the signals to be reported by the data
     * producer. Each row on the file shall have the channel samples separated
     * by tabulators.
     */
    void setDemoMode (bool b, const char * signalFile);

    /*!
     * deprecated since the communication is keeped alive in the background.
     * If the device is not present it is reported through the status
     * producer.
     */
    bool ping ();

    /*!
     * It returns the DataConsumer object identified with an ID.
     *
     * \param id The identification of the DataConsumer to be obtained.
     *
     * \return Reference to the solicited DataConsumer.
     */
    virtual IDataConsumer * getDataConsumer (const int id);

    /*!
     * It returns the DataProducer object identified with an ID.
     *
     * \param id The identification of the DataProducer to be obtained.
     *
     * \return Reference to the solicited DataProducer.
     */
    virtual IDataProducer * getDataProducer (const int id);


    /*!
     * It adds a DataConsumer to the list of the consumers of the Producer
     * identified by the parameter id.
     *
     * \param id Identification of the Producer to use.
     *
     * \param dataConsumer The DataConsumer to add to the Producer.
     *
     * \return If the producer specified does not exist the function returns
     * false.
     */
    virtual bool registerConsumer (const int id,
                                    IDataConsumer& dataConsumer);

    /*!
     * It removes a DataConsumer from the list of consumers of the
     * Producer identified by the parameter id.
     *
     * \param id Identification of the Producer to use.
     *
     * \param dataConsumer The DataConsumer to remove from the Producer.
     *
     * \return If the producer specified does not exist the function returns
     * false.
     */
    virtual bool deregisterConsumer (const int id,
                                     const IDataConsumer& dataConsumer);

    /*!
     * It sets the number of samples upon which the Enobio signal power on the 
     * different frequency bands will calculated.
     *
     * \param L number of samples upon which the power will be calculated.
     */
    void setPowerComputingLength(int L);

    /*!
     * It sets the rate of calculation of power (in samples). Its sample rate
     * corresponds to the sample rate of Enobio (500samples/s) divided by the
     * powerCalculationRate.
     *
     * \param powerCalculationRate Power calculation rate.
     */
    void setPowerCalculationRate(int powerCalculationRate);

    /*!
     * It sets the frequency limits (in Hz) of the user-defined band #1 whose
     * power is provided by the Enobio3G::POWER_USERDEFINED_1 producer.
     *
     * \param f1 Low frequency limit.
     *
     * \param f2 High frequency limit.
     */
    void setUserDefinedBand1(double f1, double f2);

    /*!
     * It sets the frequency limits (in Hz) of the user-defined band #2 whose
     * power is provided by the Enobio3G::POWER_USERDEFINED_2 producer.
     *
     * \param f1 Low frequency limit.
     *
     * \param f2 High frequency limit.
     */
    void setUserDefinedBand2(double f1, double f2);

    /*!
     * This method allows configuring up to five different power combination
     * features. The combination of the power in some specific frequency bands
     * for a set of channels placed on specific locations might provide
     * relevant features (i.e. information regarding affective state). The
     * method allows selecting up to two sets of channels that will be used to
     * extract the feature of interest as well as the frequency bands for the
     * two sets of channels and the mathematical operation to combine the power
     * from the set of channels.
     *
     * As an example, to extract information regarding valence the following
     * configuration might be used: Perform the subtraction operation over the
     * channel placed on F3 on the Alpha band and the channel placed on F4
     * filtered on the Alpha band too.
     *
     * The length of the raw EEG signal which is used to perform the
     * computations is defined by the Enobio3G::setPowerComputingLength method.
     *
     * The obtained results are delivered through the
     * Enobio3G::POWER_COMBINATION_FEATURE_X producers (being X a number from 0 to
     * 4). The IDataConsumer::receiveData implementation connected to the producers
     * shall cast the pointer obtained through PData::getData method to the
     * ProcessedData class. The result of the combination is a single value stored
     * on channel 0 of the class.
     *
     * The results are delivered at rate defined by the
     * Enobio3G::setPowerCalculationRate method.
     *
     * This method is not thread-safe. It shall be called only when the EEG
     * streaming is off.
     *
     * \param index This a zero-based index of the power combination features
     * to be configured. Up to 5 different operations can be configured. If
     * this value is out of range no operation is performed.
     *
     * \param freqBand1 The frequency band used to filter the channels defined
     * for the first operand of the combination operation. The valid values
     * are the ones defined on the Enobio3G::bandsIds enumeration.
     *
     * \param channels1 Integer that indicates which channels shall be used for
     * the power in band calculation for the first operand. The channels are
     * represented at bit level. Bit 0 stands for channel 1, and so on. If the
     * bit value is equal to ‘1’ this channel is taken into account for the
     * power in band calculation.
     *
     * \param operation This is a value that identifies the operation to be
     * performed. The valid values are the ones defined on the
     * Enobio3G::operations enumeration. If an invalid value is provided the
     * operation is set to a No operation value.
     *
     * \param freqBand2 The frequency band used to filter the channels defined
     * for the second operand of the combination operation. See freqBand1
     * parameter information to see the valid values.
     *
     * \param channels2 Integer that indicates which channels shall be used for
     * the power in band calculation for the second operand. The channels are
     * represented at bit level. Bit 0 stands for channel 1, and so on. If the
     * bit value is equal to ‘1’ this channel is taken into account for the
     * power in band calculation.
     *
     * \param normalizationType The type of normalization to perform before
     * delivering the values from the power combination operation. The valid
     * values are the ones defined on the Enobio3G::normalizationTypes
     * enumeration.
     */
    void configurePowerCombinationFeatures
                                        (unsigned int index,
                                         unsigned int freqBand1,
                                         unsigned int channels1,
                                         unsigned int operation,
                                         unsigned int freqBand2,
                                         unsigned int channels2,
                                         unsigned int normalizationType);

private:

    /*!
     * Instance of the implementation class.
     */
    void * _enobioPrivate;
};


#endif // ENOBIO3G_H
