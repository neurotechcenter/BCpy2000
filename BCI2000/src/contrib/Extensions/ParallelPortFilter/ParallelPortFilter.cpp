////////////////////////////////////////////////////////////////////////////////
// $Id: ParallelPortFilter.cpp 4536 2013-08-05 14:30:13Z mellinger $
// Author: kaleb.goering@gmail.com, juergen.mellinger@uni-tuebingen.de
// Description: A filter which controls parallel ports
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
#include "ParallelPortFilter.h"

#include "BCIStream.h"
#include "Expression.h"
#include "ParallelPort.h"

RegisterFilter( ParallelPortFilter, 3.B );

struct ParallelPortFilter::Private
{
  ParallelPort::List mPorts;
  std::vector< std::pair<Expression, ParallelPort::Ptr> > mOutputs;
};

ParallelPortFilter::ParallelPortFilter()
: p( new Private )
{
}

ParallelPortFilter::~ParallelPortFilter()
{
  delete p;
}

void
ParallelPortFilter::Publish()
{
  BEGIN_PARAMETER_DEFINITIONS
    "ParallelPort:Output matrix ParallelPortOutput= 0 {Device%20Name Expression} % % %"
      " // Matrix of Expressions for parallel port output",
  END_PARAMETER_DEFINITIONS
}

void
ParallelPortFilter::AutoConfig( const SignalProperties& )
{
  p->mOutputs.clear();
  p->mPorts.clear();

  ParallelPort::List portsDetected;
  if( ParallelPort::Enumerate( portsDetected ) )
  {
    if( portsDetected.empty() )
      bciout << "No parallel ports detected.";
    else
    {
      std::ostringstream ports;
      for( const auto& p : portsDetected )
        ports << *p << "\n";
      bciout << "Parallel ports detected:\n" << ports.str();
    }
  }

  ParamRef ParallelPortOutput = Parameter( "ParallelPortOutput" );
  if( ParallelPortOutput->NumRows() > 0 && ParallelPortOutput->NumColumns() != 2 )
    throw bcierr << "Expected two columns in ParallelPortOutput matrix: Device name, and Expression";

  std::vector<std::string> missingPorts;
  for( int i = 0; i < ParallelPortOutput->NumRows(); ++i )
  {
    std::string portName = ParallelPortOutput( i, 0 );
    ParallelPort::Ptr port = portsDetected.Find( portName );
    if( !port )
      missingPorts.push_back( portName );
    else
      p->mPorts.push_back( port );
  }
  for( const auto& portName : missingPorts )
    bcierr << "Parallel port device \"" << portName << "\" is not present in the system";
  for( auto& port : p->mPorts )
    port->Open();
}

void
ParallelPortFilter::Preflight( const SignalProperties &Input, SignalProperties &Output ) const
{
  GenericSignal preflightSignal( Input );
  ParamRef ParallelPortOutput = Parameter( "ParallelPortOutput" );
  for( int i = 0; i < ParallelPortOutput->NumRows(); ++i )
    Expression( ParallelPortOutput( i, 1 ) ).Evaluate( &preflightSignal );
  Output = Input;
}

void
ParallelPortFilter::Initialize( const SignalProperties &Input, const SignalProperties &Output )
{
  ParamRef ParallelPortOutput = Parameter( "ParallelPortOutput" );
  for( int i = 0; i < ParallelPortOutput->NumRows(); ++i )
  {
    std::string portName = ParallelPortOutput( i, 0 );
    auto& port = p->mPorts.Find( portName );
    port->Write( ParallelPort::DCR, 0x00 ); // output mode
    p->mOutputs.push_back( std::make_pair( Expression( ParallelPortOutput( i, 1 ) ), port ) );
    p->mOutputs.back().first.Compile();
  }
}

void
ParallelPortFilter::StartRun()
{
  for( auto& output : p->mOutputs )
    output.second->Write( ParallelPort::Data, 0 );
}

void
ParallelPortFilter::StopRun()
{
  for( auto& output : p->mOutputs )
    output.second->Write( ParallelPort::Data, 0 );
}

void
ParallelPortFilter::Process( const GenericSignal &Input, GenericSignal &Output )
{
  for( auto& output : p->mOutputs )
    output.second->Write( ParallelPort::Data, output.first.Evaluate( &Input ) );
  Output = Input;
}

