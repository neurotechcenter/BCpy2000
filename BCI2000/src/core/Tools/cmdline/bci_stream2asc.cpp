////////////////////////////////////////////////////////////////////
// $Id: bci_stream2asc.cpp 5414 2016-07-01 19:45:56Z mellinger $
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
#include "ProtocolVersion.h"
#include "Status.h"
#include "Param.h"
#include "ParamList.h"
#include "State.h"
#include "StateList.h"
#include "StateVector.h"
#include "SysCommand.h"
#include "GenericVisualization.h"
#include "MessageChannel.h"
#include <iomanip>

using namespace std;

string ToolInfo[] =
{
  "bci_stream2asc",
  "Convert a binary BCI2000 stream into a human readable form.",
  "Reads a BCI2000 compliant binary stream from standard input, "
    "and writes it to standard output as a sequence of "
    "BCI2000 messages in a human readable representation.",
  "text",
  "",
};

class StreamToAsc : public MessageChannel
{
public:
  StreamToAsc( Streambuf& in, Streambuf& out )
    : MessageChannel( in ), mOut( out ), mpStatevector( nullptr ) {}
  ~StreamToAsc() { delete mpStatevector; }

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
  template<typename T> void Convert( istream& );
  template<typename T> void Print( const T& );

  OStream mOut;
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
  if( arOptions.size() > 0 )
    return illegalOption;
  StreamToAsc converter( in, out );
  while( converter.Good() )
    converter.HandleMessage();
  if( converter.Bad() )
    return fileIOError;
  if( converter.Failed() )
    return illegalInput;
  return noError;
}

template<typename T>
void
StreamToAsc::Convert( istream& is )
{
  T t;
  t.Unserialize( is );
  Print( t );
}

template<typename T>
void
StreamToAsc::Print( const T& arObj )
{
  mOut << ClassName( typeid( T ) ) << " { "
       << setw( 2 ) << arObj << "\n}\n";
}

bool
StreamToAsc::OnProtocolVersion( istream& is )
{
  Convert<ProtocolVersion>( is );
  return true;
}

bool
StreamToAsc::OnStatus( istream& is )
{
  Convert<Status>( is );
  return true;
}

bool
StreamToAsc::OnParam( istream& is )
{
  Convert<Param>( is );
  return true;
}

bool
StreamToAsc::OnState( istream& is )
{
  State s;
  s.Unserialize( is );
  if( is )
  {
    mStatelist.Delete( s.Name() );
    mStatelist.Add( s );
    if( mpStatevector != NULL )
    {
      delete mpStatevector;
      mpStatevector = new StateVector( mStatelist );
    }
    Print( s );
  }
  return true;
}

bool
StreamToAsc::OnVisSignalProperties( istream& is )
{
  Convert<VisSignalProperties>( is );
  return true;
}

bool
StreamToAsc::OnVisSignal( istream& is )
{
  Convert<VisSignal>( is );
  return true;
}

bool
StreamToAsc::OnStateVector( istream& is )
{
  if( !mpStatevector )
    mpStatevector = new StateVector( mStatelist );
  mpStatevector->Unserialize( is );
  Print( *mpStatevector );
  return true;
}

bool
StreamToAsc::OnSysCommand( istream& is )
{
  Convert<SysCommand>( is );
  return true;
}
