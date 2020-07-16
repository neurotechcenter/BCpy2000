////////////////////////////////////////////////////////////////////////////////
// $Id: buffers.cpp 5494 2016-08-05 19:24:21Z mellinger $
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
#include "buffers.h"
#include "FileUtils.h"
#include "BinaryDataIO/Win32BmpFormat.h"
#include <fstream>
#include <sstream>

using namespace std;

map<string, GLuint> buffers::sTextureHandles;
map<GLuint, int> buffers::sTextureUsage;

#if _WIN32
map<string, GLuint> buffers::sFontHandles2D;
map<GLuint, buffers::Font2DEntry> buffers::sFontData2D;

map<std::string, GLuint> buffers::sFontHandles3D;
map<GLuint, buffers::Font3DEntry> buffers::sFontData3D;
#endif // _WIN32

bool
buffers::loadWindowsBitmap( const std::string& file, int& width, int& height, std::vector<GLubyte>& img )
{
  std::ifstream f( FileUtils::AbsolutePath( file ), std::ios::binary );
  if( !f.is_open() )
    return false;
  std::string err = Win32::Bmp::Decode( f, width, height, img, PixelFormat::RGB );
  if( width < 0 )
    return false;
  if( height < 0 )
  {
    height = -height;
    std::vector<GLubyte> tmp = img;
    int bytesPerLine = 3 * width;
    const GLubyte* p1 = tmp.data();
    GLubyte* p2 = img.data() + img.size();
    for( int i = 0; i < height; ++i )
    {
      p2 -= bytesPerLine;
      ::memcpy( p2, p1, bytesPerLine );
      p1 += bytesPerLine;
    }
  }
  return err.empty();
}

GLuint
buffers::loadTexture( const std::string& inTextureFile )
{
  GLuint textureHandle = 0;
  if( !inTextureFile.empty() )
  {
    textureHandle = sTextureHandles[inTextureFile];
    if( textureHandle == 0 )
    {
      std::vector<GLubyte> texImg;
      int width, height;
      if( loadWindowsBitmap( inTextureFile, width, height, texImg ) )
      {
        ::glPixelStorei( GL_UNPACK_ROW_LENGTH, 0 );
        ::glPixelStorei( GL_UNPACK_ALIGNMENT, 4 );
        ::glPixelZoom( 1, 1 );
        {
          glGenTextures( 1, &textureHandle );
          if( textureHandle != 0 )
          {
            glBindTexture( GL_TEXTURE_2D, textureHandle );
            gluBuild2DMipmaps( GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, texImg.data() );
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST );
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
          }
        }
      }
      sTextureHandles[inTextureFile] = textureHandle;
    }
    if( textureHandle != 0 )
      ++sTextureUsage[textureHandle];
  }
  return textureHandle;
}

void
buffers::releaseTexture( const std::string& inTextureFile )
{
  GLuint textureHandle = sTextureHandles[inTextureFile];
  if( textureHandle && --sTextureUsage[textureHandle] < 1 )
  {
    glDeleteTextures( 1, &textureHandle );
    sTextureHandles.erase( inTextureFile );
    sTextureUsage.erase( textureHandle );
  }
}

#if _WIN32
std::string
buffers::buildFontName2D( const std::string& inFontName, int inFontSize )
{
  ostringstream fontName;
  fontName << inFontName << inFontSize;
  return fontName.str();
}

GLuint
buffers::loadFont2D( const std::string& inFontName, int inFontSize )
{
  string name = buildFontName2D( inFontName, inFontSize );
  GLuint fontID = sFontHandles2D[name];
  if( fontID == 0 )
  {
    fontID = glGenLists(256); // Generate the list for the font
    HFONT hFont = ::CreateFont(
      inFontSize,           // Our desired HEIGHT of the font
      0,                  // The WIDTH (If we leave this zero it will pick the best width depending on the height)
      0,                  // The angle of escapement
      0,                  // The angle of orientation
      FW_BOLD,            // The font's weight (We want it bold)
      FALSE,              // Italic - We don't want italic
      FALSE,              // Underline - We don't want it underlined
      FALSE,              // Strikeout - We don't want it strikethrough
      ANSI_CHARSET,       // This is the type of character set
      OUT_TT_PRECIS,      // The Output Precision
      CLIP_DEFAULT_PRECIS,        // The Clipping Precision
      ANTIALIASED_QUALITY,        // The quality of the font - We want anitaliased fonts
      FF_DONTCARE|DEFAULT_PITCH,  // The family and pitch of the font.  We don't care.
      inFontName.c_str()          // The font name (Like "Arial", "Courier", etc...)
    );
    HDC hDC = ::CreateCompatibleDC( NULL );
    HFONT oldFont = ( HFONT )::SelectObject(hDC, hFont);
    if( oldFont )
    {
      ::wglUseFontBitmaps(hDC, 0,255, fontID);  // Builds 255 bitmap characters
      ::GetCharABCWidthsFloat(hDC,0,255,sFontData2D[fontID].abcf);
      ::SelectObject(hDC, oldFont); // Selects The Font We Want
      sFontHandles2D[name] = fontID;
      sFontData2D[fontID].usage = 1;
    }
    else
    {
      fontID = 0;
      glDeleteLists( fontID, 256 );
    }
    ::DeleteObject(hFont);  // Delete The Font
    ::DeleteDC( hDC );
  }
  return fontID;
}

void
buffers::releaseFont2D( const std::string& inFontName, int inFontSize )
{
  string name = buildFontName2D( inFontName, inFontSize );
  GLuint fontID = sFontHandles2D[name];
  if( fontID != 0 && --sFontData2D[fontID].usage < 1 )
  {
    glDeleteLists( fontID, 256 );
    sFontHandles2D.erase( name );
    sFontData2D.erase( fontID );
  }
}

const ABCFLOAT*
buffers::getFontData2D( GLuint inHandle )
{
  return sFontData2D.find( inHandle ) != sFontData2D.end()
         ? sFontData2D[inHandle].abcf
         : NULL;
}


GLuint
buffers::loadFont3D( const std::string& inFontName )
{
  GLuint fontID = sFontHandles3D[inFontName];
  if( fontID == 0 )
  { //build the font
    fontID = glGenLists( 256 ); // Storage for 256 characters
    HDC hDC = ::CreateCompatibleDC( NULL );
    HFONT hFont = ::CreateFont( 0,                           // Height of font
                                0,                           // Width of font
                                0,                           // Angle of escapement
                                0,                           // Orientation angle
                                FW_BOLD,                     // Font weight
                                FALSE,                       // Italic
                                FALSE,                       // Underline
                                FALSE,                       // Strikeout
                                ANSI_CHARSET,                // Character set identifier
                                OUT_TT_PRECIS,               // Output precision
                                CLIP_DEFAULT_PRECIS,         // Clipping precision
                                PROOF_QUALITY,               // Output quality
                                FF_DONTCARE|DEFAULT_PITCH,   // Family and pitch
                                inFontName.c_str());         // Font name

    HFONT prevFont = ( HFONT )::SelectObject( hDC, hFont ); // Selects the font we created
    bool success = ::wglUseFontOutlines(  hDC,               // Select the temporary DC
                                          0,                 // Starting character
                                          256,               // Number of display lists to build
                                          fontID,            // Starting display lists
                                          0.01f,             // Deviation from the true outlines
                                          0.3f,              // Font thickness in the Z direction
                                          WGL_FONT_POLYGONS, // Use polygons, not lines
                                          sFontData3D[ fontID ].gmf // Address of buffer to receive data
                                        );
    ::SelectObject( hDC, prevFont );
    ::DeleteObject( hFont );
    ::DeleteDC( hDC );
    if( success )
    {
      sFontHandles3D[ inFontName ] = fontID;
      sFontData3D[ fontID ].usage = 1;
    }
    else
    {
      glDeleteLists( fontID, 256 );
      sFontHandles3D.erase( inFontName );
      sFontData3D.erase( fontID );
      fontID = 0;
    }
  }
  return fontID;
}

void
buffers::releaseFont3D( const std::string& inFontName )
{
  GLuint fontID = sFontHandles3D[inFontName];
  if( fontID != 0 && --sFontData3D[fontID].usage < 1 )
  {
    glDeleteLists( fontID, 256 );
    sFontHandles3D.erase( inFontName );
    sFontData3D.erase( fontID );
  }
}

const GLYPHMETRICSFLOAT*
buffers::getFontData3D( GLuint inHandle )
{
  return sFontData3D.find( inHandle ) != sFontData3D.end()
         ? sFontData3D[inHandle].gmf
         : NULL;
}

#endif // _WIN32
