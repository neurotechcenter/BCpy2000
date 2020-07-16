#include <iostream>

#include "eeprom.h"

// TO-DO: Implement unfinished functions (check SiLabs' offical C# demo application for reference -- check file EEPROM.cs for function implementations)


BYTE * EEPROM::EepromWrBuf = new BYTE[EEPROM::EEPROM_MAX_BYTES + EEPROM::DATA_OFFSET];
BYTE * EEPROM::EepromRdBuf = new BYTE[EEPROM::EEPROM_MAX_BYTES + EEPROM::DATA_OFFSET];
BYTE EEPROM::reg_status = 0x00;

EEPROM::EEPROM()
{
}


EEPROM::~EEPROM()
{
	// delete[]EepromWrBuf;
	// delete[]EepromRdBuf;
}

/*
Description:
	Initialize the CP2130 device and set the SPI channel for EEPROM

Parameters:
	[in] hDevice: USB Interface Handle

Returns:
	A boolean value indicating function success/failure (true is success)
*/
bool EEPROM::EEPROM_Setup(CP213x_DEVICE hDevice) {
	bool bResult;
	bResult = EEPROM_Interface_Init(hDevice, EEPROM_NSS_INDEX, 0x0A);
	if (bResult)
	{
		// Only GPIO2's SPI CS is enabled
		if ((CP213x_SetChipSelect(hDevice, 2, CSMODE_ACTIVE_OTHERS_IDLE) != USB_SPI_ERRCODE_SUCCESS))
		{
			bResult = false;
		}
	}
	else
	{
		bResult = false;
	}
	return bResult;
}

/*
Description:
	Reads a number of bytes from EEPROM

Parameters:
	[in] hDevice: USB Interface Handle
	[in] address: The read starting point on EEPROM
	[in] num: number of bytes to read
	[out] readBuf: Result read from EEPROM

Returns:
	A boolean value indicating function success/failure (true is success)
*/
bool EEPROM::EEPROM_Read(CP213x_DEVICE hDevice, BYTE address, uint16_t num, BYTE* readBuf) {
	bool bResult = true;
	DWORD bytesTransferred = 0;
	for (int i = 0; i < EEPROM_MAX_BYTES + DATA_OFFSET; ++i) {
		EepromWrBuf[i] = 0xFF;
	}

	EepromWrBuf[0] = M25AA_READ;
	EepromWrBuf[1] = address;

	if (CP213x_TransferWriteRead(hDevice, EepromWrBuf, readBuf, num + 2, RELEASE_AFTER_TRANSFER == 1, 1000, &bytesTransferred) != USB_SPI_ERRCODE_SUCCESS) {
		bResult = false;
	}
	else {
		bResult = true;
	}
	return bResult;
}

/*
Description:
	Write multiple blocks of data into EEPROM. (block size is 16 bytes)
	The head block and tail block can be parital. (partially filled)

Parameters:
	[in] hDevice: USB Interface Handle
	[in] address: Write starting point
	[in] writeBuf: Data to write
	[in] totalNum: Number of bytes to write

Returns:
	A boolean value indicating function success/failure (true is success)
*/
bool EEPROM::EEPROM_Multiple_Write(CP213x_DEVICE hDevice, BYTE address, BYTE* writeBuf, uint16_t totalNum) {
	/*Implement function here*/
	return true;
}

/*
Description:
	Read EEPROM status

Parameters:
	[in] hDevice: USB Interface Handle
	
Returns:
A BYTE value indicating EEPROM's status
*/
BYTE EEPROM::EEPROM_Read_Status(CP213x_DEVICE hDevice) {
	DWORD bytesTransferred = 0;
	EepromWrBuf[0] = M25AA_RDSR;
	EepromWrBuf[1] = 0xFF;

	CP213x_TransferWriteRead(hDevice, EepromWrBuf, EepromRdBuf, 2, RELEASE_AFTER_TRANSFER == 1, 500, &bytesTransferred);
	return EepromRdBuf[1];
}

/*
Description:
	Initialize the CP2130 Device for EEPROM

Parameters:
	[in] hDevice: USB Interface Handle
	[in] csIndex: The SPI channel number
	[in] spiMode: The SPI control word for SPI configuration

Returns:
	A boolean value indicating function success/failure (true is success)
*/
bool EEPROM::EEPROM_Interface_Init(CP213x_DEVICE hDevice, BYTE csIndex, BYTE spiMode) {
	// initialize CP2130 spi
	bool bResult;
	if (CP213x_SetSpiControlByte(hDevice, csIndex, spiMode) != USB_SPI_ERRCODE_SUCCESS) {
		bResult = false;
	}
	else {
		bResult = true;
	}
	return bResult;
}

/*
Description:
	Write data to a page on EEPROM (a page is 16 bytes)

Parameters:
[in] hDevice: USB Interface Handle
[in] address: Address to write data to
[in] writeBuf: Data buffer
[in] wrBufOffset: The buffer offset. (starting point for data of interest)
[in] num: Number of bytes to write. 

Returns:
A boolean value indicating function success/failure (true is success)
*/
bool EEPROM::EEPROM_Page_Write(CP213x_DEVICE hDevice, BYTE address, BYTE* writeBuf, BYTE wrBufOffset, uint16_t num) {
	bool bResult = true;
	DWORD bytesTransferred = 0;

	// data exceeds the page limit
	if (((address & 0x0F) + num) > PAGE_SIZE) {
		bResult = false;
	}
	else {
		EEPROM_Write_Control(hDevice, 1);
		reg_status = EEPROM_Read_Status(hDevice);
		while ((reg_status & WEL_MASK) == 0) {
			EEPROM_Write_Control(hDevice, 1);
			reg_status = EEPROM_Read_Status(hDevice);
		}

		EepromWrBuf[0] = M25AA_WRITE;
		EepromWrBuf[1] = address;

		for (int i = 0; i < num; i += 1) {
			EepromWrBuf[i + 2] = writeBuf[i + wrBufOffset];
		}

		/*
		Debug purpose
		std::cout << "wrote: " << std::endl;
		for (int i = 0; i < num+2; i += 1) {
			std::cout << EepromWrBuf[i] << std::endl;
		}	
		*/
	

		if (CP213x_TransferWrite(hDevice, EepromWrBuf, num + 2, RELEASE_AFTER_TRANSFER == 1, 500, &bytesTransferred) != USB_SPI_ERRCODE_SUCCESS) {
			bResult = false;
		}
		std::cout << "Transferred " << bytesTransferred << " bytes" << std::endl;
	}
	return bResult;
}

/*
Description:
	Enable the write access to EEPROM

Parameters:
	[in] hDevice: USB Interface Handle
	[in] writeEn: The control flag for enabling write operation

Returns:
	A boolean value indicating function success/failure (true is success)
*/
bool EEPROM::EEPROM_Write_Control(CP213x_DEVICE hDevice, BYTE writeEn) {
	bool bResult = true;
	DWORD bytesTransferred = 0;
	
	if ((writeEn != 0)) {
		EepromWrBuf[0] = M25AA_WREN; // Set the write enable latch (enable write operations)
	}
	else {
		EepromWrBuf[0] = M25AA_WRDI; // Reset the write enable latch (disable write operations)
	}
	if (CP213x_TransferWrite(hDevice, EepromWrBuf, 1, RELEASE_AFTER_TRANSFER == 1, 500, &bytesTransferred) != USB_SPI_ERRCODE_SUCCESS) {
		bResult = false;
	}
	return bResult;
}

/*
Description:
	Entry point function of EEPROM operation
	Writes data to EEPROM

Parameters:
	[in] hDevice: USB Interface Handle
	[in] writeBuffer: Data to write

Returns:
	A boolean value indicating function success/failure (true is success)
*/
bool EEPROM::writeFromTransferBoxToEEPROM(CP213x_DEVICE hDevice, BYTE* writeBuffer) {
	/*Implement function here*/
	return true;
}

