#ifndef H_ACQ_MESSAGEPACKETQUEUE
#define H_ACQ_MESSAGEPACKETQUEUE

#include <limits.h>
#include "ACQ_MessagePacket.h"

/**
	
	Queue of ACQ_MessagePacket 's used to receive data  from \b Acq by a third
	party receiver program.
	
	This class creates a shared memory segment containing  \c ACQ_MSGQ_SIZE ( 600 )
	ACQ_MessagePacket s. The class can then monitor the shared memory for message 
	packets updated by \b Acq using the getNextPacket() method. When a packet 
	containing a \c message_type other than \c ACQ_MSGQ_INVALID	is detected, 
	one of setupCollectionCallback(), receiveDataCallback() or closeCollectionCallback()
	is called depending on the value in the \c message_type field. These methods forward the
	data to the approriate user supplied callback routines added using addSetupCollectionCallback(),
	addReceiveDataCallback() and addCloseCollectionCallback().
	
*/
class ACQ_MessagePacketQueue
{
public:
	/**
		\name	Constructors and destructors
	*/
	//@{	
		/**
			constructor
		
			Creates an initializes the shared memory to receive data from a sender. A 
			shared memory segment is created with using the same key used by \b Acq.
			
			\post	A shared memory segment with key ACQ_MSGQ_SHMKEY is created and
					attached to.
			        
		*/		
		ACQ_MessagePacketQueue(  );

		/**
			Destructor
		
			Clean up shared memory.

			\post	The program is detached from the shared memory segment,
					and marked for removal. If no other processes are attached
					to the shared memory segment, the the segment is deleted.
		*/		
		virtual ~ACQ_MessagePacketQueue(  );

	protected:	
		/**
			Initialize the shared memory to receive data from a sender.
		*/		
		void init(  );

		/**
			Close the shared memory .
		*/		
		void close(  );
	//@}

	/**
		\name	Add callbacks utility routines
	
		These routines add the callback methods for the setup, receive data and close callbacks
		
		The callback methods have the form
		
		\code
		
		void myCallback( ACQ_MessagePacket & packet, void * clientData );
		
		\endcode
		
		When called, the user supplied routines are passed a reference to the current message packet, 
		and the value ( \e callbackData ) passed as the second argument to the add callback method.
	*/
public:
	//@{
		/**
			Call back definition for user supplied callbacks
		
			The prototype for the user supplied callbacks is
			
			\code
			
            void myCallback( ACQ_MessagePacket & messagePacket, void * callbackdata );
			
			\endcode
		*/	
		typedef void (*CBProc) ( ACQ_MessagePacket & packet, void * callbackData );

		/**
			Add the user supplied Setup callback  method.
		*/		
		void addSetupCollectionCallback( CBProc callProcedure, void * callbackData );

		/**
			Add the user supplied Receive data callback  method.
		*/		
		void addReceiveDataCallback( CBProc callProcedure, void * callbackData );

		/**
			Add the user supplied Close callback  method.
		*/		
		void addCloseCollectionCallback( CBProc callProcedure, void * callbackData );

	//@}
	
	/**
		\name	Callbacks utility routines
	
		These routines are called by getNextPacket() when a the \c message_type
		field of the packet is a value other than \c ACQ_MSGD_INVALID
	*/
	//@{
	public:	
		/**
			Get next packet
		*/
		void getNextPacket();

	protected:
		/**
			Setup collection callback
		*/
		void	setupCollectionCallback( ACQ_MessagePacket & messagePacket );

		/**
			receive data callback
		*/
		void	receiveDataCallback( ACQ_MessagePacket & messagePacket);

		/**
			Close collection callback
		*/
		void	closeCollectionCallback( ACQ_MessagePacket & messagePacket );

	//@}
	
	
private:
	/**
		Shared memory id for data packet queue. Used as argument to
		shmat() ( shared memory attach )
	*/
	int			_msgQid;

	/**
		Shared memory key.
	*/
	key_t 		_msgQkey;

	/**
		Shared memory address
	*/
	ACQ_MessagePacket *	_msgQAddr;

	/**
		Current index in message queue of next sample of data
	*/
	int	_msgqIdx;

	/**
		Indicates whether queue is currently recording
	*/
	bool	_isRecording;

	/**
		\name	Callback procedure data
	*/
	//@{
	
		CBProc 	_setupCBProcedure;
		void *	_setupCBData;
		
		CBProc  _receiveDataCBProcedure;
		void *	 _receiveDataCBData;

		CBProc  _closeCBProcedure;
		void *	_closeCBData;
	//@}

};

#endif // H_ACQ_MESSAGEPACKETQUEUE


