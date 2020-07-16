////////////////////////////////////////////////////////////////////////////////
// $Id: ConnectorFilters.h 5076 2015-10-28 02:27:26Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A pair of filters that send/receive states and signals over a
//         UDP connection.
//
//         Data transmission is done via UDP socket connections.
//         Messages consist of a name and a value, separated by white space
//         and terminated with a single newline '\n' character.
//
//         Names may identify
//         -- BCI2000 states by name, and are then followed
//            by an integer value in decimal ASCII representation;
//         -- Signal elements in the form Signal(<channel>,<element>), and are
//            then followed by a float value in decimal ASCII representation.
//
//         Examples:
//           Running 0
//           ResultCode 2
//           Signal(1,2) 1e-8
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
#ifndef CONNECTOR_FILTERS_H
#define CONNECTOR_FILTERS_H

#include "GenericFilter.h"

#include "Sockets.h"
#include "Streambuf.h"
#include "Lockable.h"
#include "Thread.h"
#include "WaitableEvent.h"
#include <sstream>
#include <vector>

class ConnectorInput : public GenericFilter
{
 public:
          ConnectorInput();
  virtual ~ConnectorInput();
  virtual void Preflight( const SignalProperties&, SignalProperties& ) const;
  virtual void Initialize( const SignalProperties&, const SignalProperties& );
  virtual void StartRun();
  virtual void StopRun();
  virtual void Process( const GenericSignal&, GenericSignal& );
  virtual bool AllowsVisualization() const { return false; }

 private:
  std::string mConnectorInputAddress;
  ReceivingUDPSocket mSocket;
  BufferedIO mStreambuf;
  std::iostream mConnection;
  std::vector<std::string> mInputFilters;
};

class ConnectorOutput : public GenericFilter
{
 public:
          ConnectorOutput();
  virtual ~ConnectorOutput();
  virtual void Preflight( const SignalProperties&, SignalProperties& ) const;
  virtual void Initialize( const SignalProperties&, const SignalProperties& );
  virtual void StartRun();
  virtual void StopRun();
  virtual void Process( const GenericSignal&, GenericSignal& );
  virtual bool AllowsVisualization() const { return false; }

 private:
  void DeleteConnections();

  class Connection : public std::stringstream, public Lockable<>, public Thread, public WaitableEvent
  {
   public:
    Connection( const std::string& inAddress ) { mSocket.Open( inAddress ); }
    ~Connection() { Terminate(); }
    bool IsOpen() const { return mSocket.IsOpen(); }
    void Clear() { str( "" ); clear(); }
   private:
    int OnExecute();
   private:
    SendingUDPSocket mSocket;
  };
  std::string mConnectorOutputAddresses;
  std::vector<Connection*> mConnections;
};
#endif // CONNECTOR_FILTERS_H


