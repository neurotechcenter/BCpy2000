////////////////////////////////////////////////////////////////////////////////
// $Id: DylibImports.cpp 4987 2015-08-31 19:18:29Z mellinger $
// Authors: juergen.mellinger@uni-tuebingen.de
// Description: A class representing a file mapped into memory.
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
#ifndef FILE_MAPPING_H
#define FILE_MAPPING_H

#include <string>
#include <cstdint>

namespace Tiny {

class FileMapping
{
public:
  enum { read = 1, write = 2, };

  FileMapping();
  FileMapping( const std::string& file, int flags = read );
  ~FileMapping();

  FileMapping& Open( const std::string&, int = read );
  FileMapping& Close();
  FileMapping& SaveAs( const std::string& );

  char* BaseAddress() const;

  FileMapping& SetLength( int64_t );
  int64_t Length() const;

  operator bool() const;
  const std::string& Error() const;

private:
  struct Private;
  Private* p;
};

} // namespace

using Tiny::FileMapping;

#endif // FILE_MAPPING_H
