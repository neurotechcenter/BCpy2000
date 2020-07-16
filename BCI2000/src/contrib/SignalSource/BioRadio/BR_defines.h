/******************************************************************************
 * Program:   BioRadio.exe                                                    *
 * Module:    defines.h                                                       *
 * Version:   0.05                                                            *
 * Author:    Yvan Pearson Lecours                                            *
 * Copyright: (C) Wadsworth Center, NYSDOH                                    *
 ******************************************************************************
 * Version History:                                                           *
 *                                                                            *
 * V0.01 - 12/15/2005 - First start                                           *
 * V0.02 - 1/1/2006   - Removed old definitions
  ******************************************************************************/
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
/*/
#ifndef BR_DEFINES_H
#define BR_DEFINES_H


// Default bit resolution
#define ALL_CHANNELS 8
#define ALL_RANGES 8
#define BIT_RES 16
#define SLEEP 1
// Channel name aliases
#define CHANNEL0 0
#define CHANNEL1 1
#define CHANNEL2 2
#define CHANNEL3 3
#define CHANNEL4 4
#define CHANNEL5 5
#define CHANNEL6 6
#define CHANNEL7 7

// Enable Freq. hopping
#define HOP 1
//Definitions of the configuration file, the data files
#define CONFIG_FILE "config.ini"

// Size of internal buffer
#define BUFFER_SIZE 32768
// Default COM port

// Success flags aliases
#define COM_STATUS 0
#define PING_STATUS 1
#define PROGRAM_SUCCESS 2
// Char buffer size for writing from double to chars
#define CHAR_BUFFER 100
// Flag to determine whether or not there will be a display window for (start,ping, stop)
#define DISPLAY_PROGRESS 0

// default error definations
#define XERROR 1
#define XNO_ERROR 0

// Constants definitions
#define ZERO 0
#define ONE 1

// Running state definitions
#define RUNNING 1
#define NOT_RUNNING 0

// Bad data value for the bioradio
#define BAD_DATA -999

// Case definitions
#define _100mV 7
#define _50mV  6
#define _25mV  5
#define _12mV  4
#define _6mV   3
#define _3mV   2
#define _1p5mV 1
#define _750uV 0

// Translated values for the Bioradio
#define RANGE100mV 0.106838
#define RANGE50mV  0.053419
#define RANGE25mV  0.026709
#define RANGE12mV  0.013355
#define RANGE6mV   0.006677
#define RANGE3mV   0.003339
#define RANGE1p5mV 0.001669
#define RANGE750uV 0.000835

// First and last channel definitions
#define FIRST_CHANNEL 1
#define LAST_CHANNEL  8


// COM port definitions
#define AUTO      0
#define COM1      1
#define COM2      2
#define COM3      3
#define COM4      4
#define COM5      5
#define COM6      6
#define COM7      7
#define COM8      8
#define COM9      9
#define COM10    10
#define COM11    11
#define COM12    12
#define COM13    13
#define COM14    14
#define COM15    15
#define ALLPORTS 16

// True and false values
#define TRUE  1
#define FALSE 0


// Max voltage value
#define VRMAX 7

// Configuration string array size
#define STRING_ARRAY_SIZE 15

//Smallest block size
#define MIN_BLOCK_SIZE 80

#endif // BR_DEFINES_H
