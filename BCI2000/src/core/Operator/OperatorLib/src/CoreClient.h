////////////////////////////////////////////////////////////////////////////////
// $Id: StateMachine.h 5293 2016-03-21 17:22:22Z mellinger $
// Authors: juergen.mellinger@uni-tuebingen.de
// Description: A class representing a core module that is connected to the
//   operator module.
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
#ifndef CORE_CLIENT_H
#define CORE_CLIENT_H

#include "CoreConnection.h"
#include "Lockable.h"
#include "Sockets.h"
#include "StateList.h"

class StateMachine;

class CoreClient : public CoreConnection, public Lockable<Mutex>
{
public:
  struct Info
  {
    Info()
    : Version( ProtocolVersion::None() ),
      Status( "no status available" ),
      MessagesSent( 0 ),
      MessagesRecv( 0 ),
      BytesSent( 0 ),
      BytesRecv( 0 )
    {}

    ProtocolVersion Version;
    std::string Name, Address, ListeningIP, ListeningPort, Status;
    long MessagesSent, MessagesRecv;
    std::streamsize BytesSent, BytesRecv;
  };
  typedef Synchronized<CoreClient*> Ptr;
  class List : private Lockable<NonrecursiveSpinLock>
  {
   public:
    ~List() { Clear(); }
    List& Add( CoreClient* );
    List& Clear();
    int Size() const { return static_cast<int>( mList.size() ); }
    Ptr operator[]( size_t i ) { return mList[i]; }
    const Ptr operator[]( size_t i ) const { return mList[i]; }
    bool Wait( int );
    bool AbortWaiting();
    void ProcessMessages();
    CoreClient** begin() { return mList.data(); }
    CoreClient** end() { return mList.data() + mList.size(); }
  private:
    std::vector<CoreClient*> mList;
    int mWaitCount;
  };

  CoreClient( StateMachine&, const std::string& name, const std::string& address, std::ptrdiff_t tag );
  ~CoreClient();

  typedef int SysState;
  SysState State() const
    { return mState; }
  Info Info() const;
  Tiny::Socket& Socket()
    { return mSocket; }
  std::ptrdiff_t Tag() const
    { return mTag; }

  void ProcessMessages();
  void EnterState( SysState );

protected:
  bool OnStatus( std::istream& ) override;
  bool OnSysCommand( std::istream& ) override;
  bool OnParam( std::istream& ) override;

private:
  void OnAccept();
  void OnDisconnect();

  StateMachine& mrParent;
  ServerTCPSocket mSocket;
  std::ptrdiff_t mTag;
  struct Info mInfo;
  Synchronized<bool> mConnected;
  Synchronized<SysState> mState;
};

#endif // CORE_CLIENT_H

