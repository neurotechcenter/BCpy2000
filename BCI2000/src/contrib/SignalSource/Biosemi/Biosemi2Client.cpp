/**
 * Author: Samuel A. Inverso (samuel.inverso@gmail.com),  Yang Zhen
 * Contributors: Many thanks to the Biosemi.nl forum members
 * Description: Client to acquire from the USB BIOSEMI 2 Labview_DLL.
 * Note: This class is not thread safe
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

#include <string>
#include <iostream>

#include "BCIStream.h"
#include "Debugging.h"

#include "Biosemi2Client.h"
#include "Labview_DLL.imports.h"

using namespace std;

Biosemi2Client::Biosemi2Client() :
    mpUsbdata(new char[USB_DATA_SIZE]),
    mpData(new char[BUFFER_SIZE_IN_BYTES]),
    mpDataAsInt( NULL ),
    mMk2( false ),
    mBattLow( false ),
    mSamplingRate( -1 ),
    mSampleBlockSize( 0 ),
    mDesiredSamplingRate( -1 ),
    mDesiredSampleBlockSize( -1 ),
    mNumChannels(-1),
    mMode( 0 ),
    mDevice( NULL ),
    mpBufferCursorPos(new DWORD),
    mIntsBuffered( 0 ),
    mOldBufferCursorPos(0),
    mStartPos(0),
    mNextPos(0),
    mIntsAvailable(0),
    mNumIntsToRead( 0 ),
    mIsDataReadyCalledYet(false),
    mDecimationFactor( 1 ),
    mWasDriverSetup(false),
    mpDataBlock(NULL)
{

// All this code assumes 1 byte chars and 4 byte ints, For sanity sake
// make sure that's what we are getting. All bets are off otherwise.

    Assert(sizeof(int) == 4);
    Assert(sizeof(char) == 1);

    mpDataAsInt = reinterpret_cast<int *>(mpData);
}

Biosemi2Client::~Biosemi2Client(){
    halt();
    delete mpDataBlock;
    delete[] mpData;
    delete[] mpUsbdata;
    delete mpBufferCursorPos;
}

void Biosemi2Client::setupDriver(){
  mWasDriverSetup = false;
/*******************************************************************************
To acquire data, there are 7 step we need to go.
*******************************************************************************/

/*******************************************************************************
Step 1:  "Initializing the USB2 interface"
Call function OPEN_DRIVER_ASYNC, the output provides a "handle" parameter,
used as an input for further function calls.
*******************************************************************************/

    mDevice=OPEN_DRIVER_ASYNC();
    if( mDevice == NULL || mDevice == INVALID_HANDLE_VALUE )
    {
      bcierr << "Could not connect to device." << endl;
      return;
    }

/*******************************************************************************
Step 2:  "Start the acquisition with initializing the USB2 interface"
Call USB_WRITE, inputs are "handle" (from step 1) and "data",
use an array of 64 zero bytes for the "data" input.
*******************************************************************************/

      memset( mpUsbdata,0,USB_DATA_SIZE);

      if( !USB_WRITE(mDevice, mpUsbdata) )
      {
        bcierr << "Could not initialize USB interface." << endl;
        return;
      }

/*******************************************************************************
Step 3:  "Initialize the ringbuffer"
Call READ_MULTIPLE_SWEEPS, inputs are "handle" (from step 1),
"data" and "nBytes to read".
Use an initialized buffer of 32 Mbyte (recommended size) as input
for "data" (this is the ringbuffer),
and use the buffersize as input for "nBytes to read" (33554432 = 2^25).
Every thing is now set to start with the acquisition,
and the only thing to do is to enable the handshake.
*******************************************************************************/

        // This is probably supposed to remove any pending data, so we don't report an error if it fails.
        READ_MULTIPLE_SWEEPS(mDevice, mpData, BUFFER_SIZE_IN_BYTES);

/*******************************************************************************
Step 4:  "Enable the handshake"
Call again USB_WRITE, inputs are "handle" (from step 1)
and "data", this time use an array of 1 byte of 255 (FF) followed by 63 zero bytes
for the "data" input.

http://www.biosemi.nl/forum/viewtopic.php?t=16&highlight=usbwrite
The "data" input of USB_WRITE is an array of 64 Bytes.
The first Byte is used to start/stop the acquisition (00=stop, FF= start).
The second and third byte are used to control the outgoing triggers
(example: 01 + 00 makes pin 17 High, and 18-31 Low ).
The remaining 61 bytes are not used.

At this moment the handshake with the USB2 interface starts
(green "USB link" LED on receiver switches on),
and the ringbuffer is filled with incoming data from the ActiveTwo.
Your acquisition program should read the proper data for the ringbuffer.
*******************************************************************************/
          mpUsbdata[0]=1;
          if( !USB_WRITE(mDevice, mpUsbdata) )
          {
            bcierr << "Could not enable the handshake." << endl;
            return;
          }

// tells halt it can actually stop the device or bad things happen

    mWasDriverSetup = true;
}


bool Biosemi2Client::initialize( int desiredSamplingRate,
    int desiredSampleBlockSize, int desiredNumChannels ){

// Store the signal attributes the caller wants

    mDesiredSamplingRate = desiredSamplingRate;
    mDesiredNumChannels  =  desiredNumChannels;
    mDesiredSampleBlockSize = desiredSampleBlockSize;

// setup the biosemi driver

    setupDriver();
    if( !mWasDriverSetup )
      return false;

// Get a few data points to check the status channel and get the number of
// channels, sampling rate, etc.

    for( int i = 0; i < 10 ; i++ ){

        if( !READ_POINTER(mDevice, mpBufferCursorPos) )
        {
          bcierr << "Could not get device status." << endl;
          return false;
        }
        Sleep (10);
    }

// If we haven't see two ints by this time either the power is probably off

    if( *mpBufferCursorPos < BYTES_PER_INT*2){
        bcierr << "No cord or power" << endl;
        return false;
    }

// Decode the status channel and place the results in the member variables passed

    bool ignored;
    decodeStatus( mpDataAsInt, 0, mMk2, mBattLow, mMode, mNumChannels,
        mSamplingRate, ignored, ignored);


// Check if the desired sampling rate evenly divides the actual sampling rate.

    if( 0 != mSamplingRate % mDesiredSamplingRate ){
        bcierr << "Sampling rate requested: " << mDesiredSamplingRate
            << " does not evenly divide biosemi sampling rate: "
            << mSamplingRate << endl;
        return false;
    }

// Determine how much we have to decimate the sample to achieve the requested
// sampling rate. (Basically how many samples we need to skip)
//This also tells us the actual sampleblock size we need

    mDecimationFactor = mSamplingRate/mDesiredSamplingRate;
    mSampleBlockSize = mDecimationFactor* mDesiredSampleBlockSize;

// Deterimine how many ints need to be buffered before we can start reading

    mNumIntsToRead = mSampleBlockSize * mNumChannels;

//stop the driver or else data will be out of sync on the getData call

    halt();

    mIntsBuffered =0;
    mOldBufferCursorPos = 0;
    *mpBufferCursorPos = 0;
    mStartPos = 0;
    mNextPos=0;
    mIsDataReadyCalledYet = false;

    return true;
}

// Returns a pointer to the data buffer

const int * Biosemi2Client::getData() const{
    return mpDataAsInt;
}

void Biosemi2Client::halt(){

// only halt if we were initialized

    if(mWasDriverSetup){
/*******************************************************************************
Step 6:  "Disable the handshake"
Call USB_WRITE, inputs are "handle" (from step 1) and "data",
use an array of 64 zero bytes for the "data" input.
Finally, the interface drivers should be closed:
*******************************************************************************/

        mpUsbdata[0]=0;

        USB_WRITE(mDevice, mpUsbdata);

/*******************************************************************************
Step 7:  "Close the drivers"
    Call CLOSE_DRIVER_ASYNC, input is "handle" (from step 1).

    If all goes OK, you will receive the following data format from the
    ActiveTwo:
    The ActiveTwo sends the following number of 32-bit words per sample:

    speedmode 0 and 4: 258
    speedmode 1 and 5: 130
    speedmode 2 and 6: 66
    speedmode 3 and 7: 34
    speedmode 8: 290

    The receiver converts every 24-bit word from the AD-box into a
    32-bit Signed Integer, by adding an extra zero Least Significant Byte to
    the ADC data. The 24-bit ADC output has an LSB value of 1/32th uV.
    The 32-bit Integer received for the USB interface has an LSB value
    of 1/32*1/256 = 1/8192th uV


*******************************************************************************/

        CLOSE_DRIVER_ASYNC(mDevice);

        mWasDriverSetup= false;
        mDevice=NULL;
    }
}


int Biosemi2Client::getNumChannels() const{
    return mNumChannels;
}

int Biosemi2Client::getNumAIBChannels() const{
    if (mMode == 8) return NUM_AIB_CHANNELS;
    else return 0;
}

int Biosemi2Client::getNumEEGChannels() const{
    return mNumChannels - FIRST_EEG_CHANNEL - getNumAIBChannels();
}

int Biosemi2Client::determineNumChannels(int mode, bool isMk2) const{
    int result = -1;
    if( isMk2 ){
        result = determineNumChannelsMk2(mode);
    }
    else{
        result = determineNumChannelsMk1(mode);
    }
    return result;
}

// http://www.biosemi.com/faq/adjust_samplerate.htm
int Biosemi2Client::determineNumChannelsMk1(int mode) const
{
    int result;
    switch(mode){
        case 0:
        case 4:
        case 8:
            result = 258;
            if(mode == 8) result += NUM_AIB_CHANNELS;  // 2 + 256 + 32 = 290
            break;
        case 1:
        case 5:
            result =  130;
            break;
        case 2:
        case 6:
            result = 66;
            break;
        case 3:
        case 7:
            result = 34;
            break;
        default:
            result= -1;
            bcierr << "mode unacceptable" << endl;
    }
    return result;
}

// http://www.biosemi.com/faq/adjust_samplerate.htm
int Biosemi2Client::determineNumChannelsMk2(int mode) const
{
    int result;
    switch(mode){
        case 0:
        case 1:
        case 2:
        case 3:
            result = 610; // 2+4*152
            break;
        case 4:
        case 8:
            result = 282;
            if(mode == 8) result += NUM_AIB_CHANNELS; // 2 + 280 + 32 = 314
            break;
        case 5:
            result = 154;
            break;
        case 6:
            result = 90;
            break;
        case 7:
            result = 58;
            break;
        default:
            result= -1;
            bcierr << "mode unacceptable" << endl;
    }
    return result;
}


int Biosemi2Client::determineSamplingRate(int mode) const{
    int result;
    switch(mode){
        case 0:
        case 4:
        case 8: // AIB mode (which means the ADC's must be in speed mode 4 according to http://www.biosemi.com/faq/adjust_samplerate.htm )
            result = 2048;  // 2 kHz
            break;
        case 1:
        case 5:
            result =  4096; // 4 kHz
            break;
        case 2:
        case 6:
            result = 8192;  // 8 kHz
            break;
        case 3:
        case 7:
            result = 16384; // 16 kHz
            break;
        default:
            result= -1;
            bcierr << "mode unacceptable" << endl;
    }
    return result;
}

//Two extra channels are leading the ADC data: before:
//channel 1 = sync (check for FFFFFF00) and channel 2 = Status (see
// http://www.biosemi.com/faq/trigger_signals.htm),channels 3-258 are ADCs 1-256.

void Biosemi2Client::decodeStatus(const int * data, int position,
    bool &mk2, bool &battLow, int &mode, int &numChannels, int &samplingRate,
    bool &cmsWithinRange, bool &newEpoch) const{
    int status = data[position+1];
    mk2 = isMK2(status);
    battLow = isBatteryLow(status);

    mode = statusToMode(status);

    numChannels = determineNumChannels(mode,mk2);

    samplingRate = determineSamplingRate(mode);

    cmsWithinRange = status & CMS_WITHIN_RANGE_MASK;

    newEpoch = status & NEW_EPOCH_MASK;

}

// Checks if the first sample  starting at startPos  in data is synced
bool  Biosemi2Client::isDataSynced(const int * data, int position) const{
    return isSampleSynced(data[position]);
}


bool Biosemi2Client::isSampleSynced( int sample ) const{
    return IS_SYNCED_MASK == sample;
}

int Biosemi2Client::statusToMode(const int *  data, int position) const{
    return statusToMode(data[position]);
}

// Returns the mode corresponding to the one on the front of the biosemi box

int Biosemi2Client::statusToMode( int status) const{
    int result = ((status & MODE_MASK) >> MODE_SHIFT_TO_INT);

    if (result & 0x10) {
        result |=0x8;
        result &=0xF;
    }
    return result;
}


int Biosemi2Client::getMode() const{
    return mMode;
}



int Biosemi2Client::getSamplingRate() const{
    return mSamplingRate;
}


//Returns true if the data is valid, false if the battery is low

bool Biosemi2Client::isDataValid(const int * data, int startPos ) const{
    int datum;
    bool result = true;
    for( int sample =0 ; sample < mSampleBlockSize ; sample++ ){
        for( int channel=0; channel <=1 ; channel++ ){
            datum =  mpData[(sample * mNumChannels+channel+startPos)
                        % Biosemi2Client::BUFFER_SIZE_IN_INT];
            if( Biosemi2Client::SYNC_CHANNEL == channel ){
                result = isSampleSynced(datum);
            }
            else if( Biosemi2Client::STATUS_CHANNEL == channel ){
                result = isStatusValid(datum);

            }
        }
    }
    return result;
}

// Validates the current data, it's up to the caller to make sure there's
// a full sampleblock

bool Biosemi2Client::isDataValid() const{
    return isDataValid( mpDataAsInt,mStartPos);
}

int Biosemi2Client::getSampleBlockSize() const{
    return mSampleBlockSize;
}

bool Biosemi2Client::isStatusValid( int sample ) const{
    bool result = true;
    // ??
    return result;
}

/*******************************************************************************
Blocks until blockSize number of elements are ready to read for each channel
Note: does not do any data validating. You need to do that yourself
Parameters:
blockSize = int number of samples you want per channel
index = int the index to start reading from in the data buffer
Note: it's a circular buffer and may rap around so you have to use modulus
numAvailable = int the number of data samples available to read from the buffer
*******************************************************************************/
void Biosemi2Client::isDataReady(){

/*******************************************************************************
    Step 5:  "generate a file pointer"
    Call READ_POINTER to generate a file pointer.
    Inputs are "handle" (from step 1), and "pointer (0).
    Output is a "pointer". Use this parameter as a file pointer.
    In our ActiVIEW software, we have two counters running continuously:
    one counter tracks the total of the buffered words,
    the other counts the total of words displayed on screen
    (or written to file).

    Every time the difference in the counters exceeds a set amount of words,
    this amount of words is displayed on screen or written to file.
    The difference between the two counters is the "backlog" value,
    as soon as it exceeds the total ringbuffer size,
    an overflow error is generated and the program stops.
    To stop the acquisition you will have to disable the handshake.
*******************************************************************************/

    if( ! mIsDataReadyCalledYet ) {
        setupDriver();
        mIsDataReadyCalledYet = true;
    }


     mStartPos = mNextPos;

// Wait until there is enough data in the buffer before returning

    while( mIntsAvailable < mNumIntsToRead){
        Sleep(10);
        if( !READ_POINTER(mDevice, mpBufferCursorPos) )
        {
          bcierr << "Could not read data from device." << endl;
          return;
        }

// Convert bufferCursorPos to integer based vs byte based

        *mpBufferCursorPos /= BYTES_PER_INT;

        if( *mpBufferCursorPos >= mOldBufferCursorPos ){
            mIntsAvailable += *mpBufferCursorPos - mOldBufferCursorPos;
        }
        else {
            mIntsAvailable+=0+ *mpBufferCursorPos + BUFFER_SIZE_IN_INT -
                mOldBufferCursorPos;
        }

       mOldBufferCursorPos = *mpBufferCursorPos;
    }

// check if we have overrun the buffer

    if( mIntsBuffered > BUFFER_SIZE_IN_INT ){
        bcierr << "Buffer overrun error" << endl;
        return;
    }

    mNextPos =  (mStartPos + mNumIntsToRead) % BUFFER_SIZE_IN_INT;

    mIntsAvailable=mIntsAvailable - mNumIntsToRead;
}

void Biosemi2Client::isDataReady(int &startPos){

// Tell the caller where they should start reading from.

    startPos = mStartPos;
    isDataReady();
}

bool Biosemi2Client::isTriggerHigh( int trigger, int datum ) const{
    bool result = false;
    switch( trigger ){
        case 0:
            result = datum & TRIGGER_0;
            break;
        case 1:
            result = datum & TRIGGER_1;
            break;
        case 2:
            result = datum & TRIGGER_2;
            break;
        case 3:
            result = datum & TRIGGER_3;
            break;
        case 4:
            result = datum & TRIGGER_4;
            break;
        case 5:
            result = datum & TRIGGER_5;
            break;
        case 6:
            result = datum & TRIGGER_6;
            break;
        case 7:
            result = datum & TRIGGER_7;
            break;
        case 8:
            result = datum & TRIGGER_8;
            break;
        case 9:
            result = datum & TRIGGER_9;
            break;
        case 10:
            result = datum & TRIGGER_10;
            break;
        case 11:
            result = datum & TRIGGER_11;
            break;
        case 12:
            result = datum & TRIGGER_12;
            break;
        case 13:
            result = datum & TRIGGER_13;
            break;
        case 14:
            result = datum & TRIGGER_14;
            break;
        case 15:
            result = datum & TRIGGER_15;
            break;
        default:
            bcierr << "Illegal trigger " << trigger
                << " (note triggers are numbered starting from 0 in Biosemi2Client::isTriggerHigh)."
                << endl;
    }
    return result;
}

bool Biosemi2Client::isBatteryLow(int datum) const{
    return datum & BATT_LOW_MASK;
}

bool Biosemi2Client::isBatteryLow() const{
    return isBatteryLow(mpDataAsInt[calcIndex(mDesiredSampleBlockSize-1,STATUS_CHANNEL)]);
}

bool Biosemi2Client::isMK2() const{
    return mMk2;
}
bool Biosemi2Client::isMK2(int status) const{
    return status & MK2_MASK;
}

Biosemi2Client::DataBlock& Biosemi2Client::getDataBlock(){
    if(NULL == mpDataBlock){
        mpDataBlock = new DataBlock(this);
    }
    return * mpDataBlock;
}

int Biosemi2Client::calcIndex(int sample,
    int channel, int startPos) const{
    return ( startPos+sample
          * mNumChannels
          * mDecimationFactor
          + channel)% Biosemi2Client::BUFFER_SIZE_IN_INT;

}
int Biosemi2Client::calcIndex(int sample,
    int channel) const{
    return calcIndex(sample, channel, mStartPos);
}
double Biosemi2Client::averageSamples(int sample, int channel) const{
    double total = 0.0;
    for(int i=0,offset=mStartPos; i<mDecimationFactor; i++,offset+=mNumChannels)
        total += (double)mpDataAsInt[calcIndex(sample, channel, offset)];
    return total / (double)mDecimationFactor;
}




// D A T A B L O C K   I M P L E M E N T A T I O N


Biosemi2Client::DataBlock::DataBlock( const Biosemi2Client *BIOSEMI ):
    mpBiosemi(BIOSEMI){
}

Biosemi2Client::DataBlock::~DataBlock(){
}


int Biosemi2Client::DataBlock::getTrigger(int sample, int trigger, int scaled ) const{
    int val, out = 0;
    for(int i=0, offset=mpBiosemi->mStartPos; i<mpBiosemi->mDecimationFactor; i++, offset+=mpBiosemi->mNumChannels) {
        val = mpBiosemi->mpDataAsInt[mpBiosemi->calcIndex(sample,Biosemi2Client::STATUS_CHANNEL,offset)];
        val = mpBiosemi->isTriggerHigh(trigger, val);
        out |= val;
    }
    return out * scaled;
}

unsigned int Biosemi2Client::DataBlock::getAllTriggers(int sample) const{
    unsigned int val, out = 0;
    for(int i=0, offset=mpBiosemi->mStartPos; i<mpBiosemi->mDecimationFactor; i++, offset+=mpBiosemi->mNumChannels) {
        val  = (unsigned int)mpBiosemi->mpDataAsInt[mpBiosemi->calcIndex(sample,Biosemi2Client::STATUS_CHANNEL,offset)];
        val &= (unsigned int)Biosemi2Client::TRIGGER_MASK;
        val /= (unsigned int)Biosemi2Client::TRIGGER_0;
        out |= val;
    }
    return out;
}

// Loops through the signal channels starting at 0
// (so it pretends sync and status do not exist)

int Biosemi2Client::DataBlock::getSignal(int sample,
    int channel) const{
    if( sample > mpBiosemi->mDesiredSampleBlockSize ){
        bcierr << "Sample out of bounds error." << endl;
        return 0;
    }
    else if( channel > mpBiosemi->mNumChannels ){
        bcierr << "Channel out of bounds error"
               << " (" << channel << ">" << mpBiosemi->mNumChannels << ")"
               << "." << endl;
        return 0;
    }
    // skip the sync and status channels

    // return mpBiosemi->mpDataAsInt[mpBiosemi->calcIndex(sample,channel)];
    return (int)(0.5 + mpBiosemi->averageSamples(sample, channel));
}

// Loop through the sync and status channels and make sure this data is valid

bool Biosemi2Client::DataBlock::isDataValid() const{
    return mpBiosemi->isDataValid();
}

int Biosemi2Client::DataBlock::getNumChannels(){
    return mpBiosemi->mDesiredNumChannels;
}

int Biosemi2Client::DataBlock::getSampleBlockSize(){
    return  mpBiosemi->mDesiredSampleBlockSize;
}

int Biosemi2Client::DataBlock::getSamplingRate(){
    return mpBiosemi->mDesiredSamplingRate;
}
