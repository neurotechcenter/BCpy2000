////////////////////////////////////////////////////////////////////////////////
// $Id: FieldTripBufferFilter.cpp 5384 2016-05-31 13:31:54Z mellinger $
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
#include "PCHIncludes.h"
#pragma hdrstop

#include "FieldTripBufferFilter.h"
#include "ClassName.h"

using namespace std;

RegisterFilter( FieldTripBufferFilter, 2.B );

FieldTripBufferFilter::FieldTripBufferFilter()
: mBlockCount( 0 ),
  mpDataBuffer( NULL ),
  mpEventBuffer( NULL )
{
  pthread_attr_init( &mThreadAttr );
  pthread_attr_setdetachstate( &mThreadAttr, PTHREAD_CREATE_JOINABLE );

  BEGIN_PARAMETER_DEFINITIONS
   "Filtering:FieldTrip%20Buffer string FTBufferAddress= localhost:1972 % % % // Buffer listening address",
   "Filtering:FieldTrip%20Buffer matrix FTInitialOutput= 1 1 0 0 % % // Initial filter output (rows map to channels, columns map to elements)",
   "Filtering:FieldTrip%20Buffer string FTOutputEventType= Signal % % % // Event type corresponding to output signal",
   "Filtering:FieldTrip%20Buffer stringlist FTStatesToBuffer= 5 * -Running -Recording -SourceTime -StimulusTime % % % // States to be written as events to buffer (* to indicate any state; leading - to exclude; FTStatesFromBuffer are always excluded)",
   "Filtering:FieldTrip%20Buffer stringlist FTStatesFromBuffer= 0 % % % // States to be read as events from buffer",
  END_PARAMETER_DEFINITIONS

  mFTBufferAddress = string( Parameter( "FTBufferAddress" ) );
  ParseHostAddress( mFTBufferAddress, mHostAddress );
  int result = pthread_create( &mServerThread, &mThreadAttr, tcpserver, &mHostAddress );
  if( 0 != result )
    bcierr << "Could not create buffer server thread" << endl;
}


FieldTripBufferFilter::~FieldTripBufferFilter()
{
  delete[] mpDataBuffer;
  delete[] mpEventBuffer;
  pthread_attr_destroy( &mThreadAttr );
}


void
FieldTripBufferFilter::Preflight( const SignalProperties& Input,
                                        SignalProperties& Output ) const
{
  PreflightCondition( WORDSIZE_CHAR        == 1  );
  PreflightCondition( WORDSIZE_UINT8       == 1  );
  PreflightCondition( WORDSIZE_UINT16      == 2  );
  PreflightCondition( WORDSIZE_UINT32      == 4  );
  PreflightCondition( WORDSIZE_UINT64      == 8  );
  PreflightCondition( WORDSIZE_INT8        == 1  );
  PreflightCondition( WORDSIZE_INT16       == 2  );
  PreflightCondition( WORDSIZE_INT32       == 4  );
  PreflightCondition( WORDSIZE_INT64       == 8  );
  PreflightCondition( WORDSIZE_FLOAT32     == 4  );
  PreflightCondition( WORDSIZE_FLOAT64     == 8  );
  PreflightCondition( sizeof(messagedef_t) == 8  );
  PreflightCondition( sizeof(headerdef_t)  == 24 );
  PreflightCondition( sizeof(datadef_t)    == 16 );
  PreflightCondition( sizeof(eventdef_t)   == 32 );
  PreflightCondition( sizeof(message_t)    == 8  );
  PreflightCondition( sizeof(header_t)     == 8  );
  PreflightCondition( sizeof(data_t)       == 8  );
  PreflightCondition( sizeof(event_t)      == 8  );
  PreflightCondition( sizeof(datasel_t)    == 8  );
  PreflightCondition( sizeof(eventsel_t)   == 8  );

  if( Parameter( "FTBufferAddress" ) != mFTBufferAddress )
    bcierr << "Cannot change FTBufferAddress after startup. "
           << "Please use --FTBufferAddress=<address:port> to specify "
           << "the buffer's address at startup"
           << endl;

  StateSet statesToBuffer, statesFromBuffer;
  ConfigureStateSets( statesToBuffer, statesFromBuffer );
  for( StateSet::const_iterator i = statesToBuffer.begin(); i != statesToBuffer.end(); ++i )
    State( *i );
  for( StateSet::const_iterator i = statesFromBuffer.begin(); i != statesFromBuffer.end(); ++i )
    State( *i );

  Output = SignalProperties( 0, 0 );
  if( Parameter( "FTOutputEventType" ) != "" )
    Output = SignalProperties(
      Parameter( "FTInitialOutput" )->NumRows(),
      Parameter( "FTInitialOutput" )->NumColumns(),
      SignalType::float32
    );
}


void
FieldTripBufferFilter::Initialize( const SignalProperties& Input,
                                   const SignalProperties& Output )
{
  int numChannels = Input.Channels(),
      numSamples  = Input.Elements();

  delete[] mpDataBuffer;
  mpDataBuffer = new float[ numChannels * numSamples ];

  mDatadef.nchans    = numChannels;
  mDatadef.nsamples  = numSamples;
  mDatadef.data_type = DATATYPE_FLOAT32;
  mDatadef.bufsize   = numChannels * numSamples * sizeof( float );

  mHeaderdef.nchans    = numChannels;
  mHeaderdef.nsamples  = numSamples;
  mHeaderdef.nevents   = 0;
  mHeaderdef.fsample   = static_cast<FLOAT32_T>( Input.SamplingRate() );
  mHeaderdef.data_type = DATATYPE_FLOAT32;
  mHeaderdef.bufsize   = 0;

  ConfigureStateSets( mFTStatesToBuffer, mFTStatesFromBuffer );
  delete[] mpEventBuffer;
  size_t bufferLength = 0;
  for( StateSet::const_iterator i = mFTStatesToBuffer.begin(); i != mFTStatesToBuffer.end(); ++i )
  {
    bufferLength = max( bufferLength, i->length() );
    bcidbg( 2 ) << "State " << *i << " will be written to buffer" << endl;
  }
  for( StateSet::const_iterator i = mFTStatesFromBuffer.begin(); i != mFTStatesFromBuffer.end(); ++i )
  {
    bufferLength = max( bufferLength, i->length() );
    bcidbg( 2 ) << "State " << *i << " will be read from buffer" << endl;
  }
  bufferLength += sizeof( INT32_T );
  mpEventBuffer = new UINT8_T[ bufferLength ];

  mSignalBuffer = GenericSignal( 0, 0 );
  mFTOutputEventType = string( Parameter( "FTOutputEventType" ) );
  if( !mFTOutputEventType.empty() )
    mSignalBuffer = GenericSignal( Output );
}

void
FieldTripBufferFilter::StartRun()
{
  mPreviousStateValues.clear();
  for( StateSet::const_iterator i = mFTStatesToBuffer.begin(); i != mFTStatesToBuffer.end(); ++i )
  {
    PutStateToBuffer( *i, 0 );
    mPreviousStateValues[ *i ] = State( *i )( 0 );
  }

  if( !mFTOutputEventType.empty() )
    for( int ch = 0; ch < mSignalBuffer.Channels(); ++ch )
      for( int el = 0; el < mSignalBuffer.Elements(); ++el )
        mSignalBuffer( ch, el ) = Parameter( "FTInitialOutput" )( ch, el );

  mBlockCount = 0;
  DeleteMessage( SendRequest( FLUSH_HDR, &mHeaderdef ) );  // this removes the previous header, and also any data and events in the buffer
  DeleteMessage( SendRequest( PUT_HDR,   &mHeaderdef ) );
}

void
FieldTripBufferFilter::StopRun()
{
  // nothing needs to be done here
  // the buffer remains accessible, with header+data+events of the previous run
}

void
FieldTripBufferFilter::Process( const GenericSignal& Input, GenericSignal& Output )
{
  // Put signal data into buffer.
  int numChannels = Input.Channels(),
      numSamples = Input.Elements();
  for( int channel = 0; channel < numChannels; ++channel )
    for( int sample = 0; sample < numSamples; ++sample )
      mpDataBuffer[ sample * numChannels + channel ] = static_cast<float>( Input( channel, sample ) );
  DeleteMessage( SendRequest( PUT_DAT, &mDatadef, mpDataBuffer ) );

  // Process events present in the buffer.
  message_t* response = SendRequest( GET_EVT );
  if( response && response->def->command == GET_OK )
  {
    event_t event = { NULL, NULL };
    size_t offset = 0;
    while( offset < response->def->bufsize )
    {
      UINT8_T* p = reinterpret_cast<UINT8_T*>( response->buf );
      event.def = reinterpret_cast<eventdef_t*>( p + offset );
      event.buf = p + offset + sizeof( eventdef_t );
      ProcessEvent( event );
      offset += sizeof( eventdef_t ) + event.def->bufsize;
    }
  }
  DeleteMessage( response );
  
  // Write state values to buffer
  for( StateSet::const_iterator i = mFTStatesToBuffer.begin(); i != mFTStatesToBuffer.end(); ++i )
    // Only if they have changed relative to previous value
    for( int s = 0; s < Statevector->Samples(); ++s )
      if( State( *i )( s ) != mPreviousStateValues[*i] )
      {
        PutStateToBuffer( *i, s );
        mPreviousStateValues[*i] = State( *i )( s );
      }

  ++mBlockCount;
  if( mSignalBuffer.Properties().IsEmpty() )
    Output = Input;
  else
    Output = mSignalBuffer;
}

void
FieldTripBufferFilter::ConfigureStateSets( StateSet& ioToBufferSet, StateSet& ioFromBufferSet ) const
{
  ioToBufferSet.clear();
  ioFromBufferSet.clear();
  StateSet dontWrite;
  for( int i = 0; i < Parameter( "FTStatesToBuffer" )->NumValues(); ++i )
  {
    string state = Parameter( "FTStatesToBuffer" )( i );
    if( state == "*" )
      for( const auto& state : *States )
        ioToBufferSet.insert( state.Name() );
    else if( !state.empty() && string( "!~-" ).find( state[ 0 ] ) != string::npos )
      dontWrite.insert( state.substr( 1 ) );
    else
      ioToBufferSet.insert( state );
  }
  for( int i = 0; i < Parameter( "FTStatesFromBuffer" )->NumValues(); ++i )
  {
    string state = Parameter( "FTStatesFromBuffer" )( i );
    ioFromBufferSet.insert( state );
    dontWrite.insert( state );
  }
  for( StateSet::const_iterator i = dontWrite.begin(); i != dontWrite.end(); ++i )
    ioToBufferSet.erase( *i );
}

void
FieldTripBufferFilter::ProcessEvent( const event_t& inEvent )
{
  // the event type must be represented as CHAR
  if( inEvent.def->type_type == DATATYPE_CHAR )
  {
    string type = string( reinterpret_cast<const char*>( inEvent.buf ), inEvent.def->type_numel );
    StateSet::const_iterator i = mFTStatesFromBuffer.find( type );
    if( i != mFTStatesFromBuffer.end() )
    {
      // the event value is assigned to the state value
      UINT8_T* p = reinterpret_cast<UINT8_T*>( inEvent.buf ) + inEvent.def->type_numel;
      State::ValueType value = 0;
      switch( inEvent.def->value_type )
      {
        case DATATYPE_UINT8:
          value = *reinterpret_cast<UINT8_T*>( p );
          break;
        case DATATYPE_UINT16:
          value = *reinterpret_cast<UINT16_T*>( p );
          break;
        case DATATYPE_UINT32:
          value = *reinterpret_cast<UINT32_T*>( p );
          break;
        case DATATYPE_INT8:
          value = *reinterpret_cast<INT8_T*>( p );
          break;
        case DATATYPE_INT16:
          value = *reinterpret_cast<INT16_T*>( p );
          break;
        case DATATYPE_INT32:
          value = *reinterpret_cast<INT32_T*>( p );
          break;
        case DATATYPE_FLOAT32:
          value = static_cast<State::ValueType>( *reinterpret_cast<FLOAT32_T*>( p ) );
          break;
        case DATATYPE_FLOAT64:
          value = static_cast<State::ValueType>( *reinterpret_cast<FLOAT64_T*>( p ) );
          break;
        default:
          bcierr << "Unexpected value_type in " << type << " event" << endl;
      }
      State( *i ) = value;
      bcidbg( 2 ) << "Set state " << *i << "=" << value << " from buffer" << endl;
    }
    else if( !type.empty() && type == mFTOutputEventType )
    {
      size_t numEntries = mSignalBuffer.Channels() * mSignalBuffer.Elements();
      if( inEvent.def->value_numel != numEntries )
        bcierr << "Number of entries in " << mFTOutputEventType
               << " event must match the total number of entries in the"
               << " FTInitialOutput matrix"
               << endl;
      else switch( inEvent.def->value_type )
      {
        case DATATYPE_UINT8:
          EventToSignal<UINT8_T>( inEvent, mSignalBuffer );
          break;
        case DATATYPE_UINT16:
          EventToSignal<UINT16_T>( inEvent, mSignalBuffer );
          break;
        case DATATYPE_UINT32:
          EventToSignal<UINT32_T>( inEvent, mSignalBuffer );
          break;
        case DATATYPE_INT8:
          EventToSignal<INT8_T>( inEvent, mSignalBuffer );
          break;
        case DATATYPE_INT16:
          EventToSignal<INT16_T>( inEvent, mSignalBuffer );
          break;
        case DATATYPE_INT32:
          EventToSignal<INT32_T>( inEvent, mSignalBuffer );
          break;
        case DATATYPE_FLOAT32:
          EventToSignal<FLOAT32_T>( inEvent, mSignalBuffer );
          break;
        case DATATYPE_FLOAT64:
          EventToSignal<FLOAT64_T>( inEvent, mSignalBuffer );
          break;
        default:
          bcierr << "Unexpected value_type in " << type << " event" << endl;
      }
    }
  }
  else
    bcierr << "Unexpected type_type in event" << endl;
}

void
FieldTripBufferFilter::PutStateToBuffer( const std::string& inName, int inSample )
{
  // this creates an event from a state and write the event to the buffer
  // the event type is always represented as CHAR and the event value is always represented as INT32
  eventdef_t eventdef;
  eventdef.type_type   = DATATYPE_CHAR;
  eventdef.type_numel  = inName.length();
  eventdef.value_type  = DATATYPE_INT32;
  eventdef.value_numel = 1;
  eventdef.offset      = 0;
  eventdef.sample      = mBlockCount * mHeaderdef.nsamples + inSample;
  eventdef.bufsize     = eventdef.type_numel + WORDSIZE_INT32;
  ::memcpy( mpEventBuffer, inName.data(), eventdef.type_numel );
  INT32_T& value = *reinterpret_cast<INT32_T*>( mpEventBuffer + eventdef.type_numel );
  value = State( inName )( inSample );
  bcidbg( 2 ) << "Put state " << inName << "==" << value << " into buffer" << endl;
  DeleteMessage( SendRequest( PUT_EVT, &eventdef, mpEventBuffer ) );
}

template<typename T>
void
FieldTripBufferFilter::EventToSignal( const event_t& inEvent, GenericSignal& outSignal ) const
{
  // FieldTrip does not write data back to BCI2000 but rather uses events to communicate
  // therefore the input of this function is an event, whose value should be used as the output signal
  UINT8_T* p = reinterpret_cast<UINT8_T*>( inEvent.buf );
  const T* data = reinterpret_cast<const T*>( p + inEvent.def->type_numel );
  for( size_t i = 0; i < inEvent.def->value_numel; ++i )
  {
    int channel = i % outSignal.Channels(),
        element = i / outSignal.Channels();
    outSignal( channel, element ) = data[ i ];
  }
}

template<typename T>
message_t*
FieldTripBufferFilter::SendRequest( int inCommand, T* inDef, void* inData ) const
{
  messagedef_t requestdef;
  requestdef.version = VERSION;
  requestdef.command = inCommand;
  requestdef.bufsize = 0;
  message_t  request = { &requestdef, NULL },
           * response = NULL;
  if( inDef != NULL )
  {
    requestdef.bufsize = append( &request.buf, requestdef.bufsize, inDef, sizeof( T ) );
    if( inData != NULL )
      requestdef.bufsize = append( &request.buf, requestdef.bufsize, inData, inDef->bufsize );
  }
  
  if( 0 != dmarequest( &request, &response ) )
    bcierr << "Could not send request of type " << ClassName( typeid( *inDef ) ) << endl;

  if( request.buf != NULL )
    ::free( request.buf );
  return response;
}

void
FieldTripBufferFilter::DeleteMessage( message_t* inMessage ) const
{
  if( inMessage != NULL )
  {
    if( inMessage->def != NULL )
      ::free( inMessage->def );
    if( inMessage->buf != NULL )
      ::free( inMessage->buf );
    ::free( inMessage );
  }
}

void
FieldTripBufferFilter::ParseHostAddress( const string& inString, host_t& outHostAddress ) const
{
  const char cSeparator = ':';
  string address = inString;
  size_t separatorPos = address.rfind( cSeparator );
  if( separatorPos == string::npos )
  {
    address += ":1972";
    separatorPos = address.rfind( cSeparator );
  }
  outHostAddress.port = ::atoi( address.substr( separatorPos + 1 ).c_str() );
  ::strncpy( outHostAddress.name, address.substr( 0, separatorPos ).c_str(), sizeof( outHostAddress.name ) );
}

