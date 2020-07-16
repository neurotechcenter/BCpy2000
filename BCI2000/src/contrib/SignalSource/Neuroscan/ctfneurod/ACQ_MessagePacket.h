// $Header: /home/qc10/release4.18-AcqSW/code_example/RCS/ACQ_MessagePacket.h,v 22.1 2004/01/15 22:50:59 dougmck Exp $
/**
	\file	ACQ_MessagePacket
	
	The ACQ_MessagePacket is used to send data from from Acq to a third
	party monitoring program.
	
*/
//	----------------------------------------------------------------------------
//	$Log: ACQ_MessagePacket.h,v $
//	Revision 22.1  2004/01/15 22:50:59  dougmck
//	SPR#2547 -- Real time output for third party software
//	initial check in
//
//	--------------------------------------------------------------------------
#include <limits.h>

	/**
		Message data type constants.
		
		Define the type of data transfered in the packet
	*/
	enum	ACQ_MessageType
	{
		ACQ_MSGQ_SETUP_COLLECTION,	//!<	Setup up for collection
		ACQ_MSGQ_DATA,				//!<	packet contains data
		ACQ_MSGQ_CLOSE_COLLECTION,	//!<	Close collection message
		ACQ_MSGQ_INVALID = INT_MAX 	//!<	Packet does not contain any data
	};
	
	///	Other constants
	enum
	{
		ACQ_MSGQ_SIZE = 600,				//!< Size of data message queue in packets
		ACQ_MESQ_SHMKEY = 0x39457f73  	//!< Key for shared memry segment
	};


/**
	
	This is the structure used to pass data from the MEG/EEG data acquisition 
	program ( \b Acq ) to a third party receiver program using shared memory.
	
	\code
		typedef struct ACQ_MessagePacket
		{
			ACQ_MessageType	message_type;
			int	messageId;	
			int	sampleNumber;
			int	numSamples;
			int	numChannels;
			int	data[ 8192 ];
		};
	\endcode	

*/	
typedef struct ACQ_MessagePacket
{
	/**
		message type
		
		Speicies the type of data in the packet.
		
		Contains one of 
		\li	\c	ACQ_MSGQ_SETUP_COLLECTION	Setup for collection, Packet contains dataset name
		\li	\c	ACQ_MSGQ_DATA, 				Data packet, packet contains data
		\li	\c	ACQ_MSGQ_CLOSE_COLLECTION	Close collection
		\li	\c	ACQ_MSGQ_INVALID			Packet does not contain any data.
	*/  
	ACQ_MessageType	message_type;
		
	/**
		message id
		
		A sequential id packet number. The id wraps around at a maximum determined by 
		Acq ( typically 300, but may be higher ). This should be used to determine whether
		any message packets have been dropped. 
	*/  
	int	messageId;	
							
	/**
		Sample number ( ACQ_MSGQ_DATA packet only )
		
		Sample counter. Increments with each packet received, by the number of samples 
		in the packet. 
	*/  
	int	sampleNumber;
	
	/**
		Number of samples in packet ( ACQ_MSGQ_DATA packet only )
		
		Contains the number of samples in the packet. A single sample contains \a numChannels
		\c long \c int values.
	*/
	int	numSamples;
	
	/**
		 Number of channels per sample in packet ( ACQ_MSGQ_DATA packet only )
	
		Contains the number of channels per sample.
	*/
	int	numChannels;
	
	/**
		Packet specific data 
		
		If message is equal to ACQ_MSGQ_SETUP_COLLECTION, \b data contains full path of
		the data set being recorded terminated with the null character
		
		If message is equal to ACQ_MSGQ_DATA, \b data contains \c numChannels * \c numSamples 32 bit words
		of multiplexed data. 
		
		\image rtf dataformat.JPG Multiplexed data format for received data
		
		\warning	The data in a \c ACQ_MSGQ_DATA packet is the same byte order as received from the
		MEG/EEG electronics. For the \b Omega \b 2000 electronics, the data is stored least significant byte 
		first. For the \b DSQ-2000/Hybrid series or older, the byte order is most-significant byte first.
		It is the \b receiver \b program's responsibility to interpret the byte order properly.
	
	*/
	int	data[ 28160 ];
};	

