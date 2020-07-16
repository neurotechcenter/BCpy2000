////////////////////////////////////////////////////////////////////////////////
// $Id: BCIStream_mex.cpp 5641 2017-07-03 16:10:21Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: Implementation of bcierr and bciout message handlers for a
//              Matlab MEX file.
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
#include "BCIException.h"
#include <sstream>
#include "mex.h"

using namespace std;

bool
BCIStream::InitialRedirection()
{
#if _WIN32 // on other platforms, this is already redirected by Matlab
  static struct : std::stringbuf
  {
    int sync() override
    {
      std::string s = str();
      ::mexPrintf("%s", s.c_str());
      str("");
      return 0;
    }
  } sMexOutputBuf;
  std::cout.rdbuf(&sMexOutputBuf);
  std::clog.rdbuf(&sMexOutputBuf);
  std::cerr.rdbuf(&sMexOutputBuf);
#endif
  return true;
}

bool
BCIStream::CompressMessages()
{
  return false;
}

void
BCIStream::PlainMessage( const string& s )
{
  ::mexPrintf( s.c_str() );
}

void
BCIStream::DebugMessage( const string& s )
{
  ::mexPrintf( ( "Dbg: " + s ).c_str() );
}

void
BCIStream::Warning( const string& s )
{
  ::mexWarnMsgTxt( s.c_str() );
}

void
BCIStream::ConfigurationError( const string& s )
{
  // mexErrMsgTxt() would abort execution without executing destructors,
  // thus we need to throw an exception and call mexErrMsgTxt() from the
  // catch() clause.
  throw bciexception << s;
}

void
BCIStream::RuntimeError( const string& s )
{
  ::mexWarnMsgTxt( ( "Runtime error: " + s + "\n" ).c_str () );
}

void
BCIStream::LogicError( const string& s )
{
  ::mexWarnMsgTxt( ( "Logic error: " + s + "\n" ).c_str () );
}
