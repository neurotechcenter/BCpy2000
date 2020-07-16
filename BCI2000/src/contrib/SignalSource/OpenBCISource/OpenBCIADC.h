////////////////////////////////////////////////////////////////////////////////
// Authors: 
// Description: OpenBCIADC header
////////////////////////////////////////////////////////////////////////////////

#ifndef INCLUDED_OPENBCIADC_H  // makes sure this header is not included more than once
#define INCLUDED_OPENBCIADC_H

#include "GenericADC.h"
#include <set>
#include "Win32Defs.h"


//define constant

//baud rate
#define DEFAULT_BAUD CBR_115200

//use 8 channels
#define EEG_CHANNELS_8 8

//use 16 channels 
#define EEG_CHANNELS_16 16 

//reference voltage 
#define ADS1299_VREF 4.5     

//buffer size 
#define BUFFERSIZE 33  

//sample block size 
#define SAMPLEBLOCKSIZE 125

//count three dollar sign  
#define THREE_DOLLAR_SIGN 3

//read buffer size 
#define READ_BUFFER_SIZE 5000

//default sampling rate 
#define SAMPLING_RATE 250

//data block size 
#define DATA_BLOCK_SIZE 255

//conversion factor for gain
#define CONVERSION_FACTOR 0.000000061362

//7 number for possible gains 
#define SIZE_OF_POSSIBLE_GAINS 7

//valid handshake iterations 
#define VALID_HANDSHAKE_TIME 200

//number of channel settings 
#define CHANNEL_SETTINGS 9

//possible gains 
#define GAIN_1 1
#define GAIN_2 2
#define GAIN_4 4
#define GAIN_6 6
#define GAIN_8 8
#define GAIN_12 12

//default channel gains 
#define DEFAULT_CHANNEL_GAIN 24

//number of com ports 
#define Num_Com_Port 21 


class OpenBCIADC : public GenericADC
{
 public:
  OpenBCIADC();
  virtual ~OpenBCIADC();
  virtual void AutoConfig( const SignalProperties& );
  virtual void Preflight( const SignalProperties&, SignalProperties& ) const;
  virtual void Initialize( const SignalProperties&, const SignalProperties& );
  virtual void Process( const GenericSignal&, GenericSignal& );
  virtual void Halt();
  HANDLE OpenPort(const char* DevName);
  int ReadSerial(HANDLE fd, unsigned char buf[], int size);
  bool StartStreaming(HANDLE fd);
  bool StopStreaming(HANDLE fd);
  int BufferHandler(unsigned char buf[]);
  bool PreStreaming(HANDLE fd);
  void ByteParser();
  void Streaming(HANDLE fd);
  void SetChannels(HANDLE fd);
  void ReadChannelSetting(HANDLE fd);
  void ResetBoard(HANDLE fd);
  void SetNumChannel(HANDLE fd);
  void SetDefault(HANDLE fd);
  void ReadBuffer(HANDLE fd, int numbytes);

  //structure used to store data 
  struct Packet
  {
	int OutPut[EEG_CHANNELS_8][SAMPLEBLOCKSIZE]; 
	int BlockNum[SAMPLEBLOCKSIZE];
  }   mPacket;

  private:

  //member variables
  HANDLE mfd;

  //used to store incoming data 
  unsigned char mParseBuffer[BUFFERSIZE*SAMPLEBLOCKSIZE];

  //flag to indicate if streaming
  bool mIsStreaming;

  //flag indicates if receive $$$
  int mDollaBill;

  //count how many bytes have been added 
  int mNumBytesAdd;

  //counter 
  int mcount;

  //flag to indicate if the first time stream 
  bool mfirst_time;

  //flag to indicate if the port is open 
  bool mOpenPort;

  //array stores the gain for every channel
  int mChannel_Gain[EEG_CHANNELS_16];

  //record the sample block size
  int mSampleBlockSize;

  //store the channels that will be displayed 
  std::vector<int> mSourceChList;

  //stor the current port 
  int mCurrentPort;

  //store number of channels 
  int mNumChannels;

  //flag to indicate use 8 channel mode or 16 channel mode 
  bool mUse8Channel;

  //flag to indicate if user sets the configure or not 
  bool msetted_config;

  //count number of lost channels 
  int mNumlostsample;

  //store the index for the previous sample
  int mPreSampleNum;

  //store the last values for the first 8 channels
  int mPre_8ch[EEG_CHANNELS_8];

  //store the last values for the last 8 channels 
  int mLast_8ch[EEG_CHANNELS_8];

  //fage to indicate if show lost packages
  bool ShowLostPages;

};

#endif 
