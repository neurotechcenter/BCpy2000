////////////////////////////////////////////////////////////////////////////////
// $Id: tbsi.h 2018-09-13 abelsten $
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
#ifndef TBSI_H
#define TBSI_H

#include <iostream>
#include <string>
#include <map>
#include <Windows.h>
#include "./extlib/include/SLAB_USB_SPI.h"

class TBSI
{
public:
	TBSI(void);
	~TBSI(void);

	static void checkErrCode(int errCode);
	static bool tbsiSendCommand(CP213x_DEVICE hDevice, const std::string& input);
	static bool tbsiStartUp(CP213x_DEVICE hDevice, const std::string& pattern);

private:
	static bool TBSI_Setup(                   CP213x_DEVICE hDevice);
	static bool TBSI_InterfaceInit(           CP213x_DEVICE hDevice,  BYTE csIndex,    BYTE spiMode);
	static bool TBSI_ChipSelect(              CP213x_DEVICE hDevice,  BYTE csIndex,    BYTE csMode);
	static bool TBSI_SpiDelayConfig(          CP213x_DEVICE hDevice);
	static bool TBSI_PinConfig(               CP213x_DEVICE hDevice,  int  pinOffset,  BYTE config);
	static bool TBSI_TransferPattern(         CP213x_DEVICE hDevice, BYTE* writeBuf);
	static bool TBSI_ReceiveAck(              CP213x_DEVICE hDevice, BYTE* readBuf);
	static bool TBSI_SendPatternAndReceiveAck(CP213x_DEVICE hDevice, BYTE* writeBuf,   BYTE*readBuf);

	static void        encodeData(       const std::string& input  , BYTE* encoded);
	static std::string decodeData(                    BYTE* encoded,   int length);

	// Please look into Stimware's Setup.ini for detailed configuration
	const static BYTE  csIndex                 = 0; 
	const static BYTE  spiMode                 = 0x3B;
	const static BYTE  SPI_DELAY_INTERVAL_MASK = 1;
	const static DWORD TBSI_PACKET_SIZE        = 128; 
	const static DWORD STIMWARE_ACK_SIZE       = 32;
	const static DWORD BUFFER_SIZE             = 255;
	const static BYTE  RELEASE_AFTER_TRANSFER  = 1;
    const static int   WRITE_MAX_WAIT_MS       = 500;
	const static int   READ_MAX_WAIT_MS        = 500;

	static BYTE* tbsiWrBuf;
	static BYTE* tbsiRdBuf;
};


#endif