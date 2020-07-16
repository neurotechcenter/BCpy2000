5DT Data Glove 5 Ultra
======================

Author: Peter Brunner <pbrunner@wadsworth.org>
        Jeremy Hill <jezhill@gmail.com>
  Date: 2007-05-17 pbrunner  Initial Release of DataGlove5DTUFilter.cpp
                             BCI2000 v2.0 compatible Filter
        2009-01-14 pbrunner  Added Support for the 14 sensor data glove
        2011-01-20 jhill     Revamped into 5DTDataGloveUltraLogger.cpp
		                     - converted from Filter to Logger mechanism.
							 - made v.3.0-compatible (2.0 compatibility untested)
							 - added support for multiple gloves (NB: this
							   entailed changes to Parameter and State names)

Description:
------------
The 5DT Data Glove 5 Ultra is a 5 sensor data glove with 1 sensor per finger that
measures average of knuckle and first joint movement. The data glove is interfaced
over its full-speed USB 1.1 interface to the computer. The sensor resolution is
12-bit with at least 8 bit dynamic range. The temporal resolution is 13.33ms which
equals a sampling rate of 75 Hz. 

The 5DT Data Glove 14 Ultra is a 14 sensor data glove with 2 sensors per finger
that measures average of knuckle and first and second joint movement and the
abduction between the 5 fingers. The data glove is interfaced over its full-speed
USB 1.1 interface to the computer. The sensor resolution is 12 bit with at least
8 bit dynamic range. The temporal resolution is 13.33ms which equals a sampling
rate of 75 Hz. 


Vendor:
-------
The 5DT Data Glove can be purchased from 5DT (www.5dt.com) as a right handed
(5DT Data Glove Ultra 5 Right-handed) and left handed (5DT Data Glove Ultra 5
Left-handed) version for 995 USD each (as of 5/17/2007). The warranty involves
a 30 day customer satisfaction guarantee and 1 year product warranty. It comes
in a hard-plastic transport case (28cm x 10cm x 10cm) with the data glove, a
manual, a CD and a 3m USB cable. 


5DTUDataGloveUltraLogger.cpp for BCI2000 v.3.0+
==============================================

	Files
	-----
		./5DTUDataGloveUltraLogger.cpp
        ./5DTUDataGloveUltraLogger.h
        ./extlib/dylib/fglove.dll
        ./extlib/include/fglove.h
        ./readme.txt

	Integration
	-----------

	1. The 5DTUDataGloveUltraLogger.cpp file must be added to your project.
	   The corresponding header, as well as the manufacturer's fglove.h,
	   must be on the include path.
	   
	2. The dynamic link library fglove.dll has to be in the same directory as
	   the executable module.
	
	Command-line switches
	---------------------
		--LogDataGlove=1  is required when starting the source module.
		
	Parameters
	----------
	Config panel "Source" under "Log Input":
	
		LogDataGlove:        the command-line parameter is repeated here.
		
		DataGloveDerivative: whether to read positions or changes in position.
		
		DataGloveHandedness: a list of "left" and "right" strings.  If you set
		                     this to "left right" then the first left-handed
							 glove found will become glove 1, and the first
							 right-handed glove found will become glove 2.
							 Swap the order, remove one of the hands, or add
							 further "left"s and "right"s, as required.
							 
    State Variables
    ----------------
    For each of the 5 (or 14) sensors of each 5DT Data Glove 5 (or 14) Ultra,
	one state variable is stored with the data.
	
	Relative to the old BCI2000 v2.0 DataGloveFilter, note two changes. Firstly the
	state name encodes the glove number (examples are shown below for Glove1*;
	additional gloves will have higher numbers). Secondly, the sensors numbers
	are always 2 digits (Sensor01, Sensor02, etc).

		---------------------------------------------------------------
		StateName      == Finger       | 5 sensor | 14 sensor 
		---------------------------------------------------------------
		Glove1Sensor01 == thumb        | flexure  | flexure lower joint   
		Glove1Sensor02 == index        | flexure  | flexure at knuckle 
		Glove1Sensor03 == middle       | flexure  | flexure at knuckle
		Glove1Sensor04 == ring         | flexure  | flexure at knuckle
		Glove1Sensor05 == little       | flexure  | flexure at knuckle
		Glove1Sensor06 == thumb        | N/A      | flexure second joint
		Glove1Sensor07 == index        | N/A      | flexure second joint
		Glove1Sensor08 == middle       | N/A      | flexure second joint
		Glove1Sensor09 == ring         | N/A      | flexure second joint
		Glove1Sensor10 == little       | N/A      | flexure second joint 
		Glove1Sensor11 == thumb        | N/A      | abduction
		Glove1Sensor12 == index/middle | N/A      | abduction 
		Glove1Sensor13 == middle/ring  | N/A      | abduction
		Glove1Sensor14 == ring/ring    | N/A      | abduction
		---------------------------------------------------------------



