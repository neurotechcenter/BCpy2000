////////////////////////////////////////////////////////////////////////////////
// $Id: $
// Authors: griffin.milsap@gmail.com
// Description: A class that parses/creates a g.Nautilus configuration
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
#include "GDSNautilusConfig.h"

using namespace std;
using namespace GDSXML;

#define NAUTILUS_MAX_CHANNELS 64

GDSNautilusConfig::GDSNautilusConfig( size_t nActiveChans ) :
  Slave( "slave_", 0 ),
  InputSignal( "input_signal_", INPUT_SIGNAL_ELECTRODE ),
  NumberOfScans( "number_of_scans_", 8 ),
  SampleRate( "sample_rate_", 250 ),
  NoiseReduction( "noise_reduction_", false ),
  CAR( "car_", false ),
  AccelerationData( "acceleration_data_", false ),
  Counter( "counter_", false ),
  LinkQualityInformation( "link_quality_information_", false ),
  BatteryLevel( "battery_level_", false ),
  DigitalIOs( "digital_ios_", false ),
  ValidationIndicator( "validation_indicator_", false ),
  NetworkChannel( "network_channel_", 11 ),
  Channels( &mChannels )
{
  for( size_t i = 0; i < NAUTILUS_MAX_CHANNELS; ++i )
    mChannels.push_back( ChannelConfig( i, i < nActiveChans ) );
}

GDSNautilusConfig::ChannelConfig::ChannelConfig( int channel_number, bool enabled ) :
  ChannelNumber( "channel_number_", channel_number ),
  Enabled( "enabled_", enabled ),
  Sensitivity( "sensitivity_", 187500 ),
  UsedForNoiseReduction( "used_for_noise_reduction_", false ),
  UsedForCAR( "used_for_car_", false ),
  BandpassFilterIndex( "bandpass_filter_index_", -1 ),
  NotchFilterIndex( "notch_filter_index_", -1 ),
  BipolarChannel( "bipolar_channel_", -1 )
{

}

string
GDSNautilusConfig::ToXML() const
{
  stringstream xml;
  xml << GDSXML::GDSValue< string >( "DeviceConfigurationBase", "", "class_id=\"3\" tracking_level=\"1\" version=\"0\" object_id=\"_1\"" ) << "\n";
  xml << Slave << "\n"
      << InputSignal << "\n"
      << NumberOfScans << "\n"
      << SampleRate << "\n"
      << NoiseReduction << "\n"
      << CAR << "\n"
      << AccelerationData << "\n"
      << Counter << "\n"
      << LinkQualityInformation << "\n"
      << BatteryLevel << "\n"
      << DigitalIOs << "\n"
      << ValidationIndicator << "\n"
      << NetworkChannel << "\n"
      << GDSXML::GDSValue< string >( "channel_configurations_", Channels, "class_id=\"5\" tracking_level=\"0\" version=\"0\"" ) << "\n";

  return GDSValue< string >( "px", xml.str(), "class_id=\"4\" class_name=\"gAPI::gNautilus::Configuration\" tracking_level=\"1\" version=\"0\" object_id=\"_0\"" );
}

string
GDSNautilusConfig::ChannelConfig::ToXML() const
{
  std::stringstream xml;
  xml << ChannelNumber << "\n"
      << Enabled << "\n"
      << Sensitivity << "\n"
      << UsedForNoiseReduction << "\n"
      << UsedForCAR << "\n"
      << BandpassFilterIndex << "\n"
      << NotchFilterIndex << "\n"
      << BipolarChannel << "\n";
  return xml.str();
}