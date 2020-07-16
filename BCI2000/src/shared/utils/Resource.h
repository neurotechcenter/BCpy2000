////////////////////////////////////////////////////////////////////////////////
// $Id: Resource.h 4651 2013-11-22 16:48:25Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A type to represent binary resource objects linked into an
//   executable.
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
#ifndef BCI_RESOURCE_H
#define BCI_RESOURCE_H

namespace bci
{

struct Resource
{
  const char* type;
  size_t length;
  const char* data;
};

} // namespace

#endif // BCI_RESOURCE_H
