////////////////////////////////////////////////////////////////////////////////
// $Id: FieldTripBufferADC.cpp 4982 2015-08-26 20:01:03Z mellinger $
// Author: sk@sklanke.de
// Description: A source class that interfaces to the FieldTrip buffer.
// State/event handling is largely copy-pasted from the FieldTripBuffer filter,
// ADC mechanism largely copy-pasted from other ADC modules.
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
#include "FieldTripBufferADC.h"
#include "GenericSignal.h"
#include "BCIStream.h"
#include <string>
#include <sstream>

using namespace std;

// Register the source class with the framework.
RegisterFilter( FieldTripBufferADC, 1 );


// **************************************************************************
// Function:   Constructor of the FieldTripBufferADC class.
// Purpose:    Setup parameter definitions
// Parameters: N/A
// Returns:    N/A
// **************************************************************************
FieldTripBufferADC::FieldTripBufferADC()
{
 BEGIN_PARAMETER_DEFINITIONS
    "Source string HostName= localhost"
            " // the name of the host to connect to",
	"Source int Port= 1972 1972 1"
			" // the port number to connect to",
	"Source int SampleBlockSize= 512 512 1"
			" // the block size to use", 
	"Source stringlist EventStates= 0 % % % // States to be read as events from buffer",			
 END_PARAMETER_DEFINITIONS
 mSocket = -1;
}

// **************************************************************************
// Function:   Destructor of the FieldTripBufferADC class.
// Purpose:    Just calls halt to close the connection.
// Parameters: N/A
// Returns:    N/A
// **************************************************************************
FieldTripBufferADC::~FieldTripBufferADC()
{
  Halt();
}



// **************************************************************************
// Function:   readHeader
// Purpose:    This function is called during Preflight and Initialize in
//             order to retrieve basic information about the data format
//             in the buffer.
// Parameters: Socket (opened connection to FT buffer) and headerdef_t object
//             that the information is written to.
// Returns:    true on success, false on errors
// **************************************************************************
bool 
FieldTripBufferADC::readHeader(int sock, headerdef_t &hdr) const
{
	FtBufferRequest req;
	FtBufferResponse resp;

	req.prepGetHeader();
	
	int r = clientrequest(sock, req.out(), resp.in());

	if (r!=0) return false;

	return resp.checkGetHeader(hdr);
}



// **************************************************************************
// Function:   Preflight
// Purpose:    Check basic information about the data format in the buffer
//             and configure the states (~events) we can modify.
// Parameters: References to input and output signal properties.
// Returns:    N/A
// **************************************************************************
void
FieldTripBufferADC::Preflight( const SignalProperties&,
                               SignalProperties& outSignalProperties ) const
{
  // Resource availability and parameter consistency checks.
  int pfSocket;
  int numInputChannels = 0;

  pfSocket = ::open_connection( Parameter( "HostName" ).c_str(), Parameter( "Port") );
  if( pfSocket < 0 ) 
  {
    bcierr << "Cannot establish a connection to the FieldTrip buffer" << endl;
  } 
  else
  {
    headerdef_t hdr;
    if (!readHeader(pfSocket, hdr))
	  bcierr << "Error while trying to read header from FT buffer" << endl;
	else
	{
		SignalType::Type sType;
	
		switch(hdr.data_type) {
			case DATATYPE_INT32:
				sType = SignalType::int32;
				break;
			case DATATYPE_INT16:
				sType = SignalType::int16;
				break;
			case DATATYPE_FLOAT32:
				sType = SignalType::float32;
				break;
			default:
				bcierr << "Datatype not supported yet" << endl;
				return;
		}

		if( Parameter( "SourceCh" ) != hdr.nchans )
          bcierr << "The number of channels in the FT buffer does not match the SourceCh parameter" << endl;

    size_t sampleBlockSize = Parameter( "SampleBlockSize" );
        
		outSignalProperties = SignalProperties(hdr.nchans, sampleBlockSize, sType);
	}
	::close_connection(pfSocket);
  }
  
  StateSet eventStates;
  ConfigureStateSet( eventStates );
  for( StateSet::const_iterator i = eventStates.begin(); i != eventStates.end(); ++i )
    State( *i );
}



// **************************************************************************
// Function:   Initialize
// Purpose:    Update sample and event counts, configure states.
// Parameters: References to input (ignored) and output signal properties.
// Returns:    N/A
// **************************************************************************
void
FieldTripBufferADC::Initialize( const SignalProperties&, const SignalProperties& )
{

  mSocket = ::open_connection( Parameter( "HostName" ).c_str(), Parameter( "Port") );
  if( mSocket < 0 ) 
  {
    bcierr << "Cannot establish a connection to the FieldTrip buffer" << endl;
  } 
  else 
  {
	headerdef_t hdr;
	if (!readHeader(mSocket, hdr))
	  bcierr << "Error while trying to read header from FT buffer" << endl;
	else
	{
		mSampleFreq  = hdr.fsample;
		mNumChannels = hdr.nchans;
		mDataType    = hdr.data_type;
		mNumSamplesRead = hdr.nsamples;
		mNumEventsRead  = hdr.nevents;
	}
  }
  
  ConfigureStateSet( mEventStates );
  for( StateSet::const_iterator i = mEventStates.begin(); i != mEventStates.end(); ++i )
  {
    bcidbg( 2 ) << "State " << *i << " will be read from buffer events" << endl;
  }
  isRunning = 0;  
}




// **************************************************************************
// Function:   Process
// Purpose:    This function is called within the data acquisition loop to
//             fill the output signal with values. It blocks until the desired
//             amount of new samples is actually present in the FT buffer.
// Parameters: References to input signal (ignored) and output signal.
// Returns:    N/A
// **************************************************************************
void
FieldTripBufferADC::Process( const GenericSignal&, GenericSignal& Output )
{
  if (mSocket < 0)
  {
	bcierr << "No connection to FieldTrip buffer" << endl;
    return;
  }
  

  FtBufferRequest req;
  FtBufferResponse resp;
  unsigned int nSamp, nEvt;
  datadef_t datadef;
  int blockSize = Output.Elements();
  int timeout = isRunning ? 5000 : 20000; // wait 20 seconds for the first block

  req.prepWaitData(mNumSamplesRead + blockSize, 0xFFFFFFFF, 5000);
  int r = clientrequest(mSocket, req.out(), resp.in());
  
  if (r!=0 || !resp.checkWait(nSamp, nEvt)) 
  {
	bcierr << "Error while polling for data from FT buffer" << endl;
	return;
  }

  if (nSamp < mNumSamplesRead + blockSize) 
  {
	bcierr << "Timeout for reading data from FT buffer" << endl;
	return;
  }

  req.prepGetData(mNumSamplesRead, mNumSamplesRead + blockSize -1);
  r = clientrequest(mSocket, req.out(), resp.in());
  if (r!=0 || !resp.checkGetData(datadef, &databuf)) 
  {
	bcierr << "Error while reading data from FT buffer" << endl;
	return;
  }

  if (datadef.nchans != mNumChannels || datadef.nsamples != blockSize)
  {
	bcierr << "Invalid shape of data returned from FT buffer";
	return;
  }
  
  isRunning = 1;

  switch(datadef.data_type) {
	  case DATATYPE_INT16:
		{
		  INT16_T *src = (INT16_T *) databuf.data();

	       for( int sample = 0; sample < Output.Elements(); ++sample )
            for( int channel = 0; channel < Output.Channels(); ++channel )
              Output( channel, sample ) = *src++;
        }
	    break;
	  case DATATYPE_INT32:
		{
		  INT32_T *src = (INT32_T *) databuf.data();

	       for( int sample = 0; sample < Output.Elements(); ++sample )
            for( int channel = 0; channel < Output.Channels(); ++channel )
              Output( channel, sample ) = *src++;
		  }
		break;
	  case DATATYPE_FLOAT32:
		{
		  float *src = (float *) databuf.data();

		  for( int sample = 0; sample < Output.Elements(); ++sample )
            for( int channel = 0; channel < Output.Channels(); ++channel )
              Output( channel, sample ) = *src++;
		}
        break;
  }
  
  mNumSamplesRead += blockSize;
  
  if (nEvt <= mNumEventsRead) return;
  
  req.prepGetEvents(mNumEventsRead, nEvt-1);
  r = clientrequest(mSocket, req.out(), resp.in());
  if (r!=0 || !resp.checkGetEvents(&databuf)) 
  {
	  bcierr << "Error while reading events from FT buffer" << endl;
	  return;
  }
  
  mNumEventsRead = nEvt;
  
  const char *evbuf = reinterpret_cast<const char*> (databuf.data());
  int offset = 0;
  while (offset + sizeof(eventdef_t) <= databuf.size()) {
	const eventdef_t *evdef = (eventdef_t *) (evbuf + offset);
	if (evdef->type_type == DATATYPE_CHAR) {
	  string type = string( evbuf + offset + sizeof(eventdef_t), evdef->type_numel );	  
      StateSet::const_iterator i = mEventStates.find( type );
      if( i != mEventStates.end() )
      {
        // the event value is assigned to the state value
        const char *p = evbuf + offset + sizeof(eventdef_t) + evdef->type_numel;
        State::ValueType value = 0;
        switch( evdef->value_type )
        {
          case DATATYPE_UINT8:
            value = *reinterpret_cast<const UINT8_T*>( p );
            break;
          case DATATYPE_UINT16:
            value = *reinterpret_cast<const UINT16_T*>( p );
            break;
          case DATATYPE_UINT32:
            value = *reinterpret_cast<const UINT32_T*>( p );
            break;
          case DATATYPE_INT8:
            value = *reinterpret_cast<const INT8_T*>( p );
            break;
          case DATATYPE_INT16:
            value = *reinterpret_cast<const INT16_T*>( p );
            break;
          case DATATYPE_INT32:
            value = *reinterpret_cast<const INT32_T*>( p );
            break;
          case DATATYPE_FLOAT32:
            value = static_cast<State::ValueType>( *reinterpret_cast<const FLOAT32_T*>( p ) );
            break;
          case DATATYPE_FLOAT64:
            value = static_cast<State::ValueType>( *reinterpret_cast<const FLOAT64_T*>( p ) );
            break;
          default:
            bcierr << "Unexpected value_type in " << type << " event" << endl;
        }
        State( *i ) = value;
        bcidbg( 2 ) << "Set state " << *i << "=" << value << " from buffer" << endl;
      } else {
        bciwarn << "Ignoring event with type '" << type << "'" << endl;
	  }
	}
	// proceed to next event
	offset += sizeof(eventdef_t) + evdef->bufsize;
  }
}

void
FieldTripBufferADC::ConfigureStateSet( StateSet& stateSet ) const
{
  stateSet.clear();
  for( int i = 0; i < Parameter( "EventStates" )->NumValues(); ++i )
  {
    string state = Parameter( "EventStates" )( i );
    stateSet.insert( state );
  }
}


void
FieldTripBufferADC::Halt()
{
	if (mSocket > 0)
		::close_connection(mSocket);
	mSocket = -1;	
}


