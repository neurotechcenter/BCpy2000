#include "si8902.h"

// TO-DO: Implement unfinished functions (check SiLabs' offical C# demo application for reference -- check file si8902.cs for function implementations)

BYTE * SI8902::AdcWrBuf = new BYTE[SI8902::ADC_BUFFER_SIZE];
double SI8902::MCP9701A_MV_PER_DEGC = 19.5;

SI8902::SI8902()
{
}

SI8902::~SI8902()
{
}
/*
	Description:
	Parameter:
	Return:
*/
bool SI8902::Si8902_AquireAdcSamples(CP213x_DEVICE hDevice, unsigned short numSamples, BYTE channel, unsigned short * sampleBuf) {
	/*Implement function here*/
	return true;
}

/*
	Description:
		Read one ADC sample

	Parameter:
		[in] hDevice: USB Interface Handle
		[in] channel: The ADC channel to read from

	Return:
		The ADC result (an unsigned 16 bit integer)
*/
unsigned short SI8902::readAdcOneSample(CP213x_DEVICE hDevice, BYTE channel) {
	BYTE readBuffer[3];
	BYTE bitChannel;
	unsigned short sample;
	bitChannel = (BYTE)((int)channel << 4);
	Si8902_Reset(hDevice);
	// SPI Clock 1.5Mbps
	Si8902_Setup(hDevice, 0x3B);

	// Get one sample from AIN0

	Si8902_StartDemandRead(hDevice, channel, readBuffer);
	sample = (unsigned short)((((unsigned short)readBuffer[1] & 0x0F) << 6) | (unsigned short)((readBuffer[2] & 0x7E) >> 1));
	return sample;
}

/*
	Description:
		Reset CP2130 Device

	Parameter:
		[in] hDevice: USB Interface Handle

	Return:
		A boolean value indicating function success/failure (true is success)
*/
bool SI8902::Si8902_Reset(CP213x_DEVICE hDevice) {
	bool bResult = true;
	int errCode = 0;

	// P1.3 GPIO7 = 1;
	errCode = CP213x_SetGpioValues(hDevice, 1 << 7, 1 << 7);
	if (errCode != USB_SPI_ERRCODE_SUCCESS)bResult = false;
	// P1.3 GPIO7 = 0;
	errCode = CP213x_SetGpioValues(hDevice, 1 << 7, 0 << 7);
	if (errCode != USB_SPI_ERRCODE_SUCCESS)bResult = false;
	// P1.3 GPIO7 = 1;
	errCode = CP213x_SetGpioValues(hDevice, 1 << 7, 1 << 7);
	if (errCode != USB_SPI_ERRCODE_SUCCESS)bResult = false;

	return bResult;
}

/*	
	Description:
		Initialize the CP2130 device and set the SPI channel for Si8902

	Parameters:
		[in] hDevice: USB Interface Handle

	Returns:
		A boolean value indicating function success/failure (true is success)
*/	
bool SI8902::Si8902_Setup(CP213x_DEVICE hDevice, BYTE spiMode) {
	bool bResult = true;
	int errCode = 0;
	Si8902_InterfaceInit(hDevice, ADC_NSS_INDEX, spiMode); 
	Si8902_SpiDelayConfig(hDevice);
	
	errCode = CP213x_SetChipSelect(hDevice, ADC_NSS_INDEX, CSMODE_ACTIVE_OTHERS_IDLE);

	if (errCode != USB_SPI_ERRCODE_SUCCESS)bResult = false;
	return bResult;
}

/*
	Description:
	Parameter:
	Return:
*/
bool SI8902::Si8902_StartBurstRead(CP213x_DEVICE hDevice, BYTE channel, unsigned int numSamples, BYTE * readBuf) {
	/*Implement function here*/
	return true;
}

/*
Description:
	Set Si8902 to read mode and read a sample from it

Parameters:
	[in] hDevice: USB Interface Handle
	[in] channel: ADC channel to read from
	[out] readBuf: Return the data read from Si8902

Returns:
	A boolean value indicating function success/failure (true is success)
*/
bool SI8902::Si8902_StartDemandRead(CP213x_DEVICE hDevice, BYTE channel, BYTE * readBuf) {
	bool bResult = true;
	int errCode = 0;
	DWORD bytesTransferred = 0;
	AdcWrBuf[0] = (BYTE)(0xC0 | (BYTE)((int)channel << 4) | ADC_VREF_EXT | ADC_DEMAND_READ | ADC_GAIN_1);
	AdcWrBuf[1] = 0xFF;
	AdcWrBuf[2] = 0xFF;
	AdcWrBuf[3] = 0xFF;

	errCode = CP213x_TransferWrite(hDevice, AdcWrBuf, 1, RELEASE_AFTER_TRANSFER == 1, WRITE_MAX_WAIT_MS, &bytesTransferred);
	if (errCode != USB_SPI_ERRCODE_SUCCESS) {
		bResult = false;
	}
	errCode = CP213x_TransferReadSync(hDevice, readBuf, (1 * BYTES_PER_SAMPLE) + 1, RELEASE_AFTER_TRANSFER == 1, READ_MAX_WAIT_MS, &bytesTransferred);
	if (errCode != USB_SPI_ERRCODE_SUCCESS) {
		bResult = false;
	}
	return bResult;
}

/*
Description:
	Initialize the CP2130 Device for Si8902

Parameters:
	[in] hDevice: USB Interface Handle
	[in] csIndex: The SPI channel number
	[in] spiMode: The SPI control word for SPI configuration

Returns:
	A boolean value indicating function success/failure (true is success)
*/
bool SI8902::Si8902_InterfaceInit(CP213x_DEVICE hDevice, BYTE csIndex, BYTE spiMode) {
	bool bResult = true;
	int errCode = 0;
	errCode = CP213x_SetSpiControlByte(hDevice, csIndex, spiMode);
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
bool SI8902::Si8902_SpiDelayConfig(CP213x_DEVICE hDevice) {
	bool bResult = true;
	int errCode = 0;

	errCode = CP213x_SetSpiDelay(hDevice, ADC_NSS_INDEX, SPI_DELAY_INTERVAL_MASK, 0, 0, 0);
	if (errCode != USB_SPI_ERRCODE_SUCCESS) {
		bResult = false;
	}
	return bResult;
}

/*
	Description:
	Parameter:
	Return:
*/
int SI8902::Si8902_SetChipSelAndSpiControl(CP213x_DEVICE hDevice, BYTE gpioNumber, BYTE csMode, BYTE spiControlWord) {
	/*Implement function here*/
	return 0;
}

/*
	Description:
	Parameter:
	Return:
*/
int SI8902::Si8902_SetSpiDelays(CP213x_DEVICE hDevice, BYTE csNum, bool toggleCS,
	bool enablePostAssertDelay, unsigned int postAssertDelayValue,
	bool enableInterByteDelay, unsigned int interByteDelayValue,
	bool enablePreDeassertDelay, unsigned int preDeassertDelayValue) {
	/*Implement function here*/
	return 0;
}

/*
	Description:
	Parameter:
	Return:
*/
bool SI8902::verifyAdcCommunication(CP213x_DEVICE hDevice) {
	/*Implement function here*/
	return true;
}