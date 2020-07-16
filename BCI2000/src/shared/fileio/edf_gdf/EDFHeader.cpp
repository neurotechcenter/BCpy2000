////////////////////////////////////////////////////////////////////////////////
// $Id: EDFHeader.cpp 4731 2014-07-03 15:18:03Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A C++ representation of a BCI2000 relevant subset of the EDF
//              data format as defined in Kemp et al, 1992, and the
//              GDF 2.10 data format as defined in Schloegl et al, 2008.
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
#include "EDFHeader.h"
#include "GDF.h"

using namespace std;
using namespace GDF;

size_t
EDFHeader::Length() const
{
  const size_t h = 8 + 80 + 80 + 16 + 8 + 44 + 8 + 8 + 4,
               ch = 16 + 80 + 5*8 + 80 + 8 + 32;
  return h + Channels.size() * ch;
}

ostream&
EDFHeader::Serialize( ostream& os ) const
{
  PutField< Str<8>  >( os, Version );
  PutField< Str<80> >( os, Patient );
  PutField< Str<80> >( os, Recording );
  PutField< Str<16> >( os, Startdate );
  PutField< Str<8>  >( os, static_cast<int>( 256 * ( Channels.size() + 1 ) ) );
  PutField< Str<44> >( os );
  PutField< Str<8>  >( os, NumBlocks );
  PutField< Str<8>  >( os, BlockDuration );
  PutField< Str<4>  >( os, Channels.size() );

  PutArray< Str<16> >( os, Channels, &ChannelInfo::Label );
  PutArray< Str<80> >( os, Channels, &ChannelInfo::TransducerType );
  PutArray< Str<8>  >( os, Channels, &ChannelInfo::PhysicalDimension );
  PutArray< Str<8>  >( os, Channels, &ChannelInfo::PhysicalMinimum );
  PutArray< Str<8>  >( os, Channels, &ChannelInfo::PhysicalMaximum );
  PutArray< Str<8>  >( os, Channels, &ChannelInfo::DigitalMinimum );
  PutArray< Str<8>  >( os, Channels, &ChannelInfo::DigitalMaximum );
  PutArray< Str<80> >( os, Channels, &ChannelInfo::Filtering );
  PutArray< Str<8>  >( os, Channels, &ChannelInfo::SamplesPerRecord );
  PutArray< Str<32> >( os, Channels );

  return os;
}


istream&
EDFHeader::Unserialize( istream& is )
{
  GetField< Str<8>  >( is, Version );
  GetField< Str<80> >( is, Patient );
  GetField< Str<80> >( is, Recording );
  GetField< Str<16> >( is, Startdate );
  GetField< Str<8>  >( is ); // redundant
  GetField< Str<44> >( is );
  GetField< Str<8>  >( is, NumBlocks );
  GetField< Str<8>  >( is, BlockDuration );
  size_t numChannels;
  GetField< Str<4>  >( is, numChannels );
  Channels.resize( numChannels );

  GetArray< Str<16> >( is, Channels, &ChannelInfo::Label );
  GetArray< Str<80> >( is, Channels, &ChannelInfo::TransducerType );
  GetArray< Str<8>  >( is, Channels, &ChannelInfo::PhysicalDimension );
  GetArray< Str<8>  >( is, Channels, &ChannelInfo::PhysicalMinimum );
  GetArray< Str<8>  >( is, Channels, &ChannelInfo::PhysicalMaximum );
  GetArray< Str<8>  >( is, Channels, &ChannelInfo::DigitalMinimum );
  GetArray< Str<8>  >( is, Channels, &ChannelInfo::DigitalMaximum );
  GetArray< Str<80> >( is, Channels, &ChannelInfo::Filtering );
  GetArray< Str<8>  >( is, Channels, &ChannelInfo::SamplesPerRecord );
  GetArray< Str<32> >( is, Channels );

  return is;
}
