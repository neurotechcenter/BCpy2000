#ifndef SI8902_H
#define SI8902_H

#pragma once
#include <iostream>
#include <windows.h>

#include "./extlib/include/SLAB_USB_SPI.h"

class SI8902
{
public:

	static bool Si8902_AquireAdcSamples(CP213x_DEVICE hDevice, unsigned short numSamples, BYTE channel, unsigned short * sampleBuf);
	static unsigned short readAdcOneSample(CP213x_DEVICE hDevice, BYTE channel);
	static bool Si8902_Reset(CP213x_DEVICE hDevice);
	static bool Si8902_Setup(CP213x_DEVICE hDevice, BYTE spiMode);
	static bool Si8902_StartBurstRead(CP213x_DEVICE hDevice, BYTE channel, unsigned int numSamples, BYTE * readBuf);
	static bool Si8902_StartDemandRead(CP213x_DEVICE hDevice, BYTE channel, BYTE * readBuf);
	static bool Si8902_InterfaceInit(CP213x_DEVICE hDevice, BYTE csIndex, BYTE spiMode);
	static bool Si8902_SpiDelayConfig(CP213x_DEVICE hDevice);
	static int Si8902_SetChipSelAndSpiControl(CP213x_DEVICE hDevice, BYTE gpioNumber, BYTE csMode, BYTE spiControlWord);
	static int Si8902_SetSpiDelays(CP213x_DEVICE hDevice, BYTE csNum, bool toggleCS,
		bool enablePostAssertDelay, unsigned int postAssertDelayValue,
		bool enableInterByteDelay, unsigned int interByteDelayValue,
		bool enablePreDeassertDelay, unsigned int preDeassertDelayValue);
	static bool verifyAdcCommunication(CP213x_DEVICE hDevice);


	

	static const int BYTES_PER_SAMPLE = 2;
	static const int MAX_AIN_NUM = 3;
	static const int ADC_MAX_SAMPLES = 8192;
	static const int ADC_BUFFER_SIZE = ((2 * ADC_MAX_SAMPLES) + 1); // First byte returned by ADC is command byte

																	// GPIO assignments for Si8902 ADC hardware interface
	static const BYTE ADC_NSS_MASK = 0x08;
	static const int ADC_NSS_INDEX = 0;
	static const BYTE ADC_NSS_DEASSERT = 0x08;
	static const BYTE ADC_NSS_ASSERT = 0x00;

	// Defines for call to SPI_Delay_Set()
	static const BYTE SPI_DELAY_INTERVAL_MASK = 0x01;
	static const BYTE SPI_DELAY_CS_EN_MASK = 0x02;
	static const BYTE SPI_DELAY_CS_DIS_MASK = 0x04;
	static const BYTE SPI_DELAY_CS_TOGGLE_MASK = 0x08;

	// ADC Vref source
	static const BYTE ADC_VREF_VDD = 0x08;
	static const BYTE ADC_VREF_EXT = 0x00;

	// ADC Read Mode
	static const BYTE ADC_DEMAND_READ = 0x02;
	static const BYTE ADC_BURST_READ = 0x00;

	// ADC Gain
	static const BYTE ADC_GAIN_1 = 0x01;
	static const BYTE ADC_GAIN_0 = 0x00;

	// From ADCGraphDlg.cpp
	static const int ADC_CH0_TIMER_ID = 0;
	
	static const int ADC_CH0_TIMER_ELAPSE = 2000;

	static const int ADC_CH1_TIMER_ID = 1;
	static const int ADC_CH1_TIMER_ELAPSE = 5000;

	static const int ADC_CH2_TIMER_ID = 2;
	static const int ADC_CH2_TIMER_ELAPSE = 120;

	static const int NUM_SAMPLES = 4;
	static const int ADC_CHANNEL0 = 0;
	static const int ADC_CHANNEL1 = 1;
	static const int ADC_CHANNEL2 = 2;
	static const int GRAPH_X_AXIS_NUMPIXELS = 900;
	static const int GRAPH_Y_AXIS_NUMPIXELS = 100;

	//For Temp sensor
	static const int ADC_VREF_MV = 3420;
	static const int SI8902_NUMCOUNTS = 1024;
	static const int MCP9701A_MV_AT_0C = 400;
	static double MCP9701A_MV_PER_DEGC;

	// From Si8902.cpp
	static const int WRITE_MAX_WAIT_MS = 500;
	static const int READ_MAX_WAIT_MS = 1000;
	static const int WRITEREAD_MAX_WAIT_MS = 1000;
	static const int RELEASE_AFTER_TRANSFER = 1;
	static BYTE * AdcWrBuf;
	SI8902();
	~SI8902();
};

#endif