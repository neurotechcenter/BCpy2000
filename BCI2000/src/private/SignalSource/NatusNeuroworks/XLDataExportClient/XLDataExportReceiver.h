#ifndef XL_DATA_EXPORT_RECEIVER_H
#define XL_DATA_EXPORT_RECEIVER_H

#include "XLDataExportReceiver.h"
#include "ComCoClass.h"
#include "ComStrings.h"
#include "ComModule.h"
#include "Resource.h"
#include <string>
#include <fstream>

/////////////////////////////////////////////////////////////////////////////
// XLDataExportReceiver
class  CXLDataExportReceiver : 
	public com::CoClass<IXLDataExportReceiver, &IID_IXLDataExportReceiver, CXLDataExportReceiver, &CLSID_XLDataExportReceiver>
{
public:
	CXLDataExportReceiver();
	~CXLDataExportReceiver();

// XLDataExportReceiver
public:
	STDMETHOD(PassExportData)(/*[in]*/ TStamp samplestamp, /*[in]*/ short numberOfChannels, /*[in, size_is(numberOfChannels)]*/ float wave_data[]);
	STDMETHOD(StopExportData)();
	STDMETHOD(StartExportData)(XLDataExportPatientMetadata patient, XLDataExportStudyMetadata study);

private:
	std::string m_firstName;
	std::string m_lastName;
	std::string m_patientGUID;
	std::string m_studyName;
	std::string m_fileName;
	std::string m_studyGuid;
	StudyType m_studyType;
	long m_headboxType;
	long m_numberOfChannels;
	double m_samplingFreq;
	__int64 m_xlCreationTime;

	bool m_studyRunning;
	std::fstream m_outfile;
};

#endif // XL_DATA_EXPORT_RECEIVER_H
