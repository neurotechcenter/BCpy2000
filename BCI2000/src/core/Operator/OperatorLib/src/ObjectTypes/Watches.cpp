////////////////////////////////////////////////////////////////////////////////
// $Id: Watches.cpp 5794 2018-08-14 13:39:22Z mellinger $
// Authors: juergen.mellinger@uni-tuebingen.de
// Description: A watch object, and a container for watches. A watch consists
//   of a number of expressions which send their values to a UDP port whenever
//   any of them changes.
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
#include "PCHIncludes.h"
#pragma hdrstop

#include "Watches.h"
#include "StateMachine.h"
#include "SystemStates.h"
#include "WildcardMatch.h"
#include "BCIException.h"
#include <sstream>
#include <iomanip>
#include <limits>
#include <algorithm>

using namespace std;

// Watch
Watch::Watch( CommandInterpreter& inInterpreter, const string& inAddress, long inID )
: mInterpreter( inInterpreter.StateMachine() ),
  mID( inID ),
  mCount( 0 ),
  mrList( inInterpreter.StateMachine().Watches() ),
  mSendMessages( &Watch::SendMessages, this )
{
  ScopedLock( mrList );
  if( mID == BCI_None )
  {
    if( inAddress.empty() )
    {
      string addr, var = inInterpreter.RemoteHostName();
      if( inInterpreter.LocalVariables().Exists( var ) )
        addr = inInterpreter.LocalVariables()[var];
      addr = inInterpreter.StateMachine().SuggestUDPAddress( addr );
      mSocket.Open( addr );
      if( mSocket.IsOpen() )
        mAddress = addr;
    }
    else
    {
      mSocket.Open( inAddress );
      if( mSocket.IsOpen() )
      {
        string s = mSocket.Address();
        if( inInterpreter.StateMachine().SuggestUDPAddress( s ) == s )
          mAddress = s;
        else
          mSocket.Close();
      }
    }
  }
  else while( mrList.FindByID( mID ) )
    ++mID;
  mrList.Add( this );
}

void
Watch::AboutToDelete()
{
  mrList.Remove( this );
  mSendMessages.Run();
}

bool
Watch::CheckAndTrigger()
{
  bool triggered = false;
  for( int i = 0; OnLoop( i ); ++i )
    if( OnCheck( i ) )
    {
      triggered = true;
      OnTrigger();
    }
  return triggered;
}

void
Watch::QueueMessage( const std::string& inMessage )
{
  mBuf = inMessage;
  WithLock( mCountLock )
  {
    char header[] = { ( mCount / 10 ) % 10 + '0', mCount % 10 + '0', '\t', 0 },
         footer[] = "\r\n";
    ++mCount %= 100;
    mQueue.Produce( header + inMessage + footer );
  }
  mThread.Run( mSendMessages );
}

void
Watch::SendMessages()
{
  SynchronizedQueue<string>::Consumable c;
  while( c = mQueue.Consume() )
  {
    if( mSocket.IsOpen() )
      mSocket.Write( c->c_str(), c->length() + 1 );
    if( mID != BCI_None )
    { // hide header from callback
      const char* p = c->c_str();
      while( *p && *p != '\t' )
        ++p;
      if( *p )
        ++p;
      Interpreter().StateMachine().ExecuteCallback( mID, p );
    }
  }
}

// SystemStateWatch
SystemStateWatch::SystemStateWatch(
  CommandInterpreter& inInterpreter,
  const string& inAddress,
  long inID )
: Watch( inInterpreter, inAddress, inID ),
  mState( BCI_None )
{
}

bool
SystemStateWatch::OnCheck( int )
{
  int state = BCI_GetStateOfOperation();
  bool result = ( state != mState );
  mState = state;
  return result;
}

void
SystemStateWatch::OnTrigger()
{
  string name = SystemStates::Name( mState );
  if( name.empty() )
    name = "<unknown>";
  QueueMessage( name );
}

// ExpressionWatch
ExpressionWatch::ExpressionWatch(
  const vector<string>& inExpressions,
  CommandInterpreter& inInterpreter,
  const string& inAddress,
  long inID )
: Watch( inInterpreter, inAddress, inID )
{
  for( size_t i = 0; i < inExpressions.size(); ++i )
  {
    mExpressions.push_back( InterpreterExpression( Interpreter(), inExpressions[i] ) );
    mExpressions.back().ForbidAssignment().Evaluate();
  }
  mValues.clear();
  mValues.resize( mExpressions.size(), numeric_limits<double>::quiet_NaN() );
}

bool
ExpressionWatch::OnLoop( int i )
{
  for( const auto& expr : mExpressions )
    if( expr.RefersStates() && i < Interpreter().StateMachine().StateVector().Samples() - 1 )
      return true;
  return i == 0;
}

bool
ExpressionWatch::OnCheck( int inSample )
{
  bool changed = false;
  size_t idx = 0;
  for( ExpressionList::iterator i = mExpressions.begin(); i != mExpressions.end(); ++i )
  {
    double result = i->Evaluate( inSample );
    if( result != mValues[idx] )
    {
      changed = true;
      mValues[idx] = result;
    }
    ++idx;
  }
  return changed;
}

void
ExpressionWatch::OnTrigger()
{
  ostringstream oss;
  oss << setprecision( 16 );
  if( !mValues.empty() )
    oss << Pretty( mValues[0] );
  for( size_t idx = 1; idx < mValues.size(); ++idx )
    oss << '\t' << Pretty( mValues[idx] );
  QueueMessage( oss.str() );
}

// Watch::Set
Watch::Set::Set( const Watch::Set& other )
: vector<Watch*>( other ), mpList( other.mpList )
{
  mpList->Lock();
}

Watch::Set::Set( const Watch::List* inList )
: mpList( inList )
{
  if( mpList != this )
    mpList->Lock();
}

Watch::Set::~Set()
{
  if( mpList != this )
    mpList->Unlock();
}


Watch*
Watch::Set::First() const
{
  Lock_ lock( this );
  return empty() ? 0 : *begin();
}

Watch*
Watch::Set::Next( Watch* inEl ) const
{
  Lock_ lock( this );
  const_iterator i = find( begin(), end(), inEl );
  if( i != end() )
    ++i;
  return i == end() ? 0 : *i;
}

Watch*
Watch::Set::FindByID( int inID ) const
{
  if( inID == BCI_None )
    return 0;

  Lock_ lock( this );
  for( const_iterator i = begin(); i != end(); ++i )
    if( ( *i )->ID() == inID )
      return *i;
  return 0;
}

Watch::Set
Watch::Set::SelectByAddress( const string& inAddress, options inOpt ) const
{
  Set result( mpList );
  struct Comp
  {
    static bool Eq( const string& a, const string& b )
      { return a == b; }
    static bool Wild( const string& a, const string& b )
      { return WildcardMatch( a, b ); }
  };
  bool (*match)( const string&, const string& ) = ( inOpt & WildcardYes ) ? &Comp::Wild : &Comp::Eq;

  Lock_ lock( this );
  for( const_iterator i = begin(); i != end(); ++i )
    if( !( *i )->Address().empty() && match( inAddress, ( *i )->Address() ) )
      result.push_back( *i );
  return result;
}

// Watch::Set::Lock_
Watch::Set::Lock_::Lock_( const Set* p )
: mp( p )
{
  mp->mpList->Lock();
}

Watch::Set::Lock_::~Lock_()
{
  mp->mpList->Unlock();
}

// Watch::List
Watch::List::List()
: Set( this ), mChecking( false )
{
}

Watch::List::~List()
{
  Lock_ lock( this );
  while( !empty() )
    delete back();
}

void
Watch::List::Add( Watch* inpWatch )
{
  Lock_ lock( this );
  if( find( begin(), end(), inpWatch ) == end() )
    push_back( inpWatch );
}

void
Watch::List::Remove( Watch* inpWatch )
{
  Lock_ lock( this );
  iterator i = find( begin(), end(), inpWatch );
  if( i != end() )
    erase( i );
}

void
Watch::List::Check()
{
  Lock_ lock( this );
  if( !mChecking )
  {
    mChecking = true;
    for( iterator i = begin(); i != end(); ++i )
      (*i)->Check();
    mChecking = false;
  }
}
