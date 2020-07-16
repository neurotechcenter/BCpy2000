CyberGloveLogger======================

Author: Sidhant Sharma <sidhant.sharma@gmail.com>
                   
Date: 2014-03-21 ssharma  Initial Release of CyberGloveLogger

- Supports 18 and 22 Sensor CyberGlove Models (wired & wireless versions)
- Maximum 2 CyberGloves supported in the extension

===============================================================================================
Integration into BCI2000:

Compile the extension into your source module by enabling contributed extensions in your CMake configuration. You can do this by going into your root build folder and deleting CMakeCache.txt and re-running the project batch file, or by running cmake -i and enabling BUILD_CYBERGLOVELOGGER.
Once the extension is built into the source module, enable it by starting the source module with the --LogCyberGlove=1 command line argument. The 'CGS_VirtualHandDevice.dll, CGS_VirtualHandRegistry.dll and CGS_DeviceManagerCore.dll' dynamic link libraries have to be in the same directory as the executable module. Refer to 'Requirements' below for useful information regarding using this extension.

===============================================================================================
Requirements:

- The CyberGloveSystems Software/drivers must be installed on the system before hand. 
- The recommended version is CyberGloveSystems SDK3.1.3 (32 bit) for Windows 7.
- A cyberglove must be connected/live in the DCU (Device Configuration Utility) interface each time for using   the CyberGloveLogger extension.
- The configuration in the DCU should be the default configuration.
- A right handed glove has to be named RightGlove in the DCU.
- A left handed glove has to be named LeftGlove in the DCU.
- For recording two gloves simultaneously, both gloves should be connected in the DCU 
  and renamed as RightGlove and LeftGlove accordingly as in the image (Click to enlarge).
- If either of the gloves added in the DCU is not live/connected, an error message will pop up in BCI. 
- This will require you to check the connection or reconnect the glove in the DCU.
- The user manual that comes with the CyberGlove explains how to use and connect gloves in the DCU.

=================================================================================================              
Parameters:

The CyberGlove is configured in the "Source" panel under the "Log Input" section

LogCyberGlove -> The command-line parameter to enable the CyberGloveLogger is repeated here.

DataGloveDerivative -> Whether to read positions(raw sensor values) or changes in position.

DataGloveHandedness -> A list of "left" and "right" strings. For a single glove, it will be either "left" or "right" depending on the handedness. For two gloves, if you set this to "left right" then the first left-handed glove found will become glove 1 and the first right-handed glove found will become glove 2. Swapping the order, that is "right left" will make the first right handed glove as glove 1 and the first left handed glove as glove 2. Accordingly the data will be recorded into the states for each glove. You can also use "R" or "L" for 1 glove. "R L" or "L R" for 2 gloves (order is not important).
[edit]State Variables

======================================================================================================
State Variables:

The state name encodes the glove number (examples are shown below for CyberGlove1*; additional glove will have higher number). Secondly, the sensors numbers are 2 digits (Sensor01, Sensor02, etc).
The 22 Sensor CyberGlove sensor mapping is given in the table below. Note that the State CyberGlove1Sensor08 is always 0 or N/A.
For the 18 Sensor CyberGlove, additionally the outer joint raw sensor values for the index, ring, middle, and pinky fingers are all 0's.

 ----------------------------------------------------------------
 StateName           == Sensor                  | 18 sensor    | 22 sensor 
 ----------------------------------------------------------------
 CyberGlove1Sensor01 == ThumbRoll               | roll         | roll  
 CyberGlove1Sensor02 == ThumbInnerJoint         | flexure      | flexure 
 CyberGlove1Sensor03 == ThumbOuterJoint         | flexure      | flexure
 CyberGlove1Sensor04 == ThumbIndexAbduction     | abduction    | abduction
 CyberGlove1Sensor05 == IndexInnerJoint         | flexure      | flexure
 CyberGlove1Sensor06 == IndexMiddleJoint        | flexure      | flexure
 CyberGlove1Sensor07 == IndexOuterJoint         | N/A          | flexure
 CyberGlove1Sensor08 == EMPTY                   | N/A          | N/A
 CyberGlove1Sensor09 == MiddleInnerJoint        | flexure      | flexure
 CyberGlove1Sensor10 == MiddleMiddleJoint       | flexure      | flexure 
 CyberGlove1Sensor11 == MiddleOuterJoint        | N/A          | flexure
 CyberGlove1Sensor12 == MiddleIndexAbduction    | abduction    | abduction 
 CyberGlove1Sensor13 == RingInnerJoint          | flexure      | flexure
 CyberGlove1Sensor14 == RingMiddleJoint         | flexure      | flexure
 CyberGlove1Sensor15 == RingOuterJoint          | N/A          | flexure 
 CyberGlove1Sensor16 == RingMiddleAbduction     | abduction    | abduction
 CyberGlove1Sensor17 == PinkyInnerJoint         | flexure      | flexure
 CyberGlove1Sensor18 == PinkyMiddleJoint        | flexure      | flexure
 CyberGlove1Sensor19 == PinkyOuterJoint         | N/A          | flexure
 CyberGlove1Sensor20 == PinkyRingAbduction      | abduction    | abduction 
 CyberGlove1Sensor21 == PalmArch                | arch         | arch
 CyberGlove1Sensor22 == WristFlexion            | flexure      | flexure 
 CyberGlove1Sensor23 == WristAbduction          | abduction    | abduction
 ----------------------------------------------------------------  

=======================================================================================================       