////////////////////////////////////////////////////////////////////////////////
// $Id: MainWindow.cpp 5143 2015-11-25 15:29:15Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: The main window of the BCI2000Launcher tool.
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
#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "AboutBox.h"
#include "ExecutableHelp.h"
#include "EncodedString.h"
#include "SysError.h"
#include "Settings.h"
#include "defines.h"

#if _WIN32
# include <windows.h>
#endif

#include <QtWidgets>
#include <string>
#include <sstream>
#include <signal.h>

using namespace std;
using namespace bci;

// The following arrays contain current or former modules from 
// the BCI2000 core/contrib directories as of r3545 (Sept 2011).
// To catch modules located in private/custom, and future core/contrib modules,
// we rely on the mechanism available via src/shared/config/ModuleInventory.
static const char* sKnownSourceModules[] =
{
  "SignalGenerator",
  "gUSBampSource",
  "gMOBIlab",
  "gMOBIlabPlus",
  "TDTclient",
  "ModularEEG",
  "DT2000",
  "RDAClient",
  "DASSource",
  "NISource",
  "AmpServerPro",
  "BioRadio",
  "Biosemi2",
  "Emotiv",
  "ENOBIO",
  "FieldTripBufferSource",
  "FilePlayback",
  "Micromed",
  "NeuroscanClient",
  "NeuroscanAccess",
  "NeuroSky",
  "NIDAQ_mx_Source",
  "PythonSource",
  "TMSi",
  "TuckerDavis",
  "vAmpSource",
  "Grapevine",
};

static const char* sKnownSigprocModules[] =
{
  "DummySignalProcessing",
  "ARSignalProcessing",
  "P3SignalProcessing",
  "FIRProcessing",
  "PeakDetectSignalProcessing",
  "SWSignalProcessing",
  "CoherenceFFTSignalProcessing",
  "CoherenceSignalProcessing",
  "FFTSignalProcessing",
  "FieldTripBuffer",
  "MatlabSignalProcessing",
  "PythonSignalProcessing",
  "SigfriedSigProc",
};

static const char* sKnownApplicationModules[] =
{
  "Screen",
  "D3Box",
  "D2Box",
  "P3AVTask",
  "P3Speller",
  "RJB",
  "Oddball",
  "Speller",
  "SWApp",
  "SWSpeller",
  "StimulusPresentation",
  "FeedbackDemo",
  "CursorTask",
  "DummyApplication",
  "PythonApplication",
};

MainWindow::MainWindow(QWidget *parent)
: QMainWindow(parent), ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  this->setWindowTitle( QApplication::applicationName() );
  SetupProgramsGroup();
  SetupParametersGroup();
  SetupStatusGroup();
  SetupActions();

  ReadSettings();

  SetupInventory();
  QStringList programs = ScanForPrograms();
  DistributePrograms( programs );
}

MainWindow::~MainWindow()
{
  SaveSettings();
  delete ui;
}

void
MainWindow::SetupProgramsGroup()
{
  // Setup layouts in the programs group
  QHBoxLayout* pHBoxProg = new QHBoxLayout;
  QVBoxLayout* pVBoxProg = new QVBoxLayout;

  QVBoxLayout* pVBoxSrc = new QVBoxLayout;
  pVBoxSrc->addWidget( new QLabel( tr("Signal Source") ) );
  pVBoxSrc->addWidget( ui->sourceList );
  pHBoxProg->addLayout( pVBoxSrc );

  QVBoxLayout* pVBoxSigproc = new QVBoxLayout;
  pVBoxSigproc->addWidget( new QLabel( tr("Signal Processing") ) );
  pVBoxSigproc->addWidget( ui->sigprocList );
  pHBoxProg->addLayout( pVBoxSigproc );

  QVBoxLayout* pVBoxApp = new QVBoxLayout;
  pVBoxApp->addWidget( new QLabel( tr("Application") ) );
  pVBoxApp->addWidget( ui->applicationList );
  pHBoxProg->addLayout( pVBoxApp );

  pHBoxProg->addWidget( ui->otherGroup );
  QHBoxLayout* pHBoxOther = new QHBoxLayout;
  pHBoxOther->addWidget( ui->otherList );
  ui->otherGroup->setLayout( pHBoxOther );

  pVBoxProg->addLayout( pHBoxProg );
  pVBoxProg->addWidget( ui->clearSelectionBtn );

  ui->programsGroup->setLayout( pVBoxProg );
}

void
MainWindow::SetupParametersGroup()
{
  // Setup layouts in the parameters group
  QVBoxLayout* pVBoxParm = new QVBoxLayout,
             * pVBox1 = new QVBoxLayout,
             * pVBox2 = new QVBoxLayout;
  QHBoxLayout* pHBox1 = new QHBoxLayout,
             * pHBox2 = new QHBoxLayout,
             * pHBox3 = new QHBoxLayout;

  pVBox2->addWidget( ui->addParmfileBtn );
  pVBox2->addWidget( ui->removeParmfileBtn );
  pVBox2->addStretch();

  pHBox1->addWidget( ui->paramfilesList );
  pHBox1->addLayout( pVBox2 );

  pVBox1->addWidget( new QLabel( tr("Parameter Files") ) );
  pVBox1->addLayout( pHBox1 );

  pHBox2->addWidget( new QLabel( tr("Save Directory") ) );
  pHBox2->addWidget( ui->saveDirEdit );
  pHBox2->addWidget( ui->chooseDirBtn );

  pHBox3->addWidget( new QLabel( tr("Subject Name") ) );
  pHBox3->addWidget( ui->subjectNameEdit );
  pHBox3->addWidget( new QLabel( tr("Session #") ) );
  pHBox3->addWidget( ui->subjectSessionEdit );

  pVBoxParm->addLayout( pVBox1 );
  pVBoxParm->addLayout( pHBox2 );
  pVBoxParm->addLayout( pHBox3 );
  ui->parametersGroup->setLayout( pVBoxParm );
}

void
MainWindow::SetupStatusGroup()
{
  // Setup layouts in the status group
  QVBoxLayout* pVBoxStatus = new QVBoxLayout;
  pVBoxStatus->addWidget( ui->statusEdit );
  pVBoxStatus->addWidget( ui->launchButton );
  ui->statusGroup->setLayout( pVBoxStatus );
}

void
MainWindow::SetupActions()
{
  // Setup context menus in the programs group
  ui->sourceList->addAction( ui->actionTo_Others );
  ui->sourceList->addAction( ui->actionTo_Signal_Source );
  ui->sourceList->addAction( ui->actionTo_Signal_Processing );
  ui->sourceList->addAction( ui->actionTo_Applications );
  ui->sourceList->addAction( ui->actionUp );
  ui->sourceList->addAction( ui->actionDown );

  ui->sigprocList->addAction( ui->actionTo_Others );
  ui->sigprocList->addAction( ui->actionTo_Signal_Source );
  ui->sigprocList->addAction( ui->actionTo_Signal_Processing );
  ui->sigprocList->addAction( ui->actionTo_Applications );
  ui->sigprocList->addAction( ui->actionUp );
  ui->sigprocList->addAction( ui->actionDown );

  ui->applicationList->addAction( ui->actionTo_Others );
  ui->applicationList->addAction( ui->actionTo_Signal_Source );
  ui->applicationList->addAction( ui->actionTo_Signal_Processing );
  ui->applicationList->addAction( ui->actionTo_Applications );
  ui->applicationList->addAction( ui->actionUp );
  ui->applicationList->addAction( ui->actionDown );

  ui->otherList->addAction( ui->actionTo_Others );
  ui->otherList->addAction( ui->actionTo_Signal_Source );
  ui->otherList->addAction( ui->actionTo_Signal_Processing );
  ui->otherList->addAction( ui->actionTo_Applications );
  ui->otherList->addAction( ui->actionUp );
  ui->otherList->addAction( ui->actionDown );
}

void
MainWindow::SetupInventory()
{
#define RANGE( x ) (x), (x) + sizeof(x)/sizeof(*x)
  mInventory[Inventory::SignalSource].insert( RANGE( sKnownSourceModules ) );
  mInventory[Inventory::SignalProcessing].insert( RANGE( sKnownSigprocModules ) );
  mInventory[Inventory::Application].insert( RANGE( sKnownApplicationModules ) );
}

QStringList
MainWindow::ScanForPrograms()
{
  QDir dir( QApplication::applicationDirPath() );
  dir.setFilter( QDir::Files | QDir::Executable );
  dir.setSorting( QDir::Name );
  QStringList entries = dir.entryList(),
              programs;
  for( int i = 0; i < entries.size(); ++i )
    programs.append( QFileInfo( entries.at( i ) ).completeBaseName() );
  // Remove the program itself.
  programs.removeOne( QFileInfo( QApplication::applicationFilePath() ).completeBaseName() );
  // Remove all operator modules.
  for( Inventory::mapped_type::const_iterator i = mInventory[Inventory::Operator].begin();
         i != mInventory[Inventory::Operator].end(); ++i )
    programs.removeOne( QString::fromLocal8Bit( i->c_str() ) );
  // Remove all tools.
  for( Inventory::mapped_type::const_iterator i = mInventory[Inventory::Tool].begin();
         i != mInventory[Inventory::Tool].end(); ++i )
    programs.removeOne( QString::fromLocal8Bit( i->c_str() ) );
  return programs;
}

void
MainWindow::DistributePrograms( const QStringList& inList )
{
  // Build a list of all programs known by the application.
  QStringList knownPrograms;
  for( int i = 0; i < ui->sourceList->count(); ++i )
    knownPrograms.append( ui->sourceList->item( i )->text() );
  for( int i = 0; i < ui->sigprocList->count(); ++i )
    knownPrograms.append( ui->sigprocList->item( i )->text() );
  for( int i = 0; i < ui->applicationList->count(); ++i )
    knownPrograms.append( ui->applicationList->item( i )->text() );
  for( int i = 0; i < ui->otherList->count(); ++i )
    knownPrograms.append( ui->otherList->item( i )->text() );
  QStringList unknownPrograms;
  foreach( QString program, inList )
  {
    if( knownPrograms.contains( program, Qt::CaseSensitive ) )
      knownPrograms.removeOne( program );
    else
      unknownPrograms.append( program );
  }
  foreach( QString program, knownPrograms )
    ClearProgramEntries( program );
  foreach( QString program, unknownPrograms )
    IntroduceNewProgram( program );
}

void
MainWindow::ClearProgramEntries( const QString& inProgram )
{ // Remove a program's entries from all program lists.
  QList<QListWidgetItem*> items = ui->sourceList->findItems( inProgram, Qt::MatchExactly );
  items.append( ui->sigprocList->findItems( inProgram, Qt::MatchExactly ) );
  items.append( ui->applicationList->findItems( inProgram, Qt::MatchExactly ) );
  items.append( ui->otherList->findItems( inProgram, Qt::MatchExactly ) );
  foreach( QListWidgetItem* item, items )
    delete item;
}

void
MainWindow::IntroduceNewProgram( const QString& inProgram )
{ // Introduce a new program into the programs group.
  // Check against the lists of known modules first.
  // If not found there, add to the "others" list.
  const Inventory::mapped_type& sources = mInventory[Inventory::SignalSource];
  if( sources.find( inProgram.toLocal8Bit().constData() ) != sources.end() )
  {
    ui->sourceList->addItem( inProgram );
    return;
  }

  const Inventory::mapped_type& sigprocs = mInventory[Inventory::SignalProcessing];
  if( sigprocs.find( inProgram.toLocal8Bit().constData() ) != sigprocs.end() )
  {
    ui->sigprocList->addItem( inProgram );
    return;
  }

  const Inventory::mapped_type& apps = mInventory[Inventory::Application];
  if( apps.find( inProgram.toLocal8Bit().constData() ) != apps.end() )
  {
    ui->applicationList->addItem( inProgram );
    return;
  }

  ui->otherList->addItem( inProgram );
}

void MainWindow::on_actionQuit_triggered()
{
  qApp->quit();
}

void MainWindow::on_actionAbout_triggered()
{
  AboutBox().SetApplicationName( QApplication::applicationName().toLocal8Bit().constData() )
            .Display();
}

void MainWindow::on_actionBCI2000_Help_triggered()
{
  ExecutableHelp().Display();
}

void MainWindow::MoveListItem( QListWidget* pTarget )
{
  QListWidget* pListWidget = qobject_cast<QListWidget*>( this->focusWidget() );
  if( pListWidget != NULL && pListWidget != pTarget )
  {
    foreach( QListWidgetItem* pItem, pListWidget->selectedItems() )
    {
      QString text = pItem->text();
      delete pItem;
      pTarget->addItem( text );
    }
  }
}

void MainWindow::on_actionTo_Others_triggered()
{
  MoveListItem( ui->otherList );
}

void MainWindow::on_actionTo_Applications_triggered()
{
  MoveListItem( ui->applicationList );
}

void MainWindow::on_actionTo_Signal_Processing_triggered()
{
  MoveListItem( ui->sigprocList );
}

void MainWindow::on_actionTo_Signal_Source_triggered()
{
  MoveListItem( ui->sourceList );
}

void
MainWindow::SaveSettings() const
{
  Settings settings;
  settings.beginGroup( KEY_CONFIG );
  settings.setValue( "Geometry", this->saveGeometry() );
  settings.setValue( "WorkingDirectory", QDir::currentPath() );

  settings.beginWriteArray( "Source Modules" );
  for( int i = 0; i < ui->sourceList->count(); ++i )
  {
    settings.setArrayIndex( i );
    settings.setValue( "Name", ui->sourceList->item( i )->text() );
    settings.setValue( "Selected", ui->sourceList->item( i )->isSelected() );
  }
  settings.endArray();

  settings.beginWriteArray( "Signal Processing Modules" );
  for( int i = 0; i < ui->sigprocList->count(); ++i )
  {
    settings.setArrayIndex( i );
    settings.setValue( "Name", ui->sigprocList->item( i )->text() );
    settings.setValue( "Selected", ui->sigprocList->item( i )->isSelected() );
  }
  settings.endArray();

  settings.beginWriteArray( "Application Modules" );
  for( int i = 0; i < ui->applicationList->count(); ++i )
  {
    settings.setArrayIndex( i );
    settings.setValue( "Name", ui->applicationList->item( i )->text() );
    settings.setValue( "Selected", ui->applicationList->item( i )->isSelected() );
  }
  settings.endArray();

  settings.beginWriteArray( "Other Modules" );
  for( int i = 0; i < ui->otherList->count(); ++i )
  {
    settings.setArrayIndex( i );
    settings.setValue( "Name", ui->otherList->item( i )->text() );
    settings.setValue( "Selected", ui->otherList->item( i )->isSelected() );
  }
  settings.endArray();

  settings.beginWriteArray( "Parameter Files" );
  for( int i = 0; i < mParameterFiles.count(); ++i )
  {
    settings.setArrayIndex( i );
    settings.setValue( "Name", mParameterFiles.at( i ) );
  }
  settings.endArray();

  settings.setValue( "Save Directory", ui->saveDirEdit->text() );
  settings.setValue( "Subject Name", ui->subjectNameEdit->text() );
  settings.setValue( "Subject Session", ui->subjectSessionEdit->text() );

  settings.endGroup();
}

void
MainWindow::ReadSettings()
{
  Settings settings;
  settings.beginGroup( KEY_CONFIG );
  this->restoreGeometry( settings.value( "Geometry" ).toByteArray() );
  QDir::setCurrent( settings.value( "WorkingDirectory" ).toString() );

  ui->sourceList->clear();
  int size = settings.beginReadArray( "Source Modules" );
  for( int i = 0; i < size; ++i )
  {
    settings.setArrayIndex( i );
    ui->sourceList->addItem( settings.value( "Name" ).toString() );
    ui->sourceList->item( i )->setSelected( settings.value( "Selected" ).toBool() );
  }
  settings.endArray();

  ui->sigprocList->clear();
  size = settings.beginReadArray( "Signal Processing Modules" );
  for( int i = 0; i < size; ++i )
  {
    settings.setArrayIndex( i );
    ui->sigprocList->addItem( settings.value( "Name" ).toString() );
    ui->sigprocList->item( i )->setSelected( settings.value( "Selected" ).toBool() );
  }
  settings.endArray();

  ui->applicationList->clear();
  size = settings.beginReadArray( "Application Modules" );
  for( int i = 0; i < size; ++i )
  {
    settings.setArrayIndex( i );
    ui->applicationList->addItem( settings.value( "Name" ).toString() );
    ui->applicationList->item( i )->setSelected( settings.value( "Selected" ).toBool() );
  }
  settings.endArray();

  ui->otherList->clear();
  size = settings.beginReadArray( "Other Modules" );
  for( int i = 0; i < size; ++i )
  {
    settings.setArrayIndex( i );
    ui->otherList->addItem( settings.value( "Name" ).toString() );
    ui->otherList->item( i )->setSelected( settings.value( "Selected" ).toBool() );
  }
  settings.endArray();

  mParameterFiles.clear();
  ui->paramfilesList->clear();
  size = settings.beginReadArray( "Parameter Files" );
  for( int i = 0; i < size; ++i )
  {
    settings.setArrayIndex( i );
    mParameterFiles.append( settings.value( "Name" ).toString() );
    ui->paramfilesList->addItem( QFileInfo( mParameterFiles.last() ).completeBaseName() );
  }
  settings.endArray();

  ui->saveDirEdit->setText( settings.value( "Save Directory" ).toString() );
  ui->subjectNameEdit->setText( settings.value( "Subject Name" ).toString() );
  ui->subjectSessionEdit->setText( settings.value( "Subject Session" ).toString() );

  settings.endGroup();
}

void MainWindow::on_clearSelectionBtn_clicked()
{
  ui->sourceList->clearSelection();
  ui->sigprocList->clearSelection();
  ui->applicationList->clearSelection();
  ui->otherList->clearSelection();
}

void MainWindow::on_actionUp_triggered()
{
  QListWidget* pListWidget = qobject_cast<QListWidget*>( this->focusWidget() );
  if( pListWidget != NULL )
  {
    if( pListWidget->currentRow() > 0 )
    {
      int targetRow = pListWidget->currentRow() - 1;
      QListWidgetItem* pItem = pListWidget->takeItem( pListWidget->currentRow() );
      pListWidget->insertItem( targetRow, pItem );
      pListWidget->setCurrentRow( targetRow );
    }
  }
}

void MainWindow::on_actionDown_triggered()
{
  QListWidget* pListWidget = qobject_cast<QListWidget*>( this->focusWidget() );
  if( pListWidget != NULL )
  {
    if( pListWidget->currentRow() >= 0 && pListWidget->currentRow() < pListWidget->count() - 1 )
    {
      int targetRow = pListWidget->currentRow() + 1;
      QListWidgetItem* pItem = pListWidget->takeItem( pListWidget->currentRow() );
      pListWidget->insertItem( targetRow, pItem );
      pListWidget->setCurrentRow( targetRow );
    }
  }
}


void MainWindow::on_addParmfileBtn_clicked()
{
  QStringList fileNames =
      QFileDialog::getOpenFileNames(
          this,
          tr("Select parameter file(s) to apply"),
          QDir::currentPath(),
          tr("BCI2000 Parameter Files (*.prm)")
      );
  if( !fileNames.empty() )
  {
    QDir::setCurrent( QFileInfo( fileNames.first() ).absolutePath() );
    foreach( QString file, fileNames )
    {
      ui->paramfilesList->addItem( QFileInfo( file ).completeBaseName() );
      mParameterFiles.append( file );
    }
  }
}

void MainWindow::on_removeParmfileBtn_clicked()
{
  int idx = ui->paramfilesList->currentRow();
  if( idx >= 0 )
  {
    QStringList::iterator i = mParameterFiles.begin() + idx;
    mParameterFiles.erase( i );
    delete ui->paramfilesList->takeItem( idx );
  }
}

void MainWindow::on_chooseDirBtn_clicked()
{
  QString dir =
      QFileDialog::getExistingDirectory(
          this,
          tr("Select top level directory to save sessions into"),
          QDir::currentPath()
      );
  if( !dir.isEmpty() )
  {
    QDir::setCurrent( dir );
    ui->saveDirEdit->setText( dir );
    ui->saveDirEdit->setToolTip( dir );
  }
}

void MainWindow::on_subjectSessionEdit_editingFinished()
{
  QLineEdit* pSender = qobject_cast<QLineEdit*>( sender() );
  if( pSender != NULL )
  {
    QString text = pSender->text();
    while( text.length() < 3 )
      text = QString( "0" ) + text;
    pSender->setText( text );
  }
}

void MainWindow::SyncToolTip()
{
  QLineEdit* pSender = qobject_cast<QLineEdit*>( sender() );
  if( pSender != NULL )
    pSender->setToolTip( pSender->text() );
}

void MainWindow::on_subjectNameEdit_textChanged(QString )
{
  SyncToolTip();
}

void MainWindow::on_saveDirEdit_textChanged(QString )
{
  SyncToolTip();
}

void MainWindow::on_subjectSessionEdit_textChanged(QString )
{
  SyncToolTip();
}

void MainWindow::on_launchButton_clicked()
{
  string progdir = string( QApplication::applicationDirPath().toLocal8Bit().constData() ) + "/";

  ui->statusEdit->clear();
  ui->statusEdit->append( tr( "Launching %1..." ).arg( "Operator" ) );
  ui->statusEdit->repaint();

  ostringstream command;
  command << "\"" << progdir << "Operator\" --Startup localhost";

  if( mParameterFiles.size() > 0 )
  {
    command << " --OnConnect \"-";
    for( int i = 0; i < mParameterFiles.size(); ++i )
      command << "LOAD PARAMETERFILE "
              << EncodedString( mParameterFiles.at( i ).toLocal8Bit().constData() )
              << "; ";
    command << " SETCONFIG;\"";
  }

  if( !ExecuteCommand( command.str().c_str(), progdir.c_str() ) )
    return;

  command.str( "" );
  if( ui->sourceList->selectedItems().empty() )
  {
    ui->statusEdit->append( tr("No Source Module selected...") );
    ui->statusEdit->repaint();
  }
  else
  {
    QListWidgetItem* item = ui->sourceList->selectedItems().first();
    ui->statusEdit->append( tr( "Launching %1..." ).arg( item->text() ) );
    ui->statusEdit->repaint();
    command << "\"" << progdir << item->text().toLocal8Bit().constData() << "\" --local";
    if( !ui->saveDirEdit->text().isEmpty() )
      command << " --DataDirectory-" << EncodedString( ui->saveDirEdit->text().toLocal8Bit().constData() );
    if( !ui->subjectNameEdit->text().isEmpty() )
      command << " --SubjectName-" << EncodedString( ui->subjectNameEdit->text().toLocal8Bit().constData() );
    if( !ui->subjectSessionEdit->text().isEmpty() )
      command << " --SubjectSession-" << EncodedString( ui->subjectSessionEdit->text().toLocal8Bit().constData() );
    if( !ExecuteCommand( command.str().c_str(), progdir.c_str() ) )
      return;
  }

  command.str( "" );
  if( ui->sigprocList->selectedItems().empty() )
  {
    ui->statusEdit->append( tr("No Signal Processing Module selected...") );
    ui->statusEdit->repaint();
  }
  else
  {
    QListWidgetItem* item = ui->sigprocList->selectedItems().first();
    ui->statusEdit->append( tr( "Launching %1..." ).arg( item->text() ) );
    ui->statusEdit->repaint();
    command << "\"" << progdir << item->text().toLocal8Bit().constData() << "\" --local";
    if( !ExecuteCommand( command.str().c_str(), progdir.c_str() ) )
      return;
  }

  command.str( "" );
  if( ui->applicationList->selectedItems().empty() )
  {
    ui->statusEdit->append( tr("No Application Module selected...") );
    ui->statusEdit->repaint();
  }
  else
  {
    QListWidgetItem* item = ui->applicationList->selectedItems().first();
    ui->statusEdit->append( tr( "Launching %1..." ).arg( item->text() ) );
    ui->statusEdit->repaint();
    command << "\"" << progdir << item->text().toLocal8Bit().constData() << "\" --local";
    if( !ExecuteCommand( command.str().c_str(), progdir.c_str() ) )
      return;
  }

  foreach( QListWidgetItem* item, ui->otherList->selectedItems() )
  {
    ui->statusEdit->append( tr( "Launching %1..." ).arg( item->text() ) );
    ui->statusEdit->repaint();
    command.str( "" );
    command << "\"" << progdir << item->text().toLocal8Bit().constData() << "\"";
    if( !ExecuteCommand( command.str().c_str(), progdir.c_str() ) )
      return;
  }

  ui->statusEdit->append( tr("Finished.") );
}

bool
MainWindow::ExecuteCommand( const char* inCommand, const char* inWorkingDir )
{
#if _WIN32
  STARTUPINFOA si;
  ::ZeroMemory( &si, sizeof( si ) );
  si.cb = sizeof( si );
  PROCESS_INFORMATION pi;
  // The command argument to CreateProcess() is read/write, so we need to make
  // a copy in writable memory.
  char* command = new char[::strlen( inCommand ) + 1];
  ::strcpy( command, inCommand );
  BOOL result = ::CreateProcessA(
      NULL, command, NULL, NULL, FALSE,
      CREATE_NEW_CONSOLE, NULL, inWorkingDir,
      &si, &pi
  );
  delete[] command;
  if( result )
  {
    ::WaitForInputIdle( pi.hProcess, 2000 );
    ::CloseHandle( pi.hProcess );
    ::CloseHandle( pi.hThread );
  }
#else // _WIN32
  QStringList arglist = ParseCommand( inCommand );
  QString program = arglist.first();
  arglist.pop_front();
  bool result = QProcess::startDetached( program, arglist, inWorkingDir );
#endif // _WIN32
  if( !result )
  {
    ui->statusEdit->append( SysError().Message() );
    ui->statusEdit->repaint();
  }
  return result;
}

QStringList
MainWindow::ParseCommand( const char* inCommand )
{
  QStringList result;
  const char* begin = inCommand;
  while( *begin != '\0' )
  {
    const char* end = begin;
    if( *end == '"' )
    {
      ++begin;
      ++end;
      while( *end != '"' && *end != '\0' )
        ++end;
    }
    else
    {
      while( *end != ' ' && *end != '\0' )
        ++end;
    }
    result.push_back( string( begin, end ).c_str() );
    begin = ++end;
    while( *begin == ' ' && *begin != '\0' )
      ++begin;
  }
  return result;
}
