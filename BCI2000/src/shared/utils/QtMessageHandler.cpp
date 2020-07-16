////////////////////////////////////////////////////////////////////////////////
// $Id$
// Author: juergen.mellinger@uni-tuebingen.de
// Description: Installs a handler for Qt messages.
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
#if USE_QT

#include <QApplication>
#include <iostream>
#include <sstream>
# define LOG( x, y, z ) (x << y <<  ": " << z << std::endl)
# if HAVE_LIB_TINY
#  include "Debugging.h"
#  include "Exception.h"
#  define NOTIFY( x, y, z ) Tiny::SuggestDebugging_( y, z );LOG( x, y, z );
#  define THROW( x, y, z ) (throw Tiny::Exception_<x>( y, z ))
# else
#  include <cassert>
#  include <stdexcept>
#  define NOTIFY( x, y, z ) assert(!x);LOG( x, y, z );
#  define THROW( x, y, z ) (throw x( (y+z).c_str() ))
# endif

static void OnQtMessage( QtMsgType type, const QMessageLogContext& inContext, const QString& inMsg )
{
  std::ostringstream oss;
  oss << "\nFile: " << inContext.file << "\nLine: " << inContext.line << "\nFunction: " << (inContext.function ? inContext.function : "");
  std::string what = inMsg.toLocal8Bit().constData(), where = oss.str();
  switch( type )
  {
  case QtDebugMsg:
    LOG( std::clog, "Qt Debug Message", what + where );
    break;
  case QtWarningMsg:
    if(what.find("QTextCursor::setPosition:") == 0)
      break;
    if(what.find("accessibility") != std::string::npos)
      break;
    NOTIFY( std::clog, "Qt Warning Message", what + where );
    break;
  case QtCriticalMsg:
    NOTIFY( std::cerr, "Qt Critical Message", what + where );
    break;
  case QtFatalMsg:
    NOTIFY( std::cerr, "Qt Fatal Message", what + where );
    THROW( std::runtime_error, what, where );
    break;
  default:
    std::clog << "Qt Message: " << what << where << std::endl;
  }
}

static QtMessageHandler sHandler = qInstallMessageHandler( &OnQtMessage );

#endif // USE_QT