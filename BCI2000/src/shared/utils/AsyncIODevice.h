////////////////////////////////////////////////////////////////////////////////
// $Id$
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A base class to simplify dealing with asynchronous IO
//   (necessary to use certain amplifier APIs).
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
#ifndef ASYNC_IO_DEVICE_H
#define ASYNC_IO_DEVICE_H

#include "TimeUtils.h"
#include "Uncopyable.h"

namespace bci
{

class AsyncIODevice : Uncopyable
{
public:
  struct IORequest { void* pRequestObject, *pBuffer; };

  AsyncIODevice();
  virtual ~AsyncIODevice();

  AsyncIODevice& SetBytesPerBuffer( int );
  int BytesPerBuffer() const;
  AsyncIODevice& SetQueueLength( int );
  int QueueLength() const;
  void ResetQueue();

  int RequestsQueued() const;
  IORequest* NextRequest();
  bool QueueRequest( IORequest* = nullptr );
  const IORequest* AwaitCompletion( const Time::Interval& = Time::Forever );
  void AbortWaiting();

  int ErrorCount() const;

protected:
  virtual bool OnQueueRequest( IORequest& ) = 0;
  virtual bool OnRequestCompletion( IORequest& ) = 0;

private:
  struct Private;
  Private* p;
};

} // namespace

#endif // ASYNC_IO_DEVICE_H
