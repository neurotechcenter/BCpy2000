////////////////////////////////////////////////////////////////////////////////
// $Id: SoundStimulus.cpp 4731 2014-07-03 15:18:03Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A stimulus that plays a sound on the Stimulus::Present event.
//   To allow for finishing playback even after the SoundStimulus object has
//   been deleted, underlying WavePlayer instances are pooled, and only
//   deallocated when playback has finished.
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
#include "SoundStimulus.h"
#include "ObjectPool.h"
#include "StaticObject.h"

namespace
{

struct Available
{
  bool operator()( WavePlayer* p )
  { return !p->IsPlaying(); }
};

StaticObject< ObjectPool<WavePlayer, Available> > WavePlayerPool;

} // namespace

SoundStimulus::SoundStimulus()
: mpWavePlayer( WavePlayerPool().Get() )
{
}

SoundStimulus::~SoundStimulus()
{
}

