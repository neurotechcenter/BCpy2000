////////////////////////////////////////////////////////////////////
// $Id$
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A tool to manipulate OS settings and to produce
//  artificial system load. May be used to investigate how scheduler
//  and system load affect the results of timing measurements.
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
////////////////////////////////////////////////////////////////////
#include <iostream>
#include <iomanip>

#include <QApplication>
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QTimer>
#include <QPushButton>
#include <QBoxLayout>
#include <QGroupBox>
#include <QMoveEvent>

#include "QtSignalConnector.h"
#include "ThreadUtils.h"
#include "Thread.h"
#include "StringUtils.h"
#include "LCRandomGenerator.h"

namespace {
	std::string toString(const QString& s) { return s.toLocal8Bit().constData(); }
}

#if _WIN32
# include <Pdh.h>

class CpuLoadCounter
{
  PDH_HQUERY mQuery;
  PDH_HCOUNTER mCounter;
public:
  CpuLoadCounter() : mQuery( 0 ), mCounter( 0 )
  {
    if( FAILED( ::PdhOpenQueryA( nullptr, 0, &mQuery ) ) )
      return;
    if( FAILED( ::PdhAddEnglishCounterA( mQuery, "\\Processor(_Total)\\% Processor Time", 0, &mCounter ) ) )
      return;
    ::PdhCollectQueryData( mQuery );
  }
  ~CpuLoadCounter()
  {
    ::PdhCloseQuery( mQuery );
  }
  double GetValue()
  {
    PDH_FMT_COUNTERVALUE value;
    if( FAILED( ::PdhCollectQueryData( mQuery ) )
        || FAILED( ::PdhGetFormattedCounterValue( mCounter, PDH_FMT_DOUBLE, nullptr, &value ) ) )
           return NaN<double>();
    return value.doubleValue / 100.0;
  }
};

#endif // _WIN32

struct CollapsingGroup : QGroupBox, QtSignalConnector
{
  QSize mMinimumSize;

  CollapsingGroup( QString s, QWidget* p = 0 )
  : QGroupBox( s, p ),
    mMinimumSize( 0, 0 )
  {
    setFlat( true );
#if 0
    setCheckable( true );
#endif
    Connect(
      this, SIGNAL(toggled(bool)),
      MemberCall<void(CollapsingGroup*)>( &CollapsingGroup::OnToggled, this )
    );
  }

  void OnToggled()
  {
    if( mMinimumSize.width() == 0 )
      mMinimumSize.setWidth( sizeHint().width() );
    auto children = findChildren<QWidget*>();
    for( auto child : children )
      child->setVisible( isChecked() );
  }

  QSize minimumSizeHint() const override
  {
    return mMinimumSize;
  }

};

struct TimeSliceControl : CollapsingGroup
{
  QLineEdit* mpActual;
  QComboBox* mpSelected;
  ThreadUtils::TimeSlice* mpTimeSlice;

  TimeSliceControl( QWidget* p = 0 )
  : CollapsingGroup( "Time Slice", p ),
    mpSelected( new QComboBox( this ) ),
    mpActual( new QLineEdit( this ) ),
    mpTimeSlice( 0 )
  {
    QHBoxLayout* pLayout = new QHBoxLayout;
    pLayout->addWidget( new QLabel( "Set max duration (ms):" ) );
    pLayout->addWidget( mpSelected );
    pLayout->addStretch();
    pLayout->addWidget( new QLabel( "Current actual duration (ms):" ) );
    pLayout->addWidget( mpActual );
    const char* sliceItems[] = { "1.0", "2.5", "5.0", "10.0", "20.0", };
    for( int i = 0; i < sizeof(sliceItems)/sizeof(*sliceItems); ++i )
      mpSelected->addItem( sliceItems[i] );
    mpSelected->setCurrentText( "10.0" );
    mpSelected->setEditable( true );
    Connect( mpSelected, SIGNAL(currentTextChanged(QString)), MemberCall<void(TimeSliceControl*)>( &TimeSliceControl::OnChange, this ) );

    mpActual->setReadOnly( true );
    OnTimer();

    setLayout( pLayout );

    QTimer* pTimer = new QTimer( this );
    Connect( pTimer, SIGNAL(timeout()), MemberCall<void(TimeSliceControl*)>( &TimeSliceControl::OnTimer, this ) );
    pTimer->start( 200 );
 }

  ~TimeSliceControl()
  {
    delete mpTimeSlice;
  }

  void OnTimer()
  {
    Time::Interval d = ThreadUtils::TimeSlice::Duration();
    String s;
    s << std::fixed << std::setprecision( 1 ) << 1e3 * d.Seconds();
    mpActual->setText( QString::fromLocal8Bit( s.c_str() ) );
  }
  
  void OnChange()
  {
    String s = mpSelected->currentText().toLocal8Bit().constData();
    double d;
    if( s >> d && d > 0 )
    {
      delete mpTimeSlice;
      mpTimeSlice = new ThreadUtils::TimeSlice( Time::Seconds( d * 1e-3 ) );
    }
    OnTimer();
  }
};

struct CycleConsumer : Thread
{
  int mMflop, mPriority, mSleep;
  CycleConsumer()
  : mMflop( 0 ), mPriority( 0 ), mSleep( 0 )
  {}
  ~CycleConsumer()
  {
    TerminateAndWait();
  }
  int OnExecute()
  {
    LCRandomGenerator rg;
    WithThreadPriority( mPriority )
    {
      while( !Terminating() )
      {
        volatile float f = 0.0;
        for( int i = 0; i < mMflop; ++i )
          for( int j = 0; j < 1000*1000; ++j )
            f += 1;
        if( mSleep <= 0 )
          ThreadUtils::Idle();
        else
          ThreadUtils::SleepForMs( rg( 2 * mSleep ) + 1 );
      }
    }
    return 0;
  }
};

struct ThreadControl : CollapsingGroup
{
  QComboBox* mpProcessPriority, *mpSleep, *mpCount, *mpPriority, *mpStep;
  QLineEdit* mpThreadCount;
  std::vector<QWidget*> mNeedThreads;
  std::vector<Thread*> mThreads;

  ThreadControl( QWidget* pParent = 0 )
  : CollapsingGroup( "Threads", pParent ),
    mpProcessPriority( new QComboBox( this ) ),
    mpSleep( new QComboBox( this ) ),
    mpCount( new QComboBox( this ) ),
    mpPriority( new QComboBox( this ) ),
    mpStep( new QComboBox( this ) ),
    mpThreadCount( new QLineEdit( this ) )
  {
    QVBoxLayout* pMainLayout = new QVBoxLayout;
    QHBoxLayout* pLayout = new QHBoxLayout;
#if _WIN32
    pLayout ->addWidget( new QLabel( "With process priority class" ) );
    pLayout ->addWidget( mpProcessPriority );
    const struct { const char* name; int value; } procPriorities[] =
    {
      { "Highest (Realtime, requires admin)", REALTIME_PRIORITY_CLASS },
      { "High", HIGH_PRIORITY_CLASS },
      { "Above Normal", ABOVE_NORMAL_PRIORITY_CLASS },
      { "Normal", NORMAL_PRIORITY_CLASS },
      { "Below Normal", BELOW_NORMAL_PRIORITY_CLASS },
      { "Lowest (Idle)", IDLE_PRIORITY_CLASS },
    };
    for( int i = 0; i < sizeof(procPriorities)/sizeof(*procPriorities); ++i )
      mpProcessPriority->addItem( procPriorities[i].name, procPriorities[i].value );
    UpdateProcessPriority();
    Connect( mpProcessPriority, SIGNAL(currentIndexChanged(int)), MemberCall<void(ThreadControl*)>( &ThreadControl::OnProcessPriority, this ) );
    pMainLayout->addLayout( pLayout );
    pLayout = new QHBoxLayout;
#endif

    pLayout->addWidget( new QLabel( "Sleep for" ) );
    pLayout->addWidget( mpSleep );
    pLayout->addWidget( new QLabel( "ms after consuming" ) );
    const char* countItems[] = { "0", "1", "5", "10", "50", "100", "500", "1000", };
    for( int i = 0; i < sizeof(countItems)/sizeof(*countItems); ++i )
    {
      mpSleep->addItem( countItems[i] );
      mpCount->addItem( countItems[i] );
      mpStep->addItem( countItems[i] );
    }
    mpSleep->setEditable( true );
    mpCount->setEditable( true );
    mpStep->setEditable( true );
    mpSleep->setCurrentText( "10" );
    mpCount->setCurrentText( "5" );
    mpStep->setCurrentText( "1" );
    pLayout->addWidget( mpCount );

    pLayout->addWidget( new QLabel( "Mflop with thread priority" ) );
    struct { const char* text; int value; } priorityItems[] =
    {
      { "maximum", ThreadUtils::Priority::Maximum },
      { "maximum-1", ThreadUtils::Priority::Maximum - 1 },
      { "maximum-2", ThreadUtils::Priority::Maximum - 2 },
      { 0, 0 },
      { "+2", 2 },
      { "+1", 1 },
      { "default", 0 },
      { "-1", -1 },
      { "-2", -2 },
      { 0, 0 },
      { "minimum+2", ThreadUtils::Priority::Minimum + 2 },
      { "minimum+1", ThreadUtils::Priority::Minimum + 1 },
      { "minimum", ThreadUtils::Priority::Minimum },
    };
    for( int i = 0; i < sizeof(priorityItems)/sizeof(*priorityItems); ++i )
    {
      if( priorityItems[i].text )
        mpPriority->addItem( priorityItems[i].text, priorityItems[i].value );
      else
        mpPriority->insertSeparator( mpPriority->count() );
    }
    mpPriority->setCurrentText( "minimum" );
    pLayout->addWidget( mpPriority );
    pMainLayout->addLayout( pLayout );

    pLayout = new QHBoxLayout;
    QPushButton* pButton = new QPushButton( "Create" );
    pLayout->addWidget( pButton );
    Connect( pButton, SIGNAL(clicked()), MemberCall<void(ThreadControl*)>( &ThreadControl::OnSpawn, this ) );
    pLayout->addWidget( new QLabel( "or" ) );
    pButton = new QPushButton( "Kill" );
    pLayout->addWidget( pButton );
    mNeedThreads.push_back( pButton );
    Connect( pButton, SIGNAL(clicked()), MemberCall<void(ThreadControl*)>( &ThreadControl::OnKill, this ) );
    pLayout->addWidget( mpStep );
    pLayout->addWidget( new QLabel( "threads" ) );

    pLayout->addStretch();
    pButton = new QPushButton( "Kill all" );
    pLayout->addWidget( pButton );
    mNeedThreads.push_back( pButton );
    Connect( pButton, SIGNAL(clicked()), MemberCall<void(ThreadControl*)>( &ThreadControl::OnKillAll, this ) );
    pLayout->addStretch();
    pLayout->addWidget( new QLabel( "threads running:" ) );
    mpThreadCount->setReadOnly( true );
    mpThreadCount->setMaximumWidth( 50 );
    mpThreadCount->setMinimumWidth( 10 );
    pLayout->addWidget( mpThreadCount );
    pMainLayout->addLayout( pLayout );

    setLayout( pMainLayout );
    UpdateThreadCount();

    QTimer* pTimer = new QTimer( this );
    Connect( pTimer, SIGNAL(timeout()), MemberCall<void(ThreadControl*)>( &ThreadControl::UpdateProcessPriority, this ) );
    pTimer->start( 250 );
  }

  ~ThreadControl()
  {
    OnKillAll();
  }

  void UpdateThreadCount()
  {
    String count;
    count << mThreads.size();
    mpThreadCount->setText( QString::fromLocal8Bit( count.c_str() ) );
    for( int i = 0; i < mNeedThreads.size(); ++i )
      mNeedThreads[i]->setEnabled( mThreads.size() > 0 );
  }

  void UpdateProcessPriority()
  {
    bool wereBlocked = mpProcessPriority->blockSignals( true );
    int priority = ::GetPriorityClass( ::GetCurrentProcess() );
    for( int i = 0; i < mpProcessPriority->count(); ++i )
      if( mpProcessPriority->itemData( i ) == priority )
        mpProcessPriority->setCurrentIndex( i );
    mpProcessPriority->blockSignals( wereBlocked );
  }

  void OnProcessPriority()
  {
    int priority = mpProcessPriority->currentData().value<int>();
    ::SetPriorityClass( ::GetCurrentProcess(), priority );
    UpdateProcessPriority();
  }

  void OnSpawn()
  {
    int n = mpStep->currentText().toInt();
    for( int i = 0; i < n; ++i )
    {
      CycleConsumer* p = new CycleConsumer;
      p->mPriority = mpPriority->currentData().toInt();
      String(toString(mpSleep->currentText())) >> p->mSleep;
      String(toString(mpCount->currentText())) >> p->mMflop;
      p->Start();
      if( !p->Running() )
        throw String() << "Could not start thread";
      mThreads.push_back( p );
      UpdateThreadCount();
    }
  }

  void OnKill()
  {
    WithThreadPriority( ThreadUtils::Priority::Maximum )
    {
      int n = std::min<int>( mpStep->currentText().toInt(), mThreads.size() ),
          newSize = mThreads.size() - n;
      for( int i = newSize; i < mThreads.size(); ++i )
      {
        mThreads[i]->Terminate();
        delete mThreads[i];
      }
      mThreads.resize( newSize );
      UpdateThreadCount();
    }
  }

  void OnKillAll()
  {
    WithThreadPriority( ThreadUtils::Priority::Maximum )
    {
      for( int i = 0; i < mThreads.size(); ++i )
      {
        mThreads[i]->Terminate();
        delete mThreads[i];
      }
      mThreads.clear();
      UpdateThreadCount();
    }
  }
};

struct Window : QWidget, QtSignalConnector
{
  Window() : mpToolWindow( new ToolWindow( this ) )
  {
    setWindowFlags(
      Qt::Window
      | Qt::MSWindowsFixedSizeDialogHint
      | Qt::CustomizeWindowHint
      | Qt::WindowMinimizeButtonHint
    );
    resize( 0, 0 );
    move( -32767, -32767 );
    mpToolWindow->move( 0, 0 );
    mpToolWindow->show();

    mTitle = qApp->applicationName() + " : CPU load %1%";

    QTimer* pTimer = new QTimer( this );
    Connect( pTimer, SIGNAL(timeout()), MemberCall<void(Window*)>( &Window::OnTimer, this ) );
    OnTimer();
    pTimer->start( 1000 );
  }

  void changeEvent( QEvent* ev ) override
  {
    if( ev->type() == QEvent::WindowStateChange )
    {
      if( windowState() == Qt::WindowMinimized )
        mpToolWindow->setVisible( !mpToolWindow->isVisible() );
      else
        showMinimized();
    }
  }

  void OnTimer()
  {
    mpToolWindow->setWindowTitle( mTitle.arg( Round( mLoadCounter.GetValue() * 1000 ) / 10.0 ) );
  }

  struct ToolWindow : QWidget
  {
    ToolWindow( QWidget* pAppWindow ) : QWidget( pAppWindow )
    {
      setWindowFlags( Qt::Tool | Qt::WindowStaysOnTopHint );
      QLayout* pLayout = new QVBoxLayout;
      pLayout->addWidget( new TimeSliceControl );
      pLayout->addWidget( new ThreadControl );
      pLayout->setSizeConstraint( QLayout::SetFixedSize );
      setLayout( pLayout );
    }
    void closeEvent( QCloseEvent* )
    { parentWidget()->close(); }
  }* mpToolWindow;
  QString mTitle;
  CpuLoadCounter mLoadCounter;
};

int
main( int argc, char** argv )
{
  QApplication app( argc, argv );
  Window w;
  w.show();
  app.exec();
  return 0;
}

