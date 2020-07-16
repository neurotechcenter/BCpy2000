//////////////////////////////////////////////////////////////////////
// $Id: UnitTest.h 4780 2015-04-03 17:24:51Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A class and macros that allow to embed testing code
//   within source code files.
//
//   #include "UnitTest.h"
//
//   UnitTest( <name> )
//   {
//     // testing code
//     ...
//     if( failed )
//       TestFail << message;
//     // or
//     TestFail_if( condition, message << data );
//   }
//
//   In CMake, add a test by calling
//   UTILS_ADD_UNIT_TEST( source1.cpp source2.cpp ... )
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
///////////////////////////////////////////////////////////////////////
#ifndef TINY_UNIT_TEST_H
#define TINY_UNIT_TEST_H

#include "BCIRegistry.h"
#include "NullStream.h"
#include <vector>
#include <sstream>

#if DISABLE_UNITTEST

# define UnitTest( name ) \
static struct name##_ { name##_() {} private: void OnRun_(); } name; \
void name##_::OnRun_()

# define TestFail Tiny::NullStream()
# define TestFail_if( cond, msg )
# define TestRequire( cond )

#else // DISABLE_UNITTEST

# define UnitTest( name ) \
static struct name##_ : Tiny::Test::Base { name##_() : Tiny::Test::Base( #name ) {} private: void OnRun_(); } name; \
RegisterTest_( name ); \
void name##_::OnRun_()

# define TestFail FailStream_( __FILE__ , __LINE__ )
# define TestFail_if( cond, msg ) if( cond ) TestFail << #cond << ", " << msg << std::endl;
# define TestRequire( cond ) if( !(cond) ) TestFail << #cond << " should evaluate to true" << std::endl;

#endif // DISABLE_UNITTEST

namespace Tiny
{

namespace Test
{
#if TODO
# error Random<> not implemented yet
#endif
template<class T, int freq = 1>
class Random
{
  public:
  Random() : mValue( 1 ) {} 
  operator T const() { return mValue; }
  private:
  T mValue;
};

class Base
{
 public:
  typedef void(*FailHandler_)( const std::string& );

  Base( const std::string& name );
  bool Run_( FailHandler_ = NULL );

  struct RunAll_
  {
    RunAll_( FailHandler_ = 0 );
    operator int() const { return failures; }
    private: int failures;
  };
  static int Parse_( int argc, char** argv, bool exitIfFound = true );

 protected:
  std::ostream& FailStream_( const char*, int );
  virtual void OnRun_() = 0;

 private:
  const std::string mDesc;
  std::ostringstream mFailStream;

  static std::vector<Base*> sInstances;
  friend struct RunAll_;
};

} // namespace Test

} // namespace

using Tiny::Test::Random;

#endif TINY_UNIT_TEST_H
