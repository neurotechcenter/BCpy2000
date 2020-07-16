////////////////////////////////////////////////////////////////////////////////
// $Id: BCIStream_guiapp.cpp 5208 2016-01-21 16:44:24Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: Implementation of bcierr and bciout message handlers for a
//              GUI application.
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
#include "BCIStream.h"

#if _WIN32
# include <Windows.h>
#elif USE_QT
# include <QMessageBox>
#endif

using namespace std;

static std::ostream cerr_( nullptr ), clog_( nullptr ), cout_( nullptr );

bool
BCIStream::InitialRedirection()
{
  return RedirectFromStdio();
}

namespace
{

void WriteDebugString( const string& inText )
{
#if _WIN32
  if( ::IsDebuggerPresent() )
  {
    ::OutputDebugStringA( inText.c_str() );
    return;
  }
#endif
  cerr_ << inText << endl;
}

void DisplayMessageBox( const string& inTitle, const string& inText )
{
#if _WIN32
  ::MessageBoxA( NULL, inText.c_str(), inTitle.c_str(), MB_OK | MB_ICONHAND | MB_SYSTEMMODAL | MB_SETFOREGROUND );
#elif USE_QT
  QMessageBox::critical( NULL, inTitle.c_str(), inText.c_str() );
#else
  cerr_ << inTitle << ": " << inText;
#endif
}

enum
{
  debug,
  warning,
  error
};

void DisplayMessage( const string& inText, int inType = debug )
{
  string text = inText;
  if( text.find_last_of( ".!?" ) != text.length() - 1 )
    text += '.';
  string kind;
  switch( inType )
  {
    case error:
      DisplayMessageBox( "BCI2000 Error", text );
      break;
    case warning:
      DisplayMessageBox( "BCI2000 Warning", text );
      break;
    default:
      WriteDebugString( text );
    }
  }
} // namespace

bool
BCIStream::CompressMessages()
{
  return false;
}

void
BCIStream::PlainMessage( const string& message )
{
  DisplayMessage( message );
}

void
BCIStream::DebugMessage( const string& message )
{
  DisplayMessage( message );
}

void
BCIStream::Warning( const string& message )
{
  DisplayMessage( message, warning );
}

void
BCIStream::ConfigurationError( const string& message )
{
  DisplayMessage( message, error );
}

void
BCIStream::RuntimeError( const string& message )
{
  DisplayMessage( message, error );
}

void
BCIStream::LogicError( const string& message )
{
  DisplayMessage( message, error );
}

