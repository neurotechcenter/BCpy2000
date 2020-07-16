////////////////////////////////////////////////////////////////////////////////
// $Id: FileWriterBase.h 5490 2016-08-04 16:40:42Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A base class that implements functionality common to all
//              file writer classes that output into a file.
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
#ifndef FILE_WRITER_BASE_H
#define FILE_WRITER_BASE_H

#include "GenericFileWriter.h"
#include "GenericOutputFormat.h"
#include "SynchronizedQueue.h"
#include "Files.h"
#include "Streambuf.h"

#include <string>
#include <iostream>
#include "Thread.h"

class FileWriterBase: public GenericFileWriter, Thread
{
 protected:
          FileWriterBase( GenericOutputFormat& );
 public:
  virtual ~FileWriterBase();
  virtual void Publish();
  virtual void Preflight( const SignalProperties& Input,
                                SignalProperties& Output ) const;
  virtual void Initialize( const SignalProperties& Input,
                           const SignalProperties& Output );
  virtual void StartRun();
  virtual void StopRun();
  virtual void Halt();
  virtual void Write( const GenericSignal& Signal,
                      const StateVector&   Statevector );

 private:
  virtual int OnExecute();

  GenericOutputFormat&     mrOutputFormat;
  BufferedIO mStreambuf;
  std::string              mFileName;
  std::ostream             mOutputFile;
  typedef SynchronizedQueue< std::pair<GenericSignal, StateVector> > Queue;
  Queue mQueue;
};

#endif // FILE_WRITER_BASE_H
