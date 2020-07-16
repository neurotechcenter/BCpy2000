////////////////////////////////////////////////////////////////////////////////
// $Id: Resources.h 4447 2013-05-22 14:26:10Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: This header file provides access to blob resources added to
//   projects via the BCI2000_ADD_RESOURCE CMake command.
//   Blob resources are binary files of any kind, converted into a Blob::Object
//   that can be linked into an executable, and accessed under the object's
//   name specified when calling BCI2000_ADD_RESOURCE.
//   Resources are defined as global objects inside the Resources namespace.
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
#ifndef RESOURCES_H
#define RESOURCES_H

#include "Blob.h"

namespace bci {
  namespace Resources { }
}

#define DECLARE_RESOURCES 1
#include "Resources.inc"

typedef Blob Resource;
namespace Resources = bci::Resources;

#endif // RESOURCES_H
