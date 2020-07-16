////////////////////////////////////////////////////////////////////////////////
// $Id: buffers.cpp 4924 2015-07-28 12:30:32Z mellinger $
// Authors: mellinger@neurotechcenter.org
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
#ifndef WIN32_BMP_FORMAT_H
#define WIN32_BMP_FORMAT_H

#include <iostream>
#include <vector>
#include <string>
#include <cstdint>

namespace PixelFormat { enum { RGB, RGBA }; }

namespace Win32 {
namespace Bmp {
// height > 0: begins with bottom left pixel (OpenGL-compatible)
// height < 0: begins with top left pixel
std::string Decode( std::istream&, int& width, int& height, std::vector<uint8_t>&, int = PixelFormat::RGBA );
std::string Encode( std::ostream&, int width, int height, const std::vector<uint8_t>& );
} // Bmp
} // Win32

#endif // WIN32_BMP_FORMAT_H

