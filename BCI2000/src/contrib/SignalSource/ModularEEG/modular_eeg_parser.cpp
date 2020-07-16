/*

   file: modular_eeg_parser.cpp
   Moular EEG Packet Parser  for BCI2000 framework
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


#include "modular_eeg_parser.h"
#include "PrecisionTime.h"

struct PACKETStruct PACKET;

ser_t openSerialPort(const char *devname)   // open and init the serial port
{
	ser_t retval;
	DCB dcb;

	retval = CreateFile(devname,GENERIC_READ | GENERIC_WRITE,
                            0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (retval == INVALID_HANDLE_VALUE)  return(retval);
	if (!SetCommMask(retval, EV_RXCHAR)) return(INVALID_HANDLE_VALUE);

	SetupComm(retval, 10240, 4096);	// Set read buffer == 10K, write buffer == 4K

	dcb.DCBlength= sizeof(DCB);
       	GetCommState(retval, &dcb);

	dcb.BaudRate= DEFAULT_BAUD;   // default = 56700 , 8 N 1
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
 	if (!SetCommState(retval, &dcb)) return (INVALID_HANDLE_VALUE);

	return retval;
}

void closeSerialPort(ser_t device)       // close an open port handle
{
	CloseHandle(device);
}


int readSerial(ser_t s, char *buf, int size)
/* reads max. <size> bytes from the serial port,
   returns number of bytes read, or -1 if error */
{
   BOOL rv;
   COMSTAT comStat;
   DWORD errorFlags;
   DWORD len;

   ClearCommError(s, &errorFlags, &comStat);   // Only try to read number of bytes in queue
   len = comStat.cbInQue;
   if (len > (DWORD) size) len = size;
   if (len > 0)
   {
      rv = ReadFile(s, (LPSTR)buf, len, &len, NULL);
      if (!rv)
      {
	 len= 0;
	 ClearCommError(s, &errorFlags, &comStat);
      }
      if (errorFlags > 0)
      {
	ClearCommError(s, &errorFlags, &comStat);
	return -1;
      }
   }
   return len;
}


void read_channels (ser_t handle, int protocol)
//  reads one packet (all 6 channels) from serial port
//  this function is called n-times by the process() member-function
//  to get a sample block of size n.
{
    unsigned char buf[1];
    PrecisionTime time1, time2;

    time1=PrecisionTime::Now();   // get timestamp for timeout-test
    do
    {
       if (readSerial(handle, (char*)buf, 1)==1)  // read one byte from serial port
       {   // byte available: parse the selected protocol
           if (protocol==2)  parse_byte_P3(buf[0]);
           else parse_byte_P2(buf[0]);
       }
       else Sleep(1);   // no byte available: free CPU
       //  sleep(0) caused a CPU-load of more than 50% on a 1,6 GHZ win2000 laptop
       //  sleep(1) improved this situation
    }
    while ((PACKET.readstate!=PACKET_FINISHED) &&
            (PrecisionTime::UnsignedDiff(time1, PrecisionTime::Now())<COM_TIMEOUT));
           // read until the packet is finished or a timeout has occured

    PACKET.readstate=0;
}


void parse_byte_P2(unsigned char actbyte)         // parse a packet in P2 format
{
    switch (PACKET.readstate)
	{
		  case 0: if (actbyte==165) PACKET.readstate++;  // first sync byte
			  break;
		  case 1: if (actbyte==90)  PACKET.readstate++; // second sync byte
			  else PACKET.readstate=0;
			  break;
		  case 2: PACKET.readstate++;    // Version Number
			  break;
	      	  case 3: PACKET.packetcount = actbyte;
			  PACKET.extract_pos=0;PACKET.readstate++;
			  break;
		  case 4: if (PACKET.extract_pos < 12)
				  {   if ((PACKET.extract_pos & 1) == 0)
					     PACKET.buffer[PACKET.extract_pos>>1]=actbyte*256;
			              else PACKET.buffer[PACKET.extract_pos>>1]+=actbyte;
				      PACKET.extract_pos++;
				  }
				  else
				  {  PACKET.switches= actbyte;
				     PACKET.readstate=PACKET_FINISHED;
			 	 //  *** PACKET ARRIVED ***
				  }
		  		  break;
                  case PACKET_FINISHED: break;
		  default: PACKET.readstate=0;
		}
}







void parse_byte_P3(unsigned char actbyte)   // parse a packet in P3 format
{
   int i, j;
   int sync;


    switch (PACKET.readstate)
	{
		  case 0: if (actbyte & 0x80) {PACKET.extract_pos=0;PACKET.readstate++;}
			   break;
		  case 1: PACKET.buffer[PACKET.extract_pos]=actbyte;
		     PACKET.extract_pos++;
			  if (PACKET.extract_pos==11)
			  {
			    PACKET.packetcount= (PACKET.buffer[0] >> 1) & 0x3f;
			    PACKET.aux = (PACKET.buffer[0] << 7) & 0x80 | PACKET.buffer[1] & 0x7f;
			    if ((PACKET.packetcount & 7)==4) PACKET.switches = PACKET.aux;
			    sync = (PACKET.buffer[0] >> 6) & 2 | (PACKET.buffer[1] >> 7) & 1;
			    for (i = 0, j = 2; i < (6 >> 1); i++, j += 3)
			    {
				// Decode and store even-channel sample
		        	PACKET.buffer[i << 1] = ((unsigned short) PACKET.buffer[j]) & 0x7f | (((unsigned short) PACKET.buffer[j+2]) << 3) & 0x380;
			        // Decode and store odd-channel sample
			        PACKET.buffer[(i << 1) + 1] = ((unsigned short) PACKET.buffer[j+1]) & 0x7f | (((unsigned short) PACKET.buffer[j+2]) << 7) & 0x380;
				// Decode and store the sync bits
				sync = (sync << 3) | (int) ((PACKET.buffer[j] >> 5) & 4 | (PACKET.buffer[j+1] >> 6) & 2 | (PACKET.buffer[j+2] >> 7) & 1);
			    }
			    if (sync==1) // The sync marker is last, so sync should be == 1, or there was an error.
				{
					PACKET.extract_pos=0;
                                        PACKET.readstate=PACKET_FINISHED;
				 	// *** PACKET ARRIVED ***
				} else PACKET.readstate=0;
			  }
			  break;
                    case PACKET_FINISHED: break;
		  default: PACKET.readstate=0;
		}
}


/*

  ModularEEG Packet Format Version 2  (P2)

  The beginning of a Packet is indicated by the two sync-bytes 'A5 5A',
  followed by the Version Number, the Packet - Counter and
  the Values for 6 Channels (16bit integers, -> 12 bytes)
  The last byte represents the status of the 4 pushbuttons of the modularEEG
  in the lower nibble. This makes a total of 17 bytes per packet:


  1:  A5
  2:  5A
  3:  version-Number
  4:  Packetcount
  5:  chn1-hibyte
  6:  chn1-lowbyte
  7:  chn2-hibyte
  8:  chn2-lowbyte
  9:  chn3-hibyte
  10: chn3-lowbyte
  11: chn4-hibyte
  12: chn4-lowbyte
  13: chn5-hibyte
  14: chn5-lowbyte
  15: chn6-hibyte
  16: chn6-lowbyte
  17: buttonstate



---------------------------


  ModularEEG Packet Format Version 3 (P3)

  One packet can have zero, two, four or six channels (or more).
  The default is a 6-channel packet, shown below.

  0ppppppx     packet header
  0xxxxxxx

  0aaaaaaa     channel 0 LSB
  0bbbbbbb     channel 1 LSB
  0aaa-bbb     channel 0 and 1 MSB

  0ccccccc     channel 2 LSB
  0ddddddd     channel 3 LSB
  0ccc-ddd     channel 2 and 3 MSB

  0eeeeeee     channel 4 LSB
  0fffffff     channel 5 LSB
  1eee-fff     channel 4 and 5 MSB

  Key:

  1 and 0 = sync bits.
    Note that the '1' sync bit is in the last byte of the packet,
    regardless of how many channels are in the packet.
  p = 6-bit packet counter
  x = auxillary channel byte
  a - f = 10-bit samples from ADC channels 0 - 5
  - = unused, must be zero

  There are 8 auxillary channels that are transmitted in sequence.
  The 3 least significant bits of the packet counter determine what
  channel is transmitted in the current packet.

  Aux Channel Allocations:

  0: Zero-terminated ID-string (ASCII encoded).
  1:
  2:
  3:
  4: Port D status bits
  5:
  6:
  7:

  The ID-string is currently "mEEGv1.0".

*/
