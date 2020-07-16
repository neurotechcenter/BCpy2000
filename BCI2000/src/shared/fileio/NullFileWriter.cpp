////////////////////////////////////////////////////////////////////////////////
// $Id: NullFileWriter.cpp 4202 2012-06-28 15:44:46Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A file writer class that does _not_ write out any data.
//   Useful when no data file output is desired.
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
#include "PCHIncludes.h"
#pragma hdrstop

#include "NullFileWriter.h"

using namespace std;

// File writer filters must have a position string greater than
// that of the DataIOFilter.
RegisterFilter( NullFileWriter, 1 );


NullFileWriter::NullFileWriter()
{
}


NullFileWriter::~NullFileWriter()
{
}

void
NullFileWriter::Publish()
{
}


void
NullFileWriter::Preflight( const SignalProperties&,
                                 SignalProperties& Output ) const
{
  Output = SignalProperties( 0, 0 );
}


void
NullFileWriter::Initialize( const SignalProperties& /*Input*/,
                            const SignalProperties& /*Output*/ )
{
}


void
NullFileWriter::Write( const GenericSignal& /*inSignal*/,
                       const StateVector&   /*inStatevector*/ )
{
}

