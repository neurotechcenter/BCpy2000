////////////////////////////////////////////////////////////////////////////////
// $Id: DylibImports.cpp 4987 2015-08-31 19:18:29Z mellinger $
// Authors: juergen.mellinger@uni-tuebingen.de
// Description: A class representing a PE (Windows) executable file header.
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
#ifndef MACH_O_HEADER_H
#define MACH_O_HEADER_H

struct mach_header;
struct load_command;

#include <cstdint>

namespace Tiny {

class MachOHeader
{
public:
  MachOHeader( void* inHeaderAddress = 0 );
  ~MachOHeader();

  int Bits() const
    { return mBits; }
  char* BaseAddress()
    { return mpAddress; }
  mach_header* MachHeader()
    { return mpHeader; }
    
  load_command* Command( int kind, int idx = 0 );
  

private:
  char* mpAddress;
  mach_header* mpHeader;
  int mBits;
  bool mNeedSwap;
};
    
class MachOFile
{
public:
    MachOFile( MachOHeader&, bool isLoadedImage = false );

    const char* ExportLibraryName();
    struct SymbolInfo { const char* name; int64_t offset; bool data; void* opaque; };
    bool NextExport( SymbolInfo& );
    
private:
    MachOHeader* mpHeader;
    bool mIsLoadedImage;
};

} // namespace

using Tiny::MachOHeader;
using Tiny::MachOFile;

#endif // MACH_O_HEADER_H
