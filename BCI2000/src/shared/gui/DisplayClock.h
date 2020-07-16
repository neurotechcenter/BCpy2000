////////////////////////////////////////////////////////////////////////////////
// $Id$
// Author: juergen.mellinger@uni-tuebingen.de
// Description: An class that manages access to display timing information,
//   and vblank synchronization.
//   The default strategy is to use OpenGL's swap interval feature to wait for
//   vblanks.
//   In some OpenGL drivers on Windows, users may disable swap interval
//   functionality, so we use Direct3D's WaitForVBlank() function instead
//   (which cannot be disabled by users).
//   On WindowsXP, only D3D9 is available, which does not provide a
//   WaitForVBlank() function but allows to read the current scan line,
//   which is then used in a Sleep()-based emulation of a WaitForVBlank()
//   function.
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
#ifndef DISPLAY_CLOCK_H
#define DISPLAY_CLOCK_H

#include "Uncopyable.h"
#include "ClockTick.h"

namespace AV {

class DisplayClock : Uncopyable
{
public:
  DisplayClock( void* /* OS window handle */ );
  ~DisplayClock();

  ClockTick LastVbl() const;
  const DisplayClock& WaitForVbl() const;

private:
  struct DisplayClockPrivate;
  DisplayClockPrivate* p;
};

} // namespace

#endif // DISPLAY_CLOCK_H
