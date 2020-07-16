////////////////////////////////////////////////////////////////////////////////////
// $Id: NIDAQFilter.h 5818 2018-11-09 15:50:15Z mellinger $                       //
// Author: justin.renga@gmail.com                                                 //
// Description: A filter that performs output for National Instruments DAQ boards //
//                                                                                //
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
////////////////////////////////////////////////////////////////////////////////////
#ifndef NIDAQ_FILTER_H
#define NIDAQ_FILTER_H

#include "GenericFilter.h"
#include "NIDAQmx.imports.h"
#include "Expression.h"
#include <vector>
#include <string>
#include <map>
#include <fstream>

#define MAX_RANGES 32

class NIDAQFilter : public GenericFilter
{
  public:
    NIDAQFilter();
    virtual ~NIDAQFilter();

    virtual void Preflight( const SignalProperties&, SignalProperties& ) const;
    virtual void Initialize( const SignalProperties&, const SignalProperties& );
    virtual void Process( const GenericSignal&, GenericSignal& );

    virtual void StartRun();
    virtual void StopRun();
    virtual void Halt();

    // Custom Member Functions //
    int    GetNumDigitalLines( std::string device );           //  acquires the number of digital lines
    int    GetNumAnalogOutputLines( std::string device );      //  acquries the number of analog output lines

  private:

    // Member Functions [Private] //
    
    int    ReportError(int errCode) const;                     //  reports any NIDAQ error that may be called
    bool   AcquireAOVRanges();                                 //  gathers the available acceptable voltages
    static std::string IntToString(int n);                     //  Returns a string version of integer argument
    static std::string FloatToString(float n);                 //  Returns a string version of float argument
    std::vector<std::string>   CollectDeviceNames();           //  collects the device names
    static bool find(std::string, std::vector<std::string>);   // determines if the specified device is connected to the computer
    static void Tokenize( std::string whole, std::vector<std::string>& parts, char delim, bool stripParts = true, bool discardEmpties = true );
    // Member Variables [Private] //
    
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
    uInt8*            mDigitalBuffer;
    float64*          mAnalogBuffer;
    bool              mRan;                          //  has the filter been run at least once?
    bool              mLogging;                      //  is there going to be logging?
    float             mSampleRate;                   //  the speed of the sampling rate of the logger
    std::vector<float>        mRanges;               //  the voltage ranges for analog input
    std::vector<std::string>  mDigitalChannelNames;  //  the names of all physical digital channels on the device(s)
    std::vector<std::string>  mAnalogChannelNames;   //  the names of all physical analog channels on the device(s)
    std::vector<Expression>   mExpressions;          //  the Expressions being used by the filter
};
#endif    // NIDAQ_FILTER_H
