/////////////////////////////////////////////////////////////////////////////
// $Id: EyetrackerLoggerTobii3.cpp 4731 2014-07-03 15:18:03Z mellinger $
// Authors: kaleb.goering@gmail.com, juergen.mellinger@neurotechcenter.org
// Description: The Eyetracker Logger logs incoming data from a Tobii
// Eyetracker and changes states
//
// Parameterization:
//   Eyetracker logging is enabled from the command line adding
//     --LogEyetrackerTobii3=1
//   As a command line option.
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
//   EyetrackerLeftPupilSize  - Left Eye Pupil Size (mm)
//   EyetrackerRightPupilSize - Right Eye Pupil Size (mm)
//   EyetrackerLeftEyePosZ    - Left Eye Distance from Screen (mm), Approx
//   EyetrackerRightEyePosZ   - Right Eye Distance from Screen (mm), Approx
//   EyetrackerTime           - The timestamp for the gaze data relative to BCI timer
//
// (C) 2000-2015, BCI2000 Project
// http://www.bci2000.org
/////////////////////////////////////////////////////////////////////////////
#include "EyetrackerLoggerTobii3.h"
#include "BCIEvent.h"
#include "TobiiNamespace.h"
#include "PhysicalUnit.h"
#include <iomanip>
#include <limits>

#define EVENTS \
    _(Time, 16, uint, T) \
//

#define PER_EYE_EVENTS \
  _(EyeValidity, 3, uint,) \
  _(PupilSize, 8, uint, Pupil) \
  _(EyeGazeX, 18, int, X) _(EyeGazeY, 18, int, Y) /* additional bits to allow for range violation */\
  _(EyePosX, 18, int, X) _(EyePosY, 18, int, Y) _(EyeDist, 14, int, Z) \
//

Extension( EyetrackerLoggerTobii3 );

namespace {
  static const double eps = std::numeric_limits<double>::epsilon();

  // currently, no vector algebra available in BCI2000 framework
  template<class T> double sq(const T& t) { return t*t; }
  struct vector3d { double dx, dy, dz; };
  vector3d operator-(const Tobii::Point3d& p1, const Tobii::Point3d& p2)
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
} // namespace

#define _(x, y, z, u) static const uint64_t bitmask_##x = (1ULL<<(y))-1;
    EVENTS PER_EYE_EVENTS
#undef _

struct EyetrackerLoggerTobii3::Private
{
  Private() : mpDevices(nullptr), mpDevice(nullptr) {}
  ~Private() { delete mpDevices; }
  static void DataCallback(int, Tobii::Arguments& args, void* data) { static_cast<Private*>(data)->onGazeData(args); }
  void onGazeData(const Tobii::GazeData&);
  void reportEyeData(std::ostream&, const char*, const Tobii::GazeData::EyeData&, const Tobii::GazeData::EyeData&);
  Tobii::Point3d tobiiUCStoScreenRelativeCoordinates(const Tobii::Point3d&);

  Tobii::DeviceList* mpDevices;
  Tobii::Device* mpDevice;

  Tobii::GazeData mPrevData;
  bool mFirstData;

  const ::State* mpSourceTimeState;
  const StateVector* mpStateVector;
  PhysicalUnit mUnitX, mUnitY, mUnitZ, mUnitPupil;
  Tobii::Point3d mScreenOrigin;
  // normalized to a length of 1e-3 to obtain data in plain meters
  vector3d mScreenXAxis, mScreenYAxis, mScreenZAxis;
};

Tobii::Point3d
EyetrackerLoggerTobii3::Private::tobiiUCStoScreenRelativeCoordinates(const Tobii::Point3d& p)
{
  vector3d s = p - mScreenOrigin;
  Tobii::Point3d r = { s*mScreenXAxis, s*mScreenYAxis, s*mScreenZAxis };
  return r;
}

EyetrackerLoggerTobii3::EyetrackerLoggerTobii3()
: p(nullptr)
{
}

EyetrackerLoggerTobii3::~EyetrackerLoggerTobii3()
{
  Halt();
  delete p;
}

void EyetrackerLoggerTobii3::Publish()
{
  delete p;
  p = nullptr;
  if(Parameters->Exists("LogEyetrackerTobii3"))
  {
    p = new Private;

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
}

void EyetrackerLoggerTobii3::AutoConfig()
{
  if(!p)
    return;

  delete p->mpDevices;
  p->mpDevices = nullptr;
  p->mpDevice = nullptr;
  if(Parameter("LogEyetrackerTobii3"))
  {
    p->mpDevices = new Tobii::DeviceList;
    if(p->mpDevices->IsEmpty())
      throw bcierr << "No eyetracker found.";
    auto d = p->mpDevices->ByIndex(0);
    if(!d->Connect())
      throw bcierr << "Unable to connect to eyetracker: " << d->Error().Message();
    if(!d->RegisterCallback(Tobii::OnGazeDataReceived, &Private::DataCallback, p))
      throw bcierr << "Failed to register callback: " << d->Error().Message();

    // collect some information for documentation purposes
    std::vector<std::pair<std::string, std::string>> info;
    std::string serialNumber, model, generation, firmwareVersion;
    if(d->GetUnitInfo(serialNumber, model, generation, firmwareVersion))
    {
      info.push_back(std::make_pair("SerialNumber", serialNumber));
      info.push_back(std::make_pair("Model", model));
      info.push_back(std::make_pair("Generation", generation));
      info.push_back(std::make_pair("FirmwareVersion", firmwareVersion));
    }
    std::string s;
    if(d->GetUnitName(s) && !s.empty())
      info.push_back(std::make_pair("UnitName", s));
    float f;
    if(d->GetFrameRate(f))
      info.push_back(std::make_pair("FrameRate", String() << f));
    if(d->GetIlluminationMode(s))
      info.push_back(std::make_pair("IlluminationMode", s));
    bool b;
    if(d->GetLowBlinkMode(b))
      info.push_back(std::make_pair("LowBlinkMode", b ? "on" : "off"));

    for(const auto& entry : info)
      if(!entry.first.empty())
        s += "\t" + entry.first + "\t" + entry.second + "\n";
    bciout << "Connected to Tobii eyetracker " << d->Id() << "\n" << s;

    info.insert(info.begin(), std::make_pair("Id", d->Id()));
    auto EyetrackerInfo = Parameter("EyetrackerInfo");
    EyetrackerInfo->SetNumColumns(1);
    EyetrackerInfo->SetNumRows(info.size());
    for(int row = 0; row < info.size(); ++row)
    {
      EyetrackerInfo->RowLabels()[row] = info[row].first;
      EyetrackerInfo(row) = info[row].second;
    }

    // set up transformation into screen-relative coordinate system
    Tobii::Point3d upperLeft, upperRight, lowerLeft;
    if(!d->GetXConfiguration(upperLeft, upperRight, lowerLeft))
      throw bcierr << "Failed to get screen coordinates: " << d->Error().Message();

    auto xAxis = upperRight - upperLeft,
         yAxis = lowerLeft - upperLeft,
         zAxis = crossprod(yAxis, xAxis); // left handed

    auto xLen = ::sqrt(sq(xAxis)), yLen = ::sqrt(sq(yAxis)), zLen = ::sqrt(sq(zAxis));
    if(xLen < eps || yLen < eps || zLen < eps)
      throw bcierr << "Invalid screen coordinates";

    // right-handed coordinate system with origin at upperLeft screen corner,
    // x axis pointing right, y axis pointing down, z axis pointing into screen
    p->mScreenOrigin = upperLeft;
    p->mScreenXAxis = xAxis * (1e-3/xLen);
    p->mScreenYAxis = yAxis * (1e-3/yLen);
    p->mScreenZAxis = zAxis * (1e-3/zLen);

    // backward compatibility:
    // raw x unit is 1/65535 screen widths, apply GazeScale/GazeOffset
    // raw y unit is 1/65535 screen heights, apply GazeScale/GazeOffset
    // raw z unit is 1mm, with direction inverted
    float gazeOffset = Parameter("GazeOffset"),
          gazeScale = Parameter("GazeScale");

    double xyRange = 65536;
    p->mUnitX.SetGain(xLen*1e-3/xyRange/gazeScale).SetSymbol("m");
    p->mUnitX.SetOffset(gazeOffset);
    p->mUnitX.SetRawMin(0).SetRawMax(xyRange - 1);

    p->mUnitY.SetGain(yLen*1e-3/xyRange/gazeScale).SetSymbol("m");
    p->mUnitY.SetOffset(gazeOffset);
    p->mUnitY.SetRawMin(0).SetRawMax(xyRange - 1);

    p->mUnitZ.SetGainWithSymbol("1mm");
    p->mUnitZ.SetRawMin(0).SetRawMax(2e3);

    p->mUnitPupil.SetGainWithSymbol("0.1mm");
    p->mUnitPupil.SetRawMin(0).SetRawMax(100);

    // physical unit documentation
    PhysicalUnit timeMs;
    timeMs.SetGainWithSymbol("1ms");
    PhysicalUnit none;

    const PhysicalUnit
      *puX = &p->mUnitX, *puY = &p->mUnitY, *puZ = &p->mUnitZ,
      *puPupil = &p->mUnitPupil, *puT = &timeMs, *pu = &none;
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
    p->mpDevice = d;
  }
}

void EyetrackerLoggerTobii3::Preflight() const
{

}

void EyetrackerLoggerTobii3::Initialize()
{
  if(p && p->mpDevice)
  {
    p->mpSourceTimeState = &States->ByName("SourceTime");
    p->mpStateVector = Statevector;
  }
}

void EyetrackerLoggerTobii3::StartRun()
{
  if(p)
  {
    p->mFirstData = true;
    if(p->mpDevice && !p->mpDevice->StartTracking())
      throw bcierr << "Could not start tracking";
  }
}

void EyetrackerLoggerTobii3::StopRun()
{
  if(p && p->mpDevice && !p->mpDevice->StopTracking())
    throw bcierr << "Could not stop tracking";
}

void EyetrackerLoggerTobii3::Halt()
{
  StopRun();
}

void EyetrackerLoggerTobii3::Private::onGazeData(const Tobii::GazeData& data)
{
  PrecisionTime blockTime = mpStateVector->StateValue(mpSourceTimeState->Location(), mpSourceTimeState->Length());
  PrecisionTime eyetrackerTime;
  if(mpDevice->ClockSyncState() == Tobii::ClockStatusSynchronized)
    eyetrackerTime = mpDevice->ClockRemoteToLocal(data.timestamp) / 1000;
  else
    eyetrackerTime = PrecisionTime::Now();
  PrecisionTime eventTime = eyetrackerTime;
  if(PrecisionTime::SignedDiff(eventTime, blockTime) < 0)
    eventTime = PrecisionTime::Now();

  BCIEvent event(eventTime);
  // Typically, eyetracker data will be delayed by more than half a second.
  // For high precision data analysis, it is necessary to use the eye tracker's
  // time stamp rather than the data position in the BCI2000 file.
  event << "EyetrackerTime " << (eyetrackerTime & bitmask_Time) << std::flush;
  reportEyeData(event, "Left", data.left, mPrevData.left);
  reportEyeData(event, "Right", data.right, mPrevData.right);
  mPrevData = data;
  mFirstData = false;
}

void EyetrackerLoggerTobii3::Private::reportEyeData(
  std::ostream& event, const char* eye,
  const Tobii::GazeData::EyeData& data,
  const Tobii::GazeData::EyeData& prevData)
{
  std::string prefix = "Eyetracker";
  prefix += eye;

  event << prefix << "EyeValidity " << data.validity << std::flush;
  if(data.validity >= 2)
    return;

  bool reportGaze = true,
       reportEyePos = true,
       reportPupilSize = true;
  reportGaze &= mFirstData || sq(data.gazePoint3d - prevData.gazePoint3d) > eps;
  reportEyePos &= mFirstData || sq(data.eyePosition3d - prevData.eyePosition3d) > eps;
  reportPupilSize &= mFirstData || sq(data.pupilDiameter - prevData.pupilDiameter) > eps;

  if(reportGaze)
  {
    auto gazePoint = tobiiUCStoScreenRelativeCoordinates(data.gazePoint3d);
    int32_t eyeGazeX = mUnitX.PhysicalToRawValue(gazePoint.x),
            eyeGazeY = mUnitY.PhysicalToRawValue(gazePoint.y);
    event << prefix << "EyeGazeX " << (eyeGazeX & bitmask_EyeGazeX) << std::flush;
    event << prefix << "EyeGazeY " << (eyeGazeY & bitmask_EyeGazeY) << std::flush;
    Assert(gazePoint.z*gazePoint.z < eps); // always zero in screen relative system
  }
  if(reportEyePos)
  {
    auto eyePos = tobiiUCStoScreenRelativeCoordinates(data.eyePosition3d);
    int32_t eyePosX = mUnitX.PhysicalToRawValue(eyePos.x),
            eyePosY = mUnitY.PhysicalToRawValue(eyePos.y),
            eyePosZ = mUnitZ.PhysicalToRawValue(eyePos.z);
    event << prefix << "EyePosX " << (eyePosX & bitmask_EyePosX) << std::flush;
    event << prefix << "EyePosY " << (eyePosY & bitmask_EyePosY) << std::flush;
    event << prefix << "EyeDist " << (eyePosZ & bitmask_EyeDist) << std::flush;
  }
  if(reportPupilSize)
  {
    int32_t pupilSize = mUnitPupil.PhysicalToRawValue(data.pupilDiameter*1e-3);
    event << prefix << "PupilSize " << (pupilSize & bitmask_PupilSize) << std::flush;
  }
}
