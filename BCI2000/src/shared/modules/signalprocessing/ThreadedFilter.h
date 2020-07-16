////////////////////////////////////////////////////////////////////////////////
// $Id: ThreadedFilter.h 4529 2013-08-05 13:56:53Z mellinger $
// Authors: juergen.mellinger@uni-tuebingen.de
// Description: ThreadedFilter<T> is a template class for filters that use
//   multiple threads to compute their results. The template argument must be a
//   descendant of the FilterThread base class.
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
#ifndef THREADED_FILTER_H
#define THREADED_FILTER_H

#include "GenericFilter.h"
#include "ReusableThread.h"
#include "Runnable.h"
#include <vector>

class FilterThread : protected Environment, private ReusableThread
{
 public:
  FilterThread()
  : mpInput( NULL ), mpOutput( NULL ),
    mProcessCall( &FilterThread::RunProcess, this ),
    mPostProcessCall( &FilterThread::OnPostProcess, this ) {}
  virtual ~FilterThread() {}

  void AddChannel( int ch ) { mChannels.push_back( ch ); }
  void Wait() { ReusableThread::Wait(); }

  void Publish() const;
  void Preflight( const SignalProperties&, SignalProperties& ) const;
  void Initialize( const SignalProperties&, const SignalProperties& );
  void Process( const GenericSignal&, GenericSignal& );
  void PostProcess();
  void StartRun();
  void StopRun();

 protected:
  typedef std::vector<int> ChannelList;
  const ChannelList& Channels() const { return mChannels; }

  virtual void OnPublish() const = 0;
  virtual void OnPreflight( const SignalProperties&, SignalProperties& ) const = 0;
  virtual void OnInitialize( const SignalProperties&, const SignalProperties& ) = 0;
  virtual void OnProcess( const GenericSignal&, GenericSignal& ) = 0;
  virtual void OnPostProcess() {}
  virtual void OnStartRun() {}
  virtual void OnStopRun() {}

 private:
  void Run( Runnable& );
  void RunProcess();

  MemberCall<void( FilterThread* )> mProcessCall;
  MemberCall<void( FilterThread* )> mPostProcessCall;

  const GenericSignal* mpInput;
  GenericSignal* mpOutput;
  ChannelList mChannels;
};

template<typename T>
class ThreadedFilter : public GenericFilter
{
 public:
  ThreadedFilter();
  ~ThreadedFilter();
  void Preflight( const SignalProperties&, SignalProperties& ) const;
  void Initialize( const SignalProperties&, const SignalProperties& );
  void Process( const GenericSignal&, GenericSignal& );
  void StartRun();
  void StopRun();

 private:
  void Cleanup();
  std::vector<T*> mThreads;
};

template<typename T>
ThreadedFilter<T>::ThreadedFilter()
{
  T().Publish();
}

template<typename T>
ThreadedFilter<T>::~ThreadedFilter()
{
  Cleanup();
}

template<typename T>
void
ThreadedFilter<T>::Preflight( const SignalProperties& Input, SignalProperties& Output ) const
{
  T().Preflight( Input, Output );
  OptionalParameter( "NumberOfThreads" );
}

template<typename T>
void
ThreadedFilter<T>::Initialize( const SignalProperties& Input, const SignalProperties& Output )
{
  Cleanup();
  int numberOfThreads = OptionalParameter( "NumberOfThreads", -1 );
  if( numberOfThreads <= 0 )
    numberOfThreads = ThreadUtils::NumberOfProcessors();
  mThreads.resize( std::min( Input.Channels(), numberOfThreads ) );
  for( size_t i = 0; i < mThreads.size(); ++i )
    mThreads[i] = new T;
  for( int i = 0; i < Input.Channels(); ++i )
    mThreads[i % mThreads.size()]->AddChannel( i );
  for( size_t i = 0; i < mThreads.size(); ++i )
    mThreads[i]->Initialize( Input, Output );
}

template<typename T>
void
ThreadedFilter<T>::Process( const GenericSignal& Input, GenericSignal& Output )
{
  for( size_t i = 0; i < mThreads.size(); ++i )
  {
    mThreads[i]->Wait();
    mThreads[i]->Process( Input, Output );
  }
  for( size_t i = 0; i < mThreads.size(); ++i )
  {
    mThreads[i]->Wait();
    mThreads[i]->PostProcess();
  }
}

template<typename T>
void
ThreadedFilter<T>::StartRun()
{
  for( size_t i = 0; i < mThreads.size(); ++i )
    mThreads[i]->StartRun();
}

template<typename T>
void
ThreadedFilter<T>::StopRun()
{
  for( size_t i = 0; i < mThreads.size(); ++i )
  {
    mThreads[i]->Wait();
    mThreads[i]->StopRun();
  }
}

template<typename T>
void
ThreadedFilter<T>::Cleanup()
{
  for( size_t i = 0; i < mThreads.size(); ++i )
    delete mThreads[i];
  mThreads.clear();
}

#endif // THREADED_FILTER_H
