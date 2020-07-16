/*
Authors: Adam Wilson, University of Wisconsin-Madison
         juergen.mellinger@neurotechcenter.org

$BEGIN_BCI2000_LICENSE$

This file is part of BCI2000, a platform for real-time bio-signal research.
[ Copyright (C) 2000-2012: BCI2000 team and many external contributors ]

BCI2000 is free software: you can redistribute it and/or modify it under the
terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later
version.

BCI2000 is distributed in the hope that it will be useful, but
                        WITHOUT ANY WARRANTY
- without even the implied warranty of MERCHANTABILITY or FITNESS FOR
A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program.  If not, see <http://www.gnu.org/licenses/>.

$END_BCI2000_LICENSE$
*/
//---------------------------------------------------------------------------
#include "CertLauncher.h"
#include "ParamList.h"
#include "ParamRef.h"
#include "FileUtils.h"
#include "EncodedString.h"
#include "ProcessUtils.h"
#include "Thread.h"
#include "ThreadUtils.h"
#include "TimeUtils.h"
#include <sstream>

#include "TaskType.h"

namespace {
  class ExternalProcess : public Waitable, private Runnable
  {
  public:
    ExternalProcess(const std::string& exe = "", const std::string& args = "", const std::string& wd = "" )
    : mThread(this, "External Process"), mResult(0), mExecutable(exe), mArguments(args),
      mWorkingDirectory(wd.empty() ? FileUtils::WorkingDirectory() : wd)
    { Waitable::AssociateWith(mThread); }
    ~ExternalProcess() {}
    ExternalProcess& setExecutable(const std::string& exe) { mExecutable = exe; return *this; }
    const std::string& executable() const { return mExecutable; }
    ExternalProcess& setArguments(const std::string& args) { mArguments = args; return *this; }
    const std::string& arguments() const { return mArguments; }
    ExternalProcess& setWorkingDirectory(const std::string& wd) { mWorkingDirectory = wd; return *this; }
    const std::string& workingDirectory() const { return mWorkingDirectory; }

    bool start(Time::Interval timeout)
    {
      if(mThread.Running())
        return false;
      Time started = TimeUtils::MonotonicTime();
      mThread.Start();
      while(!mThread.Running() && TimeUtils::MonotonicTime() - started < timeout)
        ThreadUtils::Idle();
      ThreadUtils::SleepFor(timeout - (TimeUtils::MonotonicTime() - started));
      return mThread.Running();
    }
    bool running() const { return mThread.Running(); }
    void kill() { mThread.Terminate(); }
    int result() const { return mResult; }
    std::string output() const { return mStdout.str(); }
    std::string errorOutput() const { return mStderr.str(); }
  protected:
    void OnRun() override
    {
      mResult = -1;
      mStdout.str("");
      mStdout.clear();
      mStderr.str("");
      mStderr.clear();
      FileUtils::ChangeDirectory(mWorkingDirectory);
      if(!ProcessUtils::ExecuteSynchronously(mExecutable, mArguments, mStdout, mStderr, mResult))
      {
        mResult = -1;
        mStderr << SysError().Message();
      }
    }
  private:
    Thread mThread;
    int mResult;
    std::string mExecutable, mArguments, mWorkingDirectory;
    std::ostringstream mStdout, mStderr;
  };
}

struct CertLauncher::Private : Runnable
{
  Private(CertLauncher* p) : mpSelf(p), mThread(this, "task launcher thread") {}
  void OnRun() override;
  void runTask(const TaskType&, const Config&);
  bool systemExecute(const Tiny::String&);
  Thread mThread;
  CertLauncher* mpSelf;
  const Config* mpConfig;
};

CertLauncher::CertLauncher()
: p(new Private(this))
{
}

CertLauncher::~CertLauncher()
{
  delete p;
}

bool
CertLauncher::run(const Config* pTasks)
{
  if(p->mThread.Running())
    return false;
  p->mpConfig = pTasks;
  p->mThread.Start();
  return true;
}

void
CertLauncher::kill()
{
  p->mThread.Terminate();
}

void
CertLauncher::Private::OnRun()
{
  MessageArg arg = {};
  arg.remaining = 0;
  for(const auto& task : mpConfig->Tasks.value())
    if(!task.Skip)
      ++arg.remaining;
  if(systemExecute(mpConfig->ExecuteBefore))
  {
    for(const auto& task : mpConfig->Tasks.value())
    {
      if(!task.Skip)
      {
        arg.pTask = &task;
        mpSelf->Emit(onTaskBegin, &arg);
        if(mThread.Terminating())
          break;
        arg.error.clear();
        try {
          runTask(task, *mpConfig);
          mpSelf->Emit(onTaskEnd, &arg);
        } catch(const std::exception& e) {
          arg.error = e.what();
          mpSelf->Emit(onTaskEnd, &arg);
        }
        --arg.remaining;
        if(mThread.Terminating())
          break;
      }
    }
    systemExecute(mpConfig->ExecuteAfter);
  }
  arg.pTask = nullptr;
  mpSelf->Emit(onDone, &arg);
}

bool
CertLauncher::Private::systemExecute(const Tiny::String& command)
{
  MessageArg arg = {};
  if(command)
  {
    int result = 0;
    std::ostringstream out;
    if(!ProcessUtils::SystemExecuteSynchronously(command, out, result) || result != 0)
    {
      arg.error = "Could not execute '" + command + "'";
      if(!out.str().empty())
        arg.error += ":\n" + out.str();
      mpSelf->Emit(onError, &arg);
      return false;
    }
  }
  return true;
}

void CertLauncher::Private::runTask(const TaskType& curTask, const Config& config)
{  
  std::vector<std::string> parmfiles;
  parmfiles.push_back("CertificationMain.prm");
  for (int i = 0; i < curTask.ParmFiles.value().size(); i++)
    parmfiles.push_back(curTask.ParmFiles.value()[i]);
  FileUtils::TemporaryFile tempfile(".prm");
  for (const auto& file : parmfiles)
  {
    std::string name = config.RootDir.value() + "/tools/BCI2000Certification/parms/" + file;
    std::ifstream stream(name, std::ios::binary);
    if(!stream.is_open())
      throw std_runtime_error << "Could not load parameter file: " << name;
    std::string line;
    while(std::getline(stream >> std::ws, line, '\n'))
      if(!line.empty())
        tempfile << line << (line.back() == '\r' ? "\n" : "\r\n");
  }
  tempfile.flush();
  ParamList params;
  if(!params.Load(tempfile.Name()))
    throw std_runtime_error << "Could not load temporary parameter file: " << tempfile.Name();

  Param channelNamesParam = Param::fromDefinition("source stringlist ChannelNames= 0 //");
  MutableParamRef ChannelNames( &channelNamesParam );
  ParamRef SourceCh( &params["SourceCh"] );
  ChannelNames->SetNumValues( SourceCh );
  for( int i = 0; i < ChannelNames->NumValues(); ++i )
    ChannelNames( i ) << "Ch" << i + 1;

  const Property::Property<int>* p[] =
  {
    &curTask.AmpChannel, 
    &curTask.AudChannel, &curTask.VidChannel, 
  };

  int maxChannel = -1;
  for(auto property : p)
    if(!property->empty())
      maxChannel = std::max(maxChannel, property->value());

  if(SourceCh < maxChannel)
    throw std_runtime_error << "SourceCh must be at least " << maxChannel << ", aborting";

  for(auto property : p)
    if(!property->empty())
      ChannelNames(property->value() - 1) = property->name();

  tempfile << channelNamesParam << "\r\n";
  if( !tempfile )
    throw std_runtime_error << "Could not write temporary parameter file: " << tempfile.Name();
  tempfile.Close();

  std::string progPath = config.RootDir.value() + "/prog/";
  std::string opName;
  const char* operatorNames[] = { "operator.exe", "operator", "operat.exe", };
  for(auto p : operatorNames)
  {
    if(FileUtils::IsFile(progPath + p))
    {
      opName = p;
      break;
    }
  }
  if(opName.empty())
    throw std_runtime_error << "Could not find operator executable in " << progPath;

  ExternalProcess operatorModule(progPath + opName);
  std::ostringstream oss;
  oss << ""
      << "--local --OnConnect \"-LOAD PARAMETERFILE " << EncodedString(tempfile.Name()) << ";"
      << "SET TITLE " << curTask.Name.value() << ";";
  if (config.AutoCfg.value())
    oss << " SETCONFIG;\"";
  else
    oss << "\"";

  if (config.AutoStart.value())
    oss << " --OnSetConfig \"-SET STATE Running 1;\""; 

  if (config.AutoQuit.value())
    oss << " --OnSuspend \"-Quit;\"";
  operatorModule.setArguments(oss.str());
  if(!operatorModule.start(Time::Interval::Seconds(2)))
    throw std_runtime_error << "Could not start operator module: " << operatorModule.errorOutput();

  int result;
  oss.str("");
  oss.clear();
  oss << "--local";
  if(!ProcessUtils::ExecuteAsynchronously(progPath + curTask.SigProc.value(), oss.str(), result))
    throw std_runtime_error << "Could not start signal processing module: " << SysError().Message();

  oss.str("");
  oss.clear();
  oss << "--local";
  if (!config.WindowLeft.empty())
    oss << " --WindowLeft=" + config.WindowLeft.toString();
  if (!config.WindowTop.empty())
    oss << " --WindowTop=" + config.WindowTop.toString();
  if (!config.WindowWidth.empty())
    oss << " --WindowWidth=" + config.WindowWidth.toString();
  if (!config.WindowHeight.empty())
    oss << " --WindowHeight=" + config.WindowHeight.toString();
  oss << " --AVPlayToEnd=0";
  oss << " " << config.AppAddArguments.value();
  if(!ProcessUtils::ExecuteAsynchronously(progPath + curTask.App.value(), oss.str(), result))
    throw std_runtime_error << "Could not start application module: " << SysError().Message();

  std::string source = curTask.Source.empty() ? config.DefaultSource.value() : curTask.Source.value();
  oss.str("");
  oss.clear();
  oss << "--local \"--SubjectName=" << curTask.Name.value() << "\"";
  oss << " --TaskDef=";
  std::ostringstream taskdef;
  taskdef << curTask;
  StringUtils::WriteAsBase64(oss, taskdef.str());
#if 0
  oss << " --CertInputScript=";
  StringUtils::WriteAsBase64(oss, curTask.toInputScript());
#endif
  if (!curTask.SampleRate.empty())
    oss << " --SamplingRate=" + curTask.SampleRate.toString();

  if (!curTask.BlockSize.empty())
    oss << " --SampleBlockSize=" + curTask.BlockSize.toString();

  if (!config.DataDir.empty())
    oss << " \"--DataDirectory=" << config.DataDir.value() << "\"";
  if(!ProcessUtils::ExecuteAsynchronously(progPath + source, oss.str(), result))
    throw std_runtime_error << "Could not start source module: " << SysError().Message();

  operatorModule.Wait();
}

