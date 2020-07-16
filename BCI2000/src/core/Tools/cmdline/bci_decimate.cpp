////////////////////////////////////////////////////////////////////
// $Id: bci_decimate.cpp 5414 2016-07-01 19:45:56Z mellinger $
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
#include "MessageChannel.h"
#include "StateList.h"
#include "StateVector.h"
#include "Status.h"
#include "Param.h"
#include "ParamList.h"
#include "State.h"
#include "SysCommand.h"
#include "GenericVisualization.h"

using namespace std;

string ToolInfo[] =
{
  "bci_decimate",
  "Decimate data in a binary BCI2000 stream.",
  "Reads a BCI2000 compliant binary stream from standard input, "
    "and writes it to standard output, retaining only every "
    "Nth statevector and signal message.",
  "binary",
  "-d<N>,    --decimate=<N>        Decimation factor, defaults to 1",
  ""
};

class Decimate : public MessageChannel
{
public:
  Decimate( int factor, Streambuf& in, Streambuf& out )
  : MessageChannel( in ),
    mOutput( out ),
    mFactor( factor ), mCount( 0 ), mpStatevector( nullptr ) {}
  ~Decimate() { delete mpStatevector; }
  template<typename T> bool Send( const T& ) = delete;

protected:
  bool OnProtocolVersion( istream& ) override;
  bool OnStatus( istream& ) override;
  bool OnParam( istream& ) override;
  bool OnState( istream& ) override;
  bool OnVisSignalProperties( istream& ) override;
  bool OnVisSignal( istream& ) override;
  bool OnStateVector( istream& ) override;
  bool OnSysCommand( istream& ) override;

private:
  template<typename T> void Forward( istream& );
  template<typename T> void Absorb( istream& );

private:
  MessageChannel mOutput;
  int mFactor,
      mCount;
  StateList mStatelist;
  StateVector* mpStatevector;
};


ToolResult
ToolInit()
{
  return noError;
}

ToolResult
ToolMain( Streambuf& in, Streambuf& out, OptionSet& arOptions )
{
  int decimation = ::atoi( arOptions.getopt( "-d|-D|--decimation", "1" ).c_str() );
  Decimate decimator( decimation, in, out );
  while( decimator.HandleMessage() )
    ;
  if( in.Input()->Failed() )
    return illegalInput;
  return noError;
}

template<typename T>
void
Decimate::Forward( istream& is )
{
  T t;
  t.Unserialize( is );
  mOutput.Send( t );
}

template<typename T>
void
Decimate::Absorb( istream& is )
{
  T t;
  t.Unserialize( is );
}

bool
Decimate::OnProtocolVersion( istream& is )
{
  Forward<ProtocolVersion>( is );
  return true;
}

bool
Decimate::OnStatus( istream& is )
{
  Forward<Status>( is );
  return true;
}

bool
Decimate::OnParam( istream& is )
{
  Forward<Param>( is );
  return true;
}

bool
Decimate::OnState( istream& is )
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
    mOutput.Send( s );
  }
  return true;
}

bool
Decimate::OnVisSignalProperties( istream& is )
{
  Forward<VisSignalProperties>( is );
  return true;
}

bool
Decimate::OnVisSignal( istream& is )
{
  if( mCount % mFactor == 0 )
    Forward<VisSignal>( is );
  else
    Absorb<VisSignal>( is );
  return true;
}

bool
Decimate::OnStateVector( istream& is )
{
  if( !mpStatevector )
    mpStatevector = new StateVector( mStatelist );
  mpStatevector->Unserialize( is );
  // state vectors are sent first, so we increase
  // the count here rather than in the signal handler
  if( ( ++mCount %= mFactor ) == 0 )
    mOutput.Send( *mpStatevector );
  return true;
}

bool
Decimate::OnSysCommand( istream& is )
{
  Forward<SysCommand>( is );
  return true;
}
