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
#include "AsyncIODevice.h"
#include "Debugging.h"

#if _WIN32
# include <Windows.h>
#endif

#include <memory>
#include <vector>

namespace bci
{

struct AsyncIODevice::Private
{
  struct Request : IORequest, Uncopyable
  {
    Request( size_t bytesPerBuffer );
    ~Request();
#if _WIN32
    OVERLAPPED mOv;
#endif
  };

  Private();
  ~Private();
  typedef std::shared_ptr<Request> RequestPtr;
  std::vector<RequestPtr> mRequests;
  int mNextToQueue, mNextToComplete,
      mQueueLength, mBytesPerBuffer,
      mRequestsQueued, mErrorCount;
#if _WIN32
  HANDLE mAbortEvent;
#endif
};

AsyncIODevice::Private::Private()
: mNextToQueue( 0 ),
  mNextToComplete( 0 ),
  mBytesPerBuffer( 0 ),
  mQueueLength( 0 ),
  mRequestsQueued( 0 ),
  mErrorCount( 0 )
{
#if _WIN32
  mAbortEvent = ::CreateEventA( nullptr, true, false, nullptr );
#endif
}

AsyncIODevice::Private::~Private()
{
#if _WIN32
  ::SetEvent( mAbortEvent );
  ::CloseHandle( mAbortEvent );
#endif
}

AsyncIODevice::Private::Request::Request( size_t bytesPerBuffer )
{
  IORequest::pBuffer = new char[bytesPerBuffer];
  IORequest::pRequestObject = nullptr;
#if _WIN32
  ::memset( &mOv, 0, sizeof( mOv ) );
  mOv.hEvent = ::CreateEventA( nullptr, true, false, nullptr );
  IORequest::pRequestObject = &mOv;
#endif
}

AsyncIODevice::Private::Request::~Request()
{
  delete[] static_cast<char*>( IORequest::pBuffer );
#if _WIN32
  ::CloseHandle( mOv.hEvent );
#endif
}

AsyncIODevice::AsyncIODevice()
: p( new Private )
{
}

AsyncIODevice::~AsyncIODevice()
{
  delete p;
}

AsyncIODevice&
AsyncIODevice::SetBytesPerBuffer( int bytesPerBuffer )
{
  if( bytesPerBuffer != p->mBytesPerBuffer )
  {
    p->mRequests.clear();
    p->mBytesPerBuffer = bytesPerBuffer;
    SetQueueLength( p->mQueueLength );
  }
  return *this;
}

int
AsyncIODevice::BytesPerBuffer() const
{
  return p->mBytesPerBuffer;
}

AsyncIODevice&
AsyncIODevice::SetQueueLength( int queueLength )
{
  while( p->mRequests.size() < queueLength )
    p->mRequests.push_back( Private::RequestPtr( new Private::Request( p->mBytesPerBuffer ) ) );
  while( p->mRequests.size() > queueLength )
    p->mRequests.pop_back();
  p->mQueueLength = queueLength;
  ResetQueue();
  return *this;
}

int
AsyncIODevice::QueueLength() const
{
  return p->mQueueLength;
}

void
AsyncIODevice::ResetQueue()
{
  p->mNextToQueue = 0;
  p->mNextToComplete = 0;
  p->mRequestsQueued = 0;
}

int
AsyncIODevice::RequestsQueued() const
{
  return p->mRequestsQueued;
}

AsyncIODevice::IORequest*
AsyncIODevice::NextRequest()
{
  if( p->mRequests.empty() )
    return nullptr;

  IORequest* pRequest = p->mRequests[p->mNextToQueue].get();
  ++p->mNextToQueue %= p->mRequests.size();
  return pRequest;
}

bool
AsyncIODevice::QueueRequest( IORequest* pRequest )
{
  bool done = false;
  if( !pRequest )
    pRequest = NextRequest();
  if( pRequest )
  {
#if _WIN32
    auto ov = static_cast<OVERLAPPED*>( pRequest->pRequestObject );
    ::ResetEvent( ov->hEvent );
    ov->Internal = 0;
    ov->InternalHigh = 0;
    ov->Pointer = 0;
#endif
    auto r = reinterpret_cast<Private::Request*>( pRequest );
    done = OnQueueRequest( *pRequest );
    if( done )
      ++p->mRequestsQueued;
  }
  return done;
}

const AsyncIODevice::IORequest*
AsyncIODevice::AwaitCompletion( const Time::Interval& timeout )
{
  if( p->mRequests.empty() )
    return nullptr;

  IORequest* pRequest = p->mRequests[p->mNextToComplete].get();
  ++p->mNextToComplete %= p->mRequests.size();
#if _WIN32
  ::ResetEvent( p->mAbortEvent );
  auto ov = static_cast<OVERLAPPED*>( pRequest->pRequestObject );
  HANDLE events[] = { ov->hEvent, p->mAbortEvent };
  auto r = ::WaitForMultipleObjects( 2, events, false, timeout.ToIntTimeout() );
  if( r != WAIT_OBJECT_0 )
    pRequest = nullptr;
#endif
  if( pRequest && !OnRequestCompletion( *pRequest ) )
    ++p->mErrorCount;
  if( pRequest )
  {
    Assert( p->mRequestsQueued > 0 );
    --p->mRequestsQueued;
  }
  return pRequest;
}

void
AsyncIODevice::AbortWaiting()
{
#if _WIN32
  ::SetEvent( p->mAbortEvent );
#endif
}

} // namespace

