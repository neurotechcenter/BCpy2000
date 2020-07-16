////////////////////////////////////////////////////////////////////////////////
// $Id$
// Author: juergen.mellinger@neurotechcenter.org
//
// $BEGIN_BCI2000_LICENSE$
// 
// This file is part of BCI2000, a platform for real-time bio-signal research.
// [ Copyright (C) 2000-2018: BCI2000 team and many external contributors ]
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
#ifndef DATAPLOT_H
#define DATAPLOT_H

#include <string>

class Dataplot
{
public:
  static bool available();
  static void closeWindows();
  static std::string escapeSpecialChars(const std::string&);

  typedef std::vector<std::pair<double, double>> PointList;

  Dataplot(const std::string& = "");
  ~Dataplot();
  std::string addPoints(const std::string& basename, const PointList&);
  std::string addData(const std::string& basename, const std::vector<double>& data, double yscale = 1.0);
  static std::string makePlotdef(
    const std::string& varname, 
    const std::string& style = "lines",
    const std::string& title = ""
  );
  void addCommand(const std::string&);
  bool display(bool wait = false);
  bool display(const std::string& windowTitle, bool wait = false);
  bool display(const std::string& windowTitle, int width, int height, bool wait = false);
  bool display(const std::string& windowTitle, int width, int height, int x, int y, bool wait = false);

private:
  Dataplot(const Dataplot&);
  Dataplot& operator=(const Dataplot&);

  struct Private;
  Private* p;
};

#endif // DATAPLOT_H
