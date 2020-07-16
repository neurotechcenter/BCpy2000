////////////////////////////////////////////////////////////////////
// $Id: bci_stream2table.cpp 5414 2016-07-01 19:45:56Z mellinger $
// Author:  juergen.mellinger@uni-tuebingen.de
// Description: See the ToolInfo definition below.
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
////////////////////////////////////////////////////////////////////
#include "bci_tool.h"
#include "Param.h"
#include "ParamList.h"
#include "State.h"
#include "StateList.h"
#include "StateVector.h"
#include "GenericVisualization.h"
#include "MessageChannel.h"
#include "Files.h"

using namespace std;

string ToolInfo[] =
{
  "bci_stream2table",
  "Convert a binary BCI2000 stream into a human readable tabular form.",
  "Reads a BCI2000 compliant binary stream from standard input, "
    "and writes it to standard output "
    "as a tab/newline separated table of values.",
  "text",
  "",
};

class StreamToTable : public MessageChannel
{
public:
  StreamToTable( Streambuf& in, Streambuf& out )
  : MessageChannel( in ),
    mOutput( out ),
    mpStatevector( nullptr ), mSignalProperties( 0, 0 ),
    mInitialized( false ), mWriteoutPending( false ) {}
  ~StreamToTable() { delete mpStatevector; }
  void Finish();

protected:
  bool OnParam( istream& ) override;
  bool OnState( istream& ) override;
  bool OnVisSignalProperties( istream& ) override;
  bool OnVisSignal( istream& ) override;
  bool OnStateVector( istream& ) override;

private:
  void WriteOut( const GenericSignal& );

  OStream             mOutput;
  StateList           mStatelist;
  StateVector*        mpStatevector;
  SignalProperties    mSignalProperties;
  typedef set<string> StringSet; // A set is a sorted container of unique values.
  StringSet           mStateNames;
  bool                mInitialized,
                      mWriteoutPending;
};

ToolResult
ToolInit()
{
  return noError;
}

ToolResult
ToolMain( Streambuf& in, Streambuf& out, OptionSet& arOptions )
{
  if( arOptions.size() > 0 )
    return illegalOption;
  StreamToTable converter( in, out );
  while( converter.HandleMessage() )
    ;
  converter.Finish();
  if( in.Input()->Failed() )
    return illegalInput;
  return noError;
}

void
StreamToTable::Finish()
{
  if( mWriteoutPending )
    WriteOut( GenericSignal() );
}

bool
StreamToTable::OnParam( istream& is )
{
  return !!Param().Unserialize( is );
}

bool
StreamToTable::OnState( istream& is )
{
  State s;
  s.Unserialize( is );
  if( is )
  {
    mStatelist.Delete( s.Name() );
    mStatelist.Add( s );
    if( mpStatevector )
    {
      delete mpStatevector;
      mpStatevector = new StateVector( mStatelist );
    }
  }
  return true;
}

bool
StreamToTable::OnVisSignalProperties( istream& is )
{
  VisSignalProperties v;
  v.Unserialize( is );
  mSignalProperties = v.SignalProperties();
  return true;
}

bool
StreamToTable::OnVisSignal( istream& is )
{
  VisSignal v;
  v.Unserialize( is );
  WriteOut( v );
  return true;
}

bool
StreamToTable::OnStateVector( istream& is )
{
  static GenericSignal nullSignal;
  if( mWriteoutPending )
    WriteOut( nullSignal );
  if( mpStatevector )
    mpStatevector = new StateVector( mStatelist );
  mpStatevector->Unserialize( is );
  mWriteoutPending = true;
  return true;
}

void
StreamToTable::WriteOut( const GenericSignal& inSignal )
{
  // Print a header line before the first line of data.
  if( !mInitialized )
  {
    mSignalProperties = inSignal.Properties();
    mOutput << "#";
    mStateNames.clear();
    for( const auto& state : mStatelist )
      mStateNames.insert( state.Name() );
    for( StringSet::const_iterator i = mStateNames.begin(); i != mStateNames.end(); ++i )
      mOutput << "\t" << *i;
    for( int i = 0; i < inSignal.Channels(); ++i )
      for( int j = 0; j < inSignal.Elements(); ++j )
        mOutput << "\tSignal(" << mSignalProperties.ChannelLabels()[i] << "," << mSignalProperties.ElementLabels()[j] << ")";
    mOutput << endl;
    mInitialized = true;
  }
  if( inSignal.Properties() != mSignalProperties )
    bcierr << "Ignored signal with inconsistent properties" << endl;
  else
  {
    if( mpStatevector )
      for( StringSet::const_iterator i = mStateNames.begin(); i != mStateNames.end(); ++i )
        mOutput << "\t" << mpStatevector->StateValue( i->c_str() );
    else
      for( StringSet::const_iterator i = mStateNames.begin(); i != mStateNames.end(); ++i )
        mOutput << "\t0";

    for( int i = 0; i < inSignal.Channels(); ++i )
      for( int j = 0; j < inSignal.Elements(); ++j )
        mOutput << "\t" << inSignal( i, j );
    mOutput << endl;
  }
  mWriteoutPending = false;
}
