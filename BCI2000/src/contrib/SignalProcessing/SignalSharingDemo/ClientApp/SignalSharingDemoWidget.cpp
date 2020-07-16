////////////////////////////////////////////////////////////////////////////////
// $Id$
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
#include "SignalSharingDemoWidget.h"

#include "Sockets.h"
#include "StringUtils.h"
#include "Streambuf.h"
#include "Thread.h"
#include "GenericSignal.h"
#include "Synchronized.h"
#include "Runnable.h"

#include <QPaintEvent>
#include <QPainter>

struct SignalSharingDemoWidget::Private
{
  SignalSharingDemoWidget* mpSelf;
  std::vector<QColor> mSignalColors;

  ServerTCPSocket mListeningSocket;
  Synchronized<bool> mConnected;
  SynchronizedObject<GenericSignal> mpSignal;

  Thread mThread;
  void ThreadFunc();
  MemberCall<void(Private*)> mThreadCall;
  void Invalidate();
  Private();
};

SignalSharingDemoWidget::Private::Private()
: mThreadCall(&Private::ThreadFunc, this),
  mThread(&mThreadCall, "SignalSharingDemoWidget listening/receiving thread")
{
  mSignalColors.resize(8);
  for(int i = 0; i < mSignalColors.size(); ++i)
    mSignalColors[i].setHsvF(i*1.0/mSignalColors.size(), 1, 0.9);
}

void
SignalSharingDemoWidget::Private::Invalidate()
{
  QMetaObject::invokeMethod(mpSelf, "update", Qt::QueuedConnection);
}

void
SignalSharingDemoWidget::Private::ThreadFunc()
{
  while(!mThread.Terminating())
  {
    while(mListeningSocket.Input().Wait())
    {
      ClientTCPSocket clientSocket;
      if(mListeningSocket.WaitForAccept(clientSocket, 0))
      {
        mConnected = true;
        Invalidate();
        UnbufferedIO buf;
        buf.SetInput(&clientSocket.Input());
        std::istream stream(&buf);
        while(stream && clientSocket.Input().Wait()) // will be interrupted by Thread::Terminate()
        {
          mpSignal.Mutable()->Unserialize(stream);
          Invalidate();
        }
        *mpSignal.Mutable() = GenericSignal();
        mConnected = false;
        Invalidate();
      }
    }
  }
}

SignalSharingDemoWidget::SignalSharingDemoWidget(const QString& inAddress)
: p(new Private)
{
  p->mpSelf = this;
  std::string address = inAddress.toStdString();
  p->mListeningSocket.Open(address);
  if(!p->mListeningSocket.Listening())
    throw std_runtime_error << "Cannot listen on " << address;
  p->mThread.Start();
}

SignalSharingDemoWidget::~SignalSharingDemoWidget()
{
  delete p;
}

void
SignalSharingDemoWidget::paintEvent(QPaintEvent* ev)
{
  ev->accept();
  WithLocked(pSignal = p->mpSignal.Const())
  {
    if(pSignal->Empty())
    {
      QPainter painter(this);
      painter.fillRect(ev->rect(), Qt::gray);
      painter.setPen(Qt::white);
      painter.drawText(geometry(), Qt::AlignCenter, 
        p->mConnected ? "Waiting for signal ..." : "Waiting for connection ...");
    }
    else
    {
      QPainter painter(this);
      painter.fillRect(ev->rect(), Qt::black);

      float radius = std::min(width(), height())/4.0;
      QPointF center = QPointF(width()/2.0, height()/2.0);

      double totalEnergy = 0;
      for(int ch = 0; ch < pSignal->Channels(); ++ch)
        for(int el = 0; el < pSignal->Elements(); ++el)
          totalEnergy += pSignal->Value(ch, el) * pSignal->Value(ch, el);
      float norm = totalEnergy > 0 ? ::sqrt(pSignal->Channels()*pSignal->Elements()/totalEnergy) : 0;

      for(int ch = 0; ch < pSignal->Channels(); ++ch)
      {
        std::vector<QPointF> line;
        line.reserve(pSignal->Elements());
        for(int el = 0; el < pSignal->Elements(); ++el)
        {
          float r = radius*(1 + norm*pSignal->Value(ch, el)/4),
                phi = el*2*Pi()/pSignal->Elements();
          line.push_back(QPointF(center.x() + r*::cos(phi), center.y() - r*::sin(phi)));
        }
        if(!line.empty())
          line.push_back(line.front());
        painter.setPen(p->mSignalColors[ch % p->mSignalColors.size()]);
        painter.drawPolyline(line.data(), line.size());
      }
    }
  }
}


