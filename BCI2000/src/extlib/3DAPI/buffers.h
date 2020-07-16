////////////////////////////////////////////////////////////////////////////////
// $Id: buffers.h 5494 2016-08-05 19:24:21Z mellinger $
// Authors: shzeng, schalk@wadsworth.org
// Description: A buffer class to make loading textures and fonts
//   more efficient.
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
#ifndef BUFFERS_H
#define BUFFERS_H

#include "glheaders.h"
#include <map>
#include <string>
#include <vector>

class buffers
{
 public:
  static bool loadWindowsBitmap( const std::string&, int& w, int& h, std::vector<GLubyte>& );

  static GLuint loadTexture( const std::string& );
  static void releaseTexture( const std::string& );

#if _WIN32
  static GLuint loadFont2D( const std::string&, int );
  static void releaseFont2D( const std::string&, int );
  static const ABCFLOAT* getFontData2D( GLuint );

  static GLuint loadFont3D( const std::string& );
  static void releaseFont3D( const std::string& );
  static const GLYPHMETRICSFLOAT* getFontData3D( GLuint );
#endif // _WIN32

 private:
  static std::map<std::string, GLuint> sTextureHandles;
  static std::map<GLuint, int> sTextureUsage;

#if _WIN32
  static std::string buildFontName2D( const std::string&, int );
  struct Font2DEntry
  {
    Font2DEntry() : usage( 0 ) {}
    ABCFLOAT abcf[256];
    int      usage;
  };
  static std::map<std::string, GLuint> sFontHandles2D;
  static std::map<GLuint, Font2DEntry> sFontData2D;

  struct Font3DEntry
  {
    Font3DEntry() : usage( 0 ) {}
    GLYPHMETRICSFLOAT gmf[256];
    int               usage;
  };
  static std::map<std::string, GLuint> sFontHandles3D;
  static std::map<GLuint, Font3DEntry> sFontData3D;
#endif // _WIN32
};

#endif // BUFFERS_H


