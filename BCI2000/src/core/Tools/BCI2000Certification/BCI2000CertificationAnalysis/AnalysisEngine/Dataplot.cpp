////////////////////////////////////////////////////////////////////////////////
// $Id$
// Author: juergen.mellinger@neurotechcenter.org
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
#if _WIN32
# include <Shlwapi.h>
#endif

#include "Dataplot.h"
#include "FileUtils.h"
#include "ChildProcess.h"
#include "Streambuf.h"
#include "StaticObject.h"
#include <set>

namespace {

const char* sTerminalType = "qt";

std::string findGnuplot()
{
  std::string path;
#if _WIN32
  const char* extension = ".plt";
  const char* verb = "open";
  DWORD size = 0;
  ::AssocQueryStringA(ASSOCF_NOTRUNCATE, ASSOCSTR_EXECUTABLE, extension, verb, nullptr, &size);
  if(size > 0)
  {
    path.resize(size);
    HRESULT r = ::AssocQueryStringA(ASSOCF_NOTRUNCATE, ASSOCSTR_EXECUTABLE, extension, verb, const_cast<char*>(path.data()), &size);
    if(FAILED(r))
      path.clear();
    else
    {
      path.pop_back();
      path = FileUtils::ExtractDirectory(path) + "gnuplot.exe";
      if(!FileUtils::IsFile(path))
        path.clear();
    }
  }
#else
  int result = 0;
  std::ostringstream out;
  ProcessUtils::ExecuteSynchronously("which", "gnuplot", out, result);
  if(result == 0)
    path = StringUtils::Trim(out.str());
#endif
  return path;
}

class Gnuplot : private Lockable<>
{
public:
  Gnuplot() : mExecutable(findGnuplot()), mProcess(mExecutable) {}
  ~Gnuplot() {}
  bool found() const { return !mExecutable.empty(); }
  static std::string fromUtf8(const std::string& input)
  {
#if _WIN32
    int size = ::MultiByteToWideChar(CP_UTF8, 0, input.c_str(), -1, nullptr, 0);
    if(size < 1)
      return input;
    std::vector<wchar_t> wide(size);
    ::MultiByteToWideChar(CP_UTF8, 0, input.c_str(), -1, wide.data(), wide.size());
    char defaultChar = '?';
    size = ::WideCharToMultiByte(CP_ACP, 0, wide.data(), -1, nullptr, 0, &defaultChar, nullptr);
    std::vector<char> narrow(size);
    ::WideCharToMultiByte(CP_ACP, 0, wide.data(), -1, narrow.data(), narrow.size(), &defaultChar, nullptr);
    return std::string(narrow.data());
#else
    return input;
#endif
  }
  static std::string escapeSpecialChars(const std::string& input)
  {
    std::string output;
    for(auto c : input)
    {
      std::string s(1, c);
      switch(c)
      {
      case '\n':
        s = "\\n";
        break;
      case '\t':
        s = "\\t";
        break;
      case '_':
        s = "\\\\_";
        break;
      case '&':
        s = "\\\\&";
        break;
      case '"':
        s = "\\\"";
        break;
#if 0 // use double quotes to delimit strings instead
      case '\'':
        s = "''";
        break;
#endif
      }
      output += s;
    }
    return output;
  }
  bool execute(const std::string& s, std::string& error)
  {
    error.clear();
    auto stdout_ = mProcess.standardIO().Input();
    auto stderr_ = mProcess.standardError().Input();
    WithLock(this)
    {
      std::iostream io(&mProcess.standardIO());
      if(!mProcess.running())
      {
        mProcess.run();
        io << "set print \"-\"\n"
           << "set terminal " << sTerminalType
           << " title \"" << FileUtils::ApplicationTitle() << "\""
           << std::endl;
      }
      if(io.write(s.data(), s.length()) << "\nprint 0" << std::endl)
      {
        Waitables outputs;
        outputs.Add(stdout_).Add(stderr_);
        outputs.Wait();
        while(outputs.Wait(Time::FromMilliseconds(1)))
        {
          io.ignore(stdout_->Available());
          std::string buf(stderr_->Available(), ' ');
          stderr_->Read(const_cast<char*>(buf.data()), buf.size());
          error += buf;
        }
      }
    }
    return error.empty();
  }
  void exit()
  {
    if(!mProcess.running())
      return;
    WithTryLock(this)
      std::ostream(&mProcess.standardIO()) << "exit gnuplot" << std::endl;
    if(!mProcess.Wait(Time::Seconds(1)))
      ProcessUtils::KillPid(mProcess.pid());
  }
private:
  const std::string mExecutable;
  ChildProcess mProcess;
};
static StaticObject<Gnuplot> sGnuplot;
} // namespace

struct Dataplot::Private
{
  std::ostringstream mData;
  std::streampos mDataBegin;
  std::string mTitle;
  std::set<std::string> mVariables;
  int mWindowId;
  static int sWindowCount;

  Private() : mWindowId(-1) {}

  std::string makeVariableName(const std::string& base)
  {
    std::string name;
    int i = 0;
    std::ostringstream oss;
    do {
      oss.str("");
      oss << ++i;
      name = base + oss.str();
    } while(mVariables.find(name) != mVariables.end());
    mVariables.insert(name);
    return name;
  }

  bool display(const char* newWindowTitle, int w, int h, bool withPos, int x, int y, bool wait)
  {
    std::string windowTitle;
    if(newWindowTitle)
      windowTitle = Gnuplot::fromUtf8(newWindowTitle);
    else
      windowTitle = FileUtils::ApplicationTitle();
    std::string promptWindowTitle = windowTitle;
    if(wait)
      promptWindowTitle += " -- paused, press key to continue";

    std::string cmd;
    if(newWindowTitle)
      ++sWindowCount;

    mWindowId = sWindowCount;
    std::ostringstream term;
    term << sTerminalType << " " << mWindowId << " title \""
         << promptWindowTitle << "\"";
    if(w > 0 && h > 0)
      term << " size " << w << "," << h;
    if(withPos)
      term << " position " << x << "," << y;

    cmd += "set terminal " + term.str() + "\n";
    mData.flush();
    cmd += mData.str();
    if(wait)
    {
      std::ostringstream oss;
      oss << "pause mouse key\n";
      oss << "set terminal " << sTerminalType << " " << mWindowId << " title \""
          << windowTitle << "\"\n"
          << "replot\n";
      cmd += oss.str();
    }
    std::string error;
    bool ok = sGnuplot->execute(cmd, error);
    if(!ok)
      SuggestDebugging << "gnuplot error: " << error;
    return ok;
  }
};

int Dataplot::Private::sWindowCount = 0;

Dataplot::Dataplot(const std::string& title)
: p(new Private)
{
  p->mData << "reset\n";
  p->mTitle = Gnuplot::escapeSpecialChars(title);
  if(!p->mTitle.empty())
    p->mData << "set title \"" << Gnuplot::fromUtf8(p->mTitle) << "\"\n";
}

Dataplot::~Dataplot()
{
  delete p;
}

bool Dataplot::available()
{
  return sGnuplot->found();
}

void Dataplot::closeWindows()
{
  sGnuplot->exit();
}

std::string Dataplot::escapeSpecialChars(const std::string& s)
{
  return Gnuplot::escapeSpecialChars(s);
}

std::string
Dataplot::addPoints(const std::string& basename, const std::vector<std::pair<double, double>>& points)
{
  std::string name = p->makeVariableName(basename);
  p->mData << "$" << name << " << EOD\n";
  for(auto v : points)
    p->mData << v.first << " " << v.second << "\n";
  p->mData << "EOD\n";
  return name;
}

std::string
Dataplot::addData(const std::string& basename, const std::vector<double>& data, double scale)
{
  std::string name = p->makeVariableName(basename);
  p->mData << "$" << name << " << EOD\n";
  for(auto v : data)
    p->mData << v*scale << "\n";
  p->mData << "EOD\n";
  return name;
}

std::string
Dataplot::makePlotdef(const std::string& varname, const std::string& style, const std::string& title)
{
  return "$" + varname + " title \"" + (title.empty() ? varname : title) + "\" with " + style;
}

void
Dataplot::addCommand(const std::string& cmd)
{
  p->mData << Gnuplot::fromUtf8(cmd) << std::endl;
}

bool
Dataplot::display(bool wait)
{
  return p->display(nullptr, 0, 0, false, 0, 0, wait);
}

bool
Dataplot::display(const std::string& title, bool wait)
{
  return p->display(title.c_str(), 0, 0, false, 0, 0, wait);
}

bool
Dataplot::display(const std::string& title, int width, int height, bool wait)
{
  return p->display(title.c_str(), width, height, false, 0, 0, wait);
}

bool
Dataplot::display(const std::string& title, int width, int height, int xpos, int ypos, bool wait)
{
  return p->display(title.c_str(), width, height, true, xpos, ypos, wait);
}

