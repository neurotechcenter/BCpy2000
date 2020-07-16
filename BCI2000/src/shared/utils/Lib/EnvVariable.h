////////////////////////////////////////////////////////////////////////////////
// $Id: EnvVariable.h 5513 2016-08-23 20:05:52Z mellinger $
// Authors: juergen.mellinger@uni-tuebingen.de
// Description: A RAII class that makes temporary changes to environment
//   variables. Also encapsulates OS specific access to environment variables.
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
#ifndef TINY_ENV_VARIABLE_H
#define TINY_ENV_VARIABLE_H

#include <string>

namespace Tiny
{

class EnvVariable
{
 public:
  static bool Set( const std::string& name, const std::string& value );
  static bool Get( const std::string& name, std::string& value );
  static bool Clear( const std::string& name );

  explicit EnvVariable( const std::string& name );
  ~EnvVariable();
  bool Exists() const;
  // Set()/Get() will return false on error.
  bool Set( const std::string& value );
  bool Get( std::string& ) const;
  // returns empty string in case of error.
  std::string Get() const;
#if 0
  // These operators may throw in case of errors.
  operator std::string() const;
  EnvVariable& operator=( const std::string& );
#endif

 private:
  std::string mName, mInitialValue;
  bool mExisted;
};

} // namespace

using Tiny::EnvVariable;

#endif // TINY_ENV_VARIABLE_H