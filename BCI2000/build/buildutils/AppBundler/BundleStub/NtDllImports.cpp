///////////////////////////////////////////////////////////////////////////////
// $Id$
// Author: juergen.mellinger@uni-tuebingen.de
// Description: C runtime imports from NtDll.dll.
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
///////////////////////////////////////////////////////////////////////////////
#include "DllImport.h"

#define IMPORTS \
_(_chkstk) _(__chkstk) \
_(memcpy) _(memset) _(memcmp) \
_(strlen) _(strcat) _(strcpy) \
_(wcslen) _(wcscat) _(wcscpy) _(_wcsnicmp) \
_(_allshl) _(_allshr) \


#pragma warning(disable : 4391 4392)
#define _(x) extern "C" void x(); static const char x##_name[] = #x;
IMPORTS
#undef _

struct DllImport NtDllImports[] =
{
#define _(x) { x##_name, &x },
IMPORTS
#undef _
 { 0, 0 }
};

#pragma function(memcpy, memset, memcmp)
#pragma function(strlen, strcat, strcpy)
#pragma function(wcslen, wcscat, wcscpy)
// This code must be large enough to accommodate up to 15 bytes for an absolute jump.
// A function call will safely exceed that limit, and will provide useful
// information if something went wrong.
// Also, each function's body must be individual, otherwise optimization may collapse
// all functions into a single one, with multiple names.
#define _(x) extern "C" void x() { DllImportNotInitialized(x##_name); }
IMPORTS
#undef _

