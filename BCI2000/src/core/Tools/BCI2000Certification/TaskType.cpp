////////////////////////////////////////////////////////////////////////////////
// $Id: TaskType.cpp 5826 2018-11-23 14:14:08Z mellinger $
// Authors: Adam Wilson, juergen.mellinger@neurotechcenter.org
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
#include "TaskType.h"
#include <iostream>
#include <string>
#include <map>
#include "FileUtils.h"
#include "Debugging.h"

TaskType::TaskType()
: Property::Holder<TaskType>(this)
{
  setValidator(Name, &TaskType::validateName);

  setValidator(AmpChannel, &TaskType::validateChannel);
  setValidator(AmpStream, &TaskType::validateStream);
  setValidator(VidChannel, &TaskType::validateChannel);
  setValidator(VidStream, &TaskType::validateStream);
  setValidator(AudChannel, &TaskType::validateChannel);
  setValidator(AudStream, &TaskType::validateStream);

  setValidator(VidStateName, &TaskType::validateStateName);
  setValidator(AudStateName, &TaskType::validateStateName);
  setValidator(VidStateValues, &TaskType::validateStateValues);
  setValidator(AudStateValues, &TaskType::validateStateValues);

  setValidator(BlockSize, &TaskType::validateBlockSize);
  setValidator(SampleRate, &TaskType::validateSampleRate);

  setValidator(Source, &TaskType::validateExecutable);
  setValidator(SigProc, &TaskType::validateExecutableNonempty);
  setValidator(App, &TaskType::validateExecutableNonempty);

  Skip.setValue(false); // make sure it is not empty
}

TaskType::~TaskType()
{
}

bool TaskType::operator==(const TaskType& other) const
{
  return Property::Holder<TaskType>::operator==(other);
}

void TaskType::validateName(const String* name)
{
  if(!name || name->empty())
    validationError() << "task name may not be empty";
  else if(name->find_first_of(" \t\n\r") != std::string::npos)
    validationError() << "task name may not contain white space";
}

void TaskType::validateChannel(const int* ch)
{
  int prevVal = 0;
  bool isempty = !ch, wasempty = true;

  bool dup = false;
  Property::Property<int>* channelFields[] =
  { &AmpChannel, &AudChannel, &VidChannel, };
  for(auto p : channelFields)
  {
    if(p == validand() && !p->empty())
      prevVal = p->value(), wasempty = false;
    else if(!p->empty() && ch && p->value() == *ch)
      dup = true;
  }
  if(ch && *ch <= 0)
    validationError() << "channel value must be > 0";
  else if(dup)
    validationError() << "channel values must be unique";
  if(isempty != wasempty || (!isempty && !wasempty && prevVal != *ch))
    for(auto p : channelFields)
      p->revalidateLater();
}

void TaskType::validateStream(const String* pName)
{
  String prevVal;
  bool isempty = !pName, wasempty = true;

  bool dup = false;
  Property::Property<String>* streamFields[] =
  { &AmpStream, &AudStream, &VidStream, };
  for(auto p : streamFields)
  {
    if(p == validand() && !p->empty())
      prevVal = p->value(), wasempty = false;
    else if(!p->empty() && pName && p->value() == *pName)
      dup = true;
  }
  if(pName && pName->find_first_of(" \t\n\r") != std::string::npos)
    validationError() << "stream names may not contain spaces";
  else if(pName && !pName->empty() && !::isalpha(pName->front()))
    validationError() << "stream names must begin with a letter";
  else if(dup)
    validationError() << "stream names must be unique";
  if(isempty != wasempty || (!isempty && !wasempty && prevVal != *pName))
    for(auto p : streamFields)
      p->revalidateLater();
}

void TaskType::validateStateName(const String* pName)
{
  int valueState = Property::EmptyValid;
  Property::BasicProperty* pCompanion = nullptr;
  if(validand() == &AudStateName)
    pCompanion = &AudStateValues;
  else if(validand() == &VidStateName)
    pCompanion = &VidStateValues;

  if((!pName || pName->empty()) && pCompanion && !pCompanion->empty())
    validationError() <<  "state name is required when state values present";
  else if(pName && pName->find_first_of(" \t\n\r") != std::string::npos)
    validationError() << "state names may not contain spaces";
  else if(pName && !pName->empty() && !::isalpha(pName->front()))
    validationError() << "state names must begin with a letter";
  if(pCompanion && pName && *pName != validand()->toString())
    pCompanion->revalidateLater();
}

void TaskType::validateStateValues(const List<int>* values)
{
  Property::BasicProperty* pCompanion = nullptr;
  if(validand() == &AudStateValues)
    pCompanion = &AudStateName;
  else if(validand() == &VidStateValues)
    pCompanion = &VidStateName;

  if((!values || values->empty()) && pCompanion && !pCompanion->empty())
    validationError() << "state values are required when state name present";
  if(pCompanion)
    pCompanion->revalidateLater();
}

void TaskType::validateSampleRate(const float* val)
{
  if (!val || !(*val > 0))
    validationError() << "sample rate must be > 0";
}

void TaskType::validateBlockSize(const int* val)
{
  if (!val || *val <= 0)
    validationError() << "block size must be > 0";
}

void TaskType::validateExecutableNonempty(const String* val)
{
  if (!val || *val == "") {
    validationError() << "executable name must not be empty";
    return;
  }
  validateExecutable(val);
}

void TaskType::validateExecutable(const String* val)
{
  if(!val || *val == "")
    return;
  auto parent = parentProperty();
  if (parent) {
    auto pConfig = dynamic_cast<Config*>(parent->holder());
    if (pConfig && !pConfig->isValidExecutable(*val)) {
      validationError() << "\"" << *val << "\" is not an executable in the prog directory";
      return;
    }
  }
}

bool TaskType::hasErrors() const
{
  for(auto p : properties())
    if(!p->valid())
      return true;
  return false;
}

void TaskType::addParm(const String& str)
{
  std::string s = str;
  size_t pos = std::string::npos;
  while((pos = s.find('\\')) != std::string::npos)
    s = s.substr(0, pos) + "/" + str.substr(pos + 1);
  auto parmFiles = ParmFiles.value();
  parmFiles.push_back(s);
  ParmFiles.setValue(parmFiles);
  pos = str.find_last_of("/");
  if (pos != std::string::npos)
    mParmFilesDisp.push_back(s.substr(pos + 1));
  else
    mParmFilesDisp.push_back(s);
}

void TaskType::delParm(int id)
{
  auto parmFiles = ParmFiles.value();
  if (id < 0 || id >= parmFiles.size())
    return;
  parmFiles.erase(parmFiles.begin() + id);
  ParmFiles.setValue(parmFiles);
  mParmFilesDisp.erase(mParmFilesDisp.begin() + id);
}

std::ostream&
TaskType::serialize(std::ostream& out) const
{
  out << "name " << Name.toString() << "\n";
  if (!Skip)
    out << "%";
  out << "skip"<<"\n";
  if (!AmpChannel.empty())
    out << "amp " << AmpChannel.toString() << "\n";
  if (!AmpStream.empty())
    out << "ampstream " << AmpStream.toString() << "\n";
  if (!VidChannel.empty())
    out << "vid " << VidChannel.toString() << " " << VidStateName.toString()
        << " " << VidStateValues.toString() << "\n";
  if (!AudChannel.empty())
    out << "aud " << AudChannel.toString() << " " << AudStateName.toString()
        << " " << AudStateValues.toString() << "\n";
  if (!Source.empty())
    out << "source " << Source.toString() << "\n";
  if (!SigProc.empty())
    out << "sigproc " << SigProc.toString() << "\n";
  if (!SampleRate.empty())
    out << "samplingrate " << SampleRate.toString() << "\n";
  if (!BlockSize.empty())
    out << "blocksize " << BlockSize.toString() << "\n";
  if (!App.empty())
    out << "app " << App.toString() << "\n";
  for (int j = 0; j < ParmFiles.value().size(); j++)
    out << "parm " << ParmFiles.value()[j] << "\n";
  if (VidFrameTest.value())
    out << "vidframetest 1\n";
  out << "end" << "\n\n";
  return out;
}

std::istream&
TaskType::unserialize(std::istream& is)
{
  *this = TaskType();
  List<Tiny::String> parms;

  std::string line, token;
  bool atBegin = true, atEnd = false;
  while(!atEnd && std::getline(is, line))
  {
    std::istringstream iss(line);
    if(!(iss >> token) || token.front() == '%')
      continue;
    token = StringUtils::ToLower(token);
    List<std::string> tokens;
    
    if (token == "name")
    {
      if(atBegin && iss >> token)
        Name.set(token);
      else
        is.setstate(std::ios::failbit);
      atBegin = false;
    }
    else if (token == "end")
      atEnd = true;
    else if (token == "parms" && iss >> tokens)
      parms.insert(parms.end(), tokens.begin(), tokens.end());
    else if (token == "amp" && iss >> token)
      AmpChannel.set(token);
    else if (token == "ampstream" && iss >> token)
      AmpStream.set(token);
    else if (token == "vid" && iss >> token)
    {
      VidChannel.set(token);
      if(iss >> token)
        VidStateName.set(token);
      if(std::getline(iss, token))
        VidStateValues.set(token);
    }
    else if (token == "aud" && iss >> token)
    {
      AudChannel.set(token);
      if(iss >> token)
        AudStateName.set(token);
      if(std::getline(iss, token))
        AudStateValues.set(token);
    }
    else if (token == "source" && iss >> token)
      Source.set(token);
    else if (token == "sigproc" && iss >> token)
      SigProc.set(token);
    else if (token == "app" && iss >> token)
      App.set(token);
    else if (token == "parm" && iss >> token)
      parms.push_back(token);
    else if (token == "vidframetest" && iss >> token)
      VidFrameTest.set(token);
    else if (token == "samplingrate" && iss >> token)
      SampleRate.set(token);
    else if (token == "blocksize" && iss >> token)
      BlockSize.set(token);
    else if (token == "skip")
      Skip.setValue(true);
    else
      is.setstate(std::ios::failbit);
  }
  for (const auto& parm : parms)
    addParm(parm);
  return is;
}

std::string
TaskType::toInputScript() const
{
    String s =
      "AmpState:=state('SourceTime');\n";
    if(AudStateName.empty())
      s += "AudState:=stream(\"\");\n";
    else
      s += "AudState:=state(\'" + AudStateName.value() + "\');\n";
    if (VidStateName.empty())
      s += "VidState:=stream(\"\");\n";
    else
      s += "VidState:=state(\'" + VidStateName.value() + "\');\n";
    s += "AmpTrigger:=computed_time_series(diff(AmpState())!=0);\n";
    if(AudStateValues.empty())
      s += "AudTrigger:=computed_time_series(AudState()!=0);\n";
    else
      s += "AudTrigger:=computed_time_series(AudState()==" + StringUtils::Join("||AudState()==", AudStateValues.value()) + ");\n";
    s +=
      "AmpChannel:=channel('AmpChannel');\n"
      "AudChannel:=channel('AudChannel');\n"
      "AmpStream:=stream(\"" + AmpStream.value() + "\");\n"
      "AudStream:=stream(\"" + AudStream.value() + "\");\n"
    ;
    if (VidStateValues.empty())
      s += "VidTrigger:=computed_time_series(VidState()!=0);\n";
    else
      s += "VidTrigger:=computed_time_series(VidState()==" + StringUtils::Join("||VidState()==", VidStateValues.value()) + ");\n";
    s +=
      "VidChannel:=channel('VidChannel');\n"
      "VidStream:=stream(\"" + VidStream.value() + "\");\n"
      ;
    return s;
}

////////////////////////

Config::Config()
: Property::Holder<Config>(this)
{
  setValidator(&TaskType::Name, &Config::validateTaskName);
  setValidator(Tasks, &Config::validateTasks);
  setValidator(RootDir, &Config::validateRootDir);
  setValidator(DataDir, &Config::validateDataDir);
  setValidator(DefaultSource, &Config::validateExecutable);
  setValidator(WindowWidth, &Config::validateWindowSize);
  setValidator(WindowHeight, &Config::validateWindowSize);
}

Config::~Config()
{
}

TaskType*
Config::taskByName(const String& name)
{
  for(auto& task : Tasks.value())
    if(task.Name.value() == name)
      return &task;
  return nullptr;
}

void Config::remove(TaskType* p)
{
  auto i = Tasks.value().begin();
  while(i != Tasks.value().end() && (&*i != p))
    ++i;
  Assert(i != Tasks.value().end());
  if(i != Tasks.value().end())
    Tasks.value().erase(i);
}

void Config::move(TaskType* p, int delta)
{
  int i = 0;
  while(i < Tasks.value().size() && &Tasks.value()[i] != p)
    ++i;
  Assert(i != Tasks.value().size());
  Assert(i + delta >= 0);
  Assert(i + delta < Tasks.value().size());
  while (delta > 0) {
    Tasks.value().swap(i, i + 1);
    --delta;
    ++i;
  }
  while (delta < 0) {
    Tasks.value().swap(i, i - 1);
    ++delta;
    --i;
  }
}

bool Config::readFromFile(const String& fname)
{
  mFileName.clear();
  DefaultSource.set("");
  Tasks.clear();

  std::ifstream file(fname);
  if(!file.is_open())
    return false;

  //setup the initial tasks
  std::string line;
  bool exportData = false;

  //go through each line of the ini and parse it

  while (std::getline(file, line))
  {
    std::istringstream iss(line);
    std::string token;
    if(!(iss >> token) || token.front() == '%')
      continue;
    token = StringUtils::ToLower(token);

    //this is a global signal source, which will be set for any task that does not
    //specify one; individual tasks will overwrite this
    if (token == "source" && iss >> token)
      DefaultSource.set(token);
    else if (token == "export")
      exportData = true;
    //go through and check out the possible acceptable tokens, and assign the values accordingly
    else if (token == "name" && iss >> token)
    {
      std::string taskdef = "name " + token + "\n";
      bool atEnd = false;
      while(!atEnd && std::getline(file, line))
      {
        taskdef += line + "\n";
        atEnd = (StringUtils::Trim(line) == "end");
      }
      TaskType curTask;
      std::istringstream iss(taskdef);
      if(!(iss >> curTask))
        return false;
      Tasks.value().push_back(curTask);
      Tasks.value().back().revalidate();
    }
  }
  mFileName = fname;
  return true;
}

bool Config::writeToFile(const Tiny::String& fname)
{
  std::ofstream out(fname);
  if(!out.is_open())
    return false;

  //first write the global setting if it exists
  if (!DefaultSource.empty())
    out << "source " << DefaultSource.toString() << "\n\n";

  for (const auto& task : Tasks.value())
    out << task;

  if(out)
    mFileName = fname;
  return !!out;
}

void
Config::validateTasks(const Property::Container<TaskType>*)
{
  std::map<std::string, std::vector<TaskType*>> names;
  for(auto& task : Tasks.value())
    names[task.Name].push_back(&task);
  for(auto& entry : names)
    if(entry.second.size() > 1)
      for(auto pTask : entry.second)
        pTask->Name.setError("duplicate task name");
}

void
Config::validateTaskName(const String* pName)
{
  Tasks.revalidateLater();
}

void
Config::validateRootDir(const String* dir)
{
  if(!dir || dir->empty())
    validationError() << "must specify a directory";
  else if(!FileUtils::IsDirectory(*dir))
    validationError() << "directory does not exist";
  else
  {
    const char* required[] = { "prog", "tools/BCI2000Certification" };
    for(auto r : required)
    {
      if(!FileUtils::IsDirectory(*dir + "/" + r))
      {
        validationError() << "directory does not contain a \"" + std::string(r) + "\" subdirectory";
        break;
      }
    }
  }
}

void
Config::validateDataDir(const String* dir)
{
  if(!dir || dir->empty())
    validationError() << "must specify a directory";
  else if(!FileUtils::IsDirectory(*dir + "/.."))
    validationError() <<  "directory's parent does not exist";
  else if(!FileUtils::IsWritableDirectory(*dir + "/.."))
    validationError() <<  "directory's parent is read-only";
}

bool
Config::isValidExecutable(const String& file) const
{
  auto exe = RootDir.value() + "/prog/" + file;
#if _WIN32
  if (!FileUtils::IsFile(exe) && StringUtils::ToLower(FileUtils::ExtractExtension(exe)) != ".exe")
    exe += ".exe";
#endif
  if (!FileUtils::IsFile(exe))
    return false;
  return true;
}

void
Config::validateExecutable(const String* file)
{
  if(file && !file->empty())
  {
    if(!isValidExecutable(*file))
      validationError() << "file does not exist in prog directory";
  }
}

void
Config::validateWindowSize(const int* val)
{
  if(val && *val < 0)
    validationError() << "must be >= 0";
}

bool Config::tasksModified() const
{
  if(mFileName.empty())
    return !Tasks.empty();

  Config temp;
  if(!temp.readFromFile(mFileName))
    return true;

  if(temp.DefaultSource != DefaultSource)
    return true;

  return temp.Tasks != Tasks;
}

int Config::activeTasks() const
{
  int count = 0;
  for(const auto& task : Tasks.value())
    if(task.valid() && !task.Skip)
      ++count;
  return count;
}

