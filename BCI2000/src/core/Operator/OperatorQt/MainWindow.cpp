//////////////////////////////////////////////////////////////////////
// $Id: MainWindow.cpp 5649 2017-07-14 17:34:26Z mellinger $
// Authors: schalk@wadsworth.org, juergen.mellinger@uni-tuebingen.de
// Description: The Operator module's main window.
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
///////////////////////////////////////////////////////////////////////
#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "OperatorUtils.h"
#include "PrefDialog.h"
#include "SysLog.h"
#include "ConnectionInfo.h"
#include "Preferences.h"
#include "ConfigWindow.h"
#include "WatchesWindow.h"
#include "ShowStates.h"
#include "ExecutableHelp.h"
#include "AboutBox.h"
#include "GenericSignal.h"
#include "VisDisplay.h"
#include "BitmapImage.h"
#include "BCIException.h"
#include "ParserToken.h"

#include <QMessageBox>
#include <QShortcut>
#include <QCloseEvent>
#include <sstream>
#include <fstream>

using namespace std;

const char* cProgramName = "Operator Module";
#define TXT_WINDOW_CAPTION      "BCI2000/Operator"
#define TXT_OPERATOR_SUSPENDED  "Suspended"
#define TXT_OPERATOR_RUNNING    "Running"

Preferences* gpPreferences = NULL;

MainWindow::MainWindow()
: ui( new Ui::MainWindow ),
  mpSyslog( new SysLog( this ) ),
  mpConnectionInfo( nullptr ),
  mHide( false ),
  mStartupIdle( false ),
  mUpdateTimerID( 0 ),
  mState( 0 )
{
  ui->setupUi(this);
  this->setWindowFlags(
          Qt::Window
          | Qt::CustomizeWindowHint
          | Qt::WindowTitleHint
          | Qt::WindowSystemMenuHint
          | Qt::WindowCloseButtonHint
  );
  for( size_t i = 0; i < Preferences::numButtons; ++i )
  {
    QString idx;
    idx.setNum( i + 1 );
    mButtons[i] = findChild<QPushButton*>( "pushButton_Btn" + idx );
    new QShortcut( QKeySequence( "F" + idx ), mButtons[i], SLOT(click()), SLOT(click()), Qt::ApplicationShortcut );
  }
  new QShortcut( QKeySequence( tr("Ctrl+W") ), this, SLOT(CloseTopmostWindow()), NULL, Qt::ApplicationShortcut );
  // Avoid undesired side effects of key presses.
  QList<QPushButton*> pushButtons = findChildren<QPushButton*>();
  for( int i = 0; i < pushButtons.size(); ++i )
    pushButtons[i]->setFocusPolicy( Qt::NoFocus );

  ReadCommandLine();

  if( gpPreferences == NULL )
    gpPreferences = new Preferences;

  mpConnectionInfo = new ConnectionInfo( this );
  mpConnectionInfo->setVisible( false );

  WatchesWindow::Initialize( this );

  QFont font = mButtons[0]->font();
  int standardFontSize = QFontInfo( font ).pixelSize(),
      standardWeight = QFontInfo( font ).weight();
  font.setPixelSize( ( standardFontSize * 150 ) / 100 );
  font.setWeight( QFont::DemiBold );
  ui->pushButton_Config->setFont( font );
  ui->pushButton_SetConfig->setFont( font );
  ui->pushButton_RunSystem->setFont( font );
  ui->pushButton_Quit->setFont( font );
  font.setPixelSize( ( standardFontSize * 84 ) / 100 );
  font.setWeight( standardWeight );

  int nStatusLabels = sizeof( mpStatusLabels ) / sizeof( *mpStatusLabels );
  for( int i = 0; i < nStatusLabels; ++i )
  {
    QLabel* pLabel = new QLabel( ui->statusBar );
#ifndef _WIN32
    pLabel->setFrameStyle( QFrame::Panel | QFrame::Sunken );
    pLabel->setFont( font );
#endif
    pLabel->setText( "N/A" );
    ui->statusBar->addWidget( pLabel, 1 );
    mpStatusLabels[i] = pLabel;
  }

  OperatorUtils::RestoreWidget( this );
  SetupScripts();
  auto pVisMenu = GetWindowMenu()->addMenu("Visualizations");
  VisDisplay::Initialize(this, pVisMenu);

  if( mTelnet.length() )
    OperatorModule::TelnetListen( mTelnet.toLocal8Bit().constData() );

  if( !mStartupIdle )
  {
    if( mStartup.length() )
      OperatorModule::Startup( mStartup.toLocal8Bit().constData() );
    else
      OperatorModule::Startup( "* SignalSource:4000 SignalProcessing:4001 Application:4002" );
  }

  if( mHide )
    this->hide();
  else
    this->show();

  mUpdateTimerID = this->startTimer( 100 );
  UpdateDisplay();
  SetFunctionButtons();
}

MainWindow::~MainWindow()
{
  Terminate();
  VisDisplay::Deinitialize();
  WatchesWindow::Deinitialize();
  OperatorUtils::SaveWidget( this );
  delete gpPreferences;
  gpPreferences = 0;
  delete ui;
  ui = 0;
}

void
MainWindow::Terminate()
{
  if( !( mState & terminating ) )
  {
    mState |= terminating;
    this->killTimer( mUpdateTimerID );
    UpdateDisplay();
    mpConnectionInfo = nullptr;
    OperatorModule::Shutdown();
    int timeout = 2000, resolution = 100;
    bool done = false;
    while( !done && timeout > 0 )
    {
      switch( GetStateOfOperation() )
      {
        case bci::SystemState::Idle:
          done = true;
          break;
        case bci::SystemState::Unavailable:
          timeout = 0;
          break;
        default:
          ThreadUtils::SleepForMs( resolution );
          timeout -= resolution;
      }
    }
    // Execute the on-exit script ...
    if( done && !mExitScript.empty() )
    {
      mpSyslog->AddEntry( "Executing OnExit script ..." );
      EncodedString exitScript;
      istringstream iss( mExitScript );
      iss >> exitScript;
      ExecuteScript( exitScript );
    }
    OperatorModule::TelnetClose();
  }
}

QMenu*
MainWindow::GetWindowMenu()
{
  return menuBar()->findChild<QMenu*>( "menuWindow" );
}

bool
MainWindow::ConfirmQuit()
{
  int answer = QMessageBox::question(
       this,
       "Question", "Do you want to quit BCI2000?",
       QMessageBox::No | QMessageBox::Yes, QMessageBox::Yes );
  return answer == QMessageBox::Yes;
}

void
MainWindow::closeEvent( QCloseEvent* ev )
{
  if( mState & forceQuit )
  {
    Terminate();
    ev->accept();
    return;
  }
  if( !( mState & userQuit ) )
  {
    if( ConfirmQuit() )
      mState |= userQuit;
  }
  if( mState & userQuit )
  {
    Terminate();
    if( !mpSyslog->close() && mpSyslog->HasUndisplayedErrors() )
    {
      mpSyslog->hide();
      mpSyslog->setWindowModality( Qt::ApplicationModal );
      mpSyslog->AddEntry( "\nClose this window to quit", SysLog::logEntryWarning, true );
      mpSyslog->show();
      mpSyslog->exec();
    }
    ev->accept();
    return;
  }
  ev->ignore();
}

void
MainWindow::ReadCommandLine()
{
  mHide = false;
  mTitle = "";
  mTelnet = "";
  mStartupIdle = false;
  mStartup = "";

  int i = 1;
  while( i < qApp->arguments().size() )
  {
    if( qApp->arguments().at( i ) == "--Title" )
    {
      if( ( i + 1 ) < qApp->arguments().size() )
        mTitle = qApp->arguments().at( ++i );
      else
        mTitle = "";
    }
    else if( qApp->arguments().at( i ) == "--Telnet" )
    {
      if( ( i + 1 ) < qApp->arguments().size() && !qApp->arguments().at( i + 1 ).startsWith( "-" ) )
        mTelnet = qApp->arguments().at( ++i );
      else
        mTelnet = "localhost:3999";
    }
    else if( qApp->arguments().at( i ) == "--Hide" )
      mHide = true;
    else if( qApp->arguments().at( i ) == "--StartupIdle" )
      mStartupIdle = true;
    else if( qApp->arguments().at( i ) == "--Startup" )
    {
      if( ( i + 1 ) < qApp->arguments().size() )
        mStartup = qApp->arguments().at( ++i );
      mStartupIdle = false;
    }
    ++i;
  }
}

void
MainWindow::timerEvent( QTimerEvent* inEvent )
{
  if( inEvent->timerId() == mUpdateTimerID )
    UpdateDisplay();
}

void
MainWindow::UpdateDisplay()
{
  int t = mStarttime.secsTo( QDateTime::currentDateTime() );
  QTime timeElapsed( t / 3600, ( t / 60 ) % 60, t % 60 );
  QString windowCaption = TXT_WINDOW_CAPTION " ",
          statusText = "N/A";
  windowCaption += VersionInfo::Current[VersionInfo::VersionID].c_str();
  if( mTitle.length() > 0 )
  {
    windowCaption += " - ";
    windowCaption += mTitle;
  }

  int stateOfOperation = GetStateOfOperation();
  switch( stateOfOperation )
  {
    case bci::SystemState::Idle:
      statusText = "System Status: <Idle>";
      break;
    case bci::SystemState::Startup:
      statusText = "Waiting for connection";
      break;
    case bci::SystemState::Initialization:
      statusText = "Initialization Phase ...";
      break;
    case bci::SystemState::Resting:
    case bci::SystemState::Suspended:
    case bci::SystemState::ParamsModified:
      windowCaption += " - " TXT_OPERATOR_SUSPENDED " " + timeElapsed.toString( "mm:ss" ) + " s";
      statusText = TXT_OPERATOR_SUSPENDED;
      break;
    case bci::SystemState::Running:
      windowCaption += " - " TXT_OPERATOR_RUNNING " " + timeElapsed.toString( "mm:ss" ) + " s";
      statusText = TXT_OPERATOR_RUNNING;
      break;
    case bci::SystemState::Busy:
      statusText = "Waiting...";
      break;
    case bci::SystemState::Unavailable:
      statusText = "Fatal Error ...";
      break;
    default:
      statusText = "System Status: <" + QString::number( stateOfOperation ) + ">";
  }

  int nStatusLabels = sizeof( mpStatusLabels ) / sizeof( *mpStatusLabels );
  if( this->windowTitle() != windowCaption )
    this->setWindowTitle( windowCaption );
  if( mpStatusLabels[ 0 ]->text() != statusText )
    mpStatusLabels[ 0 ]->setText( statusText );

  QStringList status;
  const char* pStatus = nullptr;
  while( pStatus = bci::BCI_GetCoreModuleStatus( status.size() ) )
  {
    status.push_back( QString::fromLocal8Bit( pStatus ) );
    bci::BCI_ReleaseObject( pStatus );
  }
  if( status.size() > nStatusLabels - 1 )
  {
    status.clear();
    status.push_back(
      "<i>More than " + QString::number( nStatusLabels - 1 )
      + " core modules attached, use View&rarr;Connection Info for status info</i>"
    );
  }
  for( int i = 0; i < nStatusLabels - 1; ++i )
  {
    if( i < status.size() )
    {
      QString& s = status[i];
      const int maxLines = 2;
      int lines = 0, pos = 0;
      while( pos >= 0 && lines++ < maxLines )
        pos = s.indexOf( '\n', pos + 1 );
      if( pos >= 0 )
        s.truncate( pos );

      if( mpStatusLabels[i + 1]->text() != s )
        mpStatusLabels[i + 1]->setText( s );
      mpStatusLabels[i+1]->show();
    }
    else
      mpStatusLabels[i+1]->hide();
  }

  QString runSystemCaption = "Start";
  bool    configEnabled = false,
          setConfigEnabled = false,
          runSystemEnabled = false,
          quitEnabled = false;

  switch( GetStateOfOperation() )
  {
    case bci::SystemState::Startup:
      quitEnabled = true;
      break;
    case bci::SystemState::Initialization:
      configEnabled = true;
      setConfigEnabled = true;
      quitEnabled = true;
      break;
    case bci::SystemState::Suspended:
      runSystemCaption = "Resume";
      /* fall through */
    case bci::SystemState::Resting:
      runSystemEnabled = true;
      configEnabled = true;
      setConfigEnabled = true;
      quitEnabled = true;
      break;
    case bci::SystemState::ParamsModified:
      configEnabled = true;
      setConfigEnabled = true;
      quitEnabled = true;
      break;
    case bci::SystemState::Busy:
      quitEnabled = true;
      break;
    case bci::SystemState::Running:
      runSystemCaption = "Suspend";
      runSystemEnabled = true;
      break;
    case bci::SystemState::Unavailable:
      quitEnabled = true;
      break;
  }
  // To avoid unnecessary redraws of controls (flicker), we check for changed
  // captions before actually assigning them.
  if( ui->pushButton_RunSystem->text() != runSystemCaption )
      ui->pushButton_RunSystem->setText( runSystemCaption );
  if( ui->pushButton_Config->isEnabled() != configEnabled )
      ui->pushButton_Config->setEnabled( configEnabled );
  if( ui->pushButton_SetConfig->isEnabled() != setConfigEnabled )
      ui->pushButton_SetConfig->setEnabled( setConfigEnabled );
  if( ui->pushButton_RunSystem->isEnabled() != runSystemEnabled )
      ui->pushButton_RunSystem->setEnabled( runSystemEnabled );
  if( ui->pushButton_Quit->isEnabled() != quitEnabled )
      ui->pushButton_Quit->setEnabled( quitEnabled );

  ui->actionConnection_Info->setChecked( mpConnectionInfo && mpConnectionInfo->isVisible() );
  ui->actionOperator_Log->setChecked( mpSyslog->isVisible() );
}

void
MainWindow::SetFunctionButtons()
{
  for( int i = 0; i < Preferences::numButtons; ++i )
  {
    QString name = gpPreferences->mButtons[i].Name,
            script = gpPreferences->mButtons[i].Cmd;
    SetFunctionButton( i, name, script );
  }
}

void
MainWindow::SetFunctionButton( int inIdx, const QString& inTitle, const QString& inScript )
{
  QPushButton* pButton = mButtons[inIdx];
  pButton->setEnabled( inTitle != "" && inScript != "" );
  if( inTitle != "" )
  {
    pButton->setText( inTitle );
  }
  else
  {
    QString num;
    num.setNum( inIdx + 1 );
    pButton->setText( "Function " + num );
  }
  mButtonScripts[inIdx] = inScript.toLocal8Bit().constData();
}

void
MainWindow::SetupScripts()
{
  if( !gpPreferences )
    throw std_logic_error << "Global preferences object does not exist";

  static const struct { int id; const char* name; }
  events[] =
  {
    #define EVENT(x) { Preferences::x, #x },
    EVENT( OnConnect )
    EVENT( OnSetConfig )
    EVENT( OnResume )
    EVENT( OnSuspend )
    EVENT( OnStart )
    #undef EVENT
  };
  QString* scripts = gpPreferences->mScript;
  for( size_t i = 0; i < sizeof( events ) / sizeof( *events ); ++i )
  {
    string command = "SET SCRIPT " + string( events[i].name ) + " " + FormatScript( events[i].name, scripts[events[i].id] );
    ExecuteScript( command );
  }
  mExitScript = FormatScript( "OnExit", scripts[Preferences::OnExit] );
}

string
MainWindow::FormatScript( const char* inEventName, const QString& inScript )
{
  string result = inScript.toLocal8Bit().constData();
  if( !result.empty() )
  {
    if( result[0] == '-' )
    { // Immediate script specified
      result = result.substr( 1 );
    }
    else
    { // Script file specified
      ifstream file( result.c_str() );
      if( !file.is_open() )
      {
        OnErrorMessage( ( "Could not open " + string( inEventName ) + " script file \"" + result + "\"" ).c_str() );
        result = "";
      }
      else
        result = "EXECUTE SCRIPT " + result;
    }
  }
  ostringstream oss;
  EncodedString( result ).InsertInto( oss, "\";`" );
  result = oss.str();
  return result;
}

void
MainWindow::GetParameters()
{
  mParameters.Clear();
  int i = 0;
  std::string p;
  do
  {
    p = GetParameter( i++ );
    if( !p.empty() )
    {
      Param param = Param::fromDefinition( p );
      mParameters[param.Name()] = param;
    }
  } while( !p.empty() );
}

void
MainWindow::PutParameters()
{
  for( int i = 0; i < mParameters.Size(); ++i )
  {
    ostringstream oss;
    oss << mParameters[i];
    PutParameter( oss.str() );
  }
}

void
MainWindow::CloseTopmostWindow()
{
  QWidget* p = qApp->activeWindow();
  if( p )
    p->close();
}

void
MainWindow::ForceQuit()
{
  mState |= forceQuit;
  close();
}

void
MainWindow::SetStartTime()
{
  mStarttime = QDateTime::currentDateTime();
}

////////////////////////////////////////////////////////////////////////////////
//----------    Callback functions called by the OperatorLib    --------------
////////////////////////////////////////////////////////////////////////////////
void
MainWindow::OnShutdown()
{
  VisDisplay::ClearVisuals();
}

const char*
MainWindow::OnQuitRequest()
{
  QMetaObject::invokeMethod( this, "ForceQuit", Qt::AutoConnection );
  return nullptr;
}

void
MainWindow::OnCoreInput()
{
  if( mpConnectionInfo )
    mpConnectionInfo->UpdateDisplay();
}

void
MainWindow::OnLogMessage( const char* s )
{
  QMetaObject::invokeMethod(
    mpSyslog,
    "AddEntry",
    Qt::AutoConnection,
    Q_ARG(QString, s ),
    Q_ARG(int, SysLog::logEntryNormal));
}

void
MainWindow::OnWarningMessage( const char* s )
{
  QMetaObject::invokeMethod(
    mpSyslog,
    "AddEntry",
    Qt::AutoConnection,
    Q_ARG(QString, s ),
    Q_ARG(int, SysLog::logEntryWarning));
}

void
MainWindow::OnErrorMessage( const char* s )
{
  QMetaObject::invokeMethod(
    mpSyslog,
    "AddEntry",
    Qt::AutoConnection,
    Q_ARG(QString, s ),
    Q_ARG(int, SysLog::logEntryError));
}

void
MainWindow::OnScriptError( const char* s )
{
  std::string message = "Script error: ";
  message += s;
  OnErrorMessage( message.c_str() );
}

void
MainWindow::OnParameter( const char* s )
{
  Param param = Param::fromDefinition( s );
  // Update the parameter in the configuration window.
  if( gpConfig != NULL )
    gpConfig->RenderParameter( &param );
}

bool
MainWindow::OnUnknownCommand( const char* inCommand )
{
  bool result = false;
  istringstream iss( inCommand );
  ParserToken verb, type, name;
  iss >> verb >> type >> name;
  enum { unknown, show, hide, set } action = unknown;
  if( !::stricmp( verb.c_str(), "Show" ) )
    action = show;
  else if( !::stricmp( verb.c_str(), "Hide" ) )
    action = hide;
  else if( !::stricmp( verb.c_str(), "Close" ) )
    action = hide;
  else if( !::stricmp( verb.c_str(), "Set" ) )
    action = set;
  if( action != unknown && !::stricmp( type.c_str(), "Window" ) )
  {
    result = true;
    enum { unknown, main, log, config } object = unknown;
    if( name.empty() || !::stricmp( name.c_str(), "Main" ) )
      object = main;
    else if( !::stricmp( name.c_str(), "Log" ) )
      object = log;
    else if( !::stricmp( name.c_str(), "Configuration" ) || !::stricmp( name.c_str(), "Config" ) )
      object = config;
    else
      result = false;

    switch( object )
    {
      case main:
        QMetaObject::invokeMethod(
          this,
          "setVisible",
          Qt::AutoConnection,
          Q_ARG(bool, action == show ));
        break;
      case log:
        QMetaObject::invokeMethod(
          mpSyslog,
          "setVisible",
          Qt::AutoConnection,
          Q_ARG(bool, action == show ));
        break;
      case config:
      {
        switch( action )
        {
          case show:
            QMetaObject::invokeMethod(
              this,
              "on_pushButton_Config_clicked",
              Qt::AutoConnection);
            break;
          case hide:
            QMetaObject::invokeMethod(
              gpConfig,
              "close",
              Qt::AutoConnection);
            break;
            break;
          default:
            result = false;
         }
      }
    }
  }
  else if( action == set && !::stricmp( type.c_str(), "Title" ) )
  {
    result = true;
    mTitle = QString::fromLocal8Bit( name.c_str() );
  }
  else if( action == set && !::stricmp( type.c_str(), "Button" ) )
  {
    int idx = ::atoi( name.c_str() ) - 1;
    if( idx >= 0 && idx < Preferences::numButtons )
    {
      result = true;
      ParserToken caption, script;
      iss >> caption >> script;
      QMetaObject::invokeMethod(
        this,
        "SetFunctionButton",
        Qt::AutoConnection,
        Q_ARG(int, idx), // cannot use typedefs here (eg, size_t)
        Q_ARG(QString, QString::fromLocal8Bit( caption.c_str() )),
        Q_ARG(QString, QString::fromLocal8Bit( script.c_str() ))
      );
    }
  }
  return result;
}

const char*
MainWindow::OnScriptHelp()
{
  return "Show Window <name>, Hide Window <name>, Set Title <window title>, "
         "Set Button <idx> <title> <script>, "
         "names are: Main, Log, Configuration";
}

void
MainWindow::OnVisSignal( const char* visID, int ch, int el, float* data )
{
  VisDisplay::HandleSignal( visID, ch, std::vector<float>( data, data + ch * el ) );
}

void
MainWindow::OnVisMemo( const char* visID, const char* s )
{
  VisDisplay::HandleMemo( visID, s );
}

void
MainWindow::OnVisBitmap( const char* visID, int width, int height, unsigned short* data )
{
  VisDisplay::HandleBitmap( visID, BitmapImage( width, height, data ) );
}

void
MainWindow::OnVisPropertyMessage( const char* visID, int cfgID, const char* value )
{
  VisDisplay::HandlePropertyMessage( visID, cfgID, value );
}

void
MainWindow::OnVisProperty( const char* visID, int cfgID, const char* value )
{
  VisDisplay::HandleProperty( visID, cfgID, value );
}

void
MainWindow::OnInitializeVis( const char* inVisID, const char* inKind )
{
  VisDisplay::Create( inVisID, inKind );
}

////////////////////////////////////////////////////////////////////////////////
//----------------------   IDE-managed Qt slots    ---------------------------
////////////////////////////////////////////////////////////////////////////////

void
MainWindow::on_pushButton_Quit_clicked()
{
  close();
}

void
MainWindow::on_actionQuit_triggered()
{
  close();
}

void
MainWindow::on_pushButton_RunSystem_clicked()
{
  if( GetStateOfOperation() == bci::SystemState::Running )
    StopRun();
  else
  {
    if( gpConfig && gpConfig->isVisible() )
      gpConfig->close();
    StartRun();
  }
}

void
MainWindow::on_pushButton_SetConfig_clicked()
{
  ui->pushButton_SetConfig->setEnabled( false );
  if( gpConfig && gpConfig->isVisible() )
    gpConfig->close();
  SetConfig();
}

void
MainWindow::on_pushButton_Config_clicked()
{
  if( gpConfig == NULL )
  {
    gpConfig = new ConfigWindow( this );
    connect( gpConfig, SIGNAL(finished(int)), this, SLOT(PutParameters()) );
  }
  GetParameters();
  gpConfig->Initialize( &mParameters, gpPreferences );
  gpConfig->show();
}

void
MainWindow::on_actionPreferences_triggered()
{
  if( PrefDialog( this ).exec() == QDialog::Accepted )
  {
    SetupScripts();
    SetFunctionButtons();
  }
}

void
MainWindow::on_actionConnection_Info_toggled( bool inOnOff )
{
  if( mpConnectionInfo )
    mpConnectionInfo->setVisible( inOnOff );
}

void
MainWindow::on_actionOperator_Log_toggled( bool inOnOff )
{
  mpSyslog->setVisible( inOnOff );
}

void
MainWindow::on_actionBCI2000_Help_triggered()
{
  ExecutableHelp()
  .Display();
}

void
MainWindow::on_actionAbout_triggered()
{
  AboutBox()
  .SetApplicationName( cProgramName )
  .Display();
}

void
MainWindow::on_actionStates_triggered()
{
  ShowStates s( this );
  s.exec();
}

void
MainWindow::on_pushButton_Btn1_clicked()
{
  ExecuteScript( mButtonScripts[0] );
}

void
MainWindow::on_pushButton_Btn2_clicked()
{
  ExecuteScript( mButtonScripts[1] );
}

void
MainWindow::on_pushButton_Btn3_clicked()
{
  ExecuteScript( mButtonScripts[2] );
}

void
MainWindow::on_pushButton_Btn4_clicked()
{
  ExecuteScript( mButtonScripts[3] );
}

