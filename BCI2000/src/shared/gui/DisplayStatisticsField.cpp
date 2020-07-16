////////////////////////////////////////////////////////////////////////////////
// $Id$
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A GraphObject intended for testing purposes.
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
#include "DisplayStatisticsField.h"
#include "DisplayWindow.h"
#include "ClassName.h"
#include "StringUtils.h"
#include <sstream>

DisplayStatisticsField::DisplayStatisticsField( GUI::GraphDisplay& display, int zOrder )
: TextField( display, zOrder ),
  mpWindow( dynamic_cast<GUI::DisplayWindow*>( &display ) )
{
  if( !mpWindow )
  {
    SetTextColor( RGBColor::Red );
    SetText( "Cannot display statistics for a " + ClassName( typeid( display ) ) );
  }
}

DisplayStatisticsField::~DisplayStatisticsField()
{
}

void
DisplayStatisticsField::OnChange(GUI::DrawContext& dc)
{
  if(mpWindow)
  {
    auto info = mpWindow->Info();
    if(!info.empty())
    {
      String vendor = info["GLVendor"], renderer = info["GLRenderer"];
      vendor = vendor.Trimmed();
      renderer = renderer.Trimmed();
      if(renderer.ToLower().find(vendor.ToLower()) == std::string::npos)
        renderer = vendor + " " + renderer;
      mInfo = "OpenGL v" + info["GLVersion"] + ", " + renderer;
    }
  }
  TextField::OnChange(dc);
}

void
DisplayStatisticsField::OnTick( const ClockTick& t )
{
  if( mpWindow )
  {
    auto s = mpWindow->FrameClockStatistics();
    double nominal = t.rate.Period().Seconds() * 1e3,
           interval = s.interval.Seconds() * 1e3,
           jitter = s.jitter.Seconds() * 1e3;
    std::ostringstream oss;
    if(!mInfo.empty())
      oss << mInfo << "\n";
    oss.precision( 2 );
    oss << std::fixed << "frame " << nominal << "ms / "
        << interval << " ± " << jitter << "ms, "
        << s.missed << " missed, ";
    if( s.type & ClockType::Hardware )
      oss << "vsync" ;
    else
      oss << "no vsync";
    oss << "\n";
    auto r = mpWindow->RenderStatistics();
    oss << "decim " << r.decimation
        << " render " << r.render.mean.Seconds() * 1e3
        << " ± " << r.render.std.Seconds() * 1e3 << "ms"
        << "\n";
    auto p = mpWindow->PresentationStatistics();
    oss << "presentations " << p.count << " late " << p.late;
    SetText( oss.str() );
  }
}

