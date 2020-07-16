////////////////////////////////////////////////////////////////////////////////
// $Id: Settings.cpp 4659 2013-12-03 16:22:46Z mellinger $
// Authors: juergen.mellinger@uni-tuebingen.de
// Description: A descendant of the Qt QSettings class with a default
//   constructor that reads/writes from/to an ini file in the application
//   directory.
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
#include "Settings.h"
#include "FileUtils.h"
#include "StaticObject.h"

using namespace std;

namespace
{

string
GetFilePath()
{
  return FileUtils::InstallationDirectory()
         + FileUtils::ExtractBase( FileUtils::ExecutablePath() )
         + ".ini";
}

StaticBuffer<string, &GetFilePath> FilePath;

} // namespace

Settings::Settings()
: QSettings( QString::fromLocal8Bit( FilePath().c_str() ), QSettings::IniFormat )
{
}

