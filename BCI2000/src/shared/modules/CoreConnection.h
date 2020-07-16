////////////////////////////////////////////////////////////////////////////////
// $Id$
// Authors: schalk@wadsworth.org, juergen.mellinger@uni-tuebingen.de
// Description: A class representing a connection between core modules.
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
#ifndef CORE_CONNECTION_H
#define CORE_CONNECTION_H

#include "MessageChannel.h"
#include "Streambuf.h"
#include "Waitable.h"
#include "Mutex.h"
#include "Runnable.h"
#include "Thread.h"
#include "SynchronizedQueue.h"

class CoreConnection 
  : public MessageChannel, public Waitable
{
  enum { unknown, local, remote };
 public:
  class Client
  {
   friend class CoreConnection;
   Lockable<Mutex> mReceiveLock, mSendLock;

   protected:
    // Return "true" if the message has been read from the stream.
    virtual bool OnProtocolVersion( CoreConnection&, std::istream& ) { return false; }
    virtual bool OnParam( CoreConnection&, std::istream& ) { return false; }
    virtual bool OnState( CoreConnection&, std::istream& )  { return false; }
    virtual bool OnVisSignal( CoreConnection&, std::istream& )  { return false; }
    virtual bool OnVisSignalProperties( CoreConnection&, std::istream& ) { return false; }
    virtual bool OnStateVector( CoreConnection&, std::istream& ) { return false; }
    virtual bool OnSysCommand( CoreConnection&, std::istream& ) { return false; }
    virtual bool OnStatus( CoreConnection&, std::istream& ) { return false; }
    virtual bool OnVisMemo( CoreConnection&, std::istream& ) { return false; }
    virtual bool OnVisBitmap( CoreConnection&, std::istream& ) { return false; }
    virtual bool OnVisCfg( CoreConnection&, std::istream& ) { return false; }

    // Override to access messages that have been read into an internal buffer.
    virtual void OnReceive( CoreConnection&, const ProtocolVersion& ) {}
    virtual void OnReceive( CoreConnection&, const Param& ) {}
    virtual void OnReceive( CoreConnection&, const State& ) {}
    virtual void OnReceive( CoreConnection&, const VisSignal& ) {}
    virtual void OnReceive( CoreConnection&, const VisSignalProperties& ) {}
    virtual void OnReceive( CoreConnection&, const StateVector& ) {}
    virtual void OnReceive( CoreConnection&, const SysCommand& ) {}
    virtual void OnReceive( CoreConnection&, const Status& ) {}
    virtual void OnReceive( CoreConnection&, const VisMemo& ) {}
    virtual void OnReceive( CoreConnection&, const VisBitmap& ) {}
    virtual void OnReceive( CoreConnection&, const VisCfg& ) {}

    // Returning "false" will omit sending the message.
    virtual bool OnSend( CoreConnection&, const ProtocolVersion& ) { return true; }
    virtual bool OnSend( CoreConnection&, const Param& ) { return true; }
    virtual bool OnSend( CoreConnection&, const State& ) { return true; }
    virtual bool OnSend( CoreConnection&, const VisSignal& ) { return true; }
    virtual bool OnSend( CoreConnection&, const VisSignalProperties& ) { return true; }
    virtual bool OnSend( CoreConnection&, const StateVector& ) { return true; }
    virtual bool OnSend( CoreConnection&, const SysCommand& ) { return true; }
    virtual bool OnSend( CoreConnection&, const Status& ) { return true; }
    virtual bool OnSend( CoreConnection&, const VisMemo& ) { return true; }
    virtual bool OnSend( CoreConnection&, const VisBitmap& ) { return true; }
    virtual bool OnSend( CoreConnection&, const VisCfg& ) { return true; }
  };

 public:
  CoreConnection( Client& );
  ~CoreConnection();

  CoreConnection& SetInput( Tiny::InputObject*, bool async = false );
  CoreConnection& SetOutput( Tiny::OutputObject*, bool async = false );
  enum { AsyncSend = 1, AsyncReceive = 2, };
  CoreConnection& SetIO( Tiny::IOObject*, int flags = 0 );

  Waitable& Sent() { return mSender.Empty(); }

  CoreConnection& SetProtocol( const ProtocolVersion& v )
    { MessageChannel::SetProtocol( v ); return *this; }

  CoreConnection& SetIsLocal( bool b )
    { mIsLocal = b; return *this; }
  bool IsLocal() const
    { return mIsLocal; }

  int HandleMessages( int = -1 );

  void* UserData() const
    { return mpUserData; }
  CoreConnection& SetUserData( void* p )
    { mpUserData = p; return *this; }

 protected:
  bool OnProtocolVersion( std::istream& ) override;
  bool OnParam( std::istream& ) override;
  bool OnState( std::istream& ) override;
  bool OnVisSignal( std::istream& ) override;
  bool OnVisSignalProperties( std::istream& ) override;
  bool OnStateVector( std::istream& ) override;
  bool OnSysCommand( std::istream& ) override;
  bool OnStatus( std::istream& ) override;
  bool OnVisMemo( std::istream& ) override;
  bool OnVisBitmap( std::istream& ) override;
  bool OnVisCfg( std::istream& ) override;

  bool OnSend( const ProtocolVersion& ) override;
  bool OnSend( const Param& ) override;
  bool OnSend( const State& ) override;
  bool OnSend( const VisSignalConst& ) override;
  bool OnSend( const VisSignalProperties& ) override;
  bool OnSend( const StateVector& ) override;
  bool OnSend( const SysCommand& ) override;
  bool OnSend( const Status& ) override;
  bool OnSend( const VisMemo& ) override;
  bool OnSend( const VisBitmap& ) override;
  bool OnSend( const VisCfg& ) override;

 protected:
  bool OnMessageBuffered( const Message& ) override;
  bool OnMessageReceived( const Message& ) override;

 private:
  AsyncQueue<Message, CoreConnection> mSender;
  class Receiver : public MessageChannel, public Waitable
  {
   typedef bool (*OnConsume)( CoreConnection*, const Message& );
   public:
    Receiver( CoreConnection*, OnConsume );
    ~Receiver();
    void SetInput( Tiny::InputObject* );
    bool SetAsync( bool );
    bool Async() const;
    int DoHandleMessages( int );

   protected:
    bool OnMessageReceived( const Message& ) override;

   private:
    bool Consume();

    Thread mThread;
    void ThreadFunc();
    MemberCall<void(Receiver*)> mThreadFunc;

    UnbufferedIO mBuffer;
    SynchronizedQueue<Message> mQueue;
    CoreConnection* mpParent;
    OnConsume mOnConsume;
    struct : Lockable<Mutex>
    {
      Synchronized<bool> value;
    } mAsync;
  } mReceiver;

  void WriteMessage( const Message& );
  static void WriteMessage( CoreConnection*, const Message& );
  static bool HandleMessage( CoreConnection*, const Message& );

 private:
  UnbufferedIO mBuffer;
  bool mIsLocal;
  Client& mrClient;
  void* mpUserData;
};

#endif // CORE_CONNECTION_H
