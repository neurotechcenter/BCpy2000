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
#ifndef GDS_NAUTILUS_CONFIG_H
#define GDS_NAUTILUS_CONFIG_H

#include "GDSProtocol.h"
#include "GDSXML.h"

#include "BCIException.h"

#include <vector>
#include <string>

class GDSNautilusConfig : public GDSXML::GDSXMLItem
{
 public:
  GDSNautilusConfig( size_t nActiveChans = 32 );

  class ChannelConfig : public GDSXML::GDSXMLItem
  {
   public:
    ChannelConfig( int channel_number, bool enabled );

    GDSXML::GDSValue< int > ChannelNumber;
    GDSXML::GDSValue< bool > Enabled;
    GDSXML::GDSValue< double > Sensitivity;
    GDSXML::GDSValue< bool > UsedForNoiseReduction;
    GDSXML::GDSValue< bool > UsedForCAR;
    GDSXML::GDSValue< int > BandpassFilterIndex;
    GDSXML::GDSValue< int > NotchFilterIndex;
    GDSXML::GDSValue< int > BipolarChannel;

    std::string MetaInfo() const { return "class_id=\"6\" tracking_level=\"0\" version=\"0\""; }

   protected:
    std::string ToXML() const;
  };

  enum InputSignal
  {
    INPUT_SIGNAL_ELECTRODE = 0,
    INPUT_SIGNAL_SHORTCUT = 1,
    INPUT_SIGNAL_TEST_SIGNAL = 5,
  };

  std::vector< ChannelConfig >& ChannelVector() { return mChannels; }

  GDSXML::GDSValue< bool > Slave;
  GDSXML::GDSValue< InputSignal > InputSignal;
  GDSXML::GDSValue< unsigned int > NumberOfScans;
  GDSXML::GDSValue< unsigned int > SampleRate;
  GDSXML::GDSValue< bool > NoiseReduction;
  GDSXML::GDSValue< bool > CAR;
  GDSXML::GDSValue< bool > AccelerationData;
  GDSXML::GDSValue< bool > Counter;
  GDSXML::GDSValue< bool > LinkQualityInformation;
  GDSXML::GDSValue< bool > BatteryLevel;
  GDSXML::GDSValue< bool > DigitalIOs;
  GDSXML::GDSValue< bool > ValidationIndicator;
  GDSXML::GDSValue< unsigned int > NetworkChannel;

  std::string MetaInfo() const { return "class_id=\"2\" tracking_level=\"0\" version=\"1\""; }

 protected:
  GDSXML::GDSVector< ChannelConfig > Channels;
  std::string ToXML() const;

 private:
  std::vector< ChannelConfig > mChannels;

};

#endif // GDS_NAUTILUS_CONFIG_H