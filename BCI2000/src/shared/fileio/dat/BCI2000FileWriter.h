////////////////////////////////////////////////////////////////////////////////
// $Id: BCI2000FileWriter.h 4704 2014-03-25 19:55:47Z gmilsap $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A FileWriter filter that stores data into a BCI2000 dat file.
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
#ifndef BCI2000_FILE_WRITER_H
#define BCI2000_FILE_WRITER_H

#include "FileWriterBase.h"
#include "BCI2000OutputFormat.h"

class BCI2000FileWriter : public FileWriterBase
{
 public:
  BCI2000FileWriter()
  : FileWriterBase( mOutputFormat )
  {}
 private:
  BCI2000OutputFormat mOutputFormat;
};

#endif // BCI2000_FILE_WRITER_H
