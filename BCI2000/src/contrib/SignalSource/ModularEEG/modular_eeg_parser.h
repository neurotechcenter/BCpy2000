/*

   file: modular_eeg_parser.h
   Moular EEG Packet Parser (Header File) for BCI2000 framework
   written by Chris Veigl, support by Gerwin Schalk, Joerg Hansmann, Rudi Cilibrasi

   supported packet formats: P2, P3
   Open EEG Hardware: Modular EEG v1.0  (6 Channels, 10 bit, 256Hz)
     for docomentation please refer to http://openeeg.sf.net

   15/7/2005 : implementation, first successful test

*/
/* $BEGIN_BCI2000_LICENSE$
 * 
 * This file is part of BCI2000, a platform for real-time bio-signal research.
 * [ Copyright (C) 2000-2012: BCI2000 team and many external contributors ]
 * 
 * BCI2000 is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 * 
 * BCI2000 is distributed in the hope that it will be useful, but
 *                         WITHOUT ANY WARRANTY
 * - without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 * $END_BCI2000_LICENSE$
 */

#include <windows.h>
#include <winbase.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#define DEFAULT_COMPORT "COM4"       // default COM-Port is COM4
#define DEFAULT_BAUD CBR_57600       // default Baud-Rate is 56k
#define COM_TIMEOUT 50               // 50 ms trasmission timeout
#define ser_t HANDLE

#define EEG_CHANNELS 6
#define PACKET_FINISHED 5            // packet-state for completed packet

ser_t openSerialPort(const char *devname);
void closeSerialPort(ser_t device);

int readSerial(ser_t s, char *buf, int size);
/* Returns number of bytes read, or -1 for error */

void parse_byte_P2(unsigned char actbyte);
void parse_byte_P3(unsigned char actbyte) ;
void read_channels(ser_t handle, int protocol);


typedef struct PACKETStruct        // the data structure for tranmission decoding
{
	unsigned char	  readstate;
	unsigned int 	  extract_pos;
	unsigned char     packetcount;
	unsigned int      buffer[EEG_CHANNELS*2];   // need bigger buffer for P3-decoding
	unsigned char     switches;
	unsigned char     aux;
} PACKETStruct;

extern struct PACKETStruct PACKET;
