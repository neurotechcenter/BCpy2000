/////////////////////////////////////////////////////////////////////////////
//
// File: Util.h
//
// Date: Oct 26, 2001
//
// Author: Juergen Mellinger
//
// Description: A collection of useful platform independent macros
//              and algorithms.
//
// Changes:
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
//////////////////////////////////////////////////////////////////////////////

#ifndef UTILH
#define UTILH

#include <string>
#include <exception>
#include <math.h>

#define EPS 1e-5
#define MAX(a,b)    ((a)>(b)?(a):(b))
#define MIN(a,b)    ((a)<(b)?(a):(b))
#define ROUND(a)    (floor((a)+0.5))
// A modulo function that always returns positive arguments.
#define MOD(a,b)    (((b)+(a)%(b))%(b))
// An integer division that does not round towards zero but towards minus infinity.
#define DIV(a,b)    ( ((a)<0) ? (-(-(a)-1)/(b)-1) : ((a)/(b)) )


class Util
{
  public:
    static std::string ConvertLiterals( const std::string& inString );

    class TPath : public std::string
    {
      public:
        TPath();
        TPath( const std::string &inString );
        ~TPath() {}

        TPath        operator+(  const std::string &inString );
        const TPath& operator+=( const std::string &inString );

        static void  Initialize( const char *inResourceDirectory );

      private:
        static std::string resourceDirectory;
    };
};

inline
const Util::TPath&
Util::TPath::operator+=( const std::string &inString )
{
  *this = *this + inString;
  return *this;
}

#endif // UTILH
