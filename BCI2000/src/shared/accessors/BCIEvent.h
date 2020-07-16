//////////////////////////////////////////////////////////////////////
// $Id: BCIEvent.h 5683 2017-08-11 13:46:32Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A std::ostream based accessor interface to add events
//   to a globally maintained event queue.
//   Basing the accessor on std::ostream allows for convenient
//   conversion of numbers (e.g., state values) into event descriptor
//   strings.
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
///////////////////////////////////////////////////////////////////////
#ifndef BCI_EVENT_H
#define BCI_EVENT_H

#include <iostream>
#include <sstream>
#include "PrecisionTime.h"

#define bcievent BCIEvent()()

class DataIOFilter;
class EnvironmentBase;

class BCIEvent : public std::ostream
{
 friend class ::DataIOFilter;
 friend class ::EnvironmentBase;

 public:
  BCIEvent();
  BCIEvent(PrecisionTime);
  ~BCIEvent();
  std::ostream& operator()() { return *this; }
  std::ostream& operator()(const char* inDescriptor);

 private:
  static void SetEventQueue( class EventQueue* );
  static void AllowEvents();
  static void DenyEvents();

  struct StringBuf : std::stringbuf
  {
    StringBuf(PrecisionTime);
    int sync() override;
    PrecisionTime mTimestamp;
  } mBuf;
};

#endif // BCI_EVENT_H

