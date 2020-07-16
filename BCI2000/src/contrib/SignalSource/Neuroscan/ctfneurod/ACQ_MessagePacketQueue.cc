//	$Header: /home/qc10/release4.18-AcqSW/code_example/RCS/ACQ_MessagePacketQueue.cc,v 22.1 2004/01/13 19:32:55 dougmck Exp $

#include <iostream>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/fcntl.h>

#include <sys/msg.h>
#include <sys/shm.h>

#include "ACQ_MessagePacketQueue.h"

/**
	Constructor
*/
	 ACQ_MessagePacketQueue::ACQ_MessagePacketQueue(  )
:
		_setupCBProcedure( 0 ),
		_setupCBData( 0 ),
		_receiveDataCBProcedure( 0 ),
		_receiveDataCBData( 0 ),
		_closeCBProcedure( 0 ),
		_closeCBData( 0 )

{
	init();
}

/**
	Destructor
*/
	 ACQ_MessagePacketQueue::~ACQ_MessagePacketQueue(  )
{
	if ( _msgQid > 0 ) close();
}

/**
	Initialize the shared memory to receive data from a sensder.
	
	The shared data segment is created large enough to hold 
	ACQ_MSGQ_SIZE ACQ message packets. This is accessed as an array .
	
	\postcondition	message queue is queated in shared memory
	
	\postcondition	message type for all packects in message queue are set
					to invalid ( i.e. packets do not ontain any data 
	
	\postcondition	message queue index is set to zero
	
	\exception	The program will exit if the shared memory cannot be created.
*/		
void ACQ_MessagePacketQueue::init(  )
{
	
	_msgQkey = ACQ_MESQ_SHMKEY;
	
	//
	//	Create the shared memory for the data queue
	//
	errno = 0;
	_msgQid = shmget( _msgQkey, sizeof( ACQ_MessagePacket ) * ACQ_MSGQ_SIZE, 0666 | IPC_CREAT );

	if ( errno != 0 )
	{
		printf( "shmget failed: error = %s\n", strerror( errno ));
		exit ( errno );
	}

	//
	//	Attach to the shared memory and get the address of
	//	the data queue
	//
	_msgQAddr = ( ACQ_MessagePacket *)shmat( _msgQid, 0, 0 ); // open pipe for read/write,blocking
	if ( errno != 0 )
	{
		printf( "open failed: error = %s\n", strerror( errno ));
		exit( errno );
	}
	
	//
	//	Initialize the type flags of the packets in shared memory to indicate
	//	that the packets do not contain any data
	//
	for ( _msgqIdx = 0; _msgqIdx < ACQ_MSGQ_SIZE; _msgqIdx++ )
	{
		_msgQAddr[ _msgqIdx ].message_type =  ACQ_MSGQ_INVALID;	
	}
	_msgqIdx = 0;
	_isRecording = false;
}

/**
	Close the shared memory .
	
	The shared data segment is detached and marked for removal 
*/		
void ACQ_MessagePacketQueue::close(  )
{
	shmdt( _msgQAddr );
	shmctl( _msgQid, IPC_RMID, 0 );
	_msgQid = -1;	
	_isRecording = false;
}

/**
	Setup collection callback
	
	Called when a ACQ_MSGQ_SETUP_COLLECTION message type packet is received.
	This method should initialize and components needed to monitor the data
	during acquisition
*/
void	ACQ_MessagePacketQueue::setupCollectionCallback( ACQ_MessagePacket & messagePacket )
{
	//
	//	Call the user supplied callback if defined
	//
	if ( _setupCBProcedure != 0 )
	{
		(*_setupCBProcedure)( messagePacket, _setupCBData );
	} 

	
	//
	//	Release the packet and set the index to the next data packet
	//
	messagePacket.message_type = ACQ_MSGQ_INVALID;
	_msgqIdx = 1; 
	_isRecording = true;
}


/**
	receive data callback
	
	Called when a ACQ_MSGQ_DATA message type packet is received.
	This method should parse the incoming data as required and 
	display or process the selected data
*/
void	ACQ_MessagePacketQueue::receiveDataCallback( ACQ_MessagePacket & messagePacket)
{
	//
	//	Call the user supplied callback if defined
	//
	if ( _receiveDataCBProcedure != 0 )
	{
		(*_receiveDataCBProcedure)( messagePacket, _receiveDataCBData );
	} 

	
	//
	//	Release the packet and set the index to the next data packet
	//
	messagePacket.message_type = ACQ_MSGQ_INVALID;
	_msgqIdx++;
	if ( _msgqIdx >= ACQ_MSGQ_SIZE ) _msgqIdx = 0;
}



/**
	Close collection callback
	
	Called when a ACQ_MSGQ_CLOSE_COLLECTION message type packet is received.
	This method should clean up any compnents created by the setupCollectionCallback
	
	\postcondition	The packet is released ( message type set to ACQ_MSGQ_INVALID )
	\postcondition	The message packet index is set to zero ( so that next read reads the 
					dataset path from first packet )
*/
void	ACQ_MessagePacketQueue::closeCollectionCallback( ACQ_MessagePacket & messagePacket )
{
	//
	//	Call the user supplied callback if defined
	//
	if ( _closeCBProcedure != 0 )
	{
		(*_closeCBProcedure)( messagePacket, _closeCBData );
	} 

	
	//
	//	Release the packet and set the index to the next data packet
	//	Resets index bak to begining of message packet array
	//
	_msgQAddr[ _msgqIdx ].message_type = ACQ_MSGQ_INVALID;
	_msgqIdx = 0;
	_isRecording = false;
}



/**
	Get next packet
	
	Main event loop to access the the message packet queue. Retrieves one packet
	from the packet queue per call and processes it based on the packet type.
	
	This routine should be called repeatedly for the life of the 
	monitor program.
	
	\postcondition	After reading the data from the next packet, the message type
					is set to ACQ_MSGQ_INVALID to release the packet to the sending program.
*/
void ACQ_MessagePacketQueue::getNextPacket()
{

	//
	//	And get the messages as they come in
	//
	int loopCounter = 10;
	while( _msgQAddr[ _msgqIdx ].message_type != ACQ_MSGQ_INVALID  )
	{
	  fprintf(stderr, "%d: packet type %d\n", _msgqIdx, _msgQAddr[ _msgqIdx ].message_type);
	  fprintf(stderr, "%d: id %d\n", _msgqIdx, _msgQAddr[ _msgqIdx ].messageId);
	  fprintf(stderr, "%d: number %d\n", _msgqIdx, _msgQAddr[ _msgqIdx ].sampleNumber);
	  fprintf(stderr, "%d: numsamp %d\n", _msgqIdx, _msgQAddr[ _msgqIdx ].numSamples);
	  fprintf(stderr, "%d: numchan %d\n", _msgqIdx, _msgQAddr[ _msgqIdx ].numChannels);
		switch ( _msgQAddr[ _msgqIdx ].message_type )
		{
			case ACQ_MSGQ_SETUP_COLLECTION:
				setupCollectionCallback( _msgQAddr[ _msgqIdx ]);
				break;
			case ACQ_MSGQ_DATA:
				if ( 	_isRecording ) receiveDataCallback( _msgQAddr[ _msgqIdx ]);
				break;
			case ACQ_MSGQ_CLOSE_COLLECTION:
				closeCollectionCallback( _msgQAddr[ _msgqIdx ]);
				break;
			default:
				// ignore invalid codes for now
				//
				_msgQAddr[ _msgqIdx ].message_type = ACQ_MSGQ_INVALID;
				_msgqIdx++;
				if ( _msgqIdx >= ACQ_MSGQ_SIZE ) _msgqIdx = 0;
				break;
		}
		if ( loopCounter-- < 0 ) break;
	}
}

/**
	Add the Setup callback  method.
	
	Set the user supplied callback routine as the Setup Collection callback.
	The specified method will be called when a ACQ_MSGQ_SETUP_COLLECTION
	message type is received from the acquisition program
	
			
	\postcondition	Setup Collection callback procedure is set to the \c callProcedure
*/		
void ACQ_MessagePacketQueue::addSetupCollectionCallback( 
	CBProc callProcedure,      //!<  Callback procedure reference
	void * callbackData			//!< callback data. This value is passed to the callback procedure 	when the procedure is called.
	 )

{
	_setupCBProcedure = callProcedure;
	_setupCBData = callbackData;
}

/**
	Add the Receive data callback  method.
	
	Set the user supplied callback routine as the Setup Collection callback.
	The specified method will be called when a ACQ_MSGQ_DATA
	message type is received from the acquisition program
	
			
	\postcondition	Receive data callback procedure is set to the \c callProcedure
*/		
void ACQ_MessagePacketQueue::addReceiveDataCallback( 
	CBProc callProcedure,      //!<  Callback procedure reference
	void * callbackData			//!< callback data. This value is passed to the callback procedure 	when the procedure is called.
	 )
{
	_receiveDataCBProcedure = callProcedure;
	_receiveDataCBData = callbackData;
}

/**
	Add the Close callback  method.
		
	Set the user supplied callback routine as the Setup Collection callback.
	The specified method will be called when a ACQ_MSGQ_CLOSE_COLLECTION
	message type is received from the acquisition program
			
	\postcondition	Close Collection callback procedure is set to the \c callProcedure
*/		
void ACQ_MessagePacketQueue::addCloseCollectionCallback( 
	CBProc callProcedure,      //!<  Callback procedure reference
	void * callbackData			//!< callback data. This value is passed to the callback procedure 	when the procedure is called.
	 )

{
	_closeCBProcedure = callProcedure;
	_closeCBData = callbackData;
}

