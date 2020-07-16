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

// if not using a lock, an exception will be thrown when two threads try to make
// the same context current
#define LOCK_WHEN_CURRENT 0
#define FORCE_SOFTWARE_RASTERIZER 0

#if _WIN32
# include <Windows.h>
#endif

#include <GL/gl.h>
#include <GL/glext.h>
#if _WIN32
# include <gl/wglext.h>
#elif __APPLE__
#else
# include <GL/glx.h>
# include <GL/glxext.h>
# undef None
#endif
#include <GL/glcorearb.h>

#if  _WIN32
# define PIXEL_FORMAT GL_BGRA_EXT
#else
# define PIXEL_FORMAT GL_RGBA
#endif

#include "OpenGLContext.h"

#include "SharedPointer.h"
#include "ThreadUtils.h"
#include "ThreadLocal.h"
#include "SysError.h"
#include "Exception.h"
#include "Debugging.h"

static const char* sVendorBlacklist[] =
{ // force software rendering if OpenGL implementation is from one of these vendors
  "VMWare",
};

static bool
IsVendorBlacklisted(const GLubyte* pName)
{
#if FORCE_SOFTWARE_RASTERIZER
  return true;
#endif
  if(!pName)
    return false;
  std::string name = StringUtils::ToLower(reinterpret_cast<const char*>(pName));
  for(const auto& p : sVendorBlacklist)
    if(name.find(StringUtils::ToLower(p)) != std::string::npos)
      return true;
  return false;
}

namespace
{
  struct GLRect { GLfloat x, y, w, h; };

  template<class T> void FailIfNot( T t )
  {
    if( !t )
      SuggestDebugging << SysError().Message();
  }

  static unsigned int NextPow2( unsigned int i )
  {
    if( i == 0 )
      return 0;
    unsigned int p = i - 1, q = 1;
    while( p )
    {
      p /= 2;
      q *= 2;
    }
    return q;
  }
}

static bool
FlushGLErrors()
{
  bool errorFound = false,
       haveContext = false;
#if _WIN32
  haveContext = ::wglGetCurrentContext();
  Assert( haveContext );
#endif
  while( haveContext && ::glGetError() )
    errorFound = true;
  return errorFound;
}

static void
ThrowOnGLError()
{
  std::vector<GLenum> errors;
  GLenum err = GL_NO_ERROR;
  while( (err = ::glGetError()) && errors.size() < 20 )
    errors.push_back( err );
  if( !errors.empty() )
  {
    std::vector<std::string> info;
    for( size_t i = 0; i < errors.size(); ++i )
      switch( errors[i] )
      {
#define CONSIDER(x) case (x): info.push_back( #x ); break;
        CONSIDER( GL_INVALID_ENUM )
        CONSIDER( GL_INVALID_VALUE )
        CONSIDER( GL_INVALID_OPERATION )
        CONSIDER( GL_STACK_OVERFLOW )
        CONSIDER( GL_STACK_UNDERFLOW )
        CONSIDER( GL_OUT_OF_MEMORY )
#undef  CONSIDER
default: info.push_back( std::string( "unknown, last system error is: " ) + SysError().Message() );
      }
    std::ostringstream oss;
    if( info.size() == 1 )
      oss << "OpenGL error: " << info.front();
    else 
    {
      oss << "OpenGL errors occurred:\n";
      for( size_t i = 0; i < info.size(); ++i )
        oss << " " << info[i] << "\n";
    }
    throw std_runtime_error << oss.str();
  }
}

void
GUI::DebugOnGLError()
{
#if GL_DEBUG_
  auto pCurrent = OpenGLContext::Current();
  if(pCurrent)
  {
    pCurrent->Finish();
    ThrowOnGLError();
  }
#endif
}

namespace GUI
{

struct GLFunctions
{
  GLFunctions();
  void Resolve();

  const char* mpGlVendor, *mpGlRenderer;
  float mGlVersion;

  bool mResolved;
#if _WIN32
  PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT;
  PFNWGLGETSWAPINTERVALEXTPROC wglGetSwapIntervalEXT;

  PFNWGLGETEXTENSIONSSTRINGARBPROC wglGetExtensionsStringARB;

  PFNWGLCREATEPBUFFERARBPROC wglCreatePbuffer;
  PFNWGLDESTROYPBUFFERARBPROC wglDestroyPbuffer;
  PFNWGLGETPBUFFERDCARBPROC wglGetPbufferDC;
  PFNWGLRELEASEPBUFFERDCARBPROC wglReleasePbufferDC;
  PFNWGLQUERYPBUFFERARBPROC wglQueryPbuffer;
#endif
  PFNGLBLENDEQUATIONPROC glBlendEquation;

  PFNGLFENCESYNCPROC glFenceSync;
  PFNGLISSYNCPROC glIsSync;
  PFNGLGETSYNCIVPROC glGetSynciv;
  PFNGLGETINTEGER64VPROC glGetInteger64v;
  PFNGLDELETESYNCPROC glDeleteSync;
  PFNGLCLIENTWAITSYNCPROC glClientWaitSync;
  PFNGLWAITSYNCPROC glWaitSync;
};
typedef void* GLFunctions::*Proc;
static const struct { const char* name; Proc proc; } GLImports[] =
{
#define FUNC(x) { #x, (Proc)&GLFunctions::x }
#if _WIN32
  FUNC( wglSwapIntervalEXT ),
  FUNC( wglGetSwapIntervalEXT ),

  FUNC( wglGetExtensionsStringARB ),

  FUNC( wglCreatePbuffer ),
  FUNC( wglDestroyPbuffer ),
  FUNC( wglGetPbufferDC ),
  FUNC( wglReleasePbufferDC ),
  FUNC( wglQueryPbuffer ),
#endif
  FUNC( glBlendEquation ),

  FUNC( glFenceSync ),
  FUNC( glIsSync ),
  FUNC( glGetSynciv ),
  FUNC( glDeleteSync ),
  FUNC( glClientWaitSync ),
  FUNC( glWaitSync ),
  FUNC( glGetInteger64v ),
#undef FUNC
};

GLFunctions::GLFunctions()
{
  ::memset( this, 0, sizeof(*this) );
}

namespace {

void* getProcAddress( const char* name )
{
#if _WIN32
    return ::wglGetProcAddress( name );
#elif __APPLE__
#else
    typedef void (*Function)();
    Function f = ::glXGetProcAddress( reinterpret_cast<const GLubyte*>( name ) );
    return reinterpret_cast<void*>( f );
#endif
}

}

void GLFunctions::Resolve()
{
  if( !mResolved )
  {
    mpGlVendor = reinterpret_cast<const char*>(::glGetString(GL_VENDOR));
    mpGlRenderer = reinterpret_cast<const char*>(::glGetString(GL_RENDERER));
    const char* p = reinterpret_cast<const char*>( ::glGetString( GL_VERSION ) );
    if(p)
      mGlVersion = ::atof(p);
    for( int i = 0; i < sizeof(GLImports)/sizeof(*GLImports); ++i )
    {
      std::string name = GLImports[i].name;
      void* p = getProcAddress( name.c_str() );
      if( !p )
        p = getProcAddress( ( name + "ARB" ).c_str() );
      if( !p )
        p = getProcAddress( ( name + "EXT" ).c_str() );
      this->*GLImports[i].proc = p;
    }
  }
  mResolved = true;
};

struct OpenGLContext::Data : Lockable<>
{
  Data();
  ~Data();
  void InitFromWindow( OSWindowHandle, OpenGLContext::Data* );
  void InitFromOther( OpenGLContext::Data* );
  void Init2( OpenGLContext::Data* );
  void SwapBuffers();
  bool SetSwapInterval( int );
  void Finish();
  void OnMakeCurrent();
  void OnDoneCurrent();

  bool mValid, mSoftwareRasterized, mIgnoreGetSwapInterval;
  ThreadUtils::ID mCreatingThread, mCurrentThread;
  Synchronized<int> mCurrentCount;
  static ThreadLocal<OpenGLContext*> stpCurrent;
  GLFunctions mFunctions;
  Framebuffer* mpRenderTarget;
  int mSwapInterval;
#if _WIN32
  bool mWindowOwned;
  HWND mWindow;
  HDC mDC;
  HGLRC mGLRC;
  int mFormat;
#endif

  GLRect& Viewport() { return mpProperties->mViewport; }

  struct ShareableProperties
  {
    ShareableProperties() { ::memset( &mViewport, 0, sizeof(mViewport) ); }
    ~ShareableProperties() { for( OpenGLTexture* p : m2DBuffers ) delete p; }
    GLRect mViewport;
    std::vector<OpenGLTexture*> m2DBuffers;
  };
  SharedPointer<ShareableProperties> mpProperties;
};

ThreadLocal<OpenGLContext*> OpenGLContext::Data::stpCurrent;

OpenGLContext::Data::Data()
: mCurrentCount( 0 ),
  mValid( true ),
  mSoftwareRasterized( false ),
  mIgnoreGetSwapInterval( false ),
  mCreatingThread( ThreadUtils::ID::Current() ),
  mCurrentThread( ThreadUtils::ID::None() ),
  mpRenderTarget( 0 )
{
#if _WIN32
  mWindowOwned = false;
  mDC = 0;
  mGLRC = 0;
  mFormat = 0;
#endif
}

void OpenGLContext::Data::InitFromWindow( OSWindowHandle inWindow, OpenGLContext::Data* inpShare )
{
#if _WIN32
  if( inWindow == NoWindow )
  {
    mWindow = ::CreateWindowA( "STATIC", nullptr, 0, 0, 0, 0, 0, NULL, NULL, NULL, nullptr );
    mWindowOwned = true;
  }
  else
    mWindow = static_cast<HWND>( inWindow );

  PIXELFORMATDESCRIPTOR pfd =
  {
    sizeof(pfd),
    1,
    PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
    PFD_TYPE_RGBA,
    32, // color depth
    0, 0, 0, 0, 0, 0,
    0,
    0,
    0,
    0, 0, 0, 0,
    32, // depth buffer bits
    0,  // stencilbuffer bits
    0,  // number of aux buffers
    PFD_MAIN_PLANE,
    0,
    0, 0, 0
  };

  // Create an auxiliary window because we need a temporary OpenGL context,
  // and SetPixelFormat() may only be called once on a window.
  RECT r;
  if(::GetWindowRect(mWindow, &r))
  {
    HWND wnd = ::CreateWindowA("STATIC", nullptr, 0, r.left, r.top, r.right - r.left, r.bottom - r.top, NULL, NULL, NULL, nullptr);
    HDC dc = ::GetDC(wnd);
    FailIfNot(dc);
    PIXELFORMATDESCRIPTOR tempPfd = pfd;
    auto format = ::ChoosePixelFormat(dc, &tempPfd);
    ::DescribePixelFormat(dc, format, sizeof(tempPfd), &tempPfd);
    FailIfNot(::SetPixelFormat(dc, format, &tempPfd));
    auto tempGLRC = ::wglCreateContext(dc);
    FailIfNot(tempGLRC);
    ::wglMakeCurrent(dc, tempGLRC);
    auto pGlVendorString = ::glGetString(GL_VENDOR);
    if(IsVendorBlacklisted(pGlVendorString))
    {
      int numFormats = 1, i = 1;
      while(i <= (numFormats = ::DescribePixelFormat(dc, i, sizeof(tempPfd), &tempPfd)))
      {
        ++i;
        if(tempPfd.cColorBits < pfd.cColorBits)
          continue;
        if(tempPfd.iPixelType != pfd.iPixelType)
          continue;
        if(tempPfd.cDepthBits < pfd.cDepthBits)
          continue;
        if(!(tempPfd.dwFlags & PFD_SUPPORT_OPENGL))
          continue;
        if(tempPfd.dwFlags & PFD_GENERIC_FORMAT|PFD_GENERIC_ACCELERATED == PFD_GENERIC_FORMAT)
        {
          mFormat = i;
          pfd = tempPfd;
        }
      }
      if(!(pfd.dwFlags & PFD_GENERIC_FORMAT))
        throw std_runtime_error << "OpenGL vendor \"" 
                                << pGlVendorString
                                << "\" is blacklisted "
                                << "but a suitable software rasterizer is not available.";
    }
    ::ReleaseDC(wnd, dc);
    ::wglMakeCurrent(0, 0);
    ::wglDeleteContext(tempGLRC);
    ::DestroyWindow(wnd);
  }

  mDC = ::GetDC(mWindow);
  FailIfNot(mDC);
  if(mFormat == 0)
    mFormat = ::ChoosePixelFormat(mDC, &pfd);
#elif __APPLE__
# error No OS X cgl code yet.
#else
# error No GLX code yet.
#endif
  Init2( inpShare );
}

void OpenGLContext::Data::InitFromOther( OpenGLContext::Data* inpOther )
{
  Assert( inpOther );
#if _WIN32
  mWindow = inpOther->mWindow;
  mDC = inpOther->mDC;
  mFormat = inpOther->mFormat;
  if( mWindow )
  {
    mDC = ::GetDC( mWindow );
    FailIfNot( mDC );
  }
#endif // _WIN32
  Init2( inpOther );
}

void
OpenGLContext::Data::Init2( OpenGLContext::Data* inpShare )
{
#if _WIN32
  if( !mFormat )
  {
    mValid = false;
    SysError err;
    if( err.Code() != ERROR_INVALID_HANDLE )
      throw std_runtime_error << "ChoosePixelFormat(): " << err.Message();
  }
  if( mValid )
  {
    PIXELFORMATDESCRIPTOR pfd;
    ::DescribePixelFormat( mDC, mFormat, sizeof(pfd), &pfd );
    FailIfNot( ::SetPixelFormat( mDC, mFormat, &pfd ) );
    mSoftwareRasterized = pfd.dwFlags & PFD_GENERIC_FORMAT;
    mGLRC = ::wglCreateContext( mDC );
    FailIfNot( mGLRC );
    if( inpShare && !::wglShareLists( inpShare->mGLRC, mGLRC ) )
      throw std_runtime_error << "wglShareLists(): " << SysError().Message();
    HGLRC glrc = ::wglGetCurrentContext();
    HDC dc = ::wglGetCurrentDC();
    FailIfNot( ::wglMakeCurrent( mDC, mGLRC ) );
    mFunctions.Resolve();

    auto p = mFunctions.mpGlVendor;
    std::string vendorName;
    while(*p && !::isalnum(*p))
      ++p;
    while(*p && ::isalnum(*p))
      vendorName += *p++;

    int interval = mSwapInterval;
    if( interval < 0 )
      interval = 0;//mSoftwareRasterized ? 0 : 1;
    if( !SetSwapInterval( interval ) )
    {
      std::string error;
      error += "Could not ";
      error += interval ? "enable" : "disable";
      error += " OpenGL vsync for \"";
      error += mFunctions.mpGlRenderer;
      error += "\" renderer.";
      if(!mSoftwareRasterized) // we don't care about proper timing in case of software rasterization
      {
        throw std_runtime_error << error << "\n"
             "In your " << vendorName << " graphics driver options, make sure that "
             "applications may enable vertical refresh synchronization:\n"
             "http://" WEBSEARCH_DOMAIN "/search?q=force+vsync+settings+" << vendorName << "\n.";
      }
    }
    FlushGLErrors();
    ::wglMakeCurrent( dc, glrc );
  }
#endif // _WIN32
  if( inpShare )
    mpProperties = inpShare->mpProperties;
  else
    mpProperties = SharedPointer<ShareableProperties>( new ShareableProperties );
}

OpenGLContext::Data::~Data()
{
  Assert( mCurrentCount == 0 );
  Assert( mCreatingThread == ThreadUtils::ID::Current() ); // Win32 ReleaseDC() must be called from the same thread as GetDC()
#if _WIN32
  if( mGLRC )
  {
    if( mDC )
      ::ReleaseDC( mWindow, mDC );
    ::wglMakeCurrent( 0, 0 );
    ::wglDeleteContext( mGLRC );
  }
  if( mWindowOwned )
    ::DestroyWindow( mWindow );
#elif __APPLE__
#else
#endif
}

void
OpenGLContext::Data::OnMakeCurrent()
{
#if _WIN32
  if( !::wglMakeCurrent( mDC, mGLRC ) )
      SuggestDebugging << "wglMakeCurrent() " << SysError().Message();
#elif __APPLE__
#else
#endif
}

void
OpenGLContext::Data::OnDoneCurrent()
{
#if _WIN32
  ::wglMakeCurrent( 0, 0 );
#elif __APPLE__
#else
#endif
}

bool
OpenGLContext::Data::SetSwapInterval( int i )
{
  if( mSwapInterval >= 0 )
    throw std_runtime_error << "Cannot set swap interval more than once";
#if _WIN32
  if( !mFunctions.wglSwapIntervalEXT )
    return i == 1;
  if( mFunctions.wglSwapIntervalEXT( i ) )
    mSwapInterval = mIgnoreGetSwapInterval ? i : mFunctions.wglGetSwapIntervalEXT();
#elif __APPLE__
#else
#endif
  return mSwapInterval == i;
}

void
OpenGLContext::Data::Finish()
{
  if(mFunctions.glFenceSync)
  {
    auto s = mFunctions.glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
    if(!s)
      ThrowOnGLError();
    int timeout_ns = 1000*1000*1000;
    GLenum r = GL_TIMEOUT_EXPIRED;
    while(r == GL_TIMEOUT_EXPIRED)
      r = mFunctions.glClientWaitSync(s, GL_SYNC_FLUSH_COMMANDS_BIT, timeout_ns);
    mFunctions.glDeleteSync(s);
    if(r == GL_WAIT_FAILED)
      ThrowOnGLError();
  }
  else
  {
    ::glFinish();
  }
}

void
OpenGLContext::Data::SwapBuffers()
{
  if( mpRenderTarget )
  {
    mpRenderTarget->SwapTextures();
  }
  else 
  {
#if _WIN32
    ::SwapBuffers( mDC );
#if 1
    // force update by reading pixel data
    int ignored;
    ::glReadPixels(0, 0, 1, 1, GL_LUMINANCE, GL_UNSIGNED_BYTE, &ignored);
#endif
#else
# error
#endif
  }
}

// Main class
OpenGLContext::OpenGLContext( OSWindowHandle inWindowHandle, OpenGLContext* inpShare, int inSwapInterval )
: d( new Data )
{
  d->mSwapInterval = inSwapInterval;
  d->InitFromWindow( inWindowHandle, inpShare ? inpShare->d : 0 );
}

OpenGLContext::OpenGLContext( OpenGLContext* inpShare )
: d( new Data )
{
  d->mSwapInterval = -1;
  d->InitFromOther( inpShare ? inpShare->d : 0 );
}

OpenGLContext::~OpenGLContext()
{
  delete d;
}

const GLFunctions&
OpenGLContext::Functions() const
{
  return d->mFunctions;
}

const char*
OpenGLContext::Extensions() const
{
  Assert( IsCurrent() );
  static const char empty[] = "";
  const char* p = 0;
#if _WIN32
  if( d->mFunctions.wglGetExtensionsStringARB )
    p = d->mFunctions.wglGetExtensionsStringARB( d->mDC );
#endif
  if( !p )
    p = reinterpret_cast<const char*>( ::glGetString( GL_EXTENSIONS ) );
  return p ? p : empty;
}

const char*
OpenGLContext::Vendor() const
{
  return d->mFunctions.mpGlVendor;
}

const char*
OpenGLContext::Renderer() const
{
  return d->mFunctions.mpGlRenderer;
}

float
OpenGLContext::Version() const
{
  return d->mFunctions.mGlVersion;
}

bool
OpenGLContext::Valid() const
{
  return d->mValid;
}

bool
OpenGLContext::SoftwareRasterized() const
{
  return d->mSoftwareRasterized;
}

bool
OpenGLContext::IsCurrent() const
{
  return d->mCurrentCount > 0 && d->mCurrentThread == ThreadUtils::ID::Current();
}

void
OpenGLContext::SetViewport( float x, float y, float w, float h )
{
  Assert( IsCurrent() );
  FlushGLErrors();
  GLRect r = { x, y, w, h };
  d->Viewport() = r;
  ::glViewport( x, y, w, h );
  ThrowOnGLError();
}

void
OpenGLContext::Clear( RGBColor c )
{
  Assert( IsCurrent() );
  FlushGLErrors();
  if( c == RGBColor::NullColor )
    c = RGBColor::Black;
  ::glDisable( GL_SCISSOR_TEST );
  ::glDisable( GL_STENCIL_TEST );
  ::glClearColor( c.R()/255., c.G()/255., c.B()/255., 1 );
  ::glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE );
  ::glEnable( GL_DEPTH_TEST );
  ::glClearDepth( 1.0f );
  ::glDepthMask( GL_TRUE );
  ::glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  ::glMatrixMode( GL_MODELVIEW );
  ::glLoadIdentity();
  ::glMatrixMode( GL_PROJECTION );
  ::glLoadIdentity();
  ::glMatrixMode( GL_TEXTURE );
  ::glLoadIdentity();
  DebugOnGLError(); // GL_COLOR not supported on 1.1
  ::glMatrixMode( GL_COLOR );
  ::glLoadIdentity();
  FlushGLErrors();
}

void
OpenGLContext::MakeCurrent()
{
#if LOCK_WHEN_CURRENT
  d->Lock();
#else
  if( d->mCurrentCount > 0 && d->mCurrentThread != ThreadUtils::ID::Current() )
    throw std_runtime_error << "Context is current in a different thread: " << d->mCurrentThread;
#endif // LOCK_WHEN_CURRENT
  if( ++d->mCurrentCount == 1 )
  {
    Data::stpCurrent = this;
    d->mCurrentThread = ThreadUtils::ID::Current();
    if( d->mpRenderTarget )
      d->mpRenderTarget->Bind();
    else
      d->OnMakeCurrent();
  }
}

void
OpenGLContext::DoneCurrent()
{
  Assert( d->mCurrentCount > 0 );
  if( --d->mCurrentCount == 0 )
  {
    DebugOnGLError();
    if( d->mpRenderTarget )
      d->mpRenderTarget->Unbind();
    else
      d->OnDoneCurrent();
    Data::stpCurrent = nullptr;
  }
#if LOCK_WHEN_CURRENT
  d->Unlock();
#endif
}

OpenGLContext*
OpenGLContext::Current()
{
  return Data::stpCurrent;
}

int
OpenGLContext::SwapInterval() const
{
  return d->mSwapInterval;
}

void
OpenGLContext::SwapBuffers()
{
  d->SwapBuffers();
}

void
OpenGLContext::Finish()
{
  Assert( IsCurrent() );
  FlushGLErrors();
  d->Finish();
  ThrowOnGLError();
}

Framebuffer*
OpenGLContext::SetRenderTarget( Framebuffer* p )
{
  Assert( IsCurrent() );
  DebugOnGLError();
  Framebuffer* prev = d->mpRenderTarget;
  if( prev )
    prev->Unbind();
  DebugOnGLError();
  d->mpRenderTarget = p;
  if( p )
    p->Bind();
  DebugOnGLError();
  return prev;
}

bool
OpenGLContext::Allocate2DBuffers( size_t n )
{
  Assert( IsCurrent() );
  if( d->mpProperties->m2DBuffers.empty() )
    for( size_t i = 0; i < n; ++i )
      d->mpProperties->m2DBuffers.push_back( new OpenGLTexture );
  else if( d->mpProperties->m2DBuffers.size() != n )
    return false;
  return true;
}

OpenGLTexture*
OpenGLContext::Get2DBuffer( size_t i )
{
  Assert( IsCurrent() );
  if( i >= d->mpProperties->m2DBuffers.size() )
    return 0;
  OpenGLTexture* p = d->mpProperties->m2DBuffers[i];
  p->SetSize( d->Viewport().w, -d->Viewport().h );
  return p;
}

// OpenGLTexture
struct OpenGLTexture::Private
{
  GLuint mName;
  PixelInfo mInfo;
  int mContentWidth, mContentHeight;
  Private() : mName( 0 ), mContentWidth( 0 ), mContentHeight( 0 )
  { ::memset( &mInfo, 0, sizeof(mInfo) ); }
};

OpenGLTexture::OpenGLTexture()
: p( new Private )
{
  DebugOnGLError();
  ::glGenTextures( 1, &p->mName );
  DebugOnGLError();
}

OpenGLTexture::~OpenGLTexture()
{
  ::glDeleteTextures( 1, &p->mName );
  delete p;
}

int
OpenGLTexture::Width() const
{
  return p->mContentWidth;
}

int
OpenGLTexture::Height() const
{
  return p->mContentHeight;
}

const OpenGLTexture&
OpenGLTexture::Bind() const
{
  DebugOnGLError();
  ::glEnable( GL_TEXTURE_2D );
  ::glBindTexture( GL_TEXTURE_2D, p->mName );
  DebugOnGLError();
  return *this;
}

OpenGLTexture&
OpenGLTexture::SetSize( int w, int h )
{
  p->mContentWidth = w;
  p->mContentHeight = h;
  int width = ::abs(w), height = ::abs(h);
  // Allocate texture store if necessary
  if( width > p->mInfo.width || height > p->mInfo.height )
  {
    Bind();
    // OpenGL 1.1 requires powers of 2 when calling glTexImage2D
    p->mInfo.bpp = 32;
    p->mInfo.width = NextPow2( width );
    p->mInfo.height = NextPow2( height );
    ::glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA8, p->mInfo.width, p->mInfo.height,
      0, PIXEL_FORMAT, GL_UNSIGNED_BYTE, 0
    );
    DebugOnGLError();
  }
  return *this;
}

const OpenGLTexture::PixelInfo&
OpenGLTexture::GetPixelInfo() const
{
  return p->mInfo;
}

OpenGLTexture&
OpenGLTexture::SetPixelData( size_t x, size_t y, size_t width, size_t height, const void* pData )
{
  Bind();
  if( x + width > p->mInfo.width || y + height > p->mInfo.height )
    SetSize( x + width, y + height );
  ::glTexSubImage2D( GL_TEXTURE_2D, 0, x, y, width, height, PIXEL_FORMAT, GL_UNSIGNED_BYTE, pData );
  DebugOnGLError();
  return *this;
}

const OpenGLTexture&
OpenGLTexture::GetPixelData( PixelInfo& info, std::vector<uint32_t>& pixels ) const
{
  Bind();
  info = p->mInfo;
  pixels.resize( info.width * info.height );
  ::glGetTexImage( GL_TEXTURE_2D, 0, PIXEL_FORMAT, GL_UNSIGNED_BYTE, pixels.data() );
  DebugOnGLError();
  return *this;
}

uint32_t
OpenGLTexture::GLName() const
{
  return p->mName;
}

const OpenGLTexture&
OpenGLTexture::CopyToCurrentViewport( bool blend ) const
{
  int width = 0, height = 0, texWidth = 0, texHeight = 0;
  width = p->mContentWidth;
  height = p->mContentHeight;
  texWidth = p->mInfo.width;
  texHeight = p->mInfo.height;
  FlushGLErrors();
  if( width != 0 && height != 0 )
  {
    Bind();
    // Switch to 1:1 orthogonal projection
    ::glMatrixMode( GL_PROJECTION );
    ::glPushMatrix();
    ::glLoadIdentity();
    GLRect viewport = { 0 };
    ::glGetFloatv( GL_VIEWPORT, &viewport.x );
    ::glOrtho( 0, viewport.w, 0, viewport.h, 0, 1 );
    ::glMatrixMode( GL_MODELVIEW );
    ::glPushMatrix();
    ::glLoadIdentity();
    ::glTranslatef( viewport.w / 2.0, viewport.h / 2.0, 0 );
    ::glScalef( width < 0 ? -1 : 1, height < 0 ? -1 : 1, 1 );
    ::glTranslatef( -viewport.w / 2.0, -viewport.h / 2.0, 0 );
    width = ::abs( width );
    height = ::abs( height );
    // Set up for proper transparency handling
    ::glPushAttrib( GL_CURRENT_BIT | GL_ENABLE_BIT );
    ::glDisable( GL_DEPTH_TEST );
    ::glDisable( GL_LIGHTING );
    ::glEnable( GL_BLEND );
    if( blend )
      ::glBlendFunc( GL_ONE_MINUS_DST_COLOR, GL_DST_COLOR );
    else
      ::glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    ::glColor4f( 1, 1, 1, 1 );
    if( width == viewport.w && height == viewport.h )
    {
      ::glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
      ::glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    }
    else
    {
      ::glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
      ::glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    }
    // Draw textured rectangle
    double relWidth = width * 1.0 / texWidth,
           relHeight = height * 1.0 / texHeight;
    ::glBegin( GL_QUADS );
      ::glTexCoord2d( 0, 0 );                ::glVertex2f( 0, 0 );
      ::glTexCoord2d( relWidth, 0 );         ::glVertex2f( viewport.w, 0 );
      ::glTexCoord2d( relWidth, relHeight ); ::glVertex2f( viewport.w, viewport.h );
      ::glTexCoord2d( 0, relHeight );        ::glVertex2f( 0, viewport.h );
    ::glEnd();
    // Restore original state
    ::glPopAttrib();
    ::glMatrixMode( GL_PROJECTION );
    ::glPopMatrix();
    ::glMatrixMode( GL_MODELVIEW );
    ::glPopMatrix();
    DebugOnGLError();
  }
  return *this;
}

// Framebuffer
struct Framebuffer::Private
{
  OpenGLTexture mTextures[2];
  struct SwapState
  {
    OpenGLTexture* back, *front;
  };
  SynchronizedObject<SwapState> mSwapState;

  struct Pbuffer
  {
    Pbuffer();
    void Create( int width, int height );
    void Destroy();
    void MakeCurrent();
    void DoneCurrent();
    void CopyPixels( OpenGLTexture* );

    int mWidth, mHeight;
    GLFunctions f;

#if _WIN32
    void CreatePbuffer();
    void CreateMemBuffer();

    HDC mDC;
    HGLRC mGLRC;
    HPBUFFERARB mPb;
    HGDIOBJ mNullBitmap;
    uint32_t* mpBuffer;
#endif
  } mPbuffer;
};

#if _WIN32
Framebuffer::Private::Pbuffer::Pbuffer()
: mWidth( 0 ), mHeight( 0 ),
  mDC( 0 ), mGLRC( 0 ), mPb( 0 ), mNullBitmap( 0 ),
  mpBuffer( 0 )
{
}

void
Framebuffer::Private::Pbuffer::Create( int width, int height )
{
  f.Resolve();
  mWidth = width;
  mHeight = height;
  if( f.wglCreatePbuffer )
    CreatePbuffer();
  else
    CreateMemBuffer();
}

void
Framebuffer::Private::Pbuffer::CreateMemBuffer()
{
  Assert( !mGLRC );
  ::SetLastError( 0 );
  HDC dc = ::wglGetCurrentDC();
  HGLRC glrc = ::wglGetCurrentContext();
  mDC = ::CreateCompatibleDC( dc );
  FailIfNot( mDC );
  HBITMAP bmp = ::CreateCompatibleBitmap( dc, mWidth, mHeight );
  FailIfNot( bmp );
  mNullBitmap = ::SelectObject( mDC, bmp );
  BITMAP bm;
  FailIfNot( ::GetObject( bmp, sizeof(BITMAP), &bm ) );
  int format = ::GetPixelFormat( dc );
  FailIfNot( format );
  PIXELFORMATDESCRIPTOR pfd;
  ::DescribePixelFormat( dc, format, sizeof(pfd), &pfd );
  pfd.dwFlags = PFD_DRAW_TO_BITMAP | PFD_SUPPORT_OPENGL | PFD_GENERIC_ACCELERATED;
  format = ::ChoosePixelFormat( mDC, &pfd );
  FailIfNot( format );
  FailIfNot( ::SetPixelFormat( mDC, format, &pfd ) );
  mGLRC = ::wglCreateContext( mDC );
  FailIfNot( mGLRC );
  FailIfNot( ::wglShareLists( glrc, mGLRC ) );
  // Reportedly, GetDIBits() may fail unless output memory has been allocated by VirtualAlloc() in a single piece.
  mpBuffer = reinterpret_cast<uint32_t*>( ::VirtualAlloc( 0, mWidth * mHeight * 4, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE ) );
  FailIfNot( mpBuffer );
}

void
Framebuffer::Private::Pbuffer::CreatePbuffer()
{
  Assert( !mGLRC );
  ::SetLastError( 0 );
  HDC dc = ::wglGetCurrentDC();
  HGLRC glrc = ::wglGetCurrentContext();
  int format = ::GetPixelFormat( dc );
  FailIfNot( format );
  int attr[] =
  {
    //WGL_PBUFFER_LARGEST_ARB, 1,
    0
  };
  mPb = f.wglCreatePbuffer( dc, format, mWidth, mHeight, attr );
  FailIfNot( mPb );
  mDC = f.wglGetPbufferDC( mPb );
  FailIfNot( mDC );
  mGLRC = ::wglCreateContext( mDC );
  FailIfNot( mGLRC );
  FailIfNot( ::wglShareLists( glrc, mGLRC ) );
}

void
Framebuffer::Private::Pbuffer::Destroy()
{
  Assert( mGLRC );
  ::wglDeleteContext( mGLRC );
  mGLRC = 0;
  if( mpBuffer )
  {
    ::VirtualFree( mpBuffer, 0, MEM_RELEASE );
    HGDIOBJ bmp = ::SelectObject( mDC, mNullBitmap );
    ::DeleteObject( bmp );
    ::DeleteDC( mDC );
  }
  if( mPb )
  {
    f.wglReleasePbufferDC( mPb, mDC );
    f.wglDestroyPbuffer( mPb );
  }
}

void
Framebuffer::Private::Pbuffer::CopyPixels( OpenGLTexture* pTexture )
{
  if( mPb )
  {
    pTexture->Bind();
    ::glCopyTexSubImage2D( GL_TEXTURE_2D, 0, 0, 0, 0, 0, mWidth, mHeight );
  }
  else if( mpBuffer )
  {
    ::glFinish();
    HBITMAP bmp = HBITMAP( ::SelectObject( mDC, mNullBitmap ) );
    BITMAPINFO info = { sizeof(info), mWidth, mHeight, 1, 32, BI_RGB, 0 };
    FailIfNot( ::GetDIBits( mDC, bmp, 0, mHeight, mpBuffer, &info, DIB_RGB_COLORS ) );
    ::SelectObject( mDC, bmp );
    uint32_t* p = mpBuffer;
    for( int i = 0; i < mWidth * mHeight; ++i )
      *p++ |= 0xff000000; // add missing alpha bits
    pTexture->SetPixelData( 0, 0, mWidth, mHeight, mpBuffer );
  }
}

void
Framebuffer::Private::Pbuffer::MakeCurrent()
{
  ::wglMakeCurrent( mDC, mGLRC );
}

void
Framebuffer::Private::Pbuffer::DoneCurrent()
{
  ::wglMakeCurrent( 0, 0 );
}
#endif // _WIN32

Framebuffer::Framebuffer( int width, int height )
: p( new Private )
{
  Private::SwapState s = { p->mTextures, p->mTextures + 1 };
  *p->mSwapState.Mutable() = s;
  p->mTextures[0].SetSize( width, height );
  p->mTextures[1].SetSize( width, height );
  FlushGLErrors();
  p->mPbuffer.Create( width, height );
  DebugOnGLError();
}

Framebuffer::~Framebuffer()
{
  p->mPbuffer.Destroy();
  delete p;
}

int
Framebuffer::Width() const
{
  return p->mTextures->Width();
}

int
Framebuffer::Height() const
{
  return p->mTextures->Height();
}

OpenGLTexture*
Framebuffer::Front()
{
  return p->mSwapState->front;
}

OpenGLTexture*
Framebuffer::Back()
{
  return p->mSwapState->back;
}

void
Framebuffer::Bind()
{
  p->mPbuffer.MakeCurrent();
  DebugOnGLError();
}

void
Framebuffer::Unbind()
{
  p->mPbuffer.DoneCurrent();
}

void
Framebuffer::SwapTextures()
{
  WithLocked( s = p->mSwapState.Mutable() )
  {
    p->mPbuffer.CopyPixels( s->back );
    std::swap( s->front, s->back );
  }
  DebugOnGLError();
}

void
Framebuffer::CopyToCurrentViewport()
{
  p->mSwapState->front->CopyToCurrentViewport();
}

void
OpenGLContext::Test()
{
  GLfloat f[] = { 1, 1, 1 };
  ::glLightfv( GL_LIGHT0, GL_AMBIENT, f );
  ::glEnable( GL_COLOR_MATERIAL );
  ::glDisable( GL_TEXTURE_2D );
  ::glColor4f( 0, 1, 0, 1 );
  ::glEnable( GL_NORMALIZE );
  ::glNormal3f( 0, 0, -1 );
  ::glBegin( GL_POLYGON );
  ::glVertex3f( 0, 0, 0 );
  ::glVertex3f( 20, 20, 20 );
  ::glVertex3f( 0, 20, -20 );
  ::glEnd();
}

} // namespace GUI
