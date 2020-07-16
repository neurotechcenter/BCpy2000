////////////////////////////////////////////////////////////////////////////////
// $Id: CommandInterpreter.h 5775 2018-06-04 13:03:53Z mellinger $
// Authors: schalk@wadsworth.org, juergen.mellinger@uni-tuebingen.de
// Description: A class that interprets individual operator scripting commands,
//   and performs command substitution.
//   The CommandInterpreter class implements three interfaces:
//   1) A client interface that provides an Execute() method, and access
//      to the last command's result.
//      The Execute() method is intentionally not thread safe. Use multiple
//      CommandInterpreter instances to execute scripting commands concurrently.
//   2) An internal "callback" interface to ObjectType descendant classes
//      which perform actual command execution.
//   3) A "listener" interface to the StateMachine class. This allows to
//      capture operator log messages.
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
#ifndef COMMAND_INTERPRETER_H
#define COMMAND_INTERPRETER_H

#include <vector>
#include <sstream>
#include <set>
#include "ArithmeticExpression.h"
#include "EnvVariable.h"
#include "Synchronized.h"
#include "Lockable.h"

class CommandInterpreter
{
 public:
  // Begin: Interface to users
  CommandInterpreter( class StateMachine& );
  CommandInterpreter( CommandInterpreter& );
  virtual ~CommandInterpreter();
  // Properties
  //  The name of the local variable holding the result of the last scripting command.
  static const char* ResultName()
     { return "Result"; }
  //  The name of the local variable holding the address and port of the current connection.
  static const char* RemoteHostName()
      { return "RemoteHost"; } 
  //  The result of the last executed scripting command.
  std::string Result() const
    { return mResultStream.str(); }
  //  A tag that is inserted in the output before reporting the exit
  //  code of a command. When no tag is found, the exit code may be
  //  assumed to be zero.
  static const char* ExitCodeTag()
    { return "\\ExitCode: "; }
  //  A tag that is inserted in the output to report that the system is
  //  going to quit.
  static const char* TerminationTag()
    { return "\\Terminating"; }
  // Methods
  // Interpret the argument as a single scripting command.
  int Execute( const std::string& );
  // Perform command substitution on the argument.
  std::string SubstituteCommands( const std::string& );
  // End: Interface to users

 public:
  // Begin: Interface to ObjectType instances.
  //  GetToken() reads a single string, which may be quoted and URL-encoded.
  //  Absence of a further token is treated as an error.
  //  Functions with "Optional" in their name do not treat missing data
  //  as errors.
  std::string GetToken();
  std::string GetOptionalToken()
    { SetFlag( optional ); return GetToken(); }
  //  GetMatchingTokens() does case-insensitive wildcard matching.
  //  Reads tokens until a match occurs, and returns the match.
  //  Without a match, it returns an empty string.
  //  In the second form, it returns whether a match has occurred,
  //  and writes values of the pattern and any subpatterns into
  //  its second argument.
  //  Use \( and \) to mark subpatterns in the input pattern.
  //  If no match is found, this causes an error.
  std::string GetMatchingTokens( const std::string& );
  bool GetMatchingTokens( const std::string&, std::vector<std::string>& );
  //  GetRemainingTokens() reads as many tokens as possible, and never
  //  results in an error.
  std::string GetRemainingTokens();
  //  GetRemainder() reads the remainder of the command line verbatim.
  std::string GetRemainder();
  std::string GetOptionalRemainder()
    { SetFlag( optional ); return GetRemainder(); }
  // Case-insensitive wildcard matching without affecting input state.
  bool MatchTokens( const std::string& );
  bool MatchRemainingTokens( const std::string& );
  bool MatchRemainder( const std::string& );
  //  Unget() undoes a read operation. Without a previous read operation, it does nothing.
  void Unget();
  //  WriteLine() writes a line to the output stream associated with a command
  //  interpreter, ReadLine() reads a line from the input stream associated with
  //  a command interpreter.
  bool WriteLine( const std::string& );
  bool ReadLine( std::string& );
  //  Set handler functions to be called by the two above functions.
  //  Handlers are pointers rather than virtual functions -- this way,
  //  they may be copied from one CommandInterpreter instance to the next.
  typedef bool (*WriteLineFunc)( void*, const std::string& );
  CommandInterpreter& WriteLineHandler( WriteLineFunc, void* );
  typedef bool (*ReadLineFunc)( void*, std::string& );
  CommandInterpreter& ReadLineHandler( ReadLineFunc, void* );

  typedef std::vector< std::vector<std::string> > ArgumentList;
  static void ParseArguments( std::string&, ArgumentList& );

  std::ostream& Out()
    { return mResultStream; }

  class StateMachine& StateMachine() const
    { return mrStateMachine; }

  ArithmeticExpression::VariableContainer& ExpressionVariables()
    { return mExpressionVariables; }

  struct VariableContainer : std::map<std::string, std::string>
  {
    bool Exists( const std::string& key )
      { return find( key ) != end(); }
  };
  VariableContainer& LocalVariables()
    { return mLocalVariables; }

  // Log and error message capturing.
  void CaptureLog( int messageCallbackID )
    { WithLock( mCaptureLock ) mLogCapture.insert( messageCallbackID ); }
  void DontCaptureLog()
    { WithLock( mCaptureLock ) mLogCapture.clear(); }
  void FlushCapturedLog()
  { WithLock( mCaptureLock ) { Out() << mLogBuffer; mLogBuffer.clear(); } }

  class LogStream : public std::ostream
  {
   public:
    LogStream( class StateMachine& s ) : std::ostream( &mBuffer ), mBuffer( s ) {}
    LogStream( const LogStream& l ) : std::ostream( &mBuffer ), mBuffer( l.mBuffer ) {}
    ~LogStream() { flush(); }
    template<typename T> std::ostream& operator<<( const T& t ) const
    { return static_cast<std::ostream&>(const_cast<LogStream&>(*this)) << t; }
   private:
    class LogBuffer : public std::stringbuf
    {
     public:
      LogBuffer( class StateMachine& s ) : mrStateMachine( s ) {}
      LogBuffer( const LogBuffer& l ) : mrStateMachine( l.mrStateMachine ) {}
     private:
      virtual int sync();
      class StateMachine& mrStateMachine;
    } mBuffer;
  };
  LogStream Log()
    { return LogStream( mrStateMachine ); }
  //  Allow for background processing (e.g., from long-running commands).
  //  Returns an estimate for the time spent inside the Background() call, in ms.
  int Background( int sleepTimeMs = -1 );
  // End: Interface to ObjectType instances.

  // Begin: StateMachine listener interface.
  //  HandleLogMessage() is called by StateMachine to report a message.
  void HandleLogMessage( int messageCallbackID, const std::string& );
  //  Set the abort flag (e.g., to interrupt long-running loops in scripts)
  void Abort();
  // End: StateMachine listener interface.
 private:
  CommandInterpreter& operator=( const CommandInterpreter& );
  void Init();
  void AddListener( CommandInterpreter& );
  void RemoveListener( CommandInterpreter& );
  std::string GetVariable( const std::string& inName );
  int EvaluateResult( const std::string& inCommand );
  static bool OnWriteLineDefault( void*, const std::string& );
  
  enum flags { none = 0, optional = 1 };
  void SetFlag( flags f ) { mFlags |= f; }
  void BeginRead();
  bool InputFailed();

  bool GetMatches( const std::string&, const std::string&, std::vector<std::string>& );

 private:
  std::ostringstream mResultStream;
  std::istringstream mInputStream;
  int mFlags;
  std::vector< std::pair<std::istream::pos_type, int> > mPosStack;

  class StateMachine& mrStateMachine;
  ArithmeticExpression::VariableContainer mExpressionVariables;
  VariableContainer mLocalVariables;

  Synchronized<bool> mAbort;
  std::set<CommandInterpreter*> mListeners;
  CommandInterpreter* mpParent;

  Lockable<> mCaptureLock;
  std::set<int> mLogCapture;
  std::string mLogBuffer;

  WriteLineFunc mWriteLineFunc;
  void* mpWriteLineData;
  ReadLineFunc mReadLineFunc;
  void* mpReadLineData;
};

#endif // COMMAND_INTERPRETER_H
