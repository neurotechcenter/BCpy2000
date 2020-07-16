////////////////////////////////////////////////////////////////////////////////
// $Id: ApplicationBase.cpp 4818 2015-04-26 00:33:09Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A base class for application modules.
//         This class defines parameters common to all application modules, and
//         defines two output streams intended for logging purposes:
//         - The AppLog.Screen stream is directed into a window displayed to the
//           operator user.
//         - The AppLog.File stream is directed into a log file, and displayed
//           in the operator user's log window.
//         - Writing to AppLog results in logging both to screen and file.
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
#include "ApplicationBase.h"

using namespace std;

ApplicationBase::ApplicationBase()
: RandomNumberGenerator( this )
{
  BEGIN_PARAMETER_DEFINITIONS
    "Visualize:Application%20Log int ShowAppLog= 1 0 0 1 // Show application log window (boolean)",
  END_PARAMETER_DEFINITIONS
  AppLog.Screen.Send( CfgID::WindowTitle, "Application Log" );
}

void
ApplicationBase::Initialize( const SignalProperties&,
                             const SignalProperties& )
{
  AppLog.Screen.Send( CfgID::Visible, Parameter( "ShowAppLog" ) );
}

int
ApplicationBase::StreamBundle::BundleStringbuf::sync()
{
  int result = stringbuf::sync();
  for( StreamSet::const_iterator i = mStreams.begin(); i != mStreams.end(); ++i )
    ( **i << this->str() ).flush();
  str( "" );
  return result;
}
