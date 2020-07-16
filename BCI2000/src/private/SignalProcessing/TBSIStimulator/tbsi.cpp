////////////////////////////////////////////////////////////////////////////////
// $Id: tbsi.cpp 2018-09-13 abelsten $
// Authors: belsten@neurotechcenter.org, yichuanwang@neurotechcenter.org
// Description: Class that represents the TBSI USB-SPI wireless stimulator and 
//				dongle.
// Known Issues: Connection to the dongle is not very reliable for the first 
//				few commands. TBSI API is currently under development.
//
// $BEGIN_BCI2000_LICENSE$
//
// This file is part of BCI2000, a platform for real-time bio-signal research.
// [ Copyright (C) 2000-2012: BCI2000 team and many external contributors ]
//
// BCI2000 is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software
// Foundation, either version 3 of the License, or (at your option) any later
// version.
//
// BCI2000 is distributed in the hope that it will be useful, but
//                         WITHOUT ANY WARRANTY
// - without even the implied warranty of MERCHANTABILITY or FITNESS FOR
// A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with
// this program.  If not, see <http://www.gnu.org/licenses/>.
//
// $END_BCI2000_LICENSE$
////////////////////////////////////////////////////////////////////////////////
#include "tbsi.h"

BYTE* TBSI::tbsiWrBuf = new BYTE[TBSI::BUFFER_SIZE];
BYTE* TBSI::tbsiRdBuf = new BYTE[TBSI::BUFFER_SIZE];

TBSI::TBSI()
{
}


TBSI::~TBSI()
{
	delete[] tbsiWrBuf;
	delete[] tbsiRdBuf;
}

/*	
	Description:
		Initialize the CP2130 device, set the SPI channel for Si8902
		and configure the device

	Parameters:
		[in] hDevice: USB Interface Handle

	Returns:
		A boolean value indicating function success/failure (true is success)
*/	
bool TBSI::TBSI_Setup(CP213x_DEVICE hDevice)
{
	bool bResult = true;
	int errCode = 0;

	// errCode = CP213x_Reset(hDevice);
	bResult &= TBSI_InterfaceInit(hDevice, csIndex, spiMode);				 // cdIndex = 0, spiMode = 0x3b
	bResult &= TBSI_ChipSelect(hDevice, csIndex, CSMODE_ACTIVE_OTHERS_IDLE); // csIndex = 0, spiMode = 2
	bResult &= TBSI_SpiDelayConfig(hDevice);								 // SPI_DELAY_INTERVAL_MASK = 1
	bResult &= TBSI_PinConfig(hDevice, 3, 0x04);							 // GPIO.3 pin configuration (0x04 is default)
	if(!bResult) std::cerr << "TBSI_Setup: setup failure" << std::endl;
	return bResult;
}

/*
	Description:
		Initialize the CP2130 Device for TBSI

	Parameters:
		[in] hDevice: USB Interface Handle
		[in] csIndex: The SPI channel number
		[in] spiMode: The SPI control word for SPI configuration

	Returns:
		A boolean value indicating function success/failure (true is success)
*/
bool TBSI::TBSI_InterfaceInit(CP213x_DEVICE hDevice, BYTE csIndex, BYTE spiMode)
{
	// initialize CP2130 spi
	bool bResult = true;
	int errCode = 0;

	errCode = CP213x_SetSpiControlByte(hDevice, csIndex, spiMode);
	if ( errCode != USB_SPI_ERRCODE_SUCCESS) {
		bResult = false;
	}
	return bResult;
}

/*	
	Description:
		Initialize the CP2130 device and select the SPI channel

	Parameters:
		[in] hDevice: USB Interface Handle
		[in] csIndex: The SPI channel number
		[in] spiMode: The SPI mode

	Returns:
		A boolean value indicating function success/failure (true is success)
*/
bool TBSI::TBSI_ChipSelect(CP213x_DEVICE hDevice, BYTE csIndex, BYTE csMode)
{
	bool bResult = true;
		
	int errCode = 0;
	errCode = CP213x_SetChipSelect(hDevice,  csIndex, csMode);
	if (errCode != USB_SPI_ERRCODE_SUCCESS) {
		bResult = false;
	}
	return bResult;
}

/*
	Description:
		SPI delay configuration

	Parameter:
		[in] hDevice: USB Interface Handle		

	Return:
		A boolean value indicating function success/failure (true is success)
*/
bool TBSI::TBSI_SpiDelayConfig(CP213x_DEVICE hDevice)
{
	bool bResult = true;
	int errCode = 0;

	errCode = CP213x_SetSpiDelay(hDevice, csIndex, SPI_DELAY_INTERVAL_MASK, 0, 0, 0);
	if (errCode != USB_SPI_ERRCODE_SUCCESS)
	{
		bResult = false;
	}
	return bResult;
}

/*
	Description:
		CP2130 pin configuration

	Parameter:
		[in] hDevice: USB Interface Handle		
		[in] pinOffset: The pin number
		[in] config: The configuration word

	Return:
		A boolean value indicating function success/failure (true is success)
*/
bool TBSI::TBSI_PinConfig(CP213x_DEVICE hDevice, int pinOffset, BYTE config)
{
	int errCode = 0;
	bool bResult = true;
	BYTE pinConfig[SIZE_PIN_CONFIG];
	errCode = CP213x_GetPinConfig(hDevice, pinConfig);

	pinConfig[pinOffset] = config;
	errCode = CP213x_SetPinConfig(hDevice, pinConfig);

	checkErrCode(errCode);
	if (errCode != 0) bResult = false;
	return bResult;
}

/*
	Description:
		Trnasfer a pattern packet. For specs on the pattern, please look at 
		TBSI documentation on info packets

	Parameter:
		[in] hDevice: USB Interface Handle		
		[in] writeBuf: Buffer that contains the pattern

	Return:
		A boolean value indicating function success/failure (true is success)
*/
bool TBSI::TBSI_TransferPattern(CP213x_DEVICE hDevice, BYTE * writeBuf)
{
	int errCode = 0;
	bool bResult = true;

	DWORD bytesTransferred = 0;
	for (int i = 0; i < TBSI_PACKET_SIZE; ++i)
	{
		tbsiWrBuf[i] = writeBuf[i];
	}

	WORD gpio;
	gpio = 0;
	CP213x_GetGpioValues(hDevice, &gpio);
	BYTE drainBuff[TBSI_PACKET_SIZE];

	// debug statement
	// std::cout << "wait for RTR to go to 0" << std::endl;
	for(; (gpio & 0x08) != 0; )  // wait for RTR to go to 0
	{
		CP213x_GetGpioValues(hDevice, &gpio);
		// drain the buffer first
		errCode = CP213x_TransferReadSync(hDevice, drainBuff, STIMWARE_ACK_SIZE, RELEASE_AFTER_TRANSFER == 1, READ_MAX_WAIT_MS, &bytesTransferred);
		checkErrCode(errCode);
	}

	errCode = CP213x_TransferWrite(hDevice, tbsiWrBuf, TBSI_PACKET_SIZE, RELEASE_AFTER_TRANSFER == 1, WRITE_MAX_WAIT_MS, &bytesTransferred);
	checkErrCode(errCode);

	//std::cout << "CP213x_TransferWrite -- bytes transferred: " << bytesTransferred << std::endl;
	// Sleep(200);

	if (errCode != USB_SPI_ERRCODE_SUCCESS) 
	{
		bResult = false;
	}
	return bResult;
}

/*
	Description:
		Receive ACK message from the TBSI dongle

	Parameter:
		[in] hDevice: USB Interface Handle		
		
		[out] readBuf: Buffer that contains the ACK information

	Return:
		A boolean value indicating function success/failure (true is success)
*/
bool TBSI::TBSI_ReceiveAck(CP213x_DEVICE hDevice, BYTE * readBuf)
{
	int errCode = 0;
	bool bResult = true;
	DWORD bytesTransferred = 0;

	WORD gpio;
	gpio = 0;
	CP213x_GetGpioValues(hDevice, &gpio);

	/*
		Important Note:
			The program will sometimes stall at this point during startup or the first few commands. Effort
			has been done to minimize this issue, but it's not completely solved. In case of a program stall,
			replug the dongle and rerun the program.
	*/
	// debug statement
	// std::cout << "wait for RTR to go to 1" << std::endl;
	int count = 0;
	for(; (gpio & 0x08) == 0; )  // wait for RTR to go to 1
	{
		CP213x_GetGpioValues(hDevice, &gpio);
		Sleep(10); // Delay between requests to avoid jamming the device
		count += 1;
		if (count > 200) 
		{
			std::cerr << "ERROR: Dongle stopped responding" << std::endl;
			bResult = false;
			return bResult;
		}
	}
	errCode = CP213x_TransferReadSync(hDevice, readBuf, STIMWARE_ACK_SIZE, RELEASE_AFTER_TRANSFER == 1, READ_MAX_WAIT_MS, &bytesTransferred);
	checkErrCode(errCode);
	//std::cout << "CP213x_TransferReadSync -- bytes transferred: " << bytesTransferred << std::endl;

	if (errCode != USB_SPI_ERRCODE_SUCCESS) 
	{
		bResult = false;
	}
	return bResult;
}

/*
	Description:
		Perform one communication cycle (send message & receive ack)

	Parameter:
		[in] hDevice: USB Interface Handle		
		[in] writeBuf: Buffer that contains the pattern

		[out] readBuf: Buffer that contains the ACK information

	Return:
		A boolean value indicating function success/failure (true is success)
*/
bool TBSI::TBSI_SendPatternAndReceiveAck(CP213x_DEVICE hDevice, BYTE * writeBuf, BYTE * readBuf)
{
	bool bResult = true;
	bResult &= TBSI::TBSI_Setup(hDevice);
	bResult &= TBSI_TransferPattern(hDevice, writeBuf);
	return bResult && TBSI_ReceiveAck(hDevice, readBuf);
}

/*
	Description:
		Print CP2130 specific error message

	Parameter:
		[in] errCode: CP2130 specific error code (look up in SLAB_USB_SPI.h)

	Return:
		none
*/
void TBSI::checkErrCode(int errCode)
{
	if (errCode != USB_SPI_ERRCODE_SUCCESS) 
	{
		if (errCode == USB_SPI_ERRCODE_HWIF_TRANSFER_TIMEOUT) 
		{
			std::cerr << "TBSI Error: Dongle time out.\n" <<
						"Try reconnecting dongle and rerunning program." << std::endl;
		}
		else if (errCode == USB_SPI_ERRCODE_PIPE_READ_FAIL) 
		{
			std::cerr << "TBSI Error: Read error" << std::endl;
		}
		else if (errCode == USB_SPI_ERRCODE_HWIF_DEVICE_ERROR) 
		{
			std::cerr << "TBSI Error: Error occurred while communicating with " <<
						 "the device or while retrieving device information.\n" <<
						 "Check if other programs are using the device and rerun." << std::endl;
		}
		else 
		{
			std::cerr << "TBSI Error: CP213x_ERR:" << std::hex << errCode << std::endl;
		}
	}
}

/*
	Description:
		Encode readable data string into buffer data

	Parameter:
		[in] input: Data string (Directly copied from Stimware verbose mode log)
		
		[out] encoded: Encoded data buffer

	Return:
		none
*/
void TBSI::encodeData(const std::string& input, BYTE * encoded)
{
	std::map<char, int> hexLookupTable; 
	hexLookupTable['0'] = 0;
	hexLookupTable['1'] = 1;
	hexLookupTable['2'] = 2;
	hexLookupTable['3'] = 3;
	hexLookupTable['4'] = 4;
	hexLookupTable['5'] = 5;
	hexLookupTable['6'] = 6;
	hexLookupTable['7'] = 7;
	hexLookupTable['8'] = 8;
	hexLookupTable['9'] = 9;
	hexLookupTable['A'] = 10;
	hexLookupTable['B'] = 11;
	hexLookupTable['C'] = 12;
	hexLookupTable['D'] = 13;
	hexLookupTable['E'] = 14;
	hexLookupTable['F'] = 15;

	for (int i = 1; i < input.size(); i+=2)
	{
		encoded[i/2] = BYTE(hexLookupTable[input[i-1]]*16 + hexLookupTable[input[i]]);
	}
}

/*
	Description:
		Decode data buffer into readable data string ( can be used to compare with
		Stimware verbose mode log)

	Parameter:
		[in] encoded: Encoded data buffer
		[in] length: Length of data in number of bytes

	Return:
		The string value of decoded data
*/
std::string TBSI::decodeData(BYTE * encoded, int length)
{
	std::string ans;
	std::map<int, char> hexLookupTable; 
	hexLookupTable[0] = '0';
	hexLookupTable[1] = '1';
	hexLookupTable[2] = '2';
	hexLookupTable[3] = '3';
	hexLookupTable[4] = '4';
	hexLookupTable[5] = '5';
	hexLookupTable[6] = '6';
	hexLookupTable[7] = '7';
	hexLookupTable[8] = '8';
	hexLookupTable[9] = '9';
	hexLookupTable[10] = 'A';
	hexLookupTable[11] = 'B';
	hexLookupTable[12] = 'C';
	hexLookupTable[13] = 'D';
	hexLookupTable[14] = 'E';
	hexLookupTable[15] = 'F';

	for (int i = 0; i < length; ++i)
	{
		ans += hexLookupTable[encoded[i]/16];
		ans += hexLookupTable[encoded[i]%16];
	}
	return ans;
}

/*
	Description:
		A wrapper function for a single communication cycle (print out the ACK message on console)

	Parameter:
		[in] hDevice: USB Interface Handle	
		[in] input: Data string (Directly copied from Stimware verbose mode log)

	Return:
		A boolean value indicating function success/failure (true is success)
*/
bool TBSI::tbsiSendCommand(CP213x_DEVICE hDevice, const std::string& input)
{
	bool bResult = true;
	BYTE pattern[TBSI::TBSI_PACKET_SIZE];
	TBSI::encodeData(input, pattern); 
	BYTE ack[TBSI::TBSI_PACKET_SIZE];
	bResult &= TBSI::TBSI_SendPatternAndReceiveAck(hDevice, pattern, ack);
	// std::cout << TBSI::decodeData(ack, 32) << std::endl;
	return bResult;
}

/*
	Description:
		TBSI dongle start up sequence. When the cycle is done, the dongle should
		ready to trigger Stim.
		
		The four critical commands (searchStim, scanAll, getDongleStatus and wakeUpStim)
		are all specific to the current dongle. To obatin the correct command for any other
		dongle, use Stimware to generate such commands and copy them into the function.

	Parameter:
		[in] hDevice: USB Interface Handle	
		[in] input: Pattern used for trigger stimulation

	Return:
		A boolean value indicating function success/failure (true is success)
*/
bool TBSI::tbsiStartUp(CP213x_DEVICE hDevice, const std::string& pattern)
{
	std::string searchStim = "AAAA230024000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000005555";
	std::string scanAll = "AAAA640288130100012400000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000005555";
	std::string getDongleStatus = "AAAA660000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000005555";
	std::string wakeUpStim = "AAAA010000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000005555";
	bool bResult = true;
	bResult &= TBSI::tbsiSendCommand(hDevice, getDongleStatus);
	bResult &= TBSI::tbsiSendCommand(hDevice, searchStim);
	bResult &= TBSI::tbsiSendCommand(hDevice, wakeUpStim);
	bResult &= TBSI::tbsiSendCommand(hDevice, pattern);
	return bResult;
}