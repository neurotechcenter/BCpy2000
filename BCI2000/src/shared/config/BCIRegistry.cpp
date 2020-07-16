////////////////////////////////////////////////////////////////////////////////
// $Id: BCIRegistry.cpp 5775 2018-06-04 13:03:53Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: This file contains a template for a registry function.
//   A registry function contains references to global variables representing
//   objects registered via the macros from BCIRegistry.h.
//
//   During compilation, define the REGISTRY_NAME macro to the name of the
//   desired registry function. This must match the name of a .inc file
//   included for actual object references.
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
#include "BCIRegistry.h"
#include <cstddef>

#ifndef REGISTRY_NAME
# error Define REGISTRY_NAME to the name of the desired registry function.
#endif // REGISTRY_NAME

#undef STR
#define STR( x ) #x
#undef INCLUDE_FILE_
#define INCLUDE_FILE_( x ) STR( x.inc )
#undef INCLUDE_FILE
#define INCLUDE_FILE( x ) INCLUDE_FILE_( x )

// Create the definition of a function that references the global variables defined by
// registration macros. Assumes that registration macros have been extracted into a file
// REGISTRY_NAME.cpp.

// We include the named registry file twice:
//  once to create a header section,
//  once to create a function definition.

// Re-define second-level macros to create declarations of global variables.
#undef RegisterFilter_
#define RegisterFilter_( name, pos, priority ) \
   extern "C" std::ptrdiff_t FilterObjectName_( name, pos, priority );

#undef RegisterExtension_
#define RegisterExtension_( x ) \
   extern "C" std::ptrdiff_t ExtensionObjectName_( x );

#undef bcitest
#define bcitest( x ) \
   extern "C" std::ptrdiff_t TestObjectName_( x );

#include INCLUDE_FILE( REGISTRY_NAME )

// Re-define second-level macros to create code referencing global variables.
#undef RegisterFilter_
#define RegisterFilter_( name, pos, priority ) \
   result += std::ptrdiff_t(FilterObjectName_( name, pos, priority ));

#undef RegisterExtension_
#define RegisterExtension_( x ) \
   result += std::ptrdiff_t(ExtensionObjectName_( x ));

#undef bcitest
#define bcitest( x ) \
   result += std::ptrdiff_t(TestObjectName_( x ));

// Define a global function called REGISTRY_NAME. Force inclusion of that function
// using MSVC's /include linker switch, or gcc's -Wl,-u option
extern "C" std::ptrdiff_t REGISTRY_NAME()
{
  std::ptrdiff_t result = 0;
#include INCLUDE_FILE( REGISTRY_NAME )
  return result;
}

