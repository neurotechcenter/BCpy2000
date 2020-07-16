// XLDataExportReceiver.cpp : Implementation of CXLDataExportReceiver
#include "XLDataExportClient.h"
#include "XLDataExportReceiver.h"
#include <iomanip>

ComRegisterCoClass(CXLDataExportReceiver);

/////////////////////////////////////////////////////////////////////////////
// CXLDataExportReceiver

CXLDataExportReceiver::CXLDataExportReceiver()
: m_studyRunning(false)
{
}

CXLDataExportReceiver::~CXLDataExportReceiver()
{
}

static const char* month_tbl[] = {
	("---"), ("JAN"), ("FEB"), ("MAR"), ("APR"), ("MAY"), ("JUN"), ("JUL"), ("AUG"), ("SEP"), ("OCT"), ("NOV"), ("DEC")
};

/*
enum
{
	EEG_HEADBOX32 = 1			// EEG32
	EEG_HEADBOX128 = 3,        		// EEG128, EEG128FS
	AMB_L_HEADBOX = 8,          	// Mobee32, Mobee32-O2
	EMU_L_HEADBOX = 9,          	// Mobee-24
	SLEEP_IPUSB_HEADBOX = 14,    	// Connex / Brain Monitor
	AMBULATORY_USB_HEADBOX = 15,	// Trex
	EMU40_HEADBOX = 17,			// EMU40
	EEG32USB_HEADBOX = 19,		// EEG32u
	EEG512_HEADBOX = 20,        //Quantum
	BRAINBOX_HEADBOX = 21,		// NeuroLink IP
	BIOLOGIC_NETLINK_HEADBOX = 22,	// Bio-logic Netlink
	BIOLOGIC_TRAVELER_HEADBOX = 23,	// Bio-logic Traveler
	V32_HEADBOX,				// 26	
	V44_HEADBOX,				// 27	
};
*/
static const char* headboxType[] = {
	("UNKNOWN"),	// 0
	("EEG32"),	// 1
	("UNKNOWN"),	// 2
	("EEG128"),	// 3
	("UNKNOWN"),	// 4
	("UNKNOWN"),	// 5
	("UNKNOWN"),	// 6
	("UNKNOWN"),	// 7
	("MOBEE32"),	// 8
	("MOBEE24"),	// 9
	("UNKNOWN"),	// 10
	("UNKNOWN"),	// 11
	("UNKNOWN"),	// 12
	("UNKNOWN"),	// 13
	("CONNEX/Brain Monitor"),	// 14
	("TREX"),		// 15
	("UNKNOWN"),	// 16
	("EMU40"),	// 17
	("UNKNOWN"),	// 18
	("EEG32u"),	// 19
	("QUANTUM"),	// 20
	("NEUROLINK IP"),	// 21
	("NETLINK"),	// 22
	("TRAVELER"),	// 23
	("UNKNOWN"),	// 24
	("UNKNOWN"),	// 25
	("V32"),	// 26
	("V44"),	// 27
};

STDMETHODIMP CXLDataExportReceiver::StartExportData(/*[in]*/ XLDataExportPatientMetadata patient, /*[in]*/ XLDataExportStudyMetadata study)
{
	m_firstName = com::DualString(patient.firstName);
	m_lastName = com::DualString(patient.lastName);

	m_patientGUID = com::DualString(patient.guid);
	
	m_studyName = com::DualString(study.studyName);
	m_fileName = com::DualString(study.fileName);
	
	m_studyGuid = com::DualString(study.guid);
	
	m_studyType = study.studyType;
	m_headboxType = study.headboxType;
	m_numberOfChannels = study.numberOfChannels;
	m_samplingFreq = study.samplingFreq;
	m_xlCreationTime = study.xlCreationTime;
	m_outfile.open( "c:\\temp\\exportedData.csv", std::ios::app );
	if(m_outfile.is_open()) 
	{

		m_outfile << "study started, study GUID: " << m_studyGuid << "\n";
		m_outfile << "patient name:[" << m_firstName << ", " << m_lastName << ", patient GUID: " << m_patientGUID << "]\n";
		// headbox information (type / frequency / number of channels)
		std::string headboxType(headboxType[m_headboxType>=0 && m_headboxType<24?m_headboxType:0]);
		m_outfile << "headbox information (type: " << headboxType << " (" << m_headboxType << "), frequency: " << m_samplingFreq << " Hz, number of channels: " << m_numberOfChannels << ")\n";

		SYSTEMTIME				StudyCreationTimeUniversal, StudyCreationTimeLocal;
		TIME_ZONE_INFORMATION	TimeZoneInformation;

		GetTimeZoneInformation(&TimeZoneInformation);
		FileTimeToSystemTime(reinterpret_cast<LPFILETIME>(&m_xlCreationTime), &StudyCreationTimeUniversal);
		SystemTimeToTzSpecificLocalTime(&TimeZoneInformation, &StudyCreationTimeUniversal, &StudyCreationTimeLocal);

//		FILETIME ft;
//		FileTimeToLocalFileTime(reinterpret_cast<LPFILETIME>(&m_xlCreationTime), &ft);
//		SystemTimeToFileTime(&StudyCreationTimeLocal, &ft);
	
		m_outfile << std::setfill('0') << "study creation time: "
			<< month_tbl[StudyCreationTimeLocal.wMonth] << " "
			<< std::setw(2) << StudyCreationTimeLocal.wDay << " "
			<< StudyCreationTimeLocal.wYear << " "
			<< std::setw(2) << StudyCreationTimeLocal.wHour << ":"
			<< std::setw(2) << StudyCreationTimeLocal.wMinute << ":"
			<< std::setw(2) << StudyCreationTimeLocal.wSecond
			<< std::endl;
	}

	m_studyRunning = true;
	return S_OK;
}

STDMETHODIMP CXLDataExportReceiver::StopExportData()
{
	// TODO: Add your implementation code here

	if(m_outfile.is_open()) 
	{
		m_outfile << "study stopped" << std::endl;
		m_outfile.close();
	}

	m_studyRunning = true;
	return S_OK;
}

STDMETHODIMP CXLDataExportReceiver::PassExportData(/*[in]*/ TStamp samplestamp, /*[in]*/ short numberOfChannels, /*[in, size_is(numberOfChannels)]*/ float wave_data[])
{
	if(m_outfile.is_open()) 
	{
		m_outfile << samplestamp << ", " << numberOfChannels << ", " << std::setprecision(5) << std::fixed << wave_data[0];
	}

	return S_OK;
}
