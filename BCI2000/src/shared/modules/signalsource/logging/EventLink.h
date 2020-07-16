/////////////////////////////////////////////////////////////////////////////
// $Id: EventLink.h 5293 2016-03-21 17:22:22Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A component that establishes a UDP connection with the
//   operator module, and asynchronously receives events over this
//   connection.
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
/////////////////////////////////////////////////////////////////////////////
#ifndef EVENT_LINK_H
#define EVENT_LINK_H

#include "Environment.h"
#include "Thread.h"
#include "Sockets.h"
#include "Streambuf.h"
#include <string>

class EventLink : public EnvironmentExtension, private Thread
{
 public:
  EventLink();
  virtual ~EventLink();

 protected:
  // EnvironmentExtension interface
  virtual void Publish();
  virtual void Preflight() const;
  virtual void Initialize();
  virtual void StartRun();
  virtual void StopRun();
  virtual void Halt();

  // OSThread interface
  virtual int OnExecute();

 private:
  ReceivingUDPSocket mInputSocket;
  BufferedIO mStreambuf;
  std::iostream mInput;
};

#endif // EVENT_LINK_H
