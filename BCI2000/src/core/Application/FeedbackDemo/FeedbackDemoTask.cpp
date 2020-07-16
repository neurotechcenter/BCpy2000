////////////////////////////////////////////////////////////////////////////////
// $Id: FeedbackDemoTask.cpp 4650 2013-11-22 16:31:14Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A demo feedback task.
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
#include "FeedbackDemoTask.h"
#include "Color.h"
#include "Localization.h"

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsRectItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsSimpleTextItem>

RegisterFilter( FeedbackDemoTask, 3 );

using namespace std;

FeedbackDemoTask::FeedbackDemoTask()
: mRunCount( 0 ),
  mTrialCount( 0 ),
  mCursorPosX( 0.0 ),
  mCursorPosY( 0.0 ),
  mCursorSpeedX( 0.0 ),
  mCursorSpeedY( 0.0 ),
  mpWindow( NULL ),
  mpScene( NULL ),
  mpSceneView( NULL ),
  mpLabel( NULL ),
  mpTarget( NULL ),
  mpCursor( NULL )
{
  BEGIN_PARAMETER_DEFINITIONS
    "Application:Window int WindowWidth= 640 640 0 % "
      " // width of application window",
    "Application:Window int WindowHeight= 480 480 0 % "
      " // height of application window",
    "Application:Window int WindowLeft= 0 0 % % "
      " // screen coordinate of application window's left edge",
    "Application:Window int WindowTop= 0 0 % % "
      " // screen coordinate of application window's top edge",

      "Application:Cursor int CursorColor= 0xFF0000 0xFF0000 % % "
      " // feedback cursor color (color)",
    "Application:Cursor float CursorWidth= 5 5 0.0 % "
      " // feedback cursor width in percent of screen width",
  END_PARAMETER_DEFINITIONS

  LANGUAGES "German",
  BEGIN_LOCALIZED_STRINGS
   "Timeout",          "Inaktiv",
   "Be prepared ...",  "Achtung ...",
  END_LOCALIZED_STRINGS

  mpWindow = new QWidget;
  mpWindow->setWindowFlags( Qt::FramelessWindowHint );
  mpWindow->setWindowTitle( "BCI2000 Feedback Demo" );

  mpScene = new QGraphicsScene;
  mpScene->setBackgroundBrush( QBrush( Qt::black ) );

  mpSceneView = new QGraphicsView( mpWindow );
  mpSceneView->setScene( mpScene );
  mpSceneView->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
  mpSceneView->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
  mpSceneView->show();

  mpLabel = new QGraphicsSimpleTextItem;
  mpScene->addItem( mpLabel );
  mpLabel->show();

  mpTarget = new QGraphicsRectItem;
  mpScene->addItem( mpTarget );
  mpTarget->setPen( Qt::NoPen );
  mpTarget->hide();

  mpCursor = new QGraphicsEllipseItem;
  mpScene->addItem( mpCursor );
  mpCursor->setPen( Qt::NoPen );
  mpCursor->hide();

  mpWindow->hide();
}

FeedbackDemoTask::~FeedbackDemoTask()
{
  if( mpWindow )
    delete mpWindow;
  mpWindow = NULL;

  if( mpScene )
    delete mpScene;
  mpScene = NULL;
}

void
FeedbackDemoTask::OnPreflight( const SignalProperties& Input ) const
{
  Parameter( "WindowHeight" );
  Parameter( "WindowWidth" );
  Parameter( "WindowLeft" );
  Parameter( "WindowTop" );

  if( RGBColor( Parameter( "CursorColor" ) ) == RGBColor( RGBColor::NullColor ) )
    bcierr << "Invalid RGB value in CursorColor" << endl;

  if( Parameter( "FeedbackDuration" ).InSampleBlocks() <= 0 )
    bcierr << "FeedbackDuration must be greater 0" << endl;

  if( Input.IsEmpty() )
    bcierr << "Requires at least one entry in control signal" << endl;

  if( Input.Channels() > 1 )
    bciout << "Will ignore additional channels in control signal" << endl;
}

void
FeedbackDemoTask::OnInitialize( const SignalProperties& /*Input*/ )
{
  mpWindow->move( Parameter( "WindowLeft" ), Parameter( "WindowTop" ) );
  mpWindow->resize( Parameter( "WindowWidth" ), Parameter( "WindowHeight" ) );
  mpSceneView->resize( Parameter( "WindowWidth" ), Parameter( "WindowHeight" ) );
  mpSceneView->setSceneRect( 0, 0, Parameter( "WindowWidth"), Parameter( "WindowHeight" ) );
  mpSceneView->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
  mpSceneView->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
  mpSceneView->show();

  RGBColor textColor( RGBColor::Green );
  SetLabel( LocalizableString( "Timeout" ), textColor );

  QRectF cursorRect;
  cursorRect.setWidth( mpWindow->width() * Parameter( "CursorWidth" ) / 100.0 );
  cursorRect.setHeight( cursorRect.width() );
  RGBColor cursorColor = RGBColor( Parameter( "CursorColor" ) );
  QBrush cursorBrush( QColor( cursorColor.R(), cursorColor.G(), cursorColor.B() ) );
  mpCursor->setRect( cursorRect );
  mpCursor->setBrush( cursorBrush );

  QRectF targetRect;
  targetRect.setLeft( mpWindow->width() - mpCursor->rect().width() );
  targetRect.setWidth( mpCursor->rect().width() );
  if( Parameter( "NumberTargets" ) > 0 )
    targetRect.setHeight( mpWindow->height() / Parameter( "NumberTargets" ) );
  mpTarget->setRect( targetRect );

  // Cursor speed in pixels per signal block duration:
  float feedbackDuration = Parameter( "FeedbackDuration" ).InSampleBlocks();
  mCursorSpeedX = ( mpWindow->width() - mpTarget->rect().width() ) / feedbackDuration;
  // In Y direction, we need only cross half the screen height during a trial.
  mCursorSpeedY = mpWindow->height() / feedbackDuration / 2;

  mpWindow->show();
}

void
FeedbackDemoTask::OnStartRun()
{
  ++mRunCount;
  mTrialCount = 0;
  mTrialStatistics.Reset();
  AppLog << "Run #" << mRunCount << " started" << endl;

  RGBColor textColor( RGBColor::White );
  SetLabel( LocalizableString( "Be prepared ..." ), textColor );
}

void
FeedbackDemoTask::OnStopRun()
{
  AppLog   << "Run " << mRunCount << " finished: "
           << mTrialStatistics.Total() << " trials, "
           << mTrialStatistics.Hits() << " hits, "
           << mTrialStatistics.Invalid() << " invalid.\n";
  int validTrials = mTrialStatistics.Total() - mTrialStatistics.Invalid();
  if( validTrials > 0 )
    AppLog << ( 200 * mTrialStatistics.Hits() + 1 ) / validTrials / 2  << "% correct, "
           << mTrialStatistics.Bits() << " bits transferred.\n";
  AppLog   << "====================="  << endl;

  RGBColor textColor( RGBColor::Green );
  SetLabel( LocalizableString( "Timeout" ), textColor );
}

void
FeedbackDemoTask::OnTrialBegin()
{
  ++mTrialCount;
  AppLog.Screen << "Trial #" << mTrialCount
                << ", target: " << State( "TargetCode" )
                << endl;

  RGBColor textColor( RGBColor::Black );
  SetLabel( "", textColor );
  if( State( "TargetCode" ) > 0 )
  {
    QRectF targetRect = mpTarget->rect();
    QRectF newTargetRect( targetRect.left(), targetRect.height() * ( State ( "TargetCode" ) - 1 ), targetRect.width(), targetRect.height() );
    mpTarget->setRect( newTargetRect );
    mpTarget->setBrush( mpCursor->brush() );
    mpTarget->show();
  }
}

void
FeedbackDemoTask::OnTrialEnd()
{
  mpLabel->setText( QString( "" ) );
  mpTarget->hide();
  mpCursor->hide();
}

void
FeedbackDemoTask::OnFeedbackBegin()
{
  mCursorPosX = 0.0;
  mCursorPosY = mpWindow->height() / 2;
  QRectF cursorRect = mpCursor->rect();
  QRectF newCursorRect( mCursorPosX, mCursorPosY - cursorRect.height() / 2, cursorRect.width(), cursorRect.height() );
  mpCursor->setRect( newCursorRect );
  mpCursor->show();
}

void
FeedbackDemoTask::OnFeedbackEnd()
{
  if( State( "ResultCode" ) == 0 )
  {
    AppLog.Screen << "-> aborted" << endl;
    mTrialStatistics.UpdateInvalid();
  }
  else
  {
    mTrialStatistics.Update( State( "TargetCode" ), State( "ResultCode" ) );
    if( State( "TargetCode" ) == State( "ResultCode" ) )
    {
      mpTarget->setBrush( QBrush( Qt::yellow ) );
      mpTarget->update();
      AppLog.Screen << "-> hit" << endl;
    }
    else
    {
      AppLog.Screen << "-> miss" << endl;
    }
  }
}

void
FeedbackDemoTask::DoPreRun( const GenericSignal&, bool& /*doProgress*/ )
{
}

void
FeedbackDemoTask::DoPreFeedback( const GenericSignal&, bool& /*doProgress*/ )
{
}

void
FeedbackDemoTask::DoFeedback( const GenericSignal& ControlSignal, bool& doProgress )
{
  mCursorPosX += mCursorSpeedX;
  mCursorPosY -= mCursorSpeedY * ControlSignal( 0, 0 );

  QRectF cursorRect = mpCursor->rect();
  QRectF newCursorRect;
  newCursorRect.setLeft( mCursorPosX );
  newCursorRect.setTop( mCursorPosY - cursorRect.height() / 2 );
  if( newCursorRect.top() < 0 )
    newCursorRect.setTop( 0 );
  else if( newCursorRect.top() > mpWindow->height() - cursorRect.height() )
    newCursorRect.setTop( mpWindow->height() - cursorRect.height() );
  newCursorRect.setWidth( cursorRect.width() );
  newCursorRect.setHeight( cursorRect.height() );
  mpCursor->setRect( newCursorRect );

  if( mCursorPosX + cursorRect.width() / 2 >= mpWindow->width() - mpTarget->rect().width() )
  { // Right margin hit:
    doProgress = true;
    int result = static_cast<int>( ::floor( mCursorPosY * Parameter( "NumberTargets" ) / mpWindow->height() ) + 1 );
    if( result < 1 )
      result = 1;
    else if( result > Parameter( "NumberTargets" ) )
      result = Parameter( "NumberTargets" );
    State( "ResultCode" ) = result;
  }
}

void
FeedbackDemoTask::DoPostFeedback( const GenericSignal&, bool& /*doProgress*/ )
{
}

void
FeedbackDemoTask::DoITI( const GenericSignal&, bool& /*doProgress*/ )
{
}

void
FeedbackDemoTask::SetLabel( const char *text, RGBColor &color )
{
  QFont labelFont;
  labelFont.fromString( "Arial" );
  labelFont.setPixelSize( mpWindow->height() / 8 );
  QFontMetrics fm( labelFont );
  mpLabel->setFont( labelFont );
  mpLabel->setPos( ( mpWindow->width() / 2 ) - ( fm.width( text ) / 2 ),
                   ( mpWindow->height() / 2 ) - ( fm.height() / 2 ) );
  mpLabel->setPen( QPen( QColor( color.R(), color.G(), color.B() ) ) );
  mpLabel->setBrush( QBrush( QColor( color.R(), color.G(), color.B() ) ) );
  mpLabel->setText( text );
}

