////////////////////////////////////////////////////////////////////////////////
/// @file    CmdParamInfo.h
/// @brief   Defines for enum and struct of EegDataSource.dll 
/// @version EegDataSource V01.01
////////////////////////////////////////////////////////////////////////////////
#pragma once

///	EegDataSource error code
enum DATA_SOURCE_ERR {
	//	General error codes
	DS_CANCEL_REQUEST					= -28,	//  Request is canceled.
	DS_MMFILE_OVERRUN_READ_OFFSET		= -27,	//	Read pointer over run for memory mapped file.
	DS_MMFILE_WRITE_APP_CLOSE			= -26,	//	Memory mapped file server is closed.
	DS_MMFILE_RESETTING					= -25,	//	Resetting the memory mapped file.
	DS_APPUDP_RR_TIMEOUT				= -24,	//	RR time out for application communication.
	DS_DISCONNECT_SOCKET				= -23,	//	Socket disconnection.
	DS_SELECT_DATA_SOURCE_CANCELED		= -22,	//	Selecting data source is canceled.
	DS_FAIL_TO_OPEN_MMFILE				= -21,	//	Failed to open memory mapped file.
	DS_RECEIVE_IRREGULAR_PACKET			= -20,	//	Received packe is invalid.
	DS_WAVE_RR_TIMEOUT					= -19,	//	RR time out for collecting waveform.
	DS_CMD_RR_TIMEOUT					= -18,	//	RR time out for commands.
	DS_SEND_CMD_ERR_RETRY_MAX			= -17,	//	Retry error of sending commands.
	DS_MMFILE_ERR_NEED_RESET			= -16,	//	Error which needs the reset of memory mapped file.
	DS_MMFILE_ERR						= -15,	//	General error for memory mapped file.
	DS_FAIL_TO_INIT_SEQUENCE			= -14,	//	Failed to initialize the sequence.
	DS_FAIL_TO_READ_XML_SETTING_FILE	= -13,	//	Error in reading the setting xml file.
	DS_FAIL_TO_DECIDE_DEST_IPADDRESS	= -12,	//	Could not find destination IP address.
	DS_FAIL_TO_CONNECT_SOCKET			= -11,	//	Failed to connect socket.
	DS_FAIL_TO_CREATE_SOCKET			= -10,	//	Failed to create socket.
	DS_FAIL_TO_CREATE_SOCKET_INFO		= -9,	//	Failed to create destination socket information.
	DS_FAIL_TO_CREATE_MMFILE			= -8,	//	Failed to carete memory mapped file.
	DS_REQ_DIF_MODE						= -7,	//	Mode is invalid.
	DS_NOT_CONNECTED					= -6,	//	Not connected.
	DS_NOT_READY						= -5,	//	Not ready. 
	DS_NO_MEMORY						= -4,	//	No enough memory.
	DS_INVALID_PARAM					= -3,	//	Invalied parameters.
	DS_ALREADY_DONE						= -2,	//	Already done.
	DS_ERR								= -1,	//	General error.
	DS_NO_ERR							= 0,	//	No error.
	// Following is the value from GetLastErr.
};


//	Macros for getting error code.
#define EEG_NOERR(x)	(x == DS_NO_ERR)								///	Check there is an error or not.
#define EEG_SUCCEEDED(x)	(x == DS_NO_ERR || x == DS_ALREADY_DONE)	///	Check succeeded or not. DS_ALREADY_DONE is also recognized as succeeded.
#define EEG_FAILED(x)	(x != DS_NO_ERR && x != DS_ALREADY_DONE)		///	Check failed or not. DS_ALREADY_DONE is also recognized as succeeded.
#define ASSERT_EEG(x)	ASSERT(EEG_SUCCEEDED(x))						///	Assert succeeded or not. DS_ALREADY_DONE is also recognized as succeeded.


///	Status change notification type
enum DATASOURCE_DLL_STS {
	DATASOURCE_DLL_STS_ERR = 0,					///	Notification for the error information
	DATASOURCE_DLL_STS_RECONNECT_SUCCESS,		///	Notification for reconnection was done successfully.
	DATASOURCE_DLL_STS_RECIEVE_CMD_FROM_JBOX,	///	Notification for receiving a command from junction box.
	DATASOURCE_DLL_STS_RECIEVE_CMD_FROM_SUBCTRL,///	Notification for receiving a command from research stream application.
};

///	Reconnection status type
enum RECONNECT_STATUS_TYPE {
	RCS_NONE,		///	Reconnection is not done.
	RCS_REQUIRED,	///	Reconnection is necessary.
	RCS_TRYING,		///	Reconnecting...
	RCS_DONE,		///	Reconnection is done successfully.
	RCS_CANCELLED,	///	Reconnection is canceled.
	RCS_FAILED,		///	Reconnection is failed.
};

///	Mode type
enum DATA_SOURCE_MODE_TYPE{
	DS_MODE_MAIN = 0,	///	Main controler mode (do not use)
	DS_MODE_SUB,		///	Sub controler mode (do not use)
	DS_MODE_READER,		///	Reader mode
	DS_MODE_MAX
};


/// QI-123A information in the memory mapped file.
struct MMFILE_QI123A_INFO {
	unsigned long	dwIPAddress;			// IP address of QI-123A			
	char			szIdentifier[32+1];		// Identifier of QI-123A
	unsigned long	dwIPAddressEx1;			// IP address of QI-124A-A
	char			szIdentifierEx1[32+1];	// Identifier of QI-124A-A
	unsigned long	dwIPAddressEx2;			// IP address of QI-124A-B
	char			szIdentifierEx2[32+1];	// Identifier of QI-124A-B
	unsigned long	dwIPAddressEx3;			// IP address of QI-124A-C
	char			szIdentifierEx3[32+1];	// Identifier of QI-124A-C
	unsigned long	dwIPAddressEx4;			// IP address of QI-124A-D
	char			szIdentifierEx4[32+1];	// Identifier of QI-124A-D
};

/// Patient information attribute in the memory mapped file.
struct MMFILE_PATIENT_ITEM_ATTR {
	unsigned long	dwID;					/// Item ID
	unsigned long	dwNameLength;			/// Max lenght of the name
	unsigned short	wType;					/// Data type
	unsigned short	wUnit;					/// Unit
	unsigned long	dwDataLength;			/// Max length of the data.
};

/// Electrode name information in the memory mapped file.
#define ELECTRODE_NAME_SIZE		(16+1)
struct MMFILE_ELECTRODE_NAME {
	unsigned int	nItemCount;														/// Item number
	char			(*pszName)[ELECTRODE_NAME_SIZE];								/// Electrode name

	NK_API __stdcall MMFILE_ELECTRODE_NAME(void);					/// Constructor
	NK_API __stdcall ~MMFILE_ELECTRODE_NAME(void);					/// Destructor
	NK_API void __stdcall SetItemCount(unsigned int _nItemCount);	/// Set the item number
	NK_API void __stdcall RemoveAllItem(void);						/// Remove all the items
};


/// Electrode code information in the memory mapped file
struct MMFILE_ELECTRODE_CODE {
	unsigned int	nItemCount;														/// Item number
	unsigned short	*pwCode;														/// Electrode code

	NK_API __stdcall MMFILE_ELECTRODE_CODE(void);					/// Constructor
	NK_API __stdcall ~MMFILE_ELECTRODE_CODE(void);					/// Destructor
	NK_API void __stdcall SetItemCount(unsigned int _nItemCount);	/// Set the item number
	NK_API void __stdcall RemoveAllItem(void);						/// Remove all the items
};


/// Data source information in the memory mapped file
struct MMFILE_DATA_SOURCE_INFO {
	BOOL	bExtendLine;				/// Stream typeÅiFALSE: Clinical stream, TRUE: Research streamÅj
	DWORD	dwIpAddress;				/// IP address
	char	szUnitIdentifier[1024];		/// Unit identifier
	char	szUnitType[1024];			/// Unit type
};


/// Reader mode initialization information
struct READER_MODE_INIT_INFO {
	BOOL			bSelectDataSource;	/// Select the data source or not. (If FALSE, bExtendedLine and ulIpAddress should be configured.)
	BOOL			bExtendLine;		/// Data type (FALSE: Clinical stream, TRUE: Research stream)
	unsigned long	ulIpAddress;		/// IP address of the amplifier.
};


