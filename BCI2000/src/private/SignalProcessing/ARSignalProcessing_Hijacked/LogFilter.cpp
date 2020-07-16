////////////////////////////////////////////////////////////////////////////////
// $Id: $
// Authors: 
// Description: LogFilter implementation
//   
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
#include "LogFilter.h"

using namespace std;


RegisterFilter( LogFilter, 2.C2 );


LogFilter::LogFilter()
{

 BEGIN_PARAMETER_DEFINITIONS

   "Filtering:LogFilter float LogFilterEps=  1e-8 1e-8 0 % // add this before taking the log, to avoid infinities",
   "Filtering:LogFilter float LogFilterBase= 10 10 0 % // base to which log is taken (0 to disable)",
   "Filtering:LogFilter float LogFilterGain= 20 20 % % // coefficient by which to multiply logged values",

 END_PARAMETER_DEFINITIONS
  
}


LogFilter::~LogFilter()
{
  
}

void
LogFilter::Preflight( const SignalProperties& Input, SignalProperties& Output ) const
{  
  Output = Input;
  double base = Parameter( "LogFilterBase" );
  double gain = Parameter( "LogFilterGain" );
  double eps = Parameter( "LogFilterEps" );
  if( base <= 0.0 ) return;
  stringstream ss;
  if( gain != 1.0 ) ss << gain << "*";
  ss << "log" << base << "(" << Output.ValueUnit().Symbol() << ")";
  Output.ValueUnit().SetSymbol( ss.str() );
}


void
LogFilter::Initialize( const SignalProperties& Input, const SignalProperties& Output )
{
  double base = Parameter( "LogFilterBase" );
  mEnable = ( base > 0.0 );
  mMultiplier = Parameter( "LogFilterGain" ) / ( mEnable ? ::log( base ) : 1.0 );
  mEps = Parameter( "LogFilterEps" );
}


void
LogFilter::Process( const GenericSignal& Input, GenericSignal& Output )
{
  if( !mEnable )
  {
    Output = Input;
    return;
  }

  for( int ch = 0; ch < Output.Channels(); ch++ )
    for( int el = 0; el < Output.Elements(); el++ )
      Output( ch, el ) = mMultiplier * ::log( Input( ch, el ) + mEps );

  /*
  for( int ch = 0; ch < Output.Channels(); ch++ )
  {
    for( int el = 0; el < Output.Elements(); el++ )
	{
       bciout << (el?" | " : "") << Input( ch, el ) << "->" << Output( ch, el );
	}
	bciout << "//" << endl;
  }*/
}
