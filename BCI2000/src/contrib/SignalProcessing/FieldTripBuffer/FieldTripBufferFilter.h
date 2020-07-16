////////////////////////////////////////////////////////////////////////////////
// $Id: FieldTripBufferFilter.h 4539 2013-08-07 10:27:06Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A filter that maintains a FieldTrip Realtime buffer, writes
//   its input data into that buffer, and optionally gets its output data from
//   buffered events. BCI2000 state variables are mapped to buffer events.
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
#ifndef FIELDTRIP_BUFFER_FILTER_H
#define FIELDTRIP_BUFFER_FILTER_H

#include "buffer.h"
#include <pthread.h>
#include <set>
#include <map>
#include <string>

#include "GenericFilter.h"

class FieldTripBufferFilter : public GenericFilter
{
 public:
          FieldTripBufferFilter();
  virtual ~FieldTripBufferFilter();
  virtual void Preflight( const SignalProperties&, SignalProperties& ) const;
  virtual void Initialize( const SignalProperties&, const SignalProperties& );
  virtual void StartRun();
  virtual void StopRun();
  virtual void Process( const GenericSignal& Input, GenericSignal& Output );

 private:
  typedef std::set<std::string> StateSet;
  typedef std::map<std::string, int> StateValueMap;
  void ConfigureStateSets( StateSet& ioToBufferSet, StateSet& ioFromBufferSet ) const;
  void ProcessEvent( const event_t& );
  void PutStateToBuffer( const std::string&, int );
  template<typename T> void EventToSignal( const event_t&, GenericSignal& ) const;
  template<typename T> message_t* SendRequest( int, T*, void* = NULL ) const;
  message_t* SendRequest( int type ) const
             { return SendRequest<messagedef_t>( type, NULL ); }
  void DeleteMessage( message_t* ) const;
  void ParseHostAddress( const std::string&, host_t& ) const;

  std::string    mFTBufferAddress;

  GenericSignal  mSignalBuffer;
  std::string    mFTOutputEventType;
  StateSet       mFTStatesToBuffer,
                 mFTStatesFromBuffer;
  StateValueMap  mPreviousStateValues;

  unsigned long  mBlockCount;
  host_t         mHostAddress;
  float*         mpDataBuffer;
  UINT8_T*       mpEventBuffer;
  datadef_t      mDatadef;
  headerdef_t    mHeaderdef;

  pthread_attr_t mThreadAttr;
  pthread_t      mServerThread;
};

#endif // FIELDTRIP_BUFFER_FILTER_H


