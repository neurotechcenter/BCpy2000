////////////////////////////////////////////////////////////////////////////////
// $Id: BCIStream.h 5599 2017-01-17 20:04:11Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: Declarations for stream symbols related to error/info messages.
//              To report an error, write e.g.
//               bcierr << "My error message" << endl;
//              For an informational message, write
//               bciout << "My info message" << endl;
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
#ifndef BCI_STREAM_H
#define BCI_STREAM_H

#include <sstream>
#include <list>
#include <climits>
#include "ClassName.h"
#include "Lockable.h"
#include "NullStream.h"

// Context info added to output.
// This is overridden with the context information
// set via SetContext() if that is non-empty.
#ifdef _MSC_VER
# define CONTEXT_   __FUNCTION__
#else
# define CONTEXT_   __FUNC__
#endif // _MSC_VER

// Declaration of user symbols with stream syntax (must be macros to include debug info):
#define bcierr      bcierr__.Locked()( CONTEXT_ )
#define bciout      bciout__.Locked()( CONTEXT_ )( 0 )
#define bciwarn     bciwarn__.Locked()( CONTEXT_ )( 0 )
#define bcierr_     bcierr__.Locked()( Tiny::ClassName( typeid( *this ) ) )
#define bciout_     bciout__.Locked()( Tiny::ClassName( typeid( *this ) ) )
#define bciwarn_    bciwarn__.Locked()( Tiny::ClassName( typeid( *this ) ) )

#if BCIDEBUG
# define bcidbg     bcidbg__.Locked()( CONTEXT_ )( BCISTREAM_DBGLEVEL_ )
  // Use #define bcidbgOn to unconditionally display all bcidbg messages below that statement,
  // and #define bcidbgOff to suppress all bcidbg messages below that statement.
# ifndef bcidbgOn
  static const int bcidbgOn = 0;
# endif
# ifndef bcidbgOff
  static const int bcidbgOff = 0;
# endif
# define BCISTREAM_DBGLEVEL_ \
     BCIStream::IsSet<bcidbgOff>::Value ? BCIStream::OutStream::NeverDisplayMessage : \
     ( BCIStream::IsSet<bcidbgOn>::Value ? BCIStream::OutStream::AlwaysDisplayMessage : 1 )
#else
# define bcidbg     bcidbg_
# define bcidbg_   BCIStream::NullStream()
#endif

class EnvironmentBase;
class CoreModule;
class ParamListView;
namespace bci { class MessageChannel; }

namespace BCIStream
{
  bool InitialRedirection(); // redirect standard streams, depending on kind of executable
  bool RedirectToStdio();
  bool RedirectFromStdio();
  bool CompressMessages();

  // Stream message handling actions for bcierr/bciout/bciwarn/bcidebug
  typedef void (*Action)( const std::string& );
  void DebugMessage( const std::string& );
  void PlainMessage( const std::string& );
  void Warning( const std::string& );
  void ConfigurationError( const std::string& );
  void RuntimeError( const std::string& );
  void LogicError( const std::string& );

  // Stream configuration
  void Apply( const ParamListView& );
  void SetOutputChannel( bci::MessageChannel* );

  class ContextFrame;
  class Dispatcher;
  class OutStream : public Lockable<>, public std::ostream
  {
    friend class ::EnvironmentBase;
    friend class ::CoreModule;
    friend class ContextFrame;
    friend void Apply( const ParamListView& );

  public:
    static const int AlwaysDisplayMessage = INT_MIN;
    static const int NeverDisplayMessage = INT_MAX;

    static const int DisplayAllMessages = INT_MAX - 1;
    static const int SuppressAllMessages = INT_MIN;

    class Ref
    {
    public:
      Ref( OutStream& os ) : p( &os ) { p->Lock(); }
      Ref( const Ref& other ) : p( other.p ) { const_cast<Ref&>( other ).p = 0; }
      ~Ref() { if( p ) { p->Flush(); p->Unlock(); } }
      bool RefersTo( const OutStream& os ) const { return p == &os; }
      template<class T> Ref& operator<<( const T& t ) { if( p ) static_cast<std::ostream&>( *p ) << t; return *this; }
      Ref& operator<<( std::ostream& f( std::ostream& ) ) { if( p ) static_cast<std::ostream&>( *p ) << f; return *this; }
      Ref& operator()( int level ) { if( p ) p->MessageVerbosity( level ); return *this; }
      Ref& operator()( const std::string& s ) { if( p ) p->MessageContext( s ); return *this; }
      bool Empty() const { return !p || p->Empty(); }
    private:
      Ref& operator=( const Ref& );
      OutStream* p;
    };

    OutStream( Action = 0, int verbosityLevel = DisplayAllMessages );
    ~OutStream();

    OutStream& MessageContext( const std::string& s );
    // Specifying a verbosity/debug level with a message will result in filtering out all
    // messages above the global verbosity/debug level variable.
    OutStream& MessageVerbosity( int level );
    OutStream& ResetFormat();

    Ref Locked() { return Ref( *this ); }

    Ref operator()( const std::string& s ) { return Locked()( s ); }
    Ref operator()( int i ) { return Locked()( i ); }

    bool Empty()
      { return Flushes() == 0; }
    void Flush()
      { mBuf.Flush(); }
    int Flushes()
      { return mBuf.Flushes(); }
    void Clear()
      { std::ostream::clear(); mBuf.Clear(); }
    void Reset()
      { std::ostream::clear(); mBuf.Reset(); }

    template<class T> Ref operator<<( const T& t )
      { return Ref( *this ) << t; }
    Ref operator<<( std::ostream& f( std::ostream& ) )
      { return Ref( *this ) << f; }

   private:
    static void SetContext( const std::string& );
    void SetVerbosity( int level )
      { mVerbosityLevel = level; }

    void SetAction( Action action = 0 );
#if 1
    void SetFlushHandler( Action action = 0 )
      { SetAction( action ); }
#endif

    class StringBuf : public std::stringbuf
    {
     public:
      StringBuf();
      void SetContext( const std::list<std::string>& );
      void SetContext( const std::string& );
      void SetDispatcher( Dispatcher* );
      void Flush();
      int Flushes()
        { return mNumFlushes; }
      void Clear()
        { SetDispatcher( mpDispatcher ); mNumFlushes = 0; }
      void Reset()
        { str( "" ); mNumFlushes = 0; }

     private:
      void DoFlush( std::string& );
      int          mNumFlushes;
      std::string  mContext;
      Dispatcher*  mpDispatcher;

     protected:
      // This function gets called on ostream::flush().
      virtual int   sync();
    } mBuf;
    int mVerbosityLevel;
    bool mVerbosityLocked;
    Dispatcher* mpDispatcher;

    static std::list<std::string> sContext;
  };

  class ContextFrame
  {
   public:
    ContextFrame( const std::string& s )
      : mCopied( false )
      { OutStream::sContext.push_back( s ); }
    ContextFrame( ContextFrame& c )
      : mCopied( false )
      { c.mCopied = true; }
    ~ContextFrame()
      { if( !mCopied ) OutStream::sContext.pop_back(); }

   private:
    bool mCopied;
  };

  struct NullStream : Tiny::NullStream
  {
    void SetVerbosity( int )
      {}
    void SetAction( Action )
      {}
  };

  class Dispatcher
  {
   public:
    Dispatcher( BCIStream::Action a = LogicError )
      : mAction( a ) {}
    virtual ~Dispatcher()
      {}
    BCIStream::Action Action() const
      { return mAction; }
    void SetAction( BCIStream::Action a )
      { mAction = a; }
    void Dispatch( const std::string& context, const std::string& message );
    void Idle()
      { OnIdle(); }

   protected:
    virtual void OnFilter( BCIStream::Action&, std::string& );
    virtual void OnCompress( BCIStream::Action a, const std::string& s )
      { a( s ); }
    virtual void OnIdle()
      {}

   private:
    BCIStream::Action mAction;
  };

  template<int x = 1> struct IsSet
  { static const bool Value = x; };

} // namespace BCIStream

#endif // BCI_STREAM_H

extern BCIStream::OutStream bcierr__;
extern BCIStream::OutStream bciout__;
extern BCIStream::OutStream bciwarn__;
extern BCIStream::OutStream bcidbg__;

