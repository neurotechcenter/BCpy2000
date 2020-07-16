#ifndef CHANNELDATA_H
#define CHANNELDATA_H

/*!
 * \class ChannelData
 *
 * \brief This class holds an Enobio3G/StarStim sample which consists on an
 * integer value for each channel and an extra value that informs about the
 * channels that are reported in that sample.
 */
class ChannelData
{
public:
    /*!
     * Default constructor
     */
    ChannelData () {}

    /*!
     * It returns an integer that reports which channels are present in the
     * current sample
     *
     * \return The returned value is organized at bit level. The bits set to
     * '1' mean that the channels are being reported. The data for those
     * channels whose channel info bits are set to zero is undefined. The least
     * significant bit corresponds to channel 0
     */
    int channelInfo () {return _channelInfo;}

    /*!
     * It returns the pointer to the vector that contains the samples of all
     * the channels
     *
     * \return pointer to the numberOfChannels-lenght vector
     */
    int * data () {return _data;}

    /*!
     * It sets the channel info value
     *
     * \param channelInfo Value which information at bit level regarding the
     * channels that are reported in the current sample. The least significant
     * bit corresponds to channel 0
     */
    void setChannelInfo (int channelInfo) {_channelInfo = channelInfo;}

    /*!
     * It sets the value of the channel for a specific channel
     *
     * \param index 0-based channel index
     *
     * \param value value of the sample
     */
    void setData(int index, int value) {if (index < 32) _data[index] = value;}

    /*!
     * It gets the timestamp of the sample
     */
    unsigned long long timestamp () {return _timeStamp;}

    /*!
     * It sets the timestamp of the sample
     */
    void setTimestamp (unsigned long long value) { _timeStamp = value;}

    /*!
     * It indicates whether the sample is a repeated one due to packet loss
     */
    void setRepeated (bool value) { _isRepeated = value;}

    /*!
     * It returns whether the sample is a repeated one due to packet loss
     */
    bool isRepeated () { return _isRepeated;}


private:
    /*!
     * \property ChannelData::_channelInfo
     *
     * Variable that holds the channel information value that indicates the
     * channels that are present in the current sample
     */
    unsigned int _channelInfo;

    /*!
     * \property ChannelData::_data
     *
     * Vector that hols the sample value of all the channels
     */
    int _data[32];

    /*!
     * \property ChannelData::_timeStamp
     *
     * Timestamp for the channel data sample
     */
    unsigned long long _timeStamp;

    /*!
     * \property ChannelData::_isRepeated
     *
     * Boolean to control if current packet is repeated to compensate for packet loss
     */
    bool _isRepeated;


};

#endif // CHANNELDATA_H
