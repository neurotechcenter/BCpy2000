////////////////////////////////////////////////////////////////////////////////
// $Id: ChannelSetSource.h 4448 2013-05-22 20:01:42Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A data source that collects its data from a channel set.
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
#ifndef CHANNEL_SET_SOURCE_H
#define CHANNEL_SET_SOURCE_H

#include "DataSource.h"
#include <vector>

class ChannelSetSource : public DataSource
{
 public:
   typedef std::vector<int> ChannelSet;

  ChannelSetSource( const std::string& inName, const ChannelSet& inSet );

 protected:
  void OnInitialize( const Context& );
  void OnProcess( const Context& );
  Value OnData( int );

 private:
  const GenericSignal* mpSignal;
  const ChannelSet mChannelSet;
};

#endif // CHANNEL_SET_SOURCE_H
