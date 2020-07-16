/////////////////////////////////////////////////////////////////////////////
// $Id: EyetrackerLoggerTobii3.cpp 4731 2014-07-03 15:18:03Z mellinger $
// Authors: markus.adamek@gmail.com
// Description: The Eyetracker Logger logs incoming data from a Tobii with TobiiPro SDK
// Eyetracker and changes states
//
// Parameterization:
//   Eyetracker logging is enabled from the command line adding
//     --EyetrackerLoggerTobiiPro=1
//   As a command line option.
//   Information may be selectively logged by setting these parameters to true
//     LogGazeData - record gaze data to states
//     LogEyePosition - record eye position to states
//     LogPupilSize - record pupil size to states
//     LogEyeDist - record eye distances to states
//
// State Variables:
//   EyetrackerLeftEyeGazeX   - Left Eye X Gaze Position (0 = leftmost, 65535 = rightmost)
//   EyetrackerLeftEyeGazeY   - Left Eye Y Gaze Position (0 = topmost, 65535 = bottommost)
//   EyetrackerRightEyeGazeX  - Right Eye X Gaze Position (0 = leftmost, 65535 = rightmost)
//   EyetrackerRightEyeGazeY  - Right Eye Y Gaze Position (0 = topmost, 65535 = bottommost)
//   EyetrackerLeftEyePosX    - Left Eye X Position (0 = leftmost, 65535 = rightmost)
//   EyetrackerLeftEyePosY    - Left Eye Y Position (0 = topmost, 65535 = bottommost)
//   EyetrackerRightEyePosX   - Right Eye X Position (0 = leftmost, 65535 = rightmost)
//   EyetrackerRightEyePosY   - Right Eye Y Position (0 = topmost, 65535 = bottommost)
//   EyetrackerLeftPupilSize  - Left Eye Pupil Size (mm), Depends on eye pos and dist
//   EyetrackerRightPupilSize - Right Eye Pupil Size (mm), Depends on eye pos and dist
//   EyetrackerLeftEyeDist    - Left Eye Distance from Screen (mm), Approx
//   EyetrackerRightEyeDist   - Right Eye Distance from Screen (mm), Approx
//   EyetrackerTime     - The timestamp for the gaze data relative to BCI timer
//
// (C) 2000-2015, BCI2000 Project
// http://www.bci2000.org
/////////////////////////////////////////////////////////////////////////////
#include "EyetrackerLoggerTobiiPro.h"
#include "BCIEvent.h"

#define EVENTS \
	_(Time, 16, uint, T) \
	//

#define PER_EYE_EVENTS \
	_(EyeValidity, 3, uint,) \
	_(PupilSize, 8, uint, Pupil) \
	_(EyeGazeX, 18, int, X) _(EyeGazeY, 18, int, Y) /* additional bits to allow for range violation */\
	_(EyePosX, 18, int, X) _(EyePosY, 18, int, Y) _(EyeDist, 14, int, Z) \
	//

using namespace std;

Extension( EyetrackerLoggerTobiiPro );

namespace {
	static const double eps = std::numeric_limits<double>::epsilon();

	// currently, no vector algebra available in BCI2000 framework
	template<class T> double sq(const T& t) { return t*t; }
	struct vector3d { double dx, dy, dz; };

	vector3d operator-(const vector3d& p1, const vector3d& p2)
	{ vector3d v = { p1.dx - p2.dx, p1.dy - p2.dy, p1.dz - p2.dz }; return v; }
	vector3d operator-(const TobiiResearchPoint3D& p1, const vector3d& p2)
	{ vector3d v = { p1.x - p2.dx, p1.y - p2.dy, p1.z - p2.dz }; return v; }

	vector3d operator-(const TobiiResearchPoint3D& p1, const TobiiResearchPoint3D& p2)
	{ vector3d v = { p1.x - p2.x, p1.y - p2.y, p1.z - p2.z }; return v; }
	double operator*(const vector3d& v1, const vector3d& v2)
	{ return v1.dx*v2.dx + v1.dy*v2.dy + v1.dz*v2.dz; }
	vector3d operator*(const vector3d& v1, double d)
	{ vector3d v2 = { v1.dx*d, v1.dy*d, v1.dz*d }; return v2; }
	vector3d crossprod(const vector3d& v1, const vector3d& v2)
	{
		vector3d v = {
			v1.dy*v2.dz - v1.dz*v2.dy,
			v1.dz*v2.dx - v1.dx*v2.dz,
			v1.dx*v2.dy - v1.dy*v2.dx
		};
		return v;
	}
}

#define _(x, y, z, u) static const uint64_t bitmask_##x = (1ULL<<(y))-1;
EVENTS PER_EYE_EVENTS
#undef _


struct EyetrackerLoggerTobiiPro::TobiiProScreenCoords
{
	TobiiProScreenCoords() {};
	~TobiiProScreenCoords(){};
	vector3d mScreenXAxis, mScreenYAxis, mScreenZAxis,mScreenOrigin;
	PhysicalUnit mUnitX, mUnitY, mUnitZ, mUnitPupil;
	vector3d tobiiUCSToscreenRelativeCoordinates(const TobiiResearchPoint3D&);

};

vector3d EyetrackerLoggerTobiiPro::TobiiProScreenCoords::tobiiUCSToscreenRelativeCoordinates(const TobiiResearchPoint3D& p)
{
	vector3d s = p - mScreenOrigin;
	vector3d r = { s*mScreenXAxis, s*mScreenYAxis, s*mScreenZAxis };
	return r;
}

EyetrackerLoggerTobiiPro::EyetrackerLoggerTobiiPro()
	: m_eyetrackerEnable( false ),
	m_gazeScale( 1 ),
	_connectedDevice(nullptr),
	_deviceList( nullptr),
	_coordSystem(nullptr),
	_isLogging(false),
	m_firstData(true)
{

}



EyetrackerLoggerTobiiPro::~EyetrackerLoggerTobiiPro()
{
	Halt();
	if(_deviceList != nullptr)
	{
		tobii_research_free_eyetrackers(_deviceList);
	}
	delete _coordSystem;

}

void EyetrackerLoggerTobiiPro::Publish()
{
	delete _coordSystem;
	bool eyetrackerEnable = false;
	eyetrackerEnable = ( ( int )OptionalParameter( "LogEyetrackerTobiiPro" ) != 0 );
	if( !eyetrackerEnable )
		return;

	_coordSystem = new TobiiProScreenCoords;
	BEGIN_PARAMETER_DEFINITIONS
		"Source:Eyetracker int LogEyetrackerTobii3= 1 0 0 1"
		" // enable eye tracker logging (boolean)",
		"Source:Eyetracker float GazeScale= 1 1 0 1"
		" // value to scale gaze data by (deprecated)",
		"Source:Eyetracker float GazeOffset= 0 0 & &"
		" // value to offset gaze data after scaled (deprecated)",
		"Source:Eyetracker matrix EyetrackerInfo= 1 1 auto "
		"// eye tracker properties and settings (readonly)(noedit)",
		"Source:Eyetracker matrix EyetrackerData= 1 1 auto "
		"// eye tracker data channel properties (readonly)(noedit)",
		"Source:Eyetracker int LogGazeData= 1 1 auto "
		"// compatability  (readonly)(noedit)",
		END_PARAMETER_DEFINITIONS

		BEGIN_EVENT_DEFINITIONS
#define _(x, y, z, u) "Eyetracker" #x " " #y " 0 0 0",
		EVENTS
#undef _
#define _(x, y, z, u) "EyetrackerLeft" #x " " #y " 0 0 0",
		PER_EYE_EVENTS
#undef _
#define _(x, y, z, u) "EyetrackerRight" #x " " #y " 0 0 0",
		PER_EYE_EVENTS
#undef _
		END_EVENT_DEFINITIONS
}


void EyetrackerLoggerTobiiPro::Preflight() const
{
	bool eyetrackerEnable = false;
	float gazeScale = 0.0f;
	float gazeOffset = 0.0f;

	eyetrackerEnable = ( ( int )OptionalParameter( "LogEyetrackerTobiiPro" ) != 0 );

	if( eyetrackerEnable )
	{

		gazeScale = ( float )Parameter( "GazeScale" );
		gazeOffset = ( float )Parameter( "GazeOffset" );
		State("SourceTime");
		Parameter("EyetrackerData");
		Parameter("EyetrackerInfo");

	}
}


void EyetrackerLoggerTobiiPro::Initialize()
{
	m_eyetrackerEnable = ( ( int )OptionalParameter( "LogEyetrackerTobiiPro" ) != 0 );
	if(m_eyetrackerEnable)
	{

		//Initialize parameters
		m_gazeScale = ( float )Parameter( "GazeScale" );
		m_gazeOffset = ( float )Parameter( "GazeOffset" );

		// Get devices and connect
		_connectedDevice=FindAndConnectToDevice();
		if(_connectedDevice != NULL)
			bciout << "Sucessfully connected to Tobii EyeLogger!" << endl;

		storeEyetrackerInformation(_connectedDevice);
		TobiiResearchDisplayArea display_area;
		if(tobii_research_get_display_area(_connectedDevice,&display_area) != TOBII_RESEARCH_STATUS_OK)
		{
			throw bcierr<< "Failed to get Screen coordinates" << endl;
		}
		auto xAxis = display_area.top_right - display_area.top_left,
			yAxis = display_area.bottom_left - display_area.top_left,
			zAxis = crossprod(yAxis, xAxis);

		auto xLen = ::sqrt(sq(xAxis)), yLen = ::sqrt(sq(yAxis)), zLen = ::sqrt(sq(zAxis));
		if(xLen < eps || yLen < eps || zLen < eps)
			throw bcierr << "Invalid screen coordinates";

		double xyRange = 65536;
		_coordSystem->mScreenOrigin.dx= display_area.top_left.x;
		_coordSystem->mScreenOrigin.dy=display_area.top_left.y;
		_coordSystem->mScreenOrigin.dz=display_area.top_left.z;

		_coordSystem->mScreenXAxis = xAxis * (1e-3/xLen);
		_coordSystem->mScreenYAxis = yAxis * (1e-3/yLen);
		_coordSystem->mScreenZAxis = zAxis * (1e-3/zLen);

		_coordSystem->mUnitX.SetGain(xLen*1e-3/xyRange/m_gazeScale).SetSymbol("m");
		_coordSystem->mUnitX.SetOffset(m_gazeOffset);
		_coordSystem->mUnitX.SetRawMin(0).SetRawMax(xyRange - 1);

		_coordSystem->mUnitY.SetGain(yLen*1e-3/xyRange/m_gazeScale).SetSymbol("m");
		_coordSystem->mUnitY.SetOffset(m_gazeOffset);
		_coordSystem->mUnitY.SetRawMin(0).SetRawMax(xyRange - 1);

		_coordSystem->mUnitZ.SetGainWithSymbol("1mm");
		_coordSystem->mUnitZ.SetRawMin(0).SetRawMax(2e3);

		_coordSystem->mUnitPupil.SetGainWithSymbol("0.1mm");
		_coordSystem->mUnitPupil.SetRawMin(0).SetRawMax(100);

		PhysicalUnit timeMs;
		timeMs.SetGainWithSymbol("1ms");
		PhysicalUnit none;

		const PhysicalUnit
			*puX = &_coordSystem->mUnitX, *puY = &_coordSystem->mUnitY, *puZ = &_coordSystem->mUnitZ,
			*puPupil = &_coordSystem->mUnitPupil, *puT = &timeMs, *pu = &none;
		struct { const char* name, *format; const PhysicalUnit* unit; }
		events[] =
		{
#define _(x, y, z, u) { "Eyetracker" #x, #z #y, pu##u },
			EVENTS
#undef _
#define _(x, y, z, u) { "EyetrackerLeft" #x, #z #y, pu##u },
			PER_EYE_EVENTS
#undef _
#define _(x, y, z, u) { "EyetrackerRight" #x, #z #y, pu##u },
			PER_EYE_EVENTS
#undef _
		};
		/*
		auto EyetrackerData = Parameter("EyetrackerData");
		EyetrackerData->SetDimensions(sizeof(events)/sizeof(*events), 2);
		EyetrackerData->ColumnLabels()[0] = "type";
		EyetrackerData->ColumnLabels()[1] = "unit";
		int row = 0;
		for(const auto& s : events)
		{
			EyetrackerData->RowLabels()[row] = s.name;
			EyetrackerData(row, 0) = s.format;
			EyetrackerData(row, 1) << *s.unit;
			++row;
		}
		*/


	}

}

void EyetrackerLoggerTobiiPro::StartRun()
{
	if(m_eyetrackerEnable) {

		if(!_isLogging)
		{
			sync_calculated=false;
			m_firstData=true;
			if(tobii_research_subscribe_to_gaze_data(_connectedDevice,gaze_data_callback, this) != TOBII_RESEARCH_STATUS_OK) 
			{
				bcierr << "Problem during start of gaze data collection" << endl;
				return;
			}

			_isLogging=true;
		}
	}
}

//static method used for subscription -> reroutes to object
//user data must be EyetrackerLogger instance
void  EyetrackerLoggerTobiiPro::gaze_data_callback(TobiiResearchGazeData* gaze_data, void* user_data)
{

	EyetrackerLoggerTobiiPro* caller=static_cast<EyetrackerLoggerTobiiPro*>(user_data);
	TobiiResearchGazeData data;
	memcpy(&data,gaze_data,sizeof(*gaze_data));
	caller->SetGazeData(data);


}



void EyetrackerLoggerTobiiPro::SetGazeData(TobiiResearchGazeData const  &gaze)
{

	WithLock(m_lock) 
	{
		uint64_t timestamp =0;
		// Write states
		if(!sync_calculated)
		{
			m_timeOffset=gaze.system_time_stamp/1000 - (uint64_t)State("SourceTime");
			sync_calculated=true;
		}

		timestamp=gaze.system_time_stamp/1000 - m_timeOffset;
		bcievent <<"EyetrackerTime "<< (timestamp & 65535) << std::flush;



		logEyeData(gaze.left_eye,m_prevGazeData.left_eye,"Left");
		logEyeData(gaze.right_eye,m_prevGazeData.right_eye,"Right");
		m_firstData = false;

		// push back data
		m_prevGazeData = gaze;
	}


}

bool EyetrackerLoggerTobiiPro::logEyeData(const TobiiResearchEyeData  &data,const TobiiResearchEyeData  &prev_data,const char* eye)
{



	string prefix="Eyetracker";
	prefix+=eye;
	uint8_t valid=0;
	if(data.gaze_point.validity != TOBII_RESEARCH_VALIDITY_VALID)
	{
		valid=4u;
	}
	bcievent << prefix <<"EyeValidity " << (valid & bitmask_EyeValidity) << std::flush;



	bool reportGaze = true,
		reportEyePos = true,
		reportPupilSize = true;
	reportGaze &= m_firstData || sq(data.gaze_point.position_in_user_coordinates - prev_data.gaze_point.position_in_user_coordinates) > eps;
	reportGaze &= (data.gaze_point.validity == TOBII_RESEARCH_VALIDITY_VALID);

	reportEyePos &= m_firstData || sq(data.gaze_origin.position_in_user_coordinates - prev_data.gaze_origin.position_in_user_coordinates) > eps;
	reportEyePos &= (data.gaze_origin.validity == TOBII_RESEARCH_VALIDITY_VALID);

	reportPupilSize &= m_firstData || sq(data.pupil_data.diameter - prev_data.pupil_data.diameter) > eps;
	reportPupilSize &= (data.pupil_data.validity == TOBII_RESEARCH_VALIDITY_VALID);



	if(reportGaze ) 
	{
		auto gazePoint=_coordSystem->tobiiUCSToscreenRelativeCoordinates(data.gaze_point.position_in_user_coordinates);
		int32_t eyeGazeX = _coordSystem->mUnitX.PhysicalToRawValue(gazePoint.dx),
			eyeGazeY = _coordSystem->mUnitY.PhysicalToRawValue(gazePoint.dy);
		bcievent << prefix << "EyeGazeX " << (eyeGazeX & bitmask_EyeGazeX) << std::flush;
		bcievent << prefix << "EyeGazeY " << (eyeGazeY & bitmask_EyeGazeY) << std::flush;

		Assert(gazePoint.dz*gazePoint.dz < eps); // always zero in screen relative system
	}

	if(reportEyePos)
	{
		auto eyePos = _coordSystem->tobiiUCSToscreenRelativeCoordinates(data.gaze_origin.position_in_user_coordinates);
		int32_t eyePosX = _coordSystem->mUnitX.PhysicalToRawValue(eyePos.dx),
			eyePosY = _coordSystem->mUnitY.PhysicalToRawValue(eyePos.dy),
			eyePosZ = _coordSystem->mUnitZ.PhysicalToRawValue(eyePos.dz);
		bcievent << prefix << "EyePosX " << (eyePosX & bitmask_EyePosX) << std::flush;
		bcievent << prefix << "EyePosY " << (eyePosY & bitmask_EyePosY) << std::flush;
		bcievent << prefix << "EyeDist " << (eyePosZ & bitmask_EyeDist) << std::flush;
	}

	if(reportPupilSize)
	{
		int32_t pupilSize = _coordSystem->mUnitPupil.PhysicalToRawValue(data.pupil_data.diameter*1e-3);
		bcievent << prefix << "PupilSize " << (pupilSize & bitmask_PupilSize) << std::flush;
	}


	return true;
}



void EyetrackerLoggerTobiiPro::StopRun()
{
	if(m_eyetrackerEnable) 
	{

		if(_isLogging)
		{
			if(tobii_research_unsubscribe_from_gaze_data(_connectedDevice,gaze_data_callback) != TOBII_RESEARCH_STATUS_OK)
			{
				bcierr << "problem during unsubscribe from gaze data callback!" << endl;
				return;
			}
			_isLogging=false;
		}

	}
}

void EyetrackerLoggerTobiiPro::Halt()
{

	StopRun();
}

TobiiResearchEyeTracker* EyetrackerLoggerTobiiPro::FindAndConnectToDevice()
{
	if(_deviceList != NULL) //free old list
	{
		tobii_research_free_eyetrackers(_deviceList);
		_deviceList=NULL;
	}
	TobiiResearchEyeTrackers* _trackers = NULL;
	TobiiResearchStatus status;
	if((status = tobii_research_find_all_eyetrackers(&_trackers)) != TOBII_RESEARCH_STATUS_OK)
	{
		bcierr << "Problem during device search: " << status << endl;
		return NULL;
	}

	if(_trackers->count < 1)
	{
		bcierr << "No device(s) found! " << endl;
		return NULL;
	}


	_deviceList=_trackers;
	return _trackers->eyetrackers[0]; //always use first tracker




}


void EyetrackerLoggerTobiiPro::storeEyetrackerInformation(TobiiResearchEyeTracker* tracker)
{
	std::vector<std::pair<std::string, std::string>> info;
	char *serialNumber, *model, *firmwareVersion, *unitName, *illuminationMode;

	if(tobii_research_get_serial_number(tracker,&serialNumber)== TOBII_RESEARCH_STATUS_OK)
	{
		std::string s_serialNumber(serialNumber);
		info.push_back(std::make_pair("SerialNumber", s_serialNumber));
		tobii_research_free_string(serialNumber);
	}

	if(tobii_research_get_model(tracker,&model)== TOBII_RESEARCH_STATUS_OK)
	{
		std::string s_model(model);
		info.push_back(std::make_pair("Model", s_model));
		tobii_research_free_string(model);
	}

	if(tobii_research_get_firmware_version(tracker,&firmwareVersion)== TOBII_RESEARCH_STATUS_OK)
	{
		std::string s_firmwareVersion(firmwareVersion);
		info.push_back(std::make_pair("FirmwareVersion", s_firmwareVersion));
		tobii_research_free_string(firmwareVersion);
	}

	if(tobii_research_get_device_name(tracker,&unitName)== TOBII_RESEARCH_STATUS_OK)
	{
		std::string s_unitName(firmwareVersion);
		info.push_back(std::make_pair("UnitName", s_unitName));
		tobii_research_free_string(unitName);
	}

	float f;

	if(tobii_research_get_gaze_output_frequency(tracker,&f)== TOBII_RESEARCH_STATUS_OK)
	{
		info.push_back(std::make_pair("FrameRate", String() << f));
	}

	if(tobii_research_get_eye_tracking_mode(tracker,&illuminationMode)== TOBII_RESEARCH_STATUS_OK)
	{
		std::string s_illuminationMode(illuminationMode);
		info.push_back(std::make_pair("EyeTrackingMode", s_illuminationMode));
	}
	std::string s;
	for(const auto& entry : info)
		if(!entry.first.empty())
			s += "\t" + entry.first + "\t" + entry.second + "\n";
	bciout << s;

	auto EyetrackerInfo = Parameter("EyetrackerInfo");
	EyetrackerInfo->SetNumColumns(1);
	EyetrackerInfo->SetNumRows(info.size());
	for(int row = 0; row < info.size(); ++row)
	{
		EyetrackerInfo->RowLabels()[row] = info[row].first;
		EyetrackerInfo(row) = info[row].second;
	}

}






