////////////////////////////////////////////////////////////////////////////////
// $Id: NeuralynxADC.cpp 3798 2012-02-01 18:07:06Z mellinger $
// Author: g.dimitriadis@donders.ru.nl s.klanke@donders.ru.nl
// Description: A source class that interfaces to the Neuralynx DigitalLynx
//              amplifier through its reversed engineered Matlab drivers
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

#include "PCHIncludes.h"
#pragma hdrstop

#include "NeuralynxADC.h"
#include "GenericSignal.h"

#include <math.h>
#include <queue>

using namespace std;

// Register the source class with the framework.
RegisterFilter( NeuralynxADC, 1 );


NeuralynxADC::NeuralynxADC()
{
  BEGIN_PARAMETER_DEFINITIONS

    "Source int SourceCh= 33 33 1 33"					        " // Number of data and event channels",
    "Source int SampleBlockSize= 512 512 1 5120"		        " // Samples transmitted per block",
    "Source int SamplingRate= 3255.6Hz 3255.6Hz 100Hz 32556Hz"  " // True Sampling Rate - Use the final Cheetah sampling Rate after subsampling.",
    "Source int EventCh= 0 0 0 1"                               " // Number of event channels (0 or 1)",

    "Source list ChannelNames= 33"
    "CSC01 CSC02 CSC03 CSC04 CSC05 CSC06 CSC07 CSC08 CSC09 CSC10 CSC11 CSC12 CSC13 CSC14 CSC15 CSC16 "
    "CSC17 CSC18 CSC19 CSC20 CSC21 CSC22 CSC23 CSC24 CSC25 CSC26 CSC27 CSC28 CSC29 CSC30 CSC31 CSC32 Events"
    " // Channel Names",

    "Source list SourceChOffset= 33 "
    "0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0"
    " // Channel Offset",

    "Source list SourceChGain= 33 "
    "1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1"
    " // Channel Gain",

  END_PARAMETER_DEFINITIONS

  dataThread = NULL;
}



NeuralynxADC::~NeuralynxADC()
{
    Halt();
    delete dataThread;
}



void NeuralynxADC::Preflight( const SignalProperties&,SignalProperties& Output ) const
{
    SignalType signalType = SignalType::float32;

    if(Parameter("SourceCh")!=Parameter("ChannelNames")->NumValues())
        bcierr<<"The list of the channel names isn't as long as the number of channels"<<endl;

    if(Parameter("SourceCh")!=Parameter("SourceChOffset")->NumValues())
        bcierr<<"The list of the channels'offsets isn't as long as the number of channels"<<endl;

    if(Parameter("SourceCh")!=Parameter("SourceChGain")->NumValues())
        bcierr<<"The list of the channels'gains isn't as long as the number of channels"<<endl;

    Output = SignalProperties(Parameter("SourceCh"), Parameter("SampleBlockSize"), signalType );

}


void NeuralynxADC::Initialize( const SignalProperties&, const SignalProperties& )
{
    samplingRate = Parameter("SamplingRate");
    dataThread = new NeuralynxThread(samplingRate);//make the new NeuralynxThread object;
    dataThread->Connect(false); //connect without user feedback
    dataThread->CheckAmp(); //Check that the amp works and get info on number of data and event channels
    int numCSC = dataThread->numCSC;
    int numEvC = dataThread->numEvC;

    //Initialize the NeuralynxThread. This will kill the previously opened connection to the amp and restart everything this
    // time in a seperate thread.
    InitializeNeuralynxTread();


    if(numCSC!=Parameter("SourceCh")-Parameter("EventCh"))
    {
        bcierr<<"The number of data channels returned from Neuralynx isn't equal to the SourceCh parameter"<<endl;
    }

    if(Parameter("EventCh")==0) numEvC=0; //If the user sets the event channels to 0 then don't use any
    if(numEvC!=Parameter("EventCh"))
    {
        bcierr<<"Set the number of Event Channels either equal to the one used by Neuralynx ("<<numEvC<<") or to 0"<<endl;
    }
}

void NeuralynxADC::Process( const GenericSignal&, GenericSignal& Output )
{
    //--Get the latest Neuralynx data from the Neuralynx buffer and push them
    //into the BCI2000 datastream
    for (int BCIsample = 0; BCIsample < Output.Elements(); ++BCIsample)
    {
        for (int channel = 0; channel < Output.Channels(); ++channel)
        {
            //bciout<<"About to push some data to Process"<<endl;
            Output( channel, BCIsample ) = dataThread->getNextValue();
        }
    }
}


void NeuralynxADC::Halt()
{
    if (dataThread != NULL)
    {
        dataThread->keepRunning = false;
        dataThread->Terminate();
        while( !dataThread->IsTerminated() ) //wait for the thread to terminate
          OSThread::Sleep( 1 );
    }
}




//Initialize the NeuralynxThread
void NeuralynxADC::InitializeNeuralynxTread()
{
    if (dataThread != NULL)
    {
        dataThread->keepRunning=false;
        dataThread->Terminate();
        while( !dataThread->IsTerminated() ) //wait for the thread to terminate
        {
            OSThread::Sleep(1);
        }
        delete dataThread;
    }
    dataThread = new NeuralynxThread(samplingRate);
    dataThread->keepRunning=true;
    dataThread->Start();
}



