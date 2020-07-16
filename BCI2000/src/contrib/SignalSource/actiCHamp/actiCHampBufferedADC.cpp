////////////////////////////////////////////////////////////////////$
// Authors: Paul Ignatenko <paul dot ignatenko at gmail dot com
// Description: Source module for the actiCHamp
//
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
// $END_BCI2000_LICENSE$
////////////////////////////////////////////////////////////////////////////////
#include "actiCHampBufferedADC.h"
#include "BCIStream.h"
#include "string"

#include "ThreadUtils.h" // for SleepFor()

using namespace std;


RegisterFilter( actiCHampBufferedADC, 1 );


actiCHampBufferedADC::actiCHampBufferedADC()
{
}

actiCHampBufferedADC::~actiCHampBufferedADC()
{
  this->OnHalt();
}

void
actiCHampBufferedADC::OnPublish()
{
 BEGIN_PARAMETER_DEFINITIONS
	 "Source:ActiCHamp int actiCHampAmplifierID= 0 "
		"0 0 10 // Amplifier device to use",

   "Source:ActiCHamp int AcquisitionMode= 0 0 0 3 // mode in which to initialize the amplifier: 0: normal signal acquisition, 1: ActiveShield signal acquisition, 2: impedance measurement, 3: test mode(enumeration)",
	 "Source:Signal%20Properties float SamplingRate= 200 "
		"200 50 2000 // Sampling rate at which to run the amplifier",

    "Source:Signal%20Properties int SampleBlockSize= 10 "
       "10 1 2000 // number of samples transmitted at a time",

   "Source:Signal%20Properties int SourceCh= 56 "
       "56 1 184 // number of source channels",
	   
    "Source:Signal%20Properties:DataIOFilter floatlist SourceChGain= 56 "
    "1 1 1 1 1 1 1 1 1 1 "
    "1 1 1 1 1 1 1 1 1 1 "
    "1 1 1 1 1 1 1 1 1 1 "
    "1 1 1 1 1 1 1 1 1 1 "
    "1 1 1 1 1 1 1 1 1 1 "
    "1 1 1 1 1 1 "
    "% % % // channels gains",

    "Source:Signal%20Properties:DataIOFilter floatlist SourceChOffset= 56 "
    "0 0 0 0 0 0 0 0 0 0 "
    "0 0 0 0 0 0 0 0 0 0 "
    "0 0 0 0 0 0 0 0 0 0 "
    "0 0 0 0 0 0 0 0 0 0 "
    "0 0 0 0 0 0 0 0 0 0 "
    "0 0 0 0 0 0 "
    "% % % // channels offset",


    "Source:ActiCHamp int ReferenceChannel= 1 "
       "% % % // Set the library reference channel",

    "Source:ActiCHamp intlist EegChList= 32 " 
        "1 2 3 4 5 6 7 8 9 10 "
        "11 12 13 14 15 16 17 18 19 20 "
        "21 22 23 24 25 26 27 28 29 30 "
        "31 32 "
       "% 1 % // list of Eeg channel indices to use",

    "Source:ActiCHamp intlist AuxChList= 8 " 
        "1 2 3 4 5 6 7 8"
       "% 1 % // list of Aux channel indices to use",

    "Source:ActiCHamp intlist TriggerChList= 16 " 
        "1 2 3 4 5 6 7 8 9 10 "
        "11 12 13 14 15 16"
       "% 1 % // list of Trigger channel indices to use",

    "Source:ActiCHamp int ActiveShieldGain= 100 "
       "100 1 100 // Set the gain in ActiveShield mode"


 END_PARAMETER_DEFINITIONS

  mp_eeg_list     = 0;
  mp_aux_list     = 0;
  mp_trigger_list = 0;
}

void actiCHampBufferedADC::OnHalt()
{
    delete[] mp_eeg_list;
    delete[] mp_aux_list;
    delete[] mp_trigger_list;
    mp_eeg_list     = 0;
    mp_aux_list     = 0;
    mp_trigger_list = 0;
    m_actiCHamp.close();
}

void actiCHampBufferedADC::OnPreflight( SignalProperties& Output ) const
{
    //Get max channel counts from device.
    actiCHampDevice preflight_tester;
    if( !preflight_tester.open((int)Parameter ("actiCHampAmplifierID")) )
      return;

    int maxEeg           = preflight_tester.get_max_supported_eeg();
    int maxAux           = preflight_tester.get_max_supported_aux();
    int maxTrigger       = preflight_tester.get_max_supported_trigger();

    //Parameters from config
    int mode             = Parameter("AcquisitionMode");
    int activeShieldGain = Parameter("ActiveShieldGain");
    int triggerCount     = Parameter("TriggerChList")->NumValues();
    int auxCount         = Parameter("AuxChList")->NumValues();
    int eegCount         = Parameter("EegChList")->NumValues();
    int samplingRate     = Parameter("SamplingRate");
    int numberOfChannels = Parameter( "SourceCh" );
    int samplesPerBlock  = Parameter( "SampleBlockSize" );
    int referenceChannel = Parameter( "ReferenceChannel");

    SignalType sigType   = SignalType::float32;
    Output               = SignalProperties( numberOfChannels, samplesPerBlock, sigType );
    preflight_tester.close();

    //Tests Follow
    //
    if( mode == 1 && (activeShieldGain < 0 || activeShieldGain > 100))
      bcierr<< "Active Shield Gain must be between 1 and 100" << endl;

    if( 10000%samplingRate != 0 || samplingRate < 1)
      bcierr << "SamplingRate must go into 10000 evenly, and be greater than 0" << endl;

    //Channel Tests
    if(triggerCount + auxCount + eegCount != numberOfChannels)
      bcierr<< "The number of channel elements in EegChList, AuxChList, and TriggerChList must add up to SourceCh"<<endl ;

    if(referenceChannel < 0 || referenceChannel > maxEeg )
      bcierr<< "ReferenceChannel must be between 0 and " << maxEeg << endl;

    if ( eegCount > maxEeg)
      bcierr<<"Eeg channels cannot exceed " << maxEeg << endl;

    if ( auxCount > maxAux)
      bcierr<<"Aux channels cannot exceed " << maxAux << endl;

    if ( triggerCount > maxTrigger)
      bcierr<<"Trigger channels cannot exceed " << maxTrigger << endl;

    for(int i = 0; i<eegCount; ++i)
    {
      int idx = Parameter( "EegChList" )(i);
      if(idx <1 || idx > maxEeg)
      {
          bcierr <<"Eeg Channels in EegChList must be between 1 and " << maxEeg << endl;
      }
    }
    for(int i = 0; i<auxCount; ++i)
    {
      int idx = Parameter( "AuxChList" )(i);
      if(idx <1 || idx > maxAux)
      {
          bcierr <<"Aux Channels in AuxChList must be between 1 and " << maxAux << endl;
      }
    }

    for(int i = 0; i<triggerCount; ++i)
    {
      int idx = Parameter( "TriggerChList" )(i);
      if(idx <1 || idx > maxTrigger)
      {
          bcierr <<"Triggers in TriggerChList must be between 1 and " << maxTrigger << endl;
      }
    }

}

void actiCHampBufferedADC::OnInitialize( const SignalProperties& Output )
{
    m_reference_channel    = (int)Parameter("referenceChannel") - 1;
    m_mode                  = (int)Parameter ("AcquisitionMode");
    m_device_number          = Parameter ("actiCHampAmplifierID");
    m_sample_rate            = Parameter( "SamplingRate" );
    m_sample_block_size      = Parameter( "SampleBlockSize" );
    m_msec_per_block         = 1000.0 * m_sample_block_size / m_sample_rate;
    m_active_shield_gain      = (int)Parameter("ActiveShieldGain");
    m_total_channel_count = Parameter("SourceCh");
    m_trigger_count       = Parameter("TriggerChList")->NumValues();
    m_aux_count           = Parameter("AuxChList")->NumValues();
    m_eeg_count           = Parameter("EegChList")->NumValues();
    mp_eeg_list           = new unsigned int[m_eeg_count];
    mp_aux_list           = new unsigned int[m_aux_count];
    mp_trigger_list       = new unsigned int[m_trigger_count];

    //Build our channel lists
    for(unsigned int i = 0; i<m_eeg_count; ++i)
    {
      mp_eeg_list[i] = (unsigned int)(Parameter( "EegChList" )(i) - 1);
    }
    for(unsigned int i = 0; i<m_aux_count; ++i)
    {
      mp_aux_list[i] = (unsigned int)(Parameter( "AuxChList" )(i) - 1);
    }
    for(unsigned int i = 0; i<m_trigger_count; ++i)
    {
      mp_trigger_list[i] = (unsigned int)(Parameter( "TriggerChList" )(i) - 1);
    }

    SetInitialAcquisitionTimeout( Time::Seconds( 3 ) );

}

void
actiCHampBufferedADC::OnStartAcquisition()
{
    m_actiCHamp.open(m_device_number);
    m_actiCHamp.set_rate(m_sample_rate);
    m_actiCHamp.set_mode((t_champMode) m_mode);
    m_actiCHamp.set_reference_channel(m_reference_channel - 1);
    m_actiCHamp.setup_channels(m_total_channel_count, m_eeg_count, m_aux_count, m_trigger_count);
    m_actiCHamp.route_channels(mp_eeg_list, mp_aux_list, mp_trigger_list);

    if(m_mode == 1)
    {
        m_actiCHamp.set_activeshield_gain(m_active_shield_gain);
    }

    m_actiCHamp.init();


    if(m_mode == 2)
    {
        if( !m_actiCHamp.start())
        {
          bcierr << "Device did not start"<<endl;;
        }
        m_actiCHamp.get_impedance_data();
        ThreadUtils::SleepForMs(2);
        m_actiCHamp.stop();
        m_actiCHamp.close();
        m_mode = 0;
        this->OnStartAcquisition();
    }
    else
    {
        if( !m_actiCHamp.start())
        {
          bcierr << "Device did not start"<<endl;
        }
    }
}

void actiCHampBufferedADC::DoAcquire( GenericSignal& Output ) 
{	
    m_actiCHamp.get_data(Output, m_sample_block_size);
}

void actiCHampBufferedADC::OnStopAcquisition()
{
    m_actiCHamp.stop();
}


