////////////////////////////////////////////////////////////////////
// $Id$
// Author: juergen.mellinger@uni-tuebingen.de
// Description: Load a PE executable into memory.
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
////////////////////////////////////////////////////////////////////
#ifndef PE_LOADER_H
#define PE_LOADER_H

#include "NtHookFs.h"

namespace PELoader
{
  NTSTATUS GetImageSizes( NtHookFs::Stream*, int* pHeaderSize, LARGE_INTEGER* pImageSize );
  NTSTATUS ReadImageHeader( NtHookFs::Stream*, IMAGE_NT_HEADERS* );
  NTSTATUS ReadImageData( NtHookFs::Stream*, void* );
  NTSTATUS ApplyExePatch( void* );
  NTSTATUS ApplyRelocation( void* );
  NTSTATUS ApplyMemoryProtection( void* );
};

#endif // PE_LOADER_H

