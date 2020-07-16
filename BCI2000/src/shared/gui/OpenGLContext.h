////////////////////////////////////////////////////////////////////////////////
// $Id$
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A platform abstraction for OpenGL contexts.
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
#ifndef OPENGL_CONTEXT_H
#define OPENGL_CONTEXT_H

#include "ClockTick.h"
#include "Uncopyable.h"
#include "Lockable.h"
#include "Color.h"

#define WithGLContext(x) for( GUI::OpenGLContext::Current_raii c_(x); c_.Once(); )
#define RecordGLDisplayList(x) for( GUI::OpenGLDisplayList::Recorder c_(x); c_.Once(); )

namespace GUI
{
  typedef void* OSWindowHandle;
  static const OSWindowHandle NoWindow = OSWindowHandle( -1 );

  class OpenGLTexture;
  class OpenGLDisplayList;
  class Framebuffer;
  struct GLFunctions;

  void DebugOnGLError();

  class OpenGLContext : Uncopyable
  {
   public:
    explicit OpenGLContext( OSWindowHandle, OpenGLContext* = 0, int swapInterval = -1 );
    explicit OpenGLContext( OpenGLContext* );
    ~OpenGLContext();

    const GLFunctions& Functions() const;
    const char* Vendor() const;
    const char* Renderer() const;
    const char* Extensions() const;
    float Version() const;
    bool Valid() const;
    bool SoftwareRasterized() const;

    void SetViewport( float x, float y, float w, float h );
    void Clear( RGBColor );

    void MakeCurrent();
    void DoneCurrent();
    bool IsCurrent() const;
    static OpenGLContext* Current();
    void Finish();

    Framebuffer* SetRenderTarget( Framebuffer* );

    bool Allocate2DBuffers( size_t );
    OpenGLTexture* Get2DBuffer( size_t idx = 0 );
    void SwapBuffers();
    int SwapInterval() const;

    void Test(); // Debugging

    class Current_raii
    {
    public:
      Current_raii( OpenGLContext& c ) : mp( &c ), mFirst( true ) { mp->MakeCurrent(); }
      Current_raii( OpenGLContext* c ) : mp( c ), mFirst( true ) { mp->MakeCurrent(); }
      ~Current_raii() { mp->DoneCurrent(); }
      bool Once() { bool r = mFirst; mFirst = false; return r; }
    private:
      OpenGLContext* mp;
      bool mFirst;
    };

   private:
    struct Data;
    Data* d;
  };

  class OpenGLDisplayList : Uncopyable
  {
  public:
    OpenGLDisplayList();
    ~OpenGLDisplayList();
    const OpenGLDisplayList& Call() const;

    class Recorder
    {
    public:
      Recorder( OpenGLDisplayList& c ) : mp( &c ), mFirst( true ) { mp->New(); }
      Recorder( OpenGLDisplayList* c ) : mp( c ), mFirst( true ) { mp->New(); }
      ~Recorder() { mp->End(); }
      bool Once() { bool r = mFirst; mFirst = false; return r; }
    private:
      OpenGLDisplayList* mp;
      bool mFirst;
    };

  private:
    void New();
    void End();
    friend class Recorder;
    struct Private;
    Private* p;
  };

  class OpenGLTexture : Uncopyable
  {
  public:
    OpenGLTexture();
    ~OpenGLTexture();
    int Width() const;
    int Height() const;
    OpenGLTexture& SetSize( int width, int height );
    OpenGLTexture& SetPixelData( size_t left, size_t top, size_t width, size_t height, const void* );

    struct PixelInfo { int width, height, bpp; };
    const PixelInfo& GetPixelInfo() const;
    const OpenGLTexture& GetPixelData( PixelInfo&, std::vector<uint32_t>& ) const;

    uint32_t GLName() const;
    const OpenGLTexture& Bind() const;
    const OpenGLTexture& CopyToCurrentViewport( bool blend = false ) const;

  private:
    struct Private;
    Private* p;
  };

  class Framebuffer : Uncopyable
  {
  public:
    explicit Framebuffer( int w, int h );
    ~Framebuffer();
    int Width() const;
    int Height() const;

    void Bind();
    void Unbind();
    void SwapTextures();
    void CopyToCurrentViewport();

    OpenGLTexture* Front();
    OpenGLTexture* Back();

  private:
    struct Private;
    Private* p;
  };

} // namespace GUI

#endif // OPENGL_CONTEXT_H
