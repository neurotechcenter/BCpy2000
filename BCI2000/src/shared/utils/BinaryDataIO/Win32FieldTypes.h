////////////////////////////////////////////////////////////////////////////////
// $Id$
// Authors: mellinger@neurotechcenter.org
// Description: Basic Win32 data types defined using the BinaryField<>
//   template.
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
#ifndef WIN32_FIELD_TYPES_H
#define WIN32_FIELD_TYPES_H

#include "BinaryData.h"
#include <cstdint>

namespace Win32 {

typedef BinaryField<uint8_t, LittleEndian> BYTE;
typedef BinaryField<uint16_t, LittleEndian> WORD;
typedef BinaryField<uint32_t, LittleEndian> DWORD;
typedef BinaryField<int32_t, LittleEndian> LONG;
typedef BinaryField<int32_t, LittleEndian> FXPT2DOT30;

}

#endif // WIN32_FIELD_TYPES_H
