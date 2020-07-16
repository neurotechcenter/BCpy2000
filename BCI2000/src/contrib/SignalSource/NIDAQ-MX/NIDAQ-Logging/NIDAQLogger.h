////////////////////////////////////////////////////////////////////////////////
// $Id: NIDAQLogger.h 5818 2018-11-09 15:50:15Z mellinger $                   //
// Authors: justin.renga@gmail.com                                            //
// Description: The NIDAQ filter implements National Instruments Loggers      //
//              into the BCI2000 Project.                                     //
//                                                                            //
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
#ifndef NIDAQ_LOGGER_H
#define NIDAQ_LOGGER_H

#include "Environment.h"
#include "NIDAQmx.imports.h"
#include <vector>
#include <string>

#define MAX_RANGES 32            //  maximum number of range values to find (MAX_RANGES / 2 low:high pairs)

class NIDAQLogger : public EnvironmentExtension
{
  public:
    // Very Important Methods //
    NIDAQLogger();                   //  initialize any veriables (member) that can be initialized without data
    virtual ~NIDAQLogger();          //  clear all large data structures, and clear all remaining tasks that are being used
    virtual void Publish();          //  check the command line parameters, and setup any additional parameters
    virtual void Preflight() const;  //  check everything is okay
    virtual void Initialize();       //  initialize everything
    virtual void Process();          //  runs the event loop
    virtual void StartRun();         //  create the thread that performs the data acquisition
    virtual void StopRun();          //  stop the thread that performs the data acquisition
    virtual void Halt();             //  stop the thread that performs the data acquisition
    // "Get" Methods //
    bool    AcquireAIVRanges();                               // gets the available voltage ranges for analog input
    int      GetNumAnalogInputLines(std::string dev_name);    // gets the number of analog input lines based on the device name
    int      GetNumDigitalLines(std::string dev_name);        // gets the number of digital lines based on the device name
    // Error Checking Method //
    int      ReportError(int error) const;                    // reports an error if something goes wrong with a DAQmx function
  private:
    // Member Methods //
    static std::string IntToString(int n);                    // converts argument to a string
    static std::string FloatToString(float n);                // converts argument to a string
    std::vector<std::string> CollectDeviceNames();
    static bool find(std::string, std::vector<std::string>);  // determines if the specified device is connected to the computer
    static void Tokenize( std::string whole, std::vector<std::string>& parts, char delim, bool stripParts = true, bool discardEmpties = true );
    // Member Callback Methods //
    void          GetAnalogData();
    void          GetDigitalData();
    static int32  CVICALLBACK AnalogCallback(TaskHandle, int32, uInt32, void *);
    static int32  CVICALLBACK DigitalCallback(TaskHandle, int32, uInt32, void *);
    // Member Variables //
    std::string       mDigitalDeviceName;
    std::string       mAnalogDeviceName;
    int               mNumberOfDigitalChannelsFound;
    int               mNumberOfAnalogChannelsFound;
    int               mNumberOfDigitalChannelsUsed;
    int               mNumberOfAnalogChannelsUsed;
    std::vector<bool> mDigitalChannelUsage;          //  which digital channels are used and which unused
    std::vector<bool> mAnalogChannelUsage;           //  which analog channels are used and which unused
    std::string       mDigitalChannelSpec;           // [comma+space]-delimited string containing names of the digital channels that are used
    std::string       mAnalogChannelSpec;            // [comma+space]-delimited string containing names of the analog channels that are used
    TaskHandle        mDigitalTaskHandle;
    TaskHandle        mAnalogTaskHandle;
    uInt8             *mDigitalBuffer;               //  buffer for digital input
    float64           *mAnalogBuffer;                //  buffer for analog input (scaled)
    std::vector<std::string>  mDigitalChannelNames;  //  the names of all physical digital channels on the device(s)
    std::vector<std::string>  mAnalogChannelNames;   //  the names of all physical analog channels on the device(s)

    bool                mLogging;            //  is there going to be logging?
    float               mDigitalSampleRate;  //  the speed of the sampling rate of the logger
    float               mAnalogSampleRate;   //  the speed of the sampling rate of the logger
    std::vector<float>  mRanges;             //  the voltage ranges for analog input
    // Buffer Variables //
};
#endif // NIDAQ_LOGGER_H
