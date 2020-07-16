////////////////////////////////////////////////////////////////////////////////
// $Id$
// Author: juergen.mellinger@uni-tuebingen.de
// Description: Data structures for periodic clock ticks.
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
#ifndef CLOCK_TICK_H
#define CLOCK_TICK_H

#include "TimeUtils.h"
#include "Rate.h"

namespace bci
{

namespace ClockType
{
  enum
  {
    None = 0,
    Hardware = 1,
    RealTime = 2,
    Recording = 4,

    Timer = 16,
    Video = 32,
    Audio = 64,
  };
}

struct ClockTick
{
  // Ordinal number of the tick in a row of identical ticks.
  // An increase larger than 1 may occur in case of an unreliable clock to indicate missed ticks.
  int64_t count;
  // Absolute time of the tick, according to TimeUtils::MonotonicTime().
  Tiny::Time time;
  // Clock rate in tick counts per time interval.
  Rate rate;
  // ClockType flags ORed together to describe the clock.
  int type;

  static const ClockTick Null;

  struct Statistics;
};

struct ClockTick::Statistics
{
  int type;
  int64_t missed;
  Tiny::Time::Interval interval, jitter;
};

} // namespace bci

namespace ClockType = bci::ClockType;
using bci::ClockTick;

#endif // CLOCK_TICK_H
