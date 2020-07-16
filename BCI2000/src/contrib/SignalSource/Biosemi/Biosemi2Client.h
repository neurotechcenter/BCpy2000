/*
 * Author: Samuel A. Inverso (samuel.inverso@gmail.com) and Yang Zhen
 * Contributors:
 * Description: Client to acquire from the USB BIOSEMI 2 Labview_DLL.
 * License:
 * Copyright (C) 2005  Samuel A. Inverso (samuel.inverso@gmail.com), Yang Zhen
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301,
 * USA.
 *
 * Revisions:
 *
 *  Revision 1.1  2005/12/12 00:05:24  sinverso
 *  Initial Revision: Working and tested offline. Not tested in real experiments.
 *
 *  Revison 1.2 2008/10/25  Maria Laura Blefari
 *  Fixed the ERROR: Mode changed from 4 to 0
 *
 *  Revison 2.0 2008/11/07  Jeremy Hill
 *  Updated design, including support for an auxiliary Analog Input Box (AIB)
 *  EEG + AIB acquisition tested---triggers not.
 *
 *  Revison 2.1 2009/05/27  Jeremy Hill
 *  Trigger signal acquisition fixed and tested.
 *  Added option to acquire trigger signals simultaneously in one 16-bit channel.
 */

#ifndef Biosemi2ClientH
#define Biosemi2ClientH

#include <Windows.h>
#include <iterator>

class Biosemi2Client{

public:
    // forward declaration

    class DataBlock;


    // C O N S T A N T S //

    static const long BUFFER_SIZE_IN_BYTES = 1024*1024*32; // 33,554,432
    static const long BYTES_PER_INT = 4; // the number of chars that make up an
                                        // int. 1 byte per char, 4 bytes per int
                                        // so 4 chars per int
    // size of the int data buffer
    static const long BUFFER_SIZE_IN_INT = BUFFER_SIZE_IN_BYTES / BYTES_PER_INT;

    static const int SYNC_CHANNEL          =   0;
    static const int STATUS_CHANNEL        =   1;
    static const int FIRST_EEG_CHANNEL     =   2;
    static const int FIRST_AIB_CHANNEL_MK1 = 258;
    static const int FIRST_AIB_CHANNEL_MK2 = 282;
    static const int NUM_AIB_CHANNELS      =  32;
    static const int NUM_TRIGGERS          =  16; // the number of triggers


    Biosemi2Client();

    virtual ~Biosemi2Client();

    /*
     * Call initialize before calling anything else to setup everything
     *
     * @param desiredSamplingRate the sampling rate you want in Hz
     *      (as in samples per channel)
     *      Must be evenly divisable into the biosemi's sampling rate
     *      e.g. Biosemi Mode 4 in Mk2 = 2048 Hz,
     *      so 2408 % desiredSamplingrate must equal 0
     *
     * @param desiredSampleBlockSize the number of samples wait for
     *      per channel before isDataReady becomes true.
     *      e.g. 32 would wait for 32 samples per channel
     *      at your desired sampling rate
     *
     * @param desiredNumChannels the number of channels you want. This
     *      does not include the sync or status channel
     *      you must ask for a number of channels less than or equal to
     *      the number of signal channels the biosemi transmits on the
     *      current mode
     *      e.g. mode 5 transmits 128 signal channels (130 with status and sync)
     *      so you can ask for 1 to 128 channels
     *
     * @param throwBatteryLowException if true we throw an exception
     *     when checking valid status if the battery is low, otherwise
     *     we just return true. (Basically how bad to care if the battery
     *     is low during your experiment, use false if you don't want
     *     the experiment to crash to a halt when the battery becomes
     *     low) Default = false
     */
    virtual bool  initialize( int desiredSamplingRate,
        int desiredSampleBlockSize, int desiredNumChannels );

    /*
     * Data block allows you to access the data as if it was actually
     * sampled at your desired sampling rate with your desired number of
     * channels, and not have to worry about the offset into the data buffer
     *
     * Note: Call isDataReady automatically makes datablock access the new data
     * just acquired.
     *
     * @return Datablock to access the data
     * @see DataBlock
     */
    virtual DataBlock& getDataBlock();

    /*
     * Allows access to the data buffer. You should normally use
     * getDataBlock instead.
     *
     * @return pointer to the data buffer in ints
     */
     virtual const int * getData() const;

    /*
     * Returns when there is enough data in the buffer to fulfill your
     * sample block size request.
     */
    virtual void isDataReady();

    /*
     * Returns when there is enough data in the buffer to fulfill your
     * sample block size request.
     *
     * @param startPos is an out variable, it contains starting position
     *  of the new data in the data buffer
     */
    virtual void isDataReady(int &startPos);

    /*
     * Return the actual number of channels the biosemi is sending
     * including sync, status and AIB channels
     */
    virtual int getNumChannels() const;

    /**
     * @return the actual number of EEG channels the biosemi is sending
     * excluding sync, status and AIB channels
     */
    virtual int getNumEEGChannels() const;

    /**
     * @return the actual number of AIB channels the biosemi is sending
     */
    virtual int getNumAIBChannels() const;

    /* Return biosemi's speedmode set in the front end of the AD box*/

    virtual int getMode() const;

    /*
     * Return biosemi's actual sampling rate in Hz
     */
    virtual int getSamplingRate() const;


    /*
     * Return the actual sampleBlockSize we are waiting for, which
     * differs from the one you desired based on the difference
     * in sampling rate
     */
    virtual int getSampleBlockSize() const;


     /* Halts data acquisition */

    virtual void halt();

    virtual bool isDataValid(const int * data, int position ) const;

    virtual bool isDataValid() const;


    /* Checks if the first sample  starting at startPos  in data is synced*/

    virtual bool isDataSynced(const int * data, int position) const;

    virtual bool isSampleSynced(int sample) const;

    virtual bool isStatusValid(int sample) const;

    virtual bool isTriggerHigh( int trigger, int datum) const;

    /*
     * True if the battery based on the last bit status sample is low. The current
     * data block only call this after isDataReady.
     */
    virtual bool isBatteryLow() const;

    /* Return true if the battery is low in this status sample */

    virtual bool isBatteryLow( int status ) const;

    /* True if this is an MK2 system, only works after initialization */

    virtual bool isMK2() const;

    /* Return true if the status sample says this is an MK2 */

    virtual bool isMK2(int status) const;


protected:

    virtual int statusToMode(const int * data, int position) const;
    virtual int statusToMode(int status) const;

    virtual int determineNumChannels(int mode, bool isMk2) const;
    virtual int determineNumChannelsMk1(int mode) const;
    virtual int determineNumChannelsMk2(int mode) const;

    virtual int determineSamplingRate(int mode)const ;

    virtual void decodeStatus(const int * data, int position,
        bool &mk2, bool &battLow, int &mode, int &numChannels,
        int &samplingRate, bool &cmsWithinRange, bool &newEpoch) const;

    virtual void setupDriver();

    /*
     * Calculate the real index into the data buffer based on sample and channel,
     * the start position of the data, number of actual channels, and decimation
     * factor.
     * Note: channel 0 is the sync, 1 is status, and 2 starts the EEG signal
     *
     */
    virtual int calcIndex(int sample,
        int channel) const;

    virtual int calcIndex( int sample,
        int channel, int startPos ) const;

    virtual double averageSamples(int sample,
        int channel) const;


protected:

    // C O N S T A N T S //


    static const int USB_DATA_SIZE = 64;

    static const int MK2_MASK = 0x80000000; // Status mask MK2 bit
    static const int BATT_LOW_MASK = 0x40000000; // Status mask for battery low
    static const int CMS_WITHIN_RANGE_MASK=0x10000000; // Status mask if CMS
                                            // is within range

    static const int MODE_MASK = 0x2E000000;
    //http://www.biosemi.nl/forum/viewtopic.php?t=26&highlight=readmultiplesweeps

    static const int MODE_SHIFT_TO_INT = 25; // How bits to shift the masked
                                             // mode bits, to get a mode int

    static const int NEW_EPOCH_MASK = 0x01000000; // status mask if this
                                                 // is a new epoch

    static const int IS_SYNCED_MASK = 0xFFFFFF00; // mask if this sample is synced



    // Mask if trigger is high using 32bits

    static const unsigned int TRIGGER_0    = 0x00000100;
    static const unsigned int TRIGGER_1    = 0x00000200;
    static const unsigned int TRIGGER_2    = 0x00000400;
    static const unsigned int TRIGGER_3    = 0x00000800;
    static const unsigned int TRIGGER_4    = 0x00001000;
    static const unsigned int TRIGGER_5    = 0x00002000;
    static const unsigned int TRIGGER_6    = 0x00004000;
    static const unsigned int TRIGGER_7    = 0x00008000;
    static const unsigned int TRIGGER_8    = 0x00010000;
    static const unsigned int TRIGGER_9    = 0x00020000;
    static const unsigned int TRIGGER_10   = 0x00040000;
    static const unsigned int TRIGGER_11   = 0x00080000;
    static const unsigned int TRIGGER_12   = 0x00100000;
    static const unsigned int TRIGGER_13   = 0x00200000;
    static const unsigned int TRIGGER_14   = 0x00400000;
    static const unsigned int TRIGGER_15   = 0x00800000;
    static const unsigned int TRIGGER_MASK = 0x00ffff00;

    // M E M B E R  V A R I A B L E S //


    /* Pointer to send start and stop codes to the usb driver*/
    PCHAR mpUsbdata;

    /* Data buffer in bytes */
    PCHAR mpData;

    /* Data buffer as ints */
    int * mpDataAsInt;

    /* True if this is an ActiveTwo Mk2 */
    bool mMk2;

    /* True if the battery is low, Note this is only updated
     * at initialization and on calles to isBatteryLow
     */
    bool mBattLow;

    /* Sampling rate of biosemi in Hz */
    int mSamplingRate;

    /* The actual sample block size we are using based */
    int mSampleBlockSize;

    /* Sampling rate the user desires */
    int mDesiredSamplingRate;

    /* Number of channels the user desires. */
    int mDesiredNumChannels;

    /* Sample block size the user desires */
    int mDesiredSampleBlockSize;

    /* Number of channels the biosemi is transmitting, including status and sync */
    int mNumChannels;

    /* Biosemi's mode, only updated on initialization*/
    int mMode;

    /* handle to the biosemi device */
    HANDLE mDevice;

    /* The position of BIOSEMI's buffer cursor */
    PDWORD mpBufferCursorPos;

    /* Number of ints that were buffered between sucessive callsto GET_POINTER*/
    long mIntsBuffered;

    /* The previous position of the buffer cursor */
    DWORD  mOldBufferCursorPos;

    /* The position to start reading from the data buffer as an int*/
    long mStartPos;

    /* The next position to start reading in the data buffer */
    long mNextPos;

    /* Number of ints ready to be read from the buffer */
    long mIntsAvailable;

    /* The number of ints that are ready to be read, these are accumulated */
    int mNumIntsToRead;

    /* True if the get data function has been called */
    bool mIsDataReadyCalledYet;

    /* If initialized was called */
    bool mWasDriverSetup;

    /* How many samples you need to skip to achieve the desired sampling rate*/
    int mDecimationFactor;

    /* Pointer to the datablock that helps the user travers the data received on
    the last call to isDataReady */
    DataBlock * mpDataBlock;

/*******************************************************************************
    To interface your own acquisition software directly with our USB2 interface,
    your acquisition software should make the function calls described below.
    The USB2 driver functions are in the library file called "Labview_DLL.dll".
    (this library file is included with ActiView). The functions used in the
    USB2 handshake are:

    HANDLE OPEN_DRIVER_ASYNC(void);
    BOOL USB_WRITE(HANDLE hdevice, PCHAR data);
    BOOL READ_MULTIPLE_SWEEPS(HANDLE hdevice,PCHAR data,DWORD nBytesToRead);
    BOOL READ_POINTER(HANDLE hdevice,PDWORD pointer);
    BOOL CLOSE_DRIVER_ASYNC(HANDLE hdevice);
*******************************************************************************/

    // I N N E R  C L A S S E S //

    public:
    /*
     * Datablock presents the data with the sampleblockSize, number of
     * channels, and sampingRate the caller desires (i.e. what
     * they passed to Biosemi2Client::initilaize
     *
     * Warning: Calls to isDataReady will change what data block DataBlock
     * accesses. So make sure you're done traversing the block before
     * calling isDataReady again, the DataBlock class cannot check for this
     */
    class DataBlock{
      public:

        DataBlock( const Biosemi2Client *BIOSEMI );

        virtual ~DataBlock();

        /*
         * Get's the data for the sample and channel specified. Acceses
         * basically like a 2d matrix. It ignores sync and status channels
         * so channel 0 is the first EEG channel
         * Signal is returned with least significat bit as 1/8192 microVolts
         * see http://www.biosemi.com/faq/make_own_acquisition_software.htm
         */
        virtual int getSignal(int sample,
            int channel) const;

        /*
         * Gets the value of the trigger specified, scaled if desired.
         * Scaled defaults to 1. Triggers are 0 for low and 1 for high
         * so scalling by 1000 would be 0 low 1000 high.
         * Scalling can be used to bring the value in range with
         * the signal for display purposes
         */
        virtual int getTrigger(int sample, int trigger,
            int scaled=1 ) const;

        virtual unsigned int getAllTriggers(int sample) const;

        /*
         * Loop through the sync and status channels and make sure this data
         * is valid. Throws an exception stating what was invalid otherwise
         * returns true. TODO should probably return a status
         */
        virtual bool isDataValid() const;

        virtual int getNumChannels();

        virtual int getSampleBlockSize();

        virtual int getSamplingRate();

protected:
        const Biosemi2Client *mpBiosemi;
    };

    /*
     * Allow DataBlock access to Biosem2Client's protected and private member
     variables.
     */
    friend class DataBlock;

};
#endif
