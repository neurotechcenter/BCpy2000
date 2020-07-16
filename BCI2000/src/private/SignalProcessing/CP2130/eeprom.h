#ifndef EEPROM_H
#define EEPROM_H

#pragma once
#include <iostream>
#include <windows.h>

#include "./extlib/include/SLAB_USB_SPI.h"

class EEPROM
{
	public:
		EEPROM();
		~EEPROM();
		typedef unsigned short uint16_t;

		static bool EEPROM_Setup(CP213x_DEVICE hDevice);
		static bool EEPROM_Read(CP213x_DEVICE hDevice, BYTE address, uint16_t num, BYTE* readBuf);
		static bool EEPROM_Multiple_Write(CP213x_DEVICE hDevice, BYTE address, BYTE* writeBuf, uint16_t totalNum);
		static BYTE EEPROM_Read_Status(CP213x_DEVICE hDevice);
		static bool EEPROM_Interface_Init(CP213x_DEVICE hDevice, BYTE csIndex, BYTE spiMode);
		static bool EEPROM_Page_Write(CP213x_DEVICE hDevice, BYTE address, BYTE* writeBuf, BYTE wrBufOffset, uint16_t num);
		static bool EEPROM_Write_Control(CP213x_DEVICE hDevice, BYTE writeEn);
		static bool writeFromTransferBoxToEEPROM(CP213x_DEVICE hDevice, BYTE* writeBuffer);

		// constants of EEPROM
		const static int PAGE_SIZE = 16;
		const static int EEPROM_TOTAL_SIZE = 256;
		const static int EEPROM_USER_SIZE = 250;
		const static int DATA_OFFSET = 2; // the first two are used for commands

		// GPIO assignments for EEPROM 25AA02E48 hardware interface
		const static BYTE EEPROM_NSS_MASK = 0x20;
		const static int EEPROM_NSS_INDEX = 2;
		const static BYTE EEPROM_NSS_DEASSERT = 0x20;
		const static BYTE EEPROM_NSS_ASSERT = 0x00;

		// Microchip 25AA02E48 Instruction Set
		const static BYTE M25AA_READ = 0x03;
		const static BYTE M25AA_WRITE = 0x02;
		const static BYTE M25AA_WRDI = 0x04;
		const static BYTE M25AA_WREN = 0x06;
		const static BYTE M25AA_RDSR = 0x05;
		const static BYTE M25AA_WRSR = 0x01;

		// Status Register
		const static BYTE WIP_MASK = 0x01;
		const static BYTE WEL_MASK = 0x02;
		const static BYTE BP_MASK = 0x0C;

		static BYTE reg_status;
		static BYTE * EepromWrBuf;
		static BYTE * EepromRdBuf;

		/////////////////////////////////////////////////////////////////////////////
		// Definitions
		/////////////////////////////////////////////////////////////////////////////

		static const int USB_BUFSIZE = 1048576;
		static const int MAX_POLL_INTERVAL_MS = 10000;
		static const int MAX_TIMEOUT_MS = 300000;

		static const int DISPLAY_POLL_UPDATES = 1;
		static const int INCREASE_THREAD_PRIORITY = 0;

		static const int READ_TIMER_ID = 1;
		static const int READ_TIMER_ELAPSE = 50;

		static const BYTE PARTNUM_CP2130 = 0x1E;

		/////////////////////////////////////////////////////////////////////////////
		// Pin Definitions (Provided in "SLAB_USB_SPI.h")
		/////////////////////////////////////////////////////////////////////////////

		/*
		static const int CP213x_NUM_GPIO = 11;

		// Pin Config Mode Array Indices
		static const int CP213x_INDEX_GPIO_0 = 0;
		static const int CP213x_INDEX_GPIO_1 = 1;
		static const int CP213x_INDEX_GPIO_2 = 2;
		static const int CP213x_INDEX_GPIO_3 = 3;
		static const int CP213x_INDEX_GPIO_4 = 4;
		static const int CP213x_INDEX_GPIO_5 = 5;
		static const int CP213x_INDEX_GPIO_6 = 6;
		static const int CP213x_INDEX_GPIO_7 = 7;
		static const int CP213x_INDEX_GPIO_8 = 8;
		static const int CP213x_INDEX_GPIO_9 = 9;
		static const int CP213x_INDEX_GPIO_10 = 10;

		// Pin Bitmasks
		static const uint16_t CP213x_MASK_GPIO_0 = 0x0001;
		static const uint16_t CP213x_MASK_GPIO_1 = 0x0002;
		static const uint16_t CP213x_MASK_GPIO_2 = 0x0004;
		static const uint16_t CP213x_MASK_GPIO_3 = 0x0008;
		static const uint16_t CP213x_MASK_GPIO_4 = 0x0010;
		static const uint16_t CP213x_MASK_GPIO_5 = 0x0020;
		static const uint16_t CP213x_MASK_GPIO_6 = 0x0040;
		static const uint16_t CP213x_MASK_GPIO_7 = 0x0080;
		static const uint16_t CP213x_MASK_GPIO_8 = 0x0100;
		static const uint16_t CP213x_MASK_GPIO_9 = 0x0200;
		static const uint16_t CP213x_MASK_GPIO_10 = 0x0400;
		*/
		

		// From TestGUIDlg.h
		//========================================================================================================


		//========================================================================================================
		// From TestGUIDLg.cpp
		// GPIO number used for Loopback and generic transfer commands (Write/Read/WriteRead/RTRead)
		static const byte GPIO_FOR_TRANSFER_CMD = 0x01;

		// EEPROM
		static const int EEPROM_MAX_BYTES = 192;

		// These LCD defines are for convenience only, and are passed as DWORDs to CP240x_Test().
		static const int LCD_SLEEP_VALUE = 120;

		// This definition determines whether to release the SPI bus after a transaction is complete
		static const int RELEASE_AFTER_TRANSFER = 1;

};


#endif