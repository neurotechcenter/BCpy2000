//////////////////////////////////////////////////////////////////////
// $Id: BCIException.h 4973 2015-08-24 18:56:49Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: Classes for convenient throwing and catching of
//   expected and unexpected exceptions.
//
//   #include "BCIException.h"
//   ...
//   throw bciexception << "Illegal value of n: " << n;
//
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
#ifndef BCI_EXCEPTION_H
#define BCI_EXCEPTION_H

#include "Exception.h"

#define bciexception (Tiny::Exception_<std::exception>())
namespace bci { typedef Tiny::Exception Exception; }
typedef bci::Exception BCIException;

#endif // BCI_EXCEPTION_H
