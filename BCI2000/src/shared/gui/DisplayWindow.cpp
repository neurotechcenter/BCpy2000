////////////////////////////////////////////////////////////////////////////////
// $Id: DisplayWindow.cpp 5810 2018-11-05 15:01:26Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de, griffin.milsap@gmail.com
// Description: A GraphDisplay descendant which is a frameless GUI window for
//   an application's user display.
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
#include "DisplayWindow.h"

#include "FileUtils.h"
#include "OpenGLContext.h"
#include "Thread.h"
#include "ThreadUtils.h"
#include "Bitmap.h"
#include "DisplayClock.h"
#include "EnvVariable.h"
#include "SelfPipeQueue.h"
#include "SimpleStatistics.h"

#if USE_QT
# include <QWindow>
# include <QScreen>
# include <QMouseEvent>
# include <QThread>
#endif

using namespace GUI;

namespace
{
#if USE_QT
class GuiWindow : QWindow
{
public:
  GuiWindow( GUI::DisplayWindow& inParent )
  : mrParent( inParent )
  {
    setFlags( Qt::FramelessWindowHint );
    setSurfaceType( QSurface::OpenGLSurface );
    setPosition( 0, 0 );
    Handle();
  }
  GUI::OSWindowHandle Handle()
  {
    return GUI::OSWindowHandle( winId() );
  }
  void Adjust( bool& outScreenChanged )
  {
    QPlatformScreen* pOriginalScreen = screen()->handle();
    qreal originalRate = screen()->refreshRate();
    setTitle( QString::fromLocal8Bit( mrParent.Title().c_str() ) );
    resize( mrParent.Width(), mrParent.Height() );
    setPosition( mrParent.Left(), mrParent.Top() );
    mrParent.Visible() ? show() : hide();
    outScreenChanged = screen()->handle() != pOriginalScreen || screen()->refreshRate() != originalRate;
  }
  GUI::Rect PhysicalRect_mm() const
  {
    auto wpix = this->geometry(), spix = screen()->geometry();
    wpix.translate(-spix.topLeft());
    spix.translate(-spix.topLeft());
    GUI::Rect smm = PhysicalScreenRect_mm();
    GUI::Rect wmm =
    {
      wpix.left()*smm.Width()/spix.width(),
      wpix.top()*smm.Height()/spix.height(),
      wpix.right()*smm.Width()/spix.width(),
      wpix.bottom()*smm.Height()/spix.height()
    };
    return wmm;
  }
  GUI::Rect PhysicalScreenRect_mm() const
  {
    auto size = screen()->physicalSize();
    GUI::Rect r = { 0, 0, size.width(), size.height() };
    return r;
  }
protected:
  void mousePressEvent( QMouseEvent* ev )
  {
    if( ev->button() == Qt::LeftButton )
    {
      GUI::Point p = { static_cast<float>(ev->x()), static_cast<float>(ev->y()) };
      mrParent.Click( p );
      ev->accept();
    }
  }
  GUI::DisplayWindow& mrParent;
};
#endif // USE_QT

class OffscreenBuffer : public Lockable<>
{
public:
  OffscreenBuffer()
  : mWidth( 0 ), mHeight( 0 ), mValid( false ), 
    mpGL( nullptr ), mpFramebuffer( nullptr ) {}
  ~OffscreenBuffer() { Destroy(); }

  bool Valid() const { return mValid; }
  int Width() const { return mWidth; }
  int Height() const { return mHeight; }

  OffscreenBuffer& SetWidth( int w )
  {
    mValid = false;
    mWidth = w;
    return *this;
  }
  OffscreenBuffer& SetHeight( int h )
  {
    mValid = false;
    mHeight = h;
    return *this;
  }

  void Create( OpenGLContext* pContext, const RGBColor& c )
  {
    Assert( !mpGL );
    if( mWidth > 0 && mHeight > 0 )
    {
      mpGL = new OpenGLContext( pContext );
      WithGLContext( mpGL )
      {
        mpFramebuffer = new Framebuffer( mWidth, mHeight );
        mpGL->SetRenderTarget( mpFramebuffer );
        mpGL->SetViewport( 0, 0, mWidth, mHeight );
      }
    }
    mValid = true;
  }
  void Destroy()
  {
    mValid = false;
    delete &*mpGL;
    mpGL = nullptr;
    delete mpFramebuffer;
    mpFramebuffer = nullptr;
  }
  OpenGLContext* Gl() { return mpGL; }
  Framebuffer* Fb() { return mpFramebuffer; }

private:
  int mWidth, mHeight;
  Synchronized<bool> mValid;
  Synchronized<OpenGLContext*> mpGL;
  Framebuffer* mpFramebuffer;
};

} // namespace

struct DisplayWindow::Data
{
  GUI::DisplayWindow* mpSelf;

  std::string mTitle;
  int mTop, mLeft, mWidth, mHeight;
  bool mVisible;
  Synchronized<bool> mAsyncRendering;
  Synchronized<int> mUpdateRateDecimation;

  SynchronizedObject<ClockTick, Mutex> mFrame;
  SynchronizedObject<ClockTick::Statistics, Mutex> mFrameClockStatistics;
  SynchronizedObject<struct RenderStatistics, Mutex> mRenderStatistics;
  SynchronizedObject<struct PresentationStatistics, Mutex> mPresentationStatistics;
  SynchronizedObject<StringUtils::NameValueList> mpInfo;

  GuiWindow* mpWindow;
  OpenGLContext* mpWindowGL;
  OffscreenBuffer mScaledContentBuffer;
  SelfPipeQueue<RenderState, 64> mRenderingQueue;

  int RenderingThreadFunc();
  struct RenderingThread : Thread
  {
    DisplayWindow::Data* d;
    RenderingThread( DisplayWindow::Data* d )
    : Thread( "Rendering Thread" ), d( d ) {}
    int OnExecute() { return d->RenderingThreadFunc(); }
    ~RenderingThread() { TerminateAndWait(); }
    void Stop() { d->mStopRendering = true; Wait(); d->mStopRendering = false; }
  } mRenderingThread;
  Synchronized<bool> mStopRendering;

  DisplayWindow& OnChange();
  bool Adjust( GUI::DrawContext& );
  void Update( void* );

  Data( DisplayWindow* );
  ~Data();
};

DisplayWindow::Data::Data( DisplayWindow* pSelf )
: mpSelf( pSelf ),
  mTitle( FileUtils::ApplicationTitle() ),
  mLeft( 0 ), mTop( 0 ), mWidth( 0 ), mHeight( 0 ),
  mVisible( false ),
  mRenderingThread( this ),
  mpWindow( 0 ),
  mpWindowGL( 0 ),
  mFrame( ClockTick::Null )
{
  mpWindow = new GuiWindow( *pSelf );
  mAsyncRendering = false;
  mUpdateRateDecimation = 0;
}

DisplayWindow::Data::~Data()
{
  mAsyncRendering = false;
  mRenderingThread.Stop();
  delete mpWindowGL;
  delete mpWindow;
}

bool
DisplayWindow::Data::Adjust( GUI::DrawContext& dc )
{
  bool sizeChanged = false;
  if( mWidth != dc.rect.Width() || mHeight != dc.rect.Height() )
  {
    dc.rect.left = 0;
    dc.rect.top = 0;
    dc.rect.right = mWidth;
    dc.rect.bottom = mHeight;
    sizeChanged = true;
  }
  bool screenChanged = false;
  mpWindow->Adjust( screenChanged );
  if( sizeChanged || screenChanged || !mVisible )
  {
    mRenderingThread.Stop();
    mRenderingQueue.Clear();

    delete mpWindowGL;
    mpWindowGL = nullptr;
    if( dc.gl )
    {
      sizeChanged = true;
      dc.gl = 0;
    }
  }
  if( mVisible )
  {
    OpenGLContext* pGL = mpWindowGL;
    if( !pGL )
    {
      Assert( mpWindow->Handle() );
      pGL = new OpenGLContext( mpWindow->Handle() );
      if( pGL->Valid() )
      {
        Assert( mpWindowGL == nullptr );
        mpWindowGL = pGL;
        GUI::Rect screen = mpWindow->PhysicalScreenRect_mm(),
                  window = mpWindow->PhysicalRect_mm();
        sizeChanged = true;
        WithLocked(&info = *mpInfo.Mutable())
        {
          info.clear();
          info["GLVendor"] << pGL->Vendor();
          info["GLRenderer"] << pGL->Renderer();
          info["GLVersion"] << pGL->Version();
          info["PhysicalWidth"] << window.Width() << "mm";
          info["PhysicalHeight"] << window.Height() << "mm";
          info["PhysicalLeftEdge"] << window.left << "mm";
          info["PhysicalTopEdge"] << window.top << "mm";
          info["PhysicalScreenWidth"] << screen.Width() << "mm";
          info["PhysicalScreenHeight"] << screen.Height() << "mm";
        }
        mRenderingThread.Start();
      }
      else
        delete pGL;
    }
  }
  return sizeChanged || screenChanged;
}

void
DisplayWindow::Data::Update( void* p )
{
  mRenderingQueue.Push( RenderState( -1, TimeUtils::MonotonicTime(), p ) );
}

int
DisplayWindow::Data::RenderingThreadFunc()
{
  OpenGLContext renderGL( mpWindowGL );
  Framebuffer* pContentBuffer = nullptr;
  GUI::DrawContext dc = mpSelf->Context();
  WithGLContext( renderGL )
  {
    pContentBuffer = new Framebuffer( dc.rect.Width(), dc.rect.Height() );
    renderGL.SetRenderTarget( pContentBuffer );
  }
  dc.gl = &renderGL;
  mpSelf->SetContext( dc );
  WithLock( mScaledContentBuffer )
    mScaledContentBuffer.Create( &renderGL, mpSelf->Color() );

  AV::DisplayClock clock( mpWindow->Handle() );
  ClockTick frame = clock.LastVbl();
  WithLocked( p = mFrameClockStatistics.Mutable() )
  {
    ::memset( &*p, 0, sizeof(*p) );
    p->type = frame.type;
  }
  WithLocked( p = mPresentationStatistics.Mutable() )
    ::memset( &*p, 0, sizeof(*p) );
  WithLocked( p = mRenderStatistics.Mutable() )
    ::memset( &*p, 0, sizeof(*p) );
  WithLocked( p = mFrame.Mutable() )
  {
    *p = frame;
    --p->count;
    p->time -= frame.rate.Period();
  }
  SimpleStatistics<float>
    frameObs( frame.rate.Period().Seconds(), 0.005, 100 ),
    renderObs( frame.rate.Period().Seconds(), 0.01, 100 ),
    latencyObs;
  const int goodStatisticsMinCount = 30;
  latencyObs.SetWindowLength( goodStatisticsMinCount ).Reset();

  int framesPerUpdate = 1;
  std::vector<RenderState> requests;
  requests.reserve( mRenderingQueue.MaxSize() );
  requests.push_back( RenderState( -1 ) );

  std::string task;
  if( !EnvVariable::Get( "GUI::DisplayWindow::TaskName", task ) )
    task = "Pro Audio";
  WithThreadTask( task, ThreadUtils::Priority::Maximum )
  {
    const Time::Interval timeSlice = Time::Seconds( 1e-3 );
    ThreadUtils::TimeSlice ts( timeSlice );
    while( !mStopRendering )
    {
      // update to current frame
      frame = clock.LastVbl();
      int frameDelta;
      Time::Interval timeDelta;
      WithLocked( p = mFrame.Mutable() )
      {
        timeDelta = frame.time - p->time;
        frameDelta = Round( ( frame.time - p->time ) * frame.rate );
        frame.count = p->count + frameDelta;
        *p = frame;
      }
      // update frame clock statistics
      for( int i = 0; i < frameDelta; ++i )
        frameObs.Observe( timeDelta.Seconds() / frameDelta );
      WithLocked( p = mFrameClockStatistics.Mutable() )
      {
        if( frameDelta > framesPerUpdate )
          p->missed += ( frameDelta - 1 ) / framesPerUpdate;
        p->interval = Time::Seconds( frameObs.Mean() );
        p->jitter = Time::Seconds( ::sqrt( frameObs.Variance() ) );
      }
      // determine rendering deadline
      ClockTick nextFrame = frame;
      nextFrame.count += framesPerUpdate;
      nextFrame.time += framesPerUpdate * frame.rate.Period();

      Time renderBegin = TimeUtils::MonotonicTime();
      // pop all requests from rendering queue
      for( RenderState r; mRenderingQueue.Pop( r ); )
      {
        if( r.contentFrame < 0 || r.contentFrame > nextFrame.count )
          r.contentFrame = nextFrame.count;
        requests.push_back( r );
      }
      // update GraphObjects state
      if( !requests.empty() )
      {
        bool presentation = mpSelf->Tick( nextFrame );
        for( auto& r : requests )
        {
          r.presentation = presentation;
          mpSelf->Emit( OnUpdateReceived, &r );
        }
      }
      if( mpSelf->InvalidRegion().Empty() )
        requests.clear();
      else
      { // paint into content buffer
        mpSelf->Paint();
        auto& b = mScaledContentBuffer;
        if( b.Gl() ) WithLock( b ) WithGLContext( b.Gl() )
        {
          // copy content buffer to visualization backbuffer
          b.Gl()->Clear( mpSelf->Color() );
          pContentBuffer->Front()->CopyToCurrentViewport();
          // swap visualization back and front buffer pointers
          b.Gl()->SwapBuffers();
        }
        Time renderEnd = TimeUtils::MonotonicTime();
        renderObs.Observe( (renderEnd - renderBegin).Seconds() );
      }

      Time blitBegin = TimeUtils::MonotonicTime(), swapEnd;
      Time timeDone = nextFrame.time - nextFrame.rate.Period() / 2;
      WithGLContext( mpWindowGL ) do
      {
        // copy content buffer to OpenGL display backbuffer
        mpWindowGL->SetViewport( 0, 0, mWidth, mHeight );
        mpWindowGL->Clear( mpSelf->Color() );
        pContentBuffer->Front()->CopyToCurrentViewport();
        // Wait for vbl and swap OpenGL back and front buffers
        mpWindowGL->SwapBuffers();
        if( mpWindowGL->SwapInterval() < 1 )
          clock.WaitForVbl();
        swapEnd = clock.LastVbl().time;
        if( mpWindowGL->SoftwareRasterized() )
          while( swapEnd < nextFrame.time )
            swapEnd = clock.WaitForVbl().LastVbl().time;
      } while( swapEnd < timeDone );

      // time to adapt scaled content buffer
      if( !mScaledContentBuffer.Valid() ) WithLock( mScaledContentBuffer )
      {
        mScaledContentBuffer.Destroy();
        mScaledContentBuffer.Create( &renderGL, mpSelf->Color() );
      }

      // UpdateRateDecimation property may have changed
      framesPerUpdate = mUpdateRateDecimation;
      bool adaptUpdateRate = framesPerUpdate < 1;
      if( adaptUpdateRate )
        framesPerUpdate = 1;

      // update render statistics
      WithLocked( p = mRenderStatistics.Mutable() )
      {
        p->render.mean = Time::Seconds( renderObs.Mean() );
        p->render.std = Time::Seconds( ::sqrt( renderObs.Variance() ) );
        if( adaptUpdateRate && renderObs.Count() > goodStatisticsMinCount )
          framesPerUpdate = Ceil( ( p->render.mean + p->render.std ) * frame.rate );
        Assert( framesPerUpdate > 0 );
        p->decimation = framesPerUpdate;
      }

      // update presentation statistics
      //   displayTime is VBL time _before_ transmission of full frame
      //   this is lower bound for CRT, and one frame short for FP displays
      int frameDelay = Floor((swapEnd - nextFrame.time) * frame.rate);
      int64_t displayFrame = nextFrame.count + frameDelay;
      Time displayTime = nextFrame.time + frameDelay * frame.rate.Period();
      for( auto& r : requests )
      {
        r.displayFrame = displayFrame;
        r.displayed = displayTime;
        if( r.contentFrame < r.displayFrame )
          ++mPresentationStatistics.Mutable()->late;
        if( !IsNaN( r.requested ) )
          latencyObs.Observe( (r.displayed - r.requested).Seconds() );
        mpSelf->Emit( OnUpdateDisplayed, &r );
      }
      WithLocked( p = mPresentationStatistics.Mutable() )
      {
        p->count += requests.empty() ? 0 : 1;
        if( latencyObs.Count() == 0 )
        {
          p->latency.mean = Time::Forever;
          p->latency.std = Time::Forever;
        }
        else
        {
          p->latency.mean = Time::Seconds( latencyObs.Mean() );
          p->latency.std = Time::Seconds( ::sqrt( latencyObs.Variance() ) );
        }
      }
      requests.clear();

      // Request next frame
      if( mAsyncRendering && !mStopRendering )
        mRenderingQueue.Push( RenderState( displayFrame + framesPerUpdate ) );
    }
  }
  mRenderingQueue.Clear();
  WithLock( mScaledContentBuffer )
    mScaledContentBuffer.Destroy();
  delete pContentBuffer;
  dc = mpSelf->Context();
  dc.gl = nullptr;
  mpSelf->SetContext( dc );
  return 0;
}

DisplayWindow&
DisplayWindow::Data::OnChange()
{
  DrawContext dc = mpSelf->Context();
  if( Adjust( dc ) )
    mpSelf->SetContext( dc );
  return *mpSelf;
}

// DisplayWindow
DisplayWindow::DisplayWindow()
: d( new Data( this ) )
{
}

DisplayWindow::~DisplayWindow()
{
  delete d;
}

#define PROPERTY_DEF( name, type, apply ) \
DisplayWindow& DisplayWindow::Set##name( type value ) { d->m##name = value; return apply; } \
type DisplayWindow::name() const { return d->m##name; }

PROPERTY_DEF( Title, const std::string&, d->OnChange() )
PROPERTY_DEF( Left, int, d->OnChange() )
PROPERTY_DEF( Top, int, d->OnChange() )
PROPERTY_DEF( Width, int, d->OnChange() )
PROPERTY_DEF( Height, int, d->OnChange() )
PROPERTY_DEF( Visible, bool, d->OnChange() )
PROPERTY_DEF( AsyncRendering, bool, d->OnChange() )
PROPERTY_DEF( UpdateRateDecimation, int, *this )

DisplayWindow&
DisplayWindow::Update( void* p )
{
  d->Update( p );
  return *this;
}

ClockTick
DisplayWindow::CurrentFrame() const
{
  return *d->mFrame;
}

ClockTick::Statistics
DisplayWindow::FrameClockStatistics() const
{
  return *d->mFrameClockStatistics;
}

struct DisplayWindow::RenderStatistics
DisplayWindow::RenderStatistics() const
{
  return *d->mRenderStatistics;
}

struct DisplayWindow::PresentationStatistics
DisplayWindow::PresentationStatistics() const
{
  return *d->mPresentationStatistics;
}

StringUtils::NameValueList
DisplayWindow::Info() const
{
  return *d->mpInfo;
}

DisplayWindow&
DisplayWindow::ConfigureBitmapBuffer( int inWidth, int inHeight )
{
  OffscreenBuffer& b = d->mScaledContentBuffer;
  WithLock( b )
    b.SetWidth( inWidth ).SetHeight( inHeight );
  return *this;
}

void
DisplayWindow::OnBitmapData( GUI::Bitmap& ioImage, const GUI::Rect* inpRect ) const
{
  OffscreenBuffer& b = d->mScaledContentBuffer;
  if( b.Valid() )
    WithLock( b ) WithGLContext( b.Gl() )
      ioImage.CopyFrom( b.Fb()->Front() );
  else
    GraphDisplay::OnBitmapData( ioImage, inpRect );
}
