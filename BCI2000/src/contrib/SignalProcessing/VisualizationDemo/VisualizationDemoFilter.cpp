////////////////////////////////////////////////////////////////////////////////
// $Id$
// Author: juergen.mellinger@neurotechcenter.org
// Description: A demo that shows how to render an image into an offscreen
//   canvas, and display it in an operator window.
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
#include "VisualizationDemoFilter.h"
#include "GenericVisualization.h"
#include "BitmapImage.h"
#include "WorkerThread.h"
#include "gui/GUI.h"
#include <iomanip>

/*
BCI2000 GraphDisplay vs. QPainter rendering

GraphDisplay is a layer of abstraction that allows to render shapes and text objects into
a normalized coordinate system.
Code that uses GraphDisplay is most likely to survive breaking changes in the
drawing backend (currently Qt) and BCI2000 dependencies.

In contrast, QPainter rendering provides access to more complex drawing functions
but suffers from a limitation in Qt which makes text rendering impossible outside
the main GUI thread.
*/

// enable the following line to switch to QPainter rendering
//#define USE_QT_PAINTER 1

#if USE_QT_PAINTER
# include <QPainter>
#else
# include "gui/GraphDisplay.h"
# include "gui/Shapes.h"
# include "gui/TextField.h"
#endif

RegisterFilter(VisualizationDemoFilter, 2.Z);

struct VisualizationDemoFilter::Private
{
  RGBColor mBackground;
  int mHeight, mWidth, mDecimation;
  int mDecimationCounter;

  class VisualizationObject;
  std::vector<VisualizationObject*> mVisualizations;

  ~Private() { destroyVisualizations(); }
  void createVisualizations(const SignalProperties&, int maxWindows);
  void destroyVisualizations();
  void resetVisualizations();
  void updateVisualizations(const GenericSignal&);
  void waitForVisualizations();

  class VisualizationObject
  {
  public:
    VisualizationObject(const Private*, const std::string& visID);
    ~VisualizationObject();
    void setTitle(const std::string&, const std::string& info = "");
    void setPosition(const GUI::Rect&);
    void reset();
    void update(const GenericSignal&);
    void wait();

    struct Computation
    {
      int inputCh1, inputCh2;
      double result;
      void run(const GenericSignal&);
    } mComputation;

  private:
    void asyncReset();
    void asyncUpdate();

    MemberCall<void(VisualizationObject*)> mCallAsyncReset;
    MemberCall<void(VisualizationObject*)> mCallAsyncUpdate;

    const Private* p;
    WorkerThread mWorker;
    BitmapVisualization mVis;
    std::string mTitle, mInfo;
    GUI::Rect mPosition;
#if USE_QT_PAINTER
    QImage mImage;
#else
    GUI::GraphDisplay mImage;
    PieShape* mpShape;
    TextField* mpValueField, *mpInfoField;
#endif
  };
};

void
VisualizationDemoFilter::Private::createVisualizations(const SignalProperties& Input, int maxWindows)
{
  destroyVisualizations();

  int count = 0;
  for(int ch1 = 0; ch1 < Input.Channels(); ++ch1)
  {
    for(int ch2 = ch1; ch2 < Input.Channels(); ++ch2)
    {
      std::ostringstream oss;
      oss << "VisDemo" << std::setw(3) << std::setfill('0') << ++count;
      auto pVis = new VisualizationObject(this, oss.str());
      pVis->setTitle(
         "Vis Demo (" + Input.ChannelLabels()[ch1] + "<->" + Input.ChannelLabels()[ch2] + ")",
         Input.ChannelLabels()[ch1] + "\n" + Input.ChannelLabels()[ch2]
      );

      int xSpacing = 0, ySpacing = 0;
      GUI::Rect r;
      r.left = ch1*(mWidth + xSpacing) + xSpacing/2;
      r.right = r.left + mWidth;
      r.top = ch2*(mHeight + ySpacing) + ySpacing/2;
      r.bottom = r.top + mHeight;
      pVis->setPosition(r);

      pVis->mComputation.inputCh1 = ch1;
      pVis->mComputation.inputCh2 = ch2;
      mVisualizations.push_back(pVis);

      if(maxWindows > 0 && count >= maxWindows)
        break;
    }
  }
}

void
VisualizationDemoFilter::Private::destroyVisualizations()
{
  for(auto pVis : mVisualizations)
  {
    pVis->wait();
    delete pVis;
  }
  mVisualizations.clear();
}

void
VisualizationDemoFilter::Private::resetVisualizations()
{
  for(auto pVis : mVisualizations)
    pVis->reset();
}

void
VisualizationDemoFilter::Private::updateVisualizations(const GenericSignal& s)
{
  for(auto pVis : mVisualizations)
    pVis->update(s);
}

void
VisualizationDemoFilter::Private::waitForVisualizations()
{
  for(auto pVis : mVisualizations)
    pVis->wait();
}

VisualizationDemoFilter::Private::VisualizationObject::VisualizationObject(const Private* p, const std::string& visID)
: p(p),
  mVis(visID),
  mCallAsyncReset(&VisualizationObject::asyncReset, this),
  mCallAsyncUpdate(&VisualizationObject::asyncUpdate, this)
{
  ::memset(&mComputation, 0, sizeof(mComputation));
#if !USE_QT_PAINTER
  mpShape = new PieShape(mImage);
  mpShape->SetPositionX(0.5).SetPositionY(0.5);
  mpShape->SetWidth(1).SetHeight(1);
  mpShape->SetLineWidth(1);
  mpShape->SetColor(RGBColor::Black);
  mpShape->SetFillColor(RGBColor::Yellow);

  mpValueField = new TextField(mImage);
  mpValueField->SetPositionX(0.5).SetPositionY(0.8);
  mpValueField->SetWidth(1).SetHeight(0.2);
  mpValueField->SetColor(RGBColor::None);
  mpValueField->SetTextColor(RGBColor::Black);

  mpInfoField = new TextField(mImage);
  mpInfoField->SetPositionX(0.5).SetPositionY(0.5).SetAlignment(GUI::Alignment::Center);
  mpInfoField->SetWidth(0).SetScalingMode(GUI::ScalingMode::AdjustWidth);
  mpInfoField->SetTextHeight(0.45).SetHeight(0.5);
  mpInfoField->SetColor(RGBColor::White);
  mpInfoField->SetTextColor(RGBColor::Black);
#endif
}

VisualizationDemoFilter::Private::VisualizationObject::~VisualizationObject()
{
  mVis.Send(CfgID::Visible, false);
}

void
VisualizationDemoFilter::Private::VisualizationObject::setTitle(const std::string& title, const std::string& info)
{
  mTitle = title;
  mInfo = info.empty() ? title : info;
}

void
VisualizationDemoFilter::Private::VisualizationObject::setPosition(const GUI::Rect& pos)
{
#if USE_QT_PAINTER
  mImage = QImage(pos.Width(), pos.Height(), QImage::Format_RGB16);
#else
  GUI::DrawContext c = { 0 };
  c.rect.right = pos.Width();
  c.rect.bottom = pos.Height();
  mImage.SetContext(c);
#endif
  mPosition = pos;
}

void
VisualizationDemoFilter::Private::VisualizationObject::reset()
{
  if(!mWorker.Run(mCallAsyncReset))
    throw bcierr << "cannot initialize: worker is busy or dead";
}

void
VisualizationDemoFilter::Private::VisualizationObject::update(const GenericSignal& Input)
{
  mComputation.run(Input);
  mWorker.Run(mCallAsyncUpdate); // will fail if worker still busy
}

void
VisualizationDemoFilter::Private::VisualizationObject::wait()
{
  mWorker.Wait();
}

void
VisualizationDemoFilter::Private::VisualizationObject::Computation::run(const GenericSignal& Input)
{
  // To have something to visualize, compute the squared correlation between channels.
  double p0 = 0, p1_1 = 0, p1_2 = 0, p2_11 = 0, p2_12 = 0, p2_22 = 0;
  for(int i = 0; i < Input.Elements(); ++i)
  {
    double val1 = Input(inputCh1, i), val2 = Input(inputCh2, i);
    p0 += 1;
    p1_1 += val1;
    p1_2 += val2;
    p2_11 += val1*val1;
    p2_12 += val1*val2;
    p2_22 += val2*val2;
  }
  double var1 = p2_11/p0 - p1_1*p1_1/p0/p0,
         var2 = p2_22/p0 - p1_2*p1_2/p0/p0,
         cov12 = p2_12/p0 - p1_1*p1_2/p0/p0;
  result = cov12*cov12/var1/var2;
}

void
VisualizationDemoFilter::Private::VisualizationObject::asyncReset()
{
  mVis.Send(CfgID::Left, mPosition.left);
  mVis.Send(CfgID::Top, mPosition.top);
  mVis.Send(CfgID::Width, mPosition.Width());
  mVis.Send(CfgID::Height, mPosition.Height());
  mVis.Send(CfgID::WindowTitle, mTitle);
  mVis.Send(CfgID::WindowFrame, false);
  mVis.Send(CfgID::Visible, true);

#if USE_QT_PAINTER
  QPainter painter(&mImage);
  painter.fillRect(mImage.rect(), p->mBackground.ToQRgb());
  mVis.SendReferenceFrame(mImage);
#else
  mpInfoField->SetText(mInfo);
  mpInfoField->SetVisible(true);
  mpValueField->SetVisible(false);
  mpShape->SetVisible(false);
  mImage.SetColor(p->mBackground);
  mImage.Paint();
  mVis.SendReferenceFrame(mImage.BitmapData());
  mpShape->SetVisible(true);
  mpValueField->SetVisible(true);
  mpInfoField->SetVisible(false);
#endif
}

void
VisualizationDemoFilter::Private::VisualizationObject::asyncUpdate()
{
  double value = mComputation.result;

#if USE_QT_PAINTER

  QPainter painter(&mImage);
  painter.fillRect(mImage.rect(), p->mBackground.ToQRgb());
  painter.setBrush(Qt::yellow);
  int angle = 270*16;
  if(value == value) // not NaN
    angle = floor(360*16 * value + 0.5);
  painter.drawPie(mImage.rect(), 180*16-angle/2, angle);

  mVis.SendDifferenceFrame(mImage);
#else

  std::ostringstream oss;
  oss << std::setprecision(2) << std::fixed << value;
  mpValueField->SetText(oss.str());

  float angle = 270;
  if(value == value) // not NaN
    angle = 360 * value;
  mpShape->SetStartAngle(180 - angle/2).SetEndAngle(180 + angle/2);

  mImage.Paint();
  mVis.SendDifferenceFrame(mImage.BitmapData());
#endif
}

// Filter boilerplate code
VisualizationDemoFilter::VisualizationDemoFilter()
: p(new Private)
{
}

VisualizationDemoFilter::~VisualizationDemoFilter()
{
  delete p;
}

void VisualizationDemoFilter::Publish()
{
 BEGIN_PARAMETER_DEFINITIONS
  "VisualizationDemoFilter int VisImageWidth= 50 % 0 % // native image width",
  "VisualizationDemoFilter int VisImageHeight= 50 % 0 % // native image height",
  "VisualizationDemoFilter int VisImageBackground= 0x808080 % // image background color (color)",
  "VisualizationDemoFilter int VisImageDecimation= 1 % 1 % // update on every nth frame only",
  "VisualizationDemoFilter int VisMaxWindows= 0 % 0 % // maximum number of windows, 0 for any",
 END_PARAMETER_DEFINITIONS
}

void VisualizationDemoFilter::Preflight(
  const SignalProperties& Input,
        SignalProperties& Output) const
{
  Output = Input;
  Parameter("VisImageBackground");
}

void VisualizationDemoFilter::Initialize(
  const SignalProperties& Input,
  const SignalProperties& Output)
{
  p->mWidth = Parameter("VisImageWidth");
  p->mHeight = Parameter("VisImageHeight");
  p->mBackground = RGBColor(Parameter("VisImageBackground"));
  p->mDecimation = Parameter("VisImageDecimation");
  p->createVisualizations(Input, Parameter("VisMaxWindows"));
  p->resetVisualizations();
}

void VisualizationDemoFilter::Process(
  const GenericSignal& Input,
        GenericSignal& Output)
{
  Output = Input;
  if(p->mDecimationCounter == 0)
    p->updateVisualizations(Input);
  ++p->mDecimationCounter %= p->mDecimation;
}

void VisualizationDemoFilter::StartRun()
{
  p->mDecimationCounter = 0;
  p->waitForVisualizations();
  p->resetVisualizations();
}

void VisualizationDemoFilter::StopRun()
{
  p->waitForVisualizations();
  p->resetVisualizations();
}
