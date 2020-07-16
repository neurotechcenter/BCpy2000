////////////////////////////////////////////////////////////////////////////////
// Authors:
// Description: OpenBCIADC implementation
////////////////////////////////////////////////////////////////////////////////

#include "OpenBCIADC.h"
#include "BCIStream.h"
#include "BCIEvent.h"
#include <cstring>
#include <string>
#include <windows.h>
#include <winbase.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <tchar.h>
#include <strsafe.h>
#include <math.h>
#include "Clock.h"
#include <time.h>
#include "GenericSignal.h"

using namespace std;




// Make the source filter known to the framework.
// ADC filters must be registered at location "1" in order to work.
RegisterFilter( OpenBCIADC, 1 ); 

//constructor
OpenBCIADC::OpenBCIADC()
:mIsStreaming(false),
mDollaBill(0),
mNumBytesAdd(0),
mcount(0),
mfirst_time(true),
mOpenPort(false),
mCurrentPort(0),
mNumChannels(EEG_CHANNELS_8),
mSampleBlockSize(0),
msetted_config(false),
mUse8Channel(true),
mNumlostsample(0),
mPreSampleNum(0),
ShowLostPages(0)
{
	//initialize the gain for all 16 channels 
	for (int i = 0; i < EEG_CHANNELS_16; i++)
	{
		 mChannel_Gain[i] = 1;
	}
	//user specified parameters 
	BEGIN_PARAMETER_DEFINITIONS
	"Source string PortName= auto "
	"// This software can automatically detect connection",

    "Source int SourceCh= auto "
     " // Number of channels of Open BCI is equals to the number of channels list in source channel list",

    "Source int SampleBlockSize= auto"
     " // Number of samples transmitted at a time for OpenBCI can be 125",

    "Source float SamplingRate= auto"
     " // Sample rate will be automatically setted",

	 "Source floatlist SourceChGain= 1 auto"
     " // Conversion factors from AD units into Microvolts",

	 "Source list ChannelNames= 1 auto"
	 " // The number of channel names must match ",

     "Source list ChannelsGain= 1 auto 24 1"
	 " // The channel gain can be 1 2 4 6 8 12 24(defult)",

	 "Source intlist SourceChList= 1 auto 16 1"
	 " // The default source channel list is 1 2 3 4 5 6 7 8",

	 "Source int ShowLostPackages= 1 1 0 1",


     END_PARAMETER_DEFINITIONS

	 BEGIN_STREAM_DEFINITIONS
	 
	 "LostSampleindicator 1 0 ",

     END_STREAM_DEFINITIONS
  
}

//destructor
OpenBCIADC::~OpenBCIADC()
{
    //close handle
	StopStreaming( mfd);
	bool close_h = CloseHandle(mfd);
	bciout<<"Close handle "<<close_h<<endl;
}



//this function is to create a new handle and set up the port
HANDLE OpenBCIADC::OpenPort(const char* DevName)
{
	//innitialize a new handle
	HANDLE fd =NULL;
	//initialize a new dcb
	DCB dcb;
	//open the port
	fd = CreateFile(DevName,GENERIC_READ | GENERIC_WRITE,
                    0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	//if fail to open the port 
	if (fd == INVALID_HANDLE_VALUE)  
	{	
		//print the error
		bciout<<"fail to open stream1"<<endl;
		return fd;
	}
	//if fail to set the mask 
	if (!SetCommMask(fd, EV_RXCHAR)) 
	{	
		//print the error 
		bciout<<"fail to open stream2"<<endl;
		return(INVALID_HANDLE_VALUE);
	}
	//set the size of read and write buffer 
	SetupComm(fd, BUFFERSIZE*SAMPLEBLOCKSIZE*10, BUFFERSIZE*SAMPLEBLOCKSIZE*10);	
	//set the length of dcb 
	dcb.DCBlength= sizeof(DCB);
       	GetCommState(fd, &dcb);
	//set common state for the handel 
    dcb.BaudRate= DEFAULT_BAUD;   // default = 115200 , 8 N 1
	dcb.ByteSize= 8;
	dcb.Parity= NOPARITY;
	dcb.StopBits= ONESTOPBIT;
	dcb.fOutxCtsFlow= FALSE;
	dcb.fOutxDsrFlow= FALSE;
	dcb.fInX= FALSE;
	dcb.fOutX= FALSE;
	dcb.fDtrControl= DTR_CONTROL_DISABLE;   // no hardware handshake
	dcb.fRtsControl= RTS_CONTROL_DISABLE;
	dcb.fBinary= TRUE;
	dcb.fParity= FALSE;
 	if (!SetCommState(fd, &dcb)) 
	{	
		bciout<<"fail to open stream3"<<endl;
		return (INVALID_HANDLE_VALUE);
	}
	//send a byte to reset the board 
	char *buf = "v";
	DWORD len = 1;
	bool wri =  WriteFile(fd,(LPSTR)buf,len,&len,NULL);
	if(wri)
	bciout<<"Finish open port"<<endl;
	return fd;
}

// this fuction reads max bytes from the serial port,
//  returns number of bytes read, or -1 if error */
int OpenBCIADC::ReadSerial(HANDLE fd, unsigned char buf[], int size)
{
   BOOL rf;
   COMSTAT comStat;
   DWORD errorFlags;
   DWORD len ;

   //require the information about the setting of the buffer 
   ClearCommError(fd, &errorFlags, &comStat);   
   len = comStat.cbInQue;
  
   //if the number of bytes that the serial provides is larger
   //than the number of bytes reuqired 
   //set the length to the size 
   if (len > (DWORD) size) len = size;

   // if read something
   if (len > 0)
   {
	   //read the file into the buffer 
	   rf = ReadFile(fd, (LPSTR)buf, len, &len, NULL);
	   //if not succeeded  
	   if (!rf)
	   {
			len= 0;
			//check the error flag 
			ClearCommError(fd, &errorFlags, &comStat);
       }

	   //if there is an error 
       if (errorFlags > 0)
       {
			ClearCommError(fd, &errorFlags, &comStat);
			return -1;
       }
    }
    return len;
}

//this function tells openbic to start streaming
bool OpenBCIADC::StartStreaming(HANDLE fd)
{
    mIsStreaming = true;
	char *buf = "b";
	DWORD len = 1;

	//send command to board
	bool wri = WriteFile(fd,(LPSTR)buf,len,&len,NULL);
	
	//if successfully write
	if(len==1)
	{
		bciout<<"Start streaming"<<endl;
	}
	return wri;
}

//this function tells openbic to stop streaming
bool OpenBCIADC::StopStreaming(HANDLE fd)
{
    mIsStreaming = false;
	char *buf = "s";
	DWORD len = 1;

	bool wri = WriteFile(fd,(LPSTR)buf,len,&len,NULL);
	
	//if successfully write 
	if(len == 1)
	{
		bciout<<"Stop streaming"<<endl;
	}
	return wri;
}

//this function is used to handle input data from the device and 
//put the data into buffer
// return 1 == read $$$ ready to start
// return 2 == it's streaming
// return 3 == fail to receive $$$
int OpenBCIADC::BufferHandler(unsigned char buf[])
{
	//check if the streaming is start 
	if(mIsStreaming == false)
	{
		//check if read three $
		if(buf[0] == '$')
		{
			mDollaBill++;
		}
		else if(buf[0] != '$' && mDollaBill > 0) mDollaBill = 0;
		//if receive 3 $ sign
		if(mDollaBill == THREE_DOLLAR_SIGN)
		{
			mDollaBill = 0; 

			//if read the handshake
			return 1;
		}
	}
	//check if the streaming is starting
	else if (mIsStreaming == true)
	{
		//add the new data into the buffer 
		mParseBuffer[mNumBytesAdd] = buf[0];

		//increament the # of bytes in the buffer 
		mNumBytesAdd++;

		//if already streaming 
		return 2;
	}

	//if fail to start streaming 
	return 3;
}

//this function is called before the streaming start
bool OpenBCIADC::PreStreaming(HANDLE fd)
{
	int len;
	unsigned char buf[1];
	int BufferValue;

	// continuously read until read handshake 
	while(1)
	{
		len = ReadSerial(fd,buf,1);

		// if it's the first time 
		if (mfirst_time)
		{
			Sleep(20);
			mcount++;
		}

		//if didn't see hand shake after 4 second
		//this port is invalid
		if (mcount>VALID_HANDSHAKE_TIME)
		{
			mcount=0;
			return 0;
		}

		//if read something
		if (len > 0)
		{
			//check if read the 3 $ 
			BufferValue=BufferHandler(buf);
			//if receive $$$ the communication between computer and bluetooth has been established 
			if (BufferValue == 1)
			{
				bciout<<"Communication initialized "<<mcount<<endl;
			    mcount =0;
				return 1;
			}
			
		}
	}
}

//this function parse the data in the buffer 
void OpenBCIADC::ByteParser()
{
	int NumChannel = 0;
	int ByteCount = 0;
	int TempVal = 0;
	float outputtemp = 0;

	//process the data in the buffer and convert them to int
	for (int block = 0; block < SAMPLEBLOCKSIZE; block++)
	{
		NumChannel =0;
		//if the first elelment in every first block does not equal to 0xA0
		//this block of data is invalid 
		if (mParseBuffer[BUFFERSIZE*block]!=0xA0)
		{
			bciout<<"invalid block"<<endl;
		}

		//store the index for current block
		mPacket.BlockNum[block] = int(mParseBuffer[1+BUFFERSIZE*block]);

		//store data in to mPacket
		for (int index = 2; index < 26; index++)
		{
			TempVal |= ((mParseBuffer[index+BUFFERSIZE*block])<<(EEG_CHANNELS_16 - (ByteCount*EEG_CHANNELS_8)));
			ByteCount++;

			//if read 3 byte for one int
			if (ByteCount == 3)
			{
				//if the int is negative 
				if((TempVal & 0x00800000) > 0)
				{
					TempVal|= 0xFF000000;
				}
				//if the int is positive  
				else 
				{
					TempVal &= 0x00FFFFFF;
				}

				//calculate one element for one channel
    			outputtemp = TempVal;

				//read it into output packet
				mPacket.OutPut[NumChannel][block] = TempVal;
				NumChannel++;

				//finish read all 8 channels
				ByteCount=0;
				TempVal=0;
			}
		}
	}	
}

//this fuction will be called again and again to streaming the data 
void OpenBCIADC::Streaming(HANDLE fd)
{
	unsigned char buf[BUFFERSIZE*SAMPLEBLOCKSIZE];
	int res=BUFFERSIZE*SAMPLEBLOCKSIZE;
	int all =BUFFERSIZE*SAMPLEBLOCKSIZE;
	int lastindex =0;

	//continuously query the device 
	while (1)
	{
		//read from the buffer 
		res = ReadSerial(fd, buf,res);

		//if there is data left in the buffer
		if (res>0)
		{
			//read enough byte into buffer 
			for (int index = 0; index < res; index++)
			{
				//read the data into a buffer
				mParseBuffer[index+lastindex] = buf[index];
			}
			lastindex+=res;
		}

		//calculate how much data left before 
		//parse the data
		res = all-res;
		all =res;

		//if read enough data 
		if (res==0)
		{
			//parse the data in the buffer 
			ByteParser();
			break;		
		}
	}
}

//this function si used to set gain for every channel
void OpenBCIADC::SetChannels(HANDLE fd )
{
	//indicate the command will change seetings for each channel
	char * buf = "v";
	DWORD len = 1;
	int numchannels =EEG_CHANNELS_8;

	//if not use 8 channels 
	if (!mUse8Channel)
	{
		numchannels =EEG_CHANNELS_16;
	}

	//parse every channel
	for(int ch = 0; ch < numchannels; ch++)
	{
		//go through channel settings
		for (int i = 0; i < CHANNEL_SETTINGS; i++)
		{
			//First bit indicate start channel setting
			if(i==0)
			{
				buf="x";
			}
	
			//second bit indicate which channel will be setted 
			if(i==1)
			{
				//sepecify which channel will be setted
				if (ch<EEG_CHANNELS_8)
				{
					string channel_index = to_string(ch+1);
					buf = new char[channel_index.length() + 1];
					strcpy(buf,channel_index.c_str());
				}
				//if channel is bigger than 8 use alphabet
				else
				{
					if (ch==8)
						buf = "Q";
					if (ch==9)
						buf = "W";
					if (ch==10)
						buf = "E";
					if (ch==11)
						buf = "R";
					if (ch==12)
						buf = "T";
					if (ch==13)
						buf = "Y";
					if (ch==14)
						buf = "U";
					if (ch==15)
						buf = "I";
				}
			}

			//third bit if turn the channel on 
			if(i==2)
			{
				buf="0";
			}

			//set channel gain
			if(i==3)
			{
				int gain_factor=6;
				if(mChannel_Gain[ch]==GAIN_1)
					gain_factor=0;
				if(mChannel_Gain[ch]==GAIN_2)
					gain_factor=1;
				if(mChannel_Gain[ch]==GAIN_4)
					gain_factor=2;
				if(mChannel_Gain[ch]==GAIN_6)
					gain_factor=3;
				if(mChannel_Gain[ch]==GAIN_8)
					gain_factor=4;
				if(mChannel_Gain[ch]==GAIN_12)
					gain_factor=5;
				if(mChannel_Gain[ch]==DEFAULT_CHANNEL_GAIN)
					gain_factor=6;
				string channel_gain = to_string(gain_factor);
				buf = new char[channel_gain.length() + 1];
				strcpy(buf, channel_gain.c_str());
			}

			//set bias and impedance for each channel 
			if(i==4)
				buf="0"; //input type
			if(i==5)
				buf="1"; //bias set
			if(i==6)
				buf="1"; //srb2 set
			if(i==7)
				buf="0"; //srb1 set
			if(i==8)
				buf="X"; //finish indicator
			bool wri = WriteFile(fd,(LPSTR)buf,len,&len,NULL);

			//board reaction time 
			Sleep(100);
		}
		bciout<<"channel "<<ch+1<<" out of "<<numchannels<<" has been done"<<endl;
	}
}

//read what's inside the buffer
void OpenBCIADC::ReadBuffer(HANDLE fd, int numbytes)
{
	unsigned char buf[READ_BUFFER_SIZE];
	DWORD size =numbytes;
	ReadFile(fd, (LPSTR)buf, size, &size, NULL);
}

//send command to quey the board 
void OpenBCIADC::ReadChannelSetting(HANDLE fd)
{
	char* buf1 = "?";
	DWORD len = 1;

	//write one bit into the board 
	bool wri = WriteFile(fd,(LPSTR)buf1,len,&len,NULL);

	//board reacting time 
	Sleep(100);

	//if successfuly write 
	if(wri)
	{
		bciout<<"Query the board"<<endl;
	}
}

//send command to reset the board
void OpenBCIADC::ResetBoard(HANDLE fd)
{
	char * buf = "v";
	DWORD len = 1;

	//write one bit into the board 
	bool wri =  WriteFile(fd,(LPSTR)buf,len,&len,NULL);

	//board reacting time 
	Sleep(100);

	//if successfullt write 
	if(wri)
	{
		bciout<<"Reset the board"<<endl;
	}
	return;
}

//set the board to 8 or 16 channel mode 
void OpenBCIADC::SetNumChannel(HANDLE fd)
{
	//if use 8 channel mode 
	if (mUse8Channel)
	{
		//send command to board to specify using 8 channel mode 
		char *buf = "c";
		DWORD len = 1;
		
		//write one bit into the board 
		bool wri =  WriteFile(fd,(LPSTR)buf,len,&len,NULL);
		
		//reactig time 
		Sleep(100);
		
		//if successfully write 
		if(wri)
		{
			bciout<<"Use 8 channel mode"<<endl;
		}
	}
	//if not use 8 channel mode 
	else if (!mUse8Channel)
	{
		//send command to board to specify using 16 channel mode 
		char *buf = "C";
		DWORD len = 1;

		//write one bit into the board 
		bool wri =  WriteFile(fd,(LPSTR)buf,len,&len,NULL);

		//reacting time 
		Sleep(100);

		//if successfully write 
		if(wri)
		{
			bciout<<"Use 16 channel mode"<<endl;
		}
	}
}

//reser the board to default settings
void OpenBCIADC::SetDefault(HANDLE fd)
{
	char *buf = "d";
	DWORD len = 1;

	//write one bit into the board 
	bool wri =  WriteFile(fd,(LPSTR)buf,len,&len,NULL);

	//reacting time 
	Sleep(100);

	//if successfully write 
	if(wri)
	{
		bciout<<"Initialize the board as default"<<endl;
	}
}

//auto configure the port 
void OpenBCIADC::AutoConfig( const SignalProperties& ) 
{
	//check if the port is already open
	if(!mOpenPort)
	{
		//check from com port 1 to com port 10 
		for (int p = 1; p < Num_Com_Port; p++)
		{		
	        string PortName =  "\\\\.\\COM" + to_string(p);
			char *CopyPortName = new char[PortName.size() + 1];
            std::strcpy(CopyPortName , PortName.c_str());
            //open the port and get the handle
            mfd= OpenPort(CopyPortName);
			//if it's a invalid handle 
            if (mfd == INVALID_HANDLE_VALUE)
            {
				//if it's a invalid handle try naother port
	            bciout<<"port COM"<<p<<" no connection"<<endl;
				   continue;
            }
			
			//if it's a valid handle flush the buffer and connect
            if(FlushFileBuffers(mfd)) 
			{
				bciout<<"successfully refresh buffer"<<endl;
			}
			
			//check the hand shake 
            bool SuccessOpenPort = PreStreaming(mfd);
            delete[] CopyPortName; 
			
			//if there is no hand shake after 3s 
            if (!SuccessOpenPort)
            {
				//turn off the port and hand shake
	            bciout<<"port COM"<<p<<" no handshake"<<endl;
				continue;
            }
            mOpenPort=true;
			break;
         }
	}
	
	//after try to connect to all port if there is no connection 
	//warning there is no device connecting to computer
	if (mOpenPort == false)
	{
		bcierr<<"no device"<<endl;
	}

	//if successfully connect to device
	//set all the parameter 
	//check if ShowLostPackages Parameter is correct
	if (Parameter("ShowLostPackages")!=0&&Parameter("ShowLostPackages")!=1)
	{
		bcierr<<"ShowLostPackages flag can only be 0 or 1(0 turn it off 1 turn it on)"<<endl;
		return;
	} 
	
	// read the user input
	ShowLostPages = Parameter("ShowLostPackages");

	//if the use specify the num of channels 
	if (Parameter("SourceCh")!="auto")
	{
		bcierr<<"this value will be automatically setted"<<endl;
	}
	
	// if the channels list is auto 
	if (Parameter("SourceChList") == "auto")
	{
		//the default value is the first 8 channels
		Parameter("SourceChList")->SetNumValues(EEG_CHANNELS_8);
		for (int ch = 0; ch < EEG_CHANNELS_8; ch++)
		{
			Parameter("SourceChList")(ch) = ch+1;
		}
	}
	mNumChannels = Parameter("SourceChList")->NumValues();
	mUse8Channel = true;
	//store channel index into member variable 
	for (int ch = 0; ch < mNumChannels; ch++)
	{
		mSourceChList.push_back(Parameter("SourceChList")(ch)-1);

		//if the undex of the channel is bigger than 8 store the channel 
		if (Parameter("SourceChList")(ch)>EEG_CHANNELS_8)
		{
			mUse8Channel = false;
		}
	}

	//read number of channels into parameter 
	if (ShowLostPages)
	{
		Parameter("SourceCh") = mNumChannels +1;
	}

	//if not show the lost channels 
	else
	{
		Parameter("SourceCh") = mNumChannels;
	}

	//if the user set the sampling rate 
    if (Parameter("SamplingRate")!="auto")
    {

	    bcierr<<"Sampling rate will be automatically setted"<<endl;
    }

	//set the default sampling rate to 250 
    Parameter("SamplingRate") = SAMPLING_RATE;

	//if the user set the sample block size 
    if (Parameter("SampleBlockSize")!="auto")
    {
	    bcierr<<"Sample block size will be automatically setted"<<endl;
    }

	//set the sample block size to 125
    Parameter("SampleBlockSize") = SAMPLEBLOCKSIZE;

	//if the user set the source channel gain 
    if (Parameter("SourceChGain")!= "auto")
    {
	    bcierr<<"source channel gain will be automatically calculated. This value can be change by changing ChannelsGain"<<endl;
    }
    mSampleBlockSize =SAMPLEBLOCKSIZE;

	//if the user set channels' name and doesn't set all channels 
    if (Parameter("ChannelNames")!= "auto"&&Parameter("ChannelNames")->NumValues()!=mNumChannels)
    {
	    bcierr<<"number of channel names must match number of source channels ("<<mNumChannels<<")"<<endl;
    }

	//if user doesn't set the channels' name 
    if (Parameter("ChannelNames")== "auto")
    {
		//set channel name 
	    Parameter("ChannelNames")->SetNumValues(mNumChannels+1);
	    for (int ch = 0; ch < mNumChannels; ch++)
	    {
		    Parameter("ChannelNames")(ch)="ch "+to_string(int(Parameter("SourceChList")(ch)));
	    }
	    Parameter("ChannelNames")(mNumChannels) = "LostSampleindicator";
    }

	//if user doesn't set the channels' gain 
	if (Parameter("ChannelsGain")!="auto"&&Parameter("ChannelsGain")->NumValues()!=mNumChannels)
	{
		bcierr<<"Please specify gain for all channels"<<endl;
		return;
	}

	//if the user didn't specify the channel gains 
	else if (Parameter("ChannelsGain")=="auto")
	{
		Parameter("ChannelsGain")->SetNumValues(mNumChannels +1);

		//set the channel gain as default 
		for (int ch = 0; ch < mNumChannels; ch++)
		{
			Parameter("ChannelsGain")(ch) =DEFAULT_CHANNEL_GAIN;
		}
	}

	//set channel gain
	Parameter("ChannelsGain")(mNumChannels)=1;
	int possiblegains[] = {GAIN_1,GAIN_2,GAIN_4,GAIN_6,GAIN_8,GAIN_12,DEFAULT_CHANNEL_GAIN};
	bool impossiblegains =false;

	////go through gains and chekc if the gain is possible 
	for (int ch = 0; ch < mNumChannels; ch++)
	{
		impossiblegains = false;
		//go through the possible gains 
		for (int i = 0; i < SIZE_OF_POSSIBLE_GAINS; i++)
		{
			//check if all gain is reasonable 
			if (Parameter("ChannelsGain")(ch)==possiblegains[i])
			{
				impossiblegains = true;
				break;
			}
		}

		//if there is impossible gans
		if(impossiblegains == false){
			bcierr<<"the gain for channel "<<mSourceChList[ch]+1<<" is out of range the possible value for gain is 1,2,3,6,8,12,24"<<endl;
			return;
		}
	}

	//set source channels' gain 
    Parameter( "SourceChGain" )->SetNumValues(mNumChannels+1 );

	//set default channel gains
	for (int ch = 0; ch < EEG_CHANNELS_16; ch++)
	{
		mChannel_Gain[ch] = DEFAULT_CHANNEL_GAIN;
	}

	//read channel gain from the user 
	for (int ch = 0; ch < mNumChannels; ch++)
	{
		mChannel_Gain[mSourceChList[ch]] = Parameter("ChannelsGain")(ch);
	}

	//convert the channel gains 
	for (int ch = 0; ch < mNumChannels; ch++)
	{
		double gainFactor = CONVERSION_FACTOR*(24.0/mChannel_Gain[mSourceChList[ch]]);
        Parameter( "SourceChGain" )(ch)<<gainFactor<<"V";
	}
	Parameter( "SourceChGain" )(mNumChannels)<<1.0<<"V";
}

//set up the output properties 
void OpenBCIADC::Preflight( const SignalProperties&,
                                       SignalProperties& outSignalProperties ) const
{
	//set up the output properties 
	SignalType sigType = SignalType::int32;
    int samplesPerBlock = Parameter("SampleBlockSize");
    int numberOfSignalChannels = Parameter("SourceCh");
    outSignalProperties = SignalProperties( numberOfSignalChannels , samplesPerBlock, sigType );  
	bciout<<"num of channels "<<Parameter("SourceCh")<<endl;
	bciout<<"sample block size  "<<Parameter("SampleBlockSize")<<endl;
	bciout<<"sampling rate "<<Parameter("SamplingRate")<<endl;

}


//open the port and start streaming 
void OpenBCIADC::Initialize( const SignalProperties&, const SignalProperties& )
{
    //request the streaming
	mNumlostsample =0;
	mPreSampleNum =-1;
	mfirst_time = false;

	//if the first time set the configuration 
    if (!msetted_config)
    {
		//reset the board 
	    ResetBoard(mfd);
		SetDefault(mfd);

		//stop streaming
		StopStreaming(mfd);
		Sleep(100);

		//flush the buffer 
		if(FlushFileBuffers(mfd))
		{
			bciout<<"Flush buffer"<<endl;
		}

		//set number of channels 
		SetNumChannel(mfd);
		SetChannels(mfd);

		//read channel setting
		ReadChannelSetting(mfd);
		ReadBuffer(mfd,3000);
		PreStreaming(mfd);

		//start streaming 
		StartStreaming(mfd);
		msetted_config =true;
	}

	//if not the first time streaing 
	else
	{
		//stop streaming 
		StopStreaming(mfd);
		Sleep(100);

		//flush buffer 
		if(FlushFileBuffers(mfd))
		{
			bciout<<"Flush buffer"<<endl;
		}

		//reacting time 
		Sleep(100);

		//set the gain for the channels 
	    SetChannels(mfd);
		ReadChannelSetting(mfd);
		ReadBuffer(mfd,300);

		//check hand shake
		PreStreaming(mfd);
		//satrt streaming

		StartStreaming(mfd);
	}

	//set the contaner for previous data to zero
	for (int i = 0; i < EEG_CHANNELS_8; i++)
	{
	    mPre_8ch[i] = 0;
		mLast_8ch[i] = 0;
	}
}

//this fuction will be called over and over again
void OpenBCIADC::Process( const GenericSignal&, GenericSignal& Output )
{
	
    //streaming the data 
	Streaming(mfd);

	//if use the 8 channel mode 
	if (mUse8Channel)
	{
		//go through every channel
		for (int ch = 0; ch < mNumChannels; ch++)
		{
			//go through every blocks 
			for (int sample = 0; sample < mSampleBlockSize; sample++)
			{
				//just copy the data from mpacket 
				Output( ch, sample ) =mPacket.OutPut[mSourceChList[ch]][sample];
			}
		}
	}

	//if use 16 channel mode 
	else if(!mUse8Channel)
	{
		//check if there is package losing 
	    mPreSampleNum = mPacket.BlockNum[0]-1;

		//go through every block 
		for( int sample = 0; sample < mSampleBlockSize; sample++ )
	    {
			//set the indicator to 0 if there is no sample lost 
			Output( mNumChannels, sample ) =0;

			//check if the block index is continuous 
			if (mPacket.BlockNum[sample]!=mPreSampleNum+1)
			{
				//if there is data losing turn the indicator to one 
				mNumlostsample++;
				Output( mNumChannels, sample ) =1;
				//print out the position where the data lost 
				bciout<<"pre "<<mPreSampleNum<<" curr "<<mPacket.BlockNum[sample]<<endl;
			}

			//get the initial sample index 
			mPreSampleNum = mPacket.BlockNum[sample];

			//if reach the block size 
			if (mPacket.BlockNum[sample] ==DATA_BLOCK_SIZE)
			{
				//after go through one package 
				//change the counter to one  
				mPreSampleNum =-1;
			}

			//finish checking 255 samples 
			if (sample == mSampleBlockSize-1)
			{
				//print out number of lost samples in every package 
				bciout<<"lost sample "<<mNumlostsample<<endl;
				mNumlostsample =0;
			}

			//if the index for this sample is even 
			//then this incoming data is for the last 8 channels 
			if (mPacket.BlockNum[sample] %2 ==0)
		    {
				//go through evey channel
				for (int ch = 0; ch < mNumChannels; ch++)
				{

					//for the last 8 channels
					if (mSourceChList[ch]>=EEG_CHANNELS_8)
					{
						//since the sampling rate is half 
						//do liner interpolation 
						Output( ch, sample ) = (mPacket.OutPut[mSourceChList[ch]-8][sample] + mLast_8ch[mSourceChList[ch]-EEG_CHANNELS_8] )/2;
						//copy current data  
						mLast_8ch[mSourceChList[ch]-EEG_CHANNELS_8] = mPacket.OutPut[mSourceChList[ch]-EEG_CHANNELS_8][sample];
					}

					//for the first 98 channels
					else
					{

						//if there is a data losing
						if (Output( mNumChannels, sample ) == 1)
						{
							//do liner interpolation 
							mPre_8ch[mSourceChList[ch]] += (Output( ch, sample -1 ) - Output( ch, sample - 2 ));
						}
						//if there is no data lost 
						//copy the previous data 
						Output( ch, sample ) = mPre_8ch[mSourceChList[ch]];
					}
				 }
			 }

			 //if the index for the current sample is odd 
			 else if (mPacket.BlockNum[sample] %2 ==1)
			 {

				 //since the sampling rate is half
				 for (int ch = 0; ch < mNumChannels; ch++)
				 {

					 //for the first 8 channel
					 if (mSourceChList[ch]<EEG_CHANNELS_8)
					 {
						 //do linear interpolation 
						 Output( ch, sample ) = (mPacket.OutPut[mSourceChList[ch]][sample]+mPre_8ch[mSourceChList[ch]])/2;
						 //copy the data back into container
						 mPre_8ch[mSourceChList[ch]] =  mPacket.OutPut[mSourceChList[ch]][sample];
					 }

					 //for the last 8 channels
					 else
					 {

						 //check if there is a data losing 
						 if (Output( mNumChannels, sample ) == 1)
						 {
							 mLast_8ch[mSourceChList[ch]-EEG_CHANNELS_8] += (Output( ch, sample -1 ) - Output( ch, sample - 2 ));
						 }
						 //if no data losing copy the previous data 
						 Output( ch, sample ) = mLast_8ch[mSourceChList[ch]-EEG_CHANNELS_8];
					 }
				 }
			 } 
		 }
    }
}

//empty function
void OpenBCIADC::Halt()
{}